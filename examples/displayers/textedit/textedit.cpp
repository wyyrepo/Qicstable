/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "textedit.h"

#include <QicsTable.h>
#include <QicsTextCellDisplay.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Text edit displayer example"));

    // dimensions of the data model
    int numRows = 14;
    int numCols = 5;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // create the table, using the data model we created above
    m_table = new QicsTable(dm, this);
    setCentralWidget(m_table);

    // set table properties
    m_table->setCellOverflowBehavior(Qics::Overflow);
    m_table->rowHeaderRef().setDefaultColumnWidthInPixels(30);

    // single line text
    m_table->cellRef(1,1).setLabel("Simple cell text");
    m_table->cellRef(2,1).setLabel("Cell's text can overflow neighboring empty cells");

    // multiline text using cell span
    QicsSpan span_text(4,1,3,3);
    m_table->addCellSpan(span_text);
    m_table->cellRef(4,1).setLabel( QString("%1%2%3").
        arg("1. This cell span contains multiline text\n").
        arg("2. This cell span contains multiline text\n").
        arg("3. This cell span contains multiline text\n"));

    // picture in the text cell
    m_table->cellRef(8,1).setLabel("Picture in the cell");
    m_table->cellRef(8,2).setPixmap(":/picture.jpg");

    // masked input
    m_table->cellRef(10,1).setLabel("Masked input edit:");
    QicsTextCellDisplay *txtmask = new QicsTextCellDisplay(m_table);
    txtmask->setContentScrollEnabled(false);
    txtmask->setInputMask("000.000.000.000;_");
    m_table->cellRef(10, 2).setDataValue(QicsDataString("192.168.0.1"));
    m_table->cellRef(10, 2).setDisplayer(txtmask);
    m_table->cellRef(10, 3).setDataValue(QicsDataString("123.45.67.89"));
    m_table->cellRef(10, 3).setDisplayer(txtmask);

    // password edit
    m_table->cellRef(12, 1).setLabel("Password edit:");
    QicsTextCellDisplay *txtpass = new QicsTextCellDisplay(m_table);
    txtpass->setContentScrollEnabled(false);
    txtpass->setEchoMode(QLineEdit::Password);
    m_table->cellRef(12, 2).setDataValue(QicsDataString("admin"));
    m_table->cellRef(12, 2).setDisplayer(txtpass);
    m_table->cellRef(12, 3).setDataValue(QicsDataString("user"));
    m_table->cellRef(12, 3).setDisplayer(txtpass);

    // make sure the table is only as large as necessary
    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);
}


