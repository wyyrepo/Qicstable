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

#ifndef QICSDIMENSIONMANAGER_H
#define QICSDIMENSIONMANAGER_H

#include <QObject>
#include <QFont>
#include <QVector>
#include <QSet>
#include <QDomElement>
#include "QicsNamespace.h"
#include "QicsGridInfo.h"

/*!
* \internal
* \class QicsDimensionManager QicsDimensionManager.h
* \brief manager for row, column, and cell dimension information
*
* The dimension manager is responsible for calculating and providing
* row, column, and cell dimension information.  It is important that all
* grid widgets that are part of a single table use the same row height
* and column width information.  The dimension manager must be called
* whenever a row height or column width is changed.  In addition, it must
* be called whenever a font property or cell margin property is set, because
* these properties can affect row and column dimensions as well.
*
* The dimension manager stores settings for both model and visual
* coordinates.  In addition, the dimension manager stores settings for
* \b sets of repeating rows and columns (these sets are always specified
* in visual coordinates).  Visual settings for rows and columns take
* precedence over repeating row and column settings, and either of these
* settings take precedence over model row and column settings.
*
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsStyleManager;

class QICS_EXPORT QicsDimensionManager : public QObject, public Qics
{
    Q_OBJECT
public:

    /*!
    * Constructor for the class.  The grid info object is used to
    * get a style manager and to do model/visual index lookups.
    */
    QicsDimensionManager(QicsGridInfo *grid_info, QObject *parent = 0);

    QicsDimensionManager(const QicsDimensionManager &dm, QicsGridInfo *grid_info, QObject *parent = 0);

    /*!
    * Class destructor.
    */
    ~QicsDimensionManager();

    /*!
    * Sets a "controlling" dimension manager for row dimensions.
    * This allows for a dimension manager object to manage
    * column dimensions itself but pass all row dimension settings
    * and queries to a different dimension manager.  This is useful
    * in the case of row header objects, where row dimension information
    * must be retrieved from the main table grid, but column widths
    * within the row header are independent of the main table.
    */
    void setControllingRowDimensionManager(QicsDimensionManager *dm);
    /*!
    * Sets a "controlling" dimension manager for column dimensions.
    * This allows for a dimension manager object to manage
    * column dimensions itself but pass all column dimension settings
    * and queries to a different dimension manager.  This is useful
    * in the case of column header objects, where column dimension
    * information must be retrieved from the main table grid, but row
    * heights within the column header are independent of the main table.
    */
    void setControllingColumnDimensionManager(QicsDimensionManager *dm);

    /*!
    * Notifies the dimension manager that a new default font has
    * been set for the entire table..  Calling this method
    * will cause new dimensions to be calculated for all rows
    * and columns.
    */
    void setDefaultFont(const QFont &fnt);

    /*!
    * Notifies the dimension manager that a new font has been set
    * for row \a row for grid type \a grid_type.  If \a visual_coords
    * is \b true, this setting is for a visual row, else it is for a
    * model row.  Calling this method
    * may cause new dimensions to be calculated for this row and
    * for all columns.
    */
    void setRowFont(QicsGridType grid_type, int row, bool visual_coords,
        const QFont &fnt);
    /*!
    * Notifies the dimension manager that a previous font setting
    * for row \a row for grid type \a grid_type should be cancelled.
    * If \a visual_coords is \b true, this setting is for a visual row,
    * else it is for a model row.  Calling this method may cause new
    * dimensions to be calculated for this row and for all columns.
    */
    void unsetRowFont(QicsGridType grid_type, int row, bool visual_coords);

    /*!
    * Notifies the dimension manager that a new font (\a fnt ) has been set
    * for a set of repeating rows (beginning at row \a row and repeating
    * every \a interval rows).
    */
    void setRepeatingRowFont(QicsGridType grid_type,
        int row, unsigned int interval,
        const QFont &fnt);

    /*!
    * Notifies the dimension manager that a new font has been set
    * for column \a col for grid type \a grid_type.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    * Calling this method may cause new
    * dimensions to be calculated for this column and for all rows.
    */
    void setColumnFont(QicsGridType grid_type, int col, bool visual_coords,
        const QFont &fnt);
    /*!
    * Notifies the dimension manager that a previous font setting
    * for column \a col for grid type \a grid_type should be cancelled.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    * Calling this method may cause new dimensions to be calculated
    * for this column and for all rows.
    */
    void unsetColumnFont(QicsGridType grid_type, int col, bool visual_coords);

    /*!
    * Notifies the dimension manager that a new font (\a fnt ) has been set
    * for a set of repeating columns (beginning at column \a col and repeating
    * every \a interval columns).
    */
    void setRepeatingColumnFont(QicsGridType grid_type,
        int col, unsigned int interval,
        const QFont &fnt);

    /*!
    * Notifies the dimension manager that a new font has been set
    * for cell (\a row, \a col ) in for grid type \a grid_type.
    * If \a visual_coords is \b true, this setting is for a visual cell,
    * else it is for a model cell.
    * Calling this method may cause new dimensions to be calculated
    * for the cell's row and column.
    */
    void setCellFont(QicsGridType grid_type, int row, int col,
        bool visual_coords, const QFont &fnt);


    /*!
    * Notifies the dimension manager that a new font has been set
    * for region in for grid type \a grid_type.
    * If \a visual_coords is \b true, this setting is for a visual cell,
    * else it is for a model cell.
    * Calling this method may cause new dimensions to be calculated
    * for the cell's row and column.
    */
    void setRegionFont(QicsGridType grid_type, const QicsRegion &region,
        bool visual_coords, const QFont &fnt);

    void prepareRegionFont(QicsGridType grid_type, const QicsRegion &region,
        bool visual_coords, const QFont &fnt);

    /*!
    * Notifies the dimension manager that a previous font setting
    * for cell (\a row, \a col ) for grid type \a grid_type should be cancelled.
    * If \a visual_coords is \b true, this setting is for a visual cell,
    * else it is for a model cell.
    * Calling this method may cause new dimensions to be calculated
    * for the cell's row and column.
    */
    void unsetCellFont(QicsGridType grid_type, int row, int col,
        bool visual_coords);

    /*!
    * Sets the height of row \a row in pixels.
    * If \a visual_coords is \b true, this setting is for a visual row,
    * else it is for a model row.
    */
    void setRowHeightInPixels(int row, bool visual_coords, int height);
    /*!
    * Sets the height of row \a row to \a height
    * character units (i.e. lines)..
    * If \a visual_coords is \b true, this setting is for a visual row,
    * else it is for a model row.
    */
    void setRowHeightInChars(int row, bool visual_coords, int height);

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
    * Sets the default row height to \a height for the entire table.
    */
    void setDefaultRowHeightInPixels(int height);

    /*!
    * returns default height (in pixels)
    */
    int defaultRowHeightInPixels() const
    {
        if (myRowDM)
            return myRowDM->defaultRowHeightInPixels();

        return myOrigDefaultDimensions.heightInPixels();
    };
    /*!
    * Sets the default row height to \a height character units (i.e. lines)
    * for the entire table.
    */
    void setDefaultRowHeightInChars(int height);

    /*!
    * returns default height (in character unit)
    */
    int defaultRowHeightInChars() const
    {
        if (myRowDM)
            return myRowDM->defaultRowHeightInChars();

        return myOrigDefaultDimensions.heightInChars();
    };

    /*!
    *  Sets the width of column \a col in pixels.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    */
    void setColumnWidthInPixels(int col, bool visual_coords, int width);
    /*!
    *  Sets the width of column \a col to \a width character units.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    */
    void setColumnWidthInChars(int col, bool visual_coords, int width);

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
    * Sets the default column width to \a width for the entire table.
    */
    void setDefaultColumnWidthInPixels(int width);

    /*!
    * Returns default column width (in pixels)
    */
    int defaultColumnWidthInPixels() const
    {
        if (myColumnDM)
            return myColumnDM->defaultColumnWidthInPixels();

        return myOrigDefaultDimensions.widthInPixels();
    };

    /*!
    * Sets the default column width to \a width character units
    * for the entire table.
    */
    void setDefaultColumnWidthInChars(int width);

    /*!
    * returns default column width (in character unit)
    */
    int defaultColumnWidthInChars() const
    {
        if (myColumnDM)
            return myColumnDM->defaultColumnWidthInChars();

        return myOrigDefaultDimensions.widthInChars();
    };

    /*!
    * Returns the height of row \a row in pixels.
    */
    int rowHeight(int row,
        bool use_model_settings = true,
        bool use_visual_settings = true,
        bool force_calc = false) const;
    /*!
    * Returns the width of column \a col in pixels.
    */
    int columnWidth(int col,
        bool use_model_settings = true,
        bool use_visual_settings = true,
        bool force_calc = false) const;

    /*!
    *  Sets the minimum height of row \a row in pixels.
    * If \a visual_coords is \b true, this setting is for a visual row,
    * else it is for a model row.
    */
    void setRowMinHeightInPixels(int row, bool visual_coords, int height);
    /*!
    * Sets the minimum height of row \a row to \a height
    * character units (i.e. lines)..
    * If \a visual_coords is \b true, this setting is for a visual row,
    * else it is for a model row.
    */
    void setRowMinHeightInChars(int row, bool visual_coords, int height);

    /*!
    *  Sets the maximum height of row \a row in pixels.
    * If \a visual_coords is \b true, this setting is for a visual row,
    * else it is for a model row.
    */
    void setRowMaxHeightInPixels(int row, bool visual_coords, int height);
    /*!
    * Sets the maximum height of row \a row to \a height
    * character units (i.e. lines)..
    * If \a visual_coords is \b true, this setting is for a visual row,
    * else it is for a model row.
    */
    void setRowMaxHeightInChars(int row, bool visual_coords, int height);

    /*!
    *  Sets the minimum width of column \a col in pixels.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    */
    void setColumnMinWidthInPixels(int col, bool visual_coords, int width);
    /*!
    *  Sets the minimum width of column \a col to \a width character units.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    */
    void setColumnMinWidthInChars(int col, bool visual_coords, int width);

    /*!
    *  Sets the maximum width of column \a col in pixels.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    */
    void setColumnMaxWidthInPixels(int col, bool visual_coords, int width);

    /*!
    *  Sets the maximum width of column \a col to \a width character units.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    */
    void setColumnMaxWidthInChars(int col, bool visual_coords, int width);

    /*!
    * Returns the minimum height of row \a row in pixels.
    */
    int rowMinHeight(int row, bool use_model_settings = true,
        bool use_visual_settings = true) const;

    /*!
    * Returns the maximum height of row \a row in pixels.
    */
    int rowMaxHeight(int row, bool use_model_settings = true,
        bool use_visual_settings = true) const;

    /*!
    * Returns the minimum width of column \a col in pixels.
    */
    int columnMinWidth(int col, bool use_model_settings = true,
        bool use_visual_settings = true) const;

    /*!
    * Returns the maximum width of column \a col in pixels.
    */
    int columnMaxWidth(int col, bool use_model_settings = true,
        bool use_visual_settings = true) const;

    /*!
    * Sets the margin of all cells in the table to \a margin.
    */
    void setDefaultMargin(int margin);
    /*!
    * Sets the margin of all cells in row \a row to \a margin.
    * If \a visual_coords is \b true, this setting is for a visual row,
    * else it is for a model row.
    */
    void setRowMargin(QicsGridType grid_type, int row, bool visual_coords,
        int margin);
    /*!
    * Sets the margin of all cells in column \a col to \a margin.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    */
    void setColumnMargin(QicsGridType grid_type, int col, bool visual_coords,
        int margin);
    /*!
    * Sets the margin of cell (\a row, \a col) to \a margin.
    * If \a visual_coords is \b true, this setting is for a visual cell,
    * else it is for a model cell.
    */
    void setCellMargin(QicsGridType grid_type, int row, int col,
        bool visual_coords, int margin);

    /*!
    * Sets the margin of all cells in a set of repeating rows
    * (beginning at row \a row and repeating every \a interval rows)
    * to \a margin.
    */
    void setRepeatingRowMargin(QicsGridType grid_type,
        int row, unsigned int interval,
        int margin);
    /*!
    * Sets the margin of all cells in a set of repeating columns
    * (beginning at column \a col and repeating every \a interval columns)
    * to \a margin.
    */
    void setRepeatingColumnMargin(QicsGridType grid_type,
        int col, unsigned int interval,
        int margin);

    /*!
    * Sets the border width of all cells in the table to \a bw.
    */
    void setDefaultBorderWidth(int bw);
    /*!
    * Sets the borderWidth of all cells in row \a row to \a bw.
    * If \a visual_coords is \b true, this setting is for a visual row,
    * else it is for a model row.
    */
    void setRowBorderWidth(QicsGridType grid_type, int row,
        bool visual_coords, int bw);
    /*!
    * Sets the border width of all cells in column \a col to \a bw.
    * If \a visual_coords is \b true, this setting is for a visual column,
    * else it is for a model column.
    */
    void setColumnBorderWidth(QicsGridType grid_type, int col,
        bool visual_coords, int bw);
    /*!
    * Sets the border width of cell (\a row, \a col) to \a bw.
    * If \a visual_coords is \b true, this setting is for a visual cell,
    * else it is for a model cell.
    */
    void setCellBorderWidth(QicsGridType grid_type, int row, int col,
        bool visual_coords, int bw);

    /*!
    * Sets the border width of all cells in a set of repeating rows
    * (beginning at row \a row and repeating every \a interval rows)
    * to \a bw.
    */
    void setRepeatingRowBorderWidth(QicsGridType grid_type,
        int row, unsigned int interval,
        int bw);
    /*!
    * Sets the border width of all cells in a set of repeating columns
    * (beginning at column \a col and repeating every \a interval columns)
    * to \a bw.
    */
    void setRepeatingColumnBorderWidth(QicsGridType grid_type,
        int col, unsigned int interval,
        int bw);

    /*!
    * Returns whether this row is stretchable.
    */
    bool isRowStretchable(int row, bool visual_coords) const;

    /*!
    * Controls whether row \a row is stretchable.
    */
    void setRowStretchable(int row, bool visual_coords, bool set);

    /*!
    * Returns whether this column is stretchable.
    */
    bool isColumnStretchable(int col, bool visual_coords) const;

    /*!
    * Controls whether column \a col is stretchable.
    */
    void setColumnStretchable(int col, bool visual_coords, bool set);

    /*!
    * Attempt to stretch row heights to accomodate the extra (or lost)
    * space specified by \a space.
    */
    unsigned int stretchRows(int start_row, int end_row, int space,
        bool visual_coords);

    /*!
    * Attempt to stretch column widths to accomodate the extra (or lost)
    * space specified by \a space.
    */
    unsigned int stretchColumns(int start_col, int end_col, int space,
        bool visual_coords);

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

