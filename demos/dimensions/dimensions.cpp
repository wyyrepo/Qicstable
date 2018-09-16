/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "dimensions.h"

#include <QicsCheckCellDisplay.h>
#include <QicsRowHeader.h>
#include <QicsColumn.h>
#include <QicsColumnHeader.h>
#include <QicsRow.h>
#include <QicsCell.h>
#include <QicsSelection.h>


DimensionsTable::DimensionsTable(QicsDataModel* dm, QWidget *parent)
    : QicsTable(dm, parent)
{
    setWindowTitle(tr("Dimensions Table Demo"));

    const int sizeHeader = 5;
    const int rows = dm->numRows();
    const int cols = dm->numColumns();

    QicsColumnHeader *cHeader = columnHeader();
    cHeader->setNumRows(sizeHeader);
    QicsRowHeader *rHeader = rowHeader();
    rHeader->setNumColumns(sizeHeader);

    CustomQicsPushButtonCellDisplay *btnDisplayer = 0;

    for(int i = 0; i < sizeHeader; ++i) {
        for (int col = 0; col < cols; ++col) {
            QicsCell &cell = cHeader->cellRef(i, col);
            switch(i)
            {
            case 0:
                cell.setDisplayer(new QicsCheckCellDisplay(this));
                cell.setAlignment(Qt::AlignCenter);
                break;
            case 1:
                btnDisplayer = new CustomQicsPushButtonCellDisplay(this);
                cell.setDisplayer(btnDisplayer);
                cell.setLabel(tr("Move"));
                cell.setToolTipText(tr("Move all checked columns before current"));
                connect(btnDisplayer, SIGNAL(clicked()), SLOT(clickRowBtnMoveCol()));
                break;
            case 2:
                btnDisplayer = new CustomQicsPushButtonCellDisplay(this);
                cell.setDisplayer(btnDisplayer);
                cell.setLabel(tr("Insert"));
                cell.setToolTipText(tr("Insert new column before current"));
                connect(btnDisplayer, SIGNAL(clicked()), SLOT(clickColBtnInsertCol()));
                break;
            case 3:
                btnDisplayer = new CustomQicsPushButtonCellDisplay(this);
                cell.setDisplayer(btnDisplayer);
                cell.setLabel("Hide");
                cell.setToolTipText(tr("Hide current column"));
                connect(btnDisplayer, SIGNAL(clicked()), SLOT(clickColBtnHideCol()));
                break;
            case 4:
                btnDisplayer = new CustomQicsPushButtonCellDisplay(this);
                cell.setDisplayer(btnDisplayer);
                cell.setLabel(tr("Delete"));
                cell.setToolTipText(tr("Delete current column"));
                connect(btnDisplayer, SIGNAL(clicked()), SLOT(clickColBtnDeleteCol()));
                break;
            }
        }
        rHeader->columnRef(i).setWidthInPixels(20);
        for (int row = 0; row < rows; ++row) {
            QicsCell &cell = rHeader->cellRef(row, i);
            switch(i)
            {
            case 0:
                cell.setDisplayer(new QicsCheckCellDisplay(this));
                cell.setAlignment(Qt::AlignCenter);
                break;
            case 1:
                btnDisplayer = new CustomQicsPushButtonCellDisplay(this);
                cell.setDisplayer(btnDisplayer);
                cell.setLabel("M");
                cell.setToolTipText(tr("Move all checked rows before current"));
                connect(btnDisplayer, SIGNAL(clicked()), SLOT(clickRowBtnMoveRow()));
                break;
            case 2:
                btnDisplayer = new CustomQicsPushButtonCellDisplay(this);
                cell.setDisplayer(btnDisplayer);
                cell.setLabel("I");
                cell.setToolTipText(tr("Insert new row before current"));
                connect(btnDisplayer, SIGNAL(clicked()), SLOT(clickColBtnInsertRow()));
                break;
            case 3:
                btnDisplayer = new CustomQicsPushButtonCellDisplay(this);
                cell.setDisplayer(btnDisplayer);
                cell.setLabel(tr("H"));
                cell.setToolTipText(tr("Hide current row"));
                connect(btnDisplayer, SIGNAL(clicked()), SLOT(clickColBtnHideRow()));
                break;
            case 4:
                btnDisplayer = new CustomQicsPushButtonCellDisplay(this);
                cell.setDisplayer(btnDisplayer);
                cell.setLabel("D");
                cell.setToolTipText(tr("Delete current row"));
                connect(btnDisplayer, SIGNAL(clicked()), SLOT(clickColBtnDeleteRow()));
                break;
            }
        }
    }

    QPushButton *spanButton = new QPushButton(tr("Add/Remove span"), this);
    setTopLeftCornerWidget(spanButton);

    connect(spanButton, SIGNAL(clicked()), SLOT(spanButtonSlot()));
}

