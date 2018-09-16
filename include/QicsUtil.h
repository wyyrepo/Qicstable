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

#ifndef QICSUTIL_H
#define QICSUTIL_H

#include <QPaintEngine>
#ifndef QICSTABLE_GPL
#include <QPrinter>
#endif
#include "QicsNamespace.h"
#ifdef QICSTABLE_GPL
#include "QicsGrid.h"
#endif
#ifndef QICSTABLE_GPL
#include "QicsPrintGrid.h"
#endif

class QPainter;
class QFont;

// debug
#ifdef __GNUC__
#define _D(__string) qDebug("file: '%s', %s { line:'%d': %s }", __FILE__, __PRETTY_FUNCTION__, __LINE__, #__string); __string;
#endif


/*! \file QicsUtil.h
*  \brief Utility functions and types
*
*  This file contains declarations of types and functions that
*  don't easily fit anywhere else.
*/

extern const char *Qics_arrow_xpm[];

/*!
* Dump dimension \a idx to debug output.
*/
#ifdef notdef

inline void dumpDimension(Qics::QicsIndexType idx,const char* add = "")
{
    switch (idx)
    {
    case Qics::RowAndColumnIndex:
        qDebug( "Qics::RowAndColumnIndex %s", add );
        break;
    case Qics::NoIndex:
        qDebug( "Qics::NoIndex %s", add );
        break;
    case Qics::RowIndex:
        qDebug( "Qics::RowIndex %s", add );
        break;
    case Qics::ColumnIndex:
        qDebug( "Qics::ColumnIndex %s", add );
        break;
    default:
        qDebug( "Qics::Unsupported %s", add );
        break;
    }
}

#endif

/*!
* sets or adds new \a addIdx dimension to \a idx if needed.
*/
inline void addDimension(Qics::QicsIndexType &idx, register Qics::QicsIndexType addIdx)
{
    switch (addIdx)
    {
    case Qics::RowAndColumnIndex:
        idx = Qics::RowAndColumnIndex;
        break;
    case Qics::NoIndex:
        // doing nothing if none dimension added
        break;
    case Qics::RowIndex:
        if (idx == Qics::ColumnIndex)
            idx = Qics::RowAndColumnIndex;
        else
            idx = addIdx;
        break;
    case Qics::ColumnIndex:
        if (idx == Qics::RowIndex)
            idx = Qics::RowAndColumnIndex;
        else
            idx = addIdx;
        break;
    default:
        break;
    }
}
/*!
* Returns true if \a painter device is a printer device
*/
inline bool isPrinterDevice(QPainter *painter, QicsGrid *grid)
{
#ifndef QICSTABLE_GPL
    return dynamic_cast<QPrinter*>(painter->device()) || dynamic_cast<QicsPrintGrid*>(grid);
#endif
#ifdef QICSTABLE_GPL
    Q_UNUSED(painter);
    Q_UNUSED(grid);
    return false;
#endif
}
/*!
* Returns the total height of this font (ascent + descent)
*/
QICS_EXPORT int qicsHeightOfFont(const QFont &fnt);

/*!
* Returns the width of the largest glyph of this font
*/
QICS_EXPORT int qicsWidthOfFont(const QFont &fnt);

/*!
* Returns the hash of data using MD5 method.
*/
QICS_EXPORT QByteArray qicsCryptData(const QByteArray &data);

#endif //QICSUTIL_H


