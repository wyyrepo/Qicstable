/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/
#include "displayers.h"

#include <QTableWidget>
#include <QicsTable.h>
#include <QicsDataModelDefault.h>
#include <QicsDateTimeCellDisplay.h>
#include <QicsComboCellDisplay.h>
#include <QicsComboCheckCellDisplay.h>
#include <QicsComboTabCellDisplay.h>
#include <QicsCheckCellDisplay.h>
#include <QicsTextCellDisplay.h>
#include <QicsTextCellDisplayHelpers.h>
#include <QicsPushButtonCellDisplay.h>
#include <QicsListCellDisplay.h>
#include <QicsRadioCellDisplay.h>
#include <QicsProgressCellDisplay.h>
#include <QicsColumn.h>
#include <QicsRowHeader.h>
#include <QicsColumnHeader.h>
#include <QicsMainGrid.h>
#include <QicsRegionalAttributeController.h>


Displayers::Displayers(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Displayers Demo"));

    // dimensions of the data model
    int numRows = 50;
    int numCols = 6;
    m_current_row = 0;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // create the table, using the data model we created above
    m_table = new QicsTable(dm, this);
    setCentralWidget(m_table);

    QicsRegionalAttributeController controller;
    m_table->setExternalAttributeController(controller);

    m_table->setSelectionPolicy(Qics::SelectMultipleRow);
    m_table->setCellOverflowBehavior(Qics::Overflow);

    m_table->rowHeaderRef().setDefaultColumnWidthInPixels(30);
    m_table->columnRef(4).setMargin(0);
    m_table->columnRef(3).setWidthInPixels(120);
    m_table->columnRef(4).setWidthInPixels(120);

    // list data
    m_colors << "Red" << "Orange" << "Yellow" << "Green" << "Blue" << "Cyan" << "Magenta";

    fillTable();

    // make sure the table is only as large as necessary
    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);
}

Displayers::~Displayers()
{
}

void Displayers::fillTable()
{
    QFont font = m_table->columnRef(0).font();
    font.setBold(true);
    m_table->columnRef(0).setFont(font);

    fillBasicDisplayers();
    fillAdvancedDisplayers();
}

void Displayers::fillBasicDisplayers()
{
    m_current_row++;
    m_table->rowRef(m_current_row).setHeightInPixels(40);
    m_table->cellRef(m_current_row,0).setLabel("Basic Cell Displayers");
    m_table->cellRef(m_current_row,0).setFont(QFont("Helvetica", 20));
    m_table->cellRef(m_current_row,0).setForegroundColor(QColor(Qt::blue));

    // text displayer
    m_current_row += 2;
    m_table->cellRef(m_current_row,0).setLabel("Single line text displayer");
    m_table->cellRef(m_current_row,3).setLabel("Simple text");
    m_table->cellRef(m_current_row,4).setLabel("Simple text");
    m_current_row++;
    m_table->cellRef(m_current_row,0).setLabel("Single line text with overflow");
    m_table->cellRef(m_current_row,3).setLabel("Simple text overflows several empty cells");

    m_current_row += 2;
    m_table->cellRef(m_current_row,0).setLabel("Multicell Text Displayer");
    QicsSpan span_text(m_current_row,3,3,2);
    m_table->addCellSpan(span_text);
    m_table->cellRef(m_current_row,3).setLabel( QString("%1%2%3").
        arg("1. This cell contains multiline text\n").
        arg("2. This cell contains multiline text\n").
        arg("3. This cell contains multiline text\n"));

    // combo
    m_current_row += 4;
    m_table->cellRef(m_current_row,0).setLabel("Combobox Cell Displayer");

    QicsComboCellDisplay *cb1 = new QicsComboCellDisplay(m_table);
    cb1->addItems(m_colors);
    cb1->setAddValueToList(false);
    cb1->setEditable(true);
    m_table->cellRef(m_current_row,3).setDataValue(QicsDataString("Red"));
    m_table->cellRef(m_current_row,3).setDisplayer(cb1);

    m_table->cellRef(m_current_row,4).setDataValue(QicsDataString("Blue"));
    m_table->cellRef(m_current_row,4).setDisplayer(cb1);

    m_current_row++;
    QicsComboCellDisplay *cb2 = new QicsComboCellDisplay(m_table);
    cb2->addItems(m_colors);
    cb2->setAddValueToList(false);
    cb2->setEditable(false);
    m_table->cellRef(m_current_row,3).setDataValue(QicsDataString("Green"));
    m_table->cellRef(m_current_row,3).setDisplayer(cb2);

    m_table->cellRef(m_current_row,4).setDataValue(QicsDataString("Yellow"));
    m_table->cellRef(m_current_row,4).setDisplayer(cb2);

    // check
    m_current_row += 2;
    m_table->cellRef(m_current_row,0).setLabel("Checkbox Cell Displayer");

    QicsCheckCellDisplay *chb1 = new QicsCheckCellDisplay(m_table);
    m_table->cellRef(m_current_row,3).setLabel("Two-state");
    m_table->cellRef(m_current_row,3).setDisplayer(chb1);

    QicsCheckCellDisplay *chb2 = new QicsCheckCellDisplay(m_table);
    chb2->setTristate(true);
    m_table->cellRef(m_current_row,4).setLabel("Tristate");
    m_table->cellRef(m_current_row,4).setDisplayer(chb2);
}

