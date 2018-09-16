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

#include "QicsMappedDimensionManager.h"

#include "QicsDimensionManager.h"
#include "QicsStyleManager.h"


QicsMappedDimensionManager::QicsMappedDimensionManager(QicsDimensionManager *_dm,
                                                       QicsGridInfo *_gi)
    : myDM(_dm), myInfo(_gi)
{
}

QicsMappedDimensionManager::~QicsMappedDimensionManager()
{
}

void QicsMappedDimensionManager::setDefaultFont(const QFont &fnt)
{
    myDM->setDefaultFont(fnt);
}

void QicsMappedDimensionManager::setRowFont(Qics::QicsGridType grid_type, int row,
                                            const QFont &fnt)
{
    myDM->setRowFont(grid_type, myInfo->modelRowIndex(row), false, fnt);
}

void QicsMappedDimensionManager::unsetRowFont(Qics::QicsGridType grid_type, int row)
{
    myDM->unsetRowFont(grid_type, myInfo->modelRowIndex(row), false);
}

void QicsMappedDimensionManager::setRepeatingRowFont(Qics::QicsGridType grid_type, int row,
                                                     unsigned int interval, const QFont &fnt)
{
    myDM->setRepeatingRowFont(grid_type, myInfo->modelRowIndex(row), interval, fnt);
}

void QicsMappedDimensionManager::setColumnFont(Qics::QicsGridType grid_type, int col,
                                               const QFont &fnt)
{
    myDM->setColumnFont(grid_type, myInfo->modelColumnIndex(col), false, fnt);
}

void QicsMappedDimensionManager::unsetColumnFont(Qics::QicsGridType grid_type,
                                                 int col)
{
    myDM->unsetColumnFont(grid_type, myInfo->modelColumnIndex(col), false);
}

void QicsMappedDimensionManager::setRepeatingColumnFont(Qics::QicsGridType grid_type,
                                                        int col, unsigned int interval,
                                                        const QFont &fnt)
{
    myDM->setRepeatingColumnFont(grid_type, myInfo->modelColumnIndex(col), interval, fnt);
}

void QicsMappedDimensionManager::setCellFont(Qics::QicsGridType grid_type,
                                             int row, int col,
                                             const QFont &fnt)
{
    myDM->setCellFont(grid_type, myInfo->modelRowIndex(row),
        myInfo->modelColumnIndex(col), false, fnt);
}

void QicsMappedDimensionManager::unsetCellFont(Qics::QicsGridType grid_type,
                                               int row, int col)
{
    myDM->unsetCellFont(grid_type, myInfo->modelRowIndex(row),
        myInfo->modelColumnIndex(col), false);
}

void QicsMappedDimensionManager::setRowHeightInPixels(int row, int height)
{
    myDM->setRowHeightInPixels(myInfo->modelRowIndex(row), false, height);
}

void QicsMappedDimensionManager::setRowHeightInChars(int row, int height)
{
    myDM->setRowHeightInChars(myInfo->modelRowIndex(row), false, height);
}

void QicsMappedDimensionManager::setRepeatingRowHeightInPixels(int row, unsigned int interval,
                                                               int height)
{
    myDM->setRepeatingRowHeightInPixels(myInfo->modelRowIndex(row), interval, height);
}

void QicsMappedDimensionManager::setRepeatingRowHeightInChars(int row, unsigned int interval,
                                                              int height)
{
    myDM->setRepeatingRowHeightInChars(myInfo->modelRowIndex(row), interval, height);
}

void QicsMappedDimensionManager::setColumnWidthInPixels(int col, int width)
{
    myDM->setColumnWidthInPixels(myInfo->modelColumnIndex(col), false, width);
}

void QicsMappedDimensionManager::setColumnWidthInChars(int col, int width)
{
    myDM->setColumnWidthInChars(myInfo->modelColumnIndex(col), false, width);
}

void QicsMappedDimensionManager::setRepeatingColumnWidthInPixels(int col, unsigned int interval,
                                                                 int width)
{
    myDM->setRepeatingColumnWidthInPixels(myInfo->modelColumnIndex(col), interval, width);
}

void QicsMappedDimensionManager::setRepeatingColumnWidthInChars(int col, unsigned int interval,
                                                                int width)
{
    myDM->setRepeatingColumnWidthInChars(myInfo->modelColumnIndex(col), interval, width);
}

int QicsMappedDimensionManager::rowHeight(int row) const
{
    return myDM->rowHeight(myInfo->modelRowIndex(row));
}

int QicsMappedDimensionManager::columnWidth(int col) const
{
    return myDM->columnWidth(myInfo->modelColumnIndex(col));
}

void QicsMappedDimensionManager::setRowMinHeightInPixels(int row, int height)
{
    myDM->setRowMinHeightInPixels(myInfo->modelRowIndex(row), false, height);
}

