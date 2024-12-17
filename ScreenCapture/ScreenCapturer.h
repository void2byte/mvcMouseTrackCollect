#ifndef SCREENCAPTURER_H
#define SCREENCAPTURER_H

#include <QScreen>
#include <QPixmap>
#include <QApplication>
#include <QRect>

class ScreenCapturer {
public:
    ScreenCapturer();
    ~ScreenCapturer();

    QPixmap captureScreen(const QRect& area = QRect());
    QPixmap captureWindow(WId windowId);
    QScreen* getCurrentScreen() const;

private:
    QScreen* primaryScreen;
};

#endif // SCREENCAPTURER_H 