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

#ifndef QICSTABLECOMMON_H
#define QICSTABLECOMMON_H

#include <QVector>
#include "QicsGridCommon.h"
#include "QicsMainGrid.h"

class QicsRow;
class QicsColumn;
class QicsRepeatingRow;
class QicsRepeatingColumn;
class QicsRowHeader;
class QicsColumnHeader;
class QicsCell;

/*!
* \class QicsTableCommon QicsTableCommon.h
*  \nosubgrouping
* \brief Superclass defining table-wide attributes and settings
*
* QicsTableCommon is a class defining table-wide attributes and settings.
* It is not meant to be instantiated directly.  Rather, it serves as a
* superclass for QicsTablePrint, and is used by QicsTable.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsTableCommon: public QicsGridCommon
{
    Q_OBJECT

    friend class QicsTable;
#ifndef QICSTABLE_GPL
    friend class QicsTablePrint;
#endif
public:
    Q_ENUMS( QicsTableDisplayOption )

    /** @name Common Table Properties
    */

    //@{
    /*!
    * Specifies whether the top header is visible.  The default value
    * of this property is \b DisplayAlways.
    */
    Q_PROPERTY( QicsTableDisplayOption topHeaderVisible READ topHeaderVisible WRITE setTopHeaderVisible )

    /*!
    * Specifies whether the bottom header is visible. The default value
    * of this property is \b DisplayNever.
    */
    Q_PROPERTY( QicsTableDisplayOption bottomHeaderVisible READ bottomHeaderVisible WRITE setBottomHeaderVisible )

    /*!
    * Specifies whether the left header is visible. The default value
    * of this property is \b DisplayAlways.
    */
    Q_PROPERTY( QicsTableDisplayOption leftHeaderVisible READ leftHeaderVisible WRITE setLeftHeaderVisible )

    /*!
    * Specifies whether the right header is visible. The default value
    * of this property is \b DisplayNever.
    */
    Q_PROPERTY( QicsTableDisplayOption rightHeaderVisible READ rightHeaderVisible WRITE setRightHeaderVisible )

    /*!
    * Specifies the number of pixels between the edge of the table widget and
    * its subwidgets (title widgets, headers, etc).
    * The default value of this property is \b 10.
    */
    Q_PROPERTY( int tableMargin READ tableMargin WRITE setTableMargin )

    /*!
    * Specifies the number of pixels between each of the table widget's
    * elements (title widgets, corner widgets, etc).  This property does
    * not control the spacing between grids in the table.  Use
    * \link #gridSpacing "gridSpacing"\endlink in that case.
    * The default value of this property is \b 10.
    */
    Q_PROPERTY( int tableSpacing READ tableSpacing WRITE setTableSpacing )

    /*!
    * Specifies the number of pixels between each of the grids in the
    * table widget.  This property does not control the spacing between
    * other element in the table.  Use
    * \link #tableSpacing "tableSpacing"\endlink in that case.
    * The default value of this property is \b 0.
    */
    Q_PROPERTY( int gridSpacing READ gridSpacing WRITE setGridSpacing )

    /*!
    * Specifies whether the values in the first row of the table's
    * associated QicsDataModel should be interpreted as column header data.  If this
    * value is \a true, the value of cell(\a 0, \a colnum) will be shown as the column
    * header for column \a colnum, and row 0 will never be shown as data in the table.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool columnHeaderUsesModel READ columnHeaderUsesModel WRITE setColumnHeaderUsesModel )

    /*!
    * Specifies whether the values in the first column of the table's
    * associated QicsDataModel should be interpreted as row header data.  If this
    * value is \a true, the value of cell(\a rownum, \a 0) will be shown as the row
    * header for row \a rownum, and column 0 will never be shown as data in the table.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool rowHeaderUsesModel READ rowHeaderUsesModel WRITE setRowHeaderUsesModel )

    /*!
    * Specifies whether the table should ignore changes to the size of the model.
    * This can be very useful when you are clearing all rows and columns from
    * the table, but you wish to retain model attribute settings on the table
    * display.  If this value is \b true, these settings will be retained.
    * If this value is \b false, row and column addition and deletion will
    * result in the appropriate changes to the attribute settings and row and
    * column widths of the table.
    */
    Q_PROPERTY( bool ignoreModelSizeChanges READ ignoreModelSizeChanges WRITE setIgnoreModelSizeChanges )
    //@}

