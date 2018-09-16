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

#ifndef QICSMAINGRID_H
#define QICSMAINGRID_H

#include "QicsGridCommon.h"


class QicsTable;

/////////////////////////////////////////////////////////////////////////////////

/*!
* \class QicsMainGrid.h QicsMainGrid.h
*  \nosubgrouping
* \brief Flyweight class representing the main grid of the table
*
* QicsMainGrid is the programmer's interface to the main grid that
* appears in the QicsTable widget.
* This class is used to query or set attributes of all the cells in the
* main grid area of the table.
*
* QicsMainGrid is a flyweight class representing the main grid in the table.
* These objects are created directly (via the public constructor)
* or indirectly (via QicsTable::mainGrid() or QicsTable::mainGridRef())
* when needed.
*
* Persistent objects, created by the public constructor or by
* QicsTable::mainGrid() are valid until they are explicitly destroyed,
* or until the table itself is destroyed.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsMainGrid: public QicsGridCommon
{
    Q_OBJECT
public:
    /*!
    * Constructor for the class.
    * \param table the cell's table
    */
    QicsMainGrid(QicsTable *table);

    /*!
    * \internal
    * Constructor for the main grid.  Meant only for use by internal objects.
    */
    QicsMainGrid(QicsGridInfo *info, QObject *parent = 0, bool forward_signals = false);

    virtual ~QicsMainGrid();

#ifdef Q_DISABLE_COPY
private:
    QicsMainGrid(const QicsMainGrid& mg);
    QicsMainGrid &operator=(const QicsMainGrid& mg);
#endif
};

#endif //QICSMAINGRID_H


