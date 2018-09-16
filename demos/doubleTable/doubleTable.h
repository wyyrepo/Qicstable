/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef DOUBLETABLE_H
#define DOUBLETABLE_H

#include <QMainWindow>

#define COUNT_ROWS 10
#define COUNT_COLS 10

class QicsTable;
class QicsDataModel;
class QicsDataModelDefault;

class DoubleTable : public QMainWindow
{
    Q_OBJECT
public:
    DoubleTable(QWidget* parent, QicsDataModelDefault* dataModel, const QString& caption);
    ~DoubleTable();

protected:
    QicsDataModel *m_dataModel;
    QicsTable *m_table;
};

class Tester : public QObject
{
    Q_OBJECT
public:
    Tester(DoubleTable*, DoubleTable*, QicsDataModel*);
    ~Tester();

private slots:
    void changeItems();

private:
    DoubleTable *m_table1;
    DoubleTable *m_table2;
    QicsDataModel *m_dataModel;
    QTimer *m_timer;
};

#endif //DOUBLETABLE_H


