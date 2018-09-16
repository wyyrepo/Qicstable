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

#include "QicsFilter.h"

#include "QicsDimensionManager.h"
#include "QicsDataModel.h"
#include "QicsAbstractFilterDelegate.h"

///////////////////////////////////////////////////////////////////////////////
// QicsFilter
///////////////////////////////////////////////////////////////////////////////

QicsFilter::QicsFilter(QicsGridInfo *info, QObject *parent)
    : QObject(parent)
{
    m_info = info;
    m_model = info->dataModel();
}

QicsFilter::~QicsFilter()
{
    qDeleteAll(m_filters);
    m_filters.clear();
}

QicsAbstractFilterDelegate* QicsFilter::filter(int index)
{
    return m_filters.value(index);
}
///////////////////////////////////////////////////////////////////////////////
// QicsRowFilter
///////////////////////////////////////////////////////////////////////////////

QicsRowFilter::QicsRowFilter(QicsGridInfo *info, QObject *parent)
    : QicsFilter(info, parent)
{
}

void QicsRowFilter::setFilter(int index, QicsAbstractFilterDelegate *filter, bool deleteOld)
{
    if (!m_model) return;

    QicsDimensionManager *dimensionManager = m_info->dimensionManager();
    if (!dimensionManager) return;

    bool old_sig = dimensionManager->emitSignals();
    dimensionManager->setEmitSignals(false);

    if (!filter)
        removeFilter(index, deleteOld);
    else {
        QicsAbstractFilterDelegate *old_filter = m_filters.take(index);
        if (deleteOld) delete old_filter;

        const int numRows = m_model->numRows();
        QString str;

        for (int i = 0; i < numRows; ++i) {
            str = m_model->itemString(i, index);

            if (isRowMatchFilters(i)) {
                dimensionManager->showRow(i);
                m_hiddenIndexes.remove(i);
            }

            if (dimensionManager->isRowHidden(i)) continue;

            if (!filter->match(str, i, index)) {
                dimensionManager->hideRow(i);
                m_hiddenIndexes.insert(i);
            }
        }

        m_filters.insert(index, filter);

        emit filterChanged(index, true);
    }

    dimensionManager->setEmitSignals(old_sig);
}

void QicsRowFilter::removeFilter(int index, bool deleteOld)
{
    QicsDimensionManager *dimensionManager = m_info->dimensionManager();
    if (!dimensionManager) return;

    bool old_sig = dimensionManager->emitSignals();
    dimensionManager->setEmitSignals(false);

    if (m_filters.size() == 1) {
        bool blocked = blockSignals(true);
        removeAll();
        blockSignals(blocked);
    }
    else {
        QicsAbstractFilterDelegate *filter = m_filters.take(index);
        if (!filter) return;

        if (deleteOld) delete filter;

        doUpdateHiddenRows();
    }

    dimensionManager->setEmitSignals(old_sig);

    emit filterChanged(index, false);
}

void QicsRowFilter::doUpdateHiddenRows()
{
    QicsDimensionManager *dimensionManager = m_info->dimensionManager();
    if (!dimensionManager) return;

    QSet<int> tmp(m_hiddenIndexes);

    QSet<int>::const_iterator it, it_end(m_hiddenIndexes.constEnd());
    for (it = m_hiddenIndexes.constBegin(); it != it_end; ++it) {
        if (isRowMatchFilters(*it)) {
            dimensionManager->showRow(*it);
            tmp.remove(*it);
        }
    }

    m_hiddenIndexes = tmp;
}

void QicsRowFilter::removeAll()
{
    QicsDimensionManager *dimensionManager = m_info->dimensionManager();
    if (!dimensionManager) return;

    bool old_sig = dimensionManager->emitSignals();
    dimensionManager->setEmitSignals(false);

    QSetIterator<int> it(m_hiddenIndexes);
    while (it.hasNext())
        dimensionManager->showRow(it.next());

    dimensionManager->setEmitSignals(old_sig);

    qDeleteAll(m_filters);
    m_filters.clear();
    m_hiddenIndexes.clear();

    emit filterChanged(-1, false);
}

