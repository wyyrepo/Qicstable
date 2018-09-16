/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "widgetsTable.h"

#include <QPushButton>

#include <QicsWidgetCellDisplay.h>
#include "QicsPushButtonHeaderDisplay.h"


WidgetsTable::WidgetsTable(QicsDataModel* dm, QWidget *parent)
    : QicsTable(dm, parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Widgets Table Demo"));

    indexRow = 9;
    indexColumn = 9;
    setVScrollBarMode(Qics::AlwaysOn);
    setHScrollBarMode(Qics::Auto);
    setVisibleRows(dataModel()->numRows()+5);
    setVisibleColumns(dataModel()->numColumns()+1);

    // creating the push button header displayer, that has more improved look
    // than standard headers.
    rowHeaderRef().setDisplayer(new QicsPushButtonHeaderDisplay(this));
    columnHeaderRef().setDisplayer(new QicsPushButtonHeaderDisplay(this));

    btnHideColumn = new QPushButton("Hide column");
    columnHeaderRef().cellRef(0, 0).setDisplayer(new QicsWidgetCellDisplay(btnHideColumn, this));
    columnHeaderRef().cellRef(0, 0).setMaxWidthInPixels(200);
    columnHeaderRef().cellRef(0, 0).setMinWidthInPixels(100);

    btnHideRow = new QPushButton(tr("Hide\n row"));
    rowHeaderRef().cellRef(0, 0).setDisplayer(new QicsWidgetCellDisplay(btnHideRow, this));
    rowHeaderRef().cellRef(0, 0).setMaxHeightInPixels(100);
    rowHeaderRef().cellRef(0, 0).setMinHeightInPixels(50);

    btnHideRowColumn = new QPushButton(tr("Hide row\nand column"));
    cellRef(0, 0).setDisplayer(new QicsWidgetCellDisplay(btnHideRowColumn, this));

    connect(btnHideColumn, SIGNAL(clicked()), SLOT(clickColBtn()));
    connect(btnHideRow, SIGNAL(clicked()), SLOT(clickRowBtn()));
    connect(btnHideRowColumn, SIGNAL(clicked()), SLOT(clickRowBtn()));
    connect(btnHideRowColumn, SIGNAL(clicked()), SLOT(clickColBtn()));
}

WidgetsTable::~WidgetsTable()
{
}

void WidgetsTable::clickRowBtn()
{
    rowRef(indexRow--).hide();
}

void WidgetsTable::clickColBtn()
{
    columnRef(indexColumn--).hide();
}


