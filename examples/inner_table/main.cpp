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
#include <QicsWidgetCellDisplay.h>


int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    // dimensions of the data model
    const int numRows = 50;
    const int numCols = 10;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // create the table, using the data model we created above
    QicsTable *table = new QicsTable(dm, 0);
    table->setWindowTitle(QObject::tr("Inner Table Example"));
    // make sure the table is only as large as necessary
    table->setVisibleRows(numRows);
    table->setVisibleColumns(numCols);

    // Add a title widget to the top of the table
    QLabel *label = new QLabel(QObject::tr("This example shows how to incorporate several QicsTables into one"), table);
    label->setAlignment(Qt::AlignCenter);
    table->setTopTitleWidget(label);

    // some labels
    table->addCellSpan(QicsSpan(2, 7, 10, 2));
    table->cellRef(2,7).setForegroundColor(Qt::white);
    table->cellRef(2,7).setBackgroundColor(Qt::gray);
    table->cellRef(2,7).setReadOnly(true);
    table->cellRef(2,7).setLabel(QObject::tr(
        "There are several tables\n"
        "with different selection modes,\n"
        "active headers, labels,\n"
        "colors and dimensions.\n"
        "\n"
        "Using QicsWidgetCellDisplay,\n"
        "it is easy to nest\n"
        "tables inside each other.\n"
        "The level of nested tables\n"
        "is unlimited.\n"
        "\n"
        "Each table's attributes can be\n"
        "configured individually.\n"));

    // create inner table 1
    QicsDataModel *dataModel1 = new QicsDataModelDefault(20, 8);
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 8; ++j)
            dataModel1->setItem(i, j, QicsDataInt(i*j));

    QicsTable *table1 = new QicsTable(dataModel1, table);
    // integrate it into displayer
    QicsWidgetCellDisplay *widgetDisplay1 = new QicsWidgetCellDisplay(table1);
    // and put displayer into span
    table->addCellSpan(QicsSpan(2, 1, 10, 5));
    table->cellRef(2,1).setDisplayer(widgetDisplay1);
    // label the table
    QLabel *label1 = new QLabel(QObject::tr("Integrated QicsTable 1 with data model 1"), table1);
    label1->setAlignment(Qt::AlignCenter);
    table1->setTopTitleWidget(label1);
    table1->setSelectionPolicy(Qics::SelectMultipleRow);
    table->cellRef(2,1).setBackgroundColor(Qt::yellow);

    // create inner table 2
    QicsDataModel *dataModel2 = new QicsDataModelDefault(20, 20);
    for (int i = 0; i < 20; i+=2)
        for (int j = 0; j < 20; ++j)
            dataModel2->setItem(i, j, QicsDataString(QObject::tr("Cell value %1").arg(i+j)));

    QicsTable *table2 = new QicsTable(dataModel2, table);
    // integrate it into displayer
    QicsWidgetCellDisplay *widgetDisplay2 = new QicsWidgetCellDisplay(table2);
    // and put displayer into span
    table->addCellSpan(QicsSpan(14, 1, 10, 6));
    table->cellRef(14,1).setDisplayer(widgetDisplay2);
    // label the table
    QLabel *label2 = new QLabel(QObject::tr("Integrated QicsTable 2 with data model 2"), table2);
    label2->setAlignment(Qt::AlignCenter);
    table2->setTopTitleWidget(label2);
    table2->setLeftHeaderVisible(false);
    table2->setSelectionPolicy(Qics::SelectSingle);
    table->cellRef(14,1).setBackgroundColor(Qt::green);

    // create inner table 3
    //QicsDataModel *dataModel2 = new QicsDataModelDefault(10, 10);
    QicsTable *table3 = new QicsTable(dataModel2, table);
    // integrate it into displayer
    QicsWidgetCellDisplay *widgetDisplay3 = new QicsWidgetCellDisplay(table3);
    // and put displayer into span
    table->addCellSpan(QicsSpan(26, 1, 22, 8));
    table->cellRef(26,1).setDisplayer(widgetDisplay3);
    // label the table
    QLabel *label3 = new QLabel(QObject::tr("Integrated QicsTable 3 with data model 2"), table3);
    label3->setAlignment(Qt::AlignCenter);
    table3->setTopTitleWidget(label3);
    //table3->setLeftHeaderVisible(false);
    table3->setRightHeaderVisible(true);
    table3->setBottomHeaderVisible(true);
    table->cellRef(26,1).setBackgroundColor(Qt::cyan);

    // create inner table 4 inside table 3
    //QicsDataModel *dataModel2 = new QicsDataModelDefault(10, 10);
    QicsTable *table4 = new QicsTable(dataModel1, table3);
    // integrate it into displayer
    QicsWidgetCellDisplay *widgetDisplay4 = new QicsWidgetCellDisplay(table4);
    // and put displayer into span
    table3->addCellSpan(QicsSpan(4, 0, 10, 5));
    table3->cellRef(4,0).setDisplayer(widgetDisplay4);
    // label the table
    QLabel *label4 = new QLabel(QObject::tr("Integrated QicsTable 4 into QicsTable 3 with data model 1"), table4);
    label4->setAlignment(Qt::AlignCenter);
    table4->setTopTitleWidget(label4);
    table3->cellRef(4,0).setBackgroundColor(Qt::magenta);

    app.setActiveWindow(table);
    table->resize(1024,768);
    table->show();

    return app.exec();
}


