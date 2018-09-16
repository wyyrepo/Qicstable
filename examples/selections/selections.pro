#############################################################################
##
## Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for QicsTable. This example
## program may be used, distributed and modified without limitation.
##
##############################################################################

#TARGET should always be defined before qicstable_examples.pri included
TEMPLATE = app
TARGET   = selections

include(../qicstable_examples.pri)

HEADERS	= select.h

SOURCES	= select.cpp main.cpp

RESOURCES = select.qrc
