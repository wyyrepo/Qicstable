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

#include <QicsTreeDataModel.h>

#include <QicsTreeTable.h>
#include <QicsSpecialRowData.h>



QicsViewTreeDataModel::QicsViewTreeDataModel(QicsDataModel *parent, bool shiftColumn)
    : QicsDataModelDefault(parent->numRows(), parent->numColumns() + shiftColumn, parent),
        m_shiftColumn(shiftColumn)
{
    m_delayInsertSignals = false;
    m_model = parent;

    connect(parent, SIGNAL(rowsInserted(int,int)), this, SLOT(insertRows(int,int)));
    connect(parent, SIGNAL(rowsDeleted(int,int)), this, SLOT(deleteRows(int,int)));
    connect(parent, SIGNAL(rowsAdded(int)), this, SLOT(addRows(int)));

    connect(parent, SIGNAL(columnsInserted(int,int)), this, SLOT(insertColumns(int,int)));
    connect(parent, SIGNAL(columnsDeleted(int,int)), this, SLOT(deleteColumns(int,int)));
    connect(parent, SIGNAL(columnsAdded(int)), this, SLOT(addColumns(int)));

    connect(parent, SIGNAL(cellValueChanged(int,int)), this, SLOT(onCellValueChanged(int,int)));
    connect(parent, SIGNAL(modelChanged(QicsRegion)), this, SIGNAL(modelChanged(QicsRegion)));
    connect(parent, SIGNAL(modelSizeChanged(int,int)), this, SIGNAL(modelSizeChanged(int,int)));
}

QicsViewTreeDataModel::~QicsViewTreeDataModel()
{
}

void QicsViewTreeDataModel::startSpecialInsertion()
{
    sr_count = 0;
    sr_start = -1;

    m_delayInsertSignals = true;
}

void QicsViewTreeDataModel::finalizeSpecialInsertion()
{
    m_delayInsertSignals = false;

    emit prepareForRowChanges(sr_count, sr_start);
    emit rowsInserted(sr_count, sr_start);

    for (int i = sr_start; sr_count; i++, sr_count--) {
        QicsSpecialRowData *data = specialRowData(i);
        if (data)
            data->init(i);
    }
}

void QicsViewTreeDataModel::onCellValueChanged(int row, int col)
{
    emit cellValueChanged(row, col+m_shiftColumn);
}

void QicsViewTreeDataModel::addRows(int count)
{
    setNumRows(numRows() + count);
    emit rowsAdded(count);
}

void QicsViewTreeDataModel::insertRows(int count, int index)
{
    if (index < 0 || count <= 0 ) return;

    if (!m_delayInsertSignals) emit prepareForRowChanges(count, index);

    if (index > lastRow()) index = numRows();
    setNumRows(numRows() + count);

    if (!m_delayInsertSignals) emit rowsInserted(count, index);

    if (m_specRows.size()) {
        // renumber special rows
        QMap<int, QicsSpecialRowData*>::iterator it_up = m_specRows.upperBound(index);
        if (it_up == m_specRows.end())
            return;

        QMap<int, QicsSpecialRowData*> temp;
        QMap<int, QicsSpecialRowData*>::iterator it;
        for (it = m_specRows.begin(); it != it_up; it++)
            temp[it.key()] = *it;
        for (it = it_up; it != m_specRows.end(); it++)
            temp[it.key() + count] = *it;

        m_specRows = temp;
    }

}

void QicsViewTreeDataModel::deleteRows(int count, int index)
{
    if (index < 0 || count <= 0 ) return;

    emit prepareForRowChanges(count, index);

    bool oldSignalFlag = m_emitSignals;
    // Only one signal please
    m_emitSignals = false;

    setNumRows(numRows() - count);

    // reset the emit flag
    m_emitSignals = oldSignalFlag;

    emit rowsDeleted(count, index);


    bool flag = false;

    if (m_specRows.size()) {
        // renumber special rows
        QMap<int, QicsSpecialRowData*> temp;
        QMap<int, QicsSpecialRowData*>::const_iterator it;
        for (it = m_specRows.constBegin(); it != m_specRows.constEnd(); ++it)
            if (it.key() > index) {
                temp[it.key() - count] = *it;
                flag = true;
            } else
                temp[it.key()] = *it;

        if (flag)
            m_specRows = temp;
    }

    if (m_emitSignals)
        emit modelSizeChanged(index, 0 - count, Qics::RowIndex);
}

