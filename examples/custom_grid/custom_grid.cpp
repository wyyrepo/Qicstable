/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "custom_grid.h"


#include <QMouseEvent>
#include <QMenu>

// This is an example of a custom table grid.  This grid acts like
// the default QicsTableGrid, except that it posts a popup menu
// when the user presses the right mouse button.

///////////////////////////////////////////////////////////////////////////

CustomGrid::CustomGrid(QWidget *w, QicsGridInfo &info,
                       int top_row, int left_column)
    : QicsTableGrid(w, info, top_row, left_column)
{
    m_menu = new QMenu(this);
    m_menu->addAction(tr("Cut"), w, SLOT(cut()));
    m_menu->addAction(tr("Copy"), w, SLOT(copy()));
    m_menu->addAction(tr("Paste"), w, SLOT(paste()));
}

QicsTableGrid *CustomGrid::createGrid(QWidget *w, QicsGridInfo &info,
                                      int top_row, int left_column)
{
    return (new CustomGrid(w, info, top_row, left_column));
}

void CustomGrid::handleMousePressEvent(const QicsICell &cell, QMouseEvent *m)
{
    if (m->button() == Qt::RightButton)
        m_menu->popup(m->globalPos());
    else
        QicsTableGrid::handleMousePressEvent(cell, m);
}


