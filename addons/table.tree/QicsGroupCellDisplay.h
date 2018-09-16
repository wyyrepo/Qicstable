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

#ifndef QICSGROUPCELLDISPLAY_H
#define QICSGROUPCELLDISPLAY_H

#include <QicsTextCellDisplay.h>
#include <QicsTreeTable.h>

/*!
* \class QicsGroupCellDisplay QicsGroupCellDisplay.h
* \brief A special cell displayer for tree items
*
* The group cell display object is used to paint special items of
* QicsTreeTable like header and summary rows.
*
* It can be subclassed in order to provide some custom drawing,
* but in general you should rather
* use QicsSummarizer class to achieve custom look of tree items.
*
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsGroupCellDisplay : public QicsTextCellDisplay
{
public:
    enum QicsOpenIconMode {
        StyleIcon = 1,
        CustomIcon = 2
    };

    QicsGroupCellDisplay(QicsTreeTable *parent);
    virtual ~QicsGroupCellDisplay();

    /*!
    *  Allows cell content be static (non-scrollable) according to \a on.
    */
    inline void setStaticContent(bool on) { m_noScroll = on; }

    /*!
    *  Sets open icon mode to \a mode (bitwise OR of QicsOpenIconMode flags).
    *  In case of CustomIcon, you also have to specify the custom icon set;
    *	see setCustomIcons() method.
    */
    inline void setOpenIconMode(int mode) {m_openIconMode = mode;}

    /*!
    *  Sets custom icons to \a icon.
    *  CustomIcon flag should be enabled via setOpenIconMode() method
    *	in order to draw custom icons.
    */
    inline void setCustomIcons(const QIcon &icon) {m_icon = icon;}

    /*!
    *  Sets level step to \a step.
    */
    inline void setLevelStep(int step = 8) {m_levelStep = step;}

    /*!
    *  If \a set is true, connections between tree nodes are drawn.
    */
    inline void setConnectorsVisible(bool set) {m_conn = set;}

    // Internals
    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    virtual void drawCellBorders(QicsGridInfo *info, int row, int col,
        const QRect &rect, QPainter *painter);

    virtual void prepareConnectors(QPainter *painter, const QRect &rect);

    virtual void drawConnectors(QPainter *painter, const QRect &rect, int row, int level, int offset);

    virtual void drawStyledIcon(QPainter *painter, const QRect &rect, int row, int level, QRect &current_rect,
        QicsExpandableStaticRowData *erow, QStyle *style);

    virtual void drawCustomIcon(QPainter *painter, const QRect &rect, int row, int level, QRect &current_rect,
        QicsExpandableStaticRowData *erow, QStyle *style);

    virtual void drawText(QPainter *painter, const QRect &rect, int row, QRect &current_rect, QStyle *style,
        int flags, const QPalette &palette, const QString &text);

    virtual bool handleClick(int row,int col,int button,const QPoint &p);

    virtual bool handleDoubleClick(int row,int col,int button,const QPoint &p);

protected:
    inline QicsTreeTable* table() { return (QicsTreeTable*)parent(); }

    bool m_noScroll;
    bool m_conn;
    int m_openIconMode;
    int m_levelStep;
    QIcon m_icon;
    QSet<int> m_drawnConns;
};

#endif //QICSGROUPCELLDISPLAY_H


