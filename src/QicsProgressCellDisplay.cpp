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

#include "QicsProgressCellDisplay.h"

#include <QApplication>
#include <QLayout>
#include <QLabel>

#include "QicsCell.h"
#include "QicsScreenGrid.h"
#include "QicsSelectionManager.h"
#include "QicsUtil.h"
#include "QicsDataItemFormatter.h"
#include "QicsMainGrid.h"
#include "QicsCellDisplay_p.h"

const QString QicsProgressCellDisplay::ProgressCellDisplayName = "ProgressCellDisplay";


QicsProgressCellDisplay::QicsProgressCellDisplay(QWidget *parent)
    : QProgressBar(parent), QicsMovableEntryWidgetCellDisplay()
{
    setAlignment(Qt::AlignCenter);
    hide();
}

QWidget* QicsProgressCellDisplay::newEntryWidget(QicsScreenGrid *)
{
    return (static_cast<QProgressBar*>(this));
}

void QicsProgressCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                          const QicsDataItem *itm,
                                          QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, this, true, false);

    // draw the list widget
    QProgressBar *pb = static_cast<QProgressBar *>(this);

    bool state = pb->blockSignals(true);
    if (itm)
        pb->setValue(qMin((int)itm->number(), pb->maximum()));
    else
        pb->setValue(pb->minimum());
    pb->blockSignals(state);

    pb->setGeometry(d->cr);
    if (pb->geometry() != d->cr) {
        QLayout* l = pb->layout();
        if (l)
            l->setSizeConstraint(QLayout::SetNoConstraint);
        pb->setMinimumSize(d->cr.size());
        pb->setGeometry(d->cr);
    }

    pb->setFont(myCell->font());
    pb->setPalette(myCell->palette());

#if QT_VERSION < 0x050000
    QPixmap pix = QPixmap::grabWidget(pb);
#else
    QPixmap pix = pb->grab();
#endif
    painter->drawPixmap(d->cr, pix);
}

QSize QicsProgressCellDisplay::sizeHint(QicsGrid *grid, int row, int col, const QicsDataItem *)
{
    QProgressBar * pb = static_cast<QProgressBar*>(this);
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);
    pb->setFont(myCell->font());

    return (pb->sizeHint());
}

void QicsProgressCellDisplay::setValue(int value)
{
    QProgressBar * pb = static_cast<QProgressBar *>(this);
    QProgressBar::setValue(value);

    QicsEntryWidgetInfo *info = getInfoFromEntry(pb);
    if (info != 0)
        info->setItem( new QicsDataInt(value) );
}

void QicsProgressCellDisplay::valueChanged(QicsGridInfo *ginfo, int row, int col, int val)
{
    const QicsDataItem* item = ginfo->cellValue(row, col);
    if (item && val == item->number())
        return;
    QicsDataInt itm(val);
    ginfo->setCellValue(row, col, itm);
}

QString QicsProgressCellDisplay::cellDisplayName() const
{
    return ProgressCellDisplayName;
}

QDomElement QicsProgressCellDisplay::toDomXml(const QString &tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());

    QDomElement itemE = doc->createElement("item");
    itemE.setAttribute("min", QProgressBar::minimum());
    itemE.setAttribute("max", QProgressBar::maximum());
    e.appendChild(itemE);

    return e;
}

void QicsProgressCellDisplay::configureFromDomXml(const QDomElement& e)
{
    QDomElement itemE = e.firstChildElement("item");

    setMinimum(itemE.attribute("min").toInt());
    setMaximum(itemE.attribute("max").toInt());
}


QicsProgressCellDisplayConfigurationWidget::QicsProgressCellDisplayConfigurationWidget(QWidget* parent)
    : QicsCellDisplayConfigurationWidget(parent)
{
    QLabel* label = new QLabel("No Configuration Options");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);

    setLayout(layout);
}

void QicsProgressCellDisplayConfigurationWidget::readConfiguration(QicsCellDisplay* cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

void QicsProgressCellDisplayConfigurationWidget::writeConfiguration(QicsCellDisplay* cellDisplay)
{
    Q_UNUSED(cellDisplay);
}


QicsCellDisplay* QicsProgressCellDisplayFactory::createCellDisplay()
{
    return new QicsProgressCellDisplay;
}

QicsCellDisplayConfigurationWidget* QicsProgressCellDisplayFactory::configurationWidget()
{
    return new QicsProgressCellDisplayConfigurationWidget();
}


