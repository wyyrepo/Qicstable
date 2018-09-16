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

#include "QicsWidgetCellDisplay.h"

#include <QApplication>
#include <QLayout>
#include <QFocusEvent>
#include <QObjectCleanupHandler>

#include "QicsCell.h"
#include "QicsScreenGrid.h"
#include "QicsUtil.h"
#include "QicsCellDisplay_p.h"


QicsWidgetCellDisplay::QicsWidgetCellDisplay(QWidget *displayWidget, QObject *parent)
    : QObject(parent), QicsCellDisplay(), m_widget(0), m_objectCleanupHandler(0)
{
    setWidget(displayWidget);
}

QicsWidgetCellDisplay::~QicsWidgetCellDisplay()
{
    delete m_objectCleanupHandler;
}

void QicsWidgetCellDisplay::setWidget(QWidget *widget)
{
    if (!widget || m_widget == widget)
        return;

    if (m_objectCleanupHandler && m_widget)
        m_objectCleanupHandler->remove(m_widget);
    if (!m_objectCleanupHandler)
        m_objectCleanupHandler = new QObjectCleanupHandler();

    m_widget = widget;

    if (m_widget) {
        m_widget->installEventFilter(this);
        m_widget->hide();
        m_objectCleanupHandler->add(m_widget);
    }
}

void QicsWidgetCellDisplay::aboutToClear(QicsGridInfo *info, int row, int col)
{
    Q_UNUSED(info);
    Q_UNUSED(row);
    Q_UNUSED(col);

    if (m_widget)
        m_widget->hide();
}

void QicsWidgetCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                   const QicsDataItem *itm, QRect &rect,
                                   QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, m_widget, true, true);

    m_widget->setGeometry(d->cr);

    if (m_widget->geometry() != d->cr) {
        QLayout* l = m_widget->layout();
        if (l)
            l->setSizeConstraint(QLayout::SetNoConstraint);
        m_widget->setMinimumSize(d->cr.size());
        m_widget->setGeometry(d->cr);
    }

    if (d->for_printer) {
        // We are trying to print, so grab a pixmap of the widget
        // and draw it to the printer.
#if QT_VERSION < 0x050000
        QPixmap pix = QPixmap::grabWidget(m_widget);
#else
        QPixmap pix = m_widget->grab();
#endif
        painter->drawPixmap(d->cr, pix);
    }
    else {
        QicsScreenGrid *screenGrid = dynamic_cast<QicsScreenGrid *>(grid);
        Q_ASSERT(screenGrid);
        m_widget->setParent(screenGrid);
        m_widget->show();
#ifdef Q_WS_X11
        screenGrid->update();
#endif
    }
}

void QicsWidgetCellDisplay::startEdit(QicsScreenGrid *grid, int row, int col, const QicsDataItem *itm)
{
    Q_UNUSED(grid);
    Q_UNUSED(row);
    Q_UNUSED(col);
    Q_UNUSED(itm);
}

void QicsWidgetCellDisplay::moveEdit(QicsScreenGrid *grid, int row, int col, const QRect &rect)
{
    Q_UNUSED(grid);
    Q_UNUSED(row);
    Q_UNUSED(col);
    Q_UNUSED(rect);
}

void QicsWidgetCellDisplay::endEdit(QicsScreenGrid *grid, int row, int col)
{
    Q_UNUSED(row);
    Q_UNUSED(col);

    if (m_widget)
        m_widget->hide();

    // need to avoid flickers
    grid->update();
}

QSize QicsWidgetCellDisplay::sizeHint(QicsGrid *grid, int row, int col, const QicsDataItem *itm)
{
    Q_UNUSED(grid);
    Q_UNUSED(row);
    Q_UNUSED(col);
    Q_UNUSED(itm);

    return m_widget->sizeHint();
}

bool QicsWidgetCellDisplay::isEmpty(QicsGridInfo *grid, int row, int col, const QicsDataItem *itm) const
{
    Q_UNUSED(grid);
    Q_UNUSED(row);
    Q_UNUSED(col);
    Q_UNUSED(itm);

    return false;
}

bool QicsWidgetCellDisplay::eventFilter(QObject *watched, QEvent *event)
{
    if (!m_widget)
        return false;

    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid*>(m_widget->parentWidget());
    if (!grid)
        return false;

    switch(event->type())
    {
    case QEvent::FocusIn: {
            grid->traverseToCell(myCell->rowIndex(), myCell->columnIndex(), true);
            return false;
        }
    case QEvent::FocusOut: {
            QFocusEvent *fe = static_cast<QFocusEvent *>(event);
            myLastFocusReason = fe->reason();
            grid->uneditCurrentCell();
            return false;
        }
    case QEvent::Move: {
            m_widget->update();
            break;
        }
    default:
        break;
    }

    bool ret = QObject::eventFilter(watched, event);

    event->ignore();

    return ret;
}


