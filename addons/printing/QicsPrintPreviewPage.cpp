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

#include <QicsPrintPreviewPage.h>

#include <QPainter>
#include <QResizeEvent>
#include <QPrinter>

#include <QicsPrintPreviewWidget.h>


QicsPrintPreviewPage::QicsPrintPreviewPage(int num, QicsPrintPreviewWidget *widget, QWidget *parent)
    : QFrame(parent),
        m_num(num),
        m_selected(false),
        m_enabled(true),
        m_widget(widget)
{
    reinit();
}

QicsPrintPreviewPage::~QicsPrintPreviewPage()
{
    disconnect();
}

void QicsPrintPreviewPage::reinit()
{
    m_realPageRect = m_widget->printer()->pageRect();
    m_realPaperRect = m_widget->printer()->paperRect();

    double res = 3.78;
    mmWidth = ((double)m_realPaperRect.width()) / res;
    mmHeight = ((double)m_realPaperRect.height()) / res;
}

bool QicsPrintPreviewPage::isActive() const
{
    return (m_widget->activePage() == this);
}

void QicsPrintPreviewPage::paintEvent ( QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(Qt::white);

    // background
    QRect r(rect().adjusted(0,0,-1,-1));
    p.drawRect(r);

    p.end();

    // page content
    emit drawPage(this);

    p.begin(this);

    // paper rect
    p.setPen(Qt::DashLine);
    p.setBrush(Qt::NoBrush);
    p.drawRect(m_pageRect);

    // disable mark
    p.setPen(Qt::SolidLine);
    if (!isEnabled()) {
        p.setPen(Qt::red);
        p.drawLine(r.topLeft(), r.bottomRight());
        p.drawLine(r.topRight(), r.bottomLeft());
    }

    // page frame
    if (isActive())
        p.setPen(QPen(Qt::green, 5));
    else if (isSelected())
        p.setPen(QPen(Qt::blue, 5));
    else
        p.setPen(Qt::black);

    p.drawRect(r);

    // page number
    if (m_widget->isPageNumbersVisible()) {
        r = QRect(width()-34, height()-34, 32, 32);
        p.setPen(QColor(0,0,100,100));
        p.setBrush(QColor(0,0,200,100));
        p.drawEllipse(r);
        p.setPen(Qt::white);
        p.drawText(r, Qt::AlignCenter, QString::number(m_num+1));
    }

    p.end();
}

void QicsPrintPreviewPage::resizeEvent ( QResizeEvent *event )
{
    double delta = (double)event->size().width() / (double)m_realPaperRect.width();

    m_pageRect.setX(int(m_realPageRect.x() * delta));
    m_pageRect.setY(int(m_realPageRect.y() * delta));
    m_pageRect.setWidth(int(m_realPageRect.width() * delta));
    m_pageRect.setHeight(int(m_realPageRect.height() * delta));

    QFrame::resizeEvent(event);
}

void QicsPrintPreviewPage::setSelected(bool yes)
{
    if (yes == m_selected) return;
    m_selected = yes;
    update();
}

void QicsPrintPreviewPage::setEnabled(bool yes)
{
    if (yes == m_enabled) return;
    m_enabled = yes;
    update();
}


