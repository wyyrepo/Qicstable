/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef QICSBOOLCELLDISPLAY_H
#define QICSBOOLCELLDISPLAY_H

#include <QPixmap>

#include "QicsCellDisplay.h"


class QicsBoolCellDisplay : public QicsNoWidgetCellDisplay
{
public:
    QicsBoolCellDisplay();

    virtual ~QicsBoolCellDisplay();

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm, QRect &rect, QPainter *painter);

    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    virtual bool isEmpty(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

    inline void setTrueDisplayString(const QString &qs) {m_trueString = qs;}
    inline QString trueDisplayString() const {return m_trueString;}

    inline void setFalseDisplayString(const QString &qs) {m_falseString = qs;}
    inline QString falseDisplayString() const {return m_falseString;}

    inline void setTrueDisplayPixmap(const QPixmap &qx) {m_truePixmap = qx;}
    inline QPixmap trueDisplayPixmap() const {return m_truePixmap;}

    inline void setFalseDisplayPixmap(const QPixmap &qx) {m_falsePixmap = qx;}
    inline QPixmap falseDisplayPixmap() const {return m_falsePixmap;}

    inline int percentageOfHeight() const {return m_percentageOfHeight;}
    inline void setPercentageOfHeight(int percent) {m_percentageOfHeight = percent;}

    /*!
    * Returns state of text shifting when icon is present.
    * \sa setIconShiftsText
    * \since 2.4.4
    */
    inline bool iconShiftsText() const {return m_iconShiftsText;}

    /*!
    * Sets state of text shifting when icon is present to \a yes.
    * If set, text will be shifted to the left if icon is present (default).
    * \sa iconShiftsText
    * \since 2.4.4
    */
    inline void setIconShiftsText(bool yes) {m_iconShiftsText = yes;}

protected:
    virtual QString textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

    virtual QPixmap pixmapToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

    virtual bool canDisplayAll(QicsGridInfo *info,
        const QRect &rect, int row, int col,
        const QString &text, int text_flags,
        const QFont &font,
        const QPixmap &pix) const;

private:
    bool boolFromItem(const QicsDataItem *item) const;

private:
    QString m_trueString;
    QString m_falseString;
    QPixmap m_truePixmap;
    QPixmap m_falsePixmap;
    int m_percentageOfHeight;
    bool m_iconShiftsText;
};

#endif //QICSBOOLCELLDISPLAY_H


