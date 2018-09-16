/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "consultant.h"

#include <QFileDialog>
#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>

#include <QicsColumnHeader.h>
#include <QicsDateTimeCellDisplay.h>
#include <QicsCell.h>
#include <QicsColumn.h>
#include <QicsDataItemFormatter.h>
#include <QicsTable.h>
#include <QicsDataModelDefault.h>
#include <QicsComboCellDisplay.h>
#include <QicsCheckCellDisplay.h>

const int NUM_COLUMNS = 7;

/*
* SSNValidator Methods
*/

QValidator::State SSNValidator::validate( QString& text, int& pos ) const
{
    /* An empty string could be made acceptable */
    if(text.isNull())
        return Intermediate;

    /* Protect against spurious calls to validate() */
    if( pos > text.length() )
        return Intermediate;

    /* Anything but decimal digits and dashes is invalid. We only need
    * to check the character at the cursor positions. This speeds
    * things up massively.
    */
    if( ( pos-1 >= 0 ) && !text[pos-1].isDigit() && ( text[pos-1] != '-' ) )
        return Invalid;

    /* If the characters entered so far are valid, but the string
    * contains less than ten digits, it could be made acceptable, but
    * is not yet.
    */
    int numDigits = text.length();
    int numDashes = 0;

    for (int i=0; i < text.length(); ++i) {
        if(text.at(i) == QChar('-')) {
            --numDigits;
            ++numDashes;
        }
    }

    if( numDigits < 9 )
        return Intermediate;

    /* More than nine digits is always invalid */
    if( numDigits > 9 )
        return Invalid;

    if (numDashes != 0)
        return Intermediate;

    /* numDashes = 0 and numDigits = 9 */
    return Acceptable;
}

void SSNValidator::fixup( QString& text ) const
{
    QString inString = text;
    int inLength = inString.length();

    for ( int i=inLength; i > 0; --i) {
        if( inString[i] == '-' ) {
            text.remove(i,1);
            qDebug ("Removing dash. \n");
        }
    }
}

////////////////////////////////////////////////////////////////////////

/*
* The main Consult class
*/

Consultant::Consultant(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Consultant Example"));

    // create the data model
    m_dataModel = new QicsDataModelDefault(0,NUM_COLUMNS);

    // create the table, using the data model we created above
    m_table = new QicsTable(m_dataModel, this);

    setCentralWidget(m_table);

    // customize the column header to make things look pretty
    m_table->columnHeaderRef().setAlignment(Qt::AlignCenter);

    m_table->columnHeaderRef().cellRef(0,0).setLabel("First Name");
    m_table->columnHeaderRef().cellRef(0,1).setLabel("Last Name");
    m_table->columnHeaderRef().cellRef(0,2).setLabel("S.S. Number");
    m_table->columnHeaderRef().cellRef(0,3).setLabel("Date of Birth");
    m_table->columnHeaderRef().cellRef(0,4).setLabel("Office");
    m_table->columnHeaderRef().cellRef(0,5).setLabel("Sec. Clearance");
    m_table->columnHeaderRef().cellRef(0,6).setLabel("Billable Rate");

    //Add a validator to the SSN to insure a consistant display
    //behaviour and error checking.
    SSNValidator* ssnValidator = new SSNValidator();
    m_table->columnRef(2).setValidator(ssnValidator);

    //Format the combo box for the office column
    QicsComboCellDisplay *officeComboDisplay = new QicsComboCellDisplay(m_table);
    officeComboDisplay->addItem("Boston");
    officeComboDisplay->addItem("Cambridge");
    officeComboDisplay->addItem("Helsinki");
    officeComboDisplay->setAddValueToList(false);
    m_table->columnRef(4).setDisplayer(officeComboDisplay);

    // Security clearance check box
    QicsCheckCellDisplay *columnHeaderCheckDisp = new QicsCheckCellDisplay(m_table);
    m_table->columnRef(5).setDisplayer(columnHeaderCheckDisp);

    // Creating date time displayer
    QicsDateTimeCellDisplay* dt = new QicsDateTimeCellDisplay(m_table);
    dt->setDisplayFormat("dd/MM/yyyy");

    m_table->columnRef(3).setDisplayer(dt);

    //Format the billable rate column with a dollar sign, decimal point,
    //and the appropriate cents digits.
    QicsDataItemSprintfFormatter *brFormatter = new QicsDataItemSprintfFormatter();
    brFormatter->addFormatString(QicsDataItem_Float, "$%.2f");
    brFormatter->addFormatString(QicsDataItem_Int, "$%d.00");
    m_table->columnRef(6).setFormatter(brFormatter);
    m_table->columnRef(6).setValidator(new QDoubleValidator(m_table));

    // Add a title widget to the top of the table
    QLabel *label = new QLabel(tr("Consultants"), m_table);
    label->setAlignment(Qt::AlignCenter);
    m_table->setTopTitleWidget(label);

    //Set column widths
    for (int i = 0; i < NUM_COLUMNS; ++i)
        m_table->columnRef(i).setWidthInChars(7);

    m_table->setVisibleColumns(NUM_COLUMNS);

    //Menu Bar
    QMenu *filemenu = new QMenu(tr("File"),this);

    QAction *save = filemenu->addAction(tr("Save..."));
    connect(save,SIGNAL(triggered()),this,SLOT ( fileSave()));

    QAction *open = filemenu->addAction(tr("Open..."));
    connect(open,SIGNAL(triggered()),this,SLOT ( fileOpen()));

    QAction *quit = filemenu->addAction( tr("Quit") );
    connect(quit,SIGNAL(triggered()),qApp,SLOT(quit()));

    QMenu *toolsmenu = new QMenu("Tools",this);

    QAction *addConsultant = toolsmenu->addAction(tr("Add Consultant"));
    connect(addConsultant,SIGNAL(triggered()),this,SLOT(addRow()));

    QAction *deleteConsultant = toolsmenu->addAction(tr("Delete Consultant"));
    connect(deleteConsultant,SIGNAL(triggered()),this,SLOT(deleteRow()));

    QAction *sortAscending = toolsmenu->addAction(tr("Sort Ascending"));
    connect(sortAscending,SIGNAL(triggered()),this,SLOT(sortAscending()));

    QAction *sortDescending = toolsmenu->addAction(tr("Sort Descending"));
    connect(sortDescending,SIGNAL(triggered()),this,SLOT(sortDescending()));

    // Help
    QMenu* help = new QMenu(tr("&Help"), this);
    help->addAction(QIcon(":/Resources/icon16_help.png"), tr("About..."), this, SLOT(about()), Qt::CTRL+Qt::Key_H);
    menuBar()->addMenu(help);

    QMenuBar *menubar = menuBar();
    menubar->addMenu(filemenu);
    menubar->addMenu(toolsmenu);
    menubar->addMenu(help);

    //Connection for autosort
    connect(&(m_table->columnHeaderRef()), SIGNAL(doubleClicked(int, int, int, const QPoint&)),
        this, SLOT(autoSort(int, int, int, const QPoint&)));

    //Reset autoSort sanity vars
    m_autoSortCol = -1;
    m_autoSortOrder = Qics::Descending;
}

