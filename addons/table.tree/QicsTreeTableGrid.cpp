/*********************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#include <QicsTreeTableGrid.h>

#include <QicsTreeDataModel.h>
#include <QicsSpecialRowData.h>


QicsTreeTableGrid::QicsTreeTableGrid(QWidget *w, QicsGridInfo &info, int top_row, int left_column)
        : QicsTableGrid(w, info, top_row, left_column)
{
}

void QicsTreeTableGrid::keyPressEvent(QKeyEvent *ke)
{
    // find matching special row
    QicsViewTreeDataModel *dm = qobject_cast<QicsViewTreeDataModel*>(gridInfo().dataModel());
    if (dm) {
        QicsICell cell = currentCell();
        if (cell.isValid()) {
            int r = gridInfo().modelRowIndex(cell.row());
            QicsSpecialRowData *data = dm->specialRowData(r);
            if (data)
                if (data->handleKeyPress(cell.row(), cell.column(), ke))
                    return;
        }
    }

    QicsTableGrid::keyPressEvent(ke);
}


