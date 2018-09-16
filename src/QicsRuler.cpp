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

#include <QicsRuler.h>

#include <QPainter>


QicsRuler::QicsRuler(Qt::Orientation orient, QWidget *parent)
    : QFrame(parent), m_orient(orient)
{
    setFrameStyle(QFrame::Box | QFrame::Plain);
    hide();
}

QicsRuler::~QicsRuler()
{
}

void QicsRuler::setOrientation(Qt::Orientation orient)
{
    if (m_orient != orient) {
        resize(height(), width());
        m_orient = orient;
        update();
    }
}

void QicsRuler::setMetrix(double mm)
{
    m_mm = mm;
    update();
}

void QicsRuler::paintEvent ( QPaintEvent *event )
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(QColor(0xfaffff));

    // background
    QRect r(rect().adjusted(0,0,-1,-1));
    p.drawRect(r);

    // 10 mm marks
    double i10 = 0;
    // 5 mm marks
    double i5 = 5;

    int cnt = 1;
    int h = height(), w = width();

    if (m_orient == Qt::Horizontal) {
        double coeff = (double)w / m_mm;

        while (i5 < m_mm) {
            // 10 mm
            int x = int(i10 * w / m_mm);
            p.drawLine(x,h,x,h-5);

            if (coeff >= 1.5 || (coeff >= 0.7 && !(cnt % 2)) || (coeff >= 0.3 && !(cnt % 4)) )
                p.drawText(x-15,0,32,h,Qt::AlignHCenter, QString::number(i10/10));

            // 5 mm
            if (coeff >= 1) {
                x = int(i5 * w / m_mm);
                p.drawLine(x,h,x,h-8);
            }

            // 1 mm
            if (coeff >= 2) {
                for (int i = 1; i < 10; ++i) {
                    x = int((double)(i10+i) * w / m_mm);
                    p.drawLine(x,h,x,h-3);
                }
            }

            i10 += 10;
            i5 += 10;
            cnt++;
        }

    }
    else {
        double coeff = (double)h / m_mm;

        while (i5 < m_mm) {
            // 10 mm
            int x = int(i10 * h / m_mm);
            p.drawLine(w,x,w-5,x);

            if (coeff >= 1.5 || (coeff >= 0.7 && !(cnt % 2)) || (coeff >= 0.3 && !(cnt % 4)) ) {
                p.rotate(-90);
                p.drawText(int(-(i10 * h / m_mm + 15)),0,32,w,Qt::AlignHCenter, QString::number(i10/10));
                p.rotate(90);
            }

            // 5 mm
            if (coeff >= 1) {
                x = int( i5 * h / m_mm );
                p.drawLine(w,x,w-8,x);
            }

            // 1 mm
            if (coeff >= 2) {
                for (int i = 1; i < 10; ++i) {
                    x = int( (double)(i10+i) * h / m_mm );
                    p.drawLine(w,x,w-3,x);
                }
            }

            i10 += 10;
            i5 += 10;
            cnt++;
        }
    }
}


