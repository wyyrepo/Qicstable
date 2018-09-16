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

#ifndef QICSGRIDINFO_H
#define QICSGRIDINFO_H

#include <QObject>
#include <QVector>
#include <QPointer>
#include "QicsNamespace.h"
#include "QicsICell.h"
#include "QicsRegion.h"
#include "QicsSorter.h"
#include "QicsDataModelDefault.h"


class QMouseEvent;
class QMimeData;
class QicsKeyboardManager;
class QicsDataModel;
class QicsStyleManager;
class QicsDimensionManager;
class QicsMappedDimensionManager;
class QicsSelectionManager;
class QicsSelection;
class QicsScreenGrid;
class QicsRowFilter;
class QicsSelectionList;
class QicsDataItem;

/*! \class QicsGridInfo QicsGridInfo.h
*  \brief Information for QicsGrids
*
* QicsGridInfo is a grouping of objects relating to a QicsGrid.
* QicsGridInfo automates the connections that are necesary between
* a grid and its associated objects.  These objects include a
* QicsDataModel (used to help determine the size of the grid, as well
* as to retrieve the data to draw in the grid), a QicsStyleManager
* (used to retrieve visual and interactive information about the cells
* in the grid), and a QicsDimensionManager (used to determine the
* dimensions of the cells in the grid).
*
* QicsGridInfo connects individual grid widgets to each of the above
* objects, and notifies the grids if any of the objects have changed.
*
* Most %QicsTable users should never need to use a QicsGridInfo object.
* These objects are automatically created by QicsTable and are used
* internally by the table components.  If you are subclassing a grid
* or a cell display object, you may need to use this class.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsGridInfo : public QObject, public Qics
{
    Q_OBJECT
public:
    /*!
    * A vector of pointers to QicsScreenGrid objects.
    */
    typedef QVector<QicsScreenGrid *> QicsScreenGridPV;

    /*!
    * \internal
    * The constructor.  The QicsGridInfo object is initially "empty".
    * The programmer must set the appropriate objects before attempting
    * to connect any grids.  The \a type argument specifies whether this
    * object is used for row headers, column headers, or the main table.
    */
    QicsGridInfo(QicsGridType type, QObject *parent = 0);
    /*!
    * \internal
    * The destructor.
    */
    virtual ~QicsGridInfo();

    /*!
    * Returns the type of this object.
    */
    inline QicsGridType gridType() const { return myType; }

    /*!
    * Returns the current data model object that has been assigned
    * to this object.
    */
    inline QicsDataModel *dataModel() const {return m_dataModel;}
    /*!
    * \internal
    * Sets the data model object that will be used by all grids
    * that connect to this grid info object.
    */
    void setDataModel(QicsDataModel *dm);

    /*!
    * \internal
    * Returns the current style manager object that has been assigned
    * to this object.
    */
    inline QicsStyleManager *styleManager() const { return myStyleMananager; }
    /*!
    * \internal
    * Sets the style manager object that will be used by all grids
    * that connect to this grid info object.
    */
    void setStyleManager(QicsStyleManager *sm);

    /*!
    * \internal
    * Returns the current mapped dimension manager object that has
    *  been assigned to this object.
    */
    inline QicsMappedDimensionManager *mappedDM() const { return myMappedDM; }

    /*!
    * \internal
    * Returns the current dimension manager object that has been assigned
    * to this object.
    */
    inline QicsDimensionManager *dimensionManager() const { return myDimensionManager; }

    /*!
    * \internal
    * Sets the dimension manager object that will be used by all grids
    * that connect to this grid info object.
    */
    void setDimensionManager(QicsDimensionManager *dm);

    /*!
    * \internal
    * Returns the current selection manager object that has been assigned
    * to this object.
    */
    inline QicsSelectionManager *selectionManager() const { return mySelM; }
    /*!
    * \internal
    * Sets the selection manager object that will be used by all grids
    * that connect to this grid info object.
    */
    void setSelectionManager(QicsSelectionManager *sel_m);

    /*!
    * \internal
    * Returns the current keyboard manager object that has been assigned
    * to this object.
    */
    inline QicsKeyboardManager *keyboardManager() const { return myKeyM; }

    /*!
    * \internal
    * Sets the keyboard manager object that will be used by all grids
    * that connect to this grid info object.
    */
    inline void setKeyboardManager(QicsKeyboardManager *key_m) {myKeyM = key_m;}

    /*!
    * Returns a copy of the list of grid widgets that are currently connected
    * to this grid info object.
    */
    inline QicsScreenGridPV grids() const { return myGrids; }
    /*!
    * \internal
    * Connects grid widget \a grid to the data model, style manager, and
    * dimension manager objects that are currently assigned
    * to this object.
    */
    void connectGrid(QicsScreenGrid *grid);
    /*!
    * \internal
    * Disconnects grid widget \a grid from the data model, style manager,
    * and dimension manager objects that are currently assigned
    * to this object.
    */
    void disconnectGrid(QicsScreenGrid *grid);

    /*!
    * Returns the value of cell (\a row, \a col ) [visual coordinates]
    * in the data model object contained in this object, after mapping
    * to model coordinates.  If the grid info object is \b RowHeader
    * and there is no value returned from the data model, returns a
    * QicsDataInt object containing \a row.
    * If the grid info object is \b ColumnHeader
    * and there is no value returned from the data model, returns
    * a QicsDataInt object containing \a col.
    *
    */
    virtual const QicsDataItem *cellValue(int row, int col) const;

    /*!
    * Sets default path for pixmaps to \a path. Pixmaps will be searched at this path on load.
    * \sa defaultImagePath();
    * \since 2.3
    */
    void setDefaultImagePath(const QString &path);
    /*!
    * Returns default path for pixmaps to \a path.
    * \sa setDefaultImagePath();
    * \since 2.3
    */
    inline const QString& defaultImagePath() const { return myDefaultImagePath; }

    /*!
    * Returns the current repaint behavior of all grids in this table.
    */
    Qics::QicsRepaintBehavior gridRepaintBehavior() const;

    /*!
    * Set the repaint behavior of all grids in this table.  Emits a signal
    * that requests a change in global repaint behavior.
    */
    void setGlobalRepaintBehavior(QicsRepaintBehavior behavior);
    /*!
    * Revert to the previous repaint behavior for all grids in this table.
    * Emits a signal that requests a revert of global repaint behavior.
    */
    void revertGlobalRepaintBehavior();

    /*!
    * Set the repaint behavior of all grids using this object.  Also, stores
    * the previous repaint behavior value for use in revertGridRepaintBehavior.
    */
    void setGridRepaintBehavior(QicsRepaintBehavior behavior);
    /*!
    * Revert to the previous repaint behavior for all grids using this object.
    */
    void revertGridRepaintBehavior();

    /*!
    * \internal
    * Causes this table to become the master for columns in another \a grid.
    * That is, if columns are added/deleted or reordered, then the other
    * grid should follow suit.   This kind of coupling should be done
    * when multiple parts of a data model must be loaded into separate
    * grids, but they all must logically have the same number of columns.
    * An example would be a grid of data, and a corresponding grid of
    * column annotations.
    */
    virtual void controlColumnsOf(QicsGridInfo *grid);

    /*!
    * \internal
    * Causes this grid to become the master for rows in another \a grid.
    * That is, if rows are added/deleted or reordered, then the other grid
    * should follow suit.   See controlColumnsOf for a rationale.
    */
    virtual void controlRowsOf(QicsGridInfo *grid);

    /*!
    * Returns the physical column index into the data model for
    * a given visual column
    */
    inline int modelColumnIndex(int column) const;

    /*!
    * Returns the physical row index into the data model for
    * a given visual row
    */
    inline int modelRowIndex(int row) const;

    /*!
    * Returns the visual column index for a given model index
    */
    inline int visualColumnIndex(int column) const;

    /*!
    * Returns visual row index for a given model index
    */
    inline int visualRowIndex(int row) const;

    /*!
    * Returns a region that contains model row and column indices
    * based on the visual region \a vis_region.
    */
    QicsRegion modelRegion(const QicsRegion &vis_region) const;

    /*!
    * Returns a region that contains visual row and column indices
    * based on the model coordinate region \a vis_region.
    */
    QicsRegion visualRegion(const QicsRegion &model_region) const;

    /*!
    * \internal
    * Reorder the presentation of the rows of a data model based
    * on a columns.
    * Note that the data model itself is not changed, only the
    * presentation order shown in this grid and it's slaves.
    * \param columns	the column to sort on
    * \param order	sort direction
    * \param from	start point
    * \param to	end point.  default (-1) means to end
    * \param func	user defined comparator function.
    */
    void orderRowsBy( const QVector<int> &columns,
        QicsSortOrder order = Qics::Ascending,
        int from = 0, int to = -1,
        DataItemComparator func = 0,
        bool models = false);

    /*!
    * \internal
    * Reorder the presentation of the columns of the data model
    * based on a rows.
    * Note that the data model itself is not changed, only the
    * presentation order shown in this grid and it's slaves.
    * \param rows	the row to sort on
    * \param order	sort direction
    * \param from	start point
    * \param to	end point.  default (-1) means to end
    * \param func	user defined comparator function.
    */
    void orderColumnsBy(const QVector<int> &row,
        QicsSortOrder order = Qics::Ascending,
        int from = 0, int to = -1,
        DataItemComparator func = 0,
        bool models = false);

    /*!
    * \internal
    * Returns the row ordering sort object.
    */
    inline QicsSorter *rowOrdering() const { return m_rowOrdering; }
    /*!
    * \internal
    * Returns the column ordering sort object.
    */
    inline QicsSorter *columnOrdering() const { return m_columnOrdering; }

    /*!
    * Sets case sensitivity while sorting to \a cs.
    * \since 2.3
    */
    void setSortingSensitivity(Qt::CaseSensitivity cs);

    /*!
    * \internal
    * Sets default sorter delegate to \a sorter.
    * \since 2.4.2
    */
    void setSorterDelegate(QicsAbstractSorterDelegate *sorter);

    /*!
    * \internal
    * Returns default sorter delegate.
    * \since 2.4.2
    */
    inline QicsAbstractSorterDelegate *sorterDelegate() const
    {return m_defaultSorterDelegate;}



    /*!
    * Sets copying policy during copy/paste and drag/drop operations to \a policy.
    * \a policy should be bitwise 'or' of QicsCopyPolicy values.
    * \since 2.2
    * \sa copyPolicy()
    */
    inline void setCopyPolicy(Qics::QicsCopyPolicy policy) {m_copyPolicy = policy;}

    /*!
    * Returns current copying policy during copy/paste and drag/drop operations
    * as bitwise 'or' of QicsCopyPolicy values.
    * \since 2.2
    * \sa setCopyPolicy()
    */
    inline Qics::QicsCopyPolicy copyPolicy() const {return m_copyPolicy;}

    /*!
    * Sets clearing policy during cut and delete operations to \a policy.
    * \a policy should be bitwise 'or' of QicsClearPolicy values.
    * \since 2.2
    * \sa clearPolicy()
    */
    inline void setClearPolicy(int policy) {myClearPolicy = policy;}

    /*!
    * Returns current clearing policy during cut and delete operations
    * as bitwise 'or' of QicsClearPolicy values.
    * \since 2.2
    * \sa setClearPolicy()
    */
    inline int clearPolicy() const {return myClearPolicy;}

    /*!
    * Returns current header drag style
    * \since 2.4
    * \sa setHeaderDragStyle()
    */
    inline QicsHeaderDragStyle headerDragStyle() const {return myHeaderDragStyle;}

    /*!
    * Returns the current cell of the grids associated with this object.
    * Note that if there is no current cell (i.e. the user has not selected
    * a cell) this method will return an invalid cell.
    */
    QicsICell currentCell(bool model = false) const;

    /*!
    * Sets the current cell of the grids associated with this object.
    */
    virtual void setCurrentCell(const QicsICell &cell);

    /*!
    * Returns the visual index of the first non hidden row in the
    * region from \a startRow to \a endRow.
    */
    int firstNonHiddenRow(int startRow, int endRow) const;
    /*!
    * Returns the model index of the first non hidden row in the
    * region from \a startRow to \a endRow.
    */
    int firstNonHiddenModelRow(int startRow, int endRow) const;
    /*!
    * Returns the visual index of the last non hidden row in the
    * region from \a startRow to \a endRow.
    */
    int lastNonHiddenRow(int startRow, int endRow) const;
    /*!
    * Returns the visual index of the first non hidden column in the
    * region from \a startColumn to \a endColumn.
    */
    int firstNonHiddenColumn(int startColumn, int endColumn) const;
    /*!
    * Returns the model index of the first non hidden column in the
    * region from \a startColumn to \a endColumn.
    */
    int firstNonHiddenModelColumn(int startColumn, int endColumn) const;
    /*!
    * Returns the visual index of the last non hidden column in the
    * region from \a startColumn to \a endColumn.
    */
    int lastNonHiddenColumn(int startColumn, int endColumn) const;

    /*!
    * \internal
    * Returns the ignoreModelSizeChange flag so that style and
    * dimension managers may ignore signals from the model.
    */
    inline bool ignoreModelSizeChanges() const {return myIgnoreModelSizeChanges;}

    /*!
    * \internal
    * Returns the allowPropagateChanges flag identifying that
    * propagating changes from editing cell to whole selection
    * enabled or disabled.
    */
    inline bool allowPropagateChanges() const {return myAllowPropagateChanges;}

    /*!
    * \internal
    * Returns the autoHeaderNamingMask variable which defines
    * the default appearance of unnamed header cells
    */
    inline QString autoHeaderNamingMask() const {return myAutoHeaderNamingMask;}

    /*!
    * \internal
    * Returns \b true if selection in a middle, otherwise \b false.
    */
    inline bool selectionInProgress() const {return myDoingSelectionFlag;}

    /*!
    * \internal
    * Report a user-initiated selection to the selection manager.
    */
    virtual void reportSelection(QicsSelectionType stype,
        int anchor_row, int anchor_col,
        int cur_row, int cur_col);

    /*!
    * \internal
    * Reports fill handle events.
    */
    void reportFillHandleEvent(Qics::QicsFillHandleEventType type, QMouseEvent *,
        const QicsSelection *, const QicsSelection *);

    /*!
    * \internal
    * Returns true if selection signals are blocked.
    */
    inline bool blockSelectionSignals()const {return myBlockSelectionSignals;}

    /*!
    * \internal
    * Sets blocking of selection signals to \a block.
    */
    inline void setBlockSelectionSignals(bool block) {myBlockSelectionSignals = block;}

    /*!
    * \internal
    * Computes a region around all the spanned cells inside \a reg.
    * \since 2.2
    */
    void computeSelectionRegion(QicsRegion &reg);

    /* FILTERING STUFF
    */

    /*!
    * \internal
    * Returns row filter manager.
    */
    inline QicsRowFilter *rowFilter() const { return m_rowFilter; }


