/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef VIEWSETTINGS_H
#define VIEWSETTINGS_H

#include <QColor>

class ViewSettings
{
public:
    ViewSettings();

    enum Displayer {TEXT = 0, CHECK, COMBO, BUTTON};

    Displayer getDisplayerForm() const {return m_displayerForm;}
    void setDisplayerForm(Displayer d) {m_displayerForm = d;}

    inline QColor getGridColor() const {return m_gridColor;}
    inline void setGridColor(QColor color) {m_gridColor = color;}

    inline QColor getFieldColor() const {return m_fieldColor;}
    inline void setFieldColor(QColor color) {m_fieldColor = color;}

    inline QColor getFigureColor() const {return m_figureColor;}
    inline void setFigureColor(QColor color) {m_figureColor = color;}

    inline bool getIsRandom() const {return m_isRandom;}
    inline void setIsRandom(bool r) {m_isRandom = r;}

protected:
    QColor m_gridColor;
    QColor m_fieldColor;
    QColor m_figureColor;
    Displayer m_displayerForm;
    bool m_isRandom;
};

#endif //VIEWSETTINGS_H


