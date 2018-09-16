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
#include <QListWidget>
#include <QTreeWidget>

#include <QicsWidgetCellDisplay.h>
#include <QicsDataModelDefault.h>
#include "widgetsTable.h"


int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(widgetsTable);

    QApplication app(argc,argv);

    // dimensions of the data model
    const int numRows = 10;
    const int numCols = 10;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // populate the data model with some data
    for (int i = 0; i < numRows; ++i)
        for (int j = 0; j < numCols; ++j)
            dm->setItem(i, j, QicsDataInt(i*j));

    // create the table, using the data model we created above
    WidgetsTable *table = new WidgetsTable(dm, 0);

    // SET SPANS AND COLORS
    table->setVisibleRows(numRows);
    table->setVisibleColumns(numCols);
    table->cellRef(2, 0).addSpan(0, numCols);
    table->rowRef(2).setHeightInChars(7);
    table->cellRef(4, 0).addSpan(0, numCols);
    table->rowRef(4).setHeightInChars(7);
    table->mainGridRef().setBackgroundColor(QColor(255, 255, 0));
    table->columnHeaderRef().setBackgroundColor(QColor(255, 200, 0));
    table->rowHeaderRef().setBackgroundColor(QColor(255, 100, 0));

    // CREATE AND INITIAL QListWidget
    QListWidget *contentsWidget = new  QListWidget(table);
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setSpacing(5);

    QPixmap pixmap;
    QListWidgetItem *configItem = new QListWidgetItem(contentsWidget);
    configItem->setText(QObject::tr("Configure"));
    pixmap.load(":/images/config.png");
    configItem->setIcon(pixmap);
    contentsWidget->addItem(configItem);

    QListWidgetItem *updateItem = new QListWidgetItem(contentsWidget);
    updateItem->setText(QObject::tr("Update"));
    pixmap.load(":/images/update.png");
    updateItem->setIcon(pixmap);
    contentsWidget->addItem(updateItem);

    QListWidgetItem *queryItem = new QListWidgetItem(contentsWidget);
    queryItem->setText(QObject::tr("Query"));
    pixmap.load(":/images/query.png");
    queryItem->setIcon(pixmap);
    contentsWidget->addItem(queryItem);
    // contentsWidget->setCurrentRow(0);

    table->rowRef(2).setDisplayer(new QicsWidgetCellDisplay(contentsWidget, table));

    // CREATE AND INITIAL QTreeWidget
    QStringList headerLabels;
    headerLabels << QObject::tr("Tree");

    QTreeWidget* treeWidget = new QTreeWidget(table);
    treeWidget->setAlternatingRowColors(false);
    treeWidget->setHeaderLabels(headerLabels);

    QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
    rootItem->setText(0, QObject::tr("Root"));
    treeWidget->setItemExpanded(rootItem, true);
    QFont boldFont = rootItem->font(0);
    boldFont.setBold(true);
    rootItem->setFont(0, boldFont);

    QTreeWidgetItem *querysItem = new QTreeWidgetItem(rootItem);
    querysItem->setText(0, QObject::tr("Querys"));
    pixmap.load(":/images/query.png");
    querysItem->setIcon(0, QIcon(pixmap));
    treeWidget->setItemExpanded(querysItem, true);

    QTreeWidgetItem *querysChileItem1 = new QTreeWidgetItem(querysItem);
    querysChileItem1->setText(0, QObject::tr("One"));
    treeWidget->setItemExpanded(querysChileItem1, true);

    QTreeWidgetItem *querysChileItem2 = new QTreeWidgetItem(querysItem);
    querysChileItem2->setText(0, QString("Two"));
    treeWidget->setItemExpanded(querysChileItem2, true);

    QTreeWidgetItem *updatesItem = new QTreeWidgetItem(rootItem);
    updatesItem->setText(0, QObject::tr("Updates"));
    pixmap.load(":/images/update.png");
    updatesItem->setIcon(0, QIcon(pixmap));
    treeWidget->setItemExpanded(updatesItem, true);

    QTreeWidgetItem *updatesChileItem1 = new QTreeWidgetItem(updatesItem);
    updatesChileItem1->setText(0, QObject::tr("One"));
    treeWidget->setItemExpanded(updatesChileItem1, true);

    QTreeWidgetItem *updatesChileItem2 = new QTreeWidgetItem(updatesItem);
    updatesChileItem2->setText(0, QObject::tr("Two"));
    treeWidget->setItemExpanded(updatesChileItem2, true);

    QTreeWidgetItem *configuresItem = new QTreeWidgetItem(rootItem);
    configuresItem->setText(0, QObject::tr("Configures"));
    pixmap.load(":/images/config.png");
    configuresItem->setIcon(0, QIcon(pixmap));
    treeWidget->setItemExpanded(configuresItem, true);

    table->rowRef(4).setDisplayer(new QicsWidgetCellDisplay(treeWidget, table));

    table->show();

    return app.exec();
}


