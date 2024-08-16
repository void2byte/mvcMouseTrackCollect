#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>


class SettingsWidget : public QWidget {
    Q_OBJECT

public:

    explicit SettingsWidget(QWidget *parent = nullptr);

    QStandardItemModel *tableModel;

signals:

        void updateTable();

private:

    QTableView *trackTableView;

};

#endif // SETTINGSWIDGET_H

