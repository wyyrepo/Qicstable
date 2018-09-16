/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef DISPLAYERS_H
#define DISPLAYERS_H

#include <QMainWindow>

class QTimer;

class Displayers : public QMainWindow
{
    Q_OBJECT

public:
    Displayers(QWidget *parent = 0);
    ~Displayers();

protected:
    void fillTable();

    void fillBasicDisplayers();
    void fillAdvancedDisplayers();

private slots:
    void onRunButtonClicked(int row, int col);
    void onTimer();

private:
    class QicsTable *m_table;

    int m_current_row;
    int m_progress_row;

    QStringList m_colors;
    QTimer *m_timer;
};


#endif //DISPLAYERS_H


