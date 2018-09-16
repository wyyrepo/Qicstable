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

#include "QicsTableGrid.h"

#include <QApplication>

#include "QicsCell.h"
#include "QicsMainGrid.h"
#include "QicsUtil.h"
#include "QicsHeader.h"
#include "QicsCellDisplay.h"
#include "QicsDataModelDefault.h"
#include "QicsTableRegionDrag.h"
#include "QicsPasteValidator.h"
#include "QicsMouseMap.h"
#include "QicsSelectionManager.h"
#include "QicsSpanManager.h"
#include "QicsStyleManager.h"
#include "QicsMappedDimensionManager.h"
#include "QicsRubberBand.h"
#include "QicsTableCommon.h"


QicsTableGrid::QicsTableGrid(QWidget *w, QicsGridInfo &info,
                             int top_row, int left_column)
    : QicsScreenGrid(w, info, top_row, left_column),
         m_exclusiveMode(false),
         m_exclusiveDrag(false),
         m_updateRect(),
         m_fillBand(0)
{
    m_selectionAnchorCell = 0;
    m_selectionCurrentCell = 0;
    m_editable = true;

    setAcceptDrops(true);

    setFocusPolicy(Qt::WheelFocus);

    m_fillBand = new QicsRubberBand(QRubberBand::Rectangle, this);
    m_fillBand->hide();
}

QicsTableGrid::~QicsTableGrid()
{
    delete m_selectionAnchorCell;
    delete m_selectionCurrentCell;

    m_selectionAnchorCell = 0;
    m_selectionCurrentCell = 0;
}

QicsTableGrid *QicsTableGrid::createGrid(QWidget *w, QicsGridInfo &info,
                          int top_row, int left_column)
{
    return (new QicsTableGrid(w, info, top_row, left_column));
}

void QicsTableGrid::reset()
{
    clearSelection();
    uneditCurrentCell();
    setCurrentCell(QicsICell());
}

/////////////////////////////////////////////////////////////////
/////////////        Event Handlers              ////////////////
/////////////////////////////////////////////////////////////////

// We reimplement this so we can get the first shot at key press
// events before the Qt traversal code steals tabs and shift-tabs.

bool QicsTableGrid::event( QEvent *e )
{
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *> (e);

        keyPressEvent(ke);
        return (ke->isAccepted());
    }

    return QicsScreenGrid::event(e);
}

QicsSelection QicsTableGrid::fillingSelection(const QicsICell &cell)
{
    QicsSelection *sel = const_cast<QicsSelection*>( gridInfo().selectionManager()->exclusiveSelection() );
    if (!sel) return QicsSelection();

    QicsSelection fillSel(*sel);
    fillSel |= QicsSelection(cell.row(), cell.column(), cell.row(), cell.column());

    if (fillSel.endColumn() == Qics::QicsLAST_COLUMN)
        fillSel.setEndColumn(dataModel()->numColumns()-1);
    if (fillSel.endRow() == Qics::QicsLAST_ROW)
        fillSel.setEndRow(dataModel()->numRows()-1);

    return fillSel;
}

QRect QicsTableGrid::fillingRect(const QicsICell &cell)
{
    QRect r;
    QicsSelection fillSel = fillingSelection(cell);
    if (!fillSel.isValid()) return r;

    // intercept filling selection with viewport
    QicsRegion reg(fillSel.topLeftCell(), fillSel.bottomRightCell());
    reg &= screenViewport();

    // convert start of the region to screen coordinates
    r = cellDimensions(reg.startCell(), false);

    r.setWidth(mappedDM().regionWidth(reg));
    r.setHeight(mappedDM().regionHeight(reg));

    return r;
}

