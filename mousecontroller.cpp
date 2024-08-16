#include <QDebug>

#include "mousecontroller.h"



MouseController::MouseController(MouseWidget *view)
    : mouse_widget(view) {

    timer.start();

    connect(mouse_widget, &MouseWidget::moveResizeButton, this,
            &MouseController::handleButtonClicked);

    connect(mouse_widget, &MouseWidget::mouseMoveSig, this,
            &MouseController::appendMouseMove);

}


void MouseController::handleButtonClicked() {

    clicksData.append(ClickData());

    emit buttonClick();
}


void MouseController::appendMouseMove(QPoint pos) {

    clicksData.back().buttonRect =      mouse_widget->getButtonGeometry();
    clicksData.back().clickPosition =   mouse_widget->getButtonPosition();
    clicksData.back().windowPosition =  mouse_widget->mapFromGlobal(QCursor::pos());
    clicksData.back().clickTime =       timer.elapsed();

    clicksData.back().track.append(pos);
    clicksData.back().times.append(timer.elapsed());

}


void MouseController::handleKeyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ControlModifier)) {
        // Действия при нажатии Ctrl+Z
        if (!clicksData.isEmpty() && mouse_widget->isActiveWindow()) {
            clicksData.pop_back();
            mouse_widget->update();
            mouse_widget->moveresizeTargetButton();
            emit buttonClick();
        }
    }
}




