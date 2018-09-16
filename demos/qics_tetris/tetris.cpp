/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "tetris.h"

#include <QLCDNumber>
#include <QTimer>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QicsDataModelDefault.h>
#include <QicsTextCellDisplay.h>
#include <QicsCheckCellDisplay.h>
#include <QicsPushButtonCellDisplay.h>
#include <QicsComboCellDisplay.h>
#include <QicsMainGrid.h>
#include <QicsDataItemFormatter.h>
#include <QicsTable.h>
#include <QicsCell.h>
#include <QicsRow.h>
#include "viewOptionsDialog.h"
#include "viewSettings.h"


QicsTetris::QicsTetris(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Tetris Game Demo"));

    qsrand(QDateTime::currentDateTime().toTime_t());
    initComponents();
    setDisposition();
    connectSlots();
}

QicsTetris::~QicsTetris()
{
}

void QicsTetris::initComponents()
{
    countRowsTableNextForm = diameterForm;
    countColumnsTableNextForm = diameterForm;
    // create data model field
    dataModelField = new QicsDataModelDefault(countRowsTableField, countColumnsTableField);
    for (int i = 0; i < countRowsTableField; ++i)
        for (int j = 0; j < countColumnsTableField; ++j)
            dataModelField->setItem(i, j, QicsDataInt(EMPTY_CELL));

    // create displayers
    checkDisplay = new QicsCheckCellDisplay(this);
    textDisplay = new QicsTextCellDisplay(this);
    buttonDisplay = new QicsPushButtonCellDisplay(this);
    comboDisplay = new QicsComboCellDisplay(this);
    // set by default
    pFieldCellDisplay = textDisplay;
    pFigureCellDisplay = textDisplay;
    // create tableField
    tableField = new QicsTable(dataModelField);
    tableField->setDefaultRowHeightInPixels(21);
    tableField->setDefaultColumnWidthInPixels(21);
    tableField->setTopHeaderVisible(false);
    tableField->setLeftHeaderVisible(false);
    tableField->setVScrollBarMode(Qics::AlwaysOff);
    tableField->setHScrollBarMode(Qics::AlwaysOff);
    tableField->setFixedSize(229, 493);
    tableField->setFocusPolicy(Qt::NoFocus);
    tableField->setEnabled(false);
    // set view settings
    gridColor = new QColor;
    backColor = new QColor;
    figureColor = new QColor;
    isRandomFigColor = true;
    viewSettings = new ViewSettings();
    // set formater for cells
    QicsDataItemSprintfFormatter* f = new QicsDataItemSprintfFormatter();
    f->addFormatString(QicsDataItem_Int, " ");
    tableField->mainGridRef().setFormatter(f);
    // grid pan
    QPen* pen = new QPen(backColor->rgb());
    tableField->setHorizontalGridLinePen(*pen);
    tableField->setVerticalGridLinePen(*pen);
    // create tableNextForm
    dmNextForm = new QicsDataModelDefault(countRowsTableNextForm, countColumnsTableNextForm);
    tableNextForm = new QicsTable(dmNextForm);
    tableNextForm->setDefaultRowHeightInPixels(21);
    tableNextForm->setDefaultColumnWidthInPixels(21);
    tableNextForm->setTopHeaderVisible(false);
    tableNextForm->setLeftHeaderVisible(false);
    tableNextForm->setVScrollBarMode(Qics::AlwaysOff);
    tableNextForm->setHScrollBarMode(Qics::AlwaysOff);
    tableNextForm->setFixedSize(97, 97);
    tableNextForm->setEnabled(false);
    // create widgets information
    scoreLcd = new QLCDNumber();
    scoreLcd->setFixedSize(92, 21);
    speedLcd = new QLCDNumber();
    speedLcd->setFixedSize(92, 21);

    scoreLabel = new QLabel("Score:");
    scoreLabel->setFixedSize(92, 21);
    speedLabel = new QLabel("Speed:");
    speedLabel->setFixedSize(92, 21);

    startButton = new QPushButton("Start");
    startButton->setFixedSize(92, 21);
    pauseButton = new QPushButton("Pause");
    pauseButton->setFixedSize(92, 21);

    timerMove = new QTimer();
    isPause = false;
    isGameOver = true;
    isMoveDown = true;
    speedFall = 15;

    speed = 0;
    score = 0;
    stepChangeSpeed = 39;
    startIntervalMove = 600;
    backColor = new QColor(255, 255, 255);

    // create menu
    file = new QMenu(tr("&File"),this);
    menuBar()->addMenu(file);
    file->addAction(tr("E&xit"), qApp, SLOT(closeAllWindows()), QKeySequence("Ctrl+x"));
    view = new QMenu(tr("&View"),this);
    menuBar()->addMenu(view);
    view->addAction(tr("&Preferences"), this, SLOT(preferences()), QKeySequence("Ctrl+P"));

    setViewSettings();
}

