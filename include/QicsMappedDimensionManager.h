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

#ifndef QICSMAPPEDDIMENSIONMANAGER_H
#define QICSMAPPEDDIMENSIONMANAGER_H

#include "QicsNamespace.h"


class QicsRegion;
class QicsGrid;
class QicsGridInfo;
class QicsDimensionManager;
class QFont;

///////////////////////////////////////////////////////////////////////////
/*!
* \class QicsMappedDimensionManager QicsMappedDimensionManager.h
* \brief internal manager for row, column, and cell dimension information
*
* QicsMappedDimensionManager is exists as an aid to the QicsGridInfo to
* automatically provide the transform from grid coordinates to model
* coordinates through the grid's sorting vectors.  All calls to
* its members should be in visual \a grid coordinates.
*
* Internally, it only holds pointers to the underlying DimensionManager
* which the GridInfo really uses, and the GridInfo.  Most calls simply
* do the transform and forward them.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsMappedDimensionManager
{
public:
    /*!
    * Constructor for the class.  The style manager \a sm is used
    * to determine the default font for the table.
    */
    QicsMappedDimensionManager(QicsDimensionManager *, QicsGridInfo *);

    /*!
    * Class destructor.
    */
    ~QicsMappedDimensionManager();

    /*!
    * Notifies the dimension manager that a new default font has
    * been set for the entire table..  Calling this method
    * will cause new dimensions to be calculated for all rows
    * and columns.
    */
    void setDefaultFont(const QFont &fnt);

    /*!
    * Notifies the dimension manager that a new font has been set
    * for row \a row for grid type \a grid_type.  Calling this method
    * may cause new dimensions to be calculated for this column and
    * for all rows.
    */
    void setRowFont(Qics::QicsGridType grid_type, int row, const QFont &fnt);
    /*!
    * Notifies the dimension manager that a previous font setting
    * for row \a row for grid type \a grid_type should be cancelled.
    * Calling this method may cause new dimensions to be calculated
    * for this row and for all columns.
    */
    void unsetRowFont(Qics::QicsGridType grid_type, int row);

    /*!
    * Notifies the dimension manager that a new font (\a fnt ) has been set
    * for a set of repeating rows (beginning at row \a row and repeating
    * every \a interval rows).
    */
    void setRepeatingRowFont(Qics::QicsGridType grid_type,
        int row, unsigned int interval,
        const QFont &fnt);

    /*!
    * Notifies the dimension manager that a new font has been set
    * for column \a col for grid type \a grid_type.  Calling this method
    * may cause new dimensions to be calculated for this column and
    * for all rows.
    */
    void setColumnFont(Qics::QicsGridType grid_type, int col, const QFont &fnt);
    /*!
    * Notifies the dimension manager that a previous font setting
    * for column \a col for grid type \a grid_type should be cancelled.
    * Calling this method may cause new dimensions to be calculated
    * for this column and for all rows.
    */
    void unsetColumnFont(Qics::QicsGridType grid_type, int col);

    /*!
    * Notifies the dimension manager that a new font (\a fnt ) has been set
    * for a set of repeating columns (beginning at column \a col and repeating
    * every \a interval columns).
    */
    void setRepeatingColumnFont(Qics::QicsGridType grid_type,
        int col, unsigned int interval,
        const QFont &fnt);

    /*!
    * Notifies the dimension manager that a new font has been set
    * for cell (\a row, \a col ) for grid type \a grid_type.
    * Calling this method may cause new dimensions to be calculated
    * for the cell's row and column.
    */
    void setCellFont(Qics::QicsGridType grid_type, int row, int col,
        const QFont &fnt);
    /*!
    * Notifies the dimension manager that a previous font setting
    * for cell (\a row, \a col ) for grid type \a grid_type should be cancelled.
    * Calling this method may cause new dimensions to be calculated
    * for the cell's row and column.
    */
    void unsetCellFont(Qics::QicsGridType grid_type, int row, int col);

    /*!
    *  Sets the height of row \a row in pixels.
    */
    void setRowHeightInPixels(int row, int height);
    /*!
    *  Sets the height of row \a row to \a height
    * character units (i.e. lines)..
    */
    void setRowHeightInChars(int row, int height);

    /*!
    * Sets the height of a set of repeating rows
    * (beginning at row \a row and repeating every \a interval rows)
    * in pixels.
    */
    void setRepeatingRowHeightInPixels(int row, unsigned int interval,
        int height);

    /*!
    * Sets the height of a set of repeating rows
    * (beginning at row \a row and repeating every \a interval rows)
    * in character units.
    */
    void setRepeatingRowHeightInChars(int row, unsigned int interval,
        int height);

    /*!
    *  Sets the width of column \a col in pixels.
    */
    void setColumnWidthInPixels(int col, int width);
    /*!
    *  Sets the width of column \a col to \a width character units.
    */
    void setColumnWidthInChars(int col, int width);

    /*!
    * Sets the width of a set of repeating columns
    * (beginning at column \a col and repeating every \a interval columns)
    * in pixels.
    */
    void setRepeatingColumnWidthInPixels(int col, unsigned int interval,
        int width);
    /*!
    * Sets the width of a set of repeating columns
    * (beginning at column \a col and repeating every \a interval columns)
    * in character units.
    */
    void setRepeatingColumnWidthInChars(int col, unsigned int interval,
        int width);

    /*!
    * Returns the height of row \a row in pixels.
    */
    int rowHeight(int row) const;
    /*!
    * Returns the width of column \a col in pixels.
    */
    int columnWidth(int col) const;

    /*!
    *  Sets the minimum height of row \a row in pixels.
    */
    void setRowMinHeightInPixels(int row, int height);
    /*!
    *  Sets the minimum height of row \a row to \a height
    * character units (i.e. lines)..
    */
    void setRowMinHeightInChars(int row, int height);

    /*!
    *  Sets the minimum width of column \a col in pixels.
    */
    void setColumnMinWidthInPixels(int col, int width);
    /*!
    *  Sets the minimum width of column \a col to \a width character units.
    */
    void setColumnMinWidthInChars(int col, int width);

    /*!
    * Returns the minimum height of row \a row in pixels.
    */
    int rowMinHeight(int row) const;

    /*!
    * Returns the maximum height of row \a row in pixels.
    */
    int rowMaxHeight(int row) const;

    /*!
    * Returns the minimum width of column \a col in pixels.
    */
    int columnMinWidth(int col) const;

    /*!
    * Returns the maximum width of column \a col in pixels.
    */
    int columnMaxWidth(int row) const;

    /*!
    * Sets the margin of all cells in the table to \a margin.
    */
    void setDefaultMargin(int margin);
    /*!
    * Sets the margin of all cells in row \a row to \a margin.
    */
    void setRowMargin(Qics::QicsGridType grid_type, int row, int margin);
    /*!
    * Sets the margin of all cells in column \a col to \a margin.
    */
    void setColumnMargin(Qics::QicsGridType grid_type, int col, int margin);
    /*!
    * Sets the margin of cell (\a row, \a col) to \a margin.
    */
    void setCellMargin(Qics::QicsGridType grid_type, int row, int col,
        int margin);

    /*!
    * Sets the margin of all cells in a set of repeating rows
    * (beginning at row \a row and repeating every \a interval rows)
    * to \a margin.
    */
    void setRepeatingRowMargin(Qics::QicsGridType grid_type,
        int row, unsigned int interval,
        int margin);

    /*!
    * Sets the margin of all cells in a set of repeating columns
    * (beginning at column \a col and repeating every \a interval columns)
    * to \a margin.
    */
    void setRepeatingColumnMargin(Qics::QicsGridType grid_type,
        int col, unsigned int interval,
        int margin);

    /*!
    * Sets the border width of all cells in the table to \a bw.
    */
    void setDefaultBorderWidth(int bw);

    /*!
    * Sets the borderWidth of all cells in row \a row to \a bw.
    */
    void setRowBorderWidth(Qics::QicsGridType grid_type, int row, int bw);
    /*!
    * Sets the border width of all cells in column \a col to \a bw.
    */
    void setColumnBorderWidth(Qics::QicsGridType grid_type, int col, int bw);
    /*!
    * Sets the border width of cell (\a row, \a col) to \a bw.
    */
    void setCellBorderWidth(Qics::QicsGridType grid_type, int row, int col,
        int bw);

    /*!
    * Sets the border width of all cells in a set of repeating rows
    * (beginning at row \a row and repeating every \a interval rows)
    * to \a bw.
    */
    void setRepeatingRowBorderWidth(Qics::QicsGridType grid_type,
        int row, unsigned int interval,
        int bw);
    /*!
    * Sets the border width of all cells in a set of repeating columns
    * (beginning at column \a col and repeating every \a interval columns)
    * to \a bw.
    */
    void setRepeatingColumnBorderWidth(Qics::QicsGridType grid_type,
        int col, unsigned int interval,
        int bw);

    /*!
    * Returns whether this row is stretchable.
    */
    bool isRowStretchable(int row) const;

    /*!
    * Controls whether row \a row is stretchable.
    */
    void setRowStretchable(int row, bool set);

    /*!
    * Returns whether this column is stretchable.
    */
    bool isColumnStretchable(int col) const;

    /*!
    * Controls whether column \a col is stretchable.
    */
    void setColumnStretchable(int col, bool set);

    /*!
    * Attempt to stretch row heights to accomodate the extra (or lost)
    * space specified by \a space.
    */
    void stretchRows(int start_row, int end_row, int space);

    /*!
    * Attempt to stretch column widths to accomodate the extra (or lost)
    * space specified by \a space.
    */
    void stretchColumns(int start_col, int end_col, int space);

    /*!
    * Computes and returns the height of the table region defined
    * by \a region.
    */
    int regionHeight(const QicsRegion &region) const;
    /*!
    * Computes and returns the width of the table region defined
    * by \a region.
    */
    int regionWidth(const QicsRegion &region) const;

    /*!
    * Temporarily override the height of row \a row to \height pixels.
    * The height of the row can be reset by calling #resetRowHeight().
    */
    void overrideRowHeight(int row, int height);

    /*!
    * Temporarily override the width of column \a col to \width pixels.
    * The width of the column can be reset by calling #resetColumnWidth().
    */
    void overrideColumnWidth(int col, int width);

    /*!
    * Reset the height of row \a row from its temporary, overridden value
    * to its normal height.
    */
    void resetRowHeight(int row);

    /*!
    * Reset the width of column \a col from its temporary, overridden value
    * to its normal width.
    */
    void resetColumnWidth(int col);

    /*!
    * Returns \b true if the height of row \a row has been temporarily
    * overridden, or \b false otherwise.
    */
    int overriddenRowHeight(int row) const;

    /*!
    * Returns \b true if the width of column \a col has been temporarily
    * overridden, or \b false otherwise.
    */
    int overriddenColumnWidth(int col) const;

    /*!
    * Marks \a row as hidden.
    */
    void hideRow(int row);

    bool hasHiddenRows() const;

    /*!
    * Marks \a col as hidden.
    */
    void hideColumn(int col);

    bool hasHiddenColumns() const;

    /*!
    * Marks \a row as shown.
    */
    void showRow(int row);

    /*!
    * Marks \a col as shown.
    */
    void showColumn(int col);

    /*!
    * Returns \b true is \a row is hidden, or \b false otherwise.
    */
    bool isRowHidden(int row) const;

    /*!
    * Returns \b true is \a col  is hidden, or \b false otherwise.
    */
    bool isColumnHidden(int col) const;

    inline QicsDimensionManager *dimensionManager() const {return myDM;}

private:
    QicsDimensionManager *myDM;
    QicsGridInfo *myInfo;
};

#endif //QICSMAPPEDDIMENSIONMANAGER_H


