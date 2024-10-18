#include "Selection/SettingsWidget.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QStandardItem>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    trackTableView = new QTableView();
    trackTableView->setSortingEnabled(true);
    trackTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    trackTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    layout->addWidget(trackTableView);

    setLayout(layout);

}



void SettingsWidget::scrollToBottom() {
    if (trackTableView->model()->rowCount() > 0) {
        trackTableView->scrollToBottom();
    }
}


void SettingsWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        QModelIndexList selectedIndexes = trackTableView->selectionModel()->selectedRows();
        if (!selectedIndexes.isEmpty()) {
            emit deleteRows(selectedIndexes);  // Отправляем сигнал для удаления строки
        }
    }
}
