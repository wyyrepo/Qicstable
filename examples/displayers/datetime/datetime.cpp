/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "datetime.h"

#include <QicsTable.h>
#include <QicsDateTimeCellDisplay.h>

// datetime displayer row
#define DATETIME_CELL_ROW 4

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("DateTime displayer example"));

    // dimensions of the data model
    int numRows = 8;
    int numCols = 3;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // create the table, using the data model we created above
    m_table = new QicsTable(dm, this);
    setCentralWidget(m_table);

    // connect valueChanged() signal to process cell's values changes
    connect(m_table, SIGNAL(valueChanged(int, int)), this, SLOT(on_cellValueChanged(int, int)));

    // set table properties
    m_table->setCellOverflowBehavior(Qics::Overflow);
    m_table->rowHeaderRef().setDefaultColumnWidthInPixels(30);
    m_table->columnHeaderRef().setDefaultColumnWidthInPixels(150);

    // show text
    m_table->cellRef(1, 0).setLabel("Change date and time to change cell's value below");
    m_table->cellRef(2, 0).setLabel("Change cell's strings to change date and time");

    // create datetime displayer
    QicsDateTimeCellDisplay *dt = new QicsDateTimeCellDisplay(m_table);
    // set initial value for the cells and displayer
    for (int i = 0; i < 3; i++) {
        m_table->cellRef(DATETIME_CELL_ROW,i).setDataValue(QicsDataDateTime(QDateTime::currentDateTime()));
        m_table->cellRef(DATETIME_CELL_ROW,i).setDisplayer(dt);
    }

    // change font for this row to bold
    QFont font = m_table->rowRef(1).font();
    font.setBold(true);
    m_table->rowRef(DATETIME_CELL_ROW + 2).setFont(font);

    // make sure the table is only as large as necessary
    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);
}

void MainWindow::on_cellValueChanged(int row, int col)
{
    // to avoid endless loop we have to block signals here
    bool blocked = m_table->blockSignals(true);
    // if the datetime editor cell value changed
    if (row == DATETIME_CELL_ROW && (col >= 0 && col <=2)) {
        // get datetime string value and set it to another cell
        QString value = m_table->cellRef(row, col).dataValue()->string();
        m_table->cellRef(row + 2, col).setDataValue(QicsDataString(value));
    }
    // if the datetime string cell value changed
    else if (row == DATETIME_CELL_ROW + 2 && (col >= 0 && col <=2)) {
        // get datetime string value
        QString value = m_table->cellRef(row, col).dataValue()->string();
        // set datetime displayer value
        m_table->cellRef(DATETIME_CELL_ROW, col).setDataValue(QicsDataDateTime(QDateTime::fromString(value)));
    }
    m_table->blockSignals(blocked);
}


