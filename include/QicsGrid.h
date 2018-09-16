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

#ifndef QICSGRID_H
#define QICSGRID_H

#include <QVector>
#include "QicsGridInfo.h"
#include "QicsRegion.h"
#include "QicsICell.h"

class QPainter;
class QicsCellDisplay;
class QicsMainGrid;
class QicsCell;
class QicsRow;
class QicsColumn;
class QicsTableCommon;

/*!
* \internal
* \class QicsPositionList QicsPositionList.h
* \brief Helper class for QicsGrid class.
*/

class QicsPositionList
{
public:
    typedef const int *iterator;

    QicsPositionList()  { setFirstIndex(0); init(); }
    QicsPositionList(int base)  { setFirstIndex(base); init(); }

    inline int firstIndex() const  { return myBase; }
    inline void setFirstIndex(int base)  { myBase = base; }

    inline int lastIndex() const { return myBase + size() - 1; }

    inline void push_back(const int &val)
    { myPositions.push_back(val); }

    inline int at(int idx) const;

    inline int operator[](int idx) const { return at(idx); }

    inline void clear() { myPositions.clear(); }

    inline int size() const { return myPositions.size(); }

    inline iterator begin() const { return myPositions.begin(); }
    inline iterator end() const { return myPositions.end(); }

    inline int firstVisible() const;
    inline int lastVisible() const;

protected:
    inline void init() { myPositions.reserve(50); }
    QVector<int> myPositions;
    int myBase;
};

inline int QicsPositionList::at(int idx) const
{
    if ((idx >= myBase) && (idx-myBase<size()))
        return (myPositions.at(idx - myBase));
    else
        return -1;
}

inline int QicsPositionList::firstVisible() const
{
    for (int i = firstIndex(); i <= lastIndex(); ++i)
        if (at(i) >= 0) return i;

    return -1;
}

