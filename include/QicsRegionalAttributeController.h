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

#ifndef QICSREGIONALATTRIBUTECONTROLLER_H
#define QICSREGIONALATTRIBUTECONTROLLER_H

#include <QVariant>
#include "QicsNamespace.h"
#include "QicsCommonAttributeController.h"
#include "QicsCommonAttrs.h"
#include "QicsRegion.h"
#include "QicsCellStyle.h"

/*!
* \class QicsRegionalAttributeController QicsRegionalAttributeController.h
* \brief QicsRegionalAttributeController is used to manage attributes of a grid object
like a table.

It is optimized to provide very fast lookup/change of attributes and low memory consumption.
Attribute model is represented as a hash which keys are attribute names, and values are
hashes with attribute values as keys. A value of such sub-hash is a list of rows in which
this attribute is set. Every row itself contains a list of regions sorted from left to right.
Each region represents contiguous line of cells and internally is a pair or two integers:
starting and ending column indexes. Thus, a single region consumes only 8 bytes of memory,
and it does not matter how many cells it contains - one or one million.

During inserting/removing columns/attributes,
controller keeps track of changed cells and combines/splits/removes regions on-the-fly,
so there are no extra fragmentations - regions are always up-to-dated.

Adding/removing rows to data model are very fast, because such operations do not require
regional changes. Only column operations are a quite "painful".

Also controller provides handling of row/column and default cell attributes.
*
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsRegionalAttributeController : public QicsCommonAttributeController
{
public:
    QicsRegionalAttributeController();

    virtual ~QicsRegionalAttributeController();

    virtual QicsAbstractAttributeController* createController()
    { return new QicsRegionalAttributeController(); }

    /*!
    Returns pointer to a property \a name at cell with row \a row and column \a col, 0 if not found.
    */
    virtual void* cellProperty(int row, int col, int name);

    /*!
    Sets a property \a name at cell with row \a row and column \a col to a value of \a val.
    Returns \a true if ok, \a false if such property cannot be set (i.e. not supported).
    */
    virtual bool setCellProperty(int row, int col, int name, const void *val);

    /*!
    Clears property \a name at cell with row \a row and column \a col.
    Returns \a true if ok, \a false if such property cannot be cleared (i.e. not supported).
    */
    virtual bool clearCellProperty(int row, int col, int name);

    virtual bool setDefaultProperty(int name, const void *val);

    virtual void* defaultProperty(int name);

protected:
    virtual void handleReinit(int rows, int columns);
    virtual void handleInsertRows(int num, int start_position);
    virtual void handleInsertColumns(int num, int start_position);
    virtual void handleDeleteRows(int num, int start_position);
    virtual void handleDeleteColumns(int num, int start_position);

    /*! \internal
    Extends QVariant in order to be used with QMap
    */
    class AttrVariant : public QVariant
    {
    public:
        AttrVariant() : QVariant() {}
        AttrVariant(quintptr v) : QVariant(v) {}
        AttrVariant(QPen& v) : QVariant(v) {}
        AttrVariant(QString& v) : QVariant(v) {}
        AttrVariant(QPixmap& v) : QVariant(v) {}
        AttrVariant(QFont& v) : QVariant(v) {}

        bool operator < ( const QVariant & v ) const
        {
            switch (v.type())
            {
            case QVariant::String:
                return this->toString() < v.toString();

            case QVariant::Pixmap:
#if defined(_MSC_VER) && _MSC_VER < 1300
                return qvariant_cast<QPixmap>(*this).cacheKey() < qvariant_cast<QPixmap>(v).cacheKey();
#else
                return this->value<QPixmap>().cacheKey() < v.value<QPixmap>().cacheKey();
#endif

            case QVariant::Font:
#if defined(_MSC_VER) && _MSC_VER < 1300
                return qvariant_cast<QFont>(*this) < qvariant_cast<QFont>(v);
#else
                return this->value<QFont>() < v.value<QFont>();
#endif
            default:
                break;
            }

#if defined(_MSC_VER) && _MSC_VER < 1300
            return qvariant_cast<quintptr>(*this) < qvariant_cast<quintptr>(v);
#else
            return this->value<quintptr>() < v.value<quintptr>();
#endif
        }

        bool operator == ( const QVariant & v ) const
        {
            switch (v.type())
            {
            case QVariant::String:
                return this->toString() == v.toString();

            case QVariant::Pixmap:
#if defined(_MSC_VER) && _MSC_VER < 1300
                return qvariant_cast<QPixmap>(*this).cacheKey() == qvariant_cast<QPixmap>(v).cacheKey();
#else
                return this->value<QPixmap>().cacheKey() == v.value<QPixmap>().cacheKey();
#endif
            case QVariant::Pen:
#if defined(_MSC_VER) && _MSC_VER < 1300
                return qvariant_cast<QPen>(*this) == qvariant_cast<QPen>(v);
#else
                return this->value<QPen>() == v.value<QPen>();
#endif
            case QVariant::Font:
#if defined(_MSC_VER) && _MSC_VER < 1300
                return qvariant_cast<QFont>(*this) == qvariant_cast<QFont>(v);
#else
                return this->value<QFont>() == v.value<QFont>();
#endif
            default:
                break;
            }
            return QVariant::operator == (v);
        }
    };

    void *cellAttr(int row, int col, int name);
    void setCellAttr(int row, int col, int name, AttrVariant val);
    void clearCellAttr(int row, int col, int name);

    //////////////////////////////////////////////////// Internal data

    struct AttrRegion
    {
        int start;
        int end;
    };

    typedef QList<AttrRegion> AttrRegionList;
    typedef QList<AttrRegionList> AttrRowList;
    typedef QMap<AttrVariant, AttrRowList> AttrValueHash;
    typedef QMap<int, AttrValueHash> AttrHash;

    /*! \internal
    * Hash of regional attributes
    * These attributes are follow the model.
    * [Type][ [Value][ [Row][Regions] ]   ]
    */
    AttrHash m_attrs;

    // flyweights
    bool boolean;
    uint uinteger;
    quintptr pointer;
    QColor clr;
    QFont font;
    QString string;
    QCursor cursor;
    QPen pen;
    QPixmap pixmap;
};

#endif //QICSREGIONALATTRIBUTECONTROLLER_H


