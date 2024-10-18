#include "Selection/SettingsController.h"
#include <QStandardItem>
#include <cmath>


SettingsController::SettingsController(QObject *parent)
    : QAbstractTableModel(parent) {
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(this);
}

int SettingsController::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return clicksData.size(); // Количество строк соответствует количеству ClickData
}


int SettingsController::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return headers.size(); // Количество столбцов
}


QVariant SettingsController::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    const ClickData& click = data_manager.clicksData[index.row()];

    switch (index.column()) {

    // "Index"
    case 0:
        return click.getId();

    // "Button Size"
    case 1:
        return click.buttonRect.width();

    // "Straight-Line Pixels per Second"
    case 2: {
        if(click.track.empty())
            return 0;
        double straight_length = click.calculateDistance();
        double duration = click.times.last() - click.times.first();
        return duration > 0 ? straight_length / (duration / 1000): 0.0;
    }

    // "Straight Length"
    case 3:
        if(click.track.empty())
            return 0;
        return click.calculateDistance();

    // "Straight Length/Curve Length"
    case 4: {
        if(click.track.empty())
            return 0;
        double straight_length = click.calculateDistance();
        double curve_length = click.calculateCurveLength();
        return curve_length > 0 ? straight_length / curve_length : 0.0;
    }

    // "Curve Length"
    case 5:
        return click.calculateCurveLength();

    // "Click Position in Button"
    case 6:
        return QString("(%1, %2)").arg(click.clickPositionInButton.x()).arg(click.clickPositionInButton.y());

    // "Click Position in Window"
    case 7:
        return QString("(%1, %2)").arg(click.globalClickPosition.x()).arg(click.globalClickPosition.y());

    // "Duration"
    case 8:
        if(click.times.empty())
            return 0;
        return  click.times.last() - click.times.first();

    // "Average Point Delay"
    case 9:
        if(click.times.empty())
            return 0;
        return click.times.isEmpty() ? 0 : (click.times.last() - click.times.first())
                                               / double(click.times.size());
    }

    return QVariant();
}


QVariant SettingsController::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || section < 0 || section >= headers.size())
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


void SettingsController::insertRowTableModel() {
    int newRowPos = rowCount();

    beginInsertRows(QModelIndex(), newRowPos, newRowPos);
    endInsertRows();

};


void SettingsController::removeSelectedRows(QModelIndexList indexList) {
    if (indexList.empty()) {
        return;
    }

    // Сортируем индексы в обратном порядке для корректного удаления
    std::sort(indexList.begin(), indexList.end(), [](const QModelIndex &a, const QModelIndex &b) {
        return a.row() > b.row();
    });

    for (const QModelIndex &index : indexList) {
        bool ok = false;
        qsizetype clickindex = index.siblingAtColumn(0).data().toLongLong(&ok);
        if (!ok) {
            qDebug() << "SettingsController::removeSelectedRows fail take index from row:" << index.row();
            continue;
        }

        auto it = clicksData.constBegin();
        while (it != clicksData.end()) {
            if (it->getId() == clickindex) {
                beginRemoveRows(QModelIndex(), index.row(), index.row());
                qDebug() << "SettingsController::removeSelectedRows delete element, index:" << it->getId();
                it = clicksData.erase(it);
                if(clicksData.empty()) {clicksData.append(ClickData());}
                endRemoveRows();
                break;
            } else {
                ++it;
            }
        }
    }
}



