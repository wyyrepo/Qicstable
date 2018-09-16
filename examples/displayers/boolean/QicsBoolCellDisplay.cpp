/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "QicsBoolCellDisplay.h"

#include <QPainter>
#include <QApplication>

#include "QicsScreenGrid.h"
#include "QicsCell.h"
#include "QicsMainGrid.h"
#include "QicsCellDisplay_p.h"


QicsBoolCellDisplay::QicsBoolCellDisplay()
    : QicsNoWidgetCellDisplay(),
    m_percentageOfHeight(100),
    m_iconShiftsText(true)
{
}

QicsBoolCellDisplay::~QicsBoolCellDisplay()
{
}

void QicsBoolCellDisplay::displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm, QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, 0, true, true);

    const int cell_alignment = myCell->alignment();
    const int flags = cell_alignment | myCell->textFlags();

    // The Grid Properties
    myGrid->setInfo(d->ginfo);

    QicsGridCellClipping clipping = myGrid->gridCellClipping();

    // Get the pixmap to display
    QPixmap pix = pixmapToDisplay(d->ginfo, row, col, itm);

    // Figure out all the dimensions
    const int pix_width = (pix.isNull() ? 0 : pix.width() + myCell->pixmapSpacing());

    // This region technically may not be accurate, if this cell is a spanner.
    // However, it won't matter, because if/when we ask for an overflow,
    // the grid will say no because (row, col) is a spanner cell.
    QicsRegion area(row, col, row, col);

    QRect cr_full(rect);

    // Ok, now it's time to determine the rectangle that we are going to draw
    // into.  Yes, a rect has been passed to this method, but that's only
    // the starting point.  We want to check if we can display the entire
    // string in our given rect.  If not, we will try to overflow into the
    // next cell.

    bool ready_to_draw = false;
    bool can_display_all = true;

    // No need to try to overflow if this cell is empty.
    if ( d->qs.isEmpty() && pix.isNull() )
        ready_to_draw = true;

    while (!ready_to_draw) {
        if ( !pix.isNull() )
            pix = pix.scaledToHeight( displayableCellArea(d->ginfo, row, col, cr_full).height() * m_percentageOfHeight / 100 );
        if (!canDisplayAll(d->ginfo, cr_full, row, col, d->qs, flags, myCell->font(), pix))
            can_display_all = false;
        // we fit, so we're ready to go
        ready_to_draw = true;
    }

    // Finally, we get to draw.

    // We only display the text and/or pixmap if the entire cell can be drawn or
    // partial cell display is allowed
    if (can_display_all || (clipping != Qics::NoDisplayOnPartial)) {
        QRect cr = displayableCellArea(d->ginfo, row, col, cr_full);

        //
        // Draw the pixmap...
        //
        if (!pix.isNull()) {
            // Because of a Qt bug, we need to "intersect" the cell area
            // with any clip mask that may be set on the painter.

            QRect pix_rect = cr;
            QRegion reg = painter->clipRegion();

            if (!reg.isEmpty()) {
                pix_rect.setRight(qMin(cr.right(), reg.boundingRect().right()));
                pix_rect.setBottom(qMin(cr.bottom(), reg.boundingRect().bottom()));
            }

            // If we have both a pixmap and a string, the pixmap is always
            // displayed on the left

            int alignment;

            if (!d->qs.isEmpty())
                // preserve the vertical alignment, change horizontal
                // alignment to left
                alignment = (cell_alignment & Qt::AlignVertical_Mask) | Qt::AlignLeft;
            else
                alignment = cell_alignment;

            if (!pix.isNull()) {
                d->the_style->drawItemPixmap(
                    painter,
                    pix_rect,
                    alignment,
                    pix);
            }
        }

        //
        // Draw the text...
        //
        if (!d->qs.isEmpty()) {
            QRect str_rect;
            if (m_iconShiftsText)
                str_rect = QRect(QPoint(cr.left() + pix_width, cr.top()), cr.bottomRight());
            else
                str_rect = cr;

            //	    QPalette palet(pal);
            d->pal.setColor(QPalette::Normal, QPalette::Text, d->fg);

            d->the_style->drawItemText(painter,
                str_rect,
                flags, //alignment only
                d->pal,
                true, //enabled
                d->qs,
                QPalette::Text);

            if (!can_display_all && (clipping == Qics::UseClippedSymbol)) {
                // mark that some of the data isn't displayed...

                QPixmap clip_pix( myGrid->moreTextPixmap() );

                if (!clip_pix.isNull()) {
                    QRect marker_size(cr.x() + cr.width() - clip_pix.width(), cr.y(),
                        clip_pix.width(), cr.height());

                    painter->fillRect(marker_size, d->bg);

                    d->the_style->drawItemPixmap(painter,
                        marker_size,
                        Qt::AlignCenter,
                        clip_pix);
                }
            }
        }
    }
}

QString QicsBoolCellDisplay::textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const
{
    Q_UNUSED(info);
    Q_UNUSED(row);
    Q_UNUSED(col);
    return boolFromItem(itm) ? m_trueString : m_falseString;
}

QPixmap QicsBoolCellDisplay::pixmapToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const
{
    Q_UNUSED(info);
    Q_UNUSED(row);
    Q_UNUSED(col);
    return boolFromItem(itm) ? m_truePixmap : m_falsePixmap;
}

