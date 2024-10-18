#include "TransformaController.h"
#include <random>
#include <limits>


QVector<PolarPoint> convertToPolarCoordinates(const ClickData &clickData) {
    QVector<PolarPoint> polarCoordinates;
    qDebug() << "Converting to polar coordinates";

    // Проверяем, что оба списка track и times имеют одинаковый размер и содержат минимум 2 точки
    if (clickData.track.size() < 2 || clickData.track.size() != clickData.times.size()) {
        qDebug() << "Invalid click data: track size and times size mismatch or insufficient points";
        return polarCoordinates;  // Если недостаточно данных или размеры не совпадают, возвращаем пустой список
    }

    QPoint startPoint = clickData.track.first();
    qDebug() << "Start point:" << startPoint;

    // Проходим по всем точкам
    for (int i = 0; i < clickData.track.size(); ++i) {
        // Вычисляем r и theta для текущей точки относительно стартовой
        QPoint relativePoint = clickData.track[i] - startPoint;
        double r = std::sqrt(std::pow(relativePoint.x(), 2) + std::pow(relativePoint.y(), 2));
        double theta = std::atan2(relativePoint.y(), relativePoint.x());
        qDebug() << "Point:" << clickData.track[i] << "Relative r:" << r << "Theta:" << theta;

        // Рассчитываем ускорение, начиная со второй точки
        double acceleration = 0;
        qint64 deltaTime = 0;
        if (i > 0) {
            deltaTime = clickData.times[i] - clickData.times[i - 1];
            qDebug() << "Delta time between points" << i - 1 << "and" << i << ":" << deltaTime;

            if (deltaTime > 0) {
                QPoint deltaPos = clickData.track[i] - clickData.track[i - 1];
                double velocity = std::sqrt(std::pow(deltaPos.x(), 2) + std::pow(deltaPos.y(), 2)) / deltaTime;
                qDebug() << "Velocity at point" << i << ":" << velocity;
                if (i > 1) {
                    QPoint prevDeltaPos = clickData.track[i - 1] - clickData.track[i - 2];
                    qint64 prevDeltaTime = clickData.times[i - 1] - clickData.times[i - 2];
                    qDebug() << "Previous delta time between points" << i - 2 << "and" << i - 1 << ":" << prevDeltaTime;

                    if (prevDeltaTime > 0) {
                        double prevVelocity = std::sqrt(std::pow(prevDeltaPos.x(), 2) + std::pow(prevDeltaPos.y(), 2)) / prevDeltaTime;
                        acceleration = (velocity - prevVelocity) / deltaTime;
                        qDebug() << "Acceleration at point" << i << ":" << acceleration;
                    }
                }
            }
        }

        polarCoordinates.append({r, theta, acceleration, deltaTime});
    }

    return polarCoordinates;
}


ClickData convertToCartesianCoordinates(const QVector<PolarPoint> &polarCoordinates) {
    ClickData clickData;
    qDebug() << "Converting to Cartesian coordinates";

    if (polarCoordinates.size() < 2) {
        qDebug() << "Insufficient polar coordinates to convert";
        return clickData;
    }

    QPoint prevPoint;
    double prevVelocity = 0.0;
    qint64 currentTime = 0;
    double prevX = 0.0;
    double prevY = 0.0;

    for (int i = 0; i < polarCoordinates.size(); ++i) {
        const PolarPoint& polarPoint = polarCoordinates[i];

        // Восстановление координат на основе r и theta
        double x = polarPoint.r * std::cos(polarPoint.theta) + prevX;
        double y = polarPoint.r * std::sin(polarPoint.theta) + prevY;
        QPoint currentPoint(x, y);
        qDebug() << "Polar point" << i << ": r=" << polarPoint.r << "theta=" << polarPoint.theta << " -> Cartesian point:" << currentPoint;

        if (i > 0) {
            // Восстановление времени
            currentTime += polarPoint.delta_time;
            qDebug() << "Current time for point" << i << ":" << currentTime;

            // Добавляем восстановленные данные в ClickData
            clickData.track.append(currentPoint);
            clickData.times.append(currentTime);
        } else {
            // Для первой точки просто сохраняем координаты и время
            clickData.track.append(currentPoint);
            clickData.times.append(currentTime);
        }

        prevPoint = currentPoint;
        prevX = x;
        prevY = y;
    }

    return clickData;
}


