#############################################################################
##
## Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for QicsTable.  This example
## program may be used, distributed and modified without limitation.
##
##############################################################################

#TARGET should always be defined before displayers_examples.pri included
TEMPLATE = app
TARGET   = pushbutton

include(../displayers_examples.pri)


SOURCES = main.cpp pushbutton.cpp

HEADERS = pushbutton.h


