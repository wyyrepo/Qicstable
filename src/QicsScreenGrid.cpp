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

#include "QicsScreenGrid.h"

#include <QRubberBand>
#include <QToolTip>
#include <QDesktopWidget>
#include <QApplication>
#include <QDropEvent>
#include <QDrag>

#include "QicsCell.h"
#include "QicsTableCommon.h"
#include "QicsRow.h"
#include "QicsColumn.h"
#include "QicsMainGrid.h"
#include "QicsCellDisplay.h"
#include "QicsDimensionManager.h"
#include "QicsSelectionManager.h"
#include "QicsSpanManager.h"
#include "QicsKeyboardManager.h"
#include "QicsStyleManager.h"
#include "QicsUtil.h"
#include "QicsTableGrid.h"
#include "QicsWidgetCellDisplay.h"
#include "QicsListCellDisplay.h"



QicsScreenGrid::QicsScreenGrid(QWidget *w, QicsGridInfo &info,
                               int top_row,
                               int left_column)
    : QFrame(w),
       QicsGrid(info, top_row, left_column),
       myReqVisibleRows(-1),
       myReqVisibleColumns(-1),
       myLastResizeLinePosition(-1),
       myNeedsRecomputeCellsFlag(Qics::RowAndColumnIndex),
       myNeedsRecomputeLastPageFlag(Qics::RowAndColumnIndex),
       myNeedsRepaintFlag(false),
       myLastPageStartRow(-1),
       myLastPageStartColumn(-1),
       m_heightFixed(false),
       m_widthFixed(false),
       myEditingCurrentCell(0),
       myUneditCell(0, 0),
       m_editable(false),
       myPlacingEntryWidgetFlag(false),
       m_isMainGrid(false),
       m_repaintAll(true),
       m_gridInPaintEvent(false),
       m_initialRepaint(true),
       myForceSkipPaintEvent(false),
       m_doingSelectionFlag(false),
       myDropAction(Qt::CopyAction),
       m_selectOnTraverse(true),
       timerScrolling(0),
       m_scrollDirec(Qics::ScrollNone),
       m_paintRegion()
{
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);

    myLastPressedKey = 0;

    // We will need this for cursor changes
    setMouseTracking(true);
    setAcceptDrops(true);

    setFocusPolicy(Qt::StrongFocus);

    updateLineWidth();
    updateFrameStyle();

    m_resizeLine = new QRubberBand(QRubberBand::Rectangle, parentWidget());
    QPalette pal = m_resizeLine->palette();
    pal.setColor(QPalette::Highlight,"darkgrey");
    m_resizeLine->setPalette(pal);
    m_resizeLine->hide();

    timerScrolling = new QTimer(this);
}

QicsScreenGrid::~QicsScreenGrid()
{
    if (isFrozen()) {
        foreach (QObject *obj, children()) {
            QicsCellDisplay *cd = dynamic_cast<QicsCellDisplay *>(obj);
            if (cd && cd->widget()) {
                cd->widget()->setParent(0);
            }
        }
    }
}

void QicsScreenGrid::hideResizeBar()
{
    m_resizeLine->hide();
}

bool QicsScreenGrid::event(QEvent *event)
{
#ifdef Q_OS_WIN32
    // forcing skip paintEvent inside paintEvent when child==QicsWidgetCellDisplay added.
    if (event->type() == QEvent::Paint) {
        QPaintEvent* pe  = static_cast<QPaintEvent *>(event);
        if (myForceSkipPaintEvent || m_gridInPaintEvent) {
            myForceSkipPaintEvent = false;
            event->ignore();
            // comboBox first show problem
            update(pe->rect());

            return false;
        }
    }

    if (event->type() == QEvent::ChildPolished) {
        QChildEvent *childEvent = static_cast<QChildEvent *>( event );
        myForceSkipPaintEvent = qobject_cast<QicsWidgetCellDisplay*>(childEvent->child());
    }
#endif

    if (event->type() == QEvent::ToolTip) {
        QString text;
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        QicsICell cell = cellAt(helpEvent->pos().x(), helpEvent->pos().y(), false);

        if (cell.isValid()) {
            // First, get the tooltip property for this cell
            m_cell->setRowIndex(cell.row());
            m_cell->setColumnIndex(cell.column());
            text = m_cell->toolTipText();
            QPoint offset = m_mainGrid->cellToolTipOffset();

            // If not, check an overflow tooltip
            if (text.isEmpty() && m_mainGrid->cellOverflowBehavior() == Qics::ToolTip) {
                QicsCellDisplay *cd = cellDisplay(cell.row(), cell.column());
                QRect rect = cellDimensions(cell, true);
                const QicsDataItem *itm = cellValue(cell.row(), cell.column());
                text = cd->tooltipText(&m_info, cell.row(), cell.column(), itm, rect);
            }

            // Display tooltip
            if (!text.isEmpty()) {
                QRect cellDimRect = cellDimensions(cell, true);
                QFontMetrics fontMetrics(m_cell->font());
                QDesktopWidget * d = QApplication::desktop();
                QPoint point(cellDimRect.x(), cellDimRect.y());
                QPoint pointGlobal = mapToGlobal(point);

                QRect geom = d->screenGeometry(pointGlobal);
                int fontHeight, fontWidth;
                fontHeight = fontMetrics.height();
                fontWidth = fontMetrics.maxWidth(); // Maximum width of char

                int maxStrWidth = geom.x()+geom.width()-pointGlobal.x();
                int size=0;
                QString retStr = "<p>";

                cellDimRect.setHeight(fontHeight + 2);

                for (int ii = 0; ii < text.size(); ++ii) {
                    if (text.at(ii) == QChar('\r')) continue;
                    if (text.at(ii) == QChar('\n')) {
                        retStr.append(QString("<br>"));
                        cellDimRect.setHeight(cellDimRect.height() + fontHeight - fontMetrics.descent());
                        continue;
                    }

                    fontWidth = fontMetrics.width( text.at(ii));
                    size += fontWidth;
                    if (size < (maxStrWidth - 15))
                        retStr.append( text.at( ii ) );
                    else {
                        retStr.append(QString("<br>") + text.at( ii ));
                        size = 0;
                    }
                }

                QToolTip::showText(helpEvent->globalPos() + offset, retStr, this, cellDimRect);
            }
            else {
                QToolTip::hideText();
            }
        }
    }

    return QFrame::event(event);
}

void QicsScreenGrid::reset()
{
    // invalidates the current cell
    setCurrentCell(QicsICell());
}

///////////////////////////////////////////////////////////////////////
////////////// Viewport related methods ///////////////////////////////
///////////////////////////////////////////////////////////////////////

void QicsScreenGrid::setTopRow(int value)
{
    QicsRegion vp = realViewport();

    if ((value >= vp.startRow()) && (value <= vp.endRow())) {
        m_topRow = gridInfo().firstNonHiddenRow(value, vp.endRow());
        recomputeCellPositions(Qics::RowIndex);
        recomputeAndDraw(Qics::RowIndex);
    }
}

void QicsScreenGrid::setLeftColumn(int value)
{
    QicsRegion vp = realViewport();

    if ((value >= vp.startColumn()) && (value <= vp.endColumn())) {
        m_leftColumn = gridInfo().firstNonHiddenColumn(value, vp.endColumn());
        recomputeCellPositions(Qics::ColumnIndex);
        recomputeAndDraw(Qics::ColumnIndex);
    }
}

void QicsScreenGrid::setViewport(const QicsRegion &reg)
{
    QicsGrid::setViewport(reg);
    updateViewport();

    recomputeAndDraw(Qics::RowAndColumnIndex);
}

void QicsScreenGrid::fixHeightToViewport(bool set)
{
    m_heightFixed = set;

    if (set) {
        QicsRegion reg = currentViewport();
        if (reg.numRows() == 0) {
            setFixedHeight(0);
            return;
        }

        int top;
        getContentsMargins(0, &top, 0, 0);

        setFixedHeight(mappedDM().regionHeight(reg) +
            (2 * m_mainGrid->horizontalGridLineWidth()) +
            m_mainGrid->horizontalShadeLineWidth() +
            (2 * frameWidth()) + top);
    }
    else {
        QSizePolicy sp = sizePolicy();
        sp.setVerticalPolicy(QSizePolicy::Expanding);
        setSizePolicy(sp);
    }
}

void QicsScreenGrid::fixWidthToViewport(bool set)
{
    m_widthFixed = set;

    if (set) {
        QicsRegion reg = currentViewport();
        if (reg.numColumns() == 0) {
            setFixedWidth(0);
            return;
        }

        int left;
        getContentsMargins(&left, 0, 0, 0);

        setFixedWidth(mappedDM().regionWidth(reg) +
            (2 * m_mainGrid->verticalGridLineWidth()) +
            m_mainGrid->verticalShadeLineWidth() +
            (2 * frameWidth()) + left);
    }
    else {
        QSizePolicy sp = sizePolicy();
        sp.setHorizontalPolicy(QSizePolicy::Expanding);
        setSizePolicy(sp);
    }
}

// make this minimum size the size of ONE cell.
QSize QicsScreenGrid::minimumSizeHint() const
{
    QSize size;
    size.setWidth((mappedDM().columnWidth(m_leftColumn) +
        (2 * m_mainGrid->verticalGridLineWidth()) +
        m_mainGrid->verticalShadeLineWidth() +
        (2 * frameWidth())));

    size.setHeight((mappedDM().rowHeight(m_topRow) +
        (2 * m_mainGrid->horizontalGridLineWidth()) +
        m_mainGrid->horizontalShadeLineWidth() +
        (2 * frameWidth())));

    return size;
}

QSize QicsScreenGrid::sizeHint() const
{
    int width;
    int height;

    if (myReqVisibleColumns > 0) {
        QicsRegion viz(m_topRow, m_leftColumn,
            m_topRow, (m_leftColumn + myReqVisibleColumns - 1));

        width = (mappedDM().regionWidth(viz) +
            (2 * m_mainGrid->verticalGridLineWidth()) +
            m_mainGrid->verticalShadeLineWidth() +
            (2 * frameWidth()));
    }
    else
        width = -1;

    if (myReqVisibleRows > 0) {
        QicsRegion viz(m_topRow, m_leftColumn,
            (m_topRow + myReqVisibleRows - 1), m_leftColumn);

        height = (mappedDM().regionHeight(viz) +
            (2 * m_mainGrid->horizontalGridLineWidth()) +
            m_mainGrid->horizontalShadeLineWidth() +
            (2 * frameWidth()));
    }
    else
        height = -1;

    return QSize(width, height);
}

int QicsScreenGrid::visibleRows()
{
    layout();

    int numVisibleRows = 0;

    for (int i = m_topRow; i <= m_bottomRow; ++i) {
        m_row->setRowIndex(i);
        if (m_row->isHidden())
            continue;
        ++numVisibleRows;
    }

    return numVisibleRows;
}

int QicsScreenGrid::fullyVisibleBottomRow()
{
    computeCellPositions(Qics::RowIndex);

    if (visibleRows() > 0)
        return myFullyVisibleBottomRow;

    return -1;
}

int QicsScreenGrid::visibleColumns()
{
    layout();

    int numVisibleColumns = 0;

    for (int i = m_leftColumn; i <= m_rightColumn; ++i) {
        m_column->setColumnIndex(i);
        if (m_column->isHidden())
            continue;
        ++numVisibleColumns;
    }

    return numVisibleColumns;
}

