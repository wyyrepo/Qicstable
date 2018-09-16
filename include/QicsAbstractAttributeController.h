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

#ifndef QICSABSTRACTATTRIBUTECONTROLLER_H
#define QICSABSTRACTATTRIBUTECONTROLLER_H

#include "QicsNamespace.h"
#include "QicsCellStyle.h"

class QicsGridInfo;

/*!
* \class QicsAbstractAttributeController QicsAbstractAttributeController.h
* \brief QicsAbstractAttributeController is used to manage attributes of a grid object
* like a table.
*
* QicsAbstractAttributeController provides abstract interface for getting/setting/clearing
* properties of a grid. It is used by QicsStyleManager to allow external management of
* some attributes. It should be subclassed in order to use.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsAbstractAttributeController : public Qics
{
public:
    QicsAbstractAttributeController();
    virtual ~QicsAbstractAttributeController();

    /*!
    * Creates new attribute controller.
    */
    virtual QicsAbstractAttributeController* createController() = 0;

    /*!
    * Returns pointer to a property \a name at cell with row \a row and column \a col, 0 if not found.
    */
    virtual void* cellProperty(int row, int col, int name) = 0;

    /*!
    * Returns pointer to a row \a row property \a name, 0 if not found.
    */
    virtual void* rowProperty(int row, int name) = 0;

    /*!
    * Returns pointer to a column \a col property \a name, 0 if not found.
    */
    virtual void* columnProperty(int col, int name) = 0;

    /*!
    * Returns pointer to default property \a name, 0 if not found.
    */
    virtual void* defaultProperty(int name) = 0;

    /*!
    * Sets a property \a name at cell with row \a row and column \a col to a value of \a val.
    * Returns \a true if ok, \a false if such property cannot be set (i.e. not supported).
    */
    virtual bool setCellProperty(int row, int col, int name, const void *val) = 0;

    /*!
    * Sets a row \a row property \a name to a value of \a val.
    * Returns \a true if ok, \a false if such property cannot be set (i.e. not supported).
    */
    virtual bool setRowProperty(int row, int name, const void *val) = 0;

    /*!
    * Sets a column \a col property \a name to a value of \a val.
    * Returns \a true if ok, \a false if such property cannot be set (i.e. not supported).
    */
    virtual bool setColumnProperty(int col, int name, const void *val) = 0;

    /*!
    * Sets default property \a name to a value of \a val.
    * Returns \a true if ok, \a false if such property cannot be set (i.e. not supported).
    */
    virtual bool setDefaultProperty(int name, const void *val) = 0;

    /*!
    * Clears property \a name at cell with row \a row and column \a col.
    * Returns \a true if ok, \a false if such property cannot be cleared (i.e. not supported).
    */
    virtual bool clearCellProperty(int row, int col, int name) = 0;

    /*!
    * Clears row \a row property \a name.
    * Returns \a true if ok, \a false if such property cannot be cleared (i.e. not supported).
    */
    virtual bool clearRowProperty(int row, int name) = 0;

    /*!
    Clears column \a col property \a name.
    Returns \a true if ok, \a false if such property cannot be cleared (i.e. not supported).
    */
    virtual bool clearColumnProperty(int col, int name) = 0;

    /*!
    * Returns true if property \a name is supported.
    * Else returns false.
    */
    virtual bool isPropertySupported(int /*name*/) { return true; }

    /*!
    * Returns true if default property \a name is supported.
    * Else returns false.
    */
    virtual bool isDefaultPropertySupported(int /*name*/) { return false; }

    /*!
    * Returns cell style from \a row and \a column.
    */
    virtual QicsCellStyle* cellStyle(int row, int column);

    /*!
    * Sets cell \a style for \a row and \a column.
    */
    virtual void setCellStyle(int row, int column, QicsCellStyle* style);

    /*!
    * Returns cell style from \a row.
    */
    virtual QicsCellStyle* rowStyle(int row);

    /*!
    * Sets cell \a style for \a row.
    */
    virtual void setRowStyle(int row, QicsCellStyle* style);

    /*!
    * Returns cell style from \a column.
    */
    virtual QicsCellStyle* columnStyle(int column);

    /*!
    * Sets cell \a style for \a column.
    */
    virtual void setColumnStyle(int column, QicsCellStyle* style);

    /*!
    * Returns default style.
    */
    virtual QicsCellStyle* defaultStyle() = 0;

    /*!
    * Sets default \a style.
    */
    virtual void setDefaultStyle(QicsCellStyle* style) = 0;

    /*!
    * Sets grid info of a parent.
    */
    virtual void setGridInfo(QicsGridInfo* /*gi*/) = 0;

    /*!
    * Performs total reinitialization based on new amount of \a columns and \a rows.
    */
    virtual void reinit(int rows, int columns) = 0;

    /*!
    * Inserts \a num rows, starting at \a start_position.
    */
    virtual void insertRows(int num, int start_position) = 0;

    /*!
    * Inserts \a num columns, starting at \a start_position.
    */
    virtual void insertColumns(int num, int start_position) = 0;

    /*!
    * Deletes \a num rows, starting at \a start_position.
    */
    virtual void deleteRows(int num, int start_position) = 0;

    /*!
    * Deletes \a num columns, starting at \a start_position.
    */
    virtual void deleteColumns(int num, int start_position) = 0;

protected:
    QicsCellStyle *myCellStyle;
};

#endif //QICSABSTRACTATTRIBUTECONTROLLER_H


