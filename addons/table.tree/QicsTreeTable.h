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

#ifndef QICSTREETABLE_H
#define QICSTREETABLE_H

#include <QicsTable.h>

#include <QicsGroupInfo.h>
#include <QicsTreeDataModel.h>

/*!
* \class QicsTreeTable QicsTreeTable.h
* \brief Extends QicsTable's functionality by adding methods for
*  grouping, sorting and filtering data.
*
*  %QicsTreeTable is the main class which should be used in a custom application.
*  It provides all the necessary methods to manage data in the tree view.
*
* \image html treetable.png "Grouped tree"
*
*  %QicsTreeTable is designed as a subclass of QicsTable. It extends QicsTable's
*  functionality by adding several methods for grouping, sorting and filtering
*  data by columns, and also provides a set of additional classes for easy
*  building of GUI.
*
*  Basic classes used directly by %QicsTreeTable:
*   -	QicsViewTreeDataModel
*   -	QicsSpecialRowData, QicsStaticRowData, and QicsExpandableStaticRowData
*   -	QicsTreeTableGrid
*
*	Helper classes for customizing look & feel:
*   -  QicsGroupInfo
*   -  QicsSummarizer
*   -  QicsGroupCellDisplay
*
*  Helper classes for building the GUI:
*   -	QicsCheckPopupDialog (based on QicsCheckPopup)
*   -	QicsGroupBar (based on QicsSectionBar)
*   -	QicsSortBar (based on QicsSectionBar)
*
*  Those helper classes are designed for usage in custom applications
*  with %QicsTreeTable. The basic classes are internal
*  and should not be used directly.
*
*  %QicsTreeTable uses dual-model internal representation of data. Actual data
*  are stored in QicsDataModel which works as "low-level" data holder
*  and may be used with any others QicsTable or %QicsTreeTable classes among
*  the project. QicsViewTreeDataModel is the "proxy model" which translates
*  data stored in external model to be compatible with %QicsTreeTable.
*  Actually, %QicsTreeTable uses QicsViewTreeDataModel as the model, and
*  QicsViewTreeDataModel reads and writes actual data through itself to/from QicsDataModel.
*
*  <h3>%QicsTreeTable grouping modes</h3>
*
*  Since 2.4, %QicsTreeTable provides two different grouping modes:
*	"Tree-in-0-column" (the older one) and "Tree-in-header".
*
*  While in "Tree-in-0-column" mode, all the columns in %QicsTreeTable are 1-based. This is because column with
*  index 0 is used for holding tree node marks in grouping mode.
*  QicsViewTreeDataModel transparently performs column index adjustment when
*  reading or storing model data.
*
*  In "Tree-in-header" mode, the tree is placed inside the left header.
*	All the columns in %QicsTreeTable are 0-based (just like in QicsTable).
*
* \image html treetable_hdr.png "Tree-in-header mode"
*
*  <h3>Installing the %QicsTreeTable</h3>
*
*  Installation of the %QicsTreeTable object is the same as for QicsTable.
*  First, data model should be created, and then it should be passed to %QicsTreeTable's
*  constructor. The 2nd constructor parameter defines the mode; by default it is false,
*	i.e. the mode will be set to "Tree-in-0-column".
*
*  \code
*  // dimensions of the data model
*  int numRows = 10;
*  int numCols = 4;
*
*  // create the data model
*  QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);
*
*  // create the table, using the data model we created above
*  // passing true as the 2nd parameter enables "Tree-in-header" grouping mode.
*  QicsTreeTable *table = new QicsTreeTable(dm, true);
*  \endcode
*
*  <h3>Grouping</h3>
*
*  %QicsTreeTable's API provides a set of members which can be called in order
*  to change grouping of the tabular data. Also, %QicsTreeTable supports some GUI actions
*  which are helpful for end users.
*
*  To add grouping by particular column,
*  groupColumn() should be called. To regroup by several columns, use groupColumns() method.
*  Remove grouping of a column with ungroupColumn(). Remove all the grouping with
*  ungroup(). To obtain number of groups, call groupCount() method, and to obtain a list
*  of group indexes, use groups().
*
*  Reference to the top group (if exists) could be obtained via topGroupRef() method.
*  Returned QicsGroupInfo object contains all the information about the group it belongs to.
*  Also it contains a list of subgroups which could be obtained by simple iteration.
*
*  After adding or removing particular groups, corresponding signals groupAdded() and groupRemoved().
*  In addition, groupingChanged() signal is called after each regroup, which may be useful to
*  handle grouping event and then do some desired things.
*
*  A column also could be grouped by double-clicking left mouse button, if isDoubleClickGroupingEnabled()
*  is true. This can be allowed/disabled by calling setDoubleClickGroupingEnabled() method.
*
*  A QicsGroupBar can be installed and connected to the %QicsTreeTable to provide more flexible
*  interaction with user. It automatically handles signals coming from the %QicsTreeTable object and
*  reacts on them. Also it provides facilities for visual regrouping of the columns. Installing
*  QicsGroupBar is simple, just pass %QicsTreeTable object and parent widget to the constructor:
*
*  \code
*	QicsGroupBar *bar = new QicsGroupBar(table, this);
*  \endcode
*
*  Each group has its own header at the top, and also it can display a summary row at the bottom.
*  To activate summary rows, use setSummaryRowsVisible() method.
*
*  <h3>Sorting</h3>
*
*  QicsTable's sorting capabilities were extended in %QicsTreeTable to correctly handle
*  sorting inside groups. A sortRows() method provides multicolumn sorting of the data.
*  unsortRows() method rejects all the sorting.
*  rowsSorted() signal is emitted after each sorting operation.

*  A QicsSortBar can be installed and connected to the %QicsTreeTable to provide more flexible
*  interaction with user. It automatically handles signals coming from the %QicsTreeTable object and
*  reacts on them. Also it provides facilities for changing sort order and columns. Installing
*  QicsSortBar is simple, just pass %QicsTreeTable object and parent widget to the constructor:

*  \code
*	QicsSortBar *bar = new QicsSortBar(table, this);
*  \endcode

*  <h3>Filtering</h3>

*  %QicsTreeTable can filter its content by installing filters on particular columns.
*  Rows which contain items do not matching the filter become hidden.
*  To set a filter for a column, use setRowFilter() method.
*  To remove a filter from a column, use removeRowFilter(), or removeAllRowFilters() to
*  remove all the filters at all. filterChanged() signal is emitted when filtering is changed.

*  Current installed filter could be obtained via rowFilter() method for the column.
*  To find out if a column has filter installed, use hasRowFilter() method.
*  To obtain a list of filtered off (i.e. hidden) rows, use filteredRows(). To check if a row
*  was filtered off, use isRowFiltered() method.

*  <h3>Customizing table</h3>
*
*  %QicsTreeTable supports all the QicsTable's API to perform a customization of its look and feel.
*  In addition, %QicsTreeTable provides facilities to customize the look of the groups when in tree mode.

*  Since the information about groups is dynamic and does not stored between grouping operations,
*  it is impossible to make some static setup of %QicsTreeTable's items. "On-the-fly" customization
*  is used instead. There are two ways to implement this.
*
*  First one is to implement a slot and connect it with groupingChanged() signal. This slot will be
*  called every time the grouping is changed. You are responcible to perform all the customization
*  within the slot usign the QicsGroupInfo object.
*
*  Second and recommended way is to use QicsSummarizer class. It is acts like a delegate item.
*  A summarizer can be installed for the %QicsTreeTable by calling setSummarizer method, and can
*  be removed with removeSummarizer().
*
*  QicsSummarizer is abstract. You need to subclassed it and reimplement QicsSummarizer::onGrouping() method
*  which will be called for every group with the information about it.
*  You are responcible to perform all the customization within this method depening of its parameters.

*  \code
// subclassing the summarizer
class MySummarizer : public QicsSummarizer
{
public:
virtual void onGrouping(QicsGroupInfo *gi)
{
// here is the implementation which will analyze \a gi and make appropriate customizations
}
};

// installing the summarizer
table->setSummarizer(new MySummarizer());
*  \endcode
*
*	Since 2.4, there is also virtual method onCellValueChanged() which can be subclassed in order to
*	handle changes of data in the particular cell.
*
*  <h3>Custom cell displayers</h3>
*
*  Since 2.4, %QicsTreeTable introduces special QicsGroupCellDisplay class which draws the header and
*  summary rows of the tree.

*  By default, there are two separate diplayers installed for the %QicsTreeTable which can be obtained with
*	groupDisplayer() (this displayer draws tree nodes and header rows) and summaryDisplayer() (this displayer
*  draws summary rows respectively).

*	It is possible to customize active displayers by using appropriate
*	methods of QicsGroupCellDisplay, or, if you need a very special customization, to subclass QicsGroupCellDisplay
*	and set the new displayer(s) with setGroupDisplayer() and setSummaryDisplayer().
*/

