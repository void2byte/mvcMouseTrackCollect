#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H
#include <QMouseEvent>
#include <QKeyEvent>
#include <QElapsedTimer>
#include <QPainter>
#include <QObject>

#include "Main/MainWindow.h"
#include "DataManager.h"
#include "Mouse/MouseWidget.h"
#include "Mouse/MouseController.h"
#include "Selection/SettingsWidget.h"
#include "Selection/SettingsController.h"
//#include "SettingsModel.h"



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
