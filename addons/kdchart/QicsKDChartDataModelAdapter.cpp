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

#include "QicsKDChartDataModelAdapter.h"

#include <QRegExp>

#include <QicsDataModel.h>
#include <QicsTable.h>

#include <KDChartChart>
#include <KDChartBarDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartThreeDBarAttributes>
#include <KDChartAttributesModel>

using namespace KDChart;


QicsKDChartDataModelAdapter::QicsKDChartDataModelAdapter(QObject *parent, QicsTable *table, const QicsRegion &reg)
    : QAbstractTableModel(parent),
        m_model(0),
        m_table(table),
        m_reg(reg),
        m_rowsReverted(false),
        m_colorsReverted(false)
{
    if (m_table)
        setModel(m_model = m_table->dataModel());
}

QicsKDChartDataModelAdapter::~QicsKDChartDataModelAdapter()
{
}

void QicsKDChartDataModelAdapter::setModel(QicsDataModel *model)
{
    m_model = model;

    if (m_model)
        connect(m_model, SIGNAL(modelChanged(QicsRegion)), this, SLOT(modelChanged(QicsRegion)));

    reset();
}

int QicsKDChartDataModelAdapter::rowCount ( const QModelIndex & ) const
{
    if (m_rowsReverted)
        return m_reg.isValid() ? m_reg.numColumns() : m_model ? m_model->numColumns() : 0;

    return m_reg.isValid() ? m_reg.numRows() : m_model ? m_model->numRows() : 0;
}

int QicsKDChartDataModelAdapter::columnCount ( const QModelIndex & ) const
{
    if (m_rowsReverted)
        return m_reg.isValid() ? m_reg.numRows() : m_model ? m_model->numRows() : 0;

    return m_reg.isValid() ? m_reg.numColumns() : m_model ? m_model->numColumns() : 0;
}

void QicsKDChartDataModelAdapter::setupIndex(int &row, int &col, const QModelIndex &index) const
{
    if (m_rowsReverted) {
        col = index.row();
        row = index.column();

        if (m_reg.isValid()) {
            col += m_reg.top();
            row += m_reg.left();
        }
    } else {
        row = index.row();
        col = index.column();

        if (m_reg.isValid()) {
            row += m_reg.top();
            col += m_reg.left();
        }
    }
}

bool QicsKDChartDataModelAdapter::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    if (!m_model) return false;

    int row, col;
    setupIndex(row, col, index);

    return QAbstractTableModel::setData(QAbstractTableModel::index(row, col), value, role);
}

QVariant QicsKDChartDataModelAdapter::data ( const QModelIndex & index, int role ) const
{
    if (!m_model) return QVariant();

    int row, col;

    setupIndex(row, col, index);

    QVariant ret;

    switch ( role ) {
        case Qt::DisplayRole:
            {
                QString data = m_model->itemString(row, col/*+1*/);
                bool b;
                data.toDouble(&b);
                if (b) return data;

                QRegExp reg("(\\d+)");
                int pos = 0;
                if ((pos = reg.indexIn(data, pos)) != -1)
                    return reg.cap(1);
                break;
            }
        case KDChart::DatasetBrushRole:
        case KDChart::DatasetPenRole: {
                QicsCell *cell = m_table->cell(row, col, true);
                if (cell) {
                    if (role == KDChart::DatasetBrushRole)
                        ret = QBrush(m_colorsReverted ? cell->foregroundColor() : cell->backgroundColor());
                    else
                        ret = QPen(m_colorsReverted ? cell->backgroundColor() : cell->foregroundColor());
                    delete cell;
                }
                break;
            }
        default:
            break;
    }
    return ret;
}

void QicsKDChartDataModelAdapter::modelChanged(QicsRegion r)
{
    if (m_reg.isValid())
        r.moveTopLeft(r.topLeft()-m_reg.topLeft());

    if (m_rowsReverted)
        emit dataChanged(index(r.startColumn(), r.startRow()), index(r.endColumn(), r.endRow()));
    else
        emit dataChanged(index(r.startRow(), r.startColumn()), index(r.endRow(), r.endColumn()));
}

void QicsKDChartDataModelAdapter::setRowsColsReverted(bool set)
{
    if (m_rowsReverted != set) {
        m_rowsReverted = set;
        modelChanged(m_reg);
    }
}

void QicsKDChartDataModelAdapter::setColorsReverted(bool set)
{
    if (m_colorsReverted != set) {
        m_colorsReverted = set;
        modelChanged(m_reg);
    }
}

void QicsKDChartDataModelAdapter::setDataRegion(const QicsRegion &dataRegion)
{
    m_reg = dataRegion;
    modelChanged(m_reg);
}


