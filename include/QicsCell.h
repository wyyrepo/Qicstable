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

#ifndef QICSCELL_H
#define QICSCELL_H

#include <QVector>
#include "QicsCellCommon.h"

class QicsTable;
class QicsHeader;
class QicsDataModel;
class QicsDataItem;

/////////////////////////////////////////////////////////////////////////////

/*! \class QicsCell
* \nosubgrouping
* \brief A lightweight object permitting access to an indvidual cell's attributes
*
* QicsCell is the programmer's access point for all attributes of individual
* cells.  All attributes of individual cells can be set using this class.
*
* QicsCell is a flyweight class representing a single cell in the table.
* Instances of this object are created only when necessary -- there is no
* large array of cell objects contained in the table.  Rather, they are created
* directly (via the public constructor) or indirectly (via QicsTable::cell(),
* QicsTable::cellRef(), QicsHeader::cell(), or QicsHeader::cellRef()) by the
* programmer when needed.

* Persistent objects, created by the public constructor,
* by QicsTable::cell() and by QicsHeader::cell(), are valid until they are
* explicitly destroyed, or until the table itself is destroyed.  This brings up
* the possibility that rows and/or columns in the table may be inserted, deleted,
* or reordered while QicsCell objects persist.  Because of this possibility,
* QicsCell objects can be created so that they always refer to the same
* cell in the data model, or so that they always refer to the same cell in
* the view (i.e. the table widget).
*
* Any attribute settings that are made while using a cell object that follows
* the model are considered to be <i>model attribute settings</i>.  These
* settings will always pertain to this model cell, even if rows and columns
* are reordered.  Attribute settings that are made while using a cell object
* that does not follow the model are considered to be
* <i>visual attribute settings</i>.  These settings will always pertain to
* the visual cell, regardless of what model cell is being displayed in that
* cell.
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsCell: public QicsCellCommon
{
    Q_OBJECT
public:
    /** @name Cell Properties
    */

    //@{
    /*!
    * Specifies the visual row index of this object.  The visual
    * index is the index of this object as it appears in the associated
    * table.  This value is not necessarily the correct index into the
    * associated data model (rows may have been reordered).  Use the
    * \link #modelRowIndex "modelRowIndex" \endlink property for
    * the correct model index.
    */
    Q_PROPERTY( int rowIndex READ rowIndex WRITE setRowIndex )

    /*!
    * Specifies the model row index of this object.  The model
    * index is the index of this object in the associated data model.
    * This value is not necessarily the correct index into the
    * current visual presentation of the table (rows may have been reordered).
    * Use the \link #rowIndex "rowIndex" \endlink property for
    * the correct visual index.
    */
    Q_PROPERTY( int modelRowIndex READ modelRowIndex )

    /*!
    * Specifies the visual column index of this object.  The visual
    * index is the index of this object as it appears in the associated
    * table.  This value is not necessarily the correct index into the
    * associated data model (columns may have been reordered).  Use the
    * \link #modelColumnIndex "modelColumnIndex" \endlink property for
    * the correct model index.
    */
    Q_PROPERTY( int columnIndex READ columnIndex WRITE setColumnIndex )

    /*!
    * Specifies the model column index of this object.  The model
    * index is the index of this object in the associated data model.
    * This value is not necessarily the correct index into the
    * current visual presentation of the table (columns may have been reordered).
    * Use the \link #columnIndex "columnIndex" \endlink property for
    * the correct visual index.
    */
    Q_PROPERTY( int modelColumnIndex READ modelColumnIndex )

    /*!
    * Specifies the width of this cell's column in pixels.  If the width of this
    * cell's column has been set in character units, retrieving this property
    * will return the equivalent width in pixels.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int widthInPixels READ widthInPixels WRITE setWidthInPixels )
    /*!
    * Specifies the width of this cell's column in character units.
    * This means that setting this value to 10 will change the width of
    * this cell's column to be just large enough to display 10 characters
    * of text (plus the margin size and border width).
    * If the width of this cell's column has been set in pixels , retrieving
    * this property will return the equivalent width in character units
    * based on the current font.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int widthInChars READ widthInChars WRITE setWidthInChars )
    /*!
    * Specifies the minimum width of this cell's column in pixels.
    * If the minimum width of this cell's column has been set in character
    * units, retrieving this property will return the equivalent width
    * in pixels.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int minWidthInPixels READ minWidthInPixels WRITE setMinWidthInPixels )
    /*!
    * Specifies the minimum width of this cell's column in character units.
    * This means that setting this value to 10 will change the minimum width
    * of this cell's column to be just large enough to display 10 characters
    * of text (plus the margin size and border width).
    * If the minimum width of this cell's column has been set in pixels,
    * retrieving this property will return the equivalent width in character
    * units based on the current font.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int minWidthInChars READ minWidthInChars WRITE setMinWidthInChars )

    /*!
    * Specifies the maximum width of this cell's column in pixels.
    * If the maximum width of this cell's column has been set in character
    * units, retrieving this property will return the equivalent width
    * in pixels.
    *
    * The default value of this property is QicsLAST_COLUMN
    */
    Q_PROPERTY( int maxWidthInPixels READ maxWidthInPixels WRITE setMaxWidthInPixels )

    /*!
    * Specifies the maximum width of this cell's column in character units.

    */
    Q_PROPERTY( int maxWidthInChars READ maxWidthInChars WRITE setMaxWidthInChars )

    /*!
    * Specifies the maximum height of this cell's row in pixels.
    * If the maximum height of this cell's row has been set in character units,
    * retrieving this property will return the equivalent height in pixels.
    *
    * The default value of this property is QicsLAST_ROW
    */
    Q_PROPERTY( int maxHeightInPixels READ maxHeightInPixels WRITE setMaxHeightInPixels )
    /*!
    * Specifies the maximum height of this cell's row in character units.
    */
    Q_PROPERTY( int maxHeightInChars READ maxHeightInChars WRITE setMaxHeightInChars )

    /*!
    * Specifies the height of this cell's row in pixels.  If the height of this
    * cell's row has been set in character units, retrieving this property will return
    * the equivalent height in pixels.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int heightInPixels READ heightInPixels WRITE setHeightInPixels )
    /*!
    * Specifies the height of this cell's row in character units.
    * This means that setting this value to 1 will change the height of
    * this cell's row to be just large enough to display one line of text (plus
    * the margin size and border width).
    * If the height of this cell's row has been set in pixels, retrieving
    * this property will return the equivalent height in character units
    * based on the current font.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int heightInChars READ heightInChars WRITE setHeightInChars )
    /*!
    * Specifies the minimum height of this cell's row in pixels.
    * If the minimum height of this cell's row has been set in character units,
    * retrieving this property will return the equivalent height in pixels.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int minHeightInPixels READ minHeightInPixels WRITE setMinHeightInPixels )
    /*!
    * Specifies the minimum height of this cell's row in character units.
    * This means that setting this value to 1 will change the minimum height of
    * this cell's row to be just large enough to display one line of text (plus
    * the margin size and border width).
    * If the minimum height of this row has been set in pixels, retrieving
    * this property will return the equivalent height in character units
    * based on the current font.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int minHeightInChars READ minHeightInChars WRITE setMinHeightInChars )
    //@}

public:
    /*!
    * Constructor for the class.
    * \param row row index
    * \param col column index
    * \param table the cell's table
    * \param follow_model indices follow model?
    */
    QicsCell(int row, int col, QicsTable *table, bool follow_model = true);

    /*! \internal
    * Constructor that should only be called by internal table objects
    */
    QicsCell(int row, int column, QicsGridInfo *info, bool follow_model = true, QObject *parent = 0);

    virtual void setInfo(QicsGridInfo *info);

    inline QicsGridInfo& gridInfo() const {return *m_info;}

    /*!
    * Returns the visual row index of this cell.
    *  See the \link #rowIndex "rowIndex" \endlink
    *  property for details.
    * \sa setRowIndex() modelRowIndex()
    */
    int rowIndex() const;

    /*!
    * Returns the visual column index of this cell.
    *  See the \link #columnIndex "columnIndex" \endlink
    *  property for details.
    * \sa setColumnIndex() modelColumnIndex()
    */
    int columnIndex() const;

    /*!
    * Returns the model row index of this cell.
    *  See the \link #modelRowIndex "modelRowIndex" \endlink
    *  property for details.
    * \sa rowIndex()
    */
    inline int modelRowIndex() const {return myRow;}

    /*!
    * Returns the model column index of this cell.
    *  See the \link #modelColumnIndex "modelColumnIndex" \endlink
    *  property for details.
    * \sa columnIndex()
    */
    inline int modelColumnIndex() const {return myColumn;}

    /*!
    * Returns \b true if the cell indices are valid, \b false if they
    * are not.  Cell indices may become invalid if rows or columns
    * in the data model are deleted.
    */
    bool isValid() const;

    /*!
    * Returns the data value for this cell.
    * \sa dataString()
    * \sa setDataValue()
    */
    const QicsDataItem *dataValue() const;

    /*!
    * Returns the data value for this cell as QString.
    * \sa dataValue()
    * \since 2.4
    */
    QString dataString() const;

    /*!
    * Returns \b true if this cell is the current cell, \b false otherwise.
    */
    bool isCurrent() const;

    /*!
    *  Returns the width of this cell's column in pixels.
    *  See the \link #widthInPixels "widthInPixels" \endlink
    *  property for details.
    * \sa setWidthInPixels()
    */
    int widthInPixels() const;

    /*!
    *  Returns the width of this cell's column in character units.
    *  See the \link #widthInChars "widthInChars" \endlink
    *  property for details.
    * \sa setWidthInChars()
    */
    int widthInChars() const;

    /*!
    *  Returns the minimum width of this cell's column in pixels.
    *  See the \link #minWidthInPixels "minWidthInPixels" \endlink
    *  property for details.
    * \sa setMinWidthInPixels()
    */
    int minWidthInPixels() const;

    /*!
    *  Returns the minimum width of this cell's column in character units.
    *  See the \link #minWidthInChars "minWidthInChars" \endlink
    *  property for details.
    * \sa setMinWidthInChars()
    */
    int minWidthInChars() const;

    /*!
    *  Returns the maximum width of this cell's column in pixels.
    *  See the \link #maxWidthInPixels "maxWidthInPixels" \endlink
    *  property for details.
    * \sa setMaxWidthInPixels()
    */
    int maxWidthInPixels() const;

    /*!
    *  Returns the maximum width of this cell's column in character units.
    *  See the \link #maxWidthInChars "maxWidthInChars" \endlink
    *  property for details.
    * \sa setMaxWidthInChars()
    */
    int maxWidthInChars() const;

    /*!
    *  Returns the height of this cell's row in pixels.
    *  See the \link #heightInPixels "heightInPixels" \endlink
    *  property for details.
    *  \sa setHeightInPixels()
    */
    int heightInPixels() const;

    /*!
    *  Returns the height of this cell's row in character units (i.e. lines).
    *  See the \link #heightInChars "heightInChars" \endlink
    *  property for details.
    *  \sa setHeightInChars()
    */
    int heightInChars() const;

    /*!
    *  Returns the maximum height of this cell's row in pixels.
    *  See the \link #maxHeightInPixels "maxHeightInPixels" \endlink
    *  property for details.
    *  \sa setMaxHeightInPixels()
    */
    int maxHeightInPixels() const;

    /*!
    *  Returns the maximum height of this cell's row in character units
    *  (i.e. lines).
    *  See the \link #maxHeightInChars "maxHeightInChars" \endlink
    *  property for details.
    *  \sa setMaxHeightInChars()
    */
    int maxHeightInChars() const;

    /*!
    *  Returns the minimum height of this cell's row in pixels.
    *  See the \link #minHeightInPixels "minHeightInPixels" \endlink
    *  property for details.
    *  \sa setMinHeightInPixels()
    */
    int minHeightInPixels() const;

    /*!
    *  Returns the minimum height of this cell's row in character units
    *  (i.e. lines).
    *  See the \link #minHeightInChars "minHeightInChars" \endlink
    *  property for details.
    *  \sa setMinHeightInChars()
    */
    int minHeightInChars() const;

    /*!
    * Returns whether this cell is hidden or not.  Cells cannot be hidden
    * individually -- only entire rows or columns may be hidden.
    */
    bool isHidden() const;

    /*!
    * Returns whether this cell is visible or not.  Cells cannot be visible
    * individually -- only entire rows or columns may be visible.
    */
    bool isVisible() const;

    /*!
    * Returns the selected attribute of the cell.
    * See the \link #selected "selected" \endlink property for details.
    * \since 2.2
    */
    virtual bool selected() const;

