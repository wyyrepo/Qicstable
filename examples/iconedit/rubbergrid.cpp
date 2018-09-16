/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "rubbergrid.h"


RubberGrid::RubberGrid(QWidget *w, QicsGridInfo &info,
                       int top_row, int left_column)
    : QicsTableGrid(w, info, top_row, left_column)
{
    m_me = 0;
}

QicsTableGrid *RubberGrid::createGrid(QWidget *w, QicsGridInfo &info,
                                      int top_row, int left_column)
{
    return (new RubberGrid(w, info, top_row, left_column));
}

void RubberGrid::mousePressEvent( QMouseEvent *m )
{
    // clone current event to store its params (i.e. buttons pressed, etc.)
    m_me = new QMouseEvent(m->type(), m->pos(), m->button(), m->buttons(), m->modifiers());
    QicsScreenGrid::mousePressEvent(m);
}

void RubberGrid::mouseReleaseEvent( QMouseEvent *m )
{
    if (m_me) {
        delete m_me;
        m_me = 0;
    }
    QicsScreenGrid::mouseReleaseEvent(m);
}

void RubberGrid::mouseMoveEvent( QMouseEvent *m )
{
    if (m_me) {
        // if in rubber mode, simulate press event with current(!) position
        QicsScreenGrid::mousePressEvent(new QMouseEvent(m_me->type(), m->pos(), m_me->button(), m_me->buttons(), m_me->modifiers()) );
    }
    else
        QicsScreenGrid::mouseMoveEvent(m);
}

// reimplement this to force enter work as "double click", and tab to leave focus
bool RubberGrid::handleTraversalKeys(QKeyEvent *ke)
{
    if (!currentCell().isValid())
        return false;

    switch (ke->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return: {
            QRect r = cellDimensions(currentCell(),false);
            QMouseEvent m(QEvent::MouseButtonDblClick, r.topLeft(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            mouseDoubleClickEvent(&m);
            break;
        }

    case Qt::Key_Tab:
    case Qt::Key_Backtab: {
            ke->ignore();
            break;
        }

    default:
        return QicsTableGrid::handleTraversalKeys(ke);
    }

    return true;
}


