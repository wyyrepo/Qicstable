/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "iconedit.h"

#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>

#include <QicsTable.h>
#include <QicsDataModelDefault.h>

#include "colorpanel.h"
#include "zoompanel.h"
#include "newdialog.h"
#include "rubbergrid.h"


// main window class
MainFrame::MainFrame(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Icon Editor Example"));
    statusBar()->showMessage(tr("Ready"));

    /*
    *  Pulldown Menu
    */

    // File
    QMenu* menuFile = new QMenu(tr("&File"), this);
    QAction* actNew = menuFile->addAction(QIcon(":/Resources/icon16_new.png"), tr("&New..."), this, SLOT(newFile()), Qt::CTRL+Qt::Key_N);
    actNew->setToolTip("Create new (empty) icon");
    QAction* actOpen = menuFile->addAction(QIcon(":/Resources/icon16_open.png"), tr("&Open..."), this, SLOT(load()), Qt::CTRL+Qt::Key_O);
    actOpen->setToolTip("Open existing icon from file");
    QAction* actSave = menuFile->addAction(QIcon(":/Resources/icon16_save.png"), tr("&Save As..."), this, SLOT(save()), Qt::CTRL+Qt::Key_S);
    actSave->setToolTip(tr("Save icon to file"));
    menuFile->addSeparator();
    menuFile->addAction(tr("E&xit"), qApp, SLOT(closeAllWindows()), Qt::CTRL+Qt::Key_X);
    menuBar()->addMenu(menuFile);

    // Image
    QMenu* menuImage = new QMenu(tr("&Image"), this);
    QAction* actClear = menuImage->addAction(QIcon(":/Resources/icon16_clear.png"), tr("&Clear"), this, SLOT(clearImage()), Qt::CTRL+Qt::Key_R);
    actClear->setToolTip(tr("Clear icon (fill with background color)"));

    menuBar()->addMenu(menuImage);

    // Help
    QMenu* help = new QMenu(tr("&Help"), this);
    QAction* actHelp = help->addAction(QIcon(":/Resources/icon16_help.png"), tr("About..."), this, SLOT(about()), Qt::CTRL+Qt::Key_H);
    menuBar()->addMenu(help);

    /*
    *  Toolbar
    */
    QToolBar *mainToolbar = addToolBar("ToolBar");
    mainToolbar->addAction(actNew);
    mainToolbar->addAction(actOpen);
    mainToolbar->addAction(actSave);
    mainToolbar->addSeparator();
    mainToolbar->addAction(actClear);
    mainToolbar->addSeparator();
    mainToolbar->addAction(actHelp);

    /*
    *  Table
    */

    // create the data model
    m_dm = new QicsDataModelDefault();

    // create the table
    m_table = new QicsTable(0, 0, RubberGrid::createGrid, 0, m_dm);
    m_table->mainGridRef().setReadOnly(true);
    m_table->setSelectionPolicy(Qics::SelectNone);
    m_table->setTopHeaderVisible(false);
    m_table->setLeftHeaderVisible(false);

    connect(m_table, SIGNAL(pressed(int,int,int,const QPoint&)), this, SLOT(cellClicked(int,int,int,const QPoint&)));
    connect(m_table, SIGNAL(currentCellChanged(int,int)), this, SLOT(cellChanged(int,int)));

    setCentralWidget(m_table);

    /*
    *  Panels
    */

    m_zpanel = new ZoomPanel(this);
    connect(this, SIGNAL(iconChanged(const QImage&)), m_zpanel, SLOT(setIcon(const QImage&)));
    connect(m_zpanel, SIGNAL(zoomChanged(int)), this, SLOT(setZoom(int)));
    addDockWidget(Qt::RightDockWidgetArea, m_zpanel);

    m_cpanel = new ColorPanel(this);
    addDockWidget(Qt::RightDockWidgetArea, m_cpanel);

    createGrid(16,16);
    setZoom(m_zpanel->currentZoom());
}

MainFrame::~MainFrame()
{
}

