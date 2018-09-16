/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/
#include "doubleTable.h"

#include <QApplication>
#include <QMenuBar>
#include <QTimer>

#include <QicsColumnHeader.h>
#include <QicsCell.h>
#include <QicsRowHeader.h>
#include <QicsTable.h>
#include <QicsDataModelDefault.h>


// DoubleTable class:
DoubleTable::DoubleTable(QWidget* parent, QicsDataModelDefault* dataModel, const QString& caption)
    : QMainWindow(parent)
{
    setWindowTitle(caption);
    m_table = new QicsTable(dataModel, this);
    setCentralWidget(m_table);
    m_table->setVisibleRows(COUNT_ROWS);
    m_table->setVisibleColumns(COUNT_COLS);
    QString textCell;

    for (int i = 0; i < COUNT_ROWS; ++i)
        for (int j = 0; j < COUNT_COLS; ++j) {
            m_table->columnHeaderRef().cellRef(0, j).setLabel(textCell.setNum(j));
            m_table->rowHeaderRef().cellRef(i, 0).setLabel(textCell.setNum(i));
            textCell = QString("Cell (");
            textCell += m_table->columnHeaderRef().cellRef(0, j).label();
            textCell += QString(";");
            textCell += QString((m_table->rowHeaderRef().cellRef(i, 0).label()));
            textCell += QString(")");
            dataModel->setItem(i, j, QicsDataString(textCell));
            m_table->cellRef(i, j).setWidthInChars(7);
        }
}

DoubleTable::~DoubleTable()
{
}

// Tester class:
Tester::Tester(DoubleTable* table1, DoubleTable* table2, QicsDataModel* dataModel)
{
    table1 = table1;
    table2 = table2;
    m_dataModel = dataModel;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(changeItems()));
    m_timer->start(1000);
    qsrand(QDateTime::currentDateTime().toTime_t());
}

Tester::~Tester()
{
}

void Tester::changeItems()
{
    int lenght;
    QString str;
    char letter;

    for (int i = 0; i < COUNT_ROWS; ++i)
        for (int j = 0; j < COUNT_COLS; ++j) {
            switch (rand() % 3 + 1)
            {
            case 2:
                m_dataModel->setItem(i, j, QicsDataFloat(static_cast <float> (rand() % 100) / ((rand() % 10) + 3.0f)));
                break;
            case 3:
                lenght = rand() % 7 + 1;
                for (int k = 0; k < lenght; ++k) {
                    letter = rand() % 25 + 65;
                    str += QString(letter);
                }
                m_dataModel->setItem(i, j, QicsDataString(str));
                str = QString();
                break;
            default:
                m_dataModel->setItem(i, j, QicsDataInt(rand() % 100));
                break;
            }
        }
}


