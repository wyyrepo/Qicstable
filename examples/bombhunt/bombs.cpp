/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "bombs.h"

#include <QTimer>
#include <QMenuBar>
#include <QApplication>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QPushButton>
#include <QMessageBox>

#include <QicsTextCellDisplay.h>
#include <QicsMainGrid.h>
#include <QicsDataModelDefault.h>
#include <QicsTable.h>

// constants for cell state
#define COUNT 0x0f				// 0000 1111	- mask for count cells
#define BOMB 0x10				// 0001 0000
#define FLAG 0x20				// 0010 0000
#define CLOSED 0x40				// 0100 0000
#define MARKED 0x80				// 1000 0000

// BombDisplayer class used to display cell state

class BombDisplayer : public QicsTextCellDisplay
{
public:
    BombDisplayer(QWidget *parent = 0) : QicsTextCellDisplay(parent) {}

protected:
    virtual void  drawBackground(QicsGridInfo *info, int row, int col, const QRect &rect, const QPalette &pal, QPainter *painter, bool is_current=false, bool is_selected=false)
    {
        QPalette cg1(pal);

        const QicsDataInt *di = (QicsDataInt*) info->cellValue(row,col);
        if (di && di->data() & CLOSED)
            cg1.setColor(QPalette::Base, Qt::yellow);

        QicsTextCellDisplay::drawBackground(info, row, col, rect, cg1, painter, is_current, is_selected);
    }

    virtual QString  textToDisplay (QicsGridInfo *, int , int , const QicsDataItem *itm) const
    {
        const QicsDataInt *di = (QicsDataInt*) itm;
        if (di && di->data() & COUNT)
            return itm->string();

        return QString();
    }

    virtual QPixmap  pixmapToDisplay (QicsGridInfo *info, int row, int col, const QicsDataItem *itm) const
    {
        const QicsDataInt *di = (QicsDataInt*) itm;
        if (di) {
            int i = di->data();

            if (i & BOMB && g_mainFrame->gameStatus() == Bombhunt::GS_OVER)
                return QPixmap(":/Resources/smallbomb.png");

            if (i & FLAG)
                return QPixmap(":/Resources/flag.png");

            if (i & MARKED)
                return QPixmap(":/Resources/greenflag.png");
        }
        return QicsTextCellDisplay::pixmapToDisplay(info, row, col, itm);
    }
};


// Main class

Bombhunt *g_mainFrame = 0;

Bombhunt::Bombhunt (QWidget *parent)
    : QMainWindow(parent)
{
    g_mainFrame = this;

    setWindowTitle(tr("Bombhunt Game Example"));

    m_in_game = GS_NONE;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(onTimer()));

    /*
    *  Pulldown Menu
    */

    // Game
    m_game = new QMenu(tr("&Game"), this);
    menuBar()->addMenu(m_game);

    QActionGroup *agGame = new QActionGroup(this);
    agGame->setExclusive(true);
    agGame->addAction(m_idBeg = m_game->addAction(tr("&Beginner"), this, SLOT(gameBeginner()), Qt::CTRL+Qt::Key_B));
    m_idBeg->setCheckable(true);
    agGame->addAction(m_idInt = m_game->addAction(tr("&Intermediate"), this, SLOT(gameIntermediate()), Qt::CTRL+Qt::Key_I));
    m_idInt->setCheckable(true);
    agGame->addAction(m_idAdv = m_game->addAction(tr("&Advanced"), this, SLOT(gameAdvanced()), Qt::CTRL+Qt::Key_A));
    m_idAdv->setCheckable(true);

    m_game->addSeparator();
    m_game->addAction(tr("E&xit"), qApp, SLOT(closeAllWindows()), Qt::CTRL+Qt::Key_X);

    // Help
    QMenu* help = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(help);

    help->addAction(QIcon(":/Resources/icon16_help.png"), tr("About Bomb Hunt..."), this, SLOT(aboutBombhunt()), Qt::CTRL+Qt::Key_H);

    /*
    *  Table
    */
    // create the data model
    m_dm = new QicsDataModelDefault(10,10);

    // create the table, using the data model we created above
    m_table = new QicsTable(m_dm, this);

    connect(m_table, SIGNAL(clicked(int,int,int,const QPoint &)), this, SLOT(cellClicked(int,int,int,const QPoint &)));

    // Set some visual resources on the table
    m_table->setTopHeaderVisible(false);
    m_table->setLeftHeaderVisible(false);
    m_table->setDefaultRowHeightInPixels(24);
    m_table->setDefaultColumnWidthInPixels(24);
    m_table->setSelectionPolicy(Qics::SelectNone);
    m_table->mainGridRef().setReadOnly(true);
    m_table->mainGridRef().setAlignment(Qt::AlignCenter);
    m_table->mainGridRef().setFont(QFont("Helvetica",12,QFont::Bold));
    m_table->setHScrollBarMode(Qics::AlwaysOff);
    m_table->setVScrollBarMode(Qics::AlwaysOff);

    //create top widget
    QWidget *topWidget = new QWidget(m_table);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);

    m_label = new QLCDNumber(8, topWidget);
    m_label->setToolTip(tr("Bombs left"));
    m_label->display(tr("PRESS---"));
    topWidgetLayout->addWidget(m_label);

    m_button = new QPushButton(topWidget);
    m_button->setIcon(QIcon(":/Resources/smallbomb.png"));
    m_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(m_button, SIGNAL(clicked()), SLOT(gameRun()));
    m_button->setToolTip(tr("Press to start a game"));
    topWidgetLayout->addWidget(m_button);

    m_time = new QLCDNumber(8, topWidget);
    m_time->setToolTip(tr("Time elapsed"));
    m_time->display(tr("---PRESS"));
    topWidgetLayout->addWidget(m_time);

    m_table->setTopTitleWidget(topWidget);
    m_table->setDisplayer(new BombDisplayer(m_table));

    setCentralWidget(m_table);

    gameBeginner();
}

