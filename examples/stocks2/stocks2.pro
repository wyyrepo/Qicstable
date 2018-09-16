#############################################################################
##
## Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for QicsTable.  This example
## program may be used, distributed and modified without limitation.
##
##############################################################################

#TARGET should always be defined before qicstable_examples.pri included
TEMPLATE = app
TARGET   = stocks2

include(../qicstable_examples.pri)


#DEFINES += ADD_DATA

HEADERS =  $$QICSTABLE_PATH/addons/table.html/QicsHTMLExportDialog.h \
           stockapp.h \
           stockui.h \
           stockdata.h \
           stocksorterdelegate.h

#PRINTING_NOOP
HEADERS += $$QICSTABLE_PATH/addons/printing/QicsPrintDialog.h \
           $$QICSTABLE_PATH/addons/printing/QicsPrintPreviewDialog.h \
#END_PRINTING_NOOP

SOURCES =  main.cpp \
           $$QICSTABLE_PATH/addons/table.html/QicsHTMLExportDialog.cpp \
           stockapp.cpp \
           stockui.cpp \
           stockdata.cpp \
           stocksorterdelegate.cpp

#PRINTING_NOOP
SOURCES += $$QICSTABLE_PATH/addons/printing/QicsPrintDialog.cpp \
           $$QICSTABLE_PATH/addons/printing/QicsPrintPreviewDialog.cpp \
#END_PRINTING_NOOP

FORMS =   $$QICSTABLE_PATH/addons/table.html/QicsHTMLExportDialog.ui

#PRINTING_NOOP
FORMS +=  $$QICSTABLE_PATH/addons/printing/QicsPrintPreviewDialog.ui \
          $$QICSTABLE_PATH/addons/printing/QicsPrintDialog.ui
#END_PRINTING_NOOP


