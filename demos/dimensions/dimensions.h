/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef DIMENSIONS_H
#define DIMENSIONS_H

#include <QicsTable.h>
#include <QicsPushButtonCellDisplay.h>

class DimensionsTable : public QicsTable
{
    Q_OBJECT
public:
    DimensionsTable(QicsDataModel* dm, QWidget *parent = 0);

private slots:
    void clickRowBtnMoveRow();
    void clickColBtnInsertRow();
    void clickColBtnHideRow();
    void clickColBtnDeleteRow();
    void clickRowBtnMoveCol();
    void clickColBtnInsertCol();
    void clickColBtnHideCol();
    void clickColBtnDeleteCol();
    void spanButtonSlot();
};

class CustomQicsPushButtonCellDisplay : public QicsPushButtonCellDisplay
{
    Q_OBJECT
public:
    CustomQicsPushButtonCellDisplay(QWidget *parent = 0)
        : QicsPushButtonCellDisplay(parent)	{}

    inline QicsCell *cell() const {return myCell;}
};

#endif


