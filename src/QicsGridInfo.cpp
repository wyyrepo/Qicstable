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

#include "QicsGridInfo.h"

#include <QFileInfo>

#include "QicsStyleManager.h"
#include "QicsSelectionManager.h"
#include "QicsDimensionManager.h"
#include "QicsSpanManager.h"
#include "QicsHeaderGrid.h"
#include "QicsTableRegionDrag.h"
#include "QicsFilter.h"
#include "QicsCell.h"
#include "QicsStandardSorterDelegate.h"


QicsGridInfo::QicsGridInfo(Qics::QicsGridType type, QObject *parent)
    : QObject(parent),
        myType(type),
        myCellValue(0),
        myDoingSelectionFlag(false),
        myIgnoreModelSizeChanges(false),
        myAllowPropagateChanges(false),
        myBlockSelectionSignals(false),
        m_copyPolicy(Qics::CopyAll),
        myClearPolicy(Qics::ClearAll),
        myHeaderDragStyle(Qics::SimpleDrag)
{
    m_dataModel = 0;
    myStyleMananager = 0;
    myDimensionManager = 0;
    myMappedDM = 0;
    mySelM = 0;
    m_rowFilter = 0;
    m_rowOrdering = 0;
    m_columnOrdering = 0;
    m_rowOrderOwner = this;
    m_columnOrderOwner = this;
    m_defaultSorterDelegate = new QicsStandardSorterDelegate();

    if (type == Qics::ColumnHeaderGrid)
        myAutoHeaderNamingMask = "%mcolumn%";
    else if (type == Qics::RowHeaderGrid)
        myAutoHeaderNamingMask = "%mrow%";
}

QicsGridInfo::~QicsGridInfo()
{
    delete m_defaultSorterDelegate;
    m_defaultSorterDelegate = 0;

    if(m_rowOrderOwner == this)
        delete m_rowOrdering;

    if(m_columnOrderOwner == this)
        delete m_columnOrdering;

    m_rowOrderOwner = 0;
    m_columnOrderOwner = 0;

    delete myMappedDM;
    myMappedDM = 0;

    delete m_rowFilter;
    m_rowFilter = 0;
}

void QicsGridInfo::setDataModel(QicsDataModel *dt)
{
    QicsDataModel *oldDT = m_dataModel;
    m_dataModel = dt;

    if (oldDT) {
        // remove all connections

        if (myStyleMananager)
            disconnect(oldDT, 0, myStyleMananager, 0);

        if (mySelM)
            disconnect(oldDT, 0, mySelM, 0);

        if (myDimensionManager)
            disconnect(oldDT, 0, myDimensionManager, 0);

        QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
        for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
            disconnect(oldDT, 0, *iter, 0);
    }

    delete m_rowFilter;
    m_rowFilter = new QicsRowFilter(this);
    connect(m_rowFilter, SIGNAL(filterChanged(int, bool)), this, SIGNAL(filterChanged(int, bool)));

    delete m_rowOrdering;
    m_rowOrdering = new QicsSorter(Qics::RowIndex, m_dataModel);
    m_rowOrdering->setDefaultSorterDelegate(m_defaultSorterDelegate);
    m_rowOrderOwner = this;

    delete m_columnOrdering;
    m_columnOrdering = new QicsSorter(Qics::ColumnIndex, m_dataModel);
    m_columnOrdering->setDefaultSorterDelegate(m_defaultSorterDelegate);
    m_columnOrderOwner = this;

    connectDTtoSM();
    connectDTtoDM();

    if (m_dataModel) {
        disconnect(m_dataModel, SIGNAL(modelChanged(const QicsRegion &)),
            this, SLOT(redrawModel(const QicsRegion &)));

        disconnect(m_dataModel, SIGNAL(cellValueChanged(int, int)),
            this, SIGNAL(cellValueChanged(int, int)));

        disconnect(this, SIGNAL(cellValueChanged(int, int)),
            this, SLOT(propagateChangesFromCell(int,int)));

        connect(m_dataModel, SIGNAL(modelChanged(const QicsRegion &)),
            this, SLOT(redrawModel(const QicsRegion &)));

        connect(m_dataModel, SIGNAL(cellValueChanged(int, int)),
            this, SIGNAL(cellValueChanged(int, int)));

        connect(this, SIGNAL(cellValueChanged(int, int)),
            this, SLOT(propagateChangesFromCell(int,int)));
    }

    QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
    for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
        connectDTtoGrid(*iter);

    emit dataModelChanged(oldDT, m_dataModel);
}

void QicsGridInfo::setStyleManager(QicsStyleManager *sm)
{
    if (!sm || sm == myStyleMananager)
        return;

    QicsStyleManager *oldSM = myStyleMananager;
    myStyleMananager = sm;

    if (oldSM) {
        disconnect(m_dataModel, 0, oldSM, 0);

        QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
        for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
            disconnect(oldSM, 0, *iter, 0);
    }

    connectDTtoSM();
    QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
    for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
        connectSMtoGrid(*iter);

    emit styleManagerChanged(oldSM, myStyleMananager);
}

