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

#ifndef QICSSCROLLBARSCROLLER_H
#define QICSSCROLLBARSCROLLER_H

#include <QScrollBar>
#include "QicsScroller.h"

class QAbstractSlider;

/*! \class QicsScrollBarScroller QicsScrollBarScroller.h
*  \brief Table scroller using a QScrollBar widget as the UI
*
* QicsScrollBarScroller is an implementation of QicsScroller
* that uses a generic QScrollBar widget as its UI.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////
class QICS_EXPORT QicsScrollBarScroller: public QicsScroller
{
    Q_OBJECT
public:
    /*!
    * The constructor.  \a type specifies if this scroller should
    * control rows or columns.  \a parent specifies the parent of
    * the scrollbar widget.  \a name specifies the name of this object.
    */
    QicsScrollBarScroller(Qics::QicsIndexType type, QWidget *parent);
    /*!
    * The destructor.
    */
    virtual ~QicsScrollBarScroller();

    /*!
    * Returns the QScrollBar widget of this scroller.
    */
    virtual QAbstractSlider *widget() const {return myScrollBar;}

    /*!
    * Sets new widget for this scroller. Old widget deletes
    * automatically.
    */
    virtual void setWidget(QAbstractSlider* widget);

    /*!
    * Returns the mode of the scrollbar.
    */
    virtual QicsScrollBarMode mode() const { return myMode; }

    /*!
    * Sets the mode of the scrollbar.
    */
    virtual void setMode(QicsScrollBarMode m);

    /*!
    * Returns true if slider in drag mode.
    */
    bool scrollSliderDrag() const {return myIsSliderPressed; }

public slots:
    /*!
    * Returns the current index of the scroller.
    */
    virtual int index() const { return myScrollBar->value(); }
    /*!
    * Sets the current \a type index of the scroller to \a idx.  The
    * request is ignored if \a type is not the same as the type that
    * was passed to the constructor.
    */
    virtual void setIndex(Qics::QicsIndexType type, int idx);

    /*!
    * Returns the current minimum index of the scroller.
    */
    virtual int minIndex() const { return myScrollBar->minimum(); }
    /*!
    * Sets the current \a type minimum index of the scroller to \a idx.  The
    * request is ignored if \a type is not the same as the type that
    * was passed to the constructor.
    */
    virtual void setMinIndex(Qics::QicsIndexType type, int idx);

    /*!
    * Returns the current maximum index of the scroller.
    */
    virtual int maxIndex() const { return myScrollBar->maximum(); }
    /*!
    * Sets the current \a type maximum index of the scroller to \a idx.  The
    * request is ignored if \a type is not the same as the type that
    * was passed to the constructor.
    */
    virtual void setMaxIndex(Qics::QicsIndexType type, int idx);

    /*!
    * This clot handle mouse pressing on slider.
    */
    inline void handleSliderPressed() {myIsSliderPressed = true;}

    /*!
    *  Handle mouse release on slider.
    */
    inline void handleSliderReleased() {myIsSliderPressed = false;}


    void handleActionTriggered(int action);

protected slots:
    /*!
    * Called when the scrollbar widget's value changes.
    */
    void valueChanged(int val);

protected:
    /*!
    * Checks the min and max indices and hideWhenNotNeeded
    * to determine if the scrollbar should be hidden or shown.
    */
    void checkBounds();

    // the scrollbar widget
    QAbstractSlider *myScrollBar;
    // the type of this scroller
    Qics::QicsIndexType myType;
    // flag used in setIndex to avoid recursive signals
    bool myInSetIndex;
    // flag which store slider press action
    bool myIsSliderPressed;
    int oldVal;
    // controls the mode of the scrollbar
    QicsScrollBarMode myMode;
};

#endif //QICSSCROLLBARSCROLLER_H


