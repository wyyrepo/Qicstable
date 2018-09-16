/*********************************************************************
**
** Copyright (C) 2002-2016 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#ifndef QICSTABLEDESIGNERPLUGIN_H
#define QICSTABLEDESIGNERPLUGIN_H

#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#else
#include <QDesignerCustomWidgetInterface>
#endif
#include <QtCore/qplugin.h>
#include <QIcon>

class QDesignerFormEditorInterface;

class QicsTableDesignerPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.ics.Qt.QicsTableDesignerPlugin")
#endif
public:
    QicsTableDesignerPlugin(QObject *parent=0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString codeTemplate() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    QDesignerFormEditorInterface* m_core;
    bool m_initialized;
    QString m_group;
    QString m_iconSet;
    QString m_includeFile;
    QString m_toolTip;
    QString m_whatsThis;
    bool m_isContainer;
};

#endif //QICSTABLEDESIGNERPLUGIN_H


