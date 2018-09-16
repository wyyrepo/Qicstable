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

#include "QicsListCellDisplay.h"

#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QScrollBar>

#include "QicsCell.h"
#include "QicsScreenGrid.h"
#include "QicsSelectionManager.h"
#include "QicsUtil.h"
#include "QicsDataItemFormatter.h"
#include "QicsMainGrid.h"
#include "QicsCellDisplay_p.h"

const QString QicsListCellDisplay::ListCellDisplayName = "ListCellDisplay";


QicsListCellDisplay::QicsListCellDisplay(QWidget *parent)
    : QListWidget(parent), QicsMovableEntryWidgetCellDisplay()
{
    connect(this, SIGNAL(currentRowChanged(int)), SLOT(rowChanged(int)));
    static_cast<QListWidget*>(this)->installEventFilter(this);
    hide();
}

void QicsListCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                      const QicsDataItem *itm,
                                      QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, this, true, false);

    // draw the list widget
    QListWidget *lw = static_cast<QListWidget *>(this);
    QListWidget *widget = new QListWidget(lw->parentWidget());

    for (int i = 0; i < lw->count(); ++i) {
        QListWidgetItem *item = lw->item(i);
        widget->addItem(item->text());
    }

    QString txt = textToDisplay(d->ginfo, row, col, itm);
    QList<QListWidgetItem *> lwiList = widget->findItems(txt, Qt::MatchCaseSensitive);
    if (lwiList.count())
        widget->setCurrentItem(lwiList.at(0));
    else
        widget->setCurrentRow(0);

    widget->horizontalScrollBar()->setValue(lw->horizontalScrollBar()->value());
    widget->verticalScrollBar()->setValue(lw->verticalScrollBar()->value());

    widget->setGeometry(d->cr);
    if (widget->geometry() != d->cr) {
        QLayout* l = widget->layout();
        if (l)
            l->setSizeConstraint(QLayout::SetNoConstraint);
        widget->setMinimumSize(d->cr.size());
        widget->setGeometry(d->cr);
    }

    widget->setFont(myCell->font());
    widget->setPalette(myCell->palette());

#if QT_VERSION < 0x050000
    QPixmap pix = QPixmap::grabWidget(widget);
#else
    QPixmap pix = lw->grab();
#endif
    painter->drawPixmap(d->cr, pix);

    delete widget;
}

void QicsListCellDisplay::startEdit(QicsScreenGrid *grid, int row, int col,
                                    const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QListWidget *widget = static_cast<QListWidget*> (info->widget());
    widget->setParent(grid);

    info->setRow(ginfo->modelRowIndex(row));
    info->setColumn(ginfo->modelColumnIndex(col));

    // Setup widget (colors, fonts, values etc) for this cell

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    widget->setFont(myCell->font());
    widget->setPalette(myCell->palette());
    widget->setFocus();

    QString txt = textToDisplay(ginfo, row, col, itm);
    bool state = widget->blockSignals(true);
    QList<QListWidgetItem *> lwiList = widget->findItems(txt, Qt::MatchCaseSensitive);
    if (lwiList.count())
        widget->setCurrentItem(lwiList.at(0));
    widget->blockSignals(state);

    QicsCellDisplay::startEdit(grid,row,col,itm);
}

void QicsListCellDisplay::moveEdit(QicsScreenGrid *grid,
                                   int row, int col,
                                   const QRect &rect)
{
    QWidget *widget = getInfoFromGrid(grid)->widget();
    QicsGridInfo *ginfo = &(grid->gridInfo());

    widget->setGeometry(entryWidgetRect(ginfo, row, col, rect));
    widget->show();

    widget->setFocus();
}

void QicsListCellDisplay::endEdit(QicsScreenGrid *grid, int row, int col)
{
    QicsCellDisplay::endEdit(grid,row,col);

    QListWidget *widget = static_cast<QListWidget *>(this);
    QicsEntryWidgetInfo *info = getInfoFromEntry(widget);
    if (!info)
        return;

    QicsGridInfo &ginfo = grid->gridInfo();

    valueChanged(&ginfo, ginfo.visualRowIndex(info->row()),
        ginfo.visualColumnIndex(info->column()), currentRow());

    info->widget()->hide();
}

