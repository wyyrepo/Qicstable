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

#ifndef QICSDATAMODEL_H
#define QICSDATAMODEL_H

#include <QVector>
#include <QString>
#include <QObject>
#include <QTextStream>
#include "QicsNamespace.h"
#include "QicsDataItem.h"
#include "QicsRegion.h"


/*! \file */

/*! \typedef QicsDataModelColumn
*
* QicsDataModelColumn represents a column of data.
*
* QicsDataModelColumn is implemented as a vector of pointers
* to QicsDataItem objects.  The programmer should use the usual
* QValueVector methods to add, remove, or access data item objects
* in the column.
*/
typedef QVector<const QicsDataItem *> QicsDataModelColumn;

/*! \typedef QicsDataModelRow
*
* QicsDataModelRow represents a row of data.
*
* QicsDataModelRow is implemented as a vector of pointers
* to QicsDataItem objects.  The programmer should use the usual
* QValueVector methods to add, remove, or access data item objects
* in the row.
*/
typedef QVector<const QicsDataItem *> QicsDataModelRow;

/*!
* \class QicsDataModel QicsDataModel.h
* \brief Abstract API for storing and retrieving table data
*
* QicsDataModel is an abstract class that defines the common API that
* all data model classes must implement.  The data model is used for
* storage and/or retrieval of data that will be displayed in QicsTable
* widgets (and, later, other widgets).  This abstract class has been
* designed so that programmers can subclass QicsDataModel and implement
* their own data storage.  As an example, a programmer may already have
* data stored in C-style arrays or even in a database.  Rather than have
* to extract each data item and add it to the default data model object
* (QicsDataModelDefault), the programmer can create a subclass of
* QicsDataModel that uses the data in place and simply implements the
* necessary data model methods.  More information about implementing a
* new data model class can be found in \ref extend_model.
*
* QicsDataModel represents a two dimensional array of data items.  Each
* data item is an instance of a subclass of QicsDataItem.  There are no
* restrictions on what type of data items can be in each row, column, or
* cell.  That is, individual rows and columns may contain different types
* of data items.  Row and column indices begin at 0 and end at #lastRow()
* and lastColumn().
*
* The default model constructor creates an empty model with no cells.
* The size of the model may be set at creation time by providing the
* desired number of rows and columns to the data model's constructor.
* After creation, the size of the model may be changed by inserting, adding,
* or deleting rows and columns.  The size of the model may be retrieved by
* calling #numRows() and #numColumns().
*
* Operations on the data model can take place on individual cells, rows,
* columns, and the entire table.  The two most basic methods are #item()
* and #setItem().  Part or all of a row or column may be retrieved by calling
* #rowItems() or #columnItems(), and entire row or column item values may
* be set by calling #setRowItems() or #setColumnItems().
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////


class QICS_EXPORT QicsDataModel: public QObject
{
    Q_OBJECT
public:
    /*!
    * Constructor for the class.  Creates a new data model object
    * with \a num_rows rows and \a num_cols columns.
    * All cells in the data model are initially empty.
    */
    QicsDataModel(int num_rows = 0 , int num_cols = 0, QObject *parent = 0);

    /*! \internal
    * Destructor for the class.
    */
    virtual ~QicsDataModel();

    /*!
    * Returns the number of rows in the model.
    */
    inline int numRows() const { return myNumRows; }

    /*!
    * Returns the number of columns in the model.
    */
    inline int numColumns() const { return myNumColumns; }

    /*!
    * Returns the index of the last row in the data model.
    * \sa lastColumn()
    */
    inline int lastRow() const { return (myNumRows - 1); }

    /*!
    * Returns the index of the last column in the data model.
    * \sa lastRow()
    */
    inline int lastColumn() const { return (myNumColumns - 1); }

    /*!
    * Returns \b true if the position (\a row, \a col ) is a valid position
    * in the data model.  Returns \b false if the position is not valid in
    * this model.
    */
    inline bool contains(int row, int col) const;

    /*!
    * Returns a const pointer to the QicsDataItem stored in (\a row, \a col).
    * The object is not a copy, and must not be modified, as modifications
    * will not be communicated to any widgets that are displaying data from
    * this table.  If there is no data item in that location, this method
    * returns 0.
    *
    * The value that is returned from this method is guaranteed to be
    * valid \b only until the next call to QicsDataModel::item().  If you
    * need to use the original item after this point, you should make a
    * copy of the data item using QicsDataItem::clone().
    *
    * A consequence of the above restriction is that if you are implementing
    * this method in a subclass, you do not have to continually allocate
    * new data items each time this method is called.  You can allocate
    * a single object and reuse it each time the method is called.  See
    * \ref cdm_example for an example of this type of usage.
    *
    * \sa #itemString()
    */
    virtual const QicsDataItem *item(int row, int col) const = 0;

    /*!
    * Returns the string representation of the data item stored in (\a row, \a col),
    * or the empty string if there is no data item at that location.
    *
    * \sa #item()
    */
    virtual QString itemString(int row, int col) const;

    /*!
    * This operator is equivalent to #item().  It is included for the convenience
    * of the programmer.
    *
    * \sa #itemString()
    */
    const QicsDataItem *operator()(int row, int col) { return item(row, col); }

    /*!
    * Returns a new vector which contains const pointers to each item from
    * cell (\a row, \a first_col ) to (\a row, \a last_col ).  If
    * \a last_col = -1, the last column will be the value of #lastColumn().
    */
    virtual QicsDataModelRow rowItems(int row, int first_col = 0,
        int last_col = -1) const = 0;

    /*!
    * Returns a new vector which contains const pointers to each item from
    * cell (\a first_row, \a col ) to (\a last_row, \a col ).  If
    * \a last_row = -1, the last row will be the value of #lastRow().
    */
    virtual QicsDataModelColumn columnItems(int col,
        int first_row = 0,
        int last_row = -1) const = 0;

    /*!
    * Sets the data items in row \a row to the values contained
    * in the vector of QicsDataItem pointers \a v.  A copy of each
    * data item is made, so the original items do not need to be
    * maintained for the life of the data model.
    *
    * The nth data item in the vector is put in position (\a row, n).
    * If the vector of items contains less data items than there are columns
    * in the model, the extra cells in the column are unset.  If the vector of
    * items contains more data items than there are columns in the model,
    * the extra data items are ignored.
    *
    * Implementers are required to emit the #modelChanged signal in this
    * method.
    *
    * \sa setColumnItems()
    */
    virtual void setRowItems(int row, const QicsDataModelRow &v) = 0;

    /*!
    * Sets the data items in column \a col to the values contained
    * in the vector of QicsDataItem pointers \a v.  A copy of each
    * data item is made, so the original items do not need to be
    * maintained for the life of the data model.
    *
    * The nth data item in the vector is put in position (n, \a col).
    * If the vector of items contains less data items than there are rows
    * in the model, the extra cells in the column are unset.  If the vector of
    * items contains more data items than there are rows in the model,
    * the extra data items are ignored.
    *
    * Implementers are required to emit the #modelChanged signal in this
    * method.
    *
    * \sa setRowItems()
    */
    virtual void setColumnItems(int col, const QicsDataModelColumn &v) = 0;

    /*!
    * Returns the current value of the emitsSignals flag.  If \b true,
    * the model will emit modelChanged() signals when the model data is modified,
    * and #modelSizeChanged()
    * signals when the size of the data model is changed.  if \b false,
    * no signals will be emitted.
    */
    inline bool emitSignals() const { return m_emitSignals; }

    /*!
    * Controls the output of QicsDataModel signals.  If \a b is \b true,
    * the model will emit #modelChanged()
    * signals when the model data is modified, and #modelSizeChanged()
    * signals when the size of the data model is changed.if \a b is \b false,
    * no signals will be emitted.
    */
    inline void setEmitSignals(bool b)  { m_emitSignals = b; }

    /*!
    * Forces model to emit \a modelChanged(reg) signal.
    * It is intended for notifying about changes after complex updates rather than
    * emit a bunch of signals for each cell being changed (which is too slow).
    * \since 2.3
    */
    inline void notifyRegionChanged(const QicsRegion &reg) { emit modelChanged(reg); }

    /*!
    * Returns true if \a row does not contain any data, false otherwise.
    * Should be reimplemented in real data model.
    * \since 2.2
    */
    virtual bool isRowEmpty(int row) const;

    /*!
    * Returns true if \a column does not contain any data, false otherwise.
    * Should be reimplemented in real data model.
    * \since 2.2
    */
    virtual bool isColumnEmpty(int column) const;

    /*!
    * Returns true if \a cell does not contain any data, false otherwise.
    * Should be reimplemented in real data model.
    * \since 2.4.2
    */
    virtual bool isCellEmpty(int row, int col) const;

    /*!
    * Reads delimited ASCII data from a stream into the model.  The data
    * must be in the format of one line per row, with each column in the
    * row separated by a delimiter character.
    * \arg stream The stream to read from.
    * \arg separator The character that separates each piece of data.
    * \arg start_row Row index of the cell where the data will be loaded.
    *                The first line of data will be loaded into cells beginning
    *                at this row, with subsequent lines loaded into rows
    *                below.
    *
    * \arg start_col Column index of the cell where the data will be loaded.
    *                The first item of each line of data will be loaded into
    *                cells beginning at this column, with subsequent items
    *                loaded into columns to the right.
    * \arg clear_model Specifies if the entire model should be cleared before
    *                  loading the data.
    * \arg add_columns Specifies if the model should extend its column count
    *                  if readed more from a row.
    */
    void readASCII(QTextStream &stream, const char separator = ';',
        int start_row = 0, int start_col = 0,
        bool clear_model = false,
        bool add_columns = true);

    /*!
    * Writes delimited ASCII data from a data model to a stream.  The data
    * is output one line per row, with each column in the row separated by
    * a configurable delimiter character.
    * \arg stream The stream to write to.
    * \arg separator The character that separates each piece of data.
    * \arg start_row Row index of the starting cell to be output.
    * \arg start_col Column index of the starting cell to be output.
    * \arg nrows Number of rows to output.
    * \arg ncols Number of columns to output.
    */
    void writeASCII(QTextStream &stream, const char separator = ';',
        int start_row = 0, int start_col = 0,
        int nrows = -1, int ncols = -1);

    /*!
    * Foundry method to create new instances of a subclass of
    * QicsDataModel.  Subclasses of QicsDataModel should implement
    * a similar method.  Methods of this type can be passed to a
    * QicsTable constructor in order to create custom data models
    * for row and column headers.
    */
    typedef QicsDataModel *(*Foundry)(int num_rows, int num_cols, QObject *parent);

    /*!
    * \internal
    * Internal foundry method to create new instances of a subclass of
    * QicsDataModel.  Methods of this type can be passed to a
    * QicsTable constructor in order to create custom data models
    * for row and column headers.  A pointer to a previously provided
    * "key" is passed as a parameter.
    * This typedef is not exposed as part of the public API.  It has
    * been added because it's necessary for the Python bindings.
    */
    typedef QicsDataModel *(*FoundryWithKey)(int num_rows, int num_cols, void *key);

