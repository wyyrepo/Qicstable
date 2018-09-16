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

#include <QicsMainGrid.h>

#include <QicsTable.h>


QicsMainGrid::QicsMainGrid(QicsTable *table)
    : QicsGridCommon(&(table->gridInfo()), table, true)
{
    initSignals();
}

QicsMainGrid::QicsMainGrid(QicsGridInfo *info, QObject *parent, bool forward_signals)
    : QicsGridCommon(info, parent, forward_signals)

{
    initSignals();
}

QicsMainGrid::~QicsMainGrid()
{
}


