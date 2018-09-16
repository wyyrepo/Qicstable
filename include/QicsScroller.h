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

#ifndef QICSSCROLLER_H
#define QICSSCROLLER_H

#include <QObject>
#include "QicsNamespace.h"

class QAbstractSlider;

/*! \class QicsScroller QicsScroller.h
*  \brief Abstract class defining interface for scroller objects
*
* QicsScroller is an absract class that defines an interface
* that all scroller objects must implement.  A scroller connects
* with a QicsScrollManager object and then controls row and/or
* column scrolling of all grids that connect to the scroll manager.
* A scroller has a minimum and maximum possible index.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsScroller: public QObject, public Qics
{
    Q_OBJECT
public:
    /*!
    * The constructor
    */
    QicsScroller(QObject *parent = 0)
        : QObject(parent)
    {
    }

    /*!
    * The destructor
    */
    virtual ~QicsScroller() {}

    // Returns widget for this scroller.
    virtual QAbstractSlider* widget()const = 0;

    // Sets new widget for this scroller.
    virtual void setWidget(QAbstractSlider* widget) = 0;

    // Returns mode of scroller.
    virtual QicsScrollBarMode mode() const = 0;

    // Sets mode of scroller.
    virtual void setMode(QicsScrollBarMode m) = 0;

signals:
    /*!
    * Emitted when the index of this scroller changes.
    * \a type specifies whether this index is for a row or a column.
    * \a idx is the new index value.
    */
    void indexChanged(Qics::QicsIndexType type, int idx);

    void scrollPageAdd(Qics::QicsIndexType type, int idx);

    void scrollPageSub(Qics::QicsIndexType type, int idx);

public slots:
    /*!
    * Returns the current index of this scroller.
    */
    virtual int index() const = 0;
    /*!
    * Sets the current \a type index of this scroller to \a idx.
    */
    virtual void setIndex(Qics::QicsIndexType type, int idx) = 0;

    /*!
    * Returns the current minimum index value of this scroller.
    */
    virtual int minIndex() const = 0;
    /*!
    * Sets the current minimum \a tyhpe index value of this scroller
    * to \a idx.
    */
    virtual void setMinIndex(Qics::QicsIndexType type, int idx) = 0;

    /*!
    * Returns the current maximum index value of this scroller.
    */
    virtual int maxIndex() const = 0;
    /*!
    * Sets the current maximum \a tyhpe index value of this scroller
    * to \a idx.
    */
    virtual void setMaxIndex(Qics::QicsIndexType type, int idx) = 0;
};

#endif //QICSSCROLLER_H


