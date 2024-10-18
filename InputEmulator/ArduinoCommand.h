#ifndef ARDUINOCOMMAND_H
#define ARDUINOCOMMAND_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDataStream>
#include <QPoint>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QThread>
#include <QTimer>
#include <QCursor>
#include <QScreen>
#include <cmath>

#include "DataManager.h"

// Определение флагов для команд
#define FLAG_MOVE 0
#define FLAG_CLICK_LEFT 1
#define FLAG_CLICK_RIGHT 2
#define FLAG_SCROLL 3
#define FLAG_KEYPRESS 4
#define FLAG_KEYRELEASE 5
#define FLAG_KEYCOMBO 6
#define FLAG_MODIFIER 7

struct CommandList;

#pragma pack(push, 1)
struct CommandData {
    uint8_t flag;      // Флаг команды
    int16_t x;          // Данные для перемещения или скролла
    int16_t y;          // Дополнительные данные
    uint8_t key;       // Код клавиши
    uint8_t modifier;  // Модификатор клавиши (Ctrl, Shift, Alt)
};
#pragma pack(pop)

struct CommandList
{
    uint8_t flag;      // Флаг команды
    int16_t x;          // Данные для перемещения или скролла
    int16_t y;          // Дополнительные данные
    uint8_t key;       // Код клавиши
    uint8_t modifier;  // Модификатор клавиши (Ctrl, Shift, Alt)

    qint64 delayMs;

};


// Структура для передачи данных
#pragma pack(push, 1)  // Отключаем выравнивание для упрощения передачи
struct MouseMovement {
    int16_t x;         // Смещение по оси X
    int16_t y;         // Смещение по оси Y
    int16_t delayTime; // Задержка в миллисекундах
};
#pragma pack(pop)

class PortSender;


class ArduinoCommandWindow : public QWidget {
    Q_OBJECT

public:
    explicit ArduinoCommandWindow(QWidget *parent = nullptr);
    ~ArduinoCommandWindow();
private:
    const DataManager &dataManager = DataManager::instance();
    PortSender *sender = nullptr;
    QThread* threadSender;
};



class PortSender : public QObject {
    Q_OBJECT

    QVector<CommandList>* _cmdlst = nullptr;
    QVector<CommandList>::iterator _lstit;

    double coeff_x_mov = 1;
    double coeff_y_mov = 1;

    QTimer timer;

public:
    PortSender();

    ~PortSender();

    /**
    * @brief Вычисляет коэффициент перемещения курсора (разница в движении с генерируемым платой перемещением)
    * и отправляет команду на перемещение курсора.
    *
    * Эта функция:
    * 1. Сохраняет текущее положение курсора.
    * 2. Перемещает курсор в точку (0, 0) и отправляет команду для перемещения на определённое расстояние.
    * 3. Рассчитывает коэффициент перемещения курсора, основываясь на конечном положении курсора.
    * 4. Возвращает курсор на его исходную позицию.
    *
    * Примечание: Использует задержки для того, чтобы обеспечить завершение перемещения курсора перед выполнением вычислений.
    */
    void calcCoefficientDifferenceCursormovement();
    /**
     * @brief Умножает массив точек QPoint на скаляр с минимальным влиянием округления на результирующий вектор.
    *
     * @param arr Входной массив точек QPoint.
    * @param scalarX Скаляр для умножения по x, scalarY по y.
    */
    void multiplyWithMinimalRounding(QVector<CommandList>* arr,
                                     double scalarX, double scalarY);

    void sendCommand(uint8_t flag, int16_t x = 0, int16_t y = 0,
                     uint8_t key = 0, uint8_t modifier = 0);

    void sendAndStartNewTimer();


    void sendNew(QVector<CommandList>* list);

    void sendLeftClick();

    void sendRightClick();

    void sendScroll(int16_t scrollAmount);

    void sendKeyPress(uint8_t key);

    void sendKeyRelease(uint8_t key);

    void sendKeyCombo(uint8_t key, uint8_t modifier);

    void sendModifier(uint8_t modifier, bool pressed);

public slots:
    // Обработчик данных, когда они готовы для чтения. для тестов
    void handleReadyRead();

private:

    QSerialPort *serialPort = nullptr;

};



#endif // ARDUINOCOMMAND_H