int QicsScreenGrid::fullyVisibleRightColumn()
{
    computeCellPositions(Qics::ColumnIndex);

    if (visibleColumns() > 0)
        return myFullyVisibleRightColumn;

    return -1;
}

void QicsScreenGrid::setVisibleRows(int num)
{
    myReqVisibleRows = num;
    layout();
    updateGeometry();
}

void QicsScreenGrid::setVisibleColumns(int num)
{
    myReqVisibleColumns = num;
    layout();
    updateGeometry();
}

///////////////////////////////////////////////////////////////////////
/////////////////  Grid layout methods  ///////////////////////////////
///////////////////////////////////////////////////////////////////////

void QicsScreenGrid::layout()
{
    if (myNeedsRecomputeCellsFlag != Qics::NoIndex)
        computeCellPositions(myNeedsRecomputeCellsFlag);
    if (myNeedsRecomputeLastPageFlag != Qics::NoIndex)
        computeLastPage(myNeedsRecomputeLastPageFlag);
    m_repaintAll = true;
}

void QicsScreenGrid::computeCellPositions(Qics::QicsIndexType indexType)
{
    if (indexType == Qics::NoIndex)
        return;

    QicsICell end_cell = QicsGrid::computeCellPositions(contentsRect(),
        QicsICell(m_topRow, m_leftColumn), indexType);

    // After resize do the redraw in buffer only if region changed

    if ((end_cell.row() != m_bottomRow) || (end_cell.column() != m_rightColumn)) {
        m_repaintAll = true;
        m_initialRepaint = true;
    }

    m_bottomRow = end_cell.row();
    m_rightColumn = end_cell.column();
    myNeedsRecomputeCellsFlag = Qics::NoIndex;

    // Now that we have computed the new positions, we have to notify
    // the cell displayers of any cells that need to know when they
    // have moved off screen.  (This is mostly for QicsWidgetCellDisplay
    // objects, so they can hide their widgets.)

    QicsICellV::const_iterator iter, iter_end(m_cellsToNotify.constEnd());
    QicsRegion visible_reg(m_topRow, m_leftColumn, m_bottomRow, m_rightColumn);
    //QicsSpanManager *spm = styleManager().spanManager();

    for (iter = m_cellsToNotify.constBegin(); iter != iter_end; ++iter) {
        QicsICell cell = *iter;
        int visRow = m_info.visualRowIndex(cell.row());
        int visCol = m_info.visualColumnIndex(cell.column());

        {
            //QicsRegion span;
            //spm->insideSpan(m_info, visRow, visCol, span);
            //QicsRegion intersect_reg = span.intersect( visible_reg );
            if (!isCellVisible(visRow, visCol) /*&& !intersect_reg.isEmpty()*/) {
                QicsCellDisplay *cd = cellDisplay(visRow, visCol);
                if (cd)
                    cd->endEdit(this, visRow, visCol);
            }
        }
    }

    // Ok, we've notified everybody.  Now let's clear the notify list
    // and rebuild it with the cells from the new positions.
    // TODO: here we can increase performance by adding/removing only one cell display
    //       not necessary to rebuild all list every time.

    m_cellsToNotify.clear();

    for (int i = m_topRow; i <= m_bottomRow; ++i) {
        m_row->setRowIndex(i);

        if (m_row->isHidden())
            continue;

        for (int j = m_leftColumn; j <= m_rightColumn; ++j) {
            m_column->setColumnIndex(j);

            if (m_column->isHidden())
                continue;

            QicsCellDisplay *cd = cellDisplay(i, j);

            if (cd && cd->needsVisibilityNotification()) {
                m_cellsToNotify.push_back(QicsICell(modelRowIndex(i),
                    modelColumnIndex(j)));
            }
        }
    }
}

void QicsScreenGrid::computeLastPage(Qics::QicsIndexType indexType)
{
    if (indexType == Qics::NoIndex)
        return;

    int start_row, start_col;

    // We are going backwards, from the bottom right of the table.
    // The goal is to figure out how many rows and columns would
    // be displayed if the bottom-right cell was in the bottom-right
    // spot on the grid.  This information will be used by the table
    // to set the correct values on the scrollbars

    QicsRegion real_vp = currentViewport();

    // If the viewport is invalid (i.e. the model is empty), bail.
    if ((!(real_vp.numRows()>0)) && (!(real_vp.numColumns()>0))) {
        myLastPageStartRow = 0;
        myLastPageStartColumn = 0;

        myNeedsRecomputeLastPageFlag = Qics::NoIndex;

        emit newBoundsInfo();
        return;
    }

    QRect contents = contentsRect();

    if (indexType == Qics::RowIndex || indexType == Qics::RowAndColumnIndex) {
        myLastPageStartRow = -1;
        //
        // Compute row locations/sizes here
        //
        int this_row = real_vp.endRow();
        int hlw = m_mainGrid->horizontalGridLineWidth() + m_mainGrid->horizontalShadeLineWidth();

        int current_y = 0;
        while (this_row >= real_vp.startRow()) {
            m_row->setRowIndex(this_row);
            if (!m_row->isHidden()) {
                int proposed_y = (current_y + mappedDM().rowHeight(this_row) + hlw);
                if ((proposed_y + hlw) > contents.height())
                    break;
                current_y = proposed_y;
            }
            --this_row;
        }
        start_row = this_row;

        myLastPageStartRow = start_row + 1;
        if (gridInfo().mappedDM()->isRowHidden(myLastPageStartRow)) {
            myLastPageStartRow = gridInfo().firstNonHiddenRow(myLastPageStartRow, real_vp.numRows());
        }
    }

    if (indexType == Qics::ColumnIndex || indexType == Qics::RowAndColumnIndex) {
        myLastPageStartColumn = -1;
        //
        // Compute column locations/sizes here
        //
        int vlw = m_mainGrid->verticalGridLineWidth() + m_mainGrid->verticalShadeLineWidth();

        if (isRightToLeft()) {   //   <<<<----
            // applications layout == right - to  - left
            int this_col = real_vp.endColumn();
            int current_x = contents.width();
            while (this_col >= real_vp.startColumn()) {
                m_column->setColumnIndex(this_col);
                if (!m_column->isHidden()) {
                    int proposed_x = (current_x - mappedDM().columnWidth(this_col) - vlw);
                    if ((proposed_x + vlw) < frameWidth())
                        break;
                    current_x = proposed_x;
                }
                --this_col;
            }
            start_col = this_col;
        }
        else {                               //  ---->>>>
            int this_col = real_vp.endColumn();
            int current_x = 0;
            while (this_col >= real_vp.startColumn()) {
                m_column->setColumnIndex(this_col);
                if (!m_column->isHidden()) {
                    int proposed_x = (current_x + mappedDM().columnWidth(this_col) + vlw);
                    if ((proposed_x + vlw) > contents.width())
                        break;
                    current_x = proposed_x;
                }
                --this_col;
            }
            start_col = this_col;
        }
        myLastPageStartColumn = start_col + 1;
        if (gridInfo().mappedDM()->isColumnHidden(myLastPageStartColumn)) {
            myLastPageStartColumn = gridInfo().firstNonHiddenColumn(myLastPageStartColumn, real_vp.numColumns());
        }
    }

    myNeedsRecomputeLastPageFlag = Qics::NoIndex;
    emit newBoundsInfo();
}
int QicsScreenGrid::lastPageStartRow()
{
    layout();

    return myLastPageStartRow;
}

int QicsScreenGrid::lastPageStartColumn()
{
    layout();

    return myLastPageStartColumn;
}

QicsICell QicsScreenGrid::cellAt(int x, int y, bool nearest) const
{
    int row = rowAt(y, nearest);
    int col = columnAt(x, nearest);

    if (row < 0 || col < 0) return QicsICell();

    QicsRegion span;
    QicsSpanManager *spm = styleManager().spanManager();

    if (spm->insideSpan(m_info, row, col, span)) {
        m_cell->setRowIndex(span.startRow());
        m_cell->setColumnIndex(span.startColumn());

        if (!m_cell->isHidden()) {
            row = span.startRow();
            col = span.startColumn();
        }
    }

    return QicsICell(row, col);
}

int QicsScreenGrid::rowAt(int y, bool nearest) const
{
    int i;

    int nrows = myRowPositions.size();

    // Sanity check the row position list
    if ((nrows < (m_bottomRow - m_topRow)) || (m_bottomRow == -1 || m_topRow == -1))
        return (-2);

    // If it's above the top row, return the top row (if we were asked
    // for the nearest row) or -1.
    if ((nrows > 0) && (y < myRowPositions.at(m_topRow)))
        return (nearest ? m_topRow : -1);

    // Now go through each row and check the position
    for (i = m_topRow; i <= m_bottomRow; ++i) {
        // Don't bother with this row if it is hidden
        m_row->setRowIndex(i);

        if (m_row->isHidden())
            continue;

        if (y >= myRowPositions.at(i)) {
            if (i == m_bottomRow) {
                // This is the last row

                if (nearest)
                    return (i);
                else {
                    // Check if the y value is within the last row's
                    // boundary.
                    int row_height =
                        mappedDM().rowHeight(i);

                    if (y > (myRowPositions.at(i) + row_height))
                        return -1;
                    else
                        return (i);
                }
            }
            else {
                int next_row = gridInfo().firstNonHiddenRow(i + 1, m_bottomRow);

                if ((next_row >= 0) && y < myRowPositions.at(next_row)) {
                    // The y value is within this row's boundary.
                    return (i);
                }
            }
        }
        else
            return (i); // if (y < myRowPositions[i])
    }

    // If we get here, the y value is too big, so we return the bottom
    // row (if we were asked for the nearest row) or -1.

    if (nearest)
        return m_bottomRow;
    return -1;
}

int QicsScreenGrid::columnAt(int x, bool nearest) const
{
    int ncols =  myColumnPositions.size();
    int i;

    // Sanity check the column position list
    if ((ncols < (m_rightColumn - m_leftColumn)) || (m_rightColumn == -1 || m_leftColumn == -1))
        return (-2);

    // If it's to the left of the left column, return the left column
    // (if we were asked for the nearest column) or -1.
    if (isRightToLeft()) { //   <<<<----
        if ((ncols > 0) && (x >frameRect().right()))
            return (nearest ?  m_leftColumn : -1);
        if ((ncols > 0) && (x < myColumnPositions.at(m_rightColumn))) {
            if (nearest)
                return m_rightColumn;
            else
                return -1;
        }
    }
    else {                              //   ---->>>>
        if ((ncols > 0) && (x < myColumnPositions.at(m_leftColumn)))
            return (nearest ?  m_leftColumn : -1);
    }

    // Now go through each column and check the position
    if (isRightToLeft()) { //   <<<<----
        for (i = m_leftColumn; i <= m_rightColumn; ++i) {
            // Don't bother with this column if it is hidden
            m_column->setColumnIndex(i);

            if (m_column->isHidden())
                continue;

            if (x <= myColumnPositions.at(i) || ((i==0) &&  (x <= frameRect().right())) ) {
                if (i == m_rightColumn) {
                    // This is the last column

                    if (nearest)
                        return (i);
                    else {
                        // Check if the x value is within the last column's
                        // boundary.
                        int col_width = mappedDM().columnWidth(i);

                        if (x > (myColumnPositions.at(i) + col_width))
                            return -1;
                        else
                            return (i);
                    }
                }
                else {
                    int next_col = gridInfo().firstNonHiddenColumn(i, m_rightColumn);

                    if ((next_col >= 0) && (x > myColumnPositions.at(next_col))) {
                        // The x value is within this column's boundary.
                        return (i);
                    }
                }
            }
            else
                return (i); // if (x < myColumnPositions[i])
        }
    }
    else {    //   --->>>>
        for (i = m_leftColumn; i <= m_rightColumn; ++i) {
            // Don't bother with this column if it is hidden
            m_column->setColumnIndex(i);

            if (m_column->isHidden())
                continue;

            if (x >= myColumnPositions.at(i)) {
                if (i == m_rightColumn) {
                    // This is the last column

                    if (nearest)
                        return (i);
                    else {
                        // Check if the x value is within the last column's
                        // boundary.
                        int col_width =
                            mappedDM().columnWidth(i);

                        if (x > (myColumnPositions.at(i) + col_width))
                            return -1;
                        else
                            return (i);
                    }
                }
                else {
                    int next_col = gridInfo().firstNonHiddenColumn(i + 1, m_rightColumn);

                    if ((next_col >= 0) && (x < myColumnPositions.at(next_col))) {
                        // The x value is within this column's boundary.
                        return (i);
                    }
                }
            }
            else
                return (i); // if (x < myColumnPositions[i])
        }
    }

    // If we get here, the x value is too big, so we return the rightmost
    // column (if we were asked for the nearest column) or -1.

    if (nearest)
        return m_rightColumn;

    return -1;
}

