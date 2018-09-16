/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "bigtable.h"

#include <QMenuBar>
#include <QApplication>
#include <QComboBox>
#include <QToolBar>
#include <QMessageBox>
#include <QStatusBar>

#include <QicsTable.h>
#include <QicsDataModelDefault.h>
#include <QicsRegionalAttributeController.h>

MainFrame::MainFrame (QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Big Table Example"));
    setAttribute(Qt::WA_DeleteOnClose);
    statusBar();

    m_viewNumber = 1;
    m_rowsCount = 1024;
    m_columnsCount = 1024;

    /*
    *  Pulldown Menu
    */

    // Table
    QMenu* menuTable = new QMenu(tr("&Table"), this);
    menuBar()->addMenu(menuTable);

    m_actFill = menuTable->addAction(QIcon(":/Resources/icon16_run.png"), tr("&Fill table"), this, SLOT(fill()), Qt::CTRL+Qt::Key_F);
    m_actRun = menuTable->addAction(QIcon(":/Resources/icon16_autorun.png"), tr("&Run autofill"), this, SLOT(run()), Qt::CTRL+Qt::Key_R);
    m_actStop = menuTable->addAction(QIcon(":/Resources/icon16_stop.png"), tr("&Stop filling"), this, SLOT(stop()), Qt::CTRL+Qt::Key_S);
    m_actStop->setEnabled(false);

    menuTable->addSeparator();

    QAction* actNew = menuTable->addAction(QIcon(":/Resources/icon16_newview.png"), tr("&New View"), this, SLOT(newTable()), Qt::CTRL+Qt::Key_N);
    QAction* actNewRgn = menuTable->addAction(QIcon(":/Resources/icon16_newregion.png"), tr("New View from selection"), this, SLOT(newTableRegion()), Qt::CTRL+Qt::Key_M);

    menuTable->addSeparator();
    menuTable->addAction(tr("E&xit"), qApp, SLOT(closeAllWindows()), Qt::CTRL+Qt::Key_X);

    // Help
    QMenu* help = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(help);

    QAction* actHelp = help->addAction(QIcon(":/Resources/icon16_help.png"), tr("About..."), this, SLOT(about()), Qt::CTRL+Qt::Key_H);

    /*
    *  Toolbar
    */
    QToolBar *mainToolbar = addToolBar("ToolBar");

    m_comboFill = new QComboBox(this);
    m_comboFill->addItem("QicsDataItem_Int", QicsDataItem_Int);
    m_comboFill->addItem("QicsDataItem_Double", QicsDataItem_Double);
    m_comboFill->addItem("QicsDataItem_Float", QicsDataItem_Float);
    m_comboFill->addItem("QicsDataItem_String", QicsDataItem_String);
    m_comboFill->addItem("QicsDataItem_Long", QicsDataItem_Long);
    m_comboFill->addItem("QicsDataItem_LongLong", QicsDataItem_LongLong);
    m_comboFill->addItem("QicsDataItem_Bool", QicsDataItem_Bool);

    mainToolbar->addWidget(m_comboFill);

    mainToolbar->addAction(m_actFill);
    mainToolbar->addAction(m_actRun);
    mainToolbar->addAction(m_actStop);

    mainToolbar->addSeparator();
    mainToolbar->addAction(actNew);
    mainToolbar->addAction(actNewRgn);

    mainToolbar->addSeparator();
    mainToolbar->addAction(actHelp);

    /*
    *  Table
    */
    // create the data model
    m_dm = new QicsDataModelDefault(m_rowsCount, m_columnsCount);

    // create the table, using the data model we created above
    m_table = createTable(this);
    m_table->repeatingRow(0, 2)->setForegroundColor(Qt::darkGreen);

    setCentralWidget(m_table);
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
        "This example shows how fast QicsTable can update large amount of data and display it in several views. "
        "<p>"
        "<b>HOW TO USE</b><br>"
        "Select from combo box type of data you want the table to be filled up.<br> "
        "Then use <i>Fill</i> action to make single fillup with random data of whole table.<br> "
        "Also you can run <i>Autofill</i> action to do the fillup repeatedly till calling <i>Stop</i> action.<br><br> "
        "Watch the status bar to see how much time is spent for full table update.<br><br>"
        "You can create more table views using <i>New View</i> action to create a view for the whole data model, "
        "or using <i>New View from selection</i> action to create a view for the current selection in the main table.<br> "
        "Having several views, you can alter their cells and see how they are updated in another views.<br> "
        "Also you can drag&drop data between views (using middle mouse button). "
        "<hr>"
        "</td></table>"
        );
    helpDialog.setWindowTitle(tr("About Big Table Example"));
    helpDialog.setIcon(QMessageBox::NoIcon);
    helpDialog.exec();
}

