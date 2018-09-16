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

#include "QicsSpanManager.h"

#include "QicsGridInfo.h"
#include "QicsDataModel.h"


QicsSpanManager::QicsSpanManager(QicsGridInfo *grid_info, QObject *parent)
    : QObject(parent),
        myReportChanges(true),
        myInfo(grid_info)
{
}

QicsSpanManager::QicsSpanManager(const QicsSpanManager &sm)
    : QObject(), myReportChanges(true),
        myInfo(sm.myInfo)
{
    QicsSpanList::const_iterator iter, iter_end(sm.myCellSpanList.constEnd());

    for (iter = sm.myCellSpanList.constBegin();iter != iter_end; ++iter) {
        myCellSpanList.push_back(*iter);
    }
}

QicsSpanManager::~QicsSpanManager()
{
}

/*
* Remember, the region is model coordinate based, not visual.
* This means it is not really a region from start to end, but
* a different beast, of start + length
*/
bool QicsSpanManager::addCellSpan(const QicsSpan &sp)
{
    QicsSpan span = sp;

    // check if span is legal - not just one cell
    if (span.height()<=1 && span.width()<=1)
        return false;

    bool f = false;
    QicsSpanList::iterator iter, it_end = myCellSpanList.end();
    for (iter = myCellSpanList.begin(); iter != it_end; ++iter) {
        const QicsSpan &sp = *iter;
        // replace the span if there is one
        if ((sp._row == span._row) && (sp._col == span._col)) {
            (*iter) = span;
            f = true;
            break;
        }
        else {
            if (span.intersects(sp))
                return false;
        }
    }

    if (!f)
        myCellSpanList.push_back(span);

    if (myReportChanges)
        emit spanChanged(span);

    return true;
}

void QicsSpanManager::removeCellSpan(int start_row, int start_col)
{
    QicsSpanList::iterator iter, iter_end(myCellSpanList.end());
    for (iter = myCellSpanList.begin(); iter != iter_end; ++iter) {
        QicsSpan r = *iter;

        if ((r.row() == start_row) && (r.column() == start_col)) {
            myCellSpanList.erase(iter);

            if (myReportChanges)
                emit spanChanged(r);
            return;
        }
    }
}

void QicsSpanManager::removeAllSpans()
{
    myCellSpanList.clear();
}

QicsSpanList *QicsSpanManager::cellSpanList() const
{
    // TODO - map this back to visual coordinates
    return new QicsSpanList(myCellSpanList);
}

bool QicsSpanManager::insideSpan(const QicsGridInfo &gi, int row, int col, QicsRegion &reg_return, bool *spanner) const
{
    if (myCellSpanList.isEmpty())
        return false;

    if (row < 0 || col < 0)
        return false;

    QicsSpanList::const_iterator it_end = myCellSpanList.constEnd();
    for (QicsSpanList::const_iterator iter = myCellSpanList.constBegin(); iter != it_end; ++iter) {
        const QicsSpan &r = *iter;

        const int top = gi.visualRowIndex(gi.firstNonHiddenModelRow(r.row(), r.row() + r.height() - 1));
        const int left = gi.visualColumnIndex(gi.firstNonHiddenModelColumn(r.column(), r.column() + r.width() - 1));
        const int &height = r._nrows;
        const int &width = r._ncols;

        if( (top <= row) && (row < top+height) &&
            (left <= col) && (col < left+width)) {
            // Return the span in VISUAL coordinates
            reg_return = QicsRegion(top, left, top + height-1, left + width - 1);

            // Return true if it is spanner
            if (spanner)
                *spanner = ((row == top) && (col == left));

            return true;
        }
    }

    return false;
}

// AM: consider make this method deprecated
bool QicsSpanManager::isSpanner(const QicsGridInfo &gi, int _row, int _col, QicsRegion &reg_return) const
{
    bool spanner = false;

    if (!insideSpan(gi, _row, _col, reg_return, &spanner))
        return false;

    return spanner;
}

