#ifndef MOUSEWIDGET_H
#define MOUSEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>

#include "DataManager.h"


class MainController;

class MouseWidget : public QWidget {
    Q_OBJECT

public:

    MouseWidget(QWidget *parent = nullptr);
    ~MouseWidget();

    DataManager&            dataManager = DataManager::instance();
    const ClickDataList&    clicksData   = dataManager.clicksData;

    QPushButton *button;
    void moveresizeTargetButton();
    QRect getButtonGeometry() const;
    QPoint getButtonPosition() const;
    QList<const ClickData*> selectedClickDataForDraw;


    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:

    void mouseMoveSig(QPoint p);

private:

    MainController *controller;
};

#endif // MOUSEWIDGET_H

