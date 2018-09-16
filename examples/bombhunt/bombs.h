/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef BOMBS_H
#define BOMBS_H

#include <QMainWindow>
#include <QTime>

class QicsDataModel;
class QicsTable;
class QLCDNumber;
class QPushButton;


class Bombhunt : public QMainWindow
{
    Q_OBJECT
public:
    Bombhunt(QWidget * parent = 0);

    enum GameStatus { GS_NONE, GS_INIT, GS_GAME, GS_OVER };
    inline GameStatus gameStatus() const { return m_in_game; }

private slots:
    void gameBeginner();
    void gameIntermediate();
    void gameAdvanced();
    void gameRun();
    void gameOver(bool win = false);
    void aboutBombhunt();

    void cellClicked(int,int,int,const QPoint &);
    void onTimer();
    void checkCell(int row, int col);
    int  getCell(int row, int col);
    bool isBomb(int row, int col);
    bool isBombOrOpen(int row, int col);
    bool canMark(int row, int col);

private:
    void initModel(int x, int y, int bombs);

private:
    QicsDataModel *m_dm;
    QicsTable *m_table;
    QMenu* m_game;
    QLCDNumber *m_label;
    QLCDNumber *m_time;
    QPushButton	*m_button;
    QTimer *m_timer;
    QTime m_timecount;
    QAction *m_idBeg;
    QAction *m_idInt;
    QAction *m_idAdv;
    GameStatus m_in_game;
    int m_tw;
    int m_th;
    int m_tb;
    int m_bombcount;
};

extern Bombhunt *g_mainFrame;

#endif //BOMBS_H


