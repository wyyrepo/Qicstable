/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "finddialog.h"


FindDialog::FindDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);
    findEdit->setFocus();

    setWindowIcon(QIcon(":/images/find.png"));

    connect(findNextB,SIGNAL(clicked()),this,SLOT(handleFindNextClicked()));
    connect(findPrevB,SIGNAL(clicked()),this,SLOT(handleFindPrevClicked()));
    connect(replacePrevB,SIGNAL(clicked()),this,SLOT(handleReplacePrevClicked()));
    connect(replaceNextB,SIGNAL(clicked()),this,SLOT(handleReplaceNextClicked()));
}

void FindDialog::handleFindNextClicked()
{
    QString serch_string = findEdit->text();
    emit findRequest(serch_string);
}

void FindDialog::handleFindPrevClicked()
{
    QString serch_string = findEdit->text();
    emit findBackwardRequest(serch_string);
}

void FindDialog::handleReplacePrevClicked()
{
    QString which_string = findWhatEdit->text();
    QString replace_string = replaceWithEdit->text();
    emit replaceBackwardRequest(which_string,replace_string);
}

void FindDialog::handleReplaceNextClicked()
{
    QString which_string = findWhatEdit->text();
    QString replace_string = replaceWithEdit->text();
    emit replaceRequest(which_string,replace_string);
}