void QicsGridInfo::setDimensionManager(QicsDimensionManager *dm)
{
    if (!dm || dm == myDimensionManager)
        return;

    QicsDimensionManager *oldDM = myDimensionManager;
    myDimensionManager = dm;
    myMappedDM = new QicsMappedDimensionManager(dm, this);

    if (oldDM) {
        disconnect(m_dataModel, 0, oldDM, 0);
        QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
        for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
            disconnect(oldDM, 0, *iter, 0);

        if (m_rowOrdering)
            disconnect(oldDM, SIGNAL(rowVisibilityChanged(int,bool)), m_rowOrdering, SLOT(elementVisibilityChanged(int,bool)));

        if (m_columnOrdering)
            disconnect(oldDM, SIGNAL(columnVisibilityChanged(int,bool)), m_columnOrdering, SLOT(elementVisibilityChanged(int,bool)));
    }

    connectDTtoDM();

    QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
    for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
        connectDMtoGrid(*iter);

    if (m_rowOrdering)
        connect(myDimensionManager, SIGNAL(rowVisibilityChanged(int,bool)), m_rowOrdering, SLOT(elementVisibilityChanged(int,bool)));

    if (m_columnOrdering)
        connect(myDimensionManager, SIGNAL(columnVisibilityChanged(int,bool)), m_columnOrdering, SLOT(elementVisibilityChanged(int,bool)));

    emit dimensionManagerChanged(oldDM, myDimensionManager);
}

void QicsGridInfo::setSelectionManager(QicsSelectionManager *sel_m)
{
    if (!sel_m || sel_m == mySelM)
        return;

    QicsSelectionManager *oldSelM = mySelM;
    mySelM = sel_m;

    if (oldSelM) {
        disconnect(oldSelM, 0, this, 0);
        if (m_dataModel)
            disconnect(m_dataModel, 0, mySelM, 0);

        QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
        for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
            disconnect(oldSelM, 0, *iter, 0);
    }

    if (mySelM && m_dataModel) {
        connect(mySelM, SIGNAL(selectionListChanged(bool)),
            this, SLOT(handleSelectionChange(bool)));
        if (myType == Qics::TableGrid) {
            connect(m_dataModel, SIGNAL(rowsDeleted(int, int)),
                mySelM, SLOT(deleteRows(int, int)));
            connect(m_dataModel, SIGNAL(columnsDeleted(int, int)),
                mySelM, SLOT(deleteColumns(int, int)));
        }

    }
    QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
    for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
        connectSelMtoGrid(*iter);

    if ((m_rowOrderOwner == this) && m_rowOrdering)
        connect(m_rowOrdering, SIGNAL(orderChanged(Qics::QicsIndexType, int*, int)),
            mySelM, SLOT(orderChanged(Qics::QicsIndexType, int*, int)));

    if ((m_columnOrderOwner == this) && m_columnOrdering)
        connect(m_columnOrdering, SIGNAL(orderChanged(Qics::QicsIndexType, int*, int)),
            mySelM, SLOT(orderChanged(Qics::QicsIndexType, int*, int)));

    emit selectionManagerChanged(oldSelM, mySelM);
}

void QicsGridInfo::setSorterDelegate(QicsAbstractSorterDelegate *sorter)
{
    if (!sorter || sorter == m_defaultSorterDelegate)
        return;

    delete m_defaultSorterDelegate;
    m_defaultSorterDelegate = sorter;

    if (m_columnOrdering)
        m_columnOrdering->setDefaultSorterDelegate(m_defaultSorterDelegate);
    if (m_rowOrdering)
        m_rowOrdering->setDefaultSorterDelegate(m_defaultSorterDelegate);
}

void QicsGridInfo::connectGrid(QicsScreenGrid *grid)
{
    if (grid) {
        connectDTtoGrid(grid);
        connectSMtoGrid(grid);
        connectDMtoGrid(grid);
        connectSelMtoGrid(grid);

        myGrids.push_back(grid);

        connect(this, SIGNAL(dataModelChanged(QicsDataModel *, QicsDataModel *)),
            grid, SLOT(  recomputeAndDraw()));
        connect(this, SIGNAL(styleManagerChanged(QicsStyleManager *, QicsStyleManager *)),
            grid, SLOT(  recomputeAndDraw()));
        connect(this, SIGNAL(dimensionManagerChanged(QicsDimensionManager *, QicsDimensionManager *)),
            grid, SLOT(  recomputeAndDraw()));
        connect(this, SIGNAL(modelReordered(Qics::QicsIndexType)),
            grid, SLOT(  recomputeAndDraw()));

        connect(grid, SIGNAL(destroyed(QObject *)), SLOT(  removeGrid(QObject *)));
        connect(grid, SIGNAL(uneditRequest()), SIGNAL(uneditRequest()));
        connect(this, SIGNAL(uneditRequest()), grid, SLOT(uneditCurrentCell()));
        connect(this, SIGNAL(gridRepaintingEnabled()), grid, SLOT(redraw()));

        emit gridAdded(grid);
    }
}