///////////////////////////////////////////////////////////////////////
/////////////////  Grid drawing methods  //////////////////////////////
///////////////////////////////////////////////////////////////////////

void QicsScreenGrid::resetAndDraw()
{
    recomputeAndDraw(Qics::RowAndColumnIndex);
}

void QicsScreenGrid::resetAndDraw(int position, int /*num_items_added_removed*/,
                             Qics::QicsIndexType indexType)
{
    // NOTE: what about repaint off state

    int visual_coord = position;

    switch( indexType )
    {
    case Qics::RowIndex:
        visual_coord = m_info.visualRowIndex( position );
        // do redraw only if changes was in visible area
        if ((visual_coord <= m_bottomRow + 1)) {
            recomputeCellPositions(indexType);
            recomputeAndDraw(indexType);
        }
        else {
            addDimension( myNeedsRecomputeCellsFlag, indexType );
            addDimension( myNeedsRecomputeLastPageFlag, indexType );
            myLastResizeLinePosition = -1;
            layout();
        }
        break;
    case Qics::ColumnIndex:
        visual_coord = m_info.visualColumnIndex( position );
        // do redraw only if changes was in visible area
        if ((visual_coord <= m_rightColumn + 1) ) {
            recomputeCellPositions(indexType);
            recomputeAndDraw(indexType);
        }
        else {
            addDimension( myNeedsRecomputeCellsFlag, indexType );
            addDimension( myNeedsRecomputeLastPageFlag, indexType );
            myLastResizeLinePosition = -1;
            layout();
        }
        break;
    case Qics::RowAndColumnIndex :
        recomputeAndDraw(indexType);
        break;
    case Qics::NoIndex:
        break;
    }
}

void QicsScreenGrid::recomputeCellPositions(Qics::QicsIndexType indexType)
{
    computeCellPositions(indexType);
}

void QicsScreenGrid::recomputeAndDraw(Qics::QicsIndexType indexType)
{
    updateGeometry();

    addDimension( myNeedsRecomputeCellsFlag, indexType );
    addDimension( myNeedsRecomputeLastPageFlag, indexType );

    if (gridInfo().gridRepaintBehavior() != Qics::RepaintOn) {
        myNeedsRepaintFlag = true;
        myLastResizeLinePosition = -1;

        if (gridInfo().gridRepaintBehavior() == Qics::RepaintCalcOnly)
            layout();
        return;
    }

    QicsRegion vp = currentViewport();

    if ((leftColumn()<0) && (vp.isValid()))
        m_leftColumn = gridInfo().firstNonHiddenColumn(0, vp.endColumn());

    if ((topRow()<0) && (vp.isValid()))
        m_topRow = gridInfo().firstNonHiddenRow(0, vp.endRow());

    redraw(vp);
    // invalidate this
    myLastResizeLinePosition = -1;
}

void QicsScreenGrid::recomputeAndDraw(Qics::QicsIndexType indexType, int startIndex, int endIndex)
{
    QicsRegion vp = realViewport();

    switch(indexType)
    {
    case Qics::RowIndex:
        startIndex = gridInfo().visualRowIndex(gridInfo().firstNonHiddenModelRow(startIndex, vp.endRow()));
        endIndex = gridInfo().visualRowIndex(gridInfo().firstNonHiddenModelRow(endIndex, vp.endRow()));

        if ((startIndex >= m_topRow && startIndex <= m_bottomRow) ||
            (endIndex >= m_topRow && endIndex <= m_bottomRow) ||
            (endIndex > m_bottomRow)) {
            recomputeAndDraw(indexType);
        }
        else if (isMainGrid()) {
            // scroller need it
            computeLastPage(indexType);
        }
        break;
    case Qics::ColumnIndex:
        startIndex = gridInfo().visualColumnIndex(gridInfo().firstNonHiddenModelColumn(startIndex, vp.endColumn()));
        endIndex = gridInfo().visualColumnIndex(gridInfo().firstNonHiddenModelColumn(endIndex, vp.endColumn()));

        if ((startIndex >= m_leftColumn && startIndex <= m_rightColumn) ||
            (endIndex >= m_leftColumn && endIndex <= m_rightColumn) ||
            (endIndex > m_rightColumn)) {
            recomputeAndDraw(indexType);
        }
        else if (isMainGrid()) {
            // scroller need it
            computeLastPage(indexType);
        }
        break;
    case Qics::RowAndColumnIndex:
        recomputeAndDraw(indexType);
        break;
    default:
        break;
    }
}

void QicsScreenGrid::redraw()
{
    m_repaintAll = true;
    update();
}

void QicsScreenGrid::redraw(const QicsRegion &region)
{
    if(region.isValid()) {
        if (m_paintRegion.isValid())
            m_paintRegion |= region;
        else
            m_paintRegion = region;


        m_repaintAll = false;
    }
    else
       m_repaintAll = true;

    update();
}

void QicsScreenGrid::redraw(const QicsSpan &span)
{
    //recomputeAndDraw();

    QicsRegion reg(span.row(), span.column(),
        span.row() + span.height() - 1, span.column() + span.width() - 1);

    redraw(reg);
}

void QicsScreenGrid::paintEvent(QPaintEvent* ev)
{
    m_gridInPaintEvent = true;
    QPainter widgetPainter(&m_imageBuffer);
    const QRect r(rect());

        if (!m_repaintAll && !m_initialRepaint) {
        const QicsRegion &svp = viewport();
        m_repaintAll = (m_paintRegion.endRow() > svp.endRow()) || (m_paintRegion.endColumn() > svp.endColumn());
    }

    if (m_repaintAll || m_initialRepaint) {
        QPen pen(widgetPainter.pen());
        pen.setJoinStyle(Qt::MiterJoin);
        pen.setWidth(m_mainGrid->frameLineWidth()*2);
        pen.setColor(m_mainGrid->windowColor());
        widgetPainter.setPen(pen);
        widgetPainter.drawRect(contentsRect());
        paintRegion(r, &widgetPainter);
    }
        else
        paintRegion(m_paintRegion, &widgetPainter, true);
    widgetPainter.end();

    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(r.topLeft(), m_imageBuffer, r);
    painter.end();

    m_paintRegion = QicsRegion();
    m_repaintAll = false;
    m_initialRepaint = false;
    m_gridInPaintEvent = false;

    QFrame::paintEvent(ev);
}

QRect QicsScreenGrid::paintRegion(const QRect &rect, QPainter *painter)
{
    QRect painted_rect;

    if (gridInfo().gridRepaintBehavior() != Qics::RepaintOn) {
        myNeedsRepaintFlag = true;
        if(gridInfo().gridRepaintBehavior() == Qics::RepaintCalcOnly)
            layout();
        return painted_rect;
    }

    // this only computes cell positions if necessary
    layout();

    // Sanity check the row position list
    if (myRowPositions.size() < (m_bottomRow - m_topRow))
        computeCellPositions(Qics::RowIndex);

    if (myColumnPositions.size() < (m_rightColumn - m_leftColumn))
        computeCellPositions(Qics::ColumnIndex);

    // Don't use cellAt() here -- it will account for spanned cells which
    // is not what we want here...
    QicsICell start_cell, end_cell;

    if (isRightToLeft()) {
        start_cell= QicsICell(rowAt(rect.top(), true), columnAt(rect.right(), true));
        end_cell= QicsICell(rowAt(rect.bottom(), true), columnAt(rect.left(), true));
    }
    else {
        start_cell= QicsICell(rowAt(rect.top(), true), columnAt(rect.left(), true));
        end_cell= QicsICell(rowAt(rect.bottom(), true), columnAt(rect.right(), true));
    }

    painter->fillRect(m_imageBuffer.rect(), m_mainGrid->windowColor());

    if ((start_cell.row() >= 0) && (start_cell.column() >= 0) &&
        (end_cell.row() >= 0) && (end_cell.column() >= 0)) {
        painted_rect = paintRegion(QicsRegion(start_cell, end_cell), painter, false);
    }

    return painted_rect;
}

