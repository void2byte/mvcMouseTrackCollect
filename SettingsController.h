#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QStandardItemModel>
#include <QStringList>
#include <QPoint>
#include <QVector>

#include "DataManager.h"
#include "SettingsWidget.h"

class SettingsController : public QObject {
    Q_OBJECT

public:
    explicit SettingsController(SettingsWidget* widget = nullptr);

    void fillModelFromData();
public slots:
    void appendNewRow() {fillModelFromData();
        //model_settings->appendRow(createRowItems(data_manager.clicksData.last()));
    };

    void removeRows(QModelIndexList index);

private:
    SettingsWidget* settings_widget;
    DataManager& data_manager = DataManager::instance();
    QStandardItemModel *model_settings;

    void initializeHeadersIfNeeded();
    QList<QStandardItem*> createRowItems(const ClickData& click);
    QList<QStandardItem*> updateRowItems(const ClickData &click);
    void updateLastRow(const QList<QStandardItem *> &rowItems);

    double calculateDistance(const QPoint& point1, const QPoint& point2) const;
    double calculateCurveLength(const QVector<QPoint>& track) const;

};

#endif // SETTINGSCONTROLLER_H
