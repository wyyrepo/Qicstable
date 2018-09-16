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

#include <QicsTreeTable.h>

#include <QStyleOption>
#include <QPainter>

#include <QicsTreeDataModel.h>
#include <QicsGroupCellDisplay.h>
#include <QicsSelectionManager.h>
#include <QicsStyleManager.h>
#include <QicsSpanManager.h>
#include <QicsColumnHeader.h>
#include <QicsListFilterDelegate.h>
#include <QicsFilter.h>
#include <QicsGridLayout.h>
#include <QicsTreeTableGrid.h>
#include <QicsTreeHeaderGrid.h>
#include <QicsSummarizer.h>
#include <QicsSpecialRowData.h>
#include <QicsExpandableStaticRowData.h>
#include <QicsUtil.h>



QicsTreeTable::QicsTreeTable(bool useHeader, QWidget *parent)
    : QicsTable(0,0, QicsTreeTableGrid::createGrid,
            QicsTreeHeaderGrid::createGrid, 0, parent)
{
    doInit(0, useHeader);
}

QicsTreeTable::QicsTreeTable(QicsDataModel *model, bool useHeader, QWidget *parent)
    : QicsTable(0,0, QicsTreeTableGrid::createGrid,
            QicsTreeHeaderGrid::createGrid, model, parent)
{
    doInit(model, useHeader);
}

QicsTreeTable::QicsTreeTable(QicsDataModel::Foundry rhdmf,
                             QicsDataModel::Foundry chdmf,
                             QicsTableGrid::Foundry tf,
                             QicsHeaderGrid::Foundry hf,
                             QicsDataModel *model,
                             bool useHeader,
                             QWidget *parent)
    : QicsTable(rhdmf, chdmf, tf ? tf : QicsTreeTableGrid::createGrid,
                             hf ? hf : QicsTreeHeaderGrid::createGrid,
                             model, parent)
{
    doInit(model, useHeader);
}

QicsTreeTable::QicsTreeTable(void *key,
                             QicsDataModel::FoundryWithKey rhdmf_wk,
                             QicsDataModel::FoundryWithKey chdmf_wk,
                             QicsTableGrid::FoundryWithKey tf_wk,
                             QicsHeaderGrid::FoundryWithKey hf_wk,
                             QicsDataModel *model,
                             bool useHeader,
                             QWidget *parent)
    : QicsTable(key, rhdmf_wk, chdmf_wk,
                             tf_wk ? tf_wk : QicsTreeTableGrid::createGridWithKey,
                             hf_wk ? hf_wk : QicsTreeHeaderGrid::createGridWithKey,
                             model,
                             parent)
{
    doInit(model, useHeader);
}

void QicsTreeTable::setDataModel(QicsDataModel *model)
{
    // #### TODO: disconnect old model

    m_userModel = model;

    if (m_userModel)
        QicsTable::setDataModel(new QicsViewTreeDataModel(m_userModel, !m_treeInHeader));
    else
        QicsTable::setDataModel(0);

    doInitNullColumn();

    if (m_userModel) {
        connect(m_userModel, SIGNAL(cellValueChanged(int,int)), this, SLOT(onCellValueChanged(int,int)));

        connect(m_userModel, SIGNAL(rowsInserted(int,int)), this, SLOT(onRowsAdded(int,int)));
        connect(m_userModel, SIGNAL(rowsDeleted(int,int)), this, SLOT(onRowsRemoved(int,int)));
        connect(m_userModel, SIGNAL(rowsAdded(int)), this, SLOT(onRowsAdded(int)));

        connect(m_userModel, SIGNAL(prepareForRowChanges(int,int)), this, SLOT(beforeRowsAdded(int,int)));

        connect(m_userModel, SIGNAL(columnsInserted(int,int)), this, SLOT(onColumnsAdded(int,int)));
        connect(m_userModel, SIGNAL(columnsDeleted(int,int)), this, SLOT(onColumnsRemoved(int,int)));
        connect(m_userModel, SIGNAL(columnsAdded(int)), this, SLOT(onColumnsAdded(int)));
    }
}

void QicsTreeTable::doInit(QicsDataModel *model, bool useHeader)
{
    m_sortOrder = Qics::Ascending;
    m_from = 0; m_to = -1;
    m_func = 0;

    m_treeInHeader = useHeader;
    m_groupColsShown = false;
    m_expandPolicy = DisplayerDesides;
    m_treeMode = Tree;

    m_topGroup = new QicsGroupInfo(this);

    m_summaryPolicy = SummaryEnabled;
    m_dclickGroup = m_dclickSort = true;

    m_summarizer = NULL;
    m_groupDisplay = new QicsGroupCellDisplay(this);
    m_summaryDisplay = new QicsGroupCellDisplay(this);

    m_IsHeaderBeganResize = false;
    m_DoingResize = false;
    m_wasResized = false;

    connect(this, SIGNAL(doubleClicked(int,int,int,const QPoint &)), this, SLOT(onDoubleClicked(int,int,int,const QPoint &)));
    connect(this, SIGNAL(clicked(int,int,int,const QPoint &)), this, SLOT(onClicked(int,int,int,const QPoint &)));

    connect(columnHeader(), SIGNAL(doubleClicked(int,int,int,const QPoint&)), this, SLOT(onHeaderDoubleClicked(int,int,int,const QPoint&)));

    setDataModel(model);
}

