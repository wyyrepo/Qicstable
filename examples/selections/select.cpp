/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "select.h"

#include <QMenuBar>
#include <QApplication>
#include <QLabel>
#include <QMessageBox>

#include <QicsDataModelDefault.h>
#include <QicsTable.h>


MainFrame::MainFrame (QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Select Example"));

    /*
    *  Pulldown Menu
    */

    // File
    QMenu* menuFile = new QMenu(tr("&File"), this);
    menuFile->addAction(tr("E&xit"), qApp, SLOT(closeAllWindows()), Qt::CTRL+Qt::Key_X);
    menuBar()->addMenu(menuFile);

    // Selection mode
    QMenu* menuSelect = new QMenu(tr("&Selection"), this);
    QActionGroup *agSelect = new QActionGroup(this);
    QAction *mul = 0;
    agSelect->setExclusive(true);
    agSelect->addAction(menuSelect->addAction(tr("SelectNone"), this, SLOT(modeSelectNone()),Qt::Key_1));
    agSelect->addAction(menuSelect->addAction(tr("SelectSingle"), this, SLOT(modeSelectSingle()),Qt::Key_2));
    agSelect->addAction(mul = menuSelect->addAction(tr("SelectMultiple"), this, SLOT(modeSelectionMultiple()),Qt::Key_3));
    agSelect->addAction(menuSelect->addAction(tr("SelectSingleRow"), this, SLOT(modeSelectSingleRow()),Qt::Key_4));
    agSelect->addAction(menuSelect->addAction(tr("SelectMultipleRow"), this, SLOT(modeSelectMultipleRow()),Qt::Key_5));

    QAction *a = 0;
    foreach(a,agSelect->actions())
        a->setCheckable(true);

    menuSelect->addSeparator();
    a = menuSelect->addAction(tr("Exclusive Mode"));
    a->setCheckable(true);
    connect(a, SIGNAL(triggered(bool)), this, SLOT(modeSelectExclusive(bool)));

    menuBar()->addMenu(menuSelect);

    // Help
    QMenu* help = new QMenu(tr("&Help"), this);
    help->addAction(QIcon(":/Resources/icon16_help.png"), tr("About..."), this, SLOT(about()), Qt::CTRL+Qt::Key_H);
    menuBar()->addMenu(help);

    /*
    *  Table
    */

    // create the data model
    m_dm = new QicsDataModelDefault(30,20);
    int cnt = 0;

    for(int row = 0; row < 30; ++row)
        for(int col = 0; col < 20; ++col)
            m_dm->setItem(row, col, cnt++);

    // create the table, using the data model we created above
    m_table = new QicsTable(m_dm, this);
    m_table->setBottomHeaderVisible(true);
    m_table->setRightHeaderVisible(true);
    m_table->setDefaultColumnWidthInPixels(40);
    m_table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_table->setVisibleColumns(20);
    m_table->setVisibleRows(30);
    m_table->show();

    setCentralWidget(m_table);

    m_label = new QLabel(this);
    m_label->setFrameShape(QFrame::Box);
    m_table->setTopTitleWidget(m_label);

    menuSelect->setDefaultAction(mul);
    mul->setChecked(true);
    modeSelectionMultiple();
}

void MainFrame::modeSelectNone()
{
    m_table->setSelectionPolicy(Qics::SelectNone);
    m_label->setText("<b>Selection disabled.</b>");
}

void MainFrame::modeSelectSingle()
{
    m_table->setSelectionPolicy(Qics::SelectSingle);
    m_label->setText("<b>Single selection mode.</b> Only one cell/row/column could be selected.");
}

void MainFrame::modeSelectionMultiple()
{
    m_table->setSelectionPolicy(Qics::SelectMultiple);
    m_label->setText("<b>Multiple selection mode.</b> Hold CTRL to extend/contract cell selection, SHIFT to contiguous selection.");
}

void MainFrame::modeSelectSingleRow()
{
    m_table->setSelectionPolicy(Qics::SelectSingleRow);
    m_label->setText("<b>Single Row selection mode.</b> Only one row could be selected.");
}

void MainFrame::modeSelectMultipleRow()
{
    m_table->setSelectionPolicy(Qics::SelectMultipleRow);
    m_label->setText("<b>Multiple Row selection mode.</b> Hold CTRL to extend/contract row selection, SHIFT to contiguous selection.");
}

void MainFrame::modeSelectExclusive(bool on)
{
    m_table->setSelectionStyle(on ? Qics::Exclusive : Qics::Normal);
    m_label->setText(on ?"<b>Exclusive selection style.</b>" : "<b>Normal selection style.</b>");
}

void MainFrame::about()
{
    // init helpDialog
    QMessageBox helpDialog(this);
    helpDialog.setText(
        "<table>"
        "<tr><td>"
        "<img src=':/Resources/ics.png'>"
        "<br><h3>Integrated Computer Solutions Incorporated</h3>"
        "</td><td>"
        "<h3>The User Interface Company</h3>"
        "54B Middlesex Turnpike<br>Bedford, MA USA 01730<br><br>"
        "<b>info@ics.com<br>www.ics.com</b>"
        "</td><tr><td colspan=2>"
        "<hr>"
        "This example shows several selection modes of QicsTable."
        "<p>"
        "<b>HOW TO USE</b><br>"
        "Select a mode from <b>Selection</b> menu, then try to select some cells."
        "<hr>"
        "</td></table>"
        );
    helpDialog.setWindowTitle( tr("About Select Example") );
    helpDialog.setIcon( QMessageBox::NoIcon );
    helpDialog.exec();
}


