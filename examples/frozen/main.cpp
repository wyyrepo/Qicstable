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

#include <QicsDataModelDefault.h>
#include "frozen.h"


int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    const int nrows = 50;
    const int ncols = 50;

    QicsDataModel *dm = new QicsDataModelDefault(nrows, ncols);

    for (int i = 0; i < nrows; ++i)
        for (int j = 0; j < ncols; ++j)
            dm->setItem(i, j, (i+j));

    Frozen *frozen = new Frozen(dm);
    frozen->show();

    return app.exec();
}


