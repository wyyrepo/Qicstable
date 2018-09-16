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

#Version check Qt
TOO_OLD_LIST=$$find(QT_VERSION, ^4\\.[0-6])
count(TOO_OLD_LIST, 1) {
    message("Cannot build the QicsTable with a Qt version that old:" $$QT_VERSION)
    error("Use at least Qt 4.7.")
}

contains(CONFIG, gpl_noop) {
    win32  {
       error(This GPL version of QicsTable cannot be built on MS Windows \
             systems because there is no recent version of Qt licensed \
         under the GPL. ICS provides commercial evaluations of \
         QicsTable that are technically and legally compatible with \
         the commercial versions of Qt available on MS Windows. \
             A commercial evaluation version for MS Windows systems is \
         available at: \
         http://www.ics.com/qt/qicstable/?cont=qicstableRegistration&v=win)
    }

    unix  {
      message(This is a GPL version of QicsTable.  Please read and understand \
          how GPL software can and cannot be used. \
          http://www.gnu.org/copyleft/gpl.html)
    }
} #END_GPL_NOOP

contains(CONFIG, eval_noop) {
    win32  {
      error("This source code evaluation is licensed for building on \
            UNIX/Linux systems only.  Therefore, the makefiles do not \
        provide support for building this source on MS Windows \
        systems. We do provide Commercial Evaluations \
        -- Binary only -- for MS Windows platforms at \
        http://www.ics.com/qt/qicstable/?cont=qicstableRegistration&v=win")
    }
} #END_EVAL_NOOP

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS  = src

#DESIGNER_NOOP
SUBDIRS += designer
#END_DESIGNER_NOOP

# developers can suppress building of examples by creating .no_examples
!exists(.no_examples) {
    SUBDIRS += examples
}

!exists(.no_demos) {
    SUBDIRS += demos
}