void Bombhunt::gameBeginner()
{
    m_idBeg->setChecked(true);
    initModel(10,10,10);
}

void Bombhunt::gameIntermediate()
{
    m_idInt->setChecked(true);
    initModel(15,15,50);
}

void Bombhunt::gameAdvanced()
{
    m_idAdv->setChecked(true);
    initModel(20,20,100);
}

void Bombhunt::gameRun()
{
    if (m_in_game == GS_GAME) {
        if (QMessageBox::question(this, tr("Abort Game"), tr("Abort Current Game ?"), tr("Abort"), tr("Continue")) == 0)
            return;
    }
    initModel(m_tw, m_th, m_tb);
    m_table->hide();
    m_table->show();
    m_in_game = GS_GAME;
    m_timer->start(1000);
    m_timecount.setHMS(0,0,0);
    m_time->display("00.00.00");
}

void Bombhunt::gameOver(bool win)
{
    m_timer->stop();
    m_in_game = GS_OVER;
    //m_table->hide();
    //m_table->show();
    m_table->repaint();
    QMessageBox::information(this, tr("Game Over"), win ? tr("Congratulations! You're WINNER!") : tr("You've exploded the BOMB! Game Over."));
}


void Bombhunt::aboutBombhunt()
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
        "<p>This program uses QicsTable in a simple (but somewhat addicting) game. "
        "BombHunt takes logic and just a little luck to win!"
        "<p>"
        "Your mission, should you choose to accept it, is to find and mark all of "
        "the bombs hidden under the squares on the screen - in the shortest time possible."
        "<p>"
        "<b>SCREEN LAYOUT</b><br>"
        "The menus in the menu bar allow you to select game difficulty, start a "
        "new game, view online help, and exit the program."
        "<p>"
        "The number at the upper left displays the number of bombs remaining to "
        "be found.  The number at the upper right displays the elapsed time."
        "<p>"
        "The bomb graphic at the top of the window indicates your progress. "
        "You can click on it at any time to start a new game."
        "<p>"
        "<b>HOW TO PLAY</b><br>"
        "Click the left mouse button on a square to uncover it and see its contents.  If you "
        "uncover an empty square, BombHunt clears all of the empty squares around it. "
        "If you uncover a square that is next to a bomb, it displays the total number of bombs "
        "adjacent to it.  A square can have 8 squares adjacent to it.  If you uncover a bomb, "
        "it explodes - and the game is over."
        "<p>"
        "When you determine that a square contains a bomb, click on it with the "
        "right mouse button to flag its location.  The remaining bombs display "
        "decreases by one."
        "<hr>"
        "</td></table>"
        );
    helpDialog.setWindowTitle(tr("About BombHunt Game Example") );
    helpDialog.setIcon( QMessageBox::NoIcon );
    helpDialog.exec();
}

