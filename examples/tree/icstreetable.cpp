/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "icstreetable.h"

#include <QMessageBox>
#include <QFile>

#include <QicsGroupCellDisplay.h>
#include <QicsTreeDataModel.h>
#include <QicsGroupBar.h>
#include <QicsSortBar.h>

#include <QicsColumnHeader.h>
#include <QicsSelection.h>
#include <QicsCell.h>
#include <QicsRow.h>
#include <QicsColumn.h>
#include <QicsRegionalAttributeController.h>

#include "QicsFilterCellDisplay.h"
#include "QicsComboCellDisplay.h"
#include "QicsGroupWidget.h"



IcsTreeTable::IcsTreeTable(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    setWindowTitle(tr("Tree Table Example"));

    connect(pbAddColumns, SIGNAL(clicked()), this, SLOT(addColumns()));
    connect(pbRemoveColumns, SIGNAL(clicked()), this, SLOT(removeColumns()));

    connect(pbAddRows, SIGNAL(clicked()), this, SLOT(addRows()));
    connect(pbRemoveRows, SIGNAL(clicked()), this, SLOT(removeRows()));

    // dimensions of the data model
    int numRows = 20;
    int numCols = 4;

    QFile data("table.txt");
    data.open(QFile::ReadOnly);
    QTextStream in(&data);
    in >> numRows >> numCols;

    // create the data model
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);

    // create the table, using the data model we created above
    m_table = new QicsTable(dm,widget);
    QVBoxLayout *vbl1 = new QVBoxLayout(widget);
    vbl1->addWidget(m_table);

    QicsRegionalAttributeController cntr;
    m_table->setExternalAttributeController(cntr);

    // populate the data model with some data
    QString s;
    for (int i = 0; i < numCols; ++i) {
        in >> s;
        m_table->columnHeaderRef().cellRef(0,i).setDataValue(QicsDataString(s));
    }
    in.readLine();
    dm->readASCII(in);
    data.close();

    m_table->setVisibleRows(numRows);
    m_table->setVisibleColumns(numCols);

    // create the tree table in header, using the data model we created above

    m_treeTable = new QicsTreeTable(dm, true);
    m_treeTable->rowHeaderRef().columnRef(0).setWidthInPixels(200);

    //table->setExternalAttributeController(cntr);

    for (int i = 0; i < numCols; ++i)
        m_treeTable->columnHeaderRef().cellRef(0,i).setDataValue(*(m_table->columnHeaderRef().cellRef(0,i).dataValue()));

    // make sure the table is only as large as necessary
    m_treeTable->setVisibleRows(numRows);
    m_treeTable->setVisibleColumns(numCols);

    // summarizer for the table
    m_treeTable->setSummarizer(new CustomSummarizer());

    // displayer for column header
    m_treeTable->columnHeaderRef().setDisplayer(new QicsFilterCellDisplay(m_treeTable));
    //table->columnHeaderRef().setDisplayer(new QicsComboCellDisplay());

    //table->setSelectedBackgroundColor(Qt::red);
    m_treeTable->groupDisplayer()->setStaticContent(true);
    m_treeTable->summaryDisplayer()->setStaticContent(true);

    m_treeTable->columnHeaderRef().setDragEnabled(true);
    m_treeTable->columnHeaderRef().setAllowUserMove(true);

    QVBoxLayout *vbl = new QVBoxLayout(widget_2);

    QicsGroupBar *bar = new QicsGroupBar(m_treeTable, widget_2);
    QHBoxLayout *hbl = new QHBoxLayout();
    hbl->addWidget(bar);

    QicsSortBar *sbar = new QicsSortBar(m_treeTable, widget_2);
    QHBoxLayout *hbls = new QHBoxLayout();
    hbls->addWidget(sbar);

    vbl->addLayout(hbl);
    vbl->addLayout(hbls);
    vbl->addWidget(m_treeTable);

    //connect(pbSummary, SIGNAL(toggled(bool)), table, SLOT(setSummaryRowsVisible(bool)));
    cbSummary->setCurrentIndex(1);
    connect(cbSummary, SIGNAL(activated(int)), m_treeTable, SLOT(setSummaryPolicy(int)));

    // group widget
    QicsGroupWidget *gw = new QicsGroupWidget(m_treeTable, this);
    m_treeTable->setTopLeftCornerWidget(gw);

    // create the tree table, using the data model we created above

    QicsTreeTable *table3 = new QicsTreeTable(dm, false);

    table3->setExternalAttributeController(cntr);

    for (int i = 0; i < numCols; ++i)
        table3->columnHeaderRef().cellRef(0,i+1).setDataValue(*(m_table->columnHeaderRef().cellRef(0,i).dataValue()));

    // make sure the table is only as large as necessary
    table3->setVisibleRows(numRows);
    table3->setVisibleColumns(numCols);

    // summarizer for the table
    table3->setSummarizer(new CustomSummarizer());

    // displayer for column header
    table3->columnHeaderRef().setDisplayer(new QicsFilterCellDisplay(table3));

    table3->groupDisplayer()->setStaticContent(true);
    table3->summaryDisplayer()->setStaticContent(true);

    QVBoxLayout *vbl3 = new QVBoxLayout(widget_3);

    QicsGroupBar *bar3 = new QicsGroupBar(table3, widget_3);
    QHBoxLayout *hbl3 = new QHBoxLayout();
    hbl3->addWidget(bar3);

    QicsSortBar *sbar3 = new QicsSortBar(table3, widget_3);
    QHBoxLayout *hbls3 = new QHBoxLayout();
    hbls3->addWidget(sbar3);

    vbl3->addLayout(hbl3);
    vbl3->addLayout(hbls3);
    vbl3->addWidget(table3);

    //connect(pbSummary_3, SIGNAL(toggled(bool)), table3, SLOT(setSummaryRowsVisible(bool)));
    cbSummary_2->setCurrentIndex(1);
    connect(cbSummary_2, SIGNAL(activated(int)), m_treeTable, SLOT(setSummaryPolicy(int)));

    resize(640,480);
}

