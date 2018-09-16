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

#include "QicsCommonAttrs.h"

#include "QicsCellStyle.h"
#include "QicsCellDisplay.h"
#include "QicsCommonAttrs_p.h"


QicsAttrCommonData *QicsAttrCommon::d;


QicsAttrCommon::QicsAttrCommon()
{
    if (!d) d = new QicsAttrCommonData();
    else d->ref();

    colorFG = colorBG = colorFGSel = colorBGSel = colorFGEdit = colorBGEdit = colorFGHighSel = colorBGHighSel = QRGB_INVALID;

    // !!! Order is important !!!
    iTextFlags = iAlignment = iCellMargin = iPixmapSpacing = iBorderWidth = iMaxLength = INT_INVALID;

    bSelected = bReadOnly = bEnabled = bCanAcceptDrops = bAutoRecognition = BOOL_INVALID;

    font = selFont = 0;
    cursor = 0;
    penTop = penBottom = penLeft = penRight = 0;
}

QicsAttrCommon::~QicsAttrCommon()
{
    if (d && d->deref()) {
        delete d;
        d = 0;
    }

    delete penTop;
    penTop = 0;
    delete penBottom;
    penBottom = 0;
    delete penLeft;
    penLeft = 0;
    delete penRight;
    penRight = 0;
    delete font;
    font = 0;
    delete selFont;
    selFont = 0;
    delete cursor;
    cursor = 0;
}

