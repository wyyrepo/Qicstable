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

#include "QicsGrid.h"

#include <qdrawutil.h>

#include "QicsCell.h"
#include "QicsRow.h"
#include "QicsColumn.h"
#include "QicsMainGrid.h"
#include "QicsCellDisplay.h"
#include "QicsMappedDimensionManager.h"
#include "QicsSpanManager.h"
#include "QicsStyleManager.h"
#include "QicsSelectionManager.h"
#include "QicsUtil.h"


QicsGrid::QicsGrid(QicsGridInfo &info, int top_row, int left_column)
    : m_info(info),
        m_topRow(top_row),
        m_leftColumn(left_column),
        m_bottomRow(-1),
        m_rightColumn(-1),
        myFullyVisibleRightColumn(-1),
        myFullyVisibleBottomRow(-1),
        m_traversalRow(-1),
        m_traversalColumn(-1)
{
    m_mainGrid = new QicsMainGrid(&info);
    m_cell = new QicsCell(-1, -1, &info, false, 0);
    m_row = new QicsRow(-1, &info, false, 0);
    m_column = new QicsColumn(-1, &info, false, 0);
}

QicsGrid::~QicsGrid()
{
    delete m_mainGrid;
    delete m_cell;
    delete m_row;
    delete m_column;

    m_mainGrid = 0;
    m_cell = 0;
    m_row = 0;
    m_column = 0;
}

///////////////////////////////////////////////////////////////////////
////////////// Viewport related methods ///////////////////////////////
///////////////////////////////////////////////////////////////////////

QicsRegion QicsGrid::viewport() const
{
    if (myViewport.isValid())
        return myViewport;

    return (m_mainGrid->viewport());
}

// computes the intersection of the data model dimensions and the
// viewport dimensions
QicsRegion QicsGrid::currentViewport() const
{
    QicsRegion vp = realViewport();

    int startRow = vp.startRow();
    int startCol = vp.startColumn();
    int endRow = vp.endRow();
    int endCol = vp.endColumn();

    endRow = gridInfo().lastNonHiddenRow(startRow, endRow);
    endCol = gridInfo().lastNonHiddenColumn(startCol, endCol);
    startRow = gridInfo().firstNonHiddenRow(startRow, endRow);
    startCol = gridInfo().firstNonHiddenColumn(startCol, endCol);

    return QicsRegion(startRow, startCol, endRow, endCol);
}

// computes the intersection of the data model dimensions and the
// viewport dimensions
QicsRegion QicsGrid::realViewport() const
{
    if (!dataModel())
        return QicsRegion(0, 0, 0, 0);

    int endRow, endCol;

    QicsRegion vp = viewport();

    const int lastRow = dataModel()->lastRow();
    const int lastCol = dataModel()->lastColumn();

    if (vp.endRow() > lastRow)
        endRow = lastRow;
    else
        endRow = vp.endRow();

    if (vp.endColumn() > lastCol)
        endCol = lastCol;
    else
        endCol = vp.endColumn();

    return QicsRegion(vp.startRow(), vp.startColumn(), endRow, endCol);
}

bool QicsGrid::isCellValid(int row, int col) const
{
    m_cell->setRowIndex(row);
    m_cell->setColumnIndex(col);

    if (m_cell->isHidden())
        return false;

    const QicsRegion &real_vp = currentViewport();

    if (real_vp.isValid()) {
        if (real_vp.containsCell(row, col))
            return true;

        // else, check if it is a span
        QicsRegion reg;
        if (m_info.styleManager()->spanManager()->isSpanner(m_info, row, col, reg))
            return real_vp.intersects(reg);
    }

    return false;
}

///////////////////////////////////////////////////////////////////////
/////////////////  Grid layout methods  ///////////////////////////////
///////////////////////////////////////////////////////////////////////

