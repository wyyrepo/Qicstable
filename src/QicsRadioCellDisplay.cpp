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

#include "QicsRadioCellDisplay.h"

#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QRadioButton>

#include "QicsCell.h"
#include "QicsScreenGrid.h"
#include "QicsSelectionManager.h"
#include "QicsUtil.h"
#include "QicsDataItemFormatter.h"
#include "QicsMainGrid.h"
#include "QicsCellDisplay_p.h"

const QString QicsRadioCellDisplay::RadioCellDisplayName = "RadioCellDisplay";


QicsRadioCellDisplay::QicsRadioCellDisplay(QWidget *parent)
    : QicsRadioWidget(parent), QicsMovableEntryWidgetCellDisplay()
{
    connect(this, SIGNAL(buttonClicked(int)), this, SLOT(changeIndex(int)));
    hide();
}

void QicsRadioCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                  const QicsDataItem *itm,
                                  QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, this, true, false);

    // draw the radio widget
    QicsRadioWidget *rw = static_cast<QicsRadioWidget *>(this);

    QicsRadioWidget *widget = new QicsRadioWidget(rw->parentWidget());
    widget->setFont(myCell->font());
    widget->setPalette(myCell->palette());
    widget->setGeometry(d->cr);

    QList<QAbstractButton *> list = rw->buttons();
    for (int i = 0; i < list.count(); ++i)
        widget->addButton(list.at(i)->text());

    if (itm) {
        QRadioButton *rb = widget->button(itm->number());
        if (rb) rb->setChecked(true);
    }

#if QT_VERSION < 0x050000
    QPixmap pix = QPixmap::grabWidget(widget);
#else
    QPixmap pix = widget->grab();
#endif
    painter->drawPixmap(d->cr, pix);
    delete widget;
}

void QicsRadioCellDisplay::startEdit(QicsScreenGrid *grid, int row, int col, const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsRadioWidget *widget = static_cast<QicsRadioWidget*> (info->widget());
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

    const QicsDataInt* id = static_cast<const QicsDataInt*> (itm);
    bool state = widget->blockSignals(true);
    if (id) {
        if (id->number() == -1 && widget->checkedId() != -1)
            widget->uncheckAll();
        QRadioButton *rb = widget->button(id->number());
        if (rb)
            rb->setChecked(true);
    }
    widget->blockSignals(state);

    QicsCellDisplay::startEdit(grid,row,col,itm);
}

void QicsRadioCellDisplay::moveEdit(QicsScreenGrid *grid,
                                    int row, int col,
                                    const QRect &rect)
{
    QWidget *widget = getInfoFromGrid(grid)->widget();
    QicsGridInfo *ginfo = &(grid->gridInfo());

    widget->setGeometry(entryWidgetRect(ginfo, row, col, rect));
    widget->show();

    widget->setFocus();
}

void QicsRadioCellDisplay::endEdit(QicsScreenGrid *grid, int row, int col)
{
    QicsCellDisplay::endEdit(grid,row,col);

    QicsRadioWidget *widget = static_cast<QicsRadioWidget *>(this);
    QicsEntryWidgetInfo *info = getInfoFromEntry(widget);
    if (!info)
        return;

    QicsGridInfo &ginfo = grid->gridInfo();

    valueChanged(&ginfo, myCell->rowIndex(),
        myCell->columnIndex(), widget->checkedId());

    info->widget()->hide();
}

bool QicsRadioCellDisplay::handleMouseEvent(QicsScreenGrid *grid, int row, int col, QMouseEvent *me)
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
        QWidget *widget = QApplication::widgetAt(me->globalPos());
        if (!widget) return false;

        QRadioButton *rb = qobject_cast<QRadioButton *>(widget);
        if (rb) {
            rb->setFocus();
            rb->setChecked(true);
            QicsRadioWidget *rw = static_cast<QicsRadioWidget*>(this);
            changeIndex(rw->checkedId());
            return true;
        }
    }

    return false;
}

