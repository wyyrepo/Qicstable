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

#include "imageviewer.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ImageViewer iv;
    iv.resize(640, 480);
    iv.show();

    return app.exec();
}


