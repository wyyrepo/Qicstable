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

#ifndef QICSSCROLLMANAGER_H
#define QICSSCROLLMANAGER_H

#include <QObject>
#include "QicsNamespace.h"

//////////////////////////////////////////////////////////////////////////////

class QicsScroller;
class QicsScreenGrid;


/*! \internal
* \class QicsScrollManager QicsScrollManager.h
* \brief handles all table scrolling activity
*
* QicsScrollManager coordinates communication between QicsScroller objects
* and QicsScreenGrid objects.  Scrollers signal when the table should be scrolled,
* while the main table grid signals when its dimensions or bounds change.
* The scroll manager also maintains the current top, left cell
* location of the table.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////
class QicsScrollManager: public QObject, public Qics
{
    Q_OBJECT
public:
    /*!
    * Public constructor
    */
    QicsScrollManager(QObject *parent = 0);
    /*!
    * Destructor method.
    */
    virtual ~QicsScrollManager();

    /*!
    * Connects \a scroller to all grids that have registered with the
    * scroll manager.  User interaction with the scroller will result
    * in a new top row and/or left column being set in the appropriate
    * grid widgets.
    */
    void connectScroller(QicsScroller *scroller);
    /*!
    * Disconnects \a scroller from all grids that have registered with the
    * scroll manager.  User interaction with the scroller will no longer
    * affect the grid widgets.
    */
    void disconnectScroller(QicsScroller *scroller);
    /*!
    * Returns the primary grid widget.  The primary grid controls
    * the min and max values of all scrollers that are connected to the
    * scroll manager.
    */
    inline QicsScreenGrid *primaryGrid() const { return myPrimaryGrid; }
    /*!
    * Sets the primary grid widget.  The primary grid controls
    * the min and max values of all scrollers that are connected to the
    * scroll manager.
    */
    void setPrimaryGrid(QicsScreenGrid *grid);
    /*!
    * Connects \a grid to all scrollers that have registered with the
    * scroll manager.  If \a control_rows is \b true, user interaction
    * with a scroller will result in a new top row being set in \a grid.
    * If \a control_columns is \b true, user interaction
    * with a scroller will result in a new top column being set in \a grid.
    */
    void connectGrid(QicsScreenGrid *grid, bool control_rows, bool control_columns);
    /*!
    * Disconnects \a grid from all scrollers that have registered with the
    * scroll manager.  User interaction with a scroller will no longer
    * affect \a grid.
    */
    void disconnectGrid(QicsScreenGrid *grid);
    /*!
    * Returns the current topmost displayed row index of the table.
    */
    inline int rowIndex() const { return myRowIndex; }
    /*!
    * Returns the current leftmost displayed column index of the table.
    */
    inline int columnIndex() const { return myColumnIndex; }
    /*!
    * Returns horizontal scroller
    */
    inline QicsScroller* horizontalScroller() const {return myHScroller;}
    /*!
    * Returns vertical scroller
    */
    inline QicsScroller* verticalScroller() const {return myVScroller;}

public slots:
    /*!
    * Changes the current value of index \a type to \a idx.
    */
    void setIndex(Qics::QicsIndexType type, int idx);

signals:
    /*!
    * Notifies grid widgets that the top row index has changed to \a idx.
    */
    void rowIndexChanged(int idx);
    /*!
    * Notifies grid widgets that the left column index has changed to \a idx.
    */
    void columnIndexChanged(int idx);
    /*!
    * Notifies scrollers that the value for index \a type has
    * changed to \a val.
    */
    void indexChanged(Qics::QicsIndexType type, int val);
    /*!
    * Notifies scrollers that the minimum value for index \a type has
    * changed to \a val.
    */
    void minIndexChanged(Qics::QicsIndexType type, int val);
    /*!
    * Notifies scrollers that the maximum value for index \a type has
    * changed to \a val.
    */
    void maxIndexChanged(Qics::QicsIndexType type, int val);

protected:
    // Function which set vertical scroller position to position idx.
    void computeVSPosition(int idx);
    // Function which set horizontal scroller position to position idx.
    void computeHSPosition(int idx);

    // the primary grid widget
    QicsScreenGrid *myPrimaryGrid;
    // the current topmost displayed row
    int myRowIndex;
    // the current leftmost displayed column
    int myColumnIndex;
    // Calculate rowIndex position for scroller

    QicsScroller* myHScroller;
    QicsScroller* myVScroller;
    // Slider pressed flag.
    bool sliderPressedFlag;
    // Which row index previous time requsted.
    int requestedRowIdx;
    // Which collumn index previous time requsted.
    int requestedColumnIdx;

protected slots:
    /*!
    * Sets the minimum possible row value in all registered scroller objects
    * to \a val.
    */
    void setRowMinValue(int val);
    /*!
    * Sets the maximum possible row value in all registered scroller objects
    * to \a val.
    */
    void setRowMaxValue(int val);
    /*!
    * Sets the minimum possible column value in all registered scroller objects
    * to \a val.
    */
    void setColumnMinValue(int val);
    /*!
    * Sets the maximum possible column value in all registered scroller objects
    * to \a val.
    */
    void setColumnMaxValue(int val);
    /*!
    * Sets the current topmost displayed row index for the table to \a idx.
    */
    void setRowIndex(int idx);
    /*!
    * Sets the current leftmost displayed column index for the table to \a idx.
    */
    void setColumnIndex(int idx);

    void setScrollerIndex(Qics::QicsIndexType type, int idx);

    /*!
    * Causes the primary grid to be queried for new bounds information,
    * which may cause new minimum and maximum row/column values to be
    * set on all registered scrollers.
    */
    void gridBoundsChanged();

    /*!
    * Notifies the scroll manager that the primary grid has been destroyed.
    */
    inline void gridDestroyed(QObject *) {myPrimaryGrid = 0;}

    /*!
    *
    */
    // TODO: if we'll use programaticaly slider change index.
    inline void sliderPressedAction() {sliderPressedFlag = true;}

    void handleScrollPageAdd(Qics::QicsIndexType type, int idx);
    void handleScrollPageSub(Qics::QicsIndexType type, int idx);

private:
    friend class QicsScrollBarScroller;
};

#endif //QICSSCROLLMANAGER_H


