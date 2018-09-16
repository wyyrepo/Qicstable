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

#include "QicsTextCellDisplay.h"

#include <QApplication>
#include <QVariant>
#include <QPainter>
#include <QPixmap>
#include <QStyle>
#include <QValidator>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QEvent>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStyleOptionButton>
#include <QString>
#include "QicsStyleManager.h"
#include "QicsDataItem.h"
#include "QicsScreenGrid.h"
#include "QicsDataItemFormatter.h"
#include "QicsUtil.h"
#include "QicsCell.h"
#include "QicsMainGrid.h"
#include "QicsTextCellDisplayHelpers.h"
#include "QicsCellDisplay_p.h"


const QString QicsTextCellDisplay::TextCellDisplayName = "TextCellDisplay";
QicsTextCellDisplayData *QicsTextCellDisplay::td;

//////////////////////////////////////////////////////////////////////
////////////      QicsTextCellDisplay Methods         ////////////////
//////////////////////////////////////////////////////////////////////

QicsTextCellDisplay::QicsTextCellDisplay(QWidget *parent)
    : QObject(parent),
    QicsMovableEntryWidgetCellDisplay()
{
    m_lastEvent = 0;
    m_widget = 0;
    m_editWhenCurrent = false;
    myLastFocusReason = static_cast<Qt::FocusReason>(-1);
    m_contentScrollEnabled = false;
    m_iconShiftsText = true;
        m_echomode = QLineEdit::Normal;

    if (!td) {
        td = new QicsTextCellDisplayData();
        td->lineEdit = new QLineEdit();
        td->lineEdit->blockSignals(true);
        td->lineEdit->setUpdatesEnabled(false);
    }
    else
        td->ref();
}

QicsTextCellDisplay::~QicsTextCellDisplay()
{
    if (td && td->deref()) {
        delete td->lineEdit;
        delete td;
        td = 0;
    }
    delete m_widget;
}

void QicsTextCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                      const QicsDataItem *itm,
                                      QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, 0, true, true);

    int cell_alignment = myCell->alignment();
    int flags = cell_alignment | myCell->textFlags();

    // The Grid Properties
    myGrid->setInfo(d->ginfo);

    QicsGridCellClipping clipping = myGrid->gridCellClipping();

    //
    // Get the pixmap and text(if any) to display
    //
    QPixmap pix = pixmapToDisplay(d->ginfo, row, col, itm);

    //
    // Figure out all the dimensions
    //
    int pix_width = (pix.isNull() ? 0 : pix.width() + myCell->pixmapSpacing());

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

    // respect the QLineEdit echo mode
    if (!m_contentScrollEnabled) {
        td->lineEdit->setText(d->qs);
        td->lineEdit->setEchoMode(m_echomode);
        td->lineEdit->setInputMask(m_inputMask);
        d->qs = td->lineEdit->displayText();
    }
    //
    // No need to try to overflow if this cell is empty.
    //
    if (d->qs.isEmpty())
        ready_to_draw = true;

    while (!ready_to_draw) {
        if (!canDisplayAll(d->ginfo, cr_full, row, col, d->qs, flags, myCell->font(), pix))
            can_display_all = false;
        // we fit, so we're ready to go
        ready_to_draw = true;
    }

    // Finally, we get to draw.

    // We only display the text and/or pixmap if the entire cell can be drawn or
    // partial cell display is allowed
    if (can_display_all || (clipping != NoDisplayOnPartial)) {
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

            if (!can_display_all && (clipping == UseClippedSymbol)) {
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

void QicsTextCellDisplay::moveEdit(QicsScreenGrid *grid,int row, int col,const QRect &rect)
{
    QicsEntryWidgetInfo* entry_info = getInfoFromGrid(grid);
    QicsTextCellDisplayWidget* widget = (QicsTextCellDisplayWidget*) entry_info->widget();

    QicsGridInfo *ginfo = &(grid->gridInfo());

    QRect r = entryWidgetRect(ginfo, row, col, rect);
    widget->setGeometry(widget->requiredRect(r,grid));

    widget->show();
    widget->setFocus();
}

void QicsTextCellDisplay::startEdit(QicsScreenGrid *grid, int row, int col,
                               const QicsDataItem *itm)
{
    QRect rect = grid->cellDimensions(row,col, true);
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsTextCellDisplayWidget* widget = qobject_cast<QicsTextCellDisplayWidget *>(info->widget());

    if (!isEditing()) {
        if ((grid->width() - rect.width()) < 0 && m_contentScrollEnabled)
            widget->switchState(QicsTextCellDisplayWidget::TextEdit);
        else
            widget->switchState(QicsTextCellDisplayWidget::LineEdit);
    }

    info->setRow(ginfo->modelRowIndex(row));
    info->setColumn(ginfo->modelColumnIndex(col));

    info->setItem(itm ? itm->clone() : 0);

    // Setup widget (colors, fonts, values etc) for this cell

    widget->clear();

    if (itm)
        widget->setText(itm->string());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    myEditCell->setInfo(ginfo);
    myEditCell->setRowIndex(row);
    myEditCell->setColumnIndex(col);

    // Setting alignment that is in current cell.
    widget->setAlignment(static_cast<Qt::AlignmentFlag>(myCell->alignment()));

    myGrid->setInfo(ginfo);

    QPalette pal = myCell->palette();
    QColor efg = myCell->editForegroundColor();
    QColor ebg = myCell->editBackgroundColor();
    pal.setColor(QPalette::Text, efg);
    pal.setColor(QPalette::Base, ebg);
    widget->setPalette(pal);

    widget->setFont(myCell->font());

    widget->setValidator(myCell->validator());
        widget->setInputMask(m_inputMask);
        widget->setEchoMode(m_echomode);

    widget->setMaxLength(myCell->maxLength());

    // Set the text of the entry widget to the underlying value
    QString valString;

    if (itm)
        valString = itm->string();
    else
        valString = "";

    widget->setText(valString);
    m_prevValue = valString;

    if (myGrid->autoSelectCellContents())
        widget->selectAll();

    QicsCellDisplay::startEdit(grid,row,col,itm);
}

void QicsTextCellDisplay::endEdit(QicsScreenGrid *grid, int row, int col)
{
    QicsEntryWidgetInfo *info = takeInfoFromGrid(grid);
    if (!info) return;

    QicsTextCellDisplayWidget* widget = (QicsTextCellDisplayWidget*)info->widget();
    widget->hide();

    if (myCell->autoRecognition() ) {
        // We need to use automatical
        // recognition for this cell
        QicsGridInfo *ginfo = &(grid->gridInfo());

        myCell->setInfo(ginfo);
        myCell->setRowIndex(row);
        myCell->setColumnIndex(col);
        myCell->dataValue()->string();

        QVariant v = widget->text();
        QString cap = widget->text();
        // By default we use string format of items
        QicsDataString _str(cap);
        myCell->setDataValue(_str);

        if (!cap.contains(QChar(':')) && (cap.size() < 11) &&
            (!cap.contains(QChar('.')))) {
                if (v.canConvert<QDate>() && (v.toDate().toString().size() > 0)) {
                    QicsDataDate date(v.toDate());
                    myCell->setDataValue(date);
                }
        }
        else
            if ((cap.contains(QChar(':'))) && (!cap.contains(QChar(' ')))) {
                if (v.canConvert<QTime>() && (v.toTime().toString().size() > 0)) {
                    QicsDataTime time(v.toTime());
                    myCell->setDataValue(time);
                }
            }
            else
                if ((cap.size() > 12) && (!cap.contains(QChar('.')))) {
                    if (v.canConvert<QDateTime>() &&
                        (v.toDateTime().toString().size() > 0)) {
                            QicsDataDateTime time(v.toDateTime());
                            myCell->setDataValue(time);
                    }
                }
                else
                    if (cap.toFloat() != 0) {
                        QicsDataFloat _data(cap.toFloat());
                        myCell->setDataValue(_data);
                    }
        if (cap.toInt() != 0) {
            QicsDataInt _data(cap.toInt());
            myCell->setDataValue(_data);
        }
        if (cap == "0") {
            QicsDataInt _data(0);
            myCell->setDataValue(_data);
        }
    }

    // We have to call this to prevent double startEdit() calling
    // through QicsTable::modelToVisualValueChanged() slot.
    QicsCellDisplay::endEdit(grid,row,col);
    setValue(info);
    delete info;

    // HACK: Application has two tables in separate windows -
    // then starting editing a cell in one of the tables, and clicking on the
    // title bar of the other dialog
    // It happens because after endEdit focus set to parent
    // Problem reproducing only on windows - bug 853
    QTimer::singleShot(0, widget, SLOT(close()));
    m_widget = 0;
    m_prevValue.clear();
}

bool QicsTextCellDisplay::handleKeyEvent(QicsScreenGrid *grid, int row, int col,
                                    QKeyEvent *ke)
{
    if (m_lastEvent == ke)
        return false;

    if((editWhenCurrent() && (ke->key() == Qt::Key_Tab)) || ke->key() == Qt::Key_Backtab)
        return false;

    myCell->setInfo(&grid->gridInfo());
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    if (!myCell->enabled() || myCell->readOnly())
        return false;

    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);

    QicsTextCellDisplayWidget* widget = (QicsTextCellDisplayWidget*)info->widget();
    if(widget)
        return widget->sendEvent(ke);
    else
        return false;
}

bool QicsTextCellDisplay::handleMouseEvent(QicsScreenGrid *grid, int row, int col,
                                            QMouseEvent *me)
{
    if (!grid->isMainGrid())
        return false;

    myCell->setInfo(&grid->gridInfo());
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    if (!myCell->enabled())
        return false;

    QicsICell cur_cell = grid->currentCell();

    if ((!cur_cell.isValid() || ((cur_cell.row() != row) || (cur_cell.column() != col))) &&
        me->type() == QEvent::MouseButtonPress) {
        grid->traverseToCell(row, col,false);
    }

    return false;
}

QSize QicsTextCellDisplay::sizeHint(QicsGrid *grid, int row, int col, const QicsDataItem *itm)
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

bool QicsTextCellDisplay::isEmpty(QicsGridInfo *info, int row, int col,
                             const QicsDataItem *itm) const
{
    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QPixmap pix = pixmapToDisplay(info, row, col, itm);
    QString text = textToDisplay(info, row, col, itm);

    return (pix.isNull() && text.isEmpty());
}

QString QicsTextCellDisplay::tooltipText(QicsGridInfo *info, int row, int col,
                                 const QicsDataItem *itm, const QRect &rect) const
{
    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QPixmap pix = pixmapToDisplay(info, row, col, itm);
    QString text = textToDisplay(info, row, col, itm);

    if (!canDisplayAll(info, rect, row, col, text,
        (myCell->alignment() | myCell->textFlags()),
        myCell->font(), pix))
        return text;

    return QString();
}

QWidget *QicsTextCellDisplay::newEntryWidget(QicsScreenGrid *grid)
{
    m_widget = new QicsTextCellDisplayWidget(grid);

    m_widget->installFilters(this);

    connect(m_widget, SIGNAL(lostFocus()), grid, SIGNAL(uneditRequest()));
    connect(m_widget, SIGNAL(focusReasonChanged(Qt::FocusReason)),
        this, SLOT(setLastFocusReason(Qt::FocusReason)));

    return m_widget;
}

QRect QicsTextCellDisplay::contentRect(QicsGrid *grid, int row, int col,
                                 const QicsDataItem *itm,
                                 QRect &rect, QPainter *painter)
{
    Q_UNUSED(painter);
    // init statics
    d->ginfo = &(grid->gridInfo());
    myGrid->setInfo(d->ginfo);

    myCell->setInfo(d->ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    int cell_alignment = myCell->alignment();
    int flags = cell_alignment | myCell->textFlags();
    QPixmap pix = pixmapToDisplay(d->ginfo, row, col, itm);
    d->qs = textToDisplay(d->ginfo, row, col, itm);

    int pix_width = 0;

    // get the pixmap dimensions
    if (!pix.isNull())
        pix_width = pix.width() + myCell->pixmapSpacing();

    QRect cr = displayableCellArea(d->ginfo, row, col, rect);

    // the area left for the string
    QRect str_rect;
    if (m_iconShiftsText)
        str_rect = QRect(QPoint(cr.left() + pix_width, cr.top()), cr.bottomRight());
    else
        str_rect = cr;

    QFontMetrics fm(myCell->font());

    QRect br = fm.boundingRect(str_rect.left(), str_rect.top(),
        str_rect.width(), str_rect.height(),
        flags, d->qs);

    if (pix_width)
        br.setWidth(br.width() + pix_width);

    return br;
}

bool QicsTextCellDisplay::canDisplayAll(QicsGridInfo *info,
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

bool QicsTextCellDisplay::eventFilter(QObject *obj, QEvent *evnt)
{
    static int escape_counter = 0;
    QicsEntryWidgetInfo *info = getInfoFromEntry(qobject_cast<QWidget*>(obj)->parentWidget());

    if(!info)
        return false;

    QicsScreenGrid* grid = info->grid();
    QicsTextCellDisplayWidget* widget = static_cast<QicsTextCellDisplayWidget*>(info->widget());
    QicsTextCellDisplayWidget::WidgetType t = widget->currentState();

    bool eatEvent = false;

    if (evnt->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *> (evnt);

        //if ((k->key() == Qt::Key_Return || k->key() == Qt::Key_Enter) && (k->modifiers() == Qt::AltModifier)) {
        // #### TODO: #962
        //}

        myGrid->setInfo(&(grid->gridInfo()));
        if (myGrid->traversalKeyEditBehavior() == Qics::TraverseBetweenCells ||
            ke->key() == Qt::Key_Tab ||
            ke->key() == Qt::Key_Backtab) {
            // special processing for key press
            eatEvent = grid->handleTraversalKeys(ke);
        }
        // Handling for QicsTextEdit.
        if((t == QicsTextCellDisplayWidget::TextEdit) &&
            (ke->key() == Qt::Key_Enter ||
            ke->key() == Qt::Key_Return ||
            ke->key() == Qt::Key_Up ||
            ke->key() == Qt::Key_Down))
            eatEvent = grid->handleTraversalKeys(ke);

        if (!eatEvent) {
            switch (ke->key())
            {
            case Qt::Key_Escape:
                if (++escape_counter == 2) {
                    grid->uneditCurrentCell();
                    escape_counter = 0;
                }
                else
                    resetValue(info);
                eatEvent = true;
                break;
            default:
                escape_counter = 0;
                break;
            }
        }
    }
    else if (evnt->type() == QEvent::FocusOut)
        escape_counter = 0;

    m_lastEvent = evnt;

    if (eatEvent)
        return true;

    return QObject::eventFilter(obj, evnt);
}

bool QicsTextCellDisplay::setValue(QicsEntryWidgetInfo *info)
{
    if (!info)
        return false;

    QicsScreenGrid *grid = info->grid();
    QicsTextCellDisplayWidget* widget = qobject_cast<QicsTextCellDisplayWidget *>(info->widget());

    QString text = widget->text();

    // If the value hasn't changed, return false.
    if ((info->item() && info->item()->string() == text))
        return false;

    int pos = 0;
    bool valid = true;

    if (widget->validator() && (!widget->validator()->validate(text, pos)))
        valid = false;

    if (!valid)
        return false;

    QicsDataItem *item;
    bool allocated = false;

    if (info->item() && info->item()->setString(text))
        item = info->item();
    else {
        // Trying to figure out what kind of data item  this is?
        item = QicsDataItem::fromString(text);
        allocated = true;
    }

    //grid->gridInfo().setCurrentCellValue(*item);
    grid->gridInfo().dataModel()->setItem(info->row(), info->column(), *item);

    if (allocated)
        delete item;

    return true;

}

void QicsTextCellDisplay::resetValue(QicsEntryWidgetInfo *info)
{
    if (!info)
        return;

    QicsTextCellDisplayWidget* widget = (QicsTextCellDisplayWidget*)info->widget();

    widget->setText( m_prevValue );

    QicsStyleManager *sm = info->grid()->gridInfo().styleManager();
    const bool auto_select = * static_cast<bool *>
        (sm->getGridProperty(QicsGridStyle::AutoSelectCellContents));

    if (auto_select)
        widget->selectAll();
}

QString QicsTextCellDisplay::cellDisplayName() const
{
    return TextCellDisplayName;
}

QWidget *QicsTextCellDisplay::widget()
{
    return m_widget;
}

QDomElement QicsTextCellDisplay::toDomXml(const QString &tag, QDomDocument *doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());
    e.setAttribute("editWhenCurrent", QString::number(static_cast<int>(editWhenCurrent())));
    return e;
}

void QicsTextCellDisplay::configureFromDomXml(const QDomElement &e)
{
    setEditWhenCurrent(static_cast<bool>(e.attribute("editWhenCurrent").toInt()));
}

void QicsTextCellDisplay::handleTextChanged(const QString &str)
{
    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(m_widget->parent());
    if (!grid)
        return;

    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);

    // We have to block signals to prevent double startEdit() calling
    // through QicsTable::modelToVisualValueChanged() slot.
    bool blocked = ginfo->blockSignals(true);
    setValue(info);//TODO: leads to the behaviour in #90083
    ginfo->blockSignals(blocked);

    emit textEdited(myEditCell->rowIndex(), myEditCell->columnIndex(), str);
}


QicsTextCellDisplayConfigurationWidget::QicsTextCellDisplayConfigurationWidget(QWidget *parent)
    : QicsCellDisplayConfigurationWidget(parent)
{
    QLabel *label = new QLabel(tr("No Configuration Options"));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);

    setLayout(layout);
}

void QicsTextCellDisplayConfigurationWidget::readConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

void QicsTextCellDisplayConfigurationWidget::writeConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

QicsCellDisplay *QicsTextCellDisplayFactory::createCellDisplay()
{
    return new QicsTextCellDisplay();
}

QicsCellDisplayConfigurationWidget *QicsTextCellDisplayFactory::configurationWidget()
{
    return new QicsTextCellDisplayConfigurationWidget();
}


