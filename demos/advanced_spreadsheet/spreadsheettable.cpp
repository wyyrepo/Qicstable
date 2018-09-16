/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "spreadsheettable.h"

#include <QicsGridLayout.h>
#include <QicsMainGrid.h>
#include <QicsColumnHeader.h>
#include <QicsCell.h>
#include <QicsRowHeader.h>
#include <QicsColumn.h>
#include <QicsRow.h>
#include <QicsNavigator.h>
#include <QicsRegionalAttributeController.h>
#include "spreadsheetcelldisplay.h"
#include "spreadsheetdm.h"

#define DEFAULT_COLUMN_WIDTH 70
#define DEFAULT_ROW_HEIGHT 21
#define DEFAULT_ROW_HEADER_WIDTH 30


SpreadsheetTable::SpreadsheetTable(QicsTableGrid::Foundry tf,QWidget* parent)
    : QicsTable(0,0,tf,0,0,parent)
{
    mainGridRef().setDisplayer(new SpreadsheetCellDisplay(this));

    setSelectionStyle(Qics::Exclusive);
    setCellWidthMode(Qics::ChangeHeightWidth);

    rowHeaderRef().setAllowUserMove(true);
    columnHeaderRef().setAllowUserMove(true);
    setHeaderDragStyle(Qics::StyledDrag);

    setNavigatorAllowed(true);
    navigator()->setText("#");
    navigator()->setToolTip("Press and hold to navigate throungh the grid");

    QicsRegionalAttributeController controller;
    setExternalAttributeController(controller);

    setWideKeyActions();

    setDefaultImagePath("./images");

    QToolButton *bSelectAll = new QToolButton(this);
    bSelectAll->setText("*");
    bSelectAll->setToolTip("Select all cells");
    bSelectAll->setFixedSize(18,18);
    connect(bSelectAll, SIGNAL(clicked()), this, SLOT(selectAll()));
    setTopLeftCornerWidget(bSelectAll);

    // dimensions of the data model
    int numRows = 1000;
    int numCols = 1000;

    // Setting column labels
    for (int i = 0; i < numCols; ++i) {
        QString text;
        encodePosition(text, i);
        columnHeaderRef().cellRef(0,i).setLabel(text);
        m_columns[text] = i;
    }

    // Setting row labels
    for (int i = 0; i < numRows; ++i) {
        QString text = QString::number(i+1);
        rowHeaderRef().cellRef(i,0).setLabel(text);
        m_rows[text] = i;
    }

    QFont defaultFont("Arial",10);
    columnHeaderRef().setFont(defaultFont);
    rowHeaderRef().setFont(defaultFont);
#ifdef Q_OS_WIN32
    mainGridRef().setFont(defaultFont);
#endif
    rowHeaderRef().columnRef(0).setWidthInPixels(DEFAULT_ROW_HEADER_WIDTH);
    columnHeaderRef().rowRef(0).setHeightInPixels(DEFAULT_ROW_HEIGHT);
    columnHeaderRef().setDefaultColumnWidthInPixels(DEFAULT_COLUMN_WIDTH);
    rowHeaderRef().setDefaultRowHeightInPixels(DEFAULT_ROW_HEIGHT);

    // create the data model
    QicsDataModel *dm = new SpreadsheetDataModel(numRows, numCols);

    setDataModel(dm);

    mainGridRef().setVerticalGridLinePen(QPen("lightgrey"));
    mainGridRef().setHorizontalGridLinePen(QPen("lightgrey"));
    rowHeaderRef().setHorizontalGridLinePen(QPen("grey"));
    rowHeaderRef().setVerticalGridLinePen(QPen("grey"));

    columnHeaderRef().setVerticalGridLinePen(QPen("grey"));
    columnHeaderRef().setHorizontalGridLinePen(QPen("grey"));

    columnHeaderRef().setAlignment(Qt::AlignCenter);
    rowHeaderRef().setAlignment(Qt::AlignCenter);
    rowHeaderRef().setDefaultColumnWidthInChars(2);
    rowHeaderRef().setAllowHeaderResize(false);
    columnHeaderRef().setAllowHeaderResize(false);

    columnHeaderRef().setFrameStyle(QFrame::Panel);
    rowHeaderRef().setFrameStyle(QFrame::Panel);

    mainGridRef().setSelectedBackgroundColor(QColor(0x92a2b9));
    rowHeaderRef().setSelectedBackgroundColor(QColor(0x92a2d9));
    rowHeaderRef().setSelectedForegroundColor(Qt::white);
    columnHeaderRef().setSelectedBackgroundColor(QColor(0x92a2d9));
    columnHeaderRef().setSelectedForegroundColor(Qt::white);
    setExclusiveSelectionDragBackColor(QColor(Qt::transparent));

    setTableSpacing(0);
    setLayoutSpacing(0);

    // make sure the table is only as large as necessary
    setVisibleRows(20);
    setVisibleColumns(10);
    setAllowPropagateChanges(true);

    setCellOverflowBehavior(Qics::ToolTip);
    mainGridRef().setCursor(QCursor(QPixmap(":/images/cursor.png")));
    setCurrentCellStyle(Qics::Spreadsheet);

#ifdef _WITH_SMART_HEADERS
    rowHeaderRef().setHighlightSelection(true);
    rowHeaderRef().setHighlightForegroundColor(Qt::yellow);
    rowHeaderRef().setHighlightBackgroundColor(QColor(0xFFB972));
    columnHeaderRef().setHighlightSelection(true);
    columnHeaderRef().setHighlightForegroundColor(Qt::yellow);
    columnHeaderRef().setHighlightBackgroundColor(QColor(0xFFB972));
#endif

    connect(this,SIGNAL(valueChanged(int,int)),this,SLOT(updateView()));
}