void Consultant::fileSave()
{
    QString outFileName = QFileDialog::getSaveFileName(this, tr("Choose a file"), "./",
        tr("Serialized data files (*.dat)"));

    QFile file(outFileName);

    if(!file.open(QIODevice::WriteOnly)) {
        qWarning("error opening file for writing...");
        return;
    }

    QDataStream ds(&file);

    int nrows = m_table->dataModel()->numRows();
    int ncols = m_table->dataModel()->numColumns();

    ds << nrows << ncols;

    for(int i = 0;i < nrows; ++i) {
        for(int j = 0;j < ncols; ++j) {
            const QicsDataItem* itm = m_table->dataModel()->item(i,j);
            if(itm)
                itm->encode(ds);
            else {
                // if item is NULL creating empty string and serializing it!
                QicsDataString str;
                str.encode(ds);
            }
        }
    }
    file.close();
}

void Consultant::fileOpen()
{

    QString inFileName = QFileDialog::getOpenFileName(this, tr("Choose a filename"), "./",
        tr("Serialized data files (*.dat)"));

    if (inFileName.isEmpty())
        return;

    QFile file(inFileName);

    if(!file.open(QIODevice::ReadOnly)) {
        qWarning("error opening file for reading...");
        return;
    }

    QDataStream ds(&file);

    int rows;
    int columns;

    ds>>rows;
    ds>>columns;

    m_table->setIgnoreModelSizeChanges(true);
    m_table->setRepaintBehavior(Qics::RepaintOff);

    m_table->dataModel()->clearModel();
    m_table->dataModel()->addRows(rows);
    m_table->dataModel()->addColumns(columns);

    m_table->clearSelectionList();
    for(int i = 0;i<rows;i++) {
        for(int j = 0;j<columns;j++) {
            QicsDataItem *it = QicsDataItem::decode(ds);
            if(it)
                m_table->dataModel()->setItem(i,j, *it);
        }
    }
    m_table->setIgnoreModelSizeChanges(false);
    m_table->setRepaintBehavior(Qics::RepaintOn);

    file.close();
}

void Consultant::addRow()
{
    m_table->addRows(1);
}

void Consultant::deleteRow()
{
    m_table->deleteRow(m_table->currentCell()->rowIndex());
}

void Consultant::sortAscending()
{
    m_table->sortRows(m_table->currentCell()->columnIndex(), Qics::Ascending);
}

void Consultant::sortDescending()
{
    m_table->sortRows(m_table->currentCell()->columnIndex(), Qics::Descending);
}

void Consultant::autoSort(int , int col, int , const QPoint &)
{
    if (m_autoSortCol == col) {
        if (m_autoSortOrder == Qics::Ascending)
            m_autoSortOrder = Qics::Descending;
        else
            m_autoSortOrder = Qics::Ascending;
    }
    else {
        m_autoSortCol = col;
        m_autoSortOrder = Qics::Ascending;
    }

    m_table->sortRows(col, m_autoSortOrder);
}

void Consultant::about()
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
        "This example represents Consultant based on QicsTable."
        "<p>"
        "<b>HOW TO USE</b><br>"
        "TOOD: Need user instruction"
        "</td></table>"
        );
    helpDialog.setWindowTitle( tr("About Consultant Example") );
    helpDialog.setIcon( QMessageBox::NoIcon );
    helpDialog.exec();
}


