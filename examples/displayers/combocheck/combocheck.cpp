/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "combocheck.h"

#include <QicsTable.h>
#include <QicsComboCheckCellDisplay.h>

// checked combo box rows
#define COMBOCHECK_CELL_ROW1  4
#define COMBOCHECK_CELL_ROW2  7
#define COMBOCHECK_CELL_ROW3  10

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Combo checkboxes displayer example"));

    // dimensions of the data model
    int numRows = 13;
    int numCols = 5;

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

    // show text
    m_table->cellRef(1, 1).setLabel("Check combo items to change cell value");
    m_table->cellRef(2, 1).setLabel("Change cell's strings to change checked items");

    // list data for the combo box items
    QStringList m_colors;
    m_colors << "Red" << "Orange" << "Yellow" << "Green" << "Blue" << "Cyan" << "Magenta";

    // create combo checkboxes displayer
    QicsComboCheckCellDisplay *ccb = new QicsComboCheckCellDisplay(m_table);
    ccb->addItems(m_colors);
    ccb->setAddValueToList(false);
    ccb->setEditable(true);
    // default text is displayed when no items are checked
    ccb->setDefaultText("Colors...");

    // create cell span for the combo boxes
    QicsSpan span_c1(COMBOCHECK_CELL_ROW1, 1, 1, 3);
    QicsSpan span_c2(COMBOCHECK_CELL_ROW2, 1, 1, 1);
    QicsSpan span_c3(COMBOCHECK_CELL_ROW3, 1, 1, 2);
    m_table->addCellSpan(span_c1);
    m_table->addCellSpan(span_c2);
    m_table->addCellSpan(span_c3);

    // set displayer objects
    m_table->cellRef(COMBOCHECK_CELL_ROW1, 1).setDisplayer(ccb);
    m_table->cellRef(COMBOCHECK_CELL_ROW2, 1).setDisplayer(ccb);
    m_table->cellRef(COMBOCHECK_CELL_ROW3, 1).setDisplayer(ccb);

    // change font for these rows to bold
    QFont font = m_table->rowRef(1).font();
    font.setBold(true);
    m_table->rowRef(COMBOCHECK_CELL_ROW1 + 1).setFont(font);
    m_table->rowRef(COMBOCHECK_CELL_ROW2 + 1).setFont(font);
    m_table->rowRef(COMBOCHECK_CELL_ROW3 + 1).setFont(font);

    // make sure the table is only as large as necessary
    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);
}

void MainWindow::on_cellValueChanged(int row, int col)
{
    // we need cells with combo displayers or "indicator cells"
    if (col != 1) return;
    // index to show which cell should be changed
    int index;
    switch (row) {
        // combobox cells changed
        case COMBOCHECK_CELL_ROW1:
        case COMBOCHECK_CELL_ROW2:
        case COMBOCHECK_CELL_ROW3: index = 1; break; // we need to change next row after combo's row
        // string "indicator cells" changed
        case COMBOCHECK_CELL_ROW1 + 1:
        case COMBOCHECK_CELL_ROW2 + 1:
        case COMBOCHECK_CELL_ROW3 + 1: index = -1; break; // we need to change previous row
        default: return;
    }
    // get current value for the changed cell
    QString value = m_table->cellRef(row, col).dataValue()->string();

    // block signals to avoid repeating calls of valueChanged() signal
    bool blocked = m_table->blockSignals(true);
    // set new value for the cell
    m_table->cellRef(row + index, col).setDataValue(QicsDataString(value));
    m_table->blockSignals(blocked);
}


