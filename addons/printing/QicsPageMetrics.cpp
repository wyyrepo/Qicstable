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

#include <QicsPageMetrics.h>

#include <cmath>

///////////////////////////////////////////////////////////////////////////////
// Paper parameters
///////////////////////////////////////////////////////////////////////////////

const QicsPageMetrics QicsPageMetrics::paper[PAPER_LAST] = {
    QicsPageMetrics(	"A4",			210,	297		),
    QicsPageMetrics(	"B5",			182,	257		),
    QicsPageMetrics(	"Letter",		216,	279		),
    QicsPageMetrics(	"Legal",		216,	356		),
    QicsPageMetrics(	"Executive",	191,	254		),

    QicsPageMetrics(	"A0",			841,	1189	),
    QicsPageMetrics(	"A1",			594,	841		),
    QicsPageMetrics(	"A2",			420,	594		),
    QicsPageMetrics(	"A3",			297,	420		),
    QicsPageMetrics(	"A5",			148,	210		),
    QicsPageMetrics(	"A6",			105,	148		),
    QicsPageMetrics(	"A7",			 74,	105		),
    QicsPageMetrics(	"A8",			 52,	74		),
    QicsPageMetrics(	"A9",			 37,	52		),

    QicsPageMetrics(	"B0",			1030,	1456	),
    QicsPageMetrics(	"B1",			728,	1030	),
    QicsPageMetrics(	"B10",			 32,	45		),
    QicsPageMetrics(	"B2",			515,	728		),
    QicsPageMetrics(	"B3",			364,	515		),
    QicsPageMetrics(	"B4",			257,	364		),
    QicsPageMetrics(	"B6",			128,	182		),
    QicsPageMetrics(	"B7",			 91,	128		),
    QicsPageMetrics(	"B8",			 64,	91		),
    QicsPageMetrics(	"B9",			 45,	64		),

    QicsPageMetrics(	"C5 Envelope",			163,	229		),
    QicsPageMetrics(	"Comm10 Envelope",		105,	241		),
    QicsPageMetrics(	"DL Envelope",	110,	220		),
    QicsPageMetrics(	"Folio",		210,	330		),
    QicsPageMetrics(	"Ledger",		432,	279		),
    QicsPageMetrics(	"Tabloid",		279,	432		),

    QicsPageMetrics(	"",				0,		0		),

    QicsPageMetrics(	"ANSI C",		432,	559		),
    QicsPageMetrics(	"ANSI D",		559,	864		),
    QicsPageMetrics(	"ANSI E",		864,	1118	),
    QicsPageMetrics(	"ANSI F",		712,	1016	),

    QicsPageMetrics(	"ARCH A",		229,	305		),
    QicsPageMetrics(	"ARCH B",		305,	458		),
    QicsPageMetrics(	"ARCH C",		458,	610		),
    QicsPageMetrics(	"ARCH D",		610,	915		),
    QicsPageMetrics(	"ARCH E",		915,	1220	),
    QicsPageMetrics(	"ARCH E1",		762,	1067	),
    QicsPageMetrics(	"ARCH E2",		661,	965		),
    QicsPageMetrics(	"ARCH E3",		686,	991		),

    QicsPageMetrics(	"C5",			162,	229		),

    QicsPageMetrics(	"ISO B1",		707,	1000	),
    QicsPageMetrics(	"ISO B2",		500,	707		),
    QicsPageMetrics(	"ISO B3",		353,	500		),
    QicsPageMetrics(	"ISO B4",		250,	353		),
    QicsPageMetrics(	"ISO B5",		176,	250		),

    QicsPageMetrics(	"A0 Oversize",	900,	1245	),
    QicsPageMetrics(	"A1 Oversize",	625,	900		),
    QicsPageMetrics(	"A2 Oversize",	480,	625		),

    QicsPageMetrics(	"A2 Envelope",	111,	146		),
    QicsPageMetrics(	"C6 Envelope",	114,	162		),

    QicsPageMetrics(	"Hagaki Card",		100,	148		),
    QicsPageMetrics(	"Index Card 1",		102,	153		),
    QicsPageMetrics(	"Index Card 2",		127,	203		),
    QicsPageMetrics(	"Japanese Envelope 3",	120,	235		),
    QicsPageMetrics(	"Japanese Envelope 4",	90,		205		)
};

///////////////////////////////////////////////////////////////////////////////

QicsPageMetrics::QicsPageMetrics(const QString &n, double w, double h)
{
    name = n;
    mmWidth = w;
    mmHeight = h;
}

QString QicsPageMetrics::paperString(int ps)
{
    const QicsPageMetrics *pm = paperMetrics(ps);
    if (!pm) return QString::null;
    return QString("%1 (%2 x %3 mm)").arg(pm->name).arg(pm->mmWidth).arg(pm->mmHeight);
}

const QicsPageMetrics* QicsPageMetrics::paperMetrics(int ps)
{
    if (ps < 0 || ps == QPrinter::Custom || ps >= PAPER_LAST) return 0;
    return &paper[ps];
}

int QicsPageMetrics::matchPaper(double mmW, double mmH)
{
    if (mmW > mmH)
        qSwap(mmW, mmH);

    double w1 = ceil(mmW), w2 = floor(mmW), h1 = ceil(mmH), h2 = floor(mmH);

    for (int i = 0; i < PAPER_LAST; ++i) {
        const QicsPageMetrics &pm = paper[i];
        if (pm.mmWidth >= w2 && pm.mmWidth <= w1 && pm.mmHeight >= h2 && pm.mmHeight <= h1)
            return i;
    }

    return QPrinter::Custom;
}