void* QicsAttrCommon::attr(int name)
{
    switch (name)
    {
    case QicsCellStyle::TopBorderPen:
        return penTop;
    case QicsCellStyle::BottomBorderPen:
        return penBottom;
    case QicsCellStyle::LeftBorderPen:
        return penLeft;
    case QicsCellStyle::RightBorderPen:
        return penRight;
    case QicsCellStyle::Cursor:
        return cursor;
    case QicsCellStyle::Font:
        return font;
    case QicsCellStyle::SelectedFont:
        return selFont;
    case QicsCellStyle::FontBold:
        if (!font) return 0;
        d->_boolean = font->bold();
        return &d->_boolean;
    case QicsCellStyle::FontItalic:
        if (!font) return 0;
        d->_boolean = font->italic();
        return &d->_boolean;
    case QicsCellStyle::FontOverline:
        if (!font) return 0;
        d->_boolean = font->overline();
        return &d->_boolean;
    case QicsCellStyle::FontUnderline:
        if (!font) return 0;
        d->_boolean = font->underline();
        return &d->_boolean;
    case QicsCellStyle::FontStrikeOut:
        if (!font) return 0;
        d->_boolean = font->strikeOut();
        return &d->_boolean;
    case QicsCellStyle::FontFixed:
        if (!font) return 0;
        d->_boolean = font->fixedPitch();
        return &d->_boolean;
    case QicsCellStyle::FontKerning:
        if (!font) return 0;
        d->_boolean = font->kerning();
        return &d->_boolean;
    case QicsCellStyle::FontStretch:
        if (!font) return 0;
        d->_integer = font->stretch();
        return &d->_integer;
    case QicsCellStyle::FontSize:
        if (!font) return 0;
        d->_integer = font->pointSize();
        return &d->_integer;
    case QicsCellStyle::FontFamily:
        if (!font) return 0;
        d->_string = font->family();
        return &d->_string;
    case QicsCellStyle::FontWeight:
        if (!font) return 0;
        d->_integer = font->weight();
        return &d->_integer;
    case QicsCellStyle::Selected:
        if (bSelected == BOOL_INVALID) return 0;
        d->_boolean = bSelected;
        return &d->_boolean;
    case QicsCellStyle::Enabled:
        if (bEnabled == BOOL_INVALID) return 0;
        d->_boolean = bEnabled;
        return &d->_boolean;
    case QicsCellStyle::ReadOnly:
        if (bReadOnly == BOOL_INVALID) return 0;
        d->_boolean = bReadOnly;
        return &d->_boolean;
    case QicsCellStyle::CanAcceptDrops:
        if (bCanAcceptDrops == BOOL_INVALID) return 0;
        d->_boolean = bCanAcceptDrops;
        return &d->_boolean;
    case QicsCellStyle::AutoRecognition:
        if (bAutoRecognition == BOOL_INVALID) return 0;
        d->_boolean = bAutoRecognition;
        return &d->_boolean;
    case QicsCellStyle::ForeColor:
        if (colorFG == QRGB_INVALID) return 0;
        d->_clr = QColor::fromRgba(colorFG);
        return &d->_clr;
    case QicsCellStyle::BackColor:
        if (colorBG == QRGB_INVALID) return 0;
        d->_clr = QColor::fromRgba(colorBG);
        return &d->_clr;
    case QicsCellStyle::SelForeColor:
        if (colorFGSel == QRGB_INVALID) return 0;
        d->_clr = QColor::fromRgba(colorFGSel);
        return &d->_clr;
    case QicsCellStyle::SelBackColor:
        if (colorBGSel == QRGB_INVALID) return 0;
        d->_clr = QColor::fromRgba(colorBGSel);
        return &d->_clr;
    case QicsCellStyle::EditBackgroundColor:
        if (colorBGEdit == QRGB_INVALID) return 0;
        d->_clr = QColor::fromRgba(colorBGEdit);
        return &d->_clr;
    case QicsCellStyle::EditForegroundColor:
        if (colorFGEdit == QRGB_INVALID) return 0;
        d->_clr = QColor::fromRgba(colorFGEdit);
        return &d->_clr;
    case QicsCellStyle::HighlightForeColor:
        if (colorFGHighSel == QRGB_INVALID) return 0;
        d->_clr = QColor::fromRgba(colorFGHighSel);
        return &d->_clr;
    case QicsCellStyle::HighlightBackColor:
        if (colorBGHighSel == QRGB_INVALID) return 0;
        d->_clr = QColor::fromRgba(colorBGHighSel);
        return &d->_clr;
    case QicsCellStyle::Alignment:
        if (iAlignment == INT_INVALID) return 0;
        d->_integer = iAlignment;
        return &d->_integer;
    case QicsCellStyle::TextFlags:
        if (iTextFlags == INT_INVALID) return 0;
        d->_integer = iTextFlags;
        return &d->_integer;
    case QicsCellStyle::CellMargin:
        if (iCellMargin == INT_INVALID) return 0;
        d->_integer = iCellMargin;
        return &d->_integer;
    case QicsCellStyle::BorderWidth:
        if (iBorderWidth == INT_INVALID) return 0;
        d->_integer = iBorderWidth;
        return &d->_integer;
    case QicsCellStyle::PixmapSpacing:
        if (iPixmapSpacing == INT_INVALID) return 0;
        d->_integer = iPixmapSpacing;
        return &d->_integer;
    case QicsCellStyle::MaxLength:
        if (iMaxLength == INT_INVALID) return 0;
        d->_integer = iMaxLength;
        return &d->_integer;
    }

    return 0;
}

void QicsAttrCommon::applyFontAttr(QFont **font, int name, const void *val)
{
    if (!*font) *font = new QFont();

    switch (name)
    {
    case QicsCellStyle::FontBold:
        (*font)->setBold(*((bool*)val));
        break;
    case QicsCellStyle::FontItalic:
        (*font)->setItalic(*((bool*)val));
        break;
    case QicsCellStyle::FontOverline:
        (*font)->setOverline(*((bool*)val));
        break;
    case QicsCellStyle::FontUnderline:
        (*font)->setUnderline(*((bool*)val));
        break;
    case QicsCellStyle::FontStrikeOut:
        (*font)->setStrikeOut(*((bool*)val));
        break;
    case QicsCellStyle::FontFixed:
        (*font)->setFixedPitch(*((bool*)val));
        break;
    case QicsCellStyle::FontKerning:
        (*font)->setKerning(*((bool*)val));
        break;
    case QicsCellStyle::FontStretch:
        (*font)->setStretch(*((int*)val));
        break;
    case QicsCellStyle::FontSize:
        (*font)->setPointSize(*((int*)val));
        break;
    case QicsCellStyle::FontFamily:
        (*font)->setFamily(*((QString*)val));
        break;
    case QicsCellStyle::FontWeight:
        (*font)->setWeight(*((int*)val));
        break;
    }
}

