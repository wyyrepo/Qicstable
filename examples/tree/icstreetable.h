/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef ICSTREETABLE_H
#define ICSTREETABLE_H

#include <QicsTreeTable.h>
#include <QicsSummarizer.h>

#include "ui_icstreetable.h"

class QicsRow;
class QicsGroupInfo;

class CustomSummarizer : public QicsSummarizer
{
public:
    virtual void onGrouping(QicsGroupInfo *gi);
    virtual void onCellValueChanged(int row, int col, QicsGroupInfo *gi);
};


class IcsTreeTable : public QDialog, private Ui::IcsTreeTableClass
{
    Q_OBJECT
public:
    IcsTreeTable(QWidget *parent = 0);
    ~IcsTreeTable();

private slots:
    void addColumns();
    void removeColumns();
    void addRows();
    void removeRows();
    void on_pbAbout_clicked();

private:
    QicsTreeTable *m_treeTable;
    QicsTable *m_table;
};

#endif //ICSTREETABLE_H