QRect QicsScreenGrid::paintRegion(const QicsRegion &reg, QPainter *painter, bool compute_cell_possitions)
{
    if (!reg.isValid()) return QRect();

    if (gridInfo().gridRepaintBehavior() != Qics::RepaintOn) {
        myNeedsRepaintFlag = true;
        return QRect();
    }

    // this only computes cell positions if necessary
    if (compute_cell_possitions)
        layout();

    QicsRegion region(reg);

    // if there are overflows, expand region by row width
    if (m_mainGrid->cellOverflowBehavior() == Qics::Overflow) {
        region.setStartColumn(leftColumn());
        region.setEndColumn(rightColumn());
    }

    // Here span drawing is WRONG! so disable this temporary
    // #### TODO: correct handling of spans and overflows!
    //QicsRegion span_region;
    //bool is_span =(styleManager(). spanManager()) ->
    //    isSpanner( myInfo, region. startRow(), region. startColumn(), span_region );
    bool is_span=false;

    QicsRegion dr(
        qMax(region.startRow(), m_topRow),
        qMax(region.startColumn(), m_leftColumn),
        (is_span) ? m_bottomRow : qMin(region.endRow(), m_bottomRow),
        (is_span) ? m_rightColumn : qMin(region.endColumn(), m_rightColumn));

    if (dr.startRow() > dr.endRow() || dr.startColumn() > dr.endColumn())
        return QRect();

    // draw grid lines if there are any owerfows
    // #### TODO: drawing borders with spans in the same region
    drawGridLines(dr, painter);

    // draw more because to make sure all cells draw their cell borders
    //QicsRegion dr2(
    //    qMax(region.startRow()-1, myTopRow),
    //    qMax(region.startColumn()-1, myLeftColumn),
    //    ( is_span ) ? myBottomRow : qMin(region.endRow()+1, myBottomRow),
    //    ( is_span ) ? myRightColumn : qMin(region.endColumn()+1, myRightColumn) );

    const int firstNonHiddenRow = qMax(gridInfo().lastNonHiddenRow(m_topRow, region.startRow() - 1), m_topRow);
    const int firstNonHiddenColumn = qMax(gridInfo().lastNonHiddenColumn(m_leftColumn, region.startColumn() - 1), m_leftColumn);
    const int lastNonHiddenRow = qMin(gridInfo().firstNonHiddenRow(region.endRow() + 1, m_bottomRow), m_bottomRow);
    const int lastNonHiddenColumn = qMin(gridInfo().firstNonHiddenColumn(region.endColumn() + 1, m_rightColumn), m_rightColumn);

    //QicsRegion dr2(firstNonHiddenRow, firstNonHiddenColumn,
   //     (is_span) ? myBottomRow : lastNonHiddenRow, (is_span) ? myRightColumn : lastNonHiddenColumn);

    QicsRegion dr2(
        firstNonHiddenRow,
        firstNonHiddenColumn,
        (is_span || lastNonHiddenRow == -1) ? m_bottomRow : lastNonHiddenRow,
        (is_span || lastNonHiddenColumn == -1) ? m_rightColumn : lastNonHiddenColumn);

    //check end bounds if we set correct region
    //if (dr2.startRow() < myTopRow)
    //    dr2.setStartRow(myTopRow);
    //if (dr2.startColumn() < myLeftColumn)
    //    dr2.setStartColumn(myLeftColumn);

    //if (dr.startRow()> dr2.endRow())
    //    dr2.setEndRow(dr.startRow());
    //if (dr.startColumn()> dr2.endColumn())
    //    dr2.setEndColumn(dr.startColumn());

    const bool old_repaint = m_repaintAll;
    QRect painted_rect = drawRegion(dr2, painter, QicsGrid::WholeCell);
    // draw only cells borders now
    //drawRegion(dr2, painter, true);

    //draw top and left border widths
    painted_rect.adjust(0 - m_mainGrid->verticalGridLineWidth() - m_mainGrid->verticalShadeLineWidth(),
        0 - m_mainGrid->horizontalGridLineWidth() - m_mainGrid->horizontalShadeLineWidth(),
        0 + m_mainGrid->verticalGridLineWidth() + m_mainGrid->verticalShadeLineWidth(),
        0 + m_mainGrid->horizontalGridLineWidth() + m_mainGrid->horizontalShadeLineWidth());

    m_repaintAll = old_repaint;
    myNeedsRepaintFlag = false;

    clearUnusedRect(painter);

    placeEntryWidget();

    return painted_rect;
}

void QicsScreenGrid::clearUnusedRect(QPainter *painter)
{
    QRect buffer_rect(m_imageBuffer.rect());
    QRect r(rect());
    const bool leftToRight = isLeftToRight();

    QicsICell end_cell;
    QicsICell start_cell;

    if (leftToRight) {
        end_cell = QicsICell(rowAt(r.bottom(), true), columnAt(r.right(), true));
        start_cell = QicsICell(rowAt(r.top(), true), columnAt(r.left(), true));
    }
    else {
        end_cell = QicsICell(rowAt(r.bottom(), true), columnAt(r.left(), true));
        start_cell = QicsICell(rowAt(r.top(), true), columnAt(r.right(), true));
    }

    QRect end_cell_dims = cellDimensions(end_cell, true);
    QRect start_cell_dims = cellDimensions(start_cell, true);
    const int vlw = m_mainGrid->verticalGridLineWidth() + m_mainGrid->verticalShadeLineWidth();
    const int hlw = m_mainGrid->horizontalGridLineWidth() + m_mainGrid->horizontalShadeLineWidth();

    QRect r1, r2, r3, r4;
                                                                                                 // |---|-----------|---|
    if (leftToRight) {                                                                           // |   |     4     |   |
        r1.setCoords(r.left(), end_cell_dims.bottom() + hlw + 1,                                 // |   |-----------|   |
            end_cell_dims.right() + vlw + 1, buffer_rect.bottom());                              // | 3 | TableGrid | 2 |
                                                                                                 // |   |-----------|   |
        r2.setCoords(end_cell_dims.right() + vlw + 1, buffer_rect.top(),                         // |   |     1     |   |
            buffer_rect.right(), buffer_rect.bottom());                                          // |---|-----------|---|

        r3.setCoords(buffer_rect.left(), buffer_rect.top(),
            start_cell_dims.left() - vlw - 1, buffer_rect.bottom());

        r4.setCoords(buffer_rect.left(), buffer_rect.top(),
            end_cell_dims.right() + vlw + 1, start_cell_dims.top() - hlw - 1);
    }                                                                                             // |---|-----------|---|
    else {                                                                                        // |   |     4     |   |
        r1.setCoords(buffer_rect.left(), buffer_rect.top() + hlw + 1,                             // |   |-----------|   |
            end_cell_dims.left() - vlw - 1, buffer_rect.bottom() + 1);                            // | 1 | TableGrid | 3 |
                                                                                                  // |   |-----------|   |
        r2.setCoords(end_cell_dims.left() - vlw - 1,                                              // |   |     2     |   |
            end_cell_dims.bottom() + hlw + 1, buffer_rect.right(), buffer_rect.bottom() + 1);     // |---|-----------|---|

        r3.setCoords(start_cell_dims.right() + vlw + 1, buffer_rect.top(),
            buffer_rect.right(), buffer_rect.bottom() + 1);

        r4.setCoords(buffer_rect.left(), buffer_rect.top(),
            buffer_rect.right(), start_cell_dims.top() - hlw - 1);
    }

    const QColor &color = m_mainGrid->windowColor();

    if (r1.isValid())
        painter->fillRect(r1, color);

    if (r2.isValid())
        painter->fillRect(r2, color);

    if (r3.isValid())
        painter->fillRect(r3, color);

    if (r4.isValid())
        painter->fillRect(r4, color);
}

QRect QicsScreenGrid::drawCell(int row, int col, int x, int y,
                         bool look_for_overflower, QPainter *painter, QicsGrid::DrawCellMode mode)
{
    // don't overflow this cell if it is the current cell

    QicsICell cur_cell = currentCell();

    bool overflow = (look_for_overflower && ((!cur_cell.isValid()) ||
        (row != cur_cell.row()) ||
        (col !=  cur_cell.column())));
    return QicsGrid::drawCell(row, col, x, y, overflow, painter, mode);
}

bool QicsScreenGrid::prepareToDraw(int, int, const QRect &rect, QPainter *)
{
    bool out_of_grid_bounds = ((rect.x() + rect.width()) > width() ||
        (rect.y() + rect.height()) > height());

    return (m_mainGrid->drawPartialCells() || !out_of_grid_bounds);
}

/////////////////////////////////////////////////////////////////
/////////////        Event Handlers              ////////////////
/////////////////////////////////////////////////////////////////

void QicsScreenGrid::resizeEvent( QResizeEvent *re )
{
    // compute horizontal and vertical difference in pixels
    const QSize &new_size = re->size();
    const QSize &old_size = re->oldSize();

    int dw = new_size.width() - old_size.width();
    int dh = new_size.height() - old_size.height();
    Qics::QicsIndexType dirrection = Qics::RowIndex;

    if (0!=dw) dirrection = Qics::ColumnIndex;
    else if (0!=dh) dirrection = Qics::RowIndex;
    if ((0!=dw) && (0!=dh)) dirrection = Qics::RowAndColumnIndex;

    // This will be followed by a paint event, so we just flag that
    // we need to recompute before we draw.
    if (!m_gridInPaintEvent &&
        ((re->size().height() > m_imageBuffer.size().height())    // allocate only if needs bigger bufer
        || (re->size().width() > m_imageBuffer.size().width())
        ||(re->size().height() - m_imageBuffer.size().height() < -500)  // reallocate if buffer is to big
        || (re->size().width() - m_imageBuffer.size().width()) < -500)
        && (!re->size().isNull()))
    {
#ifdef notdef
        if (qobject_cast<QicsTableGrid*>(this))
            qDebug( "Resize %d %d", re->size().height()+100, re->size().width()+100  );
#endif
        // imageBuffer create 100x100 bigger to increase perfomance
        // by removing unnecessary image reallocating
        m_imageBuffer = QPixmap(re->size() + QSize(100, 100));
        m_imageBuffer.fill(palette().window().color());
        m_repaintAll = true;
        m_initialRepaint = true;

        addDimension( myNeedsRecomputeCellsFlag, dirrection );
        addDimension( myNeedsRecomputeLastPageFlag, dirrection );
        layout();

    }
    else {
        addDimension( myNeedsRecomputeCellsFlag, dirrection );
        addDimension( myNeedsRecomputeLastPageFlag, dirrection );
        layout();

        // #### TODO: here we can provide optimization by moving pixmap to right
        m_repaintAll = !isLeftToRight();
    }

    if (m_isMainGrid) {
        Qics::QicsIndexType redraw = Qics::NoIndex;

        if ((dw != 0) && (dimensionManager().stretchColumns(currentViewport().startColumn(),
            currentViewport().endColumn(),
            new_size.width(), true)>0)) {
            addDimension( redraw, Qics::ColumnIndex );
        }

        if ((dh != 0) && (dimensionManager().stretchRows(currentViewport().startRow(),
            currentViewport().endRow(),
            new_size.height(), true) > 0)) {
            addDimension( redraw, Qics::RowIndex );
        }
        gridInfo().redrawAllGrids(redraw);
    }

#ifdef Q_OS_WIN32
    if(!isVisible())
        redraw();
#endif

    QFrame::resizeEvent(re);
}

void QicsScreenGrid::mousePressEvent( QMouseEvent *m )
{
    // make sure that this function will rich the end line
    QicsICell cell = cellAt(m->x(), m->y(), false);

    if (cell.isValid()) {
        myPressedCell = cell;

        emitMouseSignals(cell,m);

        const int row = cell.row();
        const int column = cell.column();
        QicsCellDisplay *cd = cellDisplay(row, column);

        m_cell->setRowIndex(row);
        m_cell->setColumnIndex(column);

        if ((!m_cell->readOnly()) && m_cell->enabled() &&
            cd && cd->handleMouseEvent(this, row, column, m)) {
            ; //do nothing

            return;
        }
        //else
        //    handleMousePressEvent(cell, m);
    }

    handleMousePressEvent(cell, m);
}

void QicsScreenGrid::mouseReleaseEvent(QMouseEvent *m)
{
    QicsICell cell = cellAt(m->x(), m->y(), gridInfo().gridType() != Qics::TableGrid /*false*/);

    stopScrollingTimer();

    if (cell.isValid()) {
        const int row = cell.row();
        const int col = cell.column();
        bool dragging=true;

        if (cell == myPressedCell) {
            dragging = false;
            emitMouseSignals(cell, m);
            if (m->button() == Qt::LeftButton && m->modifiers() == Qt::ShiftModifier)
                gridInfo().reportSelection(Qics::SelectionEnd, 0, 0, row, col);
        }

        myPressedCell = QicsICell();

        m_cell->setRowIndex(row);
        m_cell->setColumnIndex(col);

        QicsCellDisplay *cd = cellDisplay(row, col);

        if ((!m_cell->readOnly()) && m_cell->enabled() &&
            cd && cd->handleMouseEvent(this, row, col, m) && !dragging) {
            ; //do nothing

            return;
        }
        //else
        //    handleMouseReleaseEvent(cell, m);
    }

    handleMouseReleaseEvent(cell, m);
}

