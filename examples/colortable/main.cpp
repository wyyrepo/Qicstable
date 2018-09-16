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
#include <QPushButton>

#include <QicsTable.h>
#include <QicsDataModelDefault.h>
#include <QicsDateTimeCellDisplay.h>
#include <QicsComboCellDisplay.h>
#include <QicsCheckCellDisplay.h>
#include <QicsTextCellDisplay.h>
#include <QicsColumn.h>
#include <QicsRowHeader.h>
#include <QicsColumnHeader.h>
#include <QicsMainGrid.h>
#include <QicsRegionalAttributeController.h>

const QString colors[]=
{
    "red",
    "orange",
    "yellow",
    "green",
    "blue",
    "cyan",
    "magenta"
};


class CustomDisplay : public QicsTextCellDisplay
{
public:
    CustomDisplay(QWidget *parent = 0) : QicsTextCellDisplay(parent) {}

    inline bool editWhenCurrent() const {return true;}
};


int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    // dimensions of the data model
    int numRows = 10;
    int numCols = 7;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // populate the data model with some data
    int i, j;
    for (i = 0; i < numRows; ++i)
        for (j = 0; j < numCols; ++j)
            dm->setItem(i, j, QicsDataInt(i*j));

    // create the table, using the data model we created above
    QicsTable *table = new QicsTable(dm, 0);
    table->setWindowTitle(QObject::tr("Color Table Example"));

    QicsRegionalAttributeController controller;
    table->setExternalAttributeController(controller);

    table->setSelectionPolicy(Qics::SelectMultipleRow);
    table->setTopLeftCornerWidget(new QPushButton(QObject::tr("Push!!!"), table));

    QLabel *label = new QLabel(QObject::tr("Hello World, Color Table"), table);
    label->setAlignment(Qt::AlignCenter);
    label->setAutoFillBackground(true);
    table->setTopTitleWidget(label);

    table->columnRef(0).setDisplayer(new CustomDisplay(table));

    QicsComboCellDisplay *cb = new QicsComboCellDisplay(table);
    for (int i=0; i<10; ++i)
        cb->addItem(QString::number(i));
    table->columnRef(1).setDisplayer(cb);

    table->columnRef(4).setDisplayer(new QicsCheckCellDisplay(table));
    table->columnRef(6).setDisplayer(new QicsDateTimeCellDisplay(table));

    // Setting colors of vertical and horizontal grid lines
    table->setVerticalGridLinePen(QPen("green"));
    table->setHorizontalGridLinePen(QPen("green"));

    //Lets change a little table's palette :)
    QPalette pal = table->palette();
    pal.setColor(QPalette::Window,"green");
    pal.setColor(QPalette::WindowText,"black");
    pal.setColor(QPalette::Button,"magenta");
    pal.setColor(QPalette::ButtonText,"white");
    pal.setColor(QPalette::Text,"blue");

    // Setting colors to whole table
    table->setPalette(pal);

    // Setting colors to row and column headers
    pal.setColor(QPalette::Base,"red");
    pal.setColor(QPalette::Text,"yellow");
    pal.setColor(QPalette::Window,"brown");
    table->rowHeaderRef().setPalette(pal);

    pal.setColor(QPalette::Base,"yellow");
    pal.setColor(QPalette::Text,"red");
    pal.setColor(QPalette::Window,"purple");
    table->columnHeaderRef().setPalette(pal);

    // Setting colors to main grid
    pal.setColor(QPalette::Base,"orange");
    pal.setColor(QPalette::Window,"blue");
    pal.setColor(QPalette::Dark,"green");
    pal.setColor(QPalette::Mid,"orange");
    pal.setColor(QPalette::Light,"yellow");
    pal.setColor(QPalette::Shadow,"black");

    table->mainGridRef().setPalette(pal);

    // Setting colors to a custom column
    pal.setColor(QPalette::Base,"lightblue");
    table->columnRef(1).setPalette(pal);
    table->columnHeaderRef().columnRef(1).setPalette(pal);

    QPalette p = table->columnRef(2).palette();
    p.setColor(QPalette::HighlightedText,"black");
    table->columnRef(2).setPalette(p);

    // Setting highlight colors for columns
    for(int i = 0;i< numCols; ++i) {
        QPalette p = table->columnRef(i).palette();
        p.setColor(QPalette::Highlight,QColor(colors[i]));
        table->columnRef(i).setPalette(p);
    }

    // Setting editing colors for custom column
    table->columnRef(0).setEditBackgroundColor("black");
    table->columnRef(0).setEditForegroundColor("white");

    // make sure the table is only as large as necessary
    table->setVisibleRows(numRows);
    table->setVisibleColumns(numCols);

    app.setActiveWindow(table);

    table->show();

    return app.exec();
}


