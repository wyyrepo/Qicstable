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

#include "mainwindow.h"


int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(res);

    QApplication app(argc,argv);
    MainWindow mw;
    mw.show();
    return app.exec();
}


