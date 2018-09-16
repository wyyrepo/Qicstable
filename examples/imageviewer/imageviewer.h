/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>

class QModelIndex;
class QDirModel;
class QTreeView;
class QProgressBar;
class QicsTable;
class QicsDataModel;

class ImageViewer: public QMainWindow
{
    Q_OBJECT
public:
    ImageViewer(QWidget * parent = 0);

protected:
    virtual void closeEvent (QCloseEvent * event);

private slots:
    void about();
    void formats();
    void cellClicked(int,int,int,const QPoint &);
    void dirSelected(const QModelIndex&);
    void stopScan();

private:
    QicsDataModel *m_dm;
    QicsTable *m_table;
    QDirModel *m_dirmodel;
    QTreeView *m_dirtree;
    QProgressBar *m_bar;
    QString m_dirName;
    QStringList m_filter;
    volatile bool m_scanning;
    volatile bool m_stop;
};

#endif //IMAGEVIEWER_H