public slots:
    /*!
    * Sets the row index of this object to \a idx.
    *  See the \link #rowIndex "rowIndex" \endlink
    *  property for details.
    * \sa rowIndex()
    */
    void setRowIndex(int idx);

    /*!
    * Sets the column index of this object to \a idx.
    *  See the \link #columnIndex "columnIndex" \endlink
    *  property for details.
    * \sa columnIndex()
    */
    void setColumnIndex(int idx);

    /*!
    * Sets the data value for this cell.
    * \sa dataValue()
    */
    void setDataValue(const QicsDataItem &val);

    /*!
    * Clears the data value for this cell.
    */
    void clearDataValue();

    /*!
    * Makes this cell span multiple rows and columns.
    * The contents of the cell at the top, left of \a span will be
    * drawn across all cells in \a span.  Two cell spanning regions
    * within a single grid cannot overlap.  If addCellSpan
    * is called with a spanning region that overlaps an existing spanning
    * region, the method will return \b false.  If the new spanning region
    * is accepted, the method will return \b true.
    * \sa removeSpan()
    * \sa spannedRegion()
    */
    bool addSpan(int height, int width);

    /*!
    * Returns spanned region.
    * \sa addSpan()
    * \sa removeSpan()
    */
    QicsRegion spannedRegion();

    /*!
    * Removes the span that contains this cell.
    * \sa addSpan()
    * \sa spannedRegion()
    */
    void removeSpan();

    /*!
    *  Sets the width of this cell's column in pixels.
    *  See the \link #widthInPixels "widthInPixels" \endlink
    *  property for details.
    * \sa widthInPixels()
    */
    void setWidthInPixels(int width);

    /*!
    *  Sets the width of this cell's column in character units.
    *  See the \link #widthInChars "widthInChars" \endlink
    *  property for details.
    * \sa widthInChars()
    */
    void setWidthInChars(int width);

    /*!
    *  Sets the maximum width of this cell's column in pixels.
    *  See the \link #maxWidthInPixels "maxWidthInPixels" \endlink
    *  property for details.
    * \sa maxWidthInPixels()
    */
    void setMaxWidthInPixels(int width);

    /*!
    *  Sets the maximum width of this cell's column in character units.
    *  See the \link #maxWidthInChars "maxWidthInChars" \endlink
    *  property for details.
    * \sa maxWidthInChars()
    */
    void setMaxWidthInChars(int width);

    /*!
    *  Sets the minimum width of this cell's column in pixels.
    *  See the \link #minWidthInPixels "minWidthInPixels" \endlink
    *  property for details.
    * \sa minWidthInPixels()
    */
    void setMinWidthInPixels(int width);

    /*!
    *  Sets the minimum width of this cell's column in character units.
    *  See the \link #minWidthInChars "minWidthInChars" \endlink
    *  property for details.
    * \sa minWidthInChars()
    */
    void setMinWidthInChars(int width);

    /*!
    *  Sets the height of this cell's row in pixels.
    *  See the \link #heightInPixels "heightInPixels" \endlink
    *  property for details.
    * \sa heightInPixels()
    */
    void setHeightInPixels(int height);

    /*!
    *  Sets the height of this cell's row in character units (i.e. lines).
    *  See the \link #heightInChars "heightInChars" \endlink
    *  property for details.
    * \sa heightInChars()
    */
    void setHeightInChars(int height);

    /*!
    *  Sets the minimum height of this cell's row in pixels.
    *  See the \link #minHeightInPixels "minHeightInPixels" \endlink
    *  property for details.
    * \sa minHeightInPixels()
    */
    void setMinHeightInPixels(int height);

    /*!
    *  Sets the minimum height of this cell's row in character units
    *  (i.e. lines).
    *  See the \link #minHeightInChars "minHeightInChars" \endlink
    *  property for details.
    * \sa minHeightInChars()
    */
    void setMinHeightInChars(int height);

    /*!
    *  Sets the maximum height of this cell's row in pixels.
    *  See the \link #maxHeightInPixels "maxHeightInPixels" \endlink
    *  property for details.
    * \sa maxHeightInPixels()
    */
    void setMaxHeightInPixels(int height);

    /*!
    *  Sets the maximum height of this cell's row in character units
    *  (i.e. lines).
    *  See the \link #maxHeightInChars "maxHeightInChars" \endlink
    *  property for details.
    * \sa maxHeightInChars()
    */
    void setMaxHeightInChars(int height);

