/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "imageviewer.h"

#include <QImageReader>
#include <QProgressBar>
#include <QStatusBar>
#include <QPushButton>
#include <QMenuBar>
#include <QSplitter>
#include <QApplication>
#include <QDirModel>
#include <QTreeView>
#include <QMessageBox>
#include <QScrollArea>
#include <QLabel>

#include <QicsTable.h>
#include <QicsDataModelDefault.h>


// Main class
ImageViewer::ImageViewer (QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Image Viewer Example"));

    m_scanning = m_stop = false;

    QList<QByteArray> fl = QImageReader::supportedImageFormats();
    for (int i = 0; i < fl.size(); ++i)
        m_filter.append("*." + fl.at(i));

    m_bar = new QProgressBar();
    statusBar()->addPermanentWidget(m_bar);

    QPushButton *btnStop = new QPushButton();
    btnStop->setText(tr("Stop"));
    connect(btnStop, SIGNAL(pressed()), this, SLOT(stopScan()));
    statusBar()->addPermanentWidget(btnStop);

    /*
    *  Pulldown Menu
    */

    // File
    QMenu* menuFile = new QMenu(tr("&File"), this);
    menuFile->addAction(tr("E&xit"), qApp, SLOT(closeAllWindows()), Qt::CTRL+Qt::Key_X);
    menuBar()->addMenu(menuFile);

    // Help
    QMenu* help = new QMenu(tr("&Help"), this);
    help->addAction(tr("View Formats..."), this, SLOT(formats()), Qt::CTRL+Qt::Key_F);
    help->addSeparator();
    help->addAction(QIcon(":/Resources/icon16_help.png"), tr("About..."), this, SLOT(about()), Qt::CTRL+Qt::Key_H);
    menuBar()->addMenu(help);

    QSplitter *splitter = new QSplitter(this);

    /*
    *  Browser
    */

    m_dirmodel = new QDirModel(this);
    m_dirmodel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    m_dirtree = new QTreeView(this);
    m_dirtree->setModel(m_dirmodel);
    m_dirtree->resize(400,200);
    m_dirtree->setColumnHidden(1,true);
    m_dirtree->setColumnHidden(2,true);
    m_dirtree->setColumnHidden(3,true);
    connect(m_dirtree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(dirSelected(const QModelIndex&)));

    /*
    *  Table
    */

    // create the data model
    m_dm = new QicsDataModelDefault(0,4);

    // create the table, using the data model we created above
    m_table = new QicsTable(m_dm, this);
    m_table->setLeftHeaderVisible(false);
    m_table->setSelectionPolicy(Qics::SelectSingleRow);
    m_table->mainGridRef().setReadOnly(true);
    m_table->mainGridRef().setAlignment(Qt::AlignCenter);
    m_table->mainGridRef().setTextFlags(Qt::TextWrapAnywhere);
    m_table->columnHeaderRef().columnRef(0).setLabel("Image");
    m_table->columnHeaderRef().columnRef(1).setLabel("Name");
    m_table->columnHeaderRef().columnRef(2).setLabel("Size");
    m_table->columnHeaderRef().columnRef(3).setLabel("Info");

    connect(m_table, SIGNAL(doubleClicked(int,int,int,const QPoint &)), this, SLOT(cellClicked(int,int,int,const QPoint &)));

    splitter->addWidget(m_dirtree);
    splitter->addWidget(m_table);

    setCentralWidget(splitter);
}

void ImageViewer::about()
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
        "This program uses QicsTable to browse images. "
        "<p>"
        "<b>HOW TO USE</b><br>"
        "<p>In the left window, select a directory which contains image files. "
        "If exist, images thumbnails and info will be shown in the table. "
        "<p>Double click on an image to open it in a new window. "
        "<hr>"
        "</td></table>"
        );
    helpDialog.setWindowTitle( tr("About Image Viewer Example") );
    helpDialog.setIcon( QMessageBox::NoIcon );
    helpDialog.exec();
}


void ImageViewer::cellClicked(int row, int col, int button, const QPoint &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(col);

    if (button != Qt::LeftButton) return;

    QString name = m_dirName + "/" + m_dm->itemString(row,1);
    QPixmap img(name);
    if (img.isNull()) return;

    QScrollArea *sa = new QScrollArea();
    sa->setAttribute(Qt::WA_DeleteOnClose);
    sa->setWindowModality(Qt::ApplicationModal);
    QLabel *l = new QLabel(sa);
    l->setPixmap(img);
    sa->setWidget(l);
    sa->setWindowTitle(name);
    sa->show();
}

void ImageViewer::stopScan()
{
    m_stop = true;
}

void ImageViewer::dirSelected(const QModelIndex& index)
{
    if (m_scanning)
        return;

    m_scanning = true;

    m_dm->deleteRows(m_dm->numRows(), 0);

    QDir dir(m_dirName = m_dirmodel->filePath(index));
    QFileInfoList fil = dir.entryInfoList(m_filter, QDir::Files);
    int cnt = fil.size();
    if (cnt) {
        statusBar()->showMessage(tr("Images: %1").arg(cnt));
        m_bar->setRange(0, cnt-1);

        m_dm->addRows(cnt);
        // ? falls with exception here...
        //table->repeatingRowRef(0,2).setBackgroundColor(QColor(0xf0f0f0));

        for (int i = 0; i < fil.size(); ++i) {
            const QFileInfo fi = fil.at(i);
            m_dm->setItem(i,1,fi.fileName());
            m_dm->setItem(i,2,fi.size());

            QPixmap img(fi.filePath());
            m_dm->setItem(i,3,QString("%1 x %2, %3 bit")
                .arg(img.width()).arg(img.height()).arg(img.depth()));

            if (img.width() > 128)
                img = img.scaledToWidth(128);

            m_table->cell(i,0)->setPixmap(img);
            m_table->cell(i,0)->setHeightInPixels(img.height()+4);
            m_table->cell(i,0)->setWidthInPixels(132);

            if (i&1)
                m_table->rowRef(i).setBackgroundColor(QColor(0xf0f0f0));

            m_bar->setValue(i);
            qApp->processEvents();

            if (m_stop) {
                m_stop = false;
                break;
            }
        }
    } else
        statusBar()->showMessage(tr("This directory does not contain image files that can be viewed."));

    //qDebug(s.toLocal8Bit().constData());

    m_scanning = false;
}

void ImageViewer::formats()
{
    QString f;
    QList<QByteArray> fl = QImageReader::supportedImageFormats();

    for (int i = 0; i < fl.size(); ++i)
        f.append(QString(fl.at(i))+"\n\r");

    QMessageBox formatDialog(this);
    formatDialog.setText(tr("Graphic formats avalaible to view:\n\r\n\r") + f);
    formatDialog.setWindowTitle(tr("Formats"));
    formatDialog.setIcon( QMessageBox::NoIcon );
    formatDialog.exec();
}

void ImageViewer::closeEvent (QCloseEvent * event)
{
    m_stop = true;
    QTimer::singleShot(0, qApp, SLOT(closeAllWindows()));
    event->accept();
}


