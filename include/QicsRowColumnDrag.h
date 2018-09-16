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

#ifndef QICSROWCOLUMNDRAG_H
#define QICSROWCOLUMNDRAG_H

#include <QMimeData>
#include <QVector>
#include "QicsNamespace.h"

class QicsGridInfo;
class QicsICell;


/*! \internal
* \class QicsRowColumnDrag QicsRowColumnDrag.h
* \brief mime object for row and column drags
*
* This is the drag object used for row and column drags
* to distinguish them from data drags.   You can only
* drop rows onto rows - not columns or cells.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsRowColumnDrag: public QMimeData, public Qics
{
    Q_OBJECT
protected:
    /*!
    * Constructor for the class.  protected, so you must
    * use the foundary.
    */
    QicsRowColumnDrag(QicsGridInfo *, Qics::QicsIndexType _type,
        QWidget *dragSource);

public:
    /*!
    * Class destructor.
    */
    ~QicsRowColumnDrag();

    /*!
    * This foundry will look at the grid and its selections
    * to see if it is a proper drag source.  If so, it creates
    * an instance with the needed data.
    */
    static QicsRowColumnDrag *getDragObject(QicsGridInfo *gi,
        Qics::QicsIndexType type, QicsICell *dragCell,
        QWidget *dragSource);

    // Required to reimplement in class derived from QMimeData.
    bool hasFormat(const QString& str)const;

    QByteArray encodedData(const char *mimetype) const;

    // Returns array of items stored in the object.
    inline const QVector<int> &itemList() const { return items; }

public:
    /* Class methods */
    static bool canDecode(const QMimeData *e, Qics::QicsIndexType axistype);

    /*!
    * Attempt to decode the mime source into a list of row or column
    * indices (depending on the value of \a axistype).
    */
    static bool decode(const QMimeData *e, Qics::QicsIndexType axistype,
        QVector<int> &results);

    /*!
    * Do a decode for the harder stuff like inter-app celldata
    */
    static bool decodeCellData(const QMimeData *e);

private:
    QicsGridInfo *myGridInfo;
    Qics::QicsIndexType myIndexType;
    QVector<int> items;
};

#define QICS_MIME_ROWLIST "application/vnd.ics.rowlist"
#define QICS_MIME_COLUMNLIST "application/vnd.ics.columnlist"
#define QICS_MIME_CELLDATA "application/vnd.ics.celldata"

#endif //QICSROWCOLUMNDRAG_H


