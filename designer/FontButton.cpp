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

#include "FontButton.h"

#include <QFontDialog>


FontButton::FontButton(QWidget* parent)
    : QPushButton(parent)
{
    connect( this, SIGNAL(clicked()), SLOT(handleClicked()));
}

void FontButton::handleClicked()
{
    bool ok;
    QFont newFont = QFontDialog::getFont(&ok, font(), this);

    if(ok) {
        setFont(newFont);
        emit fontChanged(font());
    }
}


