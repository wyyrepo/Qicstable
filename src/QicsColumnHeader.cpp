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

#include "QicsColumnHeader.h"

#include "QicsDataModel.h"
#include "QicsTable.h"



QicsColumnHeader::QicsColumnHeader(QicsTable *table)
    : QicsHeader(&(table->chGridInfo()), table, true)
{
}

QicsColumnHeader::QicsColumnHeader(QicsGridInfo *info, QObject *parent,
                                   bool forward_signals)
    : QicsHeader(info, parent, forward_signals)
{
}

QicsColumnHeader::~QicsColumnHeader()
{
}

int QicsColumnHeader::numRows() const
{
    if (!dataModel()) return 0;
    return dataModel()->numRows();
}

void QicsColumnHeader::setNumRows(int num)
{
    if (!dataModel()) return;

    int last = dataModel()->lastRow();

    if (num > (last + 1))
        dataModel()->addRows(num-last-1);
    else if (num <= last)
        dataModel()->deleteRows(last-num+1, num);
}

void QicsColumnHeader::setDefaultColumnWidthInPixels(int width)
{
    dimensionManager().setDefaultColumnWidthInPixels(width);
}

void QicsColumnHeader::setDefaultColumnWidthInChars(int width)
{
    dimensionManager().setDefaultColumnWidthInChars(width);
}

void QicsColumnHeader::setDefaultRowHeightInPixels(int height)
{
    dimensionManager().setDefaultRowHeightInPixels(height);
}

void QicsColumnHeader::setDefaultRowHeightInChars(int height)
{
    dimensionManager().setDefaultColumnWidthInPixels(height);
}


