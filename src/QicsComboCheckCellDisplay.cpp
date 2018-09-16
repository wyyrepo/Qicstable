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

#include "QicsComboCheckCellDisplay.h"

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


const QString QicsComboCheckCellDisplay::ComboCheckCellDisplayName = "ComboCheckCellDisplay";


QicsComboCheckCellDisplay::QicsComboCheckCellDisplay(QWidget *parent)
    : QicsComboCellDisplay(parent)
{
    m_separator = ",";
    m_defaultText = "";
    setModel(new QicsCheckComboModel(this));

    disconnect(this, SIGNAL(activated(const QString &)), this, SLOT(itemSelected(const QString &)));
    connect(this, SIGNAL(activated(int)), SLOT(toggleCheckState(int)));
    connect(model(), SIGNAL(checkStateChanged()), SLOT(updateCheckedItems()));
}

void QicsComboCheckCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                            const QicsDataItem *itm,
                                            QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, this, true, false);

    int flags = myCell->alignment() | myCell->textFlags();

    // draw the combo box
    QIcon ico;

    // deside what text to display
    if (d->qs.isEmpty())
        d->qs = m_defaultText;

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

    d->the_style->drawComplexControl(QStyle::CC_ComboBox,
        &ccOptions,
        painter/*,
        cb*/);

    // ### TODO: Polish the style... for #1281, Bugzilla
    if (d->the_style->metaObject()->className() == QString("QWindowsStyle")) {
        if (hasFrame()) {
            painter->setPen(d->pal.mid().color());
            painter->drawLine(rect.right()-1, rect.top()+1, rect.right()-1, rect.bottom()-1);
            painter->drawLine(rect.left()+1, rect.bottom()-1, rect.right()-1, rect.bottom()-1);
        }
    }

    QRect trect = d->the_style->subControlRect(QStyle::CC_ComboBox,
        &ccOptions,
        QStyle::SC_ComboBoxEditField/*,
        cb*/);

    if (d->is_selected && !d->is_current)
        d->pal.setColor(QPalette::Text, d->pal.highlightedText().color());

    // draw icon
    if (!ico.isNull()) {        // ### TODO: not sure if we should handle margins here...
        trect.setLeft(trect.left() + myCell->margin());
        ico.paint(painter, trect, Qt::AlignLeft, d->is_enabled ? QIcon::Normal : QIcon::Disabled);
        QSize sz = ico.actualSize(trect.size());
        trect.setLeft(trect.left() + sz.width() + myCell->pixmapSpacing());
    }

    d->the_style->drawItemText(painter, trect, flags, d->pal, d->is_enabled, d->qs, QPalette::Text);

    painter->restore();
}

void QicsComboCheckCellDisplay::endEdit(QicsScreenGrid *grid, int row, int col)
{
    QicsCellDisplay::endEdit(grid,row,col);

    QicsEntryWidgetInfo *info = getInfoFromEntry(this);
    if (!info)
        return;

    if (isEditable())
        updateCheckedItems();

    info->widget()->hide();
}

void QicsComboCheckCellDisplay::popup()
{
    QComboBox * cb = static_cast<QComboBox *>(this);
    QicsEntryWidgetInfo *info = getInfoFromEntry(cb);
    if (info) {
        QicsGridInfo *ginfo = &(info->grid()->gridInfo());
        const QicsDataItem* item = ginfo->cellValue(info->row(), info->column());
        QStringList list;
        if (item)
            list = (item->string().split(m_separator));
        bool blocked = model()->blockSignals(true);
        setCheckedItems(list, true);
        model()->blockSignals(blocked);
    }

    QicsComboCellDisplay::popup();
}

Qt::CheckState QicsComboCheckCellDisplay::itemCheckState(int index) const
{
    return static_cast<Qt::CheckState>(itemData(index, Qt::CheckStateRole).toInt());
}

void QicsComboCheckCellDisplay::setItemCheckState(int index, Qt::CheckState state)
{
    setItemData(index, state, Qt::CheckStateRole);
}

