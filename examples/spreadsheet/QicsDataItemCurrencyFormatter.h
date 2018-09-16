/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef QICSDATAITEMCURRENCYFORMATTER_H
#define QICSDATAITEMCURRENCYFORMATTER_H

#include <QicsDataItemFormatter.h>

class QicsDataItemCurrencyFormatter : public QicsDataItemFormatter
{
public:
    /*!
    *  Constructs an empty formatter.
    */
    QicsDataItemCurrencyFormatter();

    virtual QString format(const QicsDataItem &itm) const;
};

#endif // QICSDATAITEMCURRENCYFORMATTER_H


