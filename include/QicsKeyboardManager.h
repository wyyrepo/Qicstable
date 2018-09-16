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

#ifndef QICSKEYBOARDMANAGER_H
#define QICSKEYBOARDMANAGER_H

#include <QObject>
#include <QKeySequence>
#include <QMap>
#include "QicsNamespace.h"


class QKeyEvent;
class QicsGridGeometry;
class QicsScreenGrid;
/*!
* \class QicsKeyCombination QicsKeyCombination.h
* \brief Class contains key combinations for specific table actions.
*/

class QICS_EXPORT QicsKeyCombination : public QKeySequence
{
public:
    QicsKeyCombination(Qt::KeyboardModifiers modifiers=0, Qt::Key key=Qt::Key_unknown)
        : QKeySequence(modifiers+key),
          myModifiers(modifiers),
          myKey(key)
      {
      }

      /*!
      *  Sets keyboard modifiers for for combination.
      */
      inline void setModifiers(Qt::KeyboardModifiers newModifiers) {myModifiers = newModifiers;}

      /*!
      * Returns keyboard modifiers for combination.
      */
      inline Qt::KeyboardModifiers modifiers() const {return myModifiers;}

      /*!
      * Sets key for for combination.
      */
      inline void setKey(Qt::Key newKey) {myKey = newKey;}

      /*!
      * Returns key for combination.
      */
      inline Qt::Key key() const {return myKey;}

      Qt::KeyboardModifiers myModifiers;
      Qt::Key myKey;
};


/*!
* \class QicsKeyboardManager QicsKeyboardManager.h
* \brief QicsKeyboardManager processes all key events of QicsTable.
*
* QicsKeyboardManager processes all key events of QicsTable, translates them into actions
* and do this actions.
*/

class QICS_EXPORT QicsKeyboardManager : public QObject
{
    Q_OBJECT
public:
    /*!
    * List of all actions that can be done:
    \code
    TraverseLeft
    TraverseRight
    TraverseUp
    TraverseDown

    TraverseBeginningOfTable
    TraverseEndOfTable
    TraverseBeginningOfRow
    TraverseEndOfRow
    TraverseBeginningOfColumn
    TraverseEndOfColumn

    TraversePageUp
    TraversePageDown
    FocusNextInTabOrder
    FocusPreviousInTabOrder
    \endcode

    Theese actions will be used for Tab and Enter keys
    which are traversing to the next row/column
    when last row/column achieved:
    \code
    TraverseLeftWithJump
    TraverseRightWithJump
    TraverseUpWithJump
    TraverseDownWithJump
    \endcode

    * \arg \b TraverseLeft - traverse left.
    * \arg \b TraverseRight - traverse right.
    * \arg \b TraverseUp - traverse up.
    * \arg \b TraverseDown - traverse down.
    * \arg \b TraverseLeftWithJump - traverse left with jumping to the next row/column when the last cell is achieved.
    * \arg \b TraverseRightWithJump - - traverse right with jumping to the next row/column when the last cell is achieved.
    * \arg \b TraverseUpWithJump - - traverse up with jumping to the next row/column when the last cell is achieved.
    * \arg \b TraverseDownWithJump - - traverse down with jumping to the next row/column when the last cell is achieved.
    * \arg \b TraverseBeginningOfTable - traverse to the top-left cell of table.
    * \arg \b TraverseEndOfTable - traverse to the bottom-right cell of table.
    * \arg \b TraverseBeginningOfRow - traverse to the leftmost cell of row.
    * \arg \b TraverseEndOfRow - traverse to the rightmost cell of row.
    * \arg \b TraverseBeginningOfColumn - traverse to the topmost cell of column.
    * \arg \b TraverseEndOfColumn - traverse to the bottommost cell of column.
    * \arg \b TraversePageUp - traverse to one page up.
    * \arg \b TraversePageDown - traverse to one page down.
    * \arg \b FocusNextInTabOrder - focus next grid in tab order.
    * \arg \b FocusPreviousInTabOrder - focus previous grid in tab order.
    */

    enum Action {
        NoAction=0,
        TraverseLeft,
        TraverseRight,
        TraverseUp,
        TraverseDown,

        TraverseLeftWithJump, // This actions will be used for Tab and Enter keys
        TraverseRightWithJump,// which are traversing to the next row/column
        TraverseUpWithJump,   // when last row/column achieved.
        TraverseDownWithJump,

        TraverseLeftWithSelection,
        TraverseRightWithSelection,
        TraverseUpWithSelection,
        TraverseDownWithSelection,

        TraverseBeginningOfTable,
        TraverseEndOfTable,

        TraverseBeginningOfRow,
        TraverseEndOfRow,

        TraverseBeginningOfRowWithSelection,
        TraverseEndOfRowWithSelection,