public:
    /*!
    * \internal
    * Constructer for the class.
    */
    QicsTableCommon(QObject *parent = 0, bool forward_signals = false);

    /*!
    * \internal
    * Destructor for the class.
    */
    virtual ~QicsTableCommon();

    /////////////////// Data Model methods

    /*!
    * Returns the data model object associated with this table.
    * \sa setDataModel()
    */
    QicsDataModel *dataModel() const;
    /*!
    * Sets the data model object associated with this table.
    * \sa dataModel()
    */
    virtual void setDataModel(QicsDataModel *dm);

    /////////////////// Grid Access classes

    /*!
    * Returns a reference to an internal flyweight QicsRow object
    * that refers to row \a rownum.
    * \sa row()
    */
    QicsRow &rowRef(int rownum);
    /*!
    * Returns a \b const reference to an internal flyweight QicsRow object
    * that refers to row \a rownum.
    * \sa row()
    */
    const QicsRow &rowRef(int rownum) const;

    /*!
    * Returns a newly allocated QicsRow object for row \a rownum.
    * If \a follow_model is \b true, the row object will always refer to the
    * row in the data model that is currently being displayed in
    * row \a rownum, even if rows of the table are reordered.
    * If follow_model is \b false, the row will always refer to
    * the visual row \a rownum, but the underlying row in the
    * data model may change.
    * \sa rowRef()
    */
    QicsRow *row(int rownum, bool follow_model = true);
    /*!
    * Returns a newly allocated \b const QicsRow object for row \a rownum.
    * If \a follow_model is \b true, the row object will always refer to the
    * row in the data model that is currently being displayed in
    * row \a rownum, even if rows of the table are reordered.
    * If follow_model is \b false, the row will always refer to
    * the visual row \a rownum, but the underlying row in the
    * data model may change.
    * \sa rowRef()
    */
    const QicsRow *row(int rownum, bool follow_model = true) const;

    /*!
    * Returns a reference to an internal flyweight QicsColumn object
    * that refers to column \a colnum.
    * \sa column()
    */
    QicsColumn &columnRef(int colnum);
    /*!
    * Returns a \b const reference to an internal flyweight QicsColumn object
    * that refers to column \a colnum.
    * \sa column()
    */
    const QicsColumn &columnRef(int colnum) const;

    /*!
    * Returns a newly allocated QicsColumn object for column \a colnum.
    * If \a follow_model is \b true, the column object will always refer to the
    * column in the data model that is currently being displayed in
    * column \a colnum, even if columns of the table are reordered.
    * If follow_model is \b false, the column will always refer to
    * the visual column \a colnum, but the underlying column in the
    * data model may change.
    * \sa columnRef()
    */
    QicsColumn *column(int colnum, bool follow_model = true);
    /*!
    * Returns a newly allocated \b const QicsColumn object for column \a colnum.
    * If \a follow_model is \b true, the column object will always refer to the
    * column in the data model that is currently being displayed in
    * column \a colnum, even if columns of the table are reordered.
    * If follow_model is \b false, the column will always refer to
    * the visual column \a colnum, but the underlying column in the
    * data model may change.
    * \sa columnRef()
    */
    const QicsColumn *column(int colnum, bool follow_model = true) const;

    /*!
    * Returns a reference to an internal flyweight QicsRepeatingRow object
    * that refers to a repeating sequence starting at \a start_row and
    * repeating each \a interval rows.
    * \sa repeatingRow()
    */
    QicsRepeatingRow &repeatingRowRef(int start_row, unsigned int interval);
    /*!
    * Returns a \b const reference to an internal flyweight QicsRepeatingRow
    * object that refers to a repeating sequence starting at \a start_row and
    * repeating each \a interval rows.
    * \sa repeatingRow()
    */
    const QicsRepeatingRow &repeatingRowRef(int start_row,
        unsigned int interval) const;

    /*!
    * Returns a newly allocated QicsRepeatingRow object for
    * a repeating sequence starting at \a start_row and repeating each
    * \a interval rows.
    * \sa repeatingRowRef()
    */
    QicsRepeatingRow *repeatingRow(int start_row, unsigned int interval);
    /*!
    * Returns a newly allocated \b const QicsRepeatingRow object for
    * a repeating sequence starting at \a start_row and repeating each
    * \a interval rows.
    * \sa repeatingRowRef()
    */
    const QicsRepeatingRow *repeatingRow(int start_row,
        unsigned int interval) const;

    /*!
    * Returns a reference to an internal flyweight QicsRepeatingColumn object
    * that refers to a repeating sequence starting at \a start_col and
    * repeating each \a interval columns.
    * \sa repeatingColumn()
    */
    QicsRepeatingColumn &repeatingColumnRef(int start_col,
        unsigned int interval);
    /*!
    * Returns a \b const reference to an internal flyweight QicsRepeatingColumn
    * object that refers to a repeating sequence starting at \a start_col and
    * repeating each \a interval columns.
    * \sa repeatingColumn()
    */
    const QicsRepeatingColumn &repeatingColumnRef(int start_col,
        unsigned int interval) const;

    /*!
    * Returns a newly allocated QicsRepeatingColumn object for
    * a repeating sequence starting at \a start_col and repeating each
    * \a interval columns.
    * \sa repeatingColumnRef()
    */
    QicsRepeatingColumn *repeatingColumn(int start_col,
        unsigned int interval);
    /*!
    * Returns a newly allocated \b const QicsRepeatingColumn object for
    * a repeating sequence starting at \a start_col and repeating each
    * \a interval columns.
    * \sa repeatingColumnRef()
    */
    const QicsRepeatingColumn *repeatingColumn(int start_col,
        unsigned int interval) const;

    /*!
    * Returns a reference to an internal flyweight QicsCell object
    * that refers to cell (\a rownum, \a colnum).
    * \sa cell()
    */
    QicsCell &cellRef(int rownum, int colnum);
    /*!
    * Returns a \b const reference to an internal flyweight QicsCell object
    * that refers to cell (\a rownum, \a colnum).
    * \sa cell()
    */
    const QicsCell &cellRef(int rownum, int colnum) const;

    /*!
    * Returns a newly allocated QicsCell object for cell (\a rownum, \a colnum).
    * If \a follow_model is \b true, the cell object will always refer to the
    * cell in the data model that is currently being displayed in
    * cell (\a rownum, \a colnum), even if rows and columns of the table are
    * reordered.  If follow_model is \b false, the cell will always refer to
    * the visual cell (\a rownum, \a colnum), but the underlying cell in the
    * data model may change.
    * \sa cellRef()
    */
    QicsCell *cell(int rownum, int colnum, bool follow_model = true);
    /*!
    * Returns a newly allocated \b const QicsCell object for cell (\a rownum, \a colnum).
    * If \a follow_model is \b true, the cell object will always refer to the
    * cell in the data model that is currently being displayed in
    * cell (\a rownum, \a colnum), even if rows and columns of the table are
    * reordered.  If follow_model is \b false, the cell will always refer to
    * the visual cell (\a rownum, \a colnum), but the underlying cell in the
    * data model may change.
    * \sa cellRef()
    */
    const QicsCell *cell(int rownum, int colnum, bool follow_model = true) const;

    /*!
    * Returns a reference to an internal flyweight QicsMainGrid object.
    * \sa mainGrid()
    */
    QicsMainGrid &mainGridRef();
    /*!
    * Returns a \b const reference to an internal flyweight QicsMainGrid object.
    * \sa mainGrid()
    */
    const QicsMainGrid &mainGridRef() const;

    /*!
    *  Returns a newly allocated main grid object.
    * \sa mainGridRef()
    */
    QicsMainGrid *mainGrid();
    /*!
    *  Returns a newly allocated \b const main grid object.
    * \sa mainGridRef()
    */
    const QicsMainGrid *mainGrid() const;

    /*!
    * Returns a reference to an internal flyweight QicsRowHeader object.
    * \sa rowHeader()
    */
    QicsRowHeader &rowHeaderRef();
    /*!
    * Returns a \b const reference to an internal flyweight QicsRowHeader object.
    * \sa rowHeader()
    */
    const QicsRowHeader &rowHeaderRef() const;

    /*!
    *  Returns a newly allocated row header object.
    * \sa rowHeaderRef()
    */
    QicsRowHeader *rowHeader();
    /*!
    *  Returns a newly allocated \b const row header object.
    * \sa rowHeaderRef()
    */
    const QicsRowHeader *rowHeader() const;

    /*!
    * Returns a reference to an internal flyweight QicsColumnHeader object.
    * \sa columnHeader()
    */
    QicsColumnHeader &columnHeaderRef();
    /*!
    * Returns a \b const reference to an internal flyweight QicsColumnHeader object.
    * \sa columnHeader()
    */
    const QicsColumnHeader &columnHeaderRef() const;

    /*!
    *  Returns a newly allocated column header object.
    * \sa columnHeaderRef()
    */
    QicsColumnHeader *columnHeader();
    /*!
    *  Returns a newly allocated \b const column header object.
    * \sa columnHeaderRef()
    */
    const QicsColumnHeader *columnHeader() const;

    /////////////////// PROPERTY GETS

    /*!
    * Returns the current value of the top header visibility property.
    * See the \link #topHeaderVisible "topHeaderVisible" \endlink
    * property for details.
    * \sa setTopHeaderVisible()
    */
    inline QicsTableDisplayOption topHeaderVisible() const
    { return myTopHeaderVisible; }

    /*!
    * Returns the current value of the bottom header visibility property.
    * See the \link #bottomHeaderVisible "bottomHeaderVisible" \endlink
    * property for details.
    * \sa setBottomHeaderVisible()
    */
    inline QicsTableDisplayOption bottomHeaderVisible() const
    { return myBottomHeaderVisible; }

    /*!
    * Returns the current value of the left header visibility property.
    * See the \link #leftHeaderVisible "leftHeaderVisible" \endlink
    * property for details.
    * \sa setLeftHeaderVisible()
    */
    inline QicsTableDisplayOption leftHeaderVisible() const
    { return myLeftHeaderVisible; }

    /*!
    * Returns the current value of the right header visibility property.
    * See the \link #rightHeaderVisible "rightHeaderVisible" \endlink
    * property for details.
    * \sa setRightHeaderVisible()
    */
    inline QicsTableDisplayOption rightHeaderVisible() const
    { return myRightHeaderVisible; }

    /*!
    * Returns the margin of the table widget.
    * See the \link #tableMargin "tableMargin" \endlink
    * property for details.
    */
    inline int tableMargin() const
    { return myTableMargin; }

    /*!
    * Returns the spacing of the table widget.
    * See the \link #tableSpacing "tableSpacing" \endlink
    * property for details.
    */
    inline int tableSpacing() const
    { return myTableSpacing; }

    /*!
    * Returns the spacing of the grids in the table widget.
    * See the \link #gridSpacing "gridSpacing" \endlink
    * property for details.
    */
    inline int gridSpacing() const
    { return myGridSpacing; }

    /*!
    * Returns whether the first row of data in the table's data model
    * is used as column header data.
    * See the \link #columnHeaderUsesModel "columnHeaderUsesModel" \endlink
    * property for details.
    * \sa setColumnHeaderUsesModel()
    */
    inline bool columnHeaderUsesModel() const
    { return myColumnHeaderUsesModel; }

    /*!
    * Returns whether the first column of data in the table's data model
    * is used as row header data.
    * See the \link #rowHeaderUsesModel "rowHeaderUsesModel" \endlink
    * property for details.
    * \sa setRowHeaderUsesModel()
    */
    inline bool rowHeaderUsesModel() const
    { return myRowHeaderUsesModel; }

    /*!
    * Returns whether the table should ignore size change signals from
    * the data model.
    * See the \link #ignoreModelSizeChanges "ignoreModelSizeChanges" \endlink
    * property for details.
    * \sa setIgnoreModelSizeChanges()
    */
    bool ignoreModelSizeChanges() const;

    /*!
    * sort the rows
    * \param columns the data model columns to sort on
    * \param order sort order <i>(default: Ascending)</i>
    * \param from only sort the rows between \a from and \a to
    * \param to only sort the rows between \a from and \a to.  The
    * default of -1 will cause all rows through the end to be sorted
    * \param func \a optional a comparator function.  Specifying
    * 0 will use a default comparator which should work for almost
    * every case.   Applications which add new subclasses of QicsDataItem
    * to their tables, should provide type specific comparators with their
    * data elements.
    * \sa sortColumns()
    */
    void sortRows(const QVector<int> &columns,
        QicsSortOrder order = Qics::Ascending,
        int from = 0, int to = -1,
        DataItemComparator func = 0);

    /*!
    * sort the columns
    * \param rows the data model rows to sort on
    * \param order sort order <i>(default: Ascending)</i>
    * \param from only sort the columns between \a from and \a to
    * \param to only sort the columns between \a from and \a to.  The
    * default of -1 will cause all columns through the end to be sorted
    * \param func \a optional a comparator function.  Specifying
    * 0 will use a default comparator which should work for almost
    * every case.   Applications which add new subclasses of QicsDataItem
    * to their tables, should provide type specific comparators with their
    * data elements.
    * \sa sortRows()
    */
    void sortColumns(const QVector<int> &rows,
        QicsSortOrder order = Qics::Ascending,
        int from = 0, int to = -1,
        DataItemComparator func = 0);

    /*!
    * Sets case sensitivity while sorting to \a cs.
    * \sa sortColumns(), sortRows()
    * \since 2.3
    */
    void setSortingSensitivity(Qt::CaseSensitivity cs);

    /*!
    * Moves a set of (possibly discontiguous) rows to a new location.
    * \param target_row the row before which the moved rows will be placed
    * \param rows a list of row indices to be moved
    * \sa moveColumns()
    */
    void moveRows(int target_row, const QVector<int> &rows);

    /*!
    * Moves a set of (possibly discontiguous) columns to a new location.
    * \param target_col the column before which the moved columns will be placed
    * \param cols a list of column indices to be moved
    * \sa moveRows()
    */
    void moveColumns(int target_col, const QVector<int> &cols);
    /*!
    * This method set mouse button mapping inside \sa QicsTable.
    * Usefull for Apple mouse, 2-button mouse or for individual
    * remapping.
    * First paramater action button, second keyboard modifier, third result
    * button (which emulate).
    */

    bool setMouseButton( Qt::MouseButton, Qt::Modifier, Qt::MouseButton );

    /*!
    * Returns a drag object/mime source that represents data that
    * is to be cut or copied from the table.  The current selection list
    * is used as the data to be cut or copied.  If \a ref_cell is
    * non-zero, it is used to find a contiguous selection from the selection
    * list that contains \a ref_cell.  If no such selection exists,
    * the contents of \a ref_cell will be used as the data.  If \a ref_cell
    * is zero, the entire selection list is cut or copied.
    *
    * If non-zero, \a drag_widget
    * will be set as the drag object's source widget.  If specified,
    * \a ref_cell will be used to determine which part
    * If \a drag_single parameter is \b true only current cell will be
    * moved/copied in case if dragging began from current cell, otherwise
    * whole selection will be moved/copied.
    */
    QMimeData *cutCopyData(QWidget *drag_widget = 0,
        QicsICell *ref_cell = 0, bool drag_single = true);

    /*!
    * Finishes a cut operation.
    * Option deletes data from the data model and then
    * cleans up any cruft from the cutCopyData operation,
    * according to current clear policy.
    *
    * \param removeData Remove the data from the model.  This would
    * be done only after a successful dragMove operation.
    *
    * \sa setClearPolicy()
    */
    void finishCut(bool removeData);

    /*!
    * Pastes data from \a md into the table, starting at \a cell.
    * Operates according to current copy policy.
    * \sa setCopyPolicy()
    */
    void paste(const QMimeData *md, const QicsICell &cell);

    /*!
    * Overlays data model \a dm onto the data model contained
    * in this object.  The overlay will start at cell \a start_cell
    * in the main data model.  If \a expand_model is \b true, the
    * main data model will be expanded if necessary.  If
    * \a clear_if_empty is \b true, empty cells in \a dm will be "copied"
    * to the main data model.  If \a clear_if_empty is \b false,
    * those cells will not be "copied" and the existing values in
    * the main data model will be retained.
    */
    QicsICell overlay(const QicsDataModel &dm, const QicsICell &start_cell,
        bool expand_model = false, bool clear_if_empty = true);

    /*!
    * Deletes data from selected cells according to current clear policy.
    * \since 2.2
    * \sa setClearPolicy()
    */
    void deleteSelected();

