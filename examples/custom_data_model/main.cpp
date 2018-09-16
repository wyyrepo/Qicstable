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

#include <QicsTable.h>
#include "custom_data_model.h"


int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    // create the data model
    StockDataModel *dm = new StockDataModel();

    // fill the data model with some values
    dm->insertStock(-1);
    dm->setSymbol(0, "ATT");
    dm->setClose(0, 37.73);
    dm->setHigh(0, 38.0);
    dm->setLow(0, 37.55);
    dm->setVolume(0, 503333);

    dm->insertStock(-1);
    dm->setSymbol(1, "RJR");
    dm->setClose(1, 67.05);
    dm->setHigh(1, 67.05);
    dm->setLow(1, 64.89);
    dm->setVolume(1, 997323);

    // create the table, using the data model we created above
    QicsTable *table = new QicsTable(dm, 0);
    table->setWindowTitle(QObject::tr("Custom Data Model Example"));

    // make sure the table is only as large as necessary
    table->setVisibleRows(2);
    table->setVisibleColumns(5);

    app.setActiveWindow(table);
    table->show();

    return app.exec();
}


