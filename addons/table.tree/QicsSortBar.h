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

#ifndef QICSSORTBAR_H
#define QICSSORTBAR_H

#include <QPushButton>
#include "QicsSectionBar.h"
#include "QicsNamespace.h"

class QicsTreeTable;
class QicsSortBarHeader;
class QicsSortArrowButton;

/*!
* \class QicsSortBar QicsSortBar.h
* \brief Class for displaying sorted columns.
*
* \image html sortbar.png
*
*  Used to represent section bar for displaying sorted columns.
*  Close button at the right of the bar removes all the sorting,
*  arrow button changes sort direction (ascending/descending).
*
*  Right-click on the bar shows popup dialog based on QicsCheckPopup
*  to choose columns which will be grouped.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsSortBar : public QWidget
{
    Q_OBJECT
public:
    /*!
    *  Constructor. Takes \a table and \a parent as arguments.
    */
    QicsSortBar(QicsTreeTable *table, QWidget *parent);

protected slots:
    /*!
    *  This slot is connected to the table's rowsSorted() signal.
    *  Changes the state of the sorting order button.
    */
    void onSortingChanged();

    /*!
    *  Changes sort order for the table for the opposite, i.e. if
    *  the sort order was Qics::Ascending then calling this slot will
    *  change the sort order to Qics::Descending.
    */
    void changeOrder();

protected:

    virtual void dragEnterEvent ( QDragEnterEvent * event );
    virtual void dropEvent ( QDropEvent * event );

    QicsSortBarHeader *m_bar;
    QPushButton *btnClose;
    QicsSortArrowButton *btnOrder;
};


/*!
* \class QicsSortBarHeader QicsSortBar.h
* \brief Helper class for QicsSortBar.
*/

class QicsSortBarHeader : public QicsSectionBar
{
    Q_OBJECT
public:
    /*!
    *	Constructor, parameters are \a table and parent widget \a parent.
    */
    QicsSortBarHeader(QicsTreeTable *table, QWidget *parent);

    /*!
    *  Destructor for the class.
    */
    ~QicsSortBarHeader();

protected slots:
    /*!
    *  Called when the section with the given index \a num has been closed.
    */
    virtual void onSectionClosed(int num);

    /*!
    *  Called when the sorting order has been changed.
    */
    void onSortingChanged();

    /*!
    *  Resorts rows in the table.
    */
    void doResort(int,int,int);

    /*!
    *  This slot is connected to a QicsCheckPopup dialog accepted() signal.
    */
    virtual void dialogAccepted();
};


/*!
* \class QicsSortArrowButton QicsSortBar.h
* \brief Provides arrow button to change sort direction.
*/

class QicsSortArrowButton : public QPushButton
{
    Q_OBJECT
public:
    /*!
    *	Constructor, parameter is parent widget \a parent.
    */
    QicsSortArrowButton(QWidget *parent);

    /*!
    *	Sets sorting state to \a state.
    */
    void setState(Qics::QicsSortOrder state);

    /*!
    *	Returns current sorting state.
    */
    inline Qics::QicsSortOrder state() const { return m_state;}

protected:
    /*!
    *	Subclassed paintEvent() handler, paints the arrow up or arrow down
    *  according to the current sorting state.
    */
    virtual void paintEvent ( QPaintEvent * event );

    Qics::QicsSortOrder	m_state;
};


#endif //QICSSORTBAR_H