void QicsViewTreeDataModel::insertColumns(int count, int index)
{
    if (index < 0 || count <= 0 ) return;
    if (!numColumns() && m_shiftColumn) {	// add null column explicitly
        setNumColumns(1);
        emit columnsAdded(1);
    }

    if (m_shiftColumn)
        index++;

    emit prepareForColumnChanges(count, index);
    setNumColumns(numColumns() + count);
    emit columnsInserted(count, index);
}

void QicsViewTreeDataModel::addColumns(int count)
{
    if (count <= 0 ) return;
    if (!numColumns() && m_shiftColumn) {	// add null column explicitly
        setNumColumns(1);
        emit columnsAdded(1);
    }

    setNumColumns(numColumns() + count);
    emit columnsAdded(count);
}

void QicsViewTreeDataModel::deleteColumns(int count, int index)
{
    if (index < 0 || index >= numColumns() || count <= 0) return;
    if (m_shiftColumn)
        index++;

    emit prepareForColumnChanges(count, index);
    setNumColumns(numColumns() - count);
    emit columnsDeleted(count, index);
}

const QicsDataItem* QicsViewTreeDataModel::item (int row, int col) const
{
    QicsDataModel *tdm = qobject_cast<QicsDataModel *>(parent());
    if (!tdm) return 0;

    int ri = row;
    if (m_specRows.size()) {
        if (m_specRows.contains(ri)) {
            QicsSpecialRowData *rowData = m_specRows[row];
            if (rowData)
                return rowData->item(row, col-m_shiftColumn);
            return 0;
        }

        QMap<int, QicsSpecialRowData*>::const_iterator it, it_end = m_specRows.end();
        for (it = m_specRows.begin(); it != it_end; it++)
            if (it.key() <= row)
                ri--;
            else
                break;

    }

    if (!col && m_shiftColumn)
        return 0;

    return tdm->item(ri, col-m_shiftColumn);
}

void QicsViewTreeDataModel::setItem (int row, int col, const QicsDataItem &item)
{
    QicsDataModel *tdm = qobject_cast<QicsDataModel *>(parent());
    if (!tdm) return;

    if (!col && m_shiftColumn) return;

    int ri = row;
    if (m_specRows.size()) {
        if (m_specRows.contains(ri)) {
            return;
        }

        QMap<int, QicsSpecialRowData*>::const_iterator it, it_end = m_specRows.end();
        for (it = m_specRows.begin(); it != it_end; it++)
            if (it.key() <= row)
                ri--;
            else
                break;
    }

    tdm->setItem(ri, col-m_shiftColumn, item);
}

void QicsViewTreeDataModel::addSpecialRow(int index, QicsSpecialRowData* rowData)
{
    if (index > numRows() || index < 0 || m_specRows.contains(index)) return ;
    m_specRows[index] = rowData;
    insertRows(1, index);
}

void QicsViewTreeDataModel::removeSpecialRow(int index)
{
    if (index > numRows() || index < 0 || !m_specRows.contains(index)) return;
    m_specRows.remove(index);
    deleteRows(1, index);
}

void QicsViewTreeDataModel::removeSpecialRows()
{
    int sr_size = m_specRows.size();
    if (!sr_size) return;

    int sr_index = -1, sr_count = 0;
    int key;

    QMap<int, QicsSpecialRowData*>::iterator it = m_specRows.end();
    do {
        --it;
        key = it.key();

        if (sr_index == -1) {
            sr_index = key;
            sr_count = 1;
        } else
            if (key == sr_index-1) {
                sr_count++;
                sr_index--;
            } else {
                if (m_emitSignals)
                    emit rowsDeleted(sr_count, sr_index);
                sr_index = key;
                sr_count = 1;
            }

    } while (it != m_specRows.begin());

    qDeleteAll(m_specRows);
    m_specRows.clear();

    if (sr_count > 0 && m_emitSignals)
        emit rowsDeleted(sr_count, sr_index);

    setNumRows(numRows() - sr_size);
}


