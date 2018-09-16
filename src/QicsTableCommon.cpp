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

#include "QicsTableCommon.h"

#include <QMetaType>
#include <QValidator>

#include "QicsPasteValidator.h"
#include "QicsStyleManager.h"
#include "QicsDimensionManager.h"
#include "QicsRepeatingRow.h"
#include "QicsRepeatingColumn.h"
#include "QicsMainGrid.h"
#include "QicsRowHeader.h"
#include "QicsColumnHeader.h"
#include "QicsCell.h"
#include "QicsRow.h"
#include "QicsColumn.h"
#include "QicsSelection.h"
#include "QicsSelectionManager.h"
#include "QicsTableRegionDrag.h"


QicsTableCommon::QicsTableCommon(QObject *parent, bool forward_signals)
    : QicsGridCommon(parent, forward_signals),
      myMainGridInfo(TableGrid),
      myRowHeaderGridInfo(RowHeaderGrid),
      myColumnHeaderGridInfo(ColumnHeaderGrid),
      myRHDataModel(0),
      myCHDataModel(0),
      myCutCopySelection(0),
      myCell(0),
      myRow(0),
      myColumn(0),
      myRepeatingRow(0),
      myRepeatingColumn(0),
      myMainGrid(0),
      myRowHeader(0),
      myColumnHeader(0),
      myTopHeaderVisible(DisplayAlways),
      myBottomHeaderVisible(DisplayNever),
      myLeftHeaderVisible(DisplayAlways),
      myRightHeaderVisible(DisplayNever),
      myTableMargin(2),
      myTableSpacing(2),
      myGridSpacing(2),
      myRowHeaderUsesModel(false),
      myColumnHeaderUsesModel(false),
      myIsOneCellMoving(false)
{
    setInfo(&gridInfo());
    registerMetaTypes();
}

void QicsTableCommon::registerMetaTypes()
{
    qRegisterMetaType<QicsSortMode> ("QicsSortMode");
    qRegisterMetaType<QicsTraversalKeyEditBehavior > ("QicsTraversalKeyEditBehavior");
    qRegisterMetaType<QicsTableDisplayOption>("QicsTableDisplayOption");
    qRegisterMetaType<QicsSortOrder>("QicsSortOrder");
    qRegisterMetaType<QicsSelectionType>("QicsSelectionType");
    qRegisterMetaType<QicsSelectionPolicy>("QicsSelectionPolicy");
    qRegisterMetaType<Qics::QicsScrollDirection>("QicsScrollDirection");
    qRegisterMetaType<QicsScrollBarMode>("QicsScrollBarMode");
    qRegisterMetaType<QicsRepaintBehavior>("QicsRepaintBehavior");
    qRegisterMetaType<QicsLineStyle>("QicsLineStyle");
    qRegisterMetaType<QicsIndexType>("QicsIndexType");
    qRegisterMetaType<Qics::QicsHeaderType>("QicsHeaderType");
    qRegisterMetaType<QicsGridType>("QicsGridType");
    qRegisterMetaType<QicsGridCellClipping>("QicsGridCellClipping");
    qRegisterMetaType<QicsCurrentCellStyle>("QicsCurrentCellStyle");
    qRegisterMetaType<QicsCellOverflowBehavior>("QicsCellOverflowBehavior");
    qRegisterMetaType<QicsSortMode>("QicsSortMode");
    qRegisterMetaType<QicsSortMode>("QicsSortMode");
    qRegisterMetaType<QValidator*>("QValidatorStar");
    qRegisterMetaType<QicsPasteValidator*>("QicsPasteValidatorStar");
    qRegisterMetaType<QicsCurrentCellDraggingPolicy>("QicsCurrentCellDraggingPolicy");
    qRegisterMetaType<QicsSelectionStyle>("QicsSelectionStyle");
}

QicsTableCommon::~QicsTableCommon()
{
    delete myCell;
    delete myRow;
    delete myColumn;
    delete myMainGrid;
    delete myRowHeader;
    delete myColumnHeader;

    myCell = 0;
    myRow = 0;
    myColumn = 0;
    myMainGrid = 0;
    myRowHeader = 0;
    myColumnHeader = 0;
}

