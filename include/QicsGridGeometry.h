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

#ifndef QICSGRIDGEOMETRY_H
#define QICSGRIDGEOMETRY_H

#include <QObject>
#include <QVector>
#include "QicsNamespace.h"

class QicsScreenGrid;

/*!
* \class QicsGridGeometry QicsGridGeometry.h
* \brief QicsGridGeometry holds information of all grids positions in QicsTable.
*/
class QICS_EXPORT QicsGridGeometry : public QObject
{
    Q_OBJECT
public:
    /*!
    * Enum of available header positions.
    * \arg \b Top - top header
    * \arg \b Bottom - bottom header
    * \arg \b Left - left header
    * \arg \b Right - right header
    */
    enum HeaderPosition {
        Top = 0,
        Bottom,
        Left,
        Right
    };

    /*!
    Enum of available table grid regions.

    Table below demonstrates how grids are placed in QicsTable layout:
    * \arg \b TopLeft - top left grid
    * \arg \b TopMiddle - top middle grid
    * \arg \b TopRight - top right grid
    * \arg \b MiddleLeft - middle left grid
    * \arg \b MiddleRight - middle right grid
    * \arg \b Main - main grid
    * \arg \b BottomLeft - bottom left grid
    * \arg \b BottomMiddle - top bottom middle
    * \arg \b BottomRight - bottom right

    \code
    frozen Left                     frozen Right
    -----------------------------------------------
    | TopLeft    |    TopMiddle    |  TopRight    |  frozen Top
    -----------------------------------------------
    | MiddleLeft |       Main      |  MiddleRight |
    -----------------------------------------------
    | BottomLeft |   BottomMiddle  |  BottomRight |  frozen Bottom
    -----------------------------------------------
    \endcode
    */
    enum TableRegion {
        TopLeft,
        TopMiddle,
        TopRight,
        MiddleLeft,
        Main,
        MiddleRight,
        BottomLeft,
        BottomMiddle,
        BottomRight
    };

    /*!
    * Default constructor
    */
    QicsGridGeometry(QObject* parent=0);

    /*!
    * Destructor
    */
    ~QicsGridGeometry();

    /*!
    * Return list of all grids that table contains.
    */
    QList<QicsScreenGrid*> allGrids();

    /*!
    * Returns grid left of \b grid.
    */
    QicsScreenGrid* gridLeftOf(QicsScreenGrid* grid);

    /*!
    * Returns grid right of \b grid.
    */
    QicsScreenGrid* gridRightOf(QicsScreenGrid* grid);

    /*!
    * Returns grid above \b grid.
    */
    QicsScreenGrid* gridAbove(QicsScreenGrid* grid);

    /*!
    * Returns grid below \b grid.
    */
    QicsScreenGrid* gridBelow(QicsScreenGrid* grid);

    /*!
    * Returns main table grid of QicsTable.
    */
    QicsScreenGrid* mainTableGrid();

    /*!
    * Sets the main table grid \b grid for QicsTable.
    */
    void setMainTableGrid(QicsScreenGrid* grid);

    /*!
    * Returns screen grid for the table region \b region.
    */
    QicsScreenGrid* tableGrid(TableRegion region);

    /*!
    * Sets screen grid \b grid for the table region \b region.
    */
    void setTableGrid(TableRegion region, QicsScreenGrid* grid);

    /*!
    * Returns header grid for the \b position header position and
    * \b section section.
    */
    QicsScreenGrid* headerGrid(HeaderPosition position, int section);

    /*!
    * Sets header grid \b grid for the \b position header position and
    * \b section section.
    */
    void setHeaderGrid(HeaderPosition position, int section, QicsScreenGrid* grid);

    /*!
    * Returns grid at \b row row and \b col column in grid geometry.
    */
    inline  QicsScreenGrid *gridAt(int row,int col) {return myGrids.at(row).at(col);}

signals:
    void gridAdded(QicsScreenGrid* grid);
    void gridRemoved(QicsScreenGrid* grid);

private slots:
    void handleGridDeleted(QObject* grid);

protected:
    /*!
    * Returns row coord of \b grid screen grid.
    */
    int gridRowCoord(QicsScreenGrid* grid);

    /*!
    * Returns column coord of \b grid screen grid.
    */
    int gridColCoord(QicsScreenGrid* grid);

private:
    QVector< QVector<QicsScreenGrid*> > myGrids;

    void connectGridSignals(QicsScreenGrid* grid);

    friend class QicsKeyboardManager;
};

#endif //QICSGRIDGEOMETRY_H


