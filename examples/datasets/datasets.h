/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef DATASETS_H
#define DATASETS_H

#include <QWidget>

class QicsTable;
class QicsDataModelDefault;
class QicsDataModelQtModelAdapter;
class QAbstractItemModel;
class QLabel;


class DataSets : public QWidget
{
    Q_OBJECT
public:
    DataSets(QWidget *parent = 0);
    ~DataSets();

private:
    QicsTable *m_qicsTable;
    QicsDataModelDefault *m_model1;
    QicsDataModelDefault *m_model2;
    QicsDataModelDefault *m_model3;
    QAbstractItemModel *m_model4;
    QAbstractItemModel *m_model5;
    QAbstractItemModel *m_model6;
    QicsDataModelQtModelAdapter *m_icsModel;
    QLabel *m_label;

private slots:
    void dataSet1();
    void dataSet2();
    void dataSet3();
    void dataSet4();
    void dataSet5();
    void dataSet6();
};

#endif //DATASETS_H


