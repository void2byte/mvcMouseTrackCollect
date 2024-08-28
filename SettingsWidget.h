#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>

#include "DataManager.h"


class SettingsWidget : public QWidget {
    Q_OBJECT

public:

    explicit SettingsWidget(QWidget *parent = nullptr);

    QStandardItemModel *tableModel;

signals:

    void deleteRows(QModelIndexList selectedIndexes);
    void updateTable();
    void selectionChanged(QList<ClickData*> selectedRows);

private slots:

    void onSelectionChanged();

protected:

    void keyPressEvent(QKeyEvent *event) override;


    QTableView *trackTableView;

};

#endif // SETTINGSWIDGET_H

