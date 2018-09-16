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

#ifndef QICSCOMMONATTRS_H
#define QICSCOMMONATTRS_H

#include <QColor>
#include <QFont>
#include <QCursor>
#include <QPen>
#include <QPixmap>

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

#define BOOL_INVALID	-1
#define INT_INVALID		-1
#define QRGB_INVALID	0xffffffff

class QicsAttrCommonData;


class QicsAttrCommon
{
public:
    QicsAttrCommon();
    virtual ~QicsAttrCommon();

    virtual void *attr(int name);
    virtual bool setAttr(int name, const void *val);
    virtual bool clearAttr(int name);

    static void applyFontAttr(QFont **font, int name, const void *val);
    static void *fontAttr(QFont **font, int name);

protected:
    // booleans require 2 bits: 00, 01 - value, 11 - invalid
    int bSelected : 2;
    int bReadOnly : 2;
    int bEnabled : 2;
    int bCanAcceptDrops : 2;
    int bAutoRecognition : 2;

    // integers
    int iAlignment : 9;
    int iCellMargin		: 16;
    int iPixmapSpacing	: 16;
    int iBorderWidth	: 16;
    int iMaxLength		: 16;
    int iTextFlags;

    // changing QColor to QRgb will reduce memory usage up to 25%
    QRgb colorFG;
    QRgb colorBG;
    QRgb colorFGSel;
    QRgb colorBGSel;
    QRgb colorFGEdit;
    QRgb colorBGEdit;
    QRgb colorFGHighSel;
    QRgb colorBGHighSel;

    // holding a pointer instead of QFont reduces memory usage
    QFont *font;
    QFont *selFont;

    // holding a pointer instead of QCursor
    QCursor	*cursor;

    // holding pointers instead of QPen
    QPen *penTop;
    QPen *penBottom;
    QPen *penLeft;
    QPen *penRight;

    static QicsAttrCommonData *d;
};


class QicsAttrCell : public QicsAttrCommon
{
public:
    QicsAttrCell();
    virtual ~QicsAttrCell();

    virtual void * attr(int name);
    virtual bool setAttr(int name, const void *val);
    virtual bool clearAttr(int name);

protected:
    QPixmap	*pixmap;
    QString	label;
    QString tooltip;
    QString pixname;

    void *displayer;
    void *formatter;
    void *validator;
    void *pasteValidator;
    void *userdata;
};

typedef QicsAttrCell QicsAttrRowCol;

#endif //QICSATTRS_H


