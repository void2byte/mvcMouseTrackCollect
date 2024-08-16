#include "settingscontroller.h"
#include <QStandardItem>
#include <cmath>


SettingsController::SettingsController(SettingsWidget *widget)
    : QObject(widget), settings_widget(widget), model_settings(widget->tableModel) {

    fillModelFromData();
}


void SettingsController::fillModelFromData() {
    int d;
    model_settings->clear();
    model_settings->setHorizontalHeaderLabels(QStringList()
        << "Button Size"
        << "Straight Length/Time Ratio" << "Straight Length"
        << "Straight Length/Curve Length" << "Curve Length"
        << "Click Position" << "Window Position" << "Duration"
        << "point delay");

    for (const ClickData& click : data_manager.clicksData) {

        QList<QStandardItem*> rowItems;
        rowItems.append(new QStandardItem(
            QString::number(click.buttonRect.width())));

        double straight_length{};
        double curve_lenght{};
        qint64 duration = 1;

        if (!click.track.isEmpty() && !click.times.isEmpty()) {

            straight_length =
                calculateDistance(click.track.first(), click.track.last());
            curve_lenght = calculateCurveLength(click.track);

            duration = click.times.first() - click.times.last();

        }

        rowItems.append(
            new QStandardItem(QString::number(straight_length / duration)));

        rowItems.append(new QStandardItem(QString::number(straight_length)));

        rowItems.append(
            new QStandardItem(QString::number(straight_length / curve_lenght)));

        rowItems.append(new QStandardItem(QString::number(curve_lenght)));

        rowItems.append(new QStandardItem(
            QString("(%1, %2)").arg(click.clickPosition.x()).arg(click.clickPosition.y())));

        rowItems.append(new QStandardItem(
            QString("(%1, %2)").arg(click.windowPosition.x()).arg(click.windowPosition.y())));

        rowItems.append(new QStandardItem(QString::number(duration)));

        rowItems.append(
            new QStandardItem(QString::number(duration / double(click.times.size()))));

        model_settings->appendRow(rowItems);
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

