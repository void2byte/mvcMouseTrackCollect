#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QVector>
#include <QRect>

struct ClickData;

struct ClickData {
    QRect buttonRect;        // Размеры и положение кнопки
    QPoint clickPosition;    // Координаты клика внутри кнопки
    QPoint windowPosition;   // Координаты клика внутри окна
    qint64 clickTime;     // Время клика

    QVector<QPoint> track;
    QVector<qint64> times;
};




class DataManager {
    DataManager();
    ~DataManager();

public:

    static DataManager& instance() {
        static DataManager manager;
        return manager;
    }

    DataManager(const DataManager&) = delete;
    void operator=(const DataManager&) = delete;

    void saveDataToXml(const QVector<ClickData>& clicksData,
                       const QString& fileName  = "MouseMoveData.xml");

    QVector<ClickData> loadDataFromXml(
        const QString& fileName = "MouseMoveData.xml");

    QVector<ClickData> clicksData;

};

#endif // DATAMANAGER_H
