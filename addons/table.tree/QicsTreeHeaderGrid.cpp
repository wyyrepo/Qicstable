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

#include <QicsTreeHeaderGrid.h>

#include <QicsTreeTable.h>
#include <QicsSpecialRowData.h>


QicsTreeHeaderGrid::QicsTreeHeaderGrid(QWidget *w, QicsGridInfo &info, Qics::QicsHeaderType type)
    : QicsHeaderGrid(w, info, type)
{
}

void QicsTreeHeaderGrid::emitMouseSignals(const QicsICell& cell, QMouseEvent *m)
{
    if (m_type == Qics::RowHeader) {
        int event_type = m->type();
        QicsTreeTable *tt = qobject_cast<QicsTreeTable*>(parent());

        switch (event_type)
        {
        case QEvent::MouseButtonRelease:
            tt->onClicked(cell.row(), cell.column(), m->button(), m->pos());
            break;
        case QEvent::MouseButtonDblClick:
            tt->onDoubleClicked(cell.row(), cell.column(), m->button(), m->pos());
            break;
        }
    }

    QicsHeaderGrid::emitMouseSignals(cell, m);
}

void QicsTreeHeaderGrid::keyPressEvent(QKeyEvent *ke)
{
    if (m_type == Qics::RowHeader) {
        // find matching special row
        QicsViewTreeDataModel *dm = qobject_cast<QicsViewTreeDataModel*>((qobject_cast<QicsTreeTable *>(parent()))->viewModel());
        if (dm) {
            QicsICell cell = currentCell();
            if (cell.isValid()) {
                int r = gridInfo().modelRowIndex(cell.row());
                QicsSpecialRowData *data = dm->specialRowData(r);
                if (data)
                    if (data->handleKeyPress(cell.row(), cell.column(), ke)) {
                        ke->accept();
                        return;
                    }
            }
        }
    }

    QicsHeaderGrid::keyPressEvent(ke);
}


