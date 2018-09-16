#############################################################################
##
## Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for QicsTable. This example
## program may be used, distributed and modified without limitation.
##
##############################################################################

#TARGET should always be defined before qicstable_demos.pri included
TEMPLATE = app
TARGET   = mdiQicsTable

include(../qicstable_demos.pri)

SOURCES = mainWindow.cpp mdiChildTable.cpp main.cpp
HEADERS = mainWindow.h mdiChildTable.h

RESOURCES = mdiQicsTable.qrc