void QicsTableGrid::handleMousePressEvent(const QicsICell &cell, QMouseEvent *m)
{
    QicsHeader hdr(&(gridInfo()));
    QicsMouseMap * mouseMap = hdr.mouseButtonRef();

    if (!cell.isValid())
        return;

    const bool ex = m_exclusiveMode && m_fillHandlePickRect.contains(m->pos());
    if (ex) {
        gridInfo().reportFillHandleEvent(Qics::FillHandlePressed, m, gridInfo().selectionManager()->exclusiveSelection(), 0);

        m_exclusiveDrag = true;
        lastFillCell = cellToScroll = cell;
        return;
    }

    if (mouseMap->checkButton(Qt::LeftButton, m)) {
        if (m->modifiers() & Qt::ShiftModifier) {
            if (gridInfo().selectionManager()->canSelectMultiple()) {
                extendSelection(cell.row(), cell.column());
                if (!traverseToCell(cell.row(), cell.column(), false))
                    setCurrentCell(QicsICell());
            }
        }
        else if (m->modifiers() & Qt::ControlModifier) {
            if (gridInfo().selectionManager()->canSelectMultiple()) {
                addSelection(cell.row(), cell.column());
                if (!traverseToCell(cell.row(), cell.column(), false))
                    setCurrentCell(QicsICell());
            }
        }
        else { // no modifiers
            QicsICell cur_cell = currentCell();
            QicsCellDisplay *cd = cellDisplay(cur_cell.row(), cur_cell.column());

            // Editing cell when  editWhenCurrent is true
            if (cur_cell == cell && cd->editWhenCurrent())
                editCurrentCell();

            beginSelection(cell.row(), cell.column());
            if (!traverseToCell(cell.row(), cell.column(), false))
                setCurrentCell(QicsICell());
        }
    }

    prepareDrag(cell, Qt::CopyAction);
}

void QicsTableGrid::handleMouseReleaseEvent(const QicsICell &cell, QMouseEvent *m)
{
    QicsHeader hdr(&(gridInfo()));
    QicsMouseMap * mouseMap = hdr.mouseButtonRef();

    if (mouseMap->checkButton(Qt::LeftButton, m)) {
        if (timerScrolling->isActive()) {
            timerScrolling->stop();
            m_scrollDirec = Qics::ScrollNone;
        }

        if (m_exclusiveMode && m_exclusiveDrag) {
            m_exclusiveDrag = false;
            if (!m_fillBand->isVisible()) return;
            m_fillBand->hide();

            if (cell.isValid()) lastFillCell = cell;
            QicsSelection sel = fillingSelection(lastFillCell);

            gridInfo().reportFillHandleEvent(Qics::FillHandleReleased, m, gridInfo().selectionManager()->exclusiveSelection(), &sel);

            beginSelection(sel.topRow(), sel.leftColumn());
            addSelection(sel.topRow(), sel.leftColumn());

            cellToScroll = sel.bottomRightCell();
            doScrollTraverse();

            endSelection(sel.bottomRow(), sel.rightColumn());

            // just to update cursor state
            QicsScreenGrid::handleMouseMoveEvent(cell, m);

            return;
        }

        endSelection(cell.row(), cell.column());
    }
}

void QicsTableGrid::handleMouseDoubleClickEvent(const QicsICell &cell, QMouseEvent *m)
{
    QicsHeader hdr(&(gridInfo()));
    QicsMouseMap * mouseMap = hdr.mouseButtonRef();

    const bool ex = m_exclusiveMode && m_fillHandlePickRect.contains(m->pos());

    if (ex) {
        gridInfo().reportFillHandleEvent(Qics::FillHandleDoubleClicked, m, gridInfo().selectionManager()->exclusiveSelection(), 0);

        m_exclusiveDrag = false;
        return;
    }

    if (mouseMap->checkButton(Qt::LeftButton, m)) {
        QicsICell cur_cell = currentCell();

        if (cur_cell.isValid() && (cur_cell == cell)) {
            // Double click means edit this cell
            editCurrentCell();
        }
    }
}

