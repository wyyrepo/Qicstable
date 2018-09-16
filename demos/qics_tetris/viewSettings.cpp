/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "viewSettings.h"


ViewSettings::ViewSettings()
{
    // init components
    m_gridColor.setRgb(255, 255, 255);
    m_fieldColor.setRgb(255, 255, 255);
    m_figureColor.setRgb(0, 0, 0);
    m_isRandom = true;
    m_displayerForm = TEXT;
}


