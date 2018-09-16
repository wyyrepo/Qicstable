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

#ifndef QICSSPAN_H
#define QICSSPAN_H

#include <QVector>
#include <QDomElement>
#include "QicsRegion.h"

/*! \file */

/*!
* \class QicsSpan QicsSpan.h
* \brief A lightweight, rectangle-like object that refers to a span of cells.
*
* A QicsSpan specifies a rectangular region of a QicsTable.  It begins
* at a particular cell (#row(), #column()) and spans for a
* given number of rows (#height()) and columns (width()).
*
* \internal It is almost like a region, but we use this instead to
* emphasize that it is a single cell spanning out, rather than a region
* anchored by two endpoint cells.   This makes a lot of calculations
* clearer when you consider reordering the rows and columns and passing
* cell coordinates throught the sorting vectors.
*/

class QICS_EXPORT QicsSpan
{
public:
    /*!
    * Constructs an empty, invalid span.
    */
    QicsSpan()
        : _row(-1), _col(-1), _nrows(0), _ncols(0) //, _vrow(-1), _vcol(-1)
    {
    }
    /*!
    * Constructs a new span object.  The span extends from
    * row \a begin_row, column \a begin_col to
    * row \a begin_row+nrows-1, column \a begin_col+width-1.
    */
    QicsSpan(int begin_row, int begin_col, int nrows, int ncols)
        : _row(begin_row), _col(begin_col), _nrows(nrows), _ncols(ncols)
        //,_vrow(-1), _vcol(-1)
    {
    }

    /*!
    * Converts itself to QicsRegion.
    * \since 2.3
    */
    inline QicsRegion toRegion() const
    {
      return QicsRegion(_row, _col, _row+_nrows-1, _col+_ncols-1);
    }

    /*!
    * Creates QicsSpan from QicsRegion \a reg.
    * \since 2.4
    */
    static QicsSpan fromRegion(const QicsRegion &reg)
    {return QicsSpan(reg.startRow(), reg.startColumn(), reg.height(), reg.width());}

    /*!
    * Returns the starting row of the span.
    */
    inline int row() const {return _row;}
    /*!
    * Returns the starting visual row of the span.
    */
  //  inline int vrow() const {return _vrow;}
    /*!
    * Returns the starting column of the span.
    */
    inline int column() const {return _col;}
    /*!
    * Returns the starting visual column of the span.
    */
   // inline int vcolumn() const {return _vcol;}

    // Kept in for backward compatibility reasons
    /*!
    * Returns the starting column of the span.
    */
    inline int col() const {return column();}
    /*!
    * Returns the height of the span.
    */
    inline int height() const {return _nrows;}
    /*!
    * Returns the width of the span.
    */
    inline int width() const {return _ncols;}

    /*!
    * Returns \b true if the span is valid (#row() and #col() are non-negative
    * and #height() and #width() are greater than 0), or \b false otherwise.
    */
    inline bool isValid() const
    {return ((_row < 0) || (_col < 0) || (_nrows <= 0) || (_ncols <= 0));}

    /*!
    * Returns \b true if the cell (\a row, \a col) is in
    * the span.
    */
    inline bool containsCell(int row, int col) const
    {
      return (isValid() && (_row <= row) && (row < _row + _nrows) &&
          (_col <= col) && (col < _col + _ncols));
    }

    /*!
    * Returns \b true if the span \a s intesects me
    */
    bool intersects(const QicsSpan &s) const;

    /*!
    * Store settings to \a QDoomElement.
    */
    QDomElement toDomXml(QString tag, QDomDocument* doc) const
    {
      QDomElement spanElement = doc->createElement(tag);

      spanElement.setAttribute("row", _row);
      spanElement.setAttribute("col", _col);
      spanElement.setAttribute("rows", _nrows);
      spanElement.setAttribute("cols", _ncols);

      return spanElement;
    }

    /*!
    * Restore settings from \a QDoomElement.
    */
    void configureFromDomXml(const QDomElement& e)
    {
      QString value;
      value = e.attribute("row", QString::number(-1));
      _row = value.toInt();
      value = e.attribute("col", QString::number(-1));
      _col = value.toInt();
      value = e.attribute("rows", QString::number(-1));
      _nrows = value.toInt();
      value = e.attribute("cols", QString::number(-1));
      _ncols = value.toInt();
    }

private:
    int _row, _col, _nrows, _ncols;//, _vrow, _vcol;
    friend class QicsSpanManager;
};

/*! \typedef
* QicsSpanList represents a list of QicsSpan objects.  The list is implemented
* using QValueVector.
*/
typedef QVector<QicsSpan> QicsSpanList;

#endif //QICSSPAN_H


