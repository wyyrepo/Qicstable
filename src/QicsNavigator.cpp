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

#include <QicsNavigator.h>

#include <QPainter>
#include <QMouseEvent>


QicsNavigator::QicsNavigator(QWidget *parent)
    : QToolButton(parent)
{
    m_nav = new QicsNavigatorFrame;
    m_nav->resize(200,100);

    connect(m_nav, SIGNAL(changed(const QRect &)), this, SIGNAL(changed(const QRect &)));
}

QicsNavigator::~QicsNavigator()
{
    delete m_nav;
    m_nav = 0;
}

void QicsNavigator::navigate(const QRect &total, const QRect &view)
{
    QPoint p = pos();
    p = parentWidget()->mapToGlobal(p);
    p -= QPoint(m_nav->size().width(), m_nav->size().height());
    p += QPoint(width(), height());
    m_nav->move(p);

    m_nav->setup(total, view);

    setDown(false);
}

void QicsNavigator::resizeWindow(const QSize &size)
{
    m_nav->resize(size);
}

void QicsNavigator::resizeWindow(int w, int h)
{
    m_nav->resize(w, h);
}

///////////////////////////////////////////////////////////////////////////
// QicsNavigatorFrame
///////////////////////////////////////////////////////////////////////////

QicsNavigatorFrame::QicsNavigatorFrame()
    : QFrame(0, Qt::Popup)
{
    setWindowModality(Qt::WindowModal);
    setFrameShape(QFrame::Box);
    setCursor(Qt::SizeAllCursor);
    setWindowOpacity(0.75);
}

QicsNavigatorFrame::~QicsNavigatorFrame()
{
}

void QicsNavigatorFrame::setup(const QRect &total, const QRect &view)
{
    m_total = total;
    m_view = view;

    show();
}

void QicsNavigatorFrame::paintEvent ( QPaintEvent * event )
{
    QFrame::paintEvent(event);

    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(Qt::white);

    QRect r(rect().adjusted(10,10,0,0));
    double wc = (double)r.width() / m_total.width();
    double hc = (double)r.height() / m_total.height();

    int dw = wc * m_view.width();
    m_label.setWidth(/*dw < 10 ? 10 : */dw+10);
    int dh = hc * m_view.height();
    m_label.setHeight(/*dh < 10 ? 10 : */dh+10);
    m_label.moveLeft(int(wc * m_view.x()));
    m_label.moveTop(int(hc * m_view.y()));

    if (m_label.right() >= r.right())
        m_label.setRight(r.right() - 1);
    if (m_label.bottom() >= r.bottom())
        m_label.setBottom(r.bottom() - 1);

    p.drawRect(m_label);
}

void QicsNavigatorFrame::mouseMoveEvent ( QMouseEvent * event )
{
    QRect r(rect().adjusted(10,10,0,0));
    double wc = (double)r.width() / m_total.width();
    double hc = (double)r.height() / m_total.height();

    m_view.moveLeft(int((double)event->pos().x() / wc));
    if (m_view.x() < 0)
        m_view.moveLeft(0);
    if (m_view.x() > m_total.width() - m_view.width())
        m_view.moveLeft(m_total.width() - m_view.width());

    m_view.moveTop(int((double)event->pos().y() / hc));
    if (m_view.y() < 0)
        m_view.moveTop(0);
    if (m_view.y() > m_total.height() - m_view.height())
        m_view.moveTop(m_total.height() - m_view.height());

    repaint();

    emit changed(m_view);
}

void QicsNavigatorFrame::mouseReleaseEvent ( QMouseEvent * )
{
    close();
}

void QicsNavigatorFrame::closeEvent ( QCloseEvent * )
{
    releaseMouse();
}


