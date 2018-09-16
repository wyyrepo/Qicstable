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

#ifndef QICSCOMMONATTRS_P_H
#define QICSCOMMONATTRS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QicsTable API.  It exists for the convenience
// of the QicsAttrCommon class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//


#include "QicsShared.h"


class QicsAttrCommonData : public QicsShared
{
public:
    QicsAttrCommonData() : QicsShared() {}
    ~QicsAttrCommonData() {}

    bool _boolean;
    int _integer;
    QColor _clr;
    QFont _font;
    QString _string;
    QCursor _cursor;
    QPen _pen;
};

#endif //QICSCOMMONATTRS_P_H


