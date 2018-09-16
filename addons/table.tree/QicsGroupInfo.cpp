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

#include <QicsGroupInfo.h>

#include <QicsTreeTable.h>
#include <QicsGroupCellDisplay.h>
#include <QicsGridInfo.h>
#include <QicsSpecialRowData.h>
#include <QicsExpandableStaticRowData.h>
#include <QicsSummarizer.h>


QicsGroupInfo::QicsGroupInfo(QicsTreeTable *table, QicsGroupInfo *parent)
    : m_table(table), m_parent(parent)
{
}

QicsGroupInfo::~QicsGroupInfo()
{
    clear();
}

void QicsGroupInfo::clear()
{
    qDeleteAll(m_groups);

    m_groups.clear();
    m_rows.clear();

    m_parent = 0;
    m_lev = 0;
    m_col = -1;
}

bool QicsGroupInfo::isLast() const
{
    return m_lev == m_table->groupCount()-1;
}

QicsGroupInfo* QicsGroupInfo::addGroup()
{
    QicsGroupInfo *gi = new QicsGroupInfo(m_table, this);
    m_groups.append(gi);
    return gi;
}

void QicsGroupInfo::addRow(int index)
{
    m_rows.append(index);
}

int QicsGroupInfo::firstDataRowVisualIndex() const
{
    return headerVisualIndex()+1;
}

int QicsGroupInfo::lastDataRowVisualIndex() const
{
    if (m_table->summaryPolicy() == QicsTreeTable::SummaryDisabled)
        return m_table->gridInfo().visualRowIndex(m_rows.last());

    return summaryVisualIndex()-1;
}

int QicsGroupInfo::headerVisualIndex() const
{
    return m_table->gridInfo().visualRowIndex(m_hmi);
}

int QicsGroupInfo::summaryVisualIndex() const
{
    return m_table->gridInfo().visualRowIndex(m_smi);
}

QicsExpandableStaticRowData* QicsGroupInfo::headerRowData() const
{
    return (QicsExpandableStaticRowData*) m_table->specialRowData(m_hmi);
}

QicsSpecialRowData* QicsGroupInfo::summaryRowData() const
{
    return m_table->specialRowData(m_smi);
}

QicsRow& QicsGroupInfo::headerRef()
{
    return m_table->rowRef(headerVisualIndex());
}

QicsRow& QicsGroupInfo::headerRowRef()
{
    return m_table->rowHeaderRef().rowRef(headerVisualIndex());
}

QicsCell& QicsGroupInfo::headerCellRef(int index)
{
    return m_table->cellRef(headerVisualIndex(), index);
}

QicsRow& QicsGroupInfo::summaryRowRef()
{
    return m_table->rowHeaderRef().rowRef(summaryVisualIndex());
}

QicsRow& QicsGroupInfo::summaryRef()
{
    return m_table->rowRef(summaryVisualIndex());
}

QicsCell& QicsGroupInfo::summaryCellRef(int index)
{
    return m_table->cellRef(summaryVisualIndex(), index);
}

void QicsGroupInfo::showSummary(bool b)
{
    summaryRowData()->setVisible(b);
}

QicsCell& QicsGroupInfo::cellRef(int row, int col)
{
    if (row < 0 || col < 0 || row >= m_rows.count())
        return m_table->cellRef(-1, -1);

    int vi = m_table->gridInfo().visualRowIndex(m_rows.at(row));
    return m_table->cellRef(vi, col);
}

void QicsGroupInfo::setSummaryData(int col, const QicsDataItem &item)
{
    summaryRowData()->setItem(0, col, item);
}

void QicsGroupInfo::setHeaderData(int col, const QicsDataItem &item)
{
    headerRowData()->setItem(0, col, item);
}

void QicsGroupInfo::spanSummary(bool span)
{
    // span if needed
    m_table->summaryDisplayer()->setSpanned(span);
}

void QicsGroupInfo::spanHeader(bool span)
{
    // span if needed
    m_table->groupDisplayer()->setSpanned(span);
}

void QicsGroupInfo::recalculate()
{
    if (m_table->summarizer())
        m_table->summarizer()->onGrouping(this);
}