QicsDataModel *QicsTableCommon::dataModel() const
{
    return gridInfo().dataModel();
}

void QicsTableCommon::setDataModel(QicsDataModel *dm)
{
    gridInfo().setDataModel(dm);
}

void QicsTableCommon::setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val)
{
    gridInfo().styleManager()->setDefaultProperty(attr, val);
}

void QicsTableCommon::setGridAttr(QicsGridStyle::QicsGridStyleProperty attr,
                             const void *val)
{
    gridInfo().styleManager()->setGridProperty(attr, val);
}

void QicsTableCommon::clearGridAttr(QicsGridStyle::QicsGridStyleProperty attr)
{
    gridInfo().styleManager()->setGridProperty(attr, static_cast<void *> (0));
}

////////////////////////////////////////////////////////////////////
///////////         Row, Column, Cell methods         //////////////
////////////////////////////////////////////////////////////////////

QicsRow &QicsTableCommon::rowRef(int rownum)
{
    if (!myRow)
        myRow = new QicsRow(rownum, &gridInfo(), true, this);
    else
        myRow->setRowIndex(rownum);

    return (*myRow);
}

const QicsRow &QicsTableCommon::rowRef(int rownum) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->rowRef(rownum));
}

QicsRow *QicsTableCommon::row(int rownum, bool follow_model)
{
    return (new QicsRow(rownum, &gridInfo(), follow_model, this));
}

const QicsRow *QicsTableCommon::row(int rownum, bool follow_model) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->row(rownum, follow_model));
}

QicsColumn &QicsTableCommon::columnRef(int colnum)
{
    if (!myColumn)
        myColumn = new QicsColumn(colnum, &gridInfo(), true, this);
    else
        myColumn->setColumnIndex(colnum);

    return (*myColumn);
}

const QicsColumn &QicsTableCommon::columnRef(int colnum) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->columnRef(colnum));
}

QicsColumn *QicsTableCommon::column(int colnum, bool follow_model)
{
    return (new QicsColumn(colnum, &gridInfo(), follow_model, this));
}

const QicsColumn *QicsTableCommon::column(int colnum, bool follow_model) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->column(colnum, follow_model));
}

QicsRepeatingRow &QicsTableCommon::repeatingRowRef(int start_row, unsigned int interval)
{
    if (!myRepeatingRow)
        myRepeatingRow = new QicsRepeatingRow(start_row, interval,
        &gridInfo(), this);
    else {
        myRepeatingRow->setStartRowIndex(start_row);
        myRepeatingRow->setInterval(interval);
    }

    return (*myRepeatingRow);
}

const QicsRepeatingRow &QicsTableCommon::repeatingRowRef(int start_row, unsigned int interval) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->repeatingRowRef(start_row, interval));
}

QicsRepeatingRow *QicsTableCommon::repeatingRow(int start_row, unsigned int interval)
{
    return (new QicsRepeatingRow(start_row, interval, &gridInfo(), this));
}

const QicsRepeatingRow *QicsTableCommon::repeatingRow(int start_row, unsigned int interval) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->repeatingRow(start_row, interval));
}

QicsRepeatingColumn &QicsTableCommon::repeatingColumnRef(int start_col,
                                    unsigned int interval)
{
    if (!myRepeatingColumn)
        myRepeatingColumn = new QicsRepeatingColumn(start_col, interval,
        &gridInfo(), this);
    else {
        myRepeatingColumn->setStartColumnIndex(start_col);
        myRepeatingColumn->setInterval(interval);
    }

    return (*myRepeatingColumn);
}

const QicsRepeatingColumn &QicsTableCommon::repeatingColumnRef(int start_col,
                                    unsigned int interval) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->repeatingColumnRef(start_col, interval));
}

QicsRepeatingColumn *QicsTableCommon::repeatingColumn(int start_col,
                                 unsigned int interval)
{
    return (new QicsRepeatingColumn(start_col, interval,
        &gridInfo(), this));
}

const QicsRepeatingColumn *QicsTableCommon::repeatingColumn(int start_col,
                                 unsigned int interval) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->repeatingColumn(start_col, interval));
}

