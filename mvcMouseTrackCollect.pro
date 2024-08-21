QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datamanager.cpp \
    main.cpp \
    maincontroller.cpp \
    mainwindow.cpp \
    mousecontroller.cpp \
    mousewidget.cpp \
    settingscontroller.cpp \
    settingswidget.cpp \
    transformationcontroller.cpp

HEADERS += \
    datamanager.h \
    maincontroller.h \
    mainwindow.h \
    mousecontroller.h \
    mousewidget.h \
    settingscontroller.h \
    settingswidget.h \
    transformationcontroller.h

#TRANSLATIONS += \
    #mvcMouseTrackCollect_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
