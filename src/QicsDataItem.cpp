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

#include "QicsDataItem.h"


// A helper class to store data item class info

class QicsDataItemInfo
{
public:
    QicsDataItemInfo(const QString &type, QicsDataItemParser parser,
        QicsDataItemDecoder decoder,
        QicsDataItemDecoderWithType decoder_with_type)
    : myType(type), myParser(parser), myDecoder(decoder),
        myDecoderWithType(decoder_with_type)
    {
    }

    inline QString type() const { return myType; }
    inline QicsDataItemParser parser() const { return myParser; }
    inline QicsDataItemDecoder decoder() const { return myDecoder; }
    inline QicsDataItemDecoderWithType decoderWithType() const { return myDecoderWithType; }

private:
    QString myType;
    QicsDataItemParser myParser;
    QicsDataItemDecoder myDecoder;
    QicsDataItemDecoderWithType myDecoderWithType;
};

static QList<QicsDataItemInfo *> registered_types;
static QicsDataItemParser user_parser = 0;

/* default implementation */

QicsDataItem::QicsDataItem()
{
}

QicsDataItem *QicsDataItem::fromString(const QString &str)
{
    QicsDataItem *val = 0;
    int i;

    // First check the user parser (if it exists)
    if (user_parser) {
        val = (user_parser)(str);
        if (val) return val;
    }

    // Now check any of our registered types
    for (i = 0; i < registered_types.count(); ++i) {
        val = ((registered_types.at(i))->parser())(str);
        if (val) return val;
    }

    // The registered types couldn't handle it, so let's try the built-in types
    if (!(val = QicsDataInt::fromString(str)))
        if (!(val = QicsDataFloat::fromString(str)))
            if (!(val = QicsDataBool::fromString(str)))
                val = new QicsDataString(str);

    return val;
}

QicsDataItem *QicsDataItem::decode(QDataStream &ds)
{
    // get the type name of the next item
    QString type;
    ds >> type;

    // Check the built-in types
    if (type == QicsDataInt::typeName())
        return QicsDataInt::decode(ds);
    if (type == QicsDataLong::typeName())
        return QicsDataLong::decode(ds);
    if (type == QicsDataLongLong::typeName())
        return QicsDataLongLong::decode(ds);
    if (type == QicsDataBool::typeName())
        return QicsDataBool::decode(ds);
    if (type == QicsDataFloat::typeName())
        return QicsDataFloat::decode(ds);
    if (type == QicsDataDouble::typeName())
        return QicsDataDouble::decode(ds);
    if (type == QicsDataString::typeName())
        return QicsDataString::decode(ds);
    if (type == QicsDataDate::typeName())
        return QicsDataDate::decode(ds);
    if (type == QicsDataTime::typeName())
        return QicsDataTime::decode(ds);
    if (type == QicsDataDateTime::typeName())
        return QicsDataDateTime::decode(ds);
    if (type == QicsDataVariant::typeName())
        return QicsDataVariant::decode(ds);

    // Now check any of our registered types
    QicsDataItem *val = 0;

    for (int i = 0; i < registered_types.count(); ++i) {
        const QicsDataItemInfo *info = registered_types.at(i);

        if (type == info->type()) {
            if (info->decoderWithType())
                val = (info->decoderWithType())(ds, type);
            else if (info->decoder())
                val = (info->decoder())(ds);

            break;
        }
    }

    return val;
}
void QicsDataItem::registerType(const QString type_name,
                           QicsDataItemParser parser,
                           QicsDataItemDecoder decoder)
{
    QicsDataItemInfo *info = new QicsDataItemInfo(type_name, parser,
        decoder, 0);
    registered_types.append(info);
}

void QicsDataItem::registerTypeI(const QString type_name,
                            QicsDataItemParser parser,
                            QicsDataItemDecoderWithType decoder)
{
    QicsDataItemInfo *info = new QicsDataItemInfo(type_name, parser,
        0, decoder);
    registered_types.append(info);
}

