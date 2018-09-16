/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "spreadsheetdm.h"

#include <QFile>
#include <QDataStream>


SpreadsheetDataModel::SpreadsheetDataModel(int nrows,int ncols)
    : QicsDataModelDefault(nrows,ncols)
{
}

SpreadsheetDataModel::~SpreadsheetDataModel()
{
}

bool SpreadsheetDataModel::writeDataModel(const QString& fname)
{
    QFile file(fname);
    if(!file.open(QIODevice::WriteOnly))
        return false;

    QDataStream ds(&file);

    ds<<myNumRows<<myNumColumns;
    ds<<myVectorOfRowPointers.count();

    for(int i = 0;i < myVectorOfRowPointers.count(); ++i) {
        QVector<QicsDataItem*> *myColumns = myVectorOfRowPointers[i];

        if(!myColumns) {
            ds<<(bool)false;
            continue;
        }
        else
            ds<<(bool)true;

        ds<<myColumns->count();

        for(int j = 0;j < myColumns->count(); ++j) {
            QicsDataItem* di = (*myColumns)[j];
            if(di)
                di->encode(ds);
            else
                ds<<(int)0;
        }
    }

    file.close();
    return true;
}

bool SpreadsheetDataModel::loadDataModel(const QString& fname)
{
    QFile file(fname);
    if(!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream ds(&file);

    int nrows,ncols;
    ds>>nrows;
    ds>>ncols;

    int rowPointersCount;
    ds>>rowPointersCount;

    clearModel();
    addRows(nrows);
    addColumns(ncols);

    setEmitSignals(false);

    for(int i = 0;i < rowPointersCount; ++i) {
        bool filled;
        ds>>filled;

        if(!filled)
            continue;

        int colsCount;
        ds>>colsCount;

        for(int j = 0;j < colsCount; ++j) {
            QicsDataItem* di = QicsDataItem::decode(ds);
            if(di)
                setItem(i,j,*di);
        }
    }

    setEmitSignals(true);

    file.close();
    return true;
}


