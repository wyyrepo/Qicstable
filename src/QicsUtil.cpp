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

#include "QicsUtil.h"


#include <QCryptographicHash>


// Pixmap for "more text"
const char * Qics_arrow_xpm[] = {
    "7 9 2 1",
    "       c white",
    ".      c black",
    "       ",
    " .     ",
    " ..    ",
    " ....  ",
    " ..... ",
    " ....  ",
    " ..    ",
    " .     ",
    "       "
};

int qicsHeightOfFont(const QFont &fnt)
{
    QFontMetrics fm(fnt);
    return fm.lineSpacing();
}

int qicsWidthOfFont(const QFont &fnt)
{
    QFontMetrics fm(fnt);
    QFontInfo fi(fnt);
    if (fi.fixedPitch())
        return fm.averageCharWidth();
    return fm.maxWidth();
}

QByteArray qicsCryptData(const QByteArray &data)
{
    return QCryptographicHash::hash(data ,QCryptographicHash::Md5).toHex();
}


