/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

/*
* Class QicsPushButtonHeaderDisplay.
* NOTE: Must be used only in headers, not in table grids!
*/

#ifndef QICSPUSHBUTTONHEADERDISPLAY_H
#define QICSPUSHBUTTONHEADERDISPLAY_H

#include <QicsTextCellDisplay.h>

class QPushButton;

class QicsPushButtonHeaderDisplay : public QicsTextCellDisplay
{
    Q_OBJECT
public:
    QicsPushButtonHeaderDisplay(QWidget *parent = 0);
    ~QicsPushButtonHeaderDisplay();

protected:
    virtual void drawBackground(QicsGridInfo *, int , int , const QRect &, const QPalette &,
        QPainter *, bool , bool );
};

#endif //QICSPUSHBUTTONHEADERDISPLAY_H


