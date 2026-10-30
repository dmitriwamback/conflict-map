#include <iostream>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>

#include "src/widgets/MapRenderer.h"

int main(int argc, char *argv[]) {
    QSurfaceFormat format;

    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setSamples(4);

    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);
    QMainWindow window;

    window.setWindowTitle("Conflict Map");
    window.resize(1200, 800);

    auto* mapViewer = new MapRenderer();
    auto* centralWidget = new QWidget();
    auto* layout = new QHBoxLayout(centralWidget);

    mapViewer->resize(1200, 800);
    layout->addWidget(mapViewer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    window.setCentralWidget(centralWidget);

    window.show();

    return app.exec();
}
