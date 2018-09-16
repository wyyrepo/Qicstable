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

#ifndef QICSTABLEDESIGNERTASKS_H
#define QICSTABLEDESIGNERTASKS_H

#include <QDesignerTaskMenuExtension>
#include <QExtensionManager>
#include <QExtensionFactory>
#include <QList>

class QicsTable;
class QAction;

class QicsTableDesignerTasks : public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
        Q_INTERFACES(QDesignerTaskMenuExtension)
public:
    QicsTableDesignerTasks(QicsTable *widget, QObject *parent = 0);
    ~QicsTableDesignerTasks();

    virtual QAction * preferredEditAction () const;
    virtual QList<QAction *> taskActions () const;

private slots:
    void designVisualAttributes();
    void designTable();
    void designRows();
    void designColumns();
    void loadSampleData();
    void loadSampleConfig();
    void saveSampleConfig();

private:
    QList <QAction *> m_tableActions;
    QicsTable* m_table;
};


class QicsTableDesignerTaskFactory : public QExtensionFactory
{
    Q_OBJECT
public:
    QicsTableDesignerTaskFactory(QExtensionManager *parent = 0);
    ~QicsTableDesignerTaskFactory();

protected:
    virtual QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const;
};


#endif //QICSTABLEDESIGNERTASKS_H


