/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "frozenconfigdialog.h"


FrozenConfigDialog::FrozenConfigDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
}

FrozenConfigDialog::~FrozenConfigDialog()
{
}

int FrozenConfigDialog::exec(int top, int bottom,int left,int right)
{
    QString s;
    if (top>0) {
        s.setNum(top);
        cbFreezeTopRows->setChecked(true);
        leNumTopFrozenRows->setText(s);
    }

    if (bottom>0) {
        s.setNum(bottom);
        cbFreezeBottomRows->setChecked(true);
        leNumBottomFrozenRows->setText(s);
    }

    if (left>0) {
        s.setNum(left);
        cbFreezeLeftColumns->setChecked(true);
        leNumLeftFrozenCols->setText(s);
    }

    if (right>0) {
        s.setNum(right);
        cbFreezeRightColumns->setChecked(true);
        leNumRightFrozenCols->setText(s);
    }

    return QDialog::exec();
}

bool FrozenConfigDialog::freezeTopRows(void) const
{
    return cbFreezeTopRows->isChecked();
}

bool FrozenConfigDialog::freezeBottomRows(void) const
{
    return cbFreezeBottomRows->isChecked();
}

bool FrozenConfigDialog::freezeLeftColumns(void) const
{
    return cbFreezeLeftColumns->isChecked();
}

bool FrozenConfigDialog::freezeRightColumns(void) const
{
    return cbFreezeRightColumns->isChecked();
}

int FrozenConfigDialog::numTopFrozenRows(void) const
{
    return (leNumTopFrozenRows->text().toInt());
}

int FrozenConfigDialog::numBottomFrozenRows(void) const
{
    return (leNumBottomFrozenRows->text().toInt());
}

int FrozenConfigDialog::numLeftFrozenCols(void) const
{
    return (leNumLeftFrozenCols->text().toInt());
}

int FrozenConfigDialog::numRightFrozenCols(void) const
{
    return (leNumRightFrozenCols->text().toInt());
}


