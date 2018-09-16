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
#include <QicsDataModelDefault.h>
#include "custom_data_type.h"


int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    QicsDataItem::registerType(QicsDataQPoint::typeName(),
        QicsDataQPoint::fromString,	QicsDataQPoint::decode);

    const int nrows = 10;
    const int ncols = 10;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(nrows, ncols);

    // fill the data mode with some QPoints
    for (int i = 0; i < nrows; ++i)
        for (int j = 0; j < ncols; ++j)
            dm->setItem(i, j, QicsDataQPoint(QPoint(i, j)));

    // create the table, using the data model we created above
    QicsTable *table = new QicsTable(dm, 0);
    table->setWindowTitle(QObject::tr("Custom Data Type Example"));

    app.setActiveWindow(table);
    table->show();

    return app.exec();
}