#ifdef notdef
    void dumpWidths() const;
    void dumpHeights() const ;
#endif

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

    bool isRowFiltered(int row) const;

    /*!
    * Returns \b true is \a col  is hidden, or \b false otherwise.
    */
    bool isColumnHidden(int col) const;

    inline void setCellWidthMode(QicsCellWidthMode mode) {myCellWidthMode=mode;}

    inline QicsCellWidthMode cellWidthMode() const { return myCellWidthMode; }

    /*!
    * Returns the current value of the emitsSignals flag.  If \b true,
    * dimension manager will emit signals when rows/columns visibility was changed.
    * If \b false, no signals will be emitted.
    * \since 2.4
    */
    inline bool emitSignals() const { return myEmitSignalsFlag; }

    /*!
    * Controls the output of dimension manager signals.  If \a b is \b true,
    * dimension manager will emit signals when rows/columns visibility was changed.
    * If \b false, no signals will be emitted.
    * \since 2.4
    */
    inline void setEmitSignals(bool b)  { myEmitSignalsFlag = b; }

public slots:
    /*!
    * \internal
    * Handles insertion of \a num rows.
    */
    void insertRows(int num, int start_position);
    /*!
    * \internal
    * Handles insertion of \a num columns.
    */
    void insertColumns(int num, int start_position);
    /*!
    * \internal
    * Handles deletion of \a num rows, starting at \a start_position.
    */
    void deleteRows(int num, int start_position);
    /*!
    * \internal
    * Handles deletion of \a num columns, starting at \a start_position.
    */
    void deleteColumns(int num, int start_position);
    /*!
    *  Store configuration to \a doc.
    */
    QDomElement toDomXml(QDomDocument* doc);
    /*!
    *  Reconfigure from QDomElement.
    */
    void configureFromDomXml(const QDomElement&);

