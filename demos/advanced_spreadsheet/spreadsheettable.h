/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SPREADSHEETTABLE_H
#define SPREADSHEETTABLE_H

#include <QicsTable.h>
#include <QMap>

/*
* Class used in spreadsheet example for data representation.
* Has some additional helper methods for spreadsheet needs.
*/
class SpreadsheetTable  : public QicsTable
{
    Q_OBJECT
public:
    SpreadsheetTable( QicsTableGrid::Foundry tf,QWidget* parent = 0);
    virtual ~SpreadsheetTable();

    inline QString labelFromColumn(int col)const{return m_columns.key(col);}
    inline int columnFromLabel(const QString& str)const{return m_columns[str];}
    inline QString labelFromRow(int col)const{return m_rows.key(col);}
    inline int rowFromLabel(const QString& str)const{return m_rows[str];}
    void setLayoutSpacing(int sp);

public slots:
    void insertRows();
    void insertColumns();
    void deleteRows();
    void deleteColumns();
    void clearContents();
    void hideRows();
    void hideColumns();
    void unhideRows();
    void unhideColumns();

protected slots:
    void updateView();

protected:
    void encodePosition(QString &text, int column);

private:
    QMap<QString,int> m_columns;
    QMap<QString,int> m_rows;
};

#endif //SPREADSHEETTABLE_H


