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

#include "QicsStyle.h"

#include <QPalette>
#include <QCursor>
#include "QicsUtil.h"
#include "QicsMouseMap.h"
#include "QicsRegion.h"
#include "QicsCellDisplay.h"


QicsStyle::QicsStyle()
    : myStyleTypeList(0), myNumProperties(0), mySetCount(0)
{
    myProperties.clear();
}

void QicsStyle::init()
{
    myProperties.resize(myNumProperties);
    myProperties.fill(0);
}

QicsStyle::~QicsStyle()
{
    clear();
    myProperties.clear();
}

void QicsStyle::setValue(int prop, const void *val)
{
    if (prop >= myNumProperties) return;

    // Clear the old value first
    clear(prop);

    // If nothing to set, then bail out
    if (!val) return;

    void *newval = 0;

    switch(myStyleTypeList->at(prop))
    {
    case QicsT_Int:
        newval = new int(*(static_cast<const int *> (val)));
        break;
    case QicsT_Float:
        newval = new float(*(static_cast<const float *>(val)));
        break;
    case QicsT_QString:
        newval = new QString(*(static_cast<const QString *>(val)));
        break;
    case QicsT_QColor:
        newval = new QColor(*(static_cast<const QColor *>(val)));
        break;
    case QicsT_QPoint:
        newval = new QPoint(*(static_cast<const QPoint *>(val)));
        break;
    case QicsT_Boolean:
        newval = new bool(*(static_cast<const bool *> (val)));
        break;
    case QicsT_QFont:
        newval = new QFont(*(static_cast<const QFont *> (val)));
        break;
    case QicsT_QPalette:
        break;
    case QicsT_QCursor:
        newval = new QCursor(*(static_cast<const QCursor *> (val)));
        break;
    case QicsT_QPixmap:
        newval = new QPixmap(*(static_cast<const QPixmap *> (val)));
        break;
    case QicsT_QPen:
        newval = new QPen(*(static_cast<const QPen *> (val)));
        break;
    case QicsT_QicsRegion:
        newval = new QicsRegion(*(static_cast<const QicsRegion *> (val)));
        break;
    case QicsT_QicsCellDisplay: {
            QicsCellDisplay *cd = (QicsCellDisplay*)myProperties.at(prop);
            if (cd)
                cd->aboutToClear(0,-1,-1);
        }
        // fall through
    case QicsT_Pointer:
    case QicsT_QicsMouseMap:
    case QicsT_PasteValidator:
    case QicsT_QValidator:
    case QicsT_QicsDataItemFormatter:
    case QicsT_QWidget:
        // We don't copy this
        newval = const_cast<void *> (val);
        break;

    default:
        qWarning("QicsStyle::setValue: invalid property %d", prop);
    }

    myProperties[prop] = newval;
    ++mySetCount;
}

void QicsStyle::clear()
{
    for (int i = 0; i < myNumProperties; ++i)
        clear(i);
}

void QicsStyle::clear(int prop)
{
    if (prop >= myNumProperties || !getValue(prop))
        return;

    switch(myStyleTypeList->at(prop))
    {
    case QicsT_Int:
        delete (static_cast<int *>(myProperties.at(prop)));
        break;
    case QicsT_Float:
        delete (static_cast<float *>(myProperties.at(prop)));
        break;
    case QicsT_QString:
        delete (static_cast<QString *>(myProperties.at(prop)));
        break;
    case QicsT_QColor:
        delete (static_cast<QColor *>(myProperties.at(prop)));
        break;
    case QicsT_QPoint:
        delete (static_cast<QPoint *>(myProperties.at(prop)));
        break;
    case QicsT_Boolean:
        delete (static_cast<bool *> (myProperties.at(prop)));
        break;
    case QicsT_QFont:
        delete (static_cast<QFont *> (myProperties.at(prop)));
        break;
    case QicsT_QPalette:
        delete (static_cast<QPalette *> (myProperties.at(prop)));
        break;
    case QicsT_QCursor:
        delete (static_cast<QCursor *> (myProperties.at(prop)));
        break;
    case QicsT_QPixmap:
        delete (static_cast<QPixmap *> (myProperties.at(prop)));
        break;
    case QicsT_QPen:
        delete (static_cast<QPen *> (myProperties.at(prop)));
        break;
    case QicsT_QicsRegion:
        delete (static_cast<QicsRegion *> (myProperties.at(prop)));
        break;
    case QicsT_QicsMouseMap:
        delete (static_cast<QicsMouseMap *> (myProperties.at(prop)));
        break;
    case QicsT_Pointer:
    case QicsT_PasteValidator:
    case QicsT_QValidator:
    case QicsT_QicsDataItemFormatter:
    case QicsT_QWidget:
        // We don't free this
        break;
    case QicsT_QicsCellDisplay: {
            QicsCellDisplay *cd = static_cast<QicsCellDisplay *>(myProperties.at(prop));
            if (cd)
                cd->aboutToClear(0,-1,-1);
        }
        break;
    default:
        qWarning("QicsStyle::clear: invalid property %d", prop);
    };
    myProperties[prop] = 0;

    --mySetCount;
}

QString QicsStyle::penToString(const QPen *pen)
{
    // color, width, style, join style
    if (!pen) return QString();

    QStringList listOfProperties;
    listOfProperties << pen->color().name();
    listOfProperties << QString::number(pen->width());
    listOfProperties << QString::number(pen->style());
    listOfProperties << QString::number(pen->joinStyle());
    return listOfProperties.join(",");
}

QPen *QicsStyle::stringToPen(const QString &penSetings)
{
    if(penSetings.isEmpty()) return 0;

    QStringList listOfProperties;
    listOfProperties=penSetings.split(",");

    QColor color;
    color.setNamedColor(listOfProperties.at(0));
    QPen * pen = new QPen(color);
    pen->setWidth(listOfProperties.at(1).toInt());
    pen->setStyle(Qt::PenStyle(listOfProperties.at(2).toInt()));
    pen->setJoinStyle(Qt::PenJoinStyle(listOfProperties.at(3).toInt()));
    return pen;
}

QString QicsStyle::paletteToString(const QPalette *pal)
{
    // Foreground, Text, Background, Base, HighlightedText, Highlight
    if (!pal) return QString();

    QStringList listOfColors;
    listOfColors << pal->color(QPalette::WindowText).name();
    listOfColors << pal->color(QPalette::Text).name();
    listOfColors << pal->color(QPalette::Window).name();
    listOfColors << pal->color(QPalette::Base).name();
    listOfColors << pal->color(QPalette::HighlightedText).name();
    listOfColors << pal->color(QPalette::Highlight).name();
    return listOfColors.join(",");
}

QPalette *QicsStyle::stringToPalette(const QString &paletteSetings)
{
    if(paletteSetings.isEmpty()) return 0;

    QStringList listOfColors;
    listOfColors=paletteSetings.split(",");

    QColor color;
    QPalette * pal = new QPalette;
    color.setNamedColor(listOfColors.at(0));
    pal->setColor(QPalette::WindowText, color);
    color.setNamedColor(listOfColors.at(1));
    pal->setColor(QPalette::Text, color);
    color.setNamedColor(listOfColors.at(2));
    pal->setColor(QPalette::Window, color);
    color.setNamedColor(listOfColors.at(3));
    pal->setColor(QPalette::Base, color);
    color.setNamedColor(listOfColors.at(4));
    pal->setColor(QPalette::HighlightedText, color);
    color.setNamedColor(listOfColors.at(5));
    pal->setColor(QPalette::Highlight, color);
    return pal;
}


