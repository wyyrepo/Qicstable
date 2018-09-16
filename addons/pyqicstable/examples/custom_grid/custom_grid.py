#!/usr/bin/env python

## Copyright ( C ) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for PyQicsTable.  This example
## program may be used, distributed and modified without limitation.


import sys

from PyQt4 import QtCore, QtGui, Qt

from qicstable import QicsTable, QicsDataModelDefault, QicsDataInt, QicsTableGrid


class CustomGrid(QicsTableGrid):
    """
    This is an example of a custom table grid.  This grid acts like the default
    QicsTableGrid, except that it posts a popup menu when the user presses the
    right mouse button.
    """
    def __init__(self, w, info, top_row=0, left_column=0):
        QicsTableGrid.__init__(self, w, info, top_row, left_column)

        self._menu = QtGui.QMenu(self)
	self.cutAct = QtGui.QAction("Cut", w)
	self.connect(self.cutAct, QtCore.SIGNAL("triggered()"), QtCore.SLOT("cut()"))
	
	self.cutAct = QtGui.QAction("Copy", w)
	self.connect(self.cutAct, QtCore.SIGNAL("triggered()"), QtCore.SLOT("copy()"))
	
	self.cutAct = QtGui.QAction("Paste", w)
	self.connect(self.cutAct, QtCore.SIGNAL("triggered()"), QtCore.SLOT("paste()"))

    def createGrid(w, info, top_row=0, left_column=0):
        return CustomGrid(w, info, top_row, left_column)
    createGrid = staticmethod(createGrid)

    def handleMousePressEvent(self, cell, m):
        if m.button() == QtCore.Qt.RightButton:
            self._menu.popup(m.globalPos())
        else:
            QicsTableGrid.handleMousePressEvent(self, cell, m)


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)

    # dimensions of the data model
    numRows = 10
    numCols = 5

    # create the data model
    dm = QicsDataModelDefault(numRows, numCols)

    # populate the data model with some data
    for i in range(numRows):
        for j in range(numCols):
	        dm.setItem(i, j, QicsDataInt(i * j))

    # create the table, using the data model we created above
    table = QicsTable(dm)

    # make sure the table is only as large as necessary
    table.setVisibleRows(numRows)
    table.setVisibleColumns(numCols)

    # Add a title widget to the top of the table
    label = QtGui.QLabel("Hello World, Table", table)
    label.setAlignment(QtCore.Qt.AlignCenter)
    table.setTopTitleWidget(label)

#    app.setMainWidget(table)
    table.show()

    sys.exit(app.exec_())
