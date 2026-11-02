//
// Created by Dmitri on 2026-10-30.
//

#include "MapRenderer.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>

#include "../core/shader/ShaderSources.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QToolTip>

#include "../util/ACLED.h"
#include "../util/UCDP.h"

MapRenderer::MapRenderer() {

    setMouseTracking(true);

    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MapRenderer::update));
    timer->start(16);
}

void MapRenderer::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    camera = Camera();
    administrativeBorderColorFramebuffer = ColorFramebuffer();
    internationalBorderColorFramebuffer = ColorFramebuffer();
    acledMarkerColorFramebuffer = ColorFramebuffer();
    ucdpMarkersColorFramebuffer = ColorFramebuffer();

    administrativeBorderColorFramebuffer.Initialize();
    internationalBorderColorFramebuffer.Initialize();
    acledMarkerColorFramebuffer.Initialize();
    ucdpMarkersColorFramebuffer.Initialize();

    administrativeBorders = Borders();
    administrativeBorders.Load("/Users/dmitri/Documents/working/conflict map/res/admin1.geojson");
    administrativeBorders.Initialize();

    internationalBorders = Borders();
    internationalBorders.Load("/Users/dmitri/Documents/working/conflict map/res/world.geojson");
    internationalBorders.Initialize();

    composite = Quad();
    composite.Initialize();

    ucdpMarker = Marker();
    ucdpMarker.Initialize();

    std::vector<MarkerInstance> markers;

    for (int i = 0; i < 17; i++) {

        std::string path = "/Users/dmitri/Documents/working/conflict map/res/ucdp/GEDEvent_v26_1_" + std::to_string(i+1) +".json";

        std::vector<MarkerInstance> coordinates = UCDP::GetConflictPoints(path.c_str());

        for (MarkerInstance m : coordinates) {
            markers.push_back(m);
        }
    }

    std::reverse(markers.begin(), markers.end());

    ucdpMarker.SetInstances(markers);

    acledMarker = Marker();
    acledMarker.Initialize();

    std::string path = "/Users/dmitri/Documents/working/conflict map/res/acled/ACLED Data_2026-10-31_event_date_from_2026-01-01_event_date_to_2026-10-25.json";

    std::vector<MarkerInstance> coordinates = ACLED::GetConflictPoints(path.c_str());
    std::reverse(coordinates.begin(), coordinates.end());

    acledMarker.SetInstances(coordinates);

    borderShader = Shader();
    borderShader.Create(BORDER_VERTEX_SHADER_SOURCE, BORDER_FRAGMENT_SHADER_SOURCE);

    compositeShader = Shader();
    compositeShader.Create(COMPOSITE_VERTEX_SHADER_SOURCE, COMPOSITE_FRAGMENT_SHADER_SOURCE);

    markerShader = Shader();
    markerShader.Create(MARKER_VERTEX_SHADER_SOURCE, MARKER_FRAGMENT_SHADER_SOURCE);
}

void MapRenderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    camera.Update();

    RenderBorders();
    RenderConflictMarkers();
    RenderComposite();
}

void MapRenderer::resizeGL(int width, int height) {

    const qreal dpr = devicePixelRatioF();

    int pixelWidth = qRound(width * dpr);
    int pixelHeight = qRound(height * dpr);


    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, pixelWidth, pixelHeight);

    administrativeBorderColorFramebuffer.Update(pixelWidth, pixelHeight);
    internationalBorderColorFramebuffer.Update(pixelWidth, pixelHeight);
    acledMarkerColorFramebuffer.Update(pixelWidth, pixelHeight);
    ucdpMarkersColorFramebuffer.Update(pixelWidth, pixelHeight);
}

void MapRenderer::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->position().toPoint();
    }
}

void MapRenderer::mouseMoveEvent(QMouseEvent *event) {

    if (camera.GetDistance() < 100) {
        //UpdateHoveredMarkers(event->pos());
    }

    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    QPoint currentMousePosition = event->position().toPoint();
    QPoint delta = currentMousePosition - lastMousePosition;

    lastMousePosition = currentMousePosition;

    float sensitivityMax = 0.00245f;
    float sensitivityMin = 0.0000065f;

    float dx = camera.ZOOM_MAX - camera.ZOOM_MIN;
    float dy = sensitivityMax - sensitivityMin;
    float slope = dy/dx;

    float b = sensitivityMax - slope * camera.ZOOM_MAX;

    float sensitivity = (camera.GetDistance() * slope + b);

    camera.Move(-delta.x() * sensitivity, delta.y() * sensitivity);
}

void MapRenderer::wheelEvent(QWheelEvent *event) {
    const float delta = static_cast<float>(event->angleDelta().y());
    camera.Zoom(delta);
    update();
}

