#!/usr/bin/env python
# This program demonstrate a FROZEN functionality with PyQicsTable

# Used modules

import sys

# Needed for create Gui

from PyQt4 import Qt, QtCore, QtGui, QtXml

# PyQicsTable modules

from qicstable import Qics, QicsTable, QicsDataModelDefault, QicsDataModel

# We need sip

try:
    import sip
except ImportError:
    import libsip as sip

# The dialog that allows the user to move rows and columns

class QicsFrozenConfigDialog(QtGui.QDialog):
    def __init__(self, name, parent=None):
        QtGui.QDialog.__init__(self, parent)

        box = QtGui.QVBoxLayout(self)

        grid = QtGui.QGridLayout()
        freezeColumn = 1
        numColumn = 3
	box.addLayout(grid)
	
        grid.setColumnStretch(2, 10)
	
	
        label = QtGui.QLabel(self)
        label.setText("How many?")
        grid.addWidget(label, 0, numColumn)

        # Top Frozen row data.
        self._frTop = QtGui.QCheckBox(self)
        self._frTop.setText("Freeze Top Rows")
        grid.addWidget(self._frTop, 1, freezeColumn)

        self._top = QtGui.QLineEdit(self)
        grid.addWidget(self._top, 1, numColumn)

        # Bottom Frozen row data.
        self._frBottom = QtGui.QCheckBox(self)
        self._frBottom.setText("Freeze Bottom Rows")
        grid.addWidget(self._frBottom, 2, freezeColumn)

        self._bottom = QtGui.QLineEdit(self)
        grid.addWidget(self._bottom, 2, numColumn)

        # Left Frozen column data.
        self._frLeft = QtGui.QCheckBox(self)
        self._frLeft.setText("Freeze Left Columns")
        grid.addWidget(self._frLeft, 3, freezeColumn)

        self._left = QtGui.QLineEdit(self)
        grid.addWidget(self._left, 3, numColumn)

        # Right Frozen column data.
        self._frRight = QtGui.QCheckBox(self)
        self._frRight.setText("Freeze Right Columns")
        grid.addWidget(self._frRight, 4, freezeColumn)

        self._right = QtGui.QLineEdit(self)
        grid.addWidget(self._right, 4, numColumn)

	grid.setRowStretch(3, 10)
	
	row = QtGui.QHBoxLayout()
	
	ok = QtGui.QPushButton("OkButton", self)
	ok.setText("OK")
	row.addWidget(ok)
        
	self.connect(ok, QtCore.SIGNAL("clicked()"), self, QtCore.SLOT("accept()"))
	
	cencel = QtGui.QPushButton("CencelButton", self)
	cencel.setText("Close")
	row.addWidget(cencel)
	
	self.connect(cencel, QtCore.SIGNAL("clicked()"), self, QtCore.SLOT("reject()"))
	box.addLayout(row)

    # This methods return the specific type of freeze that was requested
    
    def freezeTopRows(self):
	return self._freezeTopRows
    def freezeBottomRows(self):
	return self._freezeBottomRows
    def freezeLeftColumns(self):
	return self._freezeLeftColumns
    def freezeRightColumns(self):
	return self._freezeRightColumns
	
    # This methods return number of frozen rows/columns that was requsted
    # NOTE: method toInt() return an list : (int, bool), where was int is converted, bool result

    def numTopFrozenRows(self):
	return self._top.text().toInt()[0]
    def numBottomFrozenRows(self):
	return self._bottom.text().toInt()[0]
    def numLeftFrozenCols(self):
	return self._left.text().toInt()[0]
    def numRightFrozenCols(self):
	return self._right.text().toInt()[0]
    def exec_(self):
	self._freezeTopRows = False
	self._freezeBottomRows = False
	self._freezeLeftColumns = False
	self._freezeRightColumns = False
	
	if QtGui.QDialog.exec_(self) == QtGui.QDialog.Accepted:
	
	    if self._frTop.isChecked():
		self._freezeTopRows = True
	    if self._frBottom.isChecked():
		self._freezeBottomRows = True
	    if self._frLeft.isChecked():
		self._freezeLeftColumns = True
	    if self._frRight.isChecked():
		self._freezeRightColumns = True