void *QicsAttrCommon::fontAttr(QFont **font, int name)
{
    if (!font || !(*font)) return 0;

    switch (name)
    {
    case QicsCellStyle::FontBold:
        d->_boolean = (*font)->bold();
        return &d->_boolean;
    case QicsCellStyle::FontItalic:
        d->_boolean = (*font)->italic();
        return &d->_boolean;
    case QicsCellStyle::FontOverline:
        d->_boolean = (*font)->overline();
        return &d->_boolean;
    case QicsCellStyle::FontUnderline:
        d->_boolean = (*font)->underline();
        return &d->_boolean;
    case QicsCellStyle::FontStrikeOut:
        d->_boolean = (*font)->strikeOut();
        return &d->_boolean;
    case QicsCellStyle::FontFixed:
        d->_boolean = (*font)->fixedPitch();
        return &d->_boolean;
    case QicsCellStyle::FontKerning:
        d->_boolean = (*font)->kerning();
        return &d->_boolean;
    case QicsCellStyle::FontStretch:
        d->_integer = (*font)->stretch();
        return &d->_integer;
    case QicsCellStyle::FontSize:
         d->_integer = (*font)->pointSize();
        return &d->_integer;
    case QicsCellStyle::FontWeight:
        d->_integer = (*font)->weight();
        return &d->_integer;
    case QicsCellStyle::FontFamily:
        d->_string = (*font)->family();
        return &d->_string;
    }

    return 0;
}

bool QicsAttrCommon::setAttr(int name, const void *val)
{
    if (name >= QicsCellStyle::FontFamily && name <= QicsCellStyle::FontStretch) {
        applyFontAttr(&font, name, val);
        return true;
    }

    switch (name)
    {
    case QicsCellStyle::TopBorderPen:
        if (!penTop) penTop = new QPen();
        *(penTop) = *(QPen*)val;
        break;
    case QicsCellStyle::BottomBorderPen:
        if (!penBottom) penBottom = new QPen();
        *(penBottom) = *(QPen*)val;
        break;
    case QicsCellStyle::LeftBorderPen:
        if (!penLeft) penLeft = new QPen();
        *(penLeft) = *(QPen*)val;
        break;
    case QicsCellStyle::RightBorderPen:
        if (!penRight) penRight = new QPen();
        *(penRight) = *(QPen*)val;
        break;
    case QicsCellStyle::Cursor:
        if (!cursor) cursor = new QCursor();
        *(cursor) = *(QCursor*)val;
        break;
    case QicsCellStyle::Font:
        if (!font) font = new QFont();
        *(font) = *(QFont*)val;
        break;
    case QicsCellStyle::SelectedFont:
        if (!selFont) selFont = new QFont();
        *(selFont) = *(QFont*)val;
        break;
    case QicsCellStyle::Selected:
        bSelected = *((bool*)val) ? 1 : 0;
        break;
    case QicsCellStyle::Enabled:
        bEnabled = *((bool*)val) ? 1 : 0;
        break;
    case QicsCellStyle::ReadOnly:
        bReadOnly = *((bool*)val) ? 1 : 0;
        break;
    case QicsCellStyle::CanAcceptDrops:
        bCanAcceptDrops = *((bool*)val) ? 1 : 0;
        break;
    case QicsCellStyle::AutoRecognition:
        bAutoRecognition = *((bool*)val) ? 1 : 0;
        break;
    case QicsCellStyle::ForeColor:
        colorFG = ((QColor*)val)->rgba();
        break;
    case QicsCellStyle::BackColor:
        colorBG = ((QColor*)val)->rgba();
        break;
    case QicsCellStyle::SelForeColor:
        colorFGSel = ((QColor*)val)->rgba();
        break;
    case QicsCellStyle::SelBackColor:
        colorBGSel = ((QColor*)val)->rgba();
        break;
    case QicsCellStyle::EditForegroundColor:
        colorFGEdit = ((QColor*)val)->rgba();
        break;
    case QicsCellStyle::EditBackgroundColor:
        colorBGEdit = ((QColor*)val)->rgba();
        break;
    case QicsCellStyle::HighlightForeColor:
        colorFGHighSel = ((QColor*)val)->rgba();
        break;
    case QicsCellStyle::HighlightBackColor:
        colorBGHighSel = ((QColor*)val)->rgba();
        break;
    case QicsCellStyle::Alignment:
        iAlignment = *(int*)val;
        break;
    case QicsCellStyle::TextFlags:
        iTextFlags = *(int*)val;
        break;
    case QicsCellStyle::CellMargin:
        iCellMargin = *(int*)val;
        break;
    case QicsCellStyle::BorderWidth:
        iBorderWidth = *(int*)val;
        break;
    case QicsCellStyle::PixmapSpacing:
        iPixmapSpacing = *(int*)val;
        break;
    case QicsCellStyle::MaxLength:
        iMaxLength = *(int*)val;
        break;
    }

    return true;
}

