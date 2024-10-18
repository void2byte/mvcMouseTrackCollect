#include "ArduinoCommand.h"

ArduinoCommandWindow::ArduinoCommandWindow(QWidget *parent)
    : QWidget(parent, Qt::Window) {
    setWindowTitle(tr("Arduino"));

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel(tr("List of Commands for Arduino"), this);
    layout->addWidget(label);

    // Добавляем кнопки для команд (пример)
    QPushButton *command1Button = new QPushButton(tr("mov"), this);
    QPushButton *command2Button = new QPushButton(tr("scalar"), this);

    layout->addWidget(command1Button);
    layout->addWidget(command2Button);

    // QThread* threadSender = new QThread;
    // sender->moveToThread(threadSender);
    // threadSender->start();

    // загрузка терков из файла

    sender = new PortSender();

    // Связываем сигналы кнопок с действиями (например, отправка команды на Arduino)
    connect(command1Button, &QPushButton::clicked, [&]() {
        if (dataManager.clicksData.size() < 1) {
            return;
        }

        const ClickData moves = dataManager.clicksData.last();
        QVector<CommandList> *cmdlst = new QVector<CommandList>();

        for (int i = 1; i < moves.track.size(); ++i) {
            int16_t x = static_cast<int16_t>(
                    moves.track.at(i).x() - moves.track.at(i - 1).x());
            int16_t y = static_cast<int16_t>(
                    moves.track.at(i).y() - moves.track.at(i - 1).y());
            qint64 delayMs = moves.times.at(i) - moves.times.at(i - 1);

             qDebug() << "{" << x << "," << y << "},";

            cmdlst->append({FLAG_MOVE, x, y, 0, 0, delayMs});
        }

        if(sender == nullptr)
            sender = new PortSender();

        sender->sendNew(cmdlst);
    });

    connect(command2Button, &QPushButton::clicked,
            sender, &PortSender::calcCoefficientDifferenceCursormovement);


    setLayout(layout);
}

ArduinoCommandWindow::~ArduinoCommandWindow()
{
    threadSender->quit();
    threadSender->deleteLater();
}




PortSender::PortSender()
    : serialPort(new QSerialPort()) {

    serialPort->setPortName("COM5");  // Укажите ваш реальный COM-порт
    serialPort->setBaudRate(QSerialPort::Baud9600);


    // Пытаемся открыть порт
    if (!serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Не удалось открыть последовательный порт:" << serialPort->errorString();
        return;
    }

    calcCoefficientDifferenceCursormovement();

    connect(&timer, &QTimer::timeout, this, &PortSender::sendAndStartNewTimer);

    //Подключаем сигнал готовности чтения к слоту для обработки данных. для тестов
    connect(serialPort, &QSerialPort::readyRead, this,
            &PortSender::handleReadyRead);

}

PortSender::~PortSender() {
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
    }
    delete serialPort;
    serialPort = nullptr;
}


void PortSender::calcCoefficientDifferenceCursormovement()
{
    QPoint originalPosition = QCursor::pos();
    QThread::msleep(500);
    QCursor::setPos(0, 0);

    QThread::msleep(500);


    for(int i = 0; i < 200; ++i) {
        this->sendCommand(FLAG_MOVE, 2);
        QThread::msleep(8);
    }

    QThread::msleep(50);

    int newXPosition = QCursor::pos().x();
    if (newXPosition != 0) {
        coeff_x_mov = double(400) / newXPosition;
    } else {
        coeff_x_mov = 1;
    }

    for(int i = 0; i < 200; ++i) {
        this->sendCommand(FLAG_MOVE, 0, 2);
        QThread::msleep(8);
    }

    QThread::msleep(50);

    int newYPosition = QCursor::pos().y();
    if (newYPosition != 0) {
        coeff_y_mov = double(400) / newYPosition;
    } else {
        coeff_y_mov = 1;
    }

    qDebug() << "calcCoefficientDifferenceCursormovement x: " << coeff_x_mov
             << " y: " << coeff_y_mov << "\n"
             << "arduno mov from 0 to new x pos: "<< newXPosition << "\n"
             << "arduno mov from 0 to new y pos: "<< newYPosition;

    QCursor::setPos(originalPosition);

    QThread::msleep(20);
}


