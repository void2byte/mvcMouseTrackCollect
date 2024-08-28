#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H
#include <QMouseEvent>
#include <QKeyEvent>
#include <QElapsedTimer>
#include <QPainter>
#include <QObject>

#include "MainWindow.h"
#include "DataManager.h"
#include "MouseWidget.h"
#include "MouseController.h"
#include "SettingsWidget.h"
#include "SettingsController.h"



class MainController : public QObject {

    Q_OBJECT

public:
    MainController();

    MainWindow*         main_window;
    MouseWidget*        mouse_widget;
    MouseController*    mouse_controller;
    SettingsWidget*     settings_widget;
    SettingsController* settings_controller;

};

#endif // MAINCONTROLLER_H
