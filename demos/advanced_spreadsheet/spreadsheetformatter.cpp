/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "spreadsheetformatter.h"


SpreadsheetFormatter::SpreadsheetFormatter()
{
}

QString SpreadsheetFormatter::format(const QicsDataItem &itm) const
{
    switch (itm.type())
    {
    case QicsDataItem_Int:
        return itm.format("$ %d.00");
    case QicsDataItem_Float:
    case QicsDataItem_Double:
        return itm.format("$ %1.2f");
    case QicsDataItem_String: {
        bool ok = false;
        double d;
        QicsDataDouble dd(d = itm.string().toDouble(&ok));
        if(!ok)
            break;
        return "$ " + QString::number(d, 'f', 2);
                              }
    }

    return itm.string();
}


