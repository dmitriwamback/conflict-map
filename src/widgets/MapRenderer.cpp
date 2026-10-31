//
// Created by Dmitri on 2026-10-30.
//

#include "MapRenderer.h"

#include <iostream>
#include <string>
#include <glm/ext/matrix_transform.hpp>

#include "../core/shader/ShaderSources.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>

#include "../util/UCDP.h"

MapRenderer::MapRenderer() {
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

    administrativeBorderColorFramebuffer.Initialize();
    internationalBorderColorFramebuffer.Initialize();
    acledMarkerColorFramebuffer.Initialize();

    administrativeBorders = Borders();
    administrativeBorders.Load("/Users/dmitri/Documents/working/conflict map/res/admin1.geojson");
    administrativeBorders.Initialize();

    internationalBorders = Borders();
    internationalBorders.Load("/Users/dmitri/Documents/working/conflict map/res/world.geojson");
    internationalBorders.Initialize();

    composite = Quad();
    composite.Initialize();

    defaultMarker = Marker();
    defaultMarker.Initialize();

    std::vector<MarkerInstance> markers;

    for (int i = 0; i < 17; i++) {

        std::string path = "/Users/dmitri/Documents/working/conflict map/res/ucdp/GEDEvent_v26_1_" + std::to_string(i+1) +".json";

        std::vector<glm::vec3> coordinates = UCDP::GetConflictPoints(path.c_str());

        for (glm::vec3 m : coordinates) {
            MarkerInstance mi{};
            mi.position = m;
            markers.push_back(mi);
        }
    }

    defaultMarker.SetInstances(markers);

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
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);

    administrativeBorderColorFramebuffer.Update(width, height);
    internationalBorderColorFramebuffer.Update(width, height);
    acledMarkerColorFramebuffer.Update(width, height);
}

void MapRenderer::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->position().toPoint();
    }
}

void MapRenderer::mouseMoveEvent(QMouseEvent *event) {
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

    administrativeBorderColorFramebuffer.Bind();

    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    administrativeBorders.Render();

    administrativeBorderColorFramebuffer.Unbind();



    internationalBorderColorFramebuffer.Bind();

    glViewport(0, 0, width(), height());
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

    acledMarkerColorFramebuffer.Bind();

    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    defaultMarker.Render();

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
    glBindTexture(GL_TEXTURE_2D, acledMarkerColorFramebuffer.GetTextureID());

    compositeShader.SetInt("adminBorderTexture", 0);
    compositeShader.SetInt("internBorderTexture", 1);
    compositeShader.SetInt("countryPolygonsTexture", 2);
    compositeShader.SetInt("acledMarkers", 3);

    composite.Render();
}


