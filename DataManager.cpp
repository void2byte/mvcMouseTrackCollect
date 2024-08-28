#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>

#include "DataManager.h"

std::atomic<uint> ClickData::counter{0};

DataManager::DataManager()
    : clicksData{ClickData()}
{}

DataManager::~DataManager()
{}

void DataManager::saveDataToXml(const ClickDataList& clicksData,
                                const QString& fileName /* = "MouseMoveData.xml" */) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Ошибка при открытии файла: " << fileName;
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
        xmlWriter.writeAttribute("x", QString::number(click.clickPosition.x()));
        xmlWriter.writeAttribute("y", QString::number(click.clickPosition.y()));
        xmlWriter.writeEndElement(); // ClickPosition

        xmlWriter.writeStartElement("WindowPosition");
        xmlWriter.writeAttribute("x", QString::number(click.windowPosition.x()));
        xmlWriter.writeAttribute("y", QString::number(click.windowPosition.y()));
        xmlWriter.writeEndElement(); // WindowPosition

        xmlWriter.writeTextElement("ClickTime", QString::number(click.clickTime));

        // Запись данных о движениях мыши для каждого клика
        xmlWriter.writeStartElement("MouseMoves");
        auto trackIt = click.track.crbegin();
        auto timesIt = click.times.crbegin();

        while (trackIt != click.track.crend() && timesIt != click.times.crend()) {
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
}

QVector<ClickData> DataManager::loadDataFromXml(const QString& fileName) {
    QVector<ClickData> clicksData;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Ошибка при открытии файла: " << fileName;
        return clicksData;
    }

    QXmlStreamReader xmlReader(&file);
    ClickData currentClickData;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (xmlReader.name().compare("Click") == 0) {
                currentClickData = ClickData();
            } else if (xmlReader.name().compare("ButtonRect") == 0) {
                QXmlStreamAttributes attrs = xmlReader.attributes();
                currentClickData.buttonRect.setX(attrs.value("x").toInt());
                currentClickData.buttonRect.setY(attrs.value("y").toInt());
                currentClickData.buttonRect.setWidth(attrs.value("width").toInt());
                currentClickData.buttonRect.setHeight(attrs.value("height").toInt());
            } else if (xmlReader.name().compare("ClickPosition") == 0) {
                QXmlStreamAttributes attrs = xmlReader.attributes();
                currentClickData.clickPosition.setX(attrs.value("x").toInt());
                currentClickData.clickPosition.setY(attrs.value("y").toInt());
            } else if (xmlReader.name().compare("WindowPosition") == 0) {
                QXmlStreamAttributes attrs = xmlReader.attributes();
                currentClickData.windowPosition.setX(attrs.value("x").toInt());
                currentClickData.windowPosition.setY(attrs.value("y").toInt());
            } else if (xmlReader.name().compare("ClickTime") == 0) {
                currentClickData.clickTime = xmlReader.readElementText().toLongLong();
            } else if (xmlReader.name().compare("MouseMove") == 0) {
                QPoint pos;
                qint64 time{};
                if (xmlReader.readNextStartElement() && xmlReader.name().compare("Position") == 0) {
                    QXmlStreamAttributes attrs = xmlReader.attributes();
                    pos.setX(attrs.value("x").toInt());
                    pos.setY(attrs.value("y").toInt());
                }
                if (xmlReader.readNextStartElement() && xmlReader.name().compare("MoveTime") == 0) {
                    time = xmlReader.readElementText().toLongLong();
                }
                currentClickData.track.append(pos);
                currentClickData.times.append(time);
            }
        } else if (xmlReader.isEndElement()) {
            if (xmlReader.name().compare("Click") == 0) {
                clicksData.append(currentClickData);
            }
        }
    }

    if (xmlReader.hasError()) {
        qDebug() << "Ошибка чтения XML:" << xmlReader.errorString();
    }

    file.close();
    return clicksData;
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
