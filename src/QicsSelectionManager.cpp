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

#include "QicsSelectionManager.h"

#include "QicsTable.h"
#include "QicsStyleManager.h"


#ifdef notdef
static void dumpList(QicsSelectionList *list)
{
    if(list == 0) return;
    QicsSelectionList::const_iterator iter;
    for (iter = list->begin(); iter != list->end(); ++iter) {
        const QicsSelection &sel = *iter;

        qDebug("[%d,%d] -- [%d,%d]\n", sel.topRow(), sel.leftColumn(),
            sel.bottomRow(), sel.rightColumn());
    }
}
#endif

//////////////////////////////////////////////////////////////////
/////////////       QicsSelection                    /////////////
//////////////////////////////////////////////////////////////////

QicsSelection::QicsSelection()
    : mySelectState(true),
        myAnchorRow(-1),
        myAnchorCol(-1),
        myStartRow(-1),
        myStartCol(-1),
        myEndRow(-1),
        myEndCol(-1)
{
}

QicsSelection::QicsSelection(int anchor_row, int anchor_col,
                             int end_row, int end_col,
                             bool select_state)
    : mySelectState(select_state),
        myAnchorRow(anchor_row),
        myAnchorCol(anchor_col),
        myStartRow(anchor_row),
        myStartCol(anchor_col),
        myEndRow(end_row),
        myEndCol(end_col)
{
}

QicsSelection::QicsSelection(int anchor_row, int anchor_col,
                             int start_row, int start_col,
                             int end_row, int end_col,
                             bool select_state)
    : mySelectState(select_state),
        myAnchorRow(anchor_row),
        myAnchorCol(anchor_col),
        myStartRow(start_row),
        myStartCol(start_col),
        myEndRow(end_row),
        myEndCol(end_col)
{
}

QicsSelection::QicsSelection(const QicsSelection &sel)
    : mySelectState(sel.mySelectState),
        myAnchorRow(sel.myAnchorRow),
        myAnchorCol(sel.myAnchorCol),
        myStartRow(sel.myStartRow),
        myStartCol(sel.myStartCol),
        myEndRow(sel.myEndRow),
        myEndCol(sel.myEndCol)
{
}

bool QicsSelection::isValid() const
{
    return ((startRow() >= 0) && (startColumn() >= 0) &&
        (endRow() >= 0) && (endColumn() >= 0));
}

void QicsSelection::invalidate()
{
    myAnchorRow = myAnchorCol = myStartRow = myStartCol = myEndRow = myEndCol = -1;
}

void QicsSelection::setAnchorCell(int anchor_row, int anchor_col)
{
    myAnchorRow = anchor_row;
    myAnchorCol = anchor_col;
}

void QicsSelection::setStartCell(int start_row, int start_col)
{
    myStartRow = start_row;
    myStartCol = start_col;
}

void QicsSelection::setEndCell(int end_row, int end_col)
{
    myEndRow = end_row;
    myEndCol = end_col;
}

void QicsSelection::setEntireRow(int row)
{
    myStartRow = myAnchorRow = row;
    myStartCol = myAnchorCol = 0;

    myEndRow = row;
    myEndCol = Qics::QicsLAST_COLUMN;
}

void QicsSelection::setEntireColumn(int col)
{
    myStartRow = myAnchorRow = 0;
    myStartCol = myAnchorCol = col;

    myEndRow = Qics::QicsLAST_ROW;
    myEndCol = col;
}

bool QicsSelection::intersects(const QicsSelection &sel)
{
    return ((qMax(topRow(), sel.topRow()) <=
        qMin(bottomRow(), sel.bottomRow())) &&
        (qMax(leftColumn(), sel.leftColumn()) <=
        qMin(rightColumn(), sel.rightColumn())));
}

QicsSelection QicsSelection::operator&(const QicsSelection &sel)
{
    QicsSelection ns;

    ns.setStartRow(qMax(topRow(), sel.topRow()));
    ns.setEndRow(qMin(bottomRow(), sel.bottomRow()));
    ns.setStartColumn(qMax(leftColumn(), sel.leftColumn()));
    ns.setEndColumn(qMin(rightColumn(), sel.rightColumn()));

    return ns;
}

QicsSelection QicsSelection::operator|(const QicsSelection &sel)
{
    QicsSelection ns;

    ns.setStartRow(qMin(topRow(), sel.topRow()));
    ns.setEndRow(qMax(bottomRow(), sel.bottomRow()));
    ns.setStartColumn(qMin(leftColumn(), sel.leftColumn()));
    ns.setEndColumn(qMax(rightColumn(), sel.rightColumn()));

    return ns;
}

QicsSelection QicsSelection::intersect(const QicsSelection &sel)
{
    return (*this & sel);
}

bool operator==(const QicsSelection &r1, const QicsSelection &r2)
{
    return ((r1.myAnchorRow == r2.myAnchorRow) &&
        (r1.myAnchorCol == r2.myAnchorCol) &&
        (r1.myEndRow == r2.myEndRow) &&
        (r1.myEndCol == r2.myEndCol) &&
        (r1.mySelectState == r2.mySelectState));
}

