/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

//----------------------------------------------------------------------------
//   Object that wraps a QicsTable and does an "Excel-lent" spreadsheet.
//   Note that QSA support can be added in by modfiying the .pro file
//----------------------------------------------------------------------------
//
#include "spreadsheet.h"

#include <QMenuBar>
#include <QApplication>
#include <QToolBar>
#include <QComboBox>
#include <QFontDatabase>
#include <QToolButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QStatusBar>
#include <QMessageBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QPrintDialog>
#include <QPrinter>

#ifdef QSA
// QSA
#include <qsproject.h>
#include <qsscript.h>
#include <qsworkbench.h>
#include <qsinterpreter.h>
#endif

#include <QicsDataModelDefault.h>
#include <QicsTable.h>
#include <QicsRowHeader.h>
#include <QicsColumnHeader.h>
#include <QicsColumn.h>
#include <QicsSelection.h>
#include <QicsMainGrid.h>
#include <QicsCellRegion.h>
#include <QicsCell.h>
#include <QicsRegionalAttributeController.h>
#include "QicsDataItemCurrencyFormatter.h"

#include "editcopy.xpm"
#include "editcut.xpm"
#include "editpaste.xpm"
#include "filenew.xpm"
#include "fileopen.xpm"
#include "filesave.xpm"
#include "fontcolor.xpm"
#include "sortasc.xpm"
#include "sortdesc.xpm"
#include "textbold.xpm"
#include "textitalic.xpm"
#include "textunder.xpm"
#include "textleft.xpm"
#include "textcenter.xpm"
#include "textright.xpm"

/*---------------------------------------------------------------------------
*  Function:  Spreadsheet constructor
*
*  Description:
*    Constructor the Spreadsheet object.  Creates a QMainWindow with a menu
*    and toolbar, a QicsTable, and connects the appropriate slots to load
*    and manipulate data.
*/

