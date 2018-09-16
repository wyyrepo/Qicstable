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
TARGET   = iconedit

include(../qicstable_examples.pri)

HEADERS += colorpanel.h \
           iconedit.h \
           newdialog.h \
           zoompanel.h \
           rubbergrid.h


SOURCES =  colorpanel.cpp \
           iconedit.cpp \
           main.cpp \
           newdialog.cpp \
           zoompanel.cpp \
           rubbergrid.cpp

FORMS = newdialog.ui

RESOURCES = iconedit.qrc
