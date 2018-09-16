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

#ifndef QICSEXPANDABLESTATICROWDATA_H
#define QICSEXPANDABLESTATICROWDATA_H

#include "QicsSpecialRowData.h"

/*!
* \class QicsExpandableStaticRowData QicsTreeTable.h
* \brief Provides handling of child rows (i.e. tree subnodes).
*/

class QicsGroupInfo;

class QicsExpandableStaticRowData : public QicsSpecialRowData
{
    Q_OBJECT
public:
    /*!
    *	Constructor, \a caption is the text being displayed in the row.
    */
    QicsExpandableStaticRowData(QicsTreeTable *table, QicsGroupInfo *info, QObject *parent = 0);
    /*!
    *	Returns  title
    */
    virtual const QicsDataItem* title() { return &m_title; }

    /*!
    *	Returns list of indexes of children rows.
    */
    inline const QVector<int>&	children() const { return m_list; }

    /*!
    *	Adds row with \a index to the internal list of children.
    */
    void addChild(int index);

    /*!
    *	Sets level of the tree node to \a i. 0 is the root level, and so on.
    */
    inline void setLevel(int i) {m_level = i;}

    /*!
    *	Returns level of the tree node.
    */
    inline int level() const { return m_level; }

    /*!
    *	Returns open state of the tree node.
    *	\since 2.4
    */
    inline bool isOpen() const { return m_open; }

    /*!
    *	Returns assotiated group info object.
    *	\since 2.4
    */
    inline QicsGroupInfo *groupInfo() const { return m_groupInfo; }

    /*!
    *	Expands this and all the child items if \a open = \a true,
    *  contracts if \a false. Remembers open state.
    * If \a repaint is true then repaints table automatically (you should pass false
    * only if you're handling expand operations manually).
    *  Calls doExpandTree().
    */
    void expandTree(bool open, bool repaint = true);

    /*!
    *	Expands this and all the child items if \a open = \a true,
    *  contracts if \a false. Does not remember open state.
    *  Unlike expandTree(bool), takes into account child's open
    *  state i?? does not expand children which are closed.
    * If \a repaint is true then repaints table automatically (you should pass false
    * only if you're handling expand operations manually).
    *  Calls doExpand().
    */
    void expand(bool open, bool repaint = true);

    /*!
    *  Toggles expand state.
    * If \a repaint is true then repaints table automatically (you should pass false
    * only if you're handling expand operations manually).
    * Calls expandTree().
    */
    void switchTree(bool repaint = true);

protected:
    /*!
    *	Performs initialization of internal data for connected row object \a row.
    *   Called internally.
    */
    virtual void init(QicsRow &row);

    /*!
    *	Performs uninitialization of internal data for connected row object \a row.
    *   Called internally.
    */
    virtual void dispose(QicsRow &row);

    /*!
    *	Event handler for mouse double click action. \a row and \a col are visual
    *  coordinates of row and column of the special row, \a button is mouse button,
    *  \a p is global coordinate of mouse cursor.
    */
    virtual void handleDoubleClick(int row,int col,int button,const QPoint &p);

    /*!
    *	Event handler for mouse single click action. \a row and \a col are visual
    *  coordinates of row and column of the special row, \a button is mouse button,
    *  \a p is global coordinate of mouse cursor.
    */
    virtual void handleClick(int row,int col,int button,const QPoint &p);

    /*!
    *	Event handler for keyboard key press action. \a row and \a col are visual
    *  coordinates of row and column of the special row, \a ke is key event.
    *  Called internally. Used for subclassing.
    */
    virtual bool handleKeyPress(int row,int col,QKeyEvent *ke);

    void doExpand(bool open, bool tree = false, bool repaint = true);

    /*!
    *	Returns \a true, if row \a index is hidden, otherwise returns \a false.
    */
    bool isRowHidden(int index);

    QVector<int> m_list;
    bool m_open;
    int m_level;

    QicsExpandableStaticRowData *m_root;

    QicsGroupInfo *m_groupInfo;

    QicsDataString	m_title;

private:
    friend class QicsTreeTable;
};

#endif //QICSEXPANDABLESTATICROWDATA_H


