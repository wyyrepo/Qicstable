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

#include "QicsDateTimeCellDisplay.h"

#include <QApplication>
#include <QMouseEvent>
#include <QListView>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>

#include "QicsCell.h"
#include "QicsScreenGrid.h"
#include "QicsUtil.h"
#include "QicsMainGrid.h"
#include "QicsCellDisplay_p.h"


const QString QicsDateTimeCellDisplay::DateTimeCellDisplayName = "DateTimeCellDisplay";


QicsDateTimeCellDisplay::QicsDateTimeCellDisplay( QWidget * parent )
    : QDateTimeEdit( parent ), QicsMovableEntryWidgetCellDisplay()
{
    myBlockSignals = false;

    setDateTime(QDateTime::currentDateTime());

    connect(this,SIGNAL(dateTimeChanged(const QDateTime&)),
        this,SLOT(setValue(const QDateTime&)));

    connect(this,SIGNAL(dateChanged(const QDate&)),
        this,SLOT(setValue(const QDate&)));

    connect(this,SIGNAL(timeChanged(const QTime&)),
        this,SLOT(setValue(const QTime&)));

    static_cast<QDateTimeEdit*>(this)->installEventFilter(this);
    hide();
}

QicsDateTimeCellDisplay::~QicsDateTimeCellDisplay()
{
}

QWidget *QicsDateTimeCellDisplay::newEntryWidget(QicsScreenGrid *)
{
    return (static_cast<QDateTimeEdit *>(this));
}

void QicsDateTimeCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                     const QicsDataItem *itm,
                                     QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, this, true, false);

    const int flags = myCell->alignment() | myCell->textFlags() | Qt::AlignVCenter;

    // draw the date time display
    QDateTimeEdit *dt = static_cast<QDateTimeEdit *>(this);

    // Finally, we get to draw.
    if (hasFrame()) {
        // polish the style... for #1281
        if (d->the_style->metaObject()->className() == QString("QWindowsStyle")) {
            painter->setPen(d->pal.mid().color());
            painter->drawLine(rect.right()-1, rect.top()+1, rect.right()-1, rect.bottom()-1);
            painter->drawLine(rect.left()+1, rect.bottom()-1, rect.right()-1, rect.bottom()-1);
        }
    }

    if (!dt->calendarPopup()) {
        painter->save();
        painter->translate(d->cr.left(), d->cr.top());

        QStyleOptionSpinBox opt;
        opt.frame = hasFrame();
        opt.buttonSymbols = buttonSymbols();
        opt.rect = QRect(0,0,rect.width(),rect.height());
        opt.palette = d->pal;
        opt.state = d->style_flags;
        opt.subControls = QStyle::SC_All;
        opt.activeSubControls = QStyle::SC_None;
        opt.stepEnabled = QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;

        dt->setPalette( d->pal );
        d->the_style->drawComplexControl(QStyle::CC_SpinBox, &opt, painter/*, dt*/);

        QRect trect = d->the_style->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxEditField/*, dt*/);

        d->pal.setColor(QPalette::Normal, QPalette::Text, d->fg);
        d->the_style->drawItemText(painter, trect, flags, d->pal, d->is_enabled, d->qs, QPalette::Text);

        painter->restore();
    }
    else {
        if (d->is_selected && !d->is_current)
            d->pal.setColor(QPalette::Base, d->pal.highlight().color());

        QStyleOptionComboBox ccOptions;
        ccOptions.rect = rect;
        ccOptions.palette = d->pal;
        ccOptions.state = d->style_flags;
        ccOptions.subControls = hasFrame() ? QStyle::SC_All :
            QStyle::SC_ComboBoxArrow | QStyle::SC_ComboBoxListBoxPopup;
        ccOptions.activeSubControls = QStyle::SC_None;

        d->the_style->drawComplexControl(QStyle::CC_ComboBox, &ccOptions, painter/*, dt*/);

        QRect trect = d->the_style->subControlRect(QStyle::CC_ComboBox, &ccOptions, QStyle::SC_ComboBoxEditField/*, dt*/);

        if (d->is_selected && !d->is_current)
            d->pal.setColor(QPalette::Text, d->pal.highlightedText().color());

        d->the_style->drawItemText(painter, trect, flags, d->pal, d->is_enabled, d->qs, QPalette::Text);
    }
}

