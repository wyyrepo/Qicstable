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

#ifndef QICSSTYLEMANAGER_H
#define QICSSTYLEMANAGER_H

#include <QObject>
#include <QDomElement>
#include "QicsNamespace.h"
#include "QicsCellStyle.h"
#include "QicsRepeatingCellStyle.h"
#include "QicsGridStyle.h"
#include "QicsRegion.h"
#include "QicsSpan.h"
#include "QicsSpanManager.h"

class QicsTable;
class QicsSpanManager;
class QicsGridInfo;
class QicsAbstractAttributeController;

/*! \internal
* \class QicsStyleManager QicsStyleManager.h
* \brief QicsStyleManager maintains an internal grid of styles for
* rows, columns, and individual cells.
*
* The style manager handles all aspects of table properties.  It contains
* a default style for the entire table, and some number of styles that have
* been set by the programmer for rows,  columns,
* and individual cells.  A style (see QicsCellStyle) is a collection of property
* settings.  These properties are both set and retrieved through the API of
* the style manager.
*
* New in version 1.1, we now have visual styles.  This means that the programmer can
* set properties on cells, rows, and columns that stay on those visual
* coordinates, even if the table's model is reordered.  All of the set*Property
* methods now take a visual_coords argument that specifies if the setting
* is in visual coordinates.  All visual coordinate settings
* (cell, row, or column), are stored in separate vectors from the original,
* model settings.  When requesting property information, if visual coordinates
* are specified IN ADDITION to the required model coordinates, visual
* settings will also be used in determining the correct property value.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsStyleManager : public QObject
{
    Q_OBJECT
public:
    /*! \internal
    * Constructor for the class.  The widget \a template_widget is used
    * to set some initial default values, possibly based on the \a type
    * of the style manager.
    */
    QicsStyleManager(Qics::QicsGridType type, QicsGridInfo *grid_info, QObject *parent = 0);

    /*!
    * Constructor for a style manager which "shadows" a base style manager.
    * The new style manager begins empty, without a default table style.
    * If a call to #getCellProperty, #getRowProperty, #getColumnProperty,
    * or #getDefaultProperty would result in no value being returned,
    * the "shadowed" base style manager is used to retrieve the appropriate
    * value for the requested property.
    */
    QicsStyleManager(QicsStyleManager *base_sm, QicsGridInfo *grid_info, QObject *parent = 0);

    /*! \internal
    * Destructor.
    */
    ~QicsStyleManager();

    inline Qics::QicsGridType type() const { return myType; }

    /*! \internal
    Returns "model" cell style, 0 if not applied.
    \a row and \a col given in model coords.
    \since 2.2
    */
    QicsCellStyle* getCellStyle(int row, int col) const;

    /*! \internal
    Sets "model" cell style to \cs.
    \a row and \a col given in model coords.
    \since 2.2
    */
    void setCellStyle(int row, int col, QicsCellStyle *cs);

    /*! \internal
    Returns default cell style.
    \since 2.2
    */
    QicsCellStyle* getDefaultCellStyle() const;

    /*! \internal
    * Returns the value of style property \a name that for
    * cell (\a row , \a col ).  This may involve "flattening" the property
    * by looking first at the cell's style setting, then its column's
    * style, then the row's style, and finally the default style.
    * The first property setting that is found is returned.
    *
    * If \a visual_row and \a visual_col are specified, any visual
    * settings for this cell will be used when retrieving the value of
    * the style property.  The order of precedence for retrieving the property
    * is:
    *
    * - visual cell
    * - model cell
    * - visual column
    * - repeating column
    * - model column
    * - visual row
    * - repeating row
    * - model row
    * - default
    */
    void *getCellProperty(int row, int col,
        QicsCellStyle::QicsCellStyleProperty name,
        int visual_row = -1, int visual_col = -1) const;

    /*! \internal
    * Returns the value of style property \a name for
    * row \a row.  This may involve "flattening" the property
    * by looking first at the row's style setting, then the default
    * style.  The first property setting that is found is returned.
    *
    * If \a visual_row is specified, any visual settings for this row will
    * be used when retrieving the value of the style property.  The order
    * of precedence for retrieving the property is:
    *
    * - visual row
    * - repeating row
    * - model row
    * - default
    */
    void *getRowProperty(int row, QicsCellStyle::QicsCellStyleProperty name,
        int visual_row = -1) const;

    /*! \internal
    * Returns the value of style property \a name for
    * column \a col.  This may involve "flattening" the property
    * by looking first at the column's style setting, then the default
    * style.  The first property setting that is found is returned.
    *
    * If \a visual_col is specified, any visual settings for this column
    * will be used when retrieving the value of the style property.
    * The of precedence for retrieving the property is:
    *
    * - visual column
    * - repeating column
    * - model column
    * - default
    */
    void *getColumnProperty(int col, QicsCellStyle::QicsCellStyleProperty name,
        int visual_col = -1) const;

    /*! \internal
    * Returns the value of style property \a name for
    * repeating rows starting at \a row and repeating every \a interval
    * rows.  This may involve "flattening" the property
    * by looking first at the repeating row's style setting, then the default
    * style.  The first property setting that is found is returned.
    */
    void *getRepeatingRowProperty(int row, unsigned int interval,
        QicsCellStyle::QicsCellStyleProperty name) const;

    /*! \internal
    * Returns the value of style property \a name for
    * repeating columns starting at \a col and repeating every \a interval
    * columns.  This may involve "flattening" the property
    * by looking first at the repeating column's style setting, then the
    * default style.  The first property setting that is found is returned.
    */
    void *getRepeatingColumnProperty(int col, unsigned int interval,
        QicsCellStyle::QicsCellStyleProperty name) const;

    /*! \internal
    * Returns the value of style property \a name on the default
    * style.
    */
    void *getDefaultProperty(QicsCellStyle::QicsCellStyleProperty name) const;

    /*! \internal
    * Sets the style property \a name for the cell (\a row , \a col ).
    *
    * If \a visual_coords is \b true, this setting will apply to the cell value
    * that is displayed in (\a row , \a col ), regardless of hiding or
    * reordering of rows and columns.  Otherwise, it will apply to the model cell
    * specified by (\a row , \a col ), and will follow that cell after any
    * reordering of rows and columns.
    */
    void setCellProperty(int row, int col,  bool visual_coords,
        QicsCellStyle::QicsCellStyleProperty name,
        const void *val);

    /*! \internal
    * Sets the style property \a name for the row specified by \a row.
    * This method will also remove any settings of this style property
    * for individual cells in this row, if the override flag is set.
    *
    * If \a visual_coords is \b true, this setting will apply to cells
    * that are displayed in \a row, regardless of hiding or
    * reordering of rows.  Otherwise, it will apply to all model cells
    * specified by \a row, and will follow that row after any
    * reordering of rows.
    */
    void setRowProperty(int row,  bool visual_coords,
        QicsCellStyle::QicsCellStyleProperty name,
        const void *val,
        bool override = true);

    /*! \internal
    * Sets the style property \a name for the column specified by \a col.
    * This method will also remove any settings of this style property
    * for individual cells in this column, if the override flag is set.
    *
    * If \a visual_coords is \b true, this setting will apply to cells
    * that are displayed in \a col, regardless of hiding or
    * reordering of columns.  Otherwise, it will apply to all model cells
    * specified by \a col, and will follow that column after any
    * reordering of columns.
    */
    void setColumnProperty(int col,  bool visual_coords,
        QicsCellStyle::QicsCellStyleProperty name,
        const void *val,
        bool override = true);

    /*! \internal
    * Sets the style property \a name for
    * repeating rows starting at \a row and repeating every \a interval
    * rows.
    */
    void setRepeatingRowProperty(int row, unsigned int interval,
        QicsCellStyle::QicsCellStyleProperty name,
        const void *val);

    /*! \internal
    * Sets the style property \a name for
    * repeating columns starting at \a col and repeating every \a interval
    * columns.
    */
    void setRepeatingColumnProperty(int col, unsigned int interval,
        QicsCellStyle::QicsCellStyleProperty name,
        const void *val);

    /*! \internal
    * Sets the style property \a name for the entire table.
    * This method will also remove any settings of this style property
    * for individual cells in the table..
    */
    void setDefaultProperty(QicsCellStyle::QicsCellStyleProperty name,
        const void *val);

    /*! \internal
    * Clears property \a name from the row style for row \a row.
    */
    void clearRowProperty(int row, bool visual_coords,
        QicsCellStyle::QicsCellStyleProperty name);

    /*! \internal
    * Clears property \a name from the column style for column \a col.
    */
    void clearColumnProperty(int col, bool visual_coords,
        QicsCellStyle::QicsCellStyleProperty name);

    /*! \internal
    * Clears the style property \a name for
    * repeating rows starting at \a row and repeating every \a interval
    * rows.
    */
    void clearRepeatingRowProperty(int row, unsigned int interval,
        QicsCellStyle::QicsCellStyleProperty name);

    /*! \internal
    * Clears the style property \a name for
    * repeating columns starting at \a col and repeating every \a interval
    * columns.
    */
    void clearRepeatingColumnProperty(int col, unsigned int interval,
        QicsCellStyle::QicsCellStyleProperty name);

    /*! \internal
    * Clears property \a name in cell (\a row, \a col ).
    */
    void clearCellProperty(int row, int col, bool visual_coords,
        QicsCellStyle::QicsCellStyleProperty name);

    /*! \internal
    * Sets property \a name from the grid-wide style.
    */
    void setGridProperty(QicsGridStyle::QicsGridStyleProperty name,
        const void *val);
    /*! \internal
    * Returns the property \a name from the grid-wide style.
    */
    void *getGridProperty(QicsGridStyle::QicsGridStyleProperty name) const;

    /*! \internal
    * Sets the flag which specifies if the style manager should emit
    * a signal whenever a property is changed.
    */
    inline void setReportChanges(bool report)
    { myReportChanges = report; mySpanManager->setReportChanges(report); }

    /*! \internal
    * Returns the flag which specifies if the style manager should emit
    * a signal whenever a property is changed.
    */
    inline bool isReportingChanges() const { return myReportChanges; }

    /*! \internal
    * Returns the span manager object.
    */
    inline QicsSpanManager *spanManager() const { return mySpanManager; }

    /*! \internal
    * Returns attribute controller for model-indexed attributes.
    * \since 2.2
    */
    inline QicsAbstractAttributeController *modelAttributeController() const
    { return myModelAttributeController; }

    /*! \internal
    * Sets attribute controller for model-indexed attributes.
    * \since 2.2
    */
    void setModelAttributeController(QicsAbstractAttributeController* mac);

    /*! \internal
    * Called when data model is set/changed.
    * \since 2.2
    */
    void setDataModel(class QicsDataModel *sm);

