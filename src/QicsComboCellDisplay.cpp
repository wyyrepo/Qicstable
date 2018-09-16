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

#include "QicsComboCellDisplay.h"

#include <QApplication>
#include <QMouseEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>

#include "QicsCell.h"
#include "QicsScreenGrid.h"
#include "QicsSelectionManager.h"
#include "QicsUtil.h"
#include "QicsDataItemFormatter.h"
#include "QicsMainGrid.h"
#include "QicsCellDisplay_p.h"


const QString QicsComboCellDisplay::ComboCellDisplayName = "ComboCellDisplay";


QicsComboCellDisplay::QicsComboCellDisplay( QWidget * parent )
    : QComboBox( parent ), QicsMovableEntryWidgetCellDisplay()
{
    myAddValueToList = true;

    connect(this, SIGNAL(activated(const QString &)), SLOT(itemSelected(const QString &)));

    QComboBox *box = static_cast<QComboBox*>(this);
    box->installEventFilter(this);
    static_cast<QListView*>(box->view())->installEventFilter(this);
    hide();
}

QicsComboCellDisplay::~QicsComboCellDisplay()
{
}

void QicsComboCellDisplay::setCurrentIndex(int index)
{
    if (index < 0) return;

    QComboBox * cb = static_cast<QComboBox *>(this);
    cb->setCurrentIndex(index);
    itemSelected(itemText(index));
}

void QicsComboCellDisplay::setCurrentText( const QString& val)
{
    setCurrentIndex(findText(val));
}

void QicsComboCellDisplay::setEditable( bool editable )
{
    QComboBox::setEditable( editable );
    //setAddValueToList( !editable );
}

QWidget *QicsComboCellDisplay::newEntryWidget(QicsScreenGrid *)
{
    return (static_cast<QComboBox *>(this));
}

void QicsComboCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                  const QicsDataItem *itm,
                                  QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, this, true, false);

    int flags = myCell->alignment() | myCell->textFlags();

    // draw the combo box
    QIcon ico;

    // deside what text to display
    if (count()) {
        int index = findText(d->qs);
        if (index == -1 && itm) {
            bool ok;
            int i = itm->number(&ok);
            if (ok && i >= 0 && i < count())
                index = i;
        }

        if (index != -1) {
            d->qs = itemText(index);
            ico = itemIcon(index);
        }
    }

    if (d->is_selected && !d->is_current)
        d->pal.setColor(QPalette::Base, d->pal.highlight().color());

    QStyleOptionComboBox ccOptions;
    //ccOptions.rect = rect;    // ### TODO: This is workaround for #87933, RT
    ccOptions.rect = QRect(0, 0, rect.width(), rect.height());
    ccOptions.palette = d->pal;
    ccOptions.state = d->style_flags;
    ccOptions.subControls = hasFrame() ? QStyle::SC_All :
    QStyle::SC_ComboBoxArrow | QStyle::SC_ComboBoxListBoxPopup;
    ccOptions.activeSubControls = QStyle::SC_None;

    painter->save();
    painter->translate(rect.x(), rect.y());

    d->the_style->drawComplexControl(QStyle::CC_ComboBox, &ccOptions, painter);

    // ### TODO: Polish the style... for #1281, Bugzilla
    if (d->the_style->metaObject()->className() == QString("QWindowsStyle")) {
        if (hasFrame()) {
            painter->setPen(d->pal.mid().color());
            painter->drawLine(rect.right()-1, rect.top()+1, rect.right()-1, rect.bottom()-1);
            painter->drawLine(rect.left()+1, rect.bottom()-1, rect.right()-1, rect.bottom()-1);
        }
    }

    QRect trect = d->the_style->subControlRect(QStyle::CC_ComboBox,
        &ccOptions, QStyle::SC_ComboBoxEditField/*, cb*/);

    if (d->is_selected && !d->is_current)
        d->pal.setColor(QPalette::Text, d->pal.highlightedText().color());

    // draw icon
    if (!ico.isNull()) {         // ### TODO: not sure if we should handle margins here...
        trect.setLeft(trect.left() + myCell->margin());
        ico.paint(painter, trect, Qt::AlignLeft, d->is_enabled ? QIcon::Normal : QIcon::Disabled);
        QSize sz = ico.actualSize(trect.size());
        trect.setLeft(trect.left() + sz.width() + myCell->pixmapSpacing());
    }

    d->the_style->drawItemText(painter, trect, flags, d->pal, d->is_enabled, d->qs, QPalette::Text);

    painter->restore();

    // draw border around if the cell is current
    if (d->is_current) {
        if (d->the_style->metaObject()->className() == QString("QWindowsXPStyle") ||
            d->the_style->metaObject()->className() == QString("QMotifStyle") ||
            d->the_style->metaObject()->className() == QString("QCDEStyle"))
            drawBorder(d->ginfo, row, col, rect, d->pal, painter, d->is_current, d->is_selected);
    }
}

