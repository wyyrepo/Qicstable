#!/usr/bin/env python

## Copyright ( C ) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of an example program for PyQicsTable.  This example
## program may be used, distributed and modified without limitation.


import sys

from PyQt4 import QtCore, QtGui, Qt

from qicstable import QicsTable, QicsDataModel, QicsDataModelDefault
from qicstable import QicsDataItem, QicsDataItem_UserDefined


class QicsDataQPoint(QicsDataItem):
    """
    This is an example of a custom data type.  This data type represents a
    QPoint value.
    """
    def __init__(self, p=None):
        QicsDataItem.__init__(self)

        if isinstance(p, QicsDataQPoint):
            self.setData(p.data())
        elif isinstance(p, QtCore.QPoint):
            self.setData(p)
        else:
            self.myData = QPoint()

    def create(self):
        return QicsDataQPoint()

    def clone(self):
        return QicsDataQPoint(self.myData)

    def type(self):
        return QicsDataItem_UserDefined

    def typeName():
        return QtCore.QString("qpoint")
    typeName = staticmethod(typeName)

    def typeString(self):
        return QicsDataQPoint.typeName()

    def string(self):
        return QtCore.QString("(%1, %2)").arg(self.myData.x()).arg(self.myData.y())

    def setString(self, str):
        # This is very simple.  There should be more error checking in a robust
        # version of this class.

        x = (str.right(str.length() - 1)).section(',', 0, 0).toInt()[0]
        y = (str.left(str.length() - 1)).section(',', 1, 1).toInt()[0]

        self.setData(QtCore.QPoint(x, y))
        return True

    def number(ok=None):
        if (ok != None):
		    ok = False
        return -1		
		
    def data(self):
        return self.myData

    def setData(self, p):
        # Take a copy of the point, not just another reference to it.
        self.myData = QtCore.QPoint(p)

    def format(self, fmt_string):
        return QtCore.QString(fmt_string % (self.myData.x(), self.myData.y()))

    def compareTo(self, v):
        assert(isinstance(v, QicsDataQPoint))

        if self.myData.x() < v.myData.x():
            return -1

        if self.myData.x() > v.myData.x():
            return 1

        if self.myData.y() < v.myData.y():
            return -1

        if self.myData.y() > v.myData.y():
            return 1

        return 0

    def encode(self, ds):
        ds << self.typeString()
        ds << self.myData

    def fromString(str):
        qp = QicsDataQPoint()

        if qp.setString(str):
            return qp

        return None

    def decode(ds):
        val = QtCore.QPoint()

        ds >> val
        return QicsDataQPoint(val)


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)

    QicsDataItem.registerType(QicsDataQPoint.typeName(),
                              QicsDataQPoint.fromString,
                              QicsDataQPoint.decode)

    nrows = 10
    ncols = 10

    # Create the data model.
    dm = QicsDataModelDefault(nrows, ncols)

    # Fill the data mode with some QPoints.
    for i in range(nrows):
        for j in range(ncols):
            dm.setItem(i, j, QicsDataQPoint(QtCore.QPoint(i, j)))

    # Create the table, using the data model we created above.
    table = QicsTable(dm)

#    app.setMainWidget(table)
    table.show()

    sys.exit(app.exec_())

