/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SPREADSHEETCALCULATOR_H
#define SPREADSHEETCALCULATOR_H

class QicsTable;
class QString;

class SpreadsheetCalculator
{
private:
    // Standart operations.
    enum OPERATIONS{ADD,DEC,MULT,DIV};
    // Helper method for arguments parsing
    static void parseArgument(const QString&,int& row,int& col,QicsTable* table);

public:
    static QString calculateFormula(const QString& formula,QicsTable* table);
};

#endif //SPREADSHEETCALCULATOR_H


