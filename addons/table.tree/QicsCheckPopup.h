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

#ifndef QICSCHECKPOPUP_H
#define QICSCHECKPOPUP_H

#include <QicsNamespace.h>
#include <QicsPopupDialog.h>

////////////////////////////////////////////////////
/*!
* \class QicsCheckPopup QicsCheckPopup.h
* \brief Common class for the list with checkboxes.
*
*  QicsCheckPopup provides facilities to show and handle
*  a list box filled with checkboxes. Also, there is a check box
*  (at the top of the list, "All") which can be used for quick
*  enabling/disabling all the checkboxes in the list.
*
*	QicsCheckPopupDialog based on this class provides popup dialog with check box.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QListWidget;
class QListWidgetItem;
class QCheckBox;


class QICS_EXPORT QicsCheckPopup : public QWidget
{
    Q_OBJECT
public:
    /*!
    *	Constructor.
    *  If \a moveItems is \a true, items in the list could be rearranged with the mouse.
    *  If \a sortItems is \a true, items in the list will be sorted in ascending order.
    */
    QicsCheckPopup(bool moveItems = false, bool sortItems = true, QWidget *parent = 0);

    /*!
    *	Destructor for the class.
    */
    ~QicsCheckPopup();

    /*!
    *	Returns QListWidget object which contains all the checkbox items.
    */
    inline QListWidget*	list() const { return m_list; }

public slots:
    /*!
    *	Switches check state of all the items to \a state.
    */
    void			checkItems(bool state);
    void			updateCheckBox(QListWidgetItem * item = 0);

protected:
    QVBoxLayout     *m_layout;
    QListWidget		*m_list;
    QPushButton		*m_pbOk, *m_pbCancel;
    QCheckBox		*m_cbAll;
    bool			m_moveItems;
};

////////////////////////////////////////////////////
/*!
* \class QicsCheckPopupDialog QicsCheckPopupDialog.h
* \brief Common class for popup dialog with checkboxes.
*
* \image html checkpopup.png
*
*  QicsCheckPopupDialog provides facilities to show and handle a popup dialog
*  with QicsCheckPopup. It is based on QicsPopupDialog class and provides
*	two buttons, "Ok" and "Cancel".
*
*	This class is mostly used as the base class for more sophisticated dialogs.
*	Virtual method accept() can be subclassed in order to customize dialog's
*	behavior when "Ok" button was clicked.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////
class QICS_EXPORT QicsCheckPopupDialog : public QicsPopupDialog
{
public:
    /*!
    *	Constructor, if \a moveItems is \a true, items in the list
    *  could be rearranged with the mouse.
    */
    QicsCheckPopupDialog(bool moveItems = false, bool sortItems = true, QWidget *parent = 0)
        : QicsPopupDialog(parent, QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
    {
        m_checks = new QicsCheckPopup(moveItems, sortItems);
        setWidget(m_checks);
    }

    inline QListWidget*	list() const { return m_checks->list(); }

    virtual void accept() { close(); }

protected:
    QicsCheckPopup *m_checks;
};

#endif //QICSCHECKPOPUP_H


