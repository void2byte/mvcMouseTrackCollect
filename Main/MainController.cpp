#include "Main/MainController.h"
#include <QDebug>
#include <QApplication>


MainController::MainController() {

    main_window         = new MainWindow();
    mouse_widget = main_window->mouse_widget;
    mouse_controller    = new MouseController(mouse_widget);

    settings_widget = main_window->settings_widget;
    settings_controller = new SettingsController(settings_widget);
    settings_widget->trackTableView->setModel(settings_controller->proxyModel);

    main_window->mouse_widget = mouse_widget;
    main_window->settings_widget = settings_widget;
    main_window->show();

    connect(mouse_controller, &MouseController::buttonClick,
            settings_controller, &SettingsController::insertRowTableModel);

    connect(mouse_controller, &MouseController::buttonClick,
            settings_widget->trackTableView, &QTableView::clearSelection);

    connect(mouse_controller, &MouseController::buttonClick,
            settings_widget, &SettingsWidget::scrollToBottom);


    connect(settings_widget->trackTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this]() {
                mouse_controller->takeIndexSelectedRows(
                    settings_widget->trackTableView->selectionModel()->selectedRows());
            });


    connect(settings_widget, &SettingsWidget::deleteRows,
            this, [&](QModelIndexList list) {
        settings_controller->removeSelectedRows(list);
        mouse_controller->takeIndexSelectedRows(
            settings_widget->trackTableView->selectionModel()->selectedRows());
    });

    settings_widget->trackTableView->resizeColumnsToContents();
    connect(settings_controller, &SettingsController::rowsInserted,
            settings_widget->trackTableView, &QTableView::resizeColumnsToContents);


}


