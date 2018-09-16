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

#include "QicsColumn.h"

#include <QVector>

#include "QicsStyleManager.h"
#include "QicsUtil.h"
#include "QicsSelectionManager.h"
#include "QicsDimensionManager.h"
#include "QicsTable.h"


QicsColumn::QicsColumn(int column, QicsTable *table, bool follow_model)
    : QicsCellCommon(&(table->gridInfo()), table)
{
    myFollowModel = follow_model;
    setColumnIndex(column);
    commonInit(&(table->gridInfo()));
}

QicsColumn::QicsColumn(int column, QicsGridInfo *info, bool follow_model, QObject *parent)
    : QicsCellCommon(parent)
{
    myFollowModel = follow_model;
    commonInit(info);

    setColumnIndex(column);
}

void QicsColumn::commonInit(QicsGridInfo *info)
{
    setInfo(info);
    init();
}

void QicsColumn::init()
{
    if (!m_info)
        return;

    if (myFollowModel) {
        // connect ourself to the data model's signals

        if (dataModel()) {
            connect(dataModel(), SIGNAL(columnsInserted(int, int)),
                this, SLOT(handleModelColumnInsert(int, int)));
            connect(dataModel(), SIGNAL(columnsDeleted(int, int)),
                this, SLOT(handleModelColumnDelete(int, int)));
        }
    }
    else {
        // connect to the column ordering object
        if (m_info->columnOrdering())
            connect(m_info->columnOrdering(), SIGNAL(orderChanged(Qics::QicsIndexType, int*, int)),
                this, SLOT(handleOrderChanged(Qics::QicsIndexType, int*, int)));
    }
}

void QicsColumn::setInfo(QicsGridInfo *info)
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

int QicsColumn::columnIndex() const
{
    if (myColumn >= 0) {
        if (myFollowModel)
            return myColumn;

        return m_info->visualColumnIndex(myColumn);
    }

    return -1;
}

int QicsColumn::modelColumnIndex() const
{
    return myColumn;
}

void QicsColumn::setColumnIndex(int idx)
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

bool QicsColumn::isValid() const
{
    if (dataModel())
        return ((myColumn >= 0) && (myColumn < dataModel()->numColumns()));

    return true;
}

////////////////////////////////////////////////////////////////////////
////////////////////   Attribute methods   /////////////////////////////
////////////////////////////////////////////////////////////////////////

void QicsColumn::setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val)
{
    styleManager().setColumnProperty(modelOrVisualColumnIndex(),
        !myFollowModel, attr, val);
}

void *QicsColumn::getAttr(QicsCellStyle::QicsCellStyleProperty attr) const
{
    return (styleManager().getColumnProperty(myColumn, attr, columnIndex()));
}

void QicsColumn::clearAttr(QicsCellStyle::QicsCellStyleProperty attr)
{
    styleManager().clearColumnProperty(myColumn, false, attr);
}

void QicsColumn::setDMMargin(int margin)
{
    dimensionManager().setColumnMargin(m_info->gridType(),
        modelOrVisualColumnIndex(),
        !myFollowModel, margin);
}

void QicsColumn::setDMFont(const QFont &font)
{
    dimensionManager().setColumnFont(m_info->gridType(),
        modelOrVisualColumnIndex(),
        !myFollowModel, font);
}

void QicsColumn::setDMBorderWidth(int bw)
{
    dimensionManager().setColumnBorderWidth(m_info->gridType(),
        modelOrVisualColumnIndex(),
        !myFollowModel, bw);
}

QicsDataModelColumn QicsColumn::dataValues(int first_row, int last_row) const
{
    if (dataModel() && isValid())
        return dataModel()->columnItems(myColumn, first_row, last_row);

    return QicsDataModelColumn();
}

void QicsColumn::setDataValues(QicsDataModelColumn &vals)
{
    dataModel()->setColumnItems(myColumn, vals);
}

int QicsColumn::widthInPixels() const
{
    return (dimensionManager().columnWidth(myColumn));
}

void QicsColumn::setWidthInPixels(int width)
{
    dimensionManager().setColumnWidthInPixels(modelOrVisualColumnIndex(),
        !myFollowModel, width);
}

int QicsColumn::widthInChars() const
{
    // divide the width in pixels by the width of the font -- round down.
    return (dimensionManager().columnWidth(myColumn)  /
        qicsWidthOfFont(font()));
}

