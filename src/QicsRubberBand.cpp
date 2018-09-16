/*********************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#include <QicsRubberBand.h>

#include <QPainter>
#include <QPaintEvent>


QicsRubberBand::QicsRubberBand(Shape s, QWidget *p)
    : QRubberBand(s, p)
{
    m_dm = DM_CUSTOM;
    m_opacity = 0.5;
    m_brush = palette().background();
    m_pen = palette().base().color();
}

QicsRubberBand::~QicsRubberBand()
{
}

void QicsRubberBand::paintEvent(QPaintEvent *event)
{
    if (m_dm == DM_STANDARD) {
        QRubberBand::paintEvent(event);
        return;
    }

    QPainter p(this);
    p.setPen(m_pen);
    p.setBrush(m_brush);
    p.setOpacity(m_opacity);

    switch (shape()) {
        case QRubberBand::Line:
            p.drawLine(rect().topLeft(), rect().bottomRight());
            break;
        case QRubberBand::Rectangle:
            p.drawRect(rect());
            break;
        default:;
    }

    event->accept();
}

void QicsRubberBand::setOpacity(qreal op)
{
    if (op < 0) op = 0;
    if (op > 1) op = 1;
    m_opacity = op;
}


