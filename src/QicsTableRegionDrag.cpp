/*********************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable Product.
**
** This is commericially licensed software. Please see the file LICENSE,
** included in the toplevel directory of this distribution, for the
** license agreement. Do not use this software if you are not willing
** to be bound by this agreement.
**
** See http://www.ics.com/qt/licenses/commericial or email sales@ics.com for
** information about ICS Commercial License Agreements.
**
** Contact info@ics.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "QicsTableRegionDrag.h"

#include <QStringList>
#include <QApplication>
#include "QicsGridInfo.h"
#include "QicsDataModelDefault.h"
#include "QicsCellStyle.h"
#include "QicsStyleManager.h"
#include "QicsDimensionManager.h"
#include "QicsSelectionManager.h"
#include "QicsSpanManager.h"
#include "QicsTableCommon.h"
#include "QicsRowHeader.h"
#include "QicsColumnHeader.h"
#include "QicsDataItemFormatter.h"
#include "QicsCell.h"
#include "QicsColumn.h"


QicsTableRegionDrag::QicsTableRegionDrag(QicsTableCommon *tableCommon, const QicsSelectionList *slist, QWidget *)
{
    QicsSelectionList mySelectionList(*slist);
    const QicsGridInfo &gi = tableCommon->gridInfo();
    const QicsGridInfo &rgi = tableCommon->rhGridInfo();
    const QicsGridInfo &cgi = tableCommon->chGridInfo();

    // we need to determine the topmost row index and the leftmost
    // column index of the selection, as well as the max size of the selections

    int top = Qics::QicsLAST_ROW;
    int left = Qics::QicsLAST_COLUMN;
    Qics::QicsCopyPolicy copyPolicy = gi.copyPolicy();
    //int bottom = 0;
    //int right = 0;

    for (QicsSelectionList::iterator iter = mySelectionList.begin(); iter != mySelectionList.end(); ++iter) {
        QicsSelection &sel = *iter;

        if (sel.bottomRow() >= Qics::QicsLAST_ROW)
            sel.setEndRow(gi.dataModel()->lastRow());
        if (sel.rightColumn() >= Qics::QicsLAST_COLUMN)
            sel.setEndColumn(gi.dataModel()->lastColumn());

        if (sel.topRow() < top)
            top = sel.topRow();
        if (sel.leftColumn() < left)
            left = sel.leftColumn();
        //if (sel.bottomRow() > bottom)
        //    bottom = sel.bottomRow();
        //if (sel.rightColumn() > right)
         //   right = sel.rightColumn();
    }

    delete slist;

    // cell data
    QByteArray retval;
    QDataStream ds(&retval, QIODevice::WriteOnly);

    // cell attr
    QByteArray retval_a;
    QDataStream ds_a(&retval_a, QIODevice::WriteOnly);

    // cell size
    QByteArray retval_s;
    QDataStream ds_s(&retval_s, QIODevice::WriteOnly);
    QMap<int, int> rowSizes;
    QMap<int, int> colSizes;
    int ar = Qics::QicsLAST_ROW;
    int ac = Qics::QicsLAST_COLUMN;

    // cell spans
    QByteArray retval_span;
    QDataStream ds_span(&retval_span, QIODevice::WriteOnly);
    QicsSpanList *sl = gi.styleManager()->spanManager()->cellSpanList();
    QicsSpanList spans;

    // encode grid id - for attributes
    ds_a << (quintptr)&gi;

    // encode number of selections
    int size = mySelectionList.size();
    ds << size;
    ds_a << size;
    ds_span << size;

    // we want to encode the topmost row index and the leftmost
    // column index of the selection, because the decoder
    // may want to use this information, and it's much easier
    // to determine it here than in the decoder.

    // encode topmost row and leftmost column of the selections
    ds << top;
    ds << left;
    ds_a << top;
    ds_a << left;

    // now go through all the selections and copy the data

    for (QicsSelectionList::const_iterator iter = mySelectionList.constBegin(); iter != mySelectionList.constEnd(); ++iter) {
        const QicsSelection &sel = *iter;

        // encode number of rows and columns in the selection
        ds << sel.numRows();
        ds << sel.numColumns();
        ds_a << sel.numRows();
        ds_a << sel.numColumns();
        ds_span << sel.numRows();
        ds_span << sel.numColumns();

        // encode the starting cell of the selection
        ds << sel.topRow();
        ds << sel.leftColumn();
        ds_a << sel.topRow();
        ds_a << sel.leftColumn();
        ds_span << sel.topRow();
        ds_span << sel.leftColumn();

        // count selection anchor
        if (sel.topRow() < ar)
            ar = sel.topRow();
        if (sel.leftColumn() < ac)
            ac = sel.leftColumn();

        for (int i = sel.topRow(); i <= sel.bottomRow(); ++i) {
            int mr = gi.modelRowIndex(i);

            for (int j = sel.leftColumn(); j <= sel.rightColumn(); ++j) {

                int mc = gi.modelColumnIndex(j);

                const QicsDataItem *itm = gi.dataModel()->item(mr, mc);

                if (itm)
                    itm->encode(ds);
                else
                    ds << QString("empty");

                // cell attr
                QicsCellStyle *cs = gi.styleManager()->getCellStyle(mr, mc);
                if (cs) {
                    ds_a << (bool)true;
                    cs->encode(ds_a);
                } else
                    ds_a << (bool)false;

                //if (!rowSizes.contains(mr))
                //if (!colSizes.contains(mc))
                colSizes[mc] = gi.dimensionManager()->columnWidth(mc, true, false);
            }
            rowSizes[mr] = gi.dimensionManager()->rowHeight(mr, true, false);
        }

        // check if a span fits current selection
        QList<int> indexes; // to properly remove processed spans
        for (int i = 0; i < sl->count(); ++i) {
            // we suppose that selection must cover spanned cells totally
            const QicsSpan &span = sl->at(i);
            if (sel.contains(gi.visualRowIndex(gi.firstNonHiddenModelRow(span.row(), span.row() + span.height() - 1)), gi.visualColumnIndex(gi.firstNonHiddenModelColumn(span.column(), span.column() + span.width() - 1))))  {
                spans.append(span);
                indexes.prepend(i); // we're reversing order of processed spans
            }
        }
        for (int i = 0; i < indexes.count(); ++i)
            sl->remove(indexes.at(i));
    }

    setData(QICS_MIME_CELLDATA, retval);
    setData(QICS_MIME_CELLATTR, retval_a);

    // cell sizes
    ds_s << rowSizes.count();
    QMap<int,int>::const_iterator it, it_end(rowSizes.constEnd());

    for (it = rowSizes.constBegin(); it != it_end; ++it)
        ds_s << it.key() - ar << it.value();

    ds_s << colSizes.count();
    it_end = colSizes.constEnd();
    for (it = colSizes.constBegin(); it != it_end; ++it)
        ds_s << it.key() - ac << it.value();

    setData(QICS_MIME_CELLSIZE, retval_s);

    // cell spans

    ds_span << spans.count();
    for (int i = 0; i < spans.count(); ++i) {
        const QicsSpan &span = spans.at(i);
        //ds_span << span.row() - ar << span.column() - ac << span.width() << span.height();
        ds_span << gi.visualRowIndex(gi.firstNonHiddenModelRow(span.row(), span.row() + span.height() - 1)) - ar << gi.visualColumnIndex(gi.firstNonHiddenModelColumn(span.column(), span.column() + span.width() - 1)) - ac << span.width() << span.height();
    }
    delete sl;

    setData(QICS_MIME_CELLSPAN, retval_span);

    // cell data for Excel
    QString cliptext;
    QicsRegion selectedRegion = mySelectionList.region();
    QSet<int> columns;
    QSet<int> rows;

    for (int i = selectedRegion.top(); i <= selectedRegion.bottom(); ++i) {
        for (int j = selectedRegion.left(); j <= selectedRegion.right(); ++j) {
            if (mySelectionList.isCellSelected(i, j)) {
                rows.insert(i);
                columns.insert(j);
            }
        }
    }

    int rs = 0;
    for (int i = 0; i < rgi.dataModel()->numColumns(); ++i)
        if (!rgi.dimensionManager()->isColumnHidden(i))
            ++rs;

    // top header
    if ((copyPolicy & CopyTopHeaderData) && (tableCommon->topHeaderVisible() != DisplayNever)) {
        // content
        for (int i = 0; i < cgi.dataModel()->numRows(); ++i) {
            if (cgi.dimensionManager()->isRowHidden(i))
                continue;

            // dummy cell for left header if present
            if (tableCommon->leftHeaderVisible() != DisplayNever) {
                for (int i = 0; i < rs; ++i)
                    cliptext += "\t";
            }

            for (int j = selectedRegion.left(); j <= selectedRegion.right(); ++j) {
                if (tableCommon->columnRef(j).isHidden())
                    continue;
                if (!columns.contains(j))
                    continue;

                cliptext += exportCell(tableCommon->columnHeaderRef().cellRef(i, j));
            }

            // dummy cell for right header if present
            if (tableCommon->rightHeaderVisible() != DisplayNever) {
                for (int i = 0; i < rs; ++i)
                    cliptext += "\t";
            }

            cliptext += "\r\n";
        }
    }

    // content
    for (int i = selectedRegion.top(); i <= selectedRegion.bottom(); ++i) {
        if (!rows.contains(i))
            continue;

        // left header content
        if ((copyPolicy & CopyLeftHeaderData) && (tableCommon->leftHeaderVisible() != DisplayNever)) {
            for (int k = 0; k < rgi.dataModel()->numColumns(); ++k) {
                if (rgi.dimensionManager()->isColumnHidden(k))
                    continue;

                cliptext += exportCell(tableCommon->rowHeaderRef().cellRef(i, k));
            }
        }

        // table content
        for (int j = selectedRegion.left(); j <= selectedRegion.right(); ++j) {
            if (tableCommon->columnRef(j).isHidden())
                continue;
            if (!columns.contains(j))
                continue;

            cliptext += exportCell(tableCommon->cellRef(i, j));
        }

        // right header content
        if ((copyPolicy & CopyRightHeaderData) && (tableCommon->rightHeaderVisible() != DisplayNever)) {
            for (int k = 0; k < rgi.dataModel()->numColumns(); ++k) {
                if (rgi.dimensionManager()->isColumnHidden(k))
                    continue;

                cliptext += exportCell(tableCommon->rowHeaderRef().cellRef(i, k));
            }
        }

        cliptext += "\r\n";
    }

    // bottom header
    if ((copyPolicy & CopyBottomHeaderData) && (tableCommon->bottomHeaderVisible() != DisplayNever)) {
        // content
        for (int i = 0; i < cgi.dataModel()->numRows(); ++i) {
            if (cgi.dimensionManager()->isRowHidden(i))
                continue;

            // dummy cell for left header if present
            if (tableCommon->leftHeaderVisible() != DisplayNever) {
                for (int i = 0; i < rs; ++i)
                    cliptext += "\t";
            }

            for (int j = selectedRegion.left(); j <= selectedRegion.right(); ++j) {
                if (tableCommon->columnRef(j).isHidden())
                    continue;
                if (!columns.contains(j))
                    continue;

                cliptext += exportCell(tableCommon->columnHeaderRef().cellRef(i, j));
            }

            // dummy cell for right header if present
            if (tableCommon->rightHeaderVisible() != DisplayNever) {
                for (int i = 0; i < rs; ++i)
                    cliptext += "\t";
            }

            cliptext += "\r\n";
        }
    }

    setData("text/plain", cliptext.toUtf8());
}

QString QicsTableRegionDrag::exportCell(const QicsCell &cell)
{
    const int row = cell.rowIndex();
    const int col = cell.columnIndex();
    const QicsGridInfo &gi = cell.gridInfo();
    QicsSpanManager *spm = cell.styleManager().spanManager();
    QicsRegion r;

    const bool spanner = spm->isSpanner(gi, row, col, r);

    if (!spanner && spm->insideSpan(gi, row, col, r))
        return "\t";

    // text content
    const QicsDataItem *item = gi.cellValue(row, col);
    QString text = cell.label();

    if (text.isEmpty() && item) {
        QicsDataItemFormatter *formatter = cell.formatter();

        if (formatter)
            text = formatter->format(*item);
        else
            text = item->string();
    }

    return text + "\t";
}

QicsTableRegionDrag *QicsTableRegionDrag::getDragObject(QicsTableCommon *tableCommon, const QicsSelectionList *slist,
                                   QicsICell *, QWidget *dragSource)
{
    return new QicsTableRegionDrag(tableCommon, slist, dragSource);
}

/*!
Returns \b true if \a e can be decoded by the QicsTableRegionDrag, otherwise
return \b false.
*/
bool QicsTableRegionDrag::canDecode(const QMimeData* mimedata)
{
    return mimedata->hasFormat(QICS_MIME_CELLDATA) || mimedata->hasFormat(QICS_MIME_CELLATTR)
            || mimedata->hasFormat(QICS_MIME_CELLSIZE) || mimedata->hasFormat(QICS_MIME_CELLSPAN)
            || mimedata->hasFormat("text/plain");
}

