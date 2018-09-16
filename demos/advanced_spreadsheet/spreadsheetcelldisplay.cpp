/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "spreadsheetcelldisplay.h"

#include <QicsTable.h>
#include <QicsCell.h>
#include <QicsDataItemFormatter.h>
#include "spreadsheetcalculator.h"


SpreadsheetCellDisplay::SpreadsheetCellDisplay(QicsTable* table)
    : QicsTextCellDisplay(table)
{
    m_table = table;
}

QString SpreadsheetCellDisplay::textToDisplay(QicsGridInfo *info, int row, int col,
                                              const QicsDataItem *itm) const
{
    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QString qs = myCell->label();
    if (qs.isEmpty() && itm) {
        QicsDataString calculatedItm = QicsDataString(
            SpreadsheetCalculator::calculateFormula(itm->string(),m_table));

        QicsDataItemFormatter *formatter = myCell->formatter();

        if (formatter)
            qs = formatter->format(calculatedItm);
        else
            qs = QString(calculatedItm.string());
    }

    return qs;
}