public slots:
    /*!
    * Sets the value of cell (\a row, \a col) to \a item.
    * This method makes a new copy of \a item and stores it in the model,
    * so the original item does not need to be maintained for the life
    * of the data model.
    *
    * Implementers are required to emit the #modelChanged signal in this
    * method.
    */
    virtual void setItem(int row, int col, const QicsDataItem &item) = 0;

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, int val)
    { setItem(row, col, QicsDataInt(val)); }

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, long val)
    { setItem(row, col, QicsDataLong(val)); }

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, qlonglong val)
    { setItem(row, col, QicsDataLongLong(val)); }
    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, float val)
    { setItem(row, col, QicsDataFloat(val)); }

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, double val)
    { setItem(row, col, QicsDataDouble(val)); }

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, const QString &val)
    { setItem(row, col, QicsDataString(val)); }

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, const char* val)
    { setItem(row, col, QicsDataString(val)); }

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, const QDate &val)
    { setItem(row, col, QicsDataDate(val)); }

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, const QTime &val)
    { setItem(row, col, QicsDataTime(val)); }

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, const QDateTime &val)
    { setItem(row, col, QicsDataDateTime(val)); }

    /*!
    * This method is essentially equivalent to the previous method.
    * \a val is used to create a new QicsDataItem of the appropriate type,
    * and that data item is placed in cell (\a row, \a col ).
    *
    * If this method is reimplemented, implementers are required to emit
    * the #modelChanged signal.
    */
    virtual void setItem(int row, int col, const bool &val)
    { setItem(row, col, QicsDataBool(val)); }

    /*!
    *  Inserts \a number_of_rows rows in to the model, beginning at
    * \a starting_position.  Initially, the new rows have no data.
    *  To populate the new rows, the programmer should call
    * either #setItem() for each item, or #setRowItems() for entire rows.
    *
    * Implementers are required to emit #rowsInserted and #modelSizeChanged
    * signals.
    */
    virtual void insertRows(int number_of_rows, int starting_position) = 0;

    /*!
    * Inserts \a number_of_columns columns in to the model, beginning at
    * \a starting_position.  Initially, the new columns have no data.
    * To populate the new columns, the programmer should call
    * either #setItem() for each item, or #setColumnItems() for entire columns.
    *
    * Implementers are required to emit #columnsInserted and
    * #modelSizeChanged signals.
    */
    virtual void insertColumns(int number_of_columns,
        int starting_position) = 0;

    /*!
    * Increases size of the model by \a num rows.
    * Initially, the new rows have no data.
    * To populate the new rows, the programmer should call
    * either #setItem() for each item, or setRowItems() for entire rows.
    *
    * Implementers are required to emit #rowsAdded and #modelSizeChanged
    * signals.
    */
    virtual void addRows(int rows) = 0;

    /*!
    * Increases size of the model by \a num columns.
    * Initially, the new columns have no data.
    * To populate the new columns, the programmer should call
    * either #setItem() for each item, or #setColumnItems() for entire columns.
    *
    * Implementers are required to emit #columnsAdded and #modelSizeChanged
    * signals.
    */
    virtual void addColumns(int num) = 0;

    /*!
    * Removes row \a row from the model.  All rows below the deleted row
    * are moved up.
    *
    * Implementers are required to emit #rowsDeleted and #modelSizeChanged
    * signals.
    *
    * \sa deleteColumn()
    */
    virtual void deleteRow(int row)  = 0;

    /*!
    * Removes \a num_rows rows from the model, beginning at row \a start_row.
    * All rows below the deleted rows are moved up.
    * \sa deleteColumns()
    *
    * Implementers are required to emit #rowsDeleted and #modelSizeChanged
    * signals.
    */
    virtual void deleteRows(int num_rows, int start_row)  = 0;

    /*!
    * Removes column \a col from the model.  All columns below the deleted column
    * are moved left.
    *
    * Implementers are required to emit #columnsDeleted and #modelSizeChanged
    * signals.
    *
    * \sa deleteRow()
    */
    virtual void deleteColumn(int col) = 0;

    /*!
    * Removes \a num_cols columns from the model, beginning at column \a start_col.
    * All columns below the deleted columns are moved left.
    *
    * Implementers are required to emit #columnsDeleted and #modelSizeChanged
    * signals.
    *
    * \sa deleteRows()
    */
    virtual void deleteColumns(int num_cols, int start_col) = 0;

    /*!
    * Removes the QicsDataItem at positon ( \a row , \a col ) by destroying it.
    * If there is no value stored at that location, the method does nothing.
    *
    * Implementers are required to emit the #modelChanged signal in this
    * method.
    */
    virtual void clearItem(int row, int col)  = 0;

    /*!
    * Clears all of the data items in the model.
    *
    * Implementers are required to emit the #modelChanged and #modelSizeChanged
    * signals in this method.
    */
    virtual void clearModel() = 0;

