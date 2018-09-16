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

#ifndef QICSREPEATINGCELLSTYLE_H
#define QICSREPEATINGCELLSTYLE_H

#include <QVector>
#include <QDomElement>
#include <QicsCellStyle.h>

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsTable;

class QicsRepeatingCellStyle : public QicsCellStyle
{
public:
    QicsRepeatingCellStyle(Qics::QicsIndexType type, int start,
        unsigned int interval);

    inline Qics::QicsIndexType type() const { return myType; };

    inline int start() const { return myStart; }

    inline unsigned int interval() const { return myInterval; }

    virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;

    virtual void configureFromDomXml(const QDomElement&, QicsTable *table);

protected:
    Qics::QicsIndexType myType;
    int myStart;
    unsigned int myInterval;
};

typedef QVector<QicsRepeatingCellStyle *> QicsRepeatingCellStylePV;

#endif //QICSREPEATINGCELLSTYLE_H