Spreadsheet::Spreadsheet(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Excellent Spreadsheet Example"));

    /*
    *  Pulldown Menu
    */
    QMenuBar *mainMenu = menuBar();

    // File
    QMenu* file = mainMenu->addMenu(tr("&File"));
    file->addAction(tr("&New"), this, SLOT(fileNew()), QKeySequence("Ctrl+N"));
    file->addAction(tr("&Open"), this, SLOT(openFile()), QKeySequence("Ctrl+O"));
    file->addAction(tr("&Save"), this, SLOT(saveFile()), QKeySequence("Ctrl+S"));
    file->addAction(tr("S&ave As..."), this, SLOT(saveFileAs()));
    file->addSeparator();
    file->addAction("P&rint", this, SLOT(print()), QKeySequence("Ctrl+P"));
    file->addSeparator();
    file->addAction(tr("E&xit"), qApp, SLOT(closeAllWindows()), QKeySequence("Ctrl+x"));

    // Edit
    QMenu* edit = mainMenu->addMenu(tr("&Edit"));
    edit->addAction(tr("&Copy"), this, SLOT(copy()),QKeySequence("Ctrl+C"));
    edit->addAction(tr("&Paste"), this, SLOT(paste()),QKeySequence("Ctrl+V"));

    // View
    //mainMenu->addMenu(tr("&View"));

    // Table
    QMenu* tablem = mainMenu->addMenu(tr("&Table"));
    tablem->addAction(tr("Insert &Row"), this, SLOT(insertRow()));
    tablem->addAction(tr("Insert &Column"), this, SLOT(insertColumn()));
    tablem->addSeparator();
    tablem->addAction(tr("Delete Row(s)"), this, SLOT(deleteRow()));
    tablem->addAction(tr("Delete Column(s)"), this, SLOT(deleteColumn()));

    // Format
    QMenu* format = mainMenu->addMenu(tr("F&ormat"));

    format->addMenu(tr("&Row"));

    format->addSeparator();

    format->addAction(tr("&Font..."), this, SLOT(formatFont()));
    format->addAction(tr("Fore&ground..."), this,
        SLOT(formatForegroundColor()));
    format->addAction(tr("&Background..."), this,
        SLOT(formatBackgroundColor()));

    // Tools
    QMenu* tools = mainMenu->addMenu(tr("Tools"));
    tools->addAction(tr("&Script Editor"), this, SLOT(showQSAWorkbench()));
    tools->addAction(tr("Script &Test"), this, SLOT(testScripts()));
    tools->addAction(tr("Save Scripts"), this, SLOT(saveScripts()));

    // Data
    QMenu* data = mainMenu->addMenu(tr("&Data"));
    data->addAction(tr("Sort &Ascending"), this, SLOT(sortAscending()));
    data->addAction(tr("Sort &Descending"), this, SLOT(sortDescending()));

    // Spans
    QMenu* spans = mainMenu->addMenu(tr("&Spans"));
    spans->addAction(tr("Add Span"), this, SLOT(addSpan()));
    spans->addAction(tr("Remove Span"), this, SLOT(removeSpan()));
    /*
    * Toolbars
    */

    // File operation toolbar
    QToolBar* fileTools = new QToolBar("File tools",this);
    fileTools->addAction(QIcon(QPixmap(filenew)),tr("&New"), this, SLOT(fileNew()));
    fileTools->addAction(QIcon(QPixmap(fileopen)),tr("&Open"), this, SLOT(openFile()));
    fileTools->addAction(QIcon(QPixmap(filesave)),tr("&Save"), this, SLOT(saveFile()));
    fileTools->addSeparator();

    // Edit operations
    fileTools->addAction(QIcon(QPixmap(editcut)),tr("&Cut"), this, SLOT(cut()));
    fileTools->addAction(QIcon(QPixmap(editcopy)),tr("&Copy"), this, SLOT(copy()));
    fileTools->addAction(QIcon(QPixmap(editpaste)),tr("Paste"), this, SLOT(paste()));
    fileTools->addSeparator();

    // Sort
    fileTools->addAction(QIcon(QPixmap(sortasc_xpm)),tr("Sort Ascending"), this, SLOT(sortAscending()));
    fileTools->addAction(QIcon(QPixmap(sortdesc_xpm)),tr("Sort Descending"), this, SLOT(sortDescending()));
    fileTools->addSeparator();
    addToolBar(fileTools);
    // Font name and point size

    QToolBar* fontTools = new QToolBar("Font tools",this);

    m_fontFamilyCombo = new QComboBox(fontTools);
    m_fontSizeCombo = new QComboBox(fontTools);

    fontTools->addWidget(m_fontFamilyCombo);
    fontTools->addWidget(m_fontSizeCombo);

    connect(m_fontFamilyCombo, SIGNAL(activated(int)), SLOT(selectFont(int)));
    connect(m_fontSizeCombo, SIGNAL(activated(int)), SLOT(selectFontSize(int)));

    QFontDatabase fdb;
    QStringList families = fdb.families();
    m_fontFamilyCombo->addItems(families);

    // Font format operations

    m_boldAction = new QAction(QIcon(QPixmap(textbold)),tr("Bold"), this);
    fontTools->addAction(m_boldAction);
    m_boldAction->setCheckable(true);
    connect(m_boldAction,SIGNAL(triggered()),this,SLOT(textBold()));

    m_italicAction = new QAction(QIcon(QPixmap(textitalic)),tr("Italic"), this);
    fontTools->addAction(m_italicAction);
    m_italicAction->setCheckable(true);
    connect(m_italicAction,SIGNAL(triggered()),this,SLOT(textItalic()));

    m_underlineAction = new QAction(QIcon(QPixmap(textunder)),tr("Underline"), this);
    fontTools->addAction(m_underlineAction);
    m_underlineAction->setCheckable(true);
    connect(m_underlineAction,SIGNAL(triggered()),this,SLOT(textUnderline()));

    fontTools->addSeparator();

    // Text Alignment

    m_textLeftAction = new QAction(QIcon(QPixmap(textleft)),tr("Align Left"), this);
    fontTools->addAction(m_textLeftAction);
    m_textLeftAction->setCheckable(true);
    connect(m_textLeftAction,SIGNAL(triggered()),this,SLOT(textAlignLeft()));

    m_textCenterAction = new QAction(QIcon(QPixmap(textcenter)),tr("Align Center"), this);
    fontTools->addAction(m_textCenterAction);
    m_textCenterAction->setCheckable(true);
    connect(m_textCenterAction,SIGNAL(triggered()),this,SLOT(textAlignCenter()));

    m_textRightAction = new QAction(QIcon(QPixmap(textright)),tr("Align Right"), this);
    fontTools->addAction(m_textRightAction);
    m_textRightAction->setCheckable(true);
    connect(m_textRightAction,SIGNAL(triggered()),this,SLOT(textAlignRight()));

    fontTools->addSeparator();

    // Formatters
    QToolButton* currencyTB = new QToolButton(fontTools);
    currencyTB->setText("$");
    connect(currencyTB, SIGNAL(clicked()), this, SLOT(setCurrencyFormatter()));

    fontTools->addWidget(currencyTB);
    fontTools->addSeparator();

    // Colors
    fontTools->addAction(
        QPixmap(fontcolor_xpm), "Font Color", this, SLOT(formatForegroundColor()));

    fontTools->addAction(
        QIcon("./images/fillcolor.png"), "Fill Color", this, SLOT(formatBackgroundColor()));

    addToolBar(fontTools);


    /*
    *  Table
    */

    QWidget *vbox = new QWidget(this);
    QVBoxLayout *vboxLayout = new QVBoxLayout();
    QWidget *cellrow = new QWidget(vbox);
    QHBoxLayout *cellrowLayout = new QHBoxLayout();
    cellrowLayout->setMargin(2);
    cellrowLayout->addWidget(new QLabel(tr("Cell Value = ")));

    m_cellLineEdit = new QLineEdit();
    cellrowLayout->addWidget(m_cellLineEdit);
    cellrow->setLayout(cellrowLayout);

    connect(m_cellLineEdit, SIGNAL(returnPressed()), SLOT(cellLineUpdate()));

    int lastRow = 1000;
    int lastCol = 1000;

    // create the data model
    m_dm = new QicsDataModelDefault(lastRow, lastCol);

    // create the table, using the data model we created above
    m_table = new QicsTable(m_dm);

    QicsRegionalAttributeController controller;
    m_table->setExternalAttributeController(controller);

    vboxLayout->addWidget(cellrow);
    vboxLayout->addWidget(m_table);
    vbox->setLayout(vboxLayout);

    connect(m_table, SIGNAL(selectionListChanged(bool)), SLOT(updateSelection(bool)));

    // Set some visual resources on the table
    m_table->rowHeaderRef().setAlignment(Qt::AlignHCenter);
    m_table->columnHeaderRef().setAlignment(Qt::AlignHCenter);
    m_table->rowHeaderRef().column(0)->setWidthInChars(3);
    m_table->mainGridRef().setBackgroundColor(Qt::white);

    // Setting column labels
    for (int i = 0; i < lastRow; ++i) {
        QString text;
        encodePosition(text, i);
        m_table->columnHeaderRef().cellRef(0,i).setLabel(text);
    }

    // Set row sorting mode to non-distractive sord
    m_table->setRowsSortingMode( Qics::QicsStableSort );

    m_table->setNavigatorAllowed(true);
    m_table->setWideKeyActions();

    m_table->columnHeaderRef().setAllowUserMove(true);
    m_table->rowHeaderRef().setAllowUserMove(true);

    setFontCombos(m_table->font());

    setCentralWidget(vbox);

    statusBar()->showMessage(tr("Ready"), -1);

    m_fileName = QString();
#ifdef QSA
    // QSA Initialization
    m_project = new QSProject( this, "spreadsheet_project" );
    m_project->addObject(this);
    QSInterpreter* interpreter = m_project->interpreter();

    m_project->load( "Spreadsheet.qsa" );
#endif

    connect(m_table,SIGNAL(rowResized(int,int,int)),this,SLOT(resized()));
    connect(m_table,SIGNAL(columnResized(int,int,int)),this,SLOT(resized()));
}

