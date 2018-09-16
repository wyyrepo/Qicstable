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

#include <QicsPrintPreviewDialog.h>

#include <QMenu>
#include <QPageSetupDialog>
#include <QPrintDialog>

#include <QicsPrintPreviewPage.h>
#include <QicsTable.h>
#include <QicsTablePrint.h>


QicsPrintPreviewDialog::QicsPrintPreviewDialog(QWidget *parent)
    : QDialog(parent, Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint)
{
    setupUi(this);

    // numeration combo
    cbNumeration->addItem("Horizontal", LO_HORIZONTAL);
    cbNumeration->addItem("Vertical", LO_VERTICAL);

    // scale combo
    cbScale->blockSignals(true);
    cbScale->addItem("500%", 500);
    cbScale->addItem("400%", 400);
    cbScale->addItem("300%", 300);
    cbScale->addItem("200%", 200);
    cbScale->addItem("150%", 150);
    cbScale->addItem("100%", 100);
    cbScale->addItem("75%", 75);
    cbScale->addItem("50%", 50);
    cbScale->addItem("25%", 25);
    cbScale->addItem("20%", 20);
    cbScale->addItem("10%", 10);
    cbScale->blockSignals(false);

    // policy combo
    cbFit->addItem("Fixed scale / Number by width", LS_FIXED + 16*LC_FIXED + 256*LD_HORIZONTAL);
    cbFit->addItem("Fixed scale / Number by height", LS_FIXED + 16*LC_FIXED + 256*LD_VERTICAL);
    cbFit->addItem("Fixed scale / Fit by width", LS_FIXED + 16*LC_FIT + 256*LD_HORIZONTAL);
    cbFit->addItem("Fixed scale / Fit by height", LS_FIXED + 16*LC_FIT + 256*LD_VERTICAL);

    cbFit->addItem("Fit scale / Number by width", LS_FIT + 16*LC_FIXED + 256*LD_HORIZONTAL);
    cbFit->addItem("Fit scale / Number by height", LS_FIT + 16*LC_FIXED + 256*LD_VERTICAL);

    // popup menu
    m_popup = new QMenu(this);
    m_popup->addAction("Select All", preview, SLOT(selectAll()));
    m_popup->addAction("Deselect All", preview, SLOT(deselectAll()));

    m_popup->addSeparator();

    m_popup->addAction("Enable All", preview, SLOT(enableAll()));
    m_popup->addAction("Disable All", preview, SLOT(disableAll()));

    m_popup->addAction("Enable Selected", preview, SLOT(enableSelected()), Qt::CTRL + Qt::Key_E);
    m_popup->addAction("Disable Selected", preview, SLOT(disableSelected()), Qt::CTRL + Qt::Key_D);

    m_popup->addSeparator();


    m_PrintDialog = new QicsPrintDialog(this);

    preview->addActions(m_popup->actions());
}

QicsPrintPreviewDialog::~QicsPrintPreviewDialog()
{
}

bool QicsPrintPreviewDialog::setupPage()
{
    QPageSetupDialog psd(&m_printer);
    return psd.exec() == QDialog::Accepted;
}

bool QicsPrintPreviewDialog::setupPrinter()
{
    QPrintDialog psd(&m_printer);
    return psd.exec() == QDialog::Accepted;
}

int QicsPrintPreviewDialog::exec()
{
    cbShowNumbers->setChecked( preview->isPageNumbersVisible() );
    cbShowRulers->setChecked( preview->isRulersVisible() );
    cbJumpToActive->setChecked( preview->isAutoShowActivePage() );
    cbZoom->setChecked( preview->isClickZoomEnabled() );

    preview->updatePages(&m_printer);

    return QDialog::exec();
}

void QicsPrintPreviewDialog::printAllPages()
{
    progressBar->setValue(0);
    preview->printAllPages();
}

void QicsPrintPreviewDialog::on_bPrinterSetup_clicked()
{
    if (setupPrinter())
        preview->updatePages(&m_printer);
}

void QicsPrintPreviewDialog::on_bPageSetup_clicked()
{
    if (setupPage())
        preview->updatePages(&m_printer);
}

void QicsPrintPreviewDialog::on_cbNumeration_activated(int index)
{
    preview->setPageOrdering((PageLayoutOrdering)(cbNumeration->itemData(index).toInt()));
}

