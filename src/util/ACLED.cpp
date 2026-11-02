
#include "ACLED.h"

#include <iostream>
#include <algorithm>
#include <climits>

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
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

    // Read and parse JSON
    QByteArray json = file.readAll();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON parsing error:" << error.errorString();
        return points;
    }

    if (!doc.isObject()) {
        qDebug() << "Invalid ACLED JSON: expected an object";
        return points;
    }

    QJsonObject root = doc.object();

    if (!root["success"].toBool()) {
        qDebug() << "ACLED API request was unsuccessful";
        return points;
    }

    if (!root["data"].isArray()) {
        qDebug() << "Invalid ACLED JSON: expected data array";
        return points;
    }

    QJsonArray events = root["data"].toArray();

    int minYear = 2025;
    int maxYear = 2025;

    if (minYear == INT_MAX) {
        qDebug() << "No valid ACLED event dates";
        return points;
    }

    float delta = static_cast<float>(maxYear - minYear);

    for (const QJsonValue& value : events) {

        QJsonObject event = value.toObject();

        QDate date = QDate::fromString(
            event["event_date"].toString(),
            Qt::ISODate
        );

        if (!date.isValid())
            continue;

        bool latitudeOk = false;
        bool longitudeOk = false;

        double latitude = event["latitude"].toString()
                              .toDouble(&latitudeOk);

        double longitude = event["longitude"].toString()
                               .toDouble(&longitudeOk);

        if (!latitudeOk || !longitudeOk)
            continue;

        if (latitude < -90.0 || latitude > 90.0 || longitude < -180.0 || longitude > 180.0) {
            continue;
        }

        float yearNormalized = delta > 0.0f
            ? (maxYear - static_cast<float>(date.year())) / delta
            : 0.0f;

        float latRad = ToRadians(latitude);
        float longRad = ToRadians(longitude);

        glm::vec2 mercator = Project(latRad, longRad);

        MarkerInstance instance{};

        instance.position = glm::vec3(
            mercator.x,
            mercator.y,
            0.0f
        ) * 10.0f;

        instance.yearNormalized = yearNormalized;

        points.push_back(instance);
    }

    return points;
}
