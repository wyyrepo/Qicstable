/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "QicsFilterGroupWidget.h"

#include <QApplication>
#include <QTreeWidget>

#include "QicsTreeTable.h"
#include "QicsGroupBar.h"


class QicsTree : public QTreeWidget
{
public:
    QicsTree (QicsTreeTable *table,  QWidget * parent = 0 ) :
      QTreeWidget(parent), m_table(table)
      {
          m_filterDialog = new QicsGroupBarFilterDialog(table);
      }

protected:
    virtual void dropEvent ( QDropEvent * event )
    {
        event->accept();

        const QPoint &pos = event->pos();
        QTreeWidgetItem *item = itemAt(pos);
        QTreeWidgetItem *c_item = currentItem();

        if (item != c_item) {
            QModelIndex idx = indexAt(pos);
            takeTopLevelItem(indexOfTopLevelItem(c_item));
            if (item && idx.isValid())
                insertTopLevelItem(idx.row(), c_item);
            else
                addTopLevelItem(c_item);

            updateGrouping();
        }
    }

    virtual void mouseDoubleClickEvent ( QMouseEvent * event )
    {
        if (event->button() != Qt::LeftButton) return;

        event->accept();

        const QPoint &pos = event->pos();
        QTreeWidgetItem *item = itemAt(pos);

        if (item) {
            QModelIndex idx = indexAt(pos);
            int id = item->data(0,1000).toInt();

            switch (idx.column())
            {
            case 1:
                takeTopLevelItem(idx.row());
                m_table->ungroupColumn(id);
                break;
            case 0:
                m_filterDialog->show(event->globalPos(), id);
                while (m_filterDialog->isVisible())
                    QApplication::processEvents();

                if (m_table->hasRowFilter(id))
                    item->setIcon(0, QIcon(":/Resources/filter.png"));
                else
                    item->setIcon(0, QIcon(":/Resources/no_filter.png"));
            default:
                break;
            }
        }
    }

    void updateGrouping()
    {
        QList<int> groups;
        QTreeWidgetItemIterator it(this);

        while (*it) {
            groups.append((*it)->data(0,1000).toInt());
            ++it;
        }
        m_table->groupColumns(groups);
    }

private:
    QicsTreeTable *m_table;
    QicsGroupBarFilterDialog *m_filterDialog;
};



QicsFilterGroupWidget::QicsFilterGroupWidget(QicsTreeTable *table, QWidget *parent)
    : QicsPopupDialog(parent, QDialogButtonBox::Close), m_table(table)
{
    m_list = new QicsTree(m_table, this);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_list->setHeaderLabels(QStringList() << "" << "" << "Group");
#if QT_VERSION < 0x050000
    m_list->header()->setResizeMode(QHeaderView::ResizeToContents);
#else
    m_list->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif

    m_list->setDragEnabled(true);
    m_list->setDragDropMode(QAbstractItemView::InternalMove);

    setWidget(m_list);

    updateView();
}

QicsFilterGroupWidget::~QicsFilterGroupWidget()
{
}

void QicsFilterGroupWidget::updateView()
{
    m_list->clear();

    const QList<int> &groups = m_table->groups();
    int size = groups.size();

    for (int i = 0; i < size; ++i) {
        int id = groups.at(i);

        QTreeWidgetItem *it = new QTreeWidgetItem(m_list);
        it->setText(2, m_table->groupText(id));

        it->setData(0,1000,id);
        if (m_table->hasRowFilter(id))
            it->setIcon(0, QIcon(":/Resources/filter.png"));
        else
            it->setIcon(0, QIcon(":/Resources/no_filter.png"));

        it->setIcon(1, QIcon(":/Resources/close.png"));
    }
}

void QicsFilterGroupWidget::show(const QPoint &pos)
{
    updateView();

    QicsPopupDialog::show(pos);
}


