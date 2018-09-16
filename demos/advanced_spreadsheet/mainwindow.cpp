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

#include <QStatusBar>
#include <QMenuBar>
#include <QMdiArea>
#include <QToolBar>
#include <QMessageBox>
#ifndef QICSTABLE_GPL
#include <QPrintDialog>
#endif
#include <QColorDialog>
#include <QFileDialog>
#include <QFontComboBox>

#ifndef QICSTABLE_GPL
#include <QicsTablePrint.h>
#include <../addons/printing/QicsPrintPreviewDialog.h>
#endif
#include <QicsCellRegion.h>
#include <QicsCell.h>
#include <QicsRowHeader.h>
#include <QicsColumnHeader.h>
#include <QicsMainGrid.h>
#include <QicsHTMLExport.h>

#include <../addons/table.html/QicsHTMLExportDialog.h>

#include "spreadsheetcalculator.h"
#include "spreadsheetcelldisplay.h"
#include "spreadsheetformatter.h"
#include "childwindow.h"
#include "spreadsheetdm.h"
#include "spreadsheettable.h"
#include "finddialog.h"
#include "setupdialog.h"

#include "frozenconfigdialog.h"


#ifdef BIND_KDCHART

#include "kdchartobject.h"

#include <KDChartChart>
#include <KDChartBarDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartThreeDBarAttributes>
#include <KDChartAttributesModel>

using namespace KDChart;

#endif


#define DEFAULT_BORDER_COLOR Qt::black
#define INIT_TABLE();  SpreadsheetTable *table = qobject_cast<SpreadsheetTable*>(activeTable()); if(!table) return;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_currentSearchCell()
{
    m_fd = 0;
    m_fileName = "Untitled.dat";
    m_workspace = new QMdiArea(this);
    setCentralWidget(m_workspace);

    initMenus();
    initToolbars();

    QStatusBar* statusBar = new QStatusBar(this);
    statusBar->setSizeGripEnabled(true);
    setStatusBar(statusBar);
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("Advanced Spreadsheet Demo");
    setWindowIcon(QIcon(":/images/new.png"));
    createNewDocument();
    statusBar->hide();
    statusBar->show();
}

