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

#include "QicsCheckCellDisplay.h"

#include <QStyle>
#include <QStyleOptionButton>
#include <QApplication>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>

#include "QicsScreenGrid.h"
#include "QicsUtil.h"
#include "QicsCell.h"
#include "QicsCellDisplay_p.h"


#define QICS_CHECK_INDICATOR_SPACING 5
const QString QicsCheckCellDisplay::CheckCellDisplayName = "CheckCellDisplay";


QicsCheckCellDisplay::QicsCheckCellDisplay( QWidget * parent )
    : QCheckBox( parent ), QicsMovableEntryWidgetCellDisplay()
{
    celldisplay_init();
}

QicsCheckCellDisplay::QicsCheckCellDisplay( const QString & text, QWidget * parent )
    : QCheckBox( text, parent ), QicsMovableEntryWidgetCellDisplay()
{
    celldisplay_init();
}

void QicsCheckCellDisplay::celldisplay_init()
{
    m_isWholeCellClickable = false;
    connect(this, SIGNAL(stateChanged(int)), SLOT(checkStateChanged(int)));
    hide();
}

QicsCheckCellDisplay::~QicsCheckCellDisplay()
{
}

QWidget *QicsCheckCellDisplay::newEntryWidget(QicsScreenGrid *)
{
    return static_cast<QCheckBox *> (this);
}

void QicsCheckCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                  const QicsDataItem *itm,
                                  QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, this, true, true);

    // the stuff to display
    QSize sz = QSize(d->the_style->pixelMetric(QStyle::PM_IndicatorWidth),
            d->the_style->pixelMetric(QStyle::PM_IndicatorHeight));

    // the stuff to display
    Qt::CheckState cs = fullCheckState(d->ginfo, row, col, itm);
    switch (cs)
    {
    case Qt::Unchecked:
        d->style_flags |= QStyle::State_Off;
        break;
    case Qt::Checked:
        d->style_flags |= QStyle::State_On;
        break;
    default:
        d->style_flags |= QStyle::State_NoChange;
        break;
    }

    bool blocked = QCheckBox::blockSignals(true);
    QCheckBox::setCheckState(cs);
    QCheckBox::blockSignals(blocked);

    QStyleOptionButton styleOps;

    if (myCell->alignment() == Qt::AlignCenter
        || myCell->alignment() == Qt::AlignVCenter
        || myCell->alignment() == Qt::AlignHCenter) {                       // ------->>><<<--------
        styleOps.rect = QRect(d->cr.left()+(d->cr.width()-sz.width())/2,
            d->cr.top() + (d->cr.height() - sz.height()) / 2,
            sz.width(), sz.height());
    }
    else if (isRightToLeft() || myCell->alignment() == Qt::AlignRight) {    //  <<<<-----
        styleOps.rect = QRect(d->cr.right()-sz.width(), d->cr.top() +
            (d->cr.height() - sz.height()) / 2,
            sz.width(), sz.height());
    }
    else {                                                                  //   ----->>>>
        styleOps.rect = QRect(d->cr.left(), d->cr.top() +
            (d->cr.height() - sz.height()) / 2,
            sz.width(), sz.height());
    }

    styleOps.state = d->style_flags;
    styleOps.palette = d->pal;

    d->the_style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &styleOps, painter);

    // Draw the pixmap
    QPixmap pix = pixmapToDisplay(d->ginfo, row, col, itm);

    QRect pix_rect;
    if (!pix.isNull() && (myCell->alignment() != Qt::AlignCenter
        && myCell->alignment() != Qt::AlignHCenter
        && myCell->alignment() != Qt::AlignVCenter)) {
        pix = pix.scaled(styleOps.rect.height(),styleOps.rect.height());

        if (isRightToLeft() || myCell->alignment() == Qt::AlignRight) {
            pix_rect = QRect(styleOps.rect.left() - QICS_CHECK_INDICATOR_SPACING - pix.width(),
                styleOps.rect.top(),pix.width(),pix.height());
        }
        else {
            pix_rect = QRect(styleOps.rect.right() + QICS_CHECK_INDICATOR_SPACING,
                styleOps.rect.top(), pix.width(),pix.height());
        }

        QApplication::style()->drawItemPixmap(painter,
            pix_rect,
            myCell->alignment(),
            pix);
    }

    // Draw text
    if (!d->qs.isEmpty() && (myCell->alignment() != Qt::AlignCenter
        && myCell->alignment() != Qt::AlignVCenter
        && myCell->alignment() != Qt::AlignHCenter)) {
        QRect trect(rect);
        QFontMetrics fm(myCell->font());
        int alignment;
        if (isRightToLeft() || myCell->alignment() == Qt::AlignRight) {
            if (isRightToLeft())
                alignment = Qt::AlignRight;
            else
                alignment = Qt::AlignLeft;
            trect.moveLeft(styleOps.rect.left() - QICS_CHECK_INDICATOR_SPACING
                - pix_rect.width() - ((pix_rect.isNull())? 0 : QICS_CHECK_INDICATOR_SPACING) - fm.width(d->qs));
        }
        else {
            alignment = Qt::AlignLeft;
            trect.moveLeft(styleOps.rect.right() + QICS_CHECK_INDICATOR_SPACING
                + pix_rect.width() + ((pix_rect.isNull())? 0 : QICS_CHECK_INDICATOR_SPACING));
        }

        trect.setTop(d->cr.top() + (d->cr.height() - fm.height())/2);
        d->the_style->drawItemText(painter, trect, alignment, d->pal, d->is_enabled, d->qs, QPalette::Text );
    }
}

