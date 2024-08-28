#include "SettingsWidget.h"
#include <QVBoxLayout>
#include <QKeyEvent>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent) {
    tableModel = new QStandardItemModel(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    trackTableView = new QTableView();
#if FIRST
    trackTableView->setModel(tableModel);
#endif // FIRST
    trackTableView->setSortingEnabled(true);
    trackTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    trackTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(trackTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SettingsWidget::onSelectionChanged);

    layout->addWidget(trackTableView);

    setLayout(layout);

}

void SettingsWidget::onSelectionChanged() {
    // Получаем список выделенных строк
    QModelIndexList selectedIndexes = trackTableView->selectionModel()->selectedRows();

    // Формируем список ClickData*
    QList<ClickData*> selectedClickData;
    for (const QModelIndex &index : selectedIndexes) {
        int row = index.row();

        // Извлекаем указатель на ClickData из первого элемента строки
        QStandardItem *item = tableModel->item(row, 0);
        if (item) {
            selectedClickData.append(item->data().value<ClickData*>());
        }
    }

    // Отправляем сигнал с выбранными данными
    emit selectionChanged(selectedClickData);
}

void SettingsWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        QModelIndexList selectedIndexes = trackTableView->selectionModel()->selectedRows();
        if (!selectedIndexes.isEmpty()) {
            emit deleteRows(selectedIndexes);  // Отправляем сигнал для удаления строки
        }
    }
}
