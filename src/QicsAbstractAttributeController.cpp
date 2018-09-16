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

#include "QicsAbstractAttributeController.h"


QicsAbstractAttributeController::QicsAbstractAttributeController()
{
    myCellStyle = new QicsCellStyle(Qics::TableGrid, false);
}

QicsAbstractAttributeController::~QicsAbstractAttributeController()
{
    delete myCellStyle;
}

QicsCellStyle* QicsAbstractAttributeController::cellStyle(int row, int column)
{
    bool b = false;
    for (int i = 0; i < QicsCellStyle::LastProperty; ++i) {
        const void *val = cellProperty(row, column, i);
        if (val) {
            if (!b) myCellStyle->clear();
            b = true;
            myCellStyle->setValue((QicsCellStyle::QicsCellStyleProperty)i, val);
        }
    }

    return b ? myCellStyle : 0;
}

void QicsAbstractAttributeController::setCellStyle(int row, int column, QicsCellStyle* style)
{
    if (!style) return;

    for (int i = 0; i < QicsCellStyle::LastProperty; ++i) {
        const void *val = style->getValue((QicsCellStyle::QicsCellStyleProperty)i);
        setCellProperty(row, column, (QicsCellStyle::QicsCellStyleProperty)i, val);
    }
}

QicsCellStyle* QicsAbstractAttributeController::rowStyle(int row)
{
    bool b = false;
    for (int i = 0; i < QicsCellStyle::LastProperty; ++i) {
        const void *val = rowProperty(row, i);
        if (val) {
            if (!b) myCellStyle->clear();
            b = true;
            myCellStyle->setValue((QicsCellStyle::QicsCellStyleProperty)i, val);
        }
    }

    return b ? myCellStyle : 0;
}

void QicsAbstractAttributeController::setRowStyle(int row, QicsCellStyle* style)
{
    if (!style) return;

    for (int i = 0; i < QicsCellStyle::LastProperty; ++i) {
        const void *val = style->getValue((QicsCellStyle::QicsCellStyleProperty)i);
        setRowProperty(row, (QicsCellStyle::QicsCellStyleProperty)i, val);
    }
}

QicsCellStyle* QicsAbstractAttributeController::columnStyle(int column)
{
    bool b = false;
    for (int i = 0; i < QicsCellStyle::LastProperty; ++i) {
        const void *val = columnProperty(column, i);
        if (val) {
            if (!b) myCellStyle->clear();
            b = true;
            myCellStyle->setValue((QicsCellStyle::QicsCellStyleProperty)i, val);
        }
    }

    return b ? myCellStyle : 0;
}

void QicsAbstractAttributeController::setColumnStyle(int col, QicsCellStyle* style)
{
    if (!style) return;

    for (int i = 0; i < QicsCellStyle::LastProperty; ++i) {
        const void *val = style->getValue((QicsCellStyle::QicsCellStyleProperty)i);
        setColumnProperty(col, (QicsCellStyle::QicsCellStyleProperty)i, val);
    }
}


