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

#include "QicsRepeatingRow.h"

#include "QicsStyleManager.h"
#include "QicsDimensionManager.h"
#include "QicsUtil.h"
#include "QicsTable.h"



QicsRepeatingRow::QicsRepeatingRow(int start_row, unsigned int interval,
                                   QicsTable *table)
    : QicsCellCommon(&(table->gridInfo()), table),
        myStartRow(start_row),
        myInterval(interval)
{
    init(start_row, interval);
}

QicsRepeatingRow::QicsRepeatingRow(int start_row, unsigned int interval,
                                   QicsGridInfo *info, QObject *parent)
    : QicsCellCommon(info, parent)
{
    init(start_row, interval);
}

void QicsRepeatingRow::init(int start_row, unsigned int interval)
{
    if (start_row < 0)
        start_row = 0;

    if (interval <= 0)
        interval = 1;

    myStartRow = start_row;
    myInterval = interval;
}

////////////////////////////////////////////////////////////////////////
////////////////////   Attribute methods   /////////////////////////////
////////////////////////////////////////////////////////////////////////

void QicsRepeatingRow::setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val)
{
    styleManager().setRepeatingRowProperty(myStartRow, myInterval, attr, val);
}

void *QicsRepeatingRow::getAttr(QicsCellStyle::QicsCellStyleProperty attr) const
{
    return (styleManager().getRepeatingRowProperty(myStartRow, myInterval, attr));
}

void QicsRepeatingRow::clearAttr(QicsCellStyle::QicsCellStyleProperty attr)
{
    styleManager().clearRepeatingRowProperty(myStartRow, myInterval, attr);
}

void QicsRepeatingRow::setDMMargin(int margin)
{
    dimensionManager().setRepeatingRowMargin(m_info->gridType(), myStartRow,
        myInterval, margin);
}

void QicsRepeatingRow::setDMFont(const QFont &font)
{
    dimensionManager().setRepeatingRowFont(m_info->gridType(), myStartRow,
        myInterval, font);
}

void QicsRepeatingRow::setDMBorderWidth(int bw)
{
    dimensionManager().setRepeatingRowBorderWidth(m_info->gridType(), myStartRow,
        myInterval, bw);
}

int QicsRepeatingRow::heightInPixels() const
{
    return (dimensionManager().rowHeight(myStartRow));
}

void QicsRepeatingRow::setHeightInPixels(int height)
{
    dimensionManager().setRepeatingRowHeightInPixels(myStartRow, myInterval,
        height);
}

int QicsRepeatingRow::heightInChars() const
{
    // divide the height in pixels by the height of the font -- round down.
    return (dimensionManager().rowHeight(myStartRow)  /
        qicsHeightOfFont(font()));
}

void QicsRepeatingRow::setHeightInChars(int height)
{
    dimensionManager().setRowHeightInChars(myStartRow, myInterval, height);
}