void MainWindow::initMenus()
{
    m_menuBar = new QMenuBar(this);

    /*
    *  ------------------ FILE MENU -----------------------------
    */
    QMenu *fileMenu = new QMenu("&File",m_menuBar);

    m_newAction = fileMenu->addAction(QIcon(":images/new.png"),"New",this,SLOT(createNewDocument()),QKeySequence("Ctrl+N"));
    m_openAction = fileMenu->addAction(QIcon(":images/open.png"),"Open",this,SLOT(openFile()),QKeySequence("Ctrl+O"));
    m_closeAction = fileMenu->addAction(QIcon(":/images/close.png"),"Close",this,SLOT(closeDocument()));
    fileMenu->addSeparator();

    m_saveAction = fileMenu->addAction(QIcon(":/images/save.png"),"Save",this,SLOT(saveFile()),QKeySequence("Ctrl+S"));
    m_saveAsAction = fileMenu->addAction(QIcon(":images/save-as.png"),"Save As...",this,SLOT(saveFileAs()));
    fileMenu->addSeparator();

#ifndef QICSTABLE_GPL
    m_printPrevAction = fileMenu->addAction(QIcon(":images/print-preview.png"),"Print with Preview...",this,SLOT(printPreview()));
    m_printAction = fileMenu->addAction(QIcon(":images/print.png"),"Print...",this,SLOT(print()));
    fileMenu->addSeparator();
#endif

    fileMenu->addAction("Exit",this,SLOT(quitProgram()));

    /*
    *  ---------------------- EDIT MENU -------------------------
    */
    QMenu *editMenu = new QMenu("&Edit",m_menuBar);
    m_cutAction = editMenu->addAction(QIcon(":/images/cut.png"),"Cut",this,SLOT(cut()),QKeySequence("Ctrl+X"));
    m_copyAction = editMenu->addAction(QIcon(":/images/copy.png"),"Copy",this,SLOT(copy()),QKeySequence("Ctrl+C"));
    m_pasteAction = editMenu->addAction(QIcon(":/images/paste.png"),"Paste",this,SLOT(paste()),QKeySequence("Ctrl+V"));
    editMenu->addSeparator();
    //selectAllAction = editMenu->addAction("Select All",this,SLOT(selectAll()),QKeySequence("Ctrl+A"));
    //editMenu->addSeparator();
    m_findAction = editMenu->addAction(QIcon(":/images/find.png"),"Find...",this,SLOT(find()),QKeySequence("Ctrl+F"));

    /*
    *  --------------------- VIEW MENU ---------------------
    */
    QMenu *viewMenu = new QMenu("&View",m_menuBar);
    viewMenu->addAction("Freeze",this,SLOT(openFrozenDialog()));
    /* ------- TO BE IMPLEMENTED ------------------ */

    /*
    *  ------------------- INSERT MENU ------------------------
    */
    QMenu *insertMenu = new QMenu("&Insert",m_menuBar);

    insertMenu->addAction("Cells");
    insertMenu->addAction("Rows",this,SLOT(insertRows()));
    insertMenu->addAction("Columns",this,SLOT(insertColumns()));

    insertMenu->addSeparator();

    insertMenu->addAction(QIcon(":/images/image.png"), "Picture", this, SLOT(insertPicture()));
    //insertMenu->addAction("WorkSheet");
#ifdef BIND_KDCHART
    insertMenu->addAction("Chart", this, SLOT(insertChart()));
#endif
    //insertMenu->addAction("Symbol");
    //insertMenu->addAction("Diagram");
    //insertMenu->addAction("Object");

    /*
    *  --------------- FORMAT MENU -------------------------------
    */
    QMenu *formatMenu = new QMenu("&Format",m_menuBar);

    QMenu* cellsMenu = formatMenu->addMenu("Cells");

    cellsMenu->addAction(QIcon(":/images/spans.png"),"Merge",this,SLOT(handleAddSpans()));
    cellsMenu->addSeparator();
    cellsMenu->addAction(QIcon(":/images/block.png"),"Disable",this,SLOT(blockCells()));
    cellsMenu->addAction(QIcon(":/images/unblock.png"),"Enable",this,SLOT(unblockCells()));
    cellsMenu->addSeparator();
    cellsMenu->addAction(QIcon(":/images/readonly.png"),"Read Only",this,SLOT(readonlyCells()));
    cellsMenu->addAction(QIcon(":/images/editable.png"),"Editable",this,SLOT(editableCells()));

    QMenu* rowMenu = formatMenu->addMenu("Row");
    rowMenu->addAction("Height");
    rowMenu->addAction("AutoFit");
    rowMenu->addAction("Hide",this,SLOT(hideRows()));
    rowMenu->addAction("Unhide",this,SLOT(unhideRows()));

    QMenu* columnMenu = formatMenu->addMenu("Column");
    columnMenu->addAction("Width");
    columnMenu->addAction("AutoFit");
    columnMenu->addAction("Hide",this,SLOT(hideColumns()));
    columnMenu->addAction("Unhide",this,SLOT(unhideColumns()));

    QMenu* sheetMenu = formatMenu->addMenu("Sheet");
    sheetMenu->addAction("Rename");
    sheetMenu->addAction("Hide");
    sheetMenu->addAction("Unhide");
    sheetMenu->addSeparator();
    sheetMenu->addAction("Background");
    sheetMenu->addAction("Tab Color");

    QMenu *toolsMenu = new QMenu("&Tools",m_menuBar);
    //toolsMenu->addAction("To be implemented...");

    toolsMenu->addAction("Settings...", this, SLOT(showSettings()));

    /* ------- TO BE IMPLEMENTED ------------------ */

    /*
    *  --------------      DATA MENU     -----------------------
    */
    QMenu *dataMenu = new QMenu("&Data",m_menuBar);
    m_sortAscAction = dataMenu->addAction(QIcon(":/images/sortasc.png"),"Sort Accending",this,SLOT(sortAscending()));
    m_sortDescAction = dataMenu->addAction(QIcon(":/images/sortdesc.png"),"Sort Descending",this,SLOT(sortDescending()));

    /*
    * --------------- WINDOW MENU -----------------------------
    */
    QMenu *windowMenu = new QMenu("&Window",m_menuBar);

    windowMenu->addAction("Activate Next Window",m_workspace,SLOT(activateNextSubWindow()));
    windowMenu->addAction("Activate Previous Window",m_workspace,SLOT(activatePreviousSubWindow()));

    windowMenu->addSeparator();
    windowMenu->addAction("Cascade",m_workspace,SLOT(cascadeSubWindows()));
    windowMenu->addAction("Tile",m_workspace,SLOT(tileSubWindows()));

    windowMenu->addSeparator();

    windowMenu->addAction("Close Active Window",m_workspace,SLOT(closeActiveSubWindow()));
    windowMenu->addAction("Close All Windows",m_workspace,SLOT(closeAllSubWindows()));

    /*
    * ------------- HELP MENU -------------------------
    */
    QMenu *helpMenu = new QMenu("&Help",m_menuBar);
    helpMenu->addAction("About...",this,SLOT(showAbout()));
    helpMenu->addAction("About Qt",qApp,SLOT(aboutQt()));

    /*
    * ------------- ADDING THE MENUS ----------------------
    */
    m_menuBar->addMenu(fileMenu);
    m_menuBar->addMenu(editMenu);
    m_menuBar->addMenu(viewMenu);
    m_menuBar->addMenu(insertMenu);
    m_menuBar->addMenu(formatMenu);
    m_menuBar->addMenu(toolsMenu);
    m_menuBar->addMenu(dataMenu);
    m_menuBar->addMenu(windowMenu);
    m_menuBar->addMenu(helpMenu);

    /*
    * --------------- SETTING THE MENUBAR -------------------------
    */
    setMenuBar(m_menuBar);
}

