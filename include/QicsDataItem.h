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

#ifndef QICSDATAITEM_H
#define QICSDATAITEM_H

#include <QDateTime>
#include <QDataStream>
#include <QVariant>
#include <QColor>

#include "QicsNamespace.h"

/*!
* default true string for QicsDataBool item
*/
#define QICS_DEFAULT_TRUE_STRING "True"

/*!
* default false string for QicsDataBool item
*/
#define QICS_DEFAULT_FALSE_STRING "False"

/*!
* default partial string for QicsDataBool item
*/
#define QICS_DEFAULT_PARTIAL_STRING "Partial"

/*!
* limited introspection of data items
*/
typedef unsigned int QicsDataItemType;
const QicsDataItemType QicsDataItem_UserDefined = 0;
const QicsDataItemType QicsDataItem_Int		= 1;
const QicsDataItemType QicsDataItem_Long	= 2;
const QicsDataItemType QicsDataItem_LongLong	= 3;
const QicsDataItemType QicsDataItem_Float	= 4;
const QicsDataItemType QicsDataItem_Double	= 5;
const QicsDataItemType QicsDataItem_String	= 6;
const QicsDataItemType QicsDataItem_Date	= 7;
const QicsDataItemType QicsDataItem_Time	= 8;
const QicsDataItemType QicsDataItem_DateTime	= 9;
const QicsDataItemType QicsDataItem_Bool	= 10;
const QicsDataItemType QicsDataItem_Variant	= 11;

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

// This is used in QicsDataItem::registerType() and must be declared
// first.

class QicsDataItem;
/*! \typedef QicsDataItemParser
* This typedef defines a static method that parses a QString and
* returns a new QicsDataItem.  If the method cannot parse the
* string, it should return 0.
*/
typedef QicsDataItem * (*QicsDataItemParser)(const QString &);
/*! \typedef QicsDataItemDecoder
* This typedef defines a static method that decodes a QDataStream and
* returns a new QicsDataItem.
*/
typedef QicsDataItem * (*QicsDataItemDecoder)(QDataStream &);

/*! \internal
* \typedef QicsDataItemDecoderWithType
* This internal typedef defines a method that decodes a
* QDataStream and returns a new QicsDataItem.  The name of the
* type that should be decoded is passed as an argument.
* This typedef is not exposed as part of the public API.  It has
* been added because it's necessary for the Python bindings.
*/
typedef QicsDataItem * (*QicsDataItemDecoderWithType)(QDataStream &,
                                                      const QString &);

////////////////////////////////////////////////////////////////////////

/*!
* \class QicsDataItem QicsDataItem.h
*  \brief A lightweight abstract wrapper class for
*  a generic item.
*
* QicsDataItem is an abstract class used to define a common inteface that
* all data items must implement.  The programmer cannot create a QicsDataItem
* directly.  Rather, s/he should create one of its subclasses.
*
* A QicsDataItem is a piece of data that is stored by QicsDataModel objects.
* By using the abstract QicsDataItem class, the QicsDataModel does not have
* to know anything about the data it is storing.  Similarly, the QicsTable
* can display data items without having any knowledge of the items' types
* (as long as the data type has implemented #string() correctly).
*
* There are subclasses defined that implement the QicsDataItem API for many
* built-in C++ and Qt data types: QicsDataString, QicsDataBool, QicsDataInt,
* QicsDataLong, QicsDataDouble, etc.  Sometimes it may be desirable to create
* new subclasses for additional data types.  Information regarding adding a
* new data type can be found in \ref extend_model.
*/

class QICS_EXPORT QicsDataItem: public Qics
{
public:
    /*!
    * A "virtual constructor".  Returns an empty, newly allocated object of
    * the same type as this object.
    */
    virtual QicsDataItem *create() const = 0;

    /*!
    * A "virtual copy constructor".  Returns a newly allocated copy
    * of this object.
    */
    virtual QicsDataItem *clone() const = 0;

    QicsDataItem();

    virtual ~QicsDataItem() {}
    /*!
    * Converts (if necessary) \a qs to the appropriate data type
    * and sets the contents of the data item to that value.
    */
    virtual bool setString(const QString &qs) = 0;

    /*!
    * Converts (if necessary) the contents of the data item to
    * a QString and returns that string.
    */
    virtual const QString string() const = 0;

    /*!
    * Converts (if necessary) the contents of the data item to
    * a double and returns that value. \a ok flag shows
    * success of the conversion in the case of true.
    * \since 2.4
    */
    virtual double number(bool *ok = 0) const = 0;

    /*!
    * Returns the type of the data item.  All user-defined types should
    * \b return QicsDataItem_UserDefined.
    */
    virtual QicsDataItemType type() const = 0;

    /*!
    * Returns a string describing the type of the data item.
    */
    virtual QString typeString() const = 0;

