#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H
#include <QMouseEvent>
#include <QKeyEvent>
#include <QElapsedTimer>
#include <QPainter>
#include <QObject>

#include "mainwindow.h"
#include "datamanager.h"
#include "mousewidget.h"
#include "mousecontroller.h"
#include "settingswidget.h"
#include "settingscontroller.h"



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