void QicsMappedDimensionManager::setRowMinHeightInChars(int row, int height)
{
    myDM->setRowMinHeightInChars(myInfo->modelRowIndex(row), false, height);
}

void QicsMappedDimensionManager::setColumnMinWidthInPixels(int col, int width)
{
    myDM->setColumnMinWidthInPixels(myInfo->modelColumnIndex(col), false,
        width);
}

void QicsMappedDimensionManager::setColumnMinWidthInChars(int col, int width)
{
    myDM->setColumnMinWidthInChars(myInfo->modelColumnIndex(col), false,
        width);
}

int QicsMappedDimensionManager::rowMinHeight(int row) const
{
    return myDM->rowMinHeight(myInfo->modelRowIndex(row), true, false);
}

int QicsMappedDimensionManager::rowMaxHeight(int row) const
{
    return myDM->rowMaxHeight(myInfo->modelRowIndex(row), true, false);
}

int QicsMappedDimensionManager::columnMinWidth(int col) const
{
    return myDM->columnMinWidth(myInfo->modelColumnIndex(col), true, false);
}

int QicsMappedDimensionManager::columnMaxWidth(int col) const
{
    return myDM->columnMaxWidth(myInfo->modelColumnIndex(col), true, false);
}

void QicsMappedDimensionManager::setDefaultMargin(int margin)
{
    myDM->setDefaultMargin(margin);
}

void QicsMappedDimensionManager::setRowMargin(Qics::QicsGridType grid_type,
                                              int row, int margin)
{
    myDM->setRowMargin(grid_type, myInfo->modelRowIndex(row), false, margin);
}

void QicsMappedDimensionManager::setColumnMargin(Qics::QicsGridType grid_type,
                                                 int col, int margin)
{
    myDM->setColumnMargin(grid_type, myInfo->modelColumnIndex(col),
        false, margin);
}

void QicsMappedDimensionManager::setCellMargin(Qics::QicsGridType grid_type,
                                               int row, int col, int margin)
{
    myDM->setCellMargin(grid_type, myInfo->modelRowIndex(row), false,
        myInfo->modelColumnIndex(col), margin);
}

void QicsMappedDimensionManager::setRepeatingRowMargin(Qics::QicsGridType grid_type,
                                                       int row, unsigned int interval, int margin)
{
    myDM->setRepeatingRowMargin(grid_type, myInfo->modelRowIndex(row), interval, margin);
}

void QicsMappedDimensionManager::setRepeatingColumnMargin(Qics::QicsGridType grid_type,
                                                          int col, unsigned int interval, int margin)
{
    myDM->setRepeatingColumnMargin(grid_type, myInfo->modelColumnIndex(col), interval, margin);
}

void QicsMappedDimensionManager::setDefaultBorderWidth(int bw)
{
    myDM->setDefaultBorderWidth(bw);
}

void QicsMappedDimensionManager::setRowBorderWidth(Qics::QicsGridType grid_type,
                                                   int row, int bw)
{
    myDM->setRowBorderWidth(grid_type, myInfo->modelRowIndex(row),
        false, bw);
}

void QicsMappedDimensionManager::setColumnBorderWidth(Qics::QicsGridType grid_type,
                                                      int col, int bw)
{
    myDM->setColumnBorderWidth(grid_type, myInfo->modelColumnIndex(col),
        false, bw);
}

void QicsMappedDimensionManager::setCellBorderWidth(Qics::QicsGridType grid_type,
                                                    int row, int col, int bw)
{
    myDM->setCellBorderWidth(grid_type, myInfo->modelRowIndex(row),
        myInfo->modelColumnIndex(col), false, bw);
}

void QicsMappedDimensionManager::setRepeatingRowBorderWidth(Qics::QicsGridType grid_type,
                                                            int row, unsigned int interval, int bw)
{
    myDM->setRepeatingRowBorderWidth(grid_type, myInfo->modelRowIndex(row),
        interval, bw);
}

void QicsMappedDimensionManager::setRepeatingColumnBorderWidth(Qics::QicsGridType grid_type,
                                                               int col, unsigned int interval, int bw)
{
    myDM->setRepeatingColumnBorderWidth(grid_type, myInfo->modelColumnIndex(col),
        interval, bw);
}

bool QicsMappedDimensionManager::isRowStretchable(int row) const
{
    return myDM->isRowStretchable(myInfo->modelRowIndex(row), false);
}

void QicsMappedDimensionManager::setRowStretchable(int row, bool set)
{
    myDM->setRowStretchable(myInfo->modelRowIndex(row), false, set);
}

bool QicsMappedDimensionManager::isColumnStretchable(int col) const
{
    return myDM->isColumnStretchable(myInfo->modelColumnIndex(col),
        false);
}

void QicsMappedDimensionManager::setColumnStretchable(int col, bool set)
{
    myDM->setColumnStretchable(myInfo->modelColumnIndex(col), false, set);
}