void QicsComboCellDisplay::startEdit(QicsScreenGrid *grid, int row, int col,
                                const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QComboBox *widget = static_cast<QComboBox *> (info->widget());
    widget->setParent(grid);

    info->setRow(ginfo->modelRowIndex(row));
    info->setColumn(ginfo->modelColumnIndex(col));

    // Setup widget (colors, fonts, values etc) for this cell
    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    if (lineEdit())
        lineEdit()->setAlignment(static_cast<Qt::Alignment>(myCell->alignment()));

    widget->setFont(myCell->font());
    widget->setPalette(myCell->palette());

    //THERE WAS HACK
    bool state = widget->blockSignals(true);
    QString txt = textToDisplay(ginfo, row, col, itm);

    if (widget->isEditable())
        widget->setEditText(txt);

    int index = widget->findText(txt);
    widget->setCurrentIndex(index);

    if ((index == -1) && (myAddValueToList && !widget->isEditable())) {
        if (!txt.isEmpty()) {
            widget->addItem(txt);
            widget->setCurrentIndex(widget->count() - 1);
        }
    }
    else
        info->setItem(new QicsDataString(widget->currentText()));

    widget->blockSignals(state);

    QicsCellDisplay::startEdit(grid,row,col,itm);
}

void QicsComboCellDisplay::moveEdit(QicsScreenGrid *grid,
                               int row, int col,
                               const QRect &rect)
{
    QWidget *widget = getInfoFromGrid(grid)->widget();
    QicsGridInfo *ginfo = &(grid->gridInfo());

    widget->setGeometry(entryWidgetRect(ginfo, row, col, rect));
    if (QComboBox::isEditable()) widget->show();

    widget->setFocus();
}

void QicsComboCellDisplay::endEdit(QicsScreenGrid *grid, int row, int col)
{
    QicsCellDisplay::endEdit(grid,row,col);

    QicsEntryWidgetInfo *info = getInfoFromEntry(this);
    if (!info)
        return;

    if (isEditable()) {
        QicsGridInfo &ginfo = grid->gridInfo();

        valueChanged(&ginfo, ginfo.visualRowIndex(info->row()),
        ginfo.visualColumnIndex(info->column()), info->row(),
              info->column(), currentText());
    }

    info->widget()->hide();
}

bool QicsComboCellDisplay::handleMouseEvent(QicsScreenGrid *grid, int row, int col,
                                       QMouseEvent *me)
{
    myCell->setInfo(&grid->gridInfo());
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    if (!myCell->enabled())
        return false;

    if (me->type() == QEvent::MouseMove || me->modifiers() & Qt::ControlModifier)
        return false;

    if (me->type() != QEvent::MouseButtonRelease) {
        if (!grid->editCell(row, col)) return false;
    }
    else {
        const bool isHeader = (grid->gridInfo().gridType() != Qics::TableGrid);
        QicsSelectionManager *selectionManager = grid->gridInfo().selectionManager();
        bool isMultiCellSelection = false;

        const QicsSelection *sel = selectionManager->currentSelection();
        if (!sel) return false;

        switch (selectionManager->selectionPolicy())
        {
        case SelectNone:
        case SelectSingle:
        case SelectSingleRow:
            isMultiCellSelection = false;
            break;
        case SelectMultiple:
            isMultiCellSelection = isHeader ? (sel->numColumns() > 1) && (sel->numRows() > 1) : (sel->numColumns() > 1) || (sel->numRows() > 1);
            break;
        case SelectMultipleRow:
            isMultiCellSelection = (sel->numRows() > 1);
            break;
        }

        if (!isMultiCellSelection)
            QTimer::singleShot(150, this, SLOT(popup()));
    }

    return false;
}

