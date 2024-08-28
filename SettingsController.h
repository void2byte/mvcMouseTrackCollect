#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QPoint>
#include <QList>
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

public slots:
    void ResetTableModel();
private:
    DataManager& data_manager = DataManager::instance();
    QStringList headers = {
        "Button Size",
        "Straight Length/Time Ratio",
        "Straight Length",
        "Straight Length/Curve Length",
        "Curve Length",
        "Click Position",
        "Window Position",
        "Duration",
        "Point Delay"
    };

};


#endif // SETTINGSCONTROLLER_H
