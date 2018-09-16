##############################################################################
##
## Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of the QicsTable software.
##
## See the top level README file for license terms under which this
## software can be used, distributed, or modified.
##
##############################################################################

QICSTABLE_PATH = ..


TEMPLATE = lib
#TARGET should always be defined before qicstable_config.pri included
TARGET = qicstabledesignerplugin

include($$QICSTABLE_PATH/qicstable_config.pri)

CONFIG += qt warn_on plugin

equals(QT_MAJOR_VERSION, 4): CONFIG += designer
equals(QT_MAJOR_VERSION, 5): QT += designer

win32::CONFIG += dll

QT += xml

DEFINES += QICS_XML

INCLUDEPATH += ./ $$QICSTABLE_PATH/include

DEPENDPATH +=  ./ $$QICSTABLE_PATH/include

LIBS        += -L$$QICSTABLE_PATH/lib -l$$QICSTABLELIB


HEADERS =   QicsTableDesignerPlugin.h \
        QicsTableDesignerTasks.h \
            TableDesigner.h \
            TableConfigDesigner.h \
            CellDesigner.h \
            RowColDesigner.h \
            ColorButton.h \
            FontButton.h

SOURCES =   QicsTableDesignerPlugin.cpp \
            QicsTableDesignerTasks.cpp \
            TableDesigner.cpp\
            TableConfigDesigner.cpp \
            CellDesigner.cpp \
            RowColDesigner.cpp \
            ColorButton.cpp \
            FontButton.cpp

FORMS =     CellDesigner.ui \
            TableConfigDesigner.ui \
            RowColDesigner.ui

# Install opts
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

unix | win32 {
    message(Plugin will be installed in: $$[QT_INSTALL_PLUGINS]/designer)
}



