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

#
# Common settings for all QicsTable builds
#

# QicsTable Version
MAJOR_VERSION = 3
VERSION       = 3.0.0

# If you want to, set local config options in local_config.pri
exists(local_config.pri) {
    include(local_config.pri)
}

#CONFIG += eval_noop

# Do not change this line
CONFIG += qt
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# Chose one of the following two lines to configure the build

#CONFIG += release                 	# Compile and link QicsTable with debugging turned off.
#CONFIG += debug                   	# Compile and link QicsTable with debugging turned on.
CONFIG += debug_and_release build_all	# Compile and link two QicsTable libraries, with and without debugging turned on.

# Uncomment this line if you want to use saving to XML, designer plugin, advanced spreadsheet demo.
CONFIG += xml

# Configuration for MacOS X
macx {
    # Using gcc
    QMAKESPEC=macx-g++
    # uncoment this line if you want use xcode
    # QMAKESPEC=macx-xcode
}

win32 {
    QMAKE_CXXFLAGS *= /MP

    contains(QMAKE_HOST.arch, x86):{
        QMAKE_LFLAGS *= /MACHINE:X86
    }

    contains(QMAKE_HOST.arch, x86_64):{
        QMAKE_LFLAGS *= /MACHINE:X64
    }
}

BUILDDIR        = .build/
win32:BUILDDIR  = $$join(BUILDDIR,,,win32)
unix:BUILDDIR   = $$join(BUILDDIR,,,unix)
macx:BUILDDIR   = $$join(BUILDDIR,,,macx)

UI_DIR          = $${BUILDDIR}/ui
UIC_DIR         = $${BUILDDIR}/uic
MOC_DIR         = $${BUILDDIR}/moc
RCC_DIR         = $${BUILDDIR}/rcc
OBJECTS_DIR     = $${BUILDDIR}/obj

CONFIG(release, debug|release) {
    OBJECTS_DIR	= $$join(OBJECTS_DIR,,,/release)
}
else {
    OBJECTS_DIR	= $$join(OBJECTS_DIR,,,/debug)
}

CONFIG(debug, debug|release) {
    unix {
        TARGET = $$join(TARGET,,,_debug)
    }
    else {
        TARGET = $$join(TARGET,,,d)
    }
}

# Install prefix. Specifies path where QicsTable will be installed(with make install command).
# For unix systems default prefix is /usr/local, for Windows - $QTDIR.
# Libs will be  added into $$INSTALL_PREFIX/lib, includes into $$INSTALL_PREFIX/include.

unix {
        INSTALL_PREFIX = /usr/local
        INSTALL_LIB_SUFFIX = qicstable
        INSTALL_INCLUDE_SUFFIX = qicstable
}

win32 {
        INSTALL_PREFIX = $$(QTDIR)
        INSTALL_LIB_SUFFIX =
        INSTALL_INCLUDE_SUFFIX = qicstable
}


# Chose one of the following two lines to configure the build
#LIB_CONFIG = staticlib
LIB_CONFIG = dll

# QicsTable flags
QICSTABLELIB = qicstable

CONFIG(debug, debug|release) {
    unix: QICSTABLELIB = $$join(QICSTABLELIB,,,_debug)
    else: QICSTABLELIB = $$join(QICSTABLELIB,,,d)
}

contains(LIB_CONFIG, staticlib) {
    DEFINES += QICS_STATICLIB
} else {
    DEFINES += QICS_SHAREDLIB
        win32 {
            QICSTABLELIB = $$join(QICSTABLELIB,,,$$MAJOR_VERSION)
        }
}

#KDCHART_NOOP
# This line enables KDChart support
# You should set KDCHART_PATH=/full/path/to/kdchart

KDCHART_PATH =

!isEmpty(KDCHART_PATH) {
    DEFINES += BIND_KDCHART
    KDCHARTLIB = kdchart
    CONFIG(debug, debug|release) {
        !unix: KDCHARTLIB = "kdchartd"
    }
    INCLUDEPATH += $$KDCHART_PATH/include
    DEPENDPATH += $$KDCHART_PATH/include
    LIBS += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB
}
#END_KDCHART_NOOP