public slots:
    /*! \internal
    * Adds \a num rows to the style table.
    * \since 2.2
    */
    void addRows(int num);
    /*! \internal
    * Adds \a num columns to the style table.
    * \since 2.2
    */
    void addColumns(int num);
    /*! \internal
    * Inserts \a num rows into the style table, starting at \a start_position.
    */
    void insertRows(int num, int start_position);
    /*! \internal
    * Inserts \a num columns into the style table, starting at \a start_position.
    */
    void insertColumns(int num, int start_position);
    /*! \internal
    * Deletes \a num rows into the style table, starting at \a start_position.
    */
    void deleteRows(int num, int start_position);
    /*! \internal
    * Deletes \a num columns into the style table, starting at \a start_position.
    */
    void deleteColumns(int num, int start_position);
    /*!
    *  Store styles to \a doc.
    */
    QDomElement toDomXml(const QString& tag, QDomDocument* doc);
    /*!
    *  Reconfigure styles from QDomElement.
    */
    void configureFromDomXml(const QDomElement&, QicsTable * table);

signals:
    /*! \internal
    * If isReportingChanges returns \b true, this signal is emitted when
    * a cell property is changed.
    */
    void cellPropertyChanged(const QicsRegion &reg,
        QicsCellStyle::QicsCellStyleProperty prop,
        bool visual_coords);
    /*! \internal
    * If isReportingChanges returns \b true, this signal is emitted when
    * a grid property is changed.
    */
    void gridPropertyChanged(QicsGridStyle::QicsGridStyleProperty prop);

    /*! \internal
    * This signal is emitted when a table span is added or removed.
    */
    void spanChanged(const QicsSpan &reg);