void MainWindow::initToolbars()
{
    QToolBar* toolBar = new QToolBar(this);

    toolBar->addAction(m_newAction);
    toolBar->addAction(m_openAction);
    toolBar->addAction(m_closeAction);
    toolBar->addAction(m_saveAction);
    toolBar->addAction(m_saveAsAction);
#ifndef QICSTABLE_GPL
    toolBar->addAction(m_printPrevAction);
    toolBar->addAction(m_printAction);
#endif

    toolBar->addSeparator();

    toolBar->addAction(m_cutAction);
    toolBar->addAction(m_copyAction);
    toolBar->addAction(m_pasteAction);
    toolBar->addAction(m_findAction);

    toolBar->addSeparator();
    toolBar->addAction(QIcon(":images/undo.png"),"Undo... not implemented yet");
    toolBar->addAction(QIcon(":/images/redo.png"),"Redo... not implemented yet");
    toolBar->addSeparator();

    toolBar->addAction(m_sortAscAction);
    toolBar->addAction(m_sortDescAction);

    addToolBar(toolBar);

    QToolBar* formatToolBar = new QToolBar(this);

    m_fontFamilyCombo = new QFontComboBox(this);
    m_fontFamilyCombo->setDuplicatesEnabled(false);
    formatToolBar->addWidget(m_fontFamilyCombo);

    m_fontSizeCombo = new QComboBox(this);

    QFontDatabase fDb;
    m_fontFamilyCombo->addItems(fDb.families());
    QList<int> sizesList = fDb.pointSizes(m_fontFamilyCombo->currentText());

    foreach(int size,sizesList)
        m_fontSizeCombo->addItem(QString::number(size));

    formatToolBar->addWidget(m_fontSizeCombo);

    connect(m_fontFamilyCombo,SIGNAL(currentFontChanged(const QFont&)),this,SLOT(handleFontFamilyChanged(const QFont&)));
    connect(m_fontSizeCombo,SIGNAL(activated(const QString&)),this,SLOT(handleFontSizeChanged(const QString&)));

    formatToolBar->addSeparator();
    m_boldAction = formatToolBar->addAction(QIcon(":/images/bold.png"),"Bold",this,SLOT(setBold()));
    m_italicAction = formatToolBar->addAction(QIcon(":/images/italic.png"),"Italic",this,SLOT(setItalic()));
    m_underlineAction = formatToolBar->addAction(QIcon(":/images/underline.png"),"Underline",this,SLOT(setUnderline()));

    m_boldAction->setCheckable(true);
    m_italicAction->setCheckable(true);
    m_underlineAction->setCheckable(true);

    formatToolBar->addSeparator();

    QActionGroup *justifyGroup = new QActionGroup(this);
    justifyGroup->setExclusive(true);

    m_justifyLeftAction = formatToolBar->addAction(QIcon(":/images/justify-left.png"),"Align Left",this,SLOT(alignLeft()));
    justifyGroup->addAction(m_justifyLeftAction);
    m_justifyCenterAction = formatToolBar->addAction(QIcon(":/images/justify-center.png"),"Align Center",this,SLOT(alignCenter()));
    justifyGroup->addAction(m_justifyCenterAction);
    m_justifyRightAction = formatToolBar->addAction(QIcon(":/images/justify-right.png"),"Align Right",this,SLOT(alignRight()));
    justifyGroup->addAction(m_justifyRightAction);

    m_justifyRightAction->setCheckable(true);
    m_justifyLeftAction->setCheckable(true);
    m_justifyCenterAction->setCheckable(true);

    formatToolBar->addSeparator();
    m_addSpansAction = formatToolBar->addAction(QIcon(":/images/spans.png"),"Merge Cells",this,SLOT(handleAddSpans()));

    formatToolBar->addSeparator();
    formatToolBar->addAction("$",this,SLOT(setCurrencyFormatter()));
    formatToolBar->addSeparator();
    formatToolBar->addAction(QIcon(":/images/fontcolor.png"),"Font Color",this,SLOT(setFontColor()));
    formatToolBar->addAction(QIcon(":/images/fillcolor.png"),"Fill Color",this,SLOT(setFillColor()));

    formatToolBar->addSeparator();
    m_borderMenu = new QMenu(formatToolBar);
    m_currentMenuAction = WHOLE;
    m_borderMenu->setIcon(QIcon(":/images/borders/whole.png"));
    m_topAction = m_borderMenu->addAction(QIcon(":/images/borders/top.png"),"Top",this,SLOT(handleBorderActions()));
    m_bottomAction = m_borderMenu->addAction(QIcon(":/images/borders/bottom.png"),"Bottom",this,SLOT(handleBorderActions()));
    m_topBottomAction = m_borderMenu->addAction(QIcon(":/images/borders/top-bottom.png"),"TopBottom",this,SLOT(handleBorderActions()));
    m_aroundAction = m_borderMenu->addAction(QIcon(":/images/borders/around.png"),"Around",this,SLOT(handleBorderActions()));
    m_wholeAction = m_borderMenu->addAction(QIcon(":/images/borders/whole.png"),"Whole",this,SLOT(handleBorderActions()));
    m_leftAction = m_borderMenu->addAction(QIcon(":/images/borders/left.png"),"Left",this,SLOT(handleBorderActions()));
    m_rightAction = m_borderMenu->addAction(QIcon(":/images/borders/right.png"),"Right",this,SLOT(handleBorderActions()));
    m_leftRightAction = m_borderMenu->addAction(QIcon(":/images/borders/left-right.png"),"LeftRight",this,SLOT(handleBorderActions()));
    m_noneAction = m_borderMenu->addAction(QIcon(":/images/borders/none.png"),"None",this,SLOT(handleBorderActions()));

    m_borderAction = m_borderMenu->menuAction();
    connect(m_borderAction,SIGNAL(triggered()),this,SLOT(makeBorders()));
    formatToolBar->addAction(m_borderMenu->menuAction());

    addToolBar(formatToolBar);

    m_dock = new QToolBar(this);
    addToolBarBreak();

    QWidget *dockedWidget = new QWidget(m_dock);

    QHBoxLayout *hl = new QHBoxLayout(dockedWidget);
    hl->setSpacing(0);
    hl->setMargin(0);

    QToolButton* cancelB = new QToolButton();
    cancelB->setIcon(QIcon(":/images/cancel.png"));
    QToolButton* applyB = new QToolButton();
    applyB->setIcon(QIcon(":/images/apply.png"));
    m_equalLabel = new QLabel("=");
    m_formulaEdit = new QLineEdit();

    connect(m_formulaEdit,SIGNAL(textEdited(const QString&)),this,SLOT(handleEditTextChanged(const QString&)));

    m_dockedBox = new QComboBox();
    m_dockedBox->setEditable(true);

    hl->addWidget(m_dockedBox);
    hl->addWidget(cancelB);
    hl->addWidget(applyB);
    hl->addWidget(m_equalLabel);
    hl->addWidget(m_formulaEdit);
    dockedWidget->setLayout(hl);

    m_dock->addWidget(dockedWidget);
    m_dock->setMovable(false);

    addToolBar(m_dock);
}