bool QicsListCellDisplay::handleMouseEvent(QicsScreenGrid *grid, int row, int col,
                                           QMouseEvent *me)
{
    myCell->setInfo(&grid->gridInfo());
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    if (!myCell->enabled())
        return false;

    if (me->type() == QEvent::MouseMove || me->modifiers() & Qt::ControlModifier)
        return false;

    if (me->type() != QEvent::MouseButtonRelease)
        grid->traverseToCell(row, col, false);
    else {
        QWidget *viewport = QApplication::widgetAt(me->globalPos());
        if (!viewport) return false;

        QListWidget *listWidget = qobject_cast<QListWidget *>(viewport->parent());
        if (listWidget) {
            QListWidgetItem *item = listWidget->itemAt(viewport->mapFrom(grid, me->pos()));
            if (!item) return false;
            listWidget->setCurrentItem(item);
            return true;
        }
    }

    return false;
}

bool QicsListCellDisplay::handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *ke)
{
    QListWidget::keyPressEvent(ke);
    return true;
}

void QicsListCellDisplay::focusOutEvent(QFocusEvent* fe)
{
    myLastFocusReason = fe->reason();

    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid*>(parentWidget());
    if (grid)
        grid->uneditCurrentCell();

    QListWidget::focusOutEvent(fe);
}

QSize QicsListCellDisplay::sizeHint(QicsGrid *grid, int row, int col,
                                    const QicsDataItem *itm)
{
    Q_UNUSED(itm);

    QListWidget * lw = static_cast<QListWidget*>(this);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    //QString qs = textToDisplay(ginfo, row, col, itm);
    lw->setFont(myCell->font());
    return (lw->sizeHint());
}

QWidget *QicsListCellDisplay::newEntryWidget(QicsScreenGrid *)
{
    return (static_cast<QListWidget*>(this));
}

void QicsListCellDisplay::setCurrentRow(int row)
{
    QString val = "";
    QListWidget *lw = static_cast<QListWidget*>(this);
    QListWidget::setCurrentRow(row);

    QListWidgetItem *lwi = lw->currentItem();
    if (lwi)
        val = lwi->text();

    QicsEntryWidgetInfo *info = getInfoFromEntry(lw);
    if (info != 0)
        info->setItem( new QicsDataString(val) );
}

void QicsListCellDisplay::setCurrentText(const QString& val)
{
    QListWidget *lw = static_cast<QListWidget*>(this);
    QList<QListWidgetItem *> lwiList = lw->findItems(val, Qt::MatchCaseSensitive);
    if (lwiList.count()) {
        setCurrentItem(lwiList.at(0));

        QicsEntryWidgetInfo *info = getInfoFromEntry(lw);
        if (info != 0)
            info->setItem(new QicsDataString(lwiList.at(0)->text()));
    }
}

void QicsListCellDisplay::rowChanged(int index)
{
    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(parent());
    if (!grid) return;

    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(ginfo->visualRowIndex(info->row()));
    myCell->setColumnIndex(ginfo->visualColumnIndex(info->column()));

    valueChanged(ginfo, ginfo->visualRowIndex(info->row()),
        ginfo->visualColumnIndex(info->column()), index);
}

void QicsListCellDisplay::valueChanged(QicsGridInfo *ginfo, int row, int col,
                                       int index)
{
    const QicsDataItem *dataitem = ginfo->cellValue(row, col);

    QString txt;
    QListWidgetItem *lwi = item(index);
    if (lwi) txt = lwi->text();
    if (dataitem && txt == dataitem->string())
        return;

    QicsDataString itm(txt);
    ginfo->setCellValue(row, col, itm);
}

QString QicsListCellDisplay::textToDisplay(QicsGridInfo *info, int row, int col,
                                    const QicsDataItem *item) const
{
    QString itemString = item ? item->string() : QString();
    if (!itemString.isEmpty()) return itemString;

    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    return myCell->label();
}

QString QicsListCellDisplay::cellDisplayName() const
{
    return ListCellDisplayName;
}

