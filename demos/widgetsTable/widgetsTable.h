/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef WIDGETSTABLE_H
#define WIDGETSTABLE_H

#include <QicsTable.h>

class QPushButton;

class WidgetsTable : public QicsTable
{
    Q_OBJECT
public:
    WidgetsTable(QicsDataModel* dm, QWidget *parent = 0);
    ~WidgetsTable();

protected slots:
    void clickRowBtn();
    void clickColBtn();

private:
    int indexRow;
    int indexColumn;
    QPushButton *btnHideColumn;
    QPushButton *btnHideRow;
    QPushButton *btnHideRowColumn;
};

#endif //WIDGETSTABLE_H


