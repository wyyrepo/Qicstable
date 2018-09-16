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

#include "QicsDataItemFormatter.h"


QicsDataItemFormatter::QicsDataItemFormatter(QObject *parent)
    : QObject(parent)
{
}

QicsDataItemFormatter::~QicsDataItemFormatter()
{
}

////////////////////////////////////////////////////////////////////////

QicsDataItemSprintfFormatter::QicsDataItemSprintfFormatter(QObject *parent)
    : QicsDataItemFormatter(parent)
{
}

QicsDataItemSprintfFormatter::~QicsDataItemSprintfFormatter()
{
}

QString QicsDataItemSprintfFormatter::format(const QicsDataItem &itm) const
{
    const char *format = myFormats.value(itm.type());
    if ( format )
        return itm.format(format);

    return itm.string();
}

void QicsDataItemSprintfFormatter::addFormatString(QicsDataItemType type,
                                              const char *format_string)
{
    myFormats.insert(type, format_string);
}

void QicsDataItemSprintfFormatter::removeFormatString(QicsDataItemType type)
{
    myFormats.remove(type);
}