void QicsGridInfo::disconnectGrid(QicsScreenGrid *grid)
{
    if (!grid)
        return;

    disconnect(m_dataModel, 0, grid, 0);
    disconnect(myStyleMananager, 0, grid, 0);
    disconnect(myDimensionManager, 0, grid, 0);
    disconnect(mySelM, 0, grid, 0);
    disconnect(grid, 0, this, 0);

    removeGrid(grid);

    emit gridRemoved(grid);
}

const QicsDataItem *QicsGridInfo::cellValue(int row, int col) const
{
    const QicsDataItem *itm = 0;

    if (m_dataModel)
        itm = m_dataModel->item(modelRowIndex(row), modelColumnIndex(col));

    if (!itm) {
        int mridx = modelRowIndex(row), mcidx = modelColumnIndex(col);

        if (gridType() == Qics::RowHeaderGrid || gridType() == Qics::ColumnHeaderGrid) {
            QString mask = autoHeaderNamingMask();
            QicsGridInfo *self = const_cast<QicsGridInfo *> (this);

            mask.replace(QRegExp("([^\\\\])%column%|^%column%"),"\\1"+QString().setNum(col));
            mask.replace(QRegExp("([^\\\\])%row%|^%row%"),"\\1"+QString().setNum(row));
            mask.replace(QRegExp("([^\\\\])%mcolumn%|^%mcolumn%"),"\\1"+QString().setNum(mcidx + 1));//#89902
            mask.replace(QRegExp("([^\\\\])%mrow%|^%mrow%"),"\\1"+QString().setNum(mridx + 1));//#89902
            mask.replace(QRegExp("\\\\%"),"%");
            self->myCellValue = QicsDataString(mask);
            itm = &(self->myCellValue);
        }
    }

    return itm;
}


Qics::QicsRepaintBehavior QicsGridInfo::gridRepaintBehavior() const
{
    return (* (static_cast<Qics::QicsRepaintBehavior *>
        (myStyleMananager->getGridProperty(QicsGridStyle::GridRepaintBehavior))));
}

void QicsGridInfo::setGlobalRepaintBehavior(Qics::QicsRepaintBehavior behavior)
{
    emit globalSetRepaintBehaviorRequest(behavior);
}

void QicsGridInfo::revertGlobalRepaintBehavior()
{
    emit globalRevertRepaintBehaviorRequest();
}

void QicsGridInfo::setGridRepaintBehavior(Qics::QicsRepaintBehavior behavior)
{
    if (gridRepaintBehavior() == behavior) return;

    myPrevGridRepaintBehavior =  * static_cast<Qics::QicsRepaintBehavior *>
        (myStyleMananager->getGridProperty(QicsGridStyle::GridRepaintBehavior));

    myStyleMananager->setGridProperty(QicsGridStyle::GridRepaintBehavior,
        static_cast<void *> (&behavior));

    if (behavior == Qics::RepaintOn)
        emit gridRepaintingEnabled();
}

void QicsGridInfo::revertGridRepaintBehavior()
{
    myStyleMananager->setGridProperty(QicsGridStyle::GridRepaintBehavior,
        static_cast<void *> (&myPrevGridRepaintBehavior));
}

void QicsGridInfo::removeGrid(QObject *obj)
{
    QicsScreenGridPV::iterator iter, iter_end(myGrids.end());
    for (iter = myGrids.begin(); iter != iter_end; ++iter)
        if (*iter == obj) {
            myGrids.erase(iter);
            mySelM->removeHeader(obj);
            break;
        }
}

void QicsGridInfo::connectDTtoSM()
{
    if (m_dataModel && myStyleMananager) {
        connect(m_dataModel, SIGNAL(rowsAdded(int)),
            myStyleMananager, SLOT(addRows(int)));
        connect(m_dataModel, SIGNAL(columnsAdded(int)),
            myStyleMananager, SLOT(addColumns(int)));
        connect(m_dataModel, SIGNAL(rowsInserted(int, int)),
            myStyleMananager, SLOT(insertRows(int, int)));
        connect(m_dataModel, SIGNAL(columnsInserted(int, int)),
            myStyleMananager, SLOT(insertColumns(int, int)));
        connect(m_dataModel, SIGNAL(rowsDeleted(int, int)),
            myStyleMananager, SLOT(deleteRows(int, int)));
        connect(m_dataModel, SIGNAL(columnsDeleted(int, int)),
            myStyleMananager, SLOT(deleteColumns(int, int)));

        myStyleMananager->setDataModel(m_dataModel);
    }
}

void QicsGridInfo::connectDTtoDM()
{
    if (m_dataModel && myDimensionManager) {
        connect(m_dataModel, SIGNAL(rowsInserted(int, int)),
            myDimensionManager, SLOT(insertRows(int, int)));
        connect(m_dataModel, SIGNAL(columnsInserted(int, int)),
            myDimensionManager, SLOT(insertColumns(int, int)));
        connect(m_dataModel, SIGNAL(rowsDeleted(int, int)),
            myDimensionManager, SLOT(deleteRows(int, int)));
        connect(m_dataModel, SIGNAL(columnsDeleted(int, int)),
            myDimensionManager, SLOT(deleteColumns(int, int)));
    }
}

