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

#include "QicsSorter.h"

#include <QStringList>
#include "QicsDataModel.h"
#include "QicsAbstractSorterDelegate.h"

// Uncoment this line if you want provide integrity check
//#define INTEGRITY_CHECK


QicsSorter	*g_self;
QVector<int> g_thingsToSort;
QVector<int>::iterator  g_thingsToSortIter;
// ### For 2.4.1 compatibility
DataItemComparator g_compareFunc;
int g_ordFlip;


int delegateCompare(const int &_a, const int &_b)
{
    const int thingToSort = (*g_thingsToSortIter);

    ++g_thingsToSortIter;

    QicsAbstractSorterDelegate *sorterDelegate = g_self->sorterDelegate(thingToSort);
    QicsAbstractSorterDelegate *sorterDelegateDefault = g_self->defaultSorterDelegate();

    if (!sorterDelegate)
        sorterDelegate = sorterDelegateDefault;
    else
        sorterDelegate->setSortOrder(sorterDelegateDefault->sortOrder());

    int retval = sorterDelegate->compare(_a, _b, thingToSort);

    if (retval == 0 && (g_thingsToSortIter < g_thingsToSort.end()))
        return delegateCompare(_a, _b);

    g_thingsToSortIter = g_thingsToSort.begin();

    return retval;
}

int functionCompare(const int &_a, const int &_b, const QicsDataModel *dm)
{
    const QicsDataItem *a = 0;
    const QicsDataItem *b = 0;

    int retval = 0;
    int thingToSort = (*g_thingsToSortIter);

    ++g_thingsToSortIter;

    if (g_self->type() == Qics::RowIndex) {
        a = dm->item(_a, thingToSort);
        b = dm->item(_b, thingToSort);
    }
    else {
        a = dm->item(thingToSort, _a);
        b = dm->item(thingToSort, _b);
    }
    if (a) a = a->clone();
    if (b) b = b->clone();

    // sometimes the cells are null
    if (a == 0) {
        if (b == 0)
            retval = 0;
        else
            retval = g_ordFlip;
    }
    else if (b == 0)
        retval = g_ordFlip * -1;
    else
        retval = g_ordFlip * g_compareFunc(a, b);

    delete a;
    delete b;

    if (retval == 0 && (g_thingsToSortIter < g_thingsToSort.end()))
        return functionCompare(_a, _b, dm);

    g_thingsToSortIter = g_thingsToSort.begin();
    return retval;
}

bool delegateLessThan(const int &_a, const int &_b)
{
    return (delegateCompare(_a, _b) < 0);
}

bool functionLessThan(const int &_a, const int &_b)
{
    const QicsDataModel *dm = g_self->dataModel();
    if (!dm) return false;

    return (functionCompare(_a, _b, dm) < 0);
}


QicsSorter::QicsSorter(Qics::QicsIndexType _type, QicsDataModel *model, QObject *parent)
    : QObject(parent),
        myType(_type),
        myDataModel(model),
        m_defaultSorterDelegate(0)
{
    setSortMode(Qics::QicsQuickSort);
    m_sorterDelegates.clear();

    if (myDataModel) {
        if(myType == RowIndex) {
            connect(myDataModel, SIGNAL(rowsInserted(int, int)),
                this, SLOT(insertElements(int, int)));
            connect(myDataModel, SIGNAL(rowsAdded(int)),
                this, SLOT(appendElements(int)));
            connect(myDataModel, SIGNAL(rowsDeleted(int, int)),
                this, SLOT(deleteElements(int, int)));
        }
        else {
            connect(myDataModel, SIGNAL(columnsInserted(int, int)),
                this, SLOT(insertElements(int, int)));
            connect(myDataModel, SIGNAL(columnsAdded(int)),
                this, SLOT(appendElements(int)));
            connect(myDataModel, SIGNAL(columnsDeleted(int, int)),
                this, SLOT(deleteElements(int, int)));
        }
    }
    fillVisualToModelMap();
}

QicsSorter::~QicsSorter()
{
    qDeleteAll(m_sorterDelegates);
    m_sorterDelegates.clear();
}

void QicsSorter::removeSorterDelegate(int index)
{
    delete m_sorterDelegates.take(index);
}

void QicsSorter::setDefaultSorterDelegate(QicsAbstractSorterDelegate *sorter)
{//#90075
    if (!sorter)
        return;

    m_defaultSorterDelegate = sorter;
    m_defaultSorterDelegate->setModel(dataModel());
}

void QicsSorter::setSorterDelegate(int index, QicsAbstractSorterDelegate *sorter)
{
    removeSorterDelegate(index);
    if (!sorter) return;

    sorter->setModel(dataModel());
    sorter->setType(type());
    m_sorterDelegates.insert(index, sorter);
}