signals:
    /*!
    * \internal
    * Signaled when a new data model has been assigned to this grid
    * info object.  \a old_dm is the previous data model,
    * \a new_dm is the new data model.
    */
    void dataModelChanged(QicsDataModel *old_dm, QicsDataModel *new_dm);
    /*!
    * \internal
    * Signaled when a new style manager has been assigned to this grid
    * info object.  \a old_sm is the previous style manager,
    * \a new_sm is the new style manager.
    */
    void styleManagerChanged(QicsStyleManager *old_sm,
        QicsStyleManager *new_sm);
    /*!
    * \internal
    * Signaled when a new dimension manager has been assigned to this grid
    * info object.  \a old_sm is the previous dimension manager,
    * \a new_sm is the new dimension manager.
    */
    void dimensionManagerChanged(QicsDimensionManager *old_dm,
        QicsDimensionManager *new_dm);
    /*!
    * \internal
    * Signaled when a new selection manager has been assigned to this grid
    * info object.  \a old_sm is the previous selection manager,
    * \a new_sm is the new selection manager.
    */
    void selectionManagerChanged(QicsSelectionManager *old_sm,
        QicsSelectionManager *new_sm);

    /*!
    * Signaled when a grid is added to the grid collection for this object.
    */
    void gridAdded(QicsScreenGrid *grid);

    /*!
    * Signaled when a grid is added from the grid collection for this object.
    */
    void gridRemoved(QicsScreenGrid *grid);

    /*!
    * Signaled as a request to the table widget to change the repaint behavior
    * for all grids in the table.  This is commonly used by QicsRow, QicsColumn
    * and QicsCell objects when they are performing complex changes that
    * would otherwise result in many grid recalculations and redraws.
    */
    void globalSetRepaintBehaviorRequest(QicsRepaintBehavior behavior);
    /*!
    * Signaled as a request to the table widget to revert the repaint behavior
    * all grids in the table to the value they had previous to the last
    * repaint behavior change request.  This is commonly used by QicsRow,
    * QicsColumn and QicsCell objects when they are performing complex changes
    * that would otherwise result in many grid recalculations and redraws.
    */
    void globalRevertRepaintBehaviorRequest();

    /*!
    * Signaled when the model is reordered in some way.  Most grids should
    * recompute things at that point
    */
    void modelReordered(Qics::QicsIndexType);

    /*!
    * This signal is emitted when the current cell of the grids associated
    * with this object changes.  The cell (\a new_row, \a new_col ) is
    * the new current cell (in \b visual coordinates ).
    */
    void currentCellChanged(int new_row, int new_col);

    /*!
    * This signal is emitted when the value of a cell in a grid associated
    * with this object changes.  The cell (\a row, \a col ) is
    * expressed in \b visual coordinates .
    */
    void cellValueChanged(int row, int col);

    /*!
    * This signal is emitted when traversed to other cell.
    */
    void uneditRequest();

    /*!
    * This signal is emitted when rows moved.
    * See \b moveRows() for more information.
    */

    void rowsMoved(int target, const QVector<int> &rows);

    /*!
    * This signal is emitted when columns moved.
    * See \b moveColumns() method for more information.
    */

    void columnsMoved(int target, const QVector<int> &columns);

    /*!
    * \internal
    * This signal is emitted via reportFillHandleEvent method
    * when fill handle is pressed/released/moved etc.
    */
    void fillHandleEvent(QicsFillHandleEventType, QMouseEvent *, const QicsSelection *, const QicsSelection *);

    /*!
    * \internal
    * This signal is emitted when repainting behavior for grid set to RepaintOn.
    * See \b setGridRepaintBehavior() and gridRepaintBehavior() methods for more information.
    */
    void gridRepaintingEnabled();

    /*!
    * \internal
    *  Emitted when a filter was set or removed.
    */
    void filterChanged(int index, bool set);

