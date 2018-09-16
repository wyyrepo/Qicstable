/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SPREADSHEETCELLDISPLAY_H
#define SPREADSHEETCELLDISPLAY_H

#include <QicsTextCellDisplay.h>

class QicsTable;
/*
* Class that represents default table grid displayer.
* Can perform simple formula calculations with help of SpreadsheetCalculator class.
*/
class SpreadsheetCellDisplay : public QicsTextCellDisplay
{
public:
    SpreadsheetCellDisplay(QicsTable* table);

protected:
    virtual QString textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

private:
    QicsTable* m_table;
};

#endif //SPREADSHEETCELLDISPLAY_H


