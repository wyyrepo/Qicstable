/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "QicsFrozenConfigDialog.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

/**
 *  Dialog to configure the frozen rows and columns;
 */
QicsFrozenConfigDialog::QicsFrozenConfigDialog(QWidget *parent) :
    QDialog(parent)
{

    QVBoxLayout *box = new QVBoxLayout(this);

    QGridLayout *grid = new QGridLayout();
    int freezeColumn = 1;
    int numColumn = 3;
    box->addLayout(grid);
    setWindowModality(Qt::WindowModal);

    QLabel *label;
    grid->setColumnStretch(2, 10);

    label = new QLabel(this);
    label->setText("How many?");
    grid->addWidget(label, 0, numColumn);

    // Top Frozen row data
    _frTop = new QCheckBox(this);
    _frTop->setText("Freeze Top Rows");
    grid->addWidget(_frTop, 1, freezeColumn);

    _top = new QLineEdit(this);
    grid->addWidget(_top, 1, numColumn);

    // Bottom Frozen row data
    _frBottom = new QCheckBox(this);
    _frBottom->setText("Freeze Bottom Rows");
    grid->addWidget(_frBottom, 2, freezeColumn);

    _bottom = new QLineEdit(this);
    grid->addWidget(_bottom, 2, numColumn);

    // Left Frozen column data
    _frLeft = new QCheckBox(this);
    _frLeft->setText("Freeze Left Columns");
    grid->addWidget(_frLeft, 3, freezeColumn);

    _left = new QLineEdit(this);
    grid->addWidget(_left, 3, numColumn);

    // Right Frozen column data
    _frRight = new QCheckBox(this);
    _frRight->setText("Freeze Right Columns");
    grid->addWidget(_frRight, 4, freezeColumn);

    _right = new QLineEdit(this);
    grid->addWidget(_right, 4, numColumn);


    grid->setRowStretch(3, 10);

    QHBoxLayout *row = new QHBoxLayout();

    QPushButton *ok = new QPushButton("OkButton",this);
    ok->setText("OK");
    row->addWidget(ok);
    connect(ok, SIGNAL(clicked()),
	    this, SLOT(accept()) );

    QPushButton *cancel = new QPushButton("CancelButton",this);
    cancel->setText("Cancel");
    row->addWidget(cancel);
    connect(cancel, SIGNAL(clicked()),
	    this, SLOT(reject()) );
    box->addLayout(row);
}


int QicsFrozenConfigDialog::exec()
{
    _freezeTopRows = false;
    _freezeBottomRows = false;
    _freezeLeftColumns = false;
    _freezeRightColumns = false;

    if (QDialog::exec() == QDialog::Accepted)
    {
	if (_frTop->isChecked())
	    _freezeTopRows = true;

	if (_frBottom->isChecked())
	    _freezeBottomRows = true;

	if (_frLeft->isChecked())
	    _freezeLeftColumns = true;

	if (_frRight->isChecked())
	    _freezeRightColumns = true;
    }

    return 0;
}

int QicsFrozenConfigDialog::numTopFrozenRows(void) const
{
    return (_top->text().toInt());
}

int QicsFrozenConfigDialog::numBottomFrozenRows(void) const
{
    return (_bottom->text().toInt());
}

int QicsFrozenConfigDialog::numLeftFrozenCols(void) const
{
    return (_left->text().toInt());
}

int QicsFrozenConfigDialog::numRightFrozenCols(void) const
{
    return (_right->text().toInt());
}
