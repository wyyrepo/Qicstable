/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef STOCKDATA_H
#define STOCKDATA_H

#include <QObject>
#include <QicsRegion.h>

const int cur_idx = 0;
const int high_idx = 1;
const int low_idx = 2;
const int day_zero = 1;
const int day_one = 4;
const int day_two = 7;
const int day_three = 10;

class QicsDataModel;

class StockData : public QObject
{
    Q_OBJECT
public:
    StockData(QObject *parent);

    inline QicsDataModel *dataModel() const { return m_dm; }
    void loadDT(const QString &initialFilePath);

public slots:
    void updateStocks();

signals:
    void stockChanged(int row, bool gained);

protected slots:
    void dataValuesChanged(QicsRegion reg);

private:
    void addDay(int start_col);
    void updateStock(int row);
    float getFloatValue(int row, int col);
    float newprice(float old_price, float percent_change = 0.02, int updown = 1);
    int roundf(float f);
    float randf();
    int posneg();

private:
    QicsDataModel *m_dm;
};

#endif //STOCKDATA_H