        TraverseBeginningOfColumn,
        TraverseEndOfColumn,

        TraversePageUp,
        TraversePageDown,

        TraversePageUpWithSelection,
        TraversePageDownWithSelection,

        FocusNextInTabOrder,
        FocusPreviousInTabOrder,

        TraverseBeginningOfTableWithSelection,
        TraverseEndOfTableWithSelection,

        UserBase = 0xff0000
    };

    /*!
    * Specifies whether cell will traverse into frozen regions. The default value
    * of this property is \b true.
    */
    Q_PROPERTY( bool traverseIntoFrozen READ traverseIntoFrozen WRITE setTraverseIntoFrozen )

    /*!
    * Specifies whether cell will jump into frozen regions. The default value
    * of this property is \b true.
    */
    Q_PROPERTY( bool jumpIntoFrozen READ jumpIntoFrozen WRITE setJumpIntoFrozen )

    /*!
    * Specifies whether cell will traverse into headers. The default value
    * of this property is \b false.
    */
    Q_PROPERTY( bool traverseIntoHeaders READ traverseIntoHeaders WRITE setTraverseIntoHeaders )

    /*!
    * Specifies whether cell will jump into headers. The default value
    * of this property is \b false.
    */
    Q_PROPERTY( bool jumpIntoHeaders READ jumpIntoHeaders WRITE setJumpIntoHeaders )

    /*!
    * Specifies whether cell will be selected after traversing. The default value
    * of this property is \b true.
    */
    Q_PROPERTY( bool selectOnTraverse READ selectOnTraverse WRITE setSelectOnTraverse )

    // Constructor for QicsKeyboardManager
    QicsKeyboardManager(QObject* parent=0);
    // Destructor
    ~QicsKeyboardManager();

    void setTableGeometry(QicsGridGeometry* geometry);
    inline QicsGridGeometry* tableGeometry() const {return myTableGeometry;}

    /*!
    * Defines specific key action for key combination.
    */
    void setKeyAction(const QicsKeyCombination& combo, Action action);

    /*!
    * Returns key action that holds \b combo parameter
    */
    Action keyAction(const QicsKeyCombination& combo);

    /*!
    * Does key action \b action in specific screen grid \b grid.
    */
    bool doAction(Action action, QicsScreenGrid* grid);

    /*!
    * Returns \b true if current cell can be traversed into frozen parts
    * otherwise returns \b false.
    * See the \link #traverseIntoFrozen "traverseIntoFrozen" \endlink
    * property for details.
    * \sa setTraverseIntoFrozen()
    */
    inline bool traverseIntoFrozen() const {return myTraverseIntoFrozen;}

    /*!
    * If \b traverse \b true current cell can be traversed into frozen parts
    * otherwise not.
    * See the \link #traverseIntoFrozen "traverseIntoFrozen" \endlink
    * property for details.
    * \sa traverseIntoFrozen()
    */
    inline void setTraverseIntoFrozen(bool traverse) {myTraverseIntoFrozen = traverse;}

    /*!
    * Returns \b true if current cell can jump into frozen parts
    * otherwise returns \b false.
    * See the \link #jumpIntoFrozen "jumpIntoFrozen" \endlink
    * property for details.
    * \sa setJumpIntoFrozen()
    */
    inline bool jumpIntoFrozen() const {return myJumpIntoFrozen;}

    /*!
    * If \b jump \b true current cell can jump into frozen parts
    * otherwise not.
    * See the \link #jumpIntoFrozen "jumpIntoFrozen" \endlink
    * property for details.
    * \sa jumpIntoFrozen()
    */
    inline void setJumpIntoFrozen(bool jump) {myJumpIntoFrozen = jump;}

    /*!
    * Returns \b true if current cell can be traversed into headers
    * otherwise returns \b false.
    * See the \link #traverseIntoHeaders "traverseIntoHeaders" \endlink
    * property for details.
    * \sa setTraverseIntoHeaders()
    */
    inline bool traverseIntoHeaders() const {return myTraverseIntoHeaders;}

    /*!
    * If \b traverse \b true current cell can be traversed into headers
    * otherwise not.
    * See the \link #traverseIntoHeaders "traverseIntoHeaders" \endlink
    * property for details.
    * \sa traverseIntoHeaders()
    */
    inline void setTraverseIntoHeaders(bool traverse) {myTraverseIntoHeaders = traverse;}

    /*!
    * Returns \b true if current cell can jump into headers
    * otherwise returns \b false.
    * See the \link #jumpIntoHeaders "jumpIntoHeaders" \endlink
    * property for details.
    * \sa setJumpIntoHeaders()
    */
    inline bool jumpIntoHeaders() const {return myJumpIntoHeaders;}