void QicsTetris::setDisposition()
{
    // field
    QVBoxLayout *vboxLayoutField = new QVBoxLayout();
    vboxLayoutField->addWidget(tableField);

    // info
    QVBoxLayout *vboxLayoutInfo = new QVBoxLayout();
    vboxLayoutInfo->addWidget(tableNextForm, 0, Qt::AlignTop);
    vboxLayoutInfo->addWidget(scoreLabel, 0, Qt::AlignTop);
    vboxLayoutInfo->addWidget(scoreLcd, 0, Qt::AlignTop);
    vboxLayoutInfo->addWidget(speedLabel, 0, Qt::AlignTop);
    vboxLayoutInfo->addWidget(speedLcd, 0, Qt::AlignTop);

    vboxLayoutInfo->addStretch();

    vboxLayoutInfo->addWidget(startButton, 0, Qt::AlignTop);
    vboxLayoutInfo->addWidget(pauseButton, 0, Qt::AlignTop);

    // main
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(vboxLayoutField);
    mainLayout->addLayout(vboxLayoutInfo);
    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    nextForm.setRandomShape();
    if (isRandomFigColor)
        nextForm.setRandomColor(backColor);
    else
        nextForm.setColor(figureColor);
}

void QicsTetris::connectSlots()
{
    connect (startButton, SIGNAL(clicked()), this, SLOT(startGame()));
    connect (timerMove, SIGNAL(timeout()), this, SLOT(timerEvent()));
    connect (pauseButton, SIGNAL(clicked()), this, SLOT(pauseButtonEvent()));
}

void QicsTetris::startGame()
{
    setFocus();
    if (!isPause)
        timerMove->stop();

    score = 0;
    changeScore();
    speedMore(0);           // set 0 speed
    isPause = false;
    isGameOver = false;

    // clear data model and draw demo
    resetGame();

    timerMove->start();
    createNewForm();        // new form
}

void QicsTetris::timerEvent()
{
    isMoveDown = form.moveDown(dataModelField);
    if (!isMoveDown)
        createNewForm();

    if (!isGameOver)
        drawForm();
}

void QicsTetris::createNewForm()
{
    form = nextForm;
    form.firstLocate();
    drawForm();
    isMoveDown = true;
    nextForm.setRandomShape();
    nextForm.firstLocateNextForm();
    if (isRandomFigColor)
        nextForm.setRandomColor(backColor);
    else
        nextForm.setColor(figureColor);

    if (!isGameOver)
        drawNextForm();
}