QicsCell &QicsTableCommon::cellRef(int rownum, int colnum)
{
    if (!myCell)
        myCell = new QicsCell(rownum, colnum, &gridInfo(), true, this);
    else {
        myCell->setRowIndex(rownum);
        myCell->setColumnIndex(colnum);
    }

    return (*myCell);
}

const QicsCell &QicsTableCommon::cellRef(int rownum, int colnum) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->cellRef(rownum, colnum));
}

QicsCell *QicsTableCommon::cell(int rownum, int colnum, bool follow_model)
{
    return (new QicsCell(rownum, colnum, &gridInfo(), follow_model, this));
}

const QicsCell *QicsTableCommon::cell(int rownum, int colnum, bool follow_model) const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->cell(rownum, colnum, follow_model));
}

QicsMainGrid &QicsTableCommon::mainGridRef()
{
    if (!myMainGrid)
        myMainGrid = new QicsMainGrid(&gridInfo(), this);

    return (*myMainGrid);
}

const QicsMainGrid &QicsTableCommon::mainGridRef() const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->mainGridRef());
}

QicsMainGrid *QicsTableCommon::mainGrid()
{
    return (new QicsMainGrid(&gridInfo(), this));
}

const QicsMainGrid *QicsTableCommon::mainGrid() const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->mainGrid());
}

QicsRowHeader &QicsTableCommon::rowHeaderRef()
{
    if (!myRowHeader)
        myRowHeader = rowHeader();

    return (*myRowHeader);
}

const QicsRowHeader &QicsTableCommon::rowHeaderRef() const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->rowHeaderRef());
}

QicsRowHeader *QicsTableCommon::rowHeader()
{
    return (new QicsRowHeader(&rhGridInfo(), this, true));
}

const QicsRowHeader *QicsTableCommon::rowHeader() const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->rowHeader());
}

QicsColumnHeader &QicsTableCommon::columnHeaderRef()
{
    if (!myColumnHeader)
        myColumnHeader = columnHeader();

    return (*myColumnHeader);
}

const QicsColumnHeader &QicsTableCommon::columnHeaderRef() const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->columnHeaderRef());
}

QicsColumnHeader *QicsTableCommon::columnHeader()
{
    return (new QicsColumnHeader(&chGridInfo(), this, true));
}

const QicsColumnHeader *QicsTableCommon::columnHeader() const
{
    QicsTableCommon *self = const_cast<QicsTableCommon *> (this);

    return (self->columnHeader());
}

void QicsTableCommon::sortRows(const QVector<int> &columns, QicsSortOrder order,
                          int from, int to,
                          DataItemComparator func)
{
    gridInfo().orderRowsBy(columns, order, from, to, func);
}

void QicsTableCommon::sortColumns(const QVector<int> &rows, QicsSortOrder order,
                             int from, int to,
                             DataItemComparator func)
{
    gridInfo().orderColumnsBy(rows, order, from, to, func);
}

void QicsTableCommon::moveRows(int target_row, const QVector<int> &rows)
{
    gridInfo().moveRows(target_row, rows);
}

void QicsTableCommon::moveColumns(int target_col, const QVector<int> &cols)
{
    gridInfo().moveColumns(target_col, cols);
}

void QicsTableCommon::addRows(int how_many)
{
    gridInfo().insertRows(-1, how_many);

    //while(how_many-- > 0) gridInfo().insertRow(-1);
}

void QicsTableCommon::insertRow(int row)
{
    gridInfo().insertRow(row);
}

void QicsTableCommon::deleteRow(int row)
{
    gridInfo().deleteRows(1, row);
}

void QicsTableCommon::deleteRows(int num, int start_position)
{
    gridInfo().deleteRows(num, start_position);
}

void QicsTableCommon::addColumns(int how_many)
{
    gridInfo().insertColumns(-1, how_many);

    //while(how_many-- > 0) gridInfo().insertColumn(-1);
}

void QicsTableCommon::insertColumn(int col)
{
    gridInfo().insertColumn(col);
}

void QicsTableCommon::deleteColumn(int col)
{
    gridInfo().deleteColumns(1, col);
}

void QicsTableCommon::deleteColumns(int num, int start_position)
{
    gridInfo().deleteColumns(num, start_position);
}

