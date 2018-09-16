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

#include "QicsScrollManager.h"

#include <QApplication>
#include "QicsScrollBarScroller.h"
#include "QicsScreenGrid.h"
#include "QicsDataModel.h"
#include "QicsMappedDimensionManager.h"
#include "QicsTable.h"


QicsScrollManager::QicsScrollManager(QObject *parent)
    : QObject(parent)
{
    myPrimaryGrid = 0;
    myRowIndex = -1;
    myColumnIndex = 0;
    myVScroller = 0;
    myHScroller = 0;
    requestedRowIdx = 0;
    requestedColumnIdx = 0;
    sliderPressedFlag = false;
}

QicsScrollManager::~QicsScrollManager()
{
}

void QicsScrollManager::connectScroller(QicsScroller *scroller)
{
    if (!scroller)
        return;

    connect(scroller, SIGNAL(indexChanged(Qics::QicsIndexType, int)),
        this, SLOT(setScrollerIndex(Qics::QicsIndexType, int)));
    connect(scroller, SIGNAL(scrollPageAdd(Qics::QicsIndexType, int)),
        this, SLOT(handleScrollPageAdd(Qics::QicsIndexType, int)));
    connect(scroller, SIGNAL(scrollPageSub(Qics::QicsIndexType, int)),
        this, SLOT(handleScrollPageSub(Qics::QicsIndexType, int)));

    connect(this, SIGNAL(minIndexChanged(Qics::QicsIndexType, int)),
        scroller, SLOT(setMinIndex(Qics::QicsIndexType, int)));
    connect(this, SIGNAL(maxIndexChanged(Qics::QicsIndexType, int)),
        scroller, SLOT(setMaxIndex(Qics::QicsIndexType, int)));
    connect(this, SIGNAL(indexChanged(Qics::QicsIndexType, int)),
        scroller, SLOT(setIndex(Qics::QicsIndexType, int)));

    if ( myVScroller == 0 )
        myVScroller = scroller;
    else
        if ( myHScroller == 0 )
            myHScroller = scroller;
}

void QicsScrollManager::disconnectScroller(QicsScroller *scroller)
{
    disconnect(scroller, 0, this, 0);
    disconnect(this, 0, scroller, 0);

    if ( myVScroller == scroller )	{ delete myVScroller; myVScroller = 0 ; }
    if ( myHScroller == scroller )	{ delete myHScroller; myHScroller = 0 ; }
}

void QicsScrollManager::setPrimaryGrid(QicsScreenGrid *grid)
{
    bool do_changed = (myPrimaryGrid != 0);

    myPrimaryGrid = grid;

    if (do_changed)
        gridBoundsChanged();

    connect(grid, SIGNAL(newBoundsInfo()),
        this, SLOT(gridBoundsChanged()));

    connect(grid, SIGNAL(destroyed(QObject *)),
        this, SLOT(gridDestroyed(QObject *)));
}

void QicsScrollManager::connectGrid(QicsScreenGrid *grid,
                               bool control_rows, bool control_columns)
{
    if (!grid) return;

    if (control_rows)
        connect(this, SIGNAL(rowIndexChanged(int)),
            grid, SLOT(setTopRow(int)));

    if (control_columns)
        connect(this, SIGNAL(columnIndexChanged(int)),
            grid, SLOT(setLeftColumn(int)));
}

void QicsScrollManager::disconnectGrid(QicsScreenGrid *grid)
{
    disconnect(this, 0, grid, 0);
}

void QicsScrollManager::setRowIndex(int idx)
{
    computeVSPosition(idx);
    requestedRowIdx = idx;
}

void QicsScrollManager::setColumnIndex(int idx)
{
    computeHSPosition(idx);
    requestedColumnIdx = idx;
}

void QicsScrollManager::setIndex(Qics::QicsIndexType type, int idx)
{
    const QicsMappedDimensionManager *dm = myPrimaryGrid->gridInfo().mappedDM();
    const QicsTable *table = qobject_cast<const QicsTable *>(parent());

    Q_ASSERT(table);

    if (type == RowIndex) {
        // map idx (comes in real coords) to scroller coords
        int real_idx = idx;
        const int freezeTopRowsCount = table->numFreezeTopRows();
        int freezeTopHiddenRowsCount = 0;

        for (int i = 0; i < real_idx; ++i)
            if (dm->isRowHidden(i)) {
                --idx;
                if (i < freezeTopRowsCount)
                    ++freezeTopHiddenRowsCount;
            }

        // now, idx has scroller coords, and real_idx has real coords - adjust them to fit ranges
        const int max = myVScroller->maxIndex();
        const int freezeTopVisibleRowsCount = freezeTopRowsCount - freezeTopHiddenRowsCount;

        if (freezeTopRowsCount && idx >= freezeTopVisibleRowsCount)
            idx -= freezeTopVisibleRowsCount;

        if (idx > max) {
            real_idx -= (idx - max);
            idx = max;
        }

        // change real index
        setRowIndex(real_idx);
        // change scroller index
        emit indexChanged(Qics::RowIndex, idx);
    }
    else {
        // map idx (comes in real coords) to scroller coords
        int real_idx = idx;
        const int freezeLeftColumnsCount = table->numFreezeLeftColumns();
        int freezeLeftHiddenColumnsCount = 0;

        for (int i = 0; i < real_idx; ++i)
            if (dm->isColumnHidden(i)) {
                --idx;
                if (i < freezeLeftColumnsCount)
                    ++freezeLeftHiddenColumnsCount;
            }

        // now, idx has scroller coords, and real_idx has real coords - adjust them to fit ranges
        const int max = myHScroller->maxIndex();
        const int freezeLeftVisibleColumnsCount = freezeLeftColumnsCount - freezeLeftHiddenColumnsCount;

        if (freezeLeftColumnsCount && idx >= freezeLeftVisibleColumnsCount)
            idx -= freezeLeftVisibleColumnsCount;

        if (idx > max) {
            real_idx -= (idx - max);
            idx = max;
        }

        // change real index
        setColumnIndex(real_idx);
        // change scroller index
        emit indexChanged(Qics::ColumnIndex, idx);
    }
}