void QicsCheckCellDisplay::startEdit(QicsScreenGrid *grid, int row, int col,
                                const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QCheckBox *widget = static_cast<QCheckBox *> (info->widget());
    widget->setParent(grid);

    info->setRow(ginfo->modelRowIndex(row));
    info->setColumn(ginfo->modelColumnIndex(col));

    // Setup widget (colors, fonts, values etc) for this cell

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    widget->setFont(myCell->font());

    // Unfortunately, we really want to use the Base color as our
    // background color, so we need to whack this a bit...

    QPalette p = myCell->palette();
    p.setColor(QPalette::Active, QPalette::Background, p.color(QPalette::Active, QPalette::Base) );
    p.setColor(QPalette::Inactive, QPalette::Background,p.color(QPalette::Inactive, QPalette::Base) );
    p.setColor(QPalette::Disabled, QPalette::Background,p.color(QPalette::Disabled, QPalette::Base) );

    widget->setPalette(p);

    bool state = widget->blockSignals(true);
    widget->setCheckState(fullCheckState(ginfo, row, col, itm));
    widget->blockSignals(state);
    QicsCellDisplay::startEdit(grid, row, col, itm);
}

void QicsCheckCellDisplay::moveEdit(QicsScreenGrid *grid, int row, int col,
                               const QRect &rect)
{
    QWidget *widget = getInfoFromGrid(grid)->widget();
    QicsGridInfo *ginfo = &(grid->gridInfo());

    widget->setGeometry(entryWidgetRect(ginfo, row, col, rect));
}

QRect QicsCheckCellDisplay::entryWidgetRect(QicsGridInfo *ginfo, int row, int col,
                                                   QRect cell_rect)
{
    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    return displayableCellArea(ginfo, row, col, cell_rect);
}

void QicsCheckCellDisplay::focusOutEvent(QFocusEvent* fe)
{
    myLastFocusReason = fe->reason();

    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid*>(parentWidget());
    if (grid)
        grid->uneditCurrentCell();

    QCheckBox::focusOutEvent(fe);
}