class QicsSummarizer;
class QicsGroupCellDisplay;


class QICS_EXPORT QicsTreeTable : public QicsTable
{
    Q_OBJECT
public:

    /*!
    * Enum denotes policy of summary rows.
    * \sa setSummaryPolicy(), summaryPolicy()
    */
    enum QicsSummaryPolicy {
        SummaryDisabled,		//!< Summary rows are not visible
        SummaryEnabled,			//!< Summary rows are visible and can be expanded/contracted as well
        SummaryAlwaysExpanded	//!< Summary rows are visible and can not be contracted
    };

    /*!
    * Enum denotes policy of expanding header rows.
    * \sa setExpandingPolicy(), expandingPolicy()
    */
    enum QicsExpandingPolicy {
        SingleClick,
        DoubleClick,
        DisplayerDesides
    };

    enum QicsTreeMode {
        Tree,
        Flat,
        Flat_No_Details
    };
    /*!
    *	Constructs a QicsTreeTable widget that will display data contained in \a model,
    *  or, an empty table if \a model is 0.
    *  \a useHeader set to true (false default) enables tree-in-table grouping mode.
    *
    *  To initialize QicsTreeTable, use one of the constructors and
    *  pass QicsDataModel in the way like:
    *  \code
    *  // dimensions of the data model
    *  int numRows = 100;
    *  int numCols = 10;
    *
    *  // create the data model
    *  QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);
    *
    *  // create the table
    *  QicsTreeTable *table = new QicsTreeTable(dm);
    *  \endcode
    */
    QicsTreeTable(QicsDataModel *model, bool useHeader = false, QWidget *parent = 0);