void Spreadsheet::encodePosition(QString &text, int column)
{
    text.clear();
    QChar letter;

    letter = 'A' + column%26;
    text.insert(0, letter);

    if (column >= 26) {
        column = column/26;
        do {
            column--;
            QChar letter('A' + column%26);
            text.insert(0, letter);
            column = column/26;
        }
        while(column>0);
    }
}

#ifdef QSA
// Prototype QSA support methods
void Spreadsheet::saveScripts()
{
    project->save("Spreadsheet.qsa");
}

void Spreadsheet::testScripts()
{
    QStringList list = project->interpreter()->functions();

    for (QStringList::Iterator it = list.begin(); it != list.end(); ++it)
        cout << *it << endl;

    // Hardcoded here, call function "sum" with specified args

    QList<QVariant> args;

    args.append(1);
    args.append(1);

    // call the function
    project->interpreter()->call("sum", args); //QValueList<QVariant>() );

    QSScript* script = project->script("Spreadsheet.qs");

    if (script)
        cout << script->code() << endl;
    else
        cout << "No script by that name" << endl;
}

static QSWorkbench *spreadsheet_ide = 0;

void Spreadsheet::showQSAWorkbench()
{
    if (!spreadsheet_ide )
        spreadsheet_ide = new QSWorkbench( project, this, "qside" );

    spreadsheet_ide->open();
}