QicsICell QicsGrid::computeCellPositions(const QRect &bounds,
                               const QicsICell &start,
                               Qics::QicsIndexType indexType)
{
    QicsRegion real_vp = currentViewport();
    const QicsMappedDimensionManager *mappedDimension = &mappedDM();

    int bottom_row = m_bottomRow;
    int right_column = m_rightColumn;
    Qics::QicsIndexType type = m_mainGrid->fakeCellsDrawingIndex();

    if( (indexType == Qics::RowIndex) || (indexType == Qics::RowAndColumnIndex) ) {
        // Clear out the old position info
        myRowPositions.clear();
        //
        // Compute row locations
        //
        int hlw = m_mainGrid->horizontalGridLineWidth() + m_mainGrid->horizontalShadeLineWidth();

        myRowPositions.setFirstIndex(start.row());

        int this_row = start.row();
        int current_y = bounds.top() + hlw;
        while ( (this_row <= real_vp.endRow() || type == Qics::RowIndex || type == Qics::RowAndColumnIndex)
            && (current_y <= bounds.bottom()) ) {
            m_row->setRowIndex(this_row);
            if (!m_row->isHidden()) {
                myRowPositions.push_back(current_y);
                current_y += (mappedDimension->rowHeight(this_row) + hlw);
            }
            else {
                // not shown, therefore a bogus row position to hold the spot
                myRowPositions.push_back(-1);
            }
            ++this_row;
        }
        // set bottom visible and bottom fully visible row
        bottom_row = this_row - 1;
        if (bounds.height() + bounds.y() < current_y && bottom_row != -1)
            myFullyVisibleBottomRow = bottom_row - 1;
        else if (bottom_row != -1)
            myFullyVisibleBottomRow = bottom_row;
    }

    if( (indexType == Qics::ColumnIndex) || (indexType == Qics::RowAndColumnIndex) ) {
        // Clear out the old position info
        myColumnPositions.clear();

        int vlw = m_mainGrid->verticalGridLineWidth() + m_mainGrid->verticalShadeLineWidth();
        //
        // Compute column locations
        //
        if (m_mainGrid->layoutDirection() == Qt::RightToLeft) {     //   <<<<----

            myColumnPositions.setFirstIndex(start.column());

            int this_col = start.column();
            int current_x = bounds.right() - vlw - mappedDimension->columnWidth(this_col) + 1;
            int lastColX = 0;
            while ((this_col <= real_vp.endColumn() || type == Qics::ColumnIndex || type == Qics::RowAndColumnIndex)
                && ((current_x >= bounds.left()) || (myColumnPositions[this_col-1] >= bounds.left() || this_col==start.column()))) {
                m_column->setColumnIndex(this_col);
                if (!m_column->isHidden()) {
                    myColumnPositions.push_back(current_x);
                    lastColX = current_x;
                    current_x -= (mappedDimension->columnWidth(this_col+1) + vlw);
                }
                else {
                    // not shown, therefore a bogus column position to hold the spot
                    myColumnPositions.push_back(-1);
                }
                ++this_col;
            }
            // set right visible and right fully visible column
            right_column = this_col-1;
            if (lastColX < 0)
                myFullyVisibleRightColumn = right_column - 1;
            else
                myFullyVisibleRightColumn = right_column;
        }
        else {                                                      //    ---->>>>

            myColumnPositions.setFirstIndex(start.column());

            int this_col = start.column();
            int current_x = bounds.left() + vlw;
            while ( (this_col <= real_vp.endColumn() || type == Qics::ColumnIndex || type == Qics::RowAndColumnIndex)
                && (current_x <= bounds.right()) ) {
                m_column->setColumnIndex(this_col);

                if (!m_column->isHidden()) {
                    myColumnPositions.push_back(current_x);
                    current_x += (mappedDimension->columnWidth(this_col) + vlw);
                }
                else {
                    // not shown, therefore a bogus column position to hold the spot
                    myColumnPositions.push_back(-1);
                }

                ++this_col;
            }
            // set right visible and right fully visible column
            right_column = this_col - 1;
            if (bounds.width() + bounds.x() < current_x && right_column != -1)
                myFullyVisibleRightColumn = right_column - 1;
            else if (right_column != -1)
                myFullyVisibleRightColumn = right_column;
        }
    }
    return (QicsICell(bottom_row, right_column));
}

QRect QicsGrid::cellDimensions(int row, int col, bool with_spans) const
{
    QRect retval;
    if ((row < myRowPositions.firstIndex()) ||
        (row > myRowPositions.lastIndex()) ||
        (col < myColumnPositions.firstIndex()) ||
        (col > myColumnPositions.lastIndex())) {
        // Cell is not on screen.  Return the invalid rectangle
        return retval;
    }

    retval.setLeft(myColumnPositions[col]);
    retval.setTop(myRowPositions[row]);

    QicsRegion span_region;
    const QicsMappedDimensionManager *mappedDimension = &mappedDM();

    if (with_spans && (styleManager().spanManager()->isSpanner(m_info, row, col, span_region))) {
        // find 1st visible cell of spanner
        while (retval.left() == -1 && ++col <= myColumnPositions.lastIndex())
            retval.setLeft(myColumnPositions[col]);
        while (retval.top() == -1 && ++row <= myRowPositions.lastIndex())
            retval.setTop(myRowPositions[row]);

        if (m_mainGrid->layoutDirection() == Qt::LeftToRight) { //  ---->>>>>
            retval.setWidth(mappedDimension->regionWidth(span_region));
            retval.setHeight(mappedDimension->regionHeight(span_region));
        }
        else {                                                  //   <<<<-----
            retval.setWidth(mappedDimension->regionWidth(span_region));
            retval.setHeight(mappedDimension->regionHeight(span_region));
            retval.moveLeft(myColumnPositions[span_region.right()]);
        }
    }
    else {
        retval.setWidth(mappedDimension->columnWidth(col));
        retval.setHeight(mappedDimension->rowHeight(row));
    }

    return retval;
}

#ifdef notdef

