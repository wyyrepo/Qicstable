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

#ifndef QICSDATAITEMFORMATTER_H
#define QICSDATAITEMFORMATTER_H

#include <QMap>
#include "QicsDataItem.h"

/*!
* \class QicsDataItemFormatter QicsDataItemFormatter.h
* \brief Formatting class for QicsDataItems
*
* QicsDataItemFormatter is an abstract class defining the interface
* to a formatting object.  Currently there is only one method
* defined for the formatter -- QicsDataItemFormater::format.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

#include <QObject>


class QICS_EXPORT QicsDataItemFormatter : public QObject
{
    Q_OBJECT
public:
    /*!
    * Constructs a formatter object.
    */
    QicsDataItemFormatter(QObject *parent = 0);

    /*!
    * Destructor.
    */
    virtual ~QicsDataItemFormatter();

    /*!
    * Returns a formatted string representing the data item \a itm.
    */
    virtual QString format(const QicsDataItem &itm) const = 0;
};

/////////////////////////////////////////////////////////////////////

/*!
* \class QicsDataItemSprintfFormatter QicsDataItemFormatter.h
* \brief Formatter that uses sprintf formatting commands
*
* QicsDataItemSprintfFormatter is an implementation of the
* QicsDataItemFormatter interface that uses sprintf-type formatting
* strings when formatting data items.  The programmer provides
* separate formatting strings for each data type by calling
* QicsDataItemFormatter::addFormatString.  If no formatting string
* has been added for a type, that type will not be formatted -- its
* default string representation will be returned.
*/

class QICS_EXPORT QicsDataItemSprintfFormatter: public QicsDataItemFormatter
{
    Q_OBJECT
public:
    /*!
    * Constructs an empty formatter.
    */
    QicsDataItemSprintfFormatter(QObject *parent = 0);
    /*!
    * Destructor.
    */
    virtual ~QicsDataItemSprintfFormatter();

    virtual QString format(const QicsDataItem &itm) const;

    /*!
    * Adds the sprintf formatting string \a format_string to
    * the formatter.  This format string will be used when formatting
    * data items of type \a type.
    */
    void addFormatString(QicsDataItemType type, const char *format_string);
    /*!
    * Removes the current formatting string (if any) for type \a type
    * from the formatter.
    */
    void removeFormatString(QicsDataItemType type);

protected:
    QMap<QicsDataItemType,const char *> myFormats;
};

#endif //QICSDATAITEMFORMATTER_H


