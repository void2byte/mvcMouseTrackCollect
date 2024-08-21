#include "mousewidget.h"

#include <random>
#include <QDockWidget>



MouseWidget::MouseWidget(QWidget *parent)
    : QWidget(parent) {

    this->setMinimumSize(200, 200);

    this->installEventFilter(this);// установка фильтра событий, обработка в maincontroller
    setMouseTracking(true); // включение захвата движенией мыши

    // Настройка окна
    this->setWindowTitle("Полноэкранное окно с кнопкой");

    // Создание и настройка кнопки
    button = new QPushButton("click", this);
    button->setStyleSheet(
        "QPushButton {background-color: red; color: white;}"
        "QPushButton:hover {background-color: blue;}"
        "QPushButton:pressed {background-color: green;}"
        );

    connect(button, &QPushButton::clicked, this, &MouseWidget::moveresizeTargetButton);

    //connect(this, &MouseWidget::moveResiseButton, controller, &MainController::handleButtonClicked);

    moveresizeTargetButton(); // Перемещение кнопки в случайное место

    button->setVisible(true);
    this->update();
}

MouseWidget::~MouseWidget() {
    delete button;
}

// Функция для генерации случайного числа в диапазоне [min, max]
int randomInRange(int min, int max) {
    // Статические объекты инициализируются только один раз
    static std::random_device rd; // Генератор случайных чисел (может использовать аппаратные источники энтропии)
    static std::mt19937 gen(rd()); // Генератор на основе алгоритма Мерсенна Твистера
    std::uniform_int_distribution<> distrib(min, max); // Равномерное распределение

    return distrib(gen);
}

void MouseWidget::moveresizeTargetButton() {

    emit moveResizeButton();

    // Получение размеров окна
    int maxX = this->width() - button->width();
    int maxY = this->height() - button->height();

    // Генерация случайных размеров кнопки 10,20,30pxl
    int a = randomInRange(1, 3) * 10;
    button->resize(a, a);

    // Генерация случайных координат в пределах окна
    static double steps_size = 1;

    if(steps_size < 0.1)
        steps_size = 1;
    else
        steps_size -= 0.01;

    int randomX = randomInRange(10, maxX - 10) * steps_size;
    int randomY = randomInRange(10, maxY - 10) * steps_size;

    // Перемещение кнопки в случайные координаты
    button->move(randomX, randomY);

}

QRect MouseWidget::getButtonGeometry() const {
    return button->geometry();
}

QPoint MouseWidget::getButtonPosition() const {
    return button->pos();
}





bool MouseWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //handleKeyPressEvent(keyEvent);
        return true;
    } else {
        // стандартная обработка остальных событий
        return QWidget::eventFilter(obj, event);
    }
}

void MouseWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // Создание структуры для хранения настроек пера и соответствующих точек
    struct PenPoints {
        QPen pen;
        QVector<QPoint> points;
    };

    QVector<PenPoints> penPoints;

    if(selectedClickDataForDraw.empty()) {
        if(clicksData.size() > 0)
            penPoints.append({{Qt::black, 3}, clicksData[clicksData.size()-1].track});
        if(clicksData.size() > 1)
            penPoints.append({{Qt::darkGray, 2}, clicksData[clicksData.size()-2].track});
        if(clicksData.size() > 2)
            penPoints.append({{Qt::gray, 1}, clicksData[clicksData.size()-3].track});
    } else {
        for(ClickData* row : selectedClickDataForDraw) {
            //penPoints.append({{Qt::black, 3}, row->track});
        }
    }

    for (const auto& pp : penPoints) {
        painter.setPen(pp.pen);
        if (!pp.points.isEmpty()) {
            painter.drawPoints(pp.points.constData(), pp.points.size());
        }
    }
}

void MouseWidget::mouseMoveEvent(QMouseEvent *event) {
    QPoint p = event->pos();
    update();
    QWidget::mouseMoveEvent(event); // Вызов базового обработчика событий

    emit mouseMoveSig(p);
}

void MouseWidget::drawSelectedTracks(QList<ClickData*> selectedClickData)
{
    selectedClickDataForDraw = selectedClickData;
}