void MainWindow::showAbout()
{
    QMessageBox mb(this);
    mb.setTextFormat(Qt::RichText);
    mb.setWindowTitle("Advanced Spreadsheet Demo");
    mb.setText(QString(
        "<table>"
        "<tr><td>"
        "<img src=':/images/ics.png'>"
        "<br><h3>Integrated Computer Solutions Incorporated</h3>"
        "</td><td>"
        "<h3>The User Interface Company</h3>"
        "54B Middlesex Turnpike<br>Bedford, MA USA 01730<br><br>"
        "<b>info@ics.com<br>www.ics.com</b>"
        "</td><tr><td colspan=2>"
        "<hr>"
        "<h2>Advanced Spreadsheet Demo</h2>"
        "<p>"
        "uses QicsTable version %1"
#ifdef BIND_KDCHART
        "<br>"
        "uses KDChart binding"
#endif
        "<hr>"
        "</td></table>"
        ).arg(QICSTABLE_VERSION_STR));
    mb.exec();
}

void MainWindow::showSettings()
{
    QMdiSubWindow* activeWindow = m_workspace->activeSubWindow();
    if (!activeWindow)
        return;

    SetupDialog sd(this);
    const QList<QicsTable*> &tables = (static_cast<ChildWindow*>(activeWindow))->tables();
    sd.exec(tables);
}

void MainWindow::formatCells()
{
    qWarning("format dialog not implemented yet...");
}

void MainWindow::handleBorderActions()
{
    if(sender() == m_noneAction) {
        m_borderMenu->setIcon(QIcon(":images/borders/none.png"));
        m_currentMenuAction = NONE;
        makeBorders();
    }
    else if(sender() == m_topAction) {
        m_borderMenu->setIcon(QIcon(":images/borders/top.png"));
        m_currentMenuAction = TOP;
        makeBorders();
    }
    else if(sender() == m_bottomAction) {
        m_borderMenu->setIcon(QIcon(":images/borders/bottom.png"));
        m_currentMenuAction = BOTTOM;
        makeBorders();
    }
    else if(sender() == m_topBottomAction) {
        m_borderMenu->setIcon(QIcon(":images/borders/top-bottom.png"));
        m_currentMenuAction = TOPBOTTOM;
        makeBorders();
    }
    else if(sender() == m_aroundAction) {
        m_borderMenu->setIcon(QIcon(":images/borders/around.png"));
        m_currentMenuAction = AROUND;
        makeBorders();
    }
    else if(sender() == m_wholeAction) {
        m_borderMenu->setIcon(QIcon(":images/borders/whole.png"));
        m_currentMenuAction = WHOLE;
        makeBorders();
    }
    else if(sender() == m_leftRightAction) {
        m_borderMenu->setIcon(QIcon(":images/borders/left-right.png"));
        m_currentMenuAction = LEFTRIGHT;
        makeBorders();
    }
    else if(sender() == m_leftAction) {
        m_borderMenu->setIcon(QIcon(":images/borders/left.png"));
        m_currentMenuAction = LEFT;
        makeBorders();
    }
    else if(sender() == m_rightAction) {
        m_borderMenu->setIcon(QIcon(":images/borders/right.png"));
        m_currentMenuAction = RIGHT;
        makeBorders();
    }
}

void MainWindow::makeBorders()
{
    QicsTable* table = activeTable();
    if(!table)
        return;

    QicsSelectionList* list = table->selectionList();
    if(!list)
        return;

    table->setRepaintBehavior(Qics::RepaintOff);
    foreach(QicsSelection sel,*list) {
        switch(m_currentMenuAction)
        {
        case TOP:
            processTopBorder(table,sel);
            break;
        case BOTTOM:
            processBottomBorder(table,sel);
            break;
        case LEFT:
            processLeftBorder(table,sel);
            break;
        case RIGHT:
            processRightBorder(table,sel);
            break;
        case TOPBOTTOM: {
            processTopBorder(table,sel);
            processBottomBorder(table,sel);
            break;
                        }
        case AROUND: {
            processTopBorder(table,sel);
            processBottomBorder(table,sel);
            processLeftBorder(table,sel);
            processRightBorder(table,sel);
            break;
                     }
        case WHOLE: {
            QPen pen("black");

            QicsCellRegion &cr = table->cellRegionRef(sel.toRegion());
            cr.setTopBorderPen(pen);
            cr.setBottomBorderPen(pen);
            cr.setLeftBorderPen(pen);
            cr.setRightBorderPen(pen);
                    }
                    break;
        case LEFTRIGHT: {
            processLeftBorder(table,sel);
            processRightBorder(table,sel);
                        }
                        break;
        case NONE: {
            QicsCellRegion &cr = table->cellRegionRef(sel.toRegion());
            cr.setTopBorderPen(Qt::NoPen);
            cr.setBottomBorderPen(Qt::NoPen);
            cr.setLeftBorderPen(Qt::NoPen);
            cr.setRightBorderPen(Qt::NoPen);
                   }
                   break;
        default:
            break;
        }
    }
    table->setRepaintBehavior(Qics::RepaintOn);
    table->repaint();
}

void MainWindow::processLeftBorder(QicsTable* table,const QicsSelection& sel)
{
    table->cellRegionRef(sel.toRegion()).setBoxPen(QPen(DEFAULT_BORDER_COLOR,1), Qics::LeftBorder);
}

void MainWindow::processRightBorder(QicsTable* table,const QicsSelection& sel)
{
    table->cellRegionRef(sel.toRegion()).setBoxPen(QPen(DEFAULT_BORDER_COLOR,1), Qics::RightBorder);
}

void MainWindow::processTopBorder(QicsTable* table,const QicsSelection& sel)
{
    table->cellRegionRef(sel.toRegion()).setBoxPen(QPen(DEFAULT_BORDER_COLOR,1), Qics::TopBorder);
}

void MainWindow::processBottomBorder(QicsTable* table,const QicsSelection& sel)
{
    table->cellRegionRef(sel.toRegion()).setBoxPen(QPen(DEFAULT_BORDER_COLOR,1), Qics::BottomBorder);
}

