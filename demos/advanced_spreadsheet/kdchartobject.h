/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef KDCHARTOBJECT_H
#define KDCHARTOBJECT_H

#include <QicsKDChartTableObject.h>

class QMenu;

class KDChartObject : public QicsKDChartTableObject
{
    Q_OBJECT

public:
    KDChartObject(QicsTable *_table, AbstractDiagram *_diagram, const QicsRegion &_region, const QicsRegion &_dataRegion);
    virtual ~KDChartObject();

protected slots:
    void assignData();

protected:
    virtual bool handleMouseEvent(QicsScreenGrid *, int, int, QMouseEvent *e);
    //virtual void eventProcessed(QObject* watched, QEvent* e, bool handled);

private:
    QMenu *m_menu;
};

#endif // KDCHARTOBJECT_H


