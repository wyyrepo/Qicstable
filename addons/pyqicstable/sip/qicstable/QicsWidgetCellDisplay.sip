// QicsWidgetCellDisplay.sip generated by MetaSIP on Thu Aug 26 12:32:36 2010
//
// This file is part of the qicstable Python extension module.
//
// Copyright (c) 2004 - 2014
// 	Integrated Computer Solutions, Inc. <info@ics.com>
// 
// This file is part of PyQicsTable.
// 
// This copy of PyQicsTable is licensed for use under the terms of the
// PyQicsTable Commercial License Agreement version ICS-2.6.  See the file
// LICENSE for more details.
// 
// PyQicsTable is supplied WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


class QicsWidgetCellDisplay : QicsCellDisplay, QObject
{
%TypeHeaderCode
#include <QicsWidgetCellDisplay.h>
%End

public:
    QicsWidgetCellDisplay(QWidget *);
    virtual ~QicsWidgetCellDisplay();
    // Old const QRect &...
    virtual void displayCell(QicsGrid *, int, int, const QicsDataItem *, QRect &, QPainter *);
    virtual void startEdit(QicsScreenGrid *, int, int, const QicsDataItem *);
    virtual void moveEdit(QicsScreenGrid *, int, int, const QRect &);
    virtual void endEdit(QicsScreenGrid *, int, int);
    virtual QSize sizeHint(QicsGrid *, int, int, const QicsDataItem *);
    virtual bool editWhenCurrent() const;
    virtual bool needsVisibilityNotification() const;
    virtual bool isEmpty(QicsGridInfo *, int, int, const QicsDataItem *) const;
	
private:
	QicsWidgetCellDisplay(const QicsWidgetCellDisplay &);
};