bool operator!=(const QicsSelection &r1, const QicsSelection &r2)
{
    return ((r1.myAnchorRow != r2.myAnchorRow) ||
        (r1.myAnchorCol != r2.myAnchorCol) ||
        (r1.myEndRow != r2.myEndRow) ||
        (r1.myEndCol != r2.myEndCol) ||
        (r1.mySelectState != r2.mySelectState));
}

////////////////////////////////////////////////////////////////////
/////////////       QicsSelectionList                  /////////////
////////////////////////////////////////////////////////////////////

QicsSelectionList::QicsSelectionList(QicsDataModel *dm)
    : QVector<QicsSelection>(), m_dataModel(dm)
{
}

bool QicsSelectionList::isCellSelected(int row, int col) const
{
    QicsSelectionList::const_iterator iter, iter_end(constEnd());

    for (iter = constBegin(); iter != iter_end; ++iter) {
        if (iter->contains(row, col))
            return true;
    }

    return false;
}

bool QicsSelectionList::isRowSelected(int row, bool complete) const
{
    QicsSelectionList::const_iterator iter, iter_end(constEnd());

    for (iter = constBegin(); iter != iter_end; ++iter) {
        const QicsSelection &sel = *iter;

        if ((row >= sel.topRow()) && (row <= sel.bottomRow()) &&
            (!complete || (complete &&
            (sel.leftColumn() == 0) && (sel.rightColumn() == Qics::QicsLAST_COLUMN)))) {
            return true;
        }
    }

    return false;
}

bool QicsSelectionList::isColumnSelected(int col, bool complete) const
{
    QicsSelectionList::const_iterator iter, iter_end(constEnd());

    for (iter = constBegin(); iter != iter_end; ++iter) {
        const QicsSelection &sel = *iter;

        if ((col >= sel.leftColumn()) && (col <= sel.rightColumn()) &&
            (!complete || (complete &&
            (sel.topRow() == 0) && (sel.bottomRow() == Qics::QicsLAST_ROW)))) {
            return true;
        }
    }

    return false;
}

QicsRegion QicsSelectionList::region() const
{
    QicsRegion r;
    if (isEmpty()) return r;

    QicsSelection sr = first();
    QicsSelectionList::const_iterator iter, iter_end(constEnd());
    for (iter = constBegin(); iter != iter_end; ++iter) {
        const QicsSelection &sel = *iter;
        sr |= sel;
    }

    r.setTop(sr.topRow());
    r.setLeft(sr.leftColumn());
    r.setBottom(sr.bottomRow());
    r.setRight(sr.rightColumn());
    return r;
}

QList<int> QicsSelectionList::rows() const
{
    QList<int> r;
    QicsSelectionList::const_iterator iter, iter_end(constEnd());
    Q_ASSERT(m_dataModel);
    if (!m_dataModel) return r;

    const int lastRow = m_dataModel->lastRow();

    for (iter = constBegin(); iter != iter_end; ++iter) {
        const QicsSelection &sel = *iter;
        const int end_row = qBound(0, sel.bottomRow(), lastRow);
        for (int i = sel.topRow(); i <= end_row; ++i)
            r.append(i);
    }

    return r;
}

QList<int> QicsSelectionList::columns() const
{
    QList<int> c;
    QicsSelectionList::const_iterator iter, iter_end(constEnd());
    Q_ASSERT(m_dataModel);
    if (!m_dataModel) return c;

    const int lastColumn = m_dataModel->lastColumn();

    for (iter = constBegin(); iter != iter_end; ++iter) {
        const QicsSelection &sel = *iter;
        const int end_col = qBound(0, sel.rightColumn(), lastColumn);
        for (int i = sel.leftColumn(); i <= end_col; ++i)
            c.append(i);
    }

    return c;
}


////////////////////////////////////////////////////////////////////
/////////////       QicsSelectionManager               /////////////
////////////////////////////////////////////////////////////////////

QicsSelectionManager::QicsSelectionManager(QObject *parent)
    : QObject(parent),
        myGridInfo(0),
        myStyleManager(0),
        myDataModel(0),
        mySelectionList(0),
        mySelectionActionList(0),
        mySelectionPolicy(SelectMultiple)
{
}

QicsSelectionManager::~QicsSelectionManager()
{
    delete mySelectionActionList;
}

void QicsSelectionManager::setDataModel(QicsDataModel *sm)
{
    myDataModel = sm;

    mySelectionList.setDataModel(sm);

    // #### AM
    if (myStyleManager) myStyleManager->setDataModel(myDataModel);

    myHeaderList.clear();
}

void QicsSelectionManager::addHeader(QicsHeaderGrid *header)
{
    myHeaderList.push_back(header);
}

void QicsSelectionManager::removeHeader(QObject *header)
{
    QicsHeaderGridPV::iterator iter, iter_end(myHeaderList.end());

    for (iter = myHeaderList.begin(); iter != iter_end; ++iter) {
        if (*iter == header) {
            myHeaderList.erase(iter);
            break;
        }
    }
}

bool QicsSelectionManager::canSelectMultiple() const
{
    return ((mySelectionPolicy == SelectMultiple) ||
        (mySelectionPolicy == SelectMultipleRow));
}

QicsSelectionList *QicsSelectionManager::selectionList(bool nocopy) const
{
    if (nocopy)
        return const_cast<QicsSelectionList*>( &mySelectionList );

    return new QicsSelectionList(mySelectionList);
}