public slots:
    /*!
    * \internal
    * deleteRows is used to delete rows from the Grid (and subsequently
    * from the data model) in view coordinates.  That is, the deletion
    * takes place through the data models ordering vector, so that
    * the actual rows deleted may be non-contiguous.   Generally,
    * code on the GUI side will use this interface.  Code which talks
    * directly to the data model (e.g. callbacks from a live data feed)
    * should delete rows with the appropriate calls directly to the
    * QicsDataModel.
    */
    void deleteRows(int how_many, int starting_position);

    /*!
    * \internal
    * insertRow is called by the application to open up a new row
    * if where < 0, row is inserted at the end.
    */
    void insertRow(int where);

    /*!
    * \internal
    * insertRows is called by the application to open up a new rows
    * if where < 0, rows is inserted at the end.
    */
    void insertRows(int where, int count);

    /*!
    * \internal
    * change the ordering of a set of rows (possibly
    * discontiguous, to a new location before the row \a target.
    */
    void moveRows(int target, const QVector<int> &rows);

    /*!
    * \internal
    * change the ordering of a set of columns (possibly
    * discontiguous, to a new location before the columns \a target.
    */
    void moveColumns(int target, const QVector<int> &cols);

    /*!
    * \internal
    * deleteColumns is used to delete columns from the Grid (and subsequently
    * from the data model) in view coordinates.  That is, the deletion
    * takes place through the data models ordering vector, so that
    * the actual columns deleted may be non-contiguous.   Generally,
    * code on the GUI side will use this interface.  Code which talks
    * directly to the data model (e.g. callbacks from a live data feed)
    * should delete columns with the appropriate calls directly to the
    * QicsDataModel.
    */
    void deleteColumns(int how_many, int starting_position);

    /*!
    * \internal
    * insertColumn is called by the application to open up a new column
    * if where < 0, column is inserted at the end.
    */
    void insertColumn(int where);

    /*!
    * \internal
    * insertColumns is called by the application to open up a new columns
    * if where < 0, columns is inserted at the end.
    */
    void insertColumns(int where, int count);

    /*!
    * \internal
    * Recompute and redraw all the connected grids. Needed mainly
    * after sorting the data.
    */
    void redrawAllGrids(bool reset = false);

    /*!
    * \internal
    * Updates all the connected grids.
    * \since 2.3
    */
    void updateAllGrids(bool redraw);

    /*!
    * Recompute and redraw vertical or horizontal grids.
    */
    void redrawAllGrids(Qics::QicsIndexType indexType);

    /*!
    * \internal
    * Tell me that a region of the date model changed
    */
    virtual void redrawModel(const QicsRegion &r, bool model = true);

    /*!
    * \internal
    * Sets the value of the appropriate cell of this object's data model.
    *
    * \arg row row index of the cell to set in \b visual coordinates
    * \arg col column index of the cell to set in \b visual coordinates
    * \arg itm new value
    */
    virtual void setCellValue(int row, int col, const QicsDataItem &itm);

    /*!
    * \internal
    * Sets the value of the grid's current cell in this object's data model
    * to \a itm.
    */
    virtual void setCurrentCellValue(const QicsDataItem &itm);

    /*!
    * \internal
    * Sets the ignoreModelSizeChanges flag so that style and dimension managers
    * may ignore signals from the model.
    */
    inline void setIgnoreModelSizeChanges(bool b) {myIgnoreModelSizeChanges = b;}

    /*!
    * \internal
    * Sets the allowPropagateChanges flag enabling or disabling propagating
    * changes of edited cell to whole selection
    */
    inline void setAllowPropagateChanges(bool b) {myAllowPropagateChanges = b;}

    /*!
    * \internal
    * Sets the autoHeaderNamingMask variable defining the default
    * appearance of unnamed header cells
    */
    inline void setAutoHeaderNamingMask(QString mask)  {myAutoHeaderNamingMask = mask;}

    /*!
    * \internal
    * Sets header drag style to \a style
    * \since 2.4
    * \sa headerDragStyle()
    */
    inline void setHeaderDragStyle(const QicsHeaderDragStyle& style) {myHeaderDragStyle = style;}

