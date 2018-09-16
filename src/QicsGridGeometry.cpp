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

#include "QicsGridGeometry.h"

#include "QicsTableGrid.h"


#define TOP_LEFT_TABLE_GRID myGrids[1][1]
#define TOP_MIDDLE_TABLE_GRID myGrids[1][2]
#define TOP_RIGHT_TABLE_GRID myGrids[1][3]
#define MIDDLE_LEFT_TABLE_GRID myGrids[2][1]
#define MAIN_TABLE_GRID myGrids[2][2]
#define MIDDLE_RIGHT_TABLE_GRID myGrids[2][3]
#define BOTTOM_LEFT_TABLE_GRID myGrids[3][1]
#define BOTTOM_MIDDLE_TABLE_GRID myGrids[3][2]
#define BOTTOM_RIGHT_TABLE_GRID myGrids[3][3]

#define HEADER_TOP_1 myGrids[0][1]
#define HEADER_TOP_2 myGrids[0][2]
#define HEADER_TOP_3 myGrids[0][3]

#define HEADER_LEFT_1 myGrids[1][0]
#define HEADER_LEFT_2 myGrids[2][0]
#define HEADER_LEFT_3 myGrids[3][0]

#define HEADER_RIGHT_1 myGrids[1][4]
#define HEADER_RIGHT_2 myGrids[2][4]
#define HEADER_RIGHT_3 myGrids[3][4]

#define HEADER_BOTTOM_1 myGrids[4][1]
#define HEADER_BOTTOM_2 myGrids[4][2]
#define HEADER_BOTTOM_3 myGrids[4][3]

#define HEADER_TOP_ROW_IDX 0
#define HEADER_BOTTOM_ROW_IDX 4

#define HEADER_LEFT_COL_IDX 0
#define HEADER_RIGHT_COL_IDX 4



QicsGridGeometry::QicsGridGeometry(QObject* parent)
    : QObject(parent)
{
    //initialize grids
    myGrids.resize(5);

    for( int row=0; row<myGrids.size(); ++row ) {
        myGrids[row].resize(5);
        myGrids[row].fill(0);
    }
}

QicsGridGeometry::~QicsGridGeometry()
{
}

QicsScreenGrid *QicsGridGeometry::gridLeftOf(QicsScreenGrid* grid)
{
    int gridRow = gridRowCoord(grid);
    int gridCol = gridColCoord(grid);

    // If the grid is 0 on the Col then there is nothing to the left
    // If this grid returns an Row or Col of -1 then its invalid
    // Either way return 0
    if( gridCol <= 0 || gridRow < 0 )
        return 0; // No grid to the left of this one

    //Walk left until we hit a grid or pass Col 0
    for( int i=gridCol-1; i>=0; --i )
        if( myGrids.at(gridRow).at(i) != 0 )
            return myGrids.at(gridRow).at(i);

    // Fall through
    return 0;
}

QicsScreenGrid *QicsGridGeometry::gridRightOf(QicsScreenGrid* grid)
{
    int gridRow = gridRowCoord(grid);
    int gridCol = gridColCoord(grid);

    // If the grid is 4 on the Col then there is nothing to the right
    // If this grid returns an X or Y of -1 then its invalid
    // Either way return 0
    if( gridCol < 0 || gridCol >= 4 || gridRow < 0 )
        return 0; // No grid to the left of this one

    //Walk right until we hit a grid or pass Col 4
    for( int i=gridCol+1; i<5; ++i )
        if( myGrids.at(gridRow).at(i) != 0 )
            return myGrids.at(gridRow).at(i);

    // Fall through
    return 0;
}

QicsScreenGrid *QicsGridGeometry::gridAbove(QicsScreenGrid* grid)
{
    int gridRow = gridRowCoord(grid);
    int gridCol = gridColCoord(grid);

    // If the grid is 0 on the Row then there is nothing above
    // If this grid returns an Row or Col of -1 then its invalid
    // Either way return 0
    if( gridCol < 0 || gridRow < 0 || gridRow <=0 || gridCol < 0 )
        return 0; // No grid to above this one

    //Walk up until we hit a grid or pass Row 0
    for( int i=gridRow-1; i>=0; --i )
        if( myGrids.at(i).at(gridCol) != 0 )
            return myGrids.at(i).at(gridCol);

    // Fall through
    return 0;
}

QicsScreenGrid *QicsGridGeometry::gridBelow(QicsScreenGrid* grid)
{
    int gridRow = gridRowCoord(grid);
    int gridCol = gridColCoord(grid);

    // If the grid is 4 on the Row then there is nothing below
    // If this grid returns an Row or Col of -1 then its invalid
    // Either way return 0
    if( gridCol < 0 || gridRow < 0 || gridRow >=4 || gridCol < 0 )
        return 0; // No grid to the left of this one

    // Walk down until we hit a grid or pass Row 4
    for( int i=gridRow+1; i<5; ++i )
        if( myGrids.at(i).at(gridCol) != 0 )
            return myGrids.at(i).at(gridCol);

    // Fall through
    return 0;
}

