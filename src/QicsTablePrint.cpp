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

#include "QicsTablePrint.h"

#include <QPainter>
#include <QPrinter>
#include "QicsPrintGrid.h"
#include "QicsDataModel.h"
#include "QicsStyleManager.h"
#include "QicsDimensionManager.h"
#include "QicsPrintPreviewPage.h"
#include "QicsPrintPreviewWidget.h"
#include "QicsTable.h"


QicsTablePrint::QicsTablePrint(QicsTable *table)
    : QicsTableCommon(table, true), myPageMargin(10)
{
    table->uneditCurrentCell();

    initObjects(table->m_tableCommon);
    customize();
}

QicsTablePrint::QicsTablePrint(QicsTableCommon *tc)
    : QicsTableCommon(tc->parent()), myPageMargin(10)
{
    initObjects(tc);
    customize();
}

QicsTablePrint::~QicsTablePrint()
{
    delete myStyleManager;
    delete myRowHeaderStyleManager;
    delete myColumnHeaderStyleManager;

    delete myDimensionManager;
    delete myRowHeaderDM;
    delete myColumnHeaderDM;

    myStyleManager = 0;
    myRowHeaderStyleManager = 0;
    myColumnHeaderStyleManager = 0;
    myDimensionManager = 0;
    myRowHeaderDM = 0;
    myColumnHeaderDM = 0;
}

void QicsTablePrint::initObjects(QicsTableCommon *tc)
{
    // the main data model
    myMainGridInfo.setDataModel(tc->gridInfo().dataModel());
    // separate data models for headers
    myRHDataModel = tc->myRHDataModel;
    myRowHeaderGridInfo.setDataModel(tc->rhGridInfo().dataModel());
    myCHDataModel = tc->myCHDataModel;
    myColumnHeaderGridInfo.setDataModel(tc->chGridInfo().dataModel());

    // the main style manager
    myStyleManager = new QicsStyleManager(tc->gridInfo().styleManager(),
        &myMainGridInfo);
    myMainGridInfo.setStyleManager(myStyleManager);
    // separate style managers for headers
    myRowHeaderStyleManager =
        new QicsStyleManager(tc->rhGridInfo().styleManager(),
        &myRowHeaderGridInfo);
    myRowHeaderGridInfo.setStyleManager(myRowHeaderStyleManager);
    myColumnHeaderStyleManager =
        new QicsStyleManager(tc->chGridInfo().styleManager(),
        &myRowHeaderGridInfo);
    myColumnHeaderGridInfo.setStyleManager(myColumnHeaderStyleManager);

    // dimension managers - the main dimension manager first...
    myDimensionManager =
        new QicsDimensionManager(*(tc->gridInfo().dimensionManager()),
        &myMainGridInfo);
    myMainGridInfo.setDimensionManager(myDimensionManager);
    // followed by a row header DM that passes on row height queries
    // to the main DM...
    myRowHeaderDM =
        new QicsDimensionManager(*(tc->rhGridInfo().dimensionManager()),
        &myRowHeaderGridInfo);
    myRowHeaderDM->setControllingRowDimensionManager(myDimensionManager);
    myRowHeaderGridInfo.setDimensionManager(myRowHeaderDM);
    // and finally a column header DM that passes on column width queries
    // to the main DM.
    myColumnHeaderDM =
        new QicsDimensionManager(*(tc->chGridInfo().dimensionManager()),
        &myColumnHeaderGridInfo);
    myColumnHeaderDM->setControllingColumnDimensionManager(myDimensionManager);
    myColumnHeaderGridInfo.setDimensionManager(myColumnHeaderDM);

    // make visual row/column ordering the same as for source table
    myMainGridInfo.rowOrdering()->reorder(tc->gridInfo().rowOrdering()->currentOrder());
    myMainGridInfo.columnOrdering()->reorder(tc->gridInfo().columnOrdering()->currentOrder());
    myRowHeaderGridInfo.rowOrdering()->reorder(tc->rhGridInfo().rowOrdering()->currentOrder());
    myRowHeaderGridInfo.columnOrdering()->reorder(tc->rhGridInfo().columnOrdering()->currentOrder());
    myColumnHeaderGridInfo.rowOrdering()->reorder(tc->chGridInfo().rowOrdering()->currentOrder());
    myColumnHeaderGridInfo.columnOrdering()->reorder(tc->chGridInfo().columnOrdering()->currentOrder());

    // no default preview widget available...
    myPreview = 0;
}

void QicsTablePrint::customize()
{
}

////////////////////////////////////////////////////////////////////
///////////         Preview Printing methods          //////////////
////////////////////////////////////////////////////////////////////

