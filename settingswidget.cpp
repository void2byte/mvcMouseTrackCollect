#include "settingswidget.h"
#include <QVBoxLayout>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent) {
    tableModel = new QStandardItemModel(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    trackTableView = new QTableView();
    trackTableView->setModel(tableModel);
    trackTableView->setSortingEnabled(true);
    layout->addWidget(trackTableView);

    setLayout(layout);

}