QDomElement QicsListCellDisplay::toDomXml(const QString &tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());

    for(int i=0; i < QListWidget::count(); ++i) {
        QDomElement itemE = doc->createElement("item");
        itemE.setAttribute("string", QListWidget::item(i)->text());
        e.appendChild(itemE);
    }

    return e;
}

void QicsListCellDisplay::configureFromDomXml(const QDomElement& e)
{
    QListWidget::clear();
    QDomElement itemE = e.firstChildElement("item");
    while( !itemE.isNull()) {
        QListWidget::addItem(itemE.attribute("string"));
        itemE = itemE.nextSiblingElement("item");
    }
}


QicsListCellDisplayConfigurationWidget::QicsListCellDisplayConfigurationWidget(QWidget* parent)
    : QicsCellDisplayConfigurationWidget(parent)
{
    _itemList = new QListWidget;
    _itemList->setEditTriggers(QAbstractItemView::DoubleClicked);
    _itemList->setSelectionMode(QAbstractItemView::SingleSelection);

    _add = new QPushButton("Add");
    _remove = new QPushButton("Remove");
    _up = new QPushButton("Up");
    _down = new QPushButton("Down");

    connect( _add, SIGNAL(clicked()), SLOT(addItem()) );
    connect( _remove, SIGNAL(clicked()), SLOT(removeItem()) );
    connect( _up, SIGNAL(clicked()), SLOT(moveItemUp()) );
    connect( _down, SIGNAL(clicked()), SLOT(moveItemDown()) );

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

void QicsListCellDisplayConfigurationWidget::addItem()
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setText("New Item");
    item->setFlags(Qt::ItemIsSelectable |
        Qt::ItemIsEnabled |
        Qt::ItemIsEditable);
    _itemList->addItem(item);

    emit configurationChanged();
}

void QicsListCellDisplayConfigurationWidget::removeItem()
{
    foreach( QListWidgetItem* item, _itemList->selectedItems() ) {
        delete item;
    }
    emit configurationChanged();
}

void QicsListCellDisplayConfigurationWidget::moveItemUp()
{
    foreach( QListWidgetItem* item, _itemList->selectedItems() ) {
        _itemList->selectionModel()->clear();
        int index = _itemList->row(item);
        QListWidgetItem* taken = _itemList->takeItem(index);
        _itemList->insertItem(--index, taken);
        _itemList->setItemSelected(taken, true);
    }
    emit configurationChanged();
}

void QicsListCellDisplayConfigurationWidget::moveItemDown()
{
    foreach( QListWidgetItem* item, _itemList->selectedItems() ) {
        _itemList->selectionModel()->clear();
        int index = _itemList->row(item);
        QListWidgetItem* taken = _itemList->takeItem(index);
        _itemList->insertItem(++index, taken);
        _itemList->setItemSelected(taken, true);
    }
    emit configurationChanged();
}

void QicsListCellDisplayConfigurationWidget::readConfiguration(QicsCellDisplay* cellDisplay)
{
    _itemList->clear();

    if(!cellDisplay)
        return;

    QListWidget* list = dynamic_cast<QListWidget*>(cellDisplay);

    if(list) {
        for( int i=0; i<list->count(); ++i ) {
            QAbstractItemModel* model = list->model();
            QModelIndex index = model->index(i,0,QModelIndex());
            QString itemStr = model->data(index, Qt::DisplayRole).toString();
            _itemList->addItem(itemStr);
        }
    }
}

void QicsListCellDisplayConfigurationWidget::writeConfiguration(QicsCellDisplay* cellDisplay)
{
    QListWidget* list = dynamic_cast<QListWidget*>(cellDisplay);

    if(list) {
        list->clear();
        for(int i=0; i<_itemList->count(); ++i) {
            QString str = _itemList->item(i)->text();
            list->addItem(str);
        }
    }
}


QicsCellDisplay* QicsListCellDisplayFactory::createCellDisplay()
{
    return new QicsListCellDisplay;
}

QicsCellDisplayConfigurationWidget* QicsListCellDisplayFactory::configurationWidget()
{
    return new QicsListCellDisplayConfigurationWidget();
}