#else

// Warn user that there is no QSA support compiled in

void Spreadsheet::showQSAWorkbench()
{
    QMessageBox::warning(this, tr("No QSA support!"),
        tr("Sorry, no QSA support compiled in"), QMessageBox::Ok, QMessageBox::NoButton);
}

void Spreadsheet::testScripts()
{
    QMessageBox::warning(this, tr("No QSA support!"),
        tr("Sorry, no QSA support compiled in"), QMessageBox::Ok, QMessageBox::NoButton);
}

void Spreadsheet::saveScripts()
{
    QMessageBox::warning(this, tr("No QSA support!"),
        tr("Sorry, no QSA support compiled in"), QMessageBox::Ok, QMessageBox::NoButton);
}

#endif

/*---------------------------------------------------------------------------
*  Function:  setCurrencyFormatter
*
*  Description:
*    Set a formatter on the selected cells do display the values
*    in dollar value ($ XX.YY)
*/

void Spreadsheet::setCurrencyFormatter()
{
    getSelectedRegion();

    if ( (m_startSelectedRow > -1 ) && (m_startSelectedColumn > -1) ) {

        QicsDataItemCurrencyFormatter* currency = new QicsDataItemCurrencyFormatter();

        for (int i = m_startSelectedRow; i < m_endSelectedRow; ++i)
            for (int j = m_startSelectedColumn; j < m_endSelectedColumn; ++j)
                m_table->cellRef(i,j).setFormatter(currency);

    }
}

/*---------------------------------------------------------------------------
*  Function:  getCellValue
*
*  Description:
*    Return the value of cell[row][col]
*/

QString Spreadsheet::getCellValue(int row, int col)
{
    return m_dm->itemString(row, col);
}

/*---------------------------------------------------------------------------
*  Function:  setCellValue
*
*  Description:
*    Set cell[row][col] = value
*/

void Spreadsheet::setCellValue(int row, int col, int value)
{
    m_dm->setItem(row, col, value);
}

/*---------------------------------------------------------------------------
*  Function:  cellLineUpdate()
*
*  Description:
*    When a value is entered in the cellLineEdit and a cell is selected in
*    the table, store the data element as a new data item.
*/

void Spreadsheet::cellLineUpdate()
{
    getSelectedRegion();

    if ( (m_startSelectedRow > -1 ) && (m_startSelectedColumn > -1) ) {
        // OK, we got a selected cell and some data.  Time to
        // put it into the data model

        // temp var because they are easier to type
        int row = m_startSelectedRow;
        int col = m_startSelectedColumn;

        bool ok;

        // Try to convert to int
        int i = m_cellLineEdit->text().toInt(&ok);

        if (ok) {
            m_dm->setItem(row, col, i);
            return;
        }

        // Try to convert to float
        double f = m_cellLineEdit->text().toDouble(&ok);

        if (ok) {
            m_dm->setItem(row, col, f);
            return;
        }
        m_dm->setItem(row, col, m_cellLineEdit->text());
    }
}

