#!/usr/bin/env python

## Copyright ( C ) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for PyQicsTable.  This example
## program may be used, distributed and modified without limitation.


import sys
import os


from PyQt4 import QtCore, QtGui, Qt

from qicstable import Qics, QicsTable, QicsDataModelDefault, QicsSelection, QicsSpan
from qicstable import QicsSelectionList, QicsDataItemFormatter
from qicstable import QicsDataItem_Int, QicsDataItem_Float, QicsDataItem_Double, QicsDataString, QicsDataInt, QicsDataFloat


class QicsDataItemCurrencyFormatter(QicsDataItemFormatter):
    """
    This class is used to display a QicsDataItem in a specified format, ie
    provide a "view" on the "model".  Currently a very simple class to display
    US Dollars in "$ 12.34" style.
    """
    def format(self, itm):
        itype = itm.type()

        if itype == QicsDataItem_Int:
            if itm.data() < 0:
                # Not quite right.  What we really want is to get rid of the
                # negative sign and replace it with the ( )

                #return itm.format("$ (%d.00)")
                pass

            return itm.format("$ %d.00")
        elif itype in (QicsDataItem_Float, QicsDataItem_Double):
            return itm.format("$ %1.2f")

        return itm.string()


class Spreadsheet(QtGui.QMainWindow):
    def __init__(self, p=None, name=None):
        """
        Construct the Spreadsheet object.  Creates a QMainWindow with a menu
        and toolbar, a QicsTable, and connects the appropriate slots to load
        and manipulate data.
        """
        QtGui.QMainWindow.__init__(self)

        self._currency = QicsDataItemCurrencyFormatter()

        #self.setCaption("Excellent Spreadsheet Demo")

        # Pulldown Menu

        # File
        file = QtGui.QMenu("&File", self)
	# Formating menu File with connect to Python function
	## New

	self._New = QtGui.QAction("&New", self)
    #self._New.setShortcut(QtGui.QKeySequence("Ctrl+N"))

	## Shortcut keys

	self._New.shortcut = QtCore.Qt.CTRL+QtCore.Qt.Key_N
	file.addAction(self._New)

	## Connect to self.fileNew Python func

	self.connect(self._New, QtCore.SIGNAL("triggered()"), self.fileNew)

	## Open
	self._Open = QtGui.QAction("&Open", self)
	## Shortcut keys
	self._Open.shortcut = QtCore.Qt.CTRL+QtCore.Qt.Key_O
	file.addAction(self._Open)
	## Connect to self.openFile Python func
	self.connect(self._Open, QtCore.SIGNAL("triggered()"), self.openFile)

	## Save
	self._Save = QtGui.QAction("&Save", self)
	## Shortcut keys
	self._Save.shortcut = QtCore.Qt.CTRL+QtCore.Qt.Key_S
	file.addAction(self._Save)
	## Connect to self.saveFile Python func
	self.connect(self._Save, QtCore.SIGNAL("triggered()"), self.saveFile)

	## Save As
	self._SaveAs = QtGui.QAction("S&ave As", self)
    ## Shortcut keys
    #self._Save.shortcut = QtCore.Qt.CTRL+QtCore.Qt.Key_S
	file.addAction(self._SaveAs)
	## Connect to self.saveFile Python func
	self.connect(self._SaveAs, QtCore.SIGNAL("triggered()"), self.saveFileAs)

	## Exit
	self._Exit = QtGui.QAction("E&xit", self)
	## Shortcut keys
	self._Save.shortcut = QtCore.Qt.CTRL+QtCore.Qt.Key_X
	file.addAction(self._Exit)
	## Connect to self.saveFile Python func
	self.connect(self._Exit,QtCore.SIGNAL("triggered()"), Qt.qApp, QtCore.SLOT("closeAllWindows()"))
	# Show File menu in menubar

	self.menuBar().addMenu(file)

    # Edit
	edit = QtGui.QMenu("&Edit", self)
	self._Copy = QtGui.QAction("&Copy", self)
	## Shortcut keys
	self._Copy.shortcut = QtCore.Qt.CTRL+QtCore.Qt.Key_C
	edit.addAction(self._Copy)
	## Connect to self.fileNew Python func
	self.connect(self._Copy, QtCore.SIGNAL("triggered()"), self.copy)

	# Paste
	self._Paste = QtGui.QAction("&Paste", self)
	## Shortcut keys
	self._Paste.shortcut = QtCore.Qt.CTRL+QtCore.Qt.Key_C
	edit.addAction(self._Paste)
    ## Connect to self.fileNew Python func
	self.connect(self._Paste, QtCore.SIGNAL("triggered()"), self.paste)
	self.menuBar().addMenu(edit)

	## View
	view = QtGui.QMenu("View", self)
	self._View = QtGui.QAction("&View", self)
	## Shortcut keys
	self._View.shortcut = QtCore.Qt.CTRL+QtCore.Qt.Key_V
	view.addAction(self._View)
    #self.connect(self._View, QtCore.SIGNAL("triggered()"), self.view)
	self.menuBar().addMenu(view)

	## Insert
	insert = QtGui.QMenu("Insert", self)
	self._InsertRow = QtGui.QAction("&Row", self)
	## Shortcut keys
	insert.addAction(self._InsertRow)
	self.connect(self._InsertRow, QtCore.SIGNAL("triggered()"), self.insertRow)
	self._InsertCol = QtGui.QAction("Co&lumn", self)
	## Shortcut keys
	insert.addAction(self._InsertCol)
	self.connect(self._InsertCol, QtCore.SIGNAL("triggered()"), self.insertColumn)
	self.menuBar().addMenu(insert)

	## Format

        format = QtGui.QMenu("Format", self)
        self.menuBar().addMenu(format)

        rowFormat = QtGui.QMenu("Row", self)
        format.addMenu(rowFormat)
        format.addSeparator()

        self._Foreground = QtGui.QAction("Fore&ground...", self)
        format.addAction(self._Foreground)
        self.connect(self._Foreground, QtCore.SIGNAL("triggered()"), self.formatForegroundColor)

        self._Background = QtGui.QAction("&Background...", self)
        format.addAction(self._Background)
        self.connect(self._Background, QtCore.SIGNAL("triggered()"), self.formatBackgroundColor)

        self._Font = QtGui.QAction("&Font...", self)
        format.addAction(self._Font)
        self.connect(self._Font, QtCore.SIGNAL("triggered()"), self.formatFont)


        # Tools
        tools = QtGui.QMenu("&Tools", self)
        self.menuBar().addMenu(tools)

        self.showQSAAction = QtGui.QAction(self.tr("Script Editor"), self)
        self.connect(self.showQSAAction, QtCore.SIGNAL("triggered()"), self.showQSAWorkbench)
        tools.addAction(self.showQSAAction)

        self.scriptTestAction = QtGui.QAction(self.tr("Script Test"), self)
        self.connect(self.scriptTestAction, QtCore.SIGNAL("triggered()"), self.testScripts)
        tools.addAction(self.scriptTestAction)

        self.saveScriptAction = QtGui.QAction(self.tr("Save Script"), self)
        self.connect(self.saveScriptAction, QtCore.SIGNAL("triggered()"), self.saveScripts)
        tools.addAction(self.saveScriptAction)

        # Data
        data = QtGui.QMenu("&Data", self)
        self.menuBar().addMenu(data)

        self.sortAscAction = QtGui.QAction(self.tr("&Ascendingt"), self)
        self.connect(self.sortAscAction, QtCore.SIGNAL("triggered()"), self.sortAscending)
        data.addAction(self.sortAscAction)

        self.sortDescAction = QtGui.QAction(self.tr("&Descendingt"), self)
        self.connect(self.sortDescAction, QtCore.SIGNAL("triggered()"), self.sortDescending)
        data.addAction(self.sortDescAction)

        # Spans
        spans = QtGui.QMenu("Spans", self)
        self.menuBar().addMenu(spans)

        self.addSpanAction = QtGui.QAction(self.tr("Add Span"), self)
        self.connect(self.addSpanAction, QtCore.SIGNAL("triggered()"), self.addSpan)
        spans.addAction(self.addSpanAction)

        self.removeSpanAction = QtGui.QAction(self.tr("Remove Span"), self)
        self.connect(self.removeSpanAction, QtCore.SIGNAL("triggered()"), self.removeSpan)
        spans.addAction(self.removeSpanAction)        

        # Toolbars

        # File operation toolbar

        fileTools = QtGui.QToolBar("File tools", self)

        #QtGui.QToolButton(_getIconSet("filenew.xpm"), "New", QtCore.QString, self.fileNew, fileTools)
        self.fileNewButton = QtGui.QToolButton()

        self.fileNewAct = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/filenew.xpm")), self.tr("&New"), self)
        self.connect(self.fileNewAct, QtCore.SIGNAL("triggered()"), self.fileNew)
        fileTools.addAction(self.fileNewAct)

        self.fileOpenAct = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/fileopen.xpm")), self.tr("&Open"), self)
        self.connect(self.fileOpenAct, QtCore.SIGNAL("triggered()"), self.openFile)
        fileTools.addAction(self.fileOpenAct)

        self.fileSaveAct = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/filesave.xpm")), self.tr("&Save"), self)
        self.connect(self.fileSaveAct, QtCore.SIGNAL("triggered()"), self.saveFile)
        fileTools.addAction(self.fileSaveAct)

        fileTools.addSeparator()

        # Edit operations
        self.cutAct = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/editcut.xpm")), self.tr("&Cut"), self)
        self.connect(self.cutAct, QtCore.SIGNAL("triggered()"), self.cut)
        fileTools.addAction(self.cutAct)

        self.copyAct = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/editcopy.xpm")), self.tr("&Copy"), self)
        self.connect(self.copyAct, QtCore.SIGNAL("triggered()"), self.copy)
        fileTools.addAction(self.copyAct)

        self.pasteAct = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/editpaste.xpm")), self.tr("&Paste"), self)
        self.connect(self.pasteAct, QtCore.SIGNAL("triggered()"), self.paste)
        fileTools.addAction(self.pasteAct)

        fileTools.addSeparator()

        # Sort

        self.sortAscendingAct = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/sortasc.xpm")), self.tr("Sort Ascending"), self)
        self.connect(self.sortAscendingAct, QtCore.SIGNAL("triggered()"), self.sortAscending)
        fileTools.addAction(self.sortAscendingAct)

        self.sortDescendingAct = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/sortdesc.xpm")), self.tr("Sort Descending"), self)
        self.connect(self.sortDescendingAct, QtCore.SIGNAL("triggered()"), self.sortDescending)
        fileTools.addAction(self.sortDescendingAct)

        fileTools.addSeparator()

        self.addToolBar(fileTools)

        # Font name and point size

        fontTools = QtGui.QToolBar("Font tools", self)

        self.fontFamilyCombo = QtGui.QComboBox(fontTools)
        self.fontSizeCombo = QtGui.QComboBox(fontTools)

        fontTools.addWidget(self.fontFamilyCombo)
        fontTools.addWidget(self.fontSizeCombo)

        self.connect(self.fontFamilyCombo, QtCore.SIGNAL("activated(int)"), self.selectFont)
        self.connect(self.fontSizeCombo, QtCore.SIGNAL("activated(int)"), self.selectFont)

        fdb = QtGui.QFontDatabase()
        families = fdb.families()
        self.fontFamilyCombo.insertItems(0, families)

        # Font format operations

        self.boldAction = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/textbold.xpm")), self.tr("Bold"), self)
        self.connect(self.boldAction, QtCore.SIGNAL("triggered()"), self.textBold)
        fontTools.addAction(self.boldAction)
        self.boldAction.setCheckable(True)

        self.italicAction = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/textitalic.xpm")), self.tr("Italic"), self)
        self.connect(self.italicAction, QtCore.SIGNAL("triggered()"), self.textItalic)
        fontTools.addAction(self.italicAction)
        self.italicAction.setCheckable(True)

        self.underlineAction = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/textunder.xpm")), self.tr("Underline"), self)
        self.connect(self.underlineAction, QtCore.SIGNAL("triggered()"), self.textUnderline)
        fontTools.addAction(self.underlineAction)
        self.underlineAction.setCheckable(True)

        fontTools.addSeparator()

        # Text Alignment

        self.textLeftAction = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/textleft.xpm")), self.tr("Align Left"), self)
        self.connect(self.textLeftAction, QtCore.SIGNAL("triggered()"), self.textAlignLeft)
        fontTools.addAction(self.textLeftAction)
        self.textLeftAction.setCheckable(True)

        self.textCenterAction = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/textcenter.xpm")), self.tr("Align Center"), self)
        self.connect(self.textCenterAction, QtCore.SIGNAL("triggered()"), self.textAlignCenter)
        fontTools.addAction(self.textCenterAction)
        self.textCenterAction.setCheckable(True)

        self.textRightAction = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/textright.xpm")),"Align Right", self)
        self.connect(self.textRightAction, QtCore.SIGNAL("triggered()"), self.textAlignRight)
        fontTools.addAction(self.textRightAction)
        self.textRightAction.setCheckable(True)

        fontTools.addSeparator()

        # Formatters

        currencyTB = QtGui.QToolButton(fontTools)
        currencyTB.setText("$")
        self.connect(currencyTB, QtCore.SIGNAL("clicked()"), self.setCurrencyFormatter)

        fontTools.addWidget(currencyTB)

        fontTools.addSeparator()

        # Colors

        self.formatForegroundColorAction = QtGui.QAction(QtGui.QIcon(QtGui.QPixmap("images/fontcolor.xpm")), self.tr("Font Color"), self)
        self.connect(self.formatForegroundColorAction, QtCore.SIGNAL("triggered()"), self.formatForegroundColor)
        fontTools.addAction(self.formatForegroundColorAction)

        self.addToolBar(fontTools)

        # Table

        vbox = QtGui.QWidget(self)
        vboxLayout = QtGui.QVBoxLayout();
        cellrow = QtGui.QWidget(vbox)
        cellrowLayout = QtGui.QHBoxLayout()
        cellrowLayout.setMargin(2)
        cellrowLayout.addWidget(QtGui.QLabel("Cell value = "))

        self.cellLineEdit = QtGui.QLineEdit()
        cellrowLayout.addWidget(self.cellLineEdit)
        cellrow.setLayout(cellrowLayout)

        self.connect(self.cellLineEdit, QtCore.SIGNAL("returnPressed()"), self.cellLineUpdate)

        lastRow = 10
        lastCol = 50

        # Create the data model
        self.dm = QicsDataModelDefault(lastRow, lastCol)

        # Create the table, using the data model we created above
        self.table = QicsTable(self.dm)
        # self.table.setViewport(Qics.QicsRegion(0, 0, 50, 50))
        vboxLayout.addWidget(cellrow)
        vboxLayout.addWidget(self.table)
        vbox.setLayout(vboxLayout)

        self.connect(self.table, QtCore.SIGNAL("selectionListChanged(bool)"), self.updateSelection)

        # Set some visual resources on the table
        self.table.rowHeader().setAlignment(QtCore.Qt.AlignHCenter)
        self.table.columnHeader().setAlignment(QtCore.Qt.AlignHCenter)
        self.table.rowHeader().column(0).setWidthInChars(3)
        self.table.mainGridRef().setBackgroundColor(QtCore.Qt.white)
        
        # Set row sorting mode to non-distractive sord

        self.table.show()

        self.setFontCombos(self.table.font())

        self.setCentralWidget(vbox)

        self.statusBar().showMessage("Ready", -1)

        self.fileName = QtCore.QString("")

    def loadFile(self, fname):
        """
        Load a Comma Delimited File into the data table.
        """
        # Create the stream from the file, read into data model
        file = QtCore.QFile(fname)

        if file.open(QtCore.QIODevice.ReadOnly):
            stream = QtCore.QTextStream(file)

            self.dm.readASCII(stream, ',')

    def writeFile(self, fname):
        """
        Save a Comma Delimited File from the data table.
        """
        # Create the stream from the file, read into data model
        file = QtCore.QFile(fname)

        if file.open(QtCore.QIODevice.WriteOnly):
            stream = QtCore.QTextStream(file)

            self.dm.writeASCII(stream, ',', 0, 0, 10, 10)

    def getSelectedRegion(self):
        """
        Query the table, find the start and end row/column of the currently
        selected region, if any.
        """
        selection = QicsSelection()
        list = self.table.selectionList()

        # Get selected boundry

        if list is not None:
            if len(list) > 0:
                selection = list[0]

                self.startSelectedRow = selection.anchorRow()
                self.startSelectedColumn = selection.anchorColumn()
        else:
            # Reset flags
            self.startSelectedRow = -1
            self.startSelectedColumn = -1

        row = selection.endRow()
        if row > self.dm.lastRow():
            row = self.dm.lastRow() + 1
        else:
            row += 1

        col = selection.endColumn()
        if col > self.dm.lastColumn():
            col = self.dm.lastColumn() + 1
        else:
            col += 1

        self.endSelectedRow = row
        self.endSelectedColumn = col

    def setFontCombos(self, font):
        """
        Given a font, set the fontFamilyCombo to show it, set the fontSizeCombo
        to contain valid sizes for that font, and then show the correct font
        size.
        """
        # Set font family
        self.fontFamilyCombo.setEditText(font.family())

        fdb = QtGui.QFontDatabase()
        sizes = fdb.pointSizes(font.family())

        # Clear the sizes combo, fill it with valid sizes based in family

        self.fontSizeCombo.clear()

        if len(sizes) > 1:
            for pts in sizes:
                self.fontSizeCombo.insertItem(pts - 6, QtCore.QString.number(pts))

            self.fontSizeCombo.setEditText(QtCore.QString.number(font.pointSize()))


    def setCellValue(self, row, col, value):
        """
        Set cell[row][col] = value.
        """
        self.dm.setItem(row, col, value)

    def getCellValue(self, row, col):
        """
        Return the value of cell[row][col].
        """
        return self.dm.itemString(row, col)

    def sortAscending(self):
        """
        Sort the table by the selected column in ascending order.
        """

        self.getSelectedRegion()

        self.table.sortRows([self.startSelectedColumn], Qics.Ascending)

    def sortDescending(self):
        """
        Sort the table by the selected column in descending order.
        """
        self.getSelectedRegion()

        self.table.sortRows([self.startSelectedColumn], Qics.Descending)

    def copy(self):
        """
        Calls the table's copy operation to place data into the QClipboard
        owned by qApp.
        """
        self.table.copy()

    def cut(self):
        """
        Calls the table's cut operation to place data into the QClipboard owned
        by qApp and remove it from the visible table area.
        """
        self.table.cut()

    def paste(self):
        """
        Calls the table's paste operation to retrieve cells from the QClipboard
        owned by qApp and paste them at the current cell.
        """
        self.table.paste()

    def fileNew(self):
        """
        Clear out all the data currently displayed in the table.
        """
        print "To be implemented"

    def openFile(self):
        """
        Bring up a QFileDialog to select a file, then load it.
        """
        fname = QtGui.QFileDialog.getOpenFileName()

        if not fname.isEmpty():
            self.fileName = fname
            self.loadFile(self.fileName)

    def saveFile(self):
        """
        Save data to the current file name.
        """
        if not self.fileName.isEmpty():
            self.writeFile(self.fileName)

    def saveFileAs(self):
        """
        Bring up a QFileDialog to select a file, then save to it.
        """
        fname = QtGui.QFileDialog.getSaveFileName()

        if not fname.isEmpty():
            self.fileName = fname
            self.writeFile(self.fileName)


    def insertRow(self):
        """
        Add a row to the table before the currently selected row.
        """
        self.getSelectedRegion()

        if self.startSelectedRow > -1:
            self.table.insertRow(self.startSelectedRow)

    def insertColumn(self):
        """
        Add a column to the table before the currently selected column.
        """
        self.getSelectedRegion()

        if self.startSelectedColumn > -1:
            self.table.insertColumn(self.startSelectedColumn)

    def formatFont(self):
        """
        Bring up the font dialog, select a font, apply it to the selected
        region.
        """
        # Select a font

        font, ok = QtGui.QFontDialog.getFont(QtGui.QFont("Helvetica", 10), self)

        if not ok:
            return

        # If cells are selected, set their display font

        self.getSelectedRegion()

        if self.startSelectedRow > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    self.table.cellRef(i, j).setFont(font)

    def formatBackgroundColor(self):
        """
        Bring up the color dialog, select a color, apply it to the selected
        region as the background color.
        """
        color = QtGui.QColorDialog.getColor()

        if not color.isValid():
            return

        # If cells are selected, set their display font

        self.getSelectedRegion()

        if self.startSelectedRow > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    self.table.cellRef(i, j).setBackgroundColor(color)

    def formatForegroundColor(self):
        """
        Bring up the color dialog, select a color, apply it to the selected
        region as the foreground color.
        """
        color = QtGui.QColorDialog.getColor()

        if not color.isValid():
            return

        # If cells are selected, set their display font

        self.getSelectedRegion()

        if self.startSelectedRow > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    self.table.cellRef(i, j).setForegroundColor(color)

    def selectFont(self, combo):
        family = self.fontFamilyCombo.currentText()
        size, ok = self.fontSizeCombo.currentText().toInt()

        # If cells are selected, set their display font

        self.getSelectedRegion()

        if self.startSelectedRow > -1:
            fdb = QtGui.QFontDatabase()

            currentFont = self.table.cellRef(self.startSelectedRow, self.startSelectedColumn).font()

            font = fdb.font(family, fdb.styleString(currentFont), size)

            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    self.table.cellRef(i, j).setFont(font)

    def textAlignLeft(self):
        """
        Set the alignment value of the displayed cell to AlignLeft.
        """
        if self.startSelectedRow < 0 and self.startSelectedColumn < 0:
            return

        if self.startSelectedRow > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    self.table.cellRef(i, j).setAlignment(QtCore.Qt.AlignLeft)

        self.textRightAction.setChecked(False)
        self.textCenterAction.setChecked(False)

    def textAlignRight(self):
        """
        Set the alignment value of the displayed cell to AlignRight.
        """
        if self.startSelectedRow < 0 and self.startSelectedColumn < 0:
            return

        if self.startSelectedRow > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    self.table.cellRef(i, j).setAlignment(QtCore.Qt.AlignRight)

        self.textLeftAction.setChecked(False)
        self.textCenterAction.setChecked(False)

    def textAlignCenter(self):
        """
        Set the alignment value of the displayed cell to AlignHCenter.
        """
        if self.startSelectedRow < 0 and self.startSelectedColumn < 0:
            return

        if self.startSelectedRow > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    self.table.cellRef(i, j).setAlignment(QtCore.Qt.AlignHCenter)

        self.textLeftAction.setChecked(False)
        self.textRightAction.setChecked(False)

    def textBold(self):
        """
        Toggle the bold-ness state of whatever cells are selected.
        """
        if self.startSelectedRow < 0 and self.startSelectedColumn < 0:
            return

        on = self.boldAction.isChecked()

        if self.startSelectedRow > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    f = self.table.cellRef(i, j).font()
                    f.setBold(on)
                    self.table.cellRef(i, j).setFont(f)

    def textItalic(self):
        """
        Toggle the italic-ness state of whatever cells are selected.
        """
        if self.startSelectedRow < 0 and self.startSelectedColumn < 0:
            return

        on = self.italicAction.isChecked()

        if self.startSelectedRow > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    f = self.table.cellRef(i, j).font()
                    f.setItalic(on)
                    self.table.cellRef(i, j).setFont(f)

    def textUnderline(self):
        """
        Toggle the underline-ed-ness state of whatever cells are selected.
        """
        if self.startSelectedRow < 0 and self.startSelectedColumn < 0:
            return

        on = self.underlineAction.isChecked()

        if self.startSelectedRow > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    f = self.table.cellRef(i, j).font()
                    f.setUnderline(on)
                    self.table.cellRef(i, j).setFont(f)

    def setCurrencyFormatter(self):
        """
        Set a formatter on the selected cells do display the values in dollar
        value ($ XX.YY).
        """
        self.getSelectedRegion()

        if self.startSelectedRow > -1 and self.startSelectedColumn > -1:
            for i in range(self.startSelectedRow, self.endSelectedRow):
                for j in range(self.startSelectedColumn, self.endSelectedColumn):
                    self.table.cellRef(i, j).setFormatter(self._currency)

    def cellLineUpdate(self):
        """
        When a value is entered in the cellLineEdit and a cell is selected in
        the table, store the data element as a new data item.
        """
        self.getSelectedRegion()

        if self.startSelectedRow > -1 and self.startSelectedColumn > -1:
            # OK, we got a selected cell and some data.  Time to put it into
            # the data model

            # temp var because they are easier to type
            row = self.startSelectedRow
            col = self.startSelectedColumn

            # Try to convert to int
            i, ok = self.cellLineEdit.text().toInt()

            if ok:
                self.dm.setItem(row, col, QicsDataInt(i))
                return

            # Try to convert to float
            f, ok = self.cellLineEdit.text().toFloat()

            if ok:
                self.dm.setItem(row, col, QicsDataFloat(f))
                return

            self.dm.setItem(row, col, QicsDataString(self.cellLineEdit.text()))

    def updateSelection(self, in_progress):
        """
        A cell was selected, so update the state of the toolbar pieces.
        """
        if in_progress:
            return

        self.getSelectedRegion()

        if self.startSelectedRow > -1 and self.startSelectedColumn > -1:
            font = self.table.cellRef(self.startSelectedRow, self.startSelectedColumn).font()
            self.setFontCombos(font)
            if font.bold():
                self.boldAction.setChecked(True)
            else:
                # Is on, turn off (not bold)
                self.boldAction.setChecked(False)
            if font.italic():
                self.italicAction.setChecked(True)
            else:
                # Is on, turn off (not bold)
                self.italicAction.setChecked(False)
            if font.underline():
                self.underlineAction.setChecked(True)
            else:
                # Is on, turn off (not bold)
                self.underlineAction.setChecked(False)
            alignment = self.table.cellRef(self.startSelectedRow, self.startSelectedColumn).alignment()

            if alignment == QtCore.Qt.AlignLeft:
                self.textLeftAction.setChecked(True)
                self.textRightAction.setChecked(False)
                self.textCenterAction.setChecked(False)
            elif alignment == QtCore.Qt.AlignRight:
                self.textLeftAction.setChecked(False)
                self.textRightAction.setChecked(True)
                self.textCenterAction.setChecked(False)
            elif alignment == QtCore.Qt.AlignCenter:
                self.textLeftAction.setChecked(False)
                self.textRightAction.setChecked(False)
                self.textCenterAction.setChecked(True)
            else:
                self.textLeftAction.setChecked(False)
                self.textRightAction.setChecked(False)
                self.textCenterAction.setChecked(False)

            if self.dm.item(self.startSelectedRow, self.startSelectedColumn) is None:
                self.cellLineEdit.setText("")
                return

            cell = self.table.cell(self.startSelectedRow, self.startSelectedColumn, True)

            if cell.dataValue():
                cellval = cell.dataValue().string()
                self.cellLineEdit.setText(cellval)



    def showQSAWorkbench(self):
        QtGui.QMessageBox.warning(None, "No QSA support!",
                            "Sorry, no QSA support compiled in",
                            QtGui.QMessageBox.Ok, QtGui.QMessageBox.NoButton)

    def testScripts(self):
        QtGui.QMessageBox.warning(None, "No QSA support!",
                            "Sorry, no QSA support compiled in",
                            QtGui.QMessageBox.Ok, QtGui.QMessageBox.NoButton)

    def saveScripts(self):
        QtGui.QMessageBox.warning(None, "No QSA support!",
                            "Sorry, no QSA support compiled in",
                            QtGui.QMessageBox.Ok, QtGui.QMessageBox.NoButton)


    def addSpan(self):
        """
        Adds span to selected cells
        """

        slist = self.table.selectionList()
        if slist:
            [self.table.addCellSpan(QicsSpan(elem.topRow(), elem.leftColumn(), elem.numRows(), elem.numColumns())) for elem in slist]
            
            
    def removeSpan(self):
        """
        Remove span from selected cells
        """
        slist = self.table.selectionList()
        if slist:
            [self.table.removeCellSpan(elem.topRow(), elem.leftColumn()) for elem in slist]
        


if __name__ == "__main__":
    qapp = QtGui.QApplication(sys.argv)

    ss = Spreadsheet(None, "SpreadsheetDemo")

    ss.show()

    QtCore.QObject.connect(QtGui.qApp, QtCore.SIGNAL("lastWindowClosed()"), QtGui.qApp, QtCore.SLOT("quit()"))

    sys.exit(qapp.exec_())