void QicsTableGrid::handleMouseMoveEvent(const QicsICell &cell, QMouseEvent *m)
{
    if (m_exclusiveDrag) {

        m_doingSelectionFlag = true;

        lastFillCell = cellToScroll = cell;
        m_scrollDirec = Qics::ScrollNone;
        if (doCheckScrollUp(m)) cellToScroll.setRow(cellToScroll.row()-1);
        if (doCheckScrollDown(m)) cellToScroll.setRow(cellToScroll.row()+1);
        if (doCheckScrollLeft(m)) cellToScroll.setColumn(cellToScroll.column()-1);
        if (doCheckScrollRight(m)) cellToScroll.setColumn(cellToScroll.column()+1);
        if (m_scrollDirec == Qics::ScrollNone)
            timerScrolling->stop();
        doScrollTraverse();

        QRect rect = fillingRect(cell);
        m_fillBand->setGeometry(rect);

        if (!m_fillBand->isVisible()) {
            m_fillBand->setBrush(m_mainGrid->exclusiveSelectionDragBackColor());
            m_fillBand->setPen(m_mainGrid->exclusiveSelectionDragPen());
            m_fillBand->setOpacity(m_mainGrid->exclusiveSelectionDragOpacity());
            m_fillBand->setDrawingMode(m_mainGrid->exclusiveSelectionDragMode());
            m_fillBand->show();
        }

        QicsSelection sel = fillingSelection(cell);

        redraw(m_oldRegion);
        m_oldRegion = sel.toRegion().adjusted(-1,-1,1,1);
        m_oldRegion.normalize();
        redraw(m_oldRegion);

        gridInfo().reportFillHandleEvent(Qics::FillHandleDragged, m, gridInfo().selectionManager()->exclusiveSelection(), &sel);

        return;
    }

    QicsHeader hdr(&(gridInfo()));
    QicsMouseMap * mouseMap = hdr.mouseButtonRef();

    if (cell.isValid()) {
        if (mouseMap->checkButton(Qt::LeftButton, m)) {
            if (m_doingSelectionFlag) {
                m_scrollDirec = Qics::ScrollNone;
                int ss = 0;

                doCheckScrollUp(m);     ss = qMax(ss, speedOfScroll);
                doCheckScrollDown(m);   ss = qMax(ss, speedOfScroll);
                doCheckScrollLeft(m);   ss = qMax(ss, speedOfScroll);
                doCheckScrollRight(m);  ss = qMax(ss, speedOfScroll);

                if (m_scrollDirec == Qics::ScrollNone)
                    timerScrolling->stop();

                speedOfScroll = ss;
                cellToScroll = cell;
                doScrollTraverse();

                return;
            }
        }

        if (mouseMap->checkButton(Qt::MidButton, m)) {
            if ((m->modifiers() & Qt::ControlModifier) ) {
                myDropAction = Qt::MoveAction;
                startDrag(Qt::MoveAction);
            }
            else {
                myDropAction = Qt::CopyAction;
                startDrag(Qt::CopyAction);
            }
        }
    }

    QicsScreenGrid::handleMouseMoveEvent(cell, m);

    if (m_exclusiveMode && m_fillHandlePickRect.contains(m->pos()))
        setCursor(Qt::SizeAllCursor);
}

void QicsTableGrid::focusInEvent(QFocusEvent *fe)
{
    QicsICell cur_cell = currentCell();

    if (!cur_cell.isValid() && fe->reason() == Qt::TabFocusReason)
        traverseToCell(topRow(), leftColumn());

    if (fe->reason() == Qt::ActiveWindowFocusReason && cur_cell.isValid()) {
        QicsCellDisplay *cd = cellDisplay(cur_cell.row(),cur_cell.column());
        if (cd && cd->editWhenCurrent())
            editCurrentCell();
    }

    // #### AM: this is to avoid strange flicker on windows with QicsWidgetCellDisplay
    if (fe->reason() == Qt::ActiveWindowFocusReason && parentWidget())
        parentWidget()->repaint();
}

void QicsTableGrid::focusOutEvent(QFocusEvent *fe)
{
    // #### AM: this is to avoid strange flicker on windows with QicsWidgetCellDisplay
    if (fe->reason() == Qt::ActiveWindowFocusReason)
        redraw();
}

void QicsTableGrid::dropEvent(QDropEvent *event)
{
    QicsScreenGrid::dropEvent(event);
}

void QicsTableGrid::dragEnterEvent(QDragEnterEvent *event)
{
    QicsScreenGrid::dragEnterEvent(event);
}

