#!/usr/bin/env python

## Copyright ( C ) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for PyQicsTable.  This example
## program may be used, distributed and modified without limitation.

########################################################################
##
## This example program demonstrates the use of a number of QicsTable
## features, including custom cell displayers, validators, and formatters.
##
## The program displays a table when can be loaded with data concerning
## "consultants".  Data can be loaded from, and saved to, a text file.
## Columns in the table are:
##
## First Name:         (string) default cell displayer
## Last Name:          (string) default cell displayer
## SSN:                (int) default cell displayer, custom validator
##                           and formatter
## Date of Birth:      (date) default cell displayer
## Office:             (string) combo cell displayer
## Security Clearance: (string) subclassed check box displayer
## Billable Rate:      (float) default cell displayer, custom formatter
##
########################################################################


import sys


from PyQt4 import QtCore, QtGui, Qt

from qicstable import QicsTable, QicsDataModelDefault, QicsCheckCellDisplay
from qicstable import QicsComboCellDisplay, QicsDataItemSprintfFormatter
from qicstable import QicsDataItem_Float, QicsDataItem_Int, Qics, QicsDataString


NUM_COLUMNS = 7


class SecCheckBox(QicsCheckCellDisplay):
    """
    SecCheckBox - A subclass of QicsCheckCellDisplay that displays a "check" if
    the underlying data value is 'y'.  This class also stores (in the data
    model) any changes to the checkbox as 'y' if the box is checked, or 'n'
    otherwise.
    """
    def checkState(self, info, model_row, model_col, itm):
        """
        Reimplemented from QicsCheckCellDisplay.
        """
        # Returns the state of the cell, based on the data item. State is false
        # unless the data item is "y".
        if itm:
            tableState = itm.string()

            return (tableState.toLatin1() == "y")
        else:
            return False

    def valueChanged(self, info, model_row, model_col, set):
        """
        Reimplemented from QicsCheckCellDisplay.
        """
        if set:
            val = "y"
        else:
            val = "n"

        info.dataModel().setItem(model_row, model_col, QicsDataString(val))


class SSNValidator(QtGui.QValidator):
    """
    SSNValidator - A subclass of QValidator that restricts input to numeral and
    '-' chars.   Once the user presses enter, fixup() will strip the dashed
    from the input in order to keep the data consistant.  A formatter will be
    used to display the value in the table in the common manner (XXX-XX-XXXX).
    """
    def __init__(self):
        QtGui.QValidator.__init__(self, None)

    def validate(self, text, pos):
        # An empty string could be made acceptable
        if text.isEmpty():
            return (QtGui.QValidator.Acceptable, pos)

        # Protect against spurious calls to validate()
        if pos > text.length():
            return (QtGui.QValidator.Acceptable, pos)

        # Anything but decimal digits and dashes is invalid. We only need to
        # check the character at the cursor positions. This speeds things up
        # massively.

        if not text.at(pos-1).isDigit() and text[pos-1] != '-' and pos-1 >= 0:
            return (QtGui.QValidator.Invalid, pos)

        # If the characters entered so far are valid, but the string contains
        # less than ten digits, it could be made acceptable, but is not yet.
        numDigits = text.length()
        numDashes = 0

        for i in range(text.length()):
            if text[i] == '-':
                numDigits -= 1
                numDashes += 1

        if numDigits < 9:
            return (QtGui.QValidator.Acceptable, pos)

        # More than nine digits is always invalid
        if numDigits > 9:
            return (QtGui.QValidator.Invalid, pos)

        if numDashes != 0:
            return (QtGui.QValidator.Acceptable, pos)

        # numDashes = 0 and numDigits = 9
        return (QtGui.QValidator.Acceptable, pos)

    def fixup(self, text):
        inString = QtCore.QString(text)
        inLength = inString.length()

        for i in range(inLength, 0, -1):
            if inString[i] == '-':
                text.remove(i, 1)


