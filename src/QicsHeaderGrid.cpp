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

#include "QicsHeaderGrid.h"

#include <QRubberBand>
#include <QPainter>
#include <QDrag>

#include "QicsCell.h"
#include "QicsCellDisplay.h"
#include "QicsHeader.h"
#include "QicsDimensionManager.h"
#include "QicsSelectionManager.h"
#include "QicsSpanManager.h"
#include "QicsRowColumnDrag.h"
#include "QicsMouseMap.h"
#include "QicsStyleManager.h"

#define MAJOR_AXIS_INDEX(row, col) (m_type == Qics::RowHeader ? (row) : (col))
#define MAJOR_AXIS_POS(x, y) (m_type == Qics::RowHeader ? (y) : (x))


QicsHeaderGrid::QicsHeaderGrid(QWidget *w, QicsGridInfo &info, Qics::QicsHeaderType type)
    : QicsScreenGrid(w, info, 0, 0),
        m_type(type),
        m_gripThreshold(5),
        m_mouseInGrip(false),
        m_doingResize(false),
        m_anchorIndex(-1),
        m_selectionIndex(-1),
        m_isHeaderBeganResize(false),
        m_startResizingPos(0),
        m_haveIdropAfter(false),
        m_doubleClicked(false)
{
    m_editable = true;

    QSizePolicy policy;
    QicsHeader hdr(&info);

    if (m_type == Qics::RowHeader) {
        policy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        hdr.setResizeCursor(Qt::SplitVCursor);
    }
    else {
        policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        hdr.setResizeCursor(Qt::SplitHCursor);
    }

    setSizePolicy(policy);
    setAcceptDrops(true);
    setFocusPolicy(Qt::ClickFocus);
}

QicsHeaderGrid::~QicsHeaderGrid()
{
}

QicsHeaderGrid *QicsHeaderGrid::createGrid(QWidget *w, QicsGridInfo &info,
                                           Qics::QicsHeaderType type)
{
    return (new QicsHeaderGrid(w, info, type));
}

QicsRegion QicsHeaderGrid::currentViewport() const
{
    QicsRegion vp = QicsGrid::currentViewport();

    if (m_type == Qics::RowHeader) {
        if (vp.endColumn() == Qics::QicsLAST_COLUMN)
            vp.setEndColumn(0);
    }
    else {
        if (vp.endRow() == Qics::QicsLAST_ROW)
            vp.setEndRow(0);
    }

    return vp;
}

QSize QicsHeaderGrid::sizeHint() const
{
    QSize min = minimumSizeHint();
    QicsRegion real_vp = currentViewport();
    QicsHeader hdr(&(gridInfo()));
    int height, width;

    if (m_type == Qics::RowHeader) {
        height = min.height();

        width = (mappedDM().regionWidth(QicsRegion(real_vp.startRow(),
            real_vp.startColumn(), real_vp.startRow(), real_vp.endColumn())) +
            (2 * hdr.verticalGridLineWidth()) +
            hdr.verticalShadeLineWidth() +
            (2 * frameWidth()));
    }
    else {
        height = (mappedDM().regionHeight(QicsRegion(real_vp.startRow(),
            real_vp.startColumn(), real_vp.endRow(), real_vp.startColumn())) +
            (2 * hdr.horizontalGridLineWidth()) +
            hdr.horizontalShadeLineWidth() +
            (2 * frameWidth()));

        width = min.width();
    }

    return QSize(width, height);
}

bool QicsHeaderGrid::event( QEvent *e )
{
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *> (e);

        keyPressEvent(ke);
        return (ke->isAccepted());
    }

    return QicsScreenGrid::event(e);
}

