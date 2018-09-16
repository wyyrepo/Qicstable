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

#ifndef QICSICELL_H
#define QICSICELL_H

#include <QPoint>
#include <QList>
#include <QVector>
#include "QicsNamespace.h"

/*!
* \class QicsICell QicsICell.h
* \brief A lightweight object that refers to a single cell
*
* A QicsICell specifies a cell of a QicsTable.  A  QicsICell instance
* is defined by a row and a column.
* As it is subclassed from \b QPoint, a QicsICell object has all the functionality
* of its superclass.
*
* QicsICell is mostly used internally, as a lightweight object that specifies
* a signle cell but can be used to set and retrieve cell information in a
* QicsRegion object.  Note that QicsICell cannot be used to set or retrieve
* cell property information -- use QicsCell for this.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsICell : public QPoint
{
public:
    /*!
    * Constructs a new cell object.
    */
    QicsICell(int row, int col)
        : QPoint(col, row)
    {
    }

    /*!
    * Constructs an empty, invalid ICell.
    */
    QicsICell()
        : QPoint(-1, -1)
    {
    }

    /*!
    * Returns the row index of the cell.
    */
    inline int row() const { return y(); }
    /*!
    * Returns the column index of the cell.
    */
    inline int column() const { return x(); }

    /*!
    * Sets the row index of the cell.
    */
    inline void setRow(int row) { setY(row); }
    /*!
    * Sets the column index of the cell.
    */
    inline void setColumn(int col) { setX(col); }

    /*!
    * Returns \b true if this is a valid cell (both row() and column()
    * are non-negative), \b false otherwise.
    */
    inline bool isValid() const { return ((x() >= 0) && (y() >= 0)); }
};

typedef QVector<QicsICell> QicsICellV;
typedef QVector<QicsICell *> QicsICellPV;
typedef QList<QicsICell> QicsICellQVL;

#endif //QICSICELL_H