void QicsTableGrid::dragMoveEvent(QDragMoveEvent *event)
{
    QicsICell cell = cellAt(event->pos().x(), event->pos().y(), false);

    bool canDrop = canDropAt(event, cell);
    QRect area = cellDimensions(cell, true);

    if (canDrop)
        event->accept(area);
    else
        event->ignore(area);
}

/////////////////////////////////////////////////////////////////
/////////////////    Actions      ///////////////////////////////
/////////////////////////////////////////////////////////////////

bool QicsTableGrid::canDropAt(QDragMoveEvent * event, const QicsICell &cell) const
{
    if (!cell.isValid())
        return false;

    m_cell->setRowIndex(cell.row());
    m_cell->setColumnIndex(cell.column());

    if (!m_cell->canAcceptDrops()) {
#ifdef notdef
        qDebug("WARN: QicsTableGrid::canDropAt: cell(%d,%d) not able to droping", cell.row(), cell.column());
#endif
        return false;
    }


    if (!m_cell->enabled() || m_cell->readOnly())
        return false;

    return (event->mimeData()->hasText() || event->mimeData()->hasImage());
}

void QicsTableGrid::dropAt(QDropEvent *event, const QicsICell &cell)
{
    const QMimeData *data = event->mimeData();

    QicsPasteValidator *pValidator = m_cell->pasteValidator();

    if (m_cell) {
        QicsDataModelDefault tmp_dm;

        if (!pValidator)
            m_tableCommon->paste(data, cell);
        else if (QicsTableRegionDrag::decode(data, tmp_dm)) {
            if (pValidator->validate(cell,selectionManager().selectionList(true),tmp_dm,gridInfo().dataModel()))
                m_tableCommon->paste(data, cell);
            else
                myDropAction = Qt::CopyAction;
        }
        else {
            tmp_dm.addRows(1);
            tmp_dm.addColumns(1);
            tmp_dm.setItem(0,0,QicsDataString(data->text()));
            if (pValidator->validate(cell,selectionManager().selectionList(true),tmp_dm,gridInfo().dataModel()))
                m_tableCommon->paste(data, cell);
            else
                myDropAction = Qt::CopyAction;
        }

        repaint();
    }
}

//////////////////////////////////////////////////////////////////////
/////////////        Selection methods                 ///////////////
//////////////////////////////////////////////////////////////////////

void QicsTableGrid::selectCell(int row, int col)
{
    Qics::QicsSelectionPolicy sel_policy = selectionManager().selectionPolicy();

    // First of all if we have entire row selected and
    // cell that gonna be selected in this row we do nothing
    // (it is already selected!!!) to avoid blinking.

    if ((sel_policy == Qics::SelectSingleRow ||
        sel_policy == Qics::SelectMultipleRow) &&
        row == currentCell().row() &&
        selectionManager().selectionList(true) &&
        selectionManager().selectionList(true)->size() == 1)
        return;

    // reg contains uncleared region
    QicsRegion reg = selectionManager().selectedRegion();

    bool blocked = gridInfo().blockSelectionSignals();
    gridInfo().setBlockSelectionSignals(true);
    clearSelection();
    addSelection(row, col);
    gridInfo().setBlockSelectionSignals(blocked);
    endSelection(row, col);

    reg |= selectionManager().selectedRegion();
    redraw(reg);
}

void QicsTableGrid::beginSelection(int row, int col)
{
    if (isCellValid(row, col)) {
        m_doingSelectionFlag = true;
        reportSelection(row, col, Qics::SelectionBegin);
    }
}

void QicsTableGrid::endSelection(int row, int col)
{
    if (m_doingSelectionFlag) {
        m_doingSelectionFlag = false;
        reportSelection(row, col, Qics::SelectionEnd);
    }
}

void QicsTableGrid::dragSelection(int row, int col)
{
    if (isCellValid(row, col))
        reportSelection(row, col, Qics::SelectionDrag);
}

void QicsTableGrid::extendSelection(int row, int col)
{
    if (isCellValid(row, col))
        reportSelection(row, col, Qics::SelectionExtend);
}

void QicsTableGrid::addSelection(int row, int col)
{
    if (isCellValid(row, col)) {
        m_doingSelectionFlag = true;
        reportSelection(row, col, Qics::SelectionAdd);
    }
}

