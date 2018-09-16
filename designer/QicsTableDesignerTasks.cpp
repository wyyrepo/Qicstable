/*********************************************************************
**
** Copyright (C) 2002-2018 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#include "QicsTableDesignerTasks.h"

#include <QAction>
#include <QFileDialog>
#include <QTextStream>

#include "TableDesigner.h"
#include "TableConfigDesigner.h"
#include "RowColDesigner.h"


QicsTableDesignerTasks::QicsTableDesignerTasks(QicsTable *widget, QObject *parent)
    : QObject(parent)
{
    m_table = widget;

    QAction *qAction;

    qAction = new QAction("Design Visual Properties...", this);
    connect(qAction, SIGNAL(triggered()), this, SLOT(designVisualAttributes()));
    m_tableActions.append(qAction);

    qAction = new QAction("Design Table Properties...", this);
    connect(qAction, SIGNAL(triggered()), this, SLOT(designTable()));
    m_tableActions.append(qAction);

    qAction = new QAction("Design Columns...", this);
    connect(qAction, SIGNAL(triggered()), this, SLOT(designColumns()));
    m_tableActions.append(qAction);

    qAction = new QAction("Design Rows...", this);
    connect(qAction, SIGNAL(triggered()), this, SLOT(designRows()));
    m_tableActions.append(qAction);

    qAction = new QAction("Load Sample Data...", this);
    connect(qAction, SIGNAL(triggered()), this, SLOT(loadSampleData()));
    m_tableActions.append(qAction);

    qAction = new QAction("Load Configuration...", this);
    connect(qAction, SIGNAL(triggered()), this, SLOT(loadSampleConfig()));
    m_tableActions.append(qAction);

    qAction = new QAction("Save Configuration...", this);
    connect(qAction, SIGNAL(triggered()), this, SLOT(saveSampleConfig()));
    m_tableActions.append(qAction);
}

QicsTableDesignerTasks::~QicsTableDesignerTasks()
{
}

QAction*QicsTableDesignerTasks::preferredEditAction () const
{
    return (QAction *)0;
}

QList<QAction *> QicsTableDesignerTasks::taskActions() const
{
    return m_tableActions;
}

void QicsTableDesignerTasks::designVisualAttributes()
{
    TableDesigner* td = new TableDesigner(m_table,0);
    td->exec();
    delete td;
}

void QicsTableDesignerTasks::designTable()
{
    TableConfigDesigner* td = new TableConfigDesigner(m_table,0);
    td->exec();
    delete td;
}

void QicsTableDesignerTasks::designRows()
{
    RowColDesigner* rcd = new RowColDesigner(m_table, RowColDesigner::Rows , 0);
    rcd->exec();
    delete rcd;
}

void QicsTableDesignerTasks::designColumns()
{
    RowColDesigner* rcd = new RowColDesigner(m_table, RowColDesigner::Columns , 0);
    rcd->exec();
    delete rcd;
}

void QicsTableDesignerTasks::loadSampleData()
{
    QString fileName = QFileDialog::getOpenFileName(0, "Choose Sample Data File",
        QDir::homePath(), "QicsTable Data(*.csv)" );

    QFile file(fileName);

    if( file.open(QIODevice::ReadOnly) ) {
        QTextStream stream(&file);
        m_table->dataModel()->readASCII(stream, ',', 0, 0, true);
    }
}

void QicsTableDesignerTasks::loadSampleConfig()
{
    QString fileName = QFileDialog::getOpenFileName(0, "Load Config File",
        QDir::homePath(), "QicsTable Data(*.tcf)" );

    QFile file(fileName);

    if( file.open(QIODevice::ReadOnly) ) {
        QTextStream stream(&file);
        QString xml = stream.readAll();
        m_table->configureFromXml(xml);
    }
}

void QicsTableDesignerTasks::saveSampleConfig()
{
    QString fileName = QFileDialog::getSaveFileName(0, "Save Config File",
        QDir::homePath(), "QicsTable Data(*.tcf)" );

    QFile file(fileName);

    if( file.open(QIODevice::WriteOnly) ) {
        QTextStream stream(&file);
        stream << m_table->configurationToXml();
    }
}

//////////////////////////////////////////////////////////////////////////////

QicsTableDesignerTaskFactory::QicsTableDesignerTaskFactory(QExtensionManager *parent)
    : QExtensionFactory(parent)
{
}

QicsTableDesignerTaskFactory::~QicsTableDesignerTaskFactory()
{
}

QObject *QicsTableDesignerTaskFactory::createExtension(QObject *object,
                                                       const QString &iid,
                                                       QObject *parent) const
{
    if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
        return 0;

    QicsTable *w = qobject_cast<QicsTable*>(object);
    if(!w) return 0;

    return new QicsTableDesignerTasks(w, parent);
}


