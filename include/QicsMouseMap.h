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

#ifndef QICSMOUSEMAP_H
#define QICSMOUSEMAP_H

#include <QMouseEvent>
#include "QicsNamespace.h"

/*!
* Class to provide mapping mouse buttons to other.
*/
class QicsMouseMap
{
public:
    QicsMouseMap();
    ~QicsMouseMap();
    /*!
    * This method set's emulate rule for mouse buttons.
    * In case Apple or 2 button mouse this can very helpfull.
    * Return true if succes, othervise false.
    * First parameter real mouse button \sa Qt::MouseButton
    * which used to emulate button; second modifier keyboard
    * key which pressed with real button,
    * third - needed result \sa Qt::MouseButton.
    */
    bool setMapMButton(Qt::MouseButton, Qt::Modifier, Qt::MouseButton);
    // setMapMButton with two modifiers.
    bool setMapMButton(Qt::MouseButton, Qt::Modifier, Qt::Modifier, Qt::MouseButton);
    /*!
    * Public method which verify pressed button. It compare inside class
    * buttons map's with button.
    */
    bool checkButton(Qt::MouseButton button, QMouseEvent* mouseEvent);
    // Not used, just for future
    Qt::MouseButton currentLeftButton();
    // Not used, just for future
    Qt::MouseButton currentMiddleButton();
    // Not used, just for future
    Qt::MouseButton currentRightButton();

    struct QicsButtonMap {
        Qt::MouseButton bt;
        Qt::Modifier mod;
        Qt::Modifier nextMod;
    } myLeftBt, myMidBt, myRightBt;
};

#endif //QICSMOUSEMAP_H


