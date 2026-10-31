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

#include "../core/objects/Marker.h"

std::vector<MarkerInstance> UCDP::GetConflictPoints(const char* jsonPath) {

    QFile file(jsonPath);
    std::vector<MarkerInstance> points;

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file";
        return std::vector<MarkerInstance>();
    }

    float ucdpMaxYear = 2025;
    float ucdpMinYear = 1989;

    float delta = ucdpMaxYear - ucdpMinYear;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    QJsonArray events = doc.array();

    for (const QJsonValue& value : events) {
        QJsonObject event = value.toObject();

        int id = event["id"].toInt();
        int year = event["year"].toInt();

        float yr = (static_cast<float>(year) - ucdpMinYear) / delta;

        QString country = event["country"].toString();
        QString conflict = event["conflict_name"].toString();

        double latitude = event["latitude"].toDouble();
        double longitude = event["longitude"].toDouble();

        float latRad = ToRadians(latitude);
        float longRad = ToRadians(longitude);
        glm::vec2 mercator = ToMercator(latRad, longRad);

        MarkerInstance instance{};
        instance.position = glm::vec3(mercator.x, mercator.y, 0) * 10.0f;
        instance.yearNormalized = yr;

        points.push_back(instance);
    }

    return points;
}
