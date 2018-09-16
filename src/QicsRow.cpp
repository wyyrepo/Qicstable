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

#include "QicsRow.h"

#include <QVector>

#include "QicsStyleManager.h"
#include "QicsDimensionManager.h"
#include "QicsSelectionManager.h"
#include "QicsUtil.h"
#include "QicsTable.h"


QicsRow::QicsRow(int row, QicsTable *table, bool follow_model)
    : QicsCellCommon(&(table->gridInfo()), table)
{
    myFollowModel = follow_model;
    commonInit(&(table->gridInfo()));

    setRowIndex(row);
}

QicsRow::QicsRow(int row, QicsGridInfo *info, bool follow_model, QObject *parent)
    : QicsCellCommon(parent)
{
    myFollowModel = follow_model;
    commonInit(info);

    setRowIndex(row);
}

void QicsRow::commonInit(QicsGridInfo *info)
{
    setInfo(info);
    init();
}

void QicsRow::init()
{
    if (!m_info)
        return;

    if (myFollowModel) {
        // connect ourself to the data model's signals

        if (dataModel()) {
            connect(dataModel(), SIGNAL(rowsInserted(int, int)),
                this, SLOT(handleModelRowInsert(int, int)));
            connect(dataModel(), SIGNAL(rowsDeleted(int, int)),
                this, SLOT(handleModelRowDelete(int, int)));
        }
    }
    else {
        // connect to the row ordering object
        if (m_info->rowOrdering())
            connect(m_info->rowOrdering(), SIGNAL(orderChanged(Qics::QicsIndexType, int*, int)),
                this, SLOT(handleOrderChanged(Qics::QicsIndexType, int*, int)));
    }
}

void QicsRow::setInfo(QicsGridInfo *info)
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

////////////////////////////////////////////////////////////////////////
/////////////////////    Index Methods   ///////////////////////////////
////////////////////////////////////////////////////////////////////////

int QicsRow::rowIndex() const
{
    if (myRow >= 0) {
        if (myFollowModel)
            return myRow;

        return m_info->visualRowIndex(myRow);
    }

    return -1;
}

void QicsRow::setRowIndex(int idx)
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

bool QicsRow::isValid() const
{
    if (dataModel())
        return ((myRow >= 0) && (myRow < dataModel()->numRows()));

    return true;
}

////////////////////////////////////////////////////////////////////////
////////////////////   Attribute methods   /////////////////////////////
////////////////////////////////////////////////////////////////////////

void QicsRow::setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val)
{
    styleManager().setRowProperty(modelOrVisualRowIndex(),
        !myFollowModel, attr, val);
}

void *QicsRow::getAttr(QicsCellStyle::QicsCellStyleProperty attr) const
{
    return (styleManager().getRowProperty(myRow, attr, rowIndex()));
}

void QicsRow::clearAttr(QicsCellStyle::QicsCellStyleProperty attr)
{
    styleManager().clearRowProperty(myRow, false, attr);
}

void QicsRow::setDMMargin(int margin)
{
    dimensionManager().setRowMargin(m_info->gridType(),
        modelOrVisualRowIndex(),
        !myFollowModel, margin);
}

void QicsRow::setDMFont(const QFont &font)
{
    dimensionManager().setRowFont(m_info->gridType(),
        modelOrVisualRowIndex(),
        !myFollowModel, font);
}

void QicsRow::setDMBorderWidth(int bw)
{
    dimensionManager().setRowBorderWidth(m_info->gridType(),
        modelOrVisualRowIndex(),
        !myFollowModel, bw);
}

QicsDataModelRow QicsRow::dataValues(int first_col, int last_col) const
{
    if (dataModel() && isValid())
        return dataModel()->rowItems(myRow, first_col, last_col);

    return QicsDataModelRow();
}

void QicsRow::setDataValues(QicsDataModelRow &vals)
{
    if (dataModel())
        dataModel()->setRowItems(myRow, vals);
}

int QicsRow::heightInPixels() const
{
    return (dimensionManager().rowHeight(myRow));
}

void QicsRow::setHeightInPixels(int height)
{
    dimensionManager().setRowHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height);
}

