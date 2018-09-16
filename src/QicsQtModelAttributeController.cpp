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

#include "QicsQtModelAttributeController.h"

#include "QicsGridInfo.h"
#include "QicsDataModelQtModelAdapter.h"


QicsQtModelAttributeController::QicsQtModelAttributeController()
    : QicsCommonAttributeController()
{
    m_model = 0;
}

QicsQtModelAttributeController::~QicsQtModelAttributeController()
{
}

bool QicsQtModelAttributeController::isPropertySupported(int name)
{
    // for header grids, there is no support
    if (!myInfo || myInfo->gridType() != Qics::TableGrid)
        return false;

    return QicsCommonAttributeController::isPropertySupported(name);
}

void QicsQtModelAttributeController::reinit(int rows, int columns)
{
    m_model = 0;
    m_adapter = 0;

    if (myInfo && myInfo->gridType() == Qics::TableGrid) {
        QicsDataModelQtModelAdapter *da = qobject_cast<QicsDataModelQtModelAdapter*>(myInfo->dataModel());
        m_adapter = da;
        if (da)
            m_model = da->model();
    }

    QicsCommonAttributeController::reinit(rows, columns);
}

void* QicsQtModelAttributeController::cellProperty(int row, int col, int name)
{
    if (!m_model)
        return 0;

    if (row < 0 || row >= m_rows || col < 0 || col >= m_cols)
        return 0;

    if (WRONG_NAME(name))
        return 0;

    const QModelIndex idx = m_model->index(row, col);

    switch (name)
    {
    case QicsCellStyle::Alignment:
        variant = m_model->data(idx, Qt::TextAlignmentRole);
        if (!variant.isValid()) return 0;
        uinteger = variant.toUInt();
        return &uinteger;
    case QicsCellStyle::ForeColor:
        clr = qvariant_cast<QColor>(m_model->data(idx, Qt::TextColorRole));
        return clr.isValid() ? &clr : 0;
    case QicsCellStyle::BackColor:
        clr = qvariant_cast<QColor>(m_model->data(idx, Qt::BackgroundColorRole));
        return clr.isValid() ? &clr : 0;
    case QicsCellStyle::Font:
        variant = m_model->data(idx, Qt::FontRole);
        if (!variant.isValid()) return 0;
        font = qvariant_cast<QFont>(variant);
        return &font;
    case QicsCellStyle::ToolTipText:
        variant = m_model->data(idx, Qt::ToolTipRole);
        if (!variant.isValid()) return 0;
        string = variant.toString();
        return &string;
    case QicsCellStyle::Pixmap:
        pixmap = qvariant_cast<QPixmap>(m_model->data(idx, Qt::DecorationRole));
        return pixmap.isNull() ? 0 : &pixmap;
    case QicsCellStyle::Enabled: {
            QModelIndex index = m_model->index(row, col, QModelIndex());
            if (!index.isValid()) return 0;
            boolean = m_model->flags(index) & Qt::ItemIsEnabled;
            return &boolean;
        }
    case QicsCellStyle::ReadOnly: {
            QModelIndex index = m_model->index(row, col, QModelIndex());
            if (!index.isValid()) return 0;
            boolean = !(m_model->flags(index) & Qt::ItemIsEditable);
            return &boolean;
        }
    }

    return 0;
}

bool QicsQtModelAttributeController::setCellProperty(int row, int col, int name, const void *val)
{
    if (!m_model)
        return false;

    if (row < 0 || row >= m_rows || col < 0 || col >= m_cols)
        return false;

    if (WRONG_NAME(name))
        return false;

    const QModelIndex idx = m_model->index(row, col);

    // special case - font virtual attributes
    if (name >= QicsCellStyle::FontFamily && name <= QicsCellStyle::FontStretch) {
        // get font stored for the cell
        QFont *f = (QFont*)cellProperty(row, col, QicsCellStyle::Font);
        if (f) {
            // clear this font attribute and make new one with applied val
            clearCellProperty(row, col, QicsCellStyle::Font);
            if (!val) return true;
        } else {
            if (!val) return true;
            f = (QFont*)defaultProperty(QicsCellStyle::Font);
            if (!f) {
                font = QFont();
                f = &font;
            } else {
                font = *f;
                f = &font;
            }
        }

        QicsAttrCommon::applyFontAttr(&f, name, val);

        bool old_sig = m_adapter->emitSignals();
        m_adapter->setEmitSignals(false);

        bool b = m_model->setData(idx, *f, Qt::FontRole);

        m_adapter->setEmitSignals(old_sig);
        return b;
    }

    // just clear an attribute as well
    bool b = clearCellProperty(row, col, name);
    if (!val) return b;

    bool old_sig = m_adapter->emitSignals();
    m_adapter->setEmitSignals(false);

    b = false;

    switch (name)
    {
    case QicsCellStyle::Alignment:
        b = m_model->setData(idx, *(uint*)val, Qt::TextAlignmentRole);
        break;
    case QicsCellStyle::ToolTipText:
        b = m_model->setData(idx, *(QString*)val, Qt::ToolTipRole);
        break;
    case QicsCellStyle::ForeColor:
        b = m_model->setData(idx, *(QColor*)val, Qt::TextColorRole);
        break;
    case QicsCellStyle::BackColor:
        b = m_model->setData(idx, *(QColor*)val, Qt::BackgroundColorRole);
        break;
    case QicsCellStyle::Font:
        b = m_model->setData(idx, *(QFont*)val, Qt::FontRole);
        break;
    case QicsCellStyle::Pixmap:
        b = m_model->setData(idx, *(QPixmap*)val, Qt::DecorationRole);
        break;
    default:
        break;
    }

    m_adapter->setEmitSignals(old_sig);
    return b;
}

bool QicsQtModelAttributeController::clearCellProperty(int row, int col, int name)
{
    if (!m_model)
        return false;

    if (row < 0 || row >= m_rows || col < 0 || col >= m_cols)
        return false;

    if (WRONG_NAME(name))
        return false;

    const QModelIndex idx = m_model->index(row, col);

    bool b = false;
    bool old_sig = m_adapter->emitSignals();
    m_adapter->setEmitSignals(false);

    switch (name)
    {
    case QicsCellStyle::Alignment:
        b = m_model->setData(idx, QVariant(), Qt::TextAlignmentRole);
        break;
    case QicsCellStyle::ToolTipText:
        b = m_model->setData(idx, QVariant(), Qt::ToolTipRole);
        break;
    case QicsCellStyle::ForeColor:
        b = m_model->setData(idx, QVariant(), Qt::TextColorRole);
        break;
    case QicsCellStyle::BackColor:
        b =  m_model->setData(idx, QVariant(), Qt::BackgroundColorRole);
        break;
    case QicsCellStyle::Font:
        b = m_model->setData(idx, QVariant(), Qt::FontRole);
        break;
    case QicsCellStyle::Pixmap:
        b = m_model->setData(idx, QVariant(), Qt::DecorationRole);
        break;
    default:
        break;
    }

    m_adapter->setEmitSignals(old_sig);
    return b;
}