void QicsTableCommon::setRowHeaderUsesModel(bool b)
{
    // check if we should do something at all
    if (myRowHeaderUsesModel == b)
        return;

    myRowHeaderUsesModel = b;
    QicsRegion vp = viewport();

    if (b) {
        // If we are currently showing column 0, don't show it anymore
        if (vp.startColumn() == 0) {
            vp.setStartColumn(1);
            setViewport(vp);

            QicsRegion rvp = columnHeaderRef().viewport();
            rvp.setStartColumn(1);
            columnHeaderRef().setViewport(rvp);
        }

        rhGridInfo().setDataModel(dataModel());

        QicsRegion rvp = rowHeaderRef().viewport();
        rvp.setStartColumn(0);
        rvp.setEndColumn(0);
        rowHeaderRef().setViewport(rvp);
    }
    else {
        // If we are currently not showing column 0, show it now
        if (vp.startColumn() == 1) {
            vp.setStartColumn(0);
            setViewport(vp);

            QicsRegion rvp = columnHeaderRef().viewport();
            rvp.setStartColumn(0);
            columnHeaderRef().setViewport(rvp);
        }

        rhGridInfo().setDataModel(myRHDataModel);

        QicsRegion rvp = rowHeaderRef().viewport();
        rvp.setStartColumn(0);
        rvp.setEndColumn(QicsLAST_COLUMN);
        rowHeaderRef().setViewport(rvp);
    }

    // set rows order owner in the end
    rhGridInfo().controlRowsOf(&gridInfo());
}

void QicsTableCommon::setColumnHeaderUsesModel(bool b)
{
    // check if we should do something at all
    if (myColumnHeaderUsesModel == b)
        return;

    myColumnHeaderUsesModel = b;
    QicsRegion vp = viewport();

    if (b) {
        // If we are currently showing row 0, don't show it anymore
        if (vp.startRow() == 0) {
            vp.setStartRow(1);
            setViewport(vp);
        }

        chGridInfo().setDataModel(dataModel());

        QicsRegion cvp = columnHeaderRef().viewport();
        cvp.setStartRow(0);
        cvp.setEndRow(0);
        columnHeaderRef().setViewport(cvp);
    }
    else {
        // If we are currently not showing row 0, show it now
        if (vp.startRow() == 1) {
            vp.setStartRow(0);
            setViewport(vp);
        }

        chGridInfo().setDataModel(myCHDataModel);

        QicsRegion cvp = columnHeaderRef().viewport();
        cvp.setStartRow(0);
        cvp.setEndRow(QicsLAST_ROW);
        columnHeaderRef().setViewport(cvp);
    }

    // set columns order owner in the end
    chGridInfo().controlColumnsOf(&gridInfo());
}

void QicsTableCommon::setDefaultRowHeightInPixels(int height)
{
    gridInfo().dimensionManager()->setDefaultRowHeightInPixels(height);
}

void QicsTableCommon::setDefaultRowHeightInChars(int height)
{
    gridInfo().dimensionManager()->setDefaultRowHeightInChars(height);
}

void QicsTableCommon::setDefaultColumnWidthInPixels(int width)
{
    gridInfo().dimensionManager()->setDefaultColumnWidthInPixels(width);
}

void QicsTableCommon::setDefaultColumnWidthInChars(int width)
{
    gridInfo().dimensionManager()->setDefaultColumnWidthInChars(width);
}

bool QicsTableCommon::ignoreModelSizeChanges() const
{
    return gridInfo().ignoreModelSizeChanges();
}

void QicsTableCommon::setIgnoreModelSizeChanges(bool b)
{
    gridInfo().setIgnoreModelSizeChanges(b);
    rhGridInfo().setIgnoreModelSizeChanges(b);
    chGridInfo().setIgnoreModelSizeChanges(b);
}

bool QicsTableCommon::setMouseButton( Qt::MouseButton realButton,
                                     Qt::Modifier mod,
                                     Qt::MouseButton emulButton)
{
    if (mainGridRef().setMouseButton( realButton, mod, emulButton ))
        return true;
    return false;
}

void QicsTableCommon::setSortingSensitivity(Qt::CaseSensitivity cs)
{
    gridInfo().setSortingSensitivity(cs);
}

