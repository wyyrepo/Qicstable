/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "QicsFilterSortDialog.h"

#include <QPushButton>
#include <QVBoxLayout>

#include <QicsTreeTable.h>


QicsFilterSortDialog::QicsFilterSortDialog(QicsTreeTable *parent)
    : QicsGroupBarFilterDialog(parent)
{
    QPushButton *pbSortA = new QPushButton(this);
    pbSortA->setText(tr("Sort Ascending"));
    pbSortA->setIcon(QIcon(QString::fromUtf8(":/Resources/sortA.png")));

    QPushButton *pbSortD = new QPushButton(this);
    pbSortD->setText(tr("Sort Descending"));
    pbSortD->setIcon(QIcon(QString::fromUtf8(":/Resources/sortD.png")));

    QPushButton *pbSortBar = new QPushButton(this);
    pbSortBar->setText(tr("Add to Sort Bar"));

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->addWidget(pbSortA);
    topLayout->addWidget(pbSortD);
    topLayout->addWidget(pbSortBar);
    topLayout->addWidget(line);

    m_layout->insertLayout(0, topLayout);

    resize(267, 139);
}

QicsFilterSortDialog::~QicsFilterSortDialog()
{
}

void QicsFilterSortDialog::on_pbSortA_clicked()
{
    QVector<int> col; col << m_group;
    m_table->sortRows(col, Qics::Ascending);
    close();
}

void QicsFilterSortDialog::on_pbSortD_clicked()
{
    QVector<int> col; col << m_group;
    m_table->sortRows(col, Qics::Descending);
    close();
}

void QicsFilterSortDialog::on_pbSortBar_clicked()
{
    m_table->sortColumn(m_group);
    close();
}