IcsTreeTable::~IcsTreeTable()
{
}

void IcsTreeTable::addColumns()
{
    QicsDataModel *model = m_table->dataModel();

    QicsSelectionList *sl = m_table->selectionList(true);
    if (sl && sl->count()) {
        QicsSelection sel = sl->at(0);
        if (sel.endColumn() != Qics::QicsLAST_COLUMN) {
            model->insertColumns(1, sel.startColumn());
            return;
        }
    }

    model->insertColumns(1, model->numColumns());
}

void IcsTreeTable::removeColumns()
{
    QicsSelectionList *sl = m_table->selectionList();
    if (!sl) return;
    for (int j = 0; j < sl->count(); ++j) {
        QicsSelection sel = sl->at(j);
        m_table->dataModel()->deleteColumns(sel.numColumns(), sel.leftColumn());
    }
    delete sl;
}

void IcsTreeTable::addRows()
{
    QicsDataModel *model = m_table->dataModel();

    QicsSelectionList *sl = m_table->selectionList(true);
    if (sl && sl->count()) {
        QicsSelection sel = sl->at(0);
        if (sel.endRow() != Qics::QicsLAST_ROW) {
            model->insertRows(1, sel.startRow());
            return;
        }
    }

    model->insertRows(1, model->numRows());
}

void IcsTreeTable::removeRows()
{
    QicsSelectionList *sl = m_table->selectionList();
    if (!sl) return;
    for (int j = 0; j < sl->count(); ++j) {
        QicsSelection sel = sl->at(j);
        m_table->dataModel()->deleteRows(sel.numRows(), sel.topRow());
    }
    delete sl;
}

