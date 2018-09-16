/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef COMBOCHECK_H
#define COMBOCHECK_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    /*!
    *  Slot to process cell's value changes.
    */
    void on_cellValueChanged(int row, int col);

private:
    class QicsTable *m_table;
};

#endif //COMBOCHECK_H