void QicsColumn::setWidthInChars(int width)
{
    dimensionManager().setColumnWidthInChars(modelOrVisualColumnIndex(),
        !myFollowModel, width);
}

int QicsColumn::minWidthInPixels() const
{
    return (dimensionManager().columnMinWidth(myColumn, myFollowModel, !myFollowModel));
}

void QicsColumn::setMinWidthInPixels(int width)
{
    dimensionManager().setColumnMinWidthInPixels(modelOrVisualColumnIndex(),
        !myFollowModel, width);
}

int QicsColumn::maxWidthInPixels() const
{
    return (dimensionManager().columnMaxWidth(myColumn, myFollowModel, !myFollowModel));
}

void QicsColumn::setMaxWidthInPixels(int width)
{
    dimensionManager().setColumnMaxWidthInPixels(modelOrVisualColumnIndex(),
        !myFollowModel, width);
}

int QicsColumn::minWidthInChars() const
{
    // divide the width in pixels by the width of the font -- round down.
    return (dimensionManager().columnMinWidth(myColumn, myFollowModel,!myFollowModel) /
        qicsWidthOfFont(font()));
}

void QicsColumn::setMinWidthInChars(int width)
{
    dimensionManager().setColumnMinWidthInPixels(myColumn, !myFollowModel, width *
        qicsWidthOfFont(font()));
}

int QicsColumn::maxWidthInChars() const
{
    // divide the width in pixels by the width of the font -- round down.
    return (dimensionManager().columnMaxWidth(myColumn, myFollowModel,!myFollowModel) /
        qicsWidthOfFont(font()));
}

void QicsColumn::setMaxWidthInChars(int width)
{
    dimensionManager().setColumnMaxWidthInPixels(myColumn, !myFollowModel, width *
        qicsWidthOfFont(font()));
}

bool QicsColumn::isHidden() const
{
    return (dimensionManager().isColumnHidden(myColumn));
}

bool QicsColumn::isVisible() const
{
    return !isHidden();
}

void QicsColumn::setVisible(bool visible)
{
    setHidden(!visible);
}

void QicsColumn::setHidden(bool hidden)
{
    if (hidden) hide();
    else show();
}

bool QicsColumn::selected() const
{
    const QicsSelectionManager *sm = m_info->selectionManager();
    if (!sm) return false;

    const QicsSelectionList* list = sm->selectionList(true);
    if (!list) return false;

    const int col = columnIndex();
    return list->isColumnSelected(col);
}

void QicsColumn::hide()
{
    dimensionManager().hideColumn(myColumn);
}

void QicsColumn::show()
{
    dimensionManager().showColumn(myColumn);
}

bool QicsColumn::stretchable() const
{
    return dimensionManager().isColumnStretchable(modelOrVisualColumnIndex(),
        !myFollowModel);
}

void QicsColumn::setStretchable(bool b)
{
    dimensionManager().setColumnStretchable(modelOrVisualColumnIndex(),
        !myFollowModel, b);
}

////////////////////////////////////////////////////////////////////////
//////////////////  Data Model Slots  //////////////////////////////////
////////////////////////////////////////////////////////////////////////

void QicsColumn::handleModelColumnInsert(int ncols, int pos)
{
    if (myColumn >= pos)
        myColumn += ncols;
}

void QicsColumn::handleModelColumnDelete(int ncols, int pos)
{
    if (myColumn >= pos) {
        if  (myColumn < (pos + ncols))
            myColumn = -1;
        else
            myColumn -= ncols;
    }
}

void QicsColumn::handleOrderChanged(Qics::QicsIndexType type, int* visChange, int size)
{
    if (type == Qics::ColumnIndex) {
        if ((myColumn >= 0) && (myColumn < size)) {
            int new_col = visChange[myColumn];
            if (new_col >= 0)
                myColumn = new_col;
        }
    }
}

void QicsColumn::changeDataModel(QicsDataModel *old_dt, QicsDataModel *)
{
    // get rid of connections to old data model
    if (old_dt)
        disconnect(old_dt, 0, this, 0);

    // the new data model will already be set in our grid info object
    init();
}

/*void QicsColumn::gridDeleted()
{
    if (myInfo)
        disconnect(myInfo, 0, this, 0);

    myInfo = 0;
}*/