/* The conversions between visual and model here are subtle to get
* the right visual effect.   One choice would be to map the incoming
* coords to the model, and see that intersects any cell.  This mostly
* works, but the problem is that it's wrong.   Let's say our column
* contains rows with the numbers 0 to 1000.   We have a span on cell
* in row 6 for 3 rows (6, 7 and 8).  Sort it lexically, so row 6 is
* now followed by row 60 and then 600.  model[6] appears at visual[556]
* so, visual[557] is in the span.   It is obvious when you put this
* all together
*  visual[667] > model[7] is in the span model[6 for 3]  -> WRONG
*  visual[557] > model[60] is not in the span model[6 for 3] - > WRONG
*
* The proper way to compute this is convert the model coordinates
* in the spans back to visual and see if the incoming visual row
* is in the new span
*  model[6 for 3] -> visual[556 for 3], visual 557 is in span -> RIGHT
*
* You would not believe the number of times I forget this and kept
* flipping between the two methods.   Learn from my pain
* - tony
*/

QicsRegion QicsSpanManager::maxSpanForRow(QicsGridInfo &gi, int row) const
{
    // Note: input and return in visual units
    QicsRegion retval(row, 0, row, QicsLAST_COLUMN);
    int height = 1;

    int modelrow = gi.modelRowIndex(row);
    QicsSpanList::const_iterator iter, iter_end(myCellSpanList.constEnd());
    for (iter = myCellSpanList.constBegin(); iter != iter_end; ++iter) {
        const QicsSpan &r = *iter;

        if (r.row() == modelrow) {
            if (height < r.height())
                height = r.height();
        }
    }

    retval.setEndRow(row + height - 1);
    return retval;
}

QicsRegion QicsSpanManager::maxSpanForColumn(QicsGridInfo &gi, int col) const
{
    // Note: input and return in visual units
    QicsRegion retval(0, col, QicsLAST_ROW, col);
    int width = 1;

    int modelcolumn = gi.modelColumnIndex(col);
    QicsSpanList::const_iterator iter, iter_end(myCellSpanList.constEnd());

    for (iter = myCellSpanList.constBegin(); iter != iter_end; ++iter) {
        const QicsSpan &r = *iter;

        if (r.column() == modelcolumn) {
            if (width < r.width())
                width = r.width();
        }
    }

    retval.setEndColumn(col + width - 1);
    return retval;
}

bool QicsSpan::intersects(const QicsSpan &span) const
{
    /* compute interection.  Consider the 9 regions
    *  A B C
    *  D E F
    *  G H I
    * where this span is in the center at E.
    * We just make sure the other span does not lie in
    * an of the other 8 regions.
    */
    int myEndrow = this->_row + this->_nrows - 1;
    int otherEndrow = span._row + span._nrows  - 1;
    // Eliminate G H I
    if(span._row > myEndrow)
        return false;
    // Eliminate A B C
    if(this->_row > otherEndrow)
        return false;

    int myEndcol = this->_col + this->_ncols - 1;
    int otherEndcol = span._col + span._ncols  - 1;
    // Eliminate C F I
    if(span._col > myEndcol)
        return false;
    // Eliminate A D G
    if(this->_col > otherEndcol)
        return false;

    // the other guy lies somewhere in E, so it hits me
    return true;
}

void QicsSpanManager::insertRows(int num, int start_position)
{
    if (num <= 0 || start_position<0)
        return;

    QicsSpanList::iterator it, it_end(myCellSpanList.end());
    for ( it = myCellSpanList.begin(); it != it_end; ++it ) {
        if ( (*it)._row >= start_position ) {
            // simply move right
            (*it)._row += num;
            continue;
        }
        if ( ((*it)._row < start_position) && ((*it)._row+(*it)._nrows)>start_position) {
            // resize
            (*it)._nrows += num;
            continue;
        }
    }
}

void QicsSpanManager::insertColumns(int num, int start_position)
{
    if (num <= 0 || start_position<0)
        return;

    QicsSpanList::iterator it, it_end(myCellSpanList.end());
    for ( it = myCellSpanList.begin(); it != it_end; ++it ) {
        if ( (*it)._col >= start_position ) {
            // simply move up
            (*it)._col += num;
            continue;
        }
        if ( ((*it)._col < start_position) && ((*it)._col+(*it)._ncols)>start_position) {
            // resize
            (*it)._ncols += num;
            continue;
        }
    }
}

