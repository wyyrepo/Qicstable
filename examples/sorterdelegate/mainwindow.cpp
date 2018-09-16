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
#include "customsorterdelegate.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
        m_sortedByCol( -1 ),
        m_sortOrder( Qics::Ascending )
{
    setWindowTitle(tr("Sorter Delegate Example"));

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
    m_table->setVisibleRows(10);
    m_table->setVisibleColumns(4);

    connect( &(m_table->columnHeaderRef()), SIGNAL( doubleClicked (int, int, int, const QPoint& ) ),
        this, SLOT( columnHeader_doubleClicked (int, int, int, const QPoint& ) ) );

    QicsColumnHeader *ch = m_table->columnHeader();
    ch->setAlignment(Qt::AlignCenter);
    ch->cellRef(0,0).setLabel("INTEGER");
    ch->cellRef(0,1).setLabel("DOUBLE");
    ch->cellRef(0,2).setLabel("HEX");
    ch->cellRef(0,3).setLabel("STRING");
    delete ch;

    m_table->setRowsSortingMode(Qics::QicsStableSort);
    m_table->setSorterDelegate(new CustomSorterDelegate()); // set CustomSorterDelegate as default

    setCentralWidget( m_table );
}

void MainWindow::createQtModel()
{
    m_qtModel = new QStandardItemModel( 40, 4, 0 );
    QModelIndex index;

    for ( int i = 0; i < 4; ++i ) {
        for ( int j = 0; j < 40; ++j ) {
            switch (i)
            {
            case 0:
                // integer data
                index = m_qtModel->index(j, i, QModelIndex());
                m_qtModel->setData(index, i+j+1, Qt::DisplayRole);
                m_qtModel->setData(index, "INTEGER", Qt::UserRole);
                break;
            case 1:
                // double data
                index = m_qtModel->index(j, i, QModelIndex());
                m_qtModel->setData(index, (i+j+1)/3.14, Qt::DisplayRole);
                m_qtModel->setData(index, "DOUBLE", Qt::UserRole);
                break;
            case 2:
                // hex data
                index = m_qtModel->index(j, i, QModelIndex());
                m_qtModel->setData(index, "0x" + QString::number((i+j+2)*(i+4), 16), Qt::DisplayRole);
                m_qtModel->setData(index, "HEX", Qt::UserRole);
                break;
            case 3:
                // string data
                index = m_qtModel->index(j, i, QModelIndex());
                m_qtModel->setData(index, QString("Cell: %1:%2").arg(j+3).arg(i), Qt::DisplayRole);
                m_qtModel->setData(index, "STRING", Qt::UserRole);
                break;
            default:
                break;
            }
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
        if ( ( m_sortedByCol == col ) && ( m_sortOrder == Qics::Descending ) )
            m_sortOrder = Qics::Ascending;
        else
            m_sortOrder = Qics::Descending;

        m_sortedByCol = col;

        m_table->sortRows( m_sortedByCol,  m_sortOrder );
    }
}


