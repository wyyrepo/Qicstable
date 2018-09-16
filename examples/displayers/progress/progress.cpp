/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "progress.h"

#include <QicsTable.h>
#include <QicsProgressCellDisplay.h>

// progress bar displayer row
#define PROGRESS_CELL_ROW 3

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Progress displayer example"));

    // dimensions of the data model
    int numRows = 7;
    int numCols = 5;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // create the table, using the data model we created above
    m_table = new QicsTable(dm, this);
    setCentralWidget(m_table);

    // set table properties
    m_table->setCellOverflowBehavior(Qics::Overflow);
    m_table->rowHeaderRef().setDefaultColumnWidthInPixels(30);

    // change font for this row to bold
    QFont font = m_table->rowRef(1).font();
    font.setBold(true);
    m_table->rowRef(1).setFont(font);
    m_table->cellRef(1,1).setLabel("Change slider position to show progress");

    // create progress displayer
    QicsProgressCellDisplay *pbar = new QicsProgressCellDisplay(m_table);
    // set progress range
    pbar->setRange(0, 100);
    // set displayers to cells
    for (int i = 1; i < 4; i++)
        m_table->cellRef(PROGRESS_CELL_ROW, i).setDisplayer(pbar);

    // Create sliders displayers and set them to cells.
    // Note: SliderDisplayer class is subclassed from QicsWidgetCellDisplay,
    // so each SliderDisplayer object can be set only for the one cell. That is
    // why we have to create three objects to display three sliders.
    SliderDisplayer *sli1 = new SliderDisplayer(m_table);
    SliderDisplayer *sli2 = new SliderDisplayer(m_table);
    SliderDisplayer *sli3 = new SliderDisplayer(m_table);
    m_table->cellRef(PROGRESS_CELL_ROW + 2, 1).setDisplayer(sli1);
    m_table->cellRef(PROGRESS_CELL_ROW + 2, 2).setDisplayer(sli2);
    m_table->cellRef(PROGRESS_CELL_ROW + 2, 3).setDisplayer(sli3);

    // Connect slider's valueChanged() signals to the slot.
    // We can't use table's valueChanged() signal here, because our
    // SliderDisplayer class doesn't update cell's value.
    connect(sli1->slider(), SIGNAL(valueChanged(int)), this, SLOT(on_sliderValueChanged(int)));
    connect(sli2->slider(), SIGNAL(valueChanged(int)), this, SLOT(on_sliderValueChanged(int)));
    connect(sli3->slider(), SIGNAL(valueChanged(int)), this, SLOT(on_sliderValueChanged(int)));

    // make sure the table is only as large as necessary
    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);
}

void MainWindow::on_sliderValueChanged(int)
{
    // We set the single slot to three slider's signals, so we will
    // process all of them on value change. It's not optimal, but
    // let us simplify the code.
    for (int i = 1; i < 4; i++) {
        // get displayer object for the cell
        SliderDisplayer *sli = static_cast<SliderDisplayer*>(m_table->cellRef(PROGRESS_CELL_ROW + 2, i).displayer());
        // get slider's value
        int value = sli->slider()->value();
        // set progress new value
        m_table->cellRef(PROGRESS_CELL_ROW, i).setDataValue(QicsDataInt(value));
    }
}