ClickData filterNoise(const ClickData &clickData, double threshold) {
    ClickData filteredData;
    qDebug() << "Filtering noise with threshold:" << threshold;

    if (clickData.track.size() < 1) {
        qDebug() << "No points to filter";
        return filteredData;
    }

    filteredData.track.append(clickData.track.first());
    filteredData.times.append(clickData.times.first());
    qDebug() << "First point added:" << clickData.track.first();

    for (int i = 1; i < clickData.track.size(); ++i) {
        QPoint deltaPos = clickData.track[i] - clickData.track[i - 1];
        double distance = std::sqrt(std::pow(deltaPos.x(), 2) + std::pow(deltaPos.y(), 2));
        qDebug() << "Distance between points" << i - 1 << "and" << i << ":" << distance;

        if (distance >= threshold) {
            filteredData.track.append(clickData.track[i]);
            filteredData.times.append(clickData.times[i]);
            qDebug() << "Point" << i << "added after filtering:" << clickData.track[i];
        }
    }

    return filteredData;
}


ClickData interpolateTrack(const ClickData &clickData, int numInterpolatedPoints) {
    ClickData interpolatedData;
    qDebug() << "Interpolating track with" << numInterpolatedPoints << "points per segment";

    if (clickData.track.size() < 2 || numInterpolatedPoints <= 0) {
        qDebug() << "Insufficient points or invalid number of interpolations";
        return clickData;
    }

    for (int i = 0; i < clickData.track.size() - 1; ++i) {
        QPoint startPoint = clickData.track[i];
        QPoint endPoint = clickData.track[i + 1];
        qint64 startTime = clickData.times[i];
        qint64 endTime = clickData.times[i + 1];

        interpolatedData.track.append(startPoint);
        interpolatedData.times.append(startTime);

        for (int j = 1; j <= numInterpolatedPoints; ++j) {
            double t = static_cast<double>(j) / (numInterpolatedPoints + 1);
            QPoint interpolatedPoint = startPoint + t * (endPoint - startPoint);
            qint64 interpolatedTime = startTime + static_cast<qint64>(t * (endTime - startTime));

            interpolatedData.track.append(interpolatedPoint);
            interpolatedData.times.append(interpolatedTime);
        }
    }

    // Добавляем последнюю точку
    interpolatedData.track.append(clickData.track.last());
    interpolatedData.times.append(clickData.times.last());

    return interpolatedData;
}


std::pair<double, double> analyzeSpeedAndAcceleration(const ClickData &clickData) {
    double maxSpeed = 0.0;
    double maxAcceleration = 0.0;


    if (clickData.track.size() < 2) {
        qDebug() << "Insufficient points for analysis";
        return {maxSpeed, maxAcceleration};
    }

    double prevSpeed = 0.0;
    for (int i = 1; i < clickData.track.size(); ++i) {
        QPoint deltaPos = clickData.track[i] - clickData.track[i - 1];
        qint64 deltaTime = clickData.times[i] - clickData.times[i - 1];

        if (deltaTime > 0) {
            double speed = std::sqrt(std::pow(deltaPos.x(), 2) + std::pow(deltaPos.y(), 2)) / deltaTime;
            maxSpeed = std::max(maxSpeed, speed);
            qDebug() << "Speed at point" << i << ":" << speed;

            if (i > 1) {
                double acceleration = (speed - prevSpeed) / deltaTime;
                maxAcceleration = std::max(maxAcceleration, acceleration);
                qDebug() << "Acceleration at point" << i << ":" << acceleration;
            }
            prevSpeed = speed;
        }
    }

    qDebug() << "Max speed:" << maxSpeed << "Max acceleration:" << maxAcceleration;
    return {maxSpeed, maxAcceleration};
}


QVector<QPointF> convertToCustomCoordinates(const ClickData &clickData, const QPointF &origin, double rotationAngle) {
    QVector<QPointF> transformedPoints;
    qDebug() << "Converting to custom coordinates with origin:" << origin << "and rotation angle:" << rotationAngle;
    double cosAngle = std::cos(rotationAngle);
    double sinAngle = std::sin(rotationAngle);

    for (const QPoint &point : clickData.track) {
        double translatedX = point.x() - origin.x();
        double translatedY = point.y() - origin.y();

        double rotatedX = translatedX * cosAngle - translatedY * sinAngle;
        double rotatedY = translatedX * sinAngle + translatedY * cosAngle;

        QPointF transformedPoint(rotatedX + origin.x(), rotatedY + origin.y());
        transformedPoints.append(transformedPoint);
        qDebug() << "Original point:" << point << "-> Transformed point:" << transformedPoint;
    }

    return transformedPoints;
}