void QicsGridInfo::connectDTtoGrid(QicsScreenGrid *grid)
{
    if (m_dataModel && grid) {
        connect(m_dataModel, SIGNAL(prepareForRowChanges(int, int)),
            grid, SLOT(handleRowsChanges(int, int)));
        connect(m_dataModel, SIGNAL(prepareForColumnChanges(int, int)),
            grid, SLOT(handleColumnsChanges(int, int)));

        connect(m_dataModel, SIGNAL(modelSizeChanged(int, int, Qics::QicsIndexType)),
            grid, SLOT(resetAndDraw(int, int, Qics::QicsIndexType)));
        connect(m_dataModel, SIGNAL(modelSizeChanged(int, int)),
            grid, SLOT(resetAndDraw()));
    }
}

void QicsGridInfo::connectSMtoGrid(QicsScreenGrid *grid)
{
    if (myStyleMananager && grid) {
        connect(myStyleMananager,
            SIGNAL(cellPropertyChanged(const QicsRegion &,
            QicsCellStyle::QicsCellStyleProperty,
            bool)),
            grid,
            SLOT(handleCellPropertyChange(const QicsRegion &,
            QicsCellStyle::QicsCellStyleProperty,
            bool)));

        connect(myStyleMananager,
            SIGNAL(gridPropertyChanged(QicsGridStyle::QicsGridStyleProperty)),
            grid,
            SLOT(handleGridPropertyChange(QicsGridStyle::QicsGridStyleProperty)));

        connect(myStyleMananager,
            SIGNAL(spanChanged(const QicsSpan &)),
            grid,
            SLOT(redraw(const QicsSpan &)));
    }
}

void QicsGridInfo::connectDMtoGrid(QicsScreenGrid *grid)
{
    if (myDimensionManager && grid) {
        connect(myDimensionManager,   SIGNAL(dimensionChanged()),
            grid,   SLOT(recomputeAndDraw()));
        connect(myDimensionManager,   SIGNAL(dimensionChanged(Qics::QicsIndexType, int, int)),
            grid,   SLOT(recomputeAndDraw(Qics::QicsIndexType, int, int)));
    }
}

void QicsGridInfo::connectSelMtoGrid(QicsScreenGrid *grid)
{
    if (mySelM && grid) {
        connect(mySelM, SIGNAL(selectionCellsChanged(const QicsRegion &)),
            grid, SLOT(redraw(const QicsRegion &)));

        if (qobject_cast<QicsHeaderGrid*>(grid)) {
            QicsHeaderGrid *hg = static_cast<QicsHeaderGrid *> (grid);

            mySelM->addHeader(hg);
        }
    }
}

void QicsGridInfo::deleteRows(int how_many, int start)
{
    if (!m_dataModel)
        return;

    // Applications should not be calling delete rows
    // on anything but the master
    Q_ASSERT(m_rowOrderOwner == this);

    m_dataModel->deleteRows(how_many, modelRowIndex(start));

    // Set Current cell to correct
    const int col = currentCell().column();
    const int row = currentCell().row();
    QicsICell cell(row, col);

    if (start <= modelRowIndex(row) && row != -1 && col != -1) {
        if (modelRowIndex(row) >= start + how_many) {
            cell.setRow(row - how_many);
            cell.setColumn(col);
            if (cell.isValid())
                setCurrentCell(cell);
        }
    }
}

void QicsGridInfo::insertRow(int where)
{
    insertRows(where, 1);
}

void QicsGridInfo::insertRows(int where, int count)
{
    if (!m_dataModel)
        return;

    // Applications should not be calling insertRow
    // on anything but the master
    Q_ASSERT(m_rowOrderOwner == this);

    /* Insert at end is easy */
    if(where < 0) {
        m_dataModel->addRows(count);
        return;
    }

    /* Insert in the middle is a littler trickier.  We
    * must insert into the model using model indices.
    */
    m_dataModel->insertRows(count, modelRowIndex(where));

    /* now the data model tells the sorters for any grids
    * looking at it that we inserted a row, but in model
    * dimensions, so the sorter has to go through gyrations,
    * but that is in QicsSorter.C, so we do not see the pain
    * here.
    */

    // Set Current cell to correct
    const int col = currentCell().column();
    const int row = currentCell().row();
    QicsICell cell(row, col);

    if (where <= modelRowIndex(row) && row != -1 && col != -1) {
        cell.setRow(row + count);
        cell.setColumn(col);
        if (cell.isValid())
            setCurrentCell(cell);
    }
}