signals:
    /*!
    * This signal is emitted whenever a dimension (row or column) has changed
    * in the dimension manager.
    */
    void dimensionChanged();

    /*!
    * This signal is emitted whenever a dimension (row or column) has changed
    * in the dimension manager. \a startIndex and \a endIndex are in visual coords.
    */
    void dimensionChanged(Qics::QicsIndexType indexType, int startIndex, int endIndex);

    /*!
    * This signal is emitted whenever a row dimension has changed
    * in the dimension manager.
    */
    void rowResized(int row,int old_height,int new_height);

    /*!
    * This signal is emitted whenever a column dimension has changed
    * in the dimension manager.
    */
    void columnResized(int column,int old_width,int new_width);

    /*!
    * This signal is emitted whenever a row header width has changed.
    * If header has several rows then old_width and
    * new_width are only valid for the top row.
    */
    void rowHeaderResized(int old_width,int new_width);

    /*!
    * This signal is emitted whenever a column header height has changed.
    * If header has several columns then old_height and
    * new_height are only valid for the left column.
    */
    void columnHeaderResized(int old_height,int new_height);

    /*!
    * This signal is emitted whenever a row visibility is changed.
    * @row parameter is row index.
    * @visible parameter holds whether the row is visible
    */
    void rowVisibilityChanged(int row, bool visible);

    /*!
    * This signal is emitted whenever a column visibility is changed.
    * @column parameter is column index.
    * @visible parameter holds whether the column is visible
    */
    void columnVisibilityChanged(int column, bool visible);

