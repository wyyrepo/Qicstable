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

#ifndef QICSCOMMONATTRIBUTECONTROLLER_H
#define QICSCOMMONATTRIBUTECONTROLLER_H

#include "QicsNamespace.h"
#include "QicsAbstractAttributeController.h"
#include "QicsCommonAttrs.h"
#include "QicsRegion.h"
#include "QicsCellStyle.h"


#define WRONG_NAME(name)    (name < 0 || name >= QicsCellStyle::LastVirtualProperty || name == QicsCellStyle::LastProperty)


/*!
* \class QicsCommonAttributeController QicsCommonAttributeController.h
* \brief QicsCommonAttributeController is an implementation of basic attribute controller's
functionality and is intended for subclassing.

This controller provides handling of row/column and default cell attributes.
*
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////


class QICS_EXPORT QicsCommonAttributeController : public QicsAbstractAttributeController
{
public:
    QicsCommonAttributeController();

    virtual ~QicsCommonAttributeController();

    /*!
    Returns pointer to a row \a row property \a name, 0 if not found.
    */
    virtual void* rowProperty(int row, int name);

    /*!
    Returns pointer to a column \a col property \a name, 0 if not found.
    */
    virtual void* columnProperty(int col, int name);

    /*!
    Returns pointer to default property \a name, 0 if not found.
    */
    virtual void* defaultProperty(int name);

    /*!
    Sets a row \a row property \a name to a value of \a val.
    Returns \a true if ok, \a false if such property cannot be set (i.e. not supported).
    */
    virtual bool setRowProperty(int row, int name, const void *val);

    /*!
    Sets a column \a col property \a name to a value of \a val.
    Returns \a true if ok, \a false if such property cannot be set (i.e. not supported).
    */
    virtual bool setColumnProperty(int col, int name, const void *val);

    virtual bool setDefaultProperty(int name, const void *val);

    /*!
    Clears row \a row property \a name.
    Returns \a true if ok, \a false if such property cannot be cleared (i.e. not supported).
    */
    virtual bool clearRowProperty(int row, int name);

    /*!
    Clears column \a col property \a name.
    Returns \a true if ok, \a false if such property cannot be cleared (i.e. not supported).
    */
    virtual bool clearColumnProperty(int col, int name);

    virtual bool isPropertySupported(int name);
    virtual bool isDefaultPropertySupported(int name);

    virtual QicsCellStyle* defaultStyle() { return myDefaultStyle; }

    virtual void setDefaultStyle(QicsCellStyle* style);
    virtual void setGridInfo(QicsGridInfo* gi) { myInfo = gi; }

    virtual void reinit(int rows, int columns);

    virtual void insertRows(int num, int start_position);
    virtual void insertColumns(int num, int start_position);

    virtual void deleteRows(int num, int start_position);
    virtual void deleteColumns(int num, int start_position);

protected:
    /*!
    Called internally from reinit(). Should be reimplemented in subclasses
    in order to handle reinitialization of cell attributes (i.e. clear).
    \since 2.2.1
    */
    virtual void handleReinit(int rows, int columns);

    /*!
    Called internally from insertRows(). Should be reimplemented in subclasses
    in order to handle change of cell attributes when rows are being inserted.
    \since 2.2.1
    */
    virtual void handleInsertRows(int num, int start_position);

    /*!
    Called internally from insertColumns(). Should be reimplemented in subclasses
    in order to handle change of cell attributes when columns are being inserted.
    \since 2.2.1
    */
    virtual void handleInsertColumns(int num, int start_position);

    /*!
    Called internally from deleteRows(). Should be reimplemented in subclasses
    in order to handle change of cell attributes when rows are being deleted.
    \since 2.2.1
    */
    virtual void handleDeleteRows(int num, int start_position);

    /*!
    Called internally from deleteColumns(). Should be reimplemented in subclasses
    in order to handle change of cell attributes when columns are being deleted.
    \since 2.2.1
    */
    virtual void handleDeleteColumns(int num, int start_position);

    inline void removeRowAttr(int r);

    inline QicsAttrRowCol*	rowAttr(int r);

    inline void setRowAttr(int r, QicsAttrRowCol* attr);

    inline void removeColumnAttr(int c);

    inline QicsAttrRowCol* columnAttr(int c);

    inline void setColumnAttr(int c, QicsAttrRowCol* attr);

    // \internal row attributes
    QList<QicsAttrRowCol*> m_arows;
    // \internal column attributes
    QList<QicsAttrRowCol*> m_acols;
    // \internal row count
    int m_rows;
    // \internal column count
    int m_cols;
    // \internal the default style
    QicsCellStyle  *myDefaultStyle;
    // \internal grid info of the parent
    QicsGridInfo *myInfo;
};

inline void QicsCommonAttributeController::removeRowAttr(int r)
{
    setRowAttr(r, 0);
}

inline QicsAttrRowCol* QicsCommonAttributeController::rowAttr(int r)
{
    return m_arows.at(r);
}

inline void QicsCommonAttributeController::setRowAttr(int r, QicsAttrRowCol* attr)
{
    delete m_arows.at(r);
    m_arows[r] = attr;
}

inline void QicsCommonAttributeController::removeColumnAttr(int c)
{
    setColumnAttr(c, 0);
}

inline QicsAttrRowCol* QicsCommonAttributeController::columnAttr(int c)
{
    return m_acols.at(c);
}

inline void QicsCommonAttributeController::setColumnAttr(int c, QicsAttrRowCol* attr)
{
    delete m_acols.at(c);
    m_acols[c] = attr;
}

#endif //QICSCOMMONATTRIBUTECONTROLLER_H