void QicsMappedDimensionManager::stretchRows(int start_row, int end_row,
                                        int space)
{
    myDM->stretchRows(start_row, end_row, space, true);
}

void QicsMappedDimensionManager::stretchColumns(int start_col, int end_col,
                                           int space)
{
    myDM->stretchColumns(start_col, end_col, space, true);
}

int QicsMappedDimensionManager::regionHeight(const QicsRegion &region) const
{
    int height = 0;
    int num_hidden_rows = 0;
    int i;

    for (i = region.startRow(); i <= region.endRow(); ++i) {
        if (isRowHidden(i))
            ++num_hidden_rows;
        else
            height += myDM->rowHeight(myInfo->modelRowIndex(i));
    }

    bool isVisible = (* static_cast<bool *>
        (myInfo->styleManager()->getGridProperty(QicsGridStyle::HorizontalGridLinesVisible)));

    int hlw = isVisible ? (* static_cast<int *>
        (myInfo->styleManager()->getGridProperty(QicsGridStyle::HorizontalGridLineWidth))) : 0;

    height += (hlw * (region.endRow() - region.startRow() -
        num_hidden_rows));

    Qics::QicsLineStyle hls = (* static_cast<Qics::QicsLineStyle *>
        (myInfo->styleManager()->getGridProperty(QicsGridStyle::HorizontalGridLineStyle)));

    if (hls == Qics::Raised || hls == Qics::Sunken)
        height += (2*hlw * (region.endRow() - region.startRow() - num_hidden_rows));

    return height;
}

int QicsMappedDimensionManager::regionWidth(const QicsRegion &region) const
{
    int width = 0;
    int num_hidden_cols = 0;
    int j;

    for (j = region.startColumn(); j <= region.endColumn(); ++j) {
        if (isColumnHidden(j))
            ++num_hidden_cols;
        else
            width += myDM->columnWidth(myInfo->modelColumnIndex(j));
    }

    bool isVisible = (* static_cast<bool *>
        (myInfo->styleManager()->getGridProperty(QicsGridStyle::VerticalGridLinesVisible)));

    int vlw = isVisible ? (* static_cast<int *>
        (myInfo->styleManager()->getGridProperty(QicsGridStyle::VerticalGridLineWidth))) : 0;

    width += (vlw * (region.endColumn() - region.startColumn() - num_hidden_cols));

    Qics::QicsLineStyle vls = (* static_cast<Qics::QicsLineStyle *>
        (myInfo->styleManager()->getGridProperty(QicsGridStyle::VerticalGridLineStyle)));

    if (vls == Qics::Raised || vls == Qics::Sunken)
        width += (2*vlw * (region.endColumn() - region.startColumn() - num_hidden_cols));

    return width;
}

void QicsMappedDimensionManager::overrideRowHeight(int row, int height)
{
    myDM->overrideRowHeight(myInfo->modelRowIndex(row), height);
}

void QicsMappedDimensionManager::overrideColumnWidth(int col, int width)
{
    myDM->overrideColumnWidth(myInfo->modelColumnIndex(col), width);
}

void QicsMappedDimensionManager::resetRowHeight(int row)
{
    myDM->resetRowHeight(myInfo->modelRowIndex(row));
}

void QicsMappedDimensionManager::resetColumnWidth(int col)
{
    myDM->resetColumnWidth(myInfo->modelColumnIndex(col));
}

int QicsMappedDimensionManager::overriddenRowHeight(int row) const
{
    return myDM->overriddenRowHeight(myInfo->modelRowIndex(row));
}

int QicsMappedDimensionManager::overriddenColumnWidth(int col) const
{
    return myDM->overriddenColumnWidth(myInfo->modelColumnIndex(col));
}

void QicsMappedDimensionManager::hideRow(int row)
{
    myDM->hideRow(myInfo->modelRowIndex(row));
}

bool QicsMappedDimensionManager::hasHiddenRows() const
{
    return myDM->hasHiddenRows();
}

void QicsMappedDimensionManager::hideColumn(int col)
{
    myDM->hideColumn(myInfo->modelColumnIndex(col));
}

bool QicsMappedDimensionManager::hasHiddenColumns() const
{
    return myDM->hasHiddenColumns();
}

void QicsMappedDimensionManager::showRow(int row)
{
    myDM->showRow(myInfo->modelRowIndex(row));
}

void QicsMappedDimensionManager::showColumn(int col)
{
    myDM->showColumn(myInfo->modelColumnIndex(col));
}

bool QicsMappedDimensionManager::isRowHidden(int row) const
{
    return (myDM->isRowHidden(myInfo->modelRowIndex(row)));
}

bool QicsMappedDimensionManager::isColumnHidden(int col) const
{
    return (myDM->isColumnHidden(myInfo->modelColumnIndex(col)));
}


