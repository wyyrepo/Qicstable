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

#ifndef QICSSPECIALROWDATA_H
#define QICSSPECIALROWDATA_H

#include <QObject>

#include <QicsDataModelDefault.h>

/*!
* \class QicsSpecialRowData QicsTreeTable.h
* \brief Base class for handling special row.
*
*  %QicsSpecialRowData provides facilities for handling special row.
*  Most its methods are called internally from QicsTreeTable object.
*  This class is very similar to QicsTable's cell displayers, but is used
*  for single rows which do not exist in actual model, but only in view.
*
*  Special rows serve for special visual representation and interaction
*  with user.  Each special row is stored as the actual row in virtual
*  data model based on QicsViewTreeDataModel class. It is drawn as
*  single spanned row.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QKeyEvent;
class QicsTreeTable;
class QicsRow;


class QicsSpecialRowData : public QObject
{
    Q_OBJECT
public:
    /*!
    *	Constructor, requires pointer to QicsTreeTable \a table which will
    *  show appropriate special row in the view.
    */
    QicsSpecialRowData(QicsTreeTable *table, QObject *parent = 0);
    virtual ~QicsSpecialRowData();

    /*!
    *	Returns pointer to actual row object in corresponding QicsViewTreeDataModel.
    */
    QicsRow *rowPointer();

    /*!
    *	Returns data item for row \a row and column \a column. Used for subclassing.
    */
    virtual const QicsDataItem* item(int row, int col);

    /*!
    *	Sets data item \a item for column \a col.
    */
    virtual void setItem(int row, int col, const QicsDataItem &item);

    inline void setVisible(bool b) { m_visible = b; }
    inline bool isVisible() const { return m_visible; }

protected:
    /*!
    *	Performs connection to actual row with index \a rowIndex and
    *  initialization of internal data. Called internally. Calls init(QicsRow &row).
    */
    virtual void init(int rowIndex);

    /*!
    *	Performs initialization of internal data for connected row object \a row.
    *  Called internally. Used for subclassing.
    */
    virtual void init(QicsRow &/*row*/) {};

    /*!
    *	Performs uninitialization of internal data for connected row object \a row.
    *  Called internally. Used for subclassing.
    */
    virtual void dispose(QicsRow &/*row*/);

    /*!
    *	Event handler for mouse single click action. \a row and \a col are visual
    *  coordinates of row and column of the special row, \a button is mouse button,
    *  \a p is global coordinate of mouse cursor.
    *  Called internally. Used for subclassing.
    */
    virtual void handleClick(int /*row*/,int /*col*/,int /*button*/,const QPoint &/*p*/) {};

    /*!
    *	Event handler for mouse double click action. \a row and \a col are visual
    *  coordinates of row and column of the special row, \a button is mouse button,
    *  \a p is global coordinate of mouse cursor.
    *  Called internally. Used for subclassing.
    */
    virtual void handleDoubleClick(int /*row*/,int /*col*/,int /*button*/,const QPoint &/*p*/) {};

    /*!
    *	Event handler for keyboard key press action. \a row and \a col are visual
    *  coordinates of row and column of the special row, \a ke is key event.
    *  Called internally. Used for subclassing.
    */
    virtual bool handleKeyPress(int /*row*/,int /*col*/,QKeyEvent * /*ke*/) { return false; };

    QicsTreeTable *m_table;
    bool m_visible;

    QicsDataItemPV m_items;

private:
    friend class QicsTreeTable;
    friend class QicsViewTreeDataModel;
    friend class QicsTreeTableGrid;
    friend class QicsTreeHeaderGrid;
};

#endif //QICSSPECIALROWDATA_H


