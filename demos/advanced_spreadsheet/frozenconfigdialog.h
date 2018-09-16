/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef FROZENCONFIGDIALOG_H
#define FROZENCONFIGDIALOG_H

#include "ui_frozenconfigdialog.h"

class FrozenConfigDialog : public QDialog, private Ui::FrozenConfigDialogClass
{
    Q_OBJECT
public:
    FrozenConfigDialog(QWidget *parent = 0);
    ~FrozenConfigDialog();

    int exec(int top, int bottom,int left,int right);

    // These methods return the specific type of freeze that was requested
    bool freezeTopRows(void) const;
    bool freezeBottomRows(void) const;
    bool freezeLeftColumns(void) const;
    bool freezeRightColumns(void) const;

    // These methods return the number of frozen rows/columns that were requested
    int numTopFrozenRows(void) const;
    int numBottomFrozenRows(void) const;
    int numLeftFrozenCols(void) const;
    int numRightFrozenCols(void) const;
};

#endif //FROZENCONFIGDIALOG_H