protected:
    /*!
    * \internal
    * Common initialization routine
    */
    void init();

    virtual void setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val);
    virtual void *getAttr(QicsCellStyle::QicsCellStyleProperty attr) const;
    virtual void clearAttr(QicsCellStyle::QicsCellStyleProperty attr);

    virtual void setDMMargin(int margin);
    virtual void setDMFont(const QFont &font);
    virtual void setDMBorderWidth(int bw);
    virtual void setDMFitSize(int w, int h);

    /*!
    * \internal
    * Returns the appropriate row index, based on myFollowModel,
    * for use in calls to the style manager and the dimension manager.
    */
    inline int modelOrVisualRowIndex() const;

    /*!
    * \internal
    * Returns the appropriate column index, based on myFollowModel,
    * for use in calls to the style manager and the dimension manager.
    */
    inline int modelOrVisualColumnIndex() const;

private:
    /*!
    * \internal
    * row index of this object
    */
    int myRow;
    /*!
    * \internal
    * column index of this object
    */
    int myColumn;

protected slots:
    void handleModelRowInsert(int nrows, int pos);
    void handleModelColumnInsert(int ncols, int pos);
    void handleModelRowDelete(int nrows, int pos);
    void handleModelColumnDelete(int ncols, int pos);
    void handleOrderChanged(Qics::QicsIndexType type, int *visChange, int size);
    void changeDataModel(QicsDataModel *old_dt, QicsDataModel *new_dt);
    //void gridDeleted();

private:
#ifdef Q_DISABLE_COPY
    QicsCell(const QicsCell& cell);
    QicsCell &operator=(const QicsCell& cell);
#endif
    void commonInit(QicsGridInfo *info);
};

inline int QicsCell::modelOrVisualRowIndex() const
{
    return (myFollowModel ? myRow : m_info->visualRowIndex(myRow));
}

inline int QicsCell::modelOrVisualColumnIndex() const
{
    return (myFollowModel ? myColumn : m_info->visualColumnIndex(myColumn));
}

#endif //QICSCELL_H


