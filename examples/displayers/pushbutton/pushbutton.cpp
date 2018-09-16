/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "pushbutton.h"

#include <QicsTable.h>
#include <QicsPushButtonCellDisplay.h>

// buttons first row
#define BUTTON_CELL_ROW1 3
// buttons second row
#define BUTTON_CELL_ROW2 7

// colors values definition
enum ColorIndexes {COL_RED = 0, COL_ORANGE = 1, COL_YELLOW = 2, COL_GREEN = 3,
                   COL_BLUE = 4, COL_CYAN = 5, COL_MAGENTA = 6};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Pushbuttons displayer example"));

    // dimensions of the data model
    int numRows = 12;
    int numCols = 7;

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

    // change font for this row to bold
    QFont font = m_table->rowRef(1).font();
    font.setBold(true);
    m_table->rowRef(1).setFont(font);
    m_table->cellRef(1,1).setLabel("Click pushbuttons to change cell's colors below");

    // list data for the buttons text
    m_colors << "Red" << "Orange" << "Yellow" << "Green" << "Blue" << "Cyan" << "Magenta";

    // create first group of pushbutton displayers
    QicsPushButtonCellDisplay *pb1 = new QicsPushButtonCellDisplay(m_table);
    for (int i = 0; i < m_colors.count(); i++) {
        // set displayers and labels for the cells
        m_table->cellRef(BUTTON_CELL_ROW1, i).setLabel(m_colors.at(i));
        m_table->cellRef(BUTTON_CELL_ROW1, i).setDisplayer(pb1);
    }

    // create second group of pushbutton displayers
    QicsPushButtonCellDisplay *pb2 = new QicsPushButtonCellDisplay(m_table);
    // make these buttons checkable
    pb2->setCheckable(true);
    for (int i = 0; i < m_colors.count(); i++) {
        // set displayers and labels for the cells
        m_table->cellRef(BUTTON_CELL_ROW2, i).setLabel(m_colors.at(i));
        m_table->cellRef(BUTTON_CELL_ROW2, i).setDisplayer(pb2);
    }

    // set button's rows dimension to appropriate value
    m_table->rowRef(BUTTON_CELL_ROW1).setHeightInPixels(24);
    m_table->rowRef(BUTTON_CELL_ROW2).setHeightInPixels(24);

    // make sure the table is only as large as necessary
    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);
}

void MainWindow::on_cellValueChanged(int row, int col)
{
    // process cells changes for the buttons displayers only
    if (row == BUTTON_CELL_ROW1 || row == BUTTON_CELL_ROW2) {
        QRgb colorvalue;
        // if button pressed (checked) get color for this button
        // else show the white color
        if (m_table->cellRef(row, col).dataValue()->number())
            colorvalue = colorValue(col);
        else
            colorvalue = 0xffffffff;
        // set "indicator cell's" background color
        m_table->cellRef(row + 2, col).setBackgroundColor(colorvalue);
    }
}

QRgb MainWindow::colorValue(int index)
{
    // default returned color value (white)
    QRgb colorvalue = 0xffffffff;
    // select QRgb color values for the given index
    switch (index) {
        case COL_RED:     colorvalue = 0xffe73219; break;
        case COL_ORANGE:  colorvalue = 0xfff2b40e; break;
        case COL_YELLOW:  colorvalue = 0xfffcfa5d; break;
        case COL_GREEN:   colorvalue = 0xff05880b; break;
        case COL_BLUE:    colorvalue = 0xff0923e4; break;
        case COL_CYAN:    colorvalue = 0xff0bddea; break;
        case COL_MAGENTA: colorvalue = 0xffea0bdd; break;
        default:          colorvalue = 0xffffffff;
    }
    return colorvalue;
}