void QicsHeaderGrid::handleMousePressEvent(const QicsICell &cell, QMouseEvent *m)
{
    if (!cell.isValid())
        return;

    const int row = cell.row();
    const int col = cell.column();

    m_pressedCoords = m->pos();

    if (!m_mouseInGrip) {
        m_traversalRow = row;
        m_traversalColumn = col;
    }

    QicsHeader hdr(&(gridInfo()));
    QicsMouseMap * mouseMap = hdr.mouseButtonRef();

    if (mouseMap->checkButton(Qt::MidButton,m) && !m_mouseInGrip)
        prepareDrag(cell, Qt::MoveAction);

    if (mouseMap->checkButton(Qt::LeftButton, m)) {
        if (m_mouseInGrip) {
            m_mouseInGrip = false;
            m_doingResize = true;
        }

        if (hdr.allowHeaderResize()) {
            QRect cellDimRect = cellDimensions(cell, true);

            if (m_type == Qics::RowHeader) {
                if((m->pos().x() > (width() - m_gripThreshold)) &&
                    (m->pos().x() < width() - frameWidth())) {
                        m_startResizingPos = m->globalX();
                        m_isHeaderBeganResize = true;
                }
                else if(((m->pos().x() > cellDimRect.x() + cellDimRect.width() - m_gripThreshold) ||
                    (m->pos().x() < cellDimRect.x() + m_gripThreshold)) &&
                    (m->pos().x() > 0) &&
                    ((m->pos().y() > cellDimRect.y() + m_gripThreshold) &&
                    (m->pos().y() < cellDimRect.y() + cellDimRect.height() - m_gripThreshold))) {
                        m_startResizingPos = m->globalX();
                        m_isHeaderBeganResize = true;
                }
            }
            else {
                if((m->pos().y() > (height() - m_gripThreshold)) &&
                    (m->pos().y() < height() - frameWidth())) {
                        m_startResizingPos = m->globalY();
                        m_isHeaderBeganResize = true;
                }
                else if(((m->pos().y() > cellDimRect.y() + cellDimRect.height() - m_gripThreshold) ||
                    (m->pos().y() < cellDimRect.y() + m_gripThreshold)) &&
                    (m->pos().y() > 0) &&
                    ((m->pos().x() > cellDimRect.x() + m_gripThreshold) &&
                    (m->pos().x() < cellDimRect.x() + cellDimRect.width() - m_gripThreshold))) {
                        m_startResizingPos = m->globalY();
                        m_isHeaderBeganResize = true;
                }
            }
        }

        if (m_doingResize) {
            if (hdr.allowUserResize())
                setResizeCursor(m);
        }
        else {
            if (m->modifiers() & Qt::ShiftModifier) {
                extendSelection(row, col);
                QicsScreenGrid::traverseToCell(row, col, false);
            }
            else if (m->modifiers() & Qt::ControlModifier) {
                addSelection(row, col);
                QicsScreenGrid::traverseToCell(row, col, false);
            }
            else {
                beginSelection(row, col);
                QicsScreenGrid::traverseToCell(row, col, false);
            }
        }
    }
}

void QicsHeaderGrid::handleMouseReleaseEvent(const QicsICell &cell, QMouseEvent *m)
{
    if (!cell.isValid())
        return;

    QicsHeader hdr(&(gridInfo()));
    QicsMouseMap *mouseMap = hdr.mouseButtonRef();

    if (hdr.allowHeaderResize() && m_isHeaderBeganResize) {
        m_isHeaderBeganResize = false;
        m_resizeLine->hide();

        if (m_type == Qics::RowHeader)
            emit headerResized(m->globalX() - m_startResizingPos, m_expandingCell.column(), m_type);
        else if (m_type == Qics::ColumnHeader)
            emit headerResized(m->globalY() - m_startResizingPos, m_expandingCell.row(), m_type);
    }
    else if (m_doingResize) {
        if (hdr.allowUserResize())
            finishResize();
    }
    else if (mouseMap->checkButton(Qt::LeftButton, m) && !m_doubleClicked) {
        endSelection(cell.row(), cell.column());
        setCurrentCell(cell);
    }

    if (timerScrolling->isActive()) {
        timerScrolling->stop();
        m_scrollDirec = Qics::ScrollNone;
    }

    m_doubleClicked = false;
}

void QicsHeaderGrid::handleMouseMoveEvent(const QicsICell &cell, QMouseEvent *m)
{
    if (!cell.isValid())
        return;

    const int row = cell.row();
    const int col = cell.column();

    QicsHeader hdr(&(gridInfo()));
    QicsMouseMap * mouseMap = hdr.mouseButtonRef();

    // check displayer
    QicsCellDisplay *cd = cellDisplay(row, col);
    // m_cell initialized already...
    if ((!m_cell->readOnly()) && m_cell->enabled()) {
        QCursor c = cursor();
        if (cd && cd->handleMouseEvent(this, row, col, m)) {
            setCursor(c);
            return;
        }
    }

    if (mouseMap->checkButton(Qt::MidButton, m) && m_dragCell.isValid())
        startDrag(Qt::MoveAction);
    else
        if (mouseMap->checkButton(Qt::LeftButton, m)) {
            if (m_doingResize) {
                Qics::QicsHeaderType type;

                m_expandingCellCurrentPosition = m->pos();

                int expandingCell = -1;
                int expandingCellCurrentPosition = 0;

                if (m_isHeaderBeganResize) {
                    if (m_type == Qics::RowHeader) {
                        type = Qics::ColumnHeader;
                        expandingCell = m_expandingCell.column();
                        expandingCellCurrentPosition = m->x();
                    }
                    else {
                        type = Qics::RowHeader;
                        expandingCell = m_expandingCell.row();
                        expandingCellCurrentPosition = m->y();
                    }
                }
                else {
                    if (m_type == Qics::RowHeader) {
                        expandingCell = m_expandingCell.row();
                        expandingCellCurrentPosition = m->y();
                    }
                    else {
                        expandingCell = m_expandingCell.column();
                        expandingCellCurrentPosition = m->x();
                    }

                    type = m_type;
                }

                drawHeaderResizeBar(expandingCell, expandingCellCurrentPosition, type);

                if (!m_isHeaderBeganResize)
                    emit resizeInProgress(expandingCell, expandingCellCurrentPosition, type);
            }

            if (m_isHeaderBeganResize || m_doingResize)
                return;

            if (m_doingSelectionFlag)
                dragSelection(m);
        }
        else {
            // check if we are interested in resizing anything..
            if (hdr.allowUserResize()) {
                setResizeCursor(m);
                return;
            }

            if (hdr.allowHeaderResize())
                return;

            QicsScreenGrid::handleMouseMoveEvent(cell, m);
        }
}