void QicsTetris::drawForm()
{
    if (isMoveDown) {
        // if form move - redraw place where form was
        for (int i = 0; i < cellRedrawArr.size(); ++i) {
            cellRedrawArr.value(i)->setBackgroundColor(*backColor);
            dataModelField->setEmitSignals(false);// only for speed redraw
            dataModelField->setItem(cellRedrawArr.value(i)->rowIndex(), cellRedrawArr.value(i)->columnIndex(), QicsDataInt(EMPTY_CELL));
            tableField->cellRef(cellRedrawArr.value(i)->rowIndex(),  cellRedrawArr.value(i)->columnIndex()).setDisplayer(pFieldCellDisplay);
            dataModelField->setEmitSignals(true);// only for speed redraw
        }
    }
    else {
        // set value for cells that are busy and not move
        for (int i = 0; i < cellRedrawArr.size(); ++i) {
            dataModelField->setEmitSignals(false);
            dataModelField->setItem(cellRedrawArr.value(i)->rowIndex(), cellRedrawArr.value(i)->columnIndex(), QicsDataInt(BUSY_CELL));
            dataModelField->setEmitSignals(true);
        }
        checkAndDelRow();
        checkOnGameOver();
        setSpeed(speed);        // set current speed andter full down
        if (isGameOver) {
            // if game over - do not draw form
            cellRedrawArr.clear();
            return;
        }
    }

    for (int i = 0; i < cellRedrawArr.size(); ++i)
        delete cellRedrawArr[i];

    cellRedrawArr.clear();
    // draw form on new place
    int row, col;
    pArrCells = form.GetArrPoints();
    QPoint* tmpPoint;

    for (int i = 0; i < diameterForm; ++i) {
        tmpPoint = &pArrCells[i];
        if (tmpPoint->x() != -1) {
            row = tmpPoint->y();
            col = tmpPoint->x();
            tableField->cellRef(row, col).setBackgroundColor(form.color());
            cellRedrawArr.push_back(tableField->cell(row, col));
            // set cell that moving shape here
            dataModelField->setEmitSignals(false);
            dataModelField->setItem(row, col, QicsDataInt(MOVING_CELL));
            tableField->cellRef(row, col).setDisplayer(pFigureCellDisplay);
            dataModelField->setEmitSignals(true);
        }
    }
}

void QicsTetris::drawNextForm()
{
    QPoint* pArrForm;
    QPoint* tmpPoint;
    int row, col;
    // redraw background
    for (int i = 0; i < diameterForm; ++i)
        for (int j = 0; j < diameterForm; ++j) {
            tableNextForm->cellRef(i, j).setBackgroundColor(*backColor);
            tableNextForm->cellRef(i, j).setDisplayer(pFieldCellDisplay);
        }
    // draw form
    pArrForm = nextForm.GetArrPoints();
    for (int i = 0; i < diameterForm; ++i) {
        tmpPoint = &pArrForm[i];
        row = tmpPoint->y();
        col = tmpPoint->x();
        tableNextForm->cellRef(row, col).setBackgroundColor(nextForm.color());
        tableNextForm->cellRef(row, col).setDisplayer(pFigureCellDisplay);
    }
    pArrForm = form.GetArrPoints();
}

void QicsTetris::checkOnGameOver()
{
    for (int i = 0; i < countColumnsTableField; ++i) {
        if (dataModelField->item(0, i)->string().toInt() == BUSY_CELL) {
            resetGame();
            isGameOver = true;
            break;
        }
    }
}

void QicsTetris::checkAndDelRow()
{
    int countBusyCellsInRow = 0;
    int countDelRows = 0;
    for (int i = countRowsTableField - 1; i >= 0; --i) {
        countBusyCellsInRow = 0;
        for (int j = 0; j < countColumnsTableField; ++j) {
            if (dataModelField->item(i, j)->string().toInt() == BUSY_CELL)
                countBusyCellsInRow++;
        }

        if (countBusyCellsInRow == countColumnsTableField) { // if busy all cells in one row
            tableField->deleteRow(i);                       // delete this row
            countDelRows++;
            score += 100;                                   // change score
            changeScore();
            if (!(score % 1000))
                speedMore();
        }
        else if (!countBusyCellsInRow)                      // if all cells in row are free -
            // breake becouse all next top row are free
            break;
    }

    for (int i = 0; i < countDelRows; ++i) {
        tableField->insertRow(0);                           // push new in front
        tableField->rowRef(0).setBackgroundColor(*backColor);
        for (int k = 0; k < countColumnsTableField; ++k)    // fill dataModel for new row
            dataModelField->setItem(0, k, QicsDataInt(EMPTY_CELL));
    }
}