void QicsScreenGrid::mouseDoubleClickEvent(QMouseEvent *m)
{
    QicsICell cell = cellAt(m->x(), m->y(), false);

    if (cell.isValid()) {

        emitMouseSignals(cell,m);

        const int row = cell.row();
        const int column = cell.column();
        QicsCellDisplay *cd = cellDisplay(row, column);

        m_cell->setRowIndex(row);
        m_cell->setColumnIndex(column);

        if ((!m_cell->readOnly()) && m_cell->enabled() &&
            cd && cd->handleMouseEvent(this, row, column, m)) {
            ; //do nothing

            return;
        }

    }

    handleMouseDoubleClickEvent(cell, m);
}

void QicsScreenGrid::emitMouseSignals(const QicsICell& cell, QMouseEvent *m)
{
    switch (m->type())
    {
    case QEvent::MouseButtonPress:
        emit pressed(cell.row(), cell.column(), m->button(), m->pos());
        break;
    case QEvent::MouseButtonRelease:
        emit clicked(cell.row(), cell.column(), m->button(), m->pos());
        break;
    case QEvent::MouseButtonDblClick:
        emit doubleClicked(cell.row(), cell.column(), m->button(), m->pos());
        break;
    default:
        break;
    }
}

void QicsScreenGrid::mouseMoveEvent( QMouseEvent *m )
{
    QicsICell cell = cellAt(m->x(), m->y(), true);
    handleMouseMoveEvent(cell, m);
    m->ignore();
}

void QicsScreenGrid::keyPressEvent(QKeyEvent *ke)
{
    emit wideKeyPressed(ke);

    if (ke->isAccepted())
        return;

    int key = ke->key();

    if
        (
        (key == Qt::Key_Alt) ||
        (key == Qt::Key_Control) ||
        (key == Qt::Key_Shift)
        )
    {
        return;
    }

    // make sure that this function will rich the end line
    ke->accept();

    QicsICell cur_cell = currentCell();

    if ( (key == Qt::Key_Tab || key == Qt::Key_Backtab)
        && (ke->modifiers() == Qt::ControlModifier) ) {
        if (key == Qt::Key_Tab)
            focusNextPrevChild(true);
        if (key == Qt::Key_Backtab)
            focusNextPrevChild(false);
    }
    else {
        if (!cur_cell.isValid()) {
            if (key == Qt::Key_Tab ||
                key == Qt::Key_Home || key == Qt::Key_End ||
                key == Qt::Key_PageUp || key == Qt::Key_PageDown) {

                traverseToCell(topRow(), leftColumn());
                if (key != Qt::Key_Tab)
                    if (!handleTraversalKeys(ke))
                        reset();
            }
        }
        else {
            // These keys by themselves aren't worth looking at.  If we send
            // them to the cell display and ClickToEdit is false, these keys
            // will cause the cell to be edited, which we don't want.

            QicsCellDisplay *cd = cellDisplay(cur_cell.row(),cur_cell.column());
            bool isCurrentCellEditWhenCurrent = cd->editWhenCurrent();

            QicsSelectionList *sl = selectionManager().selectionList(true);
            bool isUneditCellSelected = false;
            if (sl)
                isUneditCellSelected = sl->isCellSelected(myUneditCell.row(),myUneditCell.column());

            if (!currentCell().isValid() && myUneditCell.isValid()) {
                if (!isUneditCellSelected && sl && sl->size()) {
                    QicsSelection sel = sl->at(0);
                    setCurrentCell(QicsICell(sel.anchorRow(),sel.anchorColumn()));
                }
                else
                    setCurrentCell(myUneditCell);

                if (isCurrentCellEditWhenCurrent)
                    cur_cell = currentCell();
            }

            if (cur_cell.isValid()) {
                if (!handleTraversalKeys(ke)) {
                    cur_cell = currentCell();

                    if (!myEditingCurrentCell) {
                        if (!m_mainGrid->clickToEdit())
                            editCell(cur_cell.row(), cur_cell.column());
                    }

                    if (!cd->handleKeyEvent(this, cur_cell.row(), cur_cell.column(), ke)) {
                        handleKeyPressEvent(cur_cell, ke);
                    }
                }
            }
        }
    }
}

void QicsScreenGrid::keyReleaseEvent (QKeyEvent *ke)
{
    QicsICell cur_cell = currentCell();

    if (!cur_cell.isValid())
        return;

    handleKeyReleaseEvent(cur_cell, ke);
}

void QicsScreenGrid::dropEvent(QDropEvent *event)
{
    QicsICell cell = cellAt(event->pos().x(), event->pos().y(), false);
    m_cell->setRowIndex( cell.row());
    m_cell->setColumnIndex( cell.column());

    if (m_cell->canAcceptDrops())
        dropAt(event, cell);

    if(!currentCell().isValid() && m_dragCell.isValid()) {
        setCurrentCell(m_dragCell);
        m_traversalColumn = m_dragCell.column();
        m_traversalRow = m_dragCell.row();
    }
}

void QicsScreenGrid::dragEnterEvent(QDragEnterEvent *event)
{
    if((event->keyboardModifiers() & Qt::ControlModifier) || !m_isMainGrid) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->accept();
}

void QicsScreenGrid::handleMousePressEvent(const QicsICell &, QMouseEvent *)
{
}

void QicsScreenGrid::handleMouseReleaseEvent(const QicsICell &, QMouseEvent *)
{
}

void QicsScreenGrid::handleMouseDoubleClickEvent(const QicsICell &, QMouseEvent *)
{
}

void QicsScreenGrid::handleMouseMoveEvent(const QicsICell &, QMouseEvent *m)
{
    QicsICell cell = cellAt(m->x(), m->y(), false);
    QCursor c;

    if (cell.isValid()) {
        m_cell->setRowIndex(cell.row());
        m_cell->setColumnIndex(cell.column());

        c = m_cell->cursor();
    }
    else {
        c = m_mainGrid->cursor();
    }
    setCursor(c);
}

void QicsScreenGrid::handleKeyPressEvent(const QicsICell &, QKeyEvent *)
{
}

void QicsScreenGrid::handleKeyReleaseEvent(const QicsICell &, QKeyEvent *)
{
}

/////////////////////////////////////////////////////////////////
/////////////////    Actions      ///////////////////////////////
/////////////////////////////////////////////////////////////////

void QicsScreenGrid::scrollLeft(int num)
{
    int cols_to_scroll = qMin((leftColumn() - currentViewport().startColumn()),
        num);

    if (cols_to_scroll > 0)
        emit scrollRequest(Qics::ScrollLeft, cols_to_scroll);
}

void QicsScreenGrid::scrollRight(int num)
{
    int cols_to_scroll = qMin((currentViewport().endColumn() - rightColumn()),
        num);

    if (cols_to_scroll > 0)
        emit scrollRequest(Qics::ScrollRight, cols_to_scroll);
}

void QicsScreenGrid::scrollUp(int num)
{
    int rows_to_scroll = qMin((topRow() - currentViewport().startRow()),
        num);

    if (rows_to_scroll > 0)
        emit scrollRequest(Qics::ScrollUp, rows_to_scroll);
}

void QicsScreenGrid::scrollDown(int num)
{
    int rows_to_scroll = qMin(currentViewport().endRow() - bottomRow() + 1,
        num);

    if (rows_to_scroll > 0)
        emit scrollRequest(Qics::ScrollDown, rows_to_scroll);
}

bool QicsScreenGrid::traverseToBeginningOfTable()
{
    QicsRegion real_vp = currentViewport();

    bool res = traverseToCell(real_vp.startRow(),
        real_vp.startColumn(),
        m_selectOnTraverse);

    scrollLeft(real_vp.startColumn());
    scrollUp(real_vp.startRow());

    if (!res) {
        if (traverseToBeginningOfColumn()) {
            traverseToBeginningOfRow();
            return true;
        }

        if (traverseToBeginningOfRow()) {
            traverseToBeginningOfColumn();
            return true;
        }
    }

    return res;
}

bool QicsScreenGrid::traverseToEndOfTable()
{
    QicsRegion real_vp = currentViewport();

    bool res = traverseToCell(real_vp.endRow(),
        real_vp.endColumn(),
        m_selectOnTraverse);

    scrollRight(real_vp.endColumn() + 1);
    scrollDown(real_vp.endRow() + 1);

    if (!res) {
        if (traverseToEndOfColumn()) {
            traverseToEndOfRow();
            return true;
        }

        if (traverseToEndOfRow()) {
            traverseToEndOfColumn();
            return true;
        }
    }

    return res;
}

bool QicsScreenGrid::doTraverseHorizontal(bool leftwards)
{
    QicsICell cur_cell = currentCell();

    if (!cur_cell.isValid())
        return false;

    int dirrection = isRightToLeft() ? -1 : 1;

    if (!leftwards) dirrection = -dirrection;

    QicsRegion real_vp = currentViewport();

    QicsRegion reg;
    int row, col;
    int start_row;
    bool spanned;

    row = start_row = (m_traversalRow >= 0) ? m_traversalRow : cur_cell.row();

    if ((styleManager().spanManager())->insideSpan(m_info, cur_cell.row(), cur_cell.column(), reg)) {
        col = (leftwards ? reg.startColumn() : reg.endColumn()) - dirrection;
    }
    else {
        col = cur_cell.column() - dirrection;
        reg = QicsRegion(row, col, row, col);
    }


    if (leftwards) {
        if (cur_cell.column() <= real_vp.startColumn() || col < real_vp.startColumn())
            return false;
    }
    else
        if (cur_cell.column() >= real_vp.endColumn() || col > real_vp.endColumn())
            return false;

    if (m_info.mappedDM()->isRowHidden(row))
        row = m_info.firstNonHiddenRow(reg.startRow(), real_vp.endRow());

    do {
        spanned = styleManager().spanManager()->insideSpan(m_info, row, col, reg);
        if (spanned) {
            row = reg.startRow();
            col = reg.startColumn();
        }

        m_cell->setRowIndex(row);
        m_cell->setColumnIndex(col);

        if (!m_cell->enabled() || m_cell->isHidden()) {
            row = start_row;
            if (spanned)
                if (dirrection == 1) col -= reg.width();
                else row += reg.width();
            else
                col -= dirrection;
        } else
            break;

    } while ((!leftwards && col <= real_vp.endColumn()) || (leftwards && col >= real_vp.startColumn()));

    if (traverseToCell(row, col, m_selectOnTraverse)) {
        QicsICell cur = currentCell();
        //m_traversalRow = start_row;
        m_traversalRow = cur.row();
        m_traversalColumn = -1;
        return true;
    }

    return false;
}

bool QicsScreenGrid::traverseLeft()
{
    return doTraverseHorizontal(true);
}

bool QicsScreenGrid::traverseRight()
{
    return doTraverseHorizontal(false);
}