#ifndef QICSTABLE_GPL
void MainWindow::print()
{
    QicsTable *table = activeTable();
    if(!table)
        return;

    QPrinter* printer = new QPrinter(QPrinter::ScreenResolution);
    QicsTablePrint* tablePrint = new QicsTablePrint(table);
    QPrintDialog pD(printer,this);
    if(pD.exec() == QDialog::Accepted)
        tablePrint->print(printer,QicsRegion(0,0,20,10));

    delete tablePrint;
}

void MainWindow::printPreview()
{
    QicsTable *table = activeTable();
    if(!table)
        return;

    QicsTablePrint *printTable = new QicsTablePrint(table);

    QicsPrintPreviewDialog ppd;

    printTable->setPreviewWidget(ppd.previewWidget());
    printTable->setPreviewRegion();

    ppd.exec();

    delete printTable;

    raise();
}
#endif

void MainWindow::handleAddSpans()
{
    QicsTable *table = activeTable();
    if (!table)
        return;

    QicsCell *cur_cell = table->currentCell();

    if (cur_cell && !cur_cell->spannedRegion().isEmpty()) {
        QicsSelectionList *slist = table->selectionList(true);
        if (slist) {
            QicsSelectionList::const_iterator iter;

            for (iter = slist->begin(); iter != slist->end(); ++iter) {
                const QicsSelection &sel = *iter;
                table->removeCellSpan(sel.topRow(), sel.leftColumn());
            }
        }
    }
    else {
        QicsSelectionList *slist = table->selectionList(true);
        if (slist) {
            QicsSelectionList::const_iterator iter;

            int cur_row = 0, cur_col = 0;
            for (iter = slist->begin(); iter != slist->end(); ++iter) {
                const QicsSelection &sel = *iter;
                int bottom_row = sel.numRows();
                int right_column = sel.numColumns();

                if (sel.bottomRow() == Qics::QicsLAST_ROW)
                    bottom_row = table->dataModel()->numRows();
                if (sel.rightColumn() == Qics::QicsLAST_COLUMN)
                    right_column = table->dataModel()->numColumns();

                table->addCellSpan(QicsSpan(sel.topRow(),sel.leftColumn(),bottom_row,right_column));
                cur_row = sel.topRow();
                cur_col = sel.leftColumn();
            }

            table->setCurrentCell(cur_row, cur_col);
        }
    }
}

void MainWindow::copy()
{
    INIT_TABLE();
    table->copy();
}

void MainWindow::cut()
{
    INIT_TABLE();
    table->cut();
}

void MainWindow::paste()
{
    INIT_TABLE();
    table->paste();
}

void MainWindow::sortAscending()
{
    QicsTable *table = activeTable();
    if (table) {
        QicsSelectionList *list = table->selectionList(true);
        if (!list)
            return;

        QVector<int> selectedCols = list->columns().toVector();
        if (selectedCols.size() <= 0) selectedCols << 0;

        QicsRegion reg = list->region();

        table->sortRows(selectedCols, Qics::Ascending, reg.startRow(), reg.endRow());
    }
}

void MainWindow::sortDescending()
{
    QicsTable *table = activeTable();
    if (table) {
        QicsSelectionList *list = table->selectionList(true);
        if (!list)
            return;

        QVector<int> selectedCols = list->columns().toVector();
        if (selectedCols.size() <= 0) selectedCols << 0;

        QicsRegion reg = list->region();

        table->sortRows(selectedCols, Qics::Descending, reg.startRow(), reg.endRow());
    }
}

void MainWindow::blockCells()
{
    QicsTable *table = activeTable();
    if (!table)
        return;

    QicsSelectionList *list  = table->selectionList(true);
    if (!list)
        return;

    foreach (QicsSelection sel, *list)
        table->cellRegionRef(sel.toRegion()).setEnabled(false);
}

void MainWindow::unblockCells()
{
    QicsTable *table = activeTable();
    if (!table)
        return;

    QicsSelectionList *list  = table->selectionList(true);
    if (!list)
        return;

    foreach (QicsSelection sel, *list)
        table->cellRegionRef(sel.toRegion()).setEnabled(true);
}

void MainWindow::readonlyCells()
{
    QicsTable *table = activeTable();
    if (!table)
        return;

    QicsSelectionList *list  = table->selectionList(true);
    if (!list)
        return;

    foreach (QicsSelection sel, *list)
        table->cellRegionRef(sel.toRegion()).setReadOnly(true);
}

void MainWindow::editableCells()
{
    QicsTable *table = activeTable();
    if (!table)
        return;

    QicsSelectionList *list  = table->selectionList(true);
    if (!list)
        return;

    foreach (QicsSelection sel, *list)
        table->cellRegionRef(sel.toRegion()).setReadOnly(false);
}

void MainWindow::setFillColor()
{
    QicsTable *table = activeTable();

    QColor color = QColorDialog::getColor();
    if (!color.isValid())
        return;

    QicsSelectionList* list = table->selectionList(true);

    if (list) {
        foreach(QicsSelection sel,*list)
            table->cellRegionRef(sel.toRegion()).setBackgroundColor(color);
    }
}

void MainWindow::setFontColor()
{
    QicsTable *table = activeTable();
    QColor color = QColorDialog::getColor();
    if (!color.isValid())
        return;

    QicsSelectionList* list =  table->selectionList(true);

    if (list) {
        foreach(QicsSelection sel,*list)
            table->cellRegionRef(sel.toRegion()).setForegroundColor(color);
    }
}

void MainWindow::handleFontFamilyChanged(const QFont& font)
{
    QicsTable *table = activeTable();

    QFontDatabase fDb;
    QList<int> sizesList = fDb.pointSizes(font.family());

    m_fontSizeCombo->clear();
    foreach(int size,sizesList)
        m_fontSizeCombo->addItem(QString::number(size));

    QicsSelectionList* list =  table->selectionList(true);

    if (list) {
        foreach(QicsSelection sel,*list)
            table->cellRegionRef(sel.toRegion()).setFontFamily(font.family(), true);
    }
}

