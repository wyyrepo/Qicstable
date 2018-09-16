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

#include "QicsRowHeader.h"

#include "QicsDataModel.h"
#include "QicsTable.h"


QicsRowHeader::QicsRowHeader(QicsTable *table)
    : QicsHeader(&(table->rhGridInfo()), table, true)
{
}

QicsRowHeader::QicsRowHeader(QicsGridInfo *info, QObject *parent,
                             bool forward_signals)
    : QicsHeader(info, parent, forward_signals)
{
}

QicsRowHeader::~QicsRowHeader()
{
}

int QicsRowHeader::numColumns() const
{
    if (!dataModel()) return 0;
    return dataModel()->numColumns();
}

void QicsRowHeader::setNumColumns(int num)
{
    if (!dataModel()) return;

    int last = dataModel()->lastColumn();

    if (num > (last + 1))
        dataModel()->addColumns(num-last-1);
    else if (num <= last)
        dataModel()->deleteColumns(last-num+1, num);
}

void QicsRowHeader::setDefaultRowHeightInPixels(int height)
{
    dimensionManager().setDefaultRowHeightInPixels(height);
}

void QicsRowHeader::setDefaultRowHeightInChars(int height)
{
    dimensionManager().setDefaultRowHeightInChars(height);
}

void QicsRowHeader::setDefaultColumnWidthInPixels(int width)
{
    dimensionManager().setDefaultColumnWidthInPixels(width);
}

void QicsRowHeader::setDefaultColumnWidthInChars(int width)
{
    dimensionManager().setDefaultColumnWidthInChars(width);
}


