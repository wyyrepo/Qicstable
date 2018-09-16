/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SPREADSHEETDM_H
#define SPREADSHEETDM_H

#include <QicsDataModelDefault.h>

/*
* Spreadsheet data model class. Derived from QicsDataModelDefault. Has an
* additional methods for fast data loading, saving and replacing.
* This methods works directly with vectors of data items, that provides
* better performance.
*/
class SpreadsheetDataModel : public QicsDataModelDefault
{
    Q_OBJECT
public:
    SpreadsheetDataModel(int,int);
    virtual ~SpreadsheetDataModel();
    // Writes data model to file
    bool writeDataModel(const QString& fname);
    // Loads data model from file
    bool loadDataModel(const QString& fname);
};

#endif //SPREADSHEETDM_H