# End of class QicsFrozenDialog

# The dialog that allows the user to move rows and columns

class MoveDialog(QtGui.QDialog):
    def __init__(self, name, parent=None):
        QtGui.QDialog.__init__(self, parent)

        box = QtGui.QVBoxLayout(self)

        bg = QtGui.QButtonGroup(box)

        self._mTop = QtGui.QRadioButton()
        self._mTop.setText("Move Rows to Top")

        self._mBottom = QtGui.QRadioButton()
        self._mBottom.setText("Move Rows to Bottom")

        self._mLeft = QtGui.QRadioButton()
        self._mLeft.setText("Move Columns to Left")

        self._mRight = QtGui.QRadioButton()
        self._mRight.setText("Move Columns to Right")
	
	bg.addButton(self._mTop)
	bg.addButton(self._mBottom)
	bg.addButton(self._mLeft)
	bg.addButton(self._mRight)
	
        box.addWidget(self._mTop)
	box.addWidget(self._mBottom)
	box.addWidget(self._mLeft)
	box.addWidget(self._mRight)

        row = QtGui.QHBoxLayout()

        ok = QtGui.QPushButton("OkButton", self)
        ok.setText("OK")
        row.addWidget(ok)
        self.connect(ok, QtCore.SIGNAL("clicked()"), self, QtCore.SLOT("accept()"))

        cancel = QtGui.QPushButton("CancelButton", self)
        cancel.setText("Cancel")
        row.addWidget(cancel)
        self.connect(cancel, QtCore.SIGNAL("clicked()"), self, QtCore.SLOT("reject()"))
	
	box.addLayout(row)
    def moveToTop(self):
        return self._moveToTop

    def moveToBottom(self):
        return self._moveToBottom
	
    def moveToLeft(self):
        return self._moveToLeft
	
    def moveToRight(self):
        return self._moveToRight
	
    def exec_(self):
        self._moveToTop = False
        self._moveToBottom = False
        self._moveToLeft = False
        self._moveToRight = False

	if QtGui.QDialog.exec_(self) == QtGui.QDialog.Accepted:
	    print "--Method-Move--"
    	    if self._mTop.isChecked():
        	self._moveToTop = True

	    if self._mBottom.isChecked():
        	self._moveToBottom = True

	    if self._mLeft.isChecked():
		self._moveToLeft = True

	    if self._mRight.isChecked():
		self._moveToRight = True
