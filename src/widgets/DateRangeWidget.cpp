//
// Created by Dmitri on 2026-11-03.
//

#include "DateRangeWidget.h"


#include "DateRangeWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <algorithm>
#include <cmath>

DateRangeWidget::DateRangeWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(230, 85);
    setMouseTracking(true);
}

QSize DateRangeWidget::sizeHint() const {
    return QSize(300, 110);
}

int DateRangeWidget::PositionFromYear(int year) const {
    constexpr int margin = 9;
    int usableWidth = width() - margin * 2;

    float t = float(year - MIN_YEAR) /
              float(MAX_YEAR - MIN_YEAR);

    return margin + int(t * usableWidth);
}

int DateRangeWidget::YearFromPosition(int x) const {
    constexpr int margin = 9;
    int usableWidth = width() - margin * 2;

    float t = float(x - margin) / float(usableWidth);
    t = std::clamp(t, 0.0f, 1.0f);

    return MIN_YEAR +
        int(std::round(t * (MAX_YEAR - MIN_YEAR)));
}

void DateRangeWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    constexpr int margin = 9;
    constexpr int trackY = 58;
    constexpr int handleRadius = 9;

    int left = margin;
    int right = width() - margin;

    int minX = PositionFromYear(minSelectedYear);
    int maxX = PositionFromYear(maxSelectedYear);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(75, 75, 75));
    painter.drawRoundedRect(QRect(left, trackY - 3, right - left, 6), 3, 3);

    painter.setBrush(QColor(65, 150, 255));
    painter.drawRoundedRect(QRect(minX, trackY - 3, maxX - minX, 6),3, 3);

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(15);
    painter.setFont(font);

    painter.drawText(QRect(0, 15, width(), 22), Qt::AlignLeft, QString("%1").arg(minSelectedYear));
    painter.drawText(QRect(0, 15, width(), 22), Qt::AlignRight, QString("%1").arg(maxSelectedYear));
    painter.setBrush(Qt::white);
    painter.drawEllipse(QPoint(minX, trackY), handleRadius, handleRadius);
    painter.drawEllipse(QPoint(maxX, trackY), handleRadius, handleRadius);
}

void DateRangeWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton)
        return;

    int x = event->pos().x();

    int minDistance = std::abs(x - PositionFromYear(minSelectedYear));
    int maxDistance = std::abs(x - PositionFromYear(maxSelectedYear));

    if (minDistance <= maxDistance)
        activeHandle = Handle::Minimum;
    else
        activeHandle = Handle::Maximum;

    UpdateHandle(x);
}

void DateRangeWidget::mouseMoveEvent(QMouseEvent* event) {
    if (activeHandle == Handle::None)
        return;

    UpdateHandle(event->pos().x());
}

void DateRangeWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        activeHandle = Handle::None;
}

void DateRangeWidget::UpdateHandle(int x) {
    int year = YearFromPosition(x);

    if (activeHandle == Handle::Minimum) {
        minSelectedYear = std::min(year, maxSelectedYear);
        emit MinYearChanged(minSelectedYear);
    }
    else if (activeHandle == Handle::Maximum) {
        maxSelectedYear = std::max(year, minSelectedYear);
        emit MaxYearChanged(maxSelectedYear);
    }

    update();
}
