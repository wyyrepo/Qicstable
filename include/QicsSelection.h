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

#ifndef QICSSELECTION_H
#define QICSSELECTION_H

#include <QVector>
#include "QicsNamespace.h"
#include "QicsICell.h"
#include "QicsRegion.h"

/*!
* \file QicsSelection.h
*
* Defines the selection class and list.
*/


// Represents a simple, rectangular selection

/*!
* \class QicsSelection QicsSelection.h
* \brief Represents a single, contiguous selection
*
* QicsSelection represents a single, contiguous selection in the QicsTable widget.
* A QicsSelection consists of an anchor cell, where the selection begins, and an
* end cell, where the cell ends.  The end cell indices are not necessarily greater
* than the anchor cell (i.e. it is valid for the anchor cell to be (5,5) and the
* end cell to be (0,0)).
*
* If an entire row is selected, leftColumn() will be set to 0, and rightColumn()
* will be set to QicsLAST_COLUMN.  Similarly, if and entire column is selected,
* topRow() will be set to 0, and bottomRow() will be set to QicsLAST_ROW.
*
* This class is used by QicsTable to define table selections.  A complete table
* selection consists of a list of selection objects (QicsSelectionList).
*
* In some cases, a QicsSelection object may represent an "unselection".  This
* can occur if a user control-clicks in an already selected cell.  If this
* happens, this type of selection is indicated by the #selected() property
* returning \b false.
*/

class QICS_EXPORT QicsSelection
{
public:
    /*!
    * Constructor that creates an empty selection
    */
    QicsSelection();

    /*!
    * Constructs a selection from (\a anchor_row, \a anchor_col ) to
    * (\a end_row, \a end_col ).  If \a select_state is \b false,
    * this "selection" represents an action by the user that has caused
    * these cells to be unselected.
    */
    QicsSelection(int anchor_row, int anchor_col,
        int end_row, int end_col, bool select_state = true);

    /*!
    * Constructs a selection from (\a start_row, \a start_col) to
    * (\a end_row, \a end_col) with anchor in (\a anchor_row, \a anchor_col).
    * If \a select_state is \b false,
    * this "selection" represents an action by the user that has caused
    * these cells to be unselected.
    */
    QicsSelection(int anchor_row, int anchor_col,
        int start_row, int start_col,
        int end_row, int end_col, bool select_state = true);

    /*!
    * Constructs for the class
    */
    QicsSelection(const QicsSelection &sel);

    /*!
    * Sets whether the cells in this selection have been selected
    * or unselected.
    */
    inline void setSelected(bool set) { mySelectState = set; }

    /*!
    * Returns whether the cells in this selection have been selected
    * or unselected.
    */
    inline bool selected() const { return mySelectState; }

    /*!
    * Makes selection invalid (isValid() will return false).
    * \sa isValid()
    * \since 2.3
    */
    void invalidate();

    /*!
    * Returns \b true if the selection is valid, or \b false if it is not valid.
    * \sa invalidate()
    */
    bool isValid() const;

    /*!
    * Sets the row index of the anchor cell in the selection.
    */
    inline void setAnchorRow(int anchor_row) {myAnchorRow = anchor_row;}
    /*!
    * Sets the column index of the anchor cell in the selection.
    */
    inline void setAnchorColumn(int anchor_col) {myAnchorCol = anchor_col;}
    /*!
    * Sets the row and column indices of the anchor cell in the selection.
    */
    void setAnchorCell(int anchor_row, int anchor_col);

    /*!
    * Sets the row index of the end cell in the selection.
    * \since 2.2
    */
    inline void setStartRow(int start_row) {myStartRow = start_row;}
    /*!
    * Sets the column index of the end cell in the selection.
    * \since 2.2
    */
    inline void setStartColumn(int start_col) {myStartCol = start_col;}
    /*!
    * Sets the row and column indices of the end cell in the selection.
    * \since 2.2
    */
    void setStartCell(int start_row, int start_col);

