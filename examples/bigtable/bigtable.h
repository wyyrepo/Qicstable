/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef BIGTABLE_H
#define BIGTABLE_H

#include <QMainWindow>

class QicsTable;
class QicsDataModel;
class QComboBox;


class MainFrame: public QMainWindow
{
    Q_OBJECT
public:
    MainFrame(QWidget * parent = 0);

protected:
    virtual void closeEvent ( QCloseEvent * event );
    void refill(int type);
    QicsTable* createTable(QWidget *parent = 0);

signals:
    void modelUpdated();

private slots:
    void about();
    void fill();
    void run();
    void stop();
    void newTable();
    void newTableRegion();

private:
    QAction *m_actFill;
    QAction *m_actRun;
    QAction *m_actStop;
    QicsDataModel *m_dm;
    QicsTable *m_table;
    QComboBox *m_comboFill;
    bool m_progress;
    int m_viewNumber;
    int m_rowsCount;
    int m_columnsCount;
};

#endif //BIGTABLE_H


