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

#include "QicsRegexpFilterDelegate.h"


QicsRegexpFilterDelegate::QicsRegexpFilterDelegate(const QRegExp &regexp, QObject *parent)
    : QicsAbstractFilterDelegate(parent), m_regexp(regexp)
{
}

QicsRegexpFilterDelegate::~QicsRegexpFilterDelegate()
{
}

bool QicsRegexpFilterDelegate::match(const QString &cellContent, int row, int col)
{
    Q_UNUSED(row);
    Q_UNUSED(col);
    return m_regexp.exactMatch(cellContent);
}