    /*!
    * Sets the row index of the end cell in the selection.
    */
    inline void setEndRow(int end_row) {myEndRow = end_row;}
    /*!
    * Sets the column index of the end cell in the selection.
    */
    inline void setEndColumn(int end_col) {myEndCol = end_col;}
    /*!
    * Sets the row and column indices of the end cell in the selection.
    */
    void setEndCell(int end_row, int end_col);

    /*!
    * Returns the row index of the anchor cell in the selection.
    */
    inline int anchorRow() const { return myAnchorRow; }
    /*!
    * Returns the column index of the anchor cell in the selection.
    */
    inline int anchorColumn() const { return myAnchorCol; }
    /*!
    * Returns the anchor cell in the selection.
    * \since 2.3
    */
    inline QicsICell anchorCell() const { return QicsICell(myAnchorRow,myAnchorCol); }

    /*!
    * Returns the row index of the start cell in the selection.
    * \since 2.2
    */
    inline int startRow() const { return myStartRow; }
    /*!
    * Returns the column index of the start cell in the selection.
    * \since 2.2
    */
    inline int startColumn() const { return myStartCol; }
    /*!
    * Returns the start cell in the selection.
    * \since 2.3
    */
    inline QicsICell startCell() const { return QicsICell(myStartRow,myStartCol); }

    /*!
    * Returns the row index of the end cell in the selection.
    */
    inline int endRow() const { return myEndRow; }
    /*!
    * Returns the column index of the end cell in the selection.
    */
    inline int endColumn() const { return myEndCol; }
    /*!
    * Returns the end cell in the selection.
    * \since 2.3
    */
    inline QicsICell endCell() const { return QicsICell(myEndRow,myEndCol); }

    /*!
    * Returns the lesser of the row indices of the anchor and end cells.
    */
    inline int topRow() const { return qMin(myStartRow, myEndRow); }
    /*!
    * Returns the lesser of the column indices of the anchor and end cells.
    */
    inline int leftColumn() const { return qMin(myStartCol, myEndCol); }
    /*!
    * Returns the top left cell in the selection.
    * \since 2.3
    */
    inline QicsICell topLeftCell() const { return QicsICell(topRow(),leftColumn()); }

    /*!
    * Returns the greater of the row indices of the anchor and end cells.
    */
    inline int bottomRow() const { return qMax(myStartRow, myEndRow); }
    /*!
    * Returns the greater of the column indices of the anchor and end cells.
    */
    inline int rightColumn() const { return qMax(myStartCol, myEndCol); }
    /*!
    * Returns the bottom right cell in the selection.
    * \since 2.3
    */
    inline QicsICell bottomRightCell() const { return QicsICell(bottomRow(),rightColumn()); }

    /*!
    * Returns the width of the selection (in columns).
    */
    inline int numColumns() const { return (rightColumn() - leftColumn() + 1); }

    /*!
    * Returns the height of the selection (in rows).
    */
    inline int numRows() const { return (bottomRow() - topRow() + 1); }

    /*!
    * Sets the value of this selection to all of row \a row.
    */
    void setEntireRow(int row);

    /*!
    * Returns true if this selection covers entire row.
    * \since 2.3
    */
    inline bool isEntireRow() const
    { return (!leftColumn() && rightColumn() == Qics::QicsLAST_COLUMN); }

    /*!
    * Sets the value of this selection to all of column \a col.
    */
    void setEntireColumn(int col);

    /*!
    * Returns true if this selection covers entire column.
    * \since 2.3
    */
    inline bool isEntireColumn() const
    { return (!topRow() && bottomRow() == Qics::QicsLAST_ROW); }

    /*!
    * Returns \b true if \a sel intersects with this selection (i.e. there
    * at least one cell that is contained in both selections), or \b false
    * otherwise.
    */
    bool intersects(const QicsSelection &sel);

    /*!
    * Returns \a true if selection contains \a row and \a col inside.
    * \since 2.2
    */
    inline bool contains(int row, int col) const
    {
        if ((row >= topRow()) && (row <= bottomRow()) &&
            (col >= leftColumn()) && (col <= rightColumn()))
            return true;
        return false;
    }

