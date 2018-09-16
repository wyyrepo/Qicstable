/*********************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#ifndef QICSPRINTDIALOG_H
#define QICSPRINTDIALOG_H

#include <ui_QicsPrintDialog.h>

class QicsPrintPreviewWidget;

class QicsPrintDialog : public QDialog, private Ui::QicsPrintDialogClass
{
    Q_OBJECT
public:
    QicsPrintDialog(QWidget *parent = 0);
    ~QicsPrintDialog();

    int	execute(QicsPrintPreviewWidget *preview);

private slots:
    void on_bPrint_clicked();
    void on_bPageSetup_clicked();
    void on_bPrinterSetup_clicked();

private:
    void doSetup();

private:
    QicsPrintPreviewWidget	*m_preview;
};

#endif //QICSPRINTDIALOG_H