    /*!
    * Converts (if necessary) the contents of the data item to
    * a formatted QString using the supplied formatting string,
    * and returns the formatted string.
    * The exact definition of the formatting string varies
    * among data types.
    */
    inline virtual QString format(const char *) const { return string(); }

    /*!
    * Compare another data item \a x to me for the purpose of
    * sorting.  compareTo returns a value < 0 if this items
    * is less than \a x, 0 if this item == \a x, and > 0 if
    * this item is greater than \a x.
    */
    virtual int compareTo(const QicsDataItem &x) const;
    /*!
    * Encodes the value of the data item onto stream \a ds.
    */
    virtual void encode(QDataStream &ds) const = 0;
    /*!
    * Returns a new data item based on the content of the string.
    */
    static QicsDataItem *fromString(const QString &str);
    /*!
    * Returns a new data item based on the encoded data in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds);
    /*!
    * Used to register a new, user-defined type so that the type
    * can be used in streaming, drag and drop, and cut and paste
    * operations.  User-defined data item parsers will be called in the
    * order that their types are registered.
    * \param type_name name of the new type
    * \param parser static member function of the new type that parses
    *               an item from a QString
    * \param decoder static member function of the new type that decodes
    *               an item from a QDataStream
    *
    * \sa registerParser
    */
    static void registerType(const QString type_name,
        QicsDataItemParser parser,
        QicsDataItemDecoder decoder);

    /*!
    * \internal
    * Used to register a new, user-defined type so that the type
    * can be used in streaming, drag and drop, and cut and paste
    * operations.  User-defined data item parsers will be called in the
    * order that their types are registered.
    * This method is not part of the public API.  It has
    * been added because it's necessary for the Python bindings.
    * \param type_name name of the new type
    * \param parser static member function of the new type that parses
    *               an item from a QString
    * \param decoder decodes a value of the new type from a data stream
    *
    * \sa registerParser
    */
    static void registerTypeI(const QString type_name,
        QicsDataItemParser parser,
        QicsDataItemDecoderWithType decoder);

    /*!
    * Used to register a new data item parser algorithm.
    * This parser can be used to reimplement the default parsing
    * "algorithm" of QicsDataItem.
    * If a user defined parser method cannot successfully parse a string,
    * it should return 0.  If the user defined parser cannot parse a string,
    * the default QicsDataItem parsing algorithm will be used.
    * The default algorithm first tries any individual user defined type
    * parsers (see #registerType()), then attempts to parse the built in
    * types.
    * \param parser the new data item parser method
    *
    * \sa registerType
    */
    static void registerParser(QicsDataItemParser parser);
};

/*!
* comparator for two QicsDataItem items
*/
// #### TODO: Must be removed in 2.5
typedef int (*DataItemComparator)(const QicsDataItem *, const QicsDataItem *);

////////////////////////////////////////////////////////////////////////

/*! \class QicsDataBool QicsDataItem.h
* \brief An boolean data item
*
*  Represents an boolean value.
*
*/
class QICS_EXPORT QicsDataBool: public QicsDataItem
{
public:
    /*!
    *  Constructs a new data item.  The initial value of this item is false.
    */
    QicsDataBool()
    {
        myData = false;
        setTrueString ( QString(QICS_DEFAULT_TRUE_STRING)  );
        setFalseString( QString(QICS_DEFAULT_FALSE_STRING) );
    }

    /*!
    *  Constructs a new data item with value \a v.
    */
    QicsDataBool(bool v, QString ts = QICS_DEFAULT_TRUE_STRING, QString fs = QICS_DEFAULT_FALSE_STRING )
    {
        myData = v;
        setTrueString ( QString(ts) );
        setFalseString( QString(fs) );
    }

    /*!
    * Copy Constructor
    */
    QicsDataBool(const QicsDataBool &db);

    inline QicsDataItem *create() const { return new QicsDataBool(); }
    inline QicsDataItem *clone() const { return new QicsDataBool(*this); }

    /*!
    *  Returns the data type of the item, QicsDataItem_Bool
    */
    inline virtual QicsDataItemType type() const { return (QicsDataItem_Bool); }

    /*!
    * Returns "bool".
    */
    inline static QString typeName() { return QString("bool"); }

    /*!
    * Returns QicsDataBool::typeName()
    */
    inline virtual QString typeString() const { return QicsDataBool::typeName(); }

    /*!
    * Assigns the value \a b to this data item.  Returns a reference
    * to the data item.
    */
    inline virtual QicsDataBool& operator=(const bool& b)
    { myData = b; return *this;}

    inline virtual const QString string() const
    {
        if ( myData ) return QString( trueString() );
        else return QString(falseString());
    }

    inline virtual bool setString(const QString &qs)
    {
        bool ok = false;
        if ( qs == trueString()  ) { myData = true;  ok = true; }
        else
            if ( qs == falseString() ) { myData = false; ok = true; }
        return ok;
    }

