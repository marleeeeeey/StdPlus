/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/QtPlus
*/

#pragma once

#include <QPainter>

namespace qtplus
{
    enum 
    {
        e_qtStdRotate = 90,
    };

    inline void drawArc(QPainter * painter, const QRectF &rect, float startAngle_deg, int endAngle_deg)
    {
        enum
        {
            e_qtArcMult   =  16,
        };

        startAngle_deg -= e_qtStdRotate;
        endAngle_deg   -= e_qtStdRotate;
        startAngle_deg *= -1;
        endAngle_deg   *= -1;
        float spanAngle_deg = endAngle_deg - startAngle_deg;
        spanAngle_deg = fmod(spanAngle_deg, 360);

        int startAngle_unit = startAngle_deg * e_qtArcMult;
        int spanAngle_unit  = spanAngle_deg * e_qtArcMult;

        painter->drawArc(rect, startAngle_unit, spanAngle_unit);
    }

    inline QRectF radiusRect(float radius)
    {
        QRectF rect(-radius, -radius, radius * 2, radius * 2);
        return rect;
    }

    inline QLineF fromPolar(float length, float angle_deg)
    {
        angle_deg -= e_qtStdRotate;
        angle_deg *= -1;
        return QLineF::fromPolar(length, angle_deg);
    }
}

