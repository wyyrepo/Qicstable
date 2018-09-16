/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <QApplication>
#include "spreadsheet.h"

int main(int argc, char **argv)
{
    QApplication qapp(argc, argv);

    Spreadsheet* ss = new Spreadsheet();

    ss->showMaximized();

    return qapp.exec();
}