QicsScreenGrid *QicsGridGeometry::mainTableGrid()
{
    return MAIN_TABLE_GRID;
}

void QicsGridGeometry::setMainTableGrid(QicsScreenGrid* grid)
{
    MAIN_TABLE_GRID = grid;

    connectGridSignals(grid);
    emit gridAdded(grid);
}

QicsScreenGrid *QicsGridGeometry::tableGrid(TableRegion region)
{
    switch(region)
    {
    case TopLeft:
        return TOP_LEFT_TABLE_GRID;
        break;
    case TopMiddle:
        return TOP_MIDDLE_TABLE_GRID;
        break;
    case TopRight:
        return TOP_RIGHT_TABLE_GRID;
        break;
    case MiddleLeft:
        return MIDDLE_LEFT_TABLE_GRID;
        break;
    case Main:
        return MAIN_TABLE_GRID;
        break;
    case MiddleRight:
        return MIDDLE_RIGHT_TABLE_GRID;
        break;
    case BottomLeft:
        return BOTTOM_LEFT_TABLE_GRID;
        break;
    case BottomMiddle:
        return BOTTOM_MIDDLE_TABLE_GRID;
        break;
    case BottomRight:
        return BOTTOM_RIGHT_TABLE_GRID;
        break;
    }

    return 0;
}

void QicsGridGeometry::setTableGrid(TableRegion region, QicsScreenGrid* grid)
{
    switch(region)
    {
    case TopLeft:
        TOP_LEFT_TABLE_GRID = grid;
        break;
    case TopMiddle:
        TOP_MIDDLE_TABLE_GRID = grid;
        break;
    case TopRight:
        TOP_RIGHT_TABLE_GRID = grid;
        break;
    case MiddleLeft:
        MIDDLE_LEFT_TABLE_GRID = grid;
        break;
    case Main:
        MAIN_TABLE_GRID = grid;
        break;
    case MiddleRight:
        MIDDLE_RIGHT_TABLE_GRID = grid;
        break;
    case BottomLeft:
        BOTTOM_LEFT_TABLE_GRID = grid;
        break;
    case BottomMiddle:
        BOTTOM_MIDDLE_TABLE_GRID = grid;
        break;
    case BottomRight:
        BOTTOM_RIGHT_TABLE_GRID = grid;
        break;
    }

    connectGridSignals(grid);
    emit gridAdded(grid);
}

QicsScreenGrid *QicsGridGeometry::headerGrid(HeaderPosition position, int section)
{
    if(section > 3 || section < 0)
        return 0;

    if( position == Top )
        return myGrids.at(HEADER_TOP_ROW_IDX).at(section);

    if( position == Bottom )
        return myGrids.at(HEADER_BOTTOM_ROW_IDX).at(section);

    if( position == Left )
        return myGrids.at(section).at(HEADER_LEFT_COL_IDX);

    return myGrids.at(section).at(HEADER_RIGHT_COL_IDX);
}

void
QicsGridGeometry::setHeaderGrid(HeaderPosition position, int section, QicsScreenGrid* grid)
{
    if( position == Top )
        myGrids[HEADER_TOP_ROW_IDX][section] = grid;
    else if( position == Bottom )
        myGrids[HEADER_BOTTOM_ROW_IDX][section] = grid;
    else if( position == Left )
        myGrids[section][HEADER_LEFT_COL_IDX] = grid;
    else
        myGrids[section][HEADER_RIGHT_COL_IDX] = grid;

    connectGridSignals(grid);
    emit gridAdded(grid);
}

int QicsGridGeometry::gridRowCoord(QicsScreenGrid* grid)
{
    // Walk array looking for grid
    for( int row=0; row<5; ++row )
        for( int col=0; col<5; ++col )
            if ( myGrids.at(row).at(col) == grid )
                return row;

    //Fall through
    return -1;
}

int QicsGridGeometry::gridColCoord(QicsScreenGrid* grid)
{
    // Walk array looking for grid
    for( int row=0; row<5; ++row )
        for( int col=0; col<5; ++col )
            if ( myGrids.at(row).at(col) == grid )
                return col;

    //Fall through
    return -1;
}

void QicsGridGeometry::connectGridSignals(QicsScreenGrid* grid)
{
    connect( grid, SIGNAL(destroyed(QObject*)),
        this, SLOT(handleGridDeleted(QObject*)));
}

void QicsGridGeometry::handleGridDeleted(QObject* grid)
{
    //Find grid and zero it
    for( int row=0; row<5; ++row )
        for( int col=0; col<5; ++col )
            if ( myGrids.at(row).at(col) == grid )
                myGrids[row][col] = 0;

    QicsScreenGrid* screenGrid = qobject_cast<QicsScreenGrid*>(grid);

    if(screenGrid)
        emit gridRemoved(screenGrid);
}

QList<QicsScreenGrid*> QicsGridGeometry::allGrids()
{
    QList<QicsScreenGrid*> grids;

    //Find grids
    for( int row=0; row<5; ++row )
        for( int col=0; col<5; ++col )
            if ( myGrids.at(row).at(col) !=0 )
                grids << myGrids.at(row).at(col);

    return grids;
}


