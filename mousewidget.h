#ifndef MOUSEWIDGET_H
#define MOUSEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>

#include "datamanager.h"


class MainController;

class MouseWidget : public QWidget {
    Q_OBJECT

public:

    MouseWidget(QWidget *parent = nullptr);
    ~MouseWidget();

    DataManager&     dataManager = DataManager::instance();
    QVector<ClickData>& clicksData   = dataManager.clicksData;


    void moveresizeTargetButton();
    QRect getButtonGeometry() const;
    QPoint getButtonPosition() const;


    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
signals:

    void moveResizeButton();
    void mouseMoveSig(QPoint p);

private:

    QPushButton *button;
    MainController *controller;
};

#endif // MOUSEWIDGET_H