void DimensionsTable::clickRowBtnMoveRow()
{
    CustomQicsPushButtonCellDisplay *btnDisplayer = qobject_cast<CustomQicsPushButtonCellDisplay *>(sender());
    QVector <int> movingRowsArr;
    QicsRowHeader *rHeader = rowHeader();
    for (int i = 0; i < dataModel()->numRows(); ++i) {
        const QicsDataItem *item = rHeader->cellRef(i, 0).dataValue();
        if (item && item->string() == "True")
            movingRowsArr.push_back(i);
    }
    if (movingRowsArr.isEmpty()) return;
    moveRows(btnDisplayer->cell()->rowIndex(), movingRowsArr);
}

void DimensionsTable::clickColBtnInsertRow()
{
    CustomQicsPushButtonCellDisplay *btnDisplayer = qobject_cast<CustomQicsPushButtonCellDisplay *>(sender());
    insertRow(btnDisplayer->cell()->rowIndex());
}
void DimensionsTable::clickColBtnHideRow()
{
    CustomQicsPushButtonCellDisplay *btnDisplayer = qobject_cast<CustomQicsPushButtonCellDisplay *>(sender());
    rowRef(btnDisplayer->cell()->rowIndex()).hide();
}
void DimensionsTable::clickColBtnDeleteRow()
{
    CustomQicsPushButtonCellDisplay *btnDisplayer = qobject_cast<CustomQicsPushButtonCellDisplay *>(sender());
    deleteRow(btnDisplayer->cell()->rowIndex());
}

void DimensionsTable::clickRowBtnMoveCol()
{
    CustomQicsPushButtonCellDisplay *btnDisplayer = qobject_cast<CustomQicsPushButtonCellDisplay *>(sender());
    QVector <int> movingColumnsArr;
    QicsColumnHeader *cHeader = columnHeader();
    for (int i = 0; i < dataModel()->numColumns(); ++i) {
        const QicsDataItem *item = cHeader->cellRef(0, i).dataValue();
        if (item && item->string() == "True")
            movingColumnsArr.push_back(i);
    }
    if (movingColumnsArr.isEmpty()) return;
    moveColumns(btnDisplayer->cell()->columnIndex(), movingColumnsArr);
}

void DimensionsTable::clickColBtnInsertCol()
{
    CustomQicsPushButtonCellDisplay *btnDisplayer = qobject_cast<CustomQicsPushButtonCellDisplay *>(sender());
    insertColumn(btnDisplayer->cell()->columnIndex());
}

void DimensionsTable::clickColBtnHideCol()
{
    CustomQicsPushButtonCellDisplay *btnDisplayer = qobject_cast<CustomQicsPushButtonCellDisplay *>(sender());
    columnRef(btnDisplayer->cell()->columnIndex()).hide();
}

void DimensionsTable::clickColBtnDeleteCol()
{
    CustomQicsPushButtonCellDisplay *btnDisplayer = qobject_cast<CustomQicsPushButtonCellDisplay *>(sender());
    deleteColumn(btnDisplayer->cell()->columnIndex());
}

void DimensionsTable::spanButtonSlot()
{
    QicsSelectionList *slist = selectionList();
    if(slist) {
        QicsSelectionList::const_iterator iter;
        QicsCell* cell = const_cast <QicsCell*> (currentCell());
        if (!cell->spannedRegion().containsCell(currentCell()->rowIndex(), currentCell()->columnIndex()) ) {
            for (iter = slist->begin(); iter != slist->end(); ++iter) {
                const QicsSelection &sel = *iter;
                int bottom_row = sel.numRows();
                int right_column = sel.numColumns();
                if(sel.bottomRow() == QicsLAST_ROW)
                    bottom_row = dataModel()->numRows();
                if(sel.rightColumn() == QicsLAST_COLUMN)
                    right_column = dataModel()->numColumns();
                addCellSpan(QicsSpan(sel.topRow(),sel.leftColumn(),bottom_row,right_column));
            }
        }
        else {
            QicsSelectionList::const_iterator iter;
            for (iter = slist->begin(); iter != slist->end(); ++iter) {
                const QicsSelection &sel = *iter;
                removeCellSpan(sel.topRow(),sel.leftColumn());
            }
        }
    }
}