    /*!
    *  Constructs an empty table.
    *  \a useHeader set to true (false default) enables tree-in-table grouping mode.
    */
    QicsTreeTable(bool useHeader = false, QWidget *parent = 0);

    /*!
    *  Constructs a QicsTreeTable widget with one or more custom components.
    *  Any foundry method parameter that is 0 will be replaced by the
    *  appropriate builtin foundry method (QicsDataModelDefault::create(),
    *  QicsTableGrid::create(), or QicsHeaderGrid::create()).
    *  \param rhdmf foundry method to create a custom row header data model
    *  \param chdmf foundry method to create a custom column header data model
    *  \param tf foundry method to create custom table grids
    *  \param hf foundry method to create custom header grids
    *  \param model data model to display
    *	\param useHeader set to true (false default) enables tree-in-table grouping mode
    *  \param parent parent widget
    */
    QicsTreeTable(QicsDataModel::Foundry rhdmf,
        QicsDataModel::Foundry chdmf,
        QicsTableGrid::Foundry tf,
        QicsHeaderGrid::Foundry hf,
        QicsDataModel *model = 0,
        bool useHeader = false,
        QWidget *parent = 0);

    /*!
    *  \internal
    *  Constructs a QicsTreeTable widget with one or more custom components.
    *  Any foundry method parameter that is 0 will be replaced by the
    *  appropriate builtin foundry method (QicsDataModelDefault::create(),
    *  QicsTableGrid::create(), or QicsHeaderGrid::create()).
    *  This constructor is not exposed as part of the public API.  It has
    *  been added because it's necessary for the Python bindings.
    *  \param key value that will be passed to each of the special foundry methods
    *  \param rhdmf_wk special foundry method to create a custom row header data model
    *  \param chdmf_wk special foundry method to create a custom column header data model
    *  \param tf_wk special foundry method to create custom table grids
    *  \param hf_wk special foundry method to create custom header grids
    *  \param model data model to display
    *	\param useHeader set to true (false default) enables tree-in-table grouping mode
    *  \param parent parent widget
    */
    QicsTreeTable(void *key,
        QicsDataModel::FoundryWithKey rhdmf_wk,
        QicsDataModel::FoundryWithKey chdmf_wk,
        QicsTableGrid::FoundryWithKey tf_wk,
        QicsHeaderGrid::FoundryWithKey hf_wk,
        QicsDataModel *model = 0,
        bool useHeader = false,
        QWidget *parent = 0);

