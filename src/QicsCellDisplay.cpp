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

#include "QicsCellDisplay.h"

#include <QTimer>
#include <QPainter>
#include <QApplication>
#include <QStyleOptionHeader>
#if QT_VERSION < 0x050000
#include <QWindowsStyle>
#include <QPlastiqueStyle>
#else
#include <QCommonStyle>
#endif

#include "QicsScreenGrid.h"
#include "QicsStyleManager.h"
#include "QicsSpanManager.h"
#include "QicsSelectionManager.h"
#include "QicsCell.h"
#include "QicsMainGrid.h"
#include "QicsRow.h"
#include "QicsColumn.h"
#include "QicsDataItemFormatter.h"
#include "QicsUtil.h"
#include "QicsCellDisplay_p.h"


const QString QicsCellDisplay::BasicCellDisplayName = "AbstractCellDisplay";
bool QicsCellDisplay::active_look = false;
QicsCellDisplayData *QicsCellDisplay::d;


QicsCellDisplay::QicsCellDisplay()
{
    if (!d) d = new QicsCellDisplayData();
    else d->ref();

    myCell = new QicsCell(-1, -1, 0, false, 0);
    myEditCell = new QicsCell(-1, -1, 0, false, 0);
    myRow = new QicsRow(-1, 0, false, 0);
    myColumn = new QicsColumn(-1, 0, false, 0);
    myGrid = new QicsMainGrid(0, 0, false);
    myIsEditing = false;
#if QT_VERSION < 0x050000
    myStyle = new QPlastiqueStyle();
#else
    myStyle = new QCommonStyle();
#endif
    myActiveLook = active_look;
    mySpanned = false;
}

QicsCellDisplay::~QicsCellDisplay()
{
    if (d && d->deref()) {
        delete d;
        d = 0;
    }

    delete myCell;
    delete myEditCell;
    delete myRow;
    delete myColumn;
    delete myGrid;
    delete myStyle;

    myCell = 0;
    myEditCell = 0;
    myRow = 0;
    myColumn = 0;
    myGrid = 0;
}

bool QicsCellDisplay::handleMouseEvent(QicsScreenGrid *, int, int, QMouseEvent *)
{
    return false;
}

bool QicsCellDisplay::handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *)
{
    return false;
}

QString QicsCellDisplay::tooltipText(QicsGridInfo *, int, int,
                             const QicsDataItem *, const QRect &) const
{
    return QString();
}

void QicsCellDisplay::drawBackground(QicsGridInfo *info, int, int,
                                const QRect &rect, const QPalette &pal,
                                QPainter *painter,
                                bool is_current, bool is_selected)
{
    Q_UNUSED(is_current);

    myGrid->setInfo(info);

    // only do the special background if drawing in spreadsheet style
    if ((!is_selected || pal.base().color() == Qt::transparent) && info->gridType() != Qics::TableGrid) {//Ticket ##89683 QicsTable
        if (myGrid->cellDecorationStyle() == Qics::Styled) {
            QStyleOptionHeader ccOptions;
            ccOptions.rect = rect;
            ccOptions.palette = pal;
            ccOptions.state = QStyle::State_Enabled;
            ccOptions.palette.setBrush(QPalette::Button, pal.base());

            QStyle* style = QApplication::style();

#if QT_VERSION < 0x050000
            if (qobject_cast<QWindowsStyle*>(style))
#else
            if (qobject_cast<QCommonStyle*>(style))
#endif
                style = myStyle;

            style->drawControl(QStyle::CE_HeaderSection, &ccOptions, painter, 0);
            return;
        }
    }

    painter->eraseRect(rect);
    painter->fillRect(rect, pal.base());
}

void QicsCellDisplay::drawBorder(QicsGridInfo *info, int, int,
                            const QRect &rect, const QPalette &pal,
                            QPainter *painter,
                            bool is_current, bool)
{
    myGrid->setInfo(info);

    // only do the special border if drawing in spreadsheet style
    is_current = is_current && (myGrid->currentCellStyle() == Qics::Spreadsheet);
    const int cbw = myGrid->currentCellBorderWidth();

    if (is_current && (cbw > 0) && !d->for_printer) { // draw current cell border in the very end
        QPen old_pen = painter->pen();
        QRect nrect(rect);
        // have to offset if we have thick lines
        const int offset = cbw / 2;

        if (myGrid->selectionStyle() != Qics::ExcelStyle)
            nrect.adjust(offset, offset, -offset-(cbw%2), -offset-(cbw%2));
        else
            nrect.adjust(-offset, -offset, offset-(cbw%2), offset-(cbw%2));

        painter->setPen(QPen(pal.windowText().color(), cbw));
        painter->drawRect(nrect);
        painter->setPen(old_pen);
    }
}