bool QicsRowFilter::isRowMatchFilters(int index)
{
    if (!m_model) return false;

    QString s;

//    QMapIterator<int, QicsAbstractFilterDelegate*> it(m_filters);
    QMapIterator<int, QPointer<QicsAbstractFilterDelegate> > it(m_filters);
    while (it.hasNext()) {
        it.next();
        s = m_model->itemString(index, it.key());
        if (!it.value()->match(s, index, it.key()))
            return false;
    }

    return true;
}

void QicsRowFilter::handleRowsAdded(int num, int start_position)
{
    QicsDimensionManager *dimensionManager = m_info->dimensionManager();
    if (!dimensionManager) return;

    // shift rows down
    QSet<int> temp;
    QSet<int>::const_iterator it, it_end(m_hiddenIndexes.end());
    for (it = m_hiddenIndexes.begin(); it != it_end; ++it)
        if (*it > start_position)
            temp.insert(*it + num);
        else
            temp.insert(*it);

    m_hiddenIndexes = temp;

    // hide row if it matches filter criteria
    for (int i = start_position; i < start_position + num; ++i) {
        if (!isRowMatchFilters(i)) {
            dimensionManager->hideRow(i);
            m_hiddenIndexes.insert(i);
        }
    }
}

void QicsRowFilter::handleRowsRemoved(int num, int start_position)
{
    // shift rows up
    QSet<int> temp;
    QSet<int>::const_iterator it, it_end(m_hiddenIndexes.end());
    for (it = m_hiddenIndexes.begin(); it != it_end; ++it)
        if (*it > start_position)
            temp.insert(*it - num);
        else
            temp.insert(*it);

    m_hiddenIndexes = temp;
}

void QicsRowFilter::handleColumnsAdded(int num, int start_position)
{
    // if columns were added - just reorder map keys up
    if (m_filters.size()) {
        // renumber special rows
//        QMap<int, QicsAbstractFilterDelegate*>::iterator it_up = m_filters.upperBound(start_position);
        QMap<int, QPointer<QicsAbstractFilterDelegate> >::iterator it_up = m_filters.upperBound(start_position);
        if (it_up == m_filters.end())
            return;

//        QMap<int, QicsAbstractFilterDelegate*> temp;
//        QMap<int, QicsAbstractFilterDelegate*>::iterator it;
        QMap<int, QPointer<QicsAbstractFilterDelegate> > temp;
        QMap<int, QPointer<QicsAbstractFilterDelegate> >::iterator it;
        for (it = m_filters.begin(); it != it_up; ++it)
            temp.insert(it.key(), *it);
//        QMap<int, QicsAbstractFilterDelegate*>::iterator it_end(m_filters.end());
        QMap<int, QPointer<QicsAbstractFilterDelegate> >::iterator it_end(m_filters.end());
        for (it = it_up; it != it_end; ++it)
            temp.insert(it.key() + num, *it);

        m_filters = temp;
    }
}

void QicsRowFilter::handleColumnsRemoved(int num, int start_position)
{
    // remove filters from removed columns
    bool recheck = false;
    for (int i = start_position; i < start_position+num; ++i) {
        if (m_filters.remove(i)) {
            recheck = true;
        }
    }

    if (recheck)
        doUpdateHiddenRows();

    // reorder map keys down
    if (m_filters.size()) {
        bool b = false;
//        QMap<int, QicsAbstractFilterDelegate*> temp;
//        QMap<int, QicsAbstractFilterDelegate*>::iterator it, it_end(m_filters.end());
        QMap<int, QPointer<QicsAbstractFilterDelegate> > temp;
        QMap<int, QPointer<QicsAbstractFilterDelegate> >::iterator it, it_end(m_filters.end());
        for (it = m_filters.begin(); it != it_end; ++it)
            if (it.key() > start_position) {
                temp.insert(it.key() - num, *it);
                b = true;
            } else
                temp.insert(it.key(), *it);

        if (b) m_filters = temp;
    }
}


