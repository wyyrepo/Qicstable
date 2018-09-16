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

#ifndef QICSSELECTIONMANAGER_H
#define QICSSELECTIONMANAGER_H

#include <QObject>
#include <QVector>
#include "QicsRegion.h"
#include "QicsSelection.h"

//#define DEBUG_SELECTION 1

class QicsGrid;
class QicsHeaderGrid;
class QicsStyleManager;
class QicsDataModel;
class QicsGridInfo;

/*!
* \class QicsSelectionManager
* \brief internal Manager for selections in the table
*
* QicsSelectionManager handles cell selection in the QicsTable widget.
* The object receives messages from the QicsGrid widget specifying
* selection actions that the user has taken.  Selection actions can
* also be performed by the programmer through the QicsTable API.
*
* When selection actions are made, the selection manager updates its
* internal state, and also updates the style manager so that affected
* cells have the correct QicsCellStyle::Selected state.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsSelectionManager: public QObject, public Qics
{
    Q_OBJECT
public:
    /*! \internal
    * Constructor for the class.
    */
    QicsSelectionManager(QObject *parent = 0);
    /*! \internal
    * Destructor.
    */
    virtual ~QicsSelectionManager();

    /*! \internal
    * Returns the data model currently being used by the selection manager.
    */
    inline QicsDataModel *dataModel() const {return myDataModel;}

    /*! \internal
    * Sets the data model currently being used by the selection manager.
    */
    void setDataModel(QicsDataModel *sm);

    /*! \internal
    * Returns the main style manager currently being used
    * by the selection manager.
    */
    inline QicsStyleManager *styleManager() const {return myStyleManager;}

    /*! \internal
    * Sets the main style manager currently being used
    * by the selection manager.
    */
    inline void setStyleManager(QicsStyleManager *sm) {myStyleManager = sm;}

    /*! \internal
    * Set the main GridInfo which will control the row/column ordering
    * of this table.
    */
    inline void setGridInfo(QicsGridInfo *gi) {myGridInfo = gi;}

    /*! \internal
    * Adds a header to the selection manager's list of headers.
    * This list is used when a selection has changed and we need to
    * set the QicsSelected property in each header's style manager.
    */
    void addHeader(QicsHeaderGrid *hdr);

    /*! \internal
    * Removes a header to the selection manager's list of headers.  This list
    * is used when a selection has changed and we need to set the QicsSelected
    * property in each header's style manager.
    */
    void removeHeader(QObject *hdr);

    /*! \internal
    * If \a nocopy is true, returns pointer to internal selection list.
    * Otherwise, returns a copy of the selection manager's current selection list.
    * It is the programmer's responsibility to delete this list when
    * it is no longer needed. Modifying the copy will not affect the
    * selection list.  You must call #setSelectionList() to change the
    * selection.
    * This method is called by the public selection methods of QicsTable.
    *
    * \sa QicsTable::selectionList
    */
    QicsSelectionList *selectionList(bool nocopy = false) const;
    /*! \internal
    * Sets the selection list to \a sel_list.
    * This method is called by the public selection methods of QicsTable.
    *
    * \sa QicsTable::setSelectionList
    */
    void setSelectionList(const QicsSelectionList &sel_list);
    /*! \internal
    * Clears the selection list.
    * This method is called by the public selection methods of QicsTable.
    *
    * \sa QicsTable::clearSelectionList
    */
    void clearSelectionList();

    /*!
    * Returns QicsRegion covering all the selections, or invalid region if the list is empty.
    * \since 2.2
    */
    inline QicsRegion selectedRegion() const { return mySelectionList.region(); }

    /*! \internal
    * Returns a copy of the selection manager's current selection action
    * list.  It is the programmer's responsibility to delete this list when
    * it is no longer needed.  Modifying the copy will not affect the
    * selection list.  You must call #setSelectionList() to change the
    * selection.
    * This method is called by the public selection methods of QicsTable.
    *
    * \sa QicsTable::selectionList
    */
    QicsSelectionList *selectionActionList() const;

    /*! \internal
    * Adds selection \a selection to the current selection list.
    * This method is called by the public selection methods of QicsTable.
    *
    * \sa QicsTable::addSelection
    */
    void addSelection(QicsSelection &selection);

    /*! \internal
    * Returns the current selection policy.
    * This method is called by the public selection methods of QicsTable.
    *
    * \sa QicsTable::selectionPolicy
    */
    inline QicsSelectionPolicy selectionPolicy() const
    { return mySelectionPolicy; }
    /*! \internal
    * Sets the selection policy.
    * This method is called by the public selection methods of QicsTable.
    *
    * \sa QicsTable::setSelectionPolicy
    */
    void setSelectionPolicy(QicsSelectionPolicy policy);

    /*! \internal
    * Returns the current cell selection policy.
    * This method is called by the public selection methods of QicsTable.
    *
    * \sa QicsTable::selectCurrentCellPolicy
    */
    inline QicsSelectCurrentCellPolicy selectCurrentCellPolicy() const
    { return mySelectCurrentCellPolicy; }
    /*! \internal
    * Sets the current cell selection policy.
    * This method is called by the public selection methods of QicsTable.
    *
    * \sa QicsTable::setSelectCurrentCellPolicy
    */
    void setSelectCurrentCellPolicy(QicsSelectCurrentCellPolicy policy);


    /*! \internal
    * Called when a selection action is performed in the QicsGrid widget.
    */
    void processSelectionEvent(QicsSelectionType stype,
        int begin_row, int begin_col,
        int end_row, int end_col);

    /*! \internal
    * Returns \b true if the current selection policy allows for the
    * selection of multiple cells or rows, \b false otherwise.
    */
    bool canSelectMultiple() const;

    /*! \internal
    Returns current exclusive selection or 0 if none.
    Works if current selection style is set to Qics::Exclusive.
    \sa QicsTable::setSelectionStyle()
    */
    const QicsSelection* exclusiveSelection() const;

    /*! \internal
    Returns current selection or 0 if nothing selected.
    This selection is NOT included into selection list while it is
    in progress (i.e. being performed with mouse etc.).
    \sa QicsTable::currentSelection()
    */
    const QicsSelection* currentSelection() const;