void QicsCellDisplay::drawCellBorders(QicsGridInfo *info, int row, int col,
                                 const QRect &rect, QPainter *painter)
{
    myGrid->setInfo(info);

    int vlw = myGrid->verticalGridLineWidth();
    if (vlw < 1) vlw = 1;
    vlw += myGrid->verticalShadeLineWidth();

    int hlw = myGrid->horizontalGridLineWidth();
    if (hlw < 1) hlw = 1;
    hlw += myGrid->horizontalShadeLineWidth();

    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QPen leftst = myCell->leftBorderPen();
    QPen topst = myCell->topBorderPen();
    QPen rightst = myCell->rightBorderPen();
    QPen bottomst = myCell->bottomBorderPen();

    if (topst.style() != Qt::NoPen) {
        // have to offset if we have thick lines
        int bw = topst.width();
        if (!bw) bw=1;

        int top = rect.top();
        if ((hlw/2) < bw) {             // grid line smaller than cell border
            top -= (hlw/2) + (hlw%2);   // center of the grid line
            top += bw/2;                // center of the bottom border line
        }
        else { // grid line bigger than cell border
            top -= bw/2 + bw%2;         // center of the bottom border line
        }

        painter->save();
        painter->setPen(topst);
        painter->drawLine(rect.left()-vlw/2, top, rect.right()+vlw/2, top);
        painter->restore();
    }

    if (bottomst.style() != Qt::NoPen) {
        // have to offset if we have thick lines
        int bw = bottomst.width();
        if(!bw) bw=1;

        int bottom = rect.bottom();
        if ((hlw/2) + (hlw%2) < bw) {       // grid line smaller than cell border
            bottom += (hlw/2) + (hlw%2);    // center of the grid line
            bottom -= bw/2 + (bw%2) - 1;    // center of the bottom border line
        }
        else { // grid line bigger than cell border
            bottom += bw/2 + 1;             // center of the bottom border line
        }

        painter->save();
        painter->setPen(bottomst);
        painter->drawLine(rect.left()-vlw/2, bottom, rect.right()+vlw/2, bottom);
        painter->restore();
    }

    if (leftst.style() != Qt::NoPen) {
        // have to offset if we have thick lines
        int bw = leftst.width();
        if(!bw) bw=1;

        int left = rect.left();
        if ((vlw/2) < bw) { // grid line smaller than cell border
            left -= (vlw/2) + (vlw%2);  // center of the grid line
            left += bw/2;               // center of the bottom border line
        }
        else { // grid line bigger than cell border
            left -= bw/2 + bw%2;        // center of the bottom border line
        }

        painter->save();
        painter->setPen(leftst);
        painter->drawLine(left, rect.top()-hlw/2, left, rect.bottom()+hlw/2);
        painter->restore();
    }

    if (rightst.style() != Qt::NoPen) {
        // have to offset if we have thick lines
        int bw = rightst.width();
        if(!bw) bw=1;

        int right = rect.right();
        if ((vlw/2) + (vlw%2) < bw) { // grid line smaller than cell border
            right += (vlw/2) + (vlw%2); // center of the grid line
            right -= bw/2 + (bw%2) - 1; // center of the bottom border line
        }
        else { // grid line bigger than cell border
            right += bw/2 + 1;          // center of the bottom border line
        }

        painter->save();
        painter->setPen(rightst);
        painter->drawLine(right, rect.top()-hlw/2, right, rect.bottom()+hlw/2);
        painter->restore();
    }
}

bool QicsCellDisplay::isCellSelected(QicsGridInfo *info, int row, int col)
{
    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    return (myCell->selected());
}

QPalette &QicsCellDisplay::cellPalette(bool enabled, bool for_printer)
{
    d->pal = myCell->palette();
    d->pal.setCurrentColorGroup(enabled ? QPalette::Active : QPalette::Disabled);

    // If we are printing, we'd like to replace the default background with
    // white.
    if (for_printer) {
        QPalette grid_pal = myGrid->palette();
        grid_pal.setCurrentColorGroup(enabled ? QPalette::Active : QPalette::Disabled);

        if (d->pal.window().color() == grid_pal.window().color())
            d->pal.setColor(QPalette::Window, Qt::color0);
    }

    return d->pal;
}

void QicsCellDisplay::aboutToClear(QicsGridInfo *info, int row, int col)
{
    Q_UNUSED(info);
    Q_UNUSED(row);
    Q_UNUSED(col);
}

