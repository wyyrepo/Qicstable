/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "QicsFilterCellDisplay.h"

#include <QStyleOptionComboBox>
#include <QApplication>
#include <QStyle>

#include <QicsScreenGrid.h>
#include <QicsColumnHeader.h>
#include <QicsCell.h>
#include <QicsTreeTable.h>
#include <QicsRegexpFilterDelegate.h>
#include <QicsListFilterDelegate.h>

#include "QicsFilterSortDialog.h"


QicsFilterCellDisplay::QicsFilterCellDisplay(QicsTreeTable *table)
    : QicsTextCellDisplay(table),
        m_over(false), m_down(false),
        m_current(QicsRegion()),
        m_table(table)
{
    m_fsd = new QicsFilterSortDialog(table);

    connect(m_table, SIGNAL(filterChanged(int)), this, SLOT(onFilterChanged(int)));
}

QicsFilterCellDisplay::~QicsFilterCellDisplay()
{
}

void QicsFilterCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                        const QicsDataItem *itm,
                                        QRect &rect, QPainter *painter)
{
    QicsTextCellDisplay::displayCell(grid, row, col, itm, rect, painter);

    // skip special column
    if (!col && !(m_table->treeInHeader())) return;

    // drop down button
    QStyleOptionComboBox co;
    co.state = QStyle::State_Active | QStyle::State_Enabled;
    co.rect = rect.adjusted(0,1,-1,-1);
    co.rect.setLeft(co.rect.right()-14);
    co.subControls = co.activeSubControls = QStyle::SC_ComboBoxArrow;

    if (m_current.topLeft() == QPoint(col, row))
        if (m_over) {
            co.state |= QStyle::State_MouseOver;
            if (m_down)
                co.state |= QStyle::State_Sunken;
        }

        qApp->style()->drawComplexControl(QStyle::CC_ComboBox, &co, painter);
}

bool QicsFilterCellDisplay::handleMouseEvent(QicsScreenGrid *grid, int row, int col, QMouseEvent *me)
{
    if (!col && !(m_table->treeInHeader()))
        return true;

    bool handled = false;

    m_over = false;
    m_down = false;

    if (m_current.isValid())
        grid->redraw(m_current);

    QRect rect = grid->cellDimensions(row, col, true);
    rect.setLeft(rect.right()-14);
    bool fit = rect.intersects(QRect(me->x(), me->y(), 1, 1));

    if (!fit)
        m_current = QicsRegion();
    else {
        if (me->type() == QEvent::MouseMove) {
            m_over = true;
            handled = true;
        }

        if (me->type() == QEvent::MouseButtonPress) {
            m_over = true;
            m_down = true;
            handled = true;
        }

        if (handled) {
            m_current = QicsRegion(row, col);
            grid->redraw(m_current);

            if (m_down) {
                QPoint d(me->globalPos() - me->pos());
                QPoint p(rect.bottomRight() + d);
                m_fsd->show(p, grid->gridInfo().modelColumnIndex(col));

                m_over = m_down = false;
                grid->redraw(m_current);
            }

            return true;
        }
    }

    return QicsTextCellDisplay::handleMouseEvent(grid, row, col, me);
}

void QicsFilterCellDisplay::onFilterChanged(int index)
{
    Q_UNUSED(index);
    QicsGridInfo &gi = m_table->gridInfo();
    int end = m_table->viewModel()->numColumns();

    for (int i = 0; i < end; ++i) {
        m_table->columnHeaderRef().cellRef(0, i).setPixmap(m_table->hasRowFilter(gi.modelColumnIndex(i)) ?
            QPixmap(":/Resources/filter.png") : QPixmap());
    }
}


