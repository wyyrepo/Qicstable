/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "mainWindow.h"

#include <QtGui>
#include <QMdiArea>
#include <QFileDialog>
#include <QAction>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QDebug>

#include "mdiChildTable.h"

#include <iostream>



MainWindow::MainWindow()
{
    m_workspace = new QMdiArea;
    setCentralWidget(m_workspace);

    connect(m_workspace, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(updateMenus()));

    m_windowMapper = new QSignalMapper(this);
    connect(m_windowMapper, SIGNAL(mapped(QWidget *)), SLOT(setActiveSubWindow(QWidget *)));


    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();

    readSettings();

    setWindowTitle(tr("MDI-QicsTable"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_workspace->closeAllSubWindows();
    if (activeMdiChild()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}

void MainWindow::newFile()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->show();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) {
            m_workspace->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        } else {
            child->close();
        }
    }
}

void MainWindow::save()
{
    if (activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::saveAs()
{
    if (activeMdiChild()->saveAs())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::cut()
{
    activeMdiChild()->cut();
}

void MainWindow::copy()
{
    activeMdiChild()->copy();
}

void MainWindow::paste()
{
    activeMdiChild()->paste();
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About MDI QicsTable"),
            tr("The <b>MDI-QicsTable</b> example demonstrates how to use QicsTable in multiple "
               "document interface applications."));
}

void MainWindow::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != 0);
    m_saveAct->setEnabled(hasMdiChild);
    m_saveAsAct->setEnabled(hasMdiChild);
    m_pasteAct->setEnabled(hasMdiChild);
    m_closeAct->setEnabled(hasMdiChild);
    m_closeAllAct->setEnabled(hasMdiChild);
    m_tileAct->setEnabled(hasMdiChild);
    m_cascadeAct->setEnabled(hasMdiChild);
    m_nextAct->setEnabled(hasMdiChild);
    m_previousAct->setEnabled(hasMdiChild);
    m_separatorAct->setVisible(hasMdiChild);
}

void MainWindow::updateWindowMenu()
{
    m_windowMenu->clear();
    m_windowMenu->addAction(m_closeAct);
    m_windowMenu->addAction(m_closeAllAct);
    m_windowMenu->addSeparator();
    m_windowMenu->addAction(m_tileAct);
    m_windowMenu->addAction(m_cascadeAct);
    m_windowMenu->addSeparator();
    m_windowMenu->addAction(m_nextAct);
    m_windowMenu->addAction(m_previousAct);
    m_windowMenu->addAction(m_separatorAct);

    QList<QMdiSubWindow *> windows = m_workspace->subWindowList();
    m_separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());
        if ( !child ) printf("uh oh!\n");
        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1)
                               .arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1)
                              .arg(child->userFriendlyCurrentFile());
        }

        QAction *action  = m_windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), m_windowMapper, SLOT(map()));
        m_windowMapper->setMapping(action, windows.at(i));
    }
}

MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    m_workspace->addSubWindow(child);
    return child;
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
    m_workspace->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::createActions()
{
    m_newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    m_newAct->setShortcut(tr("Ctrl+N"));
    m_newAct->setStatusTip(tr("Create a new file"));
    connect(m_newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    m_openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    m_openAct->setShortcut(tr("Ctrl+O"));
    m_openAct->setStatusTip(tr("Open an existing file"));
    connect(m_openAct, SIGNAL(triggered()), this, SLOT(open()));

    m_saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    m_saveAct->setShortcut(tr("Ctrl+S"));
    m_saveAct->setStatusTip(tr("Save the document to disk"));
    connect(m_saveAct, SIGNAL(triggered()), this, SLOT(save()));

    m_saveAsAct = new QAction(tr("Save &As..."), this);
    m_saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(m_saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    m_exitAct = new QAction(tr("E&xit"), this);
    m_exitAct->setShortcut(tr("Ctrl+Q"));
    m_exitAct->setStatusTip(tr("Exit the application"));
    connect(m_exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    m_cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    m_cutAct->setShortcut(tr("Ctrl+X"));
    m_cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(m_cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    m_copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    m_copyAct->setShortcut(tr("Ctrl+C"));
    m_copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(m_copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    m_pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    m_pasteAct->setShortcut(tr("Ctrl+V"));
    m_pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(m_pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    m_closeAct = new QAction(tr("Cl&ose"), this);
    m_closeAct->setShortcut(tr("Ctrl+F4"));
    m_closeAct->setStatusTip(tr("Close the active window"));
    connect(m_closeAct, SIGNAL(triggered()),
            m_workspace, SLOT(closeActiveSubWindow()));

    m_closeAllAct = new QAction(tr("Close &All"), this);
    m_closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(m_closeAllAct, SIGNAL(triggered()),
            m_workspace, SLOT(closeAllSubWindows()));

    m_tileAct = new QAction(tr("&Tile"), this);
    m_tileAct->setStatusTip(tr("Tile the windows"));
    connect(m_tileAct, SIGNAL(triggered()), m_workspace, SLOT(tileSubWindows()));

    m_cascadeAct = new QAction(tr("&Cascade"), this);
    m_cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(m_cascadeAct, SIGNAL(triggered()), m_workspace, SLOT(cascadeSubWindows()));

    m_nextAct = new QAction(tr("Ne&xt"), this);
    m_nextAct->setShortcut(tr("Ctrl+F6"));
    m_nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(m_nextAct, SIGNAL(triggered()),
            m_workspace, SLOT(activateNextSubWindow()));

    m_previousAct = new QAction(tr("Pre&vious"), this);
    m_previousAct->setShortcut(tr("Ctrl+Shift+F6"));
    m_previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(m_previousAct, SIGNAL(triggered()),
            m_workspace, SLOT(activatePreviousSubWindow()));

    m_separatorAct = new QAction(this);
    m_separatorAct->setSeparator(true);

    m_aboutAct = new QAction(tr("&About"), this);
    m_aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAct);
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addAction(m_saveAct);
    m_fileMenu->addAction(m_saveAsAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_cutAct);
    m_editMenu->addAction(m_copyAct);
    m_editMenu->addAction(m_pasteAct);

    m_windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(m_windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAct);
}

void MainWindow::createToolBars()
{
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->addAction(m_newAct);
    m_fileToolBar->addAction(m_openAct);
    m_fileToolBar->addAction(m_saveAct);

    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->addAction(m_cutAct);
    m_editToolBar->addAction(m_copyAct);
    m_editToolBar->addAction(m_pasteAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("MDI Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings("MDI Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

MdiChild *MainWindow::activeMdiChild()
{
    if (m_workspace->activeSubWindow())
        return qobject_cast<MdiChild *>(m_workspace->activeSubWindow()->widget());

    return 0;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *subWindow, m_workspace->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(subWindow->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return subWindow;
    }

    return 0;
}
