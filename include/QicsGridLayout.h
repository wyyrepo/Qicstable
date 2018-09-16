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

#ifndef QICSGRIDLAYOUT_H
#define QICSGRIDLAYOUT_H

#include <QGridLayout>
#include <QPointer>
#include <QVector>
#include "QicsNamespace.h"

//
// Indices for the "grids" grid layout
//
#define GRID_LAYOUT_LEFT_TITLE_IDX 0
#define GRID_LAYOUT_LEFT_SCROLLBAR_IDX 1
#define GRID_LAYOUT_LEFT_HDR_IDX 2
#define GRID_LAYOUT_LEFT_FROZEN_GRID_IDX 3
#define GRID_LAYOUT_LEFT_FROZEN_LINE_IDX 4
#define GRID_LAYOUT_MAIN_GRID_IDX 5
#define GRID_LAYOUT_RIGHT_FROZEN_LINE_IDX 6
#define GRID_LAYOUT_RIGHT_FROZEN_GRID_IDX 7
#define GRID_LAYOUT_RIGHT_HDR_IDX 8
#define GRID_LAYOUT_RIGHT_SCROLLBAR_IDX 9
#define GRID_LAYOUT_RIGHT_TITLE_IDX 10

#define GRID_LAYOUT_TOP_TITLE_IDX 0
#define GRID_LAYOUT_TOP_SCROLLBAR_IDX 1
#define GRID_LAYOUT_TOP_HDR_IDX 2
#define GRID_LAYOUT_TOP_FROZEN_GRID_IDX 3
#define GRID_LAYOUT_TOP_FROZEN_LINE_IDX 4
//5 it is main grid
#define GRID_LAYOUT_BOTTOM_FROZEN_LINE_IDX 6
#define GRID_LAYOUT_BOTTOM_FROZEN_GRID_IDX 7
#define GRID_LAYOUT_BOTTOM_HDR_IDX 8
#define GRID_LAYOUT_BOTTOM_SCROLLBAR_IDX 9
#define GRID_LAYOUT_BOTTOM_TITLE_IDX 10
/////////////////////////////////////////////////////////////////////////////////

/*! \internal
* \class QicsGridLayout QicsGridLayout.h
* \brief helper class for QicsTable layout management
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////


class QICS_EXPORT QicsGridLayout : public QGridLayout
{
    Q_OBJECT
public:
    QicsGridLayout(QWidget *parent = 0);
    ~QicsGridLayout();

    void addWidget ( QWidget * widget, int row, int column, Qt::Alignment alignment = 0 );
    QWidget* widgetAt(int row,int column) const;

protected:
    QVector< QVector< QPointer<QWidget> > > myWidgets;
};

#endif //QICSGRIDLAYOUT_H