ClickData normalizeTrack(const ClickData &clickData) {
    ClickData normalizedData;
    qDebug() << "Normalizing track";

    if (clickData.track.isEmpty()) {
        qDebug() << "No points to normalize";
        return normalizedData;
    }

    // Найти минимальные и максимальные значения координат
    int minX = std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int maxY = std::numeric_limits<int>::min();

    for (const QPoint &point : clickData.track) {
        minX = std::min(minX, point.x());
        minY = std::min(minY, point.y());
        maxX = std::max(maxX, point.x());
        maxY = std::max(maxY, point.y());
    }
    qDebug() << "Min X:" << minX << "Max X:" << maxX << "Min Y:" << minY << "Max Y:" << maxY;

    // Нормализация координат в диапазон [0, 1]
    for (const QPoint &point : clickData.track) {
        double normalizedX = (point.x() - minX) / static_cast<double>(maxX - minX);
        double normalizedY = (point.y() - minY) / static_cast<double>(maxY - minY);
        QPoint normalizedPoint(normalizedX, normalizedY);
        normalizedData.track.append(normalizedPoint);
        qDebug() << "Original point:" << point << "-> Normalized point:" << normalizedPoint;
    }

    normalizedData.times = clickData.times;
    return normalizedData;
}


double compareTracks(const ClickData &track1, const ClickData &track2) {
    if (track1.track.size() != track2.track.size() || track1.times.size() != track2.times.size()) {
        return 0.0; // Если размеры не совпадают, считаем треки не схожими
    }

    double similarity = 0.0;
    int n = track1.track.size();

    for (int i = 0; i < n; ++i) {
        QPoint delta = track1.track[i] - track2.track[i];
        double distance = std::sqrt(std::pow(delta.x(), 2) + std::pow(delta.y(), 2));
        similarity += distance;
    }

    // Нормализация схожести: чем меньше значение, тем больше схожесть
    return 1.0 / (1.0 + similarity / n);
}




ClickData TrackGenerator::generateRandomTrack(const ClickData &baseClickData, float rnd_coefficient,
                                              int min_time_interval, int max_time_interval, double acceleration_range, double deviation_range) {
    ClickData newClickData;
    if (baseClickData.track.isEmpty()) {
        return newClickData;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    QPoint startPoint = baseClickData.track.first();
    qint64 startTime = baseClickData.times.first();

    newClickData.track.append(startPoint);
    newClickData.times.append(startTime);

    std::uniform_real_distribution<float> distX(-rnd_coefficient, rnd_coefficient);
    std::uniform_real_distribution<float> distY(-rnd_coefficient, rnd_coefficient);
    std::uniform_int_distribution<int> distTime(min_time_interval, max_time_interval);
    std::uniform_real_distribution<double> distAccel(-acceleration_range, acceleration_range);
    std::uniform_real_distribution<double> distDeviation(-deviation_range, deviation_range);

    double cumulativeDeviation = 0.0;

    for (int i = 1; i < baseClickData.track.size(); ++i) {
        QPoint lastPoint = newClickData.track.last();
        qint64 lastTime = newClickData.times.last();

        // Случайное отклонение от предыдущей точки и добавление к центру масс
        cumulativeDeviation += distDeviation(gen);
        QPoint newPoint = lastPoint + QPoint(distX(gen) + cumulativeDeviation, distY(gen) + cumulativeDeviation);
        qint64 newTime = lastTime + distTime(gen);

        newClickData.track.append(newPoint);
        newClickData.times.append(newTime);

        // Добавление случайного ускорения
        // ТРЕБУЕТСЯ РЕАЛИЗАЦИЯ
        PolarPoint polar = { static_cast<double>(newPoint.manhattanLength()), 0.0, distAccel(gen), static_cast<qint64>(distTime(gen)) };
        //newClickData.polarData.append(polar);
    }

    return newClickData;
}
