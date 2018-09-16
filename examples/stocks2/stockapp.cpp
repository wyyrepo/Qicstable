/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "stockapp.h"

#include <QTimer>

#include <QicsTable.h>

#include "stockui.h"
#include "stockdata.h"

static int update_period = 500;


StockApp::StockApp(int &argc, char **argv)
: QApplication(argc, argv)
{
    m_ui = new StockUI();
    connect(m_ui, SIGNAL(updateToggled(bool)), this, SLOT(toggleUpdates(bool)));

    m_timer = new QTimer( this );
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateStocks()));
}

void StockApp::toggleUpdates(bool on)
{
    if (on)
        m_timer->start(update_period);
    else
        m_timer->stop();
}

void StockApp::updateStocks()
{
    m_ui->data()->updateStocks();
    m_ui->table()->sortRows(day_zero+cur_idx, Qics::Descending, 2, -1);
}