protected slots:
    /*!
    * \internal
    * Removes a grid from the grid list
    */
    void removeGrid(QObject *obj);

    /*!
    * \internal
    * Called when the selection changes.  We may have to invalidate the
    * current cell when this happens.
    */
    void handleSelectionChange(bool in_progress);

    /*!
    * \internal
    * Called to propagate changes from one cell to whole selection.
    */
    void propagateChangesFromCell(int row, int col);

protected:
    /*!
    * \internal
    * Connects the current data model to the current style manager.
    */
    void connectDTtoSM();
    /*!
    * \internal
    * Connects the current data model to the current dimension manager.
    */
    void connectDTtoDM();

    /*!
    * \internal
    * Connects the current data model to grid widget \a grid.
    */
    void connectDTtoGrid(QicsScreenGrid *grid);
    /*!
    * \internal
    * Connects the current style manager to grid widget \a grid.
    */
    void connectSMtoGrid(QicsScreenGrid *grid);
    /*!
    * \internal
    * Connects the current dimension manager to grid widget \a grid.
    */
    void connectDMtoGrid(QicsScreenGrid *grid);
    /*!
    * \internal
    * Connects the current selection manager to grid widget \a grid.
    */
    void connectSelMtoGrid(QicsScreenGrid *grid);

    /*!
    * \internal
    * Reorganizes cell spans after rows/columns have been moved.
    */
    void reorganizeSpans(QicsSorter *sorter, int target, const QVector<int> &items);

