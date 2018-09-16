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

#include "QicsTableDesignerPlugin.h"

#include <QtPlugin>
#include <QDesignerFormEditorInterface>
#include <QIcon>

#include "QicsTableDesignerTasks.h"
#include <QicsTable.h>
#include <QicsDataModelDefault.h>


static const char *qicstable_pixmap[] =
{
    "22 22 11 1",
    "  c Gray100",
    ". c Gray97",
    "X c #4f504f",
    "b c #0000ff",
    "g c #00c000",
    "r c #c00000",
    "o c #00007f",
    "O c Gray0",
    "+ c none",
    "@ c Gray0",
    "# c Gray0",
    "++++++++++++++++++++++",
    "OOOOOOOOOOOOOOOOOOOOOO",
    "O                    O",
    "O   O                O",
    "O   O                O",
    "O   O          bb    O",
    "O   O          bb    O",
    "O   O          bb    O",
    "O   O  rr      bb    O",
    "O   O  rr      bb    O",
    "O   O  rr      bb    O",
    "O   O  rr  gg  bb    O",
    "O   O  rr  gg  bb    O",
    "O   O  rr  gg  bb    O",
    "O   O  rr  gg  bb    O",
    "O   O  rr  gg  bb    O",
    "O   O  rr  gg  bb    O",
    "O   O  rr  gg  bb    O",
    "O   OOOOOOOOOOOOOOO  O",
    "O                    O",
    "OOOOOOOOOOOOOOOOOOOOOO",
    "++++++++++++++++++++++"
};

QicsTableDesignerPlugin::QicsTableDesignerPlugin(QObject* parent)
    : QObject(parent),
        m_initialized(false),
        m_group("ICS Widgets"),
        m_iconSet(),
        m_includeFile("QicsTable.h"),
        m_toolTip("High Performance Table Widget"),
        m_whatsThis(),
        m_isContainer(false)
{
    Q_UNUSED(qicstable_pixmap);
}

void QicsTableDesignerPlugin::initialize(QDesignerFormEditorInterface * core)
{
    if (m_initialized)
        return;

    m_core = core;

    QExtensionManager *mgr = core->extensionManager();

    mgr->registerExtensions(new QicsTableDesignerTaskFactory(mgr),
        Q_TYPEID(QDesignerTaskMenuExtension));

    m_initialized = true;

}

bool QicsTableDesignerPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QicsTableDesignerPlugin::createWidget(QWidget *parent)
{
    QicsDataModelDefault* model = new QicsDataModelDefault(100,100);
    QicsTable* table = new QicsTable(model, parent);
    return table;
}

QString QicsTableDesignerPlugin::name() const
{
    return QLatin1String("QicsTable");
}

QString QicsTableDesignerPlugin::group() const
{
    return m_group;
}

QIcon QicsTableDesignerPlugin::icon() const
{
    return QIcon(QPixmap(qicstable_pixmap));
}

QString QicsTableDesignerPlugin::toolTip() const
{
    return m_toolTip;
}

QString QicsTableDesignerPlugin::whatsThis() const
{
    return m_whatsThis;
}

bool QicsTableDesignerPlugin::isContainer() const
{
    return m_isContainer;
}

QString QicsTableDesignerPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QicsTable\" name=\"table\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>10</width>\n"
        "   <height>10</height>\n"
        "  </rect>\n"
        " </property>\n"
        "</widget>");
}

QString QicsTableDesignerPlugin::includeFile() const
{
    return m_includeFile;
}

QString QicsTableDesignerPlugin::codeTemplate() const
{
    return QString();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qicstabledesignerplugin,QicsTableDesignerPlugin)
#endif


