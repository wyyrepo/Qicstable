/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef CUSTOMSORTERDELEGATE_H
#define CUSTOMSORTERDELEGATE_H

#include <QicsAbstractSorterDelegate.h>


class CustomSorterDelegate : public QicsAbstractSorterDelegate
{
public:
    CustomSorterDelegate();
    virtual ~CustomSorterDelegate();

    virtual int compare(const int &index1, const int &index2, const int &sortIndex);

private:
    bool isNumberType(const QString &typeName) const;
};

#endif //CUSTOMSORTERDELEGATE_H


