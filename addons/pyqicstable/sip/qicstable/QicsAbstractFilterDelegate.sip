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






class QicsAbstractFilterDelegate : Qics / NoDefaultCtors /
{
%TypeHeaderCode
#include <QicsAbstractFilterDelegate.h>
%End

public:
    QicsAbstractFilterDelegate();
    virtual ~QicsAbstractFilterDelegate();


    virtual bool match(const QString &string, int, int) = 0;
};













class QicsListFilterDelegate : QicsAbstractFilterDelegate / NoDefaultCtors /
{
%TypeHeaderCode
#include <QicsListFilterDelegate.h>
%End

public:

    QicsListFilterDelegate(const QStringList &, Qt::CaseSensitivity cs);
    ~QicsListFilterDelegate();


    QStringList list() const;
    void setList(const QStringList &);
    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity cs);
    virtual bool match(const QString &, int, int);
};








class QicsRegexpFilterDelegate : QicsAbstractFilterDelegate / NoDefaultCtors /
{
%TypeHeaderCode
#include <QicsRegexpFilterDelegate.h>
%End

public:

    QicsRegexpFilterDelegate(const QRegExp &);
    virtual ~QicsRegexpFilterDelegate();


    QRegExp regexp() const;
    void setRegexp(const QRegExp &);
    virtual bool match(const QString &, int, int);
};