    /*!
    * If \b jump \b true current cell can jump into headers
    * otherwise not.
    * See the \link #jumpIntoHeaders "jumpIntoHeaders" \endlink
    * property for details.
    * \sa jumpIntoHeaders()
    */
    inline void setJumpIntoHeaders(bool jump) {myJumpIntoFrozen = jump;}

    /*!
    * Returns \b true if current cell will be selected after traversing.
    * See the \link #selectOnTraverse "selectOnTraverse" \endlink
    * property for details.
    * \sa setSelectOnTraverse()
    */
    inline bool selectOnTraverse() const {return mySelectOnTraverse;}

    /*!
    * If \b select is true current cell will be selected during
    * the traversing, otherwise not.
    * See the \link #selectOnTraverse "selectOnTraverse" \endlink
    * property for details.
    * \sa selectOnTraverse()
    */
    inline void setSelectOnTraverse(bool select) {mySelectOnTraverse = select;}

    /*!
    * Handles traversal keys from specified screen grid \b grid.
    */
    virtual bool handleTraversalKeys(QKeyEvent* e, QicsScreenGrid* grid);

protected:
    /*!
    * Returns \b true if key combination \b combo is mapped,
    * otherwise \b false.
    */
    bool isMappedKeyCombination(const QicsKeyCombination& combo);

    /*!
    * Returns \b true if traversing into screen grid \b grid available,
    * otherwise \b false.
    */
    bool canTraverseInto(QicsScreenGrid* grid);

    /*!
    * Returns \b true if jumping into screen grid \b grid available,
    * otherwise \b false.
    */
    bool canJumpInto(QicsScreenGrid* grid);

    /*!
    * Moves the current cell to the nearest cell to the left of the
    * current cell that is enabled. Returns \b true if cell is moved
    * otherwise \b false.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traverseLeft(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the nearest cell to the right of the
    * current cell that is enabled. Returns \b true if cell is moved
    * otherwise \b false.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traverseRight(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the nearest cell to the up of the
    * current cell that is enabled. Returns \b true if cell is moved
    * otherwise \b false.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traverseUp(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the nearest cell to the down of the
    * current cell that is enabled. Returns \b true if cell is moved
    * otherwise \b false.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traverseDown(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the top-left cell in the grid's
    * current viewport.
    */
    bool traverseBeginningOfTable(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the bottom-right cell in the grid's
    * current viewport.
    */
    bool traverseEndOfTable(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the leftmost cell of the current row
    * in the grid's current viewport.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traverseBeginningOfRow(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the rightmost cell of the current row
    * in the grid's current viewport.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traverseEndOfRow(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the upmost cell of the current column
    * in the grid's current viewport.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traverseBeginningOfColumn(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the bottommost cell of the current column
    * in the grid's current viewport.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traverseEndOfColumn(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the page up cell of the current row
    * in the grid's current viewport.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traversePageUp(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Moves the current cell to the page down cell of the current row
    * in the grid's current viewport.
    * If \b with_jump parameter is \b true current cell will jump
    * to the next row/column when the end of current row/column achieved.
    */
    bool traversePageDown(QicsScreenGrid* grid,bool with_jump = false);

    /*!
    * Focus next screen grid in tab order.
    */
    bool focusNextInTabOrder(QicsScreenGrid* grid);

    /*!
    * Focus previous screen grid in tab order.
    */
    bool focusPreviousInTabOrder(QicsScreenGrid* grid);

    /*!
    * Does the traversing to the cell with  \b row \b col coords
    * in the \b grid screen grid.
    */
    bool traverseAcrossGridsToCell(QicsScreenGrid* grid, int row, int col,bool = true);

    /*!
    * Clears all current cells for all grids.
    */
    void clearCurrentCells();

    /*!
    * Clears table's selection list.
    */
    void clearSelections();

    typedef bool (QicsKeyboardManager::*TraverseMethod)(QicsScreenGrid*,bool);

    /*!
    *  Performs traversing with selection(using ARROW + SHIFT keys)
    *  \b grid parameter - grid that does the traversing
    *  \b tm - pointer to traverse class method.
    */
    bool traverseWithSelection(QicsScreenGrid* grid,TraverseMethod tm);

protected slots:
    /*!
    * Specifies the handler for user defined action \b action in the grid \b grid.
    */
    bool handleUserDefinedAction(Action action, QicsScreenGrid* grid);

private:
    // Current table geometry
    QicsGridGeometry* myTableGeometry;

    // All available mapped key kombinations
    QMap<QicsKeyCombination, Action> myActionMap;

    bool myTraverseIntoFrozen;
    bool myJumpIntoFrozen;
    bool myTraverseIntoHeaders;
    bool myJumpIntoHeaders;

    bool mySelectOnTraverse;

};

#endif //QICSKEYBOARDMANAGER_H


