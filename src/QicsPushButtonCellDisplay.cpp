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

#include "QicsPushButtonCellDisplay.h"

#include <QStyleOptionButton>
#include <QApplication>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>

#include "QicsCell.h"
#include "QicsScreenGrid.h"
#include "QicsUtil.h"
#include "QicsCellDisplay_p.h"

#define QICS_PUSHBUTTON_SPACING 3
const QString QicsPushButtonCellDisplay::PushButtonCellDisplayName = "PushButtonCellDisplay";


////////////////////////////////////////////////////////////////////
////////////      QicsPushButtonCellDisplay Methods        ////////////////
////////////////////////////////////////////////////////////////////

QicsPushButtonCellDisplay::QicsPushButtonCellDisplay( QWidget * parent )
    : QPushButton( parent ), QicsMovableEntryWidgetCellDisplay()
{
    celldisplay_init();
}

QicsPushButtonCellDisplay::QicsPushButtonCellDisplay( const QString & text, QWidget * parent )
    : QPushButton( text, parent ), QicsMovableEntryWidgetCellDisplay()
{
    celldisplay_init();
}

void QicsPushButtonCellDisplay::celldisplay_init()
{
    static_cast<QPushButton*>(this)->installEventFilter(this);

    connect(this,SIGNAL(pressed()),this,SLOT(setCheckState()));
    connect(this,SIGNAL(released()),this,SLOT(setCheckState()));
    connect(this,SIGNAL(clicked()),this,SLOT(handleMouseClick()));
    hide();
}

QicsPushButtonCellDisplay::~QicsPushButtonCellDisplay()
{
}

QWidget *QicsPushButtonCellDisplay::newEntryWidget(QicsScreenGrid * grid)
{
    Q_UNUSED(grid);
    QPushButton* pb = static_cast<QPushButton *> (this);
    return pb;
}

void QicsPushButtonCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                       const QicsDataItem *itm,
                                       QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, this, true, true);

    // reparent
    //if (!for_printer && parent() != static_cast<QicsScreenGrid*>(grid))   // #### TODO: Remove these lines
    //    setParent(static_cast<QicsScreenGrid*>(grid));

    // the stuff to display
    bool ch = isChecked(d->ginfo,row,col,itm);
    d->style_flags |= (isCheckable() && ch) ? QStyle::State_On : QStyle::State_Off;
    d->style_flags |= (!isCheckable() && ch) ? QStyle::State_Sunken : QStyle::State_Raised;

    QStyleOptionButton styleOps;

    if (isFlat())
        styleOps.features |= QStyleOptionButton::Flat;

    if (isDefault())
        styleOps.features |= QStyleOptionButton::DefaultButton;

    styleOps.rect = rect;
    styleOps.state = d->style_flags;
    styleOps.palette = d->pal;

    QPixmap pixmap = pixmapToDisplay(d->ginfo, row, col, itm);
    styleOps.icon = QIcon(pixmap);

    d->the_style->drawControl(QStyle::CE_PushButton,
        &styleOps, painter/*, static_cast<QPushButton*>(this)*/);

    QRect contents_rect = d->the_style->subElementRect(
        QStyle::SE_PushButtonContents, &styleOps/*,static_cast<QPushButton*>(this)*/);

    // #### TODO: This needs more work for RightToLeft layout direction

    QRect text_rect;
    int alignment;

    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(static_cast<QPushButton*>(this)->iconSize().width(),
            static_cast<QPushButton*>(this)->iconSize().height());

        d->the_style->drawItemPixmap(painter, contents_rect, Qt::AlignVCenter, pixmap);

        text_rect = QRect(contents_rect.x() + pixmap.width() + QICS_PUSHBUTTON_SPACING,
            contents_rect.y(),
            contents_rect.width() - pixmap.width() - QICS_PUSHBUTTON_SPACING,
            contents_rect.height());
        alignment = myCell->alignment() | Qt::AlignVCenter;
    }
    else {
        text_rect = contents_rect;
        alignment = Qt::AlignCenter;
    }

    d->the_style->drawItemText(painter, text_rect, alignment, d->pal, d->is_enabled, d->qs, QPalette::WindowText);
}

void QicsPushButtonCellDisplay::startEdit(QicsScreenGrid *grid, int row, int col,
                                     const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QPushButton *widget = static_cast<QPushButton*> (info->widget());
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
    p.setColor(QPalette::Inactive, QPalette::Background, p.color(QPalette::Inactive, QPalette::Base) );
    p.setColor(QPalette::Disabled, QPalette::Background, p.color(QPalette::Disabled, QPalette::Base) );

    widget->setPalette(p);

    widget->setText(textToDisplay(ginfo, row, col, itm));
    QPixmap pixmap_to_display(pixmapToDisplay(ginfo, row, col, itm));
    if (!pixmap_to_display.isNull())
        widget->setIcon(pixmap_to_display);

    if (widget->isCheckable()) {
        bool blocked = widget->blockSignals(true);
        widget->setChecked(isChecked(ginfo, row, col, itm));
        widget->blockSignals(blocked);
    }
}

void QicsPushButtonCellDisplay::focusOutEvent(QFocusEvent* fe)
{
    myLastFocusReason = fe->reason();

    QicsScreenGrid* grid = qobject_cast<QicsScreenGrid*>(parent());
    if (grid)
        grid->uneditCurrentCell();

    QPushButton::focusOutEvent(fe);
}