    /*!
    * Returns the intersection of this selection and \a sel.
    */
    QicsSelection operator&(const QicsSelection &sel);

    /*!
    * Returns the intersection of this selection and \a sel.
    */
    QicsSelection intersect(const QicsSelection &sel);

    /*!
    * Returns the union of this selection and \a sel.
    * \since 2.2
    */
    QicsSelection operator|(const QicsSelection &sel);

    /*!
    * Returns the union of this selection and \a sel.
    * \since 2.2
    */
    QicsSelection& operator|=(const QicsSelection &sel)
    { *this = *this | sel; return *this; }

    /*!
    * Returns the region covered by this selection.
    * \since 2.3.1
    */
    inline QicsRegion toRegion() const
    { return QicsRegion(topRow(), leftColumn(), bottomRow(), rightColumn()); }

    /*!
    * Returns \b true if the two selections are identical, or \b false
    * otherwise.
    *
    * \relates QicsSelection
    */
    friend QICS_EXPORT bool operator==(const QicsSelection &r1,
        const QicsSelection &r2);
    /*!
    * Returns \b true if the two selections are not identical, or \b false
    * otherwise.
    *
    * \relates QicsSelection
    */
    friend QICS_EXPORT bool operator!=(const QicsSelection &r1,
        const QicsSelection &r2);
protected:
    /*!
    * \internal
    * are these cells selected or unselected
    */
    bool mySelectState;

    /*!
    * \internal
    * the row index of the anchor cell
    */
    int myAnchorRow;

    /*!
    * \internal
    * the column index of the anchor cell
    */
    int myAnchorCol;

    /*!
    * \internal
    * the row index of the starting cell
    */
    int myStartRow;

    /*!
    * \internal
    * the column index of the starting cell
    */
    int myStartCol;

    /*!
    * \internal
    * the row index of the ending cell
    */
    int myEndRow;

    /*!
    * \internal
    * the column index of the ending cell
    */
    int myEndCol;
};

// A complex selection may have more than one area, hence the separate class

/*!
* \brief Represents a complete table selection
*
* QicsSelectionList represents a complete QicsTable selection.  This list consists
* of 0 or more QicsSelection objects, each of which represents a contiguous,
* rectangular set of selected cells.
*
* QicsSelectionList is implemented as a subclass of \b QValueVector<QicsSelection>.
* As such, you may use all the usual vector methods when using this class.
*/

class QicsDataModel;

class QICS_EXPORT QicsSelectionList : public QVector<QicsSelection>
{
public:
    /*!
    * Constructor for the class
    */
    QicsSelectionList(QicsDataModel *dm);

    inline void setDataModel(QicsDataModel *dm) {m_dataModel = dm;}

    inline QicsDataModel *dataModel() const {return m_dataModel;}

    /*!
    * Returns \b true if (\a row, \a col ) is selected in the list, or
    * \b false otherwise.
    */
    bool isCellSelected(int row, int col) const;

    /*!
    * Returns \b true if the \a row is selected in the list, or
    * \b false otherwise.
    * If \a complete is true, then true will be returned only if the whole row
    * is selected.
    */
    bool isRowSelected(int row, bool complete = true) const;

    /*!
    * Returns \b true if the column \a col is selected in the list, or
    * \b false otherwise.
    * If \a complete is true, then true will be returned only if the whole column
    * is selected.
    */
    bool isColumnSelected(int col, bool complete = true) const;

    /*!
    * Returns QicsRegion covering all the selections, or invalid region if the list is empty.
    * \since 2.2
    */
    QicsRegion region() const;

    /*!
    * Returns vector of selected rows.
    * \since 2.3
    */
    QList<int> rows() const;

    /*!
    * Returns vector of selected columns.
    * \since 2.3
    */
    QList<int> columns() const;
private:
    QicsDataModel *m_dataModel;
};

#endif //QICSSELECTION_H