void MainWindow::handleFontSizeChanged(const QString& size)
{
    QicsTable *table = activeTable();

    QicsSelectionList* list =  table->selectionList(true);

    if (list) {
        foreach(QicsSelection sel,*list) {
            table->cellRegionRef(sel.toRegion()).setFontSize(size.toInt(), true);
        }
    }
}

void MainWindow::setBold()
{
    QicsTable* table = activeTable();
    if(!table)
        return;

    QicsSelectionList* list  = table->selectionList(true);

    if(!list)
        return;

    foreach(QicsSelection sel,*list)
        table->cellRegionRef(sel.toRegion()).setFontAttribute(QicsCellStyle::FontBold, m_boldAction->isChecked());
}

void MainWindow::setItalic()
{
    QicsTable* table = activeTable();
    if(!table)
        return;

    QicsSelectionList* list  = table->selectionList(true);

    if(!list)
        return;

    foreach(QicsSelection sel,*list)
        table->cellRegionRef(sel.toRegion()).setFontAttribute(QicsCellStyle::FontItalic, m_italicAction->isChecked());
}

void MainWindow::setUnderline()
{
    QicsTable* table = activeTable();
    if(!table)
        return;

    QicsSelectionList* list  = table->selectionList(true);

    if(!list)
        return;

    foreach(QicsSelection sel,*list)
        table->cellRegionRef(sel.toRegion()).setFontAttribute(QicsCellStyle::FontUnderline, m_underlineAction->isChecked());
}

void MainWindow::alignLeft()
{
    QicsTable* table = activeTable();
    if(!table)
        return;

    QicsSelectionList* list  = table->selectionList(true);

    if(!list)
        return;

    foreach(QicsSelection sel,*list)
        table->cellRegionRef(sel.toRegion()).setAlignment(Qt::AlignLeft/* | Qt::AlignVCenter*/);

    m_justifyRightAction->setChecked(false);
    m_justifyCenterAction->setChecked(false);
}

void MainWindow::alignRight()
{
    QicsTable* table = activeTable();
    if(!table)
        return;

    QicsSelectionList* list  = table->selectionList(true);

    if(!list)
        return;

    foreach(QicsSelection sel,*list)
        table->cellRegionRef(sel.toRegion()).setAlignment(Qt::AlignRight/* | Qt::AlignVCenter*/);

    m_justifyCenterAction->setChecked(false);
    m_justifyLeftAction->setChecked(false);
}

void MainWindow::alignCenter()
{
    QicsTable* table = activeTable();
    if(!table)
        return;

    QicsSelectionList* list  = table->selectionList(true);

    if(!list)
        return;

    foreach(QicsSelection sel,*list)
        table->cellRegionRef(sel.toRegion()).setAlignment(Qt::AlignHCenter);

    m_justifyLeftAction->setChecked(false);
    m_justifyRightAction->setChecked(false);
}

void MainWindow::setCurrencyFormatter()
{
    QicsTable* table = activeTable();
    if(!table)
        return;

    QicsSelectionList* list  = table->selectionList(true);

    if(!list)
        return;

    SpreadsheetFormatter* currency = new SpreadsheetFormatter();

    foreach(QicsSelection sel,*list)
        table->cellRegionRef(sel.toRegion()).setFormatter(currency);
}

void MainWindow::selectAll()
{
    // It is too slow yet. We need to find more fast selection algorithm.
    // AM - no need to search for something else, it is very fast for now ;)

    QicsTable* table = activeTable();
    if(!table)
        return;

    table->selectAll();
}

void MainWindow::find()
{
    if(!m_fd) {
        m_fd = new FindDialog(this);
        connect(m_fd,SIGNAL(findRequest(const QString&)),this,SLOT(performSearch(const QString&)));
        connect(m_fd,SIGNAL(findBackwardRequest(const QString&)),this,SLOT(performBackwardSearch(const QString&)));

        connect(m_fd,SIGNAL(replaceBackwardRequest(const QString&,const QString&)),
            this,SLOT(performBackwardReplace(const QString&,const QString&)));

        connect(m_fd,SIGNAL(replaceRequest(const QString&,const QString&)),
            this,SLOT(performReplace(const QString&,const QString&)));
    }

    m_fd->show();
}

void MainWindow::performBackwardSearch(const QString& str)
{
    doSearch(str,true);
}

void MainWindow::performSearch(const QString& str)
{
    doSearch(str,false);
}

void MainWindow::performReplace(const QString& which,const QString& replace)
{
    doReplace(which,replace,false);
}

void MainWindow::performBackwardReplace(const QString& which,const QString& replace)
{
    doReplace(which,replace,true);
}

void MainWindow::doReplace(const QString& which,const QString& replace,bool backward)
{
    QicsICell cell;
    if (!(cell = doSearch(which,backward)).isValid())
        return;

    QicsTable *table = activeTable();
    if (!table)
        return;

    const QicsDataItem *itm = table->dataModel()->item(cell.row(),cell.column());
    if (!itm)
        return;

    QString itemString = itm->string();
    table->dataModel()->setItem(cell.row(),cell.column(),
        QicsDataString(itemString.replace(which,replace)));
}


QicsICell MainWindow::doSearch(const QString& str,bool backward)
{
    QicsTable *table = activeTable();
    if (!table)
        return QicsICell();

    int row = table->currentCell()->rowIndex();
    int col = table->currentCell()->columnIndex();

    const int rows = table->dataModel()->numRows();
    const int columns = table->dataModel()->numColumns();

    do {
        if (backward) {
            col--;
            if (col < 0) {
                col = columns-1;
                row--;
                if (row < 0)
                    return QicsICell();
            }
        } else {
            col++;
            if (col > columns) {
                col = 0; row++;
                if (row > rows)
                    return QicsICell();
            }
        }

        const QicsDataItem *itm = table->cellRef(row,col).dataValue();
        if (itm && itm->string().contains(str, Qt::CaseInsensitive))
            break;

    } while (true);

    table->setCurrentCell(row,col);

    return QicsICell(row,col);
}