bool QicsRadioCellDisplay::handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *ke)
{
    QicsRadioWidget::keyPressEvent(ke);
    return true;
}

void QicsRadioCellDisplay::focusOutEvent(QFocusEvent* fe)
{

    QicsRadioWidget *widget = static_cast<QicsRadioWidget *>(this);
    bool myfocus = false;

    // if focus passed to the radio buttons return it to the widget
    QList<QAbstractButton*> abList = widget->buttons();
    for (int i = 0; i < abList.count(); ++i) {
        QRadioButton *rb = qobject_cast<QRadioButton*>(abList.at(i));
        if (rb->hasFocus()) {
            myfocus = true;
            rb->setChecked(true);
            changeIndex(i);
            widget->setFocus();
            break;
        }
    }

    if (!myfocus) {
        myLastFocusReason = fe->reason();

        QicsScreenGrid *grid = qobject_cast<QicsScreenGrid*>(parentWidget());
        if (grid)
            grid->uneditCurrentCell();

        QicsRadioWidget::focusOutEvent(fe);
    }
}

QSize QicsRadioCellDisplay::sizeHint(QicsGrid *grid, int row, int col, const QicsDataItem *)
{
    QicsRadioWidget *widget = static_cast<QicsRadioWidget*>(this);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    widget->setFont(myCell->font());
    return (widget->sizeHint());
}

QWidget *QicsRadioCellDisplay::newEntryWidget(QicsScreenGrid *)
{
    return (static_cast<QicsRadioWidget*>(this));
}

void QicsRadioCellDisplay::setChecked(int id)
{
    QicsRadioWidget *widget = static_cast<QicsRadioWidget*>(this);

    QRadioButton *rb = widget->button(id);
    if (rb) {
        QicsEntryWidgetInfo *info = getInfoFromEntry(widget);
        if (info != 0)
            info->setItem( new QicsDataInt(id) );
        rb->setChecked(true);
    }
}

void QicsRadioCellDisplay::changeIndex(int index)
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

void QicsRadioCellDisplay::valueChanged(QicsGridInfo *ginfo, int row, int col, int index)
{
    const QicsDataInt* item = static_cast<const QicsDataInt*> (ginfo->cellValue(row, col));

    if (item && index == item->number())
        return;
    QicsDataInt itm(index);
    ginfo->setCellValue(row, col, itm);
}

QString QicsRadioCellDisplay::cellDisplayName() const
{
    return RadioCellDisplayName;
}

QDomElement QicsRadioCellDisplay::toDomXml(const QString &tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());

    QicsRadioWidget *widget = static_cast<QicsRadioWidget*>(this);
    QList<QAbstractButton*> abList = widget->buttons();
    for (int i = 0; i < abList.count(); ++i) {
        QDomElement itemE = doc->createElement("item");
        itemE.setAttribute("string", abList.at(i)->text());
        e.appendChild(itemE);
    }
    return e;
}

void QicsRadioCellDisplay::configureFromDomXml(const QDomElement& e)
{
    QicsRadioWidget *widget = static_cast<QicsRadioWidget*>(this);
    widget->clear();

    QDomElement itemE = e.firstChildElement("item");
    while( !itemE.isNull()) {
        widget->addButton(itemE.attribute("string"));
        itemE = itemE.nextSiblingElement("item");
    }
}


QicsRadioCellDisplayConfigurationWidget::QicsRadioCellDisplayConfigurationWidget(QWidget* parent)
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

void QicsRadioCellDisplayConfigurationWidget::addItem()
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setText("New Item");
    item->setFlags(Qt::ItemIsSelectable |
        Qt::ItemIsEnabled |
        Qt::ItemIsEditable);
    _itemList->addItem(item);

    emit configurationChanged();
}

void QicsRadioCellDisplayConfigurationWidget::removeItem()
{
    foreach( QListWidgetItem* item, _itemList->selectedItems() )
        delete item;

    emit configurationChanged();
}

