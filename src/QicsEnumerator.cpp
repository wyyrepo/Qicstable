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

#include "QicsEnumerator.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QAbstractItemModel>
#include "QicsDataModel.h"


void QicsEnumerator::clear()
{
    m_valueToKey.clear();
    m_keyToValue.clear();
    emit cleared();
}

void QicsEnumerator::loadFromString( const QString &s, const QChar & sep )
{
    m_type = MAP_String;
    clear();
    disconnect( this, SLOT(reloadFromModel()) );
    QTextStream stream( const_cast<QString *>(&s));
    QString line;

    do {
        line = stream.readLine();
        QStringList list = line.split( sep );
        if ( list.size() == 2 ) {
            m_keyToValue.insert( list.at(0), list.at(1) );
            m_valueToKey.insert( list.at(1), list.at(0) );
            emit textInserted(  list.at(1) );
        }
    } while (!line.isNull());
}

bool QicsEnumerator::loadFromFile( const QString &fileName, const QChar & sep  )
{
    disconnect( this, SLOT(reloadFromModel()) );
    QFile f( fileName );

    if ( !f.open(QIODevice::ReadOnly) )
        return false;

    QString s = f.readAll();
    loadFromString( s, sep );
    f.close();
    m_type = MAP_File;

    return true;
}

void QicsEnumerator::setModel(QicsDataModel *m, int idCol, int displayCol )
{
    if ( !m || ( m->numColumns() - 1 < idCol ) || ( m->numColumns() - 1 < displayCol ) )
        return;

    disconnect( this, SLOT(reloadFromModel()) );
    m_type = MAP_QicsDataModel;
    dataModel = m;
    m_idCol = idCol;
    m_displayCol = displayCol;
    connect( dataModel, SIGNAL(modelChanged(QicsRegion) ), this, SLOT(reloadFromModel()));
    reloadFromModel();
}

void QicsEnumerator::setModel(QAbstractItemModel *m, int idCol, int displayCol )
{
    if ( !m || ( m->columnCount() - 1 < idCol ) || ( m->columnCount() - 1 < displayCol ) )
        return;

    disconnect( this, SLOT(reloadFromModel()) );
    m_type = MAP_QAbstractItemModel;
    itemModel = m;
    m_idCol = idCol;
    m_displayCol = displayCol;
    connect( itemModel, SIGNAL( dataChanged ( const QModelIndex &, const QModelIndex & ) ), this, SLOT(reloadFromModel()));
    reloadFromModel();

}

void QicsEnumerator::reloadFromModel()
{
    clear();

    if ( m_type == MAP_QicsDataModel ) {
        for ( int row = 0; row < dataModel->numRows(); ++row ) {
            const QicsDataItem * displayItem = dataModel->item( row, m_displayCol);
            const QicsDataItem * idItem = dataModel->item( row, m_idCol);
            emit textInserted( displayItem->string() );

            m_keyToValue.insert( idItem->string(), displayItem->string() );
            m_valueToKey.insert( displayItem->string(), idItem->string() );
        }
    }
    else {
        for ( int row = 0; row < itemModel->rowCount(); ++row ) {
            QString displayString = itemModel->data( itemModel->index(row, m_displayCol)).toString();
            QString idString = itemModel->data( itemModel->index(row, m_idCol)).toString();
            emit textInserted( displayString );

            m_keyToValue.insert( idString, displayString );
            m_valueToKey.insert( displayString, idString );
        }
    }
}

QObject *QicsEnumerator::mapModel() const
{
    if ( m_type == MAP_QicsDataModel )
        return dataModel;

    if ( m_type == MAP_QicsDataModel )
        return itemModel;

    return 0;
}


