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

#include "QicsComboTabCellDisplay.h"

#include <QApplication>
#include <QMouseEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>

#include "QicsCell.h"
#include "QicsScreenGrid.h"
#include "QicsSelectionManager.h"
#include "QicsUtil.h"
#include "QicsDataItemFormatter.h"
#include "QicsMainGrid.h"
#include "QStandardItemModel"
#include "QicsCellDisplay_p.h"

const QString QicsComboTabCellDisplay::ComboTabCellDisplayName = "ComboTabCellDisplay";


QicsComboTabCellDisplay::QicsComboTabCellDisplay(QWidget *parent)
    : QicsComboCellDisplay(parent)
{
    m_activecolumn = 0;

    disconnect(this, SIGNAL(activated(const QString &)), this, SLOT(itemSelected(const QString &)));
    connect(this, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));

    QTableWidget *tw = new QTableWidget(parent);

#if QT_VERSION < 0x050000
    tw->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    tw->verticalHeader()->hide();
    tw->resizeRowsToContents();
    tw->resizeColumnsToContents();
    tw->setSelectionBehavior(QAbstractItemView::SelectRows);
    tw->setSelectionMode(QAbstractItemView::ExtendedSelection);

    setModel(tw->model());
    setView(tw);
}

void QicsComboTabCellDisplay::displayCell(QicsGrid *grid, int row, int col,
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
        &ccOptions, painter);

    // ### TODO: Polish the style... for #1281, Bugzilla
    if (d->the_style->metaObject()->className() == QString("QWindowsStyle")) {
        if (hasFrame()) {
            painter->setPen(d->pal.mid().color());
            painter->drawLine(rect.right()-1, rect.top()+1, rect.right()-1, rect.bottom()-1);
            painter->drawLine(rect.left()+1, rect.bottom()-1, rect.right()-1, rect.bottom()-1);
        }
    }

    QRect trect = d->the_style->subControlRect(QStyle::CC_ComboBox,
        &ccOptions, QStyle::SC_ComboBoxEditField);

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

void QicsComboTabCellDisplay::setDefaultText(const QString& text)
{
    if (m_defaultText != text)
        m_defaultText = text;
}

QString QicsComboTabCellDisplay::tabData(int row, int column) const
{
    if (model()) {
        QModelIndex index = model()->index(row, column, rootModelIndex());
        if (index.isValid())
            return index.data(Qt::DisplayRole).toString();
    }
    return QString();
}

void QicsComboTabCellDisplay::setTabData(int row, int column, const QString &data)
{
    if (model()) {
        QModelIndex index = model()->index(row, column, rootModelIndex());
        if (index.isValid()) {
            model()->setData(index, data, Qt::DisplayRole);
            (static_cast<QTableWidget *>(view()))->resizeRowsToContents();
            (static_cast<QTableWidget *>(view()))->resizeColumnsToContents();
        }
    }
}

void QicsComboTabCellDisplay::addRowData(int row, const QStringList& items)
{
    if (model()) {
        for (int i = 0; i < model()->columnCount(rootModelIndex()); ++i) {
            QModelIndex index = model()->index(row, i, rootModelIndex());
            if (index.isValid() && i < items.count())
                model()->setData(index, items.at(i), Qt::DisplayRole);
        }
        (static_cast<QTableWidget *>(view()))->resizeRowsToContents();
        (static_cast<QTableWidget *>(view()))->resizeColumnsToContents();
    }
}

void QicsComboTabCellDisplay::addColumnData(int column, const QStringList& items)
{
    if (model()) {
        for (int i = 0; i < model()->rowCount(rootModelIndex()); ++i) {
            QModelIndex index = model()->index(i, column, rootModelIndex());
            if (index.isValid() && i < items.count())
                model()->setData(index, items.at(i), Qt::DisplayRole);
        }
        (static_cast<QTableWidget *>(view()))->resizeRowsToContents();
        (static_cast<QTableWidget *>(view()))->resizeColumnsToContents();
    }
}

