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

#ifndef QICSSECTIONBAR_H
#define QICSSECTIONBAR_H

#include <QHeaderView>
#include <QStandardItemModel>

#include <QicsCheckPopup.h>

/*!
* \class QicsSectionBar QicsSectionBar.h
* \brief Basic class for headers with sections.
*
*  It is derived from QHeaderView and additionally to its features
*  provides close button on every section. Sections could be dragged
*  with mouse to another place. Also, it provides common dialog
*  based on QicsCheckPopup for popup checkbox list.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsTreeTable;

class QICS_EXPORT QicsSectionBar : public QHeaderView
{
    Q_OBJECT
public:
    /*!
    *  Constructor. Takes \a table and \a parent as arguments.
    */
    QicsSectionBar(QicsTreeTable *table, QWidget *parent);

    /*!
    *  Destructor for the class.
    */
    virtual ~QicsSectionBar();

    /*!
    *  Returns QicsTreeTable assigned to the bar.
    */
    inline QicsTreeTable *table() const { return m_table; }

protected:
    virtual QSize sectionSizeFromContents(int logicalIndex) const;
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void leaveEvent ( QEvent * event );

    /*!
    *  Called with the given \a event, \a rect and \a dx from the mouseMoveEvent().
    *  Returns \a true if needs redraw.
    */
    virtual bool extendMouseMove(QMouseEvent * /*event*/, const QRect &/*rect*/, int /*dx*/)
    { return false; }

    /*!
    *  Draws the section specified by the given \a logicalIndex,
    *  using the given \a painter and \a rect with QStyleOptionHeader
    *  options \a opt and suboptions \a subopt.
    */
    virtual void drawSection(QPainter *painter, const QRect &rect, int logicalIndex,
        QStyleOptionHeader opt, QStyleOptionHeader subopt) const;

    /*!
    *  Receives \a event when the context menu for a widget is about to be
    *  activated. Shows the QicsCheckPopup dialog.
    */
    virtual void contextMenuEvent ( QContextMenuEvent * event );

protected slots:
    /*!
    *  Called when a section with index \a id is closed by close button.
    *  Used for subclassing.
    */
    virtual void onSectionClosed(int id) = 0;

    /*!
    *  Called when a section with index \a id is added to the bar.
    */
    void onAdded(int id);

    /*!
    *  Called when a section with index \a id is removed from the bar.
    */
    void onRemoved(int id);

    /*!
    *  Called when popup dialog is accepted. Used for subclassing.
    */
    virtual void dialogAccepted() = 0;

protected:
    Q_DECLARE_PRIVATE(QHeaderView)

    QicsTreeTable *m_table;
    QStandardItemModel m_model;

    bool in_progress;
    int m_sect;
    bool m_closeActive;

    QicsCheckPopupDialog *m_dialog;
};

#endif //QICSSECTIONBAR_H


