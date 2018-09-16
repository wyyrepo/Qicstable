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

#include "QicsKeyboardManager.h"

#include <QKeyEvent>
#include "QicsHeaderGrid.h"
#include "QicsSelectionManager.h"
#include "QicsGridGeometry.h"


QicsKeyboardManager::QicsKeyboardManager(QObject* parent)
    : QObject(parent),
        myTableGeometry(0),
        myTraverseIntoFrozen(true),
        myJumpIntoFrozen(false),
        myTraverseIntoHeaders(false),
        myJumpIntoHeaders(false),
        mySelectOnTraverse(true)
{
    // Test Some Defaults
    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_Up), TraverseUp);
    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_Down), TraverseDown);
    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_Left), TraverseLeft);
    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_Right), TraverseRight);

    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Up), TraverseUpWithSelection);
    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Down), TraverseDownWithSelection);
    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Left), TraverseLeftWithSelection);
    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Right), TraverseRightWithSelection);

    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_Enter), TraverseDownWithJump);
    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Enter), TraverseUpWithJump);
    setKeyAction(QicsKeyCombination(Qt::KeypadModifier, Qt::Key_Enter), TraverseDownWithJump);
    setKeyAction(QicsKeyCombination(Qt::KeypadModifier | Qt::ShiftModifier, Qt::Key_Enter), TraverseUpWithJump);
    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_Return), TraverseDownWithJump);
    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Return), TraverseUpWithJump);

    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_Tab), TraverseRightWithJump);
    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_Backtab), TraverseLeftWithJump);
    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Backtab), TraverseLeftWithJump);

    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_Home), TraverseBeginningOfRow);
    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_End), TraverseEndOfRow);

    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Home), TraverseBeginningOfRowWithSelection);
    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_End), TraverseEndOfRowWithSelection);

    setKeyAction(QicsKeyCombination(Qt::ControlModifier, Qt::Key_Home), TraverseBeginningOfTable);
    setKeyAction(QicsKeyCombination(Qt::ControlModifier, Qt::Key_End), TraverseEndOfTable);

    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_PageUp), TraversePageUp);
    setKeyAction(QicsKeyCombination(Qt::NoModifier, Qt::Key_PageDown), TraversePageDown);

    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_PageUp), TraversePageUpWithSelection);
    setKeyAction(QicsKeyCombination(Qt::ShiftModifier, Qt::Key_PageDown), TraversePageDownWithSelection);

    setKeyAction(QicsKeyCombination(Qt::ControlModifier, Qt::Key_Tab), FocusNextInTabOrder);
    setKeyAction(QicsKeyCombination(Qt::ControlModifier, Qt::Key_Backtab), FocusPreviousInTabOrder);
    setKeyAction(QicsKeyCombination(Qt::ShiftModifier | Qt::ControlModifier, Qt::Key_Backtab), FocusPreviousInTabOrder);

    setKeyAction(QicsKeyCombination(Qt::ControlModifier | Qt::ShiftModifier, Qt::Key_Home), TraverseBeginningOfTableWithSelection);
    setKeyAction(QicsKeyCombination(Qt::ControlModifier | Qt::ShiftModifier, Qt::Key_End), TraverseEndOfTableWithSelection);
}

QicsKeyboardManager::~QicsKeyboardManager()
{
}

void QicsKeyboardManager::setTableGeometry(QicsGridGeometry* geometry)
{
    if(myTableGeometry)
        disconnect(myTableGeometry);

    myTableGeometry = geometry;
}

void QicsKeyboardManager::setKeyAction(const QicsKeyCombination& combo, Action action)
{
    myActionMap.insert(combo, action);
}

QicsKeyboardManager::Action QicsKeyboardManager::keyAction(const QicsKeyCombination& combo)
{
    return myActionMap.value(combo);
}