void QicsSorter::setSortingSensitivity(Qt::CaseSensitivity cs)
{
    m_defaultSorterDelegate->setSortingSensitivity(cs);
}

Qt::CaseSensitivity QicsSorter::sortingSensitivity() const
{
    return m_defaultSorterDelegate->sortingSensitivity();
}

void QicsSorter::expandTo(int newsize)
{
    m_order.resize(newsize);
}

void QicsSorter::integrityCheck()
{
    const int order_size = m_order.size();
    QVector<int> used;
    used.fill(0, order_size);
    QVector<int>::const_iterator iter, iter_end(m_order.constEnd());

    for(iter = m_order.constBegin(); iter <  iter_end; ++iter) {
        int j = (*iter);
        Q_ASSERT(j >= 0);
        Q_ASSERT(j < order_size);
        Q_ASSERT(used[j] == 0);
        used[j] = 1;
    }
}

/*! internal slot
*/
void QicsSorter::appendElements(int how_many)
{
    // If we haven't yet reordered anything, we don't need to do anything
    // here.
    if (m_order.isEmpty()) {
        fillVisualToModelMap();
        return;
    }

    /* The logic of appending a row or column is pretty
    * clear.   We added something to the data model,
    * and we should open up space at the end of the view.
    * Towards that end, we simply alloc more space int
    * the order vector and stuff in new indices.  This
    * will put the new rows at the end of the view.
    */

    int cur_size = m_order.size();

    expandTo(cur_size + how_many);

    for(int i = 0; i < how_many; ++i) {
        m_order[cur_size] = cur_size;
        ++cur_size;
    }

    flushModelToVisualMap();
}

void QicsSorter::elementVisibilityChanged(int index, bool visible)
{
    if (m_order.isEmpty()) {
        fillVisualToModelMap();
        return;
    }

    const int cur_size = m_order.size();
    Q_ASSERT(index >= 0 && index < cur_size);
    if ((index < 0) || (index >= cur_size))
        return;

    const int element = m_order.at(index);

    if (visible) {
        m_hidden.remove(element);
    }
    else {
        m_hidden.insert(element);
    }

    flushModelToVisualMap();
    fillModelToVisualMap();
}

/*! internal slot
*/
void QicsSorter::insertElements(int how_many, int start)
{
    /* guard ourselves */
    Q_ASSERT(how_many > 0);

    // If we haven't yet reordered anything, we don't need to do anything
    // here.

    if (m_order.isEmpty()) {
        fillVisualToModelMap();
        return;
    }

    const int cur_size = m_order.size();

    Q_ASSERT(start >= 0 && start < cur_size);
    if ((start < 0) || (start >= cur_size))
        return;

    int *visChange = new int[cur_size];

    /* what should happen when you insert things in the middle
    * of a reorders set is not obvious.   We start with some points
    *
    * - We will expand norder by how_many.
    * - Viewers will probably expect the new rows to appear next
    *   to each other
    */

    expandTo(cur_size + how_many);

    /* incoming data is in model units.
    * - all indices >= start must be increased by how_many
    * - there should be how_many slots opened up at start,
    * - these will contain the indices start up to start+how_many
    * Doing it in three passes makes it pretty obvious.
    */

    // fix all the existing indices
    int i;
    int	insertPoint = cur_size;
    for(i = 0; i < cur_size; ++i) {
        int x = m_order.at(i);
        if(x == start) insertPoint = i;
        if(x >= start) m_order[i] += how_many;
        visChange[i] = i;
    }

    //    assert(insertPoint >= 0);
    // open up the space
    for(i = cur_size-1; i >= insertPoint; --i) {
        m_order[i + how_many] = m_order.at(i);
        visChange[i] += how_many;
    }

    // create the new ones
    for(i = 0; i < how_many; ++i)
        m_order[insertPoint+i] = start+i;

#ifdef INTEGRITY_CHECK
    integrityCheck();
#endif
    flushModelToVisualMap();

    emit orderChanged(myType, visChange, cur_size);

    delete[] visChange;
}

/*! internal slot
*/
void QicsSorter::deleteElements(int how_many, int start)
{
    // If we haven't yet reordered anything, we don't need to do anything
    // here.

    if (m_order.isEmpty())
        return;

    /* incoming data is in model units.   We have to
    * delete from the ordering vector all positions x, where
    * start <= x <start+how_many.  Now, we can't have gaps
    * in the indices, so any positions y
    * (start+how_many < y) must be reduced by how_many,
    *
    * At the same time we build the visual reorder vector.
    * which maps old visual locations to new ones.  If
    * we did not delete anything then visChange[i] == i,
    * for all i.  If delete the fifth slot, then v0 = 0 ...
    * v3==3, v4=-1, v5=4.
    *
    */

    const int size = m_order.size();
    int *visChange = new int[size];

    int ndone = 0;
    for(int i = 0; i < size; ++i) {
        int x = m_order.at(i);
        if(start <= x && x < start+how_many) {
            // This slot is deleted
            visChange[i] = -1;
            continue;
        }

        if(x >= start+how_many)
            x-=how_many;
        visChange[i] = ndone;
        m_order[ndone++] = x;
    }

    Q_ASSERT(ndone == size - how_many);

    m_order.resize(ndone);

#ifdef INTEGRITY_CHECK
    integrityCheck();
#endif
    flushModelToVisualMap();

    emit orderChanged(myType, visChange, size);

    delete[] visChange;
}

