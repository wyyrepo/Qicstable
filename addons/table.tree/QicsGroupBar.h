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

#ifndef QICSGROUPBAR_H
#define QICSGROUPBAR_H

#include <QicsSectionBar.h>
#include <QicsCheckPopup.h>
#include <QicsPopupDialog.h>

/*!
* \class QicsGroupBar QicsGroupBar.h
* \brief Class for displaying grouped columns.
*
* \image html groupbar.png
*
*  Used to represent section bar for displaying grouped columns. Close button
*  at the right of the bar removes all the grouping.
*  Every section has additional combo button which provides drop-down list with
*  unique keys to filter off several groups. Section which has filter applied
*  is marked with asterisk to the right of its caption.
*
*  Right-click on the bar shows popup dialog QicsGroupBarFilterDialog inherited
*  from QicsCheckPopup to choose columns which will be grouped.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsTreeTable;
class QicsGroupBarHeader;
class QGridLayout;
class QLabel;
class QComboBox;
class QTextEdit;

class QICS_EXPORT QicsGroupBar : public QWidget
{
    Q_OBJECT
public:
    /*!
    *	Constructor, creates group bar for \a table and
    *  places it on widget \a parent.
    */
    QicsGroupBar(QicsTreeTable *table, QWidget *parent);

protected:
    virtual void dragEnterEvent ( QDragEnterEvent * event );
    virtual void dropEvent ( QDropEvent * event );

    QicsGroupBarHeader	*m_bar;
    QPushButton			*btnClose;
};


/*!
* \class QicsGroupBarFilterDialog QicsGroupBar.h
* \brief Popup dialog with checkboxes to choose grouped columns.
*
*  %QicsGroupBarFilterDialog inherits QicsCheckPopup and
*  provides facilities to show and handle a popup dialog
*  with list box filled with checkboxes. Also, there is a check box
*  (at the top of the list, "All") which can be used for quick
*  enabling/disabling all the checkboxes in the list.
*/

class QICS_EXPORT QicsGroupBarFilterDialog : public QicsPopupDialog
{
public:
    /*!
    *	Constructor, creates %QicsGroupBarFilterDialog popup dialog for
    *  the QicsTreeTable \a table.
    */
    QicsGroupBarFilterDialog(QicsTreeTable *table, QWidget *parent = 0);

    /*!
    *	Shows popup dialog for \a group at position \a pos.
    */
    virtual void show(QPoint pos, int group);

protected:
    /*!
    *	Called when Apply button is clicked. Filters the table
    *  corresponding to checked boxed.
    */
    virtual void accept();

protected:
    QicsTreeTable *m_table;
    int m_group;
    QicsCheckPopup *m_checks;
    QTabWidget *twFilter;
    QWidget *tab_2;
    QGridLayout *gridLayout;
    QCheckBox *cbRegexpCase;
    QCheckBox *cbRegexpMinimal;
    QLabel *label;
    QComboBox *cbRegexpSyntax;
    QTextEdit *teRegexpPattern;
    QWidget *tab_3;
};


/*!
* \class QicsGroupBarHeader QicsGroupBar.h
* \brief Helper class for QicsGroupBar.
*/

class QICS_EXPORT QicsGroupBarHeader : public QicsSectionBar
{
    Q_OBJECT
public:
    /*!
    *	Constructor, parameters are \a table and parent widget \a parent.
    */
    QicsGroupBarHeader(QicsTreeTable *table, QWidget *parent);

    /*!
    *  Destructor for the class.
    */
    virtual ~QicsGroupBarHeader();

protected:
    virtual QSize sectionSizeFromContents(int logicalIndex) const;

    /*!
    *  Draws the section specified by the given \a logicalIndex,
    *  using the given \a painter and \a rect with QStyleOptionHeader
    *  options \a opt and suboptions \a subopt.
    */
    virtual void drawSection(QPainter *painter, const QRect &rect, int logicalIndex,
        QStyleOptionHeader opt, QStyleOptionHeader subopt) const;

    virtual void mousePressEvent ( QMouseEvent * event );

    virtual void leaveEvent ( QEvent * event );

    /*!
    *  Called with the given \a event, \a rect and \a dx from the mouseMoveEvent().
    *  Returns \a true if needs redraw.
    */
    virtual bool extendMouseMove(QMouseEvent * event, const QRect &rect, int dx);

private slots:
    /*!
    *  Called when the given \a column has been added.
    */
    virtual void onAdded(int column);

    /*!
    *  Called when the given \a column has been removed.
    */
    virtual void onRemoved(int column);

    /*!
    *  This function regroups the table.
    */
    void doRegroup(int,int,int);

    /*!
    *  Called when the filter for the given \a column gas been changed.
    */
    void onFilterChanged(int column);

    /*!
    *  Called when the section with the given index \a idx has been closed.
    */
    virtual void onSectionClosed(int idx);

    /*!
    *  This slot is connected to a QicsCheckPopup dialog accepted() signal.
    */
    virtual void dialogAccepted();

private:
    Q_DECLARE_PRIVATE(QHeaderView)

    void doWorkAround();

    QPoint m_dropPos;
    bool m_dropActive;
    bool m_dropPressed;
    QicsGroupBarFilterDialog *m_filterDialog;
};


#endif //QICSGROUPBAR_H


