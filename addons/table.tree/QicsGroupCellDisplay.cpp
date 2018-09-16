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

#include <QicsGroupCellDisplay.h>

#include <QStyleOption>
#include <QPainter>

#include <QicsExpandableStaticRowData.h>
#include <QicsCellDisplay_p.h>


QicsGroupCellDisplay::QicsGroupCellDisplay(QicsTreeTable *parent)
    : QicsTextCellDisplay(parent),
        m_noScroll(false),
        m_conn(true),
        m_openIconMode(StyleIcon),
        m_levelStep(8)
{
    mySpanned = true;
}

QicsGroupCellDisplay::~QicsGroupCellDisplay()
{
}

bool QicsGroupCellDisplay::handleClick(int row,int col,int button,const QPoint &p)
{
    Q_UNUSED(col);

    if (button != Qt::LeftButton)
        return false;

    int mrow = table()->gridInfo().modelRowIndex(row);
    QicsSpecialRowData *srow = table()->specialRowData(mrow);
    QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(srow);
    if (erow) {
        int x = erow->level() * m_levelStep;
        int dx = x + 16;
        if (p.x() >= x && p.x() <= dx) {
            erow->switchTree();
            return true;
        }
    }

    return false;
}

bool QicsGroupCellDisplay::handleDoubleClick(int row,int col,int button,const QPoint &p)
{
    Q_UNUSED(col);
    Q_UNUSED(p);

    if (button != Qt::LeftButton)
        return false;

    int mrow = table()->gridInfo().modelRowIndex(row);
    QicsSpecialRowData *srow = table()->specialRowData(mrow);
    QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(srow);
    if (erow) {
        erow->switchTree();
        return true;
    }

    return false;
}

void QicsGroupCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                       const QicsDataItem *itm,
                                       QRect &rect, QPainter *painter)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsGridInfo *tinfo = &(table()->gridInfo());
    bool treeHeader = table()->treeInHeader();
    bool fakeHeader = treeHeader && ginfo->gridType() == Qics::RowHeaderGrid;

    commonInit(grid, row, col, itm, rect, painter, 0, false, false);
    int cell_alignment = myCell->alignment();
    int flags = cell_alignment | myCell->textFlags();

    int mrow = tinfo->modelRowIndex(row);
    QicsSpecialRowData *srow = table()->specialRowData(mrow);
    QicsExpandableStaticRowData *erow = qobject_cast<QicsExpandableStaticRowData*>(srow);

    if ( mySpanned || fakeHeader ) {

        // fake pal to table if drawing header (same colors)
        if (tinfo->currentCell().row() == row && ((!fakeHeader && srow) || (fakeHeader && erow))) {
            d->is_current = d->is_selected = true;
            d->pal.setColor(QPalette::Base, d->pal.color(QPalette::Highlight));
            d->pal.setColor(QPalette::Text, d->pal.color(QPalette::HighlightedText));
        }

        // fake grid to table if drawing header (flat drawing)
        painter->fillRect(rect, d->pal.base());

    }

    QRect str_rect(rect);

    // draw icons
    if (erow) {
        if (fakeHeader || ((!col || m_noScroll) && !treeHeader)) {

            int x = erow->level() * m_levelStep;

            str_rect.setLeft(str_rect.left() + x);

            // draw styled open icon
            if ((m_openIconMode & StyleIcon) && ( table()->treeMode() != QicsTreeTable::Flat_No_Details) )
                drawStyledIcon(painter, rect, row, erow->level(), str_rect, erow, d->the_style);

            // draw custom open icon
            if ((m_openIconMode & CustomIcon) && ( table()->treeMode() != QicsTreeTable::Flat_No_Details) )
                drawCustomIcon(painter, rect, row, erow->level(), str_rect, erow, d->the_style);
        }
    }

    // draw connectors
    if (m_conn && fakeHeader &&  ( table()->treeMode() != QicsTreeTable::Flat_No_Details) ) {
        QicsGroupInfo *top = &(table()->topGroupRef());
        if (top) {
            m_drawnConns.clear();
            prepareConnectors(painter, rect);

            QicsGroupInfo *gi = table()->groupForRow(mrow);
            QicsGroupInfo *pgi;
            if (gi) {
                do {
                    pgi = gi->parentGroup();
                    int hvi = gi->headerVisualIndex();

                    for (int i = 0; i < pgi->groups().count(); ++i) {
                        int ghvi = pgi->groups().at(i)->headerVisualIndex();
                        if (ghvi > hvi) {
                            int level = gi->level();
                            drawConnectors(painter, rect, row, level, level * m_levelStep);
                            break;
                        }
                    }

                    gi = pgi;
                } while (pgi != top);
            }
        }
    }

    bool sum = table()->isSummary(mrow);

    // draw text
    if ( mySpanned || fakeHeader) {

        if (fakeHeader)
            d->qs = table()->rowHeaderRef().rowRef(row).label();

        bool paint = false;
        if (fakeHeader) {
            if (!sum) paint = true;
        } else {
            if ((!col || m_noScroll) && (!treeHeader || sum)) paint = true;
        }

        if (paint) {
            QFontMetrics fm( painter->font() );
            int width = str_rect.x() + fm.boundingRect( d->qs ).width() - rect.width();
            if ( width > 0 )
                table()->headerResizeNeeds( width );
            drawText(painter, rect, row, str_rect, d->the_style, flags, d->pal, d->qs);
            return;
        }

        if (sum)
            return;
    }

    QicsTextCellDisplay::displayCell(grid, row, col, itm, str_rect, painter);
}