private:
    /*!
    * \internal
    * the type of this grid info
    */
    QicsGridType myType;

    /*!
    * \internal
    * the data model/table object
    */
    QPointer<QicsDataModel> m_dataModel;

    /*!
    * \internal
    * the style manager object
    */
    QicsStyleManager *myStyleMananager;

    /*!
    * \internal
    * the dimension manager object
    */
    QicsDimensionManager *myDimensionManager;

    /*!
    * \internal
    * a dimension manager object which goes through my row/col mapping
    */
    QicsMappedDimensionManager *myMappedDM;

    /*!
    * \internal
    * the style manager object
    */
    QicsSelectionManager *mySelM;

    /*!
    * \internal
    * vector of connected grid widgets
    */
    QicsScreenGridPV myGrids;

    /*!
    * \internal
    * cell value created by cellValue()
    */
    QicsDataString myCellValue;

    /*!
    * \internal
    * the previous value of this grid's repaint behavior
    */

    QicsRepaintBehavior myPrevGridRepaintBehavior;

    /*!
    * \internal
    * Holds the current cell.
    */
    QicsICell myCurrentCell;

    /*!
    * \internal
    * flag signifying if we are in the middle of a selection
    */
    bool myDoingSelectionFlag;

    /*!
    * \internal
    * This variable specifies the appearance of unnamed header cells
    */
    QString myAutoHeaderNamingMask;

    QicsKeyboardManager *myKeyM;

    /*!
    * \internal
    * The vector which controls presentation order of my rows
    */
    QPointer<QicsSorter> m_rowOrdering;

    /*!
    * \internal
    * The vector which controls presentation order of my rows
    */
    QPointer<QicsSorter> m_columnOrdering;

    /*!
    * \internal
    * Default sorter delegate
    */
    QicsAbstractSorterDelegate *m_defaultSorterDelegate;

    /*!
    * \internal
    * Who really owns my row ordering vector
    */
    QPointer<QicsGridInfo> m_rowOrderOwner;

    /*!
    * \internal
    * Who really owns column ordering vector
    */
    QPointer<QicsGridInfo> m_columnOrderOwner;

    /*!
    * \internal
    * should style and dimension managers ignore signals from model?
    */
    bool myIgnoreModelSizeChanges;

    /*!
    * \internal
    * flag signifying if propagating changes to whole selection is allowed
    */
    bool myAllowPropagateChanges;

    QicsRowFilter *m_rowFilter;

    bool myBlockSelectionSignals;

    /*! \internal
    * denotes current copying policy
    */
    Qics::QicsCopyPolicy m_copyPolicy;
    /*! \internal
    * denotes current clear policy
    */
    int myClearPolicy;

    /*! \internal
    * Stores default path used for loading images
    */
    QString myDefaultImagePath;

    /*! \internal
    * denotes current header drag style
    */
    QicsHeaderDragStyle myHeaderDragStyle;
};


inline int QicsGridInfo::modelColumnIndex(int column) const
{
    return m_columnOrdering ? m_columnOrdering->visualToModel(column) : -1;
}

inline int QicsGridInfo::modelRowIndex(int row) const
{
    return m_rowOrdering ? m_rowOrdering->visualToModel(row) : -1;
}

inline int QicsGridInfo::visualColumnIndex(int column) const
{
    return m_columnOrdering ? m_columnOrdering->modelToVisual(column) : -1;
}

inline int QicsGridInfo::visualRowIndex(int row) const
{
    return m_rowOrdering ? m_rowOrdering->modelToVisual(row) : -1;
}

#endif //QICSGRIDINFO_H


