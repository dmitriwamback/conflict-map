//
// Created by Dmitri on 2026-11-02.
//

#include "DatasetFilterWidget.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>

DatasetFilterWidget::DatasetFilterWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);

    auto* title = new QLabel("Dataset visibility", this);

    ucdpCheckBox = new QCheckBox("UCDP", this);
    acledCheckBox = new QCheckBox("ACLED", this);

    ucdpCheckBox->setChecked(true);
    acledCheckBox->setChecked(true);

    layout->addWidget(title);
    layout->addWidget(ucdpCheckBox);
    layout->addWidget(acledCheckBox);

    connect(ucdpCheckBox, &QCheckBox::toggled,
            this, &DatasetFilterWidget::UCDPVisibilityChanged);

    connect(acledCheckBox, &QCheckBox::toggled,
            this, &DatasetFilterWidget::ACLEDVisibilityChanged);
}

void DatasetFilterWidget::SetUCDPCheckBoxText(QString text) {
    ucdpCheckBox->setText(text);
}

void DatasetFilterWidget::SetACLEDCheckBoxText(QString text) {
    acledCheckBox->setText(text);
}