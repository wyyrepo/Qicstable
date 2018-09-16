/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "QicsPushButtonHeaderDisplay.h"

#include <QStyle>
#include <QStyleOptionButton>
#include <QPainter>
#include <QPushButton>
#include <QApplication>

#if QT_VERSION < 0x050000
#ifdef Q_OS_WIN32
#include <QWindowsXPStyle>
#endif
#include <QPlastiqueStyle>
#include <QWindowsStyle>
#else
#include <QCommonStyle>
#endif


// This is an example of custom header displayer using.
// It may be used for better look of headers.
// We only have to reimplement drawBackground() method,
// that will draw PushButton control instead of Header
// control.

QicsPushButtonHeaderDisplay::QicsPushButtonHeaderDisplay(QWidget *parent)
    : QicsTextCellDisplay(parent)
{
}

QicsPushButtonHeaderDisplay::~QicsPushButtonHeaderDisplay()
{
}

void QicsPushButtonHeaderDisplay::drawBackground(QicsGridInfo *info, int row, int col,
                                                 const QRect &rect, const QPalette &pal,
                                                 QPainter *painter,
                                                 bool is_current, bool is_selected)
{
    QStyle *style =  QApplication::style();

#if QT_VERSION < 0x050000
    if (qobject_cast<QWindowsStyle*>(style)) {
#else
    if (qobject_cast<QCommonStyle*>(style)) {
#endif
        QColor bg;
        QStyleOptionButton ccOptions;
        ccOptions.rect = rect;
        ccOptions.palette = pal;
        ccOptions.features |= QStyleOptionButton::DefaultButton;

        if (is_current || !is_selected) {
            bg = pal.button().color();
            ccOptions.palette.setBrush(QPalette::Button, pal.background());
            ccOptions.state = QStyle::State_Raised;
        }
        else {
            bg = pal.highlight().color();
            ccOptions.palette.setBrush(QPalette::Button, pal.highlight());
            ccOptions.palette.setBrush(QPalette::Window, pal.highlight());
            ccOptions.state = QStyle::State_Sunken;
        }

        painter->fillRect(rect, bg);

#if QT_VERSION < 0x050000
#ifdef Q_OS_WIN32
        // If current style is WindowsXP style we are using
        // Plastique style because it seems that WindowsXP style
        // does not react to a widget palette changing(it using
        // inner WindowsXP style engine).
        bool style_needs_to_be_deleted = false;

        if(qobject_cast<QWindowsXPStyle*>(style)) {
            style = new QPlastiqueStyle();
            style_needs_to_be_deleted = true;
        }
#endif
#endif

        style->drawControl(QStyle::CE_PushButton,&ccOptions, painter, 0);

#if QT_VERSION < 0x050000
#ifdef Q_OS_WIN32
        if(style_needs_to_be_deleted)
            delete style;
#endif
#endif
    }
    else {
        QicsTextCellDisplay::drawBackground(info,row,col,rect,pal,painter,is_current,is_selected);
    }
}


