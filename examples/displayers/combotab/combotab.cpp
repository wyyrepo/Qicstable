/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "combotab.h"

#include <QicsTable.h>
#include <QicsComboTabCellDisplay.h>
#include <QTableWidget>

// combo box row
#define COMBO_CELL_ROW     3
// tabbed combo box row
#define COMBOTAB_CELL_ROW  5

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Tabbed comboboxes displayer example"));

    // dimensions of the data model
    int numRows = 9;
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
    m_table->cellRef(1, 1).setLabel("Select tabbed combo items to change cell value");

    // combo to select active column in tabbed combobox
    m_table->cellRef(COMBO_CELL_ROW, 1).setLabel("Tabbed combo active column");
    // create combo box displayer
    QicsComboCellDisplay *cb = new QicsComboCellDisplay(m_table);

    // add items to combo box
    lColumns << "Id" << "Value";
    cb->addItems(lColumns);
    // set combo box properties
    cb->setAddValueToList(false);
    cb->setEditable(false);
    // set displayer and initial value
    m_table->cellRef(COMBO_CELL_ROW, 3).setDisplayer(cb);
    m_table->cellRef(COMBO_CELL_ROW, 3).setDataValue(QicsDataString("Id"));

    // list data for the tabbed combo box
    QStringList m_colors;
    m_colors << "Red" << "Orange" << "Yellow" << "Green" << "Blue" << "Cyan" << "Magenta";

    // create tabbed combobox display object
    QicsComboTabCellDisplay *tcb = new QicsComboTabCellDisplay(m_table);
    // set tabbed combo dimensions (rows, columns)
    tcb->setTabSize(m_colors.size(),2);
    // items from active column will be displayed in the combo box's edit
    tcb->setActiveColumn(0);
    // set header labels
    tcb->tabWidget()->setHorizontalHeaderLabels(lColumns);
    // set items for the first column
    for (int i = 0; i<m_colors.size(); ++i)
        tcb->setTabData(i, 0, QString("%1").arg(i+1));
    // set items for the whole second column
    tcb->addColumnData(1, m_colors);
    tcb->setAddValueToList(false);
    // set tabbed combo box displayer
    for (int i = 1; i < 4; i++)
        m_table->cellRef(COMBOTAB_CELL_ROW, i).setDisplayer(tcb);

    // change font for this row to bold
    QFont font = m_table->rowRef(COMBOTAB_CELL_ROW + 2).font();
    font.setBold(true);
    m_table->rowRef(COMBOTAB_CELL_ROW + 2).setFont(font);

    // make sure the table is only as large as necessary
    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);
}

void MainWindow::on_cellValueChanged(int row, int col)
{
    // "active column selection" combo
    if (row == COMBO_CELL_ROW && col == 3) {
        // get combo current value
        QString value = m_table->cellRef(row, col).dataValue()->string();
        int index = lColumns.indexOf(value);
        // get tabbed combo displayer
        QicsComboTabCellDisplay *tcb = static_cast<QicsComboTabCellDisplay *>(m_table->cellRef(COMBOTAB_CELL_ROW, col).displayer());
        // change active column for the displayer
        tcb->setActiveColumn(index);
    }

    // to avoid endless loop we have to block signals here
    bool blocked = m_table->blockSignals(true);
    // tabbed combo box displayers
    if (row == COMBOTAB_CELL_ROW && (col >= 1 && col <=3)) {
        // get string value for the current cell
        QString value = m_table->cellRef(row, col).dataValue()->string();
        // set new value to the "indicator cell"
        m_table->cellRef(row + 2, col).setDataValue(QicsDataString(value));
    }
    m_table->blockSignals(blocked);
}