bool QicsCheckCellDisplay::handleMouseEvent(QicsScreenGrid *grid, int row, int col,
                       QMouseEvent *me)
{
    if (me->button() == Qt::RightButton)
        return true;
    if (me->button() == Qt::MidButton)
        return false;

    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);
    myCell->setInfo(&grid->gridInfo());

    if (!myCell->enabled())
        return false;

    QicsICell cur_cell = grid->currentCell();

    if ((!cur_cell.isValid() ||
        ((cur_cell.row() != row) || (cur_cell.column() != col))) &&
        me->type() != QEvent::MouseButtonRelease && me->type() != QEvent::MouseMove)
    {
       if (!grid->editCell(row, col)) return false;
    }

    QPoint res_point;

    if (!m_isWholeCellClickable) {
        QRect r = grid->cellDimensions(row, col, false);

        QPoint p(me->x() - r.left() - myCell->borderWidth() - myCell->margin(),
            me->y() - r.top() - myCell->borderWidth() - myCell->margin());

        // size of indicator
        int ind_width = QApplication::style()->pixelMetric(QStyle::PM_IndicatorWidth);

        QFontMetrics fm(myCell->font());
        int text_width = fm.width(myCell->label());
        int pix_width = myCell->pixmap().width();
        int res_width = ind_width+text_width+pix_width;

        if (myCell->alignment() == Qt::AlignRight && !isRightToLeft())
            res_point = QPoint(r.width() - p.x(),p.y());
        else if (myCell->alignment() == Qt::AlignCenter
            || myCell->alignment() == Qt::AlignHCenter
            || myCell->alignment() == Qt::AlignVCenter) {
                res_point = p;
                if(isRightToLeft())
                    res_point.rx() += (r.width() - res_width)/2;
                else
                    res_point.rx() -= (r.width() - res_width)/2;
        }
        else
            res_point = p;
    }
    else
        res_point = QPoint(1,1);

    QMouseEvent new_event(me->type(), res_point, me->button(), me->buttons(), me->modifiers());
    QCheckBox::event(&new_event);

    return false;
}