void QicsTetris::pauseButtonEvent()
{
    if (isGameOver)
        return;

    if (isPause) {
        timerMove->start();
        isPause = false;
        setFocus();
        startButton->setEnabled(true);
    }
    else {
        timerMove->stop();
        isPause = true;
        startButton->setEnabled(false);
    }
}

void QicsTetris::setSpeed(int speed)
{
    currIntervalMove = startIntervalMove - speed * stepChangeSpeed;
    timerMove->setInterval(currIntervalMove);
}

void QicsTetris::speedMore(int more)
{
    timerMove->stop();
    if (!more)
        speed = 0;
    else if (speed < 14)
        speed += more;

    speedLcd->display(speed);
    currIntervalMove = startIntervalMove - speed * stepChangeSpeed;
    timerMove->setInterval(currIntervalMove);
    timerMove->start();
}

void QicsTetris::changeScore()
{
    scoreLcd->display(score);
}

void QicsTetris::keyPressEvent(QKeyEvent *event)
{
    if (!isPause) {
        switch (event->key())
        {
        case Qt::Key_Left:
            if (form.moveLeft(dataModelField))
                drawForm();
            break;
        case Qt::Key_Right:
            if (form.moveRight(dataModelField))
                drawForm();
            break;
        case Qt::Key_Down:
            if (form.rotatedRight(dataModelField))
                drawForm();
            break;
        case Qt::Key_Up:
            if (form.rotatedLeft(dataModelField))
                drawForm();
            break;
        case Qt::Key_Space:
            if (speed != speedFall)
                setSpeed(speedFall);
            break;
        case Qt::Key_D:
            break;
        default:
            QMainWindow::keyPressEvent(event);
            break;
        }
    }
}

void QicsTetris::resetGame()
{
    file->setEnabled(false);
    view->setEnabled(false);
    startButton->setEnabled(false);
    pauseButton->setEnabled(false);
    timerMove->stop();
    // draw demo and clear data model
    QColor backTmpColor;
    backTmpColor = *backColor;
    *backColor = viewSettings->getFigureColor();
    drawDemo(0, 0, countRowsTableField, countColumnsTableField, true, pFigureCellDisplay);
    *backColor = backTmpColor;
    drawDemo(0, 0, countRowsTableField, countColumnsTableField, true, pFieldCellDisplay);
    startButton->setEnabled(true);
    pauseButton->setEnabled(true);
    file->setEnabled(true);
    view->setEnabled(true);
}

void QicsTetris::preferences()
{
    ViewOptionsDialog *ldgOptions = new ViewOptionsDialog(viewSettings, this);
    ldgOptions->exec();
    setViewSettings();
}

