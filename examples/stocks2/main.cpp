/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "stockapp.h"
#include "stockui.h"

int main(int argc, char **argv)
{
    StockApp app(argc,argv);
    app.mainWindow()->show();
    return app.exec();
}