    /*!
    *  Destructor for the class.
    */
    virtual ~QicsTreeTable();

    /*!
    *  Returns the data model object that is being displayed by the table widget.
    *  \sa setDataModel()
    */
    virtual QicsDataModel* dataModel() const { return m_userModel; }

    /*!
    * Returns the QicsViewTreeDataModel object that is set as view model in the table.
    */
    inline QicsViewTreeDataModel* viewModel() const
    { return (QicsViewTreeDataModel*)QicsTable::dataModel(); }

    /*!
    * Sets the data model object for the table widget to display.
    * \sa dataModel()
    */
    void setDataModel(QicsDataModel *model);

    /*!
    * Returns grid info object used by the table.
    */
    inline QicsGridInfo& gridInfo() const { return QicsTable::gridInfo(); }

    /*!
    * Returns a pointer to special row QicsRow matching \a rowData, or 0 if there
    * is no such row.
    */
    const QicsRow* specialRow(QicsSpecialRowData *rowData) const;

    /*!
    * Returns index of special row matching \a rowData, or -1 if there is no such row.
    */
    int specialRowIndex(QicsSpecialRowData *rowData) const;

    /*!
    * Returns a pointer to QicsSpecialRowData for special row with \a index,
    * or 0 if there is no special row with such index.
    */
    QicsSpecialRowData* specialRowData(int index) const;

    /*!
    * Returns number of columns available.
    */
    inline int columnCount() const
    { return viewModel() ? viewModel()->numColumns() : 0; }

    /*!
    * Returns number of rows available.
    * \since 2.4
    */
    inline int rowCount() const
    { return viewModel() ? viewModel()->numRows() : 0; }

    /*!
    * Returns true if flat tree mode was enabled.
    * \sa setFlatMode()
    */
    inline QicsTreeMode treeMode() const
    { return m_treeMode; }

    /*!
    * Returns \a true if column grouping by double clicking the header
    * is enabled, or returns \a false if column grouping by double clicking
    * the header is disabled.
    * \sa setDoubleClickGroupingEnabled()
    */
    inline  bool isDoubleClickGroupingEnabled() const
    { return m_dclickGroup; }

    /*!
    * Returns \a true if rows sorting by double clicking the header is enabled,
    * or returns \a false if rows sorting by double clicking the header is disabled.
    * \sa setDoubleClickSortingEnabled()
    */
    inline bool isDoubleClickSortingEnabled() const
    { return m_dclickSort; }

    /*!
    *	Sets tree expanding policy to \a policy.
    *  \sa expandPolicy()
    *	\since 2.4
    */
    inline void setExpandPolicy(QicsExpandingPolicy policy)
    { m_expandPolicy = policy; }