QicsSelectionList *QicsSelectionManager::selectionActionList() const
{
    if (mySelectionActionList)
        return new QicsSelectionList(*mySelectionActionList);

    return 0;
}

void QicsSelectionManager::setSelectionList(const QicsSelectionList &sel_list)
{
    // Blow away the old one
    deleteSelection();

    if (SelectNone == mySelectionPolicy)
        return;

    // Copy the new list
    mySelectionActionList = new QicsSelectionList(dataModel());

    QicsSelectionList::const_iterator iter, iter_end(sel_list.constEnd());

    // Go through each selection in list
    for (iter = sel_list.constBegin(); iter != iter_end; ++iter) {
        const QicsSelection &sel = *iter;

        if ((mySelectionPolicy == SelectSingleRow) ||
            (mySelectionPolicy == SelectMultipleRow)) {
            // Skip this selection if not selecting entire row
            if ((sel.leftColumn() != 0) ||
                (sel.rightColumn() != Qics::QicsLAST_COLUMN)) {
                continue;
            }
        }

        if (mySelectionList.count() &&
            ((mySelectionPolicy == SelectSingle) ||
            (mySelectionPolicy == SelectSingleRow))) {
            // We already got one selection
            break;
        }

        // Save it off
        addToSelectionList(sel);
    }

    // Set the selection properties
    for (iter = mySelectionActionList->begin();
        iter != mySelectionActionList->end(); ++iter) {
        QicsSelectState state;

        if ((*iter).selected())
            state = QicsSelectTrue;
        else
            state = QicsSelectFalse;

        setSelectionProperty(*iter, state);
    }

    announceChanges(false);
}

void QicsSelectionManager::clearSelectionList()
{
    deleteSelection();
    announceChanges(false);
}

void QicsSelectionManager::addSelection(QicsSelection &selection)
{
    if ((mySelectionPolicy != SelectMultiple) &&
        (mySelectionPolicy != SelectMultipleRow)) {
        // Can't have more than one selection in other modes
        if (mySelectionList.count())
            return;
    }

    QicsSelectState state;

    if (selection.selected())
        state = QicsSelectTrue;
    else
        state = QicsSelectFalse;

    addToSelectionList(selection);
    setSelectionProperty(selection, state);
    announceChanges(false);
}

void QicsSelectionManager::setSelectionPolicy(QicsSelectionPolicy policy)
{
    if (mySelectionPolicy != policy) {
        mySelectionPolicy = policy;

        if (mySelectionPolicy != SelectMultiple) {
            // Clear the current selection
            deleteSelection();
            announceChanges(false);
        }
    }
}

void QicsSelectionManager::setSelectCurrentCellPolicy(QicsSelectCurrentCellPolicy policy)
{
    if (mySelectCurrentCellPolicy != policy) {
        mySelectCurrentCellPolicy = policy;
    }
}

void QicsSelectionManager::processSelectionEvent(QicsSelectionType stype,
                                                 int begin_row, int begin_col,
                                                 int end_row, int end_col)
{
#ifdef DEBUG_SELECTION
    qDebug("***** QicsSelectionManager(%d, %d) to (%d, %d) %d",
        begin_row, begin_col, end_row, end_col, stype);
#endif

    // Invalidate the region
    myAffectedRegion.setSize(QSize(-1,-1));

    switch (mySelectionPolicy)
    {
    case SelectSingleRow:
    case SelectMultipleRow: {
            // If the user clicked on the column header, we don't allow that.

            if ((begin_row == 0) && (end_row == Qics::QicsLAST_ROW))
                return;

            begin_col = 0;
            end_col = Qics::QicsLAST_COLUMN;
            break;
        }

    default:
        break;
    }

    bool in_progress = false;

    switch (stype)
    {
    case SelectionBegin:
        beginSelection(begin_row, begin_col, end_row, end_col);
        in_progress = true;
        break;
    case SelectionDrag:
        dragSelection(begin_row, begin_col, end_row, end_col);
        in_progress = true;
        break;
    case SelectionEnd:
        endSelection(begin_row, begin_col, end_row, end_col);
        in_progress = false;
        break;
    case SelectionExtend:
        extendSelection(begin_row, begin_col, end_row, end_col);
        in_progress = true;
        break;
    case SelectionAdd:
        addSelection(begin_row, begin_col, end_row, end_col);
        in_progress = true;
        break;
    case SelectionNone:
        deleteSelection();
        in_progress = false;
        break;
    case SelectionReplace:
        replaceSelection(begin_row, begin_col, end_row, end_col);
        in_progress = true;
        break;
    default:
        deleteSelection();
        return;
    }

    announceChanges(in_progress);
}


const QicsSelection* QicsSelectionManager::exclusiveSelection() const
{
    if (myCurrentSelection.isValid() && mySelectionList.isEmpty())
        return &myCurrentSelection;

    if (mySelectionList.count() == 1 && !myCurrentSelection.isValid())
        return &(mySelectionList.first());

    return 0;
}

const QicsSelection* QicsSelectionManager::currentSelection() const
{
    if (myCurrentSelection.isValid())
        return &myCurrentSelection;

    return 0;
}