void QicsGridInfo::deleteColumns(int how_many, int start)
{
    if (!m_dataModel)
        return;

    // Applications should not be calling delete rows
    // on anything but the master
    Q_ASSERT(m_columnOrderOwner == this);

    m_dataModel->deleteColumns(how_many, modelColumnIndex(start));

    // And set Current cell to correct
    const int col = currentCell().column();
    const int row = currentCell().row();
    QicsICell cell(row, col);

    if (start <= modelColumnIndex(col) && row != -1 && col != -1) {
        if (modelColumnIndex(col) >= start + how_many) {
            cell.setRow(row);
            cell.setColumn(col - how_many);
            if (cell.isValid())
                setCurrentCell(cell);
        }
    }
}

void QicsGridInfo::insertColumn(int where)
{
    insertColumns(where, 1);
}

void QicsGridInfo::insertColumns(int where, int count)
{
    if (!m_dataModel)
        return;

    // Applications should not be calling insertColumn
    // on anything but the master
    Q_ASSERT(m_columnOrderOwner == this);

    /* Insert at end is easy */
    if(where < 0) {
        m_dataModel->addColumns(count);
        return;
    }

    /* Insert in the middle is a littler trickier.  We
    * must insert into the model using model indices.
    */
    m_dataModel->insertColumns(count, modelColumnIndex(where));

    /* now the data model tells the sorters for any grids
    * looking at it that we inserted a row, but in model
    * dimensions, so the sorter has to go through gyrations,
    * but that is in QicsSorter.C, so we do not see the pain
    * here.
    */

    // Set Current cell to correct
    const int col = currentCell().column();
    const int row = currentCell().row();
    QicsICell cell(row, col);

    if (where <= modelColumnIndex(col) && row != -1 && col != -1) {
        cell.setRow(row);
        cell.setColumn(col + count);
        if (cell.isValid())
            setCurrentCell(cell);
    }
}

void QicsGridInfo::controlColumnsOf(QicsGridInfo *slave)
{
    if (m_dataModel && m_dataModel != slave->m_dataModel) {
        //remove previous connections (if exists) of Data Model to Slave Data Model
        disconnect(m_dataModel, SIGNAL(columnsInserted(int, int)),
            slave->m_dataModel, SLOT(insertColumns(int, int)));
        disconnect(m_dataModel, SIGNAL(columnsAdded(int)),
            slave->m_dataModel, SLOT(addColumns(int)));
        disconnect(m_dataModel, SIGNAL(columnsDeleted(int, int)),
            slave->m_dataModel, SLOT(deleteColumns(int, int)));

        //remove previous connections (if exists) of Slave Data Model to Data Model
        disconnect(slave->m_dataModel, SIGNAL(columnsInserted(int, int)),
            m_dataModel, SLOT(insertColumns(int, int)));
        disconnect(slave->m_dataModel, SIGNAL(columnsAdded(int)),
            m_dataModel, SLOT(addColumns(int)));
        disconnect(slave->m_dataModel, SIGNAL(columnsDeleted(int, int)),
            m_dataModel, SLOT(deleteColumns(int, int)));

        //create a new connections of Data Model to Slave Data Model
        connect(m_dataModel, SIGNAL(columnsInserted(int, int)),
            slave->m_dataModel, SLOT(insertColumns(int, int)));
        connect(m_dataModel, SIGNAL(columnsAdded(int)),
            slave->m_dataModel, SLOT(addColumns(int)));
        connect(m_dataModel, SIGNAL(columnsDeleted(int, int)),
            slave->m_dataModel, SLOT(deleteColumns(int, int)));
    }

    //remove previous connection
    disconnect(this, SIGNAL(modelReordered(Qics::QicsIndexType)),
        slave, SLOT(redrawAllGrids()));
    //create a new connection
    connect(this, SIGNAL(modelReordered(Qics::QicsIndexType)),
        slave, SLOT(redrawAllGrids()));

    /* I am not really happy with the master modifying
    * the slave.  What we should have is a helper function
    * named "becomeColumnSlaveOf()", which does this part.
    * But then we have another special method, which should
    * only be called from this method.  C++ has no way to
    * enforce that contract other than documentation.
    * The angst of it all.   Maybe I'm just nit-picking.
    * -tony
    */
    if(slave->m_columnOrderOwner == slave)
        delete slave->m_columnOrdering;

    slave->m_columnOrdering = m_columnOrdering;
    slave->m_columnOrderOwner = this;
}