void Spreadsheet::selectFont(int combo)
{
    Q_UNUSED(combo);

    QString family = m_fontFamilyCombo->currentText();

    // If cells are selected, set their display font

    getSelectedRegion();

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setFontFamily(family, true);
}

void Spreadsheet::selectFontSize(int combo)
{
    Q_UNUSED(combo);

    int size = m_fontSizeCombo->currentText().toInt();

    // If cells are selected, set their display font

    getSelectedRegion();

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setFontSize(size, true);
}

/*---------------------------------------------------------------------------
*  Function:  formatBackgroundColor
*
*  Description:
*    Bring up the color dialog, select a color, apply it to the selected
*    region as the background color
*/

void Spreadsheet::formatBackgroundColor()
{
    QColor color = QColorDialog::getColor();

    if (! color.isValid())
        return;

    // If cells are selected, set their display font

    getSelectedRegion();

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setBackgroundColor(color);
}

/*---------------------------------------------------------------------------
*  Function:  formatForegroundColor
*
*  Description:
*    Bring up the color dialog, select a color, apply it to the selected
*    region as the foreground color
*/

void Spreadsheet::formatForegroundColor()
{
    QColor color = QColorDialog::getColor();

    if (!color.isValid())
        return;

    // If cells are selected, set their display font

    getSelectedRegion();

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setForegroundColor(color);
}

/*---------------------------------------------------------------------------
*  Function:  formatFont
*
*  Description:
*    Bring up the font dialog, select a font, apply it to the selected region
*/

void Spreadsheet::formatFont()
{
    bool ok;

    // Select a font

    QFont font = QFontDialog::getFont(&ok, QFont("Helvetica", 10), this);

    if (!ok )
        return;

    // If cells are selected, set their display font

    getSelectedRegion();

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setFont(font);
}

/*---------------------------------------------------------------------------
*  Function:  updateSelection
*
*  Description:
*    A cell was selected, so update the state of the toolbar pieces.
*/

void Spreadsheet::updateSelection(bool in_progress)
{
    if (in_progress)
        return;

    getSelectedRegion();


    if ( (m_startSelectedRow > -1 ) && (m_startSelectedColumn > -1) ) {
        QFont font = m_table->cellRef(m_startSelectedRow, m_startSelectedColumn).font();
        setFontCombos(font);

        if (font.bold())
            m_boldAction->setChecked(true);
        else {
            // Is on, turn off (not bold)
            m_boldAction->setChecked(false);
        }

        if (font.italic())
            m_italicAction->setChecked(true);
        else {
            // Is on, turn off (not bold)
            m_italicAction->setChecked(false);
        }

        if (font.underline())
            m_underlineAction->setChecked(true);
        else {
            // Is on, turn off (not bold)
            m_underlineAction->setChecked(false);
        }

        int alignment = m_table->cellRef(m_startSelectedRow, m_startSelectedColumn).alignment();

        switch (alignment)
        {

        case Qt::AlignLeft:
            m_textLeftAction->setChecked(true);
            m_textRightAction->setChecked(false);
            m_textCenterAction->setChecked(false);
            break;
        case Qt::AlignRight:
            m_textLeftAction->setChecked(false);
            m_textRightAction->setChecked(true);
            m_textCenterAction->setChecked(false);
            break;
        case Qt::AlignHCenter:
            m_textLeftAction->setChecked(false);
            m_textRightAction->setChecked(false);
            m_textCenterAction->setChecked(true);
            break;
        default:
            m_textLeftAction->setChecked(false);
            m_textRightAction->setChecked(false);
            m_textCenterAction->setChecked(false);
            break;
        }

        if (m_dm->item(m_startSelectedRow, m_startSelectedColumn) == NULL) {
            m_cellLineEdit->setText("");
            return;
        }

        QicsCell* cell = m_table->cell(m_startSelectedRow, m_startSelectedColumn, true);

        if (cell->dataValue()) {
            QString cellval = cell->dataValue()->string();
            m_cellLineEdit->setText(cellval);
        }
    }
}