void QicsTablePrint::setPreviewWidget(QicsPrintPreviewWidget *pw)
{
    if (myPreview)
        myPreview->disconnect(this);

    myPreview = pw;

    if (myPreview) {
        connect(myPreview, SIGNAL(countPages(int *, QPrinter *)), this, SLOT(countPages(int *, QPrinter *)));
        connect(myPreview, SIGNAL(drawPage(QicsPrintPreviewPage*)), this, SLOT(drawPage(QicsPrintPreviewPage*)));
        connect(myPreview, SIGNAL(printPage(QicsPrintPreviewPage*, QPainter*)), this, SLOT(printPage(QicsPrintPreviewPage*, QPainter*)));

        connect(myPreview, SIGNAL(terminateCount()), this, SLOT(terminateCount()));
    }
}

void QicsTablePrint::setPreviewRegion(const QicsRegion &region)
{
    myRegion = region.isValid() ? region : QicsRegion::completeRegion() /*viewport()*/;
}

void QicsTablePrint::countPages(int *pages, QPrinter *printer)
{
    m_progress = true;

    myPageParams.clear();

    int cur_row, cur_col, last_col;

    int end_row = (myRegion.endRow() == QicsLAST_ROW ?
        myMainGridInfo.dataModel()->lastRow() :
    myRegion.endRow());

    int end_col = (myRegion.endColumn() == QicsLAST_COLUMN ?
        myMainGridInfo.dataModel()->lastColumn() :
    myRegion.endColumn());

    cur_col = myRegion.startColumn();

    while (cur_col <= end_col) {
        cur_row = myRegion.startRow();
        last_col = end_col + 1;

        while (cur_row <= end_row) {
            if (!m_progress) return;

            (*pages)++;

            PagePreviewParams pp;
            pp.start_cell = QicsICell(cur_row, cur_col);
            pp.first_row = (cur_row == myRegion.startRow());
            pp.first_col = (cur_col == myRegion.startColumn());

            QPixmap pm(printer->pageRect().size());
            pm.fill();
            QPainter painter;
            painter.begin(&pm);

            QicsICell last_cell = printPage(pp.start_cell,
                &painter,
                pp.first_row,
                pp.first_col);

            painter.end();
            pp.buf = pm;
            myPageParams.append(pp);

            // callback to update current state
            myPreview->pageCounted();

            cur_row = last_cell.row();
            last_col = last_cell.column();
        }

        cur_col = last_col;
    }

    m_progress = false;
}

void QicsTablePrint::terminateCount()
{
    m_progress = false;
}

void QicsTablePrint::drawPage(QicsPrintPreviewPage *page)
{
    PagePreviewParams &pp = myPageParams[page->number()-1];
    const QRect &r = page->pageRect();

    QPainter p1(page);
    p1.setRenderHint(QPainter::SmoothPixmapTransform);

    // if actual scale >= 100%, do not deal with cache
    if (r.width() >= pp.buf.width() && r.height() >= pp.buf.height()) {
        p1.drawPixmap(r, pp.buf, pp.buf.rect());
        return;
    }

    // else, look into the cache before actual draw
    QPixmap &pb = pp.buf_tn;
    if (pb.isNull() || pb.size() != r.size())
        pb = pp.buf.scaled(r.size());
    p1.drawPixmap(r, pb);
}

void QicsTablePrint::printPage(QicsPrintPreviewPage *page, QPainter *painter)
{
    PagePreviewParams &pp = myPageParams[page->number()-1];

    printPage(pp.start_cell,
        painter,
        pp.first_row,
        pp.first_col);
}

////////////////////////////////////////////////////////////////////
///////////               Printing methods            //////////////
////////////////////////////////////////////////////////////////////

void QicsTablePrint::print(QPrinter *printer)
{
    print(printer, viewport());
}

void QicsTablePrint::print(QPrinter *printer, const QicsRegion &region)
{
    int cur_row, cur_col, last_col;

    QPainter painter;

    int end_row = (region.endRow() == QicsLAST_ROW ?
        myMainGridInfo.dataModel()->lastRow() :
    region.endRow());

    int end_col = (region.endColumn() == QicsLAST_COLUMN ?
        myMainGridInfo.dataModel()->lastColumn() :
    region.endColumn());

    painter.begin(printer);

    cur_col = region.startColumn();
    bool first_time = true;

    while (cur_col <= end_col) {
        cur_row = region.startRow();
        last_col = end_col + 1;

        while (cur_row <= end_row) {
            // start a new page, if this isn't the first page
            if (!first_time)
                printer->newPage();

            QicsICell last_cell = printPage(QicsICell(cur_row, cur_col),
                &painter,
                (cur_row == region.startRow()),
                (cur_col == region.startColumn()));
            cur_row = last_cell.row();
            last_col = last_cell.column();

            first_time = false;
        }

        cur_col = last_col;
    }

    painter.end();
}