bool QicsKeyboardManager::doAction(Action action, QicsScreenGrid* grid)
{
    switch(action)
    {
    case TraverseLeft:
        return traverseLeft(grid);

    case TraverseLeftWithJump:
        return traverseLeft(grid,true);

    case TraverseLeftWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traverseLeft);

    case TraverseRight:
        return traverseRight(grid);

    case TraverseRightWithJump:
        return traverseRight(grid,true);

    case TraverseRightWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traverseRight);

    case TraverseUp:
        return traverseUp(grid);

    case TraverseUpWithJump:
        return traverseUp(grid,true);

    case TraverseUpWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traverseUp);

    case TraverseDown:
        return traverseDown(grid);

    case TraverseDownWithJump:
        return traverseDown(grid,true);

    case TraverseDownWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traverseDown);

    case TraverseBeginningOfTable:
        return traverseBeginningOfTable(grid);

    case TraverseBeginningOfTableWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traverseBeginningOfTable);

    case TraverseEndOfTableWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traverseEndOfTable);

    case TraverseEndOfTable:
        return traverseEndOfTable(grid);

    case TraverseBeginningOfRow:
        return traverseBeginningOfRow(grid);

    case TraverseEndOfRow:
        return traverseEndOfRow(grid);

    case TraverseBeginningOfRowWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traverseBeginningOfRow);

    case TraverseEndOfRowWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traverseEndOfRow);

    case TraverseBeginningOfColumn:
        return traverseBeginningOfColumn(grid);

    case TraverseEndOfColumn:
        return traverseEndOfColumn(grid);

    case TraversePageUp:
        return traversePageUp(grid);

    case TraversePageUpWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traversePageUp);

    case TraversePageDown:
        return traversePageDown(grid);

    case TraversePageDownWithSelection:
        return traverseWithSelection(grid,&QicsKeyboardManager::traversePageDown);

    case FocusNextInTabOrder:
        return focusNextInTabOrder(grid);

    case FocusPreviousInTabOrder:
        return focusPreviousInTabOrder(grid);

    case UserBase:
        return handleUserDefinedAction(action,grid);

    case NoAction:
    default:
        return false;
    }
}

bool QicsKeyboardManager::handleTraversalKeys(QKeyEvent* ke, QicsScreenGrid* grid)
{
    QicsKeyCombination combo = QicsKeyCombination(ke->modifiers(), (Qt::Key) ke->key());
    bool mapped = isMappedKeyCombination(combo);

    if (!mapped)
        return false;

    Action action = keyAction(combo);

    bool done = doAction(action, grid);
    ke->setAccepted(done);

    return true;
}

bool QicsKeyboardManager::isMappedKeyCombination(const QicsKeyCombination& combo)
{
    return myActionMap.contains(combo);
}

bool QicsKeyboardManager::handleUserDefinedAction(Action action, QicsScreenGrid *grid)
{
    Q_UNUSED(action);
    Q_UNUSED(grid);
    return false;
}

bool QicsKeyboardManager::traverseLeft(QicsScreenGrid* grid, bool with_jump)
{
    // If the traversal succeeded we are done
    // return true to keep the grid from getting the event
    if (grid->traverseLeft())
        return true;
    // Try to move the current cell to a grid left this one
    QicsScreenGrid* nextGrid = myTableGeometry->gridLeftOf(grid);

    if (nextGrid) {
        if (canTraverseInto(nextGrid)) {
            int row = grid->currentCell().row();
            int col = nextGrid->currentViewport().endColumn();
            traverseAcrossGridsToCell(nextGrid, row, col);
            return true;
        }
    }

    if (!with_jump)
        return false;

    int my_row = myTableGeometry->gridRowCoord(grid);
    int my_col = myTableGeometry->gridColCoord(grid);

    //int row = grid->currentCell().row()-1;
    for (int row = grid->currentCell().row()-1; row >= grid->currentViewport().startRow(); --row) {
        for (int j = 4; j >= my_col; --j) {
            for (int i = my_row; i >= 0; --i) {
                QicsScreenGrid* current_grid = myTableGeometry->gridAt(i,j);
                if (current_grid) {
                    for (int col = current_grid->currentViewport().endColumn(); col >= 0; --col) {
                        if (canTraverseInto(current_grid) &&
                            traverseAcrossGridsToCell(current_grid,row,col,grid != current_grid))
                            return true;
                    }
                }
            }
        }
    }

    // Fall through **Should not get here**
    return false;
}