void QicsGroupCellDisplay::prepareConnectors(QPainter *painter, const QRect &rect)
{
    Q_UNUSED(rect);
    painter->setPen(QPen(Qt::gray, 1, Qt::DotLine));
}

void QicsGroupCellDisplay::drawConnectors(QPainter *painter, const QRect &rect, int row, int level, int offset)
{
    Q_UNUSED(row);
    Q_UNUSED(level);

    QRect rr(rect);
    rr.setLeft(rr.left() + offset + 4);
    painter->drawLine(rr.topLeft(), rr.bottomLeft());
}

void QicsGroupCellDisplay::drawStyledIcon(QPainter *painter, const QRect &rect, int row, int level, QRect &current_rect,
                                          QicsExpandableStaticRowData *erow, QStyle *style)
{
    Q_UNUSED(row);
    Q_UNUSED(rect);
    Q_UNUSED(level);
    Q_UNUSED(style);

    QStyleOption so;
    so.rect = QRect(current_rect.topLeft(), QSize(12,12));
    if (erow->children().count()) {
        so.state = QStyle::State_Children;
        if (erow->isOpen())
            so.state |= QStyle::State_Open;
        d->the_style->drawPrimitive(QStyle::PE_IndicatorBranch, &so, painter);
    }

    current_rect.setLeft(current_rect.left() + 12 + 4);
}

void QicsGroupCellDisplay::drawCustomIcon(QPainter *painter, const QRect &rect, int row, int level, QRect &current_rect,
                                          QicsExpandableStaticRowData *erow, QStyle *style)
{
    Q_UNUSED(row);
    Q_UNUSED(rect);
    Q_UNUSED(level);
    Q_UNUSED(style);

    QIcon::Mode mode = QIcon::Normal;
    if (d->is_selected) mode = QIcon::Selected;
    if (d->is_current) mode = QIcon::Active;
    if (!d->is_enabled || !erow->children().count()) mode = QIcon::Disabled;

    QPixmap pm( m_icon.pixmap(QSize(16,16), mode, erow->isOpen() ? QIcon::On : QIcon::Off) );
    if (!pm.isNull()) {
        painter->drawPixmap(current_rect.topLeft(), pm);
        current_rect.setLeft(current_rect.left() + pm.width() + 4);
    }
}

void QicsGroupCellDisplay::drawText(QPainter *painter, const QRect &rect, int row, QRect &current_rect, QStyle *style,
                                    int flags, const QPalette &palette, const QString &text)
{
    Q_UNUSED(row);
    Q_UNUSED(rect);

    style->drawItemText(painter,
        current_rect,
        flags,          //alignment only
        palette,
        d->is_enabled,     //enabled
        text,
        QPalette::Text);
}

void QicsGroupCellDisplay::drawCellBorders(QicsGridInfo *info, int row, int col,
                                           const QRect &rect, QPainter *painter)
{
    Q_UNUSED(info);
    Q_UNUSED(row);
    Q_UNUSED(col);
    Q_UNUSED(rect);
    Q_UNUSED(painter);
}


