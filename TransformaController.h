#ifndef TRASFORMACONTROLLER_H
#define TRASFORMACONTROLLER_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QDebug>
#include "DataManager.h"
#include <cmath>


struct PolarPoint {
    double r;
    double theta;
    double acceleration;
    qint64 delta_time;
};


class TrackGenerator {
public:
    /*!
     * \brief Генерирует случайный трек на основе исходных данных.
     * \param baseClickData Исходные данные кликов.
     * \param rnd_coefficient Коэффициент случайности.
     * \param min_time_interval Минимальный интервал времени.
     * \param max_time_interval Максимальный интервал времени.
     * \param acceleration_range Диапазон случайного ускорения.
     * \param deviation_range Диапазон отклонения от центра масс.
     * \return Сгенерированные данные кликов.
     */
    static ClickData generateRandomTrack(const ClickData &baseClickData,
                                         float rnd_coefficient  = 0.1,
                                         int min_time_interval = 10,
                                         int max_time_interval = 100,
                                         double acceleration_range = 0.5,
                                         double deviation_range = 5.0);
};



/*!
     * \brief Преобразует траекторию в полярные координаты и рассчитывает ускорение.
     * \param clickData Объект ClickData, содержащий данные кликов.
     * \return Список полярных координат.
     */
QVector<PolarPoint> convertToPolarCoordinates(const ClickData &clickData);

/*!
     * \brief Преобразует данные из полярных координат обратно в декартовые координаты.
     * \param polarCoordinates Список полярных координат.
     * \return Объект ClickData с восстановленными координатами.
     */
ClickData convertToCartesianCoordinates(const QVector<PolarPoint> &polarCoordinates);

/*!
     * \brief Фильтрует шум из данных кликов.
     * \param clickData Данные кликов.
     * \param threshold Пороговое значение для фильтрации.
     * \return Отфильтрованный ClickData.
     */
ClickData filterNoise(const ClickData &clickData, double threshold);

/*!
     * \brief Интерполирует трек, добавляя дополнительные точки.
     * \param clickData Данные кликов.
     * \param numInterpolatedPoints Количество точек для интерполяции.
     * \return ClickData с интерполированными точками.
     */
ClickData interpolateTrack(const ClickData &clickData, int numInterpolatedPoints);

/*!
     * \brief Анализирует скорость и ускорение.
     * \param clickData Данные кликов.
     * \return Пара значений, содержащая максимальную скорость и ускорение.
     */
std::pair<double, double> analyzeSpeedAndAcceleration(const ClickData &clickData);

/*!
     * \brief Преобразует координаты в заданную пользовательскую систему координат.
     * \param clickData Данные кликов.
     * \param origin Начало координат для новой системы.
     * \param rotationAngle Угол поворота.
     * \return Список преобразованных точек.
     */
QVector<QPointF> convertToCustomCoordinates(const ClickData &clickData, const QPointF &origin, double rotationAngle);

/*!
     * \brief Нормализует данные трека.
     * \param clickData Данные кликов.
     * \return Нормализованные данные кликов.
     */
ClickData normalizeTrack(const ClickData &clickData);

/*!
     * \brief Сравнивает два трека.
     * \param track1 Первый трек.
     * \param track2 Второй трек.
     * \return Степень схожести между треками.
     */
double compareTracks(const ClickData &track1, const ClickData &track2);


#endif // TRASFORMACONTROLLER_H
