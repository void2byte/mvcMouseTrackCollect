#include "settingscontroller.h"
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

    rowItems = createRowItems();

    // Получаем последний клик и создаем для него элементы строки

    int lastRow = model_settings->rowCount();

    if (lastRow > 0) {
        // Обновляем последнюю строку
        updateLastRow(rowItems);
    } else {
        // Добавляем новую строку
        appendNewRow();
    }

    // Инициализируем заголовки, если это необходимо
    initializeHeadersIfNeeded();

    qDebug() << "Последняя строка в таблице обновлена.";
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

QList<QStandardItem*> SettingsController::createRowItems() {
    QList<QStandardItem*> rowItems;

    rowItems.append(new QStandardItem(QString::number(click.buttonRect.width())));

    double straight_length = 0.0;
    double curve_length = 0.0;
    qint64 duration = 1;

    if (!click.track.isEmpty() && !click.times.isEmpty()) {
        straight_length = calculateDistance(click.track.first(), click.track.last());
        curve_length = calculateCurveLength(click.track);
        duration = click.times.first() - click.times.last();
    }

    rowItems.append(new QStandardItem(QString::number(straight_length / duration)));
    rowItems.append(new QStandardItem(QString::number(straight_length)));
    rowItems.append(new QStandardItem(QString::number(straight_length / curve_length)));
    rowItems.append(new QStandardItem(QString::number(curve_length)));
    rowItems.append(new QStandardItem(QString("(%1, %2)").arg(click.clickPosition.x()).arg(click.clickPosition.y())));
    rowItems.append(new QStandardItem(QString("(%1, %2)").arg(click.windowPosition.x()).arg(click.windowPosition.y())));
    rowItems.append(new QStandardItem(QString::number(duration)));
    rowItems.append(new QStandardItem(QString::number(duration / double(click.times.size()))));

    return rowItems;
}

void SettingsController::updateLastRow(const QList<QStandardItem*>& rowItems) {
    int lastRow = model_settings->rowCount() - 1; // Получаем индекс последней строки
    for (int i = 0; i < rowItems.size(); ++i) {
        model_settings->setItem(lastRow, i, rowItems[i]);
    }
}

void SettingsController::appendNewRow() {

    model_settings->appendRow(createRowItems()); // Добавляем новую строку в модель
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

