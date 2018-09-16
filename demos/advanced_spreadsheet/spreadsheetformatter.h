/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SPREADSHEETFORMATTER_H
#define SPREADSHEETFORMATTER_H

#include <QicsDataItemFormatter.h>

/*
* Class that can perform simple data formatting.
*/
class SpreadsheetFormatter : public QicsDataItemFormatter
{
public:
    SpreadsheetFormatter();

    virtual QString format(const QicsDataItem &itm) const;
};

#endif //SPREADSHEETFORMATTER_H