bool QicsPushButtonCellDisplay::handleMouseEvent(QicsScreenGrid *grid, int row, int col,
                                            QMouseEvent *me)
{
    myCell->setInfo(&grid->gridInfo());
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    if (!myCell->enabled())
        return false;

    QicsICell cur_cell = grid->currentCell();

    if (( !cur_cell.isValid() ||
        ((cur_cell.row() != row) || (cur_cell.column() != col))) &&
        me->type() == QEvent::MouseButtonPress) {
        grid->traverseToCell(row, col,false);
    }

    QRect r = grid->cellDimensions(row, col, false);

    QPoint p(me->x() - r.left() - myCell -> borderWidth() - myCell -> margin(),
        me->y() - r.top() - myCell -> borderWidth() - myCell -> margin());

    QMouseEvent new_event(me->type(), p, me->button(), me->buttons(), me->modifiers());

    return QPushButton::event(&new_event);
}

void QicsPushButtonCellDisplay::handleMouseClick()
{
    emit clicked(myCell->rowIndex(), myCell->columnIndex());
}

void QicsPushButtonCellDisplay::keyPressEvent(QKeyEvent *ke)
{
    QPushButton::keyPressEvent(ke);
}

bool QicsPushButtonCellDisplay::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        // special processing for key press
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        QicsScreenGrid* grid = qobject_cast<QicsScreenGrid*>(parent());
        if(grid)
            return grid->handleTraversalKeys(ke);
    }
    return false;
}

QSize QicsPushButtonCellDisplay::sizeHint(QicsGrid *grid, int row, int col,
                                    const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QStyle *style = QApplication::style();

    QString text = textToDisplay(ginfo, row, col, itm);

    int xoffset = 0;

    QSize sz = QSize(style->pixelMetric(QStyle::PM_IndicatorWidth),
        style->pixelMetric(QStyle::PM_IndicatorHeight));

    // spacing between indicator and text
    sz.setWidth(sz.width() + QICS_PUSHBUTTON_SPACING);

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

//////////////// Subclassable methods //////////////////////

QString QicsPushButtonCellDisplay::textToDisplay(QicsGridInfo *info, int row, int col,
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

void QicsPushButtonCellDisplay::setCheckState()
{
    bool state = (static_cast<QPushButton*>(this)->isChecked()
        || static_cast<QPushButton*>(this)->isDown());

    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(parent());
    if (!grid) return;

    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(ginfo->visualRowIndex(info->row()));
    myCell->setColumnIndex(ginfo->visualColumnIndex(info->column()));

    valueChanged(ginfo, ginfo->visualRowIndex(info->row()),
        ginfo->visualColumnIndex(info->column()), state);
}

bool QicsPushButtonCellDisplay::isChecked(QicsGridInfo *info, int row, int col, const QicsDataItem *item)
{
    Q_UNUSED(info);
    Q_UNUSED(row);
    Q_UNUSED(col);

    if (!item)
        item = new QicsDataBool(false);
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
    switch ( item->type() ) {
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
        case QicsDataItem_String: {
                tmpString = static_cast<const QicsDataString*>( item );
                if(tmpString->data() == QICS_DEFAULT_TRUE_STRING)
                    return true;
                else
                    return false;
            }
        case QicsDataItem_Time:
            tmpTime = static_cast<const QicsDataTime*>( item );
            return !tmpTime->data().isNull();
    }
    return false;
}

void QicsPushButtonCellDisplay::valueChanged(QicsGridInfo *info, int vis_row, int vis_col, bool set)
{
    const QicsDataItem * item = info->cellValue(vis_row, vis_col);
    if(item == 0)
        item = new QicsDataBool(false);

    switch ( item->type() )
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
        if(set)
            info->setCellValue(vis_row,vis_col,QicsDataString(QICS_DEFAULT_TRUE_STRING));
        else
            info->setCellValue(vis_row,vis_col,QicsDataString(QICS_DEFAULT_FALSE_STRING));
        break;
    case QicsDataItem_Time:
        if(set)
            info->setCellValue(vis_row,vis_col,QicsDataTime(QTime(0,0)));
        else
            info->setCellValue(vis_row,vis_col,QicsDataTime());
        break;
    case QicsDataItem_Date:
        if(set)
            info->setCellValue(vis_row,vis_col,QicsDataDate(0,0,0));
        else
            info->setCellValue(vis_row,vis_col,QicsDataDate());
        break;
    case QicsDataItem_DateTime:
        if(set)
            info->setCellValue(vis_row,vis_col,QicsDataDateTime(QDate(0,0,0),QTime(0,0,0,0)));
        else
            info->setCellValue(vis_row,vis_col,QicsDataDateTime());
        break;
    }
}

bool QicsPushButtonCellDisplay::handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *ke)
{
    QPushButton::keyPressEvent(ke);
    return true;
}

QString QicsPushButtonCellDisplay::cellDisplayName() const
{
    return PushButtonCellDisplayName;
}
QDomElement QicsPushButtonCellDisplay::toDomXml(const QString &tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());
    return e;
}

void QicsPushButtonCellDisplay::configureFromDomXml(const QDomElement &e)
{
    Q_UNUSED(e);
}

QicsPushButtonCellDisplayConfigurationWidget::QicsPushButtonCellDisplayConfigurationWidget(QWidget* parent)
    : QicsCellDisplayConfigurationWidget(parent)
{
    QLabel* label = new QLabel("No Configuration Options");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);

    setLayout(layout);
}

void QicsPushButtonCellDisplayConfigurationWidget::readConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

void QicsPushButtonCellDisplayConfigurationWidget::writeConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

QicsCellDisplay *QicsPushButtonCellDisplayFactory::createCellDisplay()
{
    return new QicsPushButtonCellDisplay;
}

QicsCellDisplayConfigurationWidget *QicsPushButtonCellDisplayFactory::configurationWidget()
{
    return new QicsPushButtonCellDisplayConfigurationWidget();
}


