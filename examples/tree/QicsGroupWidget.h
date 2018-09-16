/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef QICSGROUPWIDGET_H
#define QICSGROUPWIDGET_H

#include <QLabel>

class QicsTreeTable;
class QicsFilterGroupWidget;

class QicsGroupWidget : public QLabel
{
    Q_OBJECT
public:
    QicsGroupWidget(QicsTreeTable *table, QWidget *parent = 0);
    ~QicsGroupWidget();

protected slots:
    void updateView();

protected:
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseDoubleClickEvent ( QMouseEvent * event );
    virtual void dragEnterEvent ( QDragEnterEvent * event );
    virtual void dropEvent ( QDropEvent * event );

private:
    QicsTreeTable *m_table;
    QicsFilterGroupWidget *m_dialog;
};

#endif //QICSGROUPWIDGET_H


