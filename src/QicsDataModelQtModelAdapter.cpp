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

#include "QicsDataModelQtModelAdapter.h"


QicsDataDate QicsDataModelQtModelAdapter::dateItem = QicsDataDate();
QicsDataDateTime QicsDataModelQtModelAdapter::dateTimeItem = QicsDataDateTime();
QicsDataDouble QicsDataModelQtModelAdapter::doubleItem = QicsDataDouble();
QicsDataFloat QicsDataModelQtModelAdapter::floatItem = QicsDataFloat();
QicsDataInt QicsDataModelQtModelAdapter::intItem = QicsDataInt();
QicsDataLongLong QicsDataModelQtModelAdapter::longLongItem = QicsDataLongLong();
QicsDataString QicsDataModelQtModelAdapter::stringItem = QicsDataString();
QicsDataTime QicsDataModelQtModelAdapter::timeItem = QicsDataTime();
QicsDataBool QicsDataModelQtModelAdapter::boolItem = QicsDataBool();

QicsDataModelQtModelAdapter::QicsDataModelQtModelAdapter(QObject *parent, QAbstractItemModel *qt4Model)
    : QicsDataModel(0, 0, parent),
    m_qt4Model(0)
{
    setModel(qt4Model);
}

void QicsDataModelQtModelAdapter::setModel(QAbstractItemModel *qt4Model)
{
    // If there is already a Qt4 model set disconnect it
    if (m_qt4Model)
        disconnectModelSignals();

    m_qt4Model = qt4Model;
    // If we have a vaild model being set
    // Connect it up
    if (m_qt4Model) {
        connectModelSignals();
        setNumRows(m_qt4Model->rowCount());
        setNumColumns(m_qt4Model->columnCount());
        recalcModelSize();
        handleModelReset();
    }
}

QModelIndex QicsDataModelQtModelAdapter::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_qt4Model)
        return QModelIndex();

    return m_qt4Model->index(row, column, parent);
}

void QicsDataModelQtModelAdapter::recalcModelSize()
{
    // Test for existance of Qt4 model
    if (!m_qt4Model)
        return;

    setNumRows(m_qt4Model->rowCount());
    setNumColumns(m_qt4Model->columnCount());
    // Is this emit necessary?
    emit modelSizeChanged(numRows(), numColumns());
}

void QicsDataModelQtModelAdapter::connectModelSignals()
{
    connect(m_qt4Model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
        this, SLOT(handleDataChanged(const QModelIndex &, const QModelIndex &)));

    connect(m_qt4Model, SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
        this, SLOT(handleHeaderDataChanged(Qt::Orientation, int, int)));

    connect(m_qt4Model, SIGNAL(layoutChanged()), this, SLOT(handleLayoutChanged()));

    connect(m_qt4Model, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)),
        this, SLOT(handleRowsAboutToBeInserted(const QModelIndex &, int, int)));

    connect(m_qt4Model, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
        this, SLOT(handleRowsInserted(const QModelIndex &, int, int)));

    connect(m_qt4Model, SIGNAL(rowsAboutToBeRemoved(const QModelIndex &, int, int)),
        this, SLOT(handleRowsAboutToBeRemoved(const QModelIndex &, int, int)));

    connect(m_qt4Model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
        this, SLOT(handleRowsRemoved(const QModelIndex &, int, int)));

    connect(m_qt4Model, SIGNAL(columnsAboutToBeInserted(const QModelIndex &, int, int)),
        this, SLOT(handleColumnsAboutToBeInserted(const QModelIndex &, int, int)));

    connect(m_qt4Model, SIGNAL(columnsInserted(const QModelIndex &, int, int)),
        this, SLOT(handleColumnsInserted(const QModelIndex &, int, int)));

    connect(m_qt4Model, SIGNAL(columnsAboutToBeRemoved(const QModelIndex &, int, int)),
        this, SLOT(handleColumnsAboutToBeRemoved(const QModelIndex &, int, int)));

    connect(m_qt4Model, SIGNAL(columnsRemoved(const QModelIndex &, int, int)),
        this, SLOT(handleColumnsRemoved(const QModelIndex &, int, int)));

    connect(m_qt4Model, SIGNAL(modelReset()), this, SLOT(handleModelReset()));
}

