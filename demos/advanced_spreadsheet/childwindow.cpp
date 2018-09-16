/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "childwindow.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMouseEvent>
#include <QMenu>
#include "mainwindow.h"
#include "spreadsheettable.h"
#include "spreadsheetgrid.h"


ChildWindow::ChildWindow(QWidget* parent)
    : QMdiSubWindow(parent),
      m_parent(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabPosition(QTabWidget::South);

    for(int i=0;i<3;++i)
        addTab(tr("Page %1").arg(i+1));

    QWidget *cornerWidget = new QWidget(m_tabWidget);
    QHBoxLayout* hl = new QHBoxLayout;
    hl->setMargin(2);
    hl->setSpacing(0);

    QToolButton* firstButton = new QToolButton;
    firstButton->setIcon(QIcon(":/images/first.png"));
    hl->addWidget(firstButton);
    hl->setMargin(1);
    QToolButton* rightButton = new QToolButton;
    rightButton->setArrowType(Qt::RightArrow);
    QToolButton* leftButton = new QToolButton;
    leftButton->setArrowType(Qt::LeftArrow);
    hl->addWidget(leftButton);
    hl->addWidget(rightButton);
    QToolButton* lastButton = new QToolButton;
    lastButton->setIcon(QIcon(":/images/last.png"));
    hl->addWidget(lastButton);

    cornerWidget->installEventFilter(this);

    connect(rightButton,SIGNAL(clicked()),this,SLOT(handleRightClicked()));
    connect(leftButton,SIGNAL(clicked()),this,SLOT(handleLeftClicked()));
    connect(firstButton,SIGNAL(clicked()),this,SLOT(handleFirstClicked()));
    connect(lastButton,SIGNAL(clicked()),this,SLOT(handleLastClicked()));

    cornerWidget->setLayout(hl);
    m_tabWidget->setTabShape(QTabWidget::Triangular);

    m_tabWidget->setCornerWidget(cornerWidget,Qt::BottomLeftCorner);

    setWidget(m_tabWidget);
}

ChildWindow::~ChildWindow()
{
}

QicsTable* ChildWindow::table()
{
    return qobject_cast<QicsTable*>(m_tabWidget->currentWidget());
}

QList<QicsTable*> ChildWindow::tables()
{
    QList<QicsTable*> list;
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        QicsTable *table = qobject_cast<QicsTable*>(m_tabWidget->widget(i));
        list.append(table);
    }
    return list;
}

void ChildWindow::addTab(const QString& name)
{
    MainWindow* mw = qobject_cast<MainWindow*>(m_parent);
    SpreadsheetTable* table = new SpreadsheetTable(SpreadsheetGrid::createGrid,this);
    m_tabWidget->addTab(table,name);
    connect(table,SIGNAL(currentCellChanged(int,int)),mw,SLOT(handleCurrentCellChanged(int,int)));
}

void ChildWindow::handleRightClicked()
{
    m_tabWidget->setCurrentIndex(m_tabWidget->currentIndex() + 1);
}
void ChildWindow::handleLeftClicked()
{
    m_tabWidget->setCurrentIndex(m_tabWidget->currentIndex() - 1);
}

void ChildWindow::handleFirstClicked()
{
    m_tabWidget->setCurrentIndex(0);
}

void ChildWindow::handleLastClicked()
{
    m_tabWidget->setCurrentIndex(m_tabWidget->count() - 1);
}

bool ChildWindow::eventFilter ( QObject * , QEvent * event )
{
    if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        if(me->button() == Qt::RightButton) {
            QMenu* menu = new QMenu;
            menu->addAction("Add Tab",this,SLOT(addTab()));
            menu->addAction("Delete Tab",this,SLOT(deleteTab()));
            menu->exec(me->globalPos());
            delete menu;
        }
    }
    return false;
}

void ChildWindow::addTab()
{
    addTab(tr("New Tab"));
}

void ChildWindow::deleteTab()
{
    // Not implemented yetBox
}


