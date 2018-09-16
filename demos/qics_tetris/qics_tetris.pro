#############################################################################
##
## Copyright ( C ) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for QicsTable.  This example
## program may be used, distributed and modified without limitation.
##
##############################################################################

#TARGET should always be defined before qicstable_demos.pri included
TEMPLATE = app
TARGET   = tetris

include(../qicstable_demos.pri)

SOURCES = main.cpp tetris.cpp form.cpp viewOptionsDialog.cpp viewSettings.cpp

HEADERS = tetris.h form.h viewOptionsDialog.h viewSettings.h

FORMS   = OptionsDialog.ui


