/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "kdchartobject.h"

#include <QMenu>
#include <QMouseEvent>
#include <QicsTable.h>
#include <QicsSelection.h>


KDChartObject::KDChartObject(QicsTable *_table, AbstractDiagram *_diagram, const QicsRegion &_region, const QicsRegion &_dataRegion)
    : QicsKDChartTableObject(_table, _diagram, _region, _dataRegion)
{
    m_menu = new QMenu();
    m_menu->addAction("Assign data from selection...", this, SLOT(assignData()));
}

KDChartObject::~KDChartObject()
{
    delete m_menu;
}

bool
KDChartObject::handleMouseEvent(QicsScreenGrid *, int, int, QMouseEvent *e)
{
    if (e->button() == Qt::RightButton) {
        if (e->type() == QEvent::MouseButtonRelease)
            m_menu->popup(e->globalPos());
        return true;
    }
    return false;
}

void
KDChartObject::assignData()
{
    QicsRegion r;
    QicsSelectionList* list = m_table->selectionList(true);
    if (!list || list->isEmpty())
        r = QicsRegion(m_table->currentCell()->rowIndex(), m_table->currentCell()->columnIndex());
    else
        r = list->first().toRegion();

    setDataRegion(r);
}


