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

#ifndef QICSHEADER_H
#define QICSHEADER_H

#include "QicsGridCommon.h"

/*!
* \class QicsHeader QicsHeader.h
*  \nosubgrouping
*  \brief A base class for header objects
*
* QicsHeader is a base class that allows setting and retrieval
* of common header attributes.  It is not intended to be directly instantiated.
* Rather, one of its subclasses should be used.
*
* New common header attributes should be added here so that all subclasses
* can use them.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsRow;
class QicsColumn;
class QicsCell;
class QicsCellRegion;
class QicsHeaderGrid;

class QICS_EXPORT QicsHeader: public QicsGridCommon
{
    Q_OBJECT
public:
    /** @name Common Header Properties
    */

    //@{
    /*!
    * Specifies the header's current policy regarding user resizing.
    * If \b true, users may resize rows or columns (depending on
    * the type of header).  If \b false, no resizing is allowed.
    *
    * The default value of this property is \b true.
    */
    Q_PROPERTY( bool allowUserResize READ allowUserResize WRITE setAllowUserResize )

    /*!
    * Specifies the header's current policy regarding header resizing.
    * If \b true, users may resize row or column headers (depending on
    * the type of header).  If \b false, no resizing is allowed.
    *
    * The default value of this property is \b true.
    */
    Q_PROPERTY( bool allowHeaderResize READ allowHeaderResize WRITE setAllowHeaderResize )

    /*!
    * Specifies the header's current policy regarding user movement of rows
    * or columns. User move operations work like all drag&drop operations in
    * QicsTable - with Middle button of mouse.  If \b true, user may move rows
    * or columns (depending on the type of header). If \b false, no movement is allowed.
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool allowUserMove READ allowUserMove WRITE setAllowUserMove )

    /*!
    * Specifies the header's current policy regarding user editing of rows
    * or columns. If \b true, user may edit caption of rows
    * or columns. If \b false, no editing is allowed.
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool allowUserEdit READ allowUserEdit WRITE setAllowUserEdit )

    /*!
    * Specifies the header cells default appearance if the name isn't
    * specified. The possible special values user might want to use is
    * %column%, %row% - inserts column and row indexes respectively
    * %mcolumn%, %mrow% - inserts model column and model row indexes respectively
    * The default value of this property is %column% for column header and
    * %row% for row header.
    */
    Q_PROPERTY( QString autoHeaderNamingMask READ autoHeaderNamingMask WRITE setAutoHeaderNamingMask )

    /*!
    * Specifies the resize cursor for the header.  This cursor will be displayed
    * whenever the mouse pointer is between two sections of header.
    */
    Q_PROPERTY( QCursor resizeCursor READ resizeCursor WRITE setResizeCursor )
    //@}

public:
    /*!
    * \internal
    * Constructor for the header.  Meant only for use by QicsTable internals.
    */
    QicsHeader(QicsGridInfo *info, QObject *parent = 0, bool forward_signals = false);

    /*!
    * \internal
    * Destructor
    */
    virtual ~QicsHeader();

    /*!
    * Returns a reference to an internal flyweight QicsRow object
    * that refers to row \a rownum of the header.
    */
    virtual QicsRow &rowRef(int rownum) const;
    /*!
    * Returns a newly allocated row object for row \a idx of the header.
    * If \a follow_model is \b true, the row object will always refer to the
    * row in the data model that is currently being displayed in
    * row \a rownum, even if rows of the table are reordered.
    * If follow_model is \b false, the row will always refer to
    * the visual row \a rownum, but the underlying row in the
    * data model may change.
    */
    virtual QicsRow *row(int idx, bool follow_model = true) const;

    /*!
    * Returns a reference to an internal flyweight QicsColumn object
    * that refers to row \a colnum of the header.
    */
    virtual QicsColumn &columnRef(int colnum) const;
    /*!
    * Returns a newly allocated column object for column \a idx of the header.
    * If \a follow_model is \b true, the column object will always refer to the
    * column in the data model that is currently being displayed in
    * column \a colnum, even if columns of the table are reordered.
    * If follow_model is \b false, the column will always refer to
    * the visual column \a colnum, but the underlying column in the
    * data model may change.
    */
    virtual QicsColumn *column(int idx, bool follow_model = true) const;

    /*!
    * Returns a reference to an internal flyweight QicsCell object
    * that refers to cell (\a rownum, \a colnum) of the header.
    */
    virtual QicsCell &cellRef(int rownum, int colnum) const;
    /*!
    * Returns a newly allocated cell object for cell (\a row, \a col ) of the header.
    * If \a follow_model is \b true, the cell object will always refer to the
    * cell in the data model that is currently being displayed in
    * cell (\a rownum, \a colnum), even if rows and columns of the table are
    * reordered.  If follow_model is \b false, the cell will always refer to
    * the visual cell (\a rownum, \a colnum), but the underlying cell in the
    * data model may change.
    */
    virtual QicsCell *cell(int row, int col, bool follow_model = true) const;

    /*!
    * Returns the header's current policy regarding user resizing.
    * If \b true, users may resize rows or columns (depending on
    * the type of header).  If \b false, no resizing is allowed.
    * \sa setAllowUserResize()
    */
    bool allowUserResize() const;

    /*!
    * Returns the header's current policy regarding user row or column
    * movement.  If \b true, users may move rows or columns (depending on
    * the type of header).  If \b false, no movement is allowed.
    * \sa setAllowUserMove()
    */
    bool allowUserMove() const;

    /*!
    * Returns the header's current policy regarding user row or column
    * editing.  If \b true, users may edit caption of rows or columns.
    * If \b false, no editing is allowed.
    * \sa setAllowUserEdit()
    */
    bool allowUserEdit() const;

    /*!
    * Returns the header's current policy regarding row or column
    * header resizing.  If \b true, users may resize row or column
    * headers(row header horizontally and column header vertically).
    * If \b false, no resizing is allowed. \sa setAllowHeaderResize()
    */
    bool allowHeaderResize()const;

    /*!
    * Expands the height of row \a row to the number of pixels necessary
    * to completely display all items in the row.  Calling this method
    * a second time will cause the row to revert to its previous height
    * or does nothing (depends from autofit mode).
    */
    void toggleRowHeightExpansion(int row, bool setOverrideCursor = false);

    /*!
    * Expands the width of column \a col to the number of pixels necessary
    * to completely display all items in the column.  Calling this method
    * a second time will cause the column to revert to its previous width
    * or does nothing (depends from autofit mode).
    */
    void toggleColumnWidthExpansion(int col, bool setOverrideCursor = false);

    /*!
    * Returns the header's current auto naming mask.
    * \sa setAutoHeaderNamingMask()
    */
    inline QString autoHeaderNamingMask() const {return m_info->autoHeaderNamingMask();}

    /*!
    * Returns the resize cursor for the header.
    * \sa setResizeCursor()
    */
    const QCursor &resizeCursor() const;

    /*!
    * Returns a reference to an internal flyweight QicsCellRegion object
    * that refers to cells in \a region of the grid.
    */
    inline QicsCellRegion& cellRegionRef(const QicsRegion &region) {return *cellRegion(region);}

    /*!
    * Returns a reference to an internal flyweight QicsCellRegion object
    * that refers to cells in region \a begin_row, \a begin_col, \a end_row, \a end_col of the grid.
    */
    inline QicsCellRegion& cellRegionRef(int begin_row, int begin_col, int end_row, int end_col)
    { return cellRegionRef(QicsRegion(begin_row, begin_col, end_row, end_col)); }

    /*!
    * Returns a pointer to an internal flyweight QicsCellRegion object
    * that refers to cells in \a region of the grid.
    */
    QicsCellRegion* cellRegion(const QicsRegion &region);

    /*!
    * Returns a pointer to an internal flyweight QicsCellRegion object
    * that refers to cells in region \a begin_row, \a begin_col, \a end_row, \a end_col of the grid.
    */
    inline QicsCellRegion* cellRegion(int begin_row, int begin_col, int end_row, int end_col)
    { return cellRegion(QicsRegion(begin_row, begin_col, end_row, end_col)); }

