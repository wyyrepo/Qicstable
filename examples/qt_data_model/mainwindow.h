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
#include <QicsNamespace.h>

class QStandardItemModel;
class QicsDataModelQtModelAdapter;
class QicsTable;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void insertRow();
    void deleteRow();
    void insertCol();
    void deleteCol();
    void columnHeader_doubleClicked (int, int, int, const QPoint& );

private:
    void createActions();
    void createMenus();
    void createTable();
    void createQtModel();

private:
    QMenu *m_fileMenu;
    QMenu *m_editMenu;

    QAction *m_quitAct;
    QAction *m_insertRowAct;
    QAction *m_deleteRowAct;
    QAction *m_insertColAct;
    QAction *m_deleteColAct;

    QStandardItemModel *m_qtModel;
    QicsDataModelQtModelAdapter *m_icsModel;
    QicsTable *m_table;

    int m_sortedByCol;
    Qics::QicsSortOrder m_sortOrder;
};

#endif //MAINWINDOW_H


