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

#ifndef QICSPRINTGRID_H
#define QICSPRINTGRID_H

#include "QicsGrid.h"

/*! \internal
* \class QicsPrintGrid QicsPrintGrid.h
* \brief Handles printing of grids
*
* QicsPrintGrid handles hardcopy layout and rendering of grid objects.
* A QicsPrintGrid object is created with an associated QicsGridInfo object
* that specifies all the relevant information about the grid.  A portion
* of the grid is drawn by calling #print().  The method #regionFromArea()
* can be useful in determining how many rows and columns can fit in a given
* area on the page.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QPalette;

class QICS_EXPORT QicsPrintGrid: public QicsGrid
{
public:
    /*!
    * Constructor for the class.
    * \param info used for style and dimension information, as well as
    *             the associated data model.
    */
    QicsPrintGrid(QicsGridInfo &info);
    /*!
    * Destructor for the class.
    */
    virtual ~QicsPrintGrid();

    /*!
    * Prints a region of the grid.
    * \param region the region of the grid to be printed
    * \param start the location on the page where the grid should be printed
    * \param painter painter bound to a printer device
    */
    QPoint print(const QicsRegion &region, const QPoint &start,
        QPainter *painter, int *w = 0, int *h = 0);

    /*!
    * Returns the row/column region that can be completely drawn inside of
    * a given area.
    * \param rect the area in which the grid should be printed
    * \param start the cell that should be displayed in the top, left corner of
    *              the grid
    * \return the row/column region that can be completely drawn
    */
    QicsRegion regionFromArea(const QRect &rect, const QicsICell &start) const;

    /*!
    * Returns the preferred size of the print grid.  The preferred size will
    * depend on the type of grid info object the print grid is using.  If
    * the grid info is type TableGrid, both dimensions will be unset (i.e.
    * the grid has no preferred size).  If the grid info is type RowHeaderGrid,
    * the height will be unset (it can be as large as necessary), but the
    * width will be set to the width of the row header.  If the grid info
    * is type ColumnHeaderGrid, the width will be unset (it can be as large
    * as necessary), but the height will be set to the height of the column
    *  header.
    * \return the preferred size of the grid
    */
    QSize preferredSize() const;

    virtual QicsRegion currentViewport() const;

protected:
    /*! \internal
    * Returns the width of the frame that is drawn around the perimeter
    * of the grid.
    */
    int frameWidth() const;

    /*! \internal
    * Draws the rame around the perimeter of the grid.
    * \param rect the area in which the frame should be drawn
    * \param fstyle the frame style
    * \param lw the frame's line width
    * \param mlw the frame's mid-line width
    * \param cg colorgroup used for drawing the frame
    * \param painter painter used for drawing the frame
    */
    void drawFrame(const QRect &rect, int fstyle, int lw, int mlw,
        const QPalette &pal, QPainter *painter);

    /*!
    * \internal
    */
    virtual const QRect boundRect() const { return myBoundRect; }

    QRect myBoundRect;
};

#endif //QICSPRINTGRID_H


