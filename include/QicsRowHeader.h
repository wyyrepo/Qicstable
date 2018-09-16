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

#ifndef QICSROWHEADER_H
#define QICSROWHEADER_H

#include "QicsHeader.h"

class QicsTable;

/////////////////////////////////////////////////////////////////////////////////

/*! \class QicsRowHeader QicsRowHeader.h
*  \nosubgrouping
*  \brief The row header object for the table
*
* QicsRowHeader is the programmer's interface to the header object that
* appears at the top and/or bottom of each row in the QicsTable widget.
* All attributes of individual cells can be set using this class.
*
* QicsRowHeader is a flyweight class representing a single header in the table.
* These objects are created directly (via the public constructor)
* or indirectly (via QicsTable::rowHeader() or QicsTable::rowHeaderRef())
* when needed.
*
* Persistent objects, created by the public constructor or by
* QicsTable::rowHeader() are valid until they are explicitly destroyed,
* or until the table itself is destroyed.
*
* If you wish to modify or query a row, column, or cell in the row header, you
* must first obtain a reference to the object by calling #cellRef(), #rowRef(),
* or columnRef() (or, if you need a newly allocated object, #cell(), #row(),
* or #column()).
* \code
* // We want to set the label of the first cell in the row header
*
* table->rowHeaderRef().cellRef(0,0).setLabel(QString("The Label"));
* \endcode
*
* Row headers can have more than one column.  To change the number of columns
* in a row header, use #setNumColumns().  Once you have added columns to
* the row header, you may add data to the new cells, set labels, and otherwise
* modify the cells.
* \code
* // We want to add a column to the row header, and set the new column's
* // background color to red
*
* table->rowHeaderRef().setNumColumns(2);
* table->rowHeaderRef().columnRef(1).setBackgroundColor(red);
* \endcode
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////
class QICS_EXPORT QicsRowHeader: public QicsHeader
{
    Q_OBJECT

    friend class QicsTableCommon;
public:

    /** @name Row Header Properties
    */

    //@{
    /*!
    * Specifies the number of columns in the row header.
    *
    * The default value of this property is \b 1.
    */
    Q_PROPERTY( int numColumns READ numColumns WRITE setNumColumns )
    //@}

public:
    /*!
    * Constructor for the class.
    * \param table the cell's table
    */
    QicsRowHeader(QicsTable *table);
    /*!
    * Destructor for the class.
    */
    virtual ~QicsRowHeader();

    /*!
    * Returns the number of columns in the row header.
    */
    int numColumns() const;

    /*!
    * Sets the default row height for row header to \a height pixels
    * for the entire table.
    */
    void setDefaultRowHeightInPixels(int height);

    /*!
    * Sets the default row height for row header to \a height character units (i.e. lines)
    * for the entire table.
    */
    void setDefaultRowHeightInChars(int height);

    /*!
    * Sets the default column width for row header to \a width pixels
    * for the entire table.
    */
    void setDefaultColumnWidthInPixels(int width);

    /*!
    * Sets the default column width for row header to \a width character units
    * for the entire table.
    */
    void setDefaultColumnWidthInChars(int width);

public slots:
    /*!
    * Sets the number of columns in the row header.
    */
    void setNumColumns(int num);

protected:
    /*! \internal
    * Constructor for the header.  Meant only for use by the QicsTable.
    */
    QicsRowHeader(QicsGridInfo *info, QObject *parent = 0,
        bool forward_signals = false);

#ifdef Q_DISABLE_COPY
private:
    QicsRowHeader(const QicsRowHeader& rh);
    QicsRowHeader &operator=(const QicsRowHeader& rh);
#endif
};

#endif //QICSROWHEADER_H


