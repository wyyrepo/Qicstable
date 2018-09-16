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


#include "QicsCommonAttributeController.h"

#include "QicsCellStyle.h"


QicsCommonAttributeController::QicsCommonAttributeController()
    : QicsAbstractAttributeController()
{
    m_rows = m_cols = 0;

    myDefaultStyle = 0;
    setDefaultStyle(0);
}

QicsCommonAttributeController::~QicsCommonAttributeController()
{
    reinit(0,0);
    delete myDefaultStyle;
}

void QicsCommonAttributeController::reinit(int rows, int columns)
{
    if (!m_rows && !m_cols && !rows && !columns) return;

    // clear old cell info
    if (m_rows) deleteRows(m_rows, 0);
    if (m_cols) deleteColumns(m_cols, 0);

    handleReinit(rows, columns);

    m_arows.clear();
    m_acols.clear();

    // fill new cell info
    m_cols = 0;
    m_rows = 0;

    if (rows) insertRows(rows, 0);
    if (columns) insertColumns(columns, 0);
}

void QicsCommonAttributeController::insertRows(int num, int at)
{
    if (at > m_rows) at = m_rows;
    if (at < 0) at = 0;

    // insert row attrs
    for (int i = 0; i < num; ++i)
        m_arows.insert(at, new QicsAttrRowCol());

    handleInsertRows(num, at);

    m_rows += num;
}

void QicsCommonAttributeController::deleteRows(int num, int at)
{
    if (at >= m_rows) return;
    if (at < 0) at = 0;

    // remove row attrs
    for (int i = 0; i < num; ++i)
        delete m_arows.takeAt(at);

    handleDeleteRows(num, at);

    m_rows -= num;
}

void QicsCommonAttributeController::insertColumns(int num, int at)
{
    if (at > m_cols) at = m_cols;
    if (at < 0) at = 0;

    for (int i = 0; i < num; ++i)
        m_acols.insert(at, new QicsAttrRowCol());

    handleInsertColumns(num, at);

    m_cols += num;
}

void QicsCommonAttributeController::deleteColumns(int num, int at)
{
    if (at >= m_cols) return;
    if (at < 0) at = 0;

    // remove column attrs
    for (int i = 0; i < num; ++i)
        delete m_acols.takeAt(at);

    handleDeleteColumns(num, at);

    m_cols -= num;
}

void* QicsCommonAttributeController::defaultProperty(int name)
{
    if (name < 0 || name >= QicsCellStyle::LastProperty)
        return 0;

    return myDefaultStyle->getValue((QicsCellStyle::QicsCellStyleProperty)name);
}

bool QicsCommonAttributeController::setDefaultProperty(int name, const void *val)
{
    // set default property
    if (name < 0 || name >= QicsCellStyle::LastProperty)
        return 0;

    myDefaultStyle->setValue((QicsCellStyle::QicsCellStyleProperty)name, val);

    // try to remove val from all the cells
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c)
            if (!clearCellProperty(r, c, name))
                return false;

    return true;
}

bool QicsCommonAttributeController::isDefaultPropertySupported(int name)
{
    if (name < 0 || name >= QicsCellStyle::LastProperty)
        return false;

    return true;
}

void QicsCommonAttributeController::setDefaultStyle(QicsCellStyle* style)
{
    delete myDefaultStyle;

    // create default style from a widget
    if (!style)
        myDefaultStyle = new QicsCellStyle(Qics::TableGrid, true);
    else
        myDefaultStyle = new QicsCellStyle(*style);
}

bool QicsCommonAttributeController::isPropertySupported(int name)
{
    if (WRONG_NAME(name))
        return false;

    return true;
}

void* QicsCommonAttributeController::rowProperty(int row, int name)
{
    if (row < 0 || row >= m_rows)
        return 0;

    if (WRONG_NAME(name))
        return 0;

    QicsAttrRowCol *ac = rowAttr(row);
    if (!ac) return 0;

    return ac->attr(name);
}

bool QicsCommonAttributeController::setRowProperty(int row, int name, const void *val)
{
    if (!val)
        return clearRowProperty(row, name);

    if (row < 0 || row >= m_rows)
        return false;

    if (WRONG_NAME(name))
        return false;

    QicsAttrRowCol *ac = rowAttr(row);
    if (!ac) setRowAttr(row, ac = new QicsAttrRowCol());

    return ac->setAttr(name, val);
}

bool QicsCommonAttributeController::clearRowProperty(int row, int name)
{
    if (row < 0 || row >= m_rows)
        return false;

    if (WRONG_NAME(name))
        return false;

    QicsAttrRowCol *ac = rowAttr(row);
    if (!ac) return true;	// no such attr at all - assuming that it is cleared

    return ac->clearAttr(name);
}

void* QicsCommonAttributeController::columnProperty(int col, int name)
{
    if (col < 0 || col >= m_cols)
        return 0;

    if (WRONG_NAME(name))
        return 0;

    QicsAttrRowCol *ac = columnAttr(col);
    if (!ac) return 0;

    return ac->attr(name);
}

bool QicsCommonAttributeController::setColumnProperty(int col, int name, const void *val)
{
    if (!val)
        return clearColumnProperty(col, name);

    if (col < 0 || col >= m_cols)
        return false;

    if (WRONG_NAME(name))
        return false;

    QicsAttrRowCol *ac = columnAttr(col);
    if (!ac) setColumnAttr(col, ac = new QicsAttrRowCol());

    return ac->setAttr(name, val);
}

bool QicsCommonAttributeController::clearColumnProperty(int col, int name)
{
    if (col < 0 || col >= m_cols)
        return false;

    if (WRONG_NAME(name))
        return false;

    QicsAttrRowCol *ac = columnAttr(col);
    if (!ac) return true;	// no such attr at all - assuming that it is cleared

    return ac->clearAttr(name);
}

void QicsCommonAttributeController::handleReinit(int rows, int columns)
{
    Q_UNUSED(rows);
    Q_UNUSED(columns);
}

void QicsCommonAttributeController::handleInsertRows(int num, int start_position)
{
    Q_UNUSED(num);
    Q_UNUSED(start_position);
}

void QicsCommonAttributeController::handleInsertColumns(int num, int start_position)
{
    Q_UNUSED(num);
    Q_UNUSED(start_position);
}

void QicsCommonAttributeController::handleDeleteRows(int num, int start_position)
{
    Q_UNUSED(num);
    Q_UNUSED(start_position);
}

void QicsCommonAttributeController::handleDeleteColumns(int num, int start_position)
{
    Q_UNUSED(num);
    Q_UNUSED(start_position);
}


