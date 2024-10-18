#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "Main/MainController.h"


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    MainController controller;

    return a.exec();
}