    /*!
    *	Returns current tree expanding policy.
    *  \sa setExpandPolicy()
    *	\since 2.4
    */
    inline QicsExpandingPolicy expandPolicy(void) const
    { return m_expandPolicy; }

    /*!
    * Returns list of column indexes in the current grouping order.
    * List is 1-based.
    */
    inline const QList<int>& groups() const
    { return m_groups; }

    /*!
    * Returns number of groups.
    */
    inline int groupCount() const
    { return m_groups.count(); }

    /*!
    * Returns text of the group at \a id.
    * \a id should be given in model coordinates.
    * \since 2.4
    */
    QString groupText(int id);

    /*!
    * Returns reference to a top QicsGroupInfo object
    * in the current grouping order.
    * \sa groupForRow()
    */
    inline QicsGroupInfo& topGroupRef()
    { return *m_topGroup; }

    /*!
    * Returns QicsGroupInfo object for \a row of 0 if not present.
    * \a row should be given in model coordinates.
    * \since 2.4
    * \sa topGroupRef()
    */
    inline QicsGroupInfo* groupForRow(int row)
    { return m_rowGroupMap.contains(row) ? m_rowGroupMap[row] : 0; }

    /*!
    * Returns current policy of summary rows.
    * \sa setSummaryPolicy()
    * \since 2.4
    */
    inline QicsSummaryPolicy summaryPolicy() const
    { return m_summaryPolicy; }

    /*!
    * Returns \a true if row with index \a rowIndex is a summary row,
    * otherwise returns \a false.
    * \a rowIndex should be given in model coordinates.
    */
    inline bool isSummary(int rowIndex) const
    { return m_summaryRows.contains(rowIndex); }

    /*!
    * Returns \a true if row with index \a rowIndex is a header row,
    * otherwise returns \a false.
    * \a rowIndex should be given in model coordinates.
    * \since 2.4
    */
    inline bool isHeader(int rowIndex) const
    { return !m_summaryRows.contains(rowIndex) && m_specialRows.contains(rowIndex); }

    /*!
    * Returns \a true if row with index \a rowIndex is a data row (not summary or header),
    * otherwise returns \a false.
    * \a rowIndex should be given in model coordinates.
    * \since 2.4
    */
    inline bool isDataRow(int rowIndex) const
    { return !m_summaryRows.contains(rowIndex); }

    /*!
    * Returns filter installed for \a column, or empty list if there is
    * no any filter.
    * \a column should be given in model coordinates.
    */
    QicsAbstractFilterDelegate* rowFilter(int column);

    /*!
    * Returns true if \a column has any filter installed.
    * \a column should be given in model coordinates.
    */
    inline bool	hasRowFilter(int column) const
    { return m_filters.contains(column); }

    /*!
    * Returns \a true if row with index \a rowIndex is filtered off
    * (i.e. not visible), \a false otherwise.
    * \a rowIndex should be given in model coordinates.
    */
    inline bool	isRowFiltered(int rowIndex) const
    { return m_filteredRows.contains(rowIndex); }

    /*!
    * Returns list of filtered rows.
    */
    inline const QList<int> &filteredRows() const
    { return m_filteredRows; }

    /*!
    * Performs multicolumn sorting of rows by \a columns list,
    * ordered by \a order, from row \a from to row \a to, using comparator \a func.
    * \a columns should be given in model coordinates, \a from and \a to in visual ones.
    */
    void sortRows( QVector< int > columns,
        QicsSortOrder  order = Qics::Ascending,
        int  from = 0,
        int  to = -1,
        DataItemComparator  func = 0 );

    /*!
    * Adds column \a column to current sorting vector.
    * \a column should be given in model coordinates.
    * \since 2.4
    */
    void sortColumn(int column);

    /*!
    * Removes column \a column from current sorting vector.
    * \a column should be given in model coordinates.
    * \since 2.4
    */
    void unsortColumn(int column);

