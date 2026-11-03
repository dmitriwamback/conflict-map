//
// Created by Dmitri on 2026-11-03.
//

#ifndef CONFLICT_MAP_DATERANGEWIDGET_H
#define CONFLICT_MAP_DATERANGEWIDGET_H


#include <QWidget>

class QMouseEvent;
class QPaintEvent;

class DateRangeWidget : public QWidget {
    Q_OBJECT

public:
    explicit DateRangeWidget(QWidget* parent = nullptr);

    QSize sizeHint() const override;

    signals:
        void MinYearChanged(int year);
    void MaxYearChanged(int year);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    static constexpr int MIN_YEAR = 1989;
    static constexpr int MAX_YEAR = 2026;

    int minSelectedYear = MIN_YEAR;
    int maxSelectedYear = MAX_YEAR;

    enum class Handle { None, Minimum, Maximum };
    Handle activeHandle = Handle::None;

    int YearFromPosition(int x) const;
    int PositionFromYear(int year) const;
    void UpdateHandle(int x);
};



#endif //CONFLICT_MAP_DATERANGEWIDGET_H