void QicsScrollManager::setScrollerIndex(Qics::QicsIndexType type, int idx)
{
    QicsMappedDimensionManager *dm = myPrimaryGrid->gridInfo().mappedDM();

    QicsRegion vp = myPrimaryGrid->currentViewport();

    if (type == RowIndex) {
        int cr = vp.startRow();

        // skip hidden rows
        for (int i = cr; i <= idx+cr; ++i)
            if (dm->isRowHidden(i))
                idx++;

        setRowIndex(idx+cr);
    }
    else {
        int cr = vp.startColumn();

        // skip hidden columns
        for (int i = cr; i <= idx+cr; ++i)
            if (dm->isColumnHidden(i))
                idx++;

        setColumnIndex(idx+cr);
    }
}

void QicsScrollManager::setRowMinValue(int val)
{
    if (val < 0) val = 0;

    emit minIndexChanged(RowIndex, val);
}

void QicsScrollManager::setRowMaxValue(int val)
{
    if (val < 0) val = 0;

    if (val >= myVScroller->minIndex())
        emit maxIndexChanged(RowIndex, val);
}

void QicsScrollManager::setColumnMinValue(int val)
{
    if (val < 0) val = 0;

    emit minIndexChanged(ColumnIndex, val);
}

void QicsScrollManager::setColumnMaxValue(int val)
{
    if (val < 0) val = 0;

    if (val >= myHScroller->minIndex())
        emit maxIndexChanged(ColumnIndex, val);
}

void QicsScrollManager::gridBoundsChanged()
{
    QicsRegion vp = myPrimaryGrid->currentViewport();
    QicsGridInfo &gi = myPrimaryGrid->gridInfo();
    QicsMappedDimensionManager *dm = gi.mappedDM();
    QicsDataModel *dataModel = gi.dataModel();

    if (dataModel && (vp.numRows() > 0 || vp.numColumns() > 0)) {
        int lpsr = myPrimaryGrid->lastPageStartRow();
        if (lpsr < 0) lpsr = vp.endRow();
        int maxrow=qMax(lpsr, vp.startRow());

        int lpsc = myPrimaryGrid->lastPageStartColumn();
        if (lpsc < 0) lpsc = vp.endColumn();
        int maxcol=qMax(lpsc, vp.startColumn());

        int row_cr = vp.startRow();
        int col_cr = vp.startColumn();

        int lastrow = gi.lastNonHiddenRow(vp.startRow(), dataModel->lastRow());
        if (maxrow > lastrow) maxrow = lastrow;
        int lastcol = gi.lastNonHiddenColumn(vp.startColumn(), dataModel->lastColumn());
        if (maxcol > lastcol) maxcol = lastcol;

        for (int i = vp.startRow(); i <= maxrow; ++i)
            if (dm->isRowHidden(i)) ++row_cr;

        for (int j = vp.startColumn(); j <= maxcol; ++j)
            if (dm->isColumnHidden(j)) ++col_cr;

        setRowMinValue( vp.startRow() - gi.firstNonHiddenRow(vp.startRow(), maxrow) );
        setRowMaxValue(maxrow - row_cr);

        setColumnMinValue( vp.startColumn() - gi.firstNonHiddenColumn(vp.startColumn(), maxcol) );
        setColumnMaxValue(maxcol - col_cr);
    }
    else {
        setRowMinValue(-1);
        setRowMaxValue(-1);
        setColumnMinValue(-1);
        setColumnMaxValue(-1);
    }
}

void QicsScrollManager::computeVSPosition( int idx )
{
    myRowIndex = idx;
    emit rowIndexChanged(idx);
}

void QicsScrollManager::computeHSPosition( int idx )
{
    myColumnIndex = idx;

    emit columnIndexChanged(myColumnIndex);
}

void QicsScrollManager::handleScrollPageAdd(Qics::QicsIndexType type, int idx)
{
    if (type == RowIndex) {
        QAbstractSlider *vSlider = myVScroller->widget();
        if (vSlider) {
            int v_range = qMax(1, myPrimaryGrid->fullyVisibleBottomRow() - myPrimaryGrid->topRow() + 1);
            vSlider->setSliderPosition(v_range+idx);
        }
    }
    else {
        QAbstractSlider *hSlider = myHScroller->widget();
        if (hSlider) {
            int h_range = qMax(1, myPrimaryGrid->fullyVisibleRightColumn() - myPrimaryGrid->leftColumn() + 1);
            hSlider->setSliderPosition(h_range+idx);
        }
    }
}

void QicsScrollManager::handleScrollPageSub(Qics::QicsIndexType type, int idx)
{
    if (type == RowIndex) {
        QAbstractSlider *vSlider = myVScroller->widget();
        if (vSlider) {
            int v_range = qMax(1, myPrimaryGrid->fullyVisibleBottomRow() - myPrimaryGrid->topRow() + 1);
            vSlider->setSliderPosition(idx-v_range);
        }
    }
    else {
        QAbstractSlider *hSlider = myHScroller->widget();
        if (hSlider) {
            int h_range = qMax(1, myPrimaryGrid->fullyVisibleRightColumn() - myPrimaryGrid->leftColumn() + 1);
            hSlider->setSliderPosition(idx-h_range);
        }
    }
}