void QicsSorter::deleteVisualElements(int how_many, int start)
{
    if (m_order.isEmpty())
        return;

    const int size = m_order.size();
    int *visChange = new int[size];

    for(int i = 0; i < size; ++i) {
        if(start <= i && i < start+how_many) {
            // This slot is deleted
            visChange[i] = -1;
            continue;
        }
        visChange[i] = i;
    }

    QVector<int>::iterator it = m_order.begin() + start;
    QVector<int>::iterator it_end = it+how_many;
    m_order.erase(it, it_end);

    flushModelToVisualMap();

    emit orderChanged(myType, visChange, size);

    delete[] visChange;
}

void QicsSorter::fillVisualToModelMap()
{
    int orderAllocated = 0;

    if (myDataModel) {
        orderAllocated = ((myType == RowIndex) ? myDataModel->numRows() :
            myDataModel->numColumns());
    }

    m_order.resize(orderAllocated);

    for(int i = 0; i < orderAllocated; ++i)
        m_order[i] = i;
}

/* We lazy evaluate this map because we don't need it all the time.
*/
void QicsSorter::fillModelToVisualMap()
{
    const int size = m_order.size();
    m_modelToVisualMap.clear();
    int index = 0;
    for(int i = 0; i < size; ++i) {
        if (!m_hidden.contains(m_order.at(i))) {
            m_modelToVisualMap.insert(m_order.at(i), index);
            ++index;
        }
    }
}

void QicsSorter::flushModelToVisualMap()
{
    m_modelToVisualMap.clear();
}

int QicsSorter::modelToVisual(int x)
{
    if(m_modelToVisualMap.isEmpty())
        fillModelToVisualMap();

    return m_modelToVisualMap.value(x, -1);
}

int QicsSorter::visualToModel(int x)
{
    if(m_modelToVisualMap.isEmpty())
        fillModelToVisualMap();

    const int model = m_modelToVisualMap.key(x, -1);
    return model;
}

void QicsSorter::sort(const QVector<int> &rows_or_columns, QicsSortOrder sort_order,
                      int from, int to, DataItemComparator func)
{
    // If we haven't moved or sorted yet, we need to fill this map.
    if (m_order.isEmpty())
        fillVisualToModelMap();

    const int oldOrderSize = m_order.size();

    if (rows_or_columns.isEmpty())
        return;

    m_defaultSorterDelegate->setSortOrder(sort_order);
    m_defaultSorterDelegate->setType(type());

    g_self = this;
    g_thingsToSort = rows_or_columns;
    g_thingsToSortIter = g_thingsToSort.begin();
    g_compareFunc = func;
    g_ordFlip = (sort_order == Qics::Ascending ) ? 1 : -1;

    if(from < 0)
        from = 0;
    if(to < 0 || to >= oldOrderSize)
        to = oldOrderSize - 1;

    // save off a copy of the order so we can build the change map
    int *visChange = new int[oldOrderSize];
    memmove(visChange, m_order.constData(), oldOrderSize*sizeof(int));

    QVector<int>::iterator _from = m_order.begin()+from;
    QVector<int>::iterator _to = m_order.begin()+to+1;

    bool sorted = false;
    int vm_i = visualToModel(from);
    int vm_i1 = -1;
    const QicsDataModel *dm = dataModel();

    for (int i=from;i<to;++i) {
        vm_i1 = visualToModel(i+1);

        int result = 0;
        if (func)
            result = functionCompare(vm_i, vm_i1, dm);
        else
            result = delegateCompare(vm_i, vm_i1);

        if(result>0) {
            sorted=false;
            break;
        }
        else {
            sorted=true;
            vm_i = vm_i1;
        }
    }

    if (!sorted) {
        if (sortMode() == Qics::QicsQuickSort)
            qSort(_from, _to, func ? functionLessThan : delegateLessThan);
        else
            qStableSort(_from, _to, func ? functionLessThan : delegateLessThan);
    }

    flushModelToVisualMap();

    /* Now we compute the mapping of old visual to new visual.
    * For old visual i, visChange[i] is the model index,
    * modelToVisualMap of that gives the current visual.
    */
    fillModelToVisualMap();

    for(int i = 0; i < oldOrderSize; ++i)
        visChange[i] = m_modelToVisualMap.value(visChange[i]);

    emit orderChanged(myType, visChange, oldOrderSize);

    delete[] visChange;
}