void MapRenderer::RenderBorders() {
    glm::mat4 projectionMatrix = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 lookAtMatrix = camera.GetLookAtMatrix();
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(500.0f));

    borderShader.Bind();
    borderShader.SetMat4("projection", projectionMatrix);
    borderShader.SetMat4("lookAt", lookAtMatrix);
    borderShader.SetMat4("model", model);
    borderShader.SetVec3("cameraDirection", camera.GetDirection());

    int pixelWidth = qRound(width() * devicePixelRatioF());
    int pixelHeight = qRound(height() * devicePixelRatioF());

    administrativeBorderColorFramebuffer.Bind();

    glViewport(0, 0, pixelWidth, pixelHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    administrativeBorders.Render();

    administrativeBorderColorFramebuffer.Unbind();



    internationalBorderColorFramebuffer.Bind();

    glViewport(0, 0, pixelWidth, pixelHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    internationalBorders.Render();

    internationalBorderColorFramebuffer.Unbind();
}

void MapRenderer::RenderConflictMarkers() {

    glm::mat4 projectionMatrix = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 lookAtMatrix = camera.GetLookAtMatrix();
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));

    markerShader.Bind();
    markerShader.SetMat4("projection", projectionMatrix);
    markerShader.SetMat4("lookAt", lookAtMatrix);
    markerShader.SetMat4("model", model);
    markerShader.SetVec3("cameraDirection", camera.GetDirection());

    int pixelWidth = qRound(width() * devicePixelRatioF());
    int pixelHeight = qRound(height() * devicePixelRatioF());

    ucdpMarkersColorFramebuffer.Bind();

    glViewport(0, 0, pixelWidth, pixelHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ucdpMarker.Render();

    ucdpMarkersColorFramebuffer.Unbind();

    acledMarkerColorFramebuffer.Bind();

    glViewport(0, 0, pixelWidth, pixelHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    acledMarker.Render();

    acledMarkerColorFramebuffer.Unbind();
}

void MapRenderer::RenderComposite() {
    const qreal dpr = devicePixelRatioF();
    glViewport(0, 0, static_cast<GLsizei>(width() * dpr), static_cast<GLsizei>(height() * dpr));

    compositeShader.Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, administrativeBorderColorFramebuffer.GetTextureID());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, internationalBorderColorFramebuffer.GetTextureID());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, countryPolygonsColorFramebuffer.GetTextureID());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ucdpMarkersColorFramebuffer.GetTextureID());

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, acledMarkerColorFramebuffer.GetTextureID());

    compositeShader.SetInt("adminBorderTexture", 0);
    compositeShader.SetInt("internBorderTexture", 1);
    compositeShader.SetInt("countryPolygonsTexture", 2);
    compositeShader.SetInt("ucdpMarkers", 3);
    compositeShader.SetInt("acledMarkers", 4);

    composite.Render();
}

QPointF MapRenderer::ProjectToScreen(const glm::vec3& position, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) {
    glm::vec4 clip = projection * view * model * glm::vec4(position, 1.0f);

    if (clip.w <= 0.0f) return QPointF(-10000.0, -10000.0);

    glm::vec3 ndc = glm::vec3(clip) / clip.w;

    double screenX = (ndc.x + 1.0) * 0.5 * width();
    double screenY = (1.0 - ndc.y) * 0.5 * height();

    return QPointF(screenX, screenY);
}

void MapRenderer::UpdateHoveredMarkers(const QPoint& mousePosition) {

    if (tooltipFrozen) return;
    hoveredMarkers.clear();

    glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 view = camera.GetLookAtMatrix();
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));

    constexpr float hoverRadius = 12.0f;
    constexpr float hoverRadiusSquared = hoverRadius * hoverRadius;

    auto checkMarkers = [&](const std::vector<MarkerInstance>& instances, const QString& source) {

        for (size_t i = 0; i < instances.size(); i++) {
            const MarkerInstance& marker = instances[i];

            QPointF screenPosition = ProjectToScreen(
                marker.position,
                projection,
                view,
                model
            );

            float dx = static_cast<float>(
                screenPosition.x() - mousePosition.x()
            );

            float dy = static_cast<float>(
                screenPosition.y() - mousePosition.y()
            );

            float distanceSquared = dx * dx + dy * dy;

            if (distanceSquared <= hoverRadiusSquared && hoveredMarkers.size() < 5) {
                hoveredMarkers.push_back({
                    source,
                    i,
                    distanceSquared,
                    marker.yearNormalized
                });
            }
        }
    };

    checkMarkers(ucdpMarker.GetInstances(), "UCDP");
    checkMarkers(acledMarker.GetInstances(), "ACLED");

    // Closest marker appears first.
    std::sort(
        hoveredMarkers.begin(),
        hoveredMarkers.end(),
        [](const HoveredMarker& a, const HoveredMarker& b) {
            return a.distanceSquared < b.distanceSquared;
        }
    );

    if (hoveredMarkers.empty()) {
        currentTooltip.clear();
        QToolTip::hideText();
        return;
    }

    currentTooltip = BuildTooltipText();

    QToolTip::showText(
        mapToGlobal(mousePosition + QPoint(15, 15)),
        currentTooltip,
        this
    );
}

QString MapRenderer::BuildTooltipText() const {
    QString text;

    for (const HoveredMarker& marker : hoveredMarkers) {
        text += QString(
            "%1 | Marker #%2\n"
            "Normalized year: %3\n\n"
        )
        .arg(marker.source)
        .arg(marker.index)
        .arg(marker.yearNormalized, 0, 'f', 3);
    }

    return text.trimmed();
}