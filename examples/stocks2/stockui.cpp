/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "stockui.h"

#include <QMenuBar>
#include <QPageSetupDialog>
#include <QicsTable.h>
#include <QicsComboCellDisplay.h>
#include <QicsTextCellDisplay.h>
#include <../../addons/table.html/QicsHTMLExportDialog.h>
#ifndef QICSTABLE_GPL
#include <QicsTablePrint.h>
#include <../../addons/printing/QicsPrintPreviewDialog.h>
#endif
#include <QicsRepeatingRow.h>
#include <QicsCell.h>
#include <QicsMainGrid.h>
#include <QicsColumnHeader.h>
#include <QicsRow.h>
#include <QicsColumn.h>
#include <QicsDataItemFormatter.h>
#include <QicsRowHeader.h>
#include "stocksorterdelegate.h"
#include "stockdata.h"
#include "high.xpm"
#include "low.xpm"

static int stock_name = 0;
static int nyse_idx = 0;
static int nasdaq_idx = 1;


StockUI::StockUI(QWidget *parent)
    : QMainWindow( parent )
{
    setAttribute( Qt::WA_DeleteOnClose );
    setWindowTitle(tr("Stock II Example"));

    m_sd = new StockData( this );
    m_sd->loadDT( "initial-values.txt" );
    connect(m_sd, SIGNAL(gainChanged(GainVector *)), this, SLOT(updateGain(GainVector *)) );

    m_table = new QicsTable(m_sd->dataModel(), this);
    m_table->setSorterDelegate(new StockSorterDelegate());
    setCentralWidget(m_table);

    m_table->repeatingRowRef(0,2).setBackgroundColor(0xf0f0f0);
    m_table->setNavigatorAllowed(true);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
#ifndef QICSTABLE_GPL
    fileMenu->addAction(tr("&Print..."), this, SLOT(printPreview()));
#endif
    fileMenu->addAction(tr("Export HTML..."), this, SLOT(exportHTML()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("E&xit"), this, SLOT(close()));

    QLabel *label = new QLabel(tr("Stock Activity"), m_table);
    QFont fnt = label->font();
    fnt.setPointSize(fnt.pointSize() + 6);
    label->setFont(fnt);
    label->setAlignment(Qt::AlignCenter);
    m_table->setTopTitleWidget(label);

    setupRowHeader();
    setupColumnHeader();
    setupGrid();

    QCheckBox *cb = new QCheckBox(tr("Auto\nUpdates"), m_table);
    m_table->setTopLeftCornerWidget(cb);
    connect(cb, SIGNAL(toggled(bool)), this, SIGNAL(updateToggled(bool)));
}

void StockUI::exportHTML()
{
    QicsHTMLExportDialog::exportTable(m_table, "output/table.html");
}

#ifndef QICSTABLE_GPL
void StockUI::printPreview()
{
    QicsPrintPreviewDialog::previewAndPrint(m_table);
    raise();
}
#endif

void StockUI::setupRowHeader()
{
    // nothing to do here
    QicsRowHeader *rh = m_table->rowHeader();
    rh->cellRef(0,0).setLabel("NYSE");
    rh->cellRef(1,0).setLabel("NASDAQ");
    delete rh;
}

void StockUI::setupColumnHeader()
{
    QicsColumnHeader *ch = m_table->columnHeader();

    ch->setNumRows(3);
    ch->setAlignment(Qt::AlignCenter);

    QicsTextCellDisplay *cd = new QicsTextCellDisplay(m_table);
    cd->setIconShiftsText(false);
    ch->setDisplayer(cd);

    // Stock header
    ch->addCellSpan(QicsSpan(0,stock_name, 3, 1));
    ch->cellRef(0,stock_name).setLabel(tr("Stock"));

    setupDay(day_zero, tr("Today"));
    setupDay(day_one, tr("One Day Ago"));
    setupDay(day_two, tr("Two Days Ago"));
    setupDay(day_three, tr("Three Days Ago"));

    delete ch;
}

void StockUI::setupDay(int day_idx, const QString &day)
{
    QicsColumnHeader *ch = m_table->columnHeader();

    ch->addCellSpan(QicsSpan(0,day_idx, 1, 3));
    ch->cellRef(0,day_idx).setLabel( day );

    ch->cellRef(0,day_idx).setForegroundColor(Qt::white);
    ch->cellRef(0,day_idx).setBackgroundColor(Qt::blue);

    ch->addCellSpan(QicsSpan(1,day_idx, 1, 2));
    ch->cellRef(1,day_idx).setLabel(tr("Show Gains in"));

    ch->dataModel()->setItem(1,day_idx+2, QicsDataString(tr("Green")));
    QicsComboCellDisplay *cd = new QicsComboCellDisplay(m_table);
    cd->addItem("Green");
    cd->addItem("Red");
    ch->cellRef(1,day_idx+2).setDisplayer(cd);

    ch->cellRef(2,day_idx+cur_idx).setLabel(tr("Current"));

    ch->cellRef(2,day_idx+high_idx).setLabel(tr("High"));
    ch->cellRef(2,day_idx+high_idx).setPixmap(QPixmap(high_xpm));

    ch->cellRef(2,day_idx+low_idx).setLabel(tr("Low"));
    ch->cellRef(2,day_idx+low_idx).setPixmap(QPixmap(low_xpm));

    delete ch;
}

void StockUI::setupGrid()
{
    QicsDataItemSprintfFormatter *v = new QicsDataItemSprintfFormatter();

    m_table->columnRef(0).setWidthInChars(7);

    v->addFormatString(QicsDataItem_Float, "$%.2f");
    v->addFormatString(QicsDataItem_Int, "$%d.00");
    m_table->mainGridRef().setFormatter(v);

    for (int i = day_zero; i <= (day_three + 2); ++i)
        m_table->columnRef(i).setAlignment(Qt::AlignRight);

    // the NYSE and NASDAQ rows...
    QFont fnt = m_table->font();
    fnt.setPointSize(fnt.pointSize() + 2);
    fnt.setBold(true);
    m_table->rowRef(nyse_idx).setFont(fnt);
    m_table->rowRef(nasdaq_idx).setFont(fnt);

    v = new QicsDataItemSprintfFormatter();
    v->addFormatString(QicsDataItem_Float, "%.2f");
    v->addFormatString(QicsDataItem_Int, "%d.00");
    m_table->rowRef(nyse_idx).setFormatter(v);
    m_table->rowRef(nasdaq_idx).setFormatter(v);

    m_table->freezeTopRows(2);
}

void StockUI::decorateStock(int idx, bool gainer)
{
    const QicsDataItem *itm = m_table->columnHeaderRef().cellRef(1, day_zero+2).dataValue();
    QString gain_color = itm->string();

    QColor c;

    if (gainer)
        c = QColor(gain_color);
    else
        c = m_table->mainGridRef().foregroundColor();

    m_table->cellRef(idx, day_zero+cur_idx).setForegroundColor(c);
}

void StockUI::updateGain(GainVector *gv)
{
    m_table->setRepaintBehavior(Qics::RepaintOff);

    const QicsDataItem *itm = m_table->columnHeaderRef().cellRef(1, day_zero+2).dataValue();
    QColor gain_color = QColor(itm->string());
    QColor fore_color = m_table->mainGridRef().foregroundColor();

    for (int i = 0; i < gv->size(); ++i) {
        m_table->cellRef(gv->at(i).row, day_zero+cur_idx).setForegroundColor(
            gv->at(i).gain ? gain_color : fore_color );
    }

    m_table->setRepaintBehavior(Qics::RepaintOn);
}


