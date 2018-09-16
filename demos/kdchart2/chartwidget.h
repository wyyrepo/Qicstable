/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>

#ifdef BIND_KDCHART
#include <KDChartChart>
#include <KDChartBarDiagram>
#endif

class QicsTable;
class QicsDataModel;
#ifdef BIND_KDCHART
class QicsKDChartDataModelAdapter;
#endif

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    ChartWidget(QWidget* parent=0);

private slots:
    void slotTimeout();

private:
#ifdef BIND_KDCHART
    KDChart::Chart m_chart;
    QicsKDChartDataModelAdapter *m_adapter;
#endif
    QicsTable *m_table;
    QicsDataModel *m_model;
    QTimer *m_timer;
};

#endif //CHARTWIDGET_H


