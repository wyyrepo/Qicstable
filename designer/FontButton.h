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

#ifndef FONTBUTTON_H
#define FONTBUTTON_H

#include <QPushButton>


class FontButton: public QPushButton
{
    Q_OBJECT
public:
    FontButton(QWidget* parent=0);

protected slots:
    void handleClicked();

signals:
    void fontChanged(const QFont&);
};

#endif //FONTBUTTON_H