int QicsRow::heightInChars() const
{
    // divide the height in pixels by the height of the font -- round down.
    return (dimensionManager().rowHeight(myRow)  /
        qicsHeightOfFont(font()));
}

void QicsRow::setHeightInChars(int height)
{
    dimensionManager().setRowHeightInChars(modelOrVisualRowIndex(),
        !myFollowModel, height);
}

int QicsRow::minHeightInPixels() const
{
    return (dimensionManager().rowMinHeight(myRow, myFollowModel, !myFollowModel));
}

void QicsRow::setMinHeightInPixels(int height)
{
    dimensionManager().setRowMinHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height);
}

int QicsRow::minHeightInChars() const
{
    // divide the height in pixels by the height of the font -- round down.
    return (dimensionManager().rowMinHeight(myRow, myFollowModel, !myFollowModel)  /
        qicsHeightOfFont(font()));
}

void QicsRow::setMinHeightInChars(int height)
{
    dimensionManager().setRowMinHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height * qicsHeightOfFont(font()));
}

int QicsRow::maxHeightInPixels() const
{
    return (dimensionManager().rowMaxHeight(myRow, myFollowModel, !myFollowModel));
}

void QicsRow::setMaxHeightInPixels(int height)
{
    dimensionManager().setRowMaxHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height);
}

int QicsRow::maxHeightInChars() const
{
    // divide the height in pixels by the height of the font -- round down.
    return (dimensionManager().rowMaxHeight(myRow, myFollowModel, !myFollowModel)  /
        qicsHeightOfFont(font()));
}

void QicsRow::setMaxHeightInChars(int height)
{
    dimensionManager().setRowMaxHeightInPixels(modelOrVisualRowIndex(),
        !myFollowModel, height * qicsHeightOfFont(font()));
}

bool QicsRow::isHidden() const
{
    return (dimensionManager().isRowHidden(myRow));
}

bool QicsRow::isVisible() const
{
    return !isHidden();
}

void QicsRow::setVisible(bool visible)
{
    setHidden(!visible);
}

void QicsRow::setHidden(bool hidden)
{
    if (hidden) hide();
    else show();
}

bool QicsRow::isFiltered() const
{
    return (dimensionManager().isRowFiltered(myRow));
}

bool QicsRow::selected() const
{
    const QicsSelectionManager *sm = m_info->selectionManager();
    if (!sm) return false;
    const QicsSelectionList* list = sm->selectionList(true);
    if (!list) return false;

    const int row = rowIndex();
    return list->isRowSelected(row);
}

void QicsRow::hide()
{
    dimensionManager().hideRow(myRow);
}

void QicsRow::show()
{
    dimensionManager().showRow(myRow);
}

bool QicsRow::stretchable() const
{
    return dimensionManager().isRowStretchable(modelOrVisualRowIndex(),
        !myFollowModel);
}

void QicsRow::setStretchable(bool b)
{
    dimensionManager().setRowStretchable(modelOrVisualRowIndex(),
        !myFollowModel, b);
}

////////////////////////////////////////////////////////////////////////
//////////////////  Data Model Slots  //////////////////////////////////
////////////////////////////////////////////////////////////////////////

void QicsRow::handleModelRowInsert(int nrows, int pos)
{
    if (myRow >= pos)
        myRow += nrows;
}

void QicsRow::handleModelRowDelete(int nrows, int pos)
{
    if (myRow >= pos) {
        if  (myRow < (pos + nrows))
            myRow = -1;
        else
            myRow -= nrows;
    }
}

void QicsRow::handleOrderChanged(Qics::QicsIndexType type, int* visChange, int size)
{
    if (type == Qics::RowIndex) {
        if ((myRow >= 0) && (myRow < size)) {
            int new_row = visChange[myRow];
            if (new_row >= 0)
                myRow = new_row;
        }
    }
}

void QicsRow::changeDataModel(QicsDataModel *old_dt, QicsDataModel *)
{
    // get rid of connections to old data model
    if (old_dt)
        disconnect(old_dt, 0, this, 0);

    // the new data model will already be set in our grid info object
    init();
}

/*void  QicsRow::gridDeleted()
{
    if (myInfo)
        disconnect(myInfo, 0, this, 0);

    myInfo = 0;
}*/