public slots:
    /*!
    * Sets the value of the topHeaderVisible property.
    * See the \link #topHeaderVisible "topHeaderVisible" \endlink
    * property for details.
    * \sa topHeaderVisible()
    */
    inline void setTopHeaderVisible(QicsTableDisplayOption tdo) {myTopHeaderVisible = tdo;}

    /*!
    * Sets the value of the bottomHeaderVisible property.  At the
    * present time, only DisplayNever and DisplayAlways are supported.
    * See the \link #bottomHeaderVisible "bottomHeaderVisible" \endlink
    * property for details.
    * \sa bottomHeaderVisible()
    */
    inline void setBottomHeaderVisible(QicsTableDisplayOption tdo) {myBottomHeaderVisible = tdo;}

    /*!
    * Sets the value of the leftHeaderVisible property.
    * See the \link #leftHeaderVisible "leftHeaderVisible" \endlink
    * property for details.
    * \sa leftHeaderVisible()
    */
    inline void setLeftHeaderVisible(QicsTableDisplayOption tdo) {myLeftHeaderVisible = tdo;}

    /*!
    * Sets the value of the rightHeaderVisible property.  At the
    * present time, only DisplayNever and DisplayAlways are supported.
    * See the \link #rightHeaderVisible "rightHeaderVisible" \endlink
    * property for details.
    * \sa rightHeaderVisible()
    */
    void setRightHeaderVisible(QicsTableDisplayOption tdo) {myRightHeaderVisible = tdo;}

    /*!
    * Sets the margin of the table widget.
    * See the \link #tableMargin "tableMargin" \endlink
    * property for details.
    */
    inline void setTableMargin(int margin) {myTableMargin = margin;}

    /*!
    * Sets the spacing of the table widget.
    * See the \link #tableSpacing "tableSpacing" \endlink
    * property for details.
    */
    inline void setTableSpacing(int spacing) {myTableSpacing = spacing;}

    /*!
    * Sets the spacing of the grids in the table widget.
    * See the \link #gridSpacing "gridSpacing" \endlink
    * property for details.
    */
    inline void setGridSpacing(int spacing) {myGridSpacing = spacing;}

    /*!
    * Controls whether the first column of data in the table's data model
    * is used as row header data.
    * See the \link #rowHeaderUsesModel "rowHeaderUsesModel" \endlink
    * property for details.
    * \sa rowHeaderUsesModel()
    */
    void setRowHeaderUsesModel(bool b);

    /*!
    * Controls whether the first row of data in the table's data model
    * is used as column header data.
    * See the \link #columnHeaderUsesModel "columnHeaderUsesModel" \endlink
    * property for details.
    * \sa columnHeaderUsesModel()
    */
    void setColumnHeaderUsesModel(bool b);

    /*!
    * Controls whether the table should ignore size change signals from
    * the data model.
    * See the \link #ignoreModelSizeChanges "ignoreModelSizeChanges" \endlink
    * property for details.
    * \sa ignoreModelSizeChanges()
    */
    void setIgnoreModelSizeChanges(bool b);

    /*!
    * Delete a column from the table.
    */
    void deleteColumn(int column);
    /*!
    * Deletes \a num columns, starting at \a start_position.
    */
    void deleteColumns(int num, int start_position);
    /*!
    * Append a column or more to the table.
    */
    void addColumns(int howMany);
    /*!
    * Insert a column into the table before \a column.
    */
    void insertColumn(int column);
    /*!
    * Delete a row from the table.
    */
    void deleteRow(int row);
    /*!
    * Deletes \a num rows, starting at \a start_position.
    */
    void deleteRows(int num, int start_position);
    /*!
    * Append a row or more to the table.
    */
    void addRows(int rows);
    /*!
    * Insert a row into the table before \a row.
    */
    void insertRow(int row);

    /*!
    * Sets the default row height to \a height pixels
    * for the entire table.
    */
    void setDefaultRowHeightInPixels(int height);
    /*!
    * Sets the default row height to \a height character units (i.e. lines)
    * for the entire table.
    */
    void setDefaultRowHeightInChars(int height);

    /*!
    * Sets the default column width to \a width pixels
    * for the entire table.
    */
    void setDefaultColumnWidthInPixels(int width);
    /*!
    * Sets the default column width to \a width character units
    * for the entire table.
    */
    void setDefaultColumnWidthInChars(int width);