void QicsHeaderGrid::handleMouseDoubleClickEvent(const QicsICell &cell, QMouseEvent *m)
{
    if (!cell.isValid())
        return;

    const int row = cell.row();
    const int col = cell.column();

    QicsHeader hdr(&(gridInfo()));
    QicsMouseMap * mouseMap = hdr.mouseButtonRef();

    // check displayer
    QicsCellDisplay *cd = cellDisplay(row, col);
    // m_cell initialized already...
    if ((!m_cell->readOnly()) && m_cell->enabled()) {
        QCursor c = cursor();
        if (cd && cd->handleMouseEvent(this, row, col, m)) {
            setCursor(c);
            return;
        }
    }

    int in_row, close_row, in_column, close_column;

    if (hdr.allowUserResize() && isWithinResizeThreshold(m->x(), m->y(), &in_row, &close_row, &in_column, &close_column)) {
        m_doubleClicked = true;

        //emit gripDoubleClicked(qMin(in_cell, close_cell), m->button(), m_type);   // XXX
        if (m_type == Qics::RowHeader) {
            emit gripDoubleClicked(qMin(in_row, close_row), m->button(), m_type);   // XXX
        } else {
            emit gripDoubleClicked(qMin(in_column, close_column), m->button(), m_type);   // XXX
        }
    }

    if (hdr.allowUserEdit() && mouseMap->checkButton(Qt::LeftButton, m)) {
        QicsICell cur_cell = currentCell();

        if (cur_cell.isValid() && (cur_cell == cell)) {
            // Double click means edit this cell
            editCurrentCell();
            selectionManager().clearSelectionList();
        }
    }
}

void QicsHeaderGrid::emitMouseSignals(const QicsICell& cell, QMouseEvent* m)
{
    if (isWithinResizeThreshold(m->x(), m->y()) || m_doingResize)
        return;

    QicsScreenGrid::emitMouseSignals(cell, m);
}

void QicsHeaderGrid::finishResize()
{
    // unset...
    unsetCursor();
    emit hideResizeBarRequest();

    m_doingResize = false;

    m_resizeLine->hide();

    if (m_expandingCellStartPosition == m_expandingCellCurrentPosition)
        return;

    int new_size;
    if (m_type == Qics::ColumnHeader)        //  --->>>
        new_size = m_expandingCellSize.width() + (m_expandingCellCurrentPosition.x() - m_expandingCellStartPosition.x());
    else                                                //   <<<---
        new_size = m_expandingCellSize.height() + (m_expandingCellCurrentPosition.y() - m_expandingCellStartPosition.y());

    int old_size;

    // set the new value, do some bounding checks to make sure they
    //aren't doing something which is insane...
    int expandingCell = -1;

    if (m_type == Qics::RowHeader) {
        expandingCell = m_expandingCell.row();
        int min = mappedDM().rowMinHeight(expandingCell);
        int max = mappedDM().rowMaxHeight(expandingCell);
        if (new_size < min)
            new_size = min;
        if (new_size > max)
            new_size = max;

        old_size = mappedDM().rowHeight(expandingCell);
        mappedDM().setRowHeightInPixels(expandingCell, new_size);
    }
    else {
        expandingCell = m_expandingCell.column();
        int min = mappedDM().columnMinWidth(expandingCell);
        int max = mappedDM().columnMaxWidth(expandingCell);

        if (new_size < min)
            new_size = min;
        if (new_size > max)
            new_size = max;

        old_size = mappedDM().columnWidth(expandingCell);
        mappedDM().setColumnWidthInPixels(expandingCell, new_size);
    }

    // let the world know...
    emit sizeChange(expandingCell, old_size, new_size, m_type);

    repaint();
    gridInfo().redrawAllGrids();
}