    /*!
    * Returns list of sorted columns (model coordinates).
    */
    inline const QVector<int>&	columnSortVector() const
    { return m_sorted; }

    /*!
    * Returns the order of sorting (Ascending/Descending).
    */
    inline QicsSortOrder columnSortOrder() const
    { return m_sortOrder; }

    /*!
    * Sets user QicsSummarizer object \a summarizer for the table.
    */
    void setSummarizer(QicsSummarizer *summarizer);

    /*!
    * Removes user summarizer.
    */
    void removeSummarizer();

    /*!
    * Returns pointer to current summarizer in the table.
    * \since 2.3
    */
    inline QicsSummarizer  *summarizer() const
    { return m_summarizer; }

    /*!
    * Returns pointer to current group cell displayer for the table.
    * \since 2.4
    */
    inline QicsGroupCellDisplay  *groupDisplayer() const
    { return m_groupDisplay; }

    /*!
    * Sets current summary cell displayer for the table to \a cd.
    * \since 2.4
    */
    void setGroupDisplayer(QicsGroupCellDisplay *cd);

    /*!
    * Returns pointer to current summary cell displayer for the table.
    * \since 2.4
    */
    inline QicsGroupCellDisplay  *summaryDisplayer() const
    { return m_summaryDisplay; }

    /*!
    * Sets current summary cell displayer for the table to \a cd.
    * \since 2.4
    */
    void setSummaryDisplayer(QicsGroupCellDisplay *cd);

    /*!
    * Returns true if tree-in-header grouping mode set.
    * \since 2.4
    */
    bool treeInHeader() const
    { return m_treeInHeader; }

    /*!
    * Returns current visibility of grouped columns.
    * \sa showGroupedColumns()
    * \since 2.4
    */
    inline bool groupedColumnsVisible() const
    { return m_groupColsShown; }

protected:
    class TreeItem
    {
    public:
        TreeItem(TreeItem *root = 0)	// -1 means special
        {
            row = -1;
            if (root)
                root->list.append(this);
        }

        ~TreeItem()
        {
            qDeleteAll(list);
            list.clear();
        }

        QList<TreeItem*>	list;
        int		row;
        QString	content;
        int		group;
    };


    struct GroupItem
    {
        void add(int _row, const QString& _content)
        {
            content[_content].append(_row);
        }

        QMap<QString, QList<int> > content;
    };

signals:
    /*!
    * Emitted when grouping by \a column is added.
    * \a column given in model coordinates.
    */
    void groupAdded(int column);

    /*!
    * Emitted when grouping by \a column is removed.
    * \a column given in model coordinates.
    */
    void groupRemoved(int column);

    /*!
    * Emitted when regrouping is happen, i.e. grouping by column
    * is added or removed.
    */
    void groupingChanged();

    /*!
    * Emitted when grouping operation started (\a yes is true) or finished (\a yes is false).
    * \since 2.4.1
    */
    void groupingInProgress(bool);

    /*!
    * Emitted when filter is changed for \a column.
    * \a column given in model coordinates.
    */
    void filterChanged(int column);

    /*!
    * Emitted when row sorting has been performed.
    */
    void rowsSorted();

public slots:
    /*!
    * Sets filter for \a column to \a filter.
    * \a column should be given in model coordinates.
    * If \a filter is 0, then filter is removed from column (like as calling removeFilter(column)).
    * When a filter is set to a column, each row item in this column is being
    * compared with each filter installed. Items NOT matching the filter become
    * filtered off (hidden).
    */
    void setRowFilter(int column, QicsAbstractFilterDelegate *filter, bool deleteOld = true);

    /*!
    * Removes filter from \a column.
    * \a column should be given in model coordinates.
    */
    void removeRowFilter(int column, bool deleteOld = true);

    /*!
    * Removes filters from all columns.
    */
    void removeAllRowFilters();

