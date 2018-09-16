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

#include <QicsExpandableStaticRowData.h>

#include <QicsGroupInfo.h>
#include <QicsTreeTable.h>
#include <QicsGroupCellDisplay.h>


QicsExpandableStaticRowData::QicsExpandableStaticRowData(QicsTreeTable *table, QicsGroupInfo *info, QObject *parent)
    : QicsSpecialRowData(table, parent),
        m_title(QString("%1 : %2").arg(info->content(), info->data()))
{

    m_groupInfo = info;
    m_open = true;
    m_level = 0;
    m_root = 0;
}

void QicsExpandableStaticRowData::init(QicsRow &row)
{
    if (m_table->treeInHeader()) {
        QicsRow &rr = m_table->rowHeaderRef().rowRef(row.rowIndex());
        rr.setDisplayer(m_table->groupDisplayer());
        rr.setLabel(m_title.string());
    }
    else
        row.setLabel(m_title.string());

    row.setDisplayer(m_table->groupDisplayer());
    row.setReadOnly(true);
}

void QicsExpandableStaticRowData::dispose(QicsRow &row)
{
    doExpand(true, true, false);

    QicsSpecialRowData::dispose(row);
}

void QicsExpandableStaticRowData::addChild(int index)
{
    m_list.append(index);

    // we will test if the added child is expandable row
    QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(m_table->specialRowData(index));
    if (erow) {
        erow->m_root = this;
        erow->setLevel(m_level+1);
    }
}

void QicsExpandableStaticRowData::handleClick(int ,int,int button,const QPoint &)
{
    if (button == Qt::LeftButton && m_table->expandPolicy() == QicsTreeTable::SingleClick)
        switchTree();
}

void QicsExpandableStaticRowData::handleDoubleClick(int /*row*/,int /*col*/,int button,const QPoint &/*p*/)
{
    if (button == Qt::LeftButton && m_table->expandPolicy() == QicsTreeTable::DoubleClick)
        switchTree();
}

bool QicsExpandableStaticRowData::handleKeyPress(int /*row*/,int /*col*/,QKeyEvent *ke)
{
    switch (ke->key())
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Space:
        switchTree();
        return true;
    case Qt::Key_Right:
        expandTree(true);
        return true;
    case Qt::Key_Left:
        expandTree(false);
        return true;
    default:
        break;
    }
    return false;
}


void QicsExpandableStaticRowData::expandTree(bool open, bool repaint)
{
    if (open == m_open) return;

    doExpand(m_open = open, true, repaint);
}

void QicsExpandableStaticRowData::expand(bool open, bool repaint)
{
    if (open == m_open) return;

    doExpand(m_open = open, repaint);
}

void QicsExpandableStaticRowData::switchTree(bool repaint)
{
    if ((m_table->treeMode() == QicsTreeTable::Flat_No_Details) && !m_open)
        return;

    m_open = !m_open;

    doExpand(m_open, true, repaint);
}

void QicsExpandableStaticRowData::doExpand(bool open, bool tree, bool repaint)
{
    if (repaint)
        m_table->setRepaintBehavior(Qics::RepaintOff);

    QicsDimensionManager *dm = m_table->gridInfo().dimensionManager();
    bool es = dm->emitSignals();
    dm->setEmitSignals(false);

    int row = -1;
    bool isHidden = false;

    //hide/show children
    for (QVector<int>::const_iterator it = m_list.constBegin(); it != m_list.constEnd(); ++it) {
        row = *it;
        isHidden = isRowHidden(row);

        if (!open || isHidden)
            dm->hideRow(row);
        else {
            if (tree)
                dm->showRow(row);
            else {
                if (open) {
                    if (m_open && !isHidden)
                        dm->showRow(row);
                    else
                        continue;
                }
            }
        }

        // if the rrow has children itself, process them too
        QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(m_table->specialRowData(row));
        if (erow && erow != this) {
            erow->doExpand(open, false, false);
        }
    }

    if (m_table->summaryPolicy() == QicsTreeTable::SummaryAlwaysExpanded) {
        if (!m_root || m_root->isOpen())
            if (m_table->specialRowData(m_list.last())->isVisible())
                dm->showRow(m_list.last());
    }

    dm->setEmitSignals(es);

    if (repaint) {
        m_table->setRepaintBehavior(Qics::RepaintOn);
        m_table->repaint();
    }
}

bool QicsExpandableStaticRowData::isRowHidden(int index)
{
    if (m_table->isRowFiltered(index))
        return true;

    if (m_table->isSummary(index)) {
        if (m_table->summaryPolicy() == QicsTreeTable::SummaryDisabled)
            return true;

        if (!m_table->specialRowData(index)->isVisible())
            return true;
    }

    return false;
}


