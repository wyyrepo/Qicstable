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

#ifndef QICSREGION_H
#define QICSREGION_H

#include <QRect>
#include <QVector>
#include "QicsNamespace.h"
#include "QicsICell.h"


/*!
* \class QicsRegion QicsRegion.h
* \brief A lightweight, rectangle-like object that refers to rows and columns
*
* A QicsRegion specifies a rectangular region of a QicsTable.  A QicsRegion instance
* is defined by a beginning (row, column) pair and an ending (row, column) pair.
* As it is subclassed from \b QRect, a QicsRegion object has all the functionality
* of its superclass, including \a union, \a intersection, and \a contains methods.
* QicsRegions are always in view rather than model coordinates.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsRegion : public QRect
{
public:
    /*!
    * Constructs a new region object.  The region extends from
    * row \a begin_row, column \a begin_col to
    * row \a end_row, column \a end_col.
    */
    QicsRegion(int begin_row, int begin_col, int end_row, int end_col)
        : QRect(begin_col, begin_row, end_col-begin_col+1, end_row-begin_row+1)
    {
    }

    /*!
    * Constructs a new region object from \a rect.
    * \since 2.4
    */
    QicsRegion(const QRect &rect)
        : QRect(rect)
    {
    }

    /*!
    * Constructs a new region object.  The region extends from
    * row \a begin_row, column \a begin_col to
    * row \a end_row, column \a end_col.
    */
    QicsRegion(const QicsICell &begin_cell, const QicsICell &end_cell)
        : QRect(begin_cell.column(), begin_cell.row(),
            end_cell.column() - begin_cell.column() + 1,
            end_cell.row() - begin_cell.row() + 1) {}

    /*!
    * Constructs a new region object covering \a row and \a col.
    * \since 2.2
    */
    QicsRegion(int row, int col)
        : QRect(col, row, 1, 1)
    {
    }

    /*!
    * Constructs a new region object covering \a cell.
    * \since 2.2
    */
    QicsRegion(const QicsICell &cell)
        : QRect(cell.column(), cell.row(), 1, 1)
    {
    }

    /*!
    * Constructs an empty, invalid region.
    */
    QicsRegion()
        : QRect()
    {
    }

    /*!
    * Returns the starting (i.e. top-left) cell of the region.
    */
    inline QicsICell startCell() const
    { return QicsICell(startRow(), startColumn()); }

    /*!
    * Returns the ending (i.e. bottom-right) cell of the region.
    */
    inline QicsICell endCell() const
    { return QicsICell(endRow(), endColumn()); }

    /*!
    * Returns the starting row of the region.
    */
    inline int startRow() const { return top(); }
    /*!
    * Returns the ending row of the region.
    */
    inline int endRow() const { return bottom(); }
    /*!
    * Returns the starting column of the region.
    */
    inline int startColumn() const { return left(); }
    /*!
    * Returns the ending column of the region.
    */
    inline int endColumn() const { return right(); }
    /*!
    * Returns the number of columns in the region.
    */
    inline int numColumns() const { return (left() >= 0 && right() >= 0) ? width() : 0; }
    /*!
    * Returns the number of rows in the region.
    */
    inline int numRows() const { return (top() >= 0 && bottom() >= 0) ? height() : 0; }

    /*!
    * Sets the starting (i.e. top-left) cell of the region.
    */
    inline void setStartCell(const QicsICell &cell)
    { setTop(cell.row()); setLeft(cell.column()); }

    /*!
    * Sets the ending (i.e. bottom-right) cell of the region.
    */
    inline void setEndCell(const QicsICell &cell)
    { setBottom(cell.row()); setRight(cell.column()); }

    /*!
    * Sets the starting row of the region.
    */
    inline void setStartRow(int val) { setTop(val); }
    /*!
    * Sets the ending row of the region.
    */
    inline void setEndRow(int val) { setBottom(val); }
    /*!
    * Sets the starting column of the region.
    */
    inline void setStartColumn(int val) { setLeft(val); }
    /*!
    * Sets the ending column of the region.
    */
    inline void setEndColumn(int val) { setRight(val); }

    /*!
    * Returns \b true if the \a cell is in the region.
    */
    inline bool containsCell(const QicsICell &cell) const
    { return contains(cell.column(), cell.row()); }

    /*!
    * Returns \b true if the cell (\a row, \a col) is in
    * the region.
    */
    inline bool containsCell(int row, int col) const
    { return contains(col, row); }

    /*!
    * Returns intersect region
    */
    inline QicsRegion intersect(const QicsRegion &reg) const
    {
        QRect r = QRect::intersected(reg);
        return QicsRegion(r.top(), r.left(), r.bottom(), r.right());
    }

    /*!
    * Returns \b true if the region is valid
    */
    inline bool isValid() const
    {
        return ((top() >= 0) && (bottom() >= 0) &&
            (left()>= 0) && (right() >= 0) &&
            QRect::isValid());
    }

    /*!
    Normalizes region (i.e. region will have a non-negative width and height).
    If width() < 0 the function swaps the left and right corners, and it swaps the top and bottom corners if height() < 0.
    */
    inline void normalize();

    /*!
    * Returns true if this region includes entire row(s).
    * \since 2.4
    */
    inline bool isEntireRow() const
    { return (right() == Qics::QicsLAST_COLUMN); }

    /*!
    * Returns true if this region includes entire column(s).
    * \since 2.4
    */
    inline bool isEntireColumn() const
    { return (bottom() == Qics::QicsLAST_ROW); }

    /*!
    * Returns a region of (0,0,Qics::QicsLAST_COLUMN,Qics::QicsLAST_ROW).
    * \since 2.4
    */
    static QicsRegion completeRegion()
    { return QicsRegion(0,0,Qics::QicsLAST_COLUMN,Qics::QicsLAST_ROW); }
};

inline void QicsRegion::normalize()
{
    if (left() > right()) {
        int l = right();
        setRight(left());
        setLeft(l);
    }
    if (top() > bottom()) {
        int l = bottom();
        setBottom(top());
        setTop(l);
    }

    if (left() < 0)
        setLeft(0);
    if (top() < 0)
        setTop(0);
}

typedef QVector<QicsRegion> QicsRegionV;

#endif //QICSREGION_H


