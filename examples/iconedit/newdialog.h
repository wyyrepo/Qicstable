/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include "ui_newdialog.h"


class NewDialog : public QDialog, private Ui::NewDialogClass
{
    Q_OBJECT
public:
    NewDialog(QWidget *parent = 0);
    ~NewDialog();

    int width() const;
    int height() const;
};

#endif // NEWDIALOG_H


