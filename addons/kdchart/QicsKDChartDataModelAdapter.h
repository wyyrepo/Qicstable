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

#ifndef QICSKDCHARTDATAMODELADAPTER_H
#define QICSKDCHARTDATAMODELADAPTER_H

#include <QicsNamespace.h>

#include <QAbstractTableModel>
#include <QicsRegion.h>


class QicsDataModel;
class QicsTable;

class QICS_EXPORT QicsKDChartDataModelAdapter : public QAbstractTableModel
{
    Q_OBJECT
public:
    QicsKDChartDataModelAdapter(QObject *parent = 0,
        QicsTable *table = 0,
        const QicsRegion &reg = QicsRegion());
    ~QicsKDChartDataModelAdapter();

    void setModel(QicsDataModel *model);
    inline QicsDataModel* model() const
    { return m_model; }

    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;

    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    void setDataRegion(const QicsRegion &dataRegion);

    inline bool rowsColsReverted() const { return m_rowsReverted; }
    inline bool colorsReverted() const { return m_colorsReverted; }

public slots:
    void setRowsColsReverted(bool set);
    void setColorsReverted(bool set);

protected slots:
    void modelChanged(QicsRegion);

protected:
    void setupIndex(int &row, int &col, const QModelIndex &index) const;

    QicsTable *m_table;
    QicsDataModel *m_model;
    QicsRegion m_reg;
    bool m_rowsReverted, m_colorsReverted;
};

#endif //QICSDATAMODEL2QTMODELADAPTER_H