int  QicsComboTabCellDisplay::rowCount() const
{
    if (model())
        return model()->rowCount(rootModelIndex());

    return 0;
}

bool QicsComboTabCellDisplay::insertRows(int count, int index)
{
    if (model())
        return model()->insertRows(index, count, rootModelIndex());

    return false;
}

int  QicsComboTabCellDisplay::columnCount() const
{
    if (model())
        return model()->columnCount(rootModelIndex());

    return 0;
}

bool QicsComboTabCellDisplay::insertColumns(int count, int index)
{
    if (model())
        return model()->insertColumns(index, count, rootModelIndex());

    return false;
}

bool QicsComboTabCellDisplay::setTabSize(int rows, int columns)
{
    bool ok = false;
    if (model()) {
        model()->removeRows(0, rowCount(), rootModelIndex());
        model()->removeColumns(0, columnCount(), rootModelIndex());
        ok = model()->insertRows(0, rows, rootModelIndex());
        ok &= model()->insertColumns(0, columns, rootModelIndex());
    }
    return ok;
}

void QicsComboTabCellDisplay::setActiveColumn(int column)
{
    if (m_activecolumn != column) {
        m_activecolumn = column;
        itemSelected(currentIndex());
    }
}

void QicsComboTabCellDisplay::valueChanged(QicsGridInfo *ginfo, int row, int col,
                                           const QString &val)
{
    const QicsDataItem* item = ginfo->cellValue(row, col);
    if (isEditable() && item && currentText() == item->string())
        return;
    QicsDataString itm(val);
    ginfo->setCellValue(row, col, itm);
}

QString QicsComboTabCellDisplay::textToDisplay(QicsGridInfo *info, int row, int col,
                                    const QicsDataItem *item) const
{
    QString itemString = item ? item->string() : QString();
    if (!itemString.isEmpty()) return itemString;

    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    return myCell->label();
}

void QicsComboTabCellDisplay::itemSelected(int index)
{
    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(parent());
    if (!grid) return;

    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(ginfo->visualRowIndex(info->row()));
    myCell->setColumnIndex(ginfo->visualColumnIndex(info->column()));

    m_activecolumn = qBound(0, m_activecolumn, columnCount()-1);

    valueChanged(ginfo, ginfo->visualRowIndex(info->row()),
        ginfo->visualColumnIndex(info->column()), tabData(index, m_activecolumn));
}

void QicsComboTabCellDisplay::popup()
{
    QicsComboCellDisplay::popup();

    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(parent());
    if (!grid) return;

    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    const int row = ginfo->visualRowIndex(info->row());
    const int column = ginfo->visualColumnIndex(info->column());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(column);

    int id = 0;
    const QicsDataItem *item = grid->gridInfo().cellValue(row, column);
    if (item) {
        QString text = textToDisplay(ginfo, row, column, item);

        if (!text.isEmpty()) {
            const int rows = rowCount();
            for (int i = 0; i < rows; ++i) {
                QModelIndex index = model()->index(i, m_activecolumn, rootModelIndex());
                if (index.isValid() && text == index.data(Qt::DisplayRole).toString()) {
                    id = i;
                    break;
                }
            }
        }
    }

    QTableWidget *tw = qobject_cast<QTableWidget *>(view());
    Q_ASSERT(tw);
    tw->setCurrentCell(id, m_activecolumn);
    tw->selectRow(id);
}

QString QicsComboTabCellDisplay::cellDisplayName() const
{
    return ComboTabCellDisplayName;
}


QicsComboTabCellDisplayConfigurationWidget::QicsComboTabCellDisplayConfigurationWidget(QWidget* parent)
    : QicsComboCellDisplayConfigurationWidget(parent)
{
}


QicsCellDisplay* QicsComboTabCellDisplayFactory::createCellDisplay()
{
    return new QicsComboTabCellDisplay;
}

QicsCellDisplayConfigurationWidget* QicsComboTabCellDisplayFactory::configurationWidget()
{
    return new QicsComboTabCellDisplayConfigurationWidget();
}