    inline virtual double number(bool *ok = 0) const
    {
        if (ok) *ok = true;
        return myData ? 1 : 0;
    }

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline bool data() const { return myData; }

    /*!
    *   Sets the value of this data item to \a i.
    */
    inline void setData(bool b) { myData = b;}

    inline virtual QString format(const char *fmt_string) const
    { QString str; return str.sprintf(fmt_string, (const char *) string().toLocal8Bit() ); }

    int compareTo(const QicsDataItem &x) const;
    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }
    /*!
    * Returns a new integer data item if \a str can be interpreted
    * as an integer, or 0 if it cannot be.
    */
    static QicsDataItem *fromString(const QString &str);
    /*!
    * Returns a new integer data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { bool val; ds >> val; return new QicsDataBool(val); }
    /*!
    * Returns a string equal true.
    */
    inline QString trueString() const { return myTrueString; }

    /*!
    * Returns a string equal false.
    */
    inline QString falseString() const { return myFalseString; }

    /*!
    * Sets a string equal true to \a qs.
    */
    bool setTrueString(const QString &qs)
    {
        if (!qs.isEmpty())
        { myTrueString = qs; return true;}
        return false;
    }

    /*!
    * Sets a string equal false to \a qs.
    */
    bool setFalseString(const QString &qs)
    {
        if (!qs.isEmpty())
        { myFalseString = qs; return true;}
        return false;
    }

protected:
    /*!
    * \internal
    * the actual data
    */
    bool myData;

    /*!
    * \internal
    * the string represent true value, default value "True"
    */
    QString myTrueString;

    /*!
    * \internal
    * the string represent false value, default value "False"
    */
    QString myFalseString;
};

////////////////////////////////////////////////////////////////////////

/*!
* \class QicsDataInt QicsDataItem.h
* \brief An integer data item
*
*  Represents an integer value.
*
*/
class QICS_EXPORT QicsDataInt: public QicsDataItem
{
public:
    /*!
    *  Constructs a new data item.  The initial value of this item is 0.
    */
    QicsDataInt() { myData = 0; }

    /*!
    *  Constructs a new data item with value \a i.
    */
    QicsDataInt(int i) { myData = i;}

    /*!
    * Copy Constructor
    */
    QicsDataInt(const QicsDataInt &di);

    inline QicsDataItem *create() const { return new QicsDataInt(); }
    inline QicsDataItem *clone() const { return new QicsDataInt(*this); }

    /*!
    *  Returns the data type of the item, QicsDataItem_Int
    */
    inline virtual QicsDataItemType type() const { return (QicsDataItem_Int); }

    /*!
    * Returns "int".
    */
    inline static QString typeName() { return QString("int"); }

    /*!
    * Returns QicsDataInt::typeName()
    */
    inline virtual QString typeString() const { return QicsDataInt::typeName(); }

    /*!
    * Assigns the value \a i to this data item.  Returns a reference
    * to the data item.
    */
    inline virtual QicsDataInt& operator=(const int& i)
    { myData = i; return *this;}

    inline virtual const QString string() const { return QString("%1").arg(myData); }
    inline virtual bool setString(const QString &qs)
    {bool ok; myData = qs.toInt(&ok); return ok;}

    inline virtual double number(bool *ok = 0) const
    {
        if (ok) *ok = true;
        return myData;
    }

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline int data() const { return myData; }

    /*!
    *   Sets the value of this data item to \a i.
    */
    inline void setData(int i) { myData = i;}

    inline virtual QString format(const char *fmt_string) const
    { QString str; return str.sprintf(fmt_string, myData); }

    int compareTo(const QicsDataItem &x) const;
    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }
    /*!
    * Returns a new integer data item if \a str can be interpreted
    * as an integer, or 0 if it cannot be.
    */
    static QicsDataItem *fromString(const QString &str);
    /*!
    * Returns a new integer data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { int val; ds >> val; return new QicsDataInt(val); }

protected:
    /*!
    *  \internal
    * the actual data
    */
    int myData;
};

////////////////////////////////////////////////////////////////////////

/*! \class QicsDataLong QicsDataItem.h
* \brief A long integer data item
*
*  Represents a long integer value.
*
*/
class QICS_EXPORT QicsDataLong: public QicsDataItem
{
public:
    /*!
    *  Constructs a new data item.  The initial value of this item is 0.
    */
    QicsDataLong() { myData = 0; }

    /*!
    *  Constructs a new data item with value \a i.
    */
    QicsDataLong(long i) { myData = i;}

    /*!
    * Copy Constructor
    */
    QicsDataLong(const QicsDataLong &dl);

    inline QicsDataItem *create() const { return new QicsDataLong(); }
    inline QicsDataItem *clone() const { return new QicsDataLong(*this); }

