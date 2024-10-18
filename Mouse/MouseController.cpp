#include <QDebug>

#include "Mouse/MouseController.h"
#include "qabstractitemmodel.h"



MouseController::MouseController(MouseWidget *view)
    : mouse_widget(view) {

    timer.start();

    connect(mouse_widget->button, &QPushButton::clicked, this,
            &MouseController::handleButtonClicked);

    connect(mouse_widget, &MouseWidget::mouseMoveSig, this,
            &MouseController::appendMouseMove);

}


void MouseController::handleButtonClicked() {

    auto currentClDt = dataManager.currentClickData;
    currentClDt->clickTime =       timer.elapsed();
    QPoint cursor = mouse_widget->mapFromGlobal(mouse_widget->cursor().pos());
    currentClDt->buttonRect =      mouse_widget->getButtonGeometry();
    currentClDt->clickPositionInButton =   cursor - mouse_widget->getButtonPosition();
    currentClDt->globalClickPosition =  cursor;

    emit buttonClick();

    timer.restart();
    mouse_widget->moveresizeTargetButton();

    dataManager.clicksData.append(*currentClDt);
    dataManager.currentClickData = new ClickData();
}


void MouseController::appendMouseMove(QPoint pos) {

    dataManager.currentClickData->track.append(pos);
    dataManager.currentClickData->times.append(timer.elapsed());
}

void MouseController::takeIndexSelectedRows(QModelIndexList indexList) {
    if (indexList.empty()) {
        mouse_widget->selectedClickDataForDraw.clear();
        mouse_widget->update();
        return;
    }

    // Сортируем индексы в обратном порядке для корректного удаления
    std::sort(indexList.begin(), indexList.end(), [](const QModelIndex &a, const QModelIndex &b) {
        return a.row() > b.row();
    });

    mouse_widget->selectedClickDataForDraw.clear();
    for (const QModelIndex &index : indexList) {
        bool ok = false;
        qsizetype clickindex = index.siblingAtColumn(0).data().toLongLong(&ok);
        if (!ok) {
            qDebug() << "SettingsController::removeSelectedRows fail take index from row:" << index.row();
            continue;
        }

        for(auto& it : clicksData) {
            if (it.getId() == clickindex) {
                mouse_widget->selectedClickDataForDraw.append(&it);
            }
        }

    }
    mouse_widget->update();
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