QSize QicsBoolCellDisplay::sizeHint(QicsGrid *grid, int row, int col,
                              const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QPixmap pix = pixmapToDisplay(ginfo, row, col, itm);
    QString text = textToDisplay(ginfo, row, col, itm);

    int tflags = myCell->alignment() | myCell->textFlags();
    QFontMetrics fm(myCell->font());
    bool wordbreak = tflags | Qt::TextWordWrap;

    QSize sz(0,0);

    int xoffset = 0;

    // get the pixmap dimensions
    if (!pix.isNull()) {
        xoffset = pix.width() + myCell->pixmapSpacing();

        sz.setWidth(sz.width() + xoffset);
        sz.setHeight(sz.height() + pix.height());
    }

    if (!wordbreak && (text.indexOf('\n') == -1)) {
        // If we aren't breaking lines, or if there's no newline in the
        // string, this is easy...

        sz.setWidth(sz.width() + fm.width(text));
        sz.setHeight(sz.height() + fm.height());
    }
    else {
        // If we do expect more than one line, then we really don't
        // care how wide we are.  So we say that we want to be as
        // wide as the column is now.  Then, once we have that width
        // fixed, we calculate how tall the cell must be in order to
        // display the whole string.

        QRect cr = QRect(0, 0, myCell->widthInPixels(), myCell->heightInPixels());
        QRect dar = displayableCellArea(ginfo, row, col, cr);

        // the area left for the string
        QRect str_rect;
        if (m_iconShiftsText)
            str_rect = QRect(QPoint(dar.left() + xoffset, 0), QPoint(dar.right(), 0));
        else
            str_rect = QRect(QPoint(dar.left(), 0), QPoint(dar.right(), 0));

        QRect br = fm.boundingRect(str_rect.left(), str_rect.top(),
            str_rect.width(), str_rect.height(),
            tflags, text);

        sz.setWidth(sz.width() + br.width());
        sz.setHeight(qMax(sz.height(), br.height()));
    }

    // add the space for the cell border
    int edge_size = myCell->borderWidth() + myCell->margin();

    sz.setWidth(sz.width() + (2 * edge_size));
    sz.setHeight(sz.height() + (2 * edge_size));

    return sz.expandedTo(QApplication::globalStrut());
}

bool QicsBoolCellDisplay::isEmpty( QicsGridInfo *info, int row, int col,
                                  const QicsDataItem *itm ) const
{
    Q_UNUSED(info);
    Q_UNUSED(row);
    Q_UNUSED(col);
    Q_UNUSED(itm);
    return (m_falseString.isNull() && m_trueString.isNull() && m_falsePixmap.isNull() && m_truePixmap.isNull());
}

bool QicsBoolCellDisplay::boolFromItem(const QicsDataItem *item) const
{
    const QicsDataBool *tmpBool;
    const QicsDataDate *tmpDate;
    const QicsDataDateTime *tmpDateTime;
    const QicsDataDouble *tmpDouble;
    const QicsDataFloat *tmpFloat;
    const QicsDataInt *tmpInt;
    const QicsDataLongLong *tmpLongLong;
    const QicsDataString *tmpString;
    const QicsDataTime *tmpTime;

    switch (item->type())
    {
    case QicsDataItem_Bool:
        tmpBool = static_cast<const QicsDataBool*>(item);
        return tmpBool->data();
    case QicsDataItem_Date:
        tmpDate = static_cast<const QicsDataDate*>(item);
        return tmpDate->data().isNull();
    case QicsDataItem_DateTime:
        tmpDateTime = static_cast<const QicsDataDateTime*>(item);
        return tmpDateTime->data().isNull();
    case QicsDataItem_Double:
        tmpDouble = static_cast<const QicsDataDouble*>(item);
        return (bool)tmpDouble->data();
    case QicsDataItem_Float:
        tmpFloat = static_cast<const QicsDataFloat*>(item);
        return (bool)tmpFloat->data();
    case QicsDataItem_Int:
        tmpInt = static_cast<const QicsDataInt*>(item);
        return (bool)tmpInt->data();
    case QicsDataItem_LongLong:
        tmpLongLong = static_cast<const QicsDataLongLong*>(item);
        return (bool)tmpLongLong->data();
    case QicsDataItem_String:
        tmpString = static_cast<const QicsDataString*>(item);
        return tmpString->data().isEmpty();
    case QicsDataItem_Time:
        tmpTime = static_cast<const QicsDataTime*>(item);
        return tmpTime->data().isNull();
    }

    return false;
}

bool QicsBoolCellDisplay::canDisplayAll(QicsGridInfo *info,
                                   const QRect &rect, int row, int col,
                                   const QString &text, int text_flags,
                                   const QFont &font,
                                   const QPixmap &pix) const
{
    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    int pix_width;

    // get the pixmap dimensions
    if (pix.isNull())
        pix_width = 0;
    else
        pix_width = pix.width() + myCell->pixmapSpacing();

    QRect cr = displayableCellArea(info, row, col, rect);

    // the area left for the string
    QRect str_rect;
    if (m_iconShiftsText)
        str_rect = QRect(QPoint(cr.left() + pix_width, cr.top()), cr.bottomRight());
    else
        str_rect = cr;

    QFontMetrics fm(font);

    QRect br = fm.boundingRect(str_rect.left(), str_rect.top(),
        str_rect.width(), str_rect.height(),
        text_flags, text);

    return ((br.width() <= (str_rect.width() + 1)) &&
        (br.height() <= (str_rect.height() + 1)));
}


