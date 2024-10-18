#ifndef MOUSECONTROLLER_H
#define MOUSECONTROLLER_H

#include <QObject>
#include <Mouse/MouseWidget.h>
#include <QElapsedTimer>
#include <QVector>
#include <QApplication>
#include <QKeyEvent>
#include <QModelIndexList>

#include <DataManager.h>
#include <Mouse/MouseWidget.h>

class MouseController : public QObject
{
    Q_OBJECT
public:
    MouseController(MouseWidget *view);

    void handleButtonClicked();
    void appendMouseMove(QPoint pos);
    void handleKeyPressEvent(QKeyEvent *event);


signals:
    void buttonClick();

public slots:
              //void appendButtonData(QRect btnsize);

    void takeIndexSelectedRows(QModelIndexList indexList);
private:
    MouseWidget*    mouse_widget;
    DataManager&    dataManager =   DataManager::instance();
    ClickDataList&  clicksData =    dataManager.clicksData;
    QVector<QPoint> track_arr;
    QVector<qint64> times_arr;
    QElapsedTimer   timer;
    
};

#endif // MOUSECONTROLLER_H