void MainWindow::updateToolBars(int row,int col,QicsTable* table)
{
    m_fontFamilyCombo->blockSignals(true);
    m_fontSizeCombo->blockSignals(true);

    QicsCell &cell = table->cellRef(row,col);

    QFont font = cell.font();
    if (!m_fontFamilyCombo->findText(font.family()))
        m_fontFamilyCombo->addItem(font.family());

    m_fontFamilyCombo->setCurrentIndex(m_fontFamilyCombo->findText(font.family()));

    QFontDatabase fDb;
    QList<int> sizesList = fDb.pointSizes(m_fontFamilyCombo->currentText());

    m_fontSizeCombo->clear();
    foreach(int size, sizesList)
        m_fontSizeCombo->addItem(QString::number(size));

    m_fontSizeCombo->setCurrentIndex(m_fontSizeCombo->findText(QString::number(font.pointSize())));

    m_boldAction->setChecked(font.bold());
    m_italicAction->setChecked(font.italic());
    m_underlineAction->setChecked(font.underline());

    int alignment = cell.alignment();

    m_justifyLeftAction->setChecked(alignment & Qt::AlignLeft);
    m_justifyRightAction->setChecked(alignment & Qt::AlignRight);
    m_justifyCenterAction->setChecked(alignment & Qt::AlignCenter);

    m_fontFamilyCombo->blockSignals(false);
    m_fontSizeCombo->blockSignals(false);
}

void MainWindow::closeDocument()
{
    delete m_workspace->activeSubWindow();
}

void MainWindow::quitProgram()
{
    QApplication::closeAllWindows();
}

QicsTable* MainWindow::activeTable()
{
    QMdiSubWindow* activeWindow = m_workspace->activeSubWindow();
    if(!activeWindow)
        return 0;

    QicsTable *table = static_cast<ChildWindow*>(activeWindow)->table();
    return table;
}

void MainWindow::createNewDocument()
{
    ChildWindow* childWindow = new ChildWindow(this);
    m_workspace->addSubWindow(childWindow);
    childWindow->show();
    childWindow->showMaximized();
}

void MainWindow::handleCurrentCellChanged(int row,int col)
{
    SpreadsheetTable *currentTable = qobject_cast<SpreadsheetTable*>(sender());
    if (!currentTable)
        return;

    updateToolBars(row,col,currentTable);

    QString columnString = currentTable->labelFromColumn(col);
    QString rowString = currentTable->labelFromRow(row);

    m_dockedBox->setEditText(QString(columnString.remove(" ") + "x" + rowString));

    QicsTable *table = activeTable();
    const QicsDataItem *itm = table->currentCell()->dataValue();
    m_formulaEdit->blockSignals(true);
    if (itm)
        m_formulaEdit->setText(itm->string());
    else
        m_formulaEdit->clear();
    m_formulaEdit->blockSignals(false);
}

void MainWindow::handleEditTextChanged(const QString& str)
{
    QicsTable *table = activeTable();

    if (table) {
        QicsICell cell(table->currentCell()->rowIndex(),table->currentCell()->columnIndex());
        if (cell.isValid())
            table->dataModel()->setItem(cell.row(),cell.column(),QicsDataString(str));
    }
}


/*---------------------------------------------------------------------------
*  Function:  openFile
*
*  Description:
*    Protected slot, bring up a QFileDialog to select a file, then load it.
*/

void MainWindow::openFile()
{
    QString fname = QFileDialog::getOpenFileName(this,"Open Data File",".",
        "Advanced Spreadsheet Data (*.ads)\nCSV Text (*.csv *.txt)");

    if (!fname.isEmpty()) {
        m_fileName = fname;
        loadFile(m_fileName);
    }
}

/*---------------------------------------------------------------------------
*  Function:  loadFile
*/

void MainWindow::loadFile(const QString &inFileName)
{
    if (inFileName.isEmpty())
        return;

    closeDocument();
    createNewDocument();
    QicsTable *table = activeTable();
    if (!table)
        return;
    table->setRepaintBehavior(Qics::RepaintOff);

    // load model
    QString xmlFile = inFileName;

    if (inFileName.endsWith(".ads")) {
        static_cast<SpreadsheetDataModel*>(table->dataModel())->loadDataModel(inFileName);
        xmlFile.remove(".ads").append(".xml");
    } else { // load as csv
        QFile file(inFileName);
        if (!file.open(QIODevice::ReadOnly))
            qDebug("error opening %s file...",xmlFile.toLocal8Bit().data());
        else {
            QTextStream ds(&file);
            table->dataModel()->readASCII(ds);
            file.close();
        }
        QFileInfo fi(inFileName);
        xmlFile.remove(fi.completeSuffix()).append(".xml");
    }

    // load layout
    QFile file(xmlFile);
    if (!file.open(QIODevice::ReadOnly))
        qDebug("error opening %s file...",xmlFile.toLocal8Bit().data());
    else {
        QTextStream ds(&file);
        QString xmlString = ds.readAll();
        table->configureFromXml(xmlString);
        file.close();
    }

    table->setTableSpacing(0);
    qobject_cast<SpreadsheetTable*>(table)->setLayoutSpacing(0);
    table->mainGridRef().setDisplayer(new SpreadsheetCellDisplay(table));
    table->setRepaintBehavior(Qics::RepaintOn);

    table->setSelectionStyle(Qics::Exclusive);
}

/*---------------------------------------------------------------------------
*  Function:  saveFile
*
*  Description:
*    Protected slot, save data to the current file name
*/

void MainWindow::saveFile()
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

