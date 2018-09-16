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
TARGET   = tree

include(../qicstable_examples.pri)


INCLUDEPATH += $$QICSTABLE_PATH/addons/table.tree

DEPENDPATH += $$QICSTABLE_PATH/addons/table.tree

HEADERS = icstreetable.h \
	  QicsFilterCellDisplay.h \
	  QicsFilterSortDialog.h \
	  QicsFilterGroupWidget.h \
	  QicsGroupWidget.h

SOURCES = icstreetable.cpp \
	  main.cpp \
	  QicsFilterCellDisplay.cpp \
	  QicsFilterSortDialog.cpp \
	  QicsFilterGroupWidget.cpp \
	  QicsGroupWidget.cpp

FORMS =  icstreetable.ui

RESOURCES = images.qrc



