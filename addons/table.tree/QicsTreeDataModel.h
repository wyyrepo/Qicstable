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

#ifndef QICSTREEDATAMODEL_H
#define QICSTREEDATAMODEL_H

#include <QicsDataModelDefault.h>
#include <QicsNamespace.h>

/*!
* \class QicsViewTreeDataModel QicsTreeDataModel.h
* \brief Internal proxy model class.
*
*  %QicsViewTreeDataModel is a proxy model class for transferring data between
*  view (QicsTreeTable) and actual data model (QicsDataModel).
*
*  %QicsViewTreeDataModel is responsible to hold and manage so-called
*  special rows. Special rows exist in view only and do not impact actual data
*  stored in model. Each special row has its internal index specifies row's
*  absolute position in the view among rows with data, and QicsSpecialRowData-based
*  additional info. Special rows are responsible for interaction with user
*  (by handling mouse events), and building of tree-like structures with
*  appropriate painting.
*
*  For each tree node entry, QicsExpandableStaticRowData is used which can
*  contain child nodes and perform some interaction with user. It is responsible
*  for showing/hiding of children when requested by a user, or when a filter
*  has been installed. Special rows are created during grouping of the tree,
*  and removed when grouping is off. For performance reasons, these rows are
*  physically being added to the end of the %QicsViewTreeDataModel data model
*  (to avoid unnecessary insertions and make data translation faster).
*  While grouping, they are changing their visual indexes to become organized
*  into tree-like structure.
*
*  This class is internal and should not be used anywhere
*  except in QicsTreeTable.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsSpecialRowData;

class QICS_EXPORT QicsViewTreeDataModel : public QicsDataModelDefault
{
    Q_OBJECT
public:
    /*!
    *  Constructor. Creates empty model and connects itself to parent model
    *  which holds actual data.
    */
    QicsViewTreeDataModel(QicsDataModel *parent, bool shiftColumn = false);

    /*!
    *  Destructor for the class.
    */
    virtual ~QicsViewTreeDataModel();

    /*!
    *  Returns a QicsDataItem item for the given row \a row and column \a col.
    *  Returns 0 if there is no item.
    */
    virtual const QicsDataItem* item (int row, int col) const;

    /*!
    *  Sets QicsDataItem \a item for the given row \a row and column \a col.
    */
    virtual void setItem (int row, int col, const QicsDataItem &item);

    /*!
    *  Adds special row with \a index and matching \a rowData.
    */
    void addSpecialRow(int index, QicsSpecialRowData *rowData = 0);

    /*!
    *  Removes special row with \a index.
    */
    void removeSpecialRow(int index);

    /*!
    *  Removes all the special rows.
    */
    void removeSpecialRows();

    /*!
    *  Returns index of special row matching \a rowData, or -1 if there is no such row.
    */
    inline int specialRowIndex(QicsSpecialRowData* rowData) const
    {return m_specRows.values().contains(rowData) ? m_specRows.key(rowData) : -1; }

    /*!
    *  Returns a pointer to QicsSpecialRowData for special row with \a index,
    *  or 0 if there is no special row with such index.
    */
    QicsSpecialRowData* specialRowData(int index) const
    {
        QMap<int, QicsSpecialRowData*>::const_iterator it = m_specRows.find(index);
        return it == m_specRows.end() ? 0 : *it;
    }

protected:
    /*! \internal
    *  Called before inserting special rows to increase performance.
    */
    void startSpecialInsertion();

    /*! \internal
    *  Called to perform actual inserting of special rows to increase performance.
    */
    void finalizeSpecialInsertion();

    /*! \internal
    *  Denotes column shift by 1 if \a set.
    */
    inline void setColumnShift(bool set) { m_shiftColumn = set; }

protected slots:
    /*! \internal
    *  Inserts \a count rows starting at row \a index.
    */
    virtual	void insertRows(int count, int index);

    /*! \internal
    *  Removes \a count rows starting at row \a index.
    */
    virtual	void deleteRows(int count, int index);

    /*! \internal
    *  Appends \a count rows in the end of row list.
    */
    virtual	void addRows(int count);

    /*! \internal
    *  Appends \a count columns to existing columns list.
    */
    virtual	void addColumns(int count);

    /*! \internal
    *  Inserts \a count columns starting at column \a index.
    */
    virtual	void insertColumns(int count, int index);

    /*! \internal
    *  Removes \a count columns starting at column \a index.
    */
    virtual	void deleteColumns(int count, int index);

    /*! \internal
    *  Called when cell at row \a row and column \a col
    *  changes it's value.
    */
    void onCellValueChanged(int row, int col);

protected:
    QMap<int, QicsSpecialRowData*> m_specRows;
    QicsDataModel *m_model;
    int sr_count;
    int sr_start;
    bool m_shiftColumn;
    bool m_delayInsertSignals;

private:
    friend class QicsTreeTable;
};


#endif //QICSTREEDATAMODEL_H


