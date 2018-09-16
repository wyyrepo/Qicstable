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

#ifndef QICSHTMLEXPORTDIALOG_H
#define QICSHTMLEXPORTDIALOG_H

#include "ui_QicsHTMLExportDialog.h"
#include <QicsHTMLExport.h>


class QicsHTMLExportDialog : public QDialog, private Ui::QicsHTMLExportDialogClass
{
    Q_OBJECT
public:
    QicsHTMLExportDialog(QWidget *parent = 0);
    ~QicsHTMLExportDialog();

    int	exec(QString &fileName, QicsHTMLExportOptions &options, class QicsTable *table = 0, bool autoExport = true);

    static QString exportTable(QicsTable *table, const QString &name = "table.html");

private slots:
    void on_tbPickPath_clicked();
};

#endif //QICSHTMLEXPORTDIALOG_H


