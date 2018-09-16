/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H

#include <QMdiSubWindow>

class QTabWidget;
class QWidget;
class QicsTable;

// Class represents child windows inside of QWorkspace.
class ChildWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    ChildWindow(QWidget* parent);
    virtual ~ChildWindow();

    // Returns table at current page that child window contains.
    QicsTable* table();
    // Returns all the tables (pages) that child window contains.
    QList<QicsTable*> tables();

protected:
    virtual bool eventFilter ( QObject * watched, QEvent * event );

public slots:
    void addTab(const QString&);
    void addTab();
    void deleteTab();
    void handleRightClicked();
    void handleLeftClicked();
    void handleFirstClicked();
    void handleLastClicked();

private:
    QWidget* m_parent;
    QTabWidget *m_tabWidget;
};

#endif //CHILDWINDOW_H


