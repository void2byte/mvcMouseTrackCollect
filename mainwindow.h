#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>

#include "mousewidget.h"
#include "settingswidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    MouseWidget     *mouse_widget;
    QDockWidget     *settingsDock;

    SettingsWidget  *settings_widget;
    QDockWidget     *mouseDock;
};


#endif // MAINWINDOW_H
