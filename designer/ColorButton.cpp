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

#include "ColorButton.h"

#include <QColorDialog>


ColorButton::ColorButton(QWidget* parent)
    : QPushButton(parent),
        m_color(Qt::black),
        m_pixmap(150,150)
{
    m_pixmap.fill(m_color);
    setIcon(m_pixmap);

    connect(this, SIGNAL(clicked()), SLOT(handleClicked()));
}

void ColorButton::setColor(const QColor& color)
{
    m_color = color;
    m_pixmap.fill(m_color);
    setIcon(m_pixmap);
    emit colorChanged(m_color);
}

void ColorButton::handleClicked()
{
    QColor newColor = QColorDialog::getColor(m_color, this);

    if(newColor.isValid())
        setColor(newColor);
}


