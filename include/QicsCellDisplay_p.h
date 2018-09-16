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

#ifndef QICSCELLDISPLAY_P_H
#define QICSCELLDISPLAY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QicsTable API.  It exists for the convenience
// of the QicsCellDisplay class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "QicsShared.h"

class QLineEdit;
class QStyle;
class QicsGridInfo;

class QicsCellDisplayData : public QicsShared
{
public:
    QicsCellDisplayData()
        : QicsShared()
    {
        for_printer = false;
        is_enabled = false;
        is_current = false;
        is_selected = false;
        alignment = -1;
        ginfo = 0;
        style_flags = QStyle::State_None;
        the_style = 0;
    }

    ~QicsCellDisplayData()
    {
    }

    bool for_printer;
    bool is_enabled;
    bool is_current;
    bool is_selected;
    int alignment;
    QicsGridInfo *ginfo;
    QStyle::State style_flags;
    QPalette pal;
    QRect cr;
    QColor fg;
    QColor bg;
    QStyle *the_style;
    QString qs;
};

class QicsTextCellDisplayData : public QicsShared
{
public:
    QicsTextCellDisplayData()
        : QicsShared(),
        lineEdit(0)
    {
    }

    ~QicsTextCellDisplayData()
    {
    }

    QLineEdit *lineEdit;
};

#endif //QICSCELLDISPLAY_P_H