    /*!
    *  Returns the data type of the item, QicsDataItem_Long
    */
    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_Long); }

    /*!
    * Returns "long".
    */
    inline static QString typeName() { return QString("long"); }

    /*!
    * Returns QicsDataLong::typeName()
    */
    inline virtual QString typeString() const
    { return QicsDataLong::typeName(); }

    /*!
    * Assigns the value \a i to this data item.  Returns a reference
    * to the data item.
    */
    inline virtual QicsDataLong& operator=(const long& i)
    { myData = i; return *this;}

    inline virtual const QString string() const { return QString("%1").arg(myData); }
    inline virtual bool setString(const QString &qs)
    {bool ok; myData = qs.toLong(&ok); return ok;}

    inline virtual double number(bool *ok = 0) const
    {
        if (ok) *ok = true;
        return myData;
    }

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline long data() const { return myData; }

    /*!
    *   Sets the value of this data item to \a i.
    */
    inline void setData(long i) { myData = i;}

    inline virtual QString format(const char *fmt_string) const
    { QString str; return str.sprintf(fmt_string, myData); }

    int compareTo(const QicsDataItem &x) const;
    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << qlonglong(myData); }
    /*!
    * Returns a new long integer data item if \a str can be interpreted
    * as a long integer, or 0 if it cannot be.
    */
    static QicsDataItem *fromString(const QString &str);
    /*!
    * Returns a new long data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { qlonglong val; ds >> val; return new QicsDataLong((long)val); }

protected:
    /*!
    * \internal
    * the actual data
    */
    long myData;
};

////////////////////////////////////////////////////////////////////////

/*! \class QicsDataLongLong QicsDataItem.h
* \brief A long long integer data item
*
*  Represents a long long integer value.
*
*/

class QICS_EXPORT QicsDataLongLong: public QicsDataItem
{
public:
    /*!
    *  Constructs a new data item.  The initial value of this item is 0.
    */
    QicsDataLongLong() { myData = 0; }

    /*!
    *  Constructs a new data item with value \a i.
    */
    QicsDataLongLong(qlonglong i) { myData = i;}

    /*!
    * Copy Constructor
    */
    QicsDataLongLong(const QicsDataLongLong &dl);

    inline QicsDataItem *create() const { return new QicsDataLongLong(); }
    inline QicsDataItem *clone() const { return new QicsDataLongLong(*this); }

    /*!
    *  Returns the data type of the item, QicsDataItem_LongLong
    */
    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_LongLong); }

    /*!
    * Returns "int64".
    */
    inline static QString typeName()
    { return QString("int64"); }

    /*!
    * Returns QicsDataLongLong::typeName()
    */
    inline virtual QString typeString() const
    { return QicsDataLongLong::typeName(); }

    /*!
    * Assigns the value \a i to this data item.  Returns a reference
    * to the data item.
    */
    inline virtual QicsDataLongLong& operator=(const qlonglong& i)
    { myData = i; return *this;}

    inline virtual const QString string() const { return QString("%1").arg(myData); }
    inline virtual bool setString(const QString &qs) ;

    inline virtual double number(bool *ok = 0) const
    {
        if (ok) *ok = true;
        return myData;
    }

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline qlonglong data() const { return myData; }

    /*!
    *   Sets the value of this data item to \a i.
    */
    inline void setData(qlonglong i) { myData = i;}

    inline virtual QString format(const char *fmt_string) const
    { QString str; return str.sprintf(fmt_string, myData); }

    int compareTo(const QicsDataItem &x) const;

    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }

    /*!
    * Returns a new Q_LLONG integer data item if \a str can be interpreted
    * as a Q_LLONG integer, or 0 if it cannot be.
    */
    static QicsDataItem *fromString(const QString &str);

    /*!
    * Returns a new Q_LLONG data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { qlonglong val; ds >> val; return new QicsDataLongLong(val); }

protected:
    /*!
    * \internal
    * the actual data
    */
    qlonglong myData;
};
////////////////////////////////////////////////////////////////////////

/*! \class QicsDataFloat QicsDataItem.h
* \brief A float data item
*
*  Represents a floating point value.
*
*/
class QICS_EXPORT QicsDataFloat: public QicsDataItem
{
public:
    /*!
    *  Constructs a new data item.  The initial value of this item is 0.0.
    */
    QicsDataFloat() { myData = 0.0; }

    /*!
    *  Contructs a new data item with value \a f.
    */
    QicsDataFloat(float f) { myData = f;}

    /*!
    * Copy Constructor
    */
    QicsDataFloat(const QicsDataFloat &df);

    inline QicsDataItem *create() const { return new QicsDataFloat(); }
    inline QicsDataItem *clone() const { return new QicsDataFloat(*this); }