class FrozenUI(QtCore.QObject):
    def __init__(self,dm):
	QtCore.QObject.__init__(self)
	
	self.myMainWindow = QtGui.QMainWindow()

        self.myTable = QicsTable(dm, self.myMainWindow)

        # Set some labels on the row and column headers.

        for i in range(dm.numRows()):
            self.myTable.rowHeaderRef().cellRef(i,0).setLabel(QtCore.QString("Row %1").arg(i))

        for i in range(dm.numColumns()):
            self.myTable.columnHeaderRef().cellRef(0,i).setLabel(QtCore.QString("Col %1").arg(i))

        self.myMainWindow.setCentralWidget(self.myTable)

        # Dialogs for use in the future.

        self.myFrozenDialog = QicsFrozenConfigDialog("frozenConfigDialolg",
                                                     self.myMainWindow)

        self.myMoveDialog = MoveDialog("moveDialog", self.myMainWindow)

        # Menu system.

        menu = self.myMainWindow.menuBar()

        file = QtGui.QMenu("&File",self.myMainWindow)

	self.exitAct = QtGui.QAction("E&xit", self)

	self.connect(self.exitAct, QtCore.SIGNAL("triggered()"), self.destroy)
	file.addAction(self.exitAct)
        menu.addMenu(file)

        view = QtGui.QMenu("View", self.myMainWindow)

	self.actFreeze = QtGui.QAction("F&reeze...", self)
	self.connect(self.actFreeze, QtCore.SIGNAL("triggered()"), self.doFrozenConfig)
	
	self.actMove = QtGui.QAction("&Move", self)
	self.connect(self.actMove, QtCore.SIGNAL("triggered()"), self.doMoveConfig)
	
	view.addAction(self.actFreeze)
	view.addAction(self.actMove)

        menu.addMenu(view)
	
	self.myMainWindow.show()
	
    def base(self):
	return self.MainWindow
    def table(self):
	return self.myTable
    def doFrozenConfig(self):
        # Pop up the frozen dialog, wait until the user dismisses it.

        self.myFrozenDialog.exec_()

        # Now do what the user requested.

        if self.myFrozenDialog.freezeTopRows():
            self.myTable.freezeTopRows(self.myFrozenDialog.numTopFrozenRows())

        if self.myFrozenDialog.freezeBottomRows():
            self.myTable.freezeBottomRows(self.myFrozenDialog.numBottomFrozenRows())

        if self.myFrozenDialog.freezeLeftColumns():
            self.myTable.freezeLeftColumns(self.myFrozenDialog.numLeftFrozenCols())

        if self.myFrozenDialog.freezeRightColumns():
            self.myTable.freezeRightColumns(self.myFrozenDialog.numRightFrozenCols())

    def doMoveConfig(self):
        # Pop up the move dialog, wait until the user dismisses it.

	self.myMoveDialog.exec_()

        # Now do what the user requested.

        if self.myMoveDialog.moveToTop or self.myMoveDialog.moveToBottom:
            # We are moving rows.

            rows_to_move = []

            for sel in self.myTable.selectionList():
                # For each selection in the selection list, check to
                # ensure that the selection is an entire row (or rows).
                # If it is, add the row indices to the rows_to_move list.

                if sel.leftColumn() == 0 and sel.rightColumn() == Qics.QicsLAST_COLUMN:
                    for i in range(sel.topRow(), sel.bottomRow() + 1):
                        rows_to_move.append(i)

            # This ensures that the rows are moved in the correct order.
            rows_to_move.sort()

            # Now we can move the rows to the top or the bottom.
            if self.myMoveDialog.moveToTop():
                target = 0
            else:
                target = self.myTable.bottomRow() + 1

            self.myTable.moveRows(target, rows_to_move)

	if self.myMoveDialog.moveToLeft() or self.myMoveDialog.moveToRight():
            # We are moving columns.

            cols_to_move = []

            for sel in self.myTable.selectionList():
                # For each selection in the selection list, check to
                # ensure that the selection is an entire column (or columns).
                # If it is, add the column indices to the cols_to_move list.

                if sel.topRow() == 0 and sel.bottomRow() == Qics.QicsLAST_ROW:
                    for i in range(sel.leftColumn(), sel.rightColumn() + 1):
                        cols_to_move.append(i)

            # This ensures that the columns are moved in the correct order.
            cols_to_move.sort()

            # Now we can move the columns to the left or the right.
            if self.myMoveDialog.moveToLeft():
                target = 0
            else:
                target = self.myTable.rightColumn() + 1

            self.myTable.moveColumns(target, cols_to_move)

    def destroy(self):
	print "Exiting..."
	QtGui.qApp.quit()

class FrozenApp(QtGui.QApplication):
    """
    The example application.
    """
    def __init__(self, argv):
        QtGui.QApplication.__init__(self, argv)

        self.myModel = self.createModel(50, 50)
        self.myUI = self.createUI(self.myModel)


    def createModel(self, nrows, ncols):
        dm = QicsDataModelDefault(nrows, ncols)

        # We need to access QicsDataModel.setItem().
        d = sip.cast(dm, QicsDataModel)

        for i in range(nrows):
            for j in range(ncols):
                d.setItem(i, j, i + j)

        return dm

    def createUI(self, dm):
        return FrozenUI(dm)

if __name__ == "__main__":
    app = FrozenApp(sys.argv)

    sys.exit(app.exec_())