bool QicsKeyboardManager::traverseRight(QicsScreenGrid* grid,bool with_jump)
{
    // If the traversal succeeded we are done
    // return true to keep the grid from getting the event
    if (grid->traverseRight())
        return true;
    // Try to move the current cell to a grid right this one
    QicsScreenGrid *nextGrid = myTableGeometry->gridRightOf(grid);
    if (nextGrid) {
        if (canTraverseInto(nextGrid)) {
            int row = grid->currentCell().row();
            int col = nextGrid->currentViewport().startColumn();
            traverseAcrossGridsToCell(nextGrid, row, col);
            return true;
        }
    }

    if (!with_jump)
        return false;

    int my_row = myTableGeometry->gridRowCoord(grid);
    int my_col = myTableGeometry->gridColCoord(grid);

    for (int row = grid->currentCell().row()+1; row <= grid->currentViewport().endRow(); ++row) {
        for (int j = 0; j <= my_col; ++j) {
            for (int i = my_row; i < 5; ++i) {
                QicsScreenGrid *current_grid = myTableGeometry->gridAt(i,j);
                if (current_grid) {
                    for (int col = current_grid->currentViewport().startColumn(); col <= grid->currentViewport().endColumn(); ++col) {
                        if (canTraverseInto(current_grid) &&
                            traverseAcrossGridsToCell(current_grid,row,col,grid != current_grid))
                            return true;
                    }
                }
            }
        }
    }

    // Fall through **Should not get here**
    return false;
}

bool QicsKeyboardManager::traverseUp(QicsScreenGrid* grid,bool with_jump)
{
    // If the traversal succeeded we are done
    // return true to keep the grid from getting the event
    if (grid->traverseUp())
        return true;
    // Try to move the current cell to a grid above this one
    QicsScreenGrid *nextGrid = myTableGeometry->gridAbove(grid);

    if (nextGrid) {
        if (canTraverseInto(nextGrid)) {
            int row = nextGrid->currentViewport().endRow();
            int col = grid->currentCell().column();
            traverseAcrossGridsToCell(nextGrid, row, col);
            return true;
        }
    }

    if (!with_jump)
        return false;

    int my_row = myTableGeometry->gridRowCoord(grid);
    int my_col = myTableGeometry->gridColCoord(grid);

    int col = grid->currentCell().column()-1;

    for (int j = 0; j <= my_col; ++j) {
        for (int i = 4; i >= my_row; --i) {
            QicsScreenGrid *current_grid = myTableGeometry->gridAt(i,j);
            if (current_grid) {
                for (int row = current_grid->currentViewport().endRow(); row >= 0; --row) {
                    if (canTraverseInto(current_grid) &&
                        traverseAcrossGridsToCell(current_grid,row,col, grid != current_grid))
                            return true;
                }
            }
        }
    }

    // Fall through **Should not get here**
    return false;
}

bool
QicsKeyboardManager::traverseDown(QicsScreenGrid* grid,bool with_jump)
{
    // If the traversal succeeded we are done
    // return true to keep the grid from getting the event
    if (grid->traverseDown())
        return true;
    // Try to move the current cell to a grid down this one
    QicsScreenGrid *nextGrid = myTableGeometry->gridBelow(grid);
    if (nextGrid) {
        if (canTraverseInto(nextGrid)) {
            int row = nextGrid->currentViewport().startRow();
            int col = grid->currentCell().column();
            traverseAcrossGridsToCell(nextGrid, row, col);
            return true;
        }
    }

    if (!with_jump)
        return false;

    int my_row = myTableGeometry->gridRowCoord(grid);
    int my_col = myTableGeometry->gridColCoord(grid);

    int col = grid->currentCell().column()+1;

    for (int j = my_col; j < 5; ++j) {
        for (int i = 0; i <= my_row; ++i) {
            QicsScreenGrid *current_grid = myTableGeometry->gridAt(i,j);
            if (current_grid) {
                for (int row = current_grid->currentViewport().startRow(); row <= grid->currentViewport().endRow(); ++row) {
                    if (canTraverseInto(current_grid) &&
                        traverseAcrossGridsToCell(current_grid,row,col, grid != current_grid))
                            return true;
                }
            }
        }
    }

    // Fall through **Should not get here**
    return false;
}