void QicsTreeTable::doInitNullColumn()
{
    if (!dataModel())
        return;

    if (m_treeInHeader) {
        QicsColumn &col = rowHeaderRef().columnRef(0);

        col.setReadOnly(true);
        col.setStretchable(false);

        rowHeaderRef().columnRef(0).setDisplayer(m_groupDisplay);

    } else  {
        QicsColumn &col = columnRef(0);

        col.setReadOnly(true);
        col.setStretchable(false);
        col.setWidthInPixels(0);
        col.hide();

        columnHeaderRef().cellRef(0,0).setDataValue(QicsDataString(""));
    }
}

QicsTreeTable::~QicsTreeTable()
{
    delete m_topGroup;
    delete m_summarizer;
}

/////////////////////////////////////////////////////////////////////////////////
// Displayers

void QicsTreeTable::setGroupDisplayer(QicsGroupCellDisplay *cd)
{
    delete m_groupDisplay;
    m_groupDisplay = cd;
}

void QicsTreeTable::setSummaryDisplayer(QicsGroupCellDisplay *cd)
{
    delete m_summaryDisplay;
    m_summaryDisplay = cd;
}

/////////////////////////////////////////////////////////////////////////////////
// Special Rows

void QicsTreeTable::addSpecialRow(int index, QicsSpecialRowData *rowData)
{
    QicsViewTreeDataModel *dm = viewModel();
    if (dm) {
        dm->addSpecialRow(index, rowData);
        if (rowData && !dm->m_delayInsertSignals) {
            rowData->init(rowRef(index));
        }
        m_specialRows.insert(index);
    }
}

void QicsTreeTable::removeSpecialRow(int index)
{
    QicsViewTreeDataModel *dm = viewModel();
    if (dm) {
        QicsSpecialRowData* data = specialRowData(index);
        if (data) {
            data->dispose(rowRef(index));
            delete data;
        }
        dm->removeSpecialRow(index);
        m_specialRows.remove(index);
    }
}

void QicsTreeTable::removeSpecialRows()
{
    m_topGroup->clear();

    m_rowGroupMap.clear();

    m_groupData.clear();
    m_summaryRows.clear();
    m_specialRows.clear();

    // ################## REVISE ######################

    styleManager()->spanManager()->removeAllSpans();

    QicsViewTreeDataModel *dm = viewModel();
    if (dm) {

        QMap<int, QicsSpecialRowData*>::iterator it;
        for (it = dm->m_specRows.begin(); it != dm->m_specRows.end(); ++it)
            if (*it) {
                (*it)->dispose(rowRef(it.key()));
            }

        dm->removeSpecialRows();
    }
}

void QicsTreeTable::onClicked(int row,int col,int button,const QPoint &p)
{
    if (m_expandPolicy == DisplayerDesides && m_groupDisplay) {
        m_groupDisplay->handleClick(row, col, button, p);
        return;
    }

    // find matching special row
    int r = gridInfo().modelRowIndex(row);
    QicsSpecialRowData *data = specialRowData(r);
    if (data)
        data->handleClick(row, col, button, p);
}

void QicsTreeTable::onDoubleClicked(int row,int col,int button,const QPoint &p)
{
    if (m_expandPolicy == DisplayerDesides && m_groupDisplay) {
        m_groupDisplay->handleDoubleClick(row, col, button, p);
        return;
    }

    // find matching special row
    int r = gridInfo().modelRowIndex(row);
    QicsSpecialRowData *data = specialRowData(r);
    if (data)
        data->handleDoubleClick(row, col, button, p);
}

const QicsRow* QicsTreeTable::specialRow(QicsSpecialRowData *rowData) const
{
    int i = specialRowIndex(rowData);
    return i >= 0 ? row(gridInfo().visualRowIndex(i)) : 0;
}

int QicsTreeTable::specialRowIndex(QicsSpecialRowData *rowData) const
{
    QicsViewTreeDataModel *dm = viewModel();
    return dm ? dm->specialRowIndex(rowData) : -1;
}