void QicsGridInfo::controlRowsOf(QicsGridInfo *slave)
{
    if (m_dataModel && m_dataModel != slave->m_dataModel) {
        //remove previous connections (if exists) of Data Model to Slave Data Model
        disconnect(m_dataModel, SIGNAL(rowsInserted(int, int)),
            slave->m_dataModel, SLOT(insertRows(int, int)));
        disconnect(m_dataModel, SIGNAL(rowsAdded(int)),
            slave->m_dataModel, SLOT(addRows(int)));
        disconnect(m_dataModel, SIGNAL(rowsDeleted(int, int)),
            slave->m_dataModel, SLOT(deleteRows(int, int)));

        //remove previous connections (if exists) of Slave Data Model to Data Model
        disconnect(slave->m_dataModel, SIGNAL(rowsInserted(int, int)),
            m_dataModel, SLOT(insertRows(int, int)));
        disconnect(slave->m_dataModel, SIGNAL(rowsAdded(int)),
            m_dataModel, SLOT(addRows(int)));
        disconnect(slave->m_dataModel, SIGNAL(rowsDeleted(int, int)),
            m_dataModel, SLOT(deleteRows(int, int)));

        //create a new connections of Data Model to Slave Data Model
        connect(m_dataModel, SIGNAL(rowsInserted(int, int)),
            slave->m_dataModel, SLOT(insertRows(int, int)));
        connect(m_dataModel, SIGNAL(rowsAdded(int)),
            slave->m_dataModel, SLOT(addRows(int)));
        connect(m_dataModel, SIGNAL(rowsDeleted(int, int)),
            slave->m_dataModel, SLOT(deleteRows(int, int)));
    }

    //remove previous connection
    disconnect(this, SIGNAL(modelReordered(Qics::QicsIndexType)),
        slave, SLOT(redrawAllGrids()));
    //create a new connection
    connect(this, SIGNAL(modelReordered(Qics::QicsIndexType)),
        slave, SLOT(redrawAllGrids()));

    if(slave->m_rowOrderOwner == slave)
        delete slave->m_rowOrdering;

    slave->m_rowOrdering = m_rowOrdering;
    slave->m_rowOrderOwner = this;
}

void QicsGridInfo::orderRowsBy(const QVector<int> &columns, Qics::QicsSortOrder order, int from, int to,
                          DataItemComparator func, bool models)
{
    if (models)
        m_rowOrdering->sort(columns, order, from, to, func);
    else {
        QVector<int>::const_iterator iter, iter_end(columns.constEnd());
        QVector<int> mySortedColumns;
        for (iter = columns.constBegin(); iter<iter_end; ++iter)
            mySortedColumns << modelColumnIndex(*iter);

        m_rowOrdering->sort(mySortedColumns, order, from, to, func);
    }

    emit modelReordered(Qics::RowIndex);
}

void QicsGridInfo::orderColumnsBy(const QVector<int> &rows, Qics::QicsSortOrder order, int from, int to,
                             DataItemComparator func, bool models)
{
    if (models)
        m_columnOrdering->sort( rows, order, from, to, func);
    else {
        QVector<int>::const_iterator iter, iter_end(rows.constEnd());
        QVector<int> mySortedRows;
        for (iter = rows.constBegin(); iter<iter_end; ++iter)
            mySortedRows << modelRowIndex(*iter);

        m_columnOrdering->sort( mySortedRows, order, from, to, func);
    }

    emit modelReordered(Qics::ColumnIndex);
}

void QicsGridInfo::moveRows(int target, const QVector<int> &rows)
{
    if(this->m_rowOrderOwner == this) {
        m_rowOrdering->moveItems(target, rows);

        emit modelReordered(Qics::RowIndex);
        emit rowsMoved(target,rows);
    }
    else
        m_rowOrderOwner->moveRows(target, rows);
}

void QicsGridInfo::moveColumns(int target, const QVector<int> &cols)
{
    if(this->m_columnOrderOwner == this) {
        m_columnOrdering->moveItems(target, cols);

        emit modelReordered(Qics::ColumnIndex);
        emit columnsMoved(target,cols);
    }
    else
        m_columnOrderOwner->moveColumns(target, cols);
}

void QicsGridInfo::reorganizeSpans(QicsSorter *sorter, int target, const QVector<int> &items)
{
    QicsSpanManager *spm = myStyleMananager->spanManager();

    QicsSpanList *spans = spm->cellSpanList();
    spm->removeAllSpans();

    sorter->moveItems(target, items);

    foreach(const QicsSpan &span, *spans)
        spm->addCellSpan(span);

    delete spans;
}

QicsRegion QicsGridInfo::modelRegion(const QicsRegion &vis_region) const
{
    QicsRegion mr(vis_region);

    // don't translate if the region contains all rows
    if ((vis_region.startRow() != 0) ||
        (vis_region.endRow() != QicsLAST_ROW)) {
        mr.setStartRow(modelRowIndex(vis_region.startRow()));
        mr.setEndRow(modelRowIndex(vis_region.endRow()));
    }

    // don't translate if the region contains all columns
    if ((vis_region.startColumn() != 0) ||
        (vis_region.endColumn() != QicsLAST_COLUMN)) {
        mr.setStartColumn(modelColumnIndex(vis_region.startColumn()));
        mr.setEndColumn(modelColumnIndex(vis_region.endColumn()));
    }

    return mr;
}

QicsRegion QicsGridInfo::visualRegion(const QicsRegion &model_region) const
{
    QicsRegion vr(model_region);

    // don't translate if the region contains all rows
    if ((model_region.startRow() != 0) ||
        (model_region.endRow() != QicsLAST_ROW)) {
        vr.setStartRow(visualRowIndex(model_region.startRow()));
        vr.setEndRow(visualRowIndex(model_region.endRow()));
    }

    // don't translate if the region contains all columns
    if ((model_region.startColumn() != 0) ||
        (model_region.endColumn() != QicsLAST_COLUMN)) {
        vr.setStartColumn(visualColumnIndex(model_region.startColumn()));
        vr.setEndColumn(visualColumnIndex(model_region.endColumn()));
    }

    return vr;
}