QStringList QicsComboCheckCellDisplay::checkedItems() const
{
    QStringList items;

    for (int i = 0; i < count(); ++i)
        if (itemCheckState(i) == Qt::Checked)
            items += itemText(i);

    return items;
}

void QicsComboCheckCellDisplay::setCheckedItems(const QStringList& items, bool douncheck)
{
    for (int i = 0; i < count(); ++i) {
        if (items.contains(itemText(i)))
            setItemCheckState(i, Qt::Checked);
        else if (douncheck)
            setItemCheckState(i, Qt::Unchecked);
    }
}

void QicsComboCheckCellDisplay::setDefaultText(const QString& text)
{
    if (m_defaultText != text) {
        m_defaultText = text;
        updateCheckedItems();
    }
}

void QicsComboCheckCellDisplay::setSeparator(const QString& separator)
{
    if (m_separator != separator) {
        m_separator = separator;
        updateCheckedItems();
    }
}

void QicsComboCheckCellDisplay::updateCheckedItems()
{
    QString val;
    QStringList items = checkedItems();
    if (items.isEmpty())
        val = m_defaultText;
    else
        val = items.join(m_separator);

    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(parent());
    if (!grid) return;

    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(ginfo->visualRowIndex(info->row()));
    myCell->setColumnIndex(ginfo->visualColumnIndex(info->column()));

    valueChanged(ginfo, ginfo->visualRowIndex(info->row()),
        ginfo->visualColumnIndex(info->column()), val);
}

void QicsComboCheckCellDisplay::toggleCheckState(int index)
{
    QVariant value = itemData(index, Qt::CheckStateRole);
    if (value.isValid()) {
        Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
        setItemData(index, (state == Qt::Unchecked ? Qt::Checked : Qt::Unchecked), Qt::CheckStateRole);
    }
}

void QicsComboCheckCellDisplay::valueChanged(QicsGridInfo *info, int row, int col,
                                             const QString &val)
{
    QicsDataString itm(val);
    info->setCellValue(row, col, itm);

    if (isEditable())
        lineEdit()->setText(val);
}

QString QicsComboCheckCellDisplay::textToDisplay(QicsGridInfo *info, int row, int col,
                                    const QicsDataItem *item) const
{
    QString itemString = item ? item->string() : QString();
    if (!itemString.isEmpty()) return itemString;

    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QString label = myCell->label();
    if (!label.isEmpty()) return label;

    return defaultText();
}

QString QicsComboCheckCellDisplay::cellDisplayName() const
{
    return ComboCheckCellDisplayName;
}


QicsComboCheckCellDisplayConfigurationWidget::QicsComboCheckCellDisplayConfigurationWidget(QWidget* parent)
    : QicsComboCellDisplayConfigurationWidget(parent)
{
}


QicsCellDisplay* QicsComboCheckCellDisplayFactory::createCellDisplay()
{
    return new QicsComboCheckCellDisplay;
}

QicsCellDisplayConfigurationWidget* QicsComboCheckCellDisplayFactory::configurationWidget()
{
    return new QicsComboCheckCellDisplayConfigurationWidget();
}


QicsCheckComboModel::QicsCheckComboModel(QObject* parent)
    : QStandardItemModel(0, 1, parent)
{
}

Qt::ItemFlags QicsCheckComboModel::flags(const QModelIndex& index) const
{
    return QStandardItemModel::flags(index) | Qt::ItemIsUserCheckable;
}

QVariant QicsCheckComboModel::data(const QModelIndex& index, int role) const
{
    QVariant value = QStandardItemModel::data(index, role);
    if (index.isValid() && role == Qt::CheckStateRole && !value.isValid())
        value = Qt::Unchecked;
    return value;
}

bool QicsCheckComboModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool ok = QStandardItemModel::setData(index, value, role);
    if (ok && role == Qt::CheckStateRole) {
        emit dataChanged(index, index);
        emit checkStateChanged();
    }
    return ok;
}


