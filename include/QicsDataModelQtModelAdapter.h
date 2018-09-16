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

#ifndef QICSDATAMODELQTMODELADAPTER_H
#define QICSDATAMODELQTMODELADAPTER_H

#include <QAbstractItemModel>

#include "QicsDataModel.h"

/*!
* \class QicsDataModelQtModelAdapter QicsDataModelQtModelAdapter.h
* \brief QicsDataModelQtModelAdapter is the interface for using native Qt
item models as the data source for QicsTable.

Installing the adapter is easy. Here's an example:

\code

// create Qt table data model
QAbstractItemModel *qtmodel = new QAbstractTableModel();

// create an adapter to QicsDataModel
QicsDataModelQtModelAdapter *model = new QicsDataModelQtModelAdapter();
// set qtmodel for the adapter
model->setModel(qtmodel);

// create the table
QicsTable *table = new QicsTable();
// set adapted model for the table
table->setDataModel(model);

\endcode

See the qt_model_adapter example in the \a examples folder.

\b NOTE: QicsDataModelQtModelAdapter deals only with data, not with
attributes. To make attributes from Qt model available for QicsTable,
use QicsQtModelAttributeController.

*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsDataModelQtModelAdapter : public QicsDataModel
{
    Q_OBJECT
public:
    QicsDataModelQtModelAdapter(QObject *parent = 0, QAbstractItemModel *qt4Model = 0);
    virtual ~QicsDataModelQtModelAdapter();

    /*!
    * Sets the model for the view to present.
    */
    void setModel(QAbstractItemModel *qt4Model);

    /*!
    * Returns the model that this view is presenting.
    */
    inline QAbstractItemModel *model() const {return m_qt4Model;}

    /*!
    * Returns the index of the item in the model specified by the given row, column
    */
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    /* QicsDataModel API */
    virtual const QicsDataItem *item(int row, int col) const;
    virtual const QicsDataItem *item(int row, int col, Qt::ItemDataRole role) const;
    virtual QicsDataModelRow rowItems(int row, int first_col = 0, int last_col = -1) const;
    virtual QicsDataModelColumn columnItems(int col, int first_row = 0, int last_row = -1) const;
    virtual void setRowItems(int row, const QicsDataModelRow &v);
    virtual void setColumnItems(int col, const QicsDataModelColumn &v);

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
    virtual void setItem(int row, int col, const QicsDataItem &item);
    virtual void setItem(int row, int col, const QicsDataItem &item, Qt::ItemDataRole role);
    /*!
    *  Inserts \a number_of_rows rows in to the model, beginning at
    * \a starting_position.  Initially, the new rows have no data.
    *  To populate the new rows, the programmer should call
    * either #setItem() for each item, or #setRowItems() for entire rows.
    *
    * Implementers are required to emit #rowsInserted and #modelSizeChanged
    * signals.
    */
    virtual void insertRows(int number_of_rows, int starting_position);
    /*!
    * Inserts \a number_of_columns columns in to the model, beginning at
    * \a starting_position.  Initially, the new columns have no data.
    * To populate the new columns, the programmer should call
    * either #setItem() for each item, or #setColumnItems() for entire columns.
    *
    * Implementers are required to emit #columnsInserted and
    * #modelSizeChanged signals.
    */
    virtual void insertColumns(int number_of_columns, int starting_position);
    /*!
    * Increases size of the model by \a num rows.
    * Initially, the new rows have no data.
    * To populate the new rows, the programmer should call
    * either #setItem() for each item, or setRowItems() for entire rows.
    *
    * Implementers are required to emit #rowsAdded and #modelSizeChanged
    * signals.
    */
    virtual void addRows(int rows);
    /*!
    * Increases size of the model by \a num columns.
    * Initially, the new columns have no data.
    * To populate the new columns, the programmer should call
    * either #setItem() for each item, or #setColumnItems() for entire columns.
    *
    * Implementers are required to emit #columnsAdded and #modelSizeChanged
    * signals.
    */
    virtual void addColumns(int num);
    /*!
    * Removes row \a row from the model.  All rows below the deleted row
    * are moved up.
    *
    * Implementers are required to emit #rowsDeleted and #modelSizeChanged
    * signals.
    *
    * \sa deleteColumn()
    */
    virtual void deleteRow(int row);
    /*!
    * Removes \a num_rows rows from the model, beginning at row \a start_row.
    * All rows below the deleted rows are moved up.
    * \sa deleteColumns()
    *
    * Implementers are required to emit #rowsDeleted and #modelSizeChanged
    * signals.
    */
    virtual void deleteRows(int num_rows, int start_row);
    /*!
    * Removes column \a col from the model.  All columns below the deleted column
    * are moved left.
    *
    * Implementers are required to emit #columnsDeleted and #modelSizeChanged
    * signals.
    *
    * \sa deleteRow()
    */
    virtual void deleteColumn(int col);
    /*!
    * Removes \a num_cols columns from the model, beginning at column \a start_col.
    * All columns below the deleted columns are moved left.
    *
    * Implementers are required to emit #columnsDeleted and #modelSizeChanged
    * signals.
    *
    * \sa deleteRows()
    */
    virtual void deleteColumns(int num_cols, int start_col);
    /*!
    * Removes the QicsDataItem at positon ( \a row , \a col ) by destroying it.
    * If there is no value stored at that location, the method does nothing.
    *
    * Implementers are required to emit the #modelChanged signal in this
    * method.
    */
    virtual void clearItem(int row, int col);
    /*!
    * Clears all of the data items in the model.
    *
    * Implementers are required to emit the #modelChanged and #modelSizeChanged
    * signals in this method.
    */
    virtual void clearModel();

protected slots:
    /*!
    * Handle updates from QAbstractItemView
    */

    /*!
    * emit cellDataChanged signal for all columns
    */
    void handleDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void handleHeaderDataChanged(Qt::Orientation orientation, int first, int last);
    void handleLayoutChanged();

    /*!
    * Including the undocumented ones...
    */
    /*!
    *emit prepareForRowChanges signal
    */
    void handleRowsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    /*!
    * emit rowsInserted signal
    */
    void handleRowsInserted(const QModelIndex &parent, int first, int last);
    /*!
    * emit repareForRowChanges signal
    */
    void handleRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    /*!
    * emit rowsDeleted signal
    */
    void handleRowsRemoved(const QModelIndex &parent, int first, int last);
    /*!
    * emit prepareForColumnChanges signal
    */
    void handleColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    /*!
    * emit columnsInserted signal
    */
    void handleColumnsInserted(const QModelIndex &parent, int first, int last);
    /*!
    * emit prepareForColumnChanges signal
    */
    void handleColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    /*!
    * emit columnsDeleted signal
    */
    void handleColumnsRemoved(const QModelIndex &parent, int first, int last);
    /*!
    * emit modelChanged signal
    */
    void handleModelReset();

protected:
    void connectModelSignals();
    void disconnectModelSignals();
    void recalcModelSize();

private:
    QAbstractItemModel *m_qt4Model;
    static QicsDataDate dateItem;
    static QicsDataDateTime dateTimeItem;
    static QicsDataDouble doubleItem;
    static QicsDataFloat floatItem;
    static QicsDataInt intItem;
    static QicsDataLongLong longLongItem;
    static QicsDataString stringItem;
    static QicsDataTime timeItem;
    static QicsDataBool boolItem;
};

#endif //QICSDATAMODELQTMODELADAPTER_H


