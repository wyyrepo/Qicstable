#!/usr/bin/env python

## Copyright ( C ) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for PyQicsTable.  This example
## program may be used, distributed and modified without limitation.


import sys

from PyQt4 import QtCore, QtGui, QtXml, Qt

from qicstable import QicsTable, QicsDataModelDefault, QicsDataInt


app = QtGui.QApplication(sys.argv)

# Dimensions of the data model.
numRows = 10
numCols = 5

# Create the data model.
dm = QicsDataModelDefault(numRows, numCols)

# Populate the data model with some data.
for i in range(numRows):
    for j in range(numCols):
        dm.setItem(i, j, QicsDataInt(i * j))

# Create the table, using the data model we created above.
table = QicsTable(dm)

# Make sure the table is only as large as necessary.
table.setVisibleRows(numRows)
table.setVisibleColumns(numCols)

# Add a title label
label = QtGui.QLabel("Hello world, table", table)

label.setAlignment(QtCore.Qt.AlignCenter)

# Uncoment to enable a button
## Add a Exit button
## button = QtGui.QPushButton("Exit", table)

## Connect button to slot quit
## QtCore.QObject.connect(button, QtCore.SIGNAL("clicked()"),
##                       app, QtCore.SLOT("quit()"))

table.setTopTitleWidget(label)

table.show()

sys.exit(app.exec_())
