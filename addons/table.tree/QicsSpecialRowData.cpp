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

#include <QicsSpecialRowData.h>

#include <QicsTreeTable.h>


QicsSpecialRowData::QicsSpecialRowData(QicsTreeTable *table, QObject *parent)
    : QObject(parent)
{
    m_items.resize(table->columnCount());
    m_visible = true;
    m_table = table;
}

QicsSpecialRowData::~QicsSpecialRowData()
{
}

void QicsSpecialRowData::init(int rowIndex)
{
    if (m_table)
        init(m_table->rowRef(rowIndex));
}

QicsRow* QicsSpecialRowData::rowPointer()
{
    return m_table ? const_cast<QicsRow*>(m_table->specialRow(this)) : 0;
}

void QicsSpecialRowData::dispose(QicsRow &)
{
    qDeleteAll(m_items);
    m_items.clear();
}

const QicsDataItem* QicsSpecialRowData::item(int ,int col)
{
    return (col < 0 || col >= m_items.size()) ? 0 : m_items.at(col);
}

void QicsSpecialRowData::setItem(int ,int col, const QicsDataItem &item)
{
    if (col < 0) return;
    if (col >= m_items.size())
        m_items.resize(col+1);
    m_items[col] = item.clone();
}


