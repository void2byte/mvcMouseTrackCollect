#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QStandardItemModel>
#include <QStringList>
#include <QPoint>
#include <QVector>

#include "datamanager.h"
#include "settingswidget.h"

class SettingsController : public QObject {
    Q_OBJECT

public:
    explicit SettingsController(SettingsWidget* widget = nullptr);

    void fillModelFromData();
    void appendNewRow();

private:
    SettingsWidget* settings_widget;
    DataManager& data_manager = DataManager::instance();
    QStandardItemModel *model_settings;

    void initializeHeadersIfNeeded();
    QList<QStandardItem*> createRowItems();
    void updateLastRow(const QList<QStandardItem *> &rowItems);

    double calculateDistance(const QPoint& point1, const QPoint& point2) const;
    double calculateCurveLength(const QVector<QPoint>& track) const;

    ClickData& click = data_manager.clicksData.last();
    QList<QStandardItem*> rowItems = createRowItems();
};

#endif // SETTINGSCONTROLLER_H
