/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SPREADSHEET_GRID_H
#define SPREADSHEET_GRID_H

#include <QicsTableGrid.h>

/*
* Custom grid. Provides context menu with default editing actions.
* Also performs editing of big data sets with disabled repaint(RepaintBehavior = Off)
*/
class QMenu;
class QicsSelection;

class SpreadsheetGrid : public QicsTableGrid
{
    Q_OBJECT
public:
    SpreadsheetGrid(QWidget *w, QicsGridInfo &info,
        int top_row = 0, int left_column = 0);

    static QicsTableGrid *createGrid(QWidget *w, QicsGridInfo &info,
        int top_row = 0, int left_column = 0);

protected:
    virtual void handleMousePressEvent(const QicsICell &cell, QMouseEvent *m);

private:
    QMenu *m_menu;
};

#endif //SPREADSHEET_GRID_H


