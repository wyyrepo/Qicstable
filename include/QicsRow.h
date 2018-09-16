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

#ifndef QICSROW_H
#define QICSROW_H

#include <QVector>
#include "QicsCellCommon.h"
#include "QicsDataModel.h"


class QicsTable;

////////////////////////////////////////////////////////////////////////////////

/*! \class QicsRow
* \nosubgrouping
* \brief A lightweight object permitting access to an indvidual row's attributes
*
* QicsRow is the programmer's access point for all attributes of individual
* rows.  All attributes of individual rows can be set using this class.
* When an attribute of a row is set, the new value overrides any previous
* row setting as well as any setting for individual cells in the row.
* When retrieving attributes of a row, remember that the value you are retrieving
* is the common setting for the entire row.  This value may have been explicitly
* set on this individual row, or may be the default value for the grid if no
* value was set for this row.
*
* QicsRow is a flyweight class representing a single row in the table.
* Instances of this object are created only when necessary -- there is no
* large array of row objects contained in the table.  Rather, they are created
* directly (via the public constructor) or indirectly (via QicsTable::row(),
* QicsTable::rowRef(), QicsHeader::row(), or QicsHeader::rowRef()) by the
* programmer when needed.

* Persistent objects, created by the public constructor,
* by QicsTable::row() and by QicsHeader::row(), are valid until they are
* explicitly destroyed, or until the table itself is destroyed.  This brings up
* the possibility that rows in the table may be inserted, deleted,
* or reordered while QicsRow objects persist.  Because of this possibility,
* QicsRow objects can be created so that they always refer to the same
* row in the data model, or so that they always refer to the same row in
* the view (i.e. the table widget).
*
* Any attribute settings that are made while using a row object
* that follows the model are considered to be <i>model attribute settings</i>.
* These settings will always pertain to this model row,
* even if rows are reordered.  Attribute settings that are made
* while using a row object that does not follow the model are
* considered to be <i>visual attribute settings</i>.  These settings will
* always pertain to the visual row, regardless of what model row is being
* displayed in that row of the table.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////
class QICS_EXPORT QicsRow: public QicsCellCommon
{
    Q_OBJECT

    friend class QicsTableCommon;
    friend class QicsHeader;
public:
    /** @name Row Properties
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
    * Specifies the height of this row in pixels.  If the height of this
    * row has been set in character units, retrieving this property will
    * return the equivalent height in pixels.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int heightInPixels READ heightInPixels WRITE setHeightInPixels )
    /*!
    * Specifies the height of this row in character units.
    * This means that setting this value to 1 will change the height of
    * this row to be just large enough to display one line of text (plus
    * the margin size and border width).
    * If the height of this row has been set by pixel, retrieving
    * this property will return the equivalent height in character units
    * based on the current font.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int heightInChars READ heightInChars WRITE setHeightInChars )
    /*!
    * Specifies the minimum height of this row in pixels.
    * If the minimum height of this row has been set by character units,
    * retrieving this property will return the equivalent height in pixels.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int minHeightInPixels READ minHeightInPixels WRITE setMinHeightInPixels )
    /*!
    * Specifies the minimum height of this row in character units.
    * This means that setting this value to 1 will change the minimum height of
    * this row to be just large enough to display one line of text (plus
    * the margin size and border width).
    * If the minimum height of this row has been set by pixel, retrieving
    * this property will return the equivalent height in character units
    * based on the current font.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int minHeightInChars READ minHeightInChars WRITE setMinHeightInChars )

    /*!
    * Specifies the maximum height of this row in pixels.
    * If the maximum height of this row has been set in character units,
    * retrieving this property will return the equivalent height in pixels.
    */
    Q_PROPERTY( int maxHeightInPixels READ maxHeightInPixels WRITE setMaxHeightInPixels )
    /*!
    * Specifies the maximum height of this row in character units.
    */
    Q_PROPERTY( int maxHeightInChars READ maxHeightInChars WRITE setMaxHeightInChars )
    /*!
    * Specifies if this row can be stretched.  Visible rows that can be
    * stretched will grow or shrink as necessary when the height of the
    * table changes.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool stretchable READ stretchable WRITE setStretchable)
    //@}

public:
    /*!
    * Constructor for the class.
    * \param row row index
    * \param table the cell's table
    * \param follow_model indices follow model?
    */
    QicsRow(int row, QicsTable *table, bool follow_model = true);

    /*!
    * \internal
    * Constructor that should only be called by QicsTable internals.
    */
    QicsRow(int row, QicsGridInfo *info, bool follow_model = true,
        QObject *parent = 0);

    virtual void setInfo(QicsGridInfo *info);

    /*!
    * Returns the visual row index of this row object.
    *  See the \link #rowIndex "rowIndex" \endlink
    *  property for details.
    * \sa setRowIndex() modelRowIndex()
    */
    int rowIndex() const;

    /*!
    * Returns the model row index of this row object.
    *  See the \link #modelRowIndex "modelRowIndex" \endlink
    *  property for details.
    * \sa rowIndex()
    */
    inline int modelRowIndex() const {return myRow; }

    /*!
    * Returns \b true if the row index is valid, \b false if it
    * is not.  A row index may become invalid if rows
    * in the data model are deleted.
    */
    bool isValid() const;

    /*!
    * Returns the data values for this row.
    * \sa setDataValues()
    */
    QicsDataModelRow dataValues(int first_col = 0, int last_col = -1) const;

    /*!
    *  Returns the height of this row in pixels.
    *  See the \link #heightInPixels "heightInPixels" \endlink
    *  property for details.
    *  \sa setHeightInPixels()
    */
    int heightInPixels() const;

    /*!
    *  Returns the height of this row in character units (i.e. lines).
    *  See the \link #heightInChars "heightInChars" \endlink
    *  property for details.
    *  \sa setHeightInChars()
    */
    int heightInChars() const;

    /*!
    *  Returns the minimum height of this row in pixels.
    *  See the \link #minHeightInPixels "minHeightInPixels" \endlink
    *  property for details.
    *  \sa setMinHeightInPixels()
    */
    int minHeightInPixels() const;

    /*!
    *  Returns the minimum height of this row in character units (i.e. lines).
    *  See the \link #minHeightInChars "minHeightInChars" \endlink
    *  property for details.
    *  \sa setMinHeightInChars()
    */
    int minHeightInChars() const;

    /*!
    *  Returns the maximum height of this row in pixels.
    *  See the \link #maxHeightInPixels "maxHeightInPixels" \endlink
    *  property for details.
    *  \sa setMaxHeightInPixels()
    */
    int maxHeightInPixels() const;
    /*!
    *  Returns the maximum height of this row in character units (i.e. lines).
    *  See the \link #maxHeightInChars "maxHeightInChars" \endlink
    *  property for details.
    *  \sa setMaxHeightInChars()
    */
    int maxHeightInChars() const;

    /*!
    * Returns whether this row is hidden or not.
    */
    bool isHidden() const;

    /*!
    * Returns whether this row is visible or not.
    */
    bool isVisible() const;

    /*!
    * Returns whether this row is filtered or not.
    */
    bool isFiltered() const;

    /*!
    * Returns \b true if this row is stretchable, or \b false if it is not.
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
    * Sets the row index of this object to \a idx.
    *  See the \link #rowIndex "rowIndex" \endlink
    *  property for details.
    * \sa rowIndex()
    */
    void setRowIndex(int idx);

    /*!
    * Sets the data values for this row.
    * \sa dataValues()
    */
    void setDataValues(QicsDataModelRow &vals);

    /*!
    *  Sets the height of this row in pixels.
    *  See the \link #heightInPixels "heightInPixels" \endlink
    *  property for details.
    * \sa heightInPixels()
    */
    void setHeightInPixels(int height);

    /*!
    *  Sets the height of this row in character units (i.e. lines).
    *  See the \link #heightInChars "heightInChars" \endlink
    *  property for details.
    * \sa heightInChars()
    */
    void setHeightInChars(int height);

    /*!
    *  Sets the minimum height of this row in pixels.
    *  See the \link #minHeightInPixels "minHeightInPixels" \endlink
    *  property for details.
    * \sa minHeightInPixels()
    */
    void setMinHeightInPixels(int height);

    /*!
    *  Sets the minimum height of this row in character units (i.e. lines).
    *  See the \link #minHeightInChars "minHeightInChars" \endlink
    *  property for details.
    * \sa minHeightInChars()
    */
    void setMinHeightInChars(int height);

    /*!
    *  Sets the maximum height of this row in pixels.
    *  See the \link #maxHeightInPixels "maxHeightInPixels" \endlink
    *  property for details.
    * \sa maxHeightInPixels()
    */
    void setMaxHeightInPixels(int height);

    /*!
    *  Sets the maximum height of this row in character units (i.e. lines).
    *  See the \link #maxHeightInChars "maxHeightInChars" \endlink
    *  property for details.
    * \sa maxHeightInChars()
    */
    void setMaxHeightInChars(int height);

    /*!
    * Hides this row.
    */
    void hide();

    /*!
    * Shows this row.
    */
    void show();

    /*!
    * Sets whether this row is stretchable.
    * See the \link #stretchable "stretchable" \endlink
    * property for details.
    * \sa stretchable()
    */
    void setStretchable(bool b);

    /*!
    * Sets whether this row is visible.
    * \sa isVisible()
    */
    void setVisible(bool visible);

    /*!
    * Sets whether this row is hidden.
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
    * Returns the appropriate row index, based on myFollowModel,
    * for use in calls to the style manager and the dimension manager.
    */
    inline int modelOrVisualRowIndex() const
    { return (myFollowModel ? myRow : m_info->visualRowIndex(myRow)); }

    /*!
    * \internal
    * row index of this object
    */
    int myRow;

protected slots:
    void handleModelRowInsert(int nrows, int pos);
    void handleModelRowDelete(int nrows, int pos);
    void handleOrderChanged(Qics::QicsIndexType type, int* visChange, int size);
    void changeDataModel(QicsDataModel *old_dt, QicsDataModel *new_dt);
    //void gridDeleted();

private:
#ifdef Q_DISABLE_COPY
    QicsRow(const QicsRow& row);
    QicsRow &operator=(const QicsRow& row);
#endif
    void commonInit(QicsGridInfo *info);
};

#endif //QICSROW_H