    /*!
    *  Returns the data type of the item, QicsDataItem_Float
    */
    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_Float); }

    /*!
    * Returns "float".
    */
    inline static QString typeName()
    { return QString("float"); }

    /*!
    * Returns QicsDataFloat::typeName()
    */
    inline virtual QString typeString() const
    { return QicsDataFloat::typeName(); }

    /*!
    * Assigns the value \a f to this data item.  Returns a reference
    * to the data item.
    */
    inline virtual QicsDataFloat& operator=(const float& f)
    { myData = f; return *this;}

    inline virtual const QString string() const { return QString("%1").arg(myData); }
    inline virtual bool setString(const QString &qs)
    {bool ok; myData = qs.toFloat(&ok); return ok;}

    inline virtual double number(bool *ok = 0) const
    {
        if (ok) *ok = true;
        return myData;
    }

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline float data() const { return myData; }

    /*!
    *   Sets the value of this data item to \a f
    */
    inline void setData(float f) { myData = f;}

    inline virtual QString format(const char *fmt_string) const
    { QString str; return str.sprintf(fmt_string, myData); }

    int compareTo(const QicsDataItem &x) const;
    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }
    /*!
    * Returns a new float data item if \a str can be interpreted
    * as a float, or 0 if it cannot be.
    */
    static QicsDataItem *fromString(const QString &str);
    /*!
    * Returns a new float data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { float val; ds >> val; return new QicsDataFloat(val); }
protected:
    /*!
    * \internal
    * the actual data
    */
    float myData;
};

////////////////////////////////////////////////////////////////////////

/*! \class QicsDataDouble QicsDataItem.h
* \brief A double data item
*
*  Represents a double-precision floating point value.
*
*/
class QICS_EXPORT QicsDataDouble: public QicsDataItem
{
public:
    /*!
    *  Constructs a new data item.  The initial value of this item is 0.0.
    */
    QicsDataDouble() { myData = 0.0; }

    /*!
    *  Contructs a new data item with value \a f.
    */
    QicsDataDouble(double d) { myData = d;}

    /*!
    * Copy Constructor
    */
    QicsDataDouble(const QicsDataDouble &dd);

    inline QicsDataItem *create() const { return new QicsDataDouble(); }
    inline QicsDataItem *clone() const { return new QicsDataDouble(*this); }

    /*!
    * Returns "double".
    */
    inline static QString typeName()
    { return QString("double"); }

    /*!
    * Returns QicsDataDouble::typeName()
    */
    inline virtual QString typeString() const
    { return QicsDataDouble::typeName(); }

    /*!
    *  Returns the data type of the item, QicsDataItem_Double
    */
    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_Double); }

    /*!
    * Assigns the value \a d to this data item.  Returns a reference
    * to the data item.
    */
    inline virtual QicsDataDouble& operator=(const double& d)
    { myData = d; return *this;}

    inline virtual const QString string() const { return QString("%1").arg(myData); }
    inline virtual bool setString(const QString &qs)
    {bool ok; myData = qs.toDouble(&ok); return ok;}

    inline virtual double number(bool *ok = 0) const
    {
        if (ok) *ok = true;
        return myData;
    }

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline double data() const { return myData; }

    /*!
    *   Sets the value of this data item to \a d
    */
    inline void setData(double d) { myData = d;}

    inline virtual QString format(const char *fmt_string) const
    { QString str; return str.sprintf(fmt_string, myData); }

    int compareTo(const QicsDataItem &x) const;
    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }
    /*!
    * Returns a new double data item if \a str can be interpreted
    * as a double, or 0 if it cannot be.
    */
    static QicsDataItem *fromString(const QString &str);
    /*!
    * Returns a new double data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { double val; ds >> val; return new QicsDataDouble(val); }

protected:
    /*!
    * \internal
    * the actual data
    */
    double myData;
};

////////////////////////////////////////////////////////////////////////

/*!
* \class QicsDataString QicsDataItem.h
* \brief A string data item
*
*  Represents an string value.
*
*/
class QICS_EXPORT QicsDataString: public QicsDataItem
{
public:
    /*!
    * Constructs a new data item.  The initial value of this item is
    * the empty string.
    */
    QicsDataString() { myData = QString(); }

    /*!
    * Contructs a new data item with value \a qs.
    */
    QicsDataString(const QString &qs);
    QicsDataString(const char* s);

    virtual ~QicsDataString() {}

    /*!
    * Copy Constructor
    */
    QicsDataString(const QicsDataString &ds);

    inline QicsDataItem *create() const { return new QicsDataString(); }
    inline QicsDataItem *clone() const { return new QicsDataString(*this); }

    /*!
    * Returns "qstring".
    */
    inline static QString typeName()
    { return QString("qstring"); }

    /*!
    * Returns QicsDataQString::typeName()
    */
    inline virtual QString typeString() const
    { return QicsDataString::typeName(); }

