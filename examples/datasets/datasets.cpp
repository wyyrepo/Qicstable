/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "datasets.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStandardItemModel>
#include <QicsDataModelQtModelAdapter.h>
#include <QicsDataModelDefault.h>
#include <QicsTable.h>


DataSets::DataSets(QWidget *parent)
    : QWidget(parent),
    m_model1(0), m_model2(0), m_model3(0),
    m_model4(0), m_model5(0), m_model6(0)
{
    setWindowTitle(tr("Data Set Changing Example"));

    QVBoxLayout* layout = new QVBoxLayout(this);

    QPushButton *pbDataSet1 = new QPushButton(tr("Data Set 1"), this);
    QPushButton *pbDataSet2 = new QPushButton(tr("Data Set 2"), this);
    QPushButton *pbDataSet3 = new QPushButton(tr("Data Set 3"), this);
    QPushButton *pbDataSet4 = new QPushButton(tr("Data Set 4"), this);
    QPushButton *pbDataSet5 = new QPushButton(tr("Data Set 5"), this);
    QPushButton *pbDataSet6 = new QPushButton(tr("Data Set 6"), this);

    QHBoxLayout* dataSetsLayout = new QHBoxLayout;
    dataSetsLayout->addWidget(pbDataSet1);
    dataSetsLayout->addWidget(pbDataSet2);
    dataSetsLayout->addWidget(pbDataSet3);
    dataSetsLayout->addWidget(pbDataSet4);
    dataSetsLayout->addWidget(pbDataSet5);
    dataSetsLayout->addWidget(pbDataSet6);

    m_qicsTable = new QicsTable(this);

    m_label = new QLabel(m_qicsTable);
    m_label->setAlignment(Qt::AlignCenter);
    m_qicsTable->setTopTitleWidget(m_label);

    layout->addWidget(m_qicsTable);
    layout->addLayout(dataSetsLayout);

    m_icsModel = new QicsDataModelQtModelAdapter(this);

    connect(pbDataSet1, SIGNAL(clicked()), SLOT(dataSet1()));
    connect(pbDataSet2, SIGNAL(clicked()), SLOT(dataSet2()));
    connect(pbDataSet3, SIGNAL(clicked()), SLOT(dataSet3()));
    connect(pbDataSet4, SIGNAL(clicked()), SLOT(dataSet4()));
    connect(pbDataSet5, SIGNAL(clicked()), SLOT(dataSet5()));
    connect(pbDataSet6, SIGNAL(clicked()), SLOT(dataSet6()));

    dataSet1();
}

DataSets::~DataSets()
{
}

void DataSets::dataSet1()
{
    if (!m_model1) {
        m_model1 = new QicsDataModelDefault(10,5,this);
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 5; ++j)
                m_model1->setItem(i, j, QicsDataInt(i*j));
    }

    m_qicsTable->setDataModel(m_model1);
    m_qicsTable->setVisibleRows(10);
    m_qicsTable->setVisibleColumns(5);

    m_label->setText(tr("Data Set 1 - QicsTable model"));
}

void DataSets::dataSet2()
{
    if (!m_model2) {
        m_model2 = new QicsDataModelDefault(20,10,this);
        for (int i = 0; i < 20; ++i)
            for (int j = 0; j < 10; ++j)
                m_model2->setItem(i, j, QicsDataDouble(double(i*j)/2.1));
    }

    m_qicsTable->setDataModel(m_model2);
    m_qicsTable->setVisibleRows(10);
    m_qicsTable->setVisibleColumns(5);

    m_label->setText(tr("Data Set 2 - QicsTable model"));
}

void DataSets::dataSet3()
{
    if (!m_model3) {
        m_model3 = new QicsDataModelDefault(30,15,this);
        for (int i = 0; i < 30; ++i)
            for (int j = 0; j < 15; ++j)
                m_model3->setItem(i, j, QicsDataString(QString("%1:%2").arg(i).arg(j)));
    }

    m_qicsTable->setDataModel(m_model3);
    m_qicsTable->setVisibleRows(10);
    m_qicsTable->setVisibleColumns(5);

    m_label->setText(tr("Data Set 3 - QicsTable model"));
}

void DataSets::dataSet4()
{
    if (!m_model4) {
        m_model4 = new QStandardItemModel(40, 20, this);
        for (int i = 0; i < 40; ++i)
            for (int j = 0; j < 20; ++j) {
                QModelIndex index = m_model4->index(i, j);
                m_model4->setData(index, i*j);
            }
    }

    m_icsModel->setModel(m_model4);
    m_qicsTable->setDataModel(m_icsModel);
    m_qicsTable->setVisibleRows(10);
    m_qicsTable->setVisibleColumns(5);

    m_label->setText(tr("Data Set 4 - Qt model"));
}

void DataSets::dataSet5()
{
    if (!m_model5) {
        m_model5 = new QStandardItemModel(50, 25, this);
        for (int i = 0; i < 50; ++i)
            for (int j = 0; j < 25; ++j) {
                QModelIndex index = m_model5->index(i, j);
                m_model5->setData(index, double(i*j)/2.1);
            }
    }

    m_icsModel->setModel(m_model5);
    m_qicsTable->setDataModel(m_icsModel);
    m_qicsTable->setVisibleRows(10);
    m_qicsTable->setVisibleColumns(5);

    m_label->setText(tr("Data Set 5 - Qt model"));
}

void DataSets::dataSet6()
{
    if (!m_model6) {
        m_model6 = new QStandardItemModel(60, 30, this);
        for (int i = 0; i < 60; ++i)
            for (int j = 0; j < 35; ++j) {
                QModelIndex index = m_model6->index(i, j);
                m_model6->setData(index, QString("%1:%2").arg(i).arg(j));
            }
    }

    m_icsModel->setModel(m_model6);
    m_qicsTable->setDataModel(m_icsModel);
    m_qicsTable->setVisibleRows(10);
    m_qicsTable->setVisibleColumns(5);

    m_label->setText(tr("Data Set 6 - Qt model"));
}


