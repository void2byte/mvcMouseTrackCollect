#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMenuBar>
#include <QAction>

#include "Mouse/MouseWidget.h"
#include "Selection/SettingsWidget.h"
#include "InputEmulator/ArduinoCommand.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    MouseWidget     *mouse_widget;
    QDockWidget     *settingsDock;

    SettingsWidget  *settings_widget;
    QDockWidget     *mouseDock;

private slots:
    void openArduinoCommandWindow(); // Слот для открытия окна с командами для Arduino

private:
    ArduinoCommandWindow *arduinoCommandWindow; // Указатель на новое окно
    QMenu *fileMenu;          // Пункт меню "Файл"
    QAction *arduinoCommandAction; // Действие для открытия окна команд
};


#endif // MAINWINDOW_H
