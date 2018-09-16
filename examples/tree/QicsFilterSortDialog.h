/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef QICSFILTERSORTDIALOG_H
#define QICSFILTERSORTDIALOG_H

#include <QicsGroupBar.h>


class QicsFilterSortDialog : public QicsGroupBarFilterDialog
{
    Q_OBJECT
public:
    QicsFilterSortDialog(QicsTreeTable *parent = 0);
    ~QicsFilterSortDialog();

public slots:
    void on_pbSortA_clicked();
    void on_pbSortD_clicked();
    void on_pbSortBar_clicked();
};

#endif //QICSFILTERSORTDIALOG_H


