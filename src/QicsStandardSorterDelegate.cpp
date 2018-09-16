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

#include "QicsStandardSorterDelegate.h"

#include "QicsDataItem.h"
#include "QicsDataModel.h"


QicsStandardSorterDelegate::QicsStandardSorterDelegate(QObject *parent)
    : QicsAbstractSorterDelegate(parent)
{
}

QicsStandardSorterDelegate::~QicsStandardSorterDelegate()
{
}

int QicsStandardSorterDelegate::compare(const int &index1, const int &index2, const int &sortIndex)
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

    int retval = 0;
    int ordFlip = (sortOrder() == Qics::Ascending) ? 1 : -1;

    if (item1 == 0) {
        if (item2 == 0)
            retval = 0;
        else
            retval = ordFlip;
    }
    else if (item2 == 0)
        retval = ordFlip * -1;
    else {
        QString a_string = item1->string();
        QString b_string = item2->string();

        if ((a_string.isEmpty())&&(b_string.isEmpty()))
            retval = 0;
        else
            if (a_string.isEmpty())
                retval = 1;
            else
                if (b_string.isEmpty())
                    retval = -1;
                else {
                    bool ok1, ok2;
                    double d1 = a_string.toDouble(&ok1);
                    double d2 = b_string.toDouble(&ok2);
                    if (ok1 && ok2) {
                        retval = d1 == d2 ? 0 : ordFlip * (d1 < d2 ? -1 : 1);
                    } else
                        if (ok1)
                            retval = -1;
                        else
                            if (ok2)
                                retval = 1;
                            else
                                retval = ordFlip * stringCompare(a_string, b_string);
                }
    }

    delete item1;
    delete item2;

    return retval;
}


