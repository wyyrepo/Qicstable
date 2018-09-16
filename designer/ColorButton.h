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

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>


class ColorButton: public QPushButton
{
    Q_OBJECT
public:
    ColorButton(QWidget* parent=0);

    inline QColor color() const {return m_color;}

public slots:
    void setColor(const QColor& color);

protected slots:
    void handleClicked();

signals:
    void colorChanged(const QColor&);

private:
    QColor m_color;
    QPixmap m_pixmap;
};

#endif //COLORBUTTON_H


