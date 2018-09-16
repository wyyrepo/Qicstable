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

#ifndef QICSCOLUMN_H
#define QICSCOLUMN_H

#include <QVector>
#include "QicsCellCommon.h"
#include "QicsDataModel.h"

class QicsHeader;
class QicsTable;

////////////////////////////////////////////////////////////////////////////////

/*! \class QicsColumn
* \nosubgrouping
* \brief A lightweight object permitting access to an indvidual column's attributes
*
* QicsColumn is the programmer's access point for all attributes of individual
* columns.  All attributes of individual columns can be set using this class.
* When an attribute of a column is set, the new value overrides any previous
* column setting as well as any setting for individual cells in the column.
* When retrieving attributes of a column, remember that the value you are retrieving
* is the common setting for the entire column.  This value may have been explicitly
* set on this individual column, or may be the default value for the grid if no
* value was set for this column.
*
* QicsColumn is a flyweight class representing a single column in the table.
* Instances of this object are created only when necessary -- there is no
* large array of column objects contained in the table.  Rather, they are created
* directly (via the public constructor) or indirectly (via QicsTable::column(),
* QicsTable::columnRef(), QicsHeader::column(), or QicsHeader::columnRef()) by the
* programmer when needed.

* Persistent objects, created by the public constructor,
* by QicsTable::column() and by QicsHeader::column(), are valid until they are
* explicitly destroyed, or until the table itself is destroyed.  This brings up
* the possibility that columns in the table may be inserted, deleted,
* or reordered while QicsColumn objects persist.  Because of this possibility,
* QicsColumn objects can be created so that they always refer to the same
* column in the data model, or so that they always refer to the same column in
* the view (i.e. the table widget).
*
* Any attribute settings that are made while using a column object
* that follows the model are considered to be <i>model attribute settings</i>.
* These settings will always pertain to this model column,
* even if columns are reordered.  Attribute settings that are made
* while using a column object that does not follow the model are
* considered to be <i>visual attribute settings</i>.  These settings will
* always pertain to the visual column, regardless of what model column is being
* displayed in that column of the table.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsColumn: public QicsCellCommon
{
    Q_OBJECT

    friend class QicsTableCommon;
    friend class QicsHeader;
public:
    /** @name Column Properties
    */

    //@{
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
    * Specifies the width of this column in pixels.  If the width of this
    * column has been set in character units, retrieving this property will
    * return the equivalent width in pixels.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int widthInPixels READ widthInPixels WRITE setWidthInPixels )
    /*!
    * Specifies the width of this column in character units.
    * This means that setting this value to 10 will change the width of
    * this column to be just large enough to display 10 characters of text
    * (plus the margin size and border width).
    * If the width of this column has been set by pixel, retrieving
    * this property will return the equivalent width in character units
    * based on the current font.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int widthInChars READ widthInChars WRITE setWidthInChars )
    /*!
    * Specifies the minimum width of this column in pixels.
    * If the minimum width of this column has been set in character units,
    * retrieving this property will return the equivalent width in pixels.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int minWidthInPixels READ minWidthInPixels WRITE setMinWidthInPixels )
    /*!
    * Specifies the minimum width of this column in character units.
    * This means that setting this value to 10 will change the minimum width
    * of this column to be just large enough to display 10 characters of text
    * (plus the margin size and border width).
    * If the minimum width of this column has been set by pixel, retrieving
    * this property will return the equivalent width in character units
    * based on the current font.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int minWidthInChars READ minWidthInChars WRITE setMinWidthInChars )
    /*!
    * Specifies the maximum width of this column in pixels.
    * If the maximum width of this column has been set in character units,
    * retrieving this property will return the equivalent width in pixels.
    *
    * The default value of this property is QicsLAST_ROW
    */
    Q_PROPERTY( int maxWidthInPixels READ maxWidthInPixels WRITE setMaxWidthInPixels )

    /*!
    * Specifies the maximum width of this column in character units.
    */
    Q_PROPERTY( int maxWidthInChars READ maxWidthInChars WRITE setMaxWidthInChars )

    /*!
    * Specifies if this column can be stretched.  Visible columns that can
    * be stretched will grow or shrink as necessary when the width of the
    * table changes.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool stretchable READ stretchable WRITE setStretchable)
    //@}

public:
    /*!
    * Constructor for the class.
    * \param col column index
    * \param table the cell's table
    * \param follow_model indices follow model?
    */
    QicsColumn(int col, QicsTable *table, bool follow_model = true);

    /*!
    * \internal
    * Constructor that should only be called by QicsTable internals.
    */
    QicsColumn(int column, QicsGridInfo *info, bool follow_model = true,
        QObject *parent = 0);

    virtual void setInfo(QicsGridInfo *info);

    /*!
    * Returns the visual column index of this column object.
    *  See the \link #columnIndex "columnIndex" \endlink
    *  property for details.
    * \sa modelColumnIndex() setModelColumnIndex()
    */
    int columnIndex() const;

    /*!
    * Returns the model column index of this column object.
    *  See the \link #modelColumnIndex "modelColumnIndex" \endlink
    *  property for details.
    * \sa columnIndex()
    */
    int modelColumnIndex() const;

    /*!
    * Returns \b true if the column index is valid, \b false if it
    * is not.  A column index may become invalid if columns
    * in the data model are deleted.
    */
    bool isValid() const;

    /*!
    * Returns the data values for this column.
    * \sa setDataValues()
    */
    QicsDataModelColumn dataValues(int first_row = 0, int last_row = -1) const;

    /*!
    *  Returns the width of this column in pixels.
    *  See the \link #widthInPixels "widthInPixels" \endlink
    *  property for details.
    * \sa setWidthInPixels()
    */
    int widthInPixels() const;

    /*!
    *  Returns the width of this column in character units.
    *  See the \link #widthInChars "widthInChars" \endlink
    *  property for details.
    * \sa setWidthInChars()
    */
    int widthInChars() const;

    /*!
    *  Returns the minimum width of this column in pixels.
    *  See the \link #minWidthInPixels "minWidthInPixels" \endlink
    *  property for details.
    * \sa setMinWidthInPixels()
    */
    int minWidthInPixels() const;

    /*!
    *  Returns the minimum width of this column in character units.
    *  See the \link #minWidthInChars "minWidthInChars" \endlink
    *  property for details.
    * \sa setMinWidthInChars()
    */
    int minWidthInChars() const;

    /*!
    *  Returns the maximum width of this column in pixels.
    *  See the \link #maxWidthInPixels "maxWidthInPixels" \endlink
    *  property for details.
    * \sa setMaxWidthInPixels()
    */
    int maxWidthInPixels() const;

    /*!
    *  Returns the maximum width of this column in character units.
    *  See the \link #maxWidthInChars "maxWidthInChars" \endlink
    *  property for details.
    * \sa setMaxWidthInChars()
    */
    int maxWidthInChars() const;

    /*!
    * Returns whether this column is hidden or not.
    */
    bool isHidden() const;

    /*!
    * Returns whether this column is visible or not.
    */
    bool isVisible() const;

    /*!
    * Returns \b true if this column is stretchable, or \b false if it is not.
    * See the \link #stretchable "stretchable" \endlink
    * property for details.
    * \sa setStretchable()
    */
    bool stretchable() const;

    /*!
    * Returns the selected attribute of the row.
    * \since 2.3
    */
    virtual bool selected() const;

