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

#include <QicsPrintDialog.h>

#include <QPrintDialog>
#include <QPageSetupDialog>
#include <QMessageBox>

#include <QicsPrintPreviewWidget.h>
#include <QicsPrintPreviewPage.h>
#include <QicsPageMetrics.h>


QicsPrintDialog::QicsPrintDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowStaysOnTopHint)
{
    setupUi(this);
    setWindowModality(Qt::WindowModal);
}

QicsPrintDialog::~QicsPrintDialog()
{
}

int	QicsPrintDialog::execute(QicsPrintPreviewWidget *preview)
{
    if (!preview) return false;

    m_preview = preview;
    QPrinter *printer = preview->printer();

    if (!printer) return false;

    // setup dialog
    //lPrinterName->setText(printer->printerName());
    doSetup();

    cbPageSequence->setCurrentIndex(preview->printPageSequence());
    cbPageOrder->setCurrentIndex(preview->printPageOrder());

    sbCopies->setValue(preview->printCopies());
    cbCopiesOrder->setCurrentIndex(preview->printCopyOrder());

    switch (preview->printPageSet())
    {
    case PS_ALL:
        rbAllPages->setChecked(true);
        break;
    case PS_ENABLED:
        rbEnabledPages->setChecked(true);
        break;
    case PS_SELECTED:
        rbSelectedPages->setChecked(true);
        break;
    default:
        rbCustomPages->setChecked(true);
        break;
    }

    //rbCurrentPage->setEnabled(preview->activePage());

    cbPageOrientation->setCurrentIndex(printer->orientation());

    // execute
    //return exec();
    show();
    return true;
}

void QicsPrintDialog::on_bPrinterSetup_clicked()
{
    QPrintDialog psd(m_preview->printer());
    if (psd.exec() == QDialog::Accepted) {
        m_preview->updatePages(m_preview->printer());
        doSetup();
    }
}

void QicsPrintDialog::on_bPageSetup_clicked()
{
    QPageSetupDialog psd(m_preview->printer());
    if (psd.exec() == QDialog::Accepted) {
        m_preview->updatePages(m_preview->printer());
        doSetup();
    }
}

void QicsPrintDialog::doSetup()
{
    lPrinterName->setText(m_preview->printer()->printerName());
    cbPageOrientation->setCurrentIndex(m_preview->printer()->orientation());
    QPrinter::PageSize ps = m_preview->printer()->pageSize();
    QRect pr = m_preview->printer()->paperRect();

    if (ps < QPrinter::Custom)
        cbPageFormat->setEditText(QicsPageMetrics::paperString(ps));
    else {
        double w = (double)pr.width()/3.78, h = (double)pr.height()/3.78;
        int p = QicsPageMetrics::matchPaper(w, h);
        if (p == QPrinter::Custom)
            cbPageFormat->setEditText(QString("%1 x %2 mm").arg(w).arg(h));
        else
            cbPageFormat->setEditText(QicsPageMetrics::paperString(p));
    }
}

void QicsPrintDialog::on_bPrint_clicked()
{
    // initialize printing
    m_preview->setPrintPageSequence((PrintPageSequence)cbPageSequence->currentIndex());
    m_preview->setPrintPageOrder((PrintPageOrder)cbPageOrder->currentIndex());
    m_preview->setPrintCopyOrder((PrintCopyOrder)cbCopiesOrder->currentIndex());
    m_preview->setPrintCopies(sbCopies->value());

    PrintPageSet ps = PS_CUSTOM;
    if (rbAllPages->isChecked()) ps = PS_ALL; else
        if (rbEnabledPages->isChecked()) ps = PS_ENABLED; else
            if (rbSelectedPages->isChecked()) ps = PS_SELECTED; else
                if (rbCurrentPage->isChecked()) {
                    if (!m_preview->activePage()) {
                        QMessageBox::critical(this, "No Active Page", "No current page selected.");
                        return;
                    }

                    QList<int> p;
                    p << m_preview->activePage()->number();
                    m_preview->setPagesToPrint(p);
                } else {	// custom print - parse entered line
                    QList<int> p;
                    QStringList sl = leCustomPages->text().split(",", QString::SkipEmptyParts);
                    int v, num = m_preview->pagesCount();
                    bool ok, errors = false;
                    if (!sl.count())
                        errors = true;
                    for (int i = 0; i < sl.count(); ++i) {
                        v = sl[i].toInt(&ok);
                        if (ok) {
                            if (v > 0 && v <= num)
                                p.append(v);
                            else
                                errors = true;
                        } else {
                            QStringList sl2 = sl[i].split("-", QString::SkipEmptyParts);
                            if (sl2.count() != 2)
                                errors = true;
                            else {
                                int v1 = sl2[0].toInt(&ok);
                                if (ok && v1 > 0 && v1 <= num) {
                                    int v2 = sl2[1].toInt(&ok);
                                    if (ok && v2 > 0 && v2 <= num && v2 >= v1) {
                                        for (int j = v1; j <= v2; ++j)
                                            p.append(j);
                                    } else
                                        errors = true;
                                } else
                                    errors = true;
                            }
                        }
                    }
                    if (errors) {
                        QMessageBox::critical(this, tr("Invalid Page Range"), tr("Invalid page range specified, please check."));
                        return;
                    }
                    m_preview->setPagesToPrint(p);
                }

    m_preview->setPrintPageSet(ps);

    // print this stuff
    m_preview->print();

    accept();
}


