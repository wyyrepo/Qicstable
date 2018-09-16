#!/usr/bin/env python

## Copyright ( C ) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for PyQicsTable.  This example
## program may be used, distributed and modified without limitation.


import sys

from PyQt4 import QtCore, QtGui, Qt

from qicstable import QicsTable, QicsDataModel, QicsDataModelDefault, QicsRegion
from qicstable import QicsDataInt, QicsDataString, QicsDataFloat
from qicstable import QicsDataItem_Int, QicsDataItem_String, QicsDataItem_Float, QicsRowHeader


# This is an example of a custom data model.
#
# First, here are some existing classes that define stock data.  These are
# supposed to simulate the kinds of classes that a typical programmer might
# already have when s/he decides to use the QicsTable.
#
# We will be using these classes when we make the custom data model.


class StockPrivate:
    """
    Represents a single stock.
    """
    def __init__(self):
        self._symbol = QtCore.QString()
        self._high = 0.0
        self._low = 0.0
        self._close = 0.0
        self._volume = 0

    def symbol(self):
        return self._symbol

    def setSymbol(self, sym):
        self._symbol = sym

    def high(self):
        return self._high

    def setHigh(self, val):
        self._high = val

    def low(self):
        return self._low

    def setLow(self, val):
        self._low = val

    def close(self):
        return self._close

    def setClose(self, val):
        self._close = val

    def volume(self):
        return self._volume

    def setVolume(self, val):
        self._volume = val


class StockDataSet:
    """
    Represents a set of stocks.
    """
    def __init__(self):
        self._stocks = []

    def numStocks(self):
        return len(self._stocks)

    def insertStock(self, position=-1):
        sd = StockPrivate()

        if position < 0:
            self._stocks.append(sd)
        else:
            self._stocks.insert(position, sd)

    def removeStock(self, idx):
        if idx < len(self._stocks):
            del self._stocks[idx]

    def symbol(self, idx):
        if idx < len(self._stocks):
            return self._stocks[idx].symbol()

        return QtCore.QString()

    def setSymbol(self, idx, sym):
        if idx < len(self._stocks):
            self._stocks[idx].setSymbol(sym)

    def high(self, idx):
        if idx < len(self._stocks):
            return self._stocks[idx].high()

        return -1.0

    def setHigh(self, idx, val):
        if idx < len(self._stocks):
            self._stocks[idx].setHigh(val)

    def low(self, idx):
        if idx < len(self._stocks):
            return self._stocks[idx].low()

        return -1.0

    def setLow(self, idx, val):
        if idx < len(self._stocks):
            self._stocks[idx].setLow(val)

    def close(self, idx):
        if idx < len(self._stocks):
            return self._stocks[idx].close()

        return -1.0

    def setClose(self, idx, val):
        if idx < len(self._stocks):
            self._stocks[idx].setClose(val)

    def volume(self, idx):
        if idx < len(self._stocks):
            return self._stocks[idx].volume()

        return 0

    def setVolume(self, idx, val):
        if idx < len(self._stocks):
            self._stocks[idx].setVolume(val)


