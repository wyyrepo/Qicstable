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

#include <QicsHTMLExportDialog.h>

#include <QFileDialog>
#include <QMessageBox>

#include <QicsSelection.h>
#include <QicsTable.h>


QicsHTMLExportDialog::QicsHTMLExportDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
}

QicsHTMLExportDialog::~QicsHTMLExportDialog()
{
}

int	QicsHTMLExportDialog::exec(QString &fileName, QicsHTMLExportOptions &options, QicsTable *table, bool autoExport)
{
    leFileName->setText(fileName);

    cbAlignment->setChecked(options.align);
    cbColors->setChecked(options.coloring);
    cbDimensions->setChecked(options.dimensions);
    cbFlags->setChecked(options.flags);
    cbFont->setChecked(options.font);
    cbHeaders->setChecked(options.headers);
    cbImages->setChecked(options.pixmaps);
    cbWidgets->setChecked(options.widgets);
    cbSelection->setChecked(options.selection);
    lePixName->setText(options.pixNamingTemplate);
    leWidgetName->setText(options.widgetNamingTemplate);
    cbPixFormat->setEditText(options.pixFormat);
    sbPixQuality->setValue(options.pixQuality);
    cbFillEmpty->setChecked(options.fillEmpty);
    cbCellBorders->setChecked(options.borders);
    cbTableBorders->setChecked(options.tableBorders);

    cbSelection->setEnabled(options.coloring);
    lePixName->setEnabled(options.pixmaps);
    leWidgetName->setEnabled(options.widgets);

    rbSelected->setEnabled(true);
    bool sel = true;

    if (table) {
        QicsSelectionList *sl = table->selectionList(true);
        bool b = (sl && sl->count());
        if (!b) {   // no selections
            rbSelected->setEnabled(false);
            sel = false;
        }
    }

    rbSelected->setChecked(sel);
    rbTable->setChecked(!sel);

    if (QDialog::exec()) {

        fileName = leFileName->text();

        options.align = cbAlignment->isChecked();
        options.coloring = cbColors->isChecked();
        options.dimensions = cbDimensions->isChecked();
        options.flags = cbFlags->isChecked();
        options.font = cbFont->isChecked();
        options.headers = cbHeaders->isChecked();
        options.pixmaps = cbImages->isChecked();
        options.widgets = cbWidgets->isChecked();
        options.selection = cbSelection->isChecked();
        options.pixNamingTemplate = lePixName->text();
        options.widgetNamingTemplate = leWidgetName->text();
        options.pixFormat = cbPixFormat->currentText();
        options.pixQuality = sbPixQuality->value();
        options.fillEmpty = cbFillEmpty->isChecked();
        options.borders = cbCellBorders->isChecked();
        options.tableBorders = cbTableBorders->isChecked();

        if (!table)
            return true;

        if (rbSelected->isChecked())
            options.region = table->selectedRegion();

        if (rbData->isChecked())
            options.region = table->dataRegion();

        if (autoExport) {
            QMessageBox box(QMessageBox::Information, tr("Export in progress"), tr("Please wait - export in progress"));
            box.show();
            qApp->processEvents();
            QicsHTMLExport exporter(table, options);
            exporter.exportToFile(fileName);
        }
        return true;
    }

    return false;
}

void QicsHTMLExportDialog::on_tbPickPath_clicked()
{
    QString s = QFileDialog::getSaveFileName(this, tr("HTML file to export"),
        leFileName->text(), "*.htm?");
    if (!s.isEmpty())
        leFileName->setText(s);
}

QString QicsHTMLExportDialog::exportTable(QicsTable *table, const QString &name)
{
    QicsHTMLExportOptions opts;
    QString fileName(name);

    QicsHTMLExportDialog ed;

    if (ed.exec(fileName, opts, table))
        return fileName;

    return QString();
}


