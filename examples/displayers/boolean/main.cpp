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
#include <QLabel>

#include <QicsTable.h>
#include <QicsDataModelDefault.h>
#include "QicsBoolCellDisplay.h"


int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    // dimensions of the data model
    const int numRows = 10;
    const int numCols = 5;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // populate the data model with some data
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if ( !j  )
                dm->setItem(i, j, QicsDataString( QString("Row %1").arg(i) ) );
            else
                dm->setItem(i, j, QicsDataBool( i % 2 ) );
        }
    }

    // create the table, using the data model we created above
    QicsTable *table = new QicsTable(dm, 0);
    table->setAttribute(Qt::WA_DeleteOnClose);
    table->setWindowTitle(QObject::tr("QicsBoolCellDisplay example"));
    table->setDefaultRowHeightInPixels( 40 );
    table->columnHeader()->rowRef(0).setHeightInPixels(40);
    table->columnRef(2).setAlignment(Qt::AlignLeft);
    table->columnRef(3).setAlignment(Qt::AlignHCenter);
    table->columnRef(4).setAlignment(Qt::AlignRight);

    // make sure the table is only as large as necessary
    table->setVisibleRows(numRows);
    table->setVisibleColumns(numCols);

    QicsBoolCellDisplay boolDisplayer_str;
    boolDisplayer_str.setTrueDisplayString("high - true");
    boolDisplayer_str.setFalseDisplayString("low - false");
    table->columnRef(1).setDisplayer(&boolDisplayer_str);

    QPixmap highPximap(":/Resources/high.xpm");
    QPixmap lowPximap(":/Resources/low.xpm");

    QicsBoolCellDisplay boolDisplayer_px;
    boolDisplayer_px.setTrueDisplayPixmap(highPximap);
    boolDisplayer_px.setFalseDisplayPixmap(lowPximap);
    table->columnRef(2).setDisplayer(&boolDisplayer_px);

    QicsBoolCellDisplay boolDisplayer_px1;
    boolDisplayer_px1.setTrueDisplayPixmap(highPximap);
    boolDisplayer_px1.setFalseDisplayPixmap(lowPximap);
    boolDisplayer_px1.setTrueDisplayString("high - true");
    boolDisplayer_px1.setFalseDisplayString("low - false");
    boolDisplayer_px1.setPercentageOfHeight(75);
    table->columnRef(3).setDisplayer(&boolDisplayer_px1);

    QicsBoolCellDisplay boolDisplayer_px2;
    boolDisplayer_px2.setTrueDisplayPixmap(highPximap);
    boolDisplayer_px2.setFalseDisplayPixmap(lowPximap);
    boolDisplayer_px2.setTrueDisplayString("high");
    boolDisplayer_px2.setFalseDisplayString("low");
    boolDisplayer_px2.setPercentageOfHeight(50);
    table->columnRef(4).setDisplayer(&boolDisplayer_px2);

    QLabel *label = new QLabel(QObject::tr("Hello World, QicsBoolCellDisplay"), table);
    label->setAlignment(Qt::AlignCenter);
    table->setTopTitleWidget(label);

    app.setActiveWindow(table);
    table->resize(700, 480);
    table->show();

    return app.exec();
}