QicsSpecialRowData* QicsTreeTable::specialRowData(int index) const
{
    QicsViewTreeDataModel *dm = viewModel();
    return dm ? dm->specialRowData(index) : 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Grouping

void QicsTreeTable::showGroupedColumns(bool set)
{
    if (set != m_groupColsShown) {
        m_groupColsShown = set;
        for (int i = 0; i < m_groups.count(); ++i) {
            if (set)
                columnRef(gridInfo().visualColumnIndex(m_groups.at(i))).show();
            else
                columnRef(gridInfo().visualColumnIndex(m_groups.at(i))).hide();
        }
    }
}

void QicsTreeTable::setTreeMode(QicsTreeMode type)
{
    if (m_treeMode != type) {
        m_treeMode = type;
        if (m_groups.count())
            regroup();
    }
}

void QicsTreeTable::groupColumn(int column)
{
    if (!m_userModel)
        return;

    if (m_treeInHeader) {
        if (column < 0 || column >= m_userModel->numColumns())
            return;
    } else
        if (column < 1 || column > m_userModel->numColumns())
            return;

    if (m_groups.contains(column))
        return;

    if (m_groups.size() == m_userModel->numColumns()-1)
        return;

    QList<int> groups = m_groups;
    groups.append(column);
    groupColumns( groups );
}

void QicsTreeTable::groupColumns(const QList<int> &columns)
{
    if (!m_userModel)
        return;

    emit groupingInProgress(true);

    setRepaintBehavior(Qics::RepaintOff);

    doUngroupTable();

    int col_num = m_userModel->numColumns();
    int row_num = m_userModel->numRows();

    m_groups.clear();
    for (int i = 0; i < columns.count(); ++i) {
        int idx = columns.at(i);
        if (idx < 0 || idx > col_num || m_groups.count(idx))
            continue;
        m_groups.append(idx);
    }

    if (m_groups.size() >= col_num-1)
        m_groups = m_groups.mid(0, col_num-1);

    for (int i = 0; i < m_groups.count(); ++i) {
        emit groupAdded(m_groups.at(i));
        if (!m_groupColsShown)
            columnRef(gridInfo().visualColumnIndex(m_groups.at(i))).hide();
    }

    doGroupTable();
    doFilterTable();
    doSortTable();

    if ( ( m_treeMode == Flat_No_Details ) && m_groups.count() )
        for ( int i = 0; i < row_num; ++i )
            gridInfo().dimensionManager()->hideRow(i);

    m_wasResized = false;
    setRepaintBehavior(Qics::RepaintOn);
    update();

    emit groupingInProgress(false);
}

void QicsTreeTable::regroup()
{
    QList<int> columns = m_groups;
    ungroup();
    groupColumns( columns );
}

void QicsTreeTable::ungroupColumn(int column)
{
    if (!m_userModel)
        return;

    if (m_treeInHeader) {
        if (column < 0 || column >= m_userModel->numColumns())
            return;
    } else
        if (column < 1 || column > m_userModel->numColumns())
            return;

    if (!m_groups.contains(column))
        return;

    columnRef(gridInfo().visualColumnIndex(column)).show();

    m_groups.removeAll(column);

    QList<int> groups = m_groups;
    groupColumns( groups );

    emit groupRemoved(column);
}

void QicsTreeTable::ungroup()
{
    if (m_groups.isEmpty())
        return;

    emit groupingInProgress(true);

    setRepaintBehavior(Qics::RepaintOff);

    doUngroupTable();

    if (!m_treeInHeader) {
        columnRef(0).setWidthInPixels(0);
        columnRef(0).hide();
    }

    doFilterTable();
    doSortTable();

    setRepaintBehavior(Qics::RepaintOn);
    repaint();

    emit groupingInProgress(false);
}

void QicsTreeTable::doUngroupTable()
{
    for (int i = m_groups.count()-1; i >= 0; --i) {
        int c = m_groups[i];
        columnRef(gridInfo().visualColumnIndex(c)).show();
        m_groups.removeAll(c);
        emit groupRemoved(c);
    }

    removeSpecialRows();

    emit groupingChanged();
}

void QicsTreeTable::doGroupTable()
{
    if (!m_userModel) return;

    bool old_smrc = styleManager()->isReportingChanges();
    styleManager()->setReportChanges(false);

    removeSpecialRows();

    if (m_groups.size()) {

        QicsViewTreeDataModel* model = viewModel();

        QHash<QString, TreeItem*> map;
        TreeItem *root = new TreeItem();
        root->row = 0;

        // build tree
        for (int i = 0; i < model->numRows(); ++i) {
            // make id string
            QString s_id, s_item,label;
            int group;
            TreeItem *wr = 0;
            for (int k = 0; k < m_groups.size(); ++k) {
                s_item = model->itemString(i, (group=m_groups.at(k)));
                s_id += QString("%1.%2.").arg(group).arg(qicsCryptData(s_item.toLocal8Bit()).constData());

                if ( m_treeMode == Tree ) {
                    if (!map.contains(s_id)) {
                        wr = new TreeItem(k ? wr : root);
                        map[s_id] = wr;
                    } else
                        wr = map.value(s_id);
                    wr->content = s_item;
                    wr->group = group;
                } else {
                    if ( k )
                        label +=QString(" => %1").arg(s_item);
                    else
                        label +=QString("%1").arg(s_item);
                }
            }
            if ( m_treeMode != Tree ) {
                if (!map.contains(s_id)) {
                    wr = new TreeItem(root);
                    map[s_id] = wr;
                    wr->content = label;
                    wr->group = group = m_groups.first();
                } else
                    wr = map.value(s_id);
            }
            TreeItem *wi = new TreeItem(wr);
            wi->row = i;
        }

        // now we have this tree - time to convert it into rows
        QVector<int> v;

        model->startSpecialInsertion();
        doConvertTree(root, model, &v, -1, 0, m_topGroup);
        model->finalizeSpecialInsertion();

        // clear all the older items
        delete root;
        //qDeleteAll(map);
        map.clear();

        //qDebug() << v;

        QicsSorter * sorter = gridInfo().rowOrdering();
        if ( sorter )
            sorter->reorder(v);

        if (m_treeInHeader) {
            //rowHeaderRef().columnRef(0).setWidthInPixels(m_groups.size()*8+8);
        }
        else {
            columnRef(0).setWidthInPixels(m_groups.size()*8+8);
            columnRef(0).show();
        }
    } else {
        if (m_treeInHeader) {
        }
        else {
            columnRef(0).setWidthInPixels(0);
            columnRef(0).hide();
        }
    }

    if (m_summarizer && m_groups.size()) {
        for (int i = 0; i < topGroupRef().count(); ++i)
            iterateGroups(topGroupRef().childAt(i));
    }

    styleManager()->setReportChanges(old_smrc);

    emit groupingChanged();
}

void QicsTreeTable::doConvertTree(TreeItem *ti, QicsViewTreeDataModel* model, QVector<int> *ord, int deep,
                                  QicsExpandableStaticRowData *lastRow,
                                  QicsGroupInfo *group)
{
    QicsExpandableStaticRowData *erow = 0;

    if (ti->row == -1) {
        int sr_index = model->numRows();
        if (model->sr_start == -1) model->sr_start = sr_index;
        ord->append(sr_index);
        m_rowGroupMap[sr_index] = group;

        m_groupData[ti->group].add(sr_index, ti->content);
        group->setHeaderIndex(sr_index);
        group->setColumnIndex(ti->group);
        group->setLevel(deep);

        group->setContent(groupText(ti->group));
        group->setData(ti->content);
        addSpecialRow(sr_index, erow = new QicsExpandableStaticRowData(this, group));

        if (lastRow)
            lastRow->addChild(sr_index);

        model->sr_count++;
    }

    TreeItem *ti_c;
    QList<TreeItem*>::iterator it, it_end = ti->list.end();
    for (it = ti->list.begin(); it != it_end; ++it) {
        ti_c = *it;
        if (ti_c->row == -1) {
            QicsGroupInfo *gi = group->addGroup();
            doConvertTree(ti_c, model, ord, deep+1, erow, gi);
        } else {
            ord->append(ti_c->row);
            m_rowGroupMap[ti_c->row] = group;
            erow->addChild(ti_c->row);
            group->addRow(ti_c->row);
        }
    }

    if (erow) {
        int i = model->numRows();
        addSpecialRow(i, new QicsSpecialRowData(this));
        erow->addChild(i);
        ord->append(i);
        m_rowGroupMap[i] = group;
        group->setSummaryIndex(i);
        m_summaryRows.insert(i);

        model->sr_count++;
    }
}

void QicsTreeTable::setSummaryPolicy(int policy)
{
    m_summaryPolicy = (QicsSummaryPolicy)policy;

    setRepaintBehavior(Qics::RepaintOff);
    QSet<int>::const_iterator it;
    for (it = m_specialRows.constBegin(); it != m_specialRows.constEnd(); ++it) {
        QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(specialRowData(*it));
        if (erow)
            erow->doExpand(erow->m_open, true, false);
    }
    setRepaintBehavior(Qics::RepaintOn);
    repaint();
}

void QicsTreeTable::iterateGroups(QicsGroupInfo *gi)
{
    if (m_summarizer)
        m_summarizer->onGrouping(gi);
    if (gi->count())
        for (int i = 0; i < gi->count(); ++i)
            iterateGroups(gi->childAt(i));
}

QString QicsTreeTable::groupText(int id)
{
    QicsDataModel* model = columnHeaderRef().gridInfo().dataModel();
    if (id < 0 || id >= model->numColumns())
        return QString();

    return model->itemString(0, id);
}

/////////////////////////////////////////////////////////////////////////////////
// Callbacks from model when it is changed

void QicsTreeTable::onCellValueChanged(int row, int col)
{
    if (!m_treeInHeader)
        col++;

    // check if there is grouping by col
    if (m_groups.contains(col)) {
        // regroup
        QList<int> tmp(m_groups);
        groupColumns(tmp);
    } else {
        setRepaintBehavior(Qics::RepaintOff);
        doFilterTable();
        doSortTable();

        // here we should handle summarizing...
        if (m_summarizer)
            m_summarizer->onCellValueChanged(row, col, m_rowGroupMap[row]);

        setRepaintBehavior(Qics::RepaintOn);
        repaint();
    }
}

void QicsTreeTable::onRowsAdded(int count, int index)
{
    Q_UNUSED(index);
    onRowsAdded(count);
}

void QicsTreeTable::beforeRowsAdded(int count, int index)
{
    Q_UNUSED(count);
    Q_UNUSED(index);

    if (m_groups.size()) {
        // regroup
        m_tmpGroups = m_groups;
        setRepaintBehavior(Qics::RepaintOff);
        doUngroupTable();
    }
}

void QicsTreeTable::onRowsAdded(int count)
{
    Q_UNUSED(count);

    repaint();

    if (m_tmpGroups.size()) {
        // regroup
        groupColumns(m_tmpGroups);
        m_tmpGroups.clear();
    }
}

void QicsTreeTable::onRowsRemoved(int count, int index)
{
    Q_UNUSED(count);
    Q_UNUSED(index);

    if (m_tmpGroups.size()) {
        // regroup
        groupColumns(m_tmpGroups);
        m_tmpGroups.clear();
    }
}

void QicsTreeTable::onColumnsAdded(int count, int index)
{
    if (!m_treeInHeader)
        index++;

    if (m_groups.size()) {
        // renumber groups and filters
        QList<int> tmpg;
        for (int i = 0; i < m_groups.count(); ++i) {
            int j = m_groups.at(i);
            if (j >= index)
                tmpg.append(j + count);
            else
                tmpg.append(j);
        }

//        QMap<int, QicsAbstractFilterDelegate* > tmpf;
//        QMap<int, QicsAbstractFilterDelegate* >::iterator it;
        QMap<int, QPointer<QicsAbstractFilterDelegate> > tmpf;
        QMap<int, QPointer<QicsAbstractFilterDelegate> >::iterator it;
        for (it = m_filters.begin(); it != m_filters.end(); ++it) {
            int j = it.key();
            if (j >= index)
                tmpf[j + count] = *it;
            else
                tmpf[j] = *it;
        }

        // regroup
        m_filters = tmpf;
        groupColumns(tmpg);
    }

    repaint();
}

void QicsTreeTable::onColumnsAdded(int count)
{
    Q_UNUSED(count);
}

void QicsTreeTable::onColumnsRemoved(int count, int index)
{
    if (!m_treeInHeader)
        index++;

    if (m_groups.size()) {
        // remove index columns
        for (int i = 0; i < count; ++i) {
            m_groups.removeAll(index+i);
            emit groupRemoved(index+i);
        }

        // renumber groups and filters
        QList<int> tmpg;
        for (int i = 0; i < m_groups.count(); ++i)
            if (index <= m_groups.at(i))
                tmpg.append(m_groups.at(i) - count);
            else
                tmpg.append(m_groups.at(i));

//        QMap<int, QicsAbstractFilterDelegate* > tmpf;
//        QMap<int, QicsAbstractFilterDelegate* >::iterator it;
        QMap<int, QPointer<QicsAbstractFilterDelegate> > tmpf;
        QMap<int, QPointer<QicsAbstractFilterDelegate> >::iterator it;
        for (it = m_filters.begin(); it != m_filters.end(); ++it)
            if (index <= it.key()) {
                tmpf[it.key() - count] = *it;
            }
            else
                tmpf[it.key()] = *it;

        // regroup
        m_filters = tmpf;
        groupColumns(tmpg);
    }

    repaint();
}


/////////////////////////////////////////////////////////////////////////////////
// Filtering

void QicsTreeTable::setRowFilter(int column, QicsAbstractFilterDelegate *filter, bool deleteOld)
{
    if (!filter) {
        removeRowFilter(column, deleteOld);
        return;
    }

    m_filters[column] = filter;

    emit filterChanged(column);

    setRepaintBehavior(Qics::RepaintOff);
    doFilterTable();
    setRepaintBehavior(Qics::RepaintOn);
    repaint();
}

QicsAbstractFilterDelegate *QicsTreeTable::rowFilter(int column)
{
    if (!m_filters.contains(column)) return 0;
    return m_filters.value(column);
}

void QicsTreeTable::removeRowFilter(int column, bool deleteOld)
{
    if (!m_filters.contains(column)) return;

    if (m_filters.size() == 1) {//#89836 reseting a filter fix
        QicsTable::removeRowFilter(column, deleteOld);
        //since QicsTable::m_filtes and QicsTreeTable::m_filters contain the same pointers,
        //QicsTabel::removeRowFilter deletes a filter.
        m_filters.remove(column);
    } else {
        QicsAbstractFilterDelegate *filter = m_filters.take(column);
        if (deleteOld)
            delete filter;

        emit filterChanged(column);

        QicsTable::removeRowFilter(column);
    }

    setRepaintBehavior(Qics::RepaintOff);
    doFilterTable();
    setRepaintBehavior(Qics::RepaintOn);
    repaint();
}

void QicsTreeTable::removeAllRowFilters()
{
//    QMap<int, QicsAbstractFilterDelegate* >::const_iterator it;
    QMap<int, QPointer<QicsAbstractFilterDelegate> >::const_iterator it;
    for (it = m_filters.constBegin(); it != m_filters.constEnd(); ++it) {
        m_filters.remove(it.key());
        emit filterChanged(it.key());
    }

    setRepaintBehavior(Qics::RepaintOff);
    doFilterTable();
    setRepaintBehavior(Qics::RepaintOn);
    repaint();
}

void QicsTreeTable::doFilterTable()
{
    m_filteredRows.clear();
    gridInfo().rowFilter()->removeAll();

    if (!dataModel())
        return;

    // unfilter all - ???
    int rows = viewModel()->numRows();
    for (int i = 0; i < rows; ++i)
        rowRef(i).show();

    if (!m_groups.size()) {
//        for (QMap<int, QicsAbstractFilterDelegate*>::const_iterator it = m_filters.constBegin(); it != m_filters.constEnd(); ++it) {
        for (QMap<int, QPointer<QicsAbstractFilterDelegate> >::const_iterator it = m_filters.constBegin(); it != m_filters.constEnd(); ++it) {
            QicsTable::setRowFilter(it.key(), it.value());
        }
        return;
    }

    QMap<QString, QList<int> >::const_iterator it;
    QList<int> lr;

    // iterate over grouped columns to build m_filteredRows
    for (int i = 0; i < m_groups.size(); ++i) {
        int group = m_groups.at(i);
        if (!(m_filters.contains(group)))
            continue;

        QicsListFilterDelegate *lf = qobject_cast<QicsListFilterDelegate*>(m_filters.value(group));
        if (!lf)
            continue;

        const QMap<QString, QList<int> > data = m_groupData.value(group).content;//#90025
        for (it = data.constBegin(); it != data.constEnd(); ++it) {
            if (!lf->match(it.key(), -1, -1)) {
                // get list of special rows
                lr = it.value();

                for (int j = 0; j < lr.size(); ++j) {
                    int index = lr.value(j);
                    doFilterRow(index);
                }
            }
        }
    }

    // filter top-level empty groups
    if (m_filters.count())
        for (QSet<int>::const_iterator it1 = m_specialRows.constBegin(); it1 != m_specialRows.constEnd(); ++it1) {
            int index = *it1;
            QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(specialRowData(index));
            if (erow)
                if (doFilterRowCheck(erow))
                    doFilterRow(index);
        }


    // hide all filtered rows
    bool old_sig = dimensionManager()->emitSignals();

    for (int i = 0; i < m_filteredRows.size(); ++i)
        rowRef(gridInfo().visualRowIndex(m_filteredRows.at(i))).hide();

    dimensionManager()->setEmitSignals(old_sig);

    // restore tree state - reexpand top level items
    const QMap<QString, QList<int> > data = m_groupData.value(m_groups.value(0)).content;//#90025
    for (it = data.constBegin(); it != data.constEnd(); ++it) {
        // get list of special rows
        lr = it.value();
        for (int j = 0; j < lr.size(); ++j) {
            QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(specialRowData(lr.value(j)));
            if (erow)
                erow->doExpand(erow->m_open, true, false);
        }
    }
}

bool QicsTreeTable::doFilterRowCheck(QicsExpandableStaticRowData *erow)
{
    int size = erow->m_list.size();
    int idx;
    bool filter = true;

    for (int k = 0; k < size-1; ++k) {
        idx = erow->m_list.at(k);

        QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(specialRowData(idx));
        if (erow) {

            if (isRowFiltered(idx))
                continue;

            if (doFilterRowCheck(erow)) {
                doFilterRow(idx);
            } else
                filter = false;
        } else {
            bool pass = true;
//            for (QMap<int, QicsAbstractFilterDelegate*>::iterator it = m_filters.begin(); it != m_filters.end(); ++it) {
            for (QMap<int, QPointer<QicsAbstractFilterDelegate> >::iterator it = m_filters.begin(); it != m_filters.end(); ++it) {
                if (!it.value())
                    continue;
                const int col = it.key();
                const int row = idx;
                QString s = viewModel()->itemString(row, col);
                if (!it.value()->match(s, row, col)) {
                    m_filteredRows.append(idx);
                    pass = false;
                    break;
                }
            }
            if (pass)
                filter = false;
        }
    }
    return filter;
}

void QicsTreeTable::doFilterRow(int index)
{
    m_filteredRows.append(index);

    QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(specialRowData(index));
    if (!erow) return;

    for (int i = 0; i < erow->m_list.size(); ++i) {
        doFilterRow(erow->m_list.at(i));
    }
}

/////////////////////////////////////////////////////////////////////////////////
// Sorting

void QicsTreeTable::sortRows(QVector<int> colsnum, Qics::QicsSortOrder order,
                        int from, int to,
                        DataItemComparator func)
{
    m_sorted = colsnum;
    m_sortOrder = order;
    m_from = from;
    m_to = to;
    m_func = func;

    setRepaintBehavior(Qics::RepaintOff);
    doSortTable();
    setRepaintBehavior(Qics::RepaintOn);
    repaint();

    emit rowsSorted();
}

void QicsTreeTable::unsortRows()
{
    m_sorted = QVector<int>();

    emit rowsSorted();
}

void QicsTreeTable::sortColumn(int col)
{
    if (!m_sorted.contains(col)) {
        m_sorted.append(col);
        sortRows(m_sorted, columnSortOrder());
    }
}

void QicsTreeTable::unsortColumn(int col)
{
    int i = m_sorted.indexOf(col);
    if (i >= 0) {
        m_sorted.remove(i);
        sortRows(m_sorted, columnSortOrder());
    }
}

void QicsTreeTable::doSortTable()
{
    if (!m_groups.count()) {
        gridInfo().orderRowsBy(m_sorted, m_sortOrder, m_from, m_to, m_func, true);
        return;
    }

    // sort node items from all the groups
    QVector<int> v;

    // build list for the 1st group
    GroupItem &gi = m_groupData[m_groups.first()];
    QMap<QString, QList<int> >::const_iterator it;
    QVector<int> list;
    for (it = gi.content.begin(); it != gi.content.end(); ++it)
        list.append(it.value().first());	// only 1 list item per key in 1st group

    doProcessSort(list, v, 0);

    gridInfo().rowOrdering()->reorder(v);

    // sort leaf items from the last group
    GroupItem &gi1 = m_groupData[m_groups.last()];
    for (it = gi1.content.begin(); it != gi1.content.end(); ++it) {
        for (int j = 0; j < it.value().count(); j++) {
            QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(specialRowData(it.value().at(j)));
            if (!erow || erow->m_list.isEmpty()) continue;

            int from = gridInfo().visualRowIndex(erow->m_list.first());
            int to = gridInfo().visualRowIndex(erow->m_list.last())-1;   // -1 because skipping summary row

            gridInfo().orderRowsBy(m_sorted, m_sortOrder, from, to, m_func, true);
        }
    }
}

void QicsTreeTable::doProcessSort(QVector<int> l, QVector<int>& v, int deep)
{
    if (deep < m_groups.count()) {

        int group = m_groups[deep];
        if (m_sorted.contains(group)) {
            /*	A quite slow place - we are building map ordered by unique keys
            and then filling list of row indexes in appropriate order.
            But to get column key from node row index, we are passing deep
            through all the column items sequentally for every row.
            This is slowdown...
            Perhaps we should be able to use own sorter here?
            */
            GroupItem &gi = m_groupData[group];
            QMap<QString, QList<int> >::const_iterator it;
            QMap<QString, int> map;
            for (int i = 0; i < l.count()-(!!deep); ++i) {            // -1 because skipping summary row
                int idx = l[i];
                for (it = gi.content.begin(); it != gi.content.end(); ++it)
                    if ((*it).contains(idx)) {
                        map[it.key()] = idx;
                        break;
                    }
            }

            // we have to remember the last item (as it is the summary row)
            int last = l.last();

            // QMap holds items in ascending order - so we're just iterating here
            l.clear();
            QMap<QString, int>::const_iterator it1;
            if (m_sortOrder == Qics::Ascending) {
                for (it1 = map.begin(); it1 != map.end(); ++it1)
                    l.append(*it1);
            } else {
                for (it1 = map.begin(); it1 != map.end(); ++it1)
                    l.prepend(*it1);
            }

            // and append the summary row as well
            if (deep)
                l.append(last);
        }
    }

    for (int i = 0; i < l.count(); ++i) {
        v.append(l[i]);
        QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(specialRowData(l[i]));
        if (!erow || erow->m_list.isEmpty())
            continue;

        QVector<int> l1(erow->m_list);
        doProcessSort(l1, v, deep+1);
    }
}

/////////////////////////////////////////////////////////////////////////////////
// Navigation

void QicsTreeTable::onHeaderDoubleClicked(int /*row*/, int col, int button, const QPoint &/*pos*/)
{
    int idx = gridInfo().modelColumnIndex(col);
    switch (button)
    {
    case Qt::LeftButton:
        if (m_dclickGroup)
            groupColumn(idx);
        break;
    case Qt::RightButton:
        if (m_dclickSort)
            sortColumn(idx);
        break;
    default:
        break;
    }
}

void QicsTreeTable::doExpandTree(bool expand)
{
    setRepaintBehavior(Qics::RepaintOff);

    for (int i = 0; i < m_topGroup->count(); ++i) {
        QicsGroupInfo *group = m_topGroup->childAt(i);
        group->headerRowData()->expandTree(expand, false);
    }

    setRepaintBehavior(Qics::RepaintOn);
    repaint();
}

void QicsTreeTable::paintEvent ( QPaintEvent * event )
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOption opt(0);
    opt.rect = splitterRect();
    opt.palette = palette();
    opt.state = QStyle::State_Horizontal;
    opt.state |= QStyle::State_Enabled;
    style()->drawControl(QStyle::CE_Splitter, &opt, &p, this);
}