void Bombhunt::initModel(int x, int y, int bombs)
{
    m_table->setRepaintBehavior(Qics::RepaintOff);
    m_timer->stop();
    m_in_game = GS_INIT;

    if (m_tw != x || m_th != y) {
        m_dm->clearModel();
        m_dm->addRows(x);
        m_dm->addColumns(y);
    }

    m_tw = x;
    m_th = y;
    m_bombcount = m_tb = bombs;
    m_label->display(bombs);

    for(int row = 0; row < x; ++row) {
        for(int col = 0; col < y; ++col) {
            m_dm->setItem(row, col, CLOSED);
        }
    }

    /* Seed the random-number generator with current time so that
    * the numbers will be different every time we run.
    */
    srand( QDateTime::currentDateTime().toTime_t() );

    for (int i = 0; i < bombs; ++i) {
        int r = rand() % x;
        int c = rand() % y;
        const QicsDataInt *di = (QicsDataInt*) m_dm->item(r,c);
        int j = di->data();
        if (j & BOMB)
            i--;
        else
            m_dm->setItem(r,c,j | BOMB);
    }

    m_table->setRepaintBehavior(Qics::RepaintOn);

    resize(24*x+40, 24*y+100);
}

void Bombhunt::cellClicked(int row, int col, int button, const QPoint &)
{
    if (m_in_game != GS_GAME) return;

    int i = getCell(row, col);

    if (button == Qt::RightButton) {
        if (i & CLOSED) {
            // check for a good mark
            if (i & BOMB && canMark(row, col)) {
                m_dm->setItem(row, col, BOMB | MARKED);
                m_label->display(--m_bombcount);
                if (!m_bombcount) {
                    // game over - WIN
                    gameOver(true);
                    return;
                }
            } else
                m_dm->setItem(row, col, i & FLAG ? i &~FLAG : i | FLAG);
        }
        return;
    }

    if (button == Qt::LeftButton) {
        if (i & BOMB) {
            // game over - LOSE
            gameOver();
            return;
        }
        m_table->setRepaintBehavior(Qics::RepaintOff);
        checkCell(row, col);
        m_table->setRepaintBehavior(Qics::RepaintOn);
    }
}

void Bombhunt::onTimer()
{
    m_timecount = m_timecount.addSecs(1);
    m_time->display(m_timecount.toString("hh.mm.ss"));
}

int Bombhunt::getCell(int row, int col)
{
    const QicsDataInt *di = (QicsDataInt*) m_dm->item(row, col);
    return di ? di->data() : -1;
}

void Bombhunt::checkCell(int row, int col)
{
    if (row < 0 || col < 0 || row >= m_tw || col >= m_th) return;
    if (!(getCell(row, col) & CLOSED)) return;

    int count = 0;
    if (isBomb(row-1,col)) count++;
    if (isBomb(row-1,col-1)) count++;
    if (isBomb(row-1,col+1)) count++;
    if (isBomb(row+1,col)) count++;
    if (isBomb(row+1,col+1)) count++;
    if (isBomb(row+1,col-1)) count++;
    if (isBomb(row,col-1)) count++;
    if (isBomb(row,col+1)) count++;
    m_dm->setItem(row, col, count);
    if (!count) {
        // expose around...
        checkCell(row-1,col);
        checkCell(row-1,col-1);
        checkCell(row-1,col+1);
        checkCell(row+1,col);
        checkCell(row+1,col+1);
        checkCell(row+1,col-1);
        checkCell(row,col-1);
        checkCell(row,col+1);
    }
}

bool Bombhunt::isBomb(int row, int col)
{
    if (row < 0 || col < 0 || row >= m_tw || col >= m_th) return false;
    int i = getCell(row,col);
    return (i & BOMB);
}

bool Bombhunt::isBombOrOpen(int row, int col)
{
    if (row < 0 || col < 0 || row >= m_tw || col >= m_th) return true;
    int i = getCell(row,col);
    return (i & BOMB) || !(i & CLOSED);
}

bool Bombhunt::canMark(int row, int col)
{
    if (!isBombOrOpen(row-1,col)) return false;
    if (!isBombOrOpen(row-1,col-1)) return false;
    if (!isBombOrOpen(row-1,col+1)) return false;
    if (!isBombOrOpen(row+1,col)) return false;
    if (!isBombOrOpen(row+1,col+1)) return false;
    if (!isBombOrOpen(row+1,col-1)) return false;
    if (!isBombOrOpen(row,col-1)) return false;
    if (!isBombOrOpen(row,col+1)) return false;
    return true;
}