protected:
    void registerMetaTypes();

    virtual void setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val);
    virtual void setGridAttr(QicsGridStyle::QicsGridStyleProperty attr, const void *val);
    virtual void clearGridAttr(QicsGridStyle::QicsGridStyleProperty attr);

    inline const QicsGridInfo &gridInfo() const { return myMainGridInfo; }
    inline const QicsGridInfo &rhGridInfo() const { return myRowHeaderGridInfo; }
    inline const QicsGridInfo &chGridInfo() const { return myColumnHeaderGridInfo; }

    inline QicsGridInfo &gridInfo()  { return myMainGridInfo; }
    inline QicsGridInfo &rhGridInfo()  { return myRowHeaderGridInfo; }
    inline QicsGridInfo &chGridInfo()  { return myColumnHeaderGridInfo; }

    /*!
    * \internal
    * Returns the selection list that will be used during cut and copy
    * operations.  See QicsGridInfo::cutCopyData for details.
    */
    QicsSelectionList *cutCopySelection(const QicsICell *ref_cell);

    /*!
    * \internal
    * info object for the main table grid
    */
    QicsGridInfo myMainGridInfo;
    /*!
    * \internal
    * info object for row header grid
    */
    QicsGridInfo myRowHeaderGridInfo;
    /*!
    * \internal
    * info object for column header grid
    */
    QicsGridInfo myColumnHeaderGridInfo;

    /*!
    * \internal
    * row header data model
    */
    QicsDataModel *myRHDataModel;

    /*!
    * \internal
    * column header data model
    */
    QicsDataModel *myCHDataModel;

    /*!
    * \internal
    * the last cut/copy selection list
    */
    QicsSelectionList *myCutCopySelection;

    QicsCell *myCell;
    QicsRow *myRow;
    QicsColumn *myColumn;
    QicsRepeatingRow *myRepeatingRow;
    QicsRepeatingColumn *myRepeatingColumn;
    QicsMainGrid *myMainGrid;
    QicsRowHeader *myRowHeader;
    QicsColumnHeader *myColumnHeader;

    /*!
    * \internal
    * show header at top?
    */
    QicsTableDisplayOption myTopHeaderVisible;
    /*!
    * \internal
    * show header at bottom?
    */
    QicsTableDisplayOption myBottomHeaderVisible;
    /*!
    * \internal
    * show header at left?
    */
    QicsTableDisplayOption myLeftHeaderVisible;
    /*!
    * \internal
    * show header at right?
    */
    QicsTableDisplayOption myRightHeaderVisible;

    /*!
    * \internal
    * margin for master grid layout
    */
    int myTableMargin;
    /*!
    * \internal
    * spacing for master grid layout
    */
    int myTableSpacing;
    /*!
    * \internal
    * spacing for "grid" grid layout
    */
    int myGridSpacing;

    /*!
    * \internal
    * storage for rowHeaderUsesModel property
    */
    bool myRowHeaderUsesModel;
    /*!
    * \internal
    * storage for columnHeaderUsesModel property
    */
    bool myColumnHeaderUsesModel;
    /*!
    * \internal used for correct data removing in finishCut() method
    *  when selection policy is set to MultipleRowSelect
    */
    bool myIsOneCellMoving;

private:
    friend class QicsTableRegionDrag;
};

#endif //QICSTABLECOMMON_H


