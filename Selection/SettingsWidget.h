#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QTableView>


class SettingsWidget : public QWidget {
    Q_OBJECT

public:

    explicit SettingsWidget(QWidget *parent = nullptr);

    QTableView *trackTableView;

public slots:

    void scrollToBottom();

signals:

    void deleteRows(QModelIndexList selectedIndexes);
    void updateTable();

protected:

    void keyPressEvent(QKeyEvent *event) override;

};

#endif // SETTINGSWIDGET_H

