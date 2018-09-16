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

#ifndef QICSEVALDIALOG_H
#define QICSEVALDIALOG_H

#include <QDateTime>

class QicsEvalDialog
{
public:
    static void checkEvalLicense(const char *which);

protected:
    enum Status {
        Accepted,
        Reject,
        Expired
    };

    static Status check(const QString &password, QDate *expireReturn);
};

#endif //QICSEVALDIALOG_H