void QicsTableGrid::clearSelection()
{
    reportSelection(-1, -1, Qics::SelectionNone);
}

void QicsTableGrid::reportSelection(int row, int col, Qics::QicsSelectionType stype)
{
    int last_row = -1;
    int last_col = -1;
    int anchor_row, anchor_col;

    if (m_selectionCurrentCell) {
        last_row = m_selectionCurrentCell->row();
        last_col = m_selectionCurrentCell->column();
    }
    else if (stype != Qics::SelectionEnd) {
        last_row = row;
        last_col = col;
    }

    // If we are dragging and we haven't moved into a new cell, don't
    // signal anything

    if ((stype == Qics::SelectionDrag) && (row == last_row) && (col == last_col))
        return;

    // Set anchor cell if necessary
    if ((stype == Qics::SelectionBegin) || (stype == Qics::SelectionAdd))
        setSelectionAnchorCell(new QicsICell(row, col));

    if (!m_selectionAnchorCell || (stype == Qics::SelectionAdd)) {
        anchor_row = row;
        anchor_col = col;
    }
    else {
        anchor_row = m_selectionAnchorCell->row();
        anchor_col = m_selectionAnchorCell->column();
    }

    // No more selection...
    if (stype == Qics::SelectionNone)
        setSelectionAnchorCell(0);

    // Set selection cell
    QicsICell *cell;

    if (stype == Qics::SelectionNone)
        cell = 0;
    else
        cell = new QicsICell(row, col);

    setSelectionCurrentCell(cell);

    gridInfo().reportSelection(stype, anchor_row, anchor_col, row, col);
#ifdef notdef

    qDebug("Grid SELECTION: (%d) (%d, %d) to (%d, %d)\n", stype,
        anchor_row, anchor_col, row, col);
#endif
}

void QicsTableGrid::setSelectionAnchorCell(QicsICell *cell)
{
    delete m_selectionAnchorCell;
    m_selectionAnchorCell = cell;
}

void QicsTableGrid::setSelectionCurrentCell(QicsICell *cell)
{
    delete m_selectionCurrentCell;
    m_selectionCurrentCell = cell;
}

//////////////////////////////////////////////////////////////////////
/////////////     Entry Widget methods                 ///////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
////////////         Slots Methods                     ///////////////
//////////////////////////////////////////////////////////////////////

void QicsTableGrid::handleGridPropertyChange(QicsGridStyle::QicsGridStyleProperty prop)
{
    switch (prop)
    {
    case QicsGridStyle::ClickToEdit:
    case QicsGridStyle::AutoSelectCellContents:
    case QicsGridStyle::EnterTraversalDirection:
    case QicsGridStyle::TabTraversalDirection:
        // Don't need to do anything for these
        break;

    default:
        QicsScreenGrid::handleGridPropertyChange(prop);
    }
}