void QicsDataModelQtModelAdapter::disconnectModelSignals()
{
    disconnect(m_qt4Model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
        this, SLOT(handleDataChanged(const QModelIndex &, const QModelIndex &)));

    disconnect(m_qt4Model, SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
        this, SLOT(handleHeaderDataChanged(Qt::Orientation, int, int)));

    disconnect(m_qt4Model, SIGNAL(layoutChanged()), this, SLOT(handleLayoutChanged()));

    disconnect(m_qt4Model, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)),
        this, SLOT(handleRowsAboutToBeInserted(const QModelIndex &, int, int)));

    disconnect(m_qt4Model, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
        this, SLOT(handleRowsInserted(const QModelIndex &, int, int)));

    disconnect(m_qt4Model, SIGNAL(rowsAboutToBeRemoved(const QModelIndex &, int, int)),
        this, SLOT(handleRowsAboutToBeRemoved(const QModelIndex &, int, int)));

    disconnect(m_qt4Model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
        this, SLOT(handleRowsRemoved(const QModelIndex &, int, int)));

    disconnect(m_qt4Model, SIGNAL(columnsAboutToBeInserted(const QModelIndex &, int, int)),
        this, SLOT(handleColumnsAboutToBeInserted(const QModelIndex &, int, int)));

    disconnect(m_qt4Model, SIGNAL(columnsInserted(const QModelIndex &, int, int)),
        this, SLOT(handleColumnsInserted(const QModelIndex &, int, int)));

    disconnect(m_qt4Model, SIGNAL(columnsAboutToBeRemoved(const QModelIndex &, int, int)),
        this, SLOT(handleColumnsAboutToBeRemoved(const QModelIndex &, int, int)));

    disconnect(m_qt4Model, SIGNAL(columnsRemoved(const QModelIndex &, int, int)),
        this, SLOT(handleColumnsRemoved(const QModelIndex &, int, int)));

    disconnect(m_qt4Model, SIGNAL(modelReset()), this, SLOT(handleModelReset()));
}

QicsDataModelQtModelAdapter::~QicsDataModelQtModelAdapter ()
{
}

const QicsDataItem *QicsDataModelQtModelAdapter::item(int row, int col, Qt::ItemDataRole role) const
{
    if (!m_qt4Model)
        return 0;

    QicsDataItem *item = 0; // default return

    QModelIndex index = m_qt4Model->index(row, col, QModelIndex());
    QVariant var = m_qt4Model->data(index, role);
    QVariant::Type type = var.type();

    switch (type)
    {
    case QVariant::Int:
    case QVariant::UInt:
        item = &intItem;
        intItem.setData(qvariant_cast<int>(var));
        break;
    case QVariant::LongLong:
    case QVariant::ULongLong:
        item = &longLongItem;
        longLongItem.setData(qvariant_cast<int>(var));
        break;
    case QVariant::Double:
        item = &doubleItem;
        doubleItem.setData(qvariant_cast<double>(var));
        break;
    case QVariant::Date:
        item = &dateItem;
        dateItem.setData(qvariant_cast<QDate>(var));
        break;
    case QVariant::DateTime:
        item = &dateTimeItem;
        dateTimeItem.setData(qvariant_cast<QDateTime>(var));
        break;
    case QVariant::Time:
        item = &timeItem;
        timeItem.setData(qvariant_cast<QTime>(var));
        break;
    case QVariant::String:
        item = &stringItem;
        stringItem.setData(qvariant_cast<QString>(var));
        break;
    case QVariant::Bool:
        item = &boolItem;
        boolItem.setData(qvariant_cast<bool>(var));
        break;
    default:
        // User Defined Type.  No luck here.
        break;
    }

    // Return item pointer or NULL
    return item;
}

const QicsDataItem *QicsDataModelQtModelAdapter::item(int row, int col) const
{
    return item(row, col, Qt::DisplayRole);
}

QicsDataModelRow QicsDataModelQtModelAdapter::rowItems(int row, int first_col, int last_col) const
{
    QicsDataModelRow rowV;
    // Collect row items and return vector
    for (int i = first_col; i < last_col + 1; ++i)
        rowV << item(row, i);

    return rowV;
}

