#include "Main\MainWindow.h"
#include <QDebug>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent} {

    this->resize(1000, 600);

    mouse_widget = new MouseWidget(this);

    mouseDock = new QDockWidget(tr("make track"), this);

    mouseDock->setWidget(mouse_widget);
    addDockWidget(Qt::LeftDockWidgetArea, mouseDock);

    settings_widget = new SettingsWidget(this);

    settingsDock = new QDockWidget(tr("Settings"), this);

    settingsDock->setWidget(settings_widget);
    addDockWidget(Qt::RightDockWidgetArea, settingsDock);

    arduinoCommandWindow = new ArduinoCommandWindow(this);
    // Создаем меню
    fileMenu = menuBar()->addMenu(tr("&File"));

    // Создаем действие для пункта меню "Команды для Arduino"
    arduinoCommandAction = new QAction(tr("Open Arduino Command Window"), this);
    fileMenu->addAction(arduinoCommandAction);

    // Подключаем слот для открытия окна команд
    connect(arduinoCommandAction, &QAction::triggered, this,
            &MainWindow::openArduinoCommandWindow);
}



// Реализуем слот для открытия окна с командами для Arduino
void MainWindow::openArduinoCommandWindow() {
    if (!arduinoCommandWindow) {
        arduinoCommandWindow = new ArduinoCommandWindow(this); // Создаем окно, если еще не создано
        qDebug("arduinComandWindow create");
    }
    arduinoCommandWindow->show(); // Показываем окно
}