void QicsGridInfo::redrawAllGrids(bool reset)
{
    QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
    for (iter = myGrids.constBegin(); iter != iter_end; ++iter) {
        if (reset)
            (*iter)->recomputeCellPositions();
        (*iter)->recomputeAndDraw();
    }
}

void QicsGridInfo::updateAllGrids(bool redraw)
{
    QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
    for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
        if (redraw)
            (*iter)->redraw();
        else
            (*iter)->update();
}

void QicsGridInfo::redrawAllGrids(Qics::QicsIndexType indexType)
{
    // hack : to force redraw needed grids
    switch (indexType)
    {
    case Qics::NoIndex:
        return;
    case Qics::ColumnIndex:
    case Qics::RowIndex:
        emit modelReordered( indexType );
        break;
    case Qics::RowAndColumnIndex:
        emit redrawAllGrids();
        break;
    default:
        emit redrawAllGrids();
        break;
    }
}

void QicsGridInfo::redrawModel(const QicsRegion &r, bool model)
{
    QicsRegion rv(model ? visualRegion(r) : r);
    QicsScreenGridPV::const_iterator iter, iter_end(myGrids.constEnd());
    for (iter = myGrids.constBegin(); iter != iter_end; ++iter)
        (*iter)->redraw(rv);
}

void QicsGridInfo::setSortingSensitivity(Qt::CaseSensitivity cs)
{
    m_rowOrdering->setSortingSensitivity(cs);
}

QicsICell QicsGridInfo::currentCell(bool model) const
{
    if (model) return myCurrentCell;

    // Create a cell in visual coordinates, as that's what the grids expect
    QicsICell cell;

    if (myCurrentCell.isValid()) {
        cell.setRow(visualRowIndex(myCurrentCell.row()));
        cell.setColumn(visualColumnIndex(myCurrentCell.column()));
    }

    return cell;
}

void QicsGridInfo::setCurrentCell(const QicsICell &vis_cell)
{
    // Yes, we have to do this.
    QicsICell old_cell = myCurrentCell;
    myCurrentCell = QicsICell();

    redrawModel(QicsRegion(old_cell));

    QicsICell cell;

    // We store this in model coordinates, so row/column reordering works
    if (vis_cell.isValid()) {
        int mr = modelRowIndex(vis_cell.row());
        int mc = modelColumnIndex(vis_cell.column());

        bool enabled = (* static_cast<bool*>
            (styleManager()->getCellProperty(mr, mc,
            QicsCellStyle::Enabled)));

        if (enabled) {
            cell.setRow(mr);
            cell.setColumn(mc);
        }
    }

    myCurrentCell = cell;

    redrawModel(QicsRegion(cell));

    emit currentCellChanged(vis_cell.row(), vis_cell.column());
}

void QicsGridInfo::setCellValue(int row, int col, const QicsDataItem &itm)
{
    if (dataModel())
        dataModel()->setItem(modelRowIndex(row), modelColumnIndex(col), itm);
}

void QicsGridInfo::setCurrentCellValue(const QicsDataItem &itm)
{
    if (myCurrentCell.isValid() && dataModel())
        dataModel()->setItem(myCurrentCell.row(), myCurrentCell.column(), itm);
}

void QicsGridInfo::setDefaultImagePath(const QString &path)
{
    QFileInfo fi(path);
    myDefaultImagePath = fi.absoluteFilePath();
    if (!myDefaultImagePath.endsWith("/"))
        myDefaultImagePath += "/";
}

int QicsGridInfo::firstNonHiddenRow(int startRow, int endRow) const
{
    while (startRow <= endRow) {
        if (!mappedDM()->isRowHidden(startRow))
            return startRow;

        ++startRow;
    }

    return -1;
}

int QicsGridInfo::firstNonHiddenModelRow(int startRow, int endRow) const
{
    while (startRow <= endRow) {
        if (!mappedDM()->dimensionManager()->isRowHidden(startRow))
            return startRow;

        ++startRow;
    }

    return -1;
}

int QicsGridInfo::lastNonHiddenRow(int startRow, int endRow) const
{
    while (endRow >= startRow) {
        if (!mappedDM()->isRowHidden(endRow))
            return endRow;
        --endRow;
    }

    return -1;
}

int QicsGridInfo::firstNonHiddenModelColumn(int startColumn, int endColumn) const
{
    while (startColumn <= endColumn) {
        if (!mappedDM()->dimensionManager()->isColumnHidden(startColumn))
            return startColumn;
        ++startColumn;
    }

    return -1;
}

int QicsGridInfo::firstNonHiddenColumn(int startColumn, int endColumn) const
{
    while (startColumn <= endColumn) {
        if (!mappedDM()->isColumnHidden(startColumn))
            return startColumn;
        ++startColumn;
    }

    return -1;
}

