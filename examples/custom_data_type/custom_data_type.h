/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef CUSTOM_DATA_TYPE_H
#define CUSTOM_DATA_TYPE_H

#include <QicsDataItem.h>
#include <QPoint>

// This is an example of a custom data type.  This data type represents
// a QPoint value.

class QicsDataQPoint : public QicsDataItem
{
public:
    QicsDataQPoint();
    QicsDataQPoint(const QPoint &p);
    QicsDataQPoint(const QicsDataQPoint &di);

    inline QicsDataItem *create() const
    { return new QicsDataQPoint(); }

    inline QicsDataItem *clone() const
    { return new QicsDataQPoint(*this); }

    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_UserDefined); }

    inline static QString typeName()
    { return QString("qpoint"); }

    inline virtual QString typeString() const
    { return QicsDataQPoint::typeName(); }

    inline virtual QicsDataQPoint& operator=(const QPoint & p)
    { setData(p); return *this;}

    inline virtual const QString string(void) const
    { return QString("(%1, %2)").arg(m_data.x()) .arg(m_data.y()); }

    virtual bool setString(const QString &str);

    virtual double number(bool *ok = 0) const	// it does not support conversion to number
    { if (ok) *ok = false; return -1; }

    inline QPoint data() const { return m_data; }
    inline void setData(const QPoint & p) { m_data = p;}

    virtual QString format(const char *fmt_string) const;

    int compareTo(const QicsDataItem &x) const;

    void encode(QDataStream &ds) const;

    static QicsDataItem *fromString(const QString &str);
    static QicsDataItem *decode(QDataStream &ds);

protected:
    QPoint m_data;
};


#endif //CUSTOM_DATA_TYPE_H