/*---------------------------------------------------------------------------
*  Function:  insertColumn
*
*  Description:
*    Add a column to the table before the currently selected column
*/

void Spreadsheet::insertColumn()
{
    getSelectedRegion();

    if (m_startSelectedColumn > -1)
        m_table->insertColumn(m_startSelectedColumn);
}

/*---------------------------------------------------------------------------
*  Function:  insertRow
*
*  Description:
*    Add a row to the table before the currently selected row
*/

void Spreadsheet::insertRow()
{
    getSelectedRegion();

    if (m_startSelectedRow > -1)
        m_table->insertRow(m_startSelectedRow);
}

/*---------------------------------------------------------------------------
*  Function:  deleteRow
*
*  Description:
*    Deletes currently selected row
*/

void Spreadsheet::deleteRow()
{
    getSelectedRegion();

    if (m_startSelectedRow > -1)
        m_table->deleteRow(m_startSelectedRow);
}

/*---------------------------------------------------------------------------
*  Function:  deleteColumn
*
*  Description:
*    Deletes currently selected column
*/

void Spreadsheet::deleteColumn()
{
    getSelectedRegion();

    if (m_startSelectedColumn > -1)
        m_table->deleteColumn(m_startSelectedColumn);
}

/*---------------------------------------------------------------------------
*  Function:  setFontCombos
*
*  Description:
*    Given a font, set the fontFamilyCombo to show it, set the fontSizeCombo
*    to contain valid sizes for that font, and then show the correct
*    font size.
*/

void Spreadsheet::setFontCombos(const QFont& font)
{
    // Set font family
    m_fontFamilyCombo->setEditText(font.family());

    QFontDatabase fdb;
    QList<int> sizes = fdb.pointSizes(font.family());

    // Clear the sizes combo, fill it with valid sizes based in family

    m_fontSizeCombo->clear();

    if (sizes.begin() != sizes.end()) {
        for (QList<int>::Iterator pts = sizes.begin(); pts != sizes.end(); ++pts)
            m_fontSizeCombo->addItem(QString::number(*pts));
        m_fontSizeCombo->setEditText(QString::number(font.pointSize()));
    }
}

/*---------------------------------------------------------------------------
*  Function:  textAlignLeft
*
*  Description:
*    Set the alignment value of the displayed cell to AlignLeft
*/

void Spreadsheet::textAlignLeft()
{
    if ( (m_startSelectedRow < 0) && (m_startSelectedColumn < 0))
        return;

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setAlignment(Qt::AlignLeft);

    m_textRightAction->setChecked(false);
    m_textCenterAction->setChecked(false);
}

/*---------------------------------------------------------------------------
*  Function:  textAlignCenter
*
*  Description:
*    Set the alignment value of the displayed cell to AlignHCenter
*/

void Spreadsheet::textAlignCenter()
{
    if ( (m_startSelectedRow < 0) && (m_startSelectedColumn < 0))
        return;

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setAlignment(Qt::AlignHCenter);

    m_textLeftAction->setChecked(false);
    m_textRightAction->setChecked(false);
}

/*---------------------------------------------------------------------------
*  Function:  textAlignRight
*
*  Description:
*    Set the alignment value of the displayed cell to AlignRight
*/

void Spreadsheet::textAlignRight()
{
    if ( (m_startSelectedRow < 0) && (m_startSelectedColumn < 0))
        return;

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setAlignment(Qt::AlignRight);

    m_textLeftAction->setChecked(false);
    m_textCenterAction->setChecked(false);
}

/*---------------------------------------------------------------------------
*  Function:  textBold
*
*  Description:
*    Toggle the boldness state of whatever cells are selected
*/

