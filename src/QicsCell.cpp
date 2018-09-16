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

#include "QicsCell.h"

#include <QVector>

#include "QicsStyleManager.h"
#include "QicsSpanManager.h"
#include "QicsDimensionManager.h"
#include "QicsSelectionManager.h"
#include "QicsDataModel.h"
#include "QicsUtil.h"
#include "QicsTable.h"



QicsCell::QicsCell(int row, int col, QicsTable *table, bool follow_model)
    : QicsCellCommon(&(table->gridInfo()), table)
{
    myFollowModel = follow_model;
    commonInit(&(table->gridInfo()));

    setRowIndex(row);
    setColumnIndex(col);
}

QicsCell::QicsCell(int row, int col, QicsGridInfo *info, bool follow_model, QObject *parent)
    : QicsCellCommon(parent)
{
    myFollowModel = follow_model;
    commonInit(info);

    setRowIndex(row);
    setColumnIndex(col);
}

void QicsCell::commonInit(QicsGridInfo *info)
{
    setInfo(info);
    init();
}

void QicsCell::init()
{
    if (!m_info)
        return;

    if (myFollowModel) {
        // connect ourself to the data model's signals
        const QicsDataModel *model = dataModel();
        if (model) {
            connect(model, SIGNAL(rowsInserted(int, int)),
                this, SLOT(handleModelRowInsert(int, int)));
            connect(model, SIGNAL(columnsInserted(int, int)),
                this, SLOT(handleModelColumnInsert(int, int)));

            connect(model, SIGNAL(rowsDeleted(int, int)),
                this, SLOT(handleModelRowDelete(int, int)));
            connect(model, SIGNAL(columnsDeleted(int, int)),
                this, SLOT(handleModelColumnDelete(int, int)));
        }
    }
    else {
        // connect to the row and column ordering objects
        if (m_info->rowOrdering())
            connect(m_info->rowOrdering(), SIGNAL(orderChanged(Qics::QicsIndexType, int*, int)),
                this, SLOT(handleOrderChanged(Qics::QicsIndexType, int*, int)));

        if (m_info->columnOrdering())
            connect(m_info->columnOrdering(), SIGNAL(orderChanged(Qics::QicsIndexType, int*, int)),
                this, SLOT(handleOrderChanged(Qics::QicsIndexType, int*, int)));
    }
}

void QicsCell::setInfo(QicsGridInfo *info)
{
    if (!info || m_info == info)
        return;

    QicsCellCommon::setInfo(info);

    if (m_info)
        disconnect(m_info, 0, this, 0);

    connect(m_info, SIGNAL(dataModelChanged(QicsDataModel *, QicsDataModel *)),
        this, SLOT(changeDataModel(QicsDataModel *, QicsDataModel *)));

    //connect(myInfo, SIGNAL(destroyed()), this, SLOT(gridDeleted()));
}

void QicsCell::setRowIndex(int idx)
{
    if (idx < 0)
        myRow = -1;
    else {
        if (myFollowModel)
            myRow = idx;
        else
            myRow = m_info->modelRowIndex(idx);
    }
}

void QicsCell::setColumnIndex(int idx)
{
    if (idx < 0)
        myColumn = -1;
    else {
        if (myFollowModel)
            myColumn = idx;
        else
            myColumn = m_info->modelColumnIndex(idx);
    }
}

int QicsCell::rowIndex() const
{
    if (myRow >= 0) {
        if (myFollowModel)
            return myRow;

        return m_info->visualRowIndex(myRow);
    }

    return -1;
}

int QicsCell::columnIndex() const
{
    if (myColumn >= 0) {
        if (myFollowModel)
            return myColumn;

        return m_info->visualColumnIndex(myColumn);
    }

    return -1;
}


bool QicsCell::isValid() const
{
    const QicsDataModel *model = dataModel();
    if (model)
        return ((myRow >= 0) && (myRow < model->numRows()) &&
            (myColumn >= 0) && (myColumn < model->numColumns()));

    return true;
}

