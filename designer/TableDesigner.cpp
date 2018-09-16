/*********************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#include "TableDesigner.h"

#include <QMenu>

#include <QicsTable.h>
#include "CellDesigner.h"


TableDesigner::TableDesigner(QicsTable* table, QWidget* parent)
    : QDialog(parent)
{
    // Cache real table
    m_designersTable = table;

    // Create new table with same Data model
    m_table = new QicsTable(m_designersTable->dataModel(), this);

    //Sync up current properties
    QString xml = m_designersTable->configurationToXml();
    m_table->configureFromXml( xml );
    m_table->update();

    //Detect clicks for popup
    connect( m_table, SIGNAL(clicked(int, int, int, const QPoint)),
        this, SLOT(handleClicked(int, int, int, const QPoint)));

    //Ok Button
    m_okButton = new QPushButton(this);
    m_okButton->setText("OK");
    connect( m_okButton, SIGNAL(clicked()), SLOT(accept()));
    connect( m_okButton, SIGNAL(clicked()), SLOT(saveChanges()));

    //Cancel Button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText("Cancel");
    connect( m_cancelButton, SIGNAL(clicked()), SLOT(reject()));

    // Set up UI
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addStretch(1);
    btnLayout->addWidget(m_okButton);
    btnLayout->addWidget(m_cancelButton);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_table);
    layout->addLayout(btnLayout);
    setLayout(layout);

    //Create Actions
    m_popup = new QMenu(m_table);

    QAction* action = new QAction(this);
    action->setText("Configure Default Cell Properties...");
    m_popup->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(configureDefaultCells()));

    m_popup->addSeparator();

    QMenu* rowMenu = m_popup->addMenu("Configure Row Cell Properties");

    action = new QAction(this);
    action->setText("Model Based Properties..");
    rowMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(configureModelRowCells()));

    action = new QAction(this);
    action->setText("Visual Based Properties..");
    rowMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(configureVisualRowCells()));

    QMenu* clearRowMenu = m_popup->addMenu("Clear Row Cell Properties");

    action = new QAction(this);
    action->setText("Model Based Properties...");
    clearRowMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(clearModelRowCells()));

    action = new QAction(this);
    action->setText("Visual Based Properties...");
    clearRowMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(clearVisualRowCells()));

    m_popup->addSeparator();

    QMenu* colMenu = m_popup->addMenu("Configure Column Cell Properties");

    action = new QAction(this);
    action->setText("Model Based Properties");
    colMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(configureModelColumnCells()));

    action = new QAction(this);
    action->setText("Visual Based Properties");
    colMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(configureVisualColumnCells()));

    QMenu* clearColMenu = m_popup->addMenu("Clear Column Cell Properties");

    action = new QAction(this);
    action->setText("Model Based Properties...");
    clearColMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(clearModelColumnCells()));

    action = new QAction(this);
    action->setText("Visual Based Properties...");
    clearColMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(clearVisualColumnCells()));

    m_popup->addSeparator();

    QMenu* cellMenu = m_popup->addMenu("Configure Cell Properties");

    action = new QAction(this);
    action->setText("Model Based Properties");
    cellMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(configureModelCell()));

    action = new QAction(this);
    action->setText("Visual Based Properties");
    cellMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(configureVisualCell()));

    QMenu* clearCellMenu = m_popup->addMenu("Clear Cell Properties");

    action = new QAction(this);
    action->setText("Model Based Properties...");
    clearCellMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(clearModelCell()));

    action = new QAction(this);
    action->setText("Visual Based Properties...");
    clearCellMenu->addAction(action);
    connect( action, SIGNAL(triggered()), SLOT(clearVisualCell()));
}

void TableDesigner::handleClicked(int row, int col, int button, const QPoint &pos)
{
    if(button == (int) Qt::RightButton) {
        m_lastRow = row;
        m_lastColumn = col;
        m_popup->popup(m_table->mapToGlobal(pos));

    }
}

void TableDesigner::configureDefaultCells()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Model,
        CellDesigner::Table, m_lastRow, m_lastColumn, 0);

    designer->setWindowTitle("Default Cell Settings");
    designer->exec();
    delete designer;
}

void TableDesigner::configureModelRowCells()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Model,
        CellDesigner::Row,m_lastRow, m_lastColumn, 0);

    designer->setWindowTitle(QString("Row %1 Settings").arg(m_lastRow));
    designer->exec();
    delete designer;
}

void TableDesigner::configureModelColumnCells()
{
    CellDesigner* designer = new CellDesigner(m_designersTable,m_table, CellDesigner::Model,
        CellDesigner::Column, m_lastRow, m_lastColumn, 0);

    designer->setWindowTitle(QString("Column %1 Settings").arg(m_lastColumn));
    designer->exec();
    delete designer;
}

void TableDesigner::configureModelCell()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Model,
        CellDesigner::Cell, m_lastRow, m_lastColumn, 0);

    designer->setWindowTitle(QString("Cell %1,%2 Settings").arg(m_lastRow).arg(m_lastColumn));
    designer->exec();
    delete designer;
}

void TableDesigner::clearModelRowCells()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Model,
        CellDesigner::Row, m_lastRow, m_lastColumn, 0);
    designer->clearTable();
    delete designer;
}

void TableDesigner::clearModelColumnCells()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Model,
        CellDesigner::Column, m_lastRow, m_lastColumn, 0);
    designer->clearTable();
    delete designer;
}

void TableDesigner::clearModelCell()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Model,
        CellDesigner::Cell, m_lastRow, m_lastColumn, 0);
    designer->clearTable();
    delete designer;
}

void TableDesigner::configureVisualRowCells()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Visual,
        CellDesigner::Row, m_lastRow, m_lastColumn, 0);

    designer->setWindowTitle(QString("Row %1 Settings").arg(m_lastRow));
    designer->exec();
    delete designer;
}

void TableDesigner::configureVisualColumnCells()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Visual,
        CellDesigner::Column, m_lastRow, m_lastColumn, 0);

    designer->setWindowTitle(QString("Column %1 Settings").arg(m_lastColumn));
    designer->exec();
    delete designer;
}

void TableDesigner::configureVisualCell()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Visual,
        CellDesigner::Cell, m_lastRow, m_lastColumn, 0);

    designer->setWindowTitle(QString("Cell %1,%2 Settings").arg(m_lastRow).arg(m_lastColumn));
    designer->exec();
    delete designer;
}

void TableDesigner::clearVisualRowCells()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Visual,
        CellDesigner::Column, m_lastRow, m_lastColumn, 0);
    designer->clearTable();
    delete designer;
}

void TableDesigner::clearVisualColumnCells()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Visual,
        CellDesigner::Column, m_lastRow, m_lastColumn, 0);
    designer->clearTable();
    delete designer;
}

void TableDesigner::clearVisualCell()
{
    CellDesigner* designer = new CellDesigner(m_designersTable, m_table, CellDesigner::Visual,
        CellDesigner::Column, m_lastRow, m_lastColumn, 0);
    designer->clearTable();
    delete designer;
}

void TableDesigner::saveChanges()
{
    //Sync up table with new properties
    QString xml = m_table->configurationToXml();
    m_designersTable->configureFromXml( xml );
    m_designersTable->update();
}


