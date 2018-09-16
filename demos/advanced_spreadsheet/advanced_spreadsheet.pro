#############################################################################
##
## Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for QicsTable.  This example
## program may be used, distributed and modified without limitation.
##
##############################################################################

#TARGET should always be defined before qicstable_demos.pri included
TEMPLATE = app
TARGET   = advanced_spreadsheet

include(../qicstable_demos.pri)


# Comment line below if you want to disallow for spreadsheet formula calculations.
DEFINES += _WITH_CALCULATIONS

# Comment line below if you want disable 'smart headers' - automatic headers highlighting in
# depending of selection.
DEFINES += _WITH_SMART_HEADERS

DEFINES += QICS_XML

SOURCES =  main.cpp \
           mainwindow.cpp \
           spreadsheetcalculator.cpp \
           spreadsheetcelldisplay.cpp \
           childwindow.cpp \
           spreadsheetgrid.cpp \
           spreadsheetdm.cpp \
           spreadsheettable.cpp \
           frozenconfigdialog.cpp \
           finddialog.cpp \
           setupdialog.cpp \
           spreadsheetformatter.cpp \
           $$QICSTABLE_PATH/addons/table.html/QicsHTMLExportDialog.cpp

#PRINTING_NOOP
SOURCES += $$QICSTABLE_PATH/addons/printing/QicsPrintDialog.cpp \
           $$QICSTABLE_PATH/addons/printing/QicsPrintPreviewDialog.cpp
#END_PRINTING_NOOP

HEADERS =  mainwindow.h \
           spreadsheetcalculator.h \
           spreadsheetcelldisplay.h \
           childwindow.h \
           spreadsheetgrid.h \
           spreadsheetdm.h \
           spreadsheettable.h \
           frozenconfigdialog.h \
           finddialog.h \
           setupdialog.h \
           spreadsheetformatter.h \
           $$QICSTABLE_PATH/addons/table.html/QicsHTMLExportDialog.h


#PRINTING_NOOP
HEADERS += $$QICSTABLE_PATH/addons/printing/QicsPrintDialog.h \
           $$QICSTABLE_PATH/addons/printing/QicsPrintPreviewDialog.h
#END_PRINTING_NOOP

FORMS =  finddialog.ui \
         frozenconfigdialog.ui \
         setupdialog.ui \
         $$QICSTABLE_PATH/addons/table.html/QicsHTMLExportDialog.ui

#PRINTING_NOOP
FORMS += $$QICSTABLE_PATH/addons/printing/QicsPrintPreviewDialog.ui \
         $$QICSTABLE_PATH/addons/printing/QicsPrintDialog.ui
#END_PRINTING_NOOP

RESOURCES = res.qrc

#KDCHART_NOOP
contains(DEFINES, BIND_KDCHART) {
       HEADERS += kdchartobject.h

       SOURCES += kdchartobject.cpp
}
#END_KDCHART_NOOP



