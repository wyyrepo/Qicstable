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

#include "QicsKDChartTableObject.h"

#include <QicsTable.h>
#include <QicsKDChartDataModelAdapter.h>

#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartPieDiagram>
#include <KDChartPolarDiagram>
#include <KDChartRingDiagram>
#include <KDChartPlotter>


QicsKDChartTableObject::QicsKDChartTableObject(QicsTable *_table,
                                               AbstractDiagram *_diagram,
                                               const QicsRegion &_region,
                                               const QicsRegion &_dataRegion)
    : QicsWidgetCellDisplay(0, _table),
       m_table(_table),
       m_diagram(_diagram),
       m_region(_region),
       m_dataRegion(_dataRegion)
{
    m_adapter = new QicsKDChartDataModelAdapter(this, m_table, m_dataRegion);
    m_diagram->setModel(m_adapter);

    setWidget(m_chart = new Chart(m_table));
    m_chart->coordinatePlane()->replaceDiagram(m_diagram);

    if (m_region.isValid()) {
        m_table->addCellSpan(QicsSpan::fromRegion(m_region));
        m_table->cellRef(m_region.top(), m_region.left()).setDisplayer(this);
    }
}

QicsCellDisplay* QicsKDChartTableObject::clone()
{
    AbstractDiagram	*_diagram = 0;

    do {
        BarDiagram *bar = dynamic_cast<BarDiagram*>(m_diagram);
        if (bar) {
            _diagram = bar->clone();
            break;
        }

        LineDiagram *line = dynamic_cast<LineDiagram*>(m_diagram);
        if (line) {
            _diagram = line->clone();
            break;
        }

        PieDiagram *pie = dynamic_cast<PieDiagram*>(m_diagram);
        if (pie) {
            _diagram = pie->clone();
            break;
        }

        PolarDiagram *polar = dynamic_cast<PolarDiagram*>(m_diagram);
        if (polar) {
            _diagram = polar->clone();
            break;
        }

        RingDiagram *ring = dynamic_cast<RingDiagram*>(m_diagram);
        if (ring) {
            _diagram = ring->clone();
            break;
        }

        Plotter *plotter = dynamic_cast<Plotter*>(m_diagram);
        if (plotter) {
            _diagram = plotter->clone();
            break;
        }

    } while (false);

    if (!_diagram) {
        Q_ASSERT_X(_diagram, "Error detected", "No such diagram");
        return 0;
    }

    return this->create(m_table, _diagram, QicsRegion(), m_dataRegion);
}

QicsCellDisplay *QicsKDChartTableObject::create(QicsTable *_table, AbstractDiagram *_diagram,
                                const QicsRegion &_region, const QicsRegion &_dataRegion)
{
    return new QicsKDChartTableObject(_table, _diagram, _region, _dataRegion);
}

QicsKDChartTableObject::~QicsKDChartTableObject()
{
}

void QicsKDChartTableObject::setDataRegion(const QicsRegion &_dataRegion)
{
    m_adapter->setDataRegion(_dataRegion);
}

bool QicsKDChartTableObject::rowsColsReverted() const
{
    return m_adapter->rowsColsReverted();
}

bool QicsKDChartTableObject::colorsReverted() const
{
    return m_adapter->colorsReverted();
}

void QicsKDChartTableObject::setRowsColsReverted(bool set)
{
    m_adapter->setRowsColsReverted(set);
}

void QicsKDChartTableObject::setColorsReverted(bool set)
{
    m_adapter->setColorsReverted(set);
}


