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

#ifndef QICSFILTER_H
#define QICSFILTER_H

#include <QPointer>
#include <QObject>
#include <QMap>
#include <QSet>
#include <QRegExp>
#include "QicsNamespace.h"

class QicsGridInfo;
class QicsDataModel;
class QicsAbstractFilterDelegate;

///////////////////////////////////////////////////////////////////////////////
// QicsFilter
///////////////////////////////////////////////////////////////////////////////

/*! \class QicsFilter QicsFilter.h
* \nosubgrouping
* \brief Abstract class that represents interface for delegate based data filters.
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsFilter : public QObject
{
    Q_OBJECT
    friend class QicsDimensionManager;
public:
    QicsFilter(QicsGridInfo *info, QObject *parent = 0);
    ~QicsFilter();

    /*! Returns filter delegate installed for \a index, or 0 if none.
    */
//    inline QicsAbstractFilterDelegate* filter(int index) { return m_filters.value(index);}
    QicsAbstractFilterDelegate* filter(int index);

    /*!
    * Returns true if \a index has any filter installed.
    */
    inline bool hasFilter(int index) const { return m_filters.contains(index); }

    virtual void setFilter(int index, QicsAbstractFilterDelegate *filter, bool deleteOld) = 0;

    virtual void removeFilter(int index, bool deleteOld = false) = 0;

    virtual void removeAll() = 0;

    /*! Returns \b true if \a index is filtered (i.e.hidden). Returns \b false otherwise.
    */
    inline bool isFiltered(int index) const { return m_hiddenIndexes.contains(index); }

signals:
    /*! Emitted when a filter was set or removed.
    *  If filter was set at \a index, \a set is true.
    *  If filter was removed at \a index, \a set is false.
    *  If \a index = -1, then multiple filters set/removal has took place.
    *  \since 2.4
    */
    void filterChanged(int index, bool set);

protected:
    // called by QicsDimensionManager
    virtual void handleRowsAdded(int num, int start_position) = 0;
    virtual void handleRowsRemoved(int num, int start_position) = 0;
    virtual void handleColumnsAdded(int num, int start_position) = 0;
    virtual void handleColumnsRemoved(int num, int start_position) = 0;

    // data
//    QMap<int, QicsAbstractFilterDelegate *> m_filters;
    QMap<int, QPointer<QicsAbstractFilterDelegate> > m_filters;
    QSet<int> m_hiddenIndexes;
    QicsGridInfo *m_info;
    QicsDataModel *m_model;
};

///////////////////////////////////////////////////////////////////////////////
// QicsRowFilter
///////////////////////////////////////////////////////////////////////////////
/*! \class QicsRowFilter
* \nosubgrouping
* \brief Implemetation of QicsFilter interface that allows to filter data by rows.
* QicsRowFilter is an implemetation of QicsFilter interface that allows to filter data by rows.
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsRowFilter : public QicsFilter
{
public:
    /*! Constructor
    */
    QicsRowFilter(QicsGridInfo *info, QObject *parent = 0);

    /*!
    *  Sets filter to \a index column. Rows that don't match \a filter
    *  regular expresiion in \a index column will be hidden.
    *  If \a filter = 0, it is equal to call removeFilter(index).
    */
    virtual void setFilter(int index, QicsAbstractFilterDelegate *filter, bool deleteOld = false);

    /*! Removes filter from \a index column.
    */
    virtual void removeFilter(int index, bool deleteOld = false);

    /*! Removes all filters from columns.
    */
    virtual void removeAll();

    /// Checks \a index row for matching all the filters installed for columns.
    /// Returns \b true if the row matchs all the filters, i.e. should be shown.
    /// Returns \b false otherwise.
    virtual bool isRowMatchFilters(int index);

    // called by QicsDimensionManager
    virtual void handleRowsAdded(int num, int start_position);
    virtual void handleRowsRemoved(int num, int start_position);
    virtual void handleColumnsAdded(int num, int start_position);
    virtual void handleColumnsRemoved(int num, int start_position);

protected:
    // internal
    void doUpdateHiddenRows();
};


#endif //QICSFILTER_H


