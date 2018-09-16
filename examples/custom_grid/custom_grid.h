/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef CUSTOM_GRID_H
#define CUSTOM_GRID_H

#include <QicsTableGrid.h>

// This is an example of a custom grid.

class QMenu;
class QMouseEvent;

class CustomGrid : public QicsTableGrid
{
    Q_OBJECT
public:
    CustomGrid(QWidget *w, QicsGridInfo &info,
        int top_row = 0, int left_column = 0);

    static QicsTableGrid *createGrid(QWidget *w, QicsGridInfo &info,
        int top_row = 0, int left_column = 0);

protected:
    virtual void handleMousePressEvent(const QicsICell &cell, QMouseEvent *m);

private:
    QMenu *m_menu;
};


#endif //CUSTOM_GRID_H