bool QicsComboCellDisplay::handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *ke)
{
    QComboBox::keyPressEvent(ke);
    return true;
}

// This is a hack to catch Tab and Backtab keys
bool QicsComboCellDisplay::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);

        switch (ke->key())
        {
        case Qt::Key_Tab:
        case Qt::Key_Backtab: {
                hidePopup();
                QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *> (this->parent());
                if (!grid) return false;
                grid->handleTraversalKeys(ke);
                return true;
            }
        case Qt::Key_Space:
            showPopup();
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if (!myAddValueToList)
                return true;
            break;
        }

        // catching Space bar key to close QComboBox popup
        if (qobject_cast<QListView*>(obj) && ke->key() == Qt::Key_Space) {
            hidePopup();
            setFocus();
            return true;
        }
    }

    if (event->type() == QEvent::Leave && qobject_cast<QListView*>(obj)) {
        hidePopup();
        setFocus();
        return true;
    }

    return QComboBox::eventFilter(obj, event);
}

void QicsComboCellDisplay::keyPressEvent(QKeyEvent *ke)
{
    if(isEditable() && ke->key() != Qt::Key_Up && ke->key() != Qt::Key_Down)
        QComboBox::keyPressEvent(ke);

    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *> (this->parent());
    if (!grid) return;

    bool eatEvent = false;

    if(( ke->key() != Qt::Key_Enter
        && ke->key() != Qt::Key_Return
        && ke->key() != Qt::Key_Left
        && ke->key() != Qt::Key_Right
        && ke->key() != Qt::Key_Tab
        && ke->key() != Qt::Key_Backtab
        && isEditable()) || !isEditable())
        eatEvent = grid->handleTraversalKeys(ke);

    if(!isEditable() && !eatEvent) {
        if (((static_cast<int>(ke->modifiers()) & Qt::ControlModifier) == 0) ||
            (ke->key() != Qt::Key_Up && ke->key() != Qt::Key_Down))
            parentWidget()->setFocus();

        QComboBox::keyPressEvent(ke);
    }
}

void QicsComboCellDisplay::focusOutEvent(QFocusEvent* e)
{
    myLastFocusReason = e->reason();
    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid*>(parentWidget());

    if(grid && !hasFocus() && !view()->hasFocus())
        grid->uneditCurrentCell();

    QComboBox::focusOutEvent(e);
}

QSize QicsComboCellDisplay::sizeHint(QicsGrid *grid, int row, int col,
                               const QicsDataItem *itm)
{
    Q_UNUSED(itm);

    QComboBox * cb = static_cast<QComboBox *>(this);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    //QString qs = textToDisplay(ginfo, row, col, itm);

    cb->setFont(myCell->font());

    return (cb->sizeHint());
}

QString QicsComboCellDisplay::textToDisplay(QicsGridInfo *info, int row, int col,
                                    const QicsDataItem *item) const
{
    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QString label = myCell->label();
    if (!label.isEmpty()) return label;

    return (item ? item->string() : QString());
}

void QicsComboCellDisplay::valueChanged(QicsGridInfo *info, int row, int col,
                        int model_row, int model_col, const QString &val)
{
    Q_UNUSED(model_row);
    Q_UNUSED(model_col);

    const QicsDataItem* item = info->cellValue(row, col);
    if (isEditable() && item && currentText() == item->string())
        return;

    QicsDataString itm(val);
    info->setCellValue(row, col, itm);
}

void QicsComboCellDisplay::itemSelected(const QString &val)
{
    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(parent());
    if (!grid) return;

    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(ginfo->visualRowIndex(info->row()));
    myCell->setColumnIndex(ginfo->visualColumnIndex(info->column()));

    valueChanged(ginfo, ginfo->visualRowIndex(info->row()),
        ginfo->visualColumnIndex(info->column()), info->row(),
        info->column(), val);
}

QString QicsComboCellDisplay::cellDisplayName() const
{
    return ComboCellDisplayName;
}

QDomElement QicsComboCellDisplay::toDomXml(const QString &tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());

    for(int i=0; i < QComboBox::count(); ++i) {
        QDomElement itemE = doc->createElement("item");
        itemE.setAttribute("string", QComboBox::itemData(i, Qt::DisplayRole).toString());
        e.appendChild(itemE);
    }

    return e;
}

