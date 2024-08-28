QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataManager.cpp \
    MainController.cpp \
    MainWindow.cpp \
    MouseController.cpp \
    MouseWidget.cpp \
    SettingsController.cpp \
    SettingsModel.cpp \
    SettingsWidget.cpp \
    TransformaController.cpp \
    main.cpp

HEADERS += \
    DataManager.h \
    MainController.h \
    MainWindow.h \
    MouseController.h \
    MouseWidget.h \
    SettingsController.h \
    SettingsModel.h \
    SettingsWidget.h \
    TransformaController.h

#TRANSLATIONS += \
    #mvcMouseTrackCollect_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