void QicsSelectionManager::beginSelection(int begin_row, int begin_col,
                                          int end_row, int end_col)
{
    // for exclusive mode
    // AM: make affected region from ALL the selections in the list before!
    QicsRegion reg;
    const QicsSelection *es = exclusiveSelection();
    if (es) reg = QicsRegion(es->topRow(), es->leftColumn(), es->endRow(), es->endColumn());

    deleteSelection();

    if (mySelectionPolicy == SelectNone)
        return;

    myCurrentSelection = QicsSelection(begin_row, begin_col, end_row, end_col, true);
    myCurrentDragAction = QicsSelectTrue;

    setSelectionProperty(qMin(begin_row, end_row),
        qMin(begin_col, end_col),
        qMax(begin_row, end_row),
        qMax(begin_col, end_col),
        QicsSelectTrue);

    // for exclusive mode
    if (myAffectedRegion.isValid())
        myAffectedRegion |= reg;
    else
        myAffectedRegion = reg;
}

void QicsSelectionManager::dragSelection(int begin_row, int begin_col,
                                         int end_row, int end_col)
{
    if ((mySelectionPolicy != SelectMultiple) &&
        (mySelectionPolicy != SelectMultipleRow)) {
        // Can't drag select in other modes
        return;
    }

    QicsSelection &sel = myCurrentSelection;

    if (!sel.isValid())
        return;

    if ((end_row != sel.endRow()) && (end_col != sel.endColumn())) {
        extendSelection(begin_row, begin_col, end_row, end_col);
        return;
    }

    QPoint new_tl(qMin(sel.anchorColumn(), end_col),
        qMin(sel.anchorRow(), end_row));
    QPoint new_br(qMax(sel.anchorColumn(), end_col),
        qMax(sel.anchorRow(), end_row));

    QRect current_selection(new_tl, new_br);

    // If the old end cell is inside of the current selection rectangle,
    // the selection is growing
    bool growing = current_selection.contains(sel.endColumn(), sel.endRow());

    // Inside means closer to the anchor cell
    int inside_row = (growing ? sel.endRow() : end_row);
    int inside_col = (growing ? sel.endColumn() : end_col);
    int outside_row = (!growing ? sel.endRow() : end_row);
    int outside_col = (!growing ? sel.endColumn() : end_col);

    int inside_row_adj, inside_col_adj;

    if (inside_row == outside_row)
        inside_row_adj = 0;
    else if (inside_row < outside_row)
        inside_row_adj = 1;
    else
        inside_row_adj = -1;

    if (inside_col == outside_col)
        inside_col_adj = 0;
    else if (inside_col < outside_col)
        inside_col_adj = 1;
    else
        inside_col_adj = -1;

    QicsSelectState action;

    if (growing)
        action = myCurrentDragAction;
    else
        action = (myCurrentDragAction == QicsSelectTrue) ? QicsSelectFalse : QicsSelectTrue;

    if (inside_col_adj) {
        setSelectionProperty(qMin(sel.anchorRow(), outside_row),
            qMin(inside_col+inside_col_adj, outside_col) ,
            qMax(sel.anchorRow(), outside_row),
            qMax(inside_col+inside_col_adj, outside_col),
            action);
    }

    if (inside_row_adj) {
        setSelectionProperty(qMin(inside_row+inside_row_adj, outside_row),
            qMin(sel.anchorColumn(), inside_col),
            qMax(inside_row+inside_row_adj, outside_row),
            qMax(sel.anchorColumn(), inside_col),
            action);
    }

    // Update the selection with the new end cell
    sel.setEndCell(end_row, end_col);
}

void QicsSelectionManager::extendSelection(int begin_row, int begin_col,
                                           int end_row, int end_col)
{
    if ((mySelectionPolicy != SelectMultiple) &&
        (mySelectionPolicy != SelectMultipleRow)) {
        // Can't extend selections in other modes
        return;
    }

    QicsSelection &sel = findSelectionBlock(begin_row, begin_col);

    if (!sel.isValid())
        return;

    QicsSelectState action;

    if (myCurrentDragAction == QicsSelectTrue)
        action = QicsSelectFalse;
    else if (myCurrentDragAction == QicsSelectFalse)
        action = QicsSelectTrue;
    else if (myCurrentDragAction == QicsSelectTrueRevert)
        action = QicsSelectFalseRevert;
    else
        action = QicsSelectTrueRevert;

    setSelectionProperty(qMin(begin_row, sel.endRow()),
        qMin(begin_col, sel.endColumn()),
        qMax(begin_row, sel.endRow()),
        qMax(begin_col, sel.endColumn()),
        action);

    if (myCurrentDragAction == QicsSelectTrue)
        action = QicsSelectTrue;
    else if (myCurrentDragAction == QicsSelectFalse)
        action = QicsSelectFalse;
    else if (myCurrentDragAction == QicsSelectTrueRevert)
        action = QicsSelectTrue;
    else
        action = QicsSelectFalse;

    setSelectionProperty(qMin(begin_row, end_row),
        qMin(begin_col, end_col),
        qMax(begin_row, end_row),
        qMax(begin_col, end_col),
        action);

    // Update the selection with the new end cell
    sel.setEndCell(end_row, end_col);
}

