/*********************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#include "QicsRepeatingCellStyle.h"


QicsRepeatingCellStyle::QicsRepeatingCellStyle(Qics::QicsIndexType type, int start,
                                               unsigned int interval)
    : QicsCellStyle(TableGrid, false), myType(type)
{
    // The grid type will not be used by the QicsCellStyle constructor,
    // because it will not initialize to default values.

    myStart = start;
    if (myStart < 0)
        myStart = 0;

    myInterval = interval;
    if (myInterval <= 0)
        myInterval = 1;
}

QDomElement QicsRepeatingCellStyle::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement repElement = QicsCellStyle::toDomXml(tag,doc);

    QString attrName = "start";
    QString attrValue = QString::number(myStart);
    repElement.setAttribute(attrName, attrValue);

    attrName = "interval";
    attrValue = QString::number(myInterval);
    repElement.setAttribute(attrName, attrValue);

    return repElement;
}

void QicsRepeatingCellStyle::configureFromDomXml(const QDomElement& e, QicsTable* table)
{
    QicsCellStyle::configureFromDomXml(e, table);
    myStart = e.attribute("start").toInt();
    myInterval = e.attribute("interval").toInt();
}