bool QicsScreenGrid::doTraverseVertical(bool upwards)
{
    QicsICell cur_cell = currentCell();

    if (!cur_cell.isValid())
        return false;

    int dirrection = upwards ? 1 : -1;

    QicsRegion real_vp = currentViewport();

    QicsRegion reg;
    int row, col;
    int start_column;
    bool spanned;

    col = start_column = (m_traversalColumn >= 0) ? m_traversalColumn : cur_cell.column();

    if ((styleManager().spanManager())->insideSpan(m_info, cur_cell.row(), cur_cell.column(), reg)) {
        row = (upwards ? reg.startRow() : reg.endRow()) - dirrection;
    }
    else {
        row = cur_cell.row() - dirrection;
        reg = QicsRegion(row, col, row, col);
    }

    if (upwards) {
        if (cur_cell.row() <= real_vp.startRow() || row < real_vp.startRow())
            return false;
    } else
        if (cur_cell.row() >= real_vp.endRow() || row > real_vp.endRow())
            return false;

    if (m_info.mappedDM()->isColumnHidden(col))
        col = m_info.firstNonHiddenColumn(reg.startColumn(), real_vp.endColumn());

    do {
        spanned = styleManager().spanManager()->insideSpan(m_info, row, col, reg);
        if (spanned) {
            row = reg.startRow();
            col = reg.startColumn();
        }

        m_cell->setRowIndex(row);
        m_cell->setColumnIndex(col);

        if (!m_cell->enabled() || m_cell->isHidden()) {
            col = start_column;
            if (spanned)
                if (dirrection == 1) row -= reg.height();
                else row += reg.height();
            else
                row -= dirrection;
        } else
            break;

    } while ((!upwards && row <= real_vp.endRow()) || (upwards && row >= real_vp.startRow()));

    if (traverseToCell(row, col, m_selectOnTraverse)) {
        QicsICell cur = currentCell();
        m_traversalColumn = cur.column();
        //m_traversalColumn = start_column;
        m_traversalRow = -1;
        return true;
    }

    return false;
}

bool QicsScreenGrid::traverseUp()
{
    return doTraverseVertical(true);
}

bool QicsScreenGrid::traverseDown()
{
    return doTraverseVertical(false);
}

bool QicsScreenGrid::doTraverseCellVertical(int row, bool upwards)
{
    QicsRegion real_vp = currentViewport();
    QicsRegion reg;
    int start_column;
    bool spanned;

    int col = start_column = (m_traversalColumn >= 0) ? m_traversalColumn : currentCell().column();

    do {
        spanned = styleManager().spanManager()->insideSpan(m_info, row, col, reg);
        if (spanned) {
            row = reg.startRow();
            col = reg.startColumn();
        }

        m_cell->setRowIndex(row);
        m_cell->setColumnIndex(col);

        if (!m_cell->enabled() || m_cell->isHidden()) {
            col = start_column;
            if (upwards)
                row -= spanned ? reg.height() : 1;
            else
                row += spanned ? reg.height() : 1;
        } else
            break;

    } while ((upwards && row > real_vp.startRow()) || (!upwards && row < real_vp.endRow()));


    if (traverseToCell(row, col, m_selectOnTraverse)) {
        QicsICell cur = currentCell();
        m_traversalRow = -1;
        m_traversalColumn = cur.column();
        //m_traversalColumn = start_column;
        return true;
    }

    return false;
}

bool QicsScreenGrid::traversePageUp()
{
    QicsICell cur_cell = currentCell();

    if (!cur_cell.isValid())
        return false;

    QicsRegion real_vp = currentViewport();
    int row = cur_cell.row() - (visibleRows()-1);

    if (row < real_vp.startRow())
        row = real_vp.startRow();

    // make scroll up
    if (topRow() != real_vp.startRow())
        scrollUp(visibleRows()-1);

    if (!doTraverseCellVertical(row, true))
        return doTraverseCellVertical(row, false);

    return true;
}

bool QicsScreenGrid::traversePageDown()
{
    QicsICell cur_cell = currentCell();

    if (!cur_cell.isValid())
        return false;

    QicsRegion real_vp = currentViewport();
    int row = cur_cell.row() + (visibleRows()-1);

    if (row > real_vp.endRow())
        row = real_vp.endRow();

    // make scroll down
    if (bottomRow() != real_vp.endRow())
        scrollDown(visibleRows()-1);

    if (!doTraverseCellVertical(row, false))
        return doTraverseCellVertical(row, true);

    return true;
}

bool QicsScreenGrid::doTraverseCellHorizontal(int col, bool leftwards)
{
    QicsRegion real_vp = currentViewport();
    QicsRegion reg;
    int start_row;
    bool spanned;

    int row = start_row = (m_traversalRow >= 0) ? m_traversalRow : currentCell().row();

    do {
        spanned = styleManager().spanManager()->insideSpan(m_info, row, col, reg);
        if (spanned) {
            row = reg.startRow();
            col = reg.startColumn();
        }

        m_cell->setRowIndex(row);
        m_cell->setColumnIndex(col);

        if (!m_cell->enabled() || m_cell->isHidden()) {
            row = start_row;
            if (leftwards)
                col -= spanned ? reg.width() : 1;
            else
                col += spanned ? reg.width() : 1;
        } else
            break;

    } while ((leftwards && col > real_vp.startColumn()) || (!leftwards && col < real_vp.endColumn()));


    if (traverseToCell(row, col, m_selectOnTraverse)) {
        QicsICell cur = currentCell();
        m_traversalRow = cur.row();
        //m_traversalRow = start_row;
        m_traversalColumn = -1;
        return true;
    }

    return false;
}

bool QicsScreenGrid::traverseToBeginningOfRow()
{
    if (!currentCell().isValid())
        return false;

    if (!doTraverseCellHorizontal(currentViewport().startColumn(), false))
        return doTraverseCellHorizontal(currentViewport().startColumn(), true);
    return true;
}

bool QicsScreenGrid::traverseToBeginningOfColumn()
{
    if (!currentCell().isValid())
        return false;

    if (!doTraverseCellVertical(currentViewport().startRow(), false))
        return doTraverseCellVertical(currentViewport().startRow(), true);
    return true;
}

bool QicsScreenGrid::traverseToEndOfRow()
{
    if (!currentCell().isValid())
        return false;

    if (!doTraverseCellHorizontal(currentViewport().endColumn(), true))
        return doTraverseCellHorizontal(currentViewport().endColumn(), false);
    return true;
}

bool QicsScreenGrid::traverseToEndOfColumn()
{
    if (!currentCell().isValid())
        return false;

    if (!doTraverseCellVertical(currentViewport().endRow(), true))
        return doTraverseCellVertical(currentViewport().endRow(), false);
    return true;
}

void QicsScreenGrid::dropAt(QDropEvent *event, const QicsICell & /*cell*/)
{
    event->ignore();
}

////////////////////////////////////////////////////////////////////////////
/////////////////////    Traversal/Editing Methods    //////////////////////
////////////////////////////////////////////////////////////////////////////

bool QicsScreenGrid::handleTraversalKeys(QKeyEvent *ke)
{
    QicsSelectionManager *sm = gridInfo().selectionManager();
    if (sm->selectCurrentCellPolicy() == Qics::Static/* && m_doingSelectionFlag*/) {
        if (sm->exclusiveSelection()) {
            QicsICell cell = sm->exclusiveSelection()->anchorCell();

            if (!(ke->modifiers() & Qt::ShiftModifier) && currentCell() != cell) {
                setCurrentCell(cell);
                m_traversalColumn = m_traversalRow = -1;
            }
        }
    }

    return gridInfo().keyboardManager()->handleTraversalKeys(ke, this);
}

bool QicsScreenGrid::traverseToCell(int row, int col, bool select_cell)
{
    if (!canTraverseToCell(row, col)) {
        if (myEditingCurrentCell && !myEditingCurrentCell->editWhenCurrent())
            uneditCurrentCell();

        return false;
    }

    m_traversalRow = -1;
    m_traversalColumn = -1;

    // check if it is a span
    QicsRegion reg;
    if (m_info.styleManager()->spanManager()->insideSpan(m_info, row, col, reg)) {
        row = reg.startRow();
        col = reg.startColumn();
    }

    QicsICell cur_cell = currentCell();
    QicsICell target_cell = QicsICell(row, col);

    if (cur_cell.isValid() && cur_cell == target_cell) {
        if (!isCellVisible(row, col))
            makeCellFullyVisible(row, col);
        // We're already there
        return true;
    }

    emit uneditRequest();

    if (select_cell)
        selectCell(row, col);

    setCurrentCell(target_cell);

    if (cellDisplay(row, col)->editWhenCurrent())
        editCurrentCell();

    makeCellFullyVisible(row, col);

    return true;
}

bool QicsScreenGrid::canTraverseToCell(int row, int col)
{
    // Sanity Check the row and column
    if (!isCellValid(row, col))
        return false;

    m_cell->setRowIndex(row);
    m_cell->setColumnIndex(col);

    // Can't traverse to a cell that's not enabled
    if (!m_cell->enabled())
        return false;

    return (true);
}

bool QicsScreenGrid::editCell(int row, int col)
{
    if (!m_editable)
        return false;

    m_cell->setRowIndex(row);
    m_cell->setColumnIndex(col);

    if (m_cell->readOnly())
        return false;

    if (!traverseToCell(row, col))
        return false;

    myEditingCurrentCell = cellDisplay(row, col);

    if (myEditingCurrentCell) {
        myEditingCurrentCell->startEdit(this, row, col, cellValue(row, col));
        myUneditCell = QicsICell(row, col);
        emit cellEditStarted(row, col);
        myEditingCell = QicsICell(row,col);
    }

    placeEntryWidget();

    return true;
}

bool QicsScreenGrid::editCurrentCell()
{
    QicsICell cur_cell = currentCell();

    // Sanity Check
    if (!cur_cell.isValid())
        return false;

    return editCell(cur_cell.row(), cur_cell.column());
}

void QicsScreenGrid::uneditCurrentCell()
{
    if (myEditingCurrentCell && currentCell().isValid()) {
        QicsICell cur_cell = currentCell();
        QicsCellDisplay* cd = myEditingCurrentCell;

        myEditingCurrentCell = 0;

        cd->endEdit(this, cur_cell.row(), cur_cell.column());

        emit cellEditEnded(cur_cell.row(), cur_cell.column());
        myEditingCell = QicsICell(-1,-1);

        // It's temporary hack to solve focus problems #bug 712
        if(cd->lastFocusReason() != Qt::MouseFocusReason)
            setFocus();
    }

    myEditingCurrentCell = 0;
}

void QicsScreenGrid::setEditable(bool b)
{
    m_editable = b;

    if (!m_editable)
        uneditCurrentCell();
}

void QicsScreenGrid::placeEntryWidget()
{
    QicsICell cur_cell = currentCell();

    if (!cur_cell.isValid())
        return;

    if (!myEditingCurrentCell)
        return;

    QicsCellDisplay *cd = cellDisplay(cur_cell.row(), cur_cell.column());
    //QicsRegion span;
    //QicsRegion visible_reg(m_topRow, m_leftColumn, m_bottomRow, m_rightColumn);
    //QicsSpanManager *spm = styleManager().spanManager();
    //spm->insideSpan(m_info, cur_cell.row(), cur_cell.column(), span);
    //QicsRegion intersect_reg = span.intersect( visible_reg );

    if (!isCellVisible(cur_cell.row(), cur_cell.column()) /*&& !intersect_reg.isEmpty()*/) {
        cd->endEdit(this, cur_cell.row(), cur_cell.column());
        return;
    }

    if (cd->isEditing()) {
        QRect rect = cellDimensions(cur_cell.row(), cur_cell.column(), true);
        myPlacingEntryWidgetFlag = true;
        cd->moveEdit(this, cur_cell.row(), cur_cell.column(), rect);
        myPlacingEntryWidgetFlag = false;
    }
}

////////////////////////////////////////////////////////////////////////////
//////////////////////    Drag and Drop Methods    /////////////////////////
////////////////////////////////////////////////////////////////////////////

