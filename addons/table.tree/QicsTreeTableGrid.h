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

#ifndef QICSTREETABLEGRID_H
#define QICSTREETABLEGRID_H

#include <QicsTableGrid.h>

/*!
* \class QicsTreeTableGrid QicsTreeTable.h
* \brief Internal class for handling events from keyboard.
*
*  QicsTreeTableGrid handles keyboard events and passes them to special
*  rows to peform tree expanding and collapsing.
*  Used internally by QicsTreeTable.
*/

class QICS_EXPORT QicsTreeTableGrid : public QicsTableGrid
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
    QicsTreeTableGrid(QWidget *w, QicsGridInfo &info,
            int top_row = 0, int left_column = 0);

    /*!
    * Foundry method to create new instances of QicsTableGrid.  Subclasses
    * of QicsTreeTableGrid should implement a similar method.  Methods
    * of this type can be passed to a QicsTreeTable constructor in order
    * to create custom grids.
    */
    static QicsTableGrid *createGrid(QWidget *w, QicsGridInfo &info,
        int top_row = 0, int left_column = 0)
    {
        return new QicsTreeTableGrid(w, info, top_row, left_column);
    }

    static QicsTableGrid *createGridWithKey(QWidget *w, QicsGridInfo &info,
        int top_row = 0, int left_column = 0, void * /*key*/ = 0)
    {
        return new QicsTreeTableGrid(w, info, top_row, left_column);
    }

protected:
    virtual void keyPressEvent  (QKeyEvent *ke) ;
};

#endif //QICSTREETABLEGRID_H


