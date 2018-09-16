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

#include "dimensions.h"


int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    // dimensions of the data model
    const int numRows = 10;
    const int numCols = 10;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // populate the data model with some data
    for (int i = 0; i < numRows; ++i)
        for (int j = 0; j < numCols; ++j)
            dm->setItem(i, j, QicsDataInt(i*j));

    DimensionsTable *table = new DimensionsTable(dm);
    table->setVisibleRows(numRows);
    table->setVisibleColumns(numCols);
    table->show();

    return app.exec();
}


