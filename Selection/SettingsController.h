#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QPoint>
#include <QList>
#include <QSortFilterProxyModel>
#include "datamanager.h"


class SettingsController : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit SettingsController(QObject *parent = nullptr);

    // Переопределяем виртуальные методы QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void removeSelectedRows(QModelIndexList index); // Метод для удаления выделенных строк

    QSortFilterProxyModel* proxyModel;
public slots:
    void insertRowTableModel();
private:
    DataManager& data_manager = DataManager::instance();
    ClickDataList& clicksData = data_manager.clicksData;
    QStringList headers = {
        "Index",
        "Button Size",
        "Straight-Line\nPixels per Second",
        "Straight Length",
        "Straight Length/\nCurve Length",
        "Curve Length",
        "Click Position\nin Button",
        "Click Position\nin Window",
        "Duration",
        "Average Point\nDelay"
    };

};


#endif // SETTINGSCONTROLLER_H
