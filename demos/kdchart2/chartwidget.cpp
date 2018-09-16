/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "chartwidget.h"

#include <QVBoxLayout>
#include <QicsTable.h>
#ifdef BIND_KDCHART
#include <QicsKDChartDataModelAdapter.h>
#else
#include <QLabel>
#endif


ChartWidget::ChartWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle(tr("KDChart II Integration Demo"));

    const int numRows = 2;
    const int numCols = 3;
    m_model = new QicsDataModelDefault(numRows, numCols, this);

    for (int row = 0; row < numRows; ++row)
        for (int column = 0; column < numCols; ++column)
            m_model->setItem(row, column, QicsDataDouble(row+1 * column + 1));

    m_table = new QicsTable(m_model, this);
    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);

    m_table->cellRegionRef(0,0,numRows,0).setBackgroundColor(Qt::red);
    m_table->cellRegionRef(0,1,numRows,1).setBackgroundColor(Qt::green);
    m_table->cellRegionRef(0,2,numRows,2).setBackgroundColor(Qt::blue);

#ifdef BIND_KDCHART
    m_adapter = new QicsKDChartDataModelAdapter(this, m_table);
    m_adapter->setModel(m_model);

    KDChart::BarDiagram* diagram = new KDChart::BarDiagram(this);
    diagram->setModel(m_adapter);
    m_chart.coordinatePlane()->replaceDiagram(diagram);
#endif

    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(m_table);
#ifdef BIND_KDCHART
    l->addWidget(&m_chart);
#else
    qWarning("Sorry, no KDChart support compiled in");
    QLabel *lWarning = new QLabel(tr("Sorry, no KDChart support compiled in"));
    lWarning->setAlignment(Qt::AlignCenter);
    l->addWidget(lWarning);
#endif
    setLayout(l);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(slotTimeout()));
    m_timer->start( 200 );
}

void ChartWidget::slotTimeout()
{
    const QicsDataItem *item = m_model->item(0, 1);
    double value = (int(item->number()) % 24 ) + 1;
    m_model->setItem(0, 1, value);
}