void QicsTreeTable::mouseMoveEvent( QMouseEvent *m )
{
    QicsHeader hdr(&(gridInfo()));
    if (hdr.allowHeaderResize() ) {
        if ( !m_DoingResize ) {
            QRect rect = splitterRect();
            if ( rect.contains( mapFromGlobal( m->globalPos () ) ) ){
                setCursor( Qt::SizeHorCursor );
            }
            else {
                if(cursor().shape() == Qt::SizeHorCursor || cursor().shape() == Qt::SizeVerCursor ||
                    cursor().shape() == Qt::SizeFDiagCursor || cursor().shape() == Qt::SizeBDiagCursor) {
                        setCursor(Qt::ArrowCursor);
                }
            }
        }
        else {
            QRect rect = gridLayout()->cellRect( GRID_LAYOUT_LEFT_SCROLLBAR_IDX, GRID_LAYOUT_LEFT_HDR_IDX );
            int newX = mapFromGlobal( m->globalPos () ).x();
            int newWidth =  newX - rect.x() + 1;
            int minWidth = 0;

            QWidget * wd = gridLayout()->widgetAt( GRID_LAYOUT_TOP_HDR_IDX, GRID_LAYOUT_LEFT_HDR_IDX );
            if ( wd )
                minWidth = qMax( minWidth, wd->minimumSizeHint().width() );
            wd = gridLayout()->widgetAt( GRID_LAYOUT_BOTTOM_HDR_IDX, GRID_LAYOUT_LEFT_HDR_IDX );
            if ( wd )
                minWidth = qMax( minWidth, wd->minimumSizeHint().width() );

            if ( newWidth  > minWidth ) {
                int dd = gridLayout()->spacing()+ gridLayout()->margin();
                rhDimensionManager()->setColumnWidthInPixels(-1, false, newWidth - dd);
                gridLayout()->update();
            }
        }
    }
}