QSize QicsCheckCellDisplay::sizeHint(QicsGrid *grid, int row, int col,
                               const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QStyle *style = QApplication::style();

    QString text(textToDisplay(ginfo, row, col, itm));

    int xoffset = 0;

    QSize sz = QSize(style->pixelMetric(QStyle::PM_IndicatorWidth),
        style->pixelMetric(QStyle::PM_IndicatorHeight));

    // spacing between indicator and text
    sz.setWidth(sz.width() + QICS_CHECK_INDICATOR_SPACING);

    xoffset += sz.width();

    // get the pixmap dimensions

    QPixmap pix = pixmapToDisplay(ginfo, row, col, itm);
    if (!pix.isNull()) {
        int pix_margin = myCell->pixmapSpacing();
        sz.setWidth(sz.width() + pix.width() + pix_margin);
        sz.setHeight(qMax(sz.height(), pix.height()));

        xoffset += pix.width() + pix_margin;
    }

    pix = pix.scaled(QSize(0,0));

    int tflags = myCell->alignment() | myCell->textFlags();
    QFontMetrics fm(myCell->font());
    bool wordbreak = tflags | Qt::TextWordWrap;

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
        QRect str_rect(QPoint(dar.left() + xoffset, 0),
            QPoint(dar.right(), 0));

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

QString QicsCheckCellDisplay::textToDisplay(QicsGridInfo *info, int row, int col,
                                    const QicsDataItem *item) const
{
    Q_UNUSED(item);

    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QString label = myCell->label();

    if (label.isEmpty())
        return text();

    return label;
}

Qt::CheckState QicsCheckCellDisplay::fullCheckState(QicsGridInfo *info, int row, int col,
                                                    const QicsDataItem *item)
{
    if (!item)
        return Qt::Unchecked;

    if (isTristate()) {
        switch (item->type())
        {
        case QicsDataItem_Double:
        case QicsDataItem_Float:
        case QicsDataItem_Int:
        case QicsDataItem_Long:
        case QicsDataItem_LongLong: {
                bool ok;
                double d = item->string().toDouble(&ok);
                if (!ok || !d)
                    return Qt::Unchecked;
                return (d == Qt::PartiallyChecked) ?
                    Qt::PartiallyChecked : Qt::Checked;
            }

        case QicsDataItem_String: {
                QString tmpString = (static_cast<const QicsDataString*>( item ))->data();
                return tmpString == QICS_DEFAULT_TRUE_STRING ? Qt::Checked :
                    tmpString == QICS_DEFAULT_FALSE_STRING ? Qt::Unchecked :
                    Qt::PartiallyChecked;
            }

        default:
            break;
        }
    }

    return checkState(info, row, col, item) ? Qt::Checked : Qt::Unchecked;
}

bool QicsCheckCellDisplay::checkState(QicsGridInfo *, int, int,
                                      const QicsDataItem *item)
{
    if (!item)
        item = new QicsDataInt(0);

    const QicsDataBool* tmpBool;
    const QicsDataDate* tmpDate;
    const QicsDataDateTime* tmpDateTime;
    const QicsDataDouble* tmpDouble;
    const QicsDataFloat* tmpFloat;
    const QicsDataInt* tmpInt;
    const QicsDataLong* tmpLong;
    const QicsDataLongLong* tmpLongLong;
    const QicsDataString* tmpString;
    const QicsDataTime* tmpTime;

    switch (item->type())
    {
    case QicsDataItem_Bool:
        tmpBool = static_cast<const QicsDataBool*>( item );
        return tmpBool->data();
    case QicsDataItem_Date:
        tmpDate = static_cast<const QicsDataDate*>( item );
        return !tmpDate->data().isNull();
    case QicsDataItem_DateTime:
        tmpDateTime = static_cast<const QicsDataDateTime*>( item );
        return !tmpDateTime->data().isNull();
    case QicsDataItem_Double:
        tmpDouble = static_cast<const QicsDataDouble*>( item );
        return ( bool ) tmpDouble->data();
    case QicsDataItem_Float:
        tmpFloat = static_cast<const QicsDataFloat*>( item );
        return ( bool ) tmpFloat->data();
    case QicsDataItem_Int:
        tmpInt = static_cast<const QicsDataInt*>( item );
        return ( bool ) tmpInt->data();
    case QicsDataItem_Long:
        tmpLong = static_cast<const QicsDataLong*>( item );
        return ( bool ) tmpLong->data();
    case QicsDataItem_LongLong:
        tmpLongLong = static_cast<const QicsDataLongLong*>( item );
        return ( bool ) tmpLongLong->data();
    case QicsDataItem_String:
        tmpString = static_cast<const QicsDataString*>( item );
        return (tmpString->data() == QICS_DEFAULT_TRUE_STRING);
    case QicsDataItem_Time:
        tmpTime = static_cast<const QicsDataTime*>( item );
        return !tmpTime->data().isNull();
    }
    return false;
}

void QicsCheckCellDisplay::valueChanged(QicsGridInfo *info, int vis_row, int vis_col, Qt::CheckState set)
{
    const QicsDataItem *item = info->cellValue(vis_row, vis_col);

    if (!item)
        item = new QicsDataInt(0);

    switch (item->type())
    {
    case QicsDataItem_Bool:
        info->setCellValue(vis_row,vis_col,QicsDataBool(set));
        break;
    case QicsDataItem_Double:
        info->setCellValue(vis_row,vis_col,static_cast<QicsDataDouble>(set));
        break;
    case QicsDataItem_Float:
        info->setCellValue(vis_row,vis_col,static_cast<QicsDataFloat>(set));
        break;
    case QicsDataItem_Int:
        info->setCellValue(vis_row,vis_col,static_cast<QicsDataInt>(set));
        break;
    case QicsDataItem_Long:
        info->setCellValue(vis_row,vis_col,static_cast<QicsDataLong>(set));
        break;
    case QicsDataItem_LongLong:
        info->setCellValue(vis_row,vis_col,static_cast<QicsDataLongLong>(set));
        break;
    case QicsDataItem_String:
        switch (set)
        {
        case Qt::Checked:
            info->setCellValue(vis_row,vis_col,QicsDataString(QICS_DEFAULT_TRUE_STRING));
            return;
        case Qt::Unchecked:
            info->setCellValue(vis_row,vis_col,QicsDataString(QICS_DEFAULT_FALSE_STRING));
            return;
        default:
            info->setCellValue(vis_row,vis_col,QicsDataString(QICS_DEFAULT_PARTIAL_STRING));
            return;
        }
    case QicsDataItem_Time:
        if (set)
            info->setCellValue(vis_row,vis_col,QicsDataTime(QTime(0,0)));
        else
            info->setCellValue(vis_row,vis_col,QicsDataTime());
        break;
    case QicsDataItem_Date:
        if (set)
            info->setCellValue(vis_row,vis_col,QicsDataDate(0,0,0));
        else
            info->setCellValue(vis_row,vis_col,QicsDataDate());
        break;
    case QicsDataItem_DateTime:
        if (set)
            info->setCellValue(vis_row,vis_col,QicsDataDateTime(QDate(0,0,0),QTime(0,0,0,0)));
        else
            info->setCellValue(vis_row,vis_col,QicsDataDateTime());
        break;
    default:
        break;
    }
}

void QicsCheckCellDisplay::checkStateChanged(int state)
{
    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(parent());
    if (!grid) return;

    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(ginfo->visualRowIndex(info->row()));
    myCell->setColumnIndex(ginfo->visualColumnIndex(info->column()));

    valueChanged(ginfo,
        ginfo->visualRowIndex(info->row()),
        ginfo->visualColumnIndex(info->column()),
        isTristate() ? (Qt::CheckState)state :
        checkState(ginfo, ginfo->visualRowIndex(info->row()), ginfo->visualColumnIndex(info->column()),
            myCell->dataValue()) ? Qt::Unchecked : Qt::Checked);
}

bool QicsCheckCellDisplay::handleKeyEvent(QicsScreenGrid * grid, int, int, QKeyEvent *ke)
{
    QWidget *widget = getInfoFromGrid(grid)->widget();
    if (ke->key() == Qt::Key_Space) {
        if(static_cast<QCheckBox*>(widget)->checkState() == Qt::Checked)
            static_cast<QCheckBox*>(widget)->setCheckState(Qt::Unchecked);
        else
            if (isTristate() && static_cast<QCheckBox*>(widget)->checkState() == Qt::Unchecked)
                static_cast<QCheckBox*>(widget)->setCheckState(Qt::PartiallyChecked);
            else
                static_cast<QCheckBox*>(widget)->setCheckState(Qt::Checked);
    }
    return true;
}

QString QicsCheckCellDisplay::cellDisplayName() const
{
    return CheckCellDisplayName;
}

QDomElement QicsCheckCellDisplay::toDomXml(const QString &tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());
    return e;
}

void QicsCheckCellDisplay::configureFromDomXml(const QDomElement &e)
{
    Q_UNUSED(e);
}

QicsCheckCellDisplayConfigurationWidget::QicsCheckCellDisplayConfigurationWidget(QWidget* parent)
    : QicsCellDisplayConfigurationWidget(parent)
{
    QLabel* label = new QLabel("No Configuration Options");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);

    setLayout(layout);
}

void QicsCheckCellDisplayConfigurationWidget::readConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

void QicsCheckCellDisplayConfigurationWidget::writeConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

QicsCellDisplay *QicsCheckCellDisplayFactory::createCellDisplay()
{
    return new QicsCheckCellDisplay;
}

QicsCellDisplayConfigurationWidget *QicsCheckCellDisplayFactory::configurationWidget()
{
    return new QicsCheckCellDisplayConfigurationWidget();
}