class StockDataModel(QicsDataModel, StockDataSet):
    SDM_Symbol = 0
    SDM_Close = 1
    SDM_High = 2
    SDM_Low = 3
    SDM_Volume = 4
    SDM_NumDataItems = 5

    def __init__(self):
        QicsDataModel.__init__(self)
        StockDataSet.__init__(self)

        self._item = None
	
	# Our data model always has SDM_NumDataItems columns.
	self.setNumColumns(self.SDM_NumDataItems)

    def item(self, row, col):
        self._item = None

        # We create a new QicsDataItem of the appropriate type and return it.

        if row < len(self._stocks):
            if col == StockDataModel.SDM_Symbol:
                self._item = QicsDataString(self.symbol(row))
            elif col == StockDataModel.SDM_Close:
                self._item = QicsDataFloat(self.close(row))
            elif col == StockDataModel.SDM_High:
                self._item = QicsDataFloat(self.high(row))
            elif col == StockDataModel.SDM_Low:
                self._item = QicsDataFloat(self.low(row))
            elif col == StockDataModel.SDM_Volume:
                self._item = QicsDataInt(self.volume(row))

        return self._item

    def rowItems(row, first_col=0, last_col=-1):
        if last_col < 0 or last_col > self.lastColumn():
            last_col = self.lastColumn()

        # Go through each column in the row and add the data item to the row
        # vector.

        rv = []

        for i in range(first_col, last_col + 1):
            rv.append(self.item(i, row).clone())

        return rv;

    def columnItems(col, first_row=0, last_row=-1):
        if last_row < 0 or last_row > self.lastRow():
            last_row = self.lastRow()

        # Go through each row in the column and add the data item to the column
        # vector.

        cv = []

        for i in range(first_row, last_row + 1):
            cv.append(self.item(i, col).clone())

        return cv;

    def setRowItems(self, row, v):
        col = 0

        # Temporarily turn signal emitting off, so setItem() doesn't emit a
        # signal for each cell that is changed.

        old_emit = self.emitSignals()
        self.setEmitSignals(False)

        # Iterate through each value in the row vector and use setItem() to do
        # the actual "setting".

        for iter in v:
            if col > StockDataModel.SDM_NumDataItems - 1:
                break

            self.setItem(row, col, iter)
            col += 1

        # Restore previous signal emitting setting.

        self.setEmitSignals(old_emit)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("modelChanged(QicsRegion)"), (QicsRegion(row, 0, row, col - 1), ))

    def setColumnItems(self, col, v):
        row = 0

        # Temporarily turn signal emitting off, so setItem() doesn't emit a
        # signal for each cell that is changed.

        old_emit = self.emitSignals()
        self.setEmitSignals(False)

        # Iterate through each value in the column vector and use setItem() to
        # do the actual "setting".

        for iter in v:
            if row > self.lastRow():
                break

            self.setItem(row, col, iter)
            row += 1

        # Restore previous signal emitting setting.

        self.setEmitSignals(old_emit)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("modelChanged(QicsRegion)"), QicsRegion(0, col, row - 1, col))

    def setSymbol(self, idx, sym):
        # We call appropriate StockDataSet method here to actually store the
        # value, and then we emit the required signal.

        StockDataSet.setSymbol(self, idx, sym)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("modelChanged(QicsRegion)"), QicsRegion(idx, StockDataModel.SDM_Symbol, idx, StockDataModel.SDM_Symbol) )

    def setHigh(self, idx, val):
        # We call appropriate StockDataSet method here to actually store the
        # value, and then we emit the required signal.

        StockDataSet.setHigh(self, idx, val)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("modelChanged(QicsRegion)"), QicsRegion(idx, StockDataModel.SDM_High, idx, StockDataModel.SDM_High))

    def setLow(self, idx, val):
        # We call appropriate StockDataSet method here to actually store the
        # value, and then we emit the required signal.

        StockDataSet.setLow(self, idx, val)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("modelChanged(QicsRegion)"), QicsRegion(idx, StockDataModel.SDM_Low, idx, StockDataModel.SDM_Low))

    def setClose(self, idx, val):
        # We call appropriate StockDataSet method here to actually store the
        # value, and then we emit the required signal.

        StockDataSet.setClose(self, idx, val)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("modelChanged(QicsRegion)"), QicsRegion(idx, StockDataModel.SDM_Close, idx, StockDataModel.SDM_Close))

    def setVolume(self, idx, val):
        # We call appropriate StockDataSet method here to actually store the
        # value, and then we emit the required signal.

        StockDataSet.setVolume(self, idx, val)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("modelChanged(QicsRegion)"), QicsRegion(idx, StockDataModel.SDM_Volume, idx, StockDataModel.SDM_Volume))

    def insertStock(self, position=-1):
        # We call appropriate StockDataSet method here to actually insert the
        # row, and then we emit the required signal.

        StockDataSet.insertStock(self, position)

        self.setNumRows(self.numRows() + 1)

        if self.emitSignals():
            if position == -1:
                self.emit(QtCore.SIGNAL("rowsAdded(int)"),1)
            else:
                self.emit(QtCore.SIGNAL("rowsInserted(int, int)"), 1, position)

            self.emit(QtCore.SIGNAL("modelSizeChanged(int, int)"), self.numRows(), self.numColumns())

    def removeStock(self, idx):
        # We call appropriate StockDataSet method here to actually remove the
        # row, and then we emit the required signal.

        StockDataSet.removeStock(self, idx)

        self.setNumRows(self.numRows() - 1)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("rowsDeleted(int, int)"), (1, idx))
            self.emit(QtCore.SIGNAL("modelSizeChanged(int, int)"), (self.numRows(), self.numColumns()))

    def setItem(self, row, col, item):
        if row < self.numStocks():
            # Here, we determine which stock attribute we should be setting by
            # looking at the column attribute.

            # The modelChanged() signal will be emitted by the individual set
            # methods (setSymbol, setClose, etc).

            if col == StockDataModel.SDM_Symbol:
                if item.type() == QicsDataItem_String:
                    self.setSymbol(row, item.data())
            elif col == StockDataModel.SDM_Close:
                if item.type() == QicsDataItem_Float:
                    self.setClose(row, item.data())
            elif col == StockDataModel.SDM_High:
                if item.type() == QicsDataItem_Float:
                    self.setHigh(row, item.data())
            elif col == StockDataModel.SDM_Low:
                if item.type() == QicsDataItem_Float:
                    self.setLow(row, item.data())
            elif col == StockDataModel.SDM_Volume:
                if item.type() == QicsDataItem_Int:
                    self.setVolume(row, item.data())

    def clearItem(self, row, col):
        if row < self.numStocks():
            # We determine which stock attribute we should be clearing by
            # looking at the column attribute.

            # The modelChanged() signal will be emitted by the individual set
            # methods (setSymbol, setClose, etc).

            if col == StockDataModel.SDM_Symbol:
                self.setSymbol(row, QString())
            elif col == StockDataModel.SDM_Close:
                self.setClose(row, 0.0)
            elif col == StockDataModel.SDM_High:
                self.setHigh(row, 0.0)
            elif col == StockDataModel.SDM_Low:
                self.setLow(row, 0.0)
            elif col == StockDataModel.SDM_Volume:
                self.setClose(row, 0)

    def clearModel(self):
        nrows = self.numStocks()

        # Temporarily turn signal emitting off, so removeStock() doesn't emit a
        # signal for each row that is removed.

        old_emit = self.emitSignals()
        self.setEmitSignals(False)

        for i in range(nrows):
            self.removeStock(0);

        # Restore previous signal emitting setting.

        self.setEmitSignals(old_emit)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("modelSizeChanged(int, int)"), (self.numRows(), self.numColumns()))

    def addRows(self, number_of_rows):
        # We use the existing insertStock() call to do the add, but we have to
        # emit the required signal so all views will update

        # Temporarily turn signal emitting off, so insertStock() doesn't emit a
        # signal for each row that is inserted.

        old_emit = self.emitSignals()
        self.setEmitSignals(False)

        for i in range(number_of_rows):
            self.insertStock();

        # Restore previous signal emitting setting.

        self.setEmitSignals(old_emit)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("rowsAdded(int)"), (number_of_rows, ))
            self.emit(QtCore.SIGNAL("modelSizeChanged(int, int)"), (self.numRows(), self.numColumns()))

    def addColumns(self, number_of_columns):
        # We don't allow adding columns (each stock has a fixed number of data
        # points).  So we just return without emitting any signals.
        return

    def insertRows(self, number_of_rows, starting_position):
        # We use the existing insertStock() call to do the insert, but we have
        # to emit the required signal so all views will update.

        # Temporarily turn signal emitting off, so setItem() doesn't emit a
        # signal for each cell that is changed.

        old_emit = self.emitSignals()
        self.setEmitSignals(False)

        for i in range(number_of_rows):
            self.insertStock(starting_position)

        # Restore previous signal emitting setting.

        self.setEmitSignals(old_emit)

        if self.emitSignals():
            self.emit(QtCore.SIGNAL("rowsInserted(int, int)"), (number_of_rows, starting_position))
            self.emit(QtCore.SIGNAL("modelSizeChanged(int, int)"), (self.numRows(), self.numColumns()))

    def insertColumns(self, number_of_columns, starting_position):
        # We don't allow inserting columns (each stock has a fixed number of
        # data points).  So we just return without emitting any signals.
        return

    def deleteRow(self, row):
        # We use the existing removeStock() call to do the delete, which will
        # also emit the required signal.

        if row < self.numStocks():
            self.removeStock(row)

    def deleteRows(self, num_rows, start_row):
        # We use the existing removeStock() call to do the delete, but we have
        # to emit the required signal so all views will update.

        # Temporarily turn signal emitting off, so setItem() doesn't emit a
        # signal for each cell that is changed.

        old_emit = self.emitSignals()
        self.setEmitSignals(False)

        num_deleted = 0

        for i in range(num_rows):
            if start_row < self.numStocks():
                self.removeStock(start_row)
                num_deleted += 1

        # Restore previous signal emitting setting.

        self.setEmitSignals(old_emit)

        if num_deleted > 0 and self.emitSignals():
            self.emit(QtCore.SIGNAL("rowsDeleted(int, int)"), (1, num_deleted))
            self.emit(QtCore.SIGNAL("modelSizeChanged(int, int)"), (self.numRows(), self.numColumns()))

    def deleteColumn(self, col):
        # We don't allow deleting columns (each stock has a fixed number of
        # data points).  So we just return without emitting any signals.
        return

    def deleteColumns(self, num_cols, start_col):
        # We don't allow deleting columns (each stock has a fixed number of
        # data points).  So we just return without emitting any signals.
        return


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)

    # Create the data model.
    dm = StockDataModel()

    # Fill the data model with some values.
    dm.insertStock()
    dm.setSymbol(0, "ATT")
    dm.setClose(0, 37.73)
    dm.setHigh(0, 38.0)
    dm.setLow(0, 37.55)
    dm.setVolume(0, 503333)

    dm.insertStock()
    dm.setSymbol(1, "RJR")
    dm.setClose(1, 67.05)
    dm.setHigh(1, 67.05)
    dm.setLow(1, 64.89)
    dm.setVolume(1, 997323)

    # Create the table, using the data model we created above.
    table = QicsTable(dm)

#    app.setMainWidget(table)
    table.show()

    sys.exit(app.exec_())