void QicsSelectionManager::addSelection(int begin_row, int begin_col,
                                        int end_row, int end_col)
{
    if ((mySelectionPolicy != SelectMultiple) &&
        (mySelectionPolicy != SelectMultipleRow)) {
        // Can't have more than one selection in other modes
        if (mySelectionList.count())
            return;
    }

    bool set;
    if ((begin_col == 0) && (end_col == Qics::QicsLAST_COLUMN)) {
        // This means that we are selecting the entire row
        set = (* static_cast<bool *>
            (myStyleManager->getRowProperty(myGridInfo->modelRowIndex(begin_row),
            QicsCellStyle::Selected)));
    }
    else if ((begin_row == 0) && (end_row == Qics::QicsLAST_ROW)) {
        // This means that we are selecting the entire column
        set = (* static_cast<bool *>
            (myStyleManager->getColumnProperty(myGridInfo->modelColumnIndex(begin_col),
            QicsCellStyle::Selected)));
    }
    else
        set = (* static_cast<bool *>
        (myStyleManager->getCellProperty(myGridInfo->modelRowIndex(begin_row),
        myGridInfo->modelColumnIndex(begin_col),
        QicsCellStyle::Selected)));

    myCurrentDragAction = (set ? QicsSelectFalseRevert : QicsSelectTrueRevert);
    myCurrentSelection.setSelected(!set);

    myCurrentSelection.setAnchorCell(begin_row, begin_col);
    myCurrentSelection.setStartCell(begin_row, begin_col);
    myCurrentSelection.setEndCell(end_row, end_col);

    setSelectionProperty(qMin(begin_row, end_row),
        qMin(begin_col, end_col),
        qMax(begin_row, end_row),
        qMax(begin_col, end_col),
        (set ? QicsSelectFalse : QicsSelectTrue));
}

void QicsSelectionManager::endSelection(int /*begin_row*/, int /*begin_col*/,
                                        int /*end_row*/, int /*end_col*/)
{
    if (!myCurrentSelection.isValid())
        return;

    // Update region from the last selection
    if (!myCurrentSelection.selected())
        invalidateSelection(myCurrentSelection);

    addToSelectionList(myCurrentSelection);

    // Invalidate the current selection item
    myCurrentSelection.invalidate();

    myCurrentDragAction = QicsSelectTrue;
}

void QicsSelectionManager::deleteSelection()
{
    if (mySelectionList.count()) {
        QicsSelectionList::const_iterator iter, iter_end(mySelectionList.constEnd());

        for (iter = mySelectionList.constBegin(); iter != iter_end; ++iter) {
            const QicsSelection &sel = *iter;

#ifdef DEBUG_SELECTION2
            qDebug("invalidateSelection (%d, %d) (%d, %d)\n",
                sel.anchorRow(), sel.anchorColumn(),
                sel.endRow(), sel.endColumn());
#endif

            invalidateSelection(sel);
        }

        mySelectionList.clear();
    }

    if (myCurrentSelection.isValid()) {
        invalidateSelection(myCurrentSelection);
        myCurrentSelection.invalidate();
    }

    if (myExclusiveSelection.isValid()) {
        invalidateSelection(myExclusiveSelection);
        myExclusiveSelection.invalidate();
    }

    if (mySelectionActionList) {
        delete mySelectionActionList;
        mySelectionActionList = 0;
    }
}

void QicsSelectionManager::invalidateSelection(const QicsSelection &sel)
{
    setSelectionProperty(sel, QicsSelectFalse);
}

void QicsSelectionManager::validateSelection(const QicsSelection &sel)
{
    setSelectionProperty(sel, QicsSelectTrue);
}

void QicsSelectionManager::replaceSelection(int begin_row, int begin_col, int end_row, int end_col)
{
    QicsSelection *current = &myCurrentSelection;
    if (!myCurrentSelection.isValid()) {
        if (mySelectionList.isEmpty()) return;
        current = &(mySelectionList.last());
    }

    // clear current selection if set
    if (current->isValid())
        invalidateSelection(*current);

    if (!canSelectMultiple())
        // Can't make selection in other modes
        return;

    // set new selection
    QicsSelection sl(current->anchorRow(), current->anchorColumn(), begin_row, begin_col, end_row, end_col);
    validateSelection(*current = sl);
}

