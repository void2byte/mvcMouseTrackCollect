#include "SettingsController.h"
#include <QStandardItem>
#include <cmath>

SettingsController::SettingsController(SettingsWidget *widget)
    : QObject(widget), settings_widget(widget), model_settings(widget->tableModel) {

    fillModelFromData();
}

void SettingsController::fillModelFromData() {
    if (data_manager.clicksData.isEmpty()) {
        qDebug() << "Нет данных для обновления.";
        return;
    }

    // Получаем последний клик
    const ClickData& _click = data_manager.clicksData.last();

    if (model_settings->rowCount() > 0) {
        // Обновляем последнюю строку
        model_settings->appendRow(
            createRowItems(( data_manager.clicksData.at(
                data_manager.clicksData.size()-2))));
    } else {
        // Добавляем новую строку
        model_settings->appendRow(createRowItems(_click));
    }

    // Инициализируем заголовки, если это необходимо
    initializeHeadersIfNeeded();

    qDebug() << "Последняя строка в таблице обновлена.";
}

void SettingsController::removeRows(QModelIndexList index) {
    if (index.empty()) {
        return;
    }

    // Сортируем индексы в обратном порядке, чтобы корректно удалять строки
    std::sort(index.begin(), index.end(), [](const QModelIndex &a, const QModelIndex &b) {
        return a.row() > b.row();
    });

    // Итерируем по индексам и удаляем строки и соответствующие элементы ClickData
    for (const QModelIndex &modelIndex : index) {
        int row = modelIndex.row();

        // Извлекаем указатель на ClickData из первого элемента строки
        QStandardItem *item = model_settings->item(row, 0);
        if (item) {
            ClickData *click = item->data().value<ClickData*>();

            // Удаляем элемент из clicksData
            auto it = std::find_if(data_manager.clicksData.begin(), data_manager.clicksData.end(),
                                   [click](const ClickData &c) { return &c == click; });

            if (it != data_manager.clicksData.end()) {
                qDebug() << "невалидный итератор при удалении элемента из datamanger.clicksData";
                data_manager.clicksData.erase(it);
            }

            // Удаляем строку из модели
            model_settings->removeRow(row);
        }
    }
}


void SettingsController::initializeHeadersIfNeeded() {
    if (model_settings->rowCount() == 1) { // Заголовки инициализируются один раз, когда добавляется первая строка
        model_settings->setHorizontalHeaderLabels(QStringList()
                                                  << "Button Size"
                                                  << "Straight Length/Time Ratio" << "Straight Length"
                                                  << "Straight Length/Curve Length" << "Curve Length"
                                                  << "Click Position" << "Window Position" << "Duration"
                                                  << "Point Delay");
    }
}

QList<QStandardItem*> SettingsController::createRowItems(const ClickData& click) {
    QList<QStandardItem*> _rowItems;
    
    _rowItems.append(new QStandardItem(QString::number(click.buttonRect.width())));
    
    _rowItems.first()->setData(QVariant::fromValue(&click), Qt::UserRole + 1);
    //указатель на ClickData для связывания данных с отображением

    double straight_length = 0.0;
    double curve_length = 0.0;
    qint64 duration = 1;

    if (!click.track.isEmpty() && !click.times.isEmpty()) {
        straight_length = calculateDistance(click.track.first(), click.track.last());
        curve_length = calculateCurveLength(click.track);
        duration = click.times.first() - click.times.last();
    }

    _rowItems.append(new QStandardItem(QString::number(straight_length / duration)));
    _rowItems.last()->setData(straight_length / duration, Qt::EditRole);

    _rowItems.append(new QStandardItem(QString::number(straight_length)));
    _rowItems.last()->setData(straight_length, Qt::EditRole);

    _rowItems.append(new QStandardItem(QString::number(straight_length / curve_length)));
    _rowItems.last()->setData(straight_length / curve_length, Qt::EditRole);

    _rowItems.append(new QStandardItem(QString::number(curve_length)));
    _rowItems.last()->setData(curve_length, Qt::EditRole);

    _rowItems.append(new QStandardItem(QString("(%1, %2)").arg(click.clickPosition.x()).arg(click.clickPosition.y())));
    _rowItems.append(new QStandardItem(QString("(%1, %2)").arg(click.windowPosition.x()).arg(click.windowPosition.y())));

    _rowItems.append(new QStandardItem(QString::number(duration)));
    _rowItems.last()->setData(duration, Qt::EditRole);

    _rowItems.append(new QStandardItem(QString::number(duration / double(click.times.size()))));
    _rowItems.last()->setData(duration / double(click.times.size()), Qt::EditRole);

    return _rowItems;
}

// нужно решить проблему падения производительности при обновление данных в строке при каждом сигнале о перемещении мыши
QList<QStandardItem*> SettingsController::updateRowItems(const ClickData& click) {

    QList<QStandardItem*> _rowItems = model_settings->takeRow(
        model_settings->rowCount() - 1);

    _rowItems[0]->setText(QString::number(click.buttonRect.width()));

    double straight_length = 0.0;
    double curve_length = 0.0;
    qint64 duration = 1;

    if (!click.track.isEmpty() && !click.times.isEmpty()) {
         straight_length = calculateDistance(click.track.first(), click.track.last());
         curve_length = calculateCurveLength(click.track);
         duration = click.times.first() - click.times.last();
    }


    _rowItems[1]->setText(QString::number(straight_length / duration));
    _rowItems[2]->setText(QString::number(straight_length));
    _rowItems[3]->setText(QString::number(straight_length / curve_length));
    _rowItems[4]->setText(QString::number(curve_length));
    _rowItems[5]->setText(QString("(%1, %2)").arg(click.clickPosition.x()).arg(click.clickPosition.y()));
    _rowItems[6]->setText(QString("(%1, %2)").arg(click.windowPosition.x()).arg(click.windowPosition.y()));
    _rowItems[7]->setText(QString::number(duration));
    _rowItems[8]->setText(QString::number(duration / double(click.times.size())));

    return _rowItems;
}

void SettingsController::updateLastRow(const QList<QStandardItem*>& rowItems) {
    int lastRow = model_settings->rowCount() - 1; // Получаем индекс последней строки
    for (int i = 0; i < rowItems.size(); ++i) {
        model_settings->setItem(lastRow, i, rowItems[i]);
    }
}

double SettingsController::calculateDistance(const QPoint& point1, const QPoint& point2) const {
    int dx = point2.x() - point1.x();
    int dy = point1.y() - point2.y();
    return std::sqrt(dx * dx + dy * dy);
}

double SettingsController::calculateCurveLength(const QVector<QPoint>& track) const {
    double length = 0.0;
    for (int i = 1; i < track.size(); ++i) {
        length += calculateDistance(track[i], track[i - 1]);
    }
    return length;
}
