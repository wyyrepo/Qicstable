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

#ifndef QICSREPEATINGROW_H
#define QICSREPEATINGROW_H

#include "QicsCellCommon.h"

class QicsRow;
class QicsTable;

////////////////////////////////////////////////////////////////////////////////

/*! \class QicsRepeatingRow
* \nosubgrouping
* \brief A lightweight object permitting access to attribute settings for
*        a repeating set of rows
*
* QicsRepeatingRow is the programmer's access point for attribute settings
* for a repeating set of rows.  These rows are specified by a starting row,
* and an repeat interval.  All attributes of individual rows can be set
* using this class.  Retrieving attributes of this class is rarely useful.
* Retrieving attributes of the class will always return the value of the
* attribute in the starting row.  This value may have been set by a repeating
* row setting, it may have been explicitly set on this individual row, or it
* may be the default value for the grid if no value was set for this row.
*
* QicsRepeatingRow is a flyweight class representing a repeating set of rows
* in the table.  Instances of this object are created only when necessary --
* there is no large array of repeating row objects contained in the table.
* Rather, they are created directly (via the public constructor) or indirectly
* (via QicsTable::repeatingRow(), QicsTable::repeatingRowRef(),
* QicsHeader::repeatingRow(), or QicsHeader::repeatingRowRef()) by the
* programmer when needed.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsRepeatingRow: public QicsCellCommon
{
    Q_OBJECT

    friend class QicsTableCommon;
    friend class QicsHeader;
public:
    /** @name Repeating Row Properties
    */

    //@{
    /*!
    * Specifies the starting visual row index of this object.
    */
    Q_PROPERTY( int startRowIndex READ startRowIndex WRITE setStartRowIndex )

    /*!
    * Specifies the row interval of this object.
    */
    Q_PROPERTY( int interval READ interval WRITE setInterval )

    /*!
    * Specifies the height of the repeating rows in pixels.
    * If the height of the rows have been set in character units,
    * retrieving this property will return the equivalent height in pixels.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int heightInPixels READ heightInPixels WRITE setHeightInPixels )
    /*!
    * Specifies the height of the repeating rows in character units.
    * This means that setting this value to 1 will change the height of
    * the rows to be just large enough to display one line of text (plus
    * the margin size and border width).
    * If the height of the rows has been set by pixel, retrieving
    * this property will return the equivalent height in character units
    * based on the current font.
    *
    * The default value of this property is dependent on the height
    * of the default font.
    */
    Q_PROPERTY( int heightInChars READ heightInChars WRITE setHeightInChars )
    //@}

public:
    /*!
    * Constructor for the class.
    * \param start_row starting row index
    * \param interval how often to repeat these settings
    * \param table the cell's table
    */
    QicsRepeatingRow(int start_row, unsigned int interval, QicsTable *table);

    /*!
    * \internal
    * Constructor that should only be called by QicsTable internals.
    */
    QicsRepeatingRow(int start_row, unsigned int interval, QicsGridInfo *info,
        QObject *parent = 0);

    /*!
    * Returns the starting row index of this repeating row object.
    */
    inline int startRowIndex() const { return myStartRow; }

    /*!
    * Returns the interval setting of this repeating row object.
    *  property for details.
    */
    inline int interval() const { return myInterval; }

    /*!
    *  Returns the height of this repeating row in pixels.
    *  See the \link #heightInPixels "heightInPixels" \endlink
    *  property for details.
    *  \sa setHeightInPixels()
    */
    int heightInPixels() const;

    /*!
    *  Returns the height of this repeating row in character units (i.e. lines).
    *  See the \link #heightInChars "heightInChars" \endlink
    *  property for details.
    *  \sa setHeightInChars()
    */
    int heightInChars() const;

public slots:
    /*!
    * Sets the starting row index of this object to \a idx.
    * \sa startRowIndex()
    */
    inline void setStartRowIndex(int idx) { myStartRow = idx; }

    /*!
    * Sets the row interval of this object to \a interval.
    * \sa interval()
    */
    inline void setInterval(int interval) { myInterval = interval; }

    /*!
    *  Sets the height of this repeating row in pixels.
    *  See the \link #heightInPixels "heightInPixels" \endlink
    *  property for details.
    * \sa heightInPixels()
    */
    void setHeightInPixels(int height);

    /*!
    *  Sets the height of this repeating row in character units (i.e. lines).
    *  See the \link #heightInChars "heightInChars" \endlink
    *  property for details.
    * \sa heightInChars()
    */
    void setHeightInChars(int height);

protected:
    /*!
    * \internal
    * Common initialization routine
    */
    void init(int start_row, unsigned int interval);

    virtual void setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val);
    virtual void *getAttr(QicsCellStyle::QicsCellStyleProperty attr) const;
    virtual void clearAttr(QicsCellStyle::QicsCellStyleProperty attr);

    virtual void setDMMargin(int margin);
    virtual void setDMFont(const QFont &font);
    virtual void setDMBorderWidth(int bw);

    /*!
    * \internal
    * starting row index of this object
    */
    int myStartRow;

    /*!
    * \internal
    * interval for this object
    */
    int myInterval;

#ifdef Q_DISABLE_COPY
private:
    QicsRepeatingRow(const QicsRow& row);
    QicsRepeatingRow &operator=(const QicsRow& row);
#endif
};

#endif //QICSREPEATINGROW_H


