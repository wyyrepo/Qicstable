/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef CHARMAP_H
#define CHARMAP_H

#include <QMainWindow>

class QicsDataModel;
class QicsTable;
class QLineEdit;

class MainFrame: public QMainWindow
{
    Q_OBJECT
public:
    MainFrame(QWidget * parent = 0);

private slots:
    void about();
    void updateFont(const QString &);
    void updateClipboard();
    void select();
    void cellClicked(int row, int col, int button, const QPoint &pos);
    void cellChanged(int row, int col);

private:
    QicsDataModel *m_dm;
    QicsTable *m_table;
    QLineEdit *m_lineEdit;
};

#endif //CHARMAP_H