void QicsCell::setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val)
{
    m_info->styleManager()->setCellProperty(modelOrVisualRowIndex(),
        modelOrVisualColumnIndex(),
        !myFollowModel, attr, val);
}

void *QicsCell::getAttr(QicsCellStyle::QicsCellStyleProperty attr) const
{
    return (m_info->styleManager()->getCellProperty(myRow, myColumn, attr,
        rowIndex(), columnIndex()));
}

void QicsCell::clearAttr(QicsCellStyle::QicsCellStyleProperty attr)
{
    m_info->styleManager()->clearCellProperty(modelOrVisualRowIndex(),
        modelOrVisualColumnIndex(), !myFollowModel, attr);
}

void QicsCell::setDMMargin(int margin)
{
    m_info->dimensionManager()->setCellMargin(m_info->gridType(),
        modelOrVisualRowIndex(), modelOrVisualColumnIndex(),
        !myFollowModel, margin);
}

void QicsCell::setDMFont(const QFont &font)
{
    m_info->dimensionManager()->setCellFont(m_info->gridType(),
        modelOrVisualRowIndex(), modelOrVisualColumnIndex(),
        !myFollowModel, font);
}

void QicsCell::setDMBorderWidth(int bw)
{
    m_info->dimensionManager()->setCellBorderWidth(m_info->gridType(),
        modelOrVisualRowIndex(), modelOrVisualColumnIndex(),
        !myFollowModel, bw);
}

void QicsCell::setDMFitSize(int w, int h)
{
    QicsDimensionManager *dm = m_info->dimensionManager();
    const int row = modelOrVisualRowIndex();
    const int col = modelOrVisualColumnIndex();

    if (dm->rowHeight(row) < h)
        dm->setRowHeightInPixels(row, !myFollowModel, h);
    if (dm->columnWidth(col) < w)
        dm->setColumnWidthInPixels(col, !myFollowModel, w);
}

const QicsDataItem *QicsCell::dataValue() const
{
    const QicsDataModel *dm = dataModel();
    if (dm && isValid())
        return (dm->item(myRow, myColumn));

    return 0;
}

QString QicsCell::dataString() const
{
    const QicsDataItem *item = dataValue();
    if (item) return item->string();

    return QString();
}

void QicsCell::setDataValue(const QicsDataItem &val)
{
    QicsDataModel *model = dataModel();
    if (model && isValid())
        model->setItem(myRow, myColumn, val);
}

void QicsCell::clearDataValue()
{
    QicsDataModel *model = dataModel();
    if (model && isValid())
        model->clearItem(myRow, myColumn);
}

bool QicsCell::addSpan(int height, int width)
{
    if (!isValid())
        return false;

    if(height < 1) height = 1;
    if(width < 1) width = 1;

    QicsSpan span(myRow, myColumn, height, width);
    return m_info->styleManager()->spanManager()->addCellSpan(span);
}

QicsRegion QicsCell::spannedRegion()
{
    QicsRegion r;
    m_info->styleManager()->spanManager()->isSpanner(*m_info, myRow, myColumn, r);
    return r;
}

void QicsCell::removeSpan()
{
    if (isValid())
        m_info->styleManager()->spanManager()->removeCellSpan(myRow, myColumn);
}

bool QicsCell::isCurrent() const
{
    return m_info->currentCell(true) == QicsICell(myRow, myColumn);
}

int QicsCell::widthInPixels() const
{
    return (m_info->dimensionManager()->columnWidth(myColumn));
}

void QicsCell::setWidthInPixels(int width)
{
    m_info->dimensionManager()->setColumnWidthInPixels(modelOrVisualColumnIndex(),
        !myFollowModel, width);
}

int QicsCell::widthInChars() const
{
    // divide the width in pixels by the width of the font -- round down.
    return (m_info->dimensionManager()->columnWidth(myColumn)  /
        qicsWidthOfFont(font()));
}