QicsICell QicsTablePrint::printPage(const QicsICell &start_cell, QPainter *painter,
                          bool first_row, bool first_col)
{
    QicsPrintGrid *main_grid = new QicsPrintGrid(myMainGridInfo);
    QicsPrintGrid *ch_grid = new QicsPrintGrid(myColumnHeaderGridInfo);
    QicsPrintGrid *rh_grid = new QicsPrintGrid(myRowHeaderGridInfo);

    bool do_top_header = false;
    bool do_bottom_header = false;
    bool do_left_header = false;
    bool do_right_header = false;

    int x = 0;
    int y = 0;

    QPaintDevice* metrics = painter->device();

    // Beginning location on the page
    x = myPageMargin;
    y = myPageMargin;

    // Get the left header width.  We need this so we can tell
    // the grid how big it should be when it prints
    int left_header_width = 0;
    if ((myLeftHeaderVisible == DisplayAlways) ||
        (((myLeftHeaderVisible == DisplayFirstPage) && first_col))) {
        do_left_header = true;
        QSize sz = rh_grid->preferredSize();
        left_header_width += sz.width();
    }

    // Get the right header width.  We need this so we can tell
    // the grid how big it should be when it prints
    int right_header_width = 0;
    if (myRightHeaderVisible) {
        do_right_header = true;
        QSize sz = rh_grid->preferredSize();
        right_header_width += sz.width();
    }

    // Get the top header height.  We need this so we can tell
    // the grid how big it should be when it prints
     int top_header_height = 0;
    if ((myTopHeaderVisible == DisplayAlways) ||
        (((myTopHeaderVisible == DisplayFirstPage) && first_row))) {
        do_top_header = true;
        QSize sz = ch_grid->preferredSize();
        top_header_height += sz.height();
    }

    // Get the bottom header height.  We need this so we can tell
    // the grid how big it should be when it prints
    int bottom_header_height = 0;
    if (myBottomHeaderVisible) {
        do_bottom_header = true;
        QSize sz = ch_grid->preferredSize();
        bottom_header_height += sz.height();
    }

    // Compute a rectangle for the grid to draw in
    QPoint tl((x + left_header_width), (y + top_header_height));
    QPoint br((metrics->width() - myPageMargin - right_header_width),
        (metrics->height() - myPageMargin - bottom_header_height));
    QRect rect = QRect(tl, br);

    QicsRegion rc_region = main_grid->regionFromArea(rect, start_cell);

    // Print top header if visible
    if (do_top_header) {
        QicsRegion real_vp = ch_grid->currentViewport();

        QicsRegion r(real_vp.startRow(), rc_region.startColumn(),
            real_vp.endRow(), rc_region.endColumn());
        ch_grid->print(r, QPoint((x + left_header_width), y),
            painter);

        y += top_header_height;
    }

    // Print left header if visible
    if (do_left_header) {
        QicsRegion real_vp = rh_grid->currentViewport();

        QicsRegion r(start_cell.row(), real_vp.startColumn(),
            rc_region.endRow(), real_vp.endColumn());
        rh_grid->print(r, QPoint(x, y), painter);

        x += left_header_width;
    }

    // Save this for use when printing bottom header
    int save_x = x;

    // Print the main grid
    QPoint grid_last = main_grid->print(rc_region, tl, painter);

    // move over to the right
    x = grid_last.x() + 1;

    // Print right header if visible
    if (do_right_header) {
        QicsRegion real_vp = rh_grid->currentViewport();

        QicsRegion r(start_cell.row(), real_vp.startColumn(),
            rc_region.endRow(), real_vp.endColumn());
        rh_grid->print(r, QPoint(x, y), painter);
    }

    // move down
    y = grid_last.y() + 1;

    // Print bottom header if visible
    if (do_bottom_header) {
        QicsRegion real_vp = ch_grid->currentViewport();

        QicsRegion r(real_vp.startRow(), start_cell.column(),
            real_vp.endRow(), rc_region.endColumn());
        ch_grid->print(r, QPoint(save_x, y), painter);
    }

    delete main_grid;
    delete ch_grid;
    delete rh_grid;

    return (QicsICell(rc_region.endRow() + 1, rc_region.endColumn() + 1));
}


