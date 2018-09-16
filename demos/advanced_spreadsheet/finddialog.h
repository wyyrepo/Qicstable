/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include "ui_finddialog.h"

/* Class that represents search/replace dialog
* Performs forward and backward search and replace.
*/
class FindDialog : public QDialog, private Ui::FindDialog
{
    Q_OBJECT
public:
    FindDialog(QWidget* parent = 0);

signals:
    void findRequest(const QString& str);
    void findBackwardRequest(const QString& str);
    void replaceRequest(const QString& ,const QString&);
    void replaceBackwardRequest(const QString& ,const QString&);

protected slots:
    void handleFindNextClicked();
    void handleFindPrevClicked();
    void handleReplacePrevClicked();
    void handleReplaceNextClicked();
};

#endif //FINDDIALOG_H


