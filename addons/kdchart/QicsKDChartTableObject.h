/*********************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#ifndef QICSKDCHARTTABLEOBJECT_H
#define QICSKDCHARTTABLEOBJECT_H

#include <QicsNamespace.h>
#include <QicsWidgetCellDisplay.h>
#include <QicsRegion.h>

#include <KDChartChart>

using namespace KDChart;

class QicsTable;
class QicsKDChartDataModelAdapter;


class QICS_EXPORT QicsKDChartTableObject : public QicsWidgetCellDisplay
{
    Q_OBJECT
public:
    QicsKDChartTableObject(QicsTable *_table, AbstractDiagram *_diagram, const QicsRegion &_region, const QicsRegion &_dataRegion);
    virtual ~QicsKDChartTableObject();

    virtual QicsCellDisplay* create(QicsTable *_table, AbstractDiagram *_diagram, const QicsRegion &_region, const QicsRegion &_dataRegion);
    virtual QicsCellDisplay* clone();

    inline AbstractDiagram* diagram() { return m_diagram; }
    inline Chart *chart() { return m_chart; }

    void setDataRegion(const QicsRegion &_dataRegion);

    bool rowsColsReverted() const;
    bool colorsReverted() const;

public slots:
    void setRowsColsReverted(bool set);
    void setColorsReverted(bool set);

protected:
    QicsTable *m_table;
    QicsRegion m_region, m_dataRegion;
    QicsKDChartDataModelAdapter *m_adapter;
    Chart *m_chart;
    AbstractDiagram *m_diagram;
};

#endif //QICSKDCHARTTABLEOBJECT_H


