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
#include <QMessageBox>
#include "chartwidget.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
#ifdef BIND_KDCHART
    ChartWidget w;
    w.show();
#else
    // Warn user that there is no KDChart support compiled in
    qWarning("Sorry, no KDChart support compiled in");
    QMessageBox::information(0, QObject::tr("KDChart Integration Demo"), QObject::tr("Sorry, no KDChart support compiled in"));
    return 0;
#endif
    return app.exec();
}


