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

#include "QicsHeader.h"

#include <QApplication>
#include "QicsStyleManager.h"
#include "QicsScreenGrid.h"
#include "QicsCellRegion.h"
#include "QicsRow.h"
#include "QicsColumn.h"
#include "QicsCell.h"
#include "QicsHeaderGrid.h"
#include "QicsMappedDimensionManager.h"
#include "QicsCellDisplay.h"


QicsHeader::QicsHeader(QicsGridInfo *info, QObject *parent, bool forward_signals)
    : QicsGridCommon(info, parent, forward_signals),
        myCell(0),
        myRow(0),
        myColumn(0),
        myCellRegion(0),
        m_headerGrid(0)
{
    initSignals();
}

QicsHeader::~QicsHeader()
{
    m_headerGrid = 0;
    delete myCellRegion;
}

QicsRow &QicsHeader::rowRef(int rownum) const
{
    QicsHeader *self = const_cast<QicsHeader *> (this);

    if (!self->myRow)
        self->myRow = new QicsRow(rownum, m_info, true, self);
    else
        self->myRow->setRowIndex(rownum);

    return (*myRow);
}

QicsRow *QicsHeader::row(int idx, bool follow_model) const
{
    return (new QicsRow(idx, m_info, follow_model, parent()));
}

QicsColumn &QicsHeader::columnRef(int colnum) const
{
    QicsHeader *self = const_cast<QicsHeader *> (this);

    if (!self->myColumn)
        self->myColumn = new QicsColumn(colnum, m_info, true, self);
    else
        self->myColumn->setColumnIndex(colnum);

    return (*myColumn);
}

QicsColumn *QicsHeader::column(int idx, bool follow_model) const
{
    return (new QicsColumn(idx, m_info, follow_model, parent()));
}

QicsCell &QicsHeader::cellRef(int rownum, int colnum) const
{
    QicsHeader *self = const_cast<QicsHeader *> (this);

    if (!self->myCell)
        self->myCell = new QicsCell(rownum, colnum, m_info, true, self);
    else {
        self->myCell->setRowIndex(rownum);
        self->myCell->setColumnIndex(colnum);
    }

    return (*myCell);
}

QicsCell *QicsHeader::cell(int row, int col, bool follow_model) const
{
    return (new QicsCell(row, col, m_info, follow_model, parent()));
}

bool QicsHeader::allowUserResize() const
{
    return (* static_cast<bool *>
        (styleManager().getGridProperty(QicsGridStyle::AllowUserResize)));
}

void QicsHeader::setAllowUserResize(bool b)
{
    styleManager().setGridProperty(QicsGridStyle::AllowUserResize,
        static_cast<void *> (&b));
}

bool QicsHeader::allowUserMove() const
{
    return (* static_cast<bool *>
        (styleManager().getGridProperty(QicsGridStyle::AllowUserMove)));
}

void QicsHeader::setAllowUserMove(bool b)
{
    styleManager().setGridProperty(QicsGridStyle::AllowUserMove,
        static_cast<void *> (&b));
}

bool QicsHeader::allowUserEdit() const
{
    return (* static_cast<bool *>
        (styleManager().getGridProperty(QicsGridStyle::AllowUserEdit)));
}

void QicsHeader::setAllowUserEdit(bool b)
{
    styleManager().setGridProperty(QicsGridStyle::AllowUserEdit,
        static_cast<void *> (&b));
}

bool QicsHeader::allowHeaderResize() const
{
    return (* static_cast<bool *>
        (styleManager().getGridProperty(QicsGridStyle::AllowHeaderResize)));
}

void QicsHeader::setAllowHeaderResize(bool b)
{
    styleManager().setGridProperty(QicsGridStyle::AllowHeaderResize,
        static_cast<void *> (&b));
}

const QCursor &QicsHeader::resizeCursor() const
{
    return (* static_cast<QCursor *>
        (styleManager().getGridProperty(QicsGridStyle::ResizeCursor)));
}