signals:
    /*!
    * This signal is emitted when any data item in the model changes value.
    * it is not emitted on creation or destruction of the model. It is not
    * emitted if the model changes size.
    * The argument is the region of the data model that changed value.
    * \sa modelSizeChanged
    */
    void modelChanged(const QicsRegion &reg);

    /*!
    * This signal is emitted when the data model changes size.
    */
    void modelSizeChanged(int new_number_of_rows, int new_number_of_columns);

    /*!
    * This signal is emitted when the data model changes size. Cause smart redraw.
    */
    void modelSizeChanged(int position, int num_items_added_removed, Qics::QicsIndexType row_or_col);

    /*!
    * This signal is emitted when rows are inserted into the model.
    */
    void rowsInserted(int number_of_rows, int starting_position);
    /*!
    * This signal is emitted when columns are inserted into the model.
    */
    void columnsInserted(int number_of_columns, int starting_position);

    /*!
    * This signal is emitted when rows are deleted from the model.
    */
    void rowsDeleted(int number_of_rows, int starting_position);
    /*!
    * This signal is emitted when columns are deleted from the model.
    */
    void columnsDeleted(int number_of_columns, int starting_position);

    /*!
    * This signal is emitted when rows are added to the end of the model.
    */
    void rowsAdded(int number_of_rows);
    /*!
    * This signal is emitted when rows are added to the end of the model.
    */
    void columnsAdded(int number_of_columns);
    /*!
    * This signal is emitted when value of the particular cell is changed.
    */
    void cellValueChanged( int row, int col );
    /*!
    * This signal is emitted when data in the model start to change rows size.
    * it is not emitted on creation or destruction of the model.
    * If \a num_rows < 0 rows ere preparing for deletion, otherwise inserting.
    */
    void prepareForRowChanges(int num_rows, int start_row);
    /*!
    * This signal is emitted when data in the model start to change columns size.
    * it is not emitted on creation or destruction of the model.
    * If \a num_cols < 0 columns are preparing for deletion, otherwise inserting.
    */
    void prepareForColumnChanges(int num_cols, int start_col);

