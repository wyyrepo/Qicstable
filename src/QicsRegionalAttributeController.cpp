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

#include "QicsRegionalAttributeController.h"

#include "QicsCellDisplay.h"


QicsRegionalAttributeController::QicsRegionalAttributeController() : QicsCommonAttributeController()
{
}

QicsRegionalAttributeController::~QicsRegionalAttributeController()
{
}

void QicsRegionalAttributeController::handleReinit(int rows, int columns)
{
    Q_UNUSED(rows);
    Q_UNUSED(columns);
    m_attrs.clear();
}

void QicsRegionalAttributeController::handleInsertRows(int num, int at)
{
    // we're iterating through inner hashes to insert rows into each of them
    AttrHash::iterator it_ah, it_ah_end(m_attrs.end());
    for (it_ah = m_attrs.begin(); it_ah != it_ah_end; ++it_ah) {
        AttrValueHash::iterator it_av;
        for (it_av = (*it_ah).begin(); it_av != (*it_ah).end(); it_av++) {
            for (int i = 0; i < num; ++i)
                (*it_av).insert(at, AttrRegionList());
        }
    }
}

void QicsRegionalAttributeController::handleDeleteRows(int num, int at)
{
    // we're iterating through inner hashes to remove rows from each of them
    AttrHash::iterator it_ah, it_ah_end(m_attrs.end());
    for (it_ah = m_attrs.begin(); it_ah != it_ah_end; ++it_ah) {
        AttrValueHash::iterator it_av;
        for (it_av = (*it_ah).begin(); it_av != (*it_ah).end(); it_av++) {
            for (int i = 0; i < num; ++i)
                (*it_av).removeAt(at);
        }
    }
}

void QicsRegionalAttributeController::handleInsertColumns(int num, int at)
{
    // we're iterating through rows to insert columns into each of region having "at" inside
    AttrHash::iterator it_ah, it_ah_and(m_attrs.end());
    for (it_ah = m_attrs.begin(); it_ah != it_ah_and; ++it_ah) {
        AttrValueHash::iterator it_av, it_av_end((*it_ah).end());
        for (it_av = (*it_ah).begin(); it_av != it_av_end; ++it_av) {
            AttrRowList::iterator it_rl, it_rl_end((*it_av).end());
            for (it_rl = (*it_av).begin(); it_rl != it_rl_end; ++it_rl) {
                AttrRegionList::iterator it_r;
                for (it_r = (*it_rl).begin(); it_r != (*it_rl).end(); ++it_r) {
                    AttrRegion &r = *it_r;
                    if (r.end < at) // very previous
                        continue;
                    if (r.start < at && r.end >= at) { // this region must be splitted into 2 regions as well
                        AttrRegion r1;
                        r1.start = at + num;
                        r1.end = r.end + num;
                        r.end = at-1;
                        it_r++;
                        it_r = (*it_rl).insert(it_r, r1);   // it_r is now inserted item - we skipping it
                        continue;
                    }
                    // just move region to "num" positions to the right
                    r.start += num;
                    r.end += num;
                }
            }
        }
    }
}

void QicsRegionalAttributeController::handleDeleteColumns(int num, int at)
{
    // we're iterating through rows to remove columns from each of region having "at" inside
    AttrHash::iterator it_ah, it_ah_end(m_attrs.end());
    for (it_ah = m_attrs.begin(); it_ah != it_ah_end; ++it_ah) {
        AttrValueHash::iterator it_av, it_av_end((*it_ah).end());
        for (it_av = (*it_ah).begin(); it_av != it_av_end; ++it_av) {
            AttrRowList::iterator it_rl, it_rl_end((*it_av).end());
            for (it_rl = (*it_av).begin(); it_rl != it_rl_end; ++it_rl) {
                AttrRegionList::iterator it_r;
                for (it_r = (*it_rl).begin(); it_r != (*it_rl).end(); it_r++) {
                    AttrRegion &r = *it_r;
                    if (r.end < at) // very previous
                        continue;
                    if (at <= r.start && at+num-1 >= r.end) { // remove this region at all
                        it_r = (*it_rl).erase(it_r);
                        --it_r; // ???
                        continue;
                    }
                    if (at <= r.start && r.start <= at+num-1) {  // remove start of region
                        r.start = at+num;
                    } else if (r.start <= at && r.end <= at+num-1) { // remove end of region
                        r.end = at-1;
                    }

                    if (r.start >= at)
                        r.start -= num;
                    if (r.end >= at)
                        r.end -= num;
                }
            }
        }
    }
}

