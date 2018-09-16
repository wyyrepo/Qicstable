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

#ifndef QICSABSTRACTFILTERDELEGATE_H
#define QICSABSTRACTFILTERDELEGATE_H

#include "QicsNamespace.h"

/*!
* \class QicsAbstractFilterDelegate QicsAbstractFilterDelegate.h
* \nosubgrouping
* \brief Abstract class that represents interface for data filters.
* \since 2.4
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsAbstractFilterDelegate : public QObject
{
    Q_OBJECT
public:
    QicsAbstractFilterDelegate(QObject *parent = 0);
    virtual ~QicsAbstractFilterDelegate();

    /*!
    * This method must be implemented in subclasses.
    * It should return true if given \a string matches a filter.
    */
    virtual bool match(const QString &cellContent, int row, int col) = 0;
};

#endif // QICSABSTRACTFILTERDELEGATE_H


