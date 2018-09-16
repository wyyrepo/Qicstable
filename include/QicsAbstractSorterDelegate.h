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

#ifndef QICSABSTRACTSORTERDELEGATE_H
#define QICSABSTRACTSORTERDELEGATE_H

#include <Qt>
#include "QicsNamespace.h"

///////////////////////////////////////////////////////////////////////////////
// QicsAbstractSorterDelegate
///////////////////////////////////////////////////////////////////////////////

/*! \class QicsAbstractSorterDelegate QicsAbstractSorterDelegate.h
* \nosubgrouping
* \brief Abstract class that represents interface for data sorting.
* \since 2.4.2
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QicsDataItem;
class QicsDataModel;

class QICS_EXPORT QicsAbstractSorterDelegate : public QObject
{
    Q_OBJECT
public:
    QicsAbstractSorterDelegate(QObject *parent = 0);
    virtual ~QicsAbstractSorterDelegate();

    inline Qt::CaseSensitivity sortingSensitivity() const {return m_sortingSensitivity;}
    inline void setSortingSensitivity(Qt::CaseSensitivity cs) {m_sortingSensitivity = cs;}

    /*!
    * This method must be implemented in subclasses.
    */
    virtual int compare(const int &index1, const int &index2, const int &sortIndex) = 0;

protected:
    int stringCompare(const QString &str1, const QString &str2);
    inline Qics::QicsSortOrder sortOrder() const {return m_sortOrder;}
    inline QicsDataModel *model() const {return m_model;}
    inline Qics::QicsIndexType indexType() const {return m_type;}

private:
    inline void setModel(QicsDataModel *model) {m_model = model;}
    inline void setType(Qics::QicsIndexType t) {m_type = t;}
    inline void setSortOrder(Qics::QicsSortOrder so) {m_sortOrder = so;}

private:
    Qt::CaseSensitivity m_sortingSensitivity;
    Qics::QicsSortOrder m_sortOrder;
    QicsDataModel *m_model;
    Qics::QicsIndexType	m_type;		// row or column indicator

    friend class QicsSorter;
    friend int delegateCompare(const int &_a, const int &_b);
};

#endif //QICSABSTRACTSORTERDELEGATE_H


