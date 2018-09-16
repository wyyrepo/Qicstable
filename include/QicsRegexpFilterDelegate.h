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

#ifndef QICSREGEXPFILTERDELEGATE_H
#define QICSREGEXPFILTERDELEGATE_H

#include <QRegExp>
#include "QicsAbstractFilterDelegate.h"


///////////////////////////////////////////////////////////////////////////////
// QicsRegexpFilterDelegate
///////////////////////////////////////////////////////////////////////////////

/*! \class QicsRegexpFilterDelegate QicsRegexpFilterDelegate.h
* \nosubgrouping
* \brief Class implements QRegExp-based data filter.
* \since 2.4
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsRegexpFilterDelegate : public QicsAbstractFilterDelegate
{
    Q_OBJECT
public:
    /*! Constructor. Takes QRegExp as argument \a regexp.
    */
    QicsRegexpFilterDelegate(const QRegExp &regexp, QObject *parent = 0);
    virtual ~QicsRegexpFilterDelegate();

    /*! Returns QRegExp.
    */
    inline QRegExp regexp() const { return m_regexp; }

    /*! Sets QRegExp as argument \a regexp.
    */
    inline void setRegexp(const QRegExp &regexp) {m_regexp = regexp;}

    virtual bool match(const QString &cellContent, int row, int col);

private:
    QRegExp m_regexp;
};

#endif //QICSREGEXPFILTERDELEGATE_H


