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

#include "QicsRowColumnDrag.h"

#include <QDropEvent>
#include "QicsGridInfo.h"
#include "QicsSelectionManager.h"


QicsRowColumnDrag::QicsRowColumnDrag(QicsGridInfo *gi, Qics::QicsIndexType _type, QWidget *)
    : myGridInfo(gi), myIndexType(_type)
{
}

QicsRowColumnDrag::~QicsRowColumnDrag()
{
}

/*!
* the foundry which looks at the grid info and the selection
* data and squirrels away the selected row or column indices
*/

QicsRowColumnDrag *QicsRowColumnDrag::getDragObject(QicsGridInfo *gi, Qics::QicsIndexType type,
                    QicsICell *dragCell, QWidget *dragSource)
{
    QicsRowColumnDrag *ret = new QicsRowColumnDrag(gi, type, dragSource);
    QicsSelectionList *slist = gi->selectionManager()->selectionList(true);

    if (!slist || slist->isEmpty() || (!slist->isColumnSelected(dragCell->column())
        && !slist->isRowSelected(dragCell->row()))) {
        if (!dragCell) return 0;

        // We have no selection, but a start cell, drag from here.

        ret->items.resize(1);
        ret->items[0] = (type == RowIndex ? gi->modelRowIndex(dragCell->row()) :
            gi->modelColumnIndex(dragCell->column()));
        return ret;
    }

    QicsSelectionList::const_iterator iter, iter_end(slist->constEnd());
    //bool okdrag = false;

    for (iter = slist->constBegin(); iter != iter_end; ++iter) {
        const QicsSelection &sel = *iter;

        int row = sel.topRow();
        int col = sel.leftColumn();
        int erow = sel.bottomRow();
        int ecol = sel.rightColumn();
        QicsRegion reg(row, col, erow, ecol);

        if (type == RowIndex) {
            // skip things that are not row selections
            if (!(col == 0) && (ecol == QicsLAST_COLUMN)) continue;

            //if (dragCell && reg.contains(*dragCell))
            //    okdrag = true;

            for( ; row <= erow; ++row)
                ret->items.append(row);

        }
        else {
            // skip things that are not column selections
            if (!(row == 0) && (erow == QicsLAST_ROW)) continue;

            //if (dragCell && reg.contains(*dragCell))
            //    okdrag = true;

            for( ; col <= ecol; ++col)
                ret->items.append(col);

        }
    }

    if (!ret->items.count()) {
        delete ret;
        return 0;
    }

    qSort(ret->items.begin(), ret->items.end());

    if (type == RowIndex) {
        for (int i = 0; i < ret->items.size(); ++i)
            ret->items[i] = gi->modelRowIndex(ret->items.at(i));
    } else {
        for (int i = 0; i < ret->items.size(); ++i)
            ret->items[i] = gi->modelColumnIndex(ret->items.at(i));
    }

    return ret;
}

bool QicsRowColumnDrag::hasFormat(const QString& str) const
{
    return (str == QICS_MIME_ROWLIST || str == QICS_MIME_COLUMNLIST);
}

/*!
Returns \b true if \a e can be decoded by the QicsRowColumnDrag, otherwise
return \b false.
*/

bool QicsRowColumnDrag::canDecode(const QMimeData* mimesource, Qics::QicsIndexType kind )
{
    const QicsRowColumnDrag *data = static_cast<const QicsRowColumnDrag*>(mimesource);

    if ( kind == RowIndex )
        return data->hasFormat(QICS_MIME_ROWLIST);
    if ( kind == ColumnIndex )
        return data->hasFormat(QICS_MIME_COLUMNLIST);
    return false;
}

QByteArray QicsRowColumnDrag::encodedData(const char *mimetype) const
{
    QByteArray retval;
    QDataStream ds(&retval, QIODevice::WriteOnly);

    if(strcmp(mimetype, "text/plain")==0) {
        // this may happen when dropped on another application

        ds << "drop data as text ";
        ds << ((myIndexType == RowIndex) ? "rows" : "columns");

        for(int i = 0; i < items.count(); ++i)
            ds << items.at(i);
        ds << "\n";

        return retval;
    }

    if(strcmp(mimetype, QICS_MIME_ROWLIST)==0 ||
        strcmp(mimetype, QICS_MIME_COLUMNLIST)==0) {
        ds << items.count();

        for(int i = 0; i < items.count(); ++i)
            ds << items.at(i);

        return retval;
    }

    // #### TODO:
    qDebug("TODO: must implement QicsRowColumnDrag::encodeData(%s)\n", mimetype);
    return retval;
}


/*!
* Decodes the data which is stored (encoded) in \a e and, if
*  successful, fills TODO with the selected data.
*
* \return \b true if there was some data, \b false otherwise.
*/

static void decodeArray(QByteArray &in, QVector<int> &out)
{
    QDataStream ds(&in, QIODevice::ReadOnly);
    int	nitems;

    // get the number of items
    ds >> nitems;

    out.resize(nitems);

    for (int i = 0; i < nitems; ++i)
        ds >> out[i];
}

bool QicsRowColumnDrag::decode(const QMimeData *mimesource,
                          Qics::QicsIndexType type,
                          QVector<int> &results)
{
    const QicsRowColumnDrag *data = static_cast<const QicsRowColumnDrag*>(mimesource);

    if (type == RowIndex
        && data->hasFormat(QICS_MIME_ROWLIST)) {
        QByteArray ba = data->encodedData( QICS_MIME_ROWLIST );

        if (ba.size() > 0) {
            decodeArray(ba, results);
            return true;
        }
    }
    else if (type == ColumnIndex &&
        data->hasFormat(QICS_MIME_COLUMNLIST)) {
        QByteArray ba = data->encodedData( QICS_MIME_COLUMNLIST );

        if (ba.size() > 0) {
            decodeArray(ba, results);
            return true;
        }
    }

    return false;
}

bool QicsRowColumnDrag::decodeCellData(const QMimeData *)
{
    // #### TODO:
#ifdef TODO
    QByteArray ba = e->encodedData( QICS_MIME_CELLDATA );
    if ( ba.size() != 0) {
        qDebug("TODO: must implement QicsRowColumnDrag::decode for celldata\n");
        // return true;
    }
#endif
    return false;
}