    /*!
    *  Returns the data type of the item, QicsDataItem_String
    */
    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_String); }

    /*!
    * Assigns the value \a qs to this data item.  Returns a reference
    * to the data item.
    */
    QicsDataString& operator=(const QicsDataString& qs);

    inline virtual bool setString(const QString &qs)
    { myData = qs; return true;}

    inline virtual double number(bool *ok = 0) const
    {
        if (myData.isEmpty()) {
            if (ok) *ok = false;
            return 0;
        }
        return myData.toDouble(ok);
    }

    /*!
    *   Sets the value of this data item to \a qs.
    */
    inline bool setData(const char *s)
    { myData = s; return true;}

    /*!
    *   Sets the value of this data item to \a qs.
    */
    inline bool setData(const QString &qs)
    { myData = qs; return true;}

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline QString data() const { return myData;}

    inline virtual const QString string() const { return myData; }

    inline virtual QString format(const char *fmt_string) const
    { QString str; return str.sprintf(fmt_string, (const char *) myData.toLocal8Bit()); }

    int compareTo(const QicsDataItem &x) const;
    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }

    /*!
    * Returns a new string data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { QString val; ds >> val; return new QicsDataString(val); }

protected:
    /*!
    * \internal
    * the actual data
    */
    QString myData;
};

////////////////////////////////////////////////////////////////////////

/*! \class QicsDataDate QicsDataItem.h
* \brief A date data item
*
* Represents a date value.
*/

class QICS_EXPORT QicsDataDate: public QicsDataItem
{
public:
    /*!
    *  Contructs a new empty data item, with a default format of \a format.
    */
    QicsDataDate(Qt::DateFormat format = Qt::TextDate);
    /*!
    * Contructs a new empty data item from \a date,
    * with a default format of \a format.
    */
    QicsDataDate(const QDate &date, Qt::DateFormat format = Qt::TextDate);
    /*!
    * Contructs a new empty data item from (\a y, \a m, \a d )
    * with a default format of \a format.
    */
    QicsDataDate(int y, int m, int d, Qt::DateFormat format = Qt::TextDate);

    /*!
    * Copy Constructor
    */
    QicsDataDate(const QicsDataDate &dd);

    inline QicsDataItem *create() const { return new QicsDataDate(); }
    inline QicsDataItem *clone() const { return new QicsDataDate(*this); }

    /*!
    * Returns "qdate".
    */
    inline static QString typeName()
    { return QString("qdate"); }

    /*!
    * Returns QicsDataQDate::typeName()
    */
    inline virtual QString typeString() const
    { return QicsDataDate::typeName(); }

    /*!
    *  Returns the data type of the item, QicsDataItem_Date
    */
    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_Date); }

    /*!
    * Assigns the value \a d to this data item.  Returns a reference
    * to the data item.
    */
    virtual QicsDataDate& operator=(const QDate &d);

    inline virtual const QString string() const
    { return myData.toString(); }
    virtual bool setString(const QString &qs);

    inline virtual double number(bool *ok = 0) const
    {
        if (*ok)
            *ok = false;
        return 0;
    }

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline QDate data() const { return myData; }

    /*!
    *   Sets the value of this data item to \a date
    */
    inline void setData(const QDate &date)
    { myData = date; }

    virtual QString format(const char *fmt_string) const;

    /*!
    * Returns the default date format of this data item.
    */
    inline virtual Qt::DateFormat defaultDateFormat() const
    { return myDefaultFormat;}

    /*!
    * Sets the default date format of this data item.
    */
    inline virtual void setDefaultDateFormat(Qt::DateFormat f)
    { myDefaultFormat = f; }

    int compareTo(const QicsDataItem &x) const;
    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }
    /*!
    * Returns a new date data item if \a str can be interpreted
    * as a date, or 0 if it cannot be.
    */
    static QicsDataItem *fromString(const QString &str);
    /*!
    * Returns a new date data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { QDate val; ds >> val; return new QicsDataDate(val); }

protected:
    /*!
    * \internal
    * the actual data
    */
    QDate myData;
    /*!
    * \internal
    * the default date format
    */
    Qt::DateFormat myDefaultFormat;
};

////////////////////////////////////////////////////////////////////////

/*! \class QicsDataTime QicsDataItem.h
* \brief A time data item
*
* Represents a time value.
*/

class QICS_EXPORT QicsDataTime: public QicsDataItem
{
public:
    /*!
    *  Contructs a new empty data item, with a default format of \a format.
    */
    QicsDataTime(Qt::DateFormat format = Qt::TextDate);
    /*!
    * Contructs a new empty data item from \a time,
    * with a default format of \a format.
    */
    QicsDataTime(const QTime &time, Qt::DateFormat format = Qt::TextDate);
    /*!
    * Contructs a new empty data item from (\a hour, \a minute, \a second,
    * \a msec ) with a default format of \a format.
    */
    QicsDataTime(int hour, int minute, int second, int msec,
        Qt::DateFormat format);

    /*!
    * Copy Constructor
    */
    QicsDataTime(const QicsDataTime &dt);

    inline QicsDataItem *create() const { return new QicsDataTime(); }
    inline QicsDataItem *clone() const { return new QicsDataTime(*this); }

    /*!
    * Returns "qtime".
    */
    inline static QString typeName()
    { return QString("qtime"); }

