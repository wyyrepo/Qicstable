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

#ifndef QICSNAVIGATOR_H
#define QICSNAVIGATOR_H

#include <QToolButton>
#include <QFrame>

#include <QicsNamespace.h>

class QicsNavigatorFrame;

///////////////////////////////////////////////////////////////////////////

/*! \class QicsNavigator QicsNavigator.h
* \nosubgrouping
* \brief Helper widget class that allows navigation through grid.
* QicsNavigator is helper widget class that allows navigation through grid.
* It is the subclass of QToolButton that placed in bottom rigth corner of table.
* Mouse press on it shows navigator frame where user can perform navigation.

* \image html nav.png "Navigation through table grid using QicsNavigator"

* QicsNavigator can be used also as standalone widget inside your own
* application. You should install two signal handlers (for \b pressed and \b changed signals) in order to setup
* navigator and react to user's actions. See #changed and #navigate members.

* \b Example

Creating navigator widget and installing signal handlers for user interaction.

* \code
myNavButton = new QicsNavigator(this);			// create navigator object
myNavButton->setFixedSize(18,18);				// set size of the button
myNavButton->resizeWindow(100,100);				// set size of the windows

connect(myNavButton, SIGNAL(pressed()), this, SLOT(showNavigator()));
connect(myNavButton, SIGNAL(changed(const QRect&)), this, SLOT(navigate(const QRect&)));
* \endcode

Handling \b #pressed signal should bring the navigation window up.

* \code
void QicsTable::showNavigator()
{
    // create a rect which fits currently visible part of the table
    QRect view(leftColumn(), topRow(), visibleColumns(), visibleRows());

    // show navigator window scaled to total table size (QicsTable::currentViewport()) with mapped visible region (view)
    myNavButton->navigate(currentViewport(), view);
}
* \endcode

Handling \b #changed signal should make visible selected part of the table.

* \code
void QicsTable::navigate(const QRect &view)
{
    // view is chosen by the user from navigator window - we should map it to the table
    // in order to make this region visible at the screen
    setLeftColumn(view.left());
    setTopRow(view.top());
}
* \endcode

\sa QicsTable::setNavigatorAllowed(), QicsTable::isNavigatorAllowed()


*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsNavigator : public QToolButton
{
    Q_OBJECT
public:
    /*! Constructor
    */
    QicsNavigator(QWidget *parent);
    /*! Destructor
    */
    ~QicsNavigator();

    /*! Returns navigator window. You can change its opacity, frame shape and other common QFrame's properties.
    */
    inline QicsNavigatorFrame *window() const { return m_nav; }

signals:
    // This signal is emitted when current region in the navigator window is changed by the user (i.e. via mouse).
    //
    // In your own application, you should handle this signal in order to setup your widget to make the \a view region visible at the screen.
    // @param view Current region displayed in the navigator.
    void changed(const QRect &view);

public slots:
    // Setup navigator to treat \a total rect as the whole area and \a view rect as the current region (part which is visible on the screen).
    //
    // Normally, it should be called \b every \b time when the navigator is about to be shown, to ensure that its window is updated
    // accordingly to the last area/viewport changes.
    // In your own application, you should create a slot which will perform this action and connect it with navigator's \b pressed signal.
    // @param total Widget's total area (in logical coordinates).
    // @param view Current region displayed in the navigator (in logical coordinates).
    void navigate(const QRect &total, const QRect &view);

    // Resizes navigator window to the given size.
    // @param w New width in pixels.
    // @param h New height in pixels.
    void resizeWindow(int w, int h);

    // Resizes navigator window to the given size (same as above).
    // @param size New size in pixels.
    void resizeWindow(const QSize &size);

protected:
    QicsNavigatorFrame *m_nav;
};

///////////////////////////////////////////////////////////////////////////

/*! \class QicsNavigatorFrame
* \nosubgrouping
* \brief Internal class that represents navigator's window.

QicsNavigatorFrame is inherited from QFrame.
*/

class QICS_EXPORT QicsNavigatorFrame : public QFrame
{
    Q_OBJECT
public:
    QicsNavigatorFrame();
    ~QicsNavigatorFrame();

    void setup(const QRect &total, const QRect &view);

signals:
    void changed(const QRect &view);

protected:
    virtual void paintEvent ( QPaintEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void closeEvent ( QCloseEvent * );

    QRect m_total;
    QRect m_view;
    QRect m_label;
};

#endif //QICSNAVIGATOR_H