bool QicsHeaderGrid::isWithinResizeThreshold(int x, int y, int *in_row, int *close_row, int *in_column, int *close_column) const
{
    QicsICell cell = cellAt(x, y, true);
    QicsRegion real_vp = currentViewport();

    if (in_row)
        *in_row = cell.row();

    if (in_column)
        *in_column = cell.column();

    QicsRegion span_region;
    const bool hasSpan = styleManager().spanManager()->isSpanner(m_info, cell.row(),cell.column(), span_region);//#90040

    if (!hasSpan && ((cell.column() < m_leftColumn) || (cell.column() > m_rightColumn)))
        return false;

    if ((cell.row() < m_topRow) || (cell.row() > m_bottomRow))
        return false;

    int prev_col, next_col, next_row, prev_row;

    if ((styleManager().spanManager())->isSpanner(m_info, cell.row(), cell.column(), span_region)) {
        prev_col = gridInfo().lastNonHiddenColumn(real_vp.startColumn(), cell.column() - 1);
        next_col = gridInfo().firstNonHiddenColumn(span_region.endColumn() + 1, real_vp.endColumn());
        prev_row = gridInfo().lastNonHiddenRow(real_vp.startRow(), cell.row() - 1);
        next_row = gridInfo().firstNonHiddenRow(span_region.endRow() + 1, real_vp.endRow());
        if (in_column)
            *in_column = gridInfo().firstNonHiddenColumn(span_region.startColumn(), span_region.endColumn());//#90040
        if (in_row)
            *in_row = gridInfo().firstNonHiddenRow(span_region.startRow(), span_region.endRow());//#90040
    }
    else {
        prev_col = gridInfo().lastNonHiddenColumn(real_vp.startColumn(), cell.column() - 1);
        next_col = gridInfo().firstNonHiddenColumn(cell.column() + 1, real_vp.endColumn());
        prev_row = gridInfo().lastNonHiddenRow(real_vp.startRow(), cell.row() - 1);
        next_row = gridInfo().firstNonHiddenRow(cell.row() + 1, real_vp.endRow());
    }

    if (next_col == -1)
        next_col = Qics::QicsLAST_COLUMN;

    if (next_row == -1)
        next_row = Qics::QicsLAST_ROW;

    bool ret = false;

    if (isLeftToRight()) {
        // right_edge of column
        if ((next_col <= m_rightColumn) && ((myColumnPositions[next_col] - m_gripThreshold) < x)) {
            if (close_column)
                *close_column = next_col;
            ret = true;
        }
        else if (next_col > m_rightColumn) { // rightmost==last column
            int right_edge = (myColumnPositions[m_rightColumn] + mappedDM().columnWidth(m_rightColumn));
            if ((x > (right_edge - m_gripThreshold)) && (x < right_edge)) {
                if (close_column)
                    *close_column = next_col;
                ret = true;
            }
        }

        if (!ret) {
            //  left age of column
            const int col = hasSpan ? gridInfo().firstNonHiddenColumn(span_region.startColumn(), span_region.endColumn()) : cell.column();//#90040
            if ((prev_col >= m_leftColumn) && ((myColumnPositions[col] + m_gripThreshold) > x)) {
                if (close_column)
                    *close_column = prev_col;
                ret = true;
            }
        }
    }
    else {
        // right_edge of column
        if ((next_col <= m_rightColumn) && ((myColumnPositions[cell.column()] + m_gripThreshold) > x)) {
            ret = true;
        }
        else if (next_col > m_rightColumn) { // rightmost==last column
            int right_edge = myColumnPositions[m_rightColumn];
            if ((x < (right_edge + m_gripThreshold)) && (x > right_edge - m_gripThreshold)) {
                ret = true;
            }
        }

        if (!ret) {
            //  left age of column
            //const int col = hasSpan ? *in_column : cell.column();//#90040
            if ((prev_col >= m_leftColumn) && ((myColumnPositions[prev_col] - m_gripThreshold) < x)) {
                if (close_column)
                    *close_column = prev_col;
                ret = true;
            }
        }
    }

    if ((next_row <= m_bottomRow) && ((myRowPositions[next_row] - m_gripThreshold) < y)) {
        if (close_row)
            *close_row = next_row;
        ret = true;
    }
    else if (next_row > m_bottomRow) {
        int bottom_edge = (myRowPositions[m_bottomRow] + mappedDM().rowHeight(m_bottomRow) );

        if ((y > (bottom_edge - m_gripThreshold)) && (y < bottom_edge)) {
            if (close_row)
                *close_row = next_row;
            ret = true;
        }
    }

    if (!ret) {
        if ((prev_row >= m_topRow) && ((myRowPositions[prev_row+1] + m_gripThreshold) > y)) {
            if (close_row)
                *close_row = prev_row;
            ret = true;
        }
    }

    return ret;
}

