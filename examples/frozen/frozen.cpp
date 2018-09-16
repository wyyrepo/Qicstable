/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "frozen.h"

#include <QApplication>
#include <QMenuBar>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QMessageBox>

#include <QicsTable.h>
#include <QicsRowHeader.h>
#include <QicsCell.h>
#include <QicsColumnHeader.h>
#include <QicsSelection.h>
#include <QicsDataModelDefault.h>
#include "QicsFrozenConfigDialog.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Frozen::Frozen(QicsDataModel *dm, QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Frozen Example"));

    m_table = new QicsTable(dm, this);

    // Set some labels on the row and column headers
    for (int i = 0; i < dm->numRows(); ++i)
        m_table->rowHeaderRef().cellRef(i,0).setLabel(tr("Row %1").arg(i));

    for (int i = 0; i < dm->numColumns(); ++i)
        m_table->columnHeaderRef().cellRef(0,i).setLabel(tr("Col %1").arg(i));

    setCentralWidget(m_table);

    // Dialogs for use in the future
    m_frozenDialog = new QicsFrozenConfigDialog(this);
    m_moveDialog = new MoveDialog(this);

    // Menu system
    QMenuBar *menu = menuBar();

    QMenu *file = new QMenu(tr("&File"), this);
    file->addAction(tr("E&xit"), qApp, SLOT(quit()));
    menu->addMenu(file);

    QMenu *view = new QMenu(tr("&View"),this);
    view->addAction(tr("F&reeze..."), this, SLOT(doFrozenConfig()));
    view->addAction(tr("&Move..."), this, SLOT(doMoveConfig()));
    menu->addMenu(view);

    // Help
    QMenu* help = new QMenu(tr("&Help"), this);
    help->addAction(QIcon(":/Resources/icon16_help.png"), tr("About..."), this, SLOT(about()), Qt::CTRL+Qt::Key_H);
    menu->addMenu(help);
}

void Frozen::doFrozenConfig()
{
    // Pop up the frozen dialog, wait until the user dismisses it

    m_frozenDialog->exec();

    // Now do what the user requested

    if (m_frozenDialog->freezeTopRows())
        m_table->freezeTopRows(m_frozenDialog->numTopFrozenRows());
    else
        m_table->unfreezeTopRows();

    if (m_frozenDialog->freezeBottomRows())
        m_table->freezeBottomRows(m_frozenDialog->numBottomFrozenRows());
    else
        m_table->unfreezeBottomRows();

    if (m_frozenDialog->freezeLeftColumns())
        m_table->freezeLeftColumns(m_frozenDialog->numLeftFrozenCols());
    else
        m_table->unfreezeLeftColumns();

    if (m_frozenDialog->freezeRightColumns())
        m_table->freezeRightColumns(m_frozenDialog->numRightFrozenCols());
    else
        m_table->unfreezeRightColumns();
}

void Frozen::doMoveConfig()
{
    // Pop up the move dialog, wait until the user dismisses it

    m_moveDialog->exec();

    // Now do what the user requested

    if (m_moveDialog->moveToTop() || m_moveDialog->moveToBottom()) {
        // We are moving rows

        QVector<int> rows_to_move;

        QicsSelectionList *slist = m_table->selectionList();
        QicsSelectionList::iterator iter;

        for (iter = slist->begin(); iter != slist->end(); ++iter) {
            // For each selection in the selection list, check to
            // ensure that the selection is an entire row (or rows).
            // If it is, add the row indices to the rows_to_move list.

            const QicsSelection &sel = *iter;

            if ((sel.leftColumn() == 0) && (sel.rightColumn() == Qics::QicsLAST_COLUMN)) {
                int nrows = rows_to_move.size();

                rows_to_move.resize(nrows + (sel.bottomRow() - sel.topRow() + 1));

                for (int i = sel.topRow(); i <= sel.bottomRow(); ++i)
                    rows_to_move[nrows++] = i;
            }
        }

        // This ensures that the rows are moved in the correct order
        qSort(rows_to_move.begin(),rows_to_move.end());

        // Now we can move the rows to the top or the bottom
        m_table->moveRows((m_moveDialog->moveToTop() ? 0 : m_table->bottomRow() + 1), rows_to_move);

        delete slist;
    }

    if (m_moveDialog->moveToLeft() || m_moveDialog->moveToRight()) {
        // We are moving columns

        QVector<int> cols_to_move;

        QicsSelectionList *slist = m_table->selectionList();
        QicsSelectionList::iterator iter;

        for (iter = slist->begin(); iter != slist->end(); ++iter) {
            // For each selection in the selection list, check to
            // ensure that the selection is an entire column (or columns).
            // If it is, add the column indices to the cols_to_move list.

            const QicsSelection &sel = *iter;

            if ((sel.topRow() == 0) && (sel.bottomRow() == Qics::QicsLAST_ROW)) {
                int ncols = cols_to_move.size();

                cols_to_move.resize(ncols +	(sel.rightColumn() - sel.leftColumn() + 1));

                for (int i = sel.leftColumn(); i <= sel.rightColumn(); ++i)
                    cols_to_move[ncols++] = i;
            }
        }

        // This ensures that the columns are moved in the correct order
        qSort(cols_to_move.begin(),cols_to_move.end());
        // Now we can move the columns to the left or the right
        m_table->moveColumns((m_moveDialog->moveToLeft() ? 0 : m_table->rightColumn() + 1), cols_to_move);

        delete slist;
    }
}

void Frozen::about()
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
        "This example represents Frozen based on QicsTable."
        "<p>"
        "<b>HOW TO USE</b><br>"
        "TOOD: Need user instruction"
        "</td></table>"
        );
    helpDialog.setWindowTitle( tr("About Frozen Example") );
    helpDialog.setIcon( QMessageBox::NoIcon );
    helpDialog.exec();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

MoveDialog::MoveDialog(QWidget *parent)
: QDialog(parent)
{
    QVBoxLayout *box = new QVBoxLayout(this);

    QButtonGroup *bg = new QButtonGroup(/*1, Qt::Horizontal, */box);

    m_top = new QRadioButton();
    m_top->setText(tr("Move Rows to Top"));

    m_bottom = new QRadioButton();
    m_bottom->setText(tr("Move Rows to Bottom"));

    m_left = new QRadioButton();
    m_left->setText(tr("Move Columns to Left"));

    m_right = new QRadioButton();
    m_right->setText(tr("Move Columns to Right"));

    bg->addButton(m_top);
    bg->addButton(m_bottom);
    bg->addButton(m_left);
    bg->addButton(m_right);

    box->addWidget(m_top);
    box->addWidget(m_bottom);
    box->addWidget(m_right);
    box->addWidget(m_left);

    QHBoxLayout *row = new QHBoxLayout();

    QPushButton *ok = new QPushButton("OkButton",this);
    ok->setText(tr("OK"));
    row->addWidget(ok);
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()) );

    QPushButton *cancel = new QPushButton("CancelButton",this);
    cancel->setText(tr("Cancel"));
    row->addWidget(cancel);
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()) );
    box->addLayout(row);
    setLayout(box);
}

int MoveDialog::exec()
{
    m_moveToTop = false;
    m_moveToBottom = false;
    m_moveToLeft = false;
    m_moveToRight = false;

    const int ret = QDialog::exec();

    if (ret == QDialog::Accepted) {
        m_moveToTop = m_top->isChecked();
        m_moveToBottom = m_bottom->isChecked();
        m_moveToLeft = m_left->isChecked();
        m_moveToRight = m_right->isChecked();
    }

    return ret;
}


