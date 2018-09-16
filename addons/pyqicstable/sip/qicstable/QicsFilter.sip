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



class QicsFilter : Qics, QObject / Abstract /
{
%TypeHeaderCode
#include <QicsFilter.h>
%End

public:
    QicsFilter(QicsGridInfo *);
    ~QicsFilter();
    QicsAbstractFilterDelegate* filter(int );
    bool hasFilter(int ) const;
    virtual void setFilter(int , QicsAbstractFilterDelegate *, bool ) = 0;
    virtual void removeFilter(int , bool ) = 0;
    virtual void removeAll() = 0;
    bool isFiltered(int ) const;

signals:

	void filterChanged(int , bool );

protected:
    // called by QicsDimensionManager
    virtual void handleRowsAdded(int , int ) = 0;
    virtual void handleRowsRemoved(int , int ) = 0;
    virtual void handleColumnsAdded(int , int ) = 0;
    virtual void handleColumnsRemoved(int , int ) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// QicsRowFilter
///////////////////////////////////////////////////////////////////////////////


class QicsRowFilter : QicsFilter
{
%TypeHeaderCode
#include <QicsFilter.h>
%End

public:
    QicsRowFilter(QicsGridInfo *info);
    virtual void setFilter(int , QicsAbstractFilterDelegate *, bool );
    virtual void removeFilter(int , bool );
    virtual void removeAll();
    virtual bool isRowMatchFilters(int );

    virtual void handleRowsAdded(int , int );
    virtual void handleRowsRemoved(int , int );
    virtual void handleColumnsAdded(int , int );
    virtual void handleColumnsRemoved(int , int );

protected:
    // internal
    void doUpdateHiddenRows();
	
private:
	QicsRowFilter(const QicsRowFilter &);
};

