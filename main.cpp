#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "maincontroller.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainController controller;

    return a.exec();
}