void MainWindow::saveFileAs()
{
    QicsTable *table = activeTable();
    if (!table) return;

    QString filter, fname =
        QFileDialog::getSaveFileName(this,"Save Data File",".","Advanced Spreadsheet Data (*.ads);;HTML Page (*.htm);;Comma Separated Values (*.csv)",&filter);

    if (!fname.isEmpty()) {
        if (fname.endsWith(".htm") || fname.endsWith(".html") || filter.contains(".htm")) {
            QicsHTMLExportDialog ed;
            QicsHTMLExportOptions opts;
            ed.exec(fname, opts, table);
        } else {
            m_fileName = fname;
            writeFile(m_fileName);
        }
    }
}

/*---------------------------------------------------------------------------
*  Function:  writeFile
*/

void MainWindow::writeFile(const QString &outFileName)
{
    if (outFileName.isEmpty())
        return;

    QicsTable *table = activeTable();
    if(!table) return;

    QString xmlFile = outFileName;

    table->setRepaintBehavior(Qics::RepaintOff);

    if (outFileName.endsWith(".csv")) { //save to CSV
        QFile file(outFileName);
        if(!file.open(QIODevice::WriteOnly)) {
            qDebug("error opening CSV file for writing...");
        }
        else {
            QTextStream ds(&file);
            table->dataModel()->writeASCII(ds);
            file.close();
        }

        QFileInfo fi(outFileName);
        xmlFile.remove(fi.completeSuffix()).append(".xml");
    }
    else {  //save to ADS
        qobject_cast<SpreadsheetDataModel*>(table->dataModel())->writeDataModel(outFileName);
        xmlFile.remove(".ads").append(".xml");
    }

    // load layout
    QFile file(xmlFile);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug("error opening XML file for writing...");
    }
    else {
        QTextStream ds(&file);
        ds << table->configurationToXml();
        file.close();
    }

    table->setRepaintBehavior(Qics::RepaintOn);
}

/*---------------------------------------------------------------------------
*  Function:  insertPicture
*/

void MainWindow::insertPicture()
{
    INIT_TABLE();

    QString s = QFileDialog::getOpenFileName(this, "Choose image file",
        table->defaultImagePath(), "Images (*.png *.xpm *.jpg)");
    if (s.isEmpty()) return;

    table->currentCell()->setPixmap(s);
}

/*---------------------------------------------------------------------------
*  Function:  insertChart
*/

#ifdef BIND_KDCHART

void MainWindow::insertChart()
{
    INIT_TABLE();

    QicsRegion r;
    QicsSelectionList* list = table->selectionList(true);
    if (!list || list->isEmpty())
        r = QicsRegion(table->currentCell()->rowIndex(), table->currentCell()->columnIndex());
    else
        r = list->first().toRegion();

    BarDiagram *bars = new BarDiagram;

    KDChartObject *kdview = new KDChartObject(table, bars, r, QicsRegion(0,0));
    Q_UNUSED(kdview);
}

#endif

/*---------------------------------------------------------------------------
*  Function:  insertRows
*/
void MainWindow::insertRows()
{
    INIT_TABLE();
    table->insertRows();
}

/*---------------------------------------------------------------------------
*  Function:  insertColumns
*/
void MainWindow::insertColumns()
{
    INIT_TABLE();
    table->insertColumns();
}

/*---------------------------------------------------------------------------
*  Function:  deleteRows
*/
void MainWindow::deleteRows()
{
    INIT_TABLE();
    table->deleteRows();
}

/*---------------------------------------------------------------------------
*  Function:  deleteColumns
*/
void MainWindow::deleteColumns()
{
    INIT_TABLE();
    table->deleteColumns();
}

/*---------------------------------------------------------------------------
*  Function:  hideRows
*/
void MainWindow::hideRows()
{
    INIT_TABLE();
    table->hideRows();
}

/*---------------------------------------------------------------------------
*  Function:  hideColumns
*/
void MainWindow::hideColumns()
{
    INIT_TABLE();
    table->hideColumns();
}

/*---------------------------------------------------------------------------
*  Function:  unhideRows
*/
void MainWindow::unhideRows()
{
    INIT_TABLE();
    table->unhideRows();
}

/*---------------------------------------------------------------------------
*  Function:  unhideColumns
*/
void MainWindow::unhideColumns()
{
    INIT_TABLE();
    table->unhideColumns();
}

/*---------------------------------------------------------------------------
*  Function:  clearContents
*/
void MainWindow::clearContents()
{
    INIT_TABLE();
    table->clearContents();
}

/*---------------------------------------------------------------------------
*  Function:  openFrozenDialog
*/
void MainWindow::openFrozenDialog()
{
    INIT_TABLE();

    FrozenConfigDialog frozen_dialog;
    frozen_dialog.exec(table->numFreezeTopRows(),
        table->numFreezeBottomRows(),
        table->numFreezeLeftColumns(),
        table->numFreezeRightColumns() );

    if (frozen_dialog.freezeBottomRows())
        table->freezeBottomRows(frozen_dialog.numBottomFrozenRows());
    else
        table->unfreezeBottomRows();

    if (frozen_dialog.freezeTopRows())
        table->freezeTopRows(frozen_dialog.numTopFrozenRows());
    else
        table->unfreezeTopRows();

    if (frozen_dialog.freezeLeftColumns())
        table->freezeLeftColumns(frozen_dialog.numLeftFrozenCols());
    else
        table->unfreezeLeftColumns();

    if (frozen_dialog.freezeRightColumns())
        table->freezeRightColumns(frozen_dialog.numRightFrozenCols());
    else
        table->unfreezeRightColumns();
}

/*---------------------------------------------------------------------------
*  Function:  normalize
*/
QicsRegion MainWindow::normalize(QicsRegion rg)
{
    QicsRegion reg(rg);
    QicsDataModel *dm = activeTable()->dataModel();

    if(reg.endRow() == Qics::QicsLAST_ROW)
        reg.setEndRow(dm->numRows()-1);
    if(reg.endColumn() == Qics::QicsLAST_COLUMN)
        reg.setEndColumn(dm->numColumns()-1);

    return reg;
}