bool QicsTableRegionDrag::decode(const QMimeData *dataIn, QicsDataModel &dm)
{
    bool ret = false;

    if (dataIn->hasFormat(QICS_MIME_CELLDATA)) {
        QByteArray ba = dataIn->data(QICS_MIME_CELLDATA);
        QDataStream ds(&ba, QIODevice::ReadOnly);

        // get the number of selections
        int nsels;
        ds >> nsels;

        // get the topmost row and leftmost column of the selections
        int top, left;
        ds >> top;
        ds >> left;
        for (int seln = 0; seln < nsels; ++seln) {
            // get the number of rows and columns
            int nrows, ncols;
            ds >> nrows;
            ds >> ncols;

            // get the starting cell of the selection
            int srow, scol;
            ds >> srow;
            ds >> scol;

            // make sure the data model is big enough

            int dm_row_index = srow - top;
            int dm_col_index = scol - left;

            int needed_rows = (dm_row_index + nrows);
            if (needed_rows > dm.numRows())
                dm.addRows(needed_rows - dm.numRows());

            int needed_cols = (dm_col_index + ncols);
            if (needed_cols > dm.numColumns())
                dm.addColumns(needed_cols - dm.numColumns());

            // go through each cell and set the value in the data model
            for (int i = 0; i < nrows; ++i) {
                for (int j = 0; j < ncols; ++j) {
                    QicsDataItem *itm = QicsDataItem::decode(ds);

                    if (itm)
                        dm.setItem(dm_row_index + i, dm_col_index + j, *itm);
                    else
                        dm.clearItem(dm_row_index + i, dm_col_index + j);

                    delete itm;
                }
            }
        }

        ret = true;
    }
    else if (dataIn->hasFormat("text/plain")) {
        QString input = dataIn->text().remove("\r");
        QStringList lines = input.split("\n");
        if (lines.count() > 0 && lines.last().isEmpty())
            lines.removeLast();

        if (dm.numRows() < lines.count())
            dm.addRows(lines.count() - dm.numRows());

        for (int row = 0; row < lines.count(); ++row) {
            QStringList items = lines.at(row).split("\t");

            if (dm.numColumns() < items.count())
                dm.addColumns(items.count() - dm.numColumns());

            for (int col = 0; col < items.count(); ++col) {
                QicsDataItem *itm = QicsDataItem::fromString(items.at(col));

                if (itm) {
                    dm.setItem(row, col, *itm);
                    delete itm;
                } else
                    dm.clearItem(row, col);
            }
        }

        ret = true;
    }

    return ret;
}

