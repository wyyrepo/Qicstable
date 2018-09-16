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

#ifndef QICSENUMERATOR_H
#define QICSENUMERATOR_H

#include <QObject>
#include <QMap>

class QAbstractItemModel;
class QicsDataModel;

class QicsEnumerator : public QObject
{
    Q_OBJECT
public:
    enum QicsMapType {
        MAP_String,
        MAP_File,
        MAP_QAbstractItemModel,
        MAP_QicsDataModel
    };

    QicsEnumerator(QObject *parent = 0)
        : QObject(parent)
    {
    }

    void clear();
    void loadFromString( const QString & s, const QChar & sep );
    bool loadFromFile( const QString & fileName, const QChar & sep  );

    void setModel(QAbstractItemModel *, int idCol = 0, int displayCol = 1);
    void setModel(QicsDataModel *, int idCol = 0, int displayCol = 1);

    QObject *mapModel() const;
    inline QicsMapType mapType() const { return m_type; }

    inline QString value( const QString & key  ) const { return m_keyToValue.value( key );}
    inline QString key( const QString & text ) const { return m_valueToKey.value( text );}
    inline QList<QString> values () const { return m_keyToValue.values(); }

signals:
    void textInserted( const QString & );
    void cleared();

protected slots:
    void reloadFromModel();

private:
    QAbstractItemModel * itemModel;
    QicsDataModel * dataModel;
    QMap<QString, QString> m_keyToValue;
    QMap<QString, QString> m_valueToKey;
    int m_idCol;
    int m_displayCol;
    QicsMapType m_type;
};

#endif //QICSENUMERATOR_H


