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

#include "QicsRepeatingColumn.h"

#include "QicsStyleManager.h"
#include "QicsDimensionManager.h"
#include "QicsUtil.h"
#include "QicsTable.h"


QicsRepeatingColumn::QicsRepeatingColumn(int start_col, unsigned int interval,
                                         QicsTable *table)
    : QicsCellCommon(&(table->gridInfo()), table),
        myStartColumn(start_col),
        myInterval(interval)
{
    init(start_col, interval);
}

QicsRepeatingColumn::QicsRepeatingColumn(int start_col, unsigned int interval,
                                         QicsGridInfo *info,
                                         QObject *parent)
    : QicsCellCommon(info, parent)
{
    init(start_col, interval);
}

void QicsRepeatingColumn::init(int start_col, unsigned int interval)
{
    if (start_col < 0)
        start_col = 0;

    if (interval <= 0)
        interval = 1;

    myStartColumn = start_col;
    myInterval = interval;
}

////////////////////////////////////////////////////////////////////////
////////////////////   Attribute methods   /////////////////////////////
////////////////////////////////////////////////////////////////////////

void QicsRepeatingColumn::setAttr(QicsCellStyle::QicsCellStyleProperty attr,
                             const void *val)
{
    styleManager().setRepeatingColumnProperty(myStartColumn, myInterval,
        attr, val);
}

void *QicsRepeatingColumn::getAttr(QicsCellStyle::QicsCellStyleProperty attr) const
{
    return (styleManager().getRepeatingColumnProperty(myStartColumn, myInterval, attr));
}

void QicsRepeatingColumn::clearAttr(QicsCellStyle::QicsCellStyleProperty attr)
{
    styleManager().clearRepeatingColumnProperty(myStartColumn, myInterval, attr);
}

void QicsRepeatingColumn::setDMMargin(int margin)
{
    dimensionManager().setRepeatingColumnMargin(m_info->gridType(),
        myStartColumn, myInterval, margin);
}

void QicsRepeatingColumn::setDMFont(const QFont &font)
{
    dimensionManager().setRepeatingColumnFont(m_info->gridType(),
        myStartColumn, myInterval, font);
}

void QicsRepeatingColumn::setDMBorderWidth(int bw)
{
    dimensionManager().setRepeatingColumnBorderWidth(m_info->gridType(),
        myStartColumn, myInterval, bw);
}

int QicsRepeatingColumn::widthInPixels() const
{
    return (dimensionManager().columnWidth(myStartColumn));
}

void QicsRepeatingColumn::setWidthInPixels(int width)
{
    dimensionManager().setRepeatingColumnWidthInPixels(myStartColumn,
        myInterval, width);
}

int QicsRepeatingColumn::widthInChars() const
{
    // divide the width in pixels by the width of the font -- round down.
    return (dimensionManager().columnWidth(myStartColumn)  /
        qicsWidthOfFont(font()));
}

void QicsRepeatingColumn::setWidthInChars(int width)
{
    dimensionManager().setColumnWidthInChars(myStartColumn, myInterval, width);
}


