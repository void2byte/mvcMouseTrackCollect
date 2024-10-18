#include "GenerateNewMouseTrack.h"

#include <QPointF>
#include <QVector>
#include <QRandomGenerator>
#include <QtMath>

GenerateNewMouseTrack::GenerateNewMouseTrack() {}


// Структура для хранения точки с временной меткой
struct MousePoint {
    QPointF position;
    qint64 time;
};

// Функция для нормализации трека
void normalizeTrack(QVector<MousePoint>& track) {
    QPointF startPoint = track.first().position;
    for (auto& point : track) {
        point.position -= startPoint;
    }
}

// Функция для масштабирования трека
void scaleTrack(QVector<MousePoint>& track, const QPointF& scaleFactors) {
    for (auto& point : track) {
        point.position.setX(point.position.x() * scaleFactors.x());
        point.position.setY(point.position.y() * scaleFactors.y());
    }
}

// Функция для поворота трека
void rotateTrack(QVector<MousePoint>& track, double angle) {
    double radians = qDegreesToRadians(angle);
    double cosAngle = qCos(radians);
    double sinAngle = qSin(radians);
    for (auto& point : track) {
        double x = point.position.x() * cosAngle - point.position.y() * sinAngle;
        double y = point.position.x() * sinAngle + point.position.y() * cosAngle;
        point.position.setX(x);
        point.position.setY(y);
    }
}

// Функция для сдвига трека
void translateTrack(QVector<MousePoint>& track, const QPointF& translation) {
    for (auto& point : track) {
        point.position += translation;
    }
}

// Функция для рандомизации траектории
void randomizeTrajectory(QVector<MousePoint>& track, double maxOffset) {
    for (auto& point : track) {
        double offsetX = QRandomGenerator::global()->bounded(maxOffset);
        double offsetY = QRandomGenerator::global()->bounded(maxOffset);
        point.position += QPointF(offsetX, offsetY);
    }
}

// Функция для рандомизации времени
void randomizeTime(QVector<MousePoint>& track, qint64 maxTimeOffset) {
    for (auto& point : track) {
        qint64 timeOffset = QRandomGenerator::global()->bounded(-maxTimeOffset, maxTimeOffset);
        point.time += timeOffset;
    }
}

// Основная функция для генерации нового трека
QVector<MousePoint> generateNewMouseTrack(const QVector<MousePoint>& originalTrack,
                                          const QPointF& newStart,
                                          const QPointF& newEnd) {
    QVector<MousePoint> newTrack = originalTrack;

    // Нормализация
    normalizeTrack(newTrack);

    // Вычисление масштабирования
    QPointF originalDelta = newTrack.last().position;
    QPointF newDelta = newEnd - newStart;
    QPointF scaleFactors(newDelta.x() / originalDelta.x(),
                         newDelta.y() / originalDelta.y());

    // Масштабирование
    scaleTrack(newTrack, scaleFactors);

    // Поворот (если необходимо)
    double angle = QRandomGenerator::global()->bounded(5.0); // Рандомный небольшой угол
    rotateTrack(newTrack, angle);

    // Рандомизация траектории
    randomizeTrajectory(newTrack, 1.5);

    // Сдвиг к новым координатам
    translateTrack(newTrack, newStart);

    // Рандомизация времени
    randomizeTime(newTrack, 10);


    return newTrack;
}