void QicsPrintPreviewDialog::on_cbScale_currentIndexChanged ( const QString & text )
{
    QString s = text;
    s = s.remove(" ");
    s = s.remove("%");
    bool ok;
    double scale = s.toDouble(&ok);
    if (ok && scale > 0)
        preview->setScale(scale);
    else
        cbScale->setEditText(QString::number(preview->scale(), 'f', 1)+"%");
}

void QicsPrintPreviewDialog::on_cbFit_activated(int index)
{
    int v = cbFit->itemData(index).toInt();

    preview->setScale(cbScale->itemData(cbScale->currentIndex()).toInt());

    preview->setPageScaling((PageLayoutScaling)(v & 15));
    preview->setPageCounting((PageLayoutCounting)((v >> 4) & 15));
    preview->setPageDirection((PageLayoutDirection)((v >> 8) & 15), slCount->value());

    //	slCount->setEnabled(preview->pageCounting() == LC_FIXED);
    //	cbScale->setEnabled(preview->pageScaling() == LC_FIXED);
}

void QicsPrintPreviewDialog::on_slCount_sliderMoved(int value)
{
    preview->setPageDirection(preview->pageDirection(), value);
}

void QicsPrintPreviewDialog::on_preview_updatePerformed(const PageLayoutData &opt)
{
    lPages->setText(QString("%1 Pages").arg(preview->pagesCount()));
    progressBar->setMaximum(preview->pagesCount());
    progressBar->setValue(preview->pagesCount());
    progressBar->setVisible(preview->pagesCount());


    cbNumeration->setCurrentIndex(cbNumeration->findData(opt.m_lo));
    int i = cbScale->findData(opt.m_zoomP);
    if (i < 0)
        cbScale->setEditText(QString::number(opt.m_zoomP, 'f', 1) + "%");
    else
        cbScale->setCurrentIndex(i);
    slCount->setValue(opt.m_ld == LD_HORIZONTAL ? opt.m_fitX : opt.m_fitY );
    int v = opt.m_ls + 16*opt.m_lc + 256*opt.m_ld;
    cbFit->setCurrentIndex(cbFit->findData(v));
    slCount->setEnabled(static_cast<int>(opt.m_lc) == static_cast<int>(LC_FIXED));
    cbScale->setEnabled(static_cast<int>(opt.m_ls) == static_cast<int>(LC_FIXED));
}

void QicsPrintPreviewDialog::on_preview_pagePrinted(int i, QicsPrintPreviewPage * /*page*/)
{
    progressBar->setValue(i);
}

void QicsPrintPreviewDialog::on_preview_printingStarted(int totalPages)
{
    progressBar->setMaximum(totalPages);
    progressBar->setValue(0);
    progressBar->setFormat(QString("Printed: %p% (%v of %1 pages)").arg(totalPages));
}

void QicsPrintPreviewDialog::on_cbShowNumbers_toggled(bool v)
{
    preview->showPageNumbers(v);
}

void QicsPrintPreviewDialog::on_cbShowRulers_toggled(bool v)
{
    preview->showRulers(v);
}

void QicsPrintPreviewDialog::on_cbJumpToActive_toggled(bool v)
{
    preview->autoShowActivePage(v);
}

void QicsPrintPreviewDialog::on_cbZoom_toggled(bool v)
{
    preview->enableClickZoom(v);
}

void QicsPrintPreviewDialog::on_preview_contextMenuRequest(QicsPrintPreviewPage * /*page*/, QContextMenuEvent* event)
{
    m_popup->popup(event->globalPos());
}

void QicsPrintPreviewDialog::on_rbSelect_toggled(bool b)
{
    if (b) preview->setControlMode(CM_SELECT);
}

void QicsPrintPreviewDialog::on_rbPan_toggled(bool b)
{
    if (b) preview->setControlMode(CM_PAN);
}

void QicsPrintPreviewDialog::on_bPrint_clicked()
{
    m_PrintDialog->execute(preview);
}


/////////////////////////////////////////////////////////
int QicsPrintPreviewDialog::previewAndPrint(QicsTable *table)
{
    QicsTablePrint *printTable = new QicsTablePrint(table);

    QicsPrintPreviewDialog ppd;

    printTable->setPreviewWidget(ppd.previewWidget());
    printTable->setPreviewRegion();

    int i = ppd.exec();

    delete printTable;
    return i;
}


