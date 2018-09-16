/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef QICSFILTERCELLDISPLAY_H
#define QICSFILTERCELLDISPLAY_H

#include <QicsTextCellDisplay.h>
#include <QicsRegion.h>

class QicsFilterSortDialog;
class QicsTreeTable;

class QicsFilterCellDisplay : public QicsTextCellDisplay
{
    Q_OBJECT
public:
    QicsFilterCellDisplay(QicsTreeTable *table);
    ~QicsFilterCellDisplay();

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,QRect &rect, QPainter *painter);

    virtual bool handleMouseEvent(QicsScreenGrid *grid, int row, int col, QMouseEvent *me);

private slots:
    void onFilterChanged(int index);

private:
    bool m_over;
    bool m_down;
    QicsRegion m_current;
    QicsTreeTable *m_table;
    QicsFilterSortDialog *m_fsd;
};

#endif //QICSFILTERCELLDISPLAY_H


