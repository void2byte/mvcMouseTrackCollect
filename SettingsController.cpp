#include "SettingsController.h"
#include <QStandardItem>
#include <cmath>


SettingsController::SettingsController(QObject *parent)
    : QAbstractTableModel(parent) {}

int SettingsController::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return data_manager.clicksData.size(); // Количество строк соответствует количеству ClickData
}


int SettingsController::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return headers.size(); // Количество столбцов
}


QVariant SettingsController::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    const ClickData &click = data_manager.clicksData[index.row()];

    switch (index.column()) {
    case 0:
        return click.buttonRect.width();
    case 1: {
        if(click.track.empty())
            return 0;
        double straight_length = click.calculateDistance();
        qint64 duration = click.times.first() - click.times.last();
        return duration > 0 ? straight_length / duration : 0.0;
    }
    case 2:
        if(click.track.empty())
            return 0;
        return click.calculateDistance();
    case 3: {
        if(click.track.empty())
            return 0;
        double straight_length = click.calculateDistance();
        double curve_length = click.calculateCurveLength();
        return curve_length > 0 ? straight_length / curve_length : 0.0;
    }
    case 4:
        return click.calculateCurveLength();
    case 5:
        return QString("(%1, %2)").arg(click.clickPosition.x()).arg(click.clickPosition.y());
    case 6:
        return QString("(%1, %2)").arg(click.windowPosition.x()).arg(click.windowPosition.y());
    case 7:
        if(click.times.empty())
            return 0;
        return click.times.first() - click.times.last();
    case 8:
        if(click.times.empty())
            return 0;
        return click.times.isEmpty() ? 0 : (click.times.first() - click.times.last()) / double(click.times.size());
    }
    return QVariant();
}


QVariant SettingsController::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return headers[section]; // Заголовки столбцов
    } else {
        return section + 1; // Нумерация строк
    }
}


bool SettingsController::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole || !index.isValid())
        return false;

    // Реализация возможности редактирования данных, если необходимо

    emit dataChanged(index, index, {role});
    return true;
}


Qt::ItemFlags SettingsController::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}


void SettingsController::ResetTableModel() {
    beginResetModel();
    endResetModel();
};

void SettingsController::removeSelectedRows(QModelIndexList indexList) {
    if (indexList.empty()) {
        return;
    }

    // Сортируем индексы в обратном порядке, чтобы корректно удалять строки
    std::sort(indexList.begin(), indexList.end(), [](const QModelIndex &a, const QModelIndex &b) {
        return a.row() > b.row();
    });

    beginResetModel(); // Уведомляем представление о начале изменения данных
    for (const QModelIndex &index : indexList) {
        int row = index.row();
        if (row >= 0 && row < data_manager.clicksData.size()) {
            qDebug()<<data_manager.clicksData.at(row).getId();
            data_manager.clicksData.removeAt(row);
            if(data_manager.clicksData.empty())
                data_manager.clicksData.append(ClickData());
        }
    }
    endResetModel(); // Уведомляем представление о завершении изменения данных
}


