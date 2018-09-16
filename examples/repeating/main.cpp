/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <QApplication>
#include <QLabel>

#include <QicsTable.h>
#include <QicsRepeatingRow.h>
#include <QicsDataModelDefault.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    //creation of a source model
    QicsDataModelDefault *dataModel = new QicsDataModelDefault(1000, 1000);

    //filling the source model with data
    for (int r = 0; r < dataModel->numRows(); ++r) {
        for (int c = 0; c < dataModel->numColumns(); ++c) {
            dataModel->setItem(r, c, QicsDataInt(r * c));
        }
    }

    //creation of a table
    QicsTable table(dataModel);

    //creation of a label
    QLabel label(QObject::tr("Repeating"));
    label.setAlignment(Qt::AlignCenter);
    table.setTopTitleWidget(&label);

    //creation of a pen for drawing borders
    QPen pen(Qt::red);

    //creation repeating row
    //QicsTable::repeatingRow creates a repeating row object by given start row (the first parametr) and given interval (the second parametr)
    QicsRepeatingRow *rr = table.repeatingRow(0, 8);
    //applying the pen for a top border
    rr->setTopBorderPen(pen);
    //applying the pen for a right border
    rr->setRightBorderPen(pen);
    //applying the pen for a bottom border
    rr->setBottomBorderPen(pen);
    //applying the pen for a left border
    rr->setLeftBorderPen(pen);

    table.show();

    return app.exec();
}