void
QicsGrid::dumpPositions() const
{
    QicsPositionList::iterator iter;
    int i;

    qDebug("ROWS: ");
    for (iter = myRowPositions.begin();
        iter != myRowPositions.end();
        ++iter)
    {
        qDebug("%d ", *iter);
    }
    qDebug("\n");

    qDebug("HEIGHTS: ");
    for (i = myRowPositions.firstIndex();
        i <= myRowPositions.lastIndex();
        ++i)
    {
        qDebug("%d ", mappedDM().rowHeight(i));
    }
    qDebug("\n");

    qDebug("COLS: ");
    for (iter = myColumnPositions.begin();
        iter != myColumnPositions.end();
        ++iter)
    {
        qDebug("%d ", *iter);
    }
    qDebug("\n");

    qDebug("WIDTHS: ");
    for (i = myColumnPositions.firstIndex();
        i <= myColumnPositions.lastIndex();
        ++i)
    {
        qDebug("%d ", mappedDM().columnWidth(i));
    }
    qDebug("\n");
}

#endif

///////////////////////////////////////////////////////////////////////
/////////////////  Grid drawing methods  //////////////////////////////
///////////////////////////////////////////////////////////////////////

QRect QicsGrid::drawRegion(const QicsRegion &region, QPainter *painter, QicsGrid::DrawCellMode mode)
{
    QRect painted_rect;

    if (region.isEmpty())
        return painted_rect;

    int i, j;
    int x, y, width, height, row = 0, col;
    const QicsMappedDimensionManager *mappedDimension = &mappedDM();

    Qics::QicsCellOverflowBehavior over = m_mainGrid->cellOverflowBehavior();
    int max_over = m_mainGrid->maxOverflowCells();

    QicsRegion screen_vp = screenViewport();

    int vlw = m_mainGrid->verticalGridLineWidth() + m_mainGrid->verticalShadeLineWidth();
    int hlw = m_mainGrid->horizontalGridLineWidth() + m_mainGrid->horizontalShadeLineWidth();

    // first, put all the drawable cells to the map
    QList<QicsICell> myCells, myOffSpans;

    for (i = region.startRow(); i <= region.endRow(); ++i) {
        m_row->setRowIndex(i);
        if (m_row->isHidden())
            continue;

        // handle special row displayers
        QicsCellDisplay *rcd = m_row->displayer();
        if (rcd && rcd->useWholeRow()) {
            col = m_info.firstNonHiddenColumn(m_leftColumn, m_rightColumn);
            if (col >= 0) {

                y = myRowPositions.at(i);
                x = myColumnPositions.at(col);

                //width = mappedDM().regionWidth(QicsRegion(i,col,i,myInfo.lastNonHiddenColumn(myLeftColumn, myRightColumn)));
                width = mappedDimension->regionWidth(region);
                height = mappedDimension->rowHeight(i);

                QRect rect(x, y, width, height);
                if (rect.isValid())
                    painted_rect |= drawCell(i, col, rect, painter, mode);

            }
            continue;
        }

        for (j = region.startColumn(); j <= region.endColumn(); ++j) {
            m_column->setColumnIndex(j);
            if (m_column->isHidden())
                continue;

            myCells.append(QicsICell(i,j));
        }
    }

    // ok, then we should handle all the spans
    //QicsSpanList *spans = styleManager().spanManager()->visualCellSpanList(myInfo);
    QicsSpanList *spans = styleManager().spanManager()->cellSpanList();
    if (!spans->isEmpty()) {
        for (i = 0; i < spans->count(); ++i) {
            const QicsSpan &span = spans->at(i);
            QicsRegion span_region;
            span_region.setStartRow(m_info.visualRowIndex(m_info.firstNonHiddenModelRow(span.row(), span.row() + span.height() - 1)));
            span_region.setStartColumn(m_info.visualColumnIndex(m_info.firstNonHiddenModelColumn(span.column(), span.column() + span.width() - 1)));
            span_region.setEndRow(span_region.startRow() + span.height() -1);
            span_region.setEndColumn(span_region.startColumn() + span.width() -1);

            QicsRegion reg(screen_vp);
            reg &= span_region;
            if (!reg.isValid()) {
                // span is off-screen, it may be used as off-span while drawing overflows
                if (over != Qics::Overflow) continue;
                if (span_region.endColumn() >= screen_vp.startColumn()) continue;   // it is too right
                if (span_region.endRow() < screen_vp.startRow()) continue;          // it is too high
                if (span_region.startRow() > screen_vp.endRow()) continue;          // it is too low
                if (screen_vp.startColumn() - span_region.endColumn() > max_over) continue; // it is too left
                // add cells to off-span list
                for (int r = span_region.startRow(); r <= span_region.endRow(); ++r)
                    myOffSpans.append(QicsICell(r,span_region.endColumn()));
            }

            for (int rr = reg.startRow(); rr <= reg.endRow(); ++rr)
                for (int c = reg.startColumn(); c <= reg.endColumn(); ++c)
                    myCells.removeAll(QicsICell(rr,c));

            row = m_info.firstNonHiddenRow( reg.startRow(), reg.endRow() );
            if (m_mainGrid->layoutDirection() == Qt::LeftToRight)
                col = m_info.firstNonHiddenColumn( reg.startColumn(), reg.endColumn() );
            else
                col = m_info.lastNonHiddenColumn( reg.startColumn(), reg.endColumn() );

            y = myRowPositions[row];
            x = myColumnPositions[col];
            //int spanner_row = span.row();
            //int spanner_col = span.column();
            int spanner_row = m_info.visualRowIndex(m_info.firstNonHiddenModelRow(span.row(), span.row() + span.height() - 1));
            int spanner_col = m_info.visualColumnIndex(m_info.firstNonHiddenModelColumn(span.column(), span.column() + span.width() - 1));

            // We need to (partially) draw the spanned cell.  We begin
            // by constructing a region that we can use to determine how
            // much of the span region is off the screen.  We then use
            // these dimensions to negatively offset the starting position
            // of the spanned cell so we only draw part of it.
            QicsRegion r(spanner_row, spanner_col,
                qMax(spanner_row, row-1), qMax(spanner_col, col-1));

            if (spanner_col < col) {
                if (m_mainGrid->layoutDirection() == Qt::RightToLeft)   //     <<<<<----
                    x = myColumnPositions[span_region.right()];
                else                                                    //     ---->>>>>
                    x -= (mappedDimension->regionWidth(r) + vlw);
            }

            if (spanner_row < row)
                y -= (mappedDimension->regionHeight(r) + hlw);

            width = mappedDimension->regionWidth(span_region);
            height = mappedDimension->regionHeight(span_region);

            // Change the row and column that we are going to draw to the
            // spanner cell.
            row = spanner_row;
            col = spanner_col;

            QRect rect(x, y, width, height);

            // draw
            if (rect.isValid())
                painted_rect |= drawCell(row, col, rect, painter, mode);
        }
    }
    delete spans;

    // and, if overflow is on, we must check all the leftmost cells
    col = region.startColumn();
    if (over == Qics::Overflow && col) {
        for (i = region.startRow(); i <= region.endRow(); ++i) {
            m_row->setRowIndex(i);
            if (m_row->isHidden())
                continue;

            // check if col is empty or drawn as a span already
            if (!myCells.contains(QicsICell(i, col)) )
                continue;

            QicsCellDisplay *cd = cellDisplay(i, col);
            const QicsDataItem *di = cellValue(i, col);
            if (cd && !(cd->isEmpty(&m_info, row, col, di)))
                continue;

            int prev_col = col-1;
            while (prev_col >= 0 && (col-prev_col) < max_over) {
                m_column->setColumnIndex(prev_col);
                if (!m_column->isHidden()) {
                    QicsICell cell(i, prev_col);
                    if (myOffSpans.contains(cell))
                        break;

                    QicsCellDisplay *cd = cellDisplay(i, prev_col);
                    const QicsDataItem *di = cellValue(i, prev_col);
                    if (cd && !(cd->isEmpty(&m_info, row, prev_col, di))) {
                        // measure...
                        y = myRowPositions.at(i);
                        width = 0;
                        for (j = prev_col; j < col; ++j) {
                            width += mappedDimension->columnWidth(j) + vlw;
                        }
                        x = myColumnPositions.at(col) - width;
                        height = mappedDimension->rowHeight(i);

                        QRect rect(x, y, width, height);
                        QRect full_rect = cd->contentRect(this, i, prev_col, di, rect, painter);
                        if (full_rect.width() > rect.width()) {
                            // draw it here...
                            int next_col = col;
                            while (full_rect.width() > rect.width()
                                && next_col <= screen_vp.endColumn()
                                && (next_col-prev_col) < max_over ) {

                                    QicsICell next_cell(i, next_col);
                                    m_column->setColumnIndex(next_col);

                                    if (!m_column->isHidden()) {
                                        if (!myCells.contains(next_cell)) break;
                                        //if (next_cell == cur_cell) break;

                                        cd = cellDisplay(i, next_col);
                                        if (cd && !(cd->isEmpty(&m_info, i, next_col, cellValue(i, next_col)))) break;

                                        rect.setWidth(rect.width() + mappedDimension->columnWidth(next_col) + vlw);
                                        myCells.removeAll(next_cell);
                                    }

                                    ++next_col;
                            }

                            painted_rect |= drawCell(i, prev_col, rect, painter, mode);
                        }

                        //myCells.append(QicsICell(i, prev_col));
                        break;
                    }
                }
                --prev_col;
            }
        }
    }

    // now, draw all the unspanned cells
    while (!myCells.isEmpty()) {
        const QicsICell &cell = myCells.first();

        y = myRowPositions.at(row = cell.row());
        x = myColumnPositions.at(col = cell.column());

        width = mappedDimension->columnWidth(col);
        height = mappedDimension->rowHeight(row);

        QRect rect(x, y, width, height);

        // here check overflow...
        if (over == Qics::Overflow) {// && cell != cur_cell
            QicsCellDisplay *cd = cellDisplay(row, col);
            const QicsDataItem *di = cellValue(row, col);

            if (cd && !(cd->isEmpty(&m_info, row, col, di))) {
                QRect full_rect = cd->contentRect(this, row, col, di, rect, painter);
                int next_col = col+1;
                while (full_rect.width() > rect.width()
                    && next_col <= screen_vp.endColumn()
                    && (next_col-col) < max_over ) {

                        QicsICell next_cell(row, next_col);
                        m_column->setColumnIndex(next_col);

                        if (!m_column->isHidden()) {
                            if (!myCells.contains(next_cell)) break;
                            //if (next_cell == cur_cell) break;

                            cd = cellDisplay(row, next_col);
                            if (cd && !(cd->isEmpty(&m_info, row, next_col, cellValue(row, next_col)))) break;

                            rect.setWidth(rect.width() + mappedDimension->columnWidth(next_col) + vlw);
                            myCells.removeAll(next_cell);
                        }

                        ++next_col;
                }
            }
        }

        // draw
        painted_rect |= drawCell(row, col, rect, painter, mode);

        myCells.takeFirst();
    }

    return painted_rect;
}