void QicsHeader::setResizeCursor(const QCursor &cursor)
{
    styleManager().setGridProperty(QicsGridStyle::ResizeCursor,
        static_cast<const void *> (&cursor));
}

void QicsHeader::connectGrid(QicsScreenGrid *grid)
{
    QicsGridCommon::connectGrid(grid);

    m_headerGrid = qobject_cast<QicsHeaderGrid*>(grid);
    if (m_headerGrid) {
        connect(grid, SIGNAL(sizeChange(int, int, int, Qics::QicsHeaderType)),
            this, SIGNAL(sizeChange(int, int, int, Qics::QicsHeaderType)));
        connect(grid, SIGNAL(resizeInProgress(int, int, Qics::QicsHeaderType)),
            this, SIGNAL(resizeInProgress(int, int, Qics::QicsHeaderType)));
        connect(grid, SIGNAL(gripDoubleClicked(int, int, Qics::QicsHeaderType)),
            this, SIGNAL(gripDoubleClicked(int, int, Qics::QicsHeaderType)));
    }
}

QicsCellRegion* QicsHeader::cellRegion(const QicsRegion &region)
{
    delete myCellRegion;
    myCellRegion = new QicsCellRegionHeader(region, this, m_info->gridType(), &styleManager(), &dimensionManager());
    return myCellRegion;
}

bool QicsHeader::editCell(int row, int col)
{
    if (!m_headerGrid) return false;

    return m_headerGrid->editCell(row, col);
}

void QicsHeader::toggleRowHeightExpansion(int row, bool setOverrideCursor)
{
    if (gridInfo().mappedDM()->overriddenRowHeight(row) >= 0 && autoFitMode() == Qics::Native) {
        gridInfo().mappedDM()->resetRowHeight(row);
        return;
    }

    if (setOverrideCursor)//#90147
        QApplication::setOverrideCursor(Qt::WaitCursor);

    QicsRegion vp = m_headerGrid->currentViewport();

    int h = 0;
    QicsCellDisplay *cd = 0;
    const QicsDataItem *itm = 0;

    QicsCell cell(row, 0, &gridInfo(), false);

    const int endColumn = vp.endColumn();
    for (int i = vp.startColumn(); i <= endColumn; ++i) {
        cell.setColumnIndex(i);
        cd = cell.displayer();
        itm = cell.dataValue();

        h = qMax(h, cd->sizeHint(m_headerGrid, row, i, itm).height());
    }

    h = qMax(h, gridInfo().mappedDM()->rowMinHeight(row));
    gridInfo().mappedDM()->overrideRowHeight(row, h);

    if (setOverrideCursor)//#90147
        QApplication::restoreOverrideCursor();
}

void QicsHeader::toggleColumnWidthExpansion(int col, bool setOverrideCursor)
{
    if (gridInfo().mappedDM()->overriddenColumnWidth(col) >= 0 && autoFitMode() == Qics::Native) {
        gridInfo().mappedDM()->resetColumnWidth(col);
        return;
    }

    if (setOverrideCursor)//#90147
        QApplication::setOverrideCursor(Qt::WaitCursor);

    QicsRegion vp = m_headerGrid->currentViewport();

   int w = 0;
    QicsCellDisplay *cd = 0;
    const QicsDataItem *itm = 0;

    QicsCell cell(0, col, &gridInfo(), false);

    const int endRow = vp.endRow();
    for (int i = vp.startRow(); i <= endRow; ++i) {
        cell.setRowIndex(i);
        cd = cell.displayer();
        itm = cell.dataValue();

        w = qMax(w, cd->sizeHint(m_headerGrid, i, col, itm).width());
    }

    w = qMax(w, gridInfo().mappedDM()->columnMinWidth(col));
    gridInfo().mappedDM()->overrideColumnWidth(col, w);

    if (setOverrideCursor)//#90147
        QApplication::restoreOverrideCursor();
}


