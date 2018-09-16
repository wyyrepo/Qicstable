/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <QApplication>

#include <QicsDataModelDefault.h>

#include "doubleTable.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QicsDataModelDefault* dataModel = new QicsDataModelDefault(COUNT_ROWS, COUNT_COLS);
    QString strCaption1(QObject::tr("Double Table Demo - Table 1"));
    QString strCaption2(QObject::tr("Double Table Demo - Table 2"));
    DoubleTable* table1 = new DoubleTable(0, dataModel, strCaption1);
    DoubleTable* table2 = new DoubleTable(0, dataModel, strCaption2);

    Tester tester(table1, table2, dataModel);
    table1->show();
    table2->show();
    return app.exec();
}


