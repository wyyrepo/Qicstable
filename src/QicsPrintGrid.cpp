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

#include "QicsPrintGrid.h"

#include <QStyleOption>
#include <QPainter>
#include <QFrame>
#include <QApplication>
#include <qdrawutil.h>


#include "QicsMainGrid.h"
#include "QicsDimensionManager.h"
#include "QicsSpanManager.h"
#include "QicsStyleManager.h"
#include "QicsDataModel.h"


static const int wpwidth = 2; // WinPanel lwidth


QicsPrintGrid::QicsPrintGrid(QicsGridInfo &info)
    : QicsGrid(info)
{
}

QicsPrintGrid::~QicsPrintGrid()
{
}

QPoint QicsPrintGrid::print(const QicsRegion &reg, const QPoint &start,
                     QPainter *painter, int *w, int *h)
{
    myBoundRect = QRect(start.x(), start.y(),
        (dimensionManager().regionWidth(reg) +
        (2 * m_mainGrid->verticalGridLineWidth()) +
        m_mainGrid->verticalShadeLineWidth()),
        (dimensionManager().regionHeight(reg) +
        (2 * m_mainGrid->horizontalGridLineWidth()) +
        m_mainGrid->horizontalShadeLineWidth()));

    int fw = frameWidth();

    computeCellPositions(myBoundRect.adjusted(fw, fw, -fw, -fw), reg.startCell());

    myAlreadyDrawnCells.clear();

    int rw = mappedDM().regionWidth(reg); if (w) *w = rw;
    int rh = mappedDM().regionHeight(reg); if (h) *h = rh;

    drawGridLines(reg, painter);
    drawRegion(reg, painter, QicsGrid::WholeCell);

    return QPoint((myBoundRect.left() + rw),
        (myBoundRect.top() + rh));
}

QicsRegion QicsPrintGrid::currentViewport() const
{
    return QicsRegion(0,0,dataModel()->lastRow(),dataModel()->lastColumn());
}

QicsRegion QicsPrintGrid::regionFromArea(const QRect &rect, const QicsICell &start) const
{
    QicsSpanManager *spm = styleManager().spanManager();
    QicsRegion real_vp = currentViewport();
    int row, col;
    int x, y;

    int fw = frameWidth();

    // Reserve space for frame
    QRect cr(QPoint(rect.left() + fw, rect.top() + fw),
        QPoint(rect.right() - fw, rect.bottom() - fw));

    y = cr.top();
    row =  start.row();

    while ((y <= cr.bottom()) && (row <= real_vp.endRow())) {
        QicsRegion rowspan = spm->maxSpanForRow(m_info, row);
        int row_height = mappedDM().regionHeight(rowspan);

        int proposed_y = y + row_height;

        if ((proposed_y > cr.bottom()) && (row > start.row()))
            break;

        y = proposed_y + m_mainGrid->horizontalGridLineWidth() + m_mainGrid->horizontalShadeLineWidth();
        row = rowspan.endRow() + 1;
    }
    // The row counter goes one too far
    --row;

    x = cr.left();
    col = start.column();

    while ((x <= cr.right()) && (col <= real_vp.endColumn())) {
        QicsRegion colspan = spm->maxSpanForColumn(m_info, col);
        int col_width = mappedDM().regionWidth(colspan);

        int proposed_x = x + col_width;

        if ((proposed_x > cr.right()) && (col > start.column()))
            break;

        x = proposed_x + m_mainGrid->verticalGridLineWidth() + m_mainGrid->verticalShadeLineWidth();
        col = colspan.endColumn() + 1;
    }
    // The column counter goes one too far
    --col;

    return (QicsRegion(start, QicsICell(row, col)));
}

QSize QicsPrintGrid::preferredSize() const
{
    QSize size;
    int fw = frameWidth();

    QicsRegion real_vp = currentViewport();

    if (m_info.gridType() == Qics::RowHeaderGrid) {
        size.setWidth(dimensionManager().regionWidth(QicsRegion(real_vp.startRow(),
            real_vp.startColumn(),
            real_vp.startRow(),
            real_vp.endColumn())) +
            (2 * fw));
    }
    else if (m_info.gridType() == Qics::ColumnHeaderGrid) {
        size.setHeight(dimensionManager().regionHeight(QicsRegion(real_vp.startRow(),
            real_vp.startColumn(),
            real_vp.endRow(),
            real_vp.startColumn())) +
            (2 * frameWidth()));
    }

    return size;
}