void QicsComboCellDisplay::configureFromDomXml(const QDomElement& e)
{
    QComboBox::clear();
    QDomElement itemE = e.firstChildElement("item");
    while( !itemE.isNull()) {
        QComboBox::addItem(itemE.attribute("string"));
        itemE = itemE.nextSiblingElement("item");
    }
}

QicsComboCellDisplayConfigurationWidget::QicsComboCellDisplayConfigurationWidget(QWidget* parent)
    : QicsCellDisplayConfigurationWidget(parent)
{
    _itemList = new QListWidget;
    _itemList->setEditTriggers(QAbstractItemView::DoubleClicked);
    _itemList->setSelectionMode(QAbstractItemView::SingleSelection);

    _add = new QPushButton("Add");
    _remove = new QPushButton("Remove");
    _up = new QPushButton("Up");
    _down = new QPushButton("Down");

    connect( _add, SIGNAL(clicked()), this, SLOT(addItem()) );
    connect( _remove, SIGNAL(clicked()), this, SLOT(removeItem()) );
    connect( _up, SIGNAL(clicked()), this, SLOT(moveItemUp()) );
    connect( _down, SIGNAL(clicked()), this, SLOT(moveItemDown()) );

    QVBoxLayout* btnLayout = new QVBoxLayout;
    btnLayout->addWidget(_add);
    btnLayout->addWidget(_remove);
    btnLayout->addWidget(_up);
    btnLayout->addWidget(_down);
    btnLayout->addStretch(1);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(_itemList);
    layout->addStretch(1);
    layout->addLayout(btnLayout);

    setLayout(layout);
}

void QicsComboCellDisplayConfigurationWidget::addItem()
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setText("New Item");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    _itemList->addItem(item);

    emit configurationChanged();
}

void QicsComboCellDisplayConfigurationWidget::removeItem()
{
    foreach( QListWidgetItem* item, _itemList->selectedItems() )
        delete item;

    emit configurationChanged();
}

void QicsComboCellDisplayConfigurationWidget::moveItemUp()
{
    int index;

    foreach( QListWidgetItem* item, _itemList->selectedItems() ) {
        _itemList->selectionModel()->clear();
        index = _itemList->row(item);
        QListWidgetItem* taken = _itemList->takeItem(index);
        _itemList->insertItem(--index, taken);
        _itemList->setItemSelected(taken, true);
    }

    emit configurationChanged();
}

void QicsComboCellDisplayConfigurationWidget::moveItemDown()
{
    int index;

    foreach( QListWidgetItem* item, _itemList->selectedItems() ) {
        _itemList->selectionModel()->clear();
        index = _itemList->row(item);
        QListWidgetItem* taken = _itemList->takeItem(index);
        _itemList->insertItem(++index, taken);
        _itemList->setItemSelected(taken, true);
    }

    emit configurationChanged();
}

void QicsComboCellDisplayConfigurationWidget::readConfiguration(QicsCellDisplay* cellDisplay)
{
    _itemList->clear();

    if(!cellDisplay)
        return;

    QComboBox* combo = dynamic_cast<QComboBox*>(cellDisplay);
    if(!combo) return;

    for(int i=0; i<combo->count(); ++i) {
        QAbstractItemModel* model = combo->model();
        QModelIndex index = model->index(i,0,QModelIndex());
        QString itemStr = model->data(index, Qt::DisplayRole).toString();
        _itemList->addItem(itemStr);
    }
}

void QicsComboCellDisplayConfigurationWidget::writeConfiguration(QicsCellDisplay* cellDisplay)
{
    QComboBox* combo = dynamic_cast<QComboBox*>(cellDisplay);
    if(!combo) return;

    combo->clear();
    for(int i=0; i<_itemList->count(); ++i) {
        QString str = _itemList->item(i)->text();
        combo->addItem(str);
    }
}

QicsCellDisplay *QicsComboCellDisplayFactory::createCellDisplay()
{
    return new QicsComboCellDisplay;
}

QicsCellDisplayConfigurationWidget *QicsComboCellDisplayFactory::configurationWidget()
{
    return new QicsComboCellDisplayConfigurationWidget();
}