bool QicsHeaderGrid::setResizeCursor(QMouseEvent *m)
{
    int row_idx, column_idx;
    int close_row_idx, close_column_idx;

    QicsHeader hdr(&(gridInfo()));
    bool isWithin = isWithinResizeThreshold(m->x(), m->y(), &row_idx, &close_row_idx, &column_idx, &close_column_idx);
    const bool allowHeaderResize = hdr.allowHeaderResize();

    QicsICell old_cell(m_cell->rowIndex(), m_cell->columnIndex());
    QicsICell cell = cellAt(m->pos().x(), m->pos().y(), false);
    QRect cellDimRect = cellDimensions(cell, true);

    if (isWithin) {
        if (m_type == Qics::RowHeader) {
            if(allowHeaderResize &&
                (m->pos().x() > (width() - m_gripThreshold)) &&
                (m->pos().x() < (width() - frameWidth())) &&
                (m->pos().y() > 0)) {
                    setCursor(Qt::SizeHorCursor);
            }
            else if(allowHeaderResize && ((m->pos().x() > cellDimRect.x() + cellDimRect.width() - m_gripThreshold) ||
                (m->pos().x() < cellDimRect.x() + m_gripThreshold)) &&
                (m->pos().x() > 0) &&
                ((m->pos().y() > cellDimRect.y() + m_gripThreshold) &&
                (m->pos().y() < cellDimRect.y() + cellDimRect.height() - m_gripThreshold))) {
                setCursor(Qt::SizeHorCursor);
            }
            else if (((m->pos().y() > cellDimRect.y() + cellDimRect.height() - m_gripThreshold) ||
                (m->pos().y() < cellDimRect.y() + m_gripThreshold)) &&
                (m->pos().x() > 0)) {
                    setCursor(hdr.resizeCursor());
            }
            else {
                isWithin = false;
            }
        }
        else {
            if(allowHeaderResize &&
                (m->pos().y() > (height() - m_gripThreshold)) &&
                (m->pos().y() < (height() - frameWidth())) &&
                (m->pos().x() > 0)) {
                    setCursor(Qt::SizeVerCursor);
            }
            else if(allowHeaderResize && ((m->pos().y() > cellDimRect.y() + cellDimRect.height() - m_gripThreshold) ||
                (m->pos().y() < cellDimRect.y() + m_gripThreshold)) &&
                (m->pos().y() > 0) &&
                ((m->pos().x() > cellDimRect.x() + m_gripThreshold) &&
                (m->pos().x() < cellDimRect.x() + cellDimRect.width() - m_gripThreshold))) {
                setCursor(Qt::SizeVerCursor);
            }
            else if (((m->pos().x() > (cellDimRect.x() + cellDimRect.width() - m_gripThreshold)) ||
                (m->pos().x() < (cellDimRect.x() + m_gripThreshold))) &&
                (m->pos().y() > 0)) {
                    setCursor(hdr.resizeCursor());
            }
            else {
                isWithin = false;
            }
        }
    }

    if (!m_mouseInGrip && isWithin) {
        m_mouseInGrip = true;

        const int expandingRow = qMin(row_idx, close_row_idx);
        const int expandingColumn = qMin(column_idx, close_column_idx);

        m_expandingCell = QicsICell(expandingRow, expandingColumn);
        m_expandingCellSize = QSize(mappedDM().columnWidth(expandingColumn), mappedDM().rowHeight(expandingRow));
        m_expandingCellStartPosition = m->pos();
        m_expandingCellCurrentPosition = m->pos();

        return true;
    }

    if (m_mouseInGrip && !isWithin) {
        unsetCursor();
        m_mouseInGrip = false;
    }

    if (!m_mouseInGrip) {
        m_cell->setRowIndex(cell.row());
        m_cell->setColumnIndex(cell.column());

        setCursor(m_cell->cursor());

        m_cell->setRowIndex(old_cell.row());
        m_cell->setColumnIndex(old_cell.column());
    }

    return false;
}