void QicsDateTimeCellDisplay::startEdit(QicsScreenGrid *grid, int row, int col,
                                   const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QDateTimeEdit *widget = static_cast<QDateTimeEdit *> (info->widget());

    widget->setParent(grid);

    info->setRow(ginfo->modelRowIndex(row));
    info->setColumn(ginfo->modelColumnIndex(col));

    // Setup widget (colors, fonts, values etc) for this cell
    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    widget->setFont(myCell->font());
    widget->setPalette(myCell->palette());
    widget->setAlignment(static_cast<Qt::Alignment>(myCell->alignment()));

    widget->setFocus();

    if (itm && itm->type() == QicsDataItem_DateTime) {
        bool blocked = blockSignals(true);
        widget->setDateTime(static_cast<const QicsDataDateTime*>(itm)->data());
        blockSignals(blocked);
    }
    else if (itm && itm->type() == QicsDataItem_Date) {
        bool blocked = blockSignals(true);
        widget->setDate(static_cast<const QicsDataDate*>(itm)->data());
        blockSignals(blocked);
    }
    else if (itm && itm->type() == QicsDataItem_Time) {
        bool blocked = blockSignals(true);
        widget->setTime(static_cast<const QicsDataTime*>(itm)->data());
        blockSignals(blocked);
    }
    else {
        bool blocked = blockSignals(true);
        widget->setDateTime(QDateTime::currentDateTime());
        blockSignals(blocked);
    }

    QicsCellDisplay::startEdit(grid,row,col,itm);
}

bool QicsDateTimeCellDisplay::handleMouseEvent(QicsScreenGrid *grid, int row, int col,
                                          QMouseEvent *me)
{
    myCell->setInfo(&grid->gridInfo());
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    if (!myCell->enabled())
        return false;

    QicsICell cur_cell = grid->currentCell();

    if (( !cur_cell.isValid() || ((cur_cell.row() != row) || (cur_cell.column() != col))) &&
        me->type() == QEvent::MouseButtonPress) {
        grid->traverseToCell(row, col,false);
    }

    QRect r = grid->cellDimensions(row, col, false);

    QPoint p(me->x() - r.left() - myCell -> borderWidth() - myCell -> margin(),
        me->y() - r.top() - myCell -> borderWidth() - myCell -> margin());

    QMouseEvent new_event(me->type(), p, me->button(), me->buttons(), me->modifiers());

    QApplication::sendEvent(lineEdit(), &new_event);
    return false;
}

// This is a hack to catch Tab and Backtab keys
bool QicsDateTimeCellDisplay::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if(ke->key() == Qt::Key_Tab || ke->key() == Qt::Key_Backtab) {
            QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *> (this->parent());
            if (!grid) return false;

            endEdit(grid, 0,0);
            grid->handleTraversalKeys(ke);
            return true;
        }
    }

    return false;
}

void QicsDateTimeCellDisplay::keyPressEvent(QKeyEvent *ke)
{
    QDateTimeEdit::keyPressEvent(ke);
}

void QicsDateTimeCellDisplay::contextMenuEvent ( QContextMenuEvent * event )
{
    myBlockSignals = true;
    QDateTimeEdit::contextMenuEvent(event);
    myBlockSignals = false;
}

QSize QicsDateTimeCellDisplay::sizeHint(QicsGrid *grid, int row, int col,
                                  const QicsDataItem *)
{
    QDateTimeEdit *dt = static_cast<QDateTimeEdit *>(this);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    dt->setFont(myCell->font());

    return (dt->sizeHint());
}

QString QicsDateTimeCellDisplay::textToDisplay(QicsGridInfo *ginfo, int row, int col,
                                       const QicsDataItem *itm) const
{
    Q_UNUSED(ginfo);
    Q_UNUSED(row);
    Q_UNUSED(col);

    QString qs;

    if (itm) {
        if (itm->type() == QicsDataItem_DateTime) {
            const QicsDataDateTime* dt = static_cast<const QicsDataDateTime*>(itm);
            qs = dt->data().toString(displayFormat());
        }
        else if (itm->type() == QicsDataItem_Date) {
            const QicsDataDate* dt = static_cast<const QicsDataDate*>(itm);
            qs = dt->data().toString(displayFormat());
        }
        else if (itm->type() == QicsDataItem_Time) {
            QDateTime dt = QDateTime::currentDateTime();
            QTime t = static_cast<const QicsDataTime*>(itm)->data();
            dt.setTime(t);
            qs = dt.toString(displayFormat());
        }
    }

    return (qs);
}