void QicsGrid::drawGridLines(const QicsRegion &region, QPainter *painter)
{
    if (region.isEmpty() || !region.isValid())
        return;

    const Qics::QicsLineStyle hls = m_mainGrid->horizontalGridLineStyle();
    const Qics::QicsLineStyle vls = m_mainGrid->verticalGridLineStyle();

    const int vslw = m_mainGrid->verticalShadeLineWidth();
    const int vlw = m_mainGrid->verticalGridLineWidth() + vslw;
    const int vlro = (vlw / 2) + (vlw % 2);
    const int vllo = vlw - vlro;

    const int hslw = m_mainGrid->horizontalShadeLineWidth();
    const int hlw = m_mainGrid->horizontalGridLineWidth() + hslw;
    const int hlbo = (hlw / 2) + (hlw % 2);
    const int hlto = hlw - hlbo;

    QicsRegion reg;
    if (vslw > 0 || hslw > 0)
        reg = QicsRegion(m_topRow, m_leftColumn, m_bottomRow, m_rightColumn);
    else
        reg = region;

    const int start_row = gridInfo().firstNonHiddenRow(reg.startRow(), reg.endRow());
    const int end_row = gridInfo().lastNonHiddenRow(start_row, reg.endRow());

    const int start_col = gridInfo().firstNonHiddenColumn(reg.startColumn(), reg.endColumn());
    const int end_col = gridInfo().lastNonHiddenColumn(start_col, reg.endColumn());

    if (start_row < 0 || end_row < start_row ||
        start_col < 0 || end_col < start_col)
        return;

    painter->save();

    const QicsMappedDimensionManager *mappedDimension = &mappedDM();
    const int end_col_w = mappedDimension->columnWidth(end_col);
    const int start_col_w = mappedDimension->columnWidth(start_col);
    const int end_row_h = mappedDimension->rowHeight(end_row);

    int i, j;
    const bool for_printer = isPrinterDevice(painter, this);

    const bool do_horiz = m_mainGrid->horizontalGridLinesVisible();
    const bool do_vert = m_mainGrid->verticalGridLinesVisible();
    const QColor &bg = m_mainGrid->backgroundColor();
    QPoint p1, p2;

    if (do_horiz) {
        if (do_horiz && ((hls == Qics::Sunken) || (hls == Qics::Raised))) {
            int last_vis_row = -1;
            const QPalette &pal = m_mainGrid->palette();
            for (i = start_row; i <= end_row; ++i) {
                m_row->setRowIndex(i);
                if (m_row->isHidden())
                    continue;

                p1.setY(myRowPositions.at(i) - hlbo);
                p2.setY(myRowPositions.at(i) - hlbo);

                if (m_mainGrid->layoutDirection() == Qt::LeftToRight) { //  ---->>>>>
                    p1.setX(myColumnPositions.at(start_col) - vlro);
                    p2.setX(myColumnPositions.at(end_col) + end_col_w);
                }
                else {                                                  //   <<<<-----
                    p1.setX(myColumnPositions.at(start_col) + start_col_w);
                    p2.setX(myColumnPositions.at(end_col));
                }

                qDrawShadeLine(painter, p1.x(), p1.y(), p2.x(), p2.y(),
                    pal, (hls == Qics::Sunken), hlw - hslw);

                last_vis_row = i;
            }

            if (last_vis_row >= 0) {
                p1.setY(myRowPositions.at(last_vis_row) + mappedDimension->rowHeight(last_vis_row) + hlto);
                p2.setY(p1.y());

                if (m_mainGrid->layoutDirection() == Qt::LeftToRight) { //  ---->>>>>
                    p1.setX(myColumnPositions.at(start_col) - vlro);
                    p2.setX(myColumnPositions.at(end_col) + end_col_w);
                }
                else {                                                  //   <<<<-----
                    p1.setX(myColumnPositions.at(start_col) + start_col_w);
                    p2.setX(myColumnPositions.at(end_col));
                }

                qDrawShadeLine(painter, p1.x(), p1.y(), p2.x(), p2.y(),
                    pal, (hls == Qics::Sunken), hlw - hslw);
            }
        }
        else {
            QPen pen, hlp;
            bool skip_lines = false;

            if (!do_horiz || (hls == Qics::None)) {
                if (for_printer)
                    skip_lines = true;
                else {
                    pen.setColor(bg);
                    pen.setWidth(hlw);
                    painter->setPen(pen);
                }
            }
            else {
                hlp = m_mainGrid->horizontalGridLinePen();
                hlp.setWidth(hlw);
                pen = QPen(bg);
                pen.setWidth(hlw);
                if(!do_horiz) hlp = pen;
            }

            if (!skip_lines) {
                int last_vis_row = -1;
                for (i = start_row; i <= end_row; ++i) {
                    m_row->setRowIndex(i);
                    if (m_row->isHidden())
                        continue;

                    p1.setY(myRowPositions.at(i) - hlbo);
                    p2.setY(myRowPositions.at(i) - hlbo);

                    if (m_mainGrid->layoutDirection() == Qt::LeftToRight) { //  ---->>>>>
                        p1.setX(myColumnPositions.at(start_col) - vlro);
                        p2.setX(myColumnPositions.at(end_col) + end_col_w);
                    }
                    else {                                                  //   <<<<-----
                        p1.setX(myColumnPositions.at(start_col) + start_col_w);
                        p2.setX(myColumnPositions.at(end_col));
                    }

                    painter->setPen(pen);
                    painter->drawLine(p1, p2);
                    painter->setPen(hlp);
                    painter->drawLine(p1, p2);

                    last_vis_row = i;
                }

                if (last_vis_row >= 0) {
                    p1.setY(myRowPositions.at(last_vis_row) + mappedDimension->rowHeight(last_vis_row) + hlto);
                    p2.setY(p1.y());

                    if (m_mainGrid->layoutDirection() == Qt::LeftToRight) { //  ---->>>>>
                        p1.setX(myColumnPositions.at(start_col) - vlro);
                        p2.setX(myColumnPositions.at(end_col) + end_col_w);
                    }
                    else {                                                  //   <<<<-----
                        p1.setX(myColumnPositions.at(start_col) + start_col_w);
                        p2.setX(myColumnPositions.at(end_col));
                    }

                    painter->setPen(pen);
                    painter->drawLine(p1, p2);
                    painter->setPen(hlp);
                    painter->drawLine(p1, p2);
                }
            }
        }
    } // do_horiz

    if (do_vert) {
        if (do_vert && ((vls == Qics::Sunken) || (vls == Qics::Raised))) {
            int last_vis_col = -1;
            const QPalette &pal = m_mainGrid->palette();
            for (j = start_col; j <= end_col; ++j) {
                m_column->setColumnIndex(j);
                if (m_column->isHidden())
                    continue;

                p1.setX(myColumnPositions.at(j) - vlro);
                p1.setY(myRowPositions.at(start_row) - hlbo);
                p2.setX(myColumnPositions.at(j) - vlro);
                p2.setY(myRowPositions.at(end_row) + end_row_h);

                qDrawShadeLine(painter, p1.x(), p1.y(), p2.x(), p2.y(),
                    pal, (vls == Qics::Sunken), vlw - vslw);

                last_vis_col = j;
            }

            // draw one more vertical line
            if (m_mainGrid->layoutDirection() == Qt::LeftToRight) { //  ---->>>>>
                if (last_vis_col >= 0) {
                    int last_vis_col_w = mappedDimension->columnWidth(last_vis_col);

                    p1.setX(myColumnPositions.at(last_vis_col) + last_vis_col_w + vllo);
                    p1.setY(myRowPositions.at(start_row) - hlbo);
                    p2.setX(myColumnPositions.at(last_vis_col) + last_vis_col_w + vllo);
                    p2.setY(myRowPositions.at(end_row) + end_row_h + hlw);

                    qDrawShadeLine(painter, p1.x(), p1.y(), p2.x(), p2.y(),
                        pal, (vls == Qics::Sunken), vlw - vslw);
                }
            }
            else {                                                  //   <<<<-----
                if (start_col >= 0) {
                    p1.setX(myColumnPositions.at(start_col) + start_col_w + vllo);
                    p1.setY(myRowPositions.at(start_row) - hlbo);
                    p2.setX(myColumnPositions.at(start_col) + start_col_w + vllo);
                    p2.setY(myRowPositions.at(end_row) + end_row_h + hlw);

                    qDrawShadeLine(painter, p1.x(), p1.y(), p2.x(), p2.y(),
                        pal, (vls == Qics::Sunken), vlw - vslw);
                }
            }

        }
        else {
            QPen pen, vlp;
            bool skip_lines = false;

            if (!do_vert || (vls == Qics::None)) {
                if (for_printer)
                    skip_lines = true;
                else {
                    pen.setColor(bg);
                    pen.setWidth(vlw);
                }
            }
            else {
                vlp = m_mainGrid->verticalGridLinePen();
                vlp.setWidth(vlw);
                pen = QPen(bg);
                pen.setWidth(vlw);
                if(!do_vert) vlp = pen;
            }

            if (!skip_lines) {
                int last_vis_col = -1;

                for (j = start_col; j <= end_col; ++j) {
                    m_column->setColumnIndex(j);
                    if (m_column->isHidden())
                        continue;

                    p1.setX(myColumnPositions.at(j) - vlro);
                    p1.setY(myRowPositions.at(start_row) - hlbo);
                    p2.setX(myColumnPositions.at(j) - vlro);
                    p2.setY(myRowPositions.at(end_row) + end_row_h + hlbo);

                    painter->setPen(pen);
                    painter->drawLine(p1, p2);
                    painter->setPen(vlp);
                    painter->drawLine(p1, p2);

                    last_vis_col = j;
                }

                // one more vertical line
                if (m_mainGrid->layoutDirection() == Qt::LeftToRight) { //  ---->>>>>
                    if (last_vis_col >= 0) {
                        int last_vis_col_w = mappedDimension->columnWidth(last_vis_col);

                        p1.setX(myColumnPositions.at(last_vis_col) + last_vis_col_w + vllo);
                        p1.setY(myRowPositions.at(start_row) - hlbo);
                        p2.setX(myColumnPositions.at(last_vis_col) + last_vis_col_w + vllo);
                        p2.setY(myRowPositions.at(end_row) + end_row_h + hlbo);

                        painter->setPen(pen);
                        painter->drawLine(p1, p2);
                        painter->setPen(vlp);
                        painter->drawLine(p1, p2);
                    }
                }
                else {                                                  //   <<<<-----
                    if (start_col >= 0) {
                        p1.setX(myColumnPositions.at(start_col) + start_col_w + vllo);
                        p1.setY(myRowPositions.at(start_row) - hlbo);
                        p2.setX(myColumnPositions.at(start_col) + start_col_w + vllo);
                        p2.setY(myRowPositions.at(end_row) + end_row_h + hlbo);

                        painter->setPen(pen);
                        painter->drawLine(p1, p2);
                        painter->setPen(vlp);
                        painter->drawLine(p1, p2);
                    }
                }
            }
        }
    }  // do_vert

    painter->restore();
}