public:
    // Should be protected, but the Sun Workshop compiler complains
    enum QicsGridRole {
        QicsTableGridRole,
        QicsRowHeaderGridRole,
        QicsColumnHeaderGridRole
    };

    enum QicsDimensionMode {
        QicsDimensionUnset,
        QicsDimensionPixel,
        QicsDimensionChar
    };

protected:
    /////////////////////////////////////////////////////////////////////
    ////////////////  A bunch of helper structs/classes  ////////////////
    /////////////////////////////////////////////////////////////////////

    /*!
    * \internal
    * \class QicsCellSetting
    * Stores a font and/or cell margin setting to an individual cell.
    */
    class QicsCellSetting
    {
    public:
        // constructor
        QicsCellSetting();
        // is anything set?
        bool isEmpty() const;

        // role of this grid (table or header)
        QicsGridRole role;
        // row index
        int row;
        // column index
        int col;
        // height of the font that was set in this cell
        int font_height;
        // width of the font that was set in this cell
        int font_width;
        // cell margin that was set in this cell
        int cell_margin;
        // border width that was set in this cell
        int border_width;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);

        virtual ~QicsCellSetting() {};
    };
    // vector of QicsCellSetting objects
    typedef QVector<QicsCellSetting> QicsCellSettingV;
    // vector of pointers to QicsCellSetting objects
    typedef QVector<QicsCellSetting *> QicsCellSettingPV;

    /*!
    * \internal
    * Stores a font and/or cell margin setting to an entire row.
    */
    class QicsRowSetting
    {
    public:
        // constructor
        QicsRowSetting();
        // is anything set?
        bool isEmpty() const;

        // role of this grid (table or header)
        QicsGridRole role;
        // row index
        int row;
        // height of the font that was set for this row
        int font_height;
        // width of the font that was set for this row
        int font_width;
        // cell margin that was set for this row
        int cell_margin;
        // border width that was set for this row
        int border_width;
        // is this row stretchable
        bool stretchable;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);

        virtual ~QicsRowSetting() {};
    };
    // vector of QicsRowSetting objects
    typedef QVector<QicsRowSetting> QicsRowSettingV;
    // vector of pointers to QicsRowSetting objects
    typedef QVector<QicsRowSetting *> QicsRowSettingPV;

    /*!
    * \internal
    * \class QicsColumnSetting
    * Stores a font and/or cell margin setting to an entire column.
    */
    class QicsColumnSetting
    {
    public:
        // constructor
        QicsColumnSetting();
        // is anything set?
        bool isEmpty() const;

        // role of this grid (table or header)
        QicsGridRole role;
        // column index
        int col;
        // height of the font that was set for this column
        int font_height;
        // width of the font that was set for this column
        int font_width;
        // cell margin that was set for this column
        int cell_margin;
        // border width that was set for this column
        int border_width;
        // is this column stretchable
        bool stretchable;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);

        virtual ~QicsColumnSetting() {};
    };
    /*!
    * \internal
    * vector of QicsColumnSetting objects
    */
    typedef QVector<QicsColumnSetting> QicsColumnSettingV;
    /*!
    * \internal
    * vector of pointers to QicsColumnSetting objects
    */
    typedef QVector<QicsColumnSetting *> QicsColumnSettingPV;

    class QicsRepeatingRowSetting : public QicsRowSetting
    {
    public:
        QicsRepeatingRowSetting();

        // interval for this repeating row
        unsigned int interval;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);

        virtual ~QicsRepeatingRowSetting() {};
    };
    // vector of QicsRepeatingRowSetting objects
    typedef QVector<QicsRepeatingRowSetting> QicsRepeatingRowSettingV;
    // vector of pointers to QicsRepeatingRowSetting objects
    typedef QVector<QicsRepeatingRowSetting *> QicsRepeatingRowSettingPV;

    class QicsRepeatingColumnSetting : public QicsColumnSetting
    {
    public:
        QicsRepeatingColumnSetting();

        // interval for this repeating column
        unsigned int interval;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);
    };
    // vector of QicsRepeatingColumnSetting objects
    typedef QVector<QicsRepeatingColumnSetting> QicsRepeatingColumnSettingV;
    // vector of pointers to QicsRepeatingColumnSetting objects
    typedef QVector<QicsRepeatingColumnSetting *> QicsRepeatingColumnSettingPV;

    /*!
    * \internal
    * \class QicsDefaultDimensionSetting
    * Stores a default dimension setting for rows and columns that
    * do not have a specific dimension set.
    */
    class QicsDefaultDimensionSetting {
    public:
        /*!
        * the constructor
        */
        QicsDefaultDimensionSetting();
        /*!
        * copy constructor
        */
        QicsDefaultDimensionSetting(const QicsDefaultDimensionSetting &);
        /*!
        * sets font_height and font_width to the dimensions of \a fnt.
        */
        void setFont(const QFont &fnt);

        /*!
        * sets default height (in pixels) to \a height.
        */
        void setHeightInPixels(int height);

        /*!
        * returns default height (in pixels)
        */
        int heightInPixels() const { return height;}

        /*!
        * sets default height (in character units) to \a height.
        */
        void setHeightInChars(int height);

        /*!
        * returns default height (in character units)
        */
        int heightInChars() const { return height_chars;}

        /*!
        * sets default width (in pixels) to \a width.
        */
        void setWidthInPixels(int width);

        /*!
        * returns default width (in pixels)
        */
        int widthInPixels() const { return width;}

        /*!
        * sets default width (in character units) to \a width.
        */
        void setWidthInChars(int width);

        /*!
        * returns default width (in character units)
        */
        int widthInChars() const { return width_chars; }

        /*!
        * computes the default height and width in characters based on
        * the current default font dimensions
        */
        void compute();

        // the default dimension mode
        QicsDimensionMode mode;
        // default height
        int height;
        // default width
        int width;
        // default height in character units
        int height_chars;
        // default width in character units
        int width_chars;
        // height of default font
        int font_height;
        // height of default font
        int font_width;
        // default cell_margin
        int cell_margin;
        // default border width
        int border_width;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);

        virtual ~QicsDefaultDimensionSetting() {};
    };

    /*!
    * \internal
    * \class QicsRowHeight
    * Stores the current height of a row
    */
    class QicsRowHeight {
    public:
        /*!
        * the constructor
        */
        QicsRowHeight();
        /*!
        * copy constructor
        */
        QicsRowHeight(const QicsRowHeight &rh);

        // the mode of this height setting
        QicsDimensionMode mode;
        // height of this row in pixels
        int height;
        // height of this row in character units
        int chars;
        // is this height always valid, or does it need to be recomputed
        int valid;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);

        virtual ~QicsRowHeight() {};
    };
    // vector of QicsRowHeight objects
    typedef QVector<QicsRowHeight> QicsRowHeightV;
    // vector of pointers to QicsRowHeight objects
    typedef QVector<QicsRowHeight *> QicsRowHeightPV;

    /*!
    * \internal
    * \class QicsColumnWidth
    * Stores the current width of a column
    */
    class QicsColumnWidth {
    public:
        /*!
        * the constructor
        */
        QicsColumnWidth();
        /*!
        * copy constructor
        */
        QicsColumnWidth(const QicsColumnWidth &cw);

        // the mode of this width setting
        QicsDimensionMode mode;
        // width of this column in pixels
        int width;
        // width of this column in character units
        int chars;
        // is this height always valid, or does it need to be recomputed
        int valid;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);

        virtual ~QicsColumnWidth() {};
    };
    /*!
    * \internal
    * vector of QicsColumnWidth objects
    */
    typedef QVector<QicsColumnWidth> QicsColumnWidthV;
    /*!
    * \internal
    * vector of pointers to QicsColumnWidth objects
    */
    typedef QVector<QicsColumnWidth *> QicsColumnWidthPV;

    class QicsRepeatingRowHeight : public QicsRowHeight
    {
    public:
        QicsRepeatingRowHeight();

        /*!
        * copy constructor
        */
        QicsRepeatingRowHeight(const QicsRepeatingRowHeight &rh);

        // starting row
        int row;
        // interval for this repeating row
        unsigned int interval;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);
    };
    // vector of QicsRepeatingRowHeight objects
    typedef QVector<QicsRepeatingRowHeight> QicsRepeatingRowHeightV;
    // vector of pointers to QicsRepeatingRowHeight objects
    typedef QVector<QicsRepeatingRowHeight *> QicsRepeatingRowHeightPV;

    class QicsRepeatingColumnWidth : public QicsColumnWidth
    {
    public:
        QicsRepeatingColumnWidth();

        /*!
        * copy constructor
        */
        QicsRepeatingColumnWidth(const QicsRepeatingColumnWidth &cw);

        // starting column
        int col;
        // interval for this repeating column
        unsigned int interval;

        virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        virtual void configureFromDomXml(const QDomElement&);

        virtual ~QicsRepeatingColumnWidth() {};
    };
    // vector of QicsRepeatingColumnWidth objects
    typedef QVector<QicsRepeatingColumnWidth> QicsRepeatingColumnWidthV;
    // vector of pointers to QicsRepeatingColumnWidth objects
    typedef QVector<QicsRepeatingColumnWidth *> QicsRepeatingColumnWidthPV;

    class QicsOverrideSetting
    {
    public:
        inline QicsOverrideSetting()
        { myIndex = -1; myVal = -1; }

        inline QicsOverrideSetting(int idx, int val)
        { myIndex = idx; myVal = val; }

        inline QicsOverrideSetting(const QicsOverrideSetting &os)
        { myIndex = os.myIndex; myVal = os.myVal; }

        inline int index() const
        { return myIndex; }

        inline void setIndex(int idx)
        { myIndex = idx; }

        inline int value() const
        { return myVal; }

        inline void setValue(int val)
        { myVal = val; }

        // XXX -- Do we need to save these?
        //virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;
        //virtual void configureFromDomXml(const QDomElement&);

    protected:
        int myIndex;
        int myVal;
    };
    typedef QList<QicsOverrideSetting> QicsOverrideSettingL;

    ////////////////////////////////////////////////////////////////////////

    /*!
    * \internal
    * Returns the role of \a grid.
    */
    QicsGridRole gridRole(QicsGridType grid_type) const;

    /*!
    * \internal
    * Computes and returns the height of a row.
    * \param row model row index
    * \param row_heights vector of model row heights
    * \param visual_row visual row index
    * \param vrow_heights vector of visual row heights
    * \param default_dims default dimension settings
    */
    int computeRowHeight(int row, QicsRowHeightPV &row_heights,
        int visual_row, QicsRowHeightPV &vrow_heights,
        QicsDefaultDimensionSetting &default_dims);

    /*!
    * \internal
    * Computes the height of a row, given a set of parameters.
    * \param nlines number of lines in the row
    * \param font_height height of the font in the row
    * \param cell_margin cell margin in the row
    * \param border_width border width of the row
    */
    int computeRowHeight(int nlines, int font_height,
        int cell_margin, int border_width);

    /*!
    * \internal
    * Computes and returns the width of a column.
    * \param col model column index
    * \param col_widths vector of model column widths
    * \param visual_col visual column index
    * \param vcol_widths vector of visual column widths
    * \param default_dims default dimension settings
    */
    int computeColumnWidth(int col, QicsColumnWidthPV &col_widths,
        int visual_col, QicsColumnWidthPV &vcol_widths,
        QicsDefaultDimensionSetting &default_dims);

    /*!
    * \internal
    * Computes the width of a column, given a set of parameters.
    * \param nlines number of characters in the column
    * \param font_width width of the font in the column
    * \param cell_margin cell margin in the column
    * \param border_width border width of the column
    */
    int computeColumnWidth(int nchars, int font_width,
        int cell_margin, int border_width);


    /*!
    * \internal
    * Computes a new default row font height based on the original default
    * font height, as well as any column-wide font settings that have
    * been made.  The new value is stored internally.
    * If the new computed default row font height
    * is different from what was previously set, returns \b true, else
    * returns \b false.
    */
    bool computeDefaultRowFontHeight();
    /*!
    * \internal
    * Computes a new default column font width based on the original default
    * font width, as well as any row-wide font settings that have
    * been made.  The new value is stored internally.
    * If the new computed default column font width
    * is different from what was previously set, returns \b true, else
    * returns \b false.
    */
    bool computeDefaultColumnFontWidth();

    /*!
    * \internal
    * Computes the height of all rows in the dimension manager, and
    * stores the values internally.  If any row heights changed,
    * returns \b true, else returns \b false.
    */
    bool computeAllRowHeights();
    /*!
    * \internal
    * Computes the height of all rows in the dimension manager, and
    * stores the values internally.  If any column widths changed,
    * returns \b true, else returns \b false.
    */
    bool computeAllColumnWidths();

    /*!
    * Invalidate any saved row height setting for model row index \a row.
    * The next call to #rowHeight for this row will force a computation
    * of the row's height.
    */
    void invalidateRowHeight(int row);
    /*!
    * Invalidate any saved column width setting for model column index \a col.
    * The next call to #columnWidth for this column will force a computation
    * of the column's width.
    */
    void invalidateColumnWidth(int col);

    /*!
    * \internal
    * Returns the grid info object for this manager.
    */
    inline QicsGridInfo *gridInfo() const { return myGridInfo; }

    inline QicsStyleManager *styleManager() const { return myGridInfo->styleManager(); }

    // XML routines
    QDomElement saveMySetCellsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMySetRowsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMySetColumnsToDomXml(const QString& tag, QDomDocument* doc);

    QDomElement saveMyRowHeightsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyColumnWidthsToDomXml(const QString& tag, QDomDocument* doc);

    QDomElement saveMyMinRowHeightsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyMaxRowHeightsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyMinColumnWidthsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyMaxColumnWidthsToDomXml(const QString& tag, QDomDocument* doc);

    QDomElement saveMyHiddenRowsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyHiddenColumnsToDomXml(const QString& tag, QDomDocument* doc);

    QDomElement saveMySetVisualCellsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMySetVisualRowsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMySetVisualColumnsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMySetRepeatingRowsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMySetRepeatingColumnsToDomXml(const QString& tag, QDomDocument* doc);

    QDomElement saveMyVisualRowHeightsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyVisualColumnWidthsToDomXml(const QString& tag, QDomDocument* doc);

    QDomElement saveMyVisualRowMinHeightsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyVisualRowMaxHeightsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyVisualColumnMinWidthsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyVisualColumnMaxWidthsToDomXml(const QString& tag, QDomDocument* doc);

    QDomElement saveMyRepeatingRowHeightsToDomXml(const QString& tag, QDomDocument* doc);
    QDomElement saveMyRepeatingColumnWidthsToDomXml(const QString& tag, QDomDocument* doc);

    void loadMySetCellsFromDomXml(const QDomElement& e);
    void loadMySetRowsFromDomXml(const QDomElement& e);
    void loadMySetColumnsFromDomXml(const QDomElement& e);

    void loadMyRowHeightsFromDomXml(const QDomElement& e);
    void loadMyColumnWidthsFromDomXml(const QDomElement& e);

    void loadMyMinRowHeightsFromDomXml(const QDomElement& e);
    void loadMyMaxRowHeightsFromDomXml(const QDomElement& e);

    void loadMyMinColumnWidthsFromDomXml(const QDomElement& e);
    void loadMyMaxColumnWidthsFromDomXml(const QDomElement& e);

    void loadMyHiddenRowsFromDomXml(const QDomElement& e);
    void loadMyHiddenColumnsFromDomXml(const QDomElement& e);

    void loadMySetVisualCellsFromDomXml(const QDomElement& e);
    void loadMySetVisualRowsFromDomXml(const QDomElement& e);
    void loadMySetVisualColumnsFromDomXml(const QDomElement& e);
    void loadMySetRepeatingRowsFromDomXml(const QDomElement& e);
    void loadMySetRepeatingColumnsFromDomXml(const QDomElement& e);

    void loadMyVisualRowHeightsFromDomXml(const QDomElement& e);
    void loadMyVisualColumnWidthsFromDomXml(const QDomElement& e);

    void loadMyVisualRowMinHeightsFromDomXml(const QDomElement& e);
    void loadMyVisualRowMaxHeightsFromDomXml(const QDomElement& e);
    void loadMyVisualColumnMinWidthsFromDomXml(const QDomElement& e);
    void loadMyVisualColumnMaxWidthsFromDomXml(const QDomElement& e);

    void loadMyRepeatingRowHeightsFromDomXml(const QDomElement& e);
    void loadMyRepeatingColumnWidthsFromDomXml(const QDomElement& e);

    /*!
    * \internal
    * the controlling dimension manager for row dimensions
    */
    QicsDimensionManager *myRowDM;
    /*!
    * \internal
    * the controlling dimension manager for column dimensions
    */
    QicsDimensionManager *myColumnDM;

    /*!
    * \internal
    * list of cells that have had dimension properties set on them.
    */
    QicsCellSettingV mySetCells;
    /*!
    * \internal
    * list of rows that have had dimension properties set on them.
    */
    QicsRowSettingV mySetRows;
    /*!
    * \internal
    * list of columns that have had dimension properties set on them.
    */
    QicsColumnSettingV mySetColumns;

    /*!
    * \internal
    * list of row heights that have been computed.
    */
    QicsRowHeightPV myRowHeights;
    /*!
    * \internal
    * list of column widths that have been computed.
    */
    QicsColumnWidthPV myColumnWidths;

    /*!
    * \internal
    * list of minimum row heights that have been computed.
    */
    QicsRowHeightPV myRowMinHeights;
    /*!
    * \internal
    * list of maximum row heights that have been computed.
    */
    QicsRowHeightPV myRowMaxHeights;

    /*!
    * \internal
    * list of minimum column widths that have been computed.
    */
    QicsColumnWidthPV myColumnMinWidths;

    /*!
    * \internal
    * list of maximum column widths that have been computed.
    */
    QicsColumnWidthPV myColumnMaxWidths;

    /*!
    * \internal
    * original default dimension settings
    */
    QicsDefaultDimensionSetting myOrigDefaultDimensions;
    /*!
    * \internal
    * current default dimension settings
    */
    QicsDefaultDimensionSetting myCurrentDefaultDimensions;

    /*!
    * \internal
    * original default minimum dimension settings
    */
    QicsDefaultDimensionSetting myOrigDefaultMinDimensions;

    /*!
    * original default maximum dimension settings
    */
    QicsDefaultDimensionSetting myOrigDefaultMaxDimensions;

    /*!
    * \internal
    * current default minimum dimension settings
    */
    QicsDefaultDimensionSetting myCurrentDefaultMinDimensions;

    /*!
    * \internal
    * current default maximum dimension settings
    */
    QicsDefaultDimensionSetting myCurrentDefaultMaxDimensions;


    QicsOverrideSettingL myRowOverrides;
    QicsOverrideSettingL myColumnOverrides;

    QSet<int> myHiddenRows;
    QSet<int> myHiddenColumns;

    /*!
    * \internal
    * list of visual cells that have had dimension properties set on them.
    */
    QicsCellSettingV mySetVisualCells;
    /*!
    * \internal
    * list of visual rows that have had dimension properties set on them.
    */
    QicsRowSettingV mySetVisualRows;
    /*!
    * \internal
    * list of visual columns that have had dimension properties set on them.
    */
    QicsColumnSettingV mySetVisualColumns;
    /*!
    * \internal
    * list of repeating rows that have had dimension properties set on them.
    */
    QicsRepeatingRowSettingV mySetRepeatingRows;
    /*!
    * \internal
    * list of repeating columns that have had dimension properties set on them.
    */
    QicsRepeatingColumnSettingV mySetRepeatingColumns;

    /*!
    * \internal
    * list of visual row heights that have been computed.
    */
    QicsRowHeightPV myVisualRowHeights;
    /*!
    * \internal
    * list of visual column widths that have been computed.
    */
    QicsColumnWidthPV myVisualColumnWidths;

    /*!
    * \internal
    * list of minimum visual row heights that have been computed.
    */
    QicsRowHeightPV myVisualRowMinHeights;
    /*!
    * \internal
    * list of minimum visual row heights that have been computed.
    */
    QicsRowHeightPV myVisualRowMaxHeights;

    /*!
    * \internal
    * list of minimum visual column widths that have been computed.
    */
    QicsColumnWidthPV myVisualColumnMinWidths;

    /*!
    * \internal
    * list of maximum visual column widths that have been computed.
    */
    QicsColumnWidthPV myVisualColumnMaxWidths;

    /*!
    * \internal
    * list of repeating row height settings
    */
    QicsRepeatingRowHeightV myRepeatingRowHeights;
    /*!
    * \internal
    * list of repeating column width settings
    */
    QicsRepeatingColumnWidthV myRepeatingColumnWidths;

    /*!
    * \internal
    * grid info object -- used for model/visual index lookups
    */
    QicsGridInfo *myGridInfo;

    /*!
    * \internal
    * last stretched row index
    */
    int myLastStretchedRow;
    /*!
    * \internal
    * last stretched column index
    */
    int myLastStretchedColumn;

    /*!
    * \internal
    * allow resize when override is in effect
    */
    bool _allow_override_changes;

    QicsCellWidthMode myCellWidthMode;

    /*!
    * \internal
    * Do we emit signals when things change?
    */
    bool myEmitSignalsFlag;

    QVector< QMap<int, int> > myFontSizeVector;

    friend class QicsDimensionManager::QicsRowHeight;
    friend class QicsDimensionManager::QicsColumnWidth;
    friend class QicsDimensionManager::QicsDefaultDimensionSetting;
    friend class QicsStyleManager;