QicsDataModelColumn QicsDataModelQtModelAdapter::columnItems(int col, int first_row, int last_row) const
{
    QicsDataModelRow colV;
    // Collect Column Items and return vector
    for (int i = first_row; i < last_row + 1; ++i)
        colV << item(i, col);

    return colV;
}

void QicsDataModelQtModelAdapter::setRowItems(int row, const QicsDataModelRow &v)
{
    for (int i = 0; i < v.size(); ++i)
        setItem(row, i, *v.at(i));
}

void QicsDataModelQtModelAdapter::setColumnItems(int col, const QicsDataModelColumn &v)
{
    for (int i = 0; i < v.size(); ++i)
        setItem(i, col, *v.at(i));
}

void QicsDataModelQtModelAdapter::setItem (int row, int col, const QicsDataItem &item, Qt::ItemDataRole role)
{
    if (!m_qt4Model)
        return;

    QVariant var;
    const QicsDataBool *tmpBool;
    const QicsDataDate *tmpDate;
    const QicsDataDateTime *tmpDateTime;
    const QicsDataDouble *tmpDouble;
    const QicsDataFloat *tmpFloat;
    const QicsDataInt *tmpInt;
    const QicsDataLongLong *tmpLongLong;
    const QicsDataString *tmpString;
    const QicsDataTime *tmpTime;

    switch ( item.type() )
    {
    case QicsDataItem_Bool:
        tmpBool = static_cast<const QicsDataBool *>(&item);
        var = QVariant(tmpBool->data());
        break;
    case QicsDataItem_Date:
        tmpDate = static_cast<const QicsDataDate *>(&item);
        var = QVariant(tmpDate->data());
        break;
    case QicsDataItem_DateTime:
        tmpDateTime = static_cast<const QicsDataDateTime *>(&item);
        var = QVariant(tmpDateTime->data());
        break;
    case QicsDataItem_Double:
        tmpDouble = static_cast<const QicsDataDouble *>(&item);
        var = QVariant(tmpDouble->data());
        break;
    case QicsDataItem_Float:
        tmpFloat = static_cast<const QicsDataFloat *>(&item);
        var = QVariant(tmpFloat->data());
        break;
    case QicsDataItem_Int:
        tmpInt = static_cast<const QicsDataInt *>(&item);
        var = QVariant(tmpInt->data());
        break;
    case QicsDataItem_LongLong:
        tmpLongLong = static_cast<const QicsDataLongLong *>(&item);
        var = QVariant(tmpLongLong->data());
        break;
    case QicsDataItem_String:
        tmpString = static_cast<const QicsDataString *>(&item);
        var = QVariant(tmpString->data());
        break;
    case QicsDataItem_Time:
        tmpTime = static_cast<const QicsDataTime *>(&item);
        var = QVariant(tmpTime->data());
        break;
    default:
        // User Defined Type.  No luck here.
        break;
    }

    QModelIndex index = m_qt4Model->index(row, col, QModelIndex());
    m_qt4Model->setData(index, var, role);
}

void QicsDataModelQtModelAdapter::setItem(int row, int col, const QicsDataItem &item)
{
    setItem(row, col, item, Qt::DisplayRole);
}

void QicsDataModelQtModelAdapter::insertRows(int num_rows, int start_row)
{
    if (start_row < 0 || num_rows <= 0)
        return;

    emit prepareForRowChanges(num_rows, start_row);

    // Sanity Check
    if (m_qt4Model)
        m_qt4Model->insertRows(start_row, num_rows, QModelIndex());

    emit modelSizeChanged(start_row, num_rows, Qics::RowIndex);
}

void QicsDataModelQtModelAdapter::insertColumns(int num_cols, int start_col)
{
    if ((num_cols <= 0) || (start_col < 0))
        return;

    emit prepareForColumnChanges(num_cols, start_col);

    // Sanity Check
    if (m_qt4Model)
        m_qt4Model->insertColumns(start_col, num_cols, QModelIndex());

    emit modelSizeChanged(start_col, num_cols, Qics::ColumnIndex);
}

void QicsDataModelQtModelAdapter::addRows(int rows)
{
    insertRows(rows, numRows());
}

void QicsDataModelQtModelAdapter::addColumns(int num)
{
    insertRows(num, numColumns());
}

