/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "newdialog.h"


NewDialog::NewDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
}

NewDialog::~NewDialog()
{
}

int NewDialog::width() const
{
    return rbCustom->isChecked() ? sbW->value() :
        rb16->isChecked() ? 16 :
        rb24->isChecked() ? 24 :
        rb32->isChecked() ? 32 :
        -1;
}

int NewDialog::height() const
{
    return rbCustom->isChecked() ? sbH->value() :
        rb16->isChecked() ? 16 :
        rb24->isChecked() ? 24 :
        rb32->isChecked() ? 32 :
        -1;
}


