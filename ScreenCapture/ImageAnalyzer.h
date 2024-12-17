#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include <QImage>
#include <QPoint>
#include <QColor>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class ImageAnalyzer {
public:
    ImageAnalyzer();
    ~ImageAnalyzer();

    // Найти элемент по цвету
    QPoint findElementByColor(const QImage& image, const QColor& targetColor, 
                            int tolerance = 5);
    
    // Найти все элементы заданного цвета
    std::vector<QPoint> findAllElementsByColor(const QImage& image, 
                                             const QColor& targetColor, 
                                             int tolerance = 5);
    
    // Проверить наличие элемента в области
    bool hasElementInArea(const QImage& image, const QRect& area, 
                         const QColor& targetColor, int tolerance = 5);

    // Добавим методы с использованием OpenCV
    cv::Mat qImageToCvMat(const QImage& image);
    QImage cvMatToQImage(const cv::Mat& mat);
    
    // Поиск шаблона на изображении
    QPoint findTemplate(const QImage& image, const QImage& templ, double threshold = 0.8);

private:
    bool colorsMatch(const QColor& c1, const QColor& c2, int tolerance) const;
};

#endif // IMAGEANALYZER_H 