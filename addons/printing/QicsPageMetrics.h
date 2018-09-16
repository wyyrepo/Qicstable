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

#ifndef QICSPAGEMETRICS_H
#define QICSPAGEMETRICS_H

#include <QPrinter>

#include <QicsNamespace.h>

///////////////////////////////////////////////////////////////////////////////
// Paper parameters
///////////////////////////////////////////////////////////////////////////////

enum QicsPageSize {
    PAPER_CUSTOM = QPrinter::Custom,
    ANSI_C,
    ANSI_D,
    ANSI_E,
    ANSI_F,
    ARCH_A,
    ARCH_B,
    ARCH_C,
    ARCH_D,
    ARCH_E,
    ARCH_E1,
    ARCH_E2,
    ARCH_E3,
    C5,
    ISO_B1,
    ISO_B2,
    ISO_B3,
    ISO_B4,
    ISO_B5,
    A0_OVERSIZE,
    A1_OVERSIZE,
    A2_OVERSIZE,
    A2_ENV,
    C6_ENV,
    HAGAKI,
    INDEX1,
    INDEX2,
    JAP3_ENV,
    JAP4_ENV,
    PAPER_LAST
};

///////////////////////////////////////////////////////////////////////////////
// QicsPageMetrics
///////////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsPageMetrics
{
public:
    QicsPageMetrics(const QString &n, double w, double h);

    static QString paperString(int paperID);
    static const QicsPageMetrics* paperMetrics(int paperID);

    static int matchPaper(double mmW, double mmH);

    QString name;
    double mmWidth;
    double mmHeight;

private:
    static const QicsPageMetrics paper[PAPER_LAST];
};

#endif //QICSPAGEMETRICS_H