void QicsScreenGrid::prepareDrag(const QicsICell &cell, Qt::DropAction)
{
    if (!m_mainGrid->dragEnabled())
        return;

    m_dragCell = cell;
}

void QicsScreenGrid::startDrag(Qt::DropAction mode)
{
    if (!m_dragCell.isValid() || !selectionManager().selectionList(true))
        return;
    if (!gridInfo().selectionManager()->selectionList(true)->isCellSelected(m_dragCell.row(),m_dragCell.column()))
        return;

    if (!m_mainGrid->dragEnabled())
        return;

    // Get the drag object
    if (m_dragCell.isValid()) {
        QMimeData *obj = dragObject(mode);
        QDrag* drag = new QDrag(this);
        drag->setMimeData(obj);
        // Set cursor
        drag->setDragCursor(QPixmap(),mode);
        // Do the drag
        bool remove = false;

        drag->start(mode);

        if (myDropAction == Qt::MoveAction)
            remove = true;
        // NOTE: remove is only true after a successful dragMove operation
        // NOTE: target is NULL if the drop is on another application

#ifdef notdef
        qDebug("Done with drag, targ = 0x%x, remove=%d\n", 0, remove);
#endif

        finishDrag(mode, remove, 0);

        m_dragCell = QicsICell();
    }
}

void QicsScreenGrid::finishDrag(Qt::DropAction, bool remove, QWidget *)
{

    // TODO: we may want to consider setting remove to
    // false if target is NULL.  That would effectively
    // prevent dragging data out into another application
    if (m_cell->canAcceptDrops()) {
        m_tableCommon->finishCut(remove);
    }
}

QMimeData *QicsScreenGrid::dragObject(Qt::DropAction)
{
    if(m_mainGrid->currentCellDraggingPolicy() == Qics::DragCurrent)
        return (m_tableCommon->cutCopyData(this, &m_dragCell));
    else
        return (m_tableCommon->cutCopyData(this, &m_dragCell, false));
}

bool QicsScreenGrid::canDropAt(QDragMoveEvent *, const QicsICell &) const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool QicsScreenGrid::isCellVisible(int row, int col) const
{
    m_cell->setRowIndex(row);
    m_cell->setColumnIndex(col);

    if (m_cell->isHidden())
        return false;

    return ((m_topRow <= row ) && (row <= m_bottomRow) &&
        (m_leftColumn <= col) && (col <= m_rightColumn));
}

void QicsScreenGrid::makeCellFullyVisible(int row, int col)
{
    QicsRegion reg;
    int radd = 0, cadd = 0;

    if (m_info.styleManager()->spanManager()->insideSpan(m_info, row, col, reg)) {
        row = m_info.firstNonHiddenRow(reg.startRow(), reg.endRow());
        col = m_info.firstNonHiddenColumn(reg.startColumn(), reg.endColumn());
        radd = reg.height()-1;
        cadd = reg.width()-1;
    }

    QicsRegion real_vp = currentViewport();

    // first check rows
    const int firstNonHiddenRow = gridInfo().firstNonHiddenRow(m_topRow, real_vp.endRow());

    if (row < firstNonHiddenRow) {
        // the easy case -- just put the cell at the top
        emit scrollRequest(Qics::ScrollToRow, row/*+radd*/);
    }
    else {
        int cur_row = firstNonHiddenRow;
        int h = 0;

        // First, sum up the heights of as many rows as can fit
        // in the grid.

        while (cur_row <= real_vp.endRow()) {
            m_row->setRowIndex(cur_row);

            if (!m_row->isHidden()) {
                int rh = mappedDM().rowHeight(cur_row) + m_mainGrid->horizontalGridLineWidth()
                    + m_mainGrid->horizontalShadeLineWidth();

                if ((h + rh) > contentsRect().height())
                    break;

                h += rh;
            }

            ++cur_row;
        }

        // Now that we've figured out how many rows can fit, we need
        // to figure out how far we need to scroll in order to put
        // the desired row at the bottom of the grid.

        int first_row = firstNonHiddenRow;
        int last_full_row = firstNonHiddenRow;

        if ((cur_row-1) >= firstNonHiddenRow)
            last_full_row = gridInfo().lastNonHiddenRow(real_vp.startRow(), cur_row - 1);

        while ((row > last_full_row) && (last_full_row < real_vp.endRow()) && first_row < real_vp.endRow()) {
            // take away the topmost row
            h -= mappedDM().rowHeight(first_row);
            first_row = gridInfo().firstNonHiddenRow(first_row+1,
                real_vp.endRow());

            // add as many rows below as will fit
            cur_row = last_full_row + 1;
            while (cur_row <= real_vp.endRow()) {
                m_row->setRowIndex(cur_row);

                if (!m_row->isHidden()) {
                    int rh = mappedDM().rowHeight(cur_row) + m_mainGrid->horizontalGridLineWidth()
                        + m_mainGrid->horizontalShadeLineWidth();

                    if ((h + rh) > contentsRect().height())
                        break;

                    h += rh;
                }

                ++cur_row;
            }

            last_full_row = gridInfo().lastNonHiddenRow(real_vp.startRow(), cur_row - 1);
        }

        if (first_row > firstNonHiddenRow) {
            if (row < first_row)
                emit scrollRequest(Qics::ScrollToRow, row);
            else
                emit scrollRequest(Qics::ScrollToRow, first_row+radd);
        }
    }

    // now check columns
    const int firstNonHiddenColumn = gridInfo().firstNonHiddenColumn(m_leftColumn, real_vp.endColumn());

    if (col < firstNonHiddenColumn) {
        // the easy case -- just put the cell at the left
        emit scrollRequest(Qics::ScrollToColumn, col/*+cadd*/);
    }
    else {
        int cur_col = firstNonHiddenColumn;
        int w = 0;

        // First, sum up the widths of as many columns as can fit
        // in the grid.

        while (cur_col <= real_vp.endColumn()) {
            m_column->setColumnIndex(cur_col);

            if (!m_column->isHidden()) {
                int cw = mappedDM().columnWidth(cur_col) + m_mainGrid->verticalGridLineWidth()
                    + m_mainGrid->verticalShadeLineWidth();

                if ((w + cw) > contentsRect().width() )
                    break;

                w += cw;
            }

            ++cur_col;
        }

        int first_col = firstNonHiddenColumn;
        int last_full_col = first_col;

        if ((cur_col-1) >= firstNonHiddenColumn)
            last_full_col = gridInfo().lastNonHiddenColumn(real_vp.startColumn(), cur_col - 1);

        while ((col > last_full_col) && (last_full_col < real_vp.endColumn()) && first_col < real_vp.endColumn()) {
            // take away the leftmost column
            w -= mappedDM().columnWidth(first_col);
            first_col = gridInfo().firstNonHiddenColumn(first_col+1, real_vp.endColumn());

            // add as many columns to the right as will fit
            cur_col = last_full_col + 1;
            while (cur_col <= real_vp.endColumn()) {
                m_column->setColumnIndex(cur_col);

                if (!m_column->isHidden()) {
                    int cw = mappedDM().columnWidth(cur_col) + m_mainGrid->verticalGridLineWidth()
                        + m_mainGrid->verticalShadeLineWidth();

                    if ((w + cw) > contentsRect().width())
                        break;

                    w += cw;
                }

                ++cur_col;
            }

            last_full_col = gridInfo().lastNonHiddenColumn(real_vp.startColumn(),
                cur_col - 1);
        }

        if (first_col > firstNonHiddenColumn) {
            if (col < first_col)
                emit scrollRequest(Qics::ScrollToColumn, col);
            else
                emit scrollRequest(Qics::ScrollToColumn, first_col+cadd);
        }
    }
}

QString QicsScreenGrid::tooltipText(const QicsICell &cell) const
{
    QString text;

    // First, check if we have an overflow tooltip

    if (m_mainGrid->cellOverflowBehavior() == Qics::ToolTip) {
        QicsCellDisplay *cd = cellDisplay(cell.row(), cell.column());
        QRect rect = cellDimensions(cell, true);
        const QicsDataItem *itm = cellValue(cell.row(), cell.column());

        text = cd->tooltipText(&m_info, cell.row(), cell.column(), itm, rect);
    }

    // If not, get the tooltip property for this cell

    if (text.isEmpty()) {
        m_cell->setRowIndex(cell.row());
        m_cell->setColumnIndex(cell.column());

        text = m_cell->toolTipText();
    }

    return (text);
}

void QicsScreenGrid::updateViewport()
{
    QicsRegion vp = QicsGrid::viewport();

    // Make sure we only show from the beginning of the viewport
    if (m_topRow < vp.startRow())
        m_topRow = vp.startRow();

    if (m_leftColumn < vp.startColumn())
        m_leftColumn = vp.startColumn();
}

void QicsScreenGrid::updateLineWidth()
{
    setLineWidth(m_mainGrid->frameLineWidth());

    emit frameStyleUpdated();
}

void QicsScreenGrid::updateFrameStyle()
{
    /*if (!(m_mainGrid->frameStyle() & QFrame::Shape_Mask)) { // NoFrame
        // The problem with NoFrame is that we really want a "blank" frame.
        // So, to get a blank frame, we make a box frame that's drawn in
        // the widget's background color, rendering it invisible.
        setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    }
    else*/

    setFrameStyle(m_mainGrid->frameStyle());

    //Background color
    QPalette pal = QFrame::palette();
    pal.setColor(QPalette::Base, m_mainGrid->backgroundColor());
    QFrame::setPalette(pal);

    emit frameStyleUpdated();
}

void QicsScreenGrid::orderRowsBy(const QVector<int> &columns, Qics::QicsSortOrder order, DataItemComparator func)
{
    QicsGrid::orderRowsBy(columns, order, func);
    recomputeAndDraw(Qics::RowIndex);
}

void QicsScreenGrid::orderColumnsBy(const QVector<int> &rows, Qics::QicsSortOrder order, DataItemComparator func)
{
    QicsGrid::orderColumnsBy(rows, order, func);
    recomputeAndDraw(Qics::ColumnIndex);
}

//////////////////////////////////////////////////////////////////////
////////////         Slots Methods                     ///////////////
//////////////////////////////////////////////////////////////////////

void QicsScreenGrid::handleCellPropertyChange(const QicsRegion &region,
                                         QicsCellStyle::QicsCellStyleProperty prop,
                                         bool visual_coords)
{
    bool recalc;
    bool repaint = true;

    switch (prop)
    {
    case QicsCellStyle::CellDisplayer:
        if (myEditingCurrentCell &&
            ((visual_coords &&
            region.contains(currentCell().x(),currentCell().y()))||
            (!visual_coords &&
            region.contains(m_info.modelColumnIndex(currentCell().x()), m_info.modelRowIndex(currentCell().y()))))) {
            editCurrentCell();
            uneditCurrentCell();
        }
        recalc = false;
        break;
    case QicsCellStyle::Font:
    case QicsCellStyle::Hidden:
        recalc = true;
        break;
    case QicsCellStyle::ToolTipText:
        repaint = false;
        // fall through
    case QicsCellStyle::Selected:
        repaint = false;
        recalc = false;
        break;
    default:
        recalc = false;
    }

    if (recalc)
        recomputeAndDraw(Qics::RowAndColumnIndex);
    else if (repaint && isVisible()) {
        const QicsRegion &r = visual_coords ? region : gridInfo().visualRegion(region);
        redraw(r);
    }
}

