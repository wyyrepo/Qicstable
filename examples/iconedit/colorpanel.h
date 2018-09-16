/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef COLORPANEL_H
#define COLORPANEL_H

#include <QDockWidget>
#include <QicsTable.h>
#include <QicsDataModelDefault.h>

class QLineEdit;
class QToolButton;

class ColorPanel : public QDockWidget
{
    Q_OBJECT
public:
    ColorPanel(QWidget *parent = 0);
    ~ColorPanel();

    void setColor(const QColor &color);
    void setColorBG(const QColor &color);

    inline const QColor& color() const { return m_color; }
    inline const QColor& colorBG() const { return m_colorBG; }

private slots:
    void cellChanged(int,int);
    void cellClicked(int,int,int,const QPoint&);
    void pickColor();
    void pickColorBG();
    void textEditedColor();
    void textEditedColorBG();

private:
    void fillColors();

private:
    QicsTable *m_table;
    QicsDataModel *m_dm;
    QToolButton *m_tbColor;
    QToolButton *m_tbColorBG;
    QLineEdit *m_leColor;
    QLineEdit *m_leColorBG;
    QColor m_color;
    QColor m_colorBG;
};

#endif // COLORPANEL_H