QRect QicsTableGrid::drawRegion(const QicsRegion &region, QPainter *painter, QicsGrid::DrawCellMode mode)
{
    QRect painted_rect = QicsGrid::drawRegion(region, painter, mode);

    if (mode & QicsGrid::CellOnly && !isPrinterDevice(painter, this)) {

        if (m_mainGrid->selectionStyle() == Qics::Normal)
            return painted_rect;

        QicsSelection *s = const_cast<QicsSelection*>(gridInfo().selectionManager()->exclusiveSelection());
        m_exclusiveMode = s;
        QicsDataModel *dm = gridInfo().dataModel();

        if (!s)
            return painted_rect;

        int exSelLeft = s->leftColumn();
        int exSelTop = s->topRow();
        int exSelRight = s->rightColumn();
        int exSelBottom = s->bottomRow();
        const bool isEntireColumn = s->isEntireColumn() || (!exSelTop && exSelBottom == dm->lastRow());
        const bool isEntireRow = s->isEntireRow() || (!exSelLeft && exSelRight == dm->lastColumn());

        // if whole table is selected, no further drawing
        if (isEntireColumn && isEntireRow) {
            m_fillHandlePickRect = QRect();
            return painted_rect;
        }

        QicsRegion reg;
        if (styleManager().spanManager()->insideSpan(m_info, exSelBottom, exSelRight, reg)) {
            exSelBottom = reg.endRow();
            exSelRight = reg.endColumn();
        }

        if (exSelRight == Qics::QicsLAST_COLUMN)
            exSelRight = currentViewport().right();
        else
            if (exSelRight != exSelLeft)
                exSelRight = m_info.lastNonHiddenColumn(exSelLeft, exSelRight);

        if (exSelBottom == Qics::QicsLAST_ROW)
            exSelBottom = currentViewport().bottom();
        else
            if (exSelTop != exSelBottom)
                exSelBottom = m_info.lastNonHiddenRow(exSelTop, exSelBottom);

        if (exSelRight != exSelLeft)
            exSelLeft = m_info.firstNonHiddenColumn(exSelLeft, exSelRight);

        if (exSelTop != exSelBottom)
            exSelTop = m_info.firstNonHiddenRow(exSelTop, exSelBottom);

        // intercept filling selection with viewport
        reg = QicsRegion(exSelTop, exSelLeft, exSelBottom, exSelRight);
        reg &= screenViewport();
        if (!reg.isValid())
            return painted_rect;

        // convert start of the region to screen coordinates
        QRect rect = cellDimensions(reg.startCell(), false);

        rect.setWidth(mappedDM().regionWidth(reg));
        rect.setHeight(mappedDM().regionHeight(reg));

        // move unvisible corner off-screen
        if (exSelTop < myRowPositions.firstIndex())
            rect.setTop(-10);
        if (exSelLeft < myColumnPositions.firstIndex())
            rect.setLeft(-10);

        // now, draw it
        int hw = m_mainGrid->exclusiveSelectionBorderWidth();
        QPen p(m_mainGrid->exclusiveSelectionBorderColor());
        p.setWidth(hw);
        painter->setPen(p);

        const int offset = hw / 2;
        QRect r;

        if (m_mainGrid->selectionStyle() == Qics::Exclusive) {
            rect.adjust(offset, offset, -offset-(hw%2), -offset-(hw%2));
            r = QRect(rect.bottomRight(), rect.bottomRight());
            m_fillHandlePickRect = r.adjusted(-8, -8, 4, 4);
            r.adjust(-5, -5, -1, -1);
        }
        else {
            rect.adjust(-offset, -offset, offset-(hw%2), offset-(hw%2));

            if (isEntireColumn) {
                r = QRect(rect.topRight(), rect.topRight());
                m_fillHandlePickRect = r.adjusted(-8, -8, 4, 4);
                r.adjust(-3, 0, 2, 5);
            }
            else if (isEntireRow) {
                r = QRect(rect.bottomLeft(), rect.bottomLeft());
                m_fillHandlePickRect = r.adjusted(-8, -8, 4, 4);
                r.adjust(0, -3, 5, 2);
            }
            else {
                r = QRect(rect.bottomRight(), rect.bottomRight());
                m_fillHandlePickRect = r.adjusted(-8, -8, 4, 4);
                r.adjust(-3, -3, 2, 2);
            }
        }

        painter->drawRect(rect);

        if (!m_mainGrid->fillHandleVisible())
            return painted_rect;

        QColor c;
        c = m_mainGrid->exclusiveSelectionHandleBackColor();
        painter->setPen(c);
        painter->setBrush(c);
        painter->drawRect(r);

        r.adjust(1,1,0,0);

        c = m_mainGrid->exclusiveSelectionHandleColor();
        painter->setPen(c);
        painter->setBrush(c);
        painter->drawRect(r);

        // drop filling
        painter->setBrush(Qt::NoBrush);
    }

    return painted_rect;
}

void QicsTableGrid::doScrollTraverse()
{
    if (m_exclusiveDrag) {
        makeCellFullyVisible(cellToScroll.row(), cellToScroll.column());
        return;
    }

    if (gridInfo().selectionManager()->canSelectMultiple()) {
        if (!traverseToCell(cellToScroll.row(), cellToScroll.column(), false))
            setCurrentCell(QicsICell());
    }

    dragSelection(cellToScroll.row(), cellToScroll.column());
}