////////////////////////////////////////////////////////////////////////
//////////////////  Clipboard / Drag and Drop Methods  /////////////////
////////////////////////////////////////////////////////////////////////

QMimeData *QicsTableCommon::cutCopyData(QWidget *widget, QicsICell *ref_cell, bool drag_single)
{
    myCutCopySelection = 0;

    if(ref_cell && *ref_cell == gridInfo().currentCell(true) && drag_single) {
        myIsOneCellMoving = true;
        QicsSelectionList * one_cell_list = new QicsSelectionList(dataModel());
        one_cell_list->append( QicsSelection(ref_cell->row(),ref_cell->column(),
            ref_cell->row(),ref_cell->column(),true));
        myCutCopySelection = one_cell_list;
    }
    else {
        myCutCopySelection = cutCopySelection(ref_cell);
        myIsOneCellMoving = false;
    }

    if (myCutCopySelection && myCutCopySelection->size() > 0)
        return QicsTableRegionDrag::getDragObject(this, myCutCopySelection, ref_cell, widget);

    return 0;
}

QicsSelectionList *QicsTableCommon::cutCopySelection(const QicsICell *ref_cell)
{
    QicsSelectionList *return_list = 0;

    QicsSelectionList *slist = gridInfo().selectionManager()->selectionList();

    if (ref_cell && slist) {
        /*
        * We only grab the part of the selection list that contains ref_cell
        *
        * This is simply because we can't think of any good semantics
        * right now for dragging discontiguous regions into a cell, or
        * into anywhere for that matter.  We need some API for the
        * application to make sense of things like this.
        *
        * TODO: provide that API for extended selections
        *
        */

        QicsSelectionList::const_iterator iter, iter_end(slist->constEnd());

        for (iter = slist->constBegin(); iter != iter_end; ++iter) {
            const QicsSelection &sel = *iter;

            QicsRegion reg(sel.topRow(), sel.leftColumn(),
                sel.bottomRow(), sel.rightColumn());

            if (reg.contains(*ref_cell)) {
                return_list = new QicsSelectionList(dataModel());

                return_list->push_back(QicsSelection(reg.startRow(),
                    reg.startColumn(),
                    reg.endRow(),
                    reg.endColumn()));
                break;
            }
        }

        delete slist;
    }
    else {
        //
        // We take the entire selection list
        //

        return_list = slist;
    }

    return (return_list);
}

void QicsTableCommon::finishCut(bool remove_data)
{
    myCutCopySelection = gridInfo().selectionManager()->selectionList(true);

    if (!myCutCopySelection || !gridInfo().dataModel())
        return;

    if (remove_data) {
        if (myIsOneCellMoving) {
            if (gridInfo().clearPolicy() & Qics::ClearData) {
                QicsICell currentCell = gridInfo().currentCell(true);
                gridInfo().dataModel()->clearItem(currentCell.row(), currentCell.column());
            }
            return;
        }

        deleteSelected();
    }
}