void Spreadsheet::textBold()
{
    if ( (m_startSelectedRow < 0) && (m_startSelectedColumn < 0))
        return;

    bool on = m_boldAction->isChecked();

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setFontAttribute(QicsCellStyle::FontBold, on);
}

/*---------------------------------------------------------------------------
*  Function:  textItalic
*
*  Description:
*    Toggle the italic-ness state of whatever cells are selected
*/

void Spreadsheet::textItalic()
{
    if ( (m_startSelectedRow < 0) && (m_startSelectedColumn < 0))
        return;

    bool on = m_italicAction->isChecked();

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setFontAttribute(QicsCellStyle::FontItalic, on);
}

/*---------------------------------------------------------------------------
*  Function:  textUnderline
*
*  Description:
*    Toggle the underline-ed-ness state of whatever cells are selected
*/

void Spreadsheet::textUnderline()
{
    if ( (m_startSelectedRow < 0) && (m_startSelectedColumn < 0))
        return;

    bool on = m_underlineAction->isChecked();

    if (m_startSelectedRow > -1)
        m_table->cellRegion(m_selectedRegion)->setFontAttribute(QicsCellStyle::FontUnderline, on);
}

/*---------------------------------------------------------------------------
*  Function:  openFile
*
*  Description:
*    Protected slot, bring up a QFileDialog to select a file, then load it.
*/

void Spreadsheet::openFile()
{
    QString fname =	QFileDialog::getOpenFileName();

    if (!fname.isEmpty()) {
        m_fileName = fname;
        loadFile(m_fileName);
    }
}

/*---------------------------------------------------------------------------
*  Function:  loadFile
*
*  Description:
*     Load a Comma Delimited File into the data table
*/

void Spreadsheet::loadFile(QString fname)
{
    // Create the stream from the file, read into data model
    QFile file(fname);

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        m_dm->readASCII(stream, ',');
    }
}

/*---------------------------------------------------------------------------
*  Function:  saveFile
*
*  Description:
*    Protected slot, save data to the current file name
*/

void Spreadsheet::saveFile()
{
    if (!m_fileName.isEmpty())
        writeFile(m_fileName);
}

/*---------------------------------------------------------------------------
*  Function:  saveFileAs
*
*  Description:
*    Protected slot, bring up a QFileDialog to select a file, then save to it.
*/

void Spreadsheet::saveFileAs()
{
    QString fname =	QFileDialog::getSaveFileName();

    if (!fname.isEmpty()) {
        m_fileName = fname;
        writeFile(m_fileName);
    }
}

/*---------------------------------------------------------------------------
*  Function:  writeFile
*
*  Description:
*     Save a Comma Delimited File from the data table
*/

void Spreadsheet::writeFile(QString fname)
{
    // Create the stream from the file, read into data model
    QFile file(fname);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        m_dm->writeASCII(stream, ',', 0, 0, 10, 10);
    }
}

/*---------------------------------------------------------------------------
*  Function:  fileNew
*
*  Description:
*     Clear out all the data currently displayed in the table.
*/

void Spreadsheet::fileNew()
{
    // To be implemented
    qDebug("To be implemented");
}

/*---------------------------------------------------------------------------
*  Function:  copy
*
*  Description:
*     Calls the table's copy operation to place data into the QClipboard
*     owned by qApp.
*/

void Spreadsheet::copy()
{
    m_table->copy();
}

/*---------------------------------------------------------------------------
*  Function:  cut
*
*  Description:
*     Calls the table's cut operation to place data into the QClipboard
*     owned by qApp and remove it from the visible table area
*/

void Spreadsheet::cut()
{
    m_table->cut();
}

/*---------------------------------------------------------------------------
*  Function:  paste
*
*  Description:
*     Calls the table's paste operation to retrieve cells from the QClipboard
*     owned by qApp and paste them at the current cell.
*/

void Spreadsheet::paste()
{
    m_table->paste();
}


/*---------------------------------------------------------------------------
*  Function:  getSelectedRegion
*
*  Description:
*     Query the table, find the start and end row/column of the currently
*     selected region, if any.
*/