bool QicsKeyboardManager::traverseBeginningOfTable(QicsScreenGrid* grid,bool)
{
    // XXX Account for frozen and headers
    return grid->traverseToBeginningOfTable();
}

bool QicsKeyboardManager::traverseEndOfTable(QicsScreenGrid* grid,bool)
{
    // XXX Account for frozen and headers
    return grid->traverseToEndOfTable();
}

bool QicsKeyboardManager::traverseBeginningOfRow(QicsScreenGrid* grid,bool)
{
    // XXX Account for frozen and headers
    return grid->traverseToBeginningOfRow();
}

bool QicsKeyboardManager::traverseEndOfRow(QicsScreenGrid* grid,bool)
{
    // XXX Account for frozen and headers
    return grid->traverseToEndOfRow();
}

bool QicsKeyboardManager::traverseBeginningOfColumn(QicsScreenGrid* grid,bool)
{
    // XXX Account for frozen and headers
    return grid->traverseToBeginningOfColumn();
}

bool QicsKeyboardManager::traverseEndOfColumn(QicsScreenGrid* grid,bool)
{
    // XXX Account for frozen and headers
    return grid->traverseToEndOfColumn();
}

bool QicsKeyboardManager::focusNextInTabOrder(QicsScreenGrid* grid)
{
    return grid->focusNextWidget();
}

bool QicsKeyboardManager::focusPreviousInTabOrder(QicsScreenGrid* grid)
{
    return grid->focusPreviousWidget();
}

bool QicsKeyboardManager::canTraverseInto(QicsScreenGrid* grid)
{
    bool isHeader = false;
    bool isFrozen = false;

    if ( qobject_cast<QicsHeaderGrid*>(grid) )
        isHeader = true;
    else
        isFrozen =  grid->isFrozen();

    if (!grid->isVisible())
        return false;

    if (!traverseIntoHeaders() && isHeader)
        return false;

    if (!traverseIntoFrozen() && isFrozen)
        return false;

    return true;
}

bool QicsKeyboardManager::canJumpInto(QicsScreenGrid* grid)
{
    bool isHeader = false;
    bool isFrozen = false;

    if ( qobject_cast<QicsHeaderGrid*>(grid) )
        isHeader = true;
    else
        isFrozen = grid->isFrozen();

    if ( jumpIntoHeaders() && isHeader )
        return true;

    if ( jumpIntoFrozen() && isFrozen )
        return true;

    return false;
}

bool QicsKeyboardManager::traverseAcrossGridsToCell(QicsScreenGrid* grid, int row, int col,bool clear_selection)
{
    bool res = grid->canTraverseToCell(row,col);
    if(res) {
        if(clear_selection) {
            clearCurrentCells();
            clearSelections();
            grid->setFocus(Qt::OtherFocusReason);
        }
        grid->traverseToCell(row,col, mySelectOnTraverse);
    }
    return res;
}

void QicsKeyboardManager::clearCurrentCells()
{
    QicsScreenGrid* grid = 0;

    foreach(grid, myTableGeometry->allGrids()) {
        if(grid)
            grid->uneditCurrentCell();
    }

    if(grid)
        grid->gridInfo().setCurrentCell(QicsICell());

}

void QicsKeyboardManager::clearSelections()
{
    myTableGeometry->mainTableGrid()->gridInfo().selectionManager()->clearSelectionList();
}

bool QicsKeyboardManager::traversePageUp(QicsScreenGrid* grid,bool)
{
    return grid->traversePageUp();
}

bool QicsKeyboardManager::traversePageDown(QicsScreenGrid* grid,bool)
{
    return grid->traversePageDown();
}

bool QicsKeyboardManager::traverseWithSelection(QicsScreenGrid* grid,TraverseMethod tm)
{
    QicsSelectionManager* sm = grid->gridInfo( ).selectionManager();
    if ( sm && sm->selectionPolicy() != Qics::SelectSingleRow ) {
        grid->setSelectOnTraverse( false );
        bool res = (this->*tm)(grid,false);
        if(res)
            grid->extendSelection(grid->currentCell( ).row(),grid->currentCell( ).column( ));
        grid->setSelectOnTraverse( true );
        return res;
    }

    return (this->*tm)(grid,false);
}