protected:
    /*! \internal
    * Returns the value of a property for a given cell, or 0 if
    * the property is not set the cell's style.
    * \param row row index of the cell
    * \param col column index of the cell
    * \param prop name of the property to retrieve
    * \param styles vector of styles to use to retrieve this property value
    */
    void *cellProp(int row, int col,
        QicsCellStyle::QicsCellStyleProperty prop,
        const QicsCellStylePVPV &styles) const;

    /*! \internal
    * Returns the value of a property for a given row, or 0 if
    * the property is not set the row's style.
    * \param row row index
    * \param prop name of the property to retrieve
    * \param styles vector of styles to use to retrieve this property value
    */
    void *rowProp(int row, QicsCellStyle::QicsCellStyleProperty prop,
        const QicsCellStylePV &styles) const;

    /*! \internal
    * Returns the value of a property for a given column, or 0 if
    * the property is not set the column's style.
    * \param col column index
    * \param prop name of the property to retrieve
    * \param styles vector of styles to use to retrieve this property value
    */
    void *columnProp(int col, QicsCellStyle::QicsCellStyleProperty prop,
        const QicsCellStylePV &styles) const;

    /*! \internal
    * Returns the value of a property for a given repeating row, or 0 if
    * the property is not set the repeating row's style.
    * \param row starting row index
    * \param interval interval of repeating
    * \param prop name of the property to retrieve
    * \param styles vector of styles to use to retrieve this property value
    */
    void *repeatingRowProp(int start_row, unsigned int interval,
        QicsCellStyle::QicsCellStyleProperty prop,
        const QicsRepeatingCellStylePV &styles) const;

    /*! \internal
    * Returns the value of a property for a visual row, if that property
    * was set in a repeating row style that matches, or 0 if
    * the property is not set.
    * \param row row index
    * \param prop name of the property to retrieve
    * \param styles vector of styles to use to retrieve this property value
    */
    void *repeatingRowProp(int row, QicsCellStyle::QicsCellStyleProperty prop,
        const QicsRepeatingCellStylePV &styles) const;

    /*! \internal
    * Returns the value of a property for a given repeating column, or 0 if
    * the property is not set the repeating column's style.
    * \param col starting column index
    * \param interval interval of repeating
    * \param prop name of the property to retrieve
    * \param styles vector of styles to use to retrieve this property value
    */
    void *repeatingColumnProp(int col, unsigned int interval,
        QicsCellStyle::QicsCellStyleProperty prop,
        const QicsRepeatingCellStylePV &styles) const;

    /*! \internal
    * Returns the value of a property for a visual column, if that property
    * was set in a repeating column style that matches, or 0 if
    * the property is not set.
    * \param col column index
    * \param prop name of the property to retrieve
    * \param styles vector of styles to use to retrieve this property value
    */
    void *repeatingColumnProp(int col, QicsCellStyle::QicsCellStyleProperty prop,
        const QicsRepeatingCellStylePV &styles) const;

    /*! \internal
    * Returns the default value of a property, or 0 if
    * the property is not set in the default style.
    * \param prop name of the property to retrieve
    */
    void *defaultProp(QicsCellStyle::QicsCellStyleProperty prop) const;

    /*! \internal
    * Clears style \a name from all cells in row vector \a row_vec.
    * If \a save_space is \b true, the method will delete any style
    * in the row vector that is now empty (i.e. now properties are set.)
    */
    void clearStyleGivenVectorOfRows(QicsCellStylePV & row_vec, int row,
        QicsCellStyle::QicsCellStyleProperty name,
        bool save_space);

    /*! \internal
    * Returns the grid info object for this manager.
    */
    inline QicsGridInfo *gridInfo() const { return myGridInfo; }

    void setCellFromDomXml(const QDomElement&, QicsTable* table, bool visual_coords);

    void setRowFromDomXml(const QDomElement&, QicsTable* table, bool visual_coords);

    void setColumnFromDomXml(const QDomElement&, QicsTable* table, bool visual_coords);

    void setRepeatingRowFromDomXml(const QDomElement&, QicsTable* table);

    void setRepeatingColumnFromDomXml(const QDomElement&, QicsTable* table);

    void setGridFromDomXml(const QDomElement&, QicsTable* table);

    void setDefaultsFromDomXml(const QDomElement&, QicsTable* table);

