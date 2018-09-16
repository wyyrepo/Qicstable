/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef CUSTOM_DATA_MODEL_H
#define CUSTOM_DATA_MODEL_H

#include <QVector>
#include <QicsDataModel.h>


// This is an example of a custom data model.

////////////////////////////////////////////////////////////////////////

// First, here are some existing classes that define stock data.  These are
// supposed to simulate the kinds of classes that a typical programmer might
// already have when s/he decides to use the QicsTable.

// We will be using these classes when we make the custom data model.

class StockPrivate;

// Represents a set of stocks
class StockDataSet
{
public:
    StockDataSet();
    virtual ~StockDataSet();

    inline unsigned int numStocks(void) const
    { return m_stocks.size(); }

    virtual void insertStock(int position = -1);
    virtual void removeStock(int idx);

    QString symbol(int idx) const;
    virtual void setSymbol(int idx, QString sym);

    double high(int idx) const;
    virtual void setHigh(int idx, double val);

    double low(int idx) const;
    virtual void setLow(int idx, double val);

    double close(int idx) const;
    virtual void setClose(int idx, double val);

    unsigned int volume(int idx) const;
    virtual void setVolume(int idx, unsigned int val);

protected:
    typedef QVector<StockPrivate *> StockList;
    StockList m_stocks;
};

////////////////////////////////////////////////////////////////////////

class StockDataModel : public QicsDataModel, public StockDataSet
{
    Q_OBJECT
public:
    StockDataModel();
    virtual ~StockDataModel();

    const QicsDataItem *item(int row, int col) const;

    QicsDataModelRow rowItems(int row, int first_col=0, int last_col=-1) const;
    QicsDataModelColumn columnItems(int col, int first_row=0, int last_row=-1) const;

    void setRowItems(int row, const QicsDataModelRow &v);
    void setColumnItems(int col, const QicsDataModelColumn &v);

    // reimplemented methods of the StockDataSet
    void setSymbol(unsigned int idx, QString sym);
    void setHigh(unsigned int idx, double val);
    void setLow(unsigned int idx, double val);
    void setClose(unsigned int idx, double val);
    void setVolume(unsigned int idx, unsigned int val);
    void insertStock(int position = -1);
    void removeStock(unsigned int idx);

public slots:
    void setItem(int row, int col, const QicsDataItem &item);
    void clearItem(int row, int col);
    void clearModel(void);
    void addRows(int number_of_rows);
    void addColumns(int number_of_columns);
    void insertRows(int number_of_rows, int starting_position);
    void insertColumns(int number_of_columns, int starting_position);
    void deleteRow(int row);
    void deleteRows(int num_rows, int start_row);
    void deleteColumn(int col);
    void deleteColumns(int num_cols, int start_col);

protected:
    enum { SDM_Symbol = 0, SDM_Close = 1, SDM_High = 2,
        SDM_Low = 3, SDM_Volume = 4, SDM_NumDataItems = 5 } StockDataIndex;

    QicsDataItem *m_item;
};

#endif //CUSTOM_DATA_MODEL_H