    /*!
    * Adds grouping by \a column.
    * Does nothing if column is wrong or data are already grouped by this column.
    * \a column should be given in model coordinates.
    * \sa groupColumns().
    */
    void groupColumn(int column);

    /*!
    * Performs grouping according to \a columns list.
    * \a columns should be given in model coordinates.
    * Ignores all the wrong or duplicated columns. Previous grouping is replaced by given.
    *
    * Equals to call ungroup() and then groupColumn() for every list item, but is
    * faster and simpler.
    */
    void groupColumns(const QList<int> &columns);

    /*!
    * Removes grouping by \a column.
    * Does nothing if column is wrong or data are not grouped by this column.
    * \a column should be given in model coordinates.
    */
    void ungroupColumn(int column);

    /*!
    * Removes all the grouping.
    */
    void ungroup();

    /*!
    * Forces regroup of the table.
    * \since 2.4
    */
    void regroup();

    /*!
    * Sets visibility of grouped columns to \a set (false by default).
    * \sa groupedColumnsVisible()
    * \since 2.4
    */
    void showGroupedColumns(bool set);

    /*!
    * Turns flat tree mode to \a set.
    * \sa isTreeMode()
    */
    void setTreeMode(QicsTreeMode type);

    /*!
    * Sets current policy of summary rows visibility to \a policy.
    * \sa summaryPolicy()
    * \since 2.4
    */
    void setSummaryPolicy(int policy);

    /*!
    * If \a yes is \a  true, enables column grouping by double clicking the header,
    * otherwise disables it.
    * \sa isDoubleClickGroupingEnabled()
    */
    inline void setDoubleClickGroupingEnabled(bool yes = true) { m_dclickGroup = yes; }

    /*!
    * If \a yes is \a  true, enables rows sorting by double clicking the header,
    * otherwise disables it.
    * \sa isDoubleClickSortingEnabled()
    */
    inline void setDoubleClickSortingEnabled(bool yes = true) { m_dclickSort = yes; }

    /*!
    * Removes all the sorting.
    */
    void unsortRows();

    /*!
    * Adds special row with \a index with matching \a rowData.
    * \a index should be given in model coordinates.
    */
    void addSpecialRow(int index, QicsSpecialRowData *rowData = 0);

    /*!
    * Removes special row with \a index.
    * \a index should be given in model coordinates.
    */
    void removeSpecialRow(int index);

    /*!
    * Removes all special rows from the table.
    */
    void removeSpecialRows();

    /*!
    * Expands all the tree items.
    * \sa collapseTree()
    * \since 2.4
    */
    inline void expandTree() { doExpandTree(true); }

    /*!
    * Collapses all the tree items.
    * \sa expandTree()
    * \since 2.4
    */
    inline void collapseTree() { doExpandTree(false); }

protected slots:
    /*!
    * Performs default actions when the mouse is double clicked on header.
    * For the left button double click grouping by column occurs, and
    * for the right button double click row sorting function is called.
    */
    void onHeaderDoubleClicked(int,int,int,const QPoint &);

    /*!
    * Called when the mouse is double clicked on any cell in the table.
    * If the cell is in the special row, then handleDoubleClick()
    * function called for this row.
    */
    void onDoubleClicked(int,int,int,const QPoint &);

    /*!
    * Called when the mouse is single clicked on any cell in the table.
    * If the cell is in the special row, then handleClick()
    * function called for this row.
    */
    void onClicked(int,int,int,const QPoint &);

    /*!
    * Called when cell value at row \a row and column \a col has changed.
    */
    void onCellValueChanged(int row, int col);

    /*!
    * Called after \a count rows have been added to the table at \a index.
    */
    void onRowsAdded(int count, int index);

    /*!
    * Called after \a count rows have been added to the table.
    */
    void onRowsAdded(int count);

    /*!
    * Called before \a count rows will be added to the table at \a index.
    */
    void beforeRowsAdded(int count, int index);

    /*!
    * Called when \a count rows have been removed from the table at \a index.
    */
    void onRowsRemoved(int count, int index);

