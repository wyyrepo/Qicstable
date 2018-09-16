/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <QApplication>
#include <QLabel>
#include <QtSql>

#include <QicsTable.h>
#include <QicsRepeatingRow.h>
#include <QicsDataModelDefault.h>
#include <QicsDataModelQtModelAdapter.h>

#include "connection.h"

int main(int argc, char **argv)
{
        QApplication app(argc, argv);

        //creation and population a database
        if (!createConnection())
            return 0;

        //creation of an Qt's sql-table model
        QSqlTableModel *sqlTableModel = new QSqlTableModel();
        sqlTableModel->setTable("person");
        sqlTableModel->select();

        //creation of an adapter
        QicsDataModelQtModelAdapter *adapter = new QicsDataModelQtModelAdapter();
        adapter->setModel(sqlTableModel);

        //creation of a table
        QicsTable table(adapter);
        // make sure the table is only as large as necessary
        table.setVisibleRows(sqlTableModel->rowCount());
        table.setVisibleColumns(sqlTableModel->columnCount());

        //creation of a label
        QLabel label(QObject::tr("QSqlTableModel feat. QicsTable"));
        label.setAlignment(Qt::AlignCenter);
        table.setTopTitleWidget(&label);

        table.show();

        return app.exec();
}