bool QicsTableRegionDrag::decode(const QMimeData *dataIn, const QicsGridInfo &gi, QicsICell &cell)
{
    bool ret = false;

    int rcell = cell.row();
    int ccell = cell.column();

    int myCopyPolicy = gi.copyPolicy();

    if (dataIn->hasFormat(QICS_MIME_CELLATTR) && (myCopyPolicy & Qics::CopyAttributes)) {
        QByteArray ba = dataIn->data(QICS_MIME_CELLATTR);
        QDataStream ds(&ba, QIODevice::ReadOnly);

        QicsStyleManager *sm = gi.styleManager();

        // get grid id
        quintptr id;
        ds >> id;

        // get the number of selections
        int nsels;
        ds >> nsels;

        // get the topmost row and leftmost column of the selections
        int top, left;
        ds >> top;
        ds >> left;
        for (int seln = 0; seln < nsels; ++seln) {
            // get the number of rows and columns
            int nrows, ncols;
            ds >> nrows;
            ds >> ncols;

            // get the starting cell of the selection
            int srow, scol;
            ds >> srow;
            ds >> scol;

            // go through each cell and set the attribute
            for (int i = 0; i < nrows; ++i) {
                for (int j = 0; j < ncols; ++j) {
                    bool isValid;
                    ds >> isValid;
                    if (isValid) {
                        QicsCellStyle *cs = QicsCellStyle::decode(ds, ((quintptr)&gi == id));
                        if (cs) {
                            int mcol = gi.modelColumnIndex(ccell+j+scol-left);
                            int mrow = gi.modelRowIndex(rcell+i+srow-top);
                            sm->setCellStyle(mrow, mcol, cs);
                            delete cs;
                        } else {
                            // #### here do clear...
                        }
                    }
                }
            }
        }

        ret = true;
    }

    if (dataIn->hasFormat(QICS_MIME_CELLSIZE) && (myCopyPolicy & Qics::CopyDimensions)) {
        QByteArray ba = dataIn->data(QICS_MIME_CELLSIZE);
        QDataStream ds(&ba, QIODevice::ReadOnly);

        QicsDimensionManager *dm = gi.dimensionManager();
        int size;

        // read rows
        ds >> size;
        for (int i = 0; i < size; ++i) {
            int row, height;
            ds >> row >> height;

            int mrow = gi.modelRowIndex(rcell+row);
            if (dm->rowHeight(mrow, true, false) < height)
                dm->setRowHeightInPixels(mrow, false, height);
        }

        // read cols
        ds >> size;
        for (int i = 0; i < size; ++i) {
            int col, width;
            ds >> col >> width;

            int mcol = gi.modelColumnIndex(ccell+col);
            if (dm->columnWidth(mcol, true, false) < width)
                dm->setColumnWidthInPixels(mcol, false, width);
        }

        ret = true;
    }

    if (dataIn->hasFormat(QICS_MIME_CELLSPAN) && (myCopyPolicy & Qics::CopySpans)) {
        QByteArray ba = dataIn->data(QICS_MIME_CELLSPAN);
        QDataStream ds(&ba, QIODevice::ReadOnly);
        int size;

        // get the number of selections
        int nsels;
        ds >> nsels;

        QicsSpanManager *sm = gi.styleManager()->spanManager();

        // remove old spans
        int nrows, ncols;
        int srow, scol;

        for (int seln = 0; seln < nsels; ++seln) {
            // get the number of rows and columns
            ds >> nrows;
            ds >> ncols;

            // get the starting cell of the selection
            ds >> srow;
            ds >> scol;

            // find spans nested into the selection
            QicsSpanList *sl = sm->cellSpanList();
            QRect selRect(ccell, rcell, ncols, nrows);
            for (int i = 0; i < sl->size(); ++i) {
                const QicsSpan &span = sl->at(i);
                QRect spanRect(gi.visualColumnIndex(gi.firstNonHiddenModelColumn(span.column(), span.column() + span.width() - 1)), gi.visualRowIndex(gi.firstNonHiddenModelRow(span.row(), span.row() + span.height() - 1)), span.width(), span.height());
                if (selRect.contains(spanRect))
                    sm->removeCellSpan(span.row(), span.column());
            }
            delete sl;
        }

        // read spans
        ds >> size;
        int r,c,w,h;

        for (int i = 0; i < size; ++i) {
            ds >> r >> c >> w >> h;
            QicsSpan span(gi.modelRowIndex(r+rcell), gi.modelColumnIndex(c+ccell), h, w);
            sm->addCellSpan(span);
        }

        ret = true;
    }

    return ret;
}