void SpreadsheetTable::encodePosition(QString &text, int column)
{
    text.clear();
    QChar letter;

    letter = 'A' + column%26;
    text.insert(0, letter);

    if (column >= 26) {
        column = column/26;
        do {
            column--;
            QChar letter('A' + column%26);
            text.insert(0, letter);
            column = column/26;
        }
        while(column>0);
    }
}

SpreadsheetTable::~SpreadsheetTable()
{
    QicsDataModel *dm = dataModel();
    dm->disconnect();
    setDataModel(0);
    delete dm;
}

void SpreadsheetTable::updateView()
{
#ifdef _WITH_CALCULATIONS
    if(repaintBehavior() != Qics::RepaintOff)
        myGrids[1][1]->redraw();
#endif
}

void SpreadsheetTable::setLayoutSpacing(int sp)
{
    gridLayout()->setSpacing(sp);
    setFrameStyle(QFrame::Box);
    gridLayout()->setMargin(sp);
}

void SpreadsheetTable::insertRows()
{
    QicsCell* cur_cell = currentCell();
    QicsICell cell(cur_cell->rowIndex(),cur_cell->columnIndex());

    if(!cell.isValid())
        return;
    else {
        setRepaintBehavior(Qics::RepaintOff);

        QList<int> v = selectionList(true)->rows();
        if (v.isEmpty())
            insertRow(cell.row());
        else
            for (int i = 0; i < v.count(); ++i)
                insertRow(cell.row());

        setRepaintBehavior(Qics::RepaintOn);
    }
}

void SpreadsheetTable::insertColumns()
{
    QicsCell* cur_cell = currentCell();
    QicsICell cell(cur_cell->rowIndex(),cur_cell->columnIndex());

    if(!cell.isValid())
        return;
    else {
        setRepaintBehavior(Qics::RepaintOff);

        QList<int> v = selectionList(true)->columns();
        if (v.isEmpty())
            insertColumn(cell.column());
        else
            for (int i = 0; i < v.count(); ++i)
                insertColumn(cell.column());

        setRepaintBehavior(Qics::RepaintOn);
    }
}


void SpreadsheetTable::deleteRows()
{
    setRepaintBehavior(Qics::RepaintOff);

    QicsSelectionList* list = selectionList(true);

    QList<int> tokill = list->rows();
    qSort(tokill.begin(), tokill.end());

    QListIterator<int> iter(tokill);
    iter.toBack();

    while (iter.hasPrevious()) {
        int i = iter.previous();
        deleteRow(i);
    }

    setRepaintBehavior(Qics::RepaintOn);
}

void SpreadsheetTable::deleteColumns()
{
    setRepaintBehavior(Qics::RepaintOff);

    QicsSelectionList* list = selectionList(true);
    QList<int> toKill = list->columns();

    qSort(toKill.begin(), toKill.end());
    QListIterator<int> iter(toKill);
    iter.toBack();

    while (iter.hasPrevious()) {
        int i = iter.previous();
        deleteColumn(i);
    }

    setRepaintBehavior(Qics::RepaintOn);
}

void SpreadsheetTable::hideRows()
{
    setRepaintBehavior(Qics::RepaintOff);

    QicsSelectionList* list = selectionList(true);
    QList<int> toHide = list->rows();

    qSort(toHide.begin(), toHide.end());
    QListIterator<int> iter(toHide);
    iter.toBack();

    while (iter.hasPrevious()) {
        int i = iter.previous();
        rowRef(i).hide();
    }

    setRepaintBehavior(Qics::RepaintOn);
}

void SpreadsheetTable::hideColumns()
{
    setRepaintBehavior(Qics::RepaintOff);

    QicsSelectionList* list = selectionList(true);
    QList<int> toHide = list->columns();

    qSort(toHide.begin(), toHide.end());
    QListIterator<int> iter(toHide);
    iter.toBack();

    while (iter.hasPrevious()) {
        int i = iter.previous();
        columnRef(i).hide();
    }

    setRepaintBehavior(Qics::RepaintOn);
}

void SpreadsheetTable::unhideRows()
{
    setRepaintBehavior(Qics::RepaintOff);

    QicsSelectionList* list = selectionList(true);
    QicsRegion r = list->region();

    const int endRow = qBound(0, r.endRow(), dataModel()->lastRow());

    for(int i = r.startRow();i <= endRow; ++i)
        rowRef(i).show();

    setRepaintBehavior(Qics::RepaintOn);
}

void SpreadsheetTable::unhideColumns()
{
    setRepaintBehavior(Qics::RepaintOff);

    QicsSelectionList* list = selectionList(true);
    QicsRegion r = list->region();

    const int endColumn = qBound(0, r.endColumn(), dataModel()->lastColumn());

    for(int i = r.startColumn(); i <= endColumn; ++i)
        columnRef(i).show();

    setRepaintBehavior(Qics::RepaintOn);
}

void SpreadsheetTable::clearContents()
{
    setRepaintBehavior(Qics::RepaintOff);

    QicsSelectionList* list = selectionList(true);
    QicsRegion r = list->region();

    bool emitSignals = dataModel()->emitSignals();
    dataModel()->setEmitSignals(false);

    const int endRow = qBound(0, r.endRow(), dataModel()->lastRow());
    const int endColumn = qBound(0, r.endColumn(), dataModel()->lastColumn());

    for(int i = r.startRow();i <= endRow; ++i)
        for(int j = r.startColumn();j <= endColumn; ++j)
            cellRef(i,j).clearDataValue();

    dataModel()->setEmitSignals(emitSignals);
    setRepaintBehavior(Qics::RepaintOn);
}


