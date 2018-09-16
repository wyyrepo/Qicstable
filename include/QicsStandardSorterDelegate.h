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

#ifndef QICSSTANDARDSORTERDELEGATE_H
#define QICSSTANDARDSORTERDELEGATE_H

#include "QicsAbstractSorterDelegate.h"

///////////////////////////////////////////////////////////////////////////////
// QicsStandardSorterDelegate
///////////////////////////////////////////////////////////////////////////////

/*! \class QicsStandardSorterDelegate QicsStandardSorterDelegate.h
* \nosubgrouping
* \brief This class represents interface for text data sorting.
* \since 2.4.2
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsStandardSorterDelegate : public QicsAbstractSorterDelegate
{
    Q_OBJECT
public:
    QicsStandardSorterDelegate(QObject *parent = 0);
    virtual ~QicsStandardSorterDelegate();

    virtual int compare(const int &index1, const int &index2, const int &sortIndex);
};

#endif //QICSSTANDARDSORTERDELEGATE_H


