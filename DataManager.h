#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QVector>
#include <QRect>
#include <atomic>

struct ClickData;

struct ClickData {

    ClickData() : id(nextId()) {}

    uint getId() const { return id; }

    QRect buttonRect{};        // Размеры и положение кнопки
    QPoint clickPosition{};    // Координаты клика внутри кнопки
    QPoint windowPosition{};   // Координаты клика внутри окна
    qint64 clickTime{};     // Время клика

    QVector<QPoint> track{};
    QVector<qint64> times{};

    double calculateDistance(const QPoint& point1, const QPoint& point2) const;
    double calculateDistance() const;

    double calculateCurveLength(const QVector<QPoint>& track ) const;
    double calculateCurveLength() const;

private:
    uint id;

    static std::atomic<uint> counter;  // Счетчик для генерации уникальных идентификаторов

    static uint nextId() {
        return ++counter;
    }
};

using ClickDataList = QList<ClickData>;

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

    void saveDataToXml(const ClickDataList& clicksData,
                       const QString& fileName  = "MouseMoveData.xml");

    ClickDataList loadDataFromXml(
        const QString& fileName = "MouseMoveData.xml");

    ClickDataList clicksData;

};

#endif // DATAMANAGER_H
