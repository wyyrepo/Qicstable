#!/usr/bin/env python

## Copyright ( C ) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for PyQicsTable.  This example
## program may be used, distributed and modified without limitation.


import sys
import math
import random

from PyQt4 import QtCore, QtGui, QtXml, Qt

from qicstable import QicsDataModelDefault, QicsDataFloat, QicsDataString
from qicstable import QicsDataItem_Float, QicsDataItem_Double, QicsDataItem_Int
from qicstable import QicsTable, QicsDataItemSprintfFormatter, QicsSpan
from qicstable import QicsComboCellDisplay, Qics


fname = "initial-values.txt"

stock_name = 0
day_zero = 1
day_one = 4
day_two = 7
day_three = 10

cur_idx = 0
high_idx = 1
low_idx = 2

nyse_idx = 0
nasdaq_idx = 1

update_period = 2000


class StockData(QtCore.QObject):
    def __init__(self):
        QtCore.QObject.__init__(self)

        self.myDM = QicsDataModelDefault()

        self.connect(self.myDM, QtCore.SIGNAL("modelChanged(QicsRegion)"), self.dataValuesChanged)

        self.loadDT()

    def dataModel(self):
        return self.myDM

    def updateStocks(self):
        for i in range(self.myDM.lastRow() + 1):
            self.updateStock(i)

    def dataValuesChanged(self, reg):
        row = reg.startCell().row()
        col = reg.startCell().column()

        if col == day_zero+cur_idx:
            yesterday = self.getFloatValue(row, day_one+cur_idx)
            today = self.getFloatValue(row, day_zero+cur_idx)

            self.emit(PYSIGNAL("stockChanged"), (row, (today > yesterday)))

    def loadDT(self):
        f = QtCore.QFile(fname)
        f.open(QtCore.QIODevice.ReadOnly)

        stream = QtCore.QTextStream(f)
    
        self.myDM.readASCII(stream)

        f.close()

    def updateStock(self, row):
        cur_price = self.getFloatValue(row, day_zero+cur_idx)
        if cur_price < 0:
            # yesterday's close
            cur_price = self.getFloatValue(row, day_one+cur_idx)

        cur_price = self.newprice(cur_price, 0.02, self.posneg())
        fdata = QicsDataFloat(cur_price)
        self.myDM.setItem(row, day_zero+cur_idx, fdata)

        # Check high.
        high = self.getFloatValue(row, day_zero+high_idx)
        if high < 0 or cur_price > high:
            self.myDM.setItem(row, day_zero+high_idx, fdata)

        # Check low.
        low = self.getFloatValue(row, day_zero+low_idx)
        if low < 0 or cur_price < low:
            self.myDM.setItem(row, day_zero+low_idx, fdata)

    def getFloatValue(self, row, col):
        itm = self.myDM.item(row, col)

        if itm:
            if itm.type() == QicsDataItem_Float:
                return itm.data()
            elif itm.type() == QicsDataItem_Double:
                return itm.data()
            elif itm.type() == QicsDataItem_Int:
                return float(itm.data())

        return -1.0

    def newprice(self, old_price, percent_change=0.02, updown=1):
        new_price = old_price + (self.randf() * percent_change * old_price * updown)
        new_price = (float(self.roundf(new_price * 100))) / 100

        return new_price

    def roundf(self, f):
        base = int(math.floor(f))

        if (f - base) < 0.5:
            return base
        else:
            return base + 1

    def randf(self):
        return random.random()

    def posneg(self):
        if self.roundf(self.randf()) < 0.5:
            return -1
        else:
            return 1