inline int QicsPositionList::lastVisible() const
{
    for (int i = lastIndex(); i >= firstIndex(); --i)
        if (at(i) >= 0) return i;

    return -1;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


/*!
* \class QicsGrid QicsGrid.h
* \brief Object that manages the layout and display of cells in a grid pattern
*
* QicsGrid is used to display information in a grid format.  The grid
* object is responsible for laying out the cells based on a given rectangle
* and based on various properties that are stored in the QicsStyleManager.
* The grid draws only the lines between the cells, the cells themselves are
* drawn by the appropriate QicsCellDisplay object.
*
* Most users of the %QicsTable product should not need to create or modify
* QicsGrid objects directly.  The QicsTable widget automatically creates
* the necessary grid objects when needed.  One instance where a user may
* need to modify a grid is to change the mouse and/or keyboard behavior of
* the grids.  In this case, please look at QicsScreenGrid and its subclasses.
*
* It is important to note that QicsGrid is not a widget.  It is equally capable
* of layout for a widget or for a printer, and serves as a superclass for
* both purposes.  See QicsScreenGrid for the grid widget hierarchy,
* or see QicsPrintGrid for grid printing.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsGrid : public Qics
{
public:

    enum DrawCellMode
    {
        CellBordersOnly = 0x1,
        CellOnly = 0x2,
        WholeCell = CellBordersOnly | CellOnly
    };

    /*!
    * Constructor for the class.
    * \param  info  used for style and dimension information, as well as
    *               the associated data model.
    */
    QicsGrid(QicsGridInfo &info, int top_row = 0, int left_column = 0);
    /*!
    * The destructor
    */
    virtual ~QicsGrid();

    /*!
    * Returns the grid info object associated with the grid.
    */
    inline QicsGridInfo &gridInfo() const { return m_info; }

    /*!
    * Restricts the grid to show a subset of the data model values.
    * \a reg specifies an area of the table that is allowed to be
    * displayed.
    */
    inline virtual void setViewport(const QicsRegion &reg) {myViewport = reg;}

    /*!
    * Returns the current viewport of the grid.  The viewport
    * specifies an area of the table that is allowed to be
    * displayed.
    * \sa currentViewport(), screenViewport()
    */
    virtual QicsRegion viewport() const;

    /*!
    * Computes the intersection of the data model dimensions and the
    * viewport dimensions.  Because the current viewport can contain
    * values that greater than the number of rows and columns in
    * the data model, it is often necessary for the grid to determine
    * the current viewport.
    * \sa viewport(), screenViewport()
    */
    virtual QicsRegion currentViewport() const;

    /*!
    * Computes the intersection of the data model dimensions and the
    * viewport dimensions.  Because the current viewport can contain
    * values that greater than the number of rows and columns in
    * the data model, it is often necessary for the grid to determine
    * the current viewport. It contains hidder rows and columns.
    * \sa viewport(), screenViewport(), currentViewport
    */
    virtual QicsRegion realViewport() const;

    /*!
    * Returns the visible part of the viewport.
    * \sa viewport(), currentViewport()
    * \since 2.3
    */
    inline QicsRegion screenViewport() const
    { return QicsRegion(myRowPositions.firstIndex(), myColumnPositions.firstIndex(),
    myRowPositions.lastIndex(), myColumnPositions.lastIndex()); }

    /*!
    * Returns \b true if the cell (\a row, \a col ) is within the current
    * viewport, \b false otherwise.
    *
    * \sa currentViewport
    */
    bool isCellValid(int row, int col) const;

    /*!
    * Returns a rectangle specifying the current location and dimension
    * of cell (\a row, \a col ).  If \a with_spans is \b true, the cell
    * dimension will include any cells that this cell spans.
    * If the cell is not currently on the
    * screen, the rectangle will be invalid.
    */
    QRect cellDimensions(int row, int col, bool with_spans) const;

    /*!
    * This is an overloaded member function, provided for convenience.
    * It behaves essentially like the above function.
    */
    inline QRect cellDimensions(const QicsICell &cell, bool with_spans) const
    { return (cellDimensions(cell.row(), cell.column(), with_spans)); }

    /*!
    * Returns the physical column index into the data model for
    * a given visual column
    */
    int modelColumnIndex(int column) const;

    /*!
    * Returns the physical row index into the data model for
    * a given visual row
    */
    int modelRowIndex(int row) const;

    /*!
    * Reorder the presentation of the rows of a data model based
    * on a columns.
    * Note that the data model itself is not changed, only the
    * presentation order shown in this grid and its slaves.
    * \param columns	the column to sort on
    * \param order	sort direction
    * \param func	user defined comparator function.
    */
    virtual void orderRowsBy(const QVector<int> &columns,
        Qics::QicsSortOrder order = Qics::Ascending,
        DataItemComparator func = 0);

    /*!
    * Reorder the presentation of the columns of the data model
    * based on a rows.
    * See ::orderRowsBy for usage description.
    */
    virtual void orderColumnsBy(const QVector<int> &rows,
        Qics::QicsSortOrder order = Qics::Ascending,
        DataItemComparator func = 0);

    /*!
    * \internal
    * Prints position information to stdout.
    */
#ifdef notdef
    void dumpPositions() const;
#endif

protected:
    /*!
    * \internal
    * Returns the data model object associated with this grid.
    */
    inline QicsDataModel *dataModel() const
    { return (m_info.dataModel()); }

    /*!
    * \internal
    * Returns the style manager object associated with this grid.
    */
    inline QicsStyleManager &styleManager() const
    { return (*(m_info.styleManager())); }

    /*!
    * \internal
    * Returns the dimension manager object associated with this grid.
    */
    inline QicsDimensionManager &dimensionManager() const
    { return (*(m_info.dimensionManager())); }

    /*!
    * \internal
    * Returns the dimension manager object associated with this grid.
    */
    inline QicsMappedDimensionManager &mappedDM() const
    { return (*(m_info.mappedDM())); }

    /*!
    * \internal
    * Returns the selection manager object associated with this grid.
    */
    inline QicsSelectionManager &selectionManager() const
    { return (*(m_info.selectionManager())); }


    /*!
    * \internal
    * Draws the portion of the grid specified by \a region, using \a painter.
    *
    * \sa QicsGrid::computeCellPositions
    */
    virtual QRect drawRegion(const QicsRegion &region, QPainter *painter, QicsGrid::DrawCellMode mode);

    /*!
    * \internal
    * Draws grid lines within the region \a reg, using painter \a painter.
    *
    * \sa QicsGrid::computeCellPositions
    */
    virtual void drawGridLines(const QicsRegion &reg, QPainter *painter);

    /*!
    * \internal
    * Draws cell (\a row, \a col ) at position (\a x, \a y ) using
    * the specified painter.  If \a look_for_overflower is \b true,
    * the method will attempt to first find a cell to the left of
    * this cell whose contents may overflow into this cell.  If such
    * a cell is found, it is drawn prior to (and perhaps instead of)
    * drawing this cell.
    */
    virtual QRect drawCell(int row, int col, int x, int y,
        bool look_for_overflower, QPainter *painter, QicsGrid::DrawCellMode mode);

    virtual QRect drawCell(int row, int col, const QRect &rect, QPainter *painter, QicsGrid::DrawCellMode mode);

    /*!
    * \internal
    * Called by QicsGrid::drawCell immediately before calling the appropriate
    * cell display object for cell (\a row, \a col ).  If this method returns
    * \b false, the drawing action will be cancelled.
    *
    * \param row row index of the cell to be drawn
    * \param col column index of the cell to be drawn
    * \param rect the area in which the cell will be drawn
    * \param painter the painter the cell will be drawn in
    */
    virtual bool prepareToDraw(int row, int col, const QRect &rect,
        QPainter *painter);

    /*!
    * \internal
    * Returns the value of cell (\a row, \a col ).  This value will
    * be passed to QicsCellDisplay::displayCell.  In the base class,
    * this method returns the appropriate data item from the grid's
    * QicsDataModel object, but subclasses may reimplement this method.
    */
    virtual const QicsDataItem *cellValue(int row, int col) const;

    /*!
    * \internal
    * Computes cell positions (beginning at cell \a start ) for
    * the area of the screen or page specified by \a bounds.
    */
    QicsICell computeCellPositions(const QRect &bounds,
        const QicsICell &start,
        Qics::QicsIndexType indexType = Qics::RowAndColumnIndex);

    /*!
    * \internal
    * returns the cell display object for the cell (\a row, \a col ).
    */
    QicsCellDisplay *cellDisplay(int row, int col) const;

    /*!
    * \internal
    * \since 2.2
    */
    virtual const QRect boundRect() const { return QRect(); }

    //////////////// Data Members /////////////////

    /*!
    * \internal
    * A pointer to the grid info object, passed in the constructor.
    */
    QicsGridInfo &m_info;

    /*!
    * \internal
    * The part of the table that the grid is allowed to display.
    */
    QicsRegion myViewport;

    /*!
    * \internal
    * List of y coordinate values for currently displayed columns.
    */
    QicsPositionList myRowPositions;
    /*!
    * \internal
    * List of x coordinate values for currently displayed columns.
    */
    QicsPositionList myColumnPositions;

    /*!
    * \internal
    * Stores cells that have already been drawn during the current
    * drawing action.
    */
    QicsICellQVL myAlreadyDrawnCells;

    /*!
    * \internal
    * Used to retrieve grid attribute information
    */
    QicsMainGrid *m_mainGrid;

    /*!
    * \internal
    * common table info
    */
    QicsTableCommon *m_tableCommon;

    /*!
    * \internal
    * Used to retrieve cell attribute information
    */
    QicsCell *m_cell;

    /*!
    * \internal
    * Used to retrieve row attribute information
    */
    QicsRow *m_row;
    /*!
    * \internal
    * Used to retrieve column attribute information
    */
    QicsColumn *m_column;

    //////////////// Data Members /////////////////

    /*!
    * \internal
    * The top displayed row.
    */
    int m_topRow;

    /*!
    * \internal
    * The leftmost displayed column.
    */
    int m_leftColumn;

    /*!
    * \internal
    * Bottom row is only set after a redraw, because it needs to know the
    * screen size to display correctly. Initially, its value is -1.
    */
    int m_bottomRow;

    /*!
    * \internal
    * Right column is only set after a redraw, because it needs to know the
    * screen size to display correctly. Initially, its value is -1.
    */
    int m_rightColumn;

    /*!
    * \internal
    * Right column is only set after a redraw, because it needs to know the
    * screen size to display correctly. Initially, its value is -1.
    */
    int myFullyVisibleRightColumn;

    /*!
    * \internal
    * Bottom row is only set after a redraw, because it needs to know the
    * screen size to display correctly. Initially, its value is -1.
    */
    int myFullyVisibleBottomRow;

    /*!
    * \internal
    * Stores the index of the row that contained the current cell when
    * the last horizontal traversal action occurred.
    */
    int m_traversalRow;
    /*!
    * \internal
    * Stores the index of the column that contained the current cell when
    * the last vertical traversal action occurred.
    */
    int m_traversalColumn;
};

#endif //QICSGRID_H