    /*!
    * Returns QicsDataQTime::typeName()
    */
    inline virtual QString typeString() const
    { return QicsDataTime::typeName(); }

    /*!
    *  Returns the data type of the item, QicsDataItem_Time
    */
    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_Time); }

    /*!
    * Assigns the value \a t to this data item.  Returns a reference
    * to the data item.
    */
    virtual QicsDataTime& operator=(const QTime &t);

    inline virtual const QString string() const { return myData.toString(); }
    virtual bool setString(const QString &qs);

    inline virtual double number(bool *ok = 0) const
    {
        if (*ok)
            *ok = false;
        return 0;
    }

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline QTime data() const { return myData; }

    /*!
    *   Sets the value of this data item to \a time
    */
    void setData(const QTime &time);

    virtual QString format(const char *fmt_string) const;

    /*!
    * Returns the default time format of this data item.
    */
    inline virtual Qt::DateFormat defaultDateFormat() const
    { return myDefaultFormat;}

    /*!
    * Sets the default time format of this data item.
    */
    inline virtual void setDefaultDateFormat(Qt::DateFormat f)
    { myDefaultFormat = f; }

    int compareTo(const QicsDataItem &x) const;
    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }
    /*!
    * Returns a new time data item if \a str can be interpreted
    * as a time, or 0 if it cannot be.
    */
    static QicsDataItem *fromString(const QString &str);
    /*!
    * Returns a new time data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { QTime val; ds >> val; return new QicsDataTime(val); }

protected:
    /*!
    * \internal
    * the actual data
    */
    QTime myData;
    /*!
    * \internal
    * the default time format
    */
    Qt::DateFormat myDefaultFormat;
};

////////////////////////////////////////////////////////////////////////

/*! \class QicsDataDateTime QicsDataItem.h
* \brief A datetime data item
*
* Represents a datetime value.
*/

class QICS_EXPORT QicsDataDateTime: public QicsDataItem
{
public:
    /*!
    *  Contructs a new empty data item, with a default format of \a format.
    */
    QicsDataDateTime(Qt::DateFormat format = Qt::TextDate);
    /*!
    * Contructs a new empty data item from \a dt,
    * with a default format of \a format.
    */
    QicsDataDateTime(const QDateTime &dt, Qt::DateFormat format = Qt::TextDate);
    /*!
    * Contructs a new empty data item from (\a date, \a time )
    * with a default format of \a format.
    */
    QicsDataDateTime(const QDate &date, const QTime &time,
        Qt::DateFormat format = Qt::TextDate);

    /*!
    * Copy Constructor
    */
    QicsDataDateTime(const QicsDataDateTime &ddt);

    inline QicsDataItem *create() const { return new QicsDataDateTime(); }
    inline QicsDataItem *clone() const { return new QicsDataDateTime(*this); }

    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_DateTime); }

    /*!
    * Returns "qdatetime".
    */
    inline static QString typeName()
    { return QString("qdatetime"); }

    /*!
    * Returns QicsDataQDateTime::typeName()
    */
    inline virtual QString typeString() const
    { return QicsDataDateTime::typeName(); }

    /*!
    * Assigns the value \a dt to this data item.  Returns a reference
    * to the data item.
    */
    virtual QicsDataDateTime& operator=(const QDateTime &dt);

    inline virtual const QString string() const { return myData.toString(); }

    /*!
    * Returns a formatted DateTime string from myData with format rule fmt_string.
    */

    inline virtual const QString string(const char *fmt_string) const
    {
        return myData.toString(fmt_string);
    }
    virtual bool setString(const QString &qs);

    inline virtual double number(bool *ok = 0) const
    {
        if (*ok)
            *ok = false;
        return 0;
    }

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline QDateTime data() const { return myData; }

    /*!
    *   Sets the value of this data item to \a dt
    */
    void setData(const QDateTime &dt);

    virtual QString format(const char *fmt_string) const;

    /*!
    * Returns the default date format of this data item.
    */
    inline virtual Qt::DateFormat defaultDateFormat() const
    { return myDefaultFormat;}

    /*!
    * Sets the default date format of this data item.
    */
    inline virtual void setDefaultDateFormat(Qt::DateFormat f)
    { myDefaultFormat = f; }

    int compareTo(const QicsDataItem &x) const;
    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }
    /*!
    * Returns a new datetime data item if \a str can be interpreted
    * as a datetime, or 0 if it cannot be.
    */
    static QicsDataItem *fromString(const QString &str);
    /*!
    * Returns a new datetime data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds)
    { QDateTime val; ds >> val; return new QicsDataDateTime(val); }

protected:
    /*!
    * \internal
    * the actual data
    */
    QDateTime myData;
    /*!
    * \internal
    * the default date format
    */
    Qt::DateFormat myDefaultFormat;
};

