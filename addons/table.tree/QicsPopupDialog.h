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

#ifndef QICSPOPUPDIALOG_H
#define QICSPOPUPDIALOG_H

#include <QFrame>
#include <QDialogButtonBox>

#include <QicsNamespace.h>

////////////////////////////////////////////////////
/*!
* \class QicsPopupDialog QicsPopupDialog.h
* \brief Common class for popup dialog with status bar and buttons.
*
* \image html popupdialog.png
*
* QicsPopupDialog provides facilities to show and handle a popup dialog
* with buttons and size grip inside the status bar. It is designed to be
* as much independent from current system window manager as possible.
*
* It is the base class for building more sophisticated popup dialogs.
* It can be customized by:
* - providing in the constructor a set of buttons to be shown at the bottom;
* - setting the main widget by calling setWidget();
* - modifying current status bar received with statusBar();
* - enabling/disabling sizing by setSizeGripEnabled();
* - changing the frame style & shape (see QFrame documentation).
*
* When a button is clicked, QicsPopupDialog emits clicked() signal with button
* id as the parameter. If the button has AcceptRole defined, signal accepted()
* is also emitted, and accepted() virtual method called. If the button defines
* RejectRole, then rejected() signal emitted. Thus, it is possible to customize
* response on the button click in one of these ways.
*
* Methods show() and exec() popup the dialog in the specified position.
* exec() additionally to show() will wait until the dialog is closed, and will
* return the id of the last button clicked (the same id can be obtained by lastButton()).
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QStatusBar;
class QVBoxLayout;
class QEventLoop;

class QICS_EXPORT QicsPopupDialog : public QFrame
{
    Q_OBJECT
public:
    /*!
    * Constructor.
    * \param parent the parent widget
    * \param buttons a set of buttons displayed in the bottom of the dialog
    */
    QicsPopupDialog(QWidget *parent = 0, QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::NoButton);
    virtual ~QicsPopupDialog();

    /*!
    * Returns true if size grip enabled.
    * \sa setSizeGripEnabled().
    */
    inline bool isSizeGripEnabled () const {return m_gripOn;}

    /*!
    * Makes size grip enabled or disabled according to \a on.
    * \sa isSizeGripEnabled().
    */
    void setSizeGripEnabled ( bool on );

    /*!
    * Returns QStatusBar widget.
    */
    inline QStatusBar* statusBar() const {return m_status;}

    /*!
    * Returns QDialogButtonBox widget.
    */
    inline QDialogButtonBox* buttons() const {return m_buttons;}

    /*!
    * Returns id of the last button clicked.
    * \sa exec()
    */
    inline QDialogButtonBox::StandardButton lastButton() const {return m_lastButton;}

    /*!
    * Installs the main widget to \a mainWidget.
    * Previously installed widget is being destroyed is \a deleteOld is true.
    * \sa widget()
    */
    void setWidget(QWidget *mainWidget, bool deleteOld = false);

    /*!
    * Returns currently installed widget.
    * \sa setWidget()
    */
    inline QWidget* widget() const {return m_widget;}

    /*!
    * Shows popup at \a pos position and exists immediately.
    * Popup will remain visible until clicked anywhere outside, or close button pressed.
    * \sa exec()
    */
    virtual void show(const QPoint &pos);
    /*!
    * Shows popup at \a pos position.
    * Same as show(), but waits till popup is closed.
    * Returned value is the id of latest button pressed (see clicked()).
    * If there was no button clicked, QDialogButtonBox::NoButton will be returned.
    * \sa show()
    */
    virtual int exec(const QPoint &pos);

    virtual void setVisible(bool visible);

signals:
    /*!
    * Emitted when a button with QDialogButtonBox::AcceptRole role was clicked.
    * \sa accept()
    */
    void accepted();
    /*!
    * Emitted when a button with QDialogButtonBox::RejectRole role was clicked.
    */
    void rejected();
    /*!
    * Emitted when a \a button was clicked.
    */
    void clicked(QDialogButtonBox::StandardButton button);

protected slots:
    void onClicked(QAbstractButton * button);
    void onAccepted();

protected:
    /*!
    * Called when a button with QDialogButtonBox::AcceptRole role was clicked.
    * Method does nothing by default and may be reimplemented.
    */
    virtual void accept() {}

    bool m_gripOn;
    QEventLoop *m_eventLoop;
    QVBoxLayout *m_layout;
    QStatusBar *m_status;
    QDialogButtonBox *m_buttons;
    QDialogButtonBox::StandardButton m_lastButton;
    QWidget *m_widget;
#ifndef Q_OS_WIN32
    class QicsSizeGrip *m_sizeGrip;
#endif
};

#endif // QICSPOPUPDIALOG_H


