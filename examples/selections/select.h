/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SELECT_H
#define SELECT_H

#include <QMainWindow>

class QicsDataModel;
class QicsTable;
class QLabel;

class MainFrame: public QMainWindow
{
    Q_OBJECT
public:
    MainFrame(QWidget * parent = 0);

private slots:
    void about();
    void modeSelectNone();
    void modeSelectSingle();
    void modeSelectionMultiple();
    void modeSelectSingleRow();
    void modeSelectMultipleRow();
    void modeSelectExclusive(bool on);

private:
    QicsDataModel *m_dm;
    QicsTable *m_table;
    QLabel *m_label;
};

#endif //SELECT_H