/*!
* \class QicsDataVariant QicsDataItem.h
* \brief A variant data item
*
*  Represents an variant value.
*
*/
class QICS_EXPORT QicsDataVariant: public QicsDataItem
{
public:
    /*!
    * Constructs a new data item.  The initial value of this item is
    * the invalid QVariant.
    */
    QicsDataVariant() { myData = QVariant(); }

    /*!
    * Copy Constructor
    */
    QicsDataVariant(const QicsDataVariant &ds);

    QicsDataVariant ( QVariant::Type type ) : myData(type){;}
    QicsDataVariant ( int typeOrUserType, const void * copy ) : myData(typeOrUserType,copy){;}
    QicsDataVariant ( const QVariant & p ) : myData(p){;}
    QicsDataVariant ( QDataStream & s ) : myData(s){;}
    QicsDataVariant ( int val ) : myData(val){;}
    QicsDataVariant ( uint val ) : myData(val){;}
    QicsDataVariant ( qlonglong val ) : myData(val){;}
    QicsDataVariant ( qulonglong val ) : myData(val){;}
    QicsDataVariant ( bool val ) : myData(val){;}
    QicsDataVariant ( double val ) : myData(val){;}
#ifdef QT_ASCII_CAST_WARN_CONSTRUCTOR
    QT_ASCII_CAST_WARN_CONSTRUCTOR QicsDataVariant ( const char * val ) : myData(val){;}
#else
    QicsDataVariant ( const char * val ) : myData(val){;}
#endif
    QicsDataVariant ( const QByteArray & val ) : myData(val){;}
    QicsDataVariant ( const QBitArray & val ) : myData(val){;}
    QicsDataVariant ( const QString & val ) : myData(val){;}
    QicsDataVariant ( const QLatin1String & val ) : myData(val){;}
    QicsDataVariant ( const QStringList & val ) : myData(val){;}
    QicsDataVariant ( const QChar & c ) : myData(c){;}
    QicsDataVariant ( const QDate & val ) : myData(val){;}
    QicsDataVariant ( const QTime & val ) : myData(val){;}
    QicsDataVariant ( const QDateTime & val ) : myData(val){;}
    QicsDataVariant ( const QList<QVariant> & val ) : myData(val){;}
    QicsDataVariant ( const QMap<QString, QVariant> & val ) : myData(val){;}
    QicsDataVariant ( const QSize & val ) : myData(val){;}
    QicsDataVariant ( const QSizeF & val ) : myData(val){;}
    QicsDataVariant ( const QPoint & val ) : myData(val){;}
    QicsDataVariant ( const QPointF & val ) : myData(val){;}
    QicsDataVariant ( const QLine & val ) : myData(val){;}
    QicsDataVariant ( const QLineF & val ) : myData(val){;}
    QicsDataVariant ( const QRect & val ) : myData(val){;}
    QicsDataVariant ( const QRectF & val ) : myData(val){;}
    QicsDataVariant ( const QUrl & val ) : myData(val){;}
    QicsDataVariant ( const QLocale & l ) : myData(l){;}
    QicsDataVariant ( const QRegExp & regExp ) : myData(regExp){;}
    QicsDataVariant ( Qt::GlobalColor color ) : myData(QColor(color)){}

    virtual ~QicsDataVariant() {}

    inline QicsDataItem *create() const { return new QicsDataVariant(); }
    inline QicsDataItem *clone() const { return new QicsDataVariant(*this); }

    /*!
    * Returns "qvariant".
    */
    inline static QString typeName()
    { return QString("qvariant"); }

    /*!
    * Returns QicsDataVariant::typeName()
    */
    inline virtual QString typeString() const
    { return QicsDataVariant::typeName(); }

    /*!
    *  Returns the data type of the item, QicsDataItem_Variant
    */
    inline virtual QicsDataItemType type() const
    { return (QicsDataItem_Variant); }

    inline virtual bool setString(const QString &qs)
    { myData = qs; return true;}

    inline virtual double number(bool *ok = 0) const
    {
        return myData.toDouble(ok);
    }

    inline virtual QString format(const char *fmt_string) const
    { QString str; return str.sprintf(fmt_string, (const char *)myData.toString().toLocal8Bit()); }

    /*!
    *   Sets the value of this data item to \a v.
    */
    inline bool setData(const QVariant &v)
    { myData = v; return true;}

    /*!
    *   Returns a copy of the value of this data item.
    */
    inline QVariant data() const { return myData;}

    inline virtual const QString string() const { return myData.toString(); }

    int compareTo(const QicsDataItem &x) const;

    inline void encode(QDataStream &ds) const
    { ds << typeString(); ds << myData; }

    /*!
    * Returns a new string data item based on the data encoded in \a ds.
    */
    static QicsDataItem *decode(QDataStream &ds);

    static QicsDataItem * fromString(const QString &str);

protected:
    /*!
    * \internal
    * the actual data
    */
    QVariant myData;
};

#endif //QICSDATAITEM_H