void MainFrame::refill(int type)
{
    // force model to do not emit any signals
    // since we need to update it as fast as possible
    m_dm->setEmitSignals(false);

    // start timer
    QTime t;
    t.start();

    // update all the values in the table with random numbers
    for(int row = 0; row < m_rowsCount; ++row) {
        for(int col = 0; col < m_columnsCount; ++col) {
            switch (type) {
            case QicsDataItem_Int:
                m_dm->setItem(row, col, rand());
                break;
            case QicsDataItem_Long:
                m_dm->setItem(row, col, (long)rand());
                break;
            case QicsDataItem_LongLong:
                m_dm->setItem(row, col, (qint64)rand());
                break;
            case QicsDataItem_Bool:
                m_dm->setItem(row, col, (bool)(rand()&1));
                break;
            case QicsDataItem_Double:
                m_dm->setItem(row, col, (double)rand());
                break;
            case QicsDataItem_Float:
                m_dm->setItem(row, col, (float)rand());
                break;
            case QicsDataItem_String:
                m_dm->setItem(row, col, QString::number(rand()));
                break;
            default:
                m_dm->setItem(row, col, tr("Unknown"));
                break;
            }
        }
        qApp->processEvents();
        if (!m_progress) break;
    }

    // get time info
    int ms = t.elapsed();
    if (m_progress)
        statusBar()->showMessage(tr("%1x%2 table - updated in %3 ms").arg(m_rowsCount).arg(m_columnsCount).arg(ms));
    else
        statusBar()->showMessage(tr("User Break"));

    // signals back
    m_dm->setEmitSignals(true);

    // update tables
    emit modelUpdated();
}

void MainFrame::fill()
{
    m_progress = true;
    m_comboFill->setEnabled(false);
    m_actFill->setEnabled(false);
    m_actRun->setEnabled(false);
    m_actStop->setEnabled(true);
    refill(m_comboFill->itemData(m_comboFill->currentIndex()).toInt());
    stop();
}

void MainFrame::run()
{
    m_progress = true;
    m_comboFill->setEnabled(false);
    m_actFill->setEnabled(false);
    m_actRun->setEnabled(false);
    m_actStop->setEnabled(true);
    refill(m_comboFill->itemData(m_comboFill->currentIndex()).toInt());
    if (m_progress)
        QTimer::singleShot(0, this, SLOT(run()));
}

void MainFrame::stop()
{
    m_progress = false;
    m_comboFill->setEnabled(true);
    m_actFill->setEnabled(true);
    m_actRun->setEnabled(true);
    m_actStop->setEnabled(false);
}

void MainFrame::newTable()
{
    QicsTable *t = createTable();
    t->setWindowTitle(tr("New Table View #%1").arg(m_viewNumber++));
    t->show();
}

QicsTable* MainFrame::createTable(QWidget *parent)
{
    QicsTable *t = new QicsTable(m_dm, parent);

    t->setDefaultColumnWidthInPixels(40);
    t->setNavigatorAllowed(true);
    QicsRegionalAttributeController c;
    t->setExternalAttributeController(c);
    t->setWideKeyActions(true);

    t->repeatingRow(0, 2)->setForegroundColor(rand() + (rand()<<8));
    t->repeatingRow(0, 2)->setBackgroundColor(0xf0f0f0);

    connect(this, SIGNAL(modelUpdated()), t, SLOT(repaint()));
    return t;
}

void MainFrame::newTableRegion()
{
    // get 1st selected region from the parent table
    QicsSelectionList* sl = m_table->selectionList();
    if (sl) {
        if (sl->count()) {
            QicsSelection& sel = sl->first();
            QicsRegion r(sel.topRow(), sel.leftColumn(), sel.bottomRow(), sel.rightColumn());

            // create table
            QicsTable *t = createTable();
            t->setViewport(r);
            t->setVisibleColumns(r.numColumns());
            t->setVisibleRows(r.numRows());
            t->setWindowTitle(tr("New Table View #%1 (Region: %2,%3-%4,%5)")
                .arg(m_viewNumber++)
                .arg(sel.topRow()).arg(sel.leftColumn()).arg(sel.bottomRow()).arg(sel.rightColumn()));

            t->show();

            delete sl;
            return;
        }
        delete sl;
    }

    // no selection - return whole new table
    newTable();
}

void MainFrame::closeEvent (QCloseEvent *event)
{
    stop();
    event->accept();
    QTimer::singleShot(0, qApp, SLOT(closeAllWindows()));
    QMainWindow::closeEvent(event);
}


