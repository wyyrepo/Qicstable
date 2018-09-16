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

#ifndef QICSREPEATINGCOLUMN_H
#define QICSREPEATINGCOLUMN_H

#include "QicsCellCommon.h"

class QicsColumn;
class QicsTable;

////////////////////////////////////////////////////////////////////////////////

/*! \class QicsRepeatingColumn
* \nosubgrouping
* \brief A lightweight object permitting access to attribute settings for
*        a repeating set of columns
*
* QicsRepeatingColumn is the programmer's access point for attribute settings
* for a repeating set of columns.  These columns are specified by a starting column,
* and an repeat interval.  All attributes of individual columns can be set
* using this class.  Retrieving attributes of this class is rarely useful.
* Retrieving attributes of the class will always return the value of the
* attribute in the starting column.  This value may have been set by a repeating
* column setting, it may have been explicitly set on this individual column, or it
* may be the default value for the grid if no value was set for this column.
*
* QicsRepeatingColumn is a flyweight class representing a repeating set of columns
* in the table.  Instances of this object are created only when necessary --
* there is no large array of repeating column objects contained in the table.
* Rather, they are created directly (via the public constructor) or indirectly
* (via QicsTable::repeatingColumn(), QicsTable::repeatingColumnRef(),
* QicsHeader::repeatingColumn(), or QicsHeader::repeatingColumnRef()) by the
* programmer when needed.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsRepeatingColumn: public QicsCellCommon
{
    Q_OBJECT

    friend class QicsTableCommon;
    friend class QicsHeader;
public:
    /** @name Column Properties
    */

    //@{
    /*!
    * Specifies the starting visual column index of this object.
    */
    Q_PROPERTY( int startColumnIndex READ startColumnIndex WRITE setStartColumnIndex )

    /*!
    * Specifies the column interval of this object.
    */
    Q_PROPERTY( int interval READ interval WRITE setInterval )

    /*!
    * Specifies the width of the repeating columns in pixels.
    * If the width of the columns have been set in character units,
    * retrieving this property will return the equivalent width in pixels.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int widthInPixels READ widthInPixels WRITE setWidthInPixels )
    /*!
    * Specifies the width of the repeating columns in character units.
    * This means that setting this value to 1 will change the width of
    * the columns to be just large enough to display one line of text (plus
    * the margin size and border width).
    * If the width of the columns has been set by pixel, retrieving
    * this property will return the equivalent width in character units
    * based on the current font.
    *
    * The default value of this property is dependent on the width
    * of the default font.
    */
    Q_PROPERTY( int widthInChars READ widthInChars WRITE setWidthInChars )
    //@}

public:
    /*!
    * Constructor for the class.
    * \param start_col starting column index
    * \param interval how often to repeat these settings
    * \param table the cell's table
    */
    QicsRepeatingColumn(int start_col, unsigned int interval, QicsTable *table);

    /*!
    * \internal
    * Constructor that should only be called by QicsTable internals.
    */
    QicsRepeatingColumn(int start_col, unsigned int interval, QicsGridInfo *info,
        QObject *parent = 0);

    /*!
    * Returns the starting column index of this repeating column object.
    */
    inline int startColumnIndex() const { return myStartColumn; }

    /*!
    * Returns the interval setting of this repeating column object.
    *  property for details.
    */
    inline int interval() const { return myInterval; }

    /*!
    *  Returns the width of this repeating column in pixels.
    *  See the \link #widthInPixels "widthInPixels" \endlink
    *  property for details.
    *  \sa setWidthInPixels()
    */
    int widthInPixels() const;

    /*!
    *  Returns the width of this repeating column in character units (i.e. lines).
    *  See the \link #widthInChars "widthInChars" \endlink
    *  property for details.
    *  \sa setWidthInChars()
    */
    int widthInChars() const;

public slots:
    /*!
    * Sets the starting column index of this object to \a idx.
    * \sa startColumnIndex()
    */
    inline void setStartColumnIndex(int idx) { myStartColumn = idx; }

    /*!
    * Sets the column interval of this object to \a interval.
    * \sa interval()
    */
    inline void setInterval(int interval) { myInterval = interval; }

    /*!
    *  Sets the width of this repeating column in pixels.
    *  See the \link #widthInPixels "widthInPixels" \endlink
    *  property for details.
    * \sa widthInPixels()
    */
    void setWidthInPixels(int width);

    /*!
    *  Sets the width of this repeating column in character units (i.e. lines).
    *  See the \link #widthInChars "widthInChars" \endlink
    *  property for details.
    * \sa widthInChars()
    */
    void setWidthInChars(int width);

protected:
    /*!
    * \internal
    * Common initialization routine
    */
    void init(int start_col, unsigned int interval);

    virtual void setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val);
    virtual void *getAttr(QicsCellStyle::QicsCellStyleProperty attr) const;
    virtual void clearAttr(QicsCellStyle::QicsCellStyleProperty attr);

    virtual void setDMMargin(int margin);
    virtual void setDMFont(const QFont &font);
    virtual void setDMBorderWidth(int bw);

    /*!
    * \internal
    * starting column index of this object
    */
    int myStartColumn;

    /*!
    * \internal
    * interval for this object
    */
    int myInterval;

#ifdef Q_DISABLE_COPY
private:
    QicsRepeatingColumn(const QicsColumn& col);
    QicsRepeatingColumn &operator=(const QicsColumn& col);
#endif
};

#endif //QICSREPEATINGCOLUMN_H


