#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QList>

#include "DataManager.h"

std::atomic<uint> ClickData::counter{0};

DataManager::DataManager()
    : currentClickData(new ClickData())
{}

DataManager::~DataManager()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    saveDataToXml(clicksData, /*currentDateTime.toString("yyyy-MM-dd_hh-mm-ss") + */"MouseMoveData.xml");
}

void DataManager::saveDataToXml(ClickDataList& clicksData,
                                const QString& fileName /* = "MouseMoveData.xml" */) {
    if(clicksData.size() < 1) {
        qDebug()<<"данные не сохранены ClicksData.size() < 1";
        return;
    }
    //удаляем все треки содержащие менне трех позиций
    clicksData.removeIf([] (const ClickData& clc) {
        return clc.track.size() < 3;
        qDebug()<<"удален элемент: clicksData.removeIf([] (const ClickData& clc) { return clc.track.size() < 3;}";
    });

    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Ошибка при открытии файла: " << fileName;
        qDebug() << "Ошибка: " << file.errorString();
        qDebug() << "Текущая директория:" << QDir::currentPath();
        return;
    }


    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Data");


    xmlWriter.writeStartElement("Clicks");
    for (const auto& click : clicksData) {
        xmlWriter.writeStartElement("Click");

        xmlWriter.writeStartElement("ButtonRect");
        xmlWriter.writeAttribute("x", QString::number(click.buttonRect.x()));
        xmlWriter.writeAttribute("y", QString::number(click.buttonRect.y()));
        xmlWriter.writeAttribute("width", QString::number(click.buttonRect.width()));
        xmlWriter.writeAttribute("height", QString::number(click.buttonRect.height()));
        xmlWriter.writeEndElement(); // ButtonRect

        xmlWriter.writeStartElement("ClickPosition");
        xmlWriter.writeAttribute("x", QString::number(click.clickPositionInButton.x()));
        xmlWriter.writeAttribute("y", QString::number(click.clickPositionInButton.y()));
        xmlWriter.writeEndElement(); // ClickPosition

        xmlWriter.writeStartElement("WindowPosition");
        xmlWriter.writeAttribute("x", QString::number(click.globalClickPosition.x()));
        xmlWriter.writeAttribute("y", QString::number(click.globalClickPosition.y()));
        xmlWriter.writeEndElement(); // WindowPosition

        xmlWriter.writeTextElement("ClickTime", QString::number(click.clickTime));

        // Запись данных о движениях мыши для каждого клика
        xmlWriter.writeStartElement("MouseMoves");
        auto trackIt = click.track.constBegin();
        auto timesIt = click.times.constBegin();

        while (trackIt != click.track.constEnd() && timesIt != click.times.constEnd()) {
            xmlWriter.writeStartElement("MouseMove");

            xmlWriter.writeStartElement("Position");
            xmlWriter.writeAttribute("x", QString::number(trackIt->x()));
            xmlWriter.writeAttribute("y", QString::number(trackIt->y()));
            xmlWriter.writeEndElement(); // Position

            xmlWriter.writeTextElement("MoveTime", QString::number(*timesIt));

            xmlWriter.writeEndElement(); // MouseMove

            ++trackIt;
            ++timesIt;
        }
        xmlWriter.writeEndElement(); // MouseMoves

        xmlWriter.writeEndElement(); // Click
    }
    xmlWriter.writeEndElement(); // Clicks

    xmlWriter.writeEndElement(); // Data
    xmlWriter.writeEndDocument();

    file.close();

    qDebug() << "Файл успешно сохранен:" << fileName;
    qDebug() << "в директории:" << QDir::currentPath();
}