public slots:
    /*! \internal
    * called when the ordering vector changes, and we have
    * to update all the selections because the things moved
    */
    void orderChanged(Qics::QicsIndexType, int *visChange, int size);
    /*! \internal
    * Deletes \a num rows into the selection list, starting at \a start_position.
    */
    void deleteRows(int num, int start_position);
    /*! \internal
    * Deletes \a num columns into the style table, starting at \a start_position.
    */
    void deleteColumns(int num, int start_position);

signals:
    /*! \internal
    * This signal is emitted when the selection list changes.  The parameter
    * \a in_progress signifies if the selection action is still in progress.
    */
    void selectionListChanged(bool in_progress);

    void selectionListChangedExternal(bool in_progress);
    /*! \internal
    * Signaled when the selection list changes.  The region that contains
    * the cells whose selection state changed is passed as a parameter.
    */
    void selectionCellsChanged(const QicsRegion &);

    /*! \internal
    * Signal emitted when the row selection changes. The \b row parameter is
    * row index, \b selected is \b true when row was selected and \b false
    * when row was deselected.
    */
    void rowSelected(int row,bool selected);

    /*! \internal
    * Signal emitted when the column selection changes. The \b col parameter is
    * column index, \b selected is \b true when column was selected and \b false
    * when column was deselected.
    */
    void columnSelected(int col,bool selected);