void QicsScreenGrid::handleGridPropertyChange(QicsGridStyle::QicsGridStyleProperty prop)
{
    bool recalc = false;
    bool repaint = false;

    switch (prop)
    {
    case QicsGridStyle::HorizontalGridLinesVisible:
    case QicsGridStyle::VerticalGridLinesVisible:
    case QicsGridStyle::HorizontalGridLineStyle:
    case QicsGridStyle::VerticalGridLineStyle:
    case QicsGridStyle::HorizontalGridLinePen:
    case QicsGridStyle::VerticalGridLinePen:
    case QicsGridStyle::GridCellClipping:
    case QicsGridStyle::DrawPartialCells:
    case QicsGridStyle::CellOverflowBehavior:
    case QicsGridStyle::CellToolTipOffset:
    case QicsGridStyle::MaxOverflowCells:
    case QicsGridStyle::CurrentCellBorderWidth:
        // These only require a redraw, not a recalc
        repaint = true;
        break;

    case QicsGridStyle::HorizontalGridLineWidth:
    case QicsGridStyle::VerticalGridLineWidth:
        // Have to recalc when these change
        recalc = true;
        repaint = true;
        break;

    case QicsGridStyle::GridRepaintBehavior:
        if (myNeedsRecomputeCellsFlag != Qics::NoIndex)
            recalc = true;
        else if (myNeedsRepaintFlag)
            repaint = true;
        break;

    case QicsGridStyle::FrameLineWidth:
        // Have to set this in the widget
        updateLineWidth();
        recalc = true;
        repaint = true;
        break;

    case QicsGridStyle::FrameStyle:
        // Have to set this in the widget
        updateFrameStyle();
        recalc = true;
        repaint = true;
        break;

    case QicsGridStyle::Viewport:
        // Only handle this if we don't have an explicit viewport set
        if (!myViewport.isValid()) {
            updateViewport();
            recalc = true;
            repaint = true;
        }
        //else {
        //    // #### TODO: Think about update() here
        //}
        break;

    default:
        // If we don't know about it, take the safe road and recalc/repaint...
        recalc = true;
        repaint = true;
        break;
    }

    if (recalc)
        recomputeAndDraw(Qics::RowAndColumnIndex);
    else if (repaint && isVisible()) {
        redraw();
    }
}

void QicsScreenGrid::drawHeaderResizeBar(int, int pos, Qics::QicsHeaderType type)
{
    if (testAttribute(Qt::WA_WState_InPaintEvent) || pos<0)
        return;

    if( type == Qics::RowHeader) {
        QPoint p = mapToParent(QPoint(frameWidth(),pos));
        m_resizeLine->setGeometry(p.x(), p.y(), width()-frameWidth(),1);
    }
    else {
        QPoint p = mapToParent(QPoint(pos,frameWidth()));
        m_resizeLine->setGeometry(p.x(), p.y(), 1,height()-frameWidth());
    }

    if (m_resizeLine->isHidden())
        m_resizeLine->show();
}

void QicsScreenGrid::traverse(Qics::QicsScrollDirection dir)
{
    switch(dir)
    {
    case Qics::ScrollLeft:
        traverseLeft();
        break;
    case Qics::ScrollRight:
        traverseRight();
        break;
    case Qics::ScrollUp:
        traverseUp();
        break;
    case Qics::ScrollDown:
        traverseDown();
        break;
    default:
        return;
    }
}

void QicsScreenGrid::handleRowsChanges(int num, int start_position)
{
    // if rows are inserted, we should shift notify cells down
    if (num > 0) {
        QicsICellV::iterator iter, iter_end(m_cellsToNotify.end());
        for (iter = m_cellsToNotify.begin(); iter != iter_end; ++iter)
        {
            int row = iter->row();
            if (row >= start_position)
                iter->setRow(row + num);
        }
    }

    int row = myUneditCell.row();
    start_position = m_info.visualRowIndex( start_position );

    // current cell is upper deleted/inserted region
    if (row < start_position && myEditingCurrentCell) {
        if (myUneditCell.isValid())
            setCurrentCell(myUneditCell);
        placeEntryWidget();
        return;
    }

    // current row deleted
    if (num<0 && myEditingCurrentCell && row >= start_position && row < start_position-num) {
        if (myUneditCell.isValid())
            setCurrentCell(myUneditCell);
        uneditCurrentCell();
        return;
    }

    // current cell is lover deleted region
    if (num<0 && row >= start_position-num && myEditingCurrentCell) {
        myUneditCell= QicsICell(myUneditCell.row()+num, myUneditCell.column() );

        if (myUneditCell.isValid())
            setCurrentCell( myUneditCell );
        if (!isCellVisible(currentCell().row(), currentCell().column()))
            makeCellFullyVisible(currentCell().row(), currentCell().column());

        placeEntryWidget();
        return;
    }

    // current cell is lover inserted region
    if (num > 0 && row >= start_position+num && myEditingCurrentCell) {
        myUneditCell = QicsICell( myUneditCell.row() + num, myUneditCell.column() );
        if (myUneditCell.isValid())
            setCurrentCell(myUneditCell);

        placeEntryWidget();
        return;
    }
}

void QicsScreenGrid::handleColumnsChanges(int num, int start_position)
{
    // if columns are inserted, we should shift notify cells down
    if (num > 0) {
        QicsICellV::iterator iter, iter_end(m_cellsToNotify.end());
        for (iter = m_cellsToNotify.begin(); iter != iter_end; ++iter)
        {
            int column = iter->column();
            if (column >= start_position)
                iter->setColumn(column + num);
        }
    }

    int column = myUneditCell.column();
    start_position = m_info.visualColumnIndex( start_position );

    // current cell is left deleted/inserted region
    if (column < start_position && myEditingCurrentCell) {
        if (myUneditCell.isValid())
            setCurrentCell(myUneditCell);
        placeEntryWidget();
        return;
    }

    // current column deleted
    if (num < 0 && myEditingCurrentCell && column >= start_position && column < start_position - num) {
        if (myUneditCell.isValid())
            setCurrentCell(myUneditCell);
        uneditCurrentCell();
        return;
    }

    // current cell is right deleted region
    if ( num<0 && column >= start_position-num && myEditingCurrentCell) {
        myUneditCell= QicsICell(myUneditCell.row(), myUneditCell.column()+num);
        if (myUneditCell.isValid())
            setCurrentCell( myUneditCell );
        if (!isCellVisible(currentCell().row(), currentCell().column()))
            makeCellFullyVisible(currentCell().row(), currentCell().column());
        placeEntryWidget();
        return;
    }

    // current cell is right inserted region
    if (num>0 && column >= start_position+num && myEditingCurrentCell) {
        myUneditCell = QicsICell( myUneditCell.row(), myUneditCell.column() + num );
        if (myUneditCell.isValid())
            setCurrentCell( myUneditCell );
        placeEntryWidget();
        return;
    }
}

bool QicsScreenGrid::focusNextWidget()
{
    return focusNextChild();
}

bool QicsScreenGrid::focusPreviousWidget()
{
    return focusPreviousChild();
}

void  QicsScreenGrid::timerScrollingEvent()
{
    const int row = cellToScroll.row();
    const int col = cellToScroll.column();
    int ss = speedOfScroll;
    QicsRegion real_vp = currentViewport();

    if (m_scrollDirec & Qics::ScrollUp) {
        if (row - speedOfScroll < real_vp.startRow()) {       // if step more than count rows to top
            speedOfScroll = row - real_vp.startRow();
        }
        if (speedOfScroll) {
            scrollUp(speedOfScroll);
            cellToScroll.setRow(row - speedOfScroll);
        }
        ss = speedOfScroll = qMax(ss, speedOfScroll);
    }
    else
        if (m_scrollDirec & Qics::ScrollDown) {
            if (row + speedOfScroll > real_vp.endRow()) {        // if step more than count rows to bottom
                speedOfScroll = real_vp.endRow() - row;
            }
            if (speedOfScroll) {
                scrollDown(speedOfScroll);
                cellToScroll.setRow(row + speedOfScroll);
            }
            ss = speedOfScroll = qMax(ss, speedOfScroll);
        }

    if (m_scrollDirec & Qics::ScrollLeft) {
        if (col - speedOfScroll < real_vp.startColumn()) {   // if step more than count columns to left
            speedOfScroll = col - real_vp.startColumn();
        }
        if (speedOfScroll) {
            scrollLeft(speedOfScroll);
            cellToScroll.setColumn(col - speedOfScroll);
        }
        ss = speedOfScroll = qMax(ss, speedOfScroll);
    }
    else
        if (m_scrollDirec & Qics::ScrollRight) {
            if (col + speedOfScroll > real_vp.endColumn()) {     // if step more than count columns to right
                speedOfScroll = real_vp.endColumn() - col;
            }
            if (speedOfScroll) {
                scrollRight(speedOfScroll);
                cellToScroll.setColumn(col + speedOfScroll);
            }
            ss = speedOfScroll = qMax(ss, speedOfScroll);
        }

    doScrollTraverse();
}

bool QicsScreenGrid::doCheckScrollUp(QMouseEvent *m)
{
    QicsRegion viewport = currentViewport();
    const int firstRow = gridInfo().firstNonHiddenRow(viewport.startRow(), viewport.endRow());

    if ((m->y() < 0) && (cellToScroll.row() != firstRow)) {
        m_scrollDirec |= Qics::ScrollUp;

        startScrollingTimer();

        speedOfScroll = (0 - m->y()) / 10 + 1;
        return true;
    }
    return false;
}

bool QicsScreenGrid::doCheckScrollDown(QMouseEvent *m)
{
    QicsRegion viewport = currentViewport();
    const int lastRow = gridInfo().lastNonHiddenRow(viewport.startRow(), viewport.endRow());

    if ((m->y() > height()) && (cellToScroll.row() != lastRow)) {
        m_scrollDirec |= Qics::ScrollDown;

        startScrollingTimer();

        speedOfScroll = -(height() - m->y()) / 10 + 1;
        return true;
    }
    return false;
}

bool QicsScreenGrid::doCheckScrollLeft(QMouseEvent *m)
{
    QicsRegion viewport = currentViewport();
    const int firstColumn = gridInfo().firstNonHiddenColumn(viewport.startColumn(), viewport.endColumn());

    if ((m->x() < 0) && (cellToScroll.column() != firstColumn)) {
        m_scrollDirec |= Qics::ScrollLeft;

        startScrollingTimer();

        speedOfScroll = (0 - m->x()) / 10 + 1;
        return true;
    }
    return false;
}

bool QicsScreenGrid::doCheckScrollRight(QMouseEvent *m)
{
    QicsRegion viewport = currentViewport();
    const int lastColumn = gridInfo().lastNonHiddenColumn(viewport.startColumn(), viewport.endColumn());

    if ((m->x() > width()) && (cellToScroll.column() != lastColumn)) {
        m_scrollDirec |= Qics::ScrollRight;

        startScrollingTimer();

        speedOfScroll = -(width() - m->x()) / 10 + 1;
        return true;
    }
    return false;
}

void QicsScreenGrid::stopScrollingTimer() const
{
    if (!timerScrolling->isActive()) return;

    timerScrolling->stop();
    disconnect (timerScrolling, SIGNAL(timeout()), this, SLOT(timerScrollingEvent()));
}

void QicsScreenGrid::startScrollingTimer() const
{
    if (timerScrolling->isActive()) return;

    connect (timerScrolling, SIGNAL(timeout()), this, SLOT(timerScrollingEvent()));
    timerScrolling->start(100);
}