public slots:
    /*!
    * Sets the header's current policy regarding user resizing.
    * If \b true, users may resize rows or columns (depending on
    * the type of header).  If \b false, no resizing is allowed.
    * \sa allowUserResize()
    */
    void setAllowUserResize(bool b);

    /*!
    * Sets the header's current policy regarding user row or column
    * movement.  If \b true, user may move rows or columns (depending on
    * the type of header).User move operations work like all drag&drop
    * operations in QicsTable - with Middle button of mouse.
    * If \b false, no movement is allowed.
    * \sa allowUserMove()
    */
    void setAllowUserMove(bool b);

    /*!
    * Sets the header's current policy regarding user row or column
    * editing.  If \b true, user may edit caption of rows or columns
    * If \b false, no editing is allowed.
    * \sa allowUserEdit()
    */
    void setAllowUserEdit(bool b);

    /*!
    * Sets the header's current policy regarding header resizing.
    * If \b true, users may resize row or column headers (depending on
    * the type of header).  If \b false, no resizing is allowed.
    * \sa allowHeaderResize()
    */

    void setAllowHeaderResize(bool b);

    /*!
    * Sets the header's current auto naming mask
    * \sa autoHeaderNamingMask()
    */

    inline void setAutoHeaderNamingMask(const QString &mask) {m_info->setAutoHeaderNamingMask(mask);}

    /*!
    * Sets the resize cursor for the header.
    * \sa resizeCursor()
    */
    void setResizeCursor(const QCursor &cursor);

    /*!
    * Begins an edit of cell (\a row, \a col ).  Returns \b true if
    * the cell successfully begins an edit operation, \b false otherwise.
    */
    bool editCell(int row, int col);

signals:
    /*!
    * Signals when the user has finished resizing a row or column.
    * \param idx the index of the row or column that was resized
    * \param old_size the old height/width of the row/column
    * \param new_size the new height/width of the row/column
    * \param type row or column header
    */
    void sizeChange(int idx, int old_size, int new_size, Qics::QicsHeaderType type);

    /*!
    * Signals when the user is in the process of interactively resizing
    * a cell by dragging the mouse.
    * \param idx the index of the row or column that is being resized
    * \param position the X (if a column header) or Y (if a row header) location of the
    * cursor in the grid
    * \param type row or column header
    */
    void resizeInProgress(int idx, int position, Qics::QicsHeaderType type);

    /*!
    * Signals when the user double clicks in the "grip" area between two cells.
    * \param idx the index of the row or column to the top/left of the grip
    *  that was double clicked in
    * \param button the button that was clicked
    * \param type row or column header
    */
    void gripDoubleClicked(int idx, int button, Qics::QicsHeaderType type);

protected slots:
    virtual void connectGrid(QicsScreenGrid *grid);

protected:
    QicsCell *myCell;
    QicsRow *myRow;
    QicsColumn *myColumn;
    QicsCellRegion *myCellRegion;
    QicsHeaderGrid *m_headerGrid;
};

#endif //QICSHEADER_H