protected:
    // List of widgets
    typedef QVector<QicsGrid *> QicsGridPV;
    typedef QVector<QicsHeaderGrid *> QicsHeaderGridPV;

    /*! \internal
    * Specifies the selection state that should be set.  \b QicsSelectFalseRevert
    * means to set the cell to False, unless the cell is already selected
    * in the selection list.  \b QicsSelectTrueRevert means to set the cell
    * to True, unless the cell is not already selected in the selection list.
    */
    enum QicsSelectState { QicsSelectFalse, QicsSelectTrue,
        QicsSelectFalseRevert, QicsSelectTrueRevert };

    /*! \internal
    * Begins a selection starting at (\a begin_row, \a begin_col ) and
    * ending at (\a end_row, \a end_col ).
    */
    void beginSelection(int begin_row, int begin_col, int end_row, int end_col);
    /*! \internal
    * Continue a selection starting at (\a begin_row, \a begin_col ) and
    * ending at (\a end_row, \a end_col ).
    */
    void dragSelection(int begin_row, int begin_col, int end_row, int end_col);
    /*! \internal
    * Extend a selection starting at (\a begin_row, \a begin_col ) and
    * ending at (\a end_row, \a end_col ).
    */
    void extendSelection(int begin_row, int begin_col, int end_row, int end_col);
    /*! \internal
    * Add a new selection to the selection list, starting at
    * (\a begin_row, \a begin_col ) and
    * ending at (\a end_row, \a end_col ).
    */
    void addSelection(int begin_row, int begin_col, int end_row, int end_col);
    /*! \internal
    * End a selection starting at (\a begin_row, \a begin_col ) and
    * ending at (\a end_row, \a end_col ).
    */
    void endSelection(int begin_row, int begin_col, int end_row, int end_col);
    /*! \internal
    * Delete (clear) the current selection.
    */
    void deleteSelection();
    /*! \internal
    * Invalidate (unset)  a selection starting at (\a begin_row, \a begin_col )
    * and ending at (\a end_row, \a end_col ).
    */
    void invalidateSelection(const QicsSelection &sel);

    void replaceSelection(int begin_row, int begin_col, int end_row, int end_col);

    void validateSelection(const QicsSelection &sel);

    /*! \internal
    * Sets the QicsCellStyle::Selected property for each cell in \a selection
    * to the state \a sel.
    */
    void setSelectionProperty(const QicsSelection &selection,
        QicsSelectionManager::QicsSelectState sel);
    /*! \internal
    * Sets the QicsCellStyle::Selected property for each cell in the region
    * starting at (\a begin_row, \a begin_col ) and ending at
    * (\a end_row, \a end_col ) to the state \a sel.
    * NOTE: param constraints:
    * begin_row <= end_row
    * begin_col <= end_col
    */
    void setSelectionProperty(int begin_row, int begin_col,
        int end_row, int end_col,
        QicsSelectionManager::QicsSelectState sel);

    /*! \internal
    * Sets the QicsCellStyle::Selected property for row \a row in all
    * style managers.
    */
    void setRowSelectionProperty(QicsGridInfo &, int row,
        QicsSelectionManager::QicsSelectState sel,
        QicsStyleManager *sm);

    /*! \internal
    * Sets the QicsCellStyle::Selected property for column \a col in all
    * style managers.
    */
    void setColumnSelectionProperty(QicsGridInfo &, int col,
        QicsSelectionManager::QicsSelectState sel,
        QicsStyleManager *sm);

    /*! \internal
    * Adds \a sel to the interal selection list.
    */
    void addToSelectionList(const QicsSelection &sel);

    /*! \internal
    * Returns a refernce to the selection from the current selection list that
    * is anchored at cell (\a anchor_row, \a anchor_col ).
    */
    QicsSelection &findSelectionBlock(int anchor_row, int anchor_col);

    /*! \internal
    * Notifies other objects that the selection list has changed.
    * The parameter
    * \a in_progress signifies if the selection action is still in progress.
    */
    void announceChanges(bool in_progress);

    // \internal the controlling gridinfo
    QicsGridInfo *myGridInfo;
    // \internal the style manager
    QicsStyleManager *myStyleManager;
    // \internal the data model (currently unused)
    QicsDataModel *myDataModel;

    // \internal the current (in progress) selection
    QicsSelection myCurrentSelection, myExclusiveSelection;

    // \internal the current (in progress) drag action
    QicsSelectState myCurrentDragAction;

    // \internal the current \b completed selection list
    QicsSelectionList mySelectionList;

    // \internal the current selection action list
    QicsSelectionList *mySelectionActionList;

    // \internal list of header widgets to update
    QicsHeaderGridPV myHeaderList;

    // \internal the cell region that contains cells whose selected property
    // has changed since the last grid redraw.
    QicsRegion myAffectedRegion;

    // \internal the table's selection policy
    QicsSelectionPolicy mySelectionPolicy;

    // \internal the table's current cell selection policy
    QicsSelectCurrentCellPolicy mySelectCurrentCellPolicy;

    // \internal flag indicating if the selection changed during the last operation
    bool mySelectionChangedFlag;
};

#endif //QICSSELECTIONMANAGER_H


