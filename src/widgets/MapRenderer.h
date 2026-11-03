//
// Created by Dmitri on 2026-10-30.
//

#ifndef CONFLICT_MAP_MAPRENDERER_H
#define CONFLICT_MAP_MAPRENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPointF>

#include "../core/framebuffer/ColorFramebuffer.h"
#include "../core/framebuffer/Quad.h"
#include "../core/objects/Borders.h"
#include "../core/objects/Camera.h"
#include "../core/objects/Marker.h"
#include "../core/shader/Shader.h"

class MapRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    MapRenderer();

    void SetACLEDVisibility(bool visible);
    void SetUCDPVisibility(bool visible);

    int GetUCDPMarkerCount();
    int GetACLEDMarkerCount();

    void SetMaxYearFilter(int year);
    void SetMinYearFilter(int year);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:

    Borders internationalBorders, administrativeBorders, disputedBorders;
    Quad composite;

    Marker ucdpMarker, acledMarker;

    Shader borderShader, compositeShader, markerShader;

    Camera camera;
    float aspectRatio = 1;
    float debugRotation = 0.0f;

    int maxYearFilter = 2026;
    int minYearFilter = 1989;

    bool tooltipFrozen, useACLEDData = true, useUCDPData = true;
    QString currentTooltip;

    std::vector<HoveredMarker> hoveredMarkers;

    glm::mat4 earthTilt;

    void RenderBorders();
    void RenderConflictMarkers();
    void RenderComposite();

    QPointF ProjectToScreen(const glm::vec3& position, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
    void UpdateHoveredMarkers(const QPoint& mousePosition);
    QString BuildTooltipText() const;

    QPoint lastMousePosition;

    ColorFramebuffer administrativeBorderColorFramebuffer,
                     internationalBorderColorFramebuffer,
                     disputedBorderColorFramebuffer,
                     countryPolygonsColorFramebuffer,
                     ucdpMarkersColorFramebuffer,
                     acledMarkerColorFramebuffer;
};


#endif //CONFLICT_MAP_MAPRENDERER_H
