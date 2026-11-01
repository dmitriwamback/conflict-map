//
// Created by Dmitri on 2026-10-30.
//

#include "ACLED.h"

#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QDate>
#include "Math.h"

#include "../core/objects/Marker.h"

std::vector<MarkerInstance> ACLED::GetConflictPoints(const char* jsonPath) {

    QFile file(jsonPath);
    std::vector<MarkerInstance> points;

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open ACLED file";
        return points;
    }

    float acledMaxYear = 2025.0f;
    float acledMinYear = 1989.0f;
    float delta = acledMaxYear - acledMinYear;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    if (!doc.isArray()) {
        qDebug() << "Invalid ACLED JSON: expected an array";
        return points;
    }

    QJsonArray events = doc.array();

    for (const QJsonValue& value : events) {
        QJsonObject event = value.toObject();

        QString week = event["week"].toString();
        QDate date = QDate::fromString(week, Qt::ISODate);

        if (!date.isValid()) {
            continue;
        }

        int year = date.year();

        float yr = (acledMaxYear - static_cast<float>(year)) / delta;

        double latitude = event["centroid_latitude"].toDouble();
        double longitude = event["centroid_longitude"].toDouble();

        float latRad = ToRadians(latitude);
        float longRad = ToRadians(longitude);

        glm::vec2 mercator = Project(latRad, longRad);

        MarkerInstance instance{};

        instance.position = glm::vec3(mercator.x, mercator.y, 0.0f) * 10.0f;

        instance.yearNormalized = yr;

        points.push_back(instance);
    }

    std::cout << points.size() << std::endl;

    return points;
}