void QicsSelectionManager::addToSelectionList(const QicsSelection &sel)
{
    if (!sel.isValid())
        return;

    if (!mySelectionActionList)
        mySelectionActionList = new QicsSelectionList(dataModel());

    mySelectionActionList->push_back(sel);

    if (sel.selected())
        mySelectionList.push_back(sel);
    else {
        QicsSelectionList *newlist = new QicsSelectionList(dataModel());

        QicsSelectionList::iterator iter, iter_end(mySelectionList.end());
        for (iter = mySelectionList.begin(); iter != iter_end; ++iter)
        {
            QicsSelection &s = *iter;

            if (s.intersects(sel))
            {
                QicsSelection is = s & sel;

                // =======================
                // |  s                  |
                // |        S1           |
                // |                     |
                // |------==========-----|
                // |      |        |     |
                // |  S3  |  is    | S4  |
                // |      |        |     |
                // |------==========-----|
                // |                     |
                // |       S2            |
                // |                     |
                // =======================

                if (is.topRow() > s.topRow()) {     // S1
                    QicsSelection s1;

                    s1.setStartRow(s.topRow());
                    s1.setEndRow(is.topRow() - 1);
                    s1.setStartColumn(s.leftColumn());
                    s1.setEndColumn(s.rightColumn());

                    newlist->push_back(s1);
                }

                if (is.bottomRow() < s.bottomRow()) {  // S2
                    QicsSelection s2;

                    s2.setStartRow(is.bottomRow() + 1);
                    s2.setEndRow(s.bottomRow());
                    s2.setStartColumn(s.leftColumn());
                    s2.setEndColumn(s.rightColumn());

                    newlist->push_back(s2);
                }

                if (is.leftColumn() > s.leftColumn()) {  // S3
                    QicsSelection s3;

                    s3.setStartRow(is.topRow());
                    s3.setEndRow(is.bottomRow());
                    s3.setStartColumn(s.leftColumn());
                    s3.setEndColumn(is.leftColumn() - 1);

                    newlist->push_back(s3);
                }

                if (is.rightColumn() < s.rightColumn()) { // S4
                    QicsSelection s4;

                    s4.setStartRow(is.topRow());
                    s4.setEndRow(is.bottomRow());
                    s4.setStartColumn(is.rightColumn() + 1);
                    s4.setEndColumn(s.rightColumn());

                    newlist->push_back(s4);
                }
            }
            else
                newlist->push_back(s);
        }

        // replace the selected list
        mySelectionList = *newlist;
    }
}

static QicsSelection invalidSelection;

QicsSelection &QicsSelectionManager::findSelectionBlock(int anchor_row,
                                                        int anchor_col)
{
    if (myCurrentSelection.isValid()) {
        if ((myCurrentSelection.anchorRow() == anchor_row) &&
            (myCurrentSelection.anchorColumn() == anchor_col)) {
            return (myCurrentSelection);
        }
    }

    QicsSelectionList::iterator iter, iter_end(mySelectionList.end());

    if (mySelectionList.isEmpty())
        return invalidSelection;

    for (iter = mySelectionList.begin(); iter != iter_end; ++iter) {
        QicsSelection &sel = *iter;

        if ((sel.anchorRow() == anchor_row) &&
            (sel.anchorColumn() == anchor_col)) {
            return (sel);
        }
    }

    return invalidSelection;
}

void QicsSelectionManager::setSelectionProperty(const QicsSelection &selection,
                                                QicsSelectionManager::QicsSelectState sel)
{
    setSelectionProperty(   selection.topRow(),
        selection.leftColumn(),
        selection.bottomRow(),
        selection.rightColumn(),
        sel);
}

void QicsSelectionManager::setSelectionProperty(int begin_row, int begin_col,
                                                int end_row, int end_col,
                                                QicsSelectionManager::QicsSelectState sel)
{
    int i, j;
    if (begin_row > end_row) qSwap(end_row, begin_row);
    if (begin_col > end_col) qSwap(end_col, begin_col);

    //#define DEBUG_SELECTION1
#ifdef DEBUG_SELECTION1
    qDebug("$$$ setSelectionProperty (%d, %d) to (%d, %d) %d\n",
        begin_row, begin_col, end_row, end_col, sel);
#endif

    if (!myStyleManager)
        return;

    if (!myDataModel)
        return;

    int lastCol = Qics::QicsLAST_COLUMN;
    int lastRow = Qics::QicsLAST_ROW;

    if ((myDataModel) && (myDataModel->lastColumn() >= 0))
        lastCol = myDataModel->lastColumn();

    if ((myDataModel) && (myDataModel->lastRow() >= 0))
        lastRow = myDataModel->lastRow();

    if (lastRow == Qics::QicsLAST_ROW) lastRow = 0;
    if (lastCol == Qics::QicsLAST_COLUMN) lastCol = 0;

    // Turn off the property change signal so the grids don't redraw each time
    // through the loop.
    myStyleManager->setReportChanges(false);

    bool skip_rest_of_rows = false;
    for (i = begin_row; (i <= end_row) && (i <= lastRow) && !skip_rest_of_rows; ++i) {
        if ((begin_col == 0) && (end_col == QicsLAST_COLUMN)) {
            // This means that we are selecting the entire row

            setRowSelectionProperty(*myGridInfo, i, sel, myStyleManager);

            QicsHeaderGridPV::const_iterator iter, iter_end(myHeaderList.constEnd());

            for (iter = myHeaderList.constBegin(); iter != iter_end; ++iter) {
                const QicsHeaderGrid *hdr = *iter;

                if (hdr->type() == RowHeader) {
                    QicsGridInfo &gi = hdr->gridInfo();
                    setRowSelectionProperty(gi, i, sel, gi.styleManager());
                }
            }
        }
        else {
            for (j = begin_col; (j <= end_col) && (j <= lastCol); ++j) {
                if ((begin_row == 0) && (end_row == QicsLAST_ROW)) {
                    // This means that we are selecting the entire column
                    skip_rest_of_rows = true;

                    setColumnSelectionProperty(*myGridInfo, j, sel, myStyleManager);

                    QicsHeaderGridPV::const_iterator iter, iter_end(myHeaderList.constEnd());

                    for (iter = myHeaderList.constBegin(); iter != iter_end; ++iter){
                        const QicsHeaderGrid *hdr = *iter;

                        // int modelCol = hdr->gridInfo().modelColumnIndex(j);
                        if (hdr->type() == ColumnHeader)
                            setColumnSelectionProperty(hdr->gridInfo(),
                            j, sel,
                            hdr->gridInfo().styleManager());
                    }
                }
            }
        }
    }

    // Now we can turn style manager reporting back on
    myStyleManager->setReportChanges(true);

    // Update the affected region
    QicsRegion region(begin_row, begin_col, end_row, end_col);

    if (myAffectedRegion.isValid())
        myAffectedRegion |= region;
    else
        myAffectedRegion = region;
}

