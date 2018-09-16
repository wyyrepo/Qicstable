/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "QicsDataItemCurrencyFormatter.h"

//   This class is used to display a QicsDataItem in a specified format, ie
//   provide a "view" on the "model".  Currently a very simple class to
//   display US Dollars in "$ 12.34" style.

QicsDataItemCurrencyFormatter::QicsDataItemCurrencyFormatter()
{
}

QString QicsDataItemCurrencyFormatter::format(const QicsDataItem &itm) const
{
    switch (itm.type())
    {
    case QicsDataItem_Int: {
        //QicsDataInt* ptr = (QicsDataInt*) &itm;
        //if (ptr->data() < 0) {
        // Not quite right.  What we really want is to get rid of the
        // negative sign and replace it with the ( )

        //return itm.format("$ (%d.00)");
        //}

        return itm.format("$ %d.00");
                           }
                           break;
    case QicsDataItem_Float:
    case QicsDataItem_Double:
        return itm.format("$ %1.2f");
    default:
        break;
    }

    return itm.string();
}


