/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef STOCKUI_H
#define STOCKUI_H

#include <QMainWindow>

class QicsDataModel;
class QicsTable;
class StockData;

class StockUI: public QMainWindow
{
    Q_OBJECT
public:
    StockUI(QWidget *parent = 0);

    inline QicsTable *table() const { return m_table; }
    inline StockData *data() const { return m_sd; }

signals:
    void updateToggled(bool on);

protected slots:
    void decorateStock(int idx, bool gainer);

private:
    void setupRowHeader();
    void setupColumnHeader();
    void setupGrid();
    void setupDay(int day_idx, const QString &day);

private:
    QicsTable *m_table;
    StockData *m_sd;
};

#endif //STOCKUI_H