void QicsSelectionManager::setRowSelectionProperty(QicsGridInfo &info, int row,
                                              QicsSelectionManager::QicsSelectState sel,
                                              QicsStyleManager *sm)
{
    bool set;
    int mrow = info.modelRowIndex(row);

    switch (sel)
    {
    case QicsSelectionManager::QicsSelectTrue:
        set = true;
        break;
    case QicsSelectionManager::QicsSelectFalse:
        set = false;
        break;
    default: {
            bool cur = mySelectionList.isRowSelected(mrow);

            if (sel == QicsSelectTrueRevert) {
                if (!cur)
                    return;
                else
                    set = true;
            }
            else {
                if (cur)
                    return;
                else
                    set = false;
            }
        }
    }

    if (set) {
        bool val = true;
        sm->setRowProperty(mrow, false, QicsCellStyle::Selected,
            static_cast<void *> (&val), false);
    }
    else {
        // This is an optimization.  Many times, the only property set on
        // a row will be QicsSelected, just because it was selected at one
        // time.  In order to possibly save space, we CLEAR the property
        // if it is unselected.  The style manager may remove the row's
        // style altogether if QicsSelected was the only thing set.

        // NOTE: This will break if the default value of QicsSelected is
        //       not false!!!!

        sm->clearRowProperty(mrow, false, QicsCellStyle::Selected);
    }

    if (info.gridType() == Qics::TableGrid)
        emit rowSelected(/*visual_row*/row, bool(sel));
}

void QicsSelectionManager::setColumnSelectionProperty(QicsGridInfo &info, int col,
                                                 QicsSelectionManager::QicsSelectState sel,
                                                 QicsStyleManager *sm)
{
    bool set;
    int mcol = info.modelColumnIndex(col);

    switch (sel)
    {
    case QicsSelectionManager::QicsSelectTrue:
        set = true;
        break;
    case QicsSelectionManager::QicsSelectFalse:
        set = false;
        break;
    default: {
            bool cur = mySelectionList.isColumnSelected(mcol);

            if (sel == QicsSelectTrueRevert) {
                if (!cur)
                    return;
                else
                    set = true;
            }
            else {
                if (cur)
                    return;
                else
                    set = false;
            }
        }
    }

    if (set) {
        bool val = true;
        sm->setColumnProperty(mcol, false, QicsCellStyle::Selected,
            static_cast<void *> (&val), false);
    }
    else {
        // This is an optimization.  Many times, the only property set on
        // a column will be QicsSelected, just because it was selected at one
        // time.  In order to possibly save space, we CLEAR the property
        // if it is unselected.  The style manager may remove the column's
        // style altogether if QicsSelected was the only thing set.

        // NOTE: This will break if the default value of QicsSelected is
        //       not false!!!!

        sm->clearColumnProperty(mcol, false, QicsCellStyle::Selected);
    }

    if (info.gridType() == Qics::TableGrid)
        emit columnSelected(/*visual_column*/col, bool(sel));
}

void QicsSelectionManager::announceChanges(bool in_progress)
{
    QicsSelection *s = 0;
    if (myCurrentSelection.isValid() && mySelectionList.isEmpty())
        s = &myCurrentSelection;
    else if (mySelectionList.count() == 1)
        s = &(mySelectionList.first());

    if (s) {
        if (myAffectedRegion.isValid())
            myAffectedRegion |= s->toRegion();
        else
            myAffectedRegion = s->toRegion();
    }
    emit selectionCellsChanged(myAffectedRegion);
    emit selectionListChanged(in_progress);

    if(!myGridInfo->blockSelectionSignals())
        emit selectionListChangedExternal(in_progress);
}