void* QicsRegionalAttributeController::cellAttr(int row, int col, int name)
{
    AttrHash::const_iterator it_attrs(m_attrs.find(name));
    if (it_attrs == m_attrs.constEnd()) return 0;

    const AttrValueHash &vh = (*it_attrs);

    AttrValueHash::const_iterator it_av, it_av_end(vh.constEnd());
    for (it_av = vh.constBegin(); it_av != it_av_end; ++it_av) {
        if ((*it_av).size() <= row)
            continue;

        const AttrRegionList &rl = (*it_av).at(row);
        AttrRegionList::const_iterator it_r, it_r_end(rl.constEnd());
        for (it_r = rl.constBegin(); it_r != it_r_end; ++it_r) {
            const AttrRegion &r = *it_r;
            if (r.start <= col && r.end >= col) {
                // found - we'll map QVariant to the value
                const AttrVariant &v = it_av.key();
                switch (name)
                {
                    case QicsCellStyle::ToolTipText:
                    case QicsCellStyle::Label:
                    case QicsCellStyle::PixmapName:
                        string = v.toString();
                        return &string;
                    case QicsCellStyle::TopBorderPen:
                    case QicsCellStyle::BottomBorderPen:
                    case QicsCellStyle::LeftBorderPen:
                    case QicsCellStyle::RightBorderPen:
#if defined(_MSC_VER) && _MSC_VER < 1300
                        pen = qvariant_cast<QPen>(v);
#else
                        pen = v.value<QPen>();
#endif
                        return &pen;
                    case QicsCellStyle::Pixmap:
#if defined(_MSC_VER) && _MSC_VER < 1300
                        pixmap = qvariant_cast<QPixmap>(v);
#else
                        pixmap = v.value<QPixmap>();
#endif
                        return &pixmap;
                    case QicsCellStyle::Font:
                    case QicsCellStyle::SelectedFont:
#if defined(_MSC_VER) && _MSC_VER < 1300
                        font = qvariant_cast<QFont>(v);
#else
                        font = v.value<QFont>();
#endif
                        return &font;
                    case QicsCellStyle::CellDisplayer:
                    case QicsCellStyle::Formatter:
                    case QicsCellStyle::Validator:
                    case QicsCellStyle::PasteValidator:
                    case QicsCellStyle::UserData:
#if defined(_MSC_VER) && _MSC_VER < 1300
                        pointer = qvariant_cast<quintptr>(v);
#else
                        pointer = v.value<quintptr>();
#endif
                        return (void*)pointer;
                    case QicsCellStyle::ForeColor:
                    case QicsCellStyle::BackColor:
                    case QicsCellStyle::WindowColor:
                    case QicsCellStyle::WindowTextColor:
                    case QicsCellStyle::SelForeColor:
                    case QicsCellStyle::SelBackColor:
                    case QicsCellStyle::EditForegroundColor:
                    case QicsCellStyle::EditBackgroundColor:
                    case QicsCellStyle::HighlightForeColor:
                    case QicsCellStyle::HighlightBackColor:
                        clr = QColor(v.toUInt());
                        return &clr;
                }
                // default case - return the rest as uint
                uinteger = v.toUInt();
                return &uinteger;
            }
        }
    }

    return 0;
}

void* QicsRegionalAttributeController::cellProperty(int row, int col, int name)
{
    if (row < 0 || row >= m_rows || col < 0 || col >= m_cols)
        return 0;

    if (WRONG_NAME(name))
        return 0;

    if (!m_attrs.size()) return 0;

    return cellAttr(row, col, name);
}

