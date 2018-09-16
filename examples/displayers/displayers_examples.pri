#############################################################################
##
## Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for QicsTable.  This example
## program may be used, distributed and modified without limitation.
##
##############################################################################

# Common settings for all QicsTable displayers examples

# NOTE:  Assumes this file is included from a directory directly below
#        this one, and that this directory is directly below the root
#        QicsTable directory.

QICSTABLE_PATH = ../../..

include(../../qicstable_config.pri)

#
# You might think we could just add "-L../../lib" to LIBDIRS, but
# a bug in older versions of qmake will cause builds on Windows
# to fail if we do it the "right" way.
#

INCLUDEPATH += \
            $$QICSTABLE_PATH/include \
            $$QICSTABLE_PATH/addons/table.html \
            $$QICSTABLE_PATH/addons/printing

DEPENDPATH += \
            $$QICSTABLE_PATH/include \
            $$QICSTABLE_PATH/addons/table.html \
            $$QICSTABLE_PATH/addons/printing

LIBS        += -L$$QICSTABLE_PATH/lib -l$$QICSTABLELIB