void QicsSelectionManager::orderChanged(Qics::QicsIndexType type, int* vismap, int size)
{
#if notdef
    qDebug("selectionMgr: orderChanged(%d)\n", type);
    for(int i = 0; i < vismap.size(); ++i) {
        if(vismap.at(i) != i) {
            qDebug("%d -> %d (model %d)\n", i, vismap[i],
                type == RowIndex ?
                myGridInfo->modelRowToVisualIndex(i) :
            myGridInfo->modelColumnToVisualIndex(i)
                );
        }
    }
    dumpList(mySelectionList);
#endif

    if (mySelectionList.isEmpty()) return;

    QicsDataModel *dm = myGridInfo->dataModel();
    if (!dm)
        return;

    QicsSelectionList *toAdd = new QicsSelectionList(dataModel());

    QicsSelectionList::iterator iter;
    for (iter = mySelectionList.begin(); iter != mySelectionList.end();++iter) {
        QicsSelection &sel = *iter;

        int top = sel.topRow();
        int left = sel.leftColumn();
        int bottom = sel.bottomRow();
        int right = sel.rightColumn();

        Q_ASSERT(top < size || left < size || bottom < size || right < size);
        if (top >= size || left >= size || bottom >= size || right >= size)
            return;

        if ((left == 0) && (right == QicsLAST_COLUMN)) {
            // row selection
            if(type == RowIndex) {
                sel.setStartRow(vismap[top]);
                sel.setAnchorRow(vismap[top]);
                sel.setEndRow(vismap[top]);
                if(top != bottom){
                    if (bottom >= dm->numRows())
                        bottom = dm->numRows()-1;

                    for(int r = top+1; r <= bottom; ++r) {
                        toAdd->push_back(QicsSelection(vismap[r], left,
                            vismap[r], right));
                    }
                }
            }
        }
        else if ((top == 0) && (bottom == QicsLAST_ROW)) {
            // column selection
            if(type == ColumnIndex) {
                sel.setStartColumn(vismap[left]);
                sel.setAnchorColumn(vismap[left]);
                sel.setEndColumn(vismap[left]);
                if(left == right) {
                    if (right >= dm->numColumns())
                        right = dm->numColumns()-1;

                    for(int c = left+1; c <= right; ++c) {
                        toAdd->push_back(QicsSelection(top, vismap[c],
                            bottom, vismap[c]));
                    }
                }
            }
        }
        else if(top == bottom && left == right) {
            // single cell - we can always adjust that
            if(type == RowIndex) {
                sel.setStartRow(vismap[top]);
                sel.setAnchorRow(vismap[top]);
                sel.setEndRow(vismap[top]);
            } else {
                sel.setStartColumn(vismap[left]);
                sel.setAnchorColumn(vismap[left]);
                sel.setEndColumn(vismap[left]);
            }
        }
        else {
            /* It is a region.  This gets tricky.
            * One rule Chris and I thought of was.
            * When sorting a set of rows (columns)
            * Any cell or region that intersects the
            * sorted set, but is not the entire set, gets
            * tossed.  The others get mapped.
            * But, how does that deal with moved rows.
            */
            if(type == RowIndex) {
                sel.setStartRow(vismap[top]);
                sel.setAnchorRow(vismap[top]);
                sel.setEndRow(vismap[top]);
                for(int r = top+1; r <= bottom; ++r) {
                    toAdd->push_back(QicsSelection(vismap[r], left,
                        vismap[r], right));
                }
            }
            else {
                sel.setStartColumn(vismap[left]);
                sel.setAnchorColumn(vismap[left]);
                sel.setEndColumn(vismap[left]);
                for(int c = left+1; c <= right; ++c) {
                    toAdd->push_back(QicsSelection(top, vismap[c],
                        bottom, vismap[c]));
                }
            }
        }
    }

    iter = mySelectionList.begin();
    bool ifSelListChanged = false;
    while (iter != mySelectionList.end()) {
        if (!iter->isValid()) {
            iter = mySelectionList.erase(iter);
            ifSelListChanged = true;
        }
        else
            ++iter;
    }
    if (ifSelListChanged)
        announceChanges(false);

    // now we have the additions, splice them in
#if notdef
    qDebug("=== Additions list ==\n");
    dumpList(toAdd);
#endif
    QicsSelectionList::const_iterator additer, additer_end(toAdd->constEnd());
    for (additer = toAdd->constBegin(); additer != additer_end; ++additer) {
        if (additer->isValid())
            mySelectionList.push_back(*additer);
    }
    delete toAdd;

    delete mySelectionActionList;
    mySelectionActionList = 0;

#if notdef
    qDebug("==== NEW LIST IS ==\n");
    dumpList(mySelectionList);
#endif
}

void QicsSelectionManager::deleteRows(int num, int start_position)
{
    if (num <= 0 || start_position < 0) return;

    if (mySelectionList.isEmpty()) return;

    QicsSelectionList::iterator it;
    for ( it = mySelectionList.begin(); it != mySelectionList.end(); ++it ) {
        QicsSelection &sel = *it;

        if (sel.topRow() >= start_position &&
            sel.bottomRow() <= start_position) {
            // We need to delete only one column near selected
            // Just do nothing
            continue;
        }

        if (sel.topRow() >= start_position &&
            sel.bottomRow() < start_position+num) { // delete selection
            mySelectionList.erase(it);
            --it;
            continue;
        }
    }

    announceChanges(false);
}

void QicsSelectionManager::deleteColumns(int num, int start_position)
{
    if (num <= 0 || start_position < 0) return;

    if (mySelectionList.isEmpty()) return;

    QicsSelectionList::iterator it;
    for ( it = mySelectionList.begin(); it != mySelectionList.end(); ++it ) {
        QicsSelection &sel = *it;

        if (sel.startColumn() >= start_position &&
            sel.leftColumn() <= start_position) {
            // We need to delete only one column near selected
            // Just do nothing
            continue;
        }

        if (sel.startColumn() >= start_position &&
            sel.rightColumn() < start_position+num) {   // delete selection
            mySelectionList.erase(it);
            --it;
            continue;
        }

    }

    announceChanges(false);
}