void QicsRegionalAttributeController::setCellAttr(int row, int col, int name, AttrVariant val)
{
    AttrValueHash &vh = m_attrs[name];
    AttrRowList &rwl = vh[val];

    if (rwl.size() <= row)
        while (rwl.size() <= row+10)
            rwl.append(AttrRegionList());

    AttrRegionList &rl = rwl[row];

    if (rl.isEmpty()) {
        AttrRegion r;
        r.start = r.end = col;
        rl.append(r);
        return;
    }

    AttrRegionList::iterator it_r, it_r_end(rl.end());
    for (it_r = rl.begin(); it_r != it_r_end; ++it_r) {
        AttrRegion &r = *it_r;

        if (r.start <= col && r.end >= col) // exists already
            return;

        if (r.start-1 == col) {
            r.start = col;
            return;
        }
        if (r.start > col) {    // insert new
            AttrRegion r1;
            r1.start = r1.end = col;
            rl.insert(it_r, r1);
            return;
        }
        if (r.end+1 == col) {
            r.end = col;

            // check if we can merge 2 regions
            AttrRegionList::iterator it_r2 = it_r + 1;
            if (it_r2 != it_r_end && (*it_r2).start == col+1) {
                r.end = (*it_r2).end;
                rl.erase(it_r2);
            }
            return;
        }
    }

    AttrRegion r;
    r.start = r.end = col;
    rl.append(r);
}

bool QicsRegionalAttributeController::setCellProperty(int row, int col, int name, const void *val)
{
    if (row < 0 || row >= m_rows || col < 0 || col >= m_cols)
        return false;

    if (WRONG_NAME(name))
        return false;

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

        setCellAttr(row, col, QicsCellStyle::Font, AttrVariant(*f));

        return true;
    }

    // just clear an attribute as well
    bool b = clearCellProperty(row, col, name);
    if (!val) return b;

    switch (name)
    {
        case QicsCellStyle::Alignment:
        case QicsCellStyle::TextFlags:
        case QicsCellStyle::CellMargin:
        case QicsCellStyle::BorderWidth:
        case QicsCellStyle::PixmapSpacing:
        case QicsCellStyle::MaxLength:
            setCellAttr(row, col, name, AttrVariant(*(int*)(val)));
            return true;

        case QicsCellStyle::AutoRecognition:
        case QicsCellStyle::CanAcceptDrops:
        case QicsCellStyle::ReadOnly:
        case QicsCellStyle::Enabled:

        case QicsCellStyle::Selected:
            setCellAttr(row, col, name, AttrVariant(*(bool*)(val)));
            return true;

        case QicsCellStyle::ForeColor:
        case QicsCellStyle::BackColor:
        case QicsCellStyle::WindowColor:
        case QicsCellStyle::WindowTextColor:
        case QicsCellStyle::SelForeColor:
        case QicsCellStyle::SelBackColor:
        case QicsCellStyle::EditBackgroundColor:
        case QicsCellStyle::EditForegroundColor:
        case QicsCellStyle::HighlightForeColor:
        case QicsCellStyle::HighlightBackColor:
            setCellAttr(row, col, name, AttrVariant( ((QColor*)val)->rgba() ));
            return true;

        case QicsCellStyle::TopBorderPen:
        case QicsCellStyle::BottomBorderPen:
        case QicsCellStyle::LeftBorderPen:
        case QicsCellStyle::RightBorderPen:
            // it is cleared - so we won't remember empty pen
            if (((QPen*)val)->style() == Qt::NoPen) return true;
            setCellAttr(row, col, name, AttrVariant( *(QPen*)val));
            return true;

        case QicsCellStyle::CellDisplayer: {
                QicsCellDisplay *cd = (QicsCellDisplay*)val;
                if (cd)
                    cd->aboutToClear(0,-1,-1);
            }
        // fall through
        case QicsCellStyle::Formatter:
        case QicsCellStyle::Validator:
        case QicsCellStyle::PasteValidator:
        case QicsCellStyle::UserData:
            setCellAttr(row, col, name, AttrVariant((quintptr)val));
            return true;

        case QicsCellStyle::ToolTipText:
        case QicsCellStyle::Label:
        case QicsCellStyle::PixmapName:
            setCellAttr(row, col, name, AttrVariant( *(QString*)val));
            return true;

        case QicsCellStyle::Pixmap:
            // it is cleared - so we won't remember empty pixmap
            if (((QPixmap*)val)->isNull()) return true;
            setCellAttr(row, col, name, AttrVariant( *(QPixmap*)val));
            return true;

        case QicsCellStyle::Font:
        case QicsCellStyle::SelectedFont:
            setCellAttr(row, col, name, AttrVariant( *(QFont*)val));
            return true;
    }

    return false;
}