private:
    void dupSetCells(const QicsCellSettingV &from, QicsCellSettingV &to);
    void dupSetRows(const QicsRowSettingV &from, QicsRowSettingV &to);
    void dupSetColumns(const QicsColumnSettingV &from, QicsColumnSettingV &to);
    void dupRowHeights(const QicsRowHeightPV &from, QicsRowHeightPV &to);
    void dupColumnWidths(const QicsColumnWidthPV &from, QicsColumnWidthPV &to);

    void delRowHeights(QicsRowHeightPV &vals);
    void delColumnWidths(QicsColumnWidthPV &vals);

    void setRowFont(QicsRowSettingV &settings, QicsGridType grid_type,
        int row, const QFont &fnt);
    void unsetRowFont(QicsRowSettingV &settings, QicsGridType grid_type,
        int row);

    void setColumnFont(QicsColumnSettingV &settings, QicsGridType grid_type,
        int col, const QFont &fnt);
    void unsetColumnFont(QicsColumnSettingV &settings, QicsGridType grid_type,
        int col);

    void setCellFont(QicsCellSettingV &settings, QicsGridType grid_type,
        int row, int col, const QFont &fnt);
    void unsetCellFont(QicsCellSettingV &settings, QicsGridType grid_type,
        int row, int col);

    void setRowHeightFromContent(QicsGridType grid_type, int row, bool visual_coords, int content_height);
};

#include "QicsMappedDimensionManager.h"

#endif //QICSDIMENSIONMANAGER_H