public slots:
    /*!
    * Sets the column index of this object to \a idx.
    *  See the \link #columnIndex "columnIndex" \endlink
    *  property for details.
    * \sa columnIndex()
    */
    void setColumnIndex(int idx);

    /*!
    * Sets the data values for this column.
    * \sa dataValues()
    */
    void setDataValues(QicsDataModelColumn &vals);

    /*!
    *  Sets the width of this column in pixels.
    *  See the \link #widthInPixels "widthInPixels" \endlink
    *  property for details.
    * \sa widthInPixels()
    */
    void setWidthInPixels(int width);

    /*!
    *  Sets the width of this column in character units.
    *  See the \link #widthInChars "widthInChars" \endlink
    *  property for details.
    * \sa widthInChars()
    */
    void setWidthInChars(int width);

    /*!
    *  Sets the minimum width of this column in pixels.
    *  See the \link #minWidthInPixels "minWidthInPixels" \endlink
    *  property for details.
    * \sa minWidthInPixels()
    */
    void setMinWidthInPixels(int width);

    /*!
    *  Sets the minimum width of this column in character units.
    *  See the \link #minWidthInChars "minWidthInChars" \endlink
    *  property for details.
    * \sa minWidthInChars()
    */
    void setMinWidthInChars(int width);

    /*!
    *  Sets the maximum width of this column in pixels.
    *  See the \link #maxWidthInPixels "maxWidthInPixels" \endlink
    *  property for details.
    * \sa maxWidthInPixels()
    */
    void setMaxWidthInPixels(int width);

    /*!
    *  Sets the maximum width of this column in character units.
    *  See the \link #maxWidthInChars "maxWidthInChars" \endlink
    *  property for details.
    * \sa maxWidthInChars()
    */
    void setMaxWidthInChars(int width);

    /*!
    * Hides this column.
    */
    void hide();

    /*!
    * Shows this column.
    */
    void show();

    /*!
    * Sets whether this column is stretchable.
    * See the \link #stretchable "stretchable" \endlink
    * property for details.
    * \sa stretchable()
    */
    void setStretchable(bool b);

    /*!
    * Sets whether this column is visible.
    * \sa isVisible()
    */
    void setVisible(bool visible);

    /*!
    * Sets whether this column is hidden.
    * \sa isHidden()
    */
    void setHidden(bool hidden);

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

    /*!
    * \internal
    * Returns the appropriate column index, based on myFollowModel,
    * for use in calls to the style manager and the dimension manager.
    */
    inline int modelOrVisualColumnIndex() const;

    /*!
    * \internal
    * column index of this object
    */
    int myColumn;

protected slots:
    void handleModelColumnInsert(int ncols, int pos);
    void handleModelColumnDelete(int ncols, int pos);
    void handleOrderChanged(Qics::QicsIndexType type, int* visChange, int size);
    void changeDataModel(QicsDataModel *old_dt, QicsDataModel *new_dt);
    //void gridDeleted();

private:
#ifdef Q_DISABLE_COPY
    QicsColumn(const QicsColumn& col);
    QicsColumn &operator=(const QicsColumn& col);
#endif
    void commonInit(QicsGridInfo *info);
};

inline int QicsColumn::modelOrVisualColumnIndex() const
{
    return (myFollowModel ? myColumn : m_info->visualColumnIndex(myColumn));
}

#endif //QICSCOLUMN_H


