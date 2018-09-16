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

#include <QStandardItemModel>
#include <QMenuBar>

#include <QicsDataModelQtModelAdapter.h>
#include <QicsTable.h>


MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
m_sortedByCol( -1 ),
m_sortOrder( Qics::Ascending )
{
    setWindowTitle(tr("Qt Data Model Example"));

    createQtModel();
    createActions();
    createMenus();
    createTable();
    QMetaObject::connectSlotsByName( this );
}

MainWindow::~MainWindow()
{
    delete m_icsModel;
    delete m_qtModel;
}

void MainWindow::createTable()
{
    m_icsModel = new QicsDataModelQtModelAdapter();
    m_icsModel->setModel(m_qtModel);
    m_table = new QicsTable( m_icsModel );

    connect( m_table->columnHeader(), SIGNAL( doubleClicked (int, int, int, const QPoint& ) ),
        this, SLOT( columnHeader_doubleClicked (int, int, int, const QPoint& ) ) );

    m_table->setRowsSortingMode(Qics::QicsStableSort);
    setCentralWidget( m_table );
}

void MainWindow::createQtModel()
{
    m_qtModel = new QStandardItemModel( 10, 10, 0 );
    QModelIndex index;

    for ( int i = 0; i < 10; ++i ) {
        for ( int j = 0; j < 10; ++j ) {
            index = m_qtModel->index( i, j, QModelIndex() );
            QString val = QString( "%1 %2" ).arg( i ).arg( j );
            m_qtModel->setData( index, QVariant( val ) );
        }
    }
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar() ->addMenu( tr( "&File" ) );
    m_fileMenu->addAction( m_quitAct );

    m_editMenu = menuBar() ->addMenu( tr( "&Edit" ) );
    m_editMenu->addAction( m_insertRowAct );
    m_editMenu->addAction( m_deleteRowAct );
    m_editMenu->addAction( m_insertColAct );
    m_editMenu->addAction( m_deleteColAct );
}

void MainWindow::createActions()
{
    m_quitAct = new QAction( tr( "&Quit" ), this );
    m_quitAct->setShortcut( tr( "Ctrl+Q" ) );
    connect( m_quitAct, SIGNAL( triggered() ), this, SLOT( close() ) );

    m_insertRowAct = new QAction( tr( "&Insert Row" ), this );
    m_insertRowAct->setShortcut( tr( "Ctrl+N" ) );
    connect( m_insertRowAct, SIGNAL( triggered() ), this, SLOT( insertRow() ) );

    m_deleteRowAct = new QAction( tr( "&Delete Row" ), this );
    m_deleteRowAct->setShortcut( tr( "Ctrl+D" ) );
    connect( m_deleteRowAct, SIGNAL( triggered() ), this, SLOT( deleteRow() ) );

    m_insertColAct = new QAction( tr( "&Insert Column" ), this );
    m_insertColAct->setShortcut( tr( "Ctrl+K" ) );
    connect( m_insertColAct, SIGNAL( triggered() ), this, SLOT( insertCol() ) );

    m_deleteColAct = new QAction( tr( "&Delete Column" ), this );
    m_deleteColAct->setShortcut( tr( "Ctrl+L" ) );
    connect( m_deleteColAct, SIGNAL( triggered() ), this, SLOT( deleteCol() ) );
}

void MainWindow::insertRow()
{
    int rowIndex = m_table->currentCell()->rowIndex();
    // int colIndex = table->currentCell() ->columnIndex();
    if (rowIndex < 0)
        return;

    m_table->insertRow( rowIndex );
}

void MainWindow::deleteRow()
{
    const QicsCell * cell = m_table->currentCell();
    m_table->deleteRow( cell->rowIndex() );
}

void MainWindow::insertCol()
{
    // int rowIndex = m_table->currentCell() ->rowIndex();
    int colIndex = m_table->currentCell() ->columnIndex();
    if (colIndex < 0)
        return;

    m_table->insertColumn( colIndex );
}

void MainWindow::deleteCol()
{
    const QicsCell * cell = m_table->currentCell();
    m_table->deleteColumn( cell->columnIndex() );
}

void MainWindow::columnHeader_doubleClicked (int row, int col, int button, const QPoint &pos)
{
    Q_UNUSED(row);
    Q_UNUSED(pos);
    if ( button == Qt::LeftButton ) {
        if ( ( m_sortedByCol == col ) && ( m_sortOrder == Qics::Ascending ) )
            m_sortOrder = Qics::Descending;
        else
            m_sortOrder = Qics::Ascending;

        m_sortedByCol = col;

        m_table->sortRows( m_sortedByCol,  m_sortOrder );
    }
}


