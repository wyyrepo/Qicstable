/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QicsTable.h>

class QMdiArea;
class QMenuBar;
class QToolBar;
class QLineEdit;
class QComboBox;
class FindDialog;
class QLabel;
class FrozenConfigDialog;
class QFontComboBox;

/*
* Main window class. Contains QWorkspace with child windows
* inside of it.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    // Default constructor
    MainWindow(QWidget *parent = 0);

    // Menus and toolbars initialization
    void initMenus();
    void initToolbars();

    // Returns pointer to the table that is active, otherwise returns NULL
    QicsTable* activeTable();

public slots:
    void createNewDocument();
    void handleCurrentCellChanged(int,int);
    void handleEditTextChanged(const QString&);
    void handleFontFamilyChanged(const QFont&);
    void handleFontSizeChanged(const QString&);
    void handleBorderActions();
    void makeBorders();
#ifndef QICSTABLE_GPL
    void print();
    void printPreview();
#endif
    void setCurrencyFormatter();
    void formatCells();

    void copy();
    void cut();
    void paste();
    void openFile();
    void saveFile();
    void saveFileAs();
    void sortAscending();
    void sortDescending();
    void closeDocument();
    void quitProgram();
    void setFillColor();
    void setFontColor();

    void openFrozenDialog();

    void setBold();
    void setItalic();
    void setUnderline();

    void alignLeft();
    void alignRight();
    void alignCenter();

    void handleAddSpans();
    void blockCells();
    void unblockCells();
    void readonlyCells();
    void editableCells();

    void showAbout();
    void find();
    void showSettings();

    void selectAll();
    void clearContents();

    void performSearch(const QString&);
    void performBackwardSearch(const QString&);
    void performReplace(const QString& str,const QString&);
    void performBackwardReplace(const QString& str,const QString&);

    void insertPicture();

#ifdef BIND_KDCHART
    void insertChart();
#endif

    void insertRows();
    void insertColumns();
    void deleteRows();
    void deleteColumns();
    void hideRows();
    void hideColumns();
    void unhideRows();
    void unhideColumns();

protected:
    // INTERNAL METHODS
    void loadFile(const QString &inFileName);
    void writeFile(const QString &outFileName);
    void updateToolBars(int,int,QicsTable*);
    QicsICell doSearch(const QString&,bool);
    void doReplace(const QString&,const QString&,bool);
    void processLeftBorder(QicsTable*,const QicsSelection&);
    void processRightBorder(QicsTable*,const QicsSelection&);
    void processTopBorder(QicsTable*,const QicsSelection&);
    void processBottomBorder(QicsTable*,const QicsSelection&);

    QicsRegion normalize(QicsRegion rg);

private:
    QToolBar *m_dock;
    QMenuBar *m_menuBar;
    QLabel *m_equalLabel;
    QLineEdit *m_formulaEdit;
    QComboBox *m_dockedBox;
    QMdiArea *m_workspace;
    QString m_fileName;
    QFontComboBox* m_fontFamilyCombo;
    QComboBox* m_fontSizeCombo;
    FindDialog* m_fd;
    QicsICell m_currentSearchCell;

    // Enum of action types that can be done with borders
    enum BORDER_ACTION{TOP, BOTTOM, TOPBOTTOM, AROUND, WHOLE,
        LEFT, RIGHT, LEFTRIGHT, NONE};

    // ACTIONS
    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_closeAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
#ifndef QICSTABLE_GPL
    QAction* m_printPrevAction;
    QAction* m_printAction;
#endif

    QAction* m_cutAction;
    QAction* m_copyAction;
    QAction* m_pasteAction;
    QAction* m_selectAllAction;
    QAction* m_findAction;

    QAction* m_sortAscAction;
    QAction* m_sortDescAction;

    QAction* m_boldAction;
    QAction* m_italicAction;
    QAction* m_underlineAction;
    QAction* m_justifyRightAction;
    QAction* m_justifyLeftAction;
    QAction* m_justifyCenterAction;

    QAction* m_addSpansAction;

    // BORDER ACTIONS
    QAction* m_topAction;
    QAction* m_bottomAction;
    QAction* m_topBottomAction;
    QAction* m_aroundAction;
    QAction* m_wholeAction;
    QAction* m_leftAction;
    QAction* m_rightAction;
    QAction* m_leftRightAction;
    QAction* m_noneAction;

    QMenu* m_borderMenu;
    QAction* m_borderAction;
    int m_currentMenuAction;
};

#endif //MAINWINDOW_H