bool QicsAttrCommon::clearAttr(int name)
{
    switch (name)
    {
    case QicsCellStyle::TopBorderPen:
        if (penTop) {
            delete penTop;
            penTop = 0;
        }
        break;
    case QicsCellStyle::BottomBorderPen:
        if (penBottom) {
            delete penBottom;
            penBottom = 0;
        }
        break;
    case QicsCellStyle::LeftBorderPen:
        if (penLeft) {
            delete penLeft;
            penLeft = 0;
        }
        break;
    case QicsCellStyle::RightBorderPen:
        if (penRight) {
            delete penRight;
            penRight = 0;
        }
        break;
    case QicsCellStyle::Cursor:
        if (cursor) {
            delete cursor;
            cursor = 0;
        }
        break;
    case QicsCellStyle::SelectedFont:
        if (selFont) {
            delete selFont;
            selFont = 0;
        }
        break;
    case QicsCellStyle::Font:
        if (font) {
            delete font;
            font = 0;
        }
        break;
    case QicsCellStyle::FontBold:
        if (font) font->setBold(false);
        break;
    case QicsCellStyle::FontItalic:
        if (font) font->setItalic(false);
        break;
    case QicsCellStyle::FontUnderline:
        if (font) font->setUnderline(false);
        break;
    case QicsCellStyle::FontOverline:
        if (font) font->setOverline(false);
        break;
    case QicsCellStyle::FontStrikeOut:
        if (font) font->setStrikeOut(false);
        break;
    case QicsCellStyle::FontFixed:
        if (font) font->setFixedPitch(false);
        break;
    case QicsCellStyle::FontKerning:
        if (font) font->setKerning(false);
        break;
    case QicsCellStyle::Selected:
        bSelected = BOOL_INVALID;
        break;
    case QicsCellStyle::Enabled:
        bEnabled = BOOL_INVALID;
        break;
    case QicsCellStyle::ReadOnly:
        bReadOnly = BOOL_INVALID;
        break;
    case QicsCellStyle::CanAcceptDrops:
        bCanAcceptDrops = BOOL_INVALID;
        break;
    case QicsCellStyle::AutoRecognition:
        bAutoRecognition = BOOL_INVALID;
        break;
    case QicsCellStyle::ForeColor:
        colorFG = QRGB_INVALID;
        break;
    case QicsCellStyle::BackColor:
        colorBG = QRGB_INVALID;
        break;
    case QicsCellStyle::SelForeColor:
        colorFGSel = QRGB_INVALID;
        break;
    case QicsCellStyle::SelBackColor:
        colorBGSel = QRGB_INVALID;
        break;
    case QicsCellStyle::EditForegroundColor:
        colorFGEdit = QRGB_INVALID;
        break;
    case QicsCellStyle::EditBackgroundColor:
        colorBGEdit = QRGB_INVALID;
        break;
    case QicsCellStyle::HighlightForeColor:
        colorFGHighSel = QRGB_INVALID;
        break;
    case QicsCellStyle::HighlightBackColor:
        colorBGHighSel = QRGB_INVALID;
        break;
    case QicsCellStyle::Alignment:
        iAlignment = INT_INVALID;
        break;
    case QicsCellStyle::TextFlags:
        iTextFlags = INT_INVALID;
        break;
    case QicsCellStyle::CellMargin:
        iCellMargin = INT_INVALID;
        break;
    case QicsCellStyle::BorderWidth:
        iBorderWidth = INT_INVALID;
        break;
    case QicsCellStyle::PixmapSpacing:
        iPixmapSpacing = INT_INVALID;
        break;
    case QicsCellStyle::MaxLength:
        iMaxLength = INT_INVALID;
        break;
    }

    return true;
}