class Consultant(QtGui.QMainWindow):
    """
    Consultant - The primary class.  Subclassed from QMainWindow, this class
    contains both the table and the data model, as well as all the methods to
    implement the UI behavior.
    """
    def __init__(self, parent, name):
        QtGui.QMainWindow.__init__(self, parent)

        # create the data model
        self.dataModel = QicsDataModelDefault(0, NUM_COLUMNS)

        # create the table, using the data model we created above
        self.table = QicsTable(self.dataModel, self)
        self.setCentralWidget(self.table)

        # customize the column header to make things look pretty
        self.table.columnHeaderRef().setAlignment(QtCore.Qt.AlignCenter)

        self.table.columnHeaderRef().cellRef(0,0).setLabel("First Name")
        self.table.columnHeaderRef().cellRef(0,1).setLabel("Last Name")
        self.table.columnHeaderRef().cellRef(0,2).setLabel("S.S. Number")
        self.table.columnHeaderRef().cellRef(0,3).setLabel("Date of Birth")
        self.table.columnHeaderRef().cellRef(0,4).setLabel("Office")
        self.table.columnHeaderRef().cellRef(0,5).setLabel("Sec. Clearance")
        self.table.columnHeaderRef().cellRef(0,6).setLabel("Billable Rate")

        # Add a validator to the SSN to insure a consistant display behaviour
        # and error checking.
        self.ssnValidator = SSNValidator()
        self.table.columnRef(2).setValidator(self.ssnValidator)

        # Format the combo box for the office column
        self.officeComboDisplay = QicsComboCellDisplay()
        self.officeComboDisplay.addItem("Boston")
        self.officeComboDisplay.addItem("Cambridge")
        self.officeComboDisplay.addItem("Helsinki")
        self.officeComboDisplay.setAddValueToList(False)
        self.table.columnRef(4).setDisplayer(self.officeComboDisplay)

        # Security clearance check box
        self.columnHeaderCheckDisp = SecCheckBox()
        self.table.columnRef(5).setDisplayer(self.columnHeaderCheckDisp)

        # Format the billable rate column with a dollar sign, decimal point,
        # and the appropriate cents digits.
        self.brFormatter = QicsDataItemSprintfFormatter()
        self.brFormatter.addFormatString(QicsDataItem_Float, "$%.2f")
        self.brFormatter.addFormatString(QicsDataItem_Int, "$%d.00")
        self.table.columnRef(6).setFormatter(self.brFormatter)
	self.table.columnRef(6).setValidator(QtGui.QDoubleValidator(self.table))

        # Add a title widget to the top of the table
        label = QtGui.QLabel("Consultants", self.table)
        label.setAlignment(QtCore.Qt.AlignCenter)
        self.table.setTopTitleWidget(label)

        # Set column widths
        for i in range(NUM_COLUMNS):
            self.table.columnRef(i).setWidthInChars(7)

        self.table.setVisibleColumns(NUM_COLUMNS)

        # Menu Bar
	menubar = self.menuBar()

	self.filemenu = QtGui.QMenu("File", self)

	self._Open = QtGui.QAction("Open...", self)
	self.filemenu.addAction(self._Open)
	self.connect(self._Open, QtCore.SIGNAL("triggered()"), self.fileOpen)

	self._Save = QtGui.QAction("Save...", self)
	self.filemenu.addAction(self._Save)
	self.connect(self._Save, QtCore.SIGNAL("triggered()"), self.fileSave)

	self._Exit = QtGui.QAction("Quit", self)
	self.filemenu.addAction(self._Exit)
	self.connect(self._Exit, QtCore.SIGNAL("triggered()"), QtGui.qApp, QtCore.SLOT("quit()"))

	menubar.addMenu(self.filemenu)

	self.toolsmenu = QtGui.QMenu("Tools", self)

	self._AddCs = QtGui.QAction("Add consultant", self)
	self._DelCs = QtGui.QAction("Delete consultant", self)
	self._SortA = QtGui.QAction("Sort Ascending", self)
	self._SortD = QtGui.QAction("Sort Descending", self)

	self.connect(self._AddCs, QtCore.SIGNAL("triggered()"), self.addRow)
	self.connect(self._DelCs, QtCore.SIGNAL("triggered()"), self.deleteRow)
	self.connect(self._SortA, QtCore.SIGNAL("triggered()"), self.sortAscending)
	self.connect(self._SortD, QtCore.SIGNAL("triggered()"), self.sortDescending)

	self.toolsmenu.addAction(self._AddCs)
	self.toolsmenu.addAction(self._DelCs)
	self.toolsmenu.addAction(self._SortA)
	self.toolsmenu.addAction(self._SortD)

	menubar.addMenu(self.toolsmenu)





        # Connection for autosort
        QtCore.QObject.connect(self.table.columnHeaderRef(),
                        QtCore.SIGNAL("doubleClicked(int, int, int, const QPoint &)"),
                        self.autoSort)

        # Reset autoSort sanity vars
        self._autoSortCol = -1
        self._autoSortOrder = Qics.Descending

    def addRow(self):
        self.table.addRows(1)

    def deleteRow(self):
        self.table.deleteRow(self.table.currentCell().rowIndex())

    def sortAscending(self):
        self.table.sortRows([self.table.currentCell().columnIndex()], Qics.Ascending)

    def sortDescending(self):
        self.table.sortRows([self.table.currentCell().columnIndex()], Qics.Descending)

    def fileOpen(self):
        inFileName = QtGui.QFileDialog.getOpenFileName(self, "Choose a file",
						 "./",
                                                 "CSV files (*.csv)")
						 #,
                                                 #"open file dialog")

        if inFileName.isEmpty():
            return;

        inFile = QtCore.QFile(inFileName)

        error = 0

        if inFile.open(QtCore.QIODevice.ReadOnly):
            inStream = QtCore.QTextStream(inFile)

            self.table.setIgnoreModelSizeChanges(True)
            self.dataModel.readASCII(inStream, ',', 0, 0, True)
            inFile.close()
            self.table.setIgnoreModelSizeChanges(False)
        else:
            error = 1

        if error:
            fileBox = QtGui.QMessageBox()
            fileBox.setCaption("File Operation")
            fileBox.setText(QtCore.QString("Could not read file: %1").arg(inFileName))
            fileBox.setButtonText(0, "OK")
            fileBox.exec_()

    def fileSave(self):
        outFileName = QtGui.QFileDialog.getSaveFileName(self, 
                                                  "save file dialog"
                                                  "Choose a file",
                                                  "./", 
                                                  "CSV files (*, csv)")

        if outFileName.isEmpty():
            return

        outFile = QtCore.QFile(outFileName)

        error = 0

        if outFile.open(QtCore.QIODevice.WriteOnly):
            outStream = QtCore.QTextStream(outFile)

            self.dataModel.writeASCII(outStream, ',')
            #outStream << "\n"
            outFile.close()
        else:
            error = 1

        fileBox = QtGui.QMessageBox()
        fileBox.setWindowTitle("File Operation")
        if error < 1:
            fileBox.setText(QtCore.QString("Saved Table Data to %1").arg(outFileName))
        else:
            fileBox.setText(QtCore.QString("Error Accessing %1").arg(outFileName))

        fileBox.setButtonText(0, "OK")
        fileBox.exec_()

    def autoSort(self, row, col, button, pos):
        if self._autoSortCol == col:
            if self._autoSortOrder == Qics.Ascending:
                self._autoSortOrder = Qics.Descending
            else:
                self._autoSortOrder = Qics.Ascending
        else:
            self._autoSortCol = col
            self._autoSortOrder = Qics.Ascending

        self.table.sortRows(col, self._autoSortOrder)


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)

    consultant = Consultant(None, "Consultant Example")
    consultant.show()

    sys.exit(app.exec_())
