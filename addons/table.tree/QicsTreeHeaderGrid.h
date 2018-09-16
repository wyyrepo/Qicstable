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

#ifndef QICSTREEHEADERGRID_H
#define QICSTREEHEADERGRID_H

#include <QicsHeaderGrid.h>

/*!
* \class QicsTreeHeaderGrid QicsTreeTable.h
* \brief Internal class for handling events from keyboard.
*
*  QicsTreeHeaderGrid handles keyboard events and passes them to special
*  rows to peform tree expanding and collapsing.
*  Used internally by QicsTreeTable.
*/

class QICS_EXPORT QicsTreeHeaderGrid : public QicsHeaderGrid
{
    Q_OBJECT
public:
    /*!
    * Constructor for the class.  \a w is the grid widget's parent.
    * \a info is used for style and dimension information, as well as
    * the associated data model.
    * Optional arguments \a top_row and \a left_column specify the
    * starting location for the top left cell of the grid.
    */
    QicsTreeHeaderGrid(QWidget *w, QicsGridInfo &info,
        Qics::QicsHeaderType type);

    /*!
    * Foundry method to create new instances of QicsTableGrid.  Subclasses
    * of QicsTreeTableGrid should implement a similar method.  Methods
    * of this type can be passed to a QicsTreeTable constructor in order
    * to create custom grids.
    */
    static QicsHeaderGrid *createGrid(QWidget *w, QicsGridInfo &info,
        Qics::QicsHeaderType type)
    {
        return new QicsTreeHeaderGrid(w, info, type);
    }

    static QicsHeaderGrid *createGridWithKey(QWidget *w, QicsGridInfo &info,
        Qics::QicsHeaderType type, void * /*key*/ = 0)
    {
        return new QicsTreeHeaderGrid(w, info, type);
    }

protected:
    virtual void keyPressEvent(QKeyEvent *ke);

    virtual void emitMouseSignals(const QicsICell &cell, QMouseEvent *event);
};

#endif //QICSTREEHEADERGRID_H


