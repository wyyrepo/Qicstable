/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef QICSFILTERGROUPWIDGET_H
#define QICSFILTERGROUPWIDGET_H

#include "QicsPopupDialog.h"

class QicsTreeTable;
class QTreeWidget;

class QicsFilterGroupWidget : public QicsPopupDialog
{
    Q_OBJECT
public:
    QicsFilterGroupWidget(QicsTreeTable *table, QWidget *parent = 0);
    ~QicsFilterGroupWidget();

    void show(const QPoint &pos);

protected slots:
    void updateView();

protected:
    QicsTreeTable *m_table;
    QTreeWidget *m_list;
};

#endif //QICSFILTERGROUPWIDGET_H