void QicsDataModelQtModelAdapter::deleteRow(int row)
{
    deleteRows(1, row);
}

void QicsDataModelQtModelAdapter::deleteRows(int num_rows, int start_row)
{
    if ((start_row < 0) || (num_rows <= 0))
        return;

    emit prepareForRowChanges(0 - num_rows, start_row);

    // Sanity Check
    if (m_qt4Model)
        m_qt4Model->removeRows(start_row, num_rows, QModelIndex());

    emit modelSizeChanged(start_row, 0 - num_rows, Qics::RowIndex);
}

void QicsDataModelQtModelAdapter::deleteColumn(int col)
{
    deleteColumns(1, col);
}

void QicsDataModelQtModelAdapter::deleteColumns(int num_cols, int start_col)
{
    if ((start_col < 0) || (num_cols <= 0))
        return;

    emit prepareForColumnChanges(0 - num_cols, start_col);

    // Sanity Check
    if (m_qt4Model)
        m_qt4Model->removeColumns(start_col, num_cols, QModelIndex());

    emit modelSizeChanged(start_col, 0 - num_cols, Qics::ColumnIndex);
}

void QicsDataModelQtModelAdapter::clearItem(int row, int col)
{
    // Sanity Check
    if (!m_qt4Model)
        return;

    QModelIndex index;
    index = m_qt4Model->index(row, col, QModelIndex());
    m_qt4Model->setData(index, QVariant(), Qt::DisplayRole);
    m_qt4Model->setData(index, QVariant(), Qt::EditRole);
    m_qt4Model->setData(index, QVariant(), Qt::CheckStateRole);
}

void QicsDataModelQtModelAdapter::clearModel()
{
    // Sanity Check
    if (!m_qt4Model)
        return;

    // Clear Model
    for (int i = 0; i < m_qt4Model->rowCount(); ++i)
        for (int j = 0; j < m_qt4Model->columnCount(); ++j)
            clearItem(i, j);
}

void QicsDataModelQtModelAdapter::handleDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (!m_emitSignals)
        return;

    int tlr = topLeft.row();
    int tlc = topLeft.column();
    int brr = bottomRight.row();
    int brc = bottomRight.column();
    QicsRegion region(tlr, tlc, brr, brc);

    emit modelChanged (region);

    // Walk window for data and set it in the model
    for (int i = tlr; i <= brr; ++i)
        for (int j = tlc; j <= brc; ++j)
            emit cellValueChanged(i, j);
}

void QicsDataModelQtModelAdapter::handleHeaderDataChanged(Qt::Orientation orientation, int first, int last)
{
    Q_UNUSED(orientation);
    Q_UNUSED(first);
    Q_UNUSED(last);
}

void QicsDataModelQtModelAdapter::handleLayoutChanged()
{
}

void QicsDataModelQtModelAdapter::handleRowsAboutToBeInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    int numRows = last - first + 1;
    emit prepareForRowChanges(numRows, first);
}

void QicsDataModelQtModelAdapter::handleRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    int numRows = last - first + 1;
    recalcModelSize();
    emit rowsInserted(numRows, first);
}

void QicsDataModelQtModelAdapter::handleRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    int numRows = last - first + 1;
    emit prepareForRowChanges(-numRows, first);
}

void QicsDataModelQtModelAdapter::handleRowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    recalcModelSize();
    int numRows = last - first + 1;
    emit rowsDeleted(numRows, first);
}

void QicsDataModelQtModelAdapter::handleColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    int numCols = last - first + 1;
    emit prepareForColumnChanges(numCols, first);
}

void QicsDataModelQtModelAdapter::handleColumnsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    recalcModelSize();
    int numCols = last - first + 1;
    emit columnsInserted(numCols, first);
}

void QicsDataModelQtModelAdapter::handleColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    int numCols = last - first + 1;
    emit prepareForColumnChanges(-numCols, first);
}

void QicsDataModelQtModelAdapter::handleColumnsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    recalcModelSize();
    int numCols = last - first + 1;
    emit columnsDeleted(numCols, first);
}

void QicsDataModelQtModelAdapter::handleModelReset()
{
    recalcModelSize();
    QicsRegion region(0, 0, lastRow(), lastColumn());
    emit modelChanged(region);
}