void IcsTreeTable::on_pbAbout_clicked()
{
    // init helpDialog
    QMessageBox helpDialog(this);
    helpDialog.setText(
        "<table>"
        "<tr><td>"
        "<img src=':/Resources/ics.png'>"
        "<br><h3>Integrated Computer Solutions Incorporated</h3>"
        "</td><td>"
        "<h3>The User Interface Company</h3>"
        "54B Middlesex Turnpike<br>Bedford, MA USA 01730<br><br>"
        "<b>info@ics.com<br>www.ics.com</b>"
        "</td><tr><td colspan=2>"
        "<hr>"
        "This example shows basic features of QicsTreeTable. "
        "<p>"
        "<b>GROUPING</b><br>"
        "There is group bar on the top of the page.<br> "
        "To group a column, double-click its header. Or right-click on group bar and choose columns to group from popup.<br> "
        "Grouped columns are represented as sections on the bar. Moving them by mouse will result in changing order of grouping.<br>"
        "To ungroup a column, click close button on the section. Use group bar's close button to remove all the groups.<br>"
        "<p>"
        "<b>SORTING</b><br>"
        "There is sort bar on the top of the page, just below the group bar.<br> "
        "To sort by a column, right double-click its header. Or right-click on sort bar and choose columns to sort by from popup.<br> "
        "Sorted columns are represented as sections on the bar. Moving them by mouse will result in changing order of sorting.<br>"
        "To unsort a column, click close button on the section. Use sort bar's close button to remove all the sorting.<br>"
        "<p>"
        "<b>FILTERING</b><br>"
        "Groups can be filtered in order to hide undesired items.<br> "
        "Click on the group section's combo button and uncheck items to filter off.<br> "
        "Filtered groups are marked with asterisk (*).<br> "
        "<hr>"
        "</td></table>"
        );
    helpDialog.setWindowTitle(tr("About Tree Table Example"));
    helpDialog.setIcon( QMessageBox::NoIcon );
    helpDialog.exec();
}


void CustomSummarizer::onCellValueChanged(int row, int col, QicsGroupInfo *gi)
{
    Q_UNUSED(row);
    if (!gi) return;

    int av = 0;
    bool ok = false;

    for (int i = gi->headerVisualIndex()+1; i < gi->summaryVisualIndex(); ++i) {
        const QicsDataInt *itm = dynamic_cast<const QicsDataInt*>(gi->table()->cellRef(i, col).dataValue());
        if (itm) {
            av += itm->data();
            ok = true;
        }
    }

    if (ok) {
        //gi->summaryCellRef(col).setLabel(QString::number(av));
        gi->summaryRef().setAlignment(Qt::AlignRight);
        gi->summaryRef().setLabel(QObject::tr("Total: %1").arg(av));
    }
}

void CustomSummarizer::onGrouping(QicsGroupInfo *gi)
{
    gi->spanHeader(true);
    gi->spanSummary(true);

    // show summaty rows only for 1st two groups
    if (gi->level() > 1)
        gi->hideSummary();
    else {
        // set attributes of the summary label
        if (gi->level() == 0) {
            // for the 1st group, we will count the sums
            //gi->spanSummary(false);
            for (int col = 0; col < gi->table()->columnCount(); ++col)
                onCellValueChanged(0, col, gi);
        } else {
            QString s = QObject::tr("Grouped by: %1  Items count: %2  Level: %3")
                .arg(gi->columnIndex()).arg(gi->rows().count()).arg(gi->level());

            // make an indent according to the level of the group
            if (gi->level()) {
                QString lvl;
                lvl.fill(' ', gi->level());
                s = lvl + s;
            }
            gi->summaryRef().setLabel(s);
        }

        gi->summaryRef().setBackgroundColor(gi->isFirst() ? Qt::yellow : gi->isLast() ? Qt::blue : Qt::gray);
        //gi->summaryRef().setFont(QFont("Courier", 10-gi->level(), QFont::Bold, true));
        gi->summaryRef().setSelectedBackgroundColor(Qt::darkBlue);

        if (gi->level())
            gi->summaryRef().setForegroundColor(Qt::cyan);
    }

    // set attributes of the header label
    gi->headerRef().setBackgroundColor(gi->level() ? Qt::cyan : Qt::green);
    gi->headerRef().setSelectedBackgroundColor(Qt::darkGreen);
    gi->headerRef().setFont(QFont("Arial", 12-gi->level(), QFont::Bold));
    gi->headerRef().setLabel(gi->data());
    gi->headerCellRef(0).setLabel(gi->data());
    //gi->headerRef().setLabel(QString("%1 - %2").arg(gi->content(), gi->data()));
}