/*
* moveItems could seem tricky, but the algorithm is
* mostly straightforward
* - build a new empty ordering vector
*   initialize the slots to something "unused" (-1)
* - compute a new target by subtracting the number of items
*   from the target
* - grab the data from the slots indicated move list and
*   copy it into the new vector at the desired target,
*   set the original slots to "unused"
* - copy the remaining slots into the new vector,
*   carefully pouring around the "used" slots
*/

void QicsSorter::moveItems(int target, const QVector<int> &itms)
{
#if notdef
    qDebug("We are moving to %d:", target);
    for(int i = 0; i < itms.size(); ++i)
        qDebug(" %d", itms[i]);
    qDebug("\n");
#endif

    int i;

    // If we haven't moved or sorted yet, we need to fill this map.
    if (m_order.isEmpty())
        fillVisualToModelMap();
    const int oldOrderSize = m_order.size();

    target = qBound(0, target, oldOrderSize);

    int *visChange = new int[oldOrderSize];
    int* data = m_order.data();
    const int* items = itms.constData();

    // - build a new empty ordering vector
    // - initialize the slots to something "unused" (-1)
    int *neworder = new int[oldOrderSize];
    memset(neworder, -1, oldOrderSize*sizeof(int));

    // compute new target
    int newtarget = target;
    int itemsSize = itms.size();
    for(i = 0; i < itemsSize; ++i) {
        if(items[i] < target) --newtarget;
        else
            break;
    }

    // - grab the data from the slots indicated move list and
    // - copy it into the new vector at the desired target,
    // - set the original slots to "unused"
    int itms_at;
    int nextNewTarget;

    for(i = 0; i < itemsSize; ++i) {
        itms_at = items[i];
        nextNewTarget = newtarget+i;
        neworder[nextNewTarget] = data[itms_at];
        visChange[itms_at] = nextNewTarget;
        data[itms_at] = -1;
    }

    // - copy the remaining slots into the new vector,
    //   carefully pouring around the "used" slots
    int newi = 0;
    int value;
    int *it_vc = visChange;

    for(i = 0; i < oldOrderSize; ++i, ++it_vc) {
        value = data[i];
        if(value >= 0) {
            while(neworder[newi] != -1)
                ++newi;
            *it_vc = newi;
            neworder[newi++] = value;
        }
    }

    memmove(data, neworder, oldOrderSize*sizeof(int));

#ifdef INTEGRITY_CHECK
    integrityCheck();
#endif
    flushModelToVisualMap();

    emit orderChanged(myType, visChange, oldOrderSize);

    delete[] visChange;
    delete[] neworder;
}

QDomElement QicsSorter::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = doc->createElement(tag);

    QStringList orderList;
    foreach(int i, m_order)
        orderList << QString::number(i);

    QString attrValue = orderList.join(",");
    e.setAttribute("order", attrValue);

    QStringList mapList;
    foreach(int i, m_modelToVisualMap)
        mapList << QString::number(i);

    attrValue = mapList.join(",");
    e.setAttribute("map", attrValue);

    return e;
}

void QicsSorter::configureFromDomXml(const QDomElement& e)
{
    QStringList orderList = e.attribute("order").split(",");
    m_order.clear();
    foreach(QString str, orderList)
        if(!str.isEmpty())
            m_order << str.toInt();

    QStringList mapList = e.attribute("map").split(",");
    m_modelToVisualMap.clear();
    int index = 0;
    foreach(const QString &str, mapList)
        if(!str.isEmpty()) {
            m_modelToVisualMap.insert(index, str.toInt());
            ++index;
        }
}

void QicsSorter::reorder(const QVector<int> &newOrder, int from, int to)
{
    register int i,j;

    fillVisualToModelMap();

    const int oldOrderSize = m_order.size();

    if (from < 0)
        from = 0;
    if (to < 0 || to >= oldOrderSize)
        to = oldOrderSize - 1;

    if (!from && to == oldOrderSize - 1)
        m_order = newOrder;
    else
        for (i = from, j = 0; i <= to; ++i, ++j)
            m_order[i] = newOrder.at(j);

    // save off a copy of the order so we can build the change map
    int *visChange = new int[oldOrderSize];
    memmove(visChange, m_order.constData(), oldOrderSize*sizeof(int));

    flushModelToVisualMap();

    /* Now we compute the mapping of old visual to new visual.
    * For old visual i, visChange[i] is the model index,
    * modelToVisualMap of that gives the current visual.
    */
    fillModelToVisualMap();

    for(i = 0; i < oldOrderSize; ++i)
        visChange[i] = m_modelToVisualMap.value(visChange[i]);

    emit orderChanged(myType, visChange, oldOrderSize);

    delete[] visChange;
}