QRect QicsGrid::drawCell(int row, int col, const QRect &rect,
                   QPainter *painter, QicsGrid::DrawCellMode mode)
{
    QRect r(rect);

    bool clip_set = false;
    if (isPrinterDevice(painter, this) && boundRect().isValid())
    {
        // This is important for spanned cells.  We want to make sure
        // that a span that is partially off the screen only draws where
        // it is supposed to.
        painter->save();
        painter->setClipRect(boundRect());
        clip_set = true;
    }

    if (prepareToDraw(row, col, r, painter)) {
        QicsCellDisplay *cd = cellDisplay(row, col);
        if (cd) {
            if (mode & QicsGrid::CellOnly)//##89640
                cd->displayCell(this, row, col, cellValue(row, col), r, painter);
            if (mode & QicsGrid::CellBordersOnly)
                cd->drawCellBorders(&m_info, row, col, r, painter);
        }
    }

    if (clip_set)
        painter->restore();

    return r;
}

QRect QicsGrid::drawCell(int row, int col, int x, int y,
                   bool look_for_overflower, QPainter *painter, QicsGrid::DrawCellMode mode)
{
    QicsSpanManager *spm = styleManager().spanManager();
    const QicsMappedDimensionManager *mappedDimension = &mappedDM();
    QicsCellDisplay *cd;
    QRect painted_rect;

    if (look_for_overflower) {
        // If this cell is empty, we look leftward for the first non-empty
        // cell that might want to overflow into this cell

        QicsRegion real_vp = currentViewport();

        QicsRegion reg;
        int new_col = col;

        m_cell->setRowIndex(row);

        while (((col - new_col) < m_mainGrid->maxOverflowCells()) &&
            (new_col >= real_vp.startColumn()) &&
            !spm->insideSpan(m_info, row, new_col, reg)) {
            m_cell->setColumnIndex(new_col);
            if (!m_cell->isHidden()) {
                cd = cellDisplay(row, col);
                if (cd && !(cd->isEmpty(&m_info, row, new_col,
                    cellValue(row, new_col)))) {
                    // Don't bother if this is the original cell
                    if (new_col == col)
                        break;
                    QicsRegion newreg(row, new_col, row, col - 1);

                    int vlw = m_mainGrid->verticalGridLineWidth() + m_mainGrid->verticalShadeLineWidth();
                    int new_x = 0;
                    // calculate new_x position of the cell
                    if (m_mainGrid->layoutDirection() == Qt::RightToLeft) {     //     <<<<<-----
                        new_x = x + mappedDimension->regionWidth(newreg) + vlw;
                        painted_rect = painted_rect.united(drawCell(row, new_col, new_x, y, false, painter, mode));
                        break;
                    }
                    else {                                                      //     ----->>>>>
                        new_x = x - mappedDimension->regionWidth(newreg) - vlw;
                        painted_rect = painted_rect.united(drawCell(row, new_col, new_x, y, false, painter, mode));
                        break;
                    }
                }
            }
            --new_col;
        }
    }

    // We may have "drawn" this cell already via an overflow,
    // so before we do anything else we had better check...

    if (myAlreadyDrawnCells.indexOf(QicsICell(row, col)) != -1)
        return painted_rect;

    QicsRegion span_region;
    int width, height;

    bool spanner;
    bool spanned = spm->insideSpan(m_info, row, col, span_region, &spanner);

    // If this cell is spanned we don't draw it...
    if (spanned && !spanner) {
        // ...unless, of course, the spanner cell is off the screen
        // to the left or to the top, in which case we have to make sure
        // the spanner cell is partially drawn.

        int spanner_row = span_region.startRow();
        int spanner_col = span_region.startColumn();

        // Look through the list of drawn cells in case the spanner has
        // already been drawn

        if (myAlreadyDrawnCells.indexOf(QicsICell(spanner_row, spanner_col)) != -1)
            return painted_rect;

        // We need to (partially) draw the spanned cell.  We begin
        // by constructing a region that we can use to determine how
        // much of the span region is off the screen.  We then use
        // these dimensions to negatively offset the starting position
        // of the spanned cell so we only draw part of it.

        QicsRegion r(spanner_row, spanner_col,
            qMax(spanner_row, row-1), qMax(spanner_col, col-1));

        int vlw = m_mainGrid->verticalGridLineWidth() + m_mainGrid->verticalShadeLineWidth();
        int hlw = m_mainGrid->horizontalGridLineWidth()+ m_mainGrid->horizontalShadeLineWidth();

        if (spanner_col < col) {
            if (m_mainGrid->layoutDirection() == Qt::RightToLeft)   //     <<<<<----
                x = myColumnPositions[span_region.right()];
            else                                                    //     ---->>>>>
                x -= (mappedDimension->regionWidth(r) + vlw);
        }

        if (spanner_row < row)
            y -= (mappedDimension->regionHeight(r) + hlw);

        width = mappedDimension->regionWidth(span_region);
        height = mappedDimension->regionHeight(span_region);

        // Change the row and column that we are going to draw to the
        // spanner cell.
        row = spanner_row;
        col = spanner_col;

        myAlreadyDrawnCells.push_back(QicsICell(row, col));
    }
    else if (spanner) {
        // the dimensions of the entire spanned region
        width = mappedDimension->regionWidth(span_region);
        height = mappedDimension->regionHeight(span_region);

        if (m_mainGrid->layoutDirection() == Qt::RightToLeft) { //     <<<<<----
            int tmpx = myColumnPositions.at(span_region.right());
            if (tmpx==-1)
                x -= width - mappedDimension->columnWidth(span_region.left());
            else
                x = tmpx;
        }
        myAlreadyDrawnCells.push_back(QicsICell(row,col));
    }
    else {
        width = mappedDimension->columnWidth(col);
        height = mappedDimension->rowHeight(row);
    }

    QRect rect(x, y, width, height);

    bool clip_set = false;
    if (isPrinterDevice(painter, this) && boundRect().isValid())
    {
        // This is important for spanned cells.  We want to make sure
        // that a span that is partially off the screen only draws where
        // it is supposed to.
        painter->save();
        painter->setClipRect(boundRect());
        clip_set = true;
    }

    if (prepareToDraw(row, col, rect, painter)) {
        cd = cellDisplay(row, col);
        if (cd) {
            if (mode & QicsGrid::CellBordersOnly)
                cd->drawCellBorders(&m_info, row, col, rect, painter);
            if (mode & QicsGrid::CellOnly)
                cd->displayCell(this, row, col, cellValue(row, col), rect, painter);
        }
    }

    if (clip_set)
        painter->restore();

    painted_rect = painted_rect.united(rect);
    return painted_rect;
}

