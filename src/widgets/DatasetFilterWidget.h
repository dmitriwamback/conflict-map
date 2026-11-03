//
// Created by Dmitri on 2026-11-02.
//

#ifndef CONFLICT_MAP_DATASETFILTERWIDGET_H
#define CONFLICT_MAP_DATASETFILTERWIDGET_H

#include <QWidget>

class QCheckBox;

class DatasetFilterWidget : public QWidget {
    Q_OBJECT

public:
    explicit DatasetFilterWidget(QWidget* parent = nullptr);

signals:
    void UCDPVisibilityChanged(bool visible);
    void ACLEDVisibilityChanged(bool visible);

private:
    QCheckBox* ucdpCheckBox;
    QCheckBox* acledCheckBox;
};

#endif //CONFLICT_MAP_DATASETFILTERWIDGET_H