    /*!
    * Called when \a count columns have been added to the table at \a index.
    */
    void onColumnsAdded(int count, int index);

    /*!
    * Called when \a count columns have been added to the table.
    */
    void onColumnsAdded(int count);

    /*!
    * Called when \a count columns have been removed from the table at \a index.
    */
    void onColumnsRemoved(int count, int index);

protected:
    /*! \internal
    * Initialization for the table, sets internal default values, makes signal-slot
    * connections and sets QicsDataModel \a model.
    */
    void doInit(QicsDataModel *model, bool useHeader);

    /*! \internal
    * Sets default values for the table's null column and hides it.
    */
    void doInitNullColumn();

    /*! \internal
    * Makes grouping of the table.
    */
    void doGroupTable();

    /*! \internal
    * Makes ungrouping of the table.
    */
    void doUngroupTable();

    /*! \internal
    * Converts the tree representation of the table contents into the rows.
    */
    void doConvertTree(TreeItem *ti, class QicsViewTreeDataModel* model, QVector<int> *ord, int deep,
        QicsExpandableStaticRowData *lastRow, QicsGroupInfo *group);

    /*! \internal
    * Filters the entire table with the current installed filters.
    */
    void doFilterTable();

    /*! \internal
    * Filters the row with \a index. Called by doFilterTable().
    */
    void doFilterRow(int index);

    bool doFilterRowCheck(QicsExpandableStaticRowData *erow);

    /*! \internal
    * Sorts the entire table by parameters which were passed by sortRows().
    */
    void doSortTable();

    /*! \internal
    * Called by doSortTable(), makes actual sorting of the rows.
    */
    void doProcessSort(QVector<int> l, QVector<int>& v, int deep);

    /*! \internal
    * Expands or collapses all the tree items according to \a expand.
    */
    void doExpandTree(bool expand);

    /*! \internal
    * Iterates through children objects of the \a gi. For each QicsGroupInfo
    * object QIcsSummarizer onGrouping() function is called.
    */
    void iterateGroups(QicsGroupInfo *gi);

    void paintEvent ( QPaintEvent * event );
    void mouseMoveEvent( QMouseEvent *m );
    void mousePressEvent( QMouseEvent *m );
    void mouseReleaseEvent( QMouseEvent *m );

    /*! \internal
    * Calculation of Rect for splitter between Row Header and Table
    */
    QRect splitterRect();

    /*! \internal
    */
    void headerResizeNeeds( int );

    bool m_IsHeaderBeganResize;
    bool m_DoingResize;
    bool m_wasResized;

    QicsDataModel *m_userModel;

    QList<int> m_groups;
    QList<int> m_tmpGroups;
    QMap<int, GroupItem> m_groupData;
    QicsGroupInfo *m_topGroup;
    QMap<int, QicsGroupInfo*> m_rowGroupMap;
//    QMap<int, QicsAbstractFilterDelegate*>  m_filters;
        QMap<int, QPointer<QicsAbstractFilterDelegate> >  m_filters;
    QList<int> m_filteredRows;

    QVector<int> m_sorted;
    QicsSortOrder m_sortOrder;
    int m_from;
    int m_to;
    DataItemComparator m_func;

    QicsSummaryPolicy m_summaryPolicy;
    QSet<int> m_summaryRows;
    QSet<int> m_specialRows;

    bool m_dclickGroup;
    bool m_dclickSort;
    bool m_treeInHeader;
    bool m_groupColsShown;

    QicsTreeMode m_treeMode;

    QicsExpandingPolicy m_expandPolicy;

    QStringList	m_tmp;

    QicsSummarizer *m_summarizer;

    QicsGroupCellDisplay *m_groupDisplay;
    QicsGroupCellDisplay *m_summaryDisplay;

private:
    friend class QicsTreeHeaderGrid;
    friend class QicsGroupCellDisplay;
};

#endif //QICSTREETABLE_H


