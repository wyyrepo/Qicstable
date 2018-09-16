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

#include "QicsListFilterDelegate.h"


QicsListFilterDelegate::QicsListFilterDelegate(const QStringList &list, Qt::CaseSensitivity cs, QObject *parent)
    : QicsAbstractFilterDelegate(parent), m_list(list), m_cs(cs)
{
}

QicsListFilterDelegate::~QicsListFilterDelegate()
{
}

void QicsListFilterDelegate::setCaseSensitivity(Qt::CaseSensitivity cs)
{
    if (m_cs != cs) {
        m_cs = cs;
    }
}

bool QicsListFilterDelegate::match(const QString &cellContent, int row, int col)
{
    Q_UNUSED(row);
    Q_UNUSED(col);
    return m_list.contains(cellContent, m_cs);
}


