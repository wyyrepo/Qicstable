/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <QtGui>
#include <QFileDialog>

#include "mdiChildTable.h"
#include <QicsDataModelDefault.h>

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    dm = new QicsDataModelDefault(10,10);
    this->setDataModel(dm);
    isUntitled = true;
}

void MdiChild::newFile()
{
    static int sequenceNumber = 1;
    isUntitled = true;
    curFile = tr("qicstable%1.dat").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");
    setWindowIcon(QIcon(":/images/new.png"));
}

bool MdiChild::loadFile(const QString &fileName)
{
    // Create the stream from the file, read into data model
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        dm->readASCII(stream, ',');
    }
    curFile = fileName;
    isUntitled = false;
    return true;
}

bool MdiChild::save()
{
    if (isUntitled == true)
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), curFile);
    if (fileName.isEmpty())
    {
        return false;
    }
    return saveFile(fileName);
}

bool MdiChild::saveFile(const QString &fileName)
{
    if (fileName != QString::null)
    {
        writeFile(fileName);
        isUntitled = false;
        return true;
    }
    return false;
}

void MdiChild::writeFile(QString fname)
{
    // Create the stream from the file, read into data model
    QFile file(fname);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        dm->writeASCII(stream, ',', 0, 0, 10, 10);
    }
}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    event = event;
    /*if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }*/
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
