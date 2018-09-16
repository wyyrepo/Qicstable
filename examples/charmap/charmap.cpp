/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "charmap.h"

#include <QStatusBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMenuBar>
#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>
#include <QClipboard>

#include <QicsWidgetCellDisplay.h>
#include <QicsMainGrid.h>
#include <QicsDataModelDefault.h>
#include <QicsTable.h>

#include "rubbergrid.h"


MainFrame::MainFrame (QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Character Map Example"));
    statusBar()->showMessage(tr("Ready"));

    /*
    *  GUI
    */

    // font combo box
    QWidget *wtop = new QWidget(this);
    QHBoxLayout *tophbox = new QHBoxLayout(wtop);
    QLabel *fontLabel = new QLabel(tr("Font:"), wtop);
    fontLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Expanding);

    QComboBox *fontCombo = new QComboBox(wtop);
    connect(fontCombo, SIGNAL(currentIndexChanged(const QString &)),
        this, SLOT(updateFont(const QString &)));

    tophbox->addWidget(fontLabel);
    tophbox->addWidget(fontCombo);

    // lineEdit and buttons
    QWidget *wbottom = new QWidget(this);
    QHBoxLayout *bottomhbox = new QHBoxLayout(wbottom);

    m_lineEdit = new QLineEdit(wbottom);

    QPushButton *selectButton = new QPushButton(QIcon(":/Resources/icon16_select.png"), tr("&Select"), wbottom);
    connect(selectButton, SIGNAL(clicked()), this, SLOT(select()));

    QPushButton *clipboardButton = new QPushButton(QIcon(":/Resources/icon16_paste.png"), tr("To &clipboard"), wbottom);
    connect(clipboardButton, SIGNAL(clicked()), this, SLOT(updateClipboard()));

    QPushButton *clipboardClearButton = new QPushButton(QIcon(":/Resources/icon16_clear.png"), tr("Clear"), wbottom);
    connect(clipboardClearButton, SIGNAL(clicked()), m_lineEdit, SLOT(clear()));

    bottomhbox->addWidget(new QLabel(tr("To copy:"), this));
    bottomhbox->addWidget(m_lineEdit);
    bottomhbox->addWidget(selectButton);
    bottomhbox->addWidget(clipboardButton);
    bottomhbox->addWidget(clipboardClearButton);

    /*
    *  Pulldown Menu
    */

    // Table
    QMenu* menuTable = new QMenu(tr("&Table"), this);
    menuTable->addAction(QIcon(":/Resources/icon16_select.png"), tr("Select Character"), this, SLOT(select()), Qt::Key_Enter);
    menuTable->addAction(QIcon(":/Resources/icon16_paste.png"), tr("Copy Selection to Clipboard"), this, SLOT(updateClipboard()), Qt::CTRL+Qt::Key_C);
    menuTable->addSeparator();
    menuTable->addAction(QIcon(":/Resources/icon16_clear.png"), tr("Clear Selection"), m_lineEdit, SLOT(clear()), Qt::CTRL+Qt::Key_N);
    menuTable->addSeparator();
    menuTable->addAction("E&xit", qApp, SLOT(closeAllWindows()), Qt::CTRL+Qt::Key_X);
    menuBar()->addMenu(menuTable);

    // Help
    QMenu* help = new QMenu(tr("&Help"), this);
    help->addAction(QIcon(":/Resources/icon16_help.png"), tr("About..."), this, SLOT(about()), Qt::CTRL+Qt::Key_H);
    menuBar()->addMenu(help);

    /*
    *  Table
    */

    // create the data model
    m_dm = new QicsDataModelDefault(2048,32);
    int cnt = 0;

    for(int row = 0; row < 2048; ++row)
        for(int col = 0; col < 32; ++col)
            m_dm->setItem(row, col, QChar(cnt++));

    // create the table, using the data model we created above
    m_table = new QicsTable(0, 0, RubberGrid::createGrid, 0, m_dm, this);
    m_table->setLeftHeaderVisible(false);
    m_table->setTopHeaderVisible(false);
    m_table->mainGridRef().setReadOnly(true);
    m_table->mainGridRef().setTextFlags(Qt::AlignCenter);
    m_table->setSelectionPolicy(Qics::SelectNone);
    m_table->setDefaultColumnWidthInPixels(24);
    m_table->setDefaultRowHeightInPixels(24);
    m_table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_table->show();
    connect(m_table, SIGNAL(doubleClicked(int,int,int,const QPoint &)), this, SLOT(cellClicked(int,int,int,const QPoint &)));
    connect(m_table, SIGNAL(currentCellChanged(int,int)), this, SLOT(cellChanged(int,int)));

    setCentralWidget(m_table);

    m_table->setTopTitleWidget(wtop);
    m_table->setBottomTitleWidget(wbottom);

    // init fonts
    QFontDatabase fontDatabase;
    QString family;
    foreach (family, fontDatabase.families())
        fontCombo->addItem(family);
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
        "This example represents Character Map based on QicsTable."
        "<p>"
        "<b>HOW TO USE</b><br>"
        "Select a font from the combo. Table will show all the chars available to select. "
        "<p>"
        "Double-click on a char or pressing Enter copies it to the line edit which could be exported to the clipboard. "
        "<hr>"
        "</td></table>"
        );
    helpDialog.setWindowTitle( tr("About Character Map Example") );
    helpDialog.setIcon( QMessageBox::NoIcon );
    helpDialog.exec();
}

void MainFrame::updateFont(const QString &fontName)
{
    m_table->mainGridRef().setFont(QFont(fontName, 12));
}

void MainFrame::updateClipboard()
{
    QApplication::clipboard()->setText(m_lineEdit->text(), QClipboard::Clipboard);
}

void MainFrame::cellClicked(int row, int col, int button, const QPoint &pos)
{
    Q_UNUSED(pos);
    if (button == Qt::LeftButton)
        m_lineEdit->insert(m_dm->itemString(row,col));
}

void MainFrame::select()
{
    const QicsCell *cell = m_table->currentCell();
    if (!cell) return;
    m_lineEdit->insert(m_dm->itemString(cell->modelRowIndex(), cell->modelColumnIndex()));
}

void MainFrame::cellChanged(int row, int col)
{
    quint32 n = (row*32)+col;
    QChar c(n);
    QString s = tr("Unicode: %1 (0x%2)   ASCII: %3")
        .arg(c.unicode())
        .arg(QString::number(c.unicode(),16))
        .arg(c.toLatin1());
    statusBar()->showMessage(s);
}


