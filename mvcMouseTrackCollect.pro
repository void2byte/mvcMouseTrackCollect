QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataManager.cpp \
    InputEmulator/ArduinoCommand.cpp \
    Main\MainController.cpp \
    Main\MainWindow.cpp \
    Mouse\MouseController.cpp \
    Mouse\MouseWidget.cpp \
    Result/GenerateNewMouseTrack.cpp \
    Selection\SettingsController.cpp \
    Selection\SettingsModel.cpp \
    Selection\SettingsWidget.cpp \
    TransformaController.cpp \
    main.cpp

HEADERS += \
    DataManager.h \
    InputEmulator/ArduinoCommand.h \
    Main\MainController.h \
    Main\MainWindow.h \
    Mouse\MouseController.h \
    Mouse\MouseWidget.h \
    Result/GenerateNewMouseTrack.h \
    Selection\SettingsController.h \
    Selection\SettingsModel.h \
    Selection\SettingsWidget.h \
    TransformaController.h

#TRANSLATIONS += \
    #mvcMouseTrackCollect_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