void QicsRadioCellDisplayConfigurationWidget::moveItemUp()
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

void QicsRadioCellDisplayConfigurationWidget::moveItemDown()
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

void QicsRadioCellDisplayConfigurationWidget::readConfiguration(QicsCellDisplay* cellDisplay)
{
    _itemList->clear();

    if(!cellDisplay)
        return;

    QicsRadioWidget * widget = dynamic_cast<QicsRadioWidget *>(cellDisplay);

    if( widget ) {
        QList<QAbstractButton*> abList = widget->buttons();
        for (int i = 0; i < abList.count(); ++i)
            _itemList->addItem(abList.at(i)->text());
    }
}

void QicsRadioCellDisplayConfigurationWidget::writeConfiguration(QicsCellDisplay* cellDisplay)
{
    QicsRadioWidget * widget = dynamic_cast<QicsRadioWidget *>(cellDisplay);

    if(widget) {
        widget->clear();
        for(int i=0; i<_itemList->count(); ++i) {
            QString str = _itemList->item(i)->text();
            widget->addButton(str);
        }
    }
}


QicsCellDisplay* QicsRadioCellDisplayFactory::createCellDisplay()
{
    return new QicsRadioCellDisplay;
}

QicsCellDisplayConfigurationWidget* QicsRadioCellDisplayFactory::configurationWidget()
{
    return new QicsRadioCellDisplayConfigurationWidget();
}



QicsRadioWidget::QicsRadioWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);

    m_bg = new QButtonGroup(this);

    m_vbox = new QVBoxLayout(this);
    m_vbox->setMargin(2);
    m_vbox->setSpacing(2);
    m_vbox->addStretch(1);

    connect(m_bg, SIGNAL(buttonClicked(int)), SIGNAL(buttonClicked(int)));
    connect(m_bg, SIGNAL(buttonPressed(int)), SIGNAL(buttonPressed(int)));
    connect(m_bg, SIGNAL(buttonReleased(int)), SIGNAL(buttonReleased(int)));
}

void QicsRadioWidget::addButton(QRadioButton * button)
{
    m_bg->addButton(button, count());
    m_vbox->insertWidget(m_vbox->count()-1, button);
}

void QicsRadioWidget::addButton(const QString &text)
{
    QRadioButton *rb = new QRadioButton(text, this);
    addButton(rb);
}

void QicsRadioWidget::addItems(const QStringList &labels)
{
    QStringList::const_iterator it = labels.constBegin();
    for (; it != labels.constEnd(); ++it)
        addButton(*it);
}

QRadioButton *QicsRadioWidget::button(int id) const
{
    return qobject_cast<QRadioButton*>(m_bg->button(id));
}

QRadioButton *QicsRadioWidget::checkedButton() const
{
    return qobject_cast<QRadioButton*>(m_bg->checkedButton());
}

int QicsRadioWidget::checkedId() const
{
    return m_bg->checkedId();
}

int QicsRadioWidget::id(QRadioButton *button) const
{
    return m_bg->id(button);
}

void QicsRadioWidget::uncheckAll()
{
    if (m_bg->checkedButton()) {
        m_bg->setExclusive(false);
        m_bg->checkedButton()->setChecked(false);
        m_bg->setExclusive(true);
    }
}

void QicsRadioWidget::removeButton(QRadioButton *button)
{
    button->disconnect(this);
    m_bg->removeButton(button);
    m_vbox->removeWidget(button);
}

void QicsRadioWidget::clear()
{
    QList<QAbstractButton*> abList = m_bg->buttons();
    for (int i = 0; i < abList.count(); ++i) {
        m_bg->removeButton(abList.at(i));
        m_vbox->removeWidget(abList.at(i));
    }
}

int QicsRadioWidget::count() const
{
    QList<QAbstractButton*> abList = m_bg->buttons();
    return abList.count();
}