// apply view setting
void QicsTetris::setViewSettings()
{
    if (viewSettings) {
        tableField->setRepaintBehavior(Qics::RepaintOff);
        tableNextForm->setRepaintBehavior(Qics::RepaintOff);
        switch (viewSettings->getDisplayerForm())
        {
        case ViewSettings::CHECK:
            pFigureCellDisplay = checkDisplay;
            break;
        case ViewSettings::COMBO:
            pFigureCellDisplay = comboDisplay;
            break;
        case ViewSettings::BUTTON:
            pFigureCellDisplay = buttonDisplay;
            break;
        default: // ViewSettings::TEXT
            pFigureCellDisplay = textDisplay;
            break;
        }
        // set figure color
        isRandomFigColor = viewSettings->getIsRandom();
        if (!isRandomFigColor) {
            *figureColor = viewSettings->getFigureColor();
            nextForm.setColor(figureColor);
            form.setColor(figureColor);
            for (int i = 0; i < countRowsTableField; ++i) {
                for (int j = 0; j < countColumnsTableField; ++j) {
                    if (dataModelField->item(i, j)->string().toInt() == MOVING_CELL ||
                        dataModelField->item(i, j)->string().toInt() == BUSY_CELL) {
                            tableField->cellRef(i, j).setBackgroundColor(*figureColor);
                    }
                }
            }
        }
        // set figure displayer
        for (int i = 0; i < countRowsTableField; ++i)
            for (int j = 0; j < countColumnsTableField; ++j) {
                if (dataModelField->item(i, j)->string().toInt() == MOVING_CELL ||
                    dataModelField->item(i, j)->string().toInt() == BUSY_CELL)
                    tableField->cellRef(i, j).setDisplayer(pFigureCellDisplay);
            }
        // set field color
        if (backColor && *backColor != viewSettings->getFieldColor()) {
            *backColor = viewSettings->getFieldColor();
            for (int i = 0; i < countRowsTableField; i++)
                for (int j = 0; j < countColumnsTableField; j++) {
                    if (dataModelField->item(i, j)->string().toInt() == EMPTY_CELL)
                        tableField->cellRef(i, j).setBackgroundColor(*backColor);
                }
        }
        drawNextForm();
        // set grid color
        if (gridColor && *gridColor != viewSettings->getGridColor()) {
            *gridColor = viewSettings->getGridColor();
            QPen* pen = new QPen(gridColor->rgb());
            tableField->setHorizontalGridLinePen(*pen);
            tableField->setVerticalGridLinePen(*pen);
            tableNextForm->setHorizontalGridLinePen(*pen);
            tableNextForm->setVerticalGridLinePen(*pen);
        }
        tableField->setRepaintBehavior(Qics::RepaintOn);
        tableNextForm->setRepaintBehavior(Qics::RepaintOn);
    }
}

void QicsTetris::drawDemo(int fromRow, int fromColumn, int toRow, int toColumn, bool beginToEnd, QicsCellDisplay* display)
{
    if (fromRow == toRow || fromColumn == toColumn)
        return;

    if (beginToEnd) {
        for (int i = fromRow; i < toRow; ++i) {
            tableField->cellRef(i, fromColumn).setBackgroundColor(*backColor);
            dataModelField->setItem(i, fromColumn, QicsDataInt(EMPTY_CELL));    // clear data model
            tableField->cellRef(i, fromColumn).setDisplayer(display);
            sleep(10);
        }
        fromColumn++;

        for (int j = fromColumn; j < toColumn; ++j) {
            tableField->cellRef(toRow - 1, j).setBackgroundColor(*backColor);
            dataModelField->setItem(toRow - 1, j, QicsDataInt(EMPTY_CELL));     // clear data model
            tableField->cellRef(toRow - 1, j).setDisplayer(display);
            sleep(10);
        }
        toRow--;
        beginToEnd = false;
    }
    else {
        for (int i = toRow; i >= fromRow; --i) {
            tableField->cellRef(i, toColumn - 1).setBackgroundColor(*backColor);
            dataModelField->setItem(i, toColumn - 1, QicsDataInt(EMPTY_CELL));   // clear data model
            tableField->cellRef(i, toColumn - 1).setDisplayer(display);
            sleep(10);
        }
        toColumn--;
        for (int j = toColumn; j >= fromColumn; --j) {
            tableField->cellRef(fromRow, j).setBackgroundColor(*backColor);
            dataModelField->setItem(fromRow, j, QicsDataInt(EMPTY_CELL));       // clear data model
            tableField->cellRef(fromRow, j).setDisplayer(display);
            sleep(10);
        }
        fromRow++;
        beginToEnd = true;
    }

    drawDemo(fromRow, fromColumn, toRow, toColumn, beginToEnd, display);
}

void QicsTetris::sleep(int ms)
{
    QTimer::singleShot(ms, this, SLOT(timeout()));
    m_eventLoop.exec();
}

void QicsTetris::timeout()
{
    m_eventLoop.quit();
}