QicsAttrCell::QicsAttrCell()
    : QicsAttrCommon()
{
    pixmap = 0;

    userdata = 0;
    displayer = 0;
    formatter = 0;
    validator = pasteValidator = 0;
}

QicsAttrCell::~QicsAttrCell()
{
    delete pixmap;
    pixmap = 0;

    //delete userdata; // #### TODO: It should be fixed
    //userdata = 0;

    if (displayer) {
        ((QicsCellDisplay*)displayer)->aboutToClear(0,-1,-1);
        displayer = 0;
    }
}

void* QicsAttrCell::attr(int name)
{
    switch (name)
    {
    case QicsCellStyle::CellDisplayer:
        return displayer;
    case QicsCellStyle::Formatter:
        return formatter;
    case QicsCellStyle::Validator:
        return validator;
    case QicsCellStyle::PasteValidator:
        return pasteValidator;
    case QicsCellStyle::UserData:
        return userdata;
    case QicsCellStyle::Pixmap:
        return pixmap;
    case QicsCellStyle::Label:
        return label.isNull() ? 0 : &label;
    case QicsCellStyle::ToolTipText:
        return tooltip.isNull() ? 0 : &tooltip;
    case QicsCellStyle::PixmapName:
        return pixname.isNull() ? 0 : &pixname;
    }

    return QicsAttrCommon::attr(name);
}

bool QicsAttrCell::setAttr(int name, const void *val)
{
    switch (name)
    {
    case QicsCellStyle::CellDisplayer: {
            QicsCellDisplay *cd = (QicsCellDisplay*)displayer;
            if (cd)
                cd->aboutToClear(0,-1,-1);
        }
        displayer = const_cast<void *> (val);
        break;
    case QicsCellStyle::Formatter:
        formatter = const_cast<void *> (val);
        break;
    case QicsCellStyle::PasteValidator:
        pasteValidator = const_cast<void *> (val);
        break;
    case QicsCellStyle::Validator:
        validator = const_cast<void *> (val);
        break;
    case QicsCellStyle::UserData:
        userdata = const_cast<void *> (val);
        break;
    case QicsCellStyle::Label:
        label = *((QString*)val);
        break;
    case QicsCellStyle::ToolTipText:
        tooltip = *((QString*)val);
        break;
    case QicsCellStyle::PixmapName:
        pixname = *((QString*)val);
        break;
    case QicsCellStyle::Pixmap:
        if (!pixmap) pixmap = new QPixmap();
        *(pixmap) = *(QPixmap*)val;
        break;
    default:
        return QicsAttrCommon::setAttr(name, val);
    }

    return true;
}

bool QicsAttrCell::clearAttr(int name)
{
    switch (name)
    {
    case QicsCellStyle::CellDisplayer: {
            QicsCellDisplay *cd = (QicsCellDisplay*)displayer;
            if (cd)
                cd->aboutToClear(0,-1,-1);
        }
        displayer = 0;
        break;
    case QicsCellStyle::Formatter:
        formatter = 0;
        break;
    case QicsCellStyle::Validator:
        validator = 0;
        break;
    case QicsCellStyle::PasteValidator:
        pasteValidator = 0;
        break;
    case QicsCellStyle::UserData:
        userdata = 0;
        break;
    case QicsCellStyle::Label:
        label = QString();
        break;
    case QicsCellStyle::ToolTipText:
        tooltip = QString();
        break;
    case QicsCellStyle::PixmapName:
        pixname = QString();
        break;
    case QicsCellStyle::Pixmap:
        delete pixmap;
        pixmap = 0;
        break;
    default:
        return QicsAttrCommon::clearAttr(name);
    }

    return true;
}


