/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "stocksorterdelegate.h"

#include "QicsDataItem.h"
#include "QicsDataModel.h"


StockSorterDelegate::StockSorterDelegate()
    : QicsAbstractSorterDelegate()
{
}

StockSorterDelegate::~StockSorterDelegate()
{
}

int StockSorterDelegate::compare(const int &index1, const int &index2, const int &sortIndex)
{
    const QicsDataModel *dataModel = model();

    if (!dataModel) return 0;

    const QicsDataItem *item1 = 0;
    const QicsDataItem *item2 = 0;

    if (indexType() == Qics::RowIndex) {
        item1 = dataModel->item(index1, sortIndex);
        if (item1) item1 = item1->clone();
        item2 = dataModel->item(index2, sortIndex);
        if (item2) item2 = item2->clone();
    }
    else {
        item1 = dataModel->item(sortIndex, index1);
        if (item1) item1 = item1->clone();
        item2 = dataModel->item(sortIndex, index2);
        if (item2) item2 = item2->clone();
    }

    int ret = 0;
    int ordFlip = (sortOrder() == Qics::Ascending) ? 1 : -1;

    if ((item1->type() == QicsDataItem_Float) && (item2->type() == QicsDataItem_Float)) {
        float fa = ((const QicsDataFloat *) item1)->data();
        float fb = ((const QicsDataFloat *) item2)->data();

        if (fa < fb)
            ret = -1;
        else if (fa == fb)
            ret = 0;
        else
            ret = 1;
    }
    else
        ret = QString::compare(item1->string(),item2->string());

    delete item1;
    delete item2;

    return ordFlip * ret;
}