void QicsTreeTable::mousePressEvent( QMouseEvent *m )
{
    QicsHeader hdr(&(gridInfo()));
    if (hdr.allowHeaderResize() && !m_DoingResize ) {
        QRect rect = splitterRect();
        if ( rect.contains( mapFromGlobal( m->globalPos () ) ) ){
            m_DoingResize = true;
            m_wasResized = true;
        }
    }
}

void QicsTreeTable::mouseReleaseEvent(QMouseEvent *)
{
    if (m_DoingResize) {
        m_DoingResize = false;
        setCursor(Qt::ArrowCursor);
    }
}

QRect QicsTreeTable::splitterRect()
{
    QRect rect = gridLayout()->cellRect( GRID_LAYOUT_LEFT_SCROLLBAR_IDX, GRID_LAYOUT_LEFT_HDR_IDX );
    int x = rect.x() + rect.width() - gridLayout()->margin() ;
    int y = gridLayout()->cellRect( GRID_LAYOUT_TOP_HDR_IDX, GRID_LAYOUT_LEFT_FROZEN_GRID_IDX ).y();
    int h = gridLayout()->cellRect( GRID_LAYOUT_BOTTOM_SCROLLBAR_IDX, GRID_LAYOUT_LEFT_FROZEN_GRID_IDX ).y() - y;
    return QRect( x,y, gridLayout()->spacing() + gridLayout()->margin()*2, h);
}

void QicsTreeTable::headerResizeNeeds(int width)
{
    if ( !m_wasResized )
        resizeHeader( width, 0, RowHeader );
}

void QicsTreeTable::setSummarizer(QicsSummarizer *summarizer)
{
    delete m_summarizer;
    m_summarizer = summarizer;
}

void QicsTreeTable::removeSummarizer()
{
    delete m_summarizer;
    m_summarizer = 0;
}