bool QicsGrid::prepareToDraw(int, int, const QRect &, QPainter *)
{
    return true;
}

const QicsDataItem *QicsGrid::cellValue(int row, int col) const
{
    return gridInfo().cellValue(row, col);
}

QicsCellDisplay *QicsGrid::cellDisplay(int row, int col) const
{
    QicsRegion span;
    QicsSpanManager *spm = styleManager().spanManager();

    if (spm->insideSpan(m_info, row, col, span)) {
        row = span.startRow();
        col = span.startColumn();
    }

    m_cell->setRowIndex(row);
    m_cell->setColumnIndex(col);

    return (m_cell->displayer());
}

int QicsGrid::modelColumnIndex(int c) const
{
    return m_info.modelColumnIndex(c);
}

int QicsGrid::modelRowIndex(int r) const
{
    return m_info.modelRowIndex(r);
}

//////////////////////////////////////////////////////////////////////
/////////////////    Mapping/Ordering methods    /////////////////////
//////////////////////////////////////////////////////////////////////

void QicsGrid::orderRowsBy(const QVector<int> &columns, Qics::QicsSortOrder order, DataItemComparator func)
{
    m_info.orderRowsBy(columns, order, 0, -1, func);
}

void QicsGrid::orderColumnsBy(const QVector<int> &rows, Qics::QicsSortOrder order, DataItemComparator func)
{
    m_info.orderColumnsBy(rows, order, 0, -1, func);
}