void PortSender::multiplyWithMinimalRounding(QVector<CommandList>* arr,
                                             double scalarX, double scalarY) {

    double errorX = 0.0;  // Переменная для накопления ошибки округления по X
    double errorY = 0.0;  // Переменная для накопления ошибки округления по Y
    //для теста
    QVector<QPoint> before;
    QVector<QPoint> after;

    for (auto& cmd : *arr) {
        // Для координаты X
        before.append(QPoint(cmd.x, cmd.y));
        double productX = cmd.x * scalarX + errorX;
        double productY = cmd.y * scalarY + errorY;

        cmd.x = static_cast<int16_t>(std::round(productX));
        cmd.y = static_cast<int16_t>(std::round(productY));
        after.append(QPoint(cmd.x, cmd.y));
        errorX = productX - cmd.x;
        errorY = productY - cmd.y;
    }

    // проверка общей длинны трека после перемножения

    double test = calculateCurveLength(before);

    double result = calculateCurveLength(after);

    qDebug() << "result " << result << "before " << test << "result / before "
             << double(result / test) << "скаляр по x: " << scalarX << "y" << scalarY;
}


void PortSender::sendCommand(uint8_t flag, int16_t x, int16_t y, uint8_t key, uint8_t modifier) {
    if (!serialPort->isOpen()) {
        qDebug() << "Ошибка: последовательный порт закрыт!";
        return;
    }

    CommandData command = { flag, x, y, key, modifier };
    QByteArray data(reinterpret_cast<const char*>(&command), sizeof(CommandData));

    // Попытка отправить данные в порт
    qint64 bytesWritten = serialPort->write(data);
    if (bytesWritten == -1) {
        qDebug() << "Ошибка записи в последовательный порт:" << serialPort->errorString();
        return;
    }

    // Проверка, что данные записаны
    if (!serialPort->waitForBytesWritten(1000)) {
        qDebug() << "Ошибка: данные не были записаны в порт за 1 секунду.";
    } else {
        //qDebug() << "X:" << x << "Y:" << y;
    }
}


void PortSender::sendAndStartNewTimer() {

    if(_lstit < _cmdlst->end()) {

        this->sendCommand(FLAG_MOVE, _lstit->x, _lstit->y,
                          _lstit->key, _lstit->modifier);

        //qDebug() << "X:" << _lstit->x << "Y:" << _lstit->y << "T" << _lstit->delayMs;
        timer.start(_lstit->delayMs);
        _lstit++;
    } else {
        timer.stop();
    }

}


void PortSender::sendNew(QVector<CommandList>* list) {
    if(list->empty()) {
        qDebug() << "в PortSender::sendNew передан путой список";
        return;
    }

    multiplyWithMinimalRounding(list, coeff_x_mov, coeff_y_mov);

    _cmdlst = list; _lstit = _cmdlst->begin();
    timer.start(10);
}


void PortSender::sendLeftClick() {
    sendCommand(FLAG_CLICK_LEFT);
}

void PortSender::sendRightClick() {
    sendCommand(FLAG_CLICK_RIGHT);
}

void PortSender::sendScroll(int16_t scrollAmount) {
    sendCommand(FLAG_SCROLL, 0, scrollAmount);
}

void PortSender::sendKeyPress(uint8_t key) {
    sendCommand(FLAG_KEYPRESS, 0, 0, key);
}

void PortSender::sendKeyRelease(uint8_t key) {
    sendCommand(FLAG_KEYRELEASE, 0, 0, key);
}

void PortSender::sendKeyCombo(uint8_t key, uint8_t modifier) {
    sendCommand(FLAG_KEYCOMBO, 0, 0, key, modifier);
}

void PortSender::sendModifier(uint8_t modifier, bool pressed) {
    uint8_t flag = pressed ? FLAG_MODIFIER : FLAG_KEYRELEASE;
    sendCommand(flag, 0, 0, 0, modifier);
}



void PortSender::handleReadyRead() {
    while (serialPort->canReadLine()) {
        QByteArray data = serialPort->readLine();  // Читаем одну строку
        if (!data.isEmpty()) {
            qDebug() << data;
        }
    }
}



