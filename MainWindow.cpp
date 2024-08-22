#include "mainwindow.h"
#include <QDebug>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow{parent} {

//     mouse_widget = new MouseWidget(mouseDock);
//     mouseDock = new QDockWidget(tr("make track"), this);
//     mouseDock->setWidget(mouse_widget);
//     addDockWidget(Qt::LeftDockWidgetArea, mouseDock);

//     settings_widget = new SettingsWidget(settingsDock);
//     settingsDock = new QDockWidget(tr("Settings"), this);
//     settingsDock->setWidget(settings_widget);
//     addDockWidget(Qt::RightDockWidgetArea, settingsDock);

// }


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent} {

    qDebug() << "Инициализация MainWindow...";

    mouse_widget = new MouseWidget(this);
    qDebug() << "mouse_widget инициализирован:" << (mouse_widget != nullptr);

    mouseDock = new QDockWidget(tr("make track"), this);
    qDebug() << "mouseDock инициализирован:" << (mouseDock != nullptr);

    mouseDock->setWidget(mouse_widget);
    addDockWidget(Qt::LeftDockWidgetArea, mouseDock);
    qDebug() << "mouse_widget добавлен в mouseDock и закреплен в LeftDockWidgetArea";

    settings_widget = new SettingsWidget(this);
    qDebug() << "settings_widget инициализирован:" << (settings_widget != nullptr);

    settingsDock = new QDockWidget(tr("Settings"), this);
    qDebug() << "settingsDock инициализирован:" << (settingsDock != nullptr);

    settingsDock->setWidget(settings_widget);
    addDockWidget(Qt::RightDockWidgetArea, settingsDock);
    qDebug() << "settings_widget добавлен в settingsDock и закреплен в RightDockWidgetArea";

    qDebug() << "MainWindow успешно инициализирован и готов к отображению.";
}

