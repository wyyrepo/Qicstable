/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QMainWindow>
#include <QicsRegion.h>

class QComboBox;
class QLineEdit;
class QicsDataModel;
class QicsTable;
#ifdef QSA
class QSProject;
#endif


class Spreadsheet : public QMainWindow
{
    Q_OBJECT
public:
    Spreadsheet(QWidget* parent = 0);

protected:
    void loadFile(QString);
    void writeFile(QString);
    void getSelectedRegion();
    void setFontCombos(const QFont&);

public slots:
    void setCellValue(int row, int col, int value);
    QString getCellValue(int row, int col);
    void sortAscending();
    void sortDescending();

private slots:
    void resized();
    void copy();
    void cut();
    void paste();
    void fileNew();
    void openFile();
    void saveFile();
    void saveFileAs();
    void insertRow();
    void insertColumn();
    void deleteRow();
    void deleteColumn();
    void formatFont();
    void formatBackgroundColor();
    void formatForegroundColor();
    void selectFont(int);
    void selectFontSize(int combo);
    void textAlignLeft();
    void textAlignRight();
    void textAlignCenter();
    void textBold();
    void textItalic();
    void textUnderline();
    void setCurrencyFormatter();
    void cellLineUpdate();
    void updateSelection(bool);
    void addSpan();
    void removeSpan();
    // Should really ifdef these out, but MOC complains....
    void showQSAWorkbench();
    void testScripts();
    void saveScripts();
    void print();

private:
    void encodePosition(QString &text, int column);

private:
    QicsDataModel *m_dm;
    QicsTable *m_table;
    QLineEdit* m_cellLineEdit;
    QComboBox* m_fontFamilyCombo;
    QComboBox* m_fontSizeCombo;
    QAction *m_boldAction;
    QAction *m_italicAction;
    QAction *m_underlineAction;
    QAction *m_textLeftAction;
    QAction *m_textCenterAction;
    QAction *m_textRightAction;
    QString m_fileName;
    int m_startSelectedRow;
    int m_startSelectedColumn;
    int m_endSelectedRow;
    int m_endSelectedColumn;
    QicsRegion	m_selectedRegion;
#ifdef QSA
    QSProject *m_project;
#endif
};

#endif // SPREADSHEET_H


