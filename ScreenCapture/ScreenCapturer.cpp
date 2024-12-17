#include "ScreenCapturer.h"

ScreenCapturer::ScreenCapturer() {
    primaryScreen = QApplication::primaryScreen();
}

ScreenCapturer::~ScreenCapturer() {}

QPixmap ScreenCapturer::captureScreen(const QRect& area) {
    if (!primaryScreen) return QPixmap();
    
    if (area.isValid()) {
        return primaryScreen->grabWindow(0, area.x(), area.y(), 
                                       area.width(), area.height());
    }
    return primaryScreen->grabWindow(0);
}

QPixmap ScreenCapturer::captureWindow(WId windowId) {
    if (!primaryScreen) return QPixmap();
    return primaryScreen->grabWindow(windowId);
}

QScreen* ScreenCapturer::getCurrentScreen() const {
    return primaryScreen;
} 