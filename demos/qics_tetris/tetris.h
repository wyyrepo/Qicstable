/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef TETRIS_H
#define TETRIS_H

#include <QMainWindow>
#include <QEventLoop>

#include "form.h"

const int EMPTY_CELL = 0;
const int BUSY_CELL = 2;
const int MOVING_CELL = 1;
const int countColumnsTableField = 10;
const int countRowsTableField = 22;
const int diameterForm = 4;

class QLCDNumber;
class QPushButton;
class QLabel;
class QTimer;
class QicsTable;
class QicsCheckCellDisplay;
class QicsDataModel;
class QicsTextCellDisplay;
class QicsPushButtonCellDisplay;
class QicsComboCellDisplay;
class QicsCellDisplay;
class QicsDataModelDefault;
class QicsCell;
class ViewSettings;


class QicsTetris : public QMainWindow
{
    Q_OBJECT
public:
    QicsTetris(QWidget* parent = 0);
    ~QicsTetris();

    void initComponents();
    void setDisposition();
    void connectSlots();
    void checkAndDelRow();
    void setViewSettings();

    void changeScore();
    void createNewForm();
    void drawForm();
    void drawNextForm();
    void speedMore(int more = 1);		// if more = null - speed = 0
    void setSpeed(int speed);			// for fall down form (set but not display)
    void checkOnGameOver();
    void resetGame();
    void drawDemo(int fromRow, int fromColumn, int toRow, int toColumn, bool beginToEnd, QicsCellDisplay* display);

protected:
    void keyPressEvent(QKeyEvent *event);

protected slots:
    void startGame();
    void timerEvent();
    void pauseButtonEvent();
    void preferences();
    void timeout();

private:
    void sleep(int ms);

private:
    QEventLoop m_eventLoop;
    QicsTable* tableField;
    QicsTable* tableNextForm;
    QicsDataModelDefault* dmNextForm;
    QicsDataModel *dataModelField;
    QicsCellDisplay* pFieldCellDisplay;
    QicsCellDisplay* pFigureCellDisplay;
    QicsCheckCellDisplay* checkDisplay;
    QicsTextCellDisplay* textDisplay;
    QicsPushButtonCellDisplay* buttonDisplay;
    QicsComboCellDisplay* comboDisplay;
    QMenu* file;
    QMenu* view;
    QLCDNumber *scoreLcd;
    QLCDNumber *speedLcd;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QLabel* scoreLabel;
    QLabel* speedLabel;
    QColor* gridColor;
    QColor* backColor;
    QColor* figureColor;
    Form form;
    Form nextForm;
    QTimer* timerMove;
    QPoint* pArrCells;					// cells where form now
    QList <QicsCell*> cellRedrawArr;
    ViewSettings* viewSettings;
    bool isPause;
    bool isMoveDown;
    bool isGameOver;
    bool isRandomFigColor;
    int speed;
    int speedFall;
    int score;
    int stepChangeSpeed;
    int currIntervalMove;
    int startIntervalMove;
    int countRowsTableNextForm;
    int countColumnsTableNextForm;
};

#endif //TETRIS_H