int QicsGridInfo::lastNonHiddenColumn(int startColumn, int endColumn) const
{
    while (endColumn >= startColumn) {
        if (!mappedDM()->isColumnHidden(endColumn))
            return endColumn;
        --endColumn;
    }

    return -1;
}

void QicsGridInfo::reportSelection(Qics::QicsSelectionType stype,
                              int anchor_row, int anchor_col,
                              int cur_row, int cur_col)
{
    if (mySelM) {
        myDoingSelectionFlag = true;

        // #### AM: consider make these types deprecated...
        if (stype == Qics::SelectionDrag || stype == Qics::SelectionExtend)  {
            QicsRegion selRegion(anchor_row, anchor_col, cur_row, cur_col);

            // compute selection region which covers all the spanned cells as well
            computeSelectionRegion(selRegion);

            mySelM->processSelectionEvent(Qics::SelectionReplace,
                selRegion.startRow(), selRegion.startColumn(), selRegion.endRow(), selRegion.endColumn());
        }
        else
            mySelM->processSelectionEvent(stype, anchor_row, anchor_col, cur_row, cur_col);

        myDoingSelectionFlag = false;
    }
}

void QicsGridInfo::computeSelectionRegion(QicsRegion &reg)
{
    QicsSpanManager *spm = myStyleMananager->spanManager();
    if (!spm) return;
    QicsRegion span;

    // we should process each side of reg and see if there is a spanned cell
    // first, we're normalizing this region to have its top/left less then bottom/right
    reg.normalize();

restart:    // it is the point to cycle expanding of the region

    int rtop = reg.startRow();
    int rbottom = reg.endRow();
    int cleft = reg.startColumn();
    int cright = reg.endColumn();
    QicsRegion tmp(reg);

    // top & bottom
    for (int i = cleft; i <= cright; ++i) {
        if (spm->insideSpan(*this, rtop, i, span)) {
            reg |= span;
            if (reg != tmp) goto restart;
        }
        if (spm->insideSpan(*this, rbottom, i, span)) {
            reg |= span;
            if (reg != tmp) goto restart;
        }
    }

    // left & right
    for (int i = rtop; i <= rbottom; ++i) {
        if (spm->insideSpan(*this, i, cleft, span)) {
            reg |= span;
            if (reg != tmp) goto restart;
        }
        if (spm->insideSpan(*this, i, cright, span)) {
            reg |= span;
            if (reg != tmp) goto restart;
        }
    }
}

void QicsGridInfo::handleSelectionChange(bool)
{
    if (!myDoingSelectionFlag) {
        QicsICell invalid_cell(-1, -1);
        setCurrentCell(invalid_cell);
    }
}

void QicsGridInfo::propagateChangesFromCell(int row, int col)
{
    // row and col are in model coordinates here, NOT visual!
    if (!mySelM) return;

    if (mySelM->selectionPolicy() == Qics::SelectSingleRow ||
        mySelM->selectionPolicy() == Qics::SelectMultipleRow ||
        !myAllowPropagateChanges)
        return;

    const QicsSelectionList *slist = mySelM->selectionList(true);
    if (slist && m_dataModel && gridType() == Qics::TableGrid) {
        // no propagation if the source cell is not selected
        if (!slist->isCellSelected(visualRowIndex(row), visualColumnIndex(col)))
            return;

        const QicsDataItem *constitm = m_dataModel->item(row, col);
        if (!constitm)
            return;

        QicsDataItem *sourceitm = constitm->clone();

        bool emits = m_dataModel->emitSignals();
        m_dataModel->setEmitSignals(false);

        if (sourceitm) {
            QicsSelectionList::const_iterator sel, sel_end(slist->constEnd());
            for (sel = slist->constBegin(); sel != sel_end; ++sel) {
                if (sel->isValid() && sel->selected()) {
                    int tr = sel->topRow(), br = sel->bottomRow();
                    int lc = sel->leftColumn(), rc = sel->rightColumn();
                    if (br >= m_dataModel->numRows())
                        br = m_dataModel->numRows()-1;
                    if (rc >= m_dataModel->numColumns())
                        rc = m_dataModel->numColumns()-1;

                    for (int r = tr; r <= br; r++) {
                        int mr = modelRowIndex(r);

                        for (int c = lc; c <= rc; c++) {
                            int mc = modelColumnIndex(c);

                            if (*static_cast<bool*>(myStyleMananager->getCellProperty(mr, mc, QicsCellStyle::ReadOnly)))
                                continue;

                            if (!(*static_cast<bool*>(myStyleMananager->getCellProperty(mr, mc, QicsCellStyle::Enabled))))
                                continue;

                            if (mc != col || mr != row)
                                m_dataModel->setItem(mr, mc, *sourceitm);
                        }
                    }
                }
            }
            delete sourceitm;
        }

        m_dataModel->setEmitSignals(emits);

        redrawAllGrids();
    }
}

void QicsGridInfo::reportFillHandleEvent(Qics::QicsFillHandleEventType type, QMouseEvent *m,
                                    const QicsSelection *initSel, const QicsSelection *curSel)
{
    emit fillHandleEvent(type, m, initSel, curSel);
}


