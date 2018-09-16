/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "spreadsheetcalculator.h"

#include "spreadsheettable.h"

#define ERROR_STRING "####"

QString SpreadsheetCalculator::calculateFormula(const QString& formula,QicsTable* table)
{
#ifndef _WITH_CALCULATIONS
    Q_UNUSED(table)
        return formula;
#else
    if(!formula.startsWith("="))
        return formula;

    QStringList args = formula.split(QRegExp("[+-/*]"));

    int operation = 0;
    if(formula.indexOf("+") != -1)
        operation = ADD;
    else if(formula.indexOf("*") != -1)
        operation = MULT;
    else if(formula.indexOf("-") != -1)
        operation = DEC;
    else if(formula.indexOf("/") != -1)
        operation = DIV;

    QString first_operand = args.count()>0?args[0]:"";
    first_operand = first_operand.remove(QRegExp("[= ]"));
    QString second_operand = args.count()>1?args[1]:"";
    second_operand = second_operand.remove(QRegExp("[= ]"));

    if(first_operand.isEmpty() || second_operand.isEmpty())
        return ERROR_STRING;

    bool ok = false;
    double first_num = first_operand.toDouble(&ok);
    if(!ok) {
        QicsDataModel* dm = table->dataModel();
        if(!dm)
            return QString();

        int row = -1; int col = -1;
        parseArgument(first_operand,row,col,table);

        const QicsDataItem* itm = dm->item(row,col);

        if(!itm)
            return ERROR_STRING;

        ok = false;
        first_num = itm->string().toDouble(&ok);
        if(!ok)
            return ERROR_STRING;

    }

    ok = false;
    double second_num = second_operand.toDouble(&ok);
    if(!ok) {
        QicsDataModel* dm = table->dataModel();
        if(!dm)
            return QString();

        int row = -1; int col = -1;
        parseArgument(second_operand,row,col,table);

        const QicsDataItem* itm = dm->item(row,col);

        if(!itm)
            return ERROR_STRING;

        ok = false;
        second_num = itm->string().toDouble(&ok);
        if(!ok)
            return ERROR_STRING;
    }

    switch(operation)
    {
    case ADD:
        return QString::number((double)first_num + second_num);
    case MULT:
        return QString::number((double)first_num * second_num);
    case DIV:
        return QString::number((double)first_num / second_num);
    case DEC:
        return QString::number((double)first_num - second_num);
    }

    return ERROR_STRING;
#endif
}

void SpreadsheetCalculator::parseArgument(const QString& str,int& row,int& col,QicsTable* table)
{
    if(!str.contains(QRegExp("[A-Z]")) || !str.contains(QRegExp("[0-9]")))
        return;

    QString operand(str);
    QString str_row = operand.remove(QRegExp("[A-Z]*"));
    row = qobject_cast<SpreadsheetTable*>(table)->rowFromLabel(str_row);

    operand = str;
    QString column = operand.remove(QRegExp("[0-9 ]*"));

    col = qobject_cast<SpreadsheetTable*>(table)->columnFromLabel(column);
}


