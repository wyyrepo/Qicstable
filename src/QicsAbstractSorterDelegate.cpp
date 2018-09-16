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

#include "QicsAbstractSorterDelegate.h"

#include <QString>


QicsAbstractSorterDelegate::QicsAbstractSorterDelegate(QObject *parent)
    : QObject(parent), m_sortingSensitivity(Qt::CaseSensitive),
        m_sortOrder(Qics::Ascending), m_model(0), m_type(Qics::ColumnIndex)
{
}

QicsAbstractSorterDelegate::~QicsAbstractSorterDelegate()
{
}

int QicsAbstractSorterDelegate::stringCompare(const QString &str1, const QString &str2)
{
    return QString::compare(str1, str2, m_sortingSensitivity);
}


