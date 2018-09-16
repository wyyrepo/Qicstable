/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "listwidget.h"

#include <QicsTable.h>
#include <QicsListCellDisplay.h>

// listwidget row
#define LIST_CELL_ROW  3
// "color indicator" row
#define COLOR_CELL_ROW 10

// colors values definition
enum Colors {COL_RED = 0, COL_ORANGE = 1, COL_YELLOW = 2, COL_GREEN = 3,
             COL_BLUE = 4, COL_CYAN = 5, COL_MAGENTA = 6};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("List widget displayer example"));

    // dimensions of the data model
    int numRows = 17;
    int numCols = 5;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // create the table, using the data model we created above
    m_table = new QicsTable(dm, this);
    setCentralWidget(m_table);

    // connect valueChanged() signal to process cell's values changes
    connect(m_table, SIGNAL(valueChanged(int, int)), this, SLOT(on_cellValueChanged(int, int)));
    // connect clicked() signal to process mouse clicks in the cells
    connect(m_table, SIGNAL(clicked(int, int, int, const QPoint &)),
            this, SLOT(on_cellClicked(int, int, int, const QPoint &)));

    // set table properties
    m_table->setCellOverflowBehavior(Qics::Overflow);
    m_table->rowHeaderRef().setDefaultColumnWidthInPixels(30);

    // change font for this row to bold
    QFont font = m_table->rowRef(1).font();
    font.setBold(true);
    m_table->rowRef(1).setFont(font);
    m_table->cellRef(1,1).setLabel("Click list widgets to change colors below");

    // // list data for the list widget
    m_colors << "Red" << "Orange" << "Yellow" << "Green" << "Blue" << "Cyan" << "Magenta";

    // create list widget displayer
    QicsListCellDisplay *lc = new QicsListCellDisplay(m_table);
    lc->addItems(m_colors);

    // set initial value for the cells
    m_table->cellRef(LIST_CELL_ROW, 1).setDataValue(QicsDataString("Red"));
    m_table->cellRef(LIST_CELL_ROW, 2).setDataValue(QicsDataString("Green"));
    m_table->cellRef(LIST_CELL_ROW, 3).setDataValue(QicsDataString("Blue"));

    // create cell span for each list widget object and set the displayer
    for (int i = 1; i < 4; i++) {
        QicsSpan span_lc(LIST_CELL_ROW, i, 6, 1);
        m_table->addCellSpan(span_lc);
        m_table->cellRef(LIST_CELL_ROW, i).setDisplayer(lc);
    }

    // "color indicators" cells
    for (int i = 1; i < 4; i++) {
        QicsSpan span_col(COLOR_CELL_ROW, i, 4, 1);
        m_table->addCellSpan(span_col);
    }

    // set font for the row and show the text
    m_table->rowRef(COLOR_CELL_ROW + 5).setFont(font);
    m_table->cellRef(COLOR_CELL_ROW + 5,1).setLabel("Click color cells to change active items above");

    // make sure the table is only as large as necessary
    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);
}

void MainWindow::on_cellValueChanged(int row, int col)
{
    // process cells changes for the list widget displayers only
    if (row == LIST_CELL_ROW && (col >= 1 && col <=3)) {
        // get current cell's value (current string)
        QString value = m_table->cellRef(LIST_CELL_ROW, col).dataValue()->string();
        // get appropriate color value for the current string index
        int index = m_colors.indexOf(value);
        QRgb colorvalue = colorValue(index);
        // set "indicator cell's" background color
        m_table->cellRef(COLOR_CELL_ROW, col).setBackgroundColor(colorvalue);
    }
}

void MainWindow::on_cellClicked(int row, int col, int button, const QPoint &)
{
    // process mouse clicks for the color "indicator cells" only
    if (row == COLOR_CELL_ROW && (col >= 1 && col <=3)) {
        // get current cell's value (current string)
        QString value = m_table->cellRef(LIST_CELL_ROW, col).dataValue()->string();
        int index = m_colors.indexOf(value);
        // increase index if the left button or decrease if the right button
        if (button == Qt::LeftButton) index++;
        else if (button == Qt::RightButton) index--;
        else return;
        // make sure our index is in the valid range
        if (index >= m_colors.count()) index = 0;
        else if (index < 0) index = m_colors.count() - 1;
        QString color = "";
        // get new string value for the index
        if (index >=0 && index < m_colors.count())
            color = m_colors.at(index);
        // set new current item for the list widget
        m_table->cellRef(LIST_CELL_ROW, col).setDataValue(QicsDataString(color));
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


