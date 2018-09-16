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

#ifndef QICSPRINTPREVIEWDIALOG_H
#define QICSPRINTPREVIEWDIALOG_H

#include <ui_QicsPrintPreviewDialog.h>
#include <QPrinter>

#include <QicsPrintDialog.h>

class QicsPrintPreviewWidget;
class QicsTable;

class QicsPrintPreviewDialog : public QDialog, private Ui::PreviewDialogClass
{
    Q_OBJECT
public:
    QicsPrintPreviewDialog(QWidget *parent = 0);
    ~QicsPrintPreviewDialog();

    inline const QPrinter&	printer() const
    { return m_printer; }

    bool setupPage();
    bool setupPrinter();

    inline QicsPrintPreviewWidget* previewWidget() const
    { return preview; }

    int exec();

    static int previewAndPrint(QicsTable *table);

public slots:
    void printAllPages();

private slots:
    void on_bPrinterSetup_clicked();
    void on_bPageSetup_clicked();
    void on_bPrint_clicked();

    void on_cbNumeration_activated(int);
    void on_cbScale_currentIndexChanged ( const QString & text );
    void on_cbFit_activated(int);
    void on_slCount_sliderMoved(int);

    void on_cbShowNumbers_toggled(bool);
    void on_cbJumpToActive_toggled(bool);
    void on_cbZoom_toggled(bool);
    void on_cbShowRulers_toggled(bool);

    void on_rbSelect_toggled(bool);
    void on_rbPan_toggled(bool);

    void on_preview_updatePerformed(const PageLayoutData &opt);
    void on_preview_pagePrinted(int i, QicsPrintPreviewPage *page);
    void on_preview_printingStarted(int totalPages);

    void on_preview_contextMenuRequest(QicsPrintPreviewPage *page, QContextMenuEvent* event);

private:
    QPrinter m_printer;
    QicsPrintDialog	*m_PrintDialog;
    QMenu *m_popup;
};

#endif //QICSPRINTPREVIEWDIALOG_H