void QicsCell::setWidthInChars(int width)
{
    m_info->dimensionManager()->setColumnWidthInChars(modelOrVisualColumnIndex(),
        !myFollowModel, width);
}

int QicsCell::minWidthInPixels() const
{
    return (m_info->dimensionManager()->columnMinWidth(myColumn,myFollowModel, !myFollowModel));
}

void QicsCell::setMinWidthInPixels(int width)
{
    m_info->dimensionManager()->setColumnMinWidthInPixels(modelOrVisualColumnIndex(),
        !myFollowModel, width);
}

int QicsCell::maxWidthInPixels() const
{
    return (m_info->dimensionManager()->columnMaxWidth(myColumn, myFollowModel, !myFollowModel));
}

void QicsCell::setMaxWidthInPixels(int width)
{
    m_info->dimensionManager()->setColumnMaxWidthInPixels(modelOrVisualColumnIndex(), !myFollowModel, width);
}

int QicsCell::minWidthInChars() const
{
    // divide the width in pixels by the width of the font -- round down.
    return (m_info->dimensionManager()->columnMinWidth(myColumn, myFollowModel, !myFollowModel)  /
        qicsWidthOfFont(font()));
}

void QicsCell::setMinWidthInChars(int width)
{
    m_info->dimensionManager()->setColumnMinWidthInPixels(myColumn, !myFollowModel, width * qicsWidthOfFont(font()));
}

int QicsCell::maxWidthInChars() const
{
    // divide the width in pixels by the width of the font -- round down.
    return (m_info->dimensionManager()->columnMaxWidth(myColumn, myFollowModel, !myFollowModel)  /
        qicsWidthOfFont(font()));
}

void QicsCell::setMaxWidthInChars(int width)
{
    m_info->dimensionManager()->setColumnMaxWidthInPixels(myColumn, !myFollowModel, width * qicsWidthOfFont(font()));
}

int QicsCell::heightInPixels() const
{
    return (m_info->dimensionManager()->rowHeight(myRow));
}

void QicsCell::setHeightInPixels(int height)
{
    m_info->dimensionManager()->setRowHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height);
}

int QicsCell::heightInChars() const
{
    // divide the height in pixels by the height of the font -- round down.
    return (m_info->dimensionManager()->rowHeight(myRow)  /
        qicsHeightOfFont(font()));
}

void QicsCell::setHeightInChars(int height)
{
    m_info->dimensionManager()->setRowHeightInChars(modelOrVisualRowIndex(),
        !myFollowModel, height);
}

int QicsCell::minHeightInPixels() const
{
    return (m_info->dimensionManager()->rowMinHeight(myRow, myFollowModel, !myFollowModel));
}

void QicsCell::setMinHeightInPixels(int height)
{
    m_info->dimensionManager()->setRowMinHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height);
}

int QicsCell::maxHeightInPixels() const
{
    return (m_info->dimensionManager()->rowMaxHeight(myRow, myFollowModel, !myFollowModel));
}

void QicsCell::setMaxHeightInPixels(int height)
{
    m_info->dimensionManager()->setRowMaxHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height);
}

int QicsCell::maxHeightInChars() const
{
    // divide the height in pixels by the height of the font -- round down.
    return (m_info->dimensionManager()->rowMaxHeight(myRow, myFollowModel,!myFollowModel)  /
        qicsHeightOfFont(font()));
}

void QicsCell::setMaxHeightInChars(int height)
{
    m_info->dimensionManager()->setRowMaxHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height * qicsHeightOfFont(font()));
}

int QicsCell::minHeightInChars() const
{
    // divide the height in pixels by the height of the font -- round down.
    return (m_info->dimensionManager()->rowMinHeight(myRow, myFollowModel,!myFollowModel)  /
        qicsHeightOfFont(font()));
}

