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

#ifndef QICSLISTFILTERDELEGATE_H
#define QICSLISTFILTERDELEGATE_H

#include <QStringList>
#include "QicsAbstractFilterDelegate.h"

///////////////////////////////////////////////////////////////////////////////
// QicsListFilterDelegate
///////////////////////////////////////////////////////////////////////////////

/*! \class QicsListFilterDelegate QicsListFilterDelegate.h
* \nosubgrouping
* \brief Class implements list-based data filter.
* \since 2.4
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsListFilterDelegate : public QicsAbstractFilterDelegate
{
    Q_OBJECT
public:
    /*! Constructor. Takes QStringList as argument \a list. Case sensitivity is set to \a cs.
    */
    QicsListFilterDelegate(const QStringList &list = QStringList(), Qt::CaseSensitivity cs = Qt::CaseInsensitive, QObject *parent = 0);
    ~QicsListFilterDelegate();

    /*! Returns QStringList.
    */
    inline QStringList list() const { return m_list; }

    /*! Sets QStringList as argument \a list.
    */
    inline void setList(const QStringList &list) {m_list = list;}

    /*! Returns case sensitivity.
    */
    inline Qt::CaseSensitivity caseSensitivity() const { return m_cs; }

    /*! Sets case sensitivity to \a cs.
    */
    void setCaseSensitivity(Qt::CaseSensitivity cs);

    virtual bool match(const QString &cellContent, int row, int col);

private:
    QStringList m_list;
    Qt::CaseSensitivity m_cs;
};

#endif //QICSLISTFILTERDELEGATE_H


