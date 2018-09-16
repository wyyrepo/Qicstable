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

#include "QicsGridLayout.h"

#include "QicsTableGrid.h"

#define LAYOUT_NUM_ROWS 11
#define LAYOUT_NUM_COLS 11

QicsGridLayout::QicsGridLayout( QWidget * parent )
    : QGridLayout(parent)
{
    myWidgets.resize(LAYOUT_NUM_ROWS);

    for ( int row = 0; row < LAYOUT_NUM_ROWS; ++row )
        myWidgets[row].resize(LAYOUT_NUM_COLS);

    setSizeConstraint(SetMinAndMaxSize);
}

QicsGridLayout::~QicsGridLayout()
{
}

QWidget* QicsGridLayout::widgetAt(int row,int column)const
{
    if(row > LAYOUT_NUM_ROWS-1 || column > LAYOUT_NUM_COLS-1)
        return 0;

    return myWidgets.at(row).at(column);
}

void QicsGridLayout::addWidget ( QWidget * widget, int row, int column, Qt::Alignment alignment)
{
    myWidgets[row][column] = widget;
    QGridLayout::addWidget(widget,row,column,alignment);
}