void QicsCellDisplay::commonInit(QicsGrid *grid, int row, int col,
                            const QicsDataItem *itm,
                            QRect &rect, QPainter *painter,
                            QWidget *wdg,
                            bool draw_bg, bool consider_frame)
{
    d->ginfo = &(grid->gridInfo());
    myGrid->setInfo(d->ginfo);

    d->for_printer = isPrinterDevice(painter, grid);
    //if (d->for_printer)
    //    myGrid->setInfo(d->ginfo);

    const QicsSelectionManager *sm = d->ginfo->selectionManager();

    // First, let's retrieve some cell and grid properties.
    // We dont want to query more than we need becasue each query is
    // potentially expensive.

    myCell->setInfo(d->ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    d->is_enabled = myCell->enabled() || myActiveLook;
    if (wdg) d->is_enabled &= wdg->isEnabled();
    d->is_current = myCell->isCurrent() && (d->ginfo->gridType() == Qics::TableGrid);
    d->is_selected = isCellSelected(d->ginfo, row, col);
    d->alignment = myCell->alignment();
    d->pal = cellPalette(d->is_enabled, d->for_printer);

    if (!d->for_printer && sm && sm->selectCurrentCellPolicy() == Qics::Static && d->ginfo->gridType() == Qics::TableGrid) {
        if (sm->exclusiveSelection()) {
            QicsICell cell = sm->exclusiveSelection()->anchorCell();
            d->is_current = (cell.row() == row && cell.column() == col);
        }
    }

    // setup some stuff
    if (d->is_selected && !myCell->selectedFont().family().isEmpty())
        painter->setFont(myCell->selectedFont());
    else
        painter->setFont(myCell->font());

    if ((d->is_current && myGrid->currentCellStyle() == Qics::Spreadsheet) || !d->is_selected) {
        d->bg = d->pal.base().color();
        d->fg = d->pal.text().color();
    }
    else {
        d->bg = d->pal.highlight().color();
        d->fg = d->pal.highlightedText().color();
    }

    // special header highlight
    if (sm && d->ginfo->gridType() != Qics::TableGrid && !d->is_selected && myGrid->highlightSelection() && !d->for_printer) {
        QicsSelectionList* list = sm->selectionList();
        if (list) {
            // add current selection to the list
            if (sm->currentSelection())
                list->append(*sm->currentSelection());

            bool sel = false;
            if (d->ginfo->gridType() == Qics::RowHeaderGrid)
                sel = list->isRowSelected(row, false);
            else
                sel = list->isColumnSelected(col, false);
            if (sel) {
                d->is_selected = true;
                d->bg = myCell->highlightBackgroundColor();
                d->fg = myCell->highlightForegroundColor();
            }
            delete list;
        }
    }

    d->pal.setColor(QPalette::Base, d->bg);
    d->pal.setColor(QPalette::Text, d->fg);


    // Finally, we get to draw.  First, draw the background and the border.
    if (draw_bg) {
        drawBackground(d->ginfo, row, col, rect, d->pal, painter, d->is_current, d->is_selected);
        drawBorder(d->ginfo, row, col, rect, d->pal, painter, d->is_current, d->is_selected);
    }

    d->cr = displayableCellArea(d->ginfo, row, col, rect, consider_frame, consider_frame);
    d->the_style = wdg ? wdg->style() : QApplication::style();
    d->qs = textToDisplay(d->ginfo, row, col, itm);
    d->style_flags = QStyle::State_Active;

    if (wdg && wdg->parentWidget()) {
        if (d->is_enabled && wdg->parentWidget()->isEnabled())
            d->style_flags |= QStyle::State_Enabled;
    } else
        if (d->is_enabled) d->style_flags |= QStyle::State_Enabled;

    const QStyle::State highlight_flags =
        QStyle::State_HasFocus | QStyle::State_Active | QStyle::State_Selected | QStyle::State_MouseOver;

    if (d->ginfo->selectionManager()) {
        switch (d->ginfo->selectionManager()->selectionPolicy())
        {
            // selection policies where something always selected
        case Qics::SelectSingleRow:
        case Qics::SelectMultipleRow:
        case Qics::SelectMultiple:
            if (((!d->is_current && d->is_selected)
                || (d->is_current && !d->ginfo->selectionInProgress() )))
                d->style_flags |= highlight_flags;
            break;
            // nothing selected
        case Qics::SelectNone:
        case Qics::SelectSingle:
            if (d->is_current)
                d->style_flags |= highlight_flags;
            break;
        }
    }
}

QRect QicsCellDisplay::displayableCellArea(QicsGridInfo *,
                                     int, int,
                                     const QRect &cr_full,
                                     bool consider_margin,
                                     bool consider_border) const
{
    int line_width = 0;
    int cell_margin = 0;

    if (myCell) {
        if (consider_border)
            line_width = myCell->borderWidth();

        if (consider_margin)
            cell_margin = myCell->margin();

        // compute the area we will be drawing in by subtracting the border and margin
        QRect cr = QRect(cr_full.x() + line_width + cell_margin,
            cr_full.y() + line_width + cell_margin,
            cr_full.width() - (line_width *2) - (cell_margin *2),
            cr_full.height() - (line_width *2) - (cell_margin *2) );

        return cr;
    }

    return cr_full;
}

QString QicsCellDisplay::textToDisplay(QicsGridInfo *info, int row, int col,
                               const QicsDataItem *itm) const
{
    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QString qs = myCell->label();

    if (qs.isEmpty() && itm) {
        QicsDataItemFormatter *formatter = myCell->formatter();

        if (formatter)
            return formatter->format(*itm);

        return itm->string();
    }

    return qs;
}

QPixmap QicsCellDisplay::pixmapToDisplay(QicsGridInfo *info, int row, int col,
                                 const QicsDataItem *) const
{
    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    return myCell->pixmap();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////     QicsNoWidgetCellDisplay      ////////////////////////
/////////////////////////////////////////////////////////////////////////////

QicsNoWidgetCellDisplay::QicsNoWidgetCellDisplay()
    : QicsCellDisplay()
{
}

QicsNoWidgetCellDisplay::~QicsNoWidgetCellDisplay()
{
}

void QicsNoWidgetCellDisplay::moveEdit(QicsScreenGrid *, int, int, const QRect &)
{
}

void QicsNoWidgetCellDisplay::startEdit(QicsScreenGrid *, int, int,
                                   const QicsDataItem *)
{
}

void QicsNoWidgetCellDisplay::endEdit(QicsScreenGrid *, int, int)
{
}

/////////////////////////////////////////////////////////////////////////////
//////////////     QicsMovableEntryWidgetCellDisplay      /////////////////////
/////////////////////////////////////////////////////////////////////////////

QicsMovableEntryWidgetCellDisplay::QicsMovableEntryWidgetCellDisplay()
    : QicsCellDisplay()
{
}

QicsMovableEntryWidgetCellDisplay::~QicsMovableEntryWidgetCellDisplay()
{
    qDeleteAll(myEntryList);
    myEntryList.clear();
}

void QicsMovableEntryWidgetCellDisplay::moveEdit(QicsScreenGrid *grid,
                                            int row, int col,
                                            const QRect &rect)
{
    QWidget *widget = getInfoFromGrid(grid)->widget();
    QicsGridInfo *ginfo = &(grid->gridInfo());

    widget->setGeometry(entryWidgetRect(ginfo, row, col, rect));
    widget->show();
    widget->setFocus();
}

void QicsMovableEntryWidgetCellDisplay::endEdit(QicsScreenGrid *grid, int row, int col)
{
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    if (info) {
        QWidget *widget = info->widget();
        if (widget)
            widget->hide();
    }
    QicsCellDisplay::endEdit(grid,row,col);
}

QicsMovableEntryWidgetCellDisplay::QicsEntryWidgetInfo *
QicsMovableEntryWidgetCellDisplay::getInfoFromGrid(QicsScreenGrid *grid)
{
    QicsEntryWidgetInfo *info = myEntryList.value(grid);

    if (!info) {
        info = new QicsEntryWidgetInfo();
        info->setWidget(newEntryWidget(grid));
        info->setGrid(grid);

        myEntryList.insert(grid, info);
    }

    return info;
}

QicsMovableEntryWidgetCellDisplay::QicsEntryWidgetInfo *
QicsMovableEntryWidgetCellDisplay::takeInfoFromGrid(QicsScreenGrid *grid)
{
    return myEntryList.take(grid);
}


QicsMovableEntryWidgetCellDisplay::QicsEntryWidgetInfo *
QicsMovableEntryWidgetCellDisplay::getInfoFromEntry(const QWidget *widget)
{
    QicsEntryWidgetInfoPL::const_iterator it(myEntryList.constBegin());
    QicsEntryWidgetInfoPL::const_iterator it_end(myEntryList.constEnd());
    while (it != it_end) {
        if ((*it)->widget() == widget)
            return (*it);
        ++it;
    }
    return 0;
}

QRect QicsMovableEntryWidgetCellDisplay::entryWidgetRect(QicsGridInfo *ginfo, int row, int col,
                                                   QRect cell_rect)
{
    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    return cell_rect;
}

QString QicsCellDisplay::cellDisplayName() const
{
    return BasicCellDisplayName;
}

QDomElement QicsCellDisplay::toDomXml(const QString &tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());
    return e;
}

void QicsCellDisplay::configureFromDomXml(const QDomElement &e)
{
    Q_UNUSED(e);
}