void QicsRegionalAttributeController::clearCellAttr(int row, int col, int name)
{
    AttrHash::iterator it_attrs(m_attrs.find(name));
    if (it_attrs == m_attrs.end()) return;				// no such attr

    AttrValueHash &vh = (*it_attrs);
    if (!vh.size()) return;

    AttrValueHash::iterator it_av, it_av_end(vh.end());

    for (it_av = vh.begin(); it_av != it_av_end; ++it_av) {
        if ((*it_av).size() <= row)
            continue;   // no such row

        AttrRegionList &rl = (*it_av)[row];
        if (!rl.size())
            continue;

        AttrRegionList::iterator it_r, it_r_end(rl.end());
        for (it_r = rl.begin(); it_r != it_r_end; ++it_r) {
            AttrRegion &r = *it_r;
            if (r.start > col)  // too far
                break;
            if (r.start == r.end && r.start == col) {   // remove this region at all
                rl.erase(it_r);
                break;
            }
            if (r.start == col) {
                r.start = col+1;
                break;
            }
            if (r.end == col) {
                r.end = col-1;
                break;
            }
            if (r.start < col && r.end > col) { // this region must be splitted into 2 regions as well
                AttrRegion r1;
                r1.start = col+1;
                r1.end = r.end;
                r.end = col-1;
                ++it_r;
                rl.insert(it_r, r1);
                break;
            }
        }
    }
}

bool QicsRegionalAttributeController::clearCellProperty(int row, int col, int name)
{
    if (row < 0 || row >= m_rows || col < 0 || col >= m_cols)
        return false;

    if (WRONG_NAME(name))
        return false;

    if (!m_attrs.size()) return true;

    switch (name) {

        case QicsCellStyle::CellDisplayer: {
                QicsCellDisplay *cd = (QicsCellDisplay*)cellAttr(row, col, name);
                if (cd)
                    cd->aboutToClear(0,-1,-1);
            }
        // fall through
        case QicsCellStyle::Font:
        case QicsCellStyle::SelectedFont:

        case QicsCellStyle::ForeColor:
        case QicsCellStyle::BackColor:
        case QicsCellStyle::SelForeColor:
        case QicsCellStyle::SelBackColor:
        case QicsCellStyle::WindowColor:
        case QicsCellStyle::WindowTextColor:
        case QicsCellStyle::EditForegroundColor:
        case QicsCellStyle::EditBackgroundColor:
        case QicsCellStyle::HighlightForeColor:
        case QicsCellStyle::HighlightBackColor:

        case QicsCellStyle::Formatter:
        case QicsCellStyle::Validator:
        case QicsCellStyle::PasteValidator:
        case QicsCellStyle::UserData:
        case QicsCellStyle::Pixmap:

        case QicsCellStyle::Label:
        case QicsCellStyle::ToolTipText:
        case QicsCellStyle::PixmapName:

        case QicsCellStyle::TopBorderPen:
        case QicsCellStyle::BottomBorderPen:
        case QicsCellStyle::LeftBorderPen:
        case QicsCellStyle::RightBorderPen:

        case QicsCellStyle::Alignment:
        case QicsCellStyle::TextFlags:
        case QicsCellStyle::CellMargin:
        case QicsCellStyle::BorderWidth:
        case QicsCellStyle::PixmapSpacing:
        case QicsCellStyle::MaxLength:

        case QicsCellStyle::AutoRecognition:
        case QicsCellStyle::CanAcceptDrops:
        case QicsCellStyle::ReadOnly:
        case QicsCellStyle::Enabled:

        case QicsCellStyle::Selected:
            clearCellAttr(row, col, name);
            return true;
    }

    return false;
}

void* QicsRegionalAttributeController::defaultProperty(int name)
{
    if (WRONG_NAME(name))
        return 0;

    // special case - font virtual attributes
    if (name >= QicsCellStyle::FontFamily && name <= QicsCellStyle::FontStretch) {
        QFont *f = (QFont*)defaultProperty(QicsCellStyle::Font);
        return QicsAttrCommon::fontAttr(&f, name);
    }

    return QicsCommonAttributeController::defaultProperty(name);
}

bool QicsRegionalAttributeController::setDefaultProperty(int name, const void *val)
{
    if (WRONG_NAME(name))
        return false;

    // special case - font virtual attributes
    if (name >= QicsCellStyle::FontFamily && name <= QicsCellStyle::FontStretch) {
        QFont *f = (QFont*)defaultProperty(QicsCellStyle::Font);
        QicsAttrCommon::applyFontAttr(&f, name, val);
        return true;
    }

    return QicsCommonAttributeController::setDefaultProperty(name, val);;
}