void Displayers::fillAdvancedDisplayers()
{
    m_current_row += 3;
    m_table->rowRef(m_current_row).setHeightInPixels(40);
    m_table->cellRef(m_current_row,0).setLabel("Advanced Cell Displayers");
    m_table->cellRef(m_current_row,0).setFont(QFont("Helvetica", 20));
    m_table->cellRef(m_current_row,0).setForegroundColor(QColor(Qt::blue));

    // datetime
    m_current_row += 2;
    m_table->cellRef(m_current_row,0).setLabel("DateTime Cell Displayer");
    QicsDateTimeCellDisplay *dt = new QicsDateTimeCellDisplay(m_table);
    m_table->cellRef(m_current_row, 3).setDataValue(QicsDataDateTime(QDateTime::currentDateTime()));
    m_table->cellRef(m_current_row, 3).setDisplayer(dt);

    m_table->cellRef(m_current_row, 4).setDataValue(QicsDataDateTime(QDateTime::currentDateTime()));
    m_table->cellRef(m_current_row, 4).setDisplayer(dt);

    // listwidget
    m_current_row += 2;
    m_table->cellRef(m_current_row,0).setLabel("List Cell Displayer");
    QicsSpan span_lc(m_current_row,3,4,1);
    m_table->addCellSpan(span_lc);
    QicsListCellDisplay *lc = new QicsListCellDisplay(m_table);
    lc->addItems(m_colors);
    m_table->cellRef(m_current_row, 3).setDataValue(QicsDataString(""));
    m_table->cellRef(m_current_row, 3).setDisplayer(lc);

    QicsSpan span_lc1(m_current_row,4,4,1);
    m_table->addCellSpan(span_lc1);
    m_table->cellRef(m_current_row, 4).setDataValue(QicsDataString(""));
    m_table->cellRef(m_current_row, 4).setDisplayer(lc);

    // radio buttons
    m_current_row += 6;
    m_table->cellRef(m_current_row,0).setLabel("Radio Cell Displayer");
    QicsSpan span_rc(m_current_row,3,6,1);
    m_table->addCellSpan(span_rc);
    m_table->cellRef(m_current_row, 3).setDataValue(QicsDataInt(-1));
    QicsRadioCellDisplay *rc = new QicsRadioCellDisplay(m_table);
    rc->addItems(m_colors);
    m_table->cellRef(m_current_row, 3).setDisplayer(rc);

    QicsSpan span_rc1(m_current_row,4,6,1);
    m_table->addCellSpan(span_rc1);
    m_table->cellRef(m_current_row, 4).setDataValue(QicsDataInt(-1));
    m_table->cellRef(m_current_row, 4).setDisplayer(rc);

    // dropdown checklist combobox
    m_current_row += 8;
    m_table->cellRef(m_current_row,0).setLabel("ComboCheck Cell Displayer");
    QicsComboCheckCellDisplay *ccb = new QicsComboCheckCellDisplay(m_table);
    ccb->addItems(m_colors);
    ccb->setAddValueToList(false);
    ccb->setEditable(true);
    ccb->setDefaultText("Colors...");
    m_table->cellRef(m_current_row, 3).setDisplayer(ccb);
    m_table->cellRef(m_current_row, 4).setDisplayer(ccb);

    // tabbed combobox
    m_current_row += 2;
    m_table->cellRef(m_current_row,0).setLabel("Tabbed Combo Cell Displayer");
    QicsComboTabCellDisplay *tcb = new QicsComboTabCellDisplay(m_table);
    tcb->setTabSize(m_colors.size(),2);
    tcb->setActiveColumn(0);
    tcb->setDefaultText("Color's Indexes...");
    QStringList labels;
    labels << "Id" << "Value";
    tcb->tabWidget()->setHorizontalHeaderLabels(labels);
    for (int i = 0; i<m_colors.size(); ++i)
        tcb->setTabData(i, 0, QString("%1").arg(i+1));
    tcb->addColumnData(1, m_colors);
    tcb->setAddValueToList(false);
    m_table->cellRef(m_current_row, 3).setDisplayer(tcb);

    QicsComboTabCellDisplay *tcb2 = new QicsComboTabCellDisplay(m_table);
    tcb2->setTabSize(m_colors.size(),2);
    tcb2->setActiveColumn(1);
    tcb2->setDefaultText("Colors...");
    QStringList labels2;
    labels2 << "Id" << "Value";
    tcb2->tabWidget()->setHorizontalHeaderLabels(labels2);
    for (int i = 0; i<m_colors.size(); ++i)
        tcb2->setTabData(i, 0, QString("%1").arg(i+1));
    tcb2->addColumnData(1, m_colors);
    tcb2->setAddValueToList(false);
    m_table->cellRef(m_current_row, 4).setDisplayer(tcb2);

    // progress bar
    m_current_row += 2;
    m_progress_row = m_current_row;
    m_table->cellRef(m_current_row,0).setLabel("Progress Bar Displayer");
    QicsProgressCellDisplay *pbar = new QicsProgressCellDisplay(m_table);
    pbar->setRange(0, 100);
    m_table->cellRef(m_current_row, 3).setDisplayer(pbar);

    QicsPushButtonCellDisplay *pbrun = new QicsPushButtonCellDisplay(m_table);
    m_table->cellRef(m_progress_row,4).setLabel("Start/Stop");
    m_table->cellRef(m_progress_row,4).setDisplayer(pbrun);
    connect(pbrun, SIGNAL(clicked(int,int)), this, SLOT(onRunButtonClicked(int,int)));

    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));

    m_current_row++;
    QicsProgressCellDisplay *pbar2 = new QicsProgressCellDisplay(m_table);
    pbar2->setRange(0, 300);
    pbar2->setTextVisible(false);
    m_table->cellRef(m_current_row, 3).setDisplayer(pbar2);

    // masked edit 1
    m_current_row += 2;
    m_table->cellRef(m_current_row,0).setLabel("Masked Edit Displayer");
    QicsTextCellDisplay *txt1 = new QicsTextCellDisplay(m_table);
    txt1->setContentScrollEnabled(false);
    txt1->setInputMask("99:99 >AA;_");
    m_table->cellRef(m_current_row, 3).setDataValue(QicsDataString("12:54 PM"));
    m_table->cellRef(m_current_row, 3).setDisplayer(txt1);

    m_table->cellRef(m_current_row, 4).setDataValue(QicsDataString("03:17 AM"));
    m_table->cellRef(m_current_row, 4).setDisplayer(txt1);

    // masked edit 2
    m_current_row++;
    QicsTextCellDisplay *txt2 = new QicsTextCellDisplay(m_table);
    txt2->setContentScrollEnabled(false);
    txt2->setInputMask("99/99/99;_");
    m_table->cellRef(m_current_row, 3).setDataValue(QicsDataString("02/10/07"));
    m_table->cellRef(m_current_row, 3).setDisplayer(txt2);

    m_table->cellRef(m_current_row, 4).setDataValue(QicsDataString("15/03/02"));
    m_table->cellRef(m_current_row, 4).setDisplayer(txt2);

    // masked edit 3
    m_current_row++;
    QicsTextCellDisplay *txt3 = new QicsTextCellDisplay(m_table);
    txt3->setContentScrollEnabled(false);
    txt3->setInputMask("000.000.000.000;_");
    m_table->cellRef(m_current_row, 3).setDataValue(QicsDataString("192.168.0.1"));
    m_table->cellRef(m_current_row, 3).setDisplayer(txt3);

    m_table->cellRef(m_current_row, 4).setDataValue(QicsDataString("123.45.67.89"));
    m_table->cellRef(m_current_row, 4).setDisplayer(txt3);

    // password edit
    m_current_row += 2;
    m_table->cellRef(m_current_row,0).setLabel("Password Edit Displayer");
    QicsTextCellDisplay *txt4 = new QicsTextCellDisplay(m_table);
    txt4->setContentScrollEnabled(false);
    txt4->setEchoMode(QLineEdit::Password);
    m_table->cellRef(m_current_row, 3).setDataValue(QicsDataString("admin"));
    m_table->cellRef(m_current_row, 3).setDisplayer(txt4);

    m_table->cellRef(m_current_row, 4).setDataValue(QicsDataString("user"));
    m_table->cellRef(m_current_row, 4).setDisplayer(txt4);
}

void Displayers::onRunButtonClicked(int row,int col)
{
    Q_UNUSED(row);
    Q_UNUSED(col);
    if (!m_timer->isActive())
        m_timer->start();
    else
        m_timer->stop();
}

void Displayers::onTimer()
{
    int val;
    // progress 1
    if (m_table->cellRef(m_progress_row, 3).dataValue())
        val = m_table->cellRef(m_progress_row, 3).dataValue()->number();
    else
        val = 0;
    if (val > 100) val = 0;
    m_table->cellRef(m_progress_row, 3).setDataValue(QicsDataInt(val + 5));

    // progress 2
    if (m_table->cellRef(m_progress_row + 1, 3).dataValue())
        val = m_table->cellRef(m_progress_row + 1, 3).dataValue()->number();
    else
        val = 0;
    if (val > 300) val = 0;
    m_table->cellRef(m_progress_row + 1, 3).setDataValue(QicsDataInt(val + 10));
}