void QicsDateTimeCellDisplay::setValue(const QDateTime &val)
{
    QicsEntryWidgetInfo *info = getInfoFromEntry(this);
    QicsDataDateTime itm(val);
    QicsGridInfo* ginfo  = &(info->grid()->gridInfo());
    ginfo->setCurrentCellValue(itm);
}

void QicsDateTimeCellDisplay::setValue(const QDate &val)
{
    QicsEntryWidgetInfo *info = getInfoFromEntry(this);
    QicsGridInfo* ginfo  = &(info->grid()->gridInfo());
    const QicsDataItem* itm = ginfo->cellValue(ginfo->currentCell().row(),ginfo->currentCell().column());

    if(itm && itm->type()== QicsDataItem_DateTime) {
        QDateTime dt = static_cast<const QicsDataDateTime*>(itm)->data();
        dt.setDate(val);
        QicsDataDateTime ddt(dt);
        info->grid()->gridInfo().setCurrentCellValue(ddt);
    }
    else {
        QicsDataDateTime ddt(val,QTime());
        info->grid()->gridInfo().setCurrentCellValue(ddt);
    }
}

void QicsDateTimeCellDisplay::setValue(const QTime &val)
{
    QicsEntryWidgetInfo *info = getInfoFromEntry(this);
    QicsGridInfo* ginfo  = &(info->grid()->gridInfo());
    const QicsDataItem* itm = ginfo->cellValue(ginfo->currentCell().row(),ginfo->currentCell().column());

    if(itm && itm->type()== QicsDataItem_DateTime) {
        QDateTime dt = static_cast<const QicsDataDateTime*>(itm)->data();
        dt.setTime(val);
        QicsDataDateTime ddt(dt);
        ginfo->setCurrentCellValue(ddt);
    }
    else {
        QicsDataDateTime ddt(QDate::currentDate(),val);
        ginfo->setCurrentCellValue(ddt);
    }
}

void QicsDateTimeCellDisplay::focusOutEvent(QFocusEvent* e)
{
    myLastFocusReason = e->reason();
    QicsScreenGrid* grid = qobject_cast<QicsScreenGrid*>(parent());

    if (grid && !myBlockSignals)
        grid->uneditCurrentCell();

    QDateTimeEdit::focusOutEvent(e);
}

bool QicsDateTimeCellDisplay::handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *ke)
{
    QDateTimeEdit::keyPressEvent(ke);
    return true;
}

QString QicsDateTimeCellDisplay::cellDisplayName() const
{
    return DateTimeCellDisplayName;
}

QDomElement QicsDateTimeCellDisplay::toDomXml(const QString &tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());
    e.setAttribute("displayFormat",displayFormat());
    e.setAttribute("minimumDate",minimumDate().toString());
    e.setAttribute("maximumDate",maximumDate().toString());
    e.setAttribute("minimumTime",minimumTime().toString());
    e.setAttribute("maximumTime",maximumTime().toString());
    return e;
}

void QicsDateTimeCellDisplay::configureFromDomXml(const QDomElement& e)
{
    setDisplayFormat(e.attribute("displayFormat"));
    setMinimumDate(QDate::fromString(e.attribute("minimumDate")));
    setMaximumDate(QDate::fromString(e.attribute("maximumDate")));
    setMinimumTime(QTime::fromString(e.attribute("minimumTime")));
    setMaximumTime(QTime::fromString(e.attribute("maximumTime")));
}


QicsDateTimeCellDisplayConfigurationWidget::QicsDateTimeCellDisplayConfigurationWidget(QWidget* parent)
    : QicsCellDisplayConfigurationWidget(parent)
{
    QLabel* label = new QLabel("No Configuration Options");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);

    setLayout(layout);
}

void QicsDateTimeCellDisplayConfigurationWidget::readConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

void QicsDateTimeCellDisplayConfigurationWidget::writeConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

QicsCellDisplay *QicsDateTimeCellDisplayFactory::createCellDisplay()
{
    return new QicsDateTimeCellDisplay;
}

QicsCellDisplayConfigurationWidget *QicsDateTimeCellDisplayFactory::configurationWidget()
{
    return new QicsDateTimeCellDisplayConfigurationWidget();
}


