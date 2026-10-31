//
// Created by Dmitri on 2026-10-31.
//

#include "UCDP.h"

#include <iostream>

#include "Math.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

std::vector<glm::vec3> UCDP::GetConflictPoints(const char* jsonPath) {

    QFile file(jsonPath);
    std::vector<glm::vec3> points;

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file";
        return std::vector<glm::vec3>();
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    QJsonArray events = doc.array();

    for (const QJsonValue& value : events) {
        QJsonObject event = value.toObject();

        int id = event["id"].toInt();
        int year = event["year"].toInt();

        QString country = event["country"].toString();
        QString conflict = event["conflict_name"].toString();

        double latitude = event["latitude"].toDouble();
        double longitude = event["longitude"].toDouble();

        float latRad = ToRadians(latitude);
        float longRad = ToRadians(longitude);
        glm::vec2 mercator = ToMercator(latRad, longRad);

        points.push_back(glm::vec3(mercator.x * 10.0f, mercator.y * 10.0f, 0));
    }

    return points;
}