protected:
    /*!
    * \internal
    * Internal method to modify the count of the number of rows in the
    * model.  This method should be used only in the implementation of
    * subclasses.  Other programmers should modify the number of rows
    * by calling #insertRow() or #deleteRow().
    */
    inline void setNumRows(int nrows) { myNumRows = nrows; }
    /*!
    * \internal
    * Internal method to modify the count of the number of columns in the
    * model.  This method should be used only in the implementation of
    * subclasses.  Other programmers should modify the number of columns
    * by calling #insertColumn() or #deleteColumn().
    */
    inline void setNumColumns(int ncols) { myNumColumns = ncols; }

    /*!
    * \internal
    * The number of columns in the model
    */
    int myNumRows;

    /*!
    * \internal
    * The number of rows in the model
    */
    int myNumColumns;

    /*!
    * \internal
    * Do we emit signals when things change?
    */
    bool m_emitSignals;

private:
    /*!
    * \internal
    * This method should never be called.  That's why it's private.
    */
    QicsDataModel(const QicsDataModel&)
        : QObject()
    {
    }
    /*!
    * \internal
    * This method should never be called.  That's why it's private.
    */
    QicsDataModel& operator= (const QicsDataModel& )
    {
        return *this;
    }
};

inline bool QicsDataModel::contains(int row, int col) const
{
    return ((col > lastColumn() || row > lastRow() ||
        row < 0 || col < 0)? false: true );
}

#endif //QICSDATAMODEL_H


