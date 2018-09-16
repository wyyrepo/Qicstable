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

#ifndef QICSSTYLE_H
#define QICSSTYLE_H

#include <QVector>
#include <QicsNamespace.h>

class QPen;
class QPalette;

/*! \internal
* \class QicsStyle
* \brief Represents properties of a row, column, region or cell.
*
* QicsStyle contains settings for all properties that pertain to a single
* row, column, region or cell.  These properties are used by QicsTable and
* its subwidgets to specify different aspects of the table's display and
* behavior.
*
* The interface of QicsStyle is very simple.  You can set a property
* (QicsStyle::setValue), get a property (QicsStyle::getValue), or clear
* a property (QicsStyle::clear).  A property is defined by its name,
* an enumerated value that allows for fast retrieval.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsStyle: public Qics
{
public:
    /*! \internal
    * The types of the different properties that can be set.
    */
    enum QicsStylePropertyType {
        QicsT_Int = 0,
        QicsT_Float,
        QicsT_QString,
        QicsT_QColor,
        QicsT_Boolean,
        QicsT_QWidget,
        QicsT_QPalette,
        QicsT_QFont,
        QicsT_QPixmap,
        QicsT_QCursor,
        QicsT_QicsCellDisplay,
        QicsT_QicsDataItemFormatter,
        QicsT_QValidator,
        QicsT_PasteValidator,
        QicsT_QPen,
        QicsT_QicsRegion,
        QicsT_QicsMouseMap,
        QicsT_Pointer,
        QicsT_QPoint
    };
    /*! \internal
    * Creates a new QicsStyle object.
    */
    QicsStyle();

    /*!
    * Destructor
    */
    virtual ~QicsStyle();

    /*! \internal
    * Returns the value of the property specified by \a prop.
    */
    inline void *getValue(int prop) const { return myProperties.at(prop); }
    /*! \internal
    * Sets the new value of the property specified by \a prop.
    */
    void setValue(int prop, const void *val);

    /*! \internal
    * Unsets the value of the property \a prop.  Clearing a property
    * also frees the memory associated with the value.
    */
    void clear(int prop);

    /*! \internal
    * Unsets all the values of the properties.  Clearing a property
    * also frees the memory associated with the value.
    * \since 2.2
    */
    void clear();

    /*!  \internal
    *Returns \b true if the style is empty (i.e. all the fields contain
    * null values), \b false otherwise.
    */
    inline bool isEmpty() const {return (mySetCount == 0);}
    /*!
    * Convert \a pen to QString.
    * \sa  stringToPen.
    */
    static QString penToString(const QPen *pen);
    /*!
    * Convert \a penSetings to new QPen*.
    * \sa   penToString.
    * string is comaseparated text of pen settings: color, width, style, join style.
    */
    static QPen * stringToPen(const QString &penSetings);
    /*!
    * Convert \a pal to QString.
    * \sa stringToPalette.
    */
    static QString paletteToString(const QPalette *pal);
    /*!
    * Convert \a penSetings to new QPalette*.
    * \sa paletteToString.
    * string is comaseparated text of palette settings: Foreground, Text, Background, Base, HighlightedText, Highlight
    */
    static QPalette * stringToPalette(const QString &paletteSetings);

protected:
    void init();

    QVector<void *> myProperties;
    QVector<QicsStylePropertyType> *myStyleTypeList;
    int myNumProperties;

    /// counter keeping track of how many properties in the style have been set
    int mySetCount;

private:
    /*! \internal
    * Should never be used.
    */
    QicsStyle& operator=(const QicsStyle&) { return *this;} // Copy operator
    /*! \internal
    * Should never be used.
    */
    QicsStyle(const QicsStyle&) {} // Copy Constructor
};

typedef QVector<QicsStyle *> QicsStylePV;
typedef QVector<QicsStylePV *> QicsStylePVPV;

#endif //QICSSTYLE_H


