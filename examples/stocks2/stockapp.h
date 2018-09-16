/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef STOCKAPP_H
#define STOCKAPP_H

#include <QApplication>

class QicsDataItem;
class StockUI;

class StockApp : public QApplication
{
    Q_OBJECT
public:
    StockApp(int &argc, char **argv);

    inline StockUI *mainWindow() const { return m_ui; }

private slots:
    void toggleUpdates(bool on);
    void updateStocks();

private:
    StockUI *m_ui;
    QTimer *m_timer;
};

#endif


