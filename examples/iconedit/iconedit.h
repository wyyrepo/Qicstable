/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef ICONEDIT_H
#define ICONEDIT_H

#include <QMainWindow>

class ColorPanel;
class ZoomPanel;
class QicsTable;
class QicsDataModel;

class MainFrame : public QMainWindow
{
    Q_OBJECT
public:
    MainFrame(QWidget *parent = 0);
    ~MainFrame();

signals:
    void iconChanged(const QImage& icon);

public slots:
    void about();
    void newFile();
    void save();
    void load();
    void createGrid(int w, int h);
    void setZoom(int z);
    void clearImage();

private slots:
    void cellClicked(int,int,int,const QPoint&);
    void cellChanged(int,int);

private:
    QicsTable *m_table;
    QicsDataModel *m_dm;
    ColorPanel *m_cpanel;
    ZoomPanel *m_zpanel;
    QImage m_icon;
};

#endif // ICONEDIT_H