bool DataManager::loadDataFromXml(const QString& fileName) {

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Ошибка при открытии файла: " << fileName;
        return false;
    }

    QXmlStreamReader xmlReader(&file);
    ClickData _currentClickData;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (xmlReader.name().compare("Click") == 0) {
                _currentClickData = ClickData();
            } else if (xmlReader.name().compare("ButtonRect") == 0) {
                QXmlStreamAttributes attrs = xmlReader.attributes();
                _currentClickData.buttonRect.setX(attrs.value("x").toInt());
                _currentClickData.buttonRect.setY(attrs.value("y").toInt());
                _currentClickData.buttonRect.setWidth(attrs.value("width").toInt());
                _currentClickData.buttonRect.setHeight(attrs.value("height").toInt());
            } else if (xmlReader.name().compare("ClickPosition") == 0) {
                QXmlStreamAttributes attrs = xmlReader.attributes();
                _currentClickData.clickPositionInButton.setX(attrs.value("x").toInt());
                _currentClickData.clickPositionInButton.setY(attrs.value("y").toInt());
            } else if (xmlReader.name().compare("WindowPosition") == 0) {
                QXmlStreamAttributes attrs = xmlReader.attributes();
                _currentClickData.globalClickPosition.setX(attrs.value("x").toInt());
                _currentClickData.globalClickPosition.setY(attrs.value("y").toInt());
            } else if (xmlReader.name().compare("ClickTime") == 0) {
                _currentClickData.clickTime = xmlReader.readElementText().toLongLong();
            } else if (xmlReader.name().compare("MouseMove") == 0) {
                QPoint pos;
                qint64 time{};
                if (xmlReader.readNextStartElement() && xmlReader.name().compare("Position") == 0) {
                    QXmlStreamAttributes attrs = xmlReader.attributes();
                    pos.setX(attrs.value("x").toInt());
                    pos.setY(attrs.value("y").toInt());
                }

                xmlReader.readNext();

                if (xmlReader.readNextStartElement() && xmlReader.name().compare("MoveTime") == 0) {
                    QString moveTimeText = xmlReader.readElementText();
                    bool success = false;
                    time = moveTimeText.toULongLong(&success);
                    if (!success) {
                        qWarning() << "Не удалось преобразовать MoveTime в число из строки" << xmlReader.readElementText();
                    }
                } else {
                    qDebug() << "Следующий элемент не является StartElement или это не MoveTime";
                }
                _currentClickData.track.append(pos);
                _currentClickData.times.append(time);
            }
        } else if (xmlReader.isEndElement()) {
            if (xmlReader.name().compare("Click") == 0) {
                if (!_currentClickData.track.isEmpty() && !_currentClickData.times.isEmpty()) {
                    clicksData.append(_currentClickData);
                } else {
                    qDebug() << "Ошибка: Неполные данные для Click";
                }
            }
        }
    }

    if (xmlReader.hasError()) {
        qDebug() << "Ошибка чтения XML:" << xmlReader.errorString();
    }

    file.close();
    return true;
}


double ClickData::calculateDistance(const QPoint& point1, const QPoint& point2) const
{
    int dx = point2.x() - point1.x();
    int dy = point1.y() - point2.y();
    return std::sqrt(dx * dx + dy * dy);
}

double ClickData::calculateCurveLength(const QVector<QPoint> &track) const
{
    if(track.empty())
        return 0;

    double length = 0.0;
    for (int i = 1; i < track.size(); ++i) {
        length += calculateDistance(track[i], track[i - 1]);
    }
    return length;
}

double ClickData::calculateDistance() const {
    if (track.isEmpty()) {
        return 0.0;  // Возвращаем 0 для пустого трека
    }
    return calculateDistance(track.first(), track.last());
}

double ClickData::calculateCurveLength() const {
    return calculateCurveLength(track);
}


double calculateDistance(const QPoint &point1, const QPoint &point2) {
    int dx = point2.x() - point1.x();
    int dy = point1.y() - point2.y();
    return std::sqrt(dx * dx + dy * dy);
}


double calculateCurveLength(const QVector<QPoint> &track) {
    if (track.empty())
        return 0;

    double length = 0.0;
    for (int i = 1; i < track.size(); ++i) {
        length += calculateDistance(track[i], track[i - 1]);
    }
    return length;
}
