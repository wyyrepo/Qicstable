/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include "ui_setupdialog.h"

class QicsTable;

class SetupDialog : public QDialog, private Ui::SetupDialogClass
{
    Q_OBJECT
public:
    SetupDialog(QWidget *parent = 0);
    ~SetupDialog();

    int exec(const QList<QicsTable*>& tables);

private slots:
    void on_bApply_clicked();

private:
    static QString m_lastStyle;
    QList<QicsTable*> m_tables;
};

#endif //SETUPDIALOG_H


