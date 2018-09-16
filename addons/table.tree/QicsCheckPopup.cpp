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

#include <QicsCheckPopup.h>

#include <QListWidget>
#include <QCheckBox>
#include <QVBoxLayout>


QicsCheckPopup::QicsCheckPopup(bool moveItems, bool sortItems, QWidget *parent)
    : QWidget(parent), m_moveItems(moveItems)
{
    m_list = new QListWidget(this);
    m_cbAll = new QCheckBox(tr("All"), this);
    m_cbAll->setTristate(true);

    QVBoxLayout *vbl = new QVBoxLayout();
    setLayout(vbl);
    QHBoxLayout *hbl = new QHBoxLayout();

    hbl->addSpacing(4);
    hbl->addWidget(m_cbAll);

    vbl->addLayout(hbl);
    vbl->addWidget(m_list);

    adjustSize();

    if (m_moveItems) {
        m_list->setDragEnabled(true);
        m_list->setDragDropMode(QAbstractItemView::InternalMove);
    }

    if (sortItems)
        m_list->setSortingEnabled(true);

    connect(m_cbAll, SIGNAL(clicked(bool)), this, SLOT(checkItems(bool)));
    connect(m_list, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(updateCheckBox(QListWidgetItem*)));
}

QicsCheckPopup::~QicsCheckPopup()
{
}

void QicsCheckPopup::checkItems(bool state)
{
    Qt::CheckState st = state ? Qt::Checked : Qt::Unchecked;

    m_list->blockSignals(true);
    m_cbAll->blockSignals(true);

    // we need only two states here
    m_cbAll->setCheckState(st);

    for (int i = 0; i < m_list->count(); ++i)
        m_list->item(i)->setCheckState(st);

    m_cbAll->blockSignals(false);
    m_list->blockSignals(false);
}

void QicsCheckPopup::updateCheckBox(QListWidgetItem *)
{
    int checked = 0;
    for (int i = 0; i < m_list->count(); ++i)
        if (m_list->item(i)->checkState() == Qt::Checked)
            checked++;

    m_cbAll->setCheckState(!checked ? Qt::Unchecked : checked == m_list->count() ? Qt::Checked : Qt::PartiallyChecked );
}


