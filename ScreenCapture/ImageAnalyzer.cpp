#include "ImageAnalyzer.h"

ImageAnalyzer::ImageAnalyzer() {}

ImageAnalyzer::~ImageAnalyzer() {}

QPoint ImageAnalyzer::findElementByColor(const QImage& image, 
                                       const QColor& targetColor, 
                                       int tolerance) {
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if (colorsMatch(image.pixelColor(x, y), targetColor, tolerance)) {
                return QPoint(x, y);
            }
        }
    }
    return QPoint(-1, -1);
}

std::vector<QPoint> ImageAnalyzer::findAllElementsByColor(const QImage& image, 
                                                         const QColor& targetColor, 
                                                         int tolerance) {
    std::vector<QPoint> results;
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if (colorsMatch(image.pixelColor(x, y), targetColor, tolerance)) {
                results.push_back(QPoint(x, y));
            }
        }
    }
    return results;
}

bool ImageAnalyzer::hasElementInArea(const QImage& image, const QRect& area, 
                                   const QColor& targetColor, int tolerance) {
    for (int y = area.top(); y <= area.bottom(); ++y) {
        for (int x = area.left(); x <= area.right(); ++x) {
            if (x >= 0 && x < image.width() && y >= 0 && y < image.height()) {
                if (colorsMatch(image.pixelColor(x, y), targetColor, tolerance)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ImageAnalyzer::colorsMatch(const QColor& c1, const QColor& c2, 
                               int tolerance) const {
    return (abs(c1.red() - c2.red()) <= tolerance &&
            abs(c1.green() - c2.green()) <= tolerance &&
            abs(c1.blue() - c2.blue()) <= tolerance);
}

cv::Mat ImageAnalyzer::qImageToCvMat(const QImage& image) {
    switch (image.format()) {
    case QImage::Format_RGB888: {
        cv::Mat mat(image.height(), image.width(), CV_8UC3, 
                   const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        return mat;
    }
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied: {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, 
                   const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
        return mat;
    }
    default:
        return cv::Mat();
    }
}

QImage ImageAnalyzer::cvMatToQImage(const cv::Mat& mat) {
    if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, 
                     QImage::Format_RGB888).copy();
    }
    return QImage();
}

QPoint ImageAnalyzer::findTemplate(const QImage& image, const QImage& templ, 
                                 double threshold) {
    cv::Mat img = qImageToCvMat(image);
    cv::Mat tpl = qImageToCvMat(templ);
    
    if (img.empty() || tpl.empty()) return QPoint(-1, -1);

    cv::Mat result;
    cv::matchTemplate(img, tpl, result, cv::TM_CCOEFF_NORMED);
    
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
    
    if (maxVal >= threshold) {
        return QPoint(maxLoc.x, maxLoc.y);
    }
    
    return QPoint(-1, -1);
} 