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

#ifndef QICSTABLEREGIONDRAG_H
#define QICSTABLEREGIONDRAG_H

#include <QMimeData>

#include "QicsNamespace.h"
#include "QicsCell.h"

class QicsGridInfo;
class QicsICell;
class QicsDataModel;
class QicsSelectionList;
class QicsTableCommon;

/*! \internal
* \class QicsTableRegionDrag QicsTableRegionDrag.h
* \nosubgrouping
* \brief mime object for table region drag operations and cut/paste
*
* This is the drag object used for table region data drag operations, as
* well as cut, copy, and paste operations.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsTableRegionDrag: public QMimeData, public Qics
{
    Q_OBJECT
protected:

    /*!
    * Constructor for the class.  protected, so you must
    * use the foundry.
    */
    QicsTableRegionDrag(QicsTableCommon *tableCommon,const QicsSelectionList *slist,
        QWidget *drag_source);

public:
    /*!
    * This foundry will look at the grid and its selections
    * to see if it is a proper drag source.  If so, it creates
    * an instance with the needed data.
    */
    static QicsTableRegionDrag *getDragObject(QicsTableCommon *tableCommon,
        const QicsSelectionList *slist,
        QicsICell *dragCell,
        QWidget *dragSource);

    /* Class methods */

    static bool canDecode(const QMimeData *e);

    /*!
    * Do a decode but only for the intra application types rowlist and
    * columnlist
    */
    static bool decode(const QMimeData *dataIn, QicsDataModel &dm);

    /*!
    * Do a decode of attrs only for the intra application types rowlist and
    * columnlist
    * \since 2.2
    */
    static bool decode(const QMimeData *dataIn, const QicsGridInfo &gi, QicsICell &cell);

private:
    QString exportCell(const QicsCell &cell);
};

#ifndef QICS_MIME_CELLDATA
#define QICS_MIME_CELLDATA	"application/vnd.ics.celldata"
#endif

#ifndef QICS_MIME_CELLATTR
#define QICS_MIME_CELLATTR	"application/vnd.ics.cellattr"
#endif

#ifndef QICS_MIME_CELLSIZE
#define QICS_MIME_CELLSIZE	"application/vnd.ics.cellsize"
#endif

#ifndef QICS_MIME_CELLSPAN
#define QICS_MIME_CELLSPAN	"application/vnd.ics.cellspan"
#endif

#ifndef QICS_MIME_COLUMNHEADERDATA
#define QICS_MIME_COLUMNHEADERDATA	"application/vnd.ics.columnheaderdata"
#endif

#ifndef QICS_MIME_ROWHEADERDATA
#define QICS_MIME_ROWHEADERDATA	"application/vnd.ics.rowheaderdata"
#endif

#endif //QICSTABLEREGIONDRAG_H


