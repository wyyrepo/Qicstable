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

#ifndef QICSWIDGETCELLDISPLAY_H
#define QICSWIDGETCELLDISPLAY_H

#include <QObject>
#include "QicsCellDisplay.h"

class QWidget;
class QPainter;
class QicsDataItem;
class QObjectCleanupHandler;


/*!
* \class QicsWidgetCellDisplay QicsWidgetCellDisplay.h
* \brief A cell displayer which uses a single widget for display
*
* QicsWidgetCellDisplay displays a single widget in a cell.
*
* \b Please \b note: it is possible to use one displayer for one cell only. You should not
* set the same displayer for several cells, or for a column/row. This is because
* after the displayer is installed for a cell, ownership of the widget transferred
* to the table grid. If you need to install another widget of the same class, please create
* new widget and new displayer.
*
* Widget to display should be passed to the constructor.
* There's an example how to integrate QDial into the cell at (0,0).
*
* \code
* QicsTable *myTable;
* // myTable initialized here...
* QDial *myDial = new QDial();
* QicsWidgetCellDisplay *myDialDisplay = new QicsWidgetCellDisplay(myDial, myTable);
* myTable->cellRef(0,0).setDisplayer(myDialDisplay);
* \endcode
*
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsWidgetCellDisplay: public QObject, public QicsCellDisplay
{
    Q_OBJECT
public:
    /*!
    * Constructor for use by programmers.
    */
    QicsWidgetCellDisplay(QWidget *displayWidget, QObject *parent = 0);

    /*!
    * Destructor is virtual because this class may be subclassed.
    */
    virtual ~QicsWidgetCellDisplay();

    /*!
    * Displays the widget in the specified location.
    */
    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm, QRect &rect, QPainter *painter);

    /*!
    * This method is a no-op in this class, because the widget is always
    * displayed, therefore there is no entry widget to update.
    */
    virtual void startEdit(QicsScreenGrid *grid, int row, int col,
        const QicsDataItem *itm);

    /*!
    * This method is a no-op in this class, because the widget is always
    * displayed, therefore there is no entry widget to place.
    */
    virtual void moveEdit(QicsScreenGrid *grid, int row, int col, const QRect &rect);

    /*!
    * This method is a no-op in this class, but may be reimplmented in subclasses.
    */
    virtual void endEdit(QicsScreenGrid *grid, int row, int col);

    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    inline virtual bool editWhenCurrent() const { return false; }

    virtual bool eventFilter(QObject *watched, QEvent *event);

    inline virtual bool needsVisibilityNotification() const { return true; }

    virtual QWidget* widget() {return m_widget;}

    virtual bool isEmpty(QicsGridInfo *grid, int row, int col,
        const QicsDataItem *itm) const;

    virtual void aboutToClear(QicsGridInfo *info, int row, int col);

protected:
    void setWidget(QWidget *widget);

    /*!
    * \internal
    * the widget we are displaying
    */
    QPointer<QWidget> m_widget;
    QObjectCleanupHandler *m_objectCleanupHandler;
};

#endif //QICSWIDGETCELLDISPLAY_H