void QicsSpanManager::deleteRows(int num, int start_position)
{
    if (num <= 0 || start_position<0)
        return;

    QicsSpanList::iterator it;
    for ( it = myCellSpanList.begin(); it != myCellSpanList.end(); ++it ) {
        int sl = (*it)._row;
        int sr = (*it)._row + (*it)._nrows;
        int dl = start_position;
        int dr = start_position + num;

        if (sr >= dl) {
            if ( sl >= dl ) {

                if ( sl > dr ) {
                    // simply move span up
                    (*it)._row -= num;
                    continue;
                }
                if ( sr <= dr ) {
                    // delete span
                    myCellSpanList.erase(it);
                    it--;
                    continue;
                }
                if ( sr > dr ) {
                    // delete span top
                    (*it)._nrows = sr-dr;
                    (*it)._row = dl;
                    continue;
                }
            }
            if ( sl < dl ) {
                if ( sr <= dr) {
                    // delete span bottom
                    (*it)._nrows = dl - sl;
                    continue;
                }
                if ( sr > dr) {
                    // delete span center
                    (*it)._nrows = (dl-sl) + (sr-dr) ;
                    continue;
                }
            }
        }
    }
}

void QicsSpanManager::deleteColumns(int num, int start_position)
{
    if (num <= 0 || start_position<0)
        return;

    QicsSpanList::iterator it;
    for ( it = myCellSpanList.begin(); it != myCellSpanList.end(); ++it ) {
        int sl = (*it)._col;
        int sr = (*it)._col + (*it)._ncols;
        int dl = start_position;
        int dr = start_position + num;
        if (sr >= dl) {
            if ( sl >= dl ) {
                if ( sl > dr ) {
                    // simply move span left
                    (*it)._col -= num;
                    continue;
                }
                if ( sr <= dr ) {
                    // delete span
                    myCellSpanList.erase(it);
                    it--;
                    continue;
                }
                if ( sr > dr ) {
                    // delete span left
                    (*it)._ncols = sr-dr;
                    (*it)._col = dl;
                    continue;
                }
            }
            if ( sl < dl ) {
                if ( sr <= dr) {
                    // delete span right
                    (*it)._ncols = dl - sl;
                    continue;
                }
                if ( sr > dr) {
                    // delete span center
                    (*it)._ncols = (dl-sl) + (sr-dr) ;
                    continue;
                }
            }
        }
    }
}

void QicsSpanManager::columnVisibilityChanged(int index, bool visible)
{
    if (myCellSpanList.isEmpty())
        return;

    QicsSpanList::iterator it;
    for (it = myCellSpanList.begin(); it != myCellSpanList.end(); ++it) {
        if ( ((*it)._col <= index) && ((*it)._col+(*it)._ncols)>index) {
            (*it)._ncols += (visible ? 1 : -1);
            emit spanChanged(*it);
        }
    }
}

void QicsSpanManager::rowVisibilityChanged(int index, bool visible)
{
    if (myCellSpanList.isEmpty())
        return;

    QicsSpanList::iterator it;
    for (it = myCellSpanList.begin(); it != myCellSpanList.end(); ++it) {
        if ( ((*it)._row <= index) && ((*it)._row+(*it)._nrows)>index) {
            (*it)._nrows += (visible ? 1 : -1);
            emit spanChanged(*it);
        }
    }
}

////////////////////////////////////////////////////////////////////
/////////////////         Configure to Xml         /////////////////
////////////////////////////////////////////////////////////////////

QDomElement QicsSpanManager::toDomXml(QDomDocument* doc)
{
    QDomElement styleElement = doc->createElement("SpanList");

    //Save Model Row Props
    for( int span=0; span < myCellSpanList.size(); ++span ) {
        if(!myCellSpanList[span].isValid()) {
            QDomElement spanElement = myCellSpanList[span].toDomXml("Span", doc);
            styleElement.appendChild(spanElement);
        }

    }
    return styleElement;
}

void QicsSpanManager::configureFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        if( child.tagName() == "Span" ) {
            QicsSpan span;
            span.configureFromDomXml(child);
            addCellSpan(span);
        }
        child = child.nextSiblingElement();
    }
}


