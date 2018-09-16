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

#ifndef QICSDATAMODELDEFAULT_H
#define QICSDATAMODELDEFAULT_H

#include <QString>
#include <QVector>
#include "QicsDataModel.h"
#include "QicsDataItem.h"

typedef QVector<QicsDataItem *> QicsDataItemPV;
typedef QVector<QicsDataItemPV *> QicsDataItemPVPV;

/*!
* \class QicsDataModelDefault QicsDataModelDefault.h
* \brief A fast implementation of the QicsDataModel interface.
*
*    This model implements the QicsDataModel interface with vectors.
*    Sparse models are supported efficently, and there are no unexpected
*    size increases for dense models.
*
*    This model implementation is row-oriented.  By this we mean that
*    certain operations are more efficient when performed on rows than when
*    performed on columns.  These operations include retrieval (#rowItems()
*    is more efficient than #columnItems()), assignment (#setRow() is more
*    efficient than #setColumn()), addition (#addRows() is more efficient
*    than #addColumns()), insertion (#insertRows() is faster than
*    #insertColumns()), and deletion (#deleteRows() is faster than
*    #deleteColumns()).
*
*    Operations on a single item (#item(), #setItem(), #clearItem()) are
*    very efficient.
*
*    Here is an example of data model creation and usage.
\code
QicsDataModel *data_table = new QicsDataModelDefault(100, 100);

for(int row = 0; row < 100; ++row)
{
    for(int col = 0; col < 100; ++col)
    {
        QString str("R%1C%2").arg(row).arg(col);
        data_table->setItem(row, col, str);
    }
}
\endcode
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsDataModelDefault: public QicsDataModel
{
    Q_OBJECT
public:
    /*!
    * Constructor
    */
    QicsDataModelDefault(int num_rows = 0, int num_cols = 0,
        QObject *parent = 0);
    virtual ~QicsDataModelDefault();

    virtual const QicsDataItem *item(int row, int col) const;
    virtual QicsDataModelRow rowItems(int row, int first_col, int last_col) const;
    virtual QicsDataModelColumn columnItems(int col, int first_row, int last_row) const;

    virtual void setColumnItems(int col, const QicsDataModelColumn &v);
    virtual void setRowItems(int row, const QicsDataModelRow &v);

    virtual bool isRowEmpty(int row) const;
    virtual bool isColumnEmpty(int column) const;
    virtual bool isCellEmpty(int row, int col) const;

    /*!
    * Foundry method to create new instances of QicsDataModelDefault.
    * Subclasses of QicsDataModelDefault should implement
    * a similar method.  Methods of this type can be passed to a
    * QicsTable constructor in order to create custom data models
    * for row and column headers.
    */
    static QicsDataModel *create(int num_rows = 0 , int num_cols = 0, QObject *parent = 0);

public slots:
    virtual void setItem(int row, int col, const QicsDataItem &item);

    virtual void insertColumns(int number_of_columns, int starting_position);
    virtual void insertRows(int number_of_rows, int starting_position);
    virtual void addColumns(int num);
    virtual void addRows(int num);

    virtual void clearItem (int row, int col);
    virtual void clearModel();
    virtual void deleteRow (int row);
    virtual void deleteColumn (int col);
    virtual void deleteRows(int num_rows, int start_row);
    virtual void deleteColumns(int num_cols, int start_col);

protected:
    /*!
    * \internal
    *   Removes all the data from the row \a rownum. If the row index is not
    *   valid in this model, or if the row is empty,
    *   no action is performed.
    */
    virtual void clearRow(int rownum);

    /*!
    *\internal
    * The storage vector.  Optimized for the idea that people will
    * create things One ROW at a time, not a column at a time.
    */
    QicsDataItemPVPV myVectorOfRowPointers;
};

#endif //QICSDATAMODELDEFAULT_H