void Spreadsheet::getSelectedRegion()
{
    QicsSelection selection;
    QicsSelectionList *list = m_table->selectionList();

    // Get selected boundry

    if (list != NULL) {
        if (list->begin() != list->end()) {
            selection = *(list->begin());

            m_startSelectedRow = selection.anchorRow();
            m_startSelectedColumn = selection.anchorColumn();
        }
    }
    else
        m_startSelectedRow = m_startSelectedColumn = -1;  // reset flags

    delete list;

    int row = selection.endRow();
    if (row > m_dm->lastRow())
        row = m_dm->lastRow() + 1;
    else
        row++;

    int col = selection.endColumn();
    if (col > m_dm->lastColumn())
        col = m_dm->lastColumn() + 1;
    else
        col++;

    m_endSelectedRow = row;
    m_endSelectedColumn = col;

    m_selectedRegion = QicsRegion(m_startSelectedRow, m_startSelectedColumn, m_endSelectedRow-1, m_endSelectedColumn-1);
    m_selectedRegion.normalize();
}

/*---------------------------------------------------------------------------
*  Function:  sortAscending
*
*  Description:
*     Sort the table by the selected column in ascending order
*/

void Spreadsheet::sortAscending()
{
    QicsSelectionList *list = m_table->selectionList();
    if(!list)
        return;

    QicsSelectionList::const_iterator iter;
    QVector<int> selectedCols;

    for (iter = list->begin(); iter < list->end(); ++iter)
        selectedCols << (*iter).anchorColumn();

    if (selectedCols.size() <= 0) selectedCols << 0;
    m_table->sortRows(selectedCols, Qics::Ascending);
}

/*---------------------------------------------------------------------------
*  Function:  sortDescending
*
*  Description:
*     Sort the table by the selected column in descending order
*/

void Spreadsheet::sortDescending()
{
    QicsSelectionList *list = m_table->selectionList();
    if(!list)
        return;

    QicsSelectionList::const_iterator iter;
    QVector<int> selectedCols;

    for (iter = list->begin(); iter < list->end(); ++iter)
        selectedCols << (*iter).anchorColumn();

    if (selectedCols.size() <= 0) selectedCols << 0;
    m_table->sortRows(selectedCols, Qics::Descending);
}


/*!
\fn Spreadsheet::resized
*/
void Spreadsheet::resized()
{
}


/*---------------------------------------------------------------------------
*  Function:  addSpan
*
*  Description:
*     Adds span to selected cells
*/


void Spreadsheet::addSpan()
{
    QicsSelectionList *slist = m_table->selectionList();
    if(slist) {
        QicsSelectionList::const_iterator iter;

        for (iter = slist->begin(); iter != slist->end(); ++iter) {
            const QicsSelection &sel = *iter;
            int bottom_row = sel.numRows();
            int right_column = sel.numColumns();

            if(sel.bottomRow() == Qics::QicsLAST_ROW)
                bottom_row = m_table->dataModel()->numRows();
            if(sel.rightColumn() == Qics::QicsLAST_COLUMN)
                right_column = m_table->dataModel()->numColumns();

            m_table->addCellSpan(QicsSpan(sel.topRow(),sel.leftColumn(),bottom_row,right_column));
        }
    }
}

/*---------------------------------------------------------------------------
*  Function:  removeSpan
*
*  Description:
*     Removes span from selected cells
*/

void Spreadsheet::removeSpan()
{
    QicsSelectionList *slist = m_table->selectionList();
    if(slist) {
        QicsSelectionList::const_iterator iter;

        for (iter = slist->begin(); iter != slist->end(); ++iter) {
            const QicsSelection &sel = *iter;
            m_table->removeCellSpan(sel.topRow(),sel.leftColumn());
        }
    }
}

/*---------------------------------------------------------------------------
*  Function print
*
*  Description:
*     prints current table
*/

void Spreadsheet::print()
{
    QPrinter* printer = new QPrinter;
    QPrintDialog printDialog(printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        // print ...
        m_table->print( printer );
    }
}