class StockUI(QtCore.QObject):
    def __init__(self, dt):
        QtCore.QObject.__init__(self)

        # We need to make sure the cell displayers aren't garbage collected.
        self._displayers = []

        self.myMainWindow = QtGui.QMainWindow()
    
        self.myTable = QicsTable(dt, self.myMainWindow)
        self.myMainWindow.setCentralWidget(self.myTable)

        menu = self.myMainWindow.menuBar()
    
        file = QtGui.QMenu("&File", self.myMainWindow)
	## Exit
	self._Exit = QtGui.QAction("E&xit", self)
	## Shortcut keys
	#self._Save.shortcut = QtCore.Qt.CTRL+QtCore.Qt.Key_X
	file.addAction(self._Exit)
	## Connect to self.saveFile Python func
	self.connect(self._Exit,QtCore.SIGNAL("triggered()"), Qt.qApp, QtCore.SLOT("closeAllWindows()"))
	#exitAct = QtGui.QAction("E&xit", self)
	#self.connect(exitAct, QtCore.SIGNAL("triggered()"), self.destroy)
        #file.addAction(exitAct)
	menu.addMenu(file)

        label = QtGui.QLabel("Stock Activity", self.myTable)
        fnt = label.font()
        fnt.setPointSize(fnt.pointSize() + 6)
        label.setFont(fnt)
        label.setAlignment(QtCore.Qt.AlignCenter)
        self.myTable.setTopTitleWidget(label)

        self.setupRowHeader()
        self.setupColumnHeader()
        self.setupGrid()

        cb = QtGui.QCheckBox("Auto\nUpdates", self.myTable)
        self.myTable.setTopLeftCornerWidget(cb)
        self.connect(cb, QtCore.SIGNAL("toggled(bool)"), self, QtCore.SIGNAL("updateToggled()"))

        self.myMainWindow.show()

    def base(self):
        return self.myMainWindow

    def table(self):
        return self.myTable

    def decorateStock(self, idx, gainer):
        itm = self.myTable.columnHeaderRef().cellRef(1, day_zero+2).dataValue()
        gain_color = itm.string()

        if gainer:
            c = QColor(gain_color)
        else:
            c = self.myTable.mainGridRef().foregroundColor()

        self.myTable.cellRef(idx, day_zero+cur_idx).setForegroundColor(c)

    def setupRowHeader(self):
        # nothing to do here
        pass

    def setupColumnHeader(self):
        ch = self.myTable.columnHeader()

        ch.setNumRows(3)

        ch.setAlignment(QtCore.Qt.AlignCenter)

        # Stock header
        ch.addCellSpan(QicsSpan(0,stock_name, 3, 1))
        ch.cellRef(0,stock_name).setLabel("Stock")

        self.setupDay(day_zero, "Today")
        self.setupDay(day_one, "One Day Ago")
        self.setupDay(day_two, "Two Days Ago")
        self.setupDay(day_three, "Three Days Ago")

    def setupGrid(self):
        self.v1 = QicsDataItemSprintfFormatter()

        self.myTable.columnRef(0).setWidthInChars(7)

        self.v1.addFormatString(QicsDataItem_Float, "$%.2f")
        self.v1.addFormatString(QicsDataItem_Int, "$%d.00")
        self.myTable.mainGridRef().setFormatter(self.v1)

        for i in range(day_zero, day_three + 3):
            self.myTable.columnRef(i).setAlignment(QtCore.Qt.AlignRight)

        # the NYSE and NASDAQ rows...
        fnt = self.myTable.font()
        fnt.setPointSize(fnt.pointSize() + 2)
        fnt.setBold(True)
        self.myTable.rowRef(nyse_idx).setFont(fnt)
        self.myTable.rowRef(nasdaq_idx).setFont(fnt)

        self.v2 = QicsDataItemSprintfFormatter()
        self.v2.addFormatString(QicsDataItem_Float, "%.2f")
        self.v2.addFormatString(QicsDataItem_Int, "%d.00")
        self.myTable.rowRef(nyse_idx).setFormatter(self.v2)
        self.myTable.rowRef(nasdaq_idx).setFormatter(self.v2)

        self.myTable.freezeTopRows(2)

    def setupDay(self, day_idx, day):
        ch = self.myTable.columnHeader()

        ch.addCellSpan(QicsSpan(0,day_idx, 1, 3))
        ch.cellRef(0,day_idx).setLabel(day)

        ch.cellRef(0,day_idx).setForegroundColor(QtCore.Qt.white)
        ch.cellRef(0,day_idx).setBackgroundColor(QtCore.Qt.blue)

        ch.addCellSpan(QicsSpan(1,day_idx, 1, 2))
        ch.cellRef(1,day_idx).setLabel("Show Gains in")

        ch.dataModel().setItem(1,day_idx+2, QicsDataString("Green"))
        cd = QicsComboCellDisplay()
        cd.addItem("Green")
        cd.addItem("Red")
        ch.cellRef(1,day_idx+2).setDisplayer(cd)

        ch.cellRef(2,day_idx+cur_idx).setLabel("Current")

        ch.cellRef(2,day_idx+high_idx).setLabel("High")
        ch.cellRef(2,day_idx+high_idx).setPixmap(QtGui.QPixmap("high.xpm"))

        ch.cellRef(2,day_idx+low_idx).setLabel("Low")
        ch.cellRef(2,day_idx+low_idx).setPixmap(QtGui.QPixmap("low.xpm"))

        self._displayers.append(cd)
    def destroy(self):
	QtGui.qApp.quit()

class StockApp(QtGui.QApplication):
    def __init__(self, argv):
        QtGui.QApplication.__init__(self, argv)

        self.mySD = StockData()
        self.myUI = StockUI(self.mySD.dataModel())
    
        self.connect(self.myUI, QtCore.SIGNAL("updateToggled()"), self.toggleUpdates)

        self.myTimer = QtCore.QTimer()
        self.connect(self.myTimer, QtCore.SIGNAL("timeout()"), self.updateStocks)

        self.connect(self.mySD, QtCore.SIGNAL("stockChanged()"), self.myUI.decorateStock)

    def toggleUpdates(self, on = 0):
	#print "Clicked?"
        if on:
	    self.myTimer.start(update_period)
        else:
            self.myTimer.stop()

    def updateStocks(self):
	print "Auto update is on"
        self.mySD.updateStocks()

        self.myUI.table().sortRows(day_zero+cur_idx, Qics.Descending, 2, -1, SortFloats)
    def destroy(self):
	QtGui.qApp.quit()


def SortFloats(a, b):
    if a.type() == QicsDataItem_Float and b.type() == QicsDataItem_Float:
        fa = a.data()
        fb = b.data()

        if fa < fb:
            return -1
        elif fa == fb:
            return 0
        else:
            return 1
    else:
        sa = a.string()
        sb = b.string()
        return sa.compare(sb)


if __name__ == "__main__":
    app = StockApp(sys.argv)

    sys.exit(app.exec_())
