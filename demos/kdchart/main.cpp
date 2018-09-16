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
#include <QMessageBox>

#include <QicsTable.h>
#include <QicsDataModelDefault.h>
#include <QicsComboCellDisplay.h>

#ifdef BIND_KDCHART

#include "QicsKDChartTableObject.h"
#include <KDChartChart>
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartThreeDBarAttributes>
#include <KDChartAttributesModel>
#include <KDChartLegend>

using namespace KDChart;

#endif


int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    // dimensions of the data model
    const int numRows = 40;
    const int numCols = 8;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // create the table, using the data model we created above
    QicsTable *table = new QicsTable(dm, 0);
    table->setWindowTitle(QObject::tr("KDChart Integration Demo"));

    // make sure the table is only as large as necessary
    table->setVisibleRows(numRows);
    table->setVisibleColumns(numCols);

    // Add a title widget to the top of the table
    QLabel *label = new QLabel(QObject::tr("KDChart Integration Demo"), table);
    label->setAlignment(Qt::AlignCenter);
    table->setTopTitleWidget(label);

    // populate the data model with data which will be charted
    for (int i = 1; i <= 10; ++i)
        for (int j = 1; j <= 6; ++j)
            dm->setItem(i, j, QicsDataInt(i*j));

    // set attributes
    table->cellRegionRef(1,1,10,1).setBackgroundColor(Qt::yellow);
    table->cellRegionRef(1,2,10,2).setBackgroundColor(Qt::cyan);
    table->cellRegionRef(1,3,10,3).setBackgroundColor(Qt::magenta);
    table->cellRegionRef(1,4,10,4).setBackgroundColor(Qt::green);
    table->cellRegionRef(1,5,10,5).setBackgroundColor(Qt::lightGray);
    table->cellRegionRef(1,6,10,6).setForegroundColor(Qt::blue);

#ifdef BIND_KDCHART
    // insert bar diagram
    BarDiagram *bars = new BarDiagram;

    QicsKDChartTableObject *kdview1 = new QicsKDChartTableObject(table,
        bars, QicsRegion(13,1,23,6), QicsRegion(1,1,10,6));

    bars->setType(BarDiagram::Normal);

    Legend *legend1 = new Legend(bars, table);
    kdview1->chart()->addLegend(legend1);
    legend1->setPosition(KDChartEnums::PositionEast);

    //QicsComboCellDisplay *cdRows1 = new QicsComboCellDisplay(table);
    //cdRows1->setAddValueToList(false);
    //cdRows1->addItem("Data in Rows");
    //cdRows1->addItem("Data in Columns");

    //table->cellRef(12,6).setDisplayer(cdRows1);
    //table->cellRef(12,6).setDataValue(QicsDataInt(1));

    // insert line diagram
    LineDiagram *lines = new LineDiagram;

    QicsKDChartTableObject *kdview2 = new QicsKDChartTableObject(table,
        lines, QicsRegion(26,1,36,6), QicsRegion(1,1,10,6));

    kdview2->setColorsReverted(true);

    lines->setType(LineDiagram::Normal);

    Legend *legend2 = new Legend(lines, table);
    kdview2->chart()->addLegend(legend2);
    legend2->setPosition(KDChartEnums::PositionEast);
#else
    // Warn user that there is no KDChart support compiled in
    qWarning("Sorry, no KDChart support compiled in");
    QMessageBox::information(table, QObject::tr("KDChart Integration Demo"), QObject::tr("Sorry, no KDChart support compiled in"));
    return 0;
#endif

    // show
    app.setActiveWindow(table);
    table->show();

    return app.exec();
}


