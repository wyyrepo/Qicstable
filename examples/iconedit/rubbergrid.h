/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef RUBBERGRID_H
#define RUBBERGRID_H

#include <QicsTable.h>

// this class is used to implement "rubber" painting on the grid
class RubberGrid : public QicsTableGrid
{
public:
    RubberGrid(QWidget *w, QicsGridInfo &info,
        int top_row = 0, int left_column = 0);

    static QicsTableGrid *createGrid(QWidget *w, QicsGridInfo &info,
        int top_row = 0, int left_column = 0);

protected:
    virtual void mousePressEvent( QMouseEvent *m );
    virtual void mouseReleaseEvent( QMouseEvent *m );
    virtual void mouseMoveEvent( QMouseEvent *m );
    virtual bool handleTraversalKeys(QKeyEvent *ke);

private:
    QMouseEvent *m_me;
};

#endif //RUBBERGRID_H