void QicsDataItem::registerParser(QicsDataItemParser parser)
{
    user_parser = parser;
}

int QicsDataItem::compareTo(const QicsDataItem &) const
{
    qDebug("Missing compareTo function for data type %d\n", this->type());
    return 0;
}

////////////////////////////////////////////////////////////////////////

QicsDataBool::QicsDataBool(const QicsDataBool &db)
    : QicsDataItem()
{
    myData = db.myData;
    setTrueString( db.trueString() );
    setFalseString( db.falseString() );
}

QicsDataItem *QicsDataBool::fromString(const QString &qs)
{
    bool ok = false;
    bool val;

    if ( qs.compare(QICS_DEFAULT_TRUE_STRING) == 0 ) {
        val = true;
        ok = true;
    }
    else
        if ( qs.compare(QICS_DEFAULT_FALSE_STRING) == 0 ) {
            val = false;
            ok = true;
        }

    if (ok)
        return new QicsDataBool(val);

    return 0;
}

int QicsDataBool::compareTo(const QicsDataItem &x) const
{
    if (this->type() == x.type()) {
        const QicsDataBool *v = static_cast<const QicsDataBool *> (&x);
        if ( myData == v->myData)  return  0;
        if ( myData && !v->myData) return  1;	// true > false
        if ( !myData && v->myData) return -1;	// false < true
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////

QicsDataInt::QicsDataInt(const QicsDataInt &di)
    : QicsDataItem()
{
    myData = di.myData;
}

QicsDataItem *QicsDataInt::fromString(const QString &str)
{
    bool ok;
    int val = str.toInt(&ok);

    if (ok)
        return new QicsDataInt(val);

    return 0;
}

int QicsDataInt::compareTo(const QicsDataItem &x) const
{
    if (this->type() == x.type()) {
        const QicsDataInt *v = static_cast<const QicsDataInt *> (&x);
        return myData - v->myData;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////

QicsDataLong::QicsDataLong(const QicsDataLong &dl)
    : QicsDataItem()
{
    myData = dl.myData;
}

QicsDataItem *QicsDataLong::fromString(const QString &str)
{
    bool ok;
    long val = str.toLong(&ok);

    if (ok)
        return new QicsDataLong(val);

    return 0;
}

int QicsDataLong::compareTo(const QicsDataItem &x) const
{
    Q_ASSERT(this->type() == x.type());

    const QicsDataLong *v = static_cast<const QicsDataLong *> (&x);
    return myData - v->myData;
}

////////////////////////////////////////////////////////////////////////

QicsDataLongLong::QicsDataLongLong(const QicsDataLongLong &dll)
    : QicsDataItem()
{
    myData = dll.myData;
}

QicsDataItem *QicsDataLongLong::fromString(const QString &str)
{
    bool ok;
    qlonglong val = str.toLongLong(&ok);

    if (ok)
        return new QicsDataLongLong(val);

    return 0;
}

int QicsDataLongLong::compareTo(const QicsDataItem &x) const
{
    Q_ASSERT(this->type() == x.type());

    const QicsDataLongLong *v = static_cast<const QicsDataLongLong *> (&x);
    qlonglong res = myData - v->myData;

    if ( res > INT_MAX) return INT_MAX;
    if ( res < INT_MIN) return INT_MIN+1;

    return  res;
}

bool QicsDataLongLong::setString(const QString &qs)
{
    bool ok;
    myData = qs.toLongLong(&ok);
    return ok;
}
////////////////////////////////////////////////////////////////////////

QicsDataFloat::QicsDataFloat(const QicsDataFloat &df)
    : QicsDataItem()
{
    myData = df.myData;
}

QicsDataItem *QicsDataFloat::fromString(const QString &str)
{
    bool ok;
    float val = str.toFloat(&ok);

    if (ok)
        return new QicsDataFloat(val);

    return 0;
}

int QicsDataFloat::compareTo(const QicsDataItem &x) const
{
    if  (this->type() == x.type()) {
        const QicsDataFloat *v = static_cast<const QicsDataFloat *> (&x);
        if(myData < v->myData) return -1;
        if(myData == v->myData) return 0;
        return 1;
    }

    return 1;
}

////////////////////////////////////////////////////////////////////////

QicsDataDouble::QicsDataDouble(const QicsDataDouble &dd)
    : QicsDataItem()
{
    myData = dd.myData;
}

QicsDataItem *QicsDataDouble::fromString(const QString &str)
{
    bool ok;
    double val = str.toDouble(&ok);

    if (ok)
        return new QicsDataDouble(val);

    return 0;
}

int QicsDataDouble::compareTo(const QicsDataItem &x) const
{
    Q_ASSERT(this->type() == x.type());

    const QicsDataDouble *v = static_cast<const QicsDataDouble *> (&x);
    if(myData < v->myData) return -1;
    if(myData == v->myData) return 0;

    return 1;
}

////////////////////////////////////////////////////////////////////////

QicsDataDate::QicsDataDate(Qt::DateFormat format)
    : QicsDataItem(), myDefaultFormat(format)
{
}

QicsDataDate::QicsDataDate(const QicsDataDate &dd)
    : QicsDataItem(), myData(dd.myData), myDefaultFormat(dd.myDefaultFormat)
{
}

QicsDataDate::QicsDataDate(const QDate &date, Qt::DateFormat format)
    : QicsDataItem(), myData(date), myDefaultFormat(format)
{
}

QicsDataDate::QicsDataDate(int y, int m, int d, Qt::DateFormat format)
    : QicsDataItem(), myDefaultFormat(format)
{
    myData = QDate(y, m, d);
}

QicsDataDate &QicsDataDate::operator=(const QDate &date)
{
    setData(date);

    return *this;
}

bool QicsDataDate::setString(const QString &qs)
{
    QDate date = QDate::fromString(qs, myDefaultFormat);

    if (date.isValid()) {
        this->setData(date);
        return true;
    }

    return false;
}

QString QicsDataDate::format(const char *fmt_string) const
{
    return (myData.toString(fmt_string));
}

QicsDataItem *QicsDataDate::fromString(const QString &str)
{
    if (str.isEmpty())
        return 0;

    QDate val = QDate::fromString(str);

    if (val.isValid())
        return new QicsDataDate(val);

    return 0;
}

int QicsDataDate::compareTo(const QicsDataItem &x) const
{
    Q_ASSERT(this->type() == x.type());

    const QicsDataDate &v = static_cast<const QicsDataDate &> (x);

    if (data() == v.data()) return 0;
    if (data() < v.data()) return -1;
    return 1;
}

////////////////////////////////////////////////////////////////////////

QicsDataTime::QicsDataTime(Qt::DateFormat format)
    : QicsDataItem(), myDefaultFormat(format)
{
}

QicsDataTime::QicsDataTime(const QicsDataTime &dt)
    : QicsDataItem(), myData(dt.myData), myDefaultFormat(dt.myDefaultFormat)
{
}

QicsDataTime::QicsDataTime(const QTime &time, Qt::DateFormat format)
    : QicsDataItem(), myData(time), myDefaultFormat(format)
{
}

QicsDataTime::QicsDataTime(int h, int m, int s, int msec,
                           Qt::DateFormat format)
    : QicsDataItem(), myDefaultFormat(format)
{
    myData = QTime(h, m, s, msec);
}

QicsDataTime &QicsDataTime::operator=(const QTime &time)
{
    setData(time);

    return *this;
}

bool QicsDataTime::setString(const QString &qs)
{
    QTime time = QTime::fromString(qs, myDefaultFormat);

    if (time.isValid()) {
        this->setData(time);
        return true;
    }

    return false;
}

void QicsDataTime::setData(const QTime &time)
{
    myData = time;
}

QString QicsDataTime::format(const char *fmt_string) const
{
    return (myData.toString(fmt_string));
}

QicsDataItem *QicsDataTime::fromString(const QString &str)
{
    if (str.isEmpty())
        return 0;

    QTime val = QTime::fromString(str);

    if (val.isValid())
        return new QicsDataTime(val);

    return 0;
}

int QicsDataTime::compareTo(const QicsDataItem &x) const
{
    Q_ASSERT(this->type() == x.type());

    const QicsDataTime &v = static_cast<const QicsDataTime &> (x);

    if (data() == v.data()) return 0;
    if (data() < v.data()) return -1;
    return 1;
}


////////////////////////////////////////////////////////////////////////

QicsDataDateTime::QicsDataDateTime(Qt::DateFormat format)
    : QicsDataItem(), myDefaultFormat(format)
{
}

QicsDataDateTime::QicsDataDateTime(const QicsDataDateTime &ddt)
    : QicsDataItem(), myData(ddt.myData), myDefaultFormat(ddt.myDefaultFormat)
{
}

QicsDataDateTime::QicsDataDateTime(const QDateTime &dt, Qt::DateFormat format)
    : QicsDataItem(), myData(dt), myDefaultFormat(format)
{
}

QicsDataDateTime::QicsDataDateTime(const QDate &date, const QTime &time,
                                   Qt::DateFormat format)
    : QicsDataItem(), myDefaultFormat(format)
{
    myData = QDateTime(date, time);
}

QicsDataDateTime &QicsDataDateTime::operator=(const QDateTime &dt)
{
    setData(dt);

    return *this;
}

bool QicsDataDateTime::setString(const QString &qs)
{
    QDateTime dt = QDateTime::fromString(qs, myDefaultFormat);

    if (dt.isValid()) {
        setData(dt);
        return true;
    }

    return false;
}

void QicsDataDateTime::setData(const QDateTime &dt)
{
    myData = dt;
}

QString QicsDataDateTime::format(const char *fmt_string) const
{
    return (myData.toString(fmt_string));
}

QicsDataItem *QicsDataDateTime::fromString(const QString &str)
{
    if (str.isEmpty())
        return 0;

    QDateTime val = QDateTime::fromString(str);

    if (val.isValid())
        return new QicsDataDateTime(val);

    return 0;
}

int QicsDataDateTime::compareTo(const QicsDataItem &x) const
{
    Q_ASSERT(this->type() == x.type());

    const QicsDataDateTime &v = static_cast<const QicsDataDateTime &> (x);

    if (data() == v.data()) return 0;
    if (data() < v.data()) return -1;
    return 1;
}


////////////////////////////////////////////////////////////////////////

QicsDataVariant::QicsDataVariant(const QicsDataVariant &ds)
    : QicsDataItem(), myData(ds.data())
{
}

int QicsDataVariant::compareTo(const QicsDataItem &x) const
{
    if  (this->type() == x.type()){
        const QicsDataVariant *v = static_cast<const QicsDataVariant *> (&x);
        return myData.toString().compare(v->myData.toString());
    }
    return 1;
}

QicsDataItem *QicsDataVariant::fromString(const QString &str)
{
    return new QicsDataVariant(str);
}

QicsDataItem* QicsDataVariant::decode(QDataStream &ds)
{
    return new QicsDataVariant(ds);
}

////////////////////////////////////////////////////////////////////////

QicsDataString::QicsDataString(const QicsDataString &ds)
    : QicsDataItem()
{
    myData = ds.myData;
}

QicsDataString::QicsDataString(const QString &qs)
{
    myData = qs;
}

QicsDataString::QicsDataString(const char* s)
{
    myData = QString(s);
}

QicsDataString &QicsDataString::operator=(const QicsDataString& s)
{
    if (this != &s)
        myData = s.string();

    return *this;
}

int QicsDataString::compareTo(const QicsDataItem &x) const
{
    if  (this->type() == x.type()) {
        const QicsDataString *v = static_cast<const QicsDataString *> (&x);
        return myData.compare(v->myData);
    }
    return 1;
}