void QicsHeaderGrid::keyPressEvent(QKeyEvent *ke)
{
    emit wideKeyPressed(ke);

    if (ke->isAccepted())
        return;

    ke->setAccepted(true);

    m_selectOnTraverse = (ke->modifiers() & Qt::ShiftModifier);

    const int key = ke->key();

    // common traversal
    switch (key)
    {
    case Qt::Key_Left:
        traverseLeft();
        return;
    case Qt::Key_Right:
        traverseRight();
        return;
    case Qt::Key_Up:
        traverseUp();
        return;
    case Qt::Key_Down:
    case Qt::Key_Enter:
    case Qt::Key_Return:
        traverseDown();
        return;
    case Qt::Key_Home:
        if (m_type == Qics::RowHeader)
            traverseToBeginningOfColumn();
        else
            traverseToBeginningOfRow();
        return;
    case Qt::Key_End:
        if (m_type == Qics::RowHeader)
            traverseToEndOfColumn();
        else
            traverseToEndOfRow();
        return;
    case Qt::Key_PageUp:
        if (m_type == Qics::RowHeader)
            traversePageUp();
        return;
    case Qt::Key_PageDown:
        if (m_type == Qics::RowHeader)
            traversePageDown();
        return;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////
/////////////        Selection methods                 ///////////////
//////////////////////////////////////////////////////////////////////

void QicsHeaderGrid::selectCell(int, int)
{
}

void QicsHeaderGrid::beginSelection(int row, int col)
{
    m_anchorIndex = MAJOR_AXIS_INDEX(row, col);
    m_doingSelectionFlag = true;

    reportSelection(row, col, Qics::SelectionBegin);
}

void QicsHeaderGrid::endSelection(int row, int col)
{
    m_doingSelectionFlag = false;

    reportSelection(row, col, Qics::SelectionEnd);
}

void QicsHeaderGrid::extendSelection(int row, int col)
{
    reportSelection(row, col, Qics::SelectionExtend);
}

void QicsHeaderGrid::addSelection(int row, int col)
{
    m_doingSelectionFlag = true;
    reportSelection(row, col, Qics::SelectionAdd);
}

void QicsHeaderGrid::clearSelection()
{
    reportSelection(-1, -1, Qics::SelectionNone);
}

void QicsHeaderGrid::dragSelection(QMouseEvent *m)
{
    QicsICell cell = cellAt(m->x(), m->y(), true);

    m_scrollDirec = Qics::ScrollNone;

    if (m_type == Qics::RowHeader) {
        if (!doCheckScrollUp(m))
            doCheckScrollDown(m);
    }
    else {
        if (!doCheckScrollLeft(m))
            doCheckScrollRight(m);
    }

    if (m_scrollDirec != Qics::ScrollNone)
        emit scrollRequest(static_cast<Qics::QicsScrollDirection>(m_scrollDirec), 1);

    cellToScroll = cell;
    doScrollTraverse();
}


void QicsHeaderGrid::reportSelection(int row, int col, Qics::QicsSelectionType stype)
{
    int idx = MAJOR_AXIS_INDEX(row, col);
    int prev_index;
    int anchor_index;

    int start_index, end_index;

    QicsRegion span;
    if ((styleManager().spanManager())->isSpanner(m_info, row, col, span)) {
        start_index = MAJOR_AXIS_INDEX(span.startRow(), span.startColumn());
        end_index = MAJOR_AXIS_INDEX(span.endRow(), span.endColumn());
    }
    else
        start_index = end_index = idx;

    if (m_selectionIndex != -1)
        prev_index = m_selectionIndex;
    else
        prev_index = idx;

    // If we are dragging and we haven't moved into a new cell, don't
    // signal anything

    if ((stype == Qics::SelectionDrag) && (idx == prev_index))
        return;

    if (stype == Qics::SelectionNone)
        m_selectionIndex = -1;
    else
        m_selectionIndex = idx;

    if (stype == Qics::SelectionAdd)
        anchor_index = start_index;
    else
        anchor_index = m_anchorIndex;

    if (m_type == Qics::RowHeader) {
        selectionManager().processSelectionEvent(stype,
            anchor_index, 0, end_index, Qics::QicsLAST_COLUMN);
    }
    else {
        selectionManager().processSelectionEvent(stype,
            0, anchor_index, Qics::QicsLAST_ROW, end_index);
    }
}

void QicsHeaderGrid::reset()
{
    updateGeometry();
}

void QicsHeaderGrid::handleGridPropertyChange(QicsGridStyle::QicsGridStyleProperty prop)
{
    bool recalc = false;
    bool repaint = false;

    switch (prop)
    {
    case QicsGridStyle::AllowUserResize:
        break;
    default:
        QicsScreenGrid::handleGridPropertyChange(prop);
    }

    if (recalc)
        recomputeAndDraw(Qics::RowAndColumnIndex);
    else if (repaint && isVisible())
        redraw();
}

void QicsHeaderGrid::handleFrameStyleUpdated()
{
    QicsScreenGrid *main_grid = qobject_cast<QicsScreenGrid *>(sender());
    Q_ASSERT(main_grid);
    if (!main_grid)
        return;

    int main_left, main_right, main_top, main_bottom;
    main_grid->getContentsMargins(&main_left, &main_top, &main_right, &main_bottom);

    int hdr_left, hdr_right, hdr_top, hdr_bottom;
    getContentsMargins(&hdr_left, &hdr_top, &hdr_right, &hdr_bottom);

    if (m_type == Qics::RowHeader)
        setContentsMargins(hdr_left, main_top, hdr_right, main_bottom);
    else
        setContentsMargins(main_left, hdr_top, main_right, hdr_bottom);

    redraw();
}

////////////////////////////////////////////////////////////////////////////
//////////////////////    Drag and Drop Methods    /////////////////////////
////////////////////////////////////////////////////////////////////////////

void QicsHeaderGrid::dragMoveEvent(QDragMoveEvent *event)
{
    QPoint pos = event->pos() - m_spotDelta;
    QicsICell cell = cellAt(pos.x(), pos.y(), false);

    bool canDrop = canDropAt(event, cell);
    QRect area = cellDimensions(cell, true);

    if (canDrop)
        event->accept(area);
    else
        event->ignore(area);
}

void QicsHeaderGrid::dropEvent(QDropEvent *event)
{
    QDropEvent *ev = new QDropEvent(event->pos() - m_spotDelta,
        event->possibleActions(), event->mimeData(), event->mouseButtons(), event->keyboardModifiers(),
        event->type());

    QicsScreenGrid::dropEvent(ev);

    delete ev;
}

void QicsHeaderGrid::dragEnterEvent(QDragEnterEvent *event)
{
    QicsScreenGrid::dragEnterEvent(event);
}

bool QicsHeaderGrid::canDropAt(QDragMoveEvent *event, const QicsICell &c) const
{
    QPoint pos = event->pos() - m_spotDelta;

    // if cell is invalid so we're going to move after last row/column
    QicsICell cell(c);
    if (!cell.isValid()) {
        if(m_type == Qics::RowHeader) {
            cell.setRow(pos.x() ?  bottomRow() : -1);
            cell.setColumn(0);
        }
        else {
            cell.setColumn(pos.y() ? rightColumn() : -1);
            cell.setRow(0);
        }
    }

    QicsHeader hdr(&(gridInfo()));

    if (!hdr.enabled() || hdr.readOnly())
        return false;

    // We must to do this to avoid moving from row headers to column headers.
    QicsHeaderGrid* sourceGrid = static_cast<QicsHeaderGrid*>(event->source());
    if(sourceGrid != 0 && sourceGrid->type() != m_type)
        return false;

    Qics::QicsIndexType select = (m_type == Qics::RowHeader ? Qics::RowIndex : Qics::ColumnIndex);
    if(event->source() == 0)
        select = (Qics::QicsIndexType) -1;

    return QicsRowColumnDrag::canDecode(event->mimeData(), select);
}

void QicsHeaderGrid::dropAt(QDropEvent *event, const QicsICell &c)
{
    /*!
    * dropAt tries the following methods of getting the data
    * If it's an intra-application drag
    *     look for row/column list first, then we can do an easy
    *     move or copy
    * if that fails, or if it is an inter-app drag, go right
    * to trying to decode celldata
    */

    // if cell is invalid so we're going to move after last row/column
    QicsICell cell(c);

    m_haveIdropAfter = true;

    if (!cell.isValid()) {
        if(m_type == Qics::RowHeader) {
            if (cell.row() == -1) {
                cell.setRow(topRow());
                m_haveIdropAfter = false;
            }

            cell.setColumn(0);
        }
        else {
            if (cell.column() == -1) {
                cell.setColumn(leftColumn());
                m_haveIdropAfter = false;
            }

            cell.setRow(0);
        }
    }

    if (event->source()) {
        // intra-application drag
        QVector<int> items;

        // qDebug("intra-app drag to %d,%d\n", cell.row(), cell.column());
        /* If we can decode a row or column list (as needed, depending
        * on what kind of header we are), then we can do a move.
        * - We have to map back from model coordinates to view, because
        * that's what the user sees.
        * - We'll want to sort the list, because multiple selections
        * can come in any order, but the user would not want them
        * any way but in visually increasing order.
        */
        Qics::QicsIndexType desiredType = (m_type == Qics::RowHeader ? Qics::RowIndex : Qics::ColumnIndex);

        if(QicsRowColumnDrag::decode(event->mimeData(), desiredType, items)) {
            for(int i = 0; i < items.size(); ++i) {
                if(m_type == Qics::RowHeader)
                    items[i] = gridInfo().visualRowIndex(items.at(i));
                else
                    items[i] = gridInfo().visualColumnIndex(items.at(i));
            }
            // we sort the indices to null out the effects of out
            // of order selection
            qSort(items);

            if(m_type == Qics::RowHeader) {
                if(m_haveIdropAfter)
                    gridInfo().moveRows(cell.row()+1, items);
                else
                    gridInfo().moveRows(cell.row(), items);
            }
            else {
                if(m_haveIdropAfter)
                    gridInfo().moveColumns(cell.column()+1, items);
                else
                    gridInfo().moveColumns(cell.column(), items);
            }
        }
        return;
    }

    // #### TODO: the whole XML transfer thing
    //if(event->provides(QICS_MIME_CELLDATA)) {
    //}

    event->ignore();
}

void QicsHeaderGrid::startDrag(Qt::DropAction mode)
{
    QicsHeader hdr(&(gridInfo()));

    if (!hdr.allowUserMove() || !m_dragCell.isValid())
        return;

    // Get the drag object
    QicsRowColumnDrag *obj =
        QicsRowColumnDrag::getDragObject(&gridInfo(),
        (m_type == Qics::RowHeader ? Qics::RowIndex : Qics::ColumnIndex), &m_dragCell, this);
    if (!obj)
        return;

    int col = m_dragCell.column();
    int row = m_dragCell.row();

    // Nuke this so we don't do a drag each time we get a mouse move event
    m_dragCell = QicsICell();

    Qics::QicsHeaderDragStyle extDrag = gridInfo().headerDragStyle();

    // Do the drag
    QDrag* drag = new QDrag(this);
    drag->setMimeData(obj);

    if ( extDrag == Qics::StyledDrag ) {
        QColor *pc1 = ( QColor* ) styleManager().getDefaultProperty ( QicsCellStyle::SelBackColor );
        QColor c1 = pc1 ? *pc1 : Qt::lightGray;

        // Set drag pixmap
        const QVector<int> &items = obj->itemList();

        if ( m_type == Qics::RowHeader ) {
            int w = mappedDM().columnWidth ( col );
            int h = 0;

            for ( int i = 0; i < items.size(); ++i )
                h += dimensionManager().rowHeight ( items.at ( i ) );

            m_dragPixmap = QPixmap ( w+300,h );
            QPainter p ( &m_dragPixmap );
            p.setOpacity ( 0.7 );

            h = 0;

            for ( int i = 0; i < items.size(); ++i ) {
                drawCell ( gridInfo().visualRowIndex ( items.at ( i ) ), col, 0, h, false, &p, QicsGrid::CellOnly );
                h += dimensionManager().rowHeight ( items.at ( i ) );
            }

            QLinearGradient lg ( 0,0,300,0 );
            lg.setColorAt ( 0, c1 );
            lg.setColorAt ( 1, Qt::white );
            p.fillRect ( w,0,300,h, QBrush ( lg ) );
            p.end();
        }
        else {
            int w = 0;
            int h = mappedDM().rowHeight ( row );

            for ( int i = 0; i < items.size(); ++i )
                w += dimensionManager().columnWidth ( items.at ( i ) );

            m_dragPixmap = QPixmap ( w,h+300 );
            QPainter p ( &m_dragPixmap );
            p.setOpacity ( 0.7 );

            w = 0;

            for ( int i = 0; i < items.size(); ++i ) {
                drawCell ( row, gridInfo().visualColumnIndex ( items.at ( i ) ), w, 0, false, &p, QicsGrid::CellOnly );
                w += dimensionManager().columnWidth ( items.at ( i ) );
            }

            QLinearGradient lg ( 0,0,0,300 );
            lg.setColorAt ( 0, c1 );
            lg.setColorAt ( 1, Qt::transparent );
            p.fillRect ( 0,h,w,300, QBrush ( lg ) );
            p.end();
        }

        drag->setPixmap ( m_dragPixmap );
    }

    if (type() == Qics::ColumnHeader) {
        m_spotDelta.setX(m_pressedCoords.x() - myColumnPositions[col]);
        m_spotDelta.setY(myRowPositions[row]);
    }
    else {
        m_spotDelta.setX(myColumnPositions[col]);
        m_spotDelta.setY(m_pressedCoords.y() - myRowPositions[row]);
    }

    drag->setHotSpot(m_spotDelta);

    drag->start(mode);
}

//////////////////////////////////////////////////////////////////////
/////////////////    Traversing methods    ///////////////////////////
//////////////////////////////////////////////////////////////////////

void QicsHeaderGrid::doScrollTraverse()
{
    reportSelection(cellToScroll.row(), cellToScroll.column(), Qics::SelectionDrag);
}

bool QicsHeaderGrid::traverseToCell(int row, int col, bool select_cell)
{
    if (!canTraverseToCell(row, col)) {
        if (myEditingCurrentCell && !myEditingCurrentCell->editWhenCurrent())
            uneditCurrentCell();
        return false;
    }

    m_traversalRow = row;
    m_traversalColumn = col;

    emit uneditRequest();

    if (select_cell) {
        selectCell(row, col);
        extendSelection(row, col);
    }
    else {
        beginSelection(row, col);
        endSelection(row, col);
    }

    setCurrentCell(QicsICell(row, col));

    makeCellFullyVisible(row, col);

    return true;
}