void QicsCell::setMinHeightInChars(int height)
{
    m_info->dimensionManager()->setRowMinHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height * qicsHeightOfFont(font()));
}

bool QicsCell::isHidden() const
{
    const QicsSpanManager *spm = m_info->styleManager()->spanManager();
    const int row = rowIndex();
    const int col = columnIndex();
    QicsRegion reg;

    if (spm && spm->isSpanner(*m_info, row, col, reg)) {
        const QicsMappedDimensionManager *mdm = m_info->mappedDM();
        for (int i = reg.startRow(); i <= reg.endRow(); ++i)
            if (!mdm->isRowHidden(i)) {
                for (int j = reg.startColumn(); j <= reg.endColumn(); ++j)
                    if (!mdm->isColumnHidden(j))
                        return false;
                break;
            }
        return true;
    }

    const QicsDimensionManager *dm = m_info->dimensionManager();
    return (dm->isRowHidden(myRow) ||
        dm->isColumnHidden(myColumn));
}

bool QicsCell::isVisible() const
{
    return !isHidden();
}

bool QicsCell::selected() const
{
    const QicsSelectionManager *sm = m_info->selectionManager();
    if (!sm) return false;

    const QicsSelectionList* list = sm->selectionList(true);
    if (!list) return false;

    QicsGridType gt = m_info->gridType();
    const int row = rowIndex();
    const int col = columnIndex();

    bool b;

    switch (gt) {
        case Qics::RowHeaderGrid:
            b = list->isRowSelected(row);
            break;
        case Qics::ColumnHeaderGrid:
            b = list->isColumnSelected(col);
            break;
        default:
            b = list->isCellSelected(row, col);
    }

    if (!b) {
        const QicsSelection *sel = sm->currentSelection();
        if (sel) {
            b = sel->contains(row, col);
            switch (gt) {
                case Qics::RowHeaderGrid:
                    b = b & (sel->endColumn() == Qics::QicsLAST_COLUMN);
                    break;
                case Qics::ColumnHeaderGrid:
                    b = b & (sel->endRow() == Qics::QicsLAST_ROW);
                    break;
                default:
                    break;
            }
        }
    }

    return b;
}

////////////////////////////////////////////////////////////////////////
//////////////////  Data Model Slots  //////////////////////////////////
////////////////////////////////////////////////////////////////////////

void QicsCell::handleModelRowInsert(int nrows, int pos)
{
    if (myRow >= pos)
        myRow += nrows;
}

void QicsCell::handleModelColumnInsert(int ncols, int pos)
{
    if (myColumn >= pos)
        myColumn += ncols;
}

void QicsCell::handleModelRowDelete(int nrows, int pos)
{
    if (myRow >= pos) {
        if  (myRow < (pos + nrows))
            myRow = -1;
        else
            myRow -= nrows;
    }
}

void QicsCell::handleModelColumnDelete(int ncols, int pos)
{
    if (myColumn >= pos) {
        if  (myColumn < (pos + ncols))
            myColumn = -1;
        else
            myColumn -= ncols;
    }
}

void QicsCell::handleOrderChanged(Qics::QicsIndexType type, int* visChange, int size)
{
    if (type == Qics::RowIndex) {
        if ((myRow >= 0) && (myRow < size)) {
            int new_row = visChange[myRow];
            if (new_row >= 0)
                myRow = new_row;
        }
    }
    else {
        if ((myColumn >= 0) && (myColumn < size)) {
            int new_col = visChange[myColumn];
            if (new_col >= 0)
                myColumn = new_col;
        }
    }
}

void QicsCell::changeDataModel(QicsDataModel *old_dt, QicsDataModel *)
{
    // get rid of connections to old data model
    if (old_dt)
        disconnect(old_dt, 0, this, 0);

    // the new data model will already be set in our grid info object
    init();
}

/*void QicsCell::gridDeleted()
{
    if (myInfo)
        disconnect(myInfo, 0, this, 0);

    myInfo = 0;
}*/


