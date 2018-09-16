/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

// This is an example of a custom data type.  This data type represents
// a QPoint value.

#include "custom_data_type.h"



QicsDataQPoint::QicsDataQPoint()
    : QicsDataItem()
{
}

QicsDataQPoint::QicsDataQPoint(const QPoint &p)
    : QicsDataItem()
{
    setData(p);
}

QicsDataQPoint::QicsDataQPoint(const QicsDataQPoint &di)
    : QicsDataItem()
{
    setData(di.data());
}

bool QicsDataQPoint::setString(const QString &str)
{
    // This is very simple.  There should be more error checking
    // in a robust version of this class

    int x = (str.right(str.length() - 1)).section(',', 0, 0).toInt();
    int y = (str.left(str.length() - 1)).section(',', 1, 1).toInt();

    setData(QPoint(x, y));

    return true;
}

QString QicsDataQPoint::format(const char *fmt_string) const
{
    QString str;

    return str.sprintf(fmt_string, m_data.x(), m_data.y());
}

int QicsDataQPoint::compareTo(const QicsDataItem &x) const
{
    Q_ASSERT(this->type() == x.type());

    const QicsDataQPoint *v = dynamic_cast<const QicsDataQPoint *> (&x);
    Q_ASSERT(v);

    if (m_data.x() < v->m_data.x())
        return -1;
    if (m_data.x() > v->m_data.x())
        return 1;
    if (m_data.y() < v->m_data.y())
        return -1;
    if (m_data.y() > v->m_data.y())
        return 1;

    return 0;
}

void QicsDataQPoint::encode(QDataStream &ds) const
{
    ds << typeString();
    ds << m_data;
}

QicsDataItem *QicsDataQPoint::fromString(const QString &str)
{
    QicsDataQPoint *qp = new QicsDataQPoint();

    if (qp->setString(str))
        return qp;

    delete qp;
    return 0;
}

QicsDataItem *QicsDataQPoint::decode(QDataStream &ds)
{
    QPoint val;

    ds >> val;
    return new QicsDataQPoint(val);
}


