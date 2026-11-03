#include <iostream>

#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QWidget>

#include "src/widgets/MapRenderer.h"
#include "src/widgets/DatasetFilterWidget.h"

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
    auto* filterWidget = new DatasetFilterWidget();

    filterWidget->setStyleSheet(R"(
        DatasetFilterWidget {
            border-radius: 10px;
        }

        QLabel {
            color: white;
            font-size: 18px;
        }

        QCheckBox {
            color: white;
            font-size: 16px;
            spacing: 10px;
        }

        QCheckBox::indicator {
            width: 20px;
            height: 20px;
        }
    )");
    filterWidget->setFixedSize(250, 100);

    std::string ACLEDMarkerCount = std::to_string(mapViewer->GetACLEDMarkerCount());
    std::string UCDPMarkerCount = std::to_string(mapViewer->GetUCDPMarkerCount());

    filterWidget->SetACLEDCheckBoxText(QString::fromStdString("ACLED (" + ACLEDMarkerCount + " Events)"));
    filterWidget->SetUCDPCheckBoxText(QString::fromStdString("UCDP (" + UCDPMarkerCount + " Events)"));

    auto* centralWidget = new QWidget();
    auto* layout = new QGridLayout(centralWidget);

    layout->addWidget(mapViewer, 0, 0);
    layout->addWidget(filterWidget, 0, 0, Qt::AlignTop | Qt::AlignRight);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    window.setCentralWidget(centralWidget);

    QObject::connect(filterWidget, &DatasetFilterWidget::UCDPVisibilityChanged, mapViewer, &MapRenderer::SetUCDPVisibility);
    QObject::connect(filterWidget, &DatasetFilterWidget::ACLEDVisibilityChanged, mapViewer, &MapRenderer::SetACLEDVisibility);

    window.show();

    return app.exec();
}