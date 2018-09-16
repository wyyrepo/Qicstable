/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef STOCKSORTERDELEGATE_H
#define STOCKSORTERDELEGATE_H

#include "QicsAbstractSorterDelegate.h"


class StockSorterDelegate : public QicsAbstractSorterDelegate
{
public:
    StockSorterDelegate();
    virtual ~StockSorterDelegate();

    virtual int compare(const int &index1, const int &index2, const int &sortIndex);
};

#endif //STOCKSORTERDELEGATE_H