void QicsTableCommon::deleteSelected()
{
    QicsDataModel* data_model = gridInfo().dataModel();
    if (!data_model)
        return;

    // if selection is present - delete all the cell content
    QicsSelectionList *list = gridInfo().selectionManager()->selectionList(true);
    if (list && list->count())  {
        bool es = data_model->emitSignals();
        data_model->setEmitSignals(false);

        int mrmin = INT_MAX, mrmax = INT_MIN, mcmin = INT_MAX, mcmax = INT_MIN;
        int mr, mc;
        QicsRegion reg;
        QicsCellStyle *cs = gridInfo().styleManager()->getDefaultCellStyle();

        foreach(QicsSelection sel, *list) {
            int bottomRow = sel.bottomRow();
            int rightColumn = sel.rightColumn();
            if (sel.bottomRow() == QicsLAST_ROW) bottomRow = data_model->numRows();
            if (sel.rightColumn() == QicsLAST_COLUMN) rightColumn = data_model->numColumns();
            for (int r = sel.topRow(); r <= bottomRow; r++)
                for (int c = sel.leftColumn(); c <= rightColumn; c++) {
                    mr = gridInfo().modelRowIndex(r);
                    mc = gridInfo().modelColumnIndex(c);
                    if (mr < mrmin) mrmin = mr;
                    if (mr > mrmax) mrmax = mr;
                    if (mc < mcmin) mcmin = mc;
                    if (mc > mcmax) mcmax = mc;

                    if (*static_cast<bool*>(gridInfo().styleManager()->getCellProperty(mr, mc, QicsCellStyle::ReadOnly)))
                        continue;

                    if (!(*static_cast<bool*>(gridInfo().styleManager()->getCellProperty(mr, mc, QicsCellStyle::Enabled))))
                        continue;

                    // remove data
                    if (gridInfo().clearPolicy() & Qics::ClearData)
                        data_model->clearItem(mr, mc);

                    // remove attr
                    if (gridInfo().clearPolicy() & Qics::ClearAttributes)
                        gridInfo().styleManager()->setCellStyle(mr, mc, cs);

                    // remove spans
                    if (gridInfo().clearPolicy() & Qics::ClearSpans)
                        if (gridInfo().styleManager()->spanManager()->isSpanner(gridInfo(), mr, mc, reg))
                            gridInfo().styleManager()->spanManager()->removeCellSpan(mr, mc);
                }
        }

        data_model->setEmitSignals(es);
        gridInfo().redrawAllGrids();
        data_model->notifyRegionChanged(QicsRegion(mrmin, mcmin, mrmax, mcmax));
    }
}

void QicsTableCommon::paste(const QMimeData *md, const QicsICell &cell)
{
    QicsDataModel* data_model = gridInfo().dataModel();
    if (!data_model)
        return;

    const bool es = data_model->emitSignals();
    data_model->setEmitSignals(false);

    QicsDataModelDefault tmp_dm;
    QString text;
    QicsICell cur_cell = gridInfo().currentCell();

    // data
    if (QicsTableRegionDrag::decode(md, tmp_dm))
        cur_cell = overlay(tmp_dm, cell, true);
    else if (md->hasText() && (gridInfo().copyPolicy() & Qics::CopyData)) {
        text = md->text();
        QicsDataString itm(text);

        data_model->setItem(gridInfo().modelRowIndex(cell.row()), gridInfo().modelColumnIndex(cell.column()), itm);
    }

    // attributes etc.
    QicsTableRegionDrag::decode(md, gridInfo(), cur_cell);

    data_model->setEmitSignals(es);

    gridInfo().redrawAllGrids();
}

QicsICell QicsTableCommon::overlay(const QicsDataModel &dm, const QicsICell &start_cell, bool expand_model, bool clear_if_empty)
{
    QicsDataModel* data_model = gridInfo().dataModel();
    if (!data_model)
        return start_cell;

    // #### AM: this does not work in the case if model rows/cols were changed
    QicsICell new_start_cell(start_cell);
    const int pastedNumColumns = dm.numColumns();
    const int pastedNumRows = dm.numRows();
    const int numColumns = data_model->numColumns();
    const int numRows = data_model->numRows();

    if (pastedNumColumns == numColumns) {
        new_start_cell.setColumn(0);
        clear_if_empty = false;
    }
    if (pastedNumRows == numRows) {
        new_start_cell.setRow(0);
        clear_if_empty = false;
    }

    if (gridInfo().copyPolicy() & Qics::CopyData) {
        int view_row = new_start_cell.row();
        for (int i = 0; i < pastedNumRows; ++i) {
            view_row = gridInfo().firstNonHiddenRow(view_row, numRows);
            if (!expand_model && (view_row >= numRows))
                break;

            int view_col = new_start_cell.column();
            for (int j = 0; j < pastedNumColumns; ++j) {
                view_col = gridInfo().firstNonHiddenColumn(view_col, numColumns);
                if (!expand_model && (view_col >= numColumns))
                    break;

                const QicsDataItem *itm = dm.item(i, j);

                if (itm) {
                    data_model->setItem(gridInfo().modelRowIndex(view_row), gridInfo().modelColumnIndex(view_col), *itm);
                }
                else if (clear_if_empty) {
                    data_model->clearItem(gridInfo().modelRowIndex(view_row), gridInfo().modelColumnIndex(view_col));
                }
                ++view_col;
            }
            ++view_row;
        }
    }

    return new_start_cell;
}
