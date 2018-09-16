#############################################################################
##
## Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for QicsTable.  This example
## program may be used, distributed and modified without limitation.
##
##############################################################################

TEMPLATE	= subdirs

CONFIG 		+= ordered

SUBDIRS		= dimensions \
                  displayers \
                  doubleTable \
                  mdiQicsTable \
                  widgetsTable \
                  advanced_spreadsheet \
                  qics_tetris
#KDCHART_NOOP
SUBDIRS    +=     kdchart \
                  kdchart2
#END_KDCHART_NOOP


