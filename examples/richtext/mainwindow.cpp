/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "mainwindow.h"

#include <QicsTable.h>
#include <QicsDataModelDefault.h>
#include "QicsRichTextCellDisplay.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("RichText Example"));

    // dimensions of the data model
    const int numRows = 100;
    const int numCols = 4;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);
    m_table = new QicsTable(dm);
    m_table->setVisibleRows(20);
    m_table->setVisibleColumns(numCols);
    m_table->setDisplayer(new QicsRichTextCellDisplay(m_table));

    QString tablePattern("<html><body><i>M<sub>%1</sub>N<sub>%2</sub></i></body></html>");
    for (int i = 0; i < dm->numRows(); ++i)
        for (int j = 0; j < dm->numColumns(); ++j)
            m_table->dataModel()->setItem(i, j, tablePattern.arg(i).arg(j));

    QicsColumnHeader *colheader = m_table->columnHeader();
    colheader->setDisplayer(new QicsRichTextCellDisplay(m_table));
    QString colPattern("<html><body><i>N<sub>%1</sub></i></body></html>");
    for (int i = 0; i < dm->numColumns(); ++i)
        colheader->dataModel()->setItem(0, i, colPattern.arg(i));
    delete colheader;

    QicsRowHeader *rowheader = m_table->rowHeader();
    rowheader->setDisplayer(new QicsRichTextCellDisplay(m_table));
    QString rowPattern("<html><body><i>M<sub>%1</sub></i></body></html>");
    for (int i = 0; i < dm->numRows(); ++i)
        rowheader->dataModel()->setItem(i, 0, rowPattern.arg(i));
    delete rowheader;

    setCentralWidget( m_table );
}

MainWindow::~MainWindow()
{
}


