#include "maincontroller.h"
#include <QDebug>
#include <QApplication>


MainController::MainController() {

    main_window         = new MainWindow();
    mouse_widget = main_window->mouse_widget;
    mouse_controller    = new MouseController(mouse_widget);
    settings_widget = main_window->settings_widget;
    settings_controller = new SettingsController(settings_widget);

    main_window->mouse_widget = mouse_widget;
    main_window->settings_widget = settings_widget;
    main_window->show();

    qDebug() << mouse_widget->parent();

    // connect(mouse_widget, &MouseWidget::mouseMoveSig, settings_controller,
    //         &SettingsController::fillModelFromData);

    connect(settings_widget, &SettingsWidget::selectionChanged, mouse_widget,
            &MouseWidget::drawSelectedTracks);

    connect(mouse_widget, &MouseWidget::moveResizeButton, settings_controller,
            &SettingsController::appendNewRow);

    connect(settings_widget, &SettingsWidget::deleteRows, settings_controller,
            &SettingsController::removeRows);

}


