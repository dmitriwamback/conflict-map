//
// Created by Dmitri on 2026-10-30.
//

#ifndef CONFLICT_MAP_MAPRENDERER_H
#define CONFLICT_MAP_MAPRENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "../core/framebuffer/ColorFramebuffer.h"
#include "../core/framebuffer/Quad.h"
#include "../core/objects/Borders.h"
#include "../core/objects/Camera.h"
#include "../core/shader/Shader.h"

class MapRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    MapRenderer();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:

    Borders internationalBorders, administrativeBorders;
    Quad composite;

    Shader borderShader, compositeShader;

    Camera camera;
    float aspectRatio = 1;
    float debugRotation = 0.0f;

    glm::mat4 earthTilt;

    void RenderBorders();
    void RenderScene();
    void RenderCountryPolygons();
    void RenderComposite();

    QPoint lastMousePosition;

    ColorFramebuffer administrativeBorderColorFramebuffer, internationalBorderColorFramebuffer, countryPolygonsColorFramebuffer;
};


#endif //CONFLICT_MAP_MAPRENDERER_H