int QicsPrintGrid::frameWidth() const
{
    int lw = m_mainGrid->frameLineWidth();
    int fls = m_mainGrid->frameStyle();
    int mlw = 0;
    int margin = 0;

    int ftype  = fls ;//& QFrame::Shape;
    int fstyle = fls ;//& QFrame::MShadow;

    int fwidth = -1;

    switch (ftype)
    {
    case QFrame::NoFrame:
        fwidth = 0;
        break;
    case QFrame::Box:
        switch (fstyle)
        {
        case QFrame::Plain:
            fwidth = lw;
            break;
        case QFrame::Raised:
        case QFrame::Sunken:
            fwidth = static_cast<int> (lw*2 + mlw);
            break;
        }
        break;
    case QFrame::StyledPanel:
        switch (fstyle)
        {
        case QFrame::Plain:
        case QFrame::Raised:
        case QFrame::Sunken:
            fwidth = lw;
            break;
        }
        break;
    case QFrame::WinPanel:
        switch (fstyle)
        {
        case QFrame::Plain:
        case QFrame::Raised:
        case QFrame::Sunken:
            fwidth =  wpwidth; //WinPanel does not use lw!
            break;
        }
        break;
#ifdef NOTDEF
    case QFrame::StyledPanel:
        fwidth = style.pixelMetric(QStyle::PM_MenuBarFrameWidth);
        break;
    case QFrame::StyledPanel:
        fwidth = style.pixelMetric(QStyle::PM_DockWindowFrameWidth);
        break;
#endif
    case QFrame::HLine:
    case QFrame::VLine:
        switch (fstyle)
        {
        case QFrame::Plain:
            fwidth = lw;
            break;
        case QFrame::Raised:
        case QFrame::Sunken:
            fwidth = static_cast<int> (lw*2 + mlw);
            break;
        }
        break;
    }

    if (fwidth == -1)   // invalid style
        fwidth = 0;

    fwidth += margin;

    return fwidth;
    return 1;
}

void QicsPrintGrid::drawFrame(const QRect &rect, int fstyle, int lw, int mlw,
                              const QPalette &pal, QPainter *painter)
{
    QPoint p1, p2;
    int type = fstyle ;//& QFrame::MShape;
    int cstyle = fstyle ;//& QFrame::MShadow;

    QStyle *style = QApplication::style();

    QStyleOption opt(lw, mlw);

    QStyle::State flags = QStyle::State_Enabled;

    if (cstyle == QFrame::Sunken)
        flags |= QStyle::State_Sunken;
    else if (cstyle == QFrame::Raised)
        flags |= QStyle::State_Raised;

    switch (type)
    {
    case QFrame::Box:
        if (cstyle == QFrame::Plain)
            style->drawPrimitive(QStyle::PE_FrameFocusRect, &opt, painter, 0);
        break;
    case QFrame::StyledPanel:
        style->drawPrimitive(QStyle::PE_PanelLineEdit, &opt, painter);
        break;
    case QFrame::Panel:
        if (cstyle == QFrame::Plain)
            style->drawPrimitive(QStyle::PE_FrameFocusRect, &opt, painter, 0);
        break;
    case QFrame::HLine:
    case QFrame::VLine:
        if (type == QFrame::HLine) {
            p1 = QPoint(rect.x(), rect.height()/2);
            p2 = QPoint(rect.x()+rect.width(), p1.y());
        }
        else {
            p1 = QPoint(rect.x()+rect.width()/2, 0);
            p2 = QPoint(p1.x(), rect.height());
        }
        if (cstyle == QFrame::Plain) {
            QPen oldPen = painter->pen();
            painter->setPen(QPen(pal.foreground().color(), lw));
            painter->drawLine(p1, p2);
            painter->setPen(oldPen);
        }
        else
            qDrawShadeLine(painter, p1, p2, pal, cstyle == QFrame::Sunken,
            lw, mlw);
        break;
    }
}