void MainFrame::about()
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
        "This example uses QicsTable to introduce simple icon editor."
        "<p>"
        "<b>HOW TO USE</b><br>"
        "Use 'File'->'New...' to create new icon.<br>"
        "Use 'File'->'Open...' to load graphic icon image from file.<br>"
        "Use 'File'->'Save As...' to save current image to file.<br><br>"
        "Use 'Image'->'Clear' to clear image (fill with background color).<br>"
        "<p>"
        "To edit an icon, use left mouse button to fill a cell with foreground color, right button to fill a cell with background color."
        "<p>"
        "On Zoom Panel, you can preview your editing in 1:1 scale.<br>"
        "Also you can set zoom factor here.<br>"
        "<p>"
        "On Color Panel, you can select foreground and background colors from swatches table, "
        "enter values directly or click color button to pick a color via dialog.<br>"
        "Use left mouse click on a swatch to select foreground color, right click to select background color."
        "<hr>"
        "</td></table>"
        );
    helpDialog.setWindowTitle( "About Icon Editor Example" );
    helpDialog.setIcon( QMessageBox::NoIcon );
    helpDialog.exec();
}


void MainFrame::cellClicked(int row, int col, int button, const QPoint &pos)
{
    Q_UNUSED(pos);
    QColor c;
    if (button == Qt::LeftButton)
        c = m_cpanel->color();
    else
        if (button == Qt::RightButton)
            c = m_cpanel->colorBG();
        else
            return;

    m_table->cell(row, col)->setBackgroundColor(c);
    m_icon.setPixel(col,row,c.rgb());

    emit iconChanged(m_icon);

    cellChanged(row, col);
}

void MainFrame::cellChanged (int row, int col)
{
    statusBar()->showMessage(QString("X: %1   Y: %2").arg(col).arg(row));
}

void MainFrame::newFile()
{
    // show size dialog
    NewDialog nd(this);
    if (nd.exec())
        createGrid(nd.width(), nd.height());
}

void MainFrame::createGrid(int w, int h)
{
    m_table->clearTable();
    m_dm->addColumns(w);
    m_dm->addRows(h);
    m_table->setRepaintBehavior(Qics::RepaintOff);
    m_table->mainGridRef().setBackgroundColor(m_cpanel->colorBG());
    m_table->setRepaintBehavior(Qics::RepaintOn);
    m_icon = QImage(w, h, QImage::Format_RGB32);
    m_icon.fill(m_cpanel->colorBG().rgb());
    emit iconChanged(m_icon);
}

void MainFrame::setZoom(int z)
{
    m_table->setRepaintBehavior(Qics::RepaintOff);
    m_table->setDefaultColumnWidthInPixels(z);
    m_table->setDefaultRowHeightInPixels(z);
    m_table->setRepaintBehavior(Qics::RepaintOn);
}

void MainFrame::clearImage()
{
    if (QMessageBox::question(this, tr("Clear Image"), tr("Do you want to clear entire image?"), QMessageBox::Yes | QMessageBox::No) == 0) {
        m_table->mainGridRef().setBackgroundColor(m_cpanel->colorBG());
        m_icon.fill(m_cpanel->colorBG().rgb());
        emit iconChanged(m_icon);
    }
}

void MainFrame::save()
{
    QString filt;
    QString s = QFileDialog::getSaveFileName(this, tr("Save Icon as..."), ".",
        tr("Images (*.png *.xpm *.jpg *.bmp)"), &filt);

    if (!s.isEmpty()) {
        filt = filt.remove(0,1);
        if (!s.endsWith(filt, Qt::CaseInsensitive))
            s.append(filt);
        filt = filt.remove(0,1);
        m_icon.save(s, filt.toLocal8Bit().data());
    }
}

void MainFrame::load()
{
    QString s = QFileDialog::getOpenFileName(this, tr("Open Icon from File"), ".",
        tr("Images (*.png *.xpm *.jpg *.bmp)"));

    if (!s.isEmpty()) {
        m_icon.load(s);
        int w = m_icon.width();
        int h = m_icon.height();
        m_table->setRepaintBehavior(Qics::RepaintOff);
        m_table->clearTable();
        m_dm->addColumns(w);
        m_dm->addRows(h);
        for (int x = 0; x < w; ++x)
            for (int y = 0; y < h; ++y)
                m_table->cell(y, x)->setBackgroundColor(m_icon.pixel(x,y));
        m_table->setRepaintBehavior(Qics::RepaintOn);
        emit iconChanged(m_icon);
    }
}