protected:
    // \internal the type of this style manager
    Qics::QicsGridType myType;

    // \internal the default style
    QicsCellStyle *myDefaultStyle;

    // \internal the grid style
    QicsGridStyle *myGridStyle;

    // \internal this vector contains styles for each model row
    QicsCellStylePV myVectorOfModelRowStyles;

    // \internal this vector contains styles for each model column
    QicsCellStylePV myVectorOfModelColumnStyles;

    // \internal this vector contains styles for each visual row
    QicsCellStylePV myVectorOfVisualRowStyles;

    // \internal this vector contains styles for each visual column
    QicsCellStylePV myVectorOfVisualColumnStyles;

    // \internal this is a vector of vectors which contains data for the table.
    QicsCellStylePVPV myVectorOfModelColumns;

    // \internal this is a vector of vectors which contains data for the table.
    QicsCellStylePVPV myVectorOfVisualColumns;

    /// \internal Should we emit a signal when a property changes
    bool myReportChanges;

    // \internal keeps track of cell spans
    QicsSpanManager *mySpanManager;

    // \internal base style manager
    QicsStyleManager *myBaseSM;

    // \internal this vector contains repeating row style settings
    QicsRepeatingCellStylePV myVectorOfRepeatingRowStyles;

    // \internal this vector contains repeating column style settings
    QicsRepeatingCellStylePV myVectorOfRepeatingColumnStyles;

    // \internal grid info object -- used for model/visual index lookups
    QicsGridInfo *myGridInfo;

    // \internal attribute controller for model-indexed attributes
    QicsAbstractAttributeController *myModelAttributeController;

private:
    /* \internal
    * Deletes the contents of a vector of allocated cell style vectors.
    */
    void deleteCellStyleVectors(QicsCellStylePVPV &vcols);

    /* \internal
    * Deletes the contents of a vector of allocated cell styles.
    */
    void deleteCellStyles(QicsCellStylePV &csv);

    /* \internal
    * Deletes the contents of a vector of allocated repeating cell styles.
    */
    void deleteRepeatingCellStyles(QicsRepeatingCellStylePV &csv);
};

#endif //QICSSTYLEMANAGER_H


