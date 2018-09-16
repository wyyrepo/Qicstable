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

#include "QicsMouseMap.h"


QicsMouseMap::QicsMouseMap()
{
    setMapMButton( Qt::LeftButton, Qt::UNICODE_ACCEL, Qt::LeftButton );
    setMapMButton( Qt::RightButton, Qt::UNICODE_ACCEL, Qt::RightButton );
    setMapMButton( Qt::MidButton, Qt::UNICODE_ACCEL, Qt::MidButton );
}

QicsMouseMap::~QicsMouseMap( )
{
}

bool QicsMouseMap::setMapMButton( Qt::MouseButton realButton, Qt::Modifier mod, Qt::Modifier nextMod, Qt::MouseButton emulButton)
{
    switch (emulButton)
    {
    case Qt::LeftButton :
        myLeftBt.bt = realButton;
        myLeftBt.mod = mod;
        myLeftBt.nextMod = nextMod;
        break;
    case Qt::MidButton :
        myMidBt.bt = realButton;
        myMidBt.mod = mod;
        myMidBt.nextMod = nextMod;
        break;
    case Qt::RightButton :
        myRightBt.bt = realButton;
        myRightBt.mod = mod;
        myRightBt.nextMod = nextMod;
        break;
    case Qt::NoButton:
    case Qt::XButton1:
    case Qt::XButton2 :
    case Qt::MouseButtonMask:
        break;
    default:
        break;
    }
    return true;
}

bool QicsMouseMap::setMapMButton( Qt::MouseButton realButton, Qt::Modifier mod,
                                 Qt::MouseButton emulButton )
{
    switch (emulButton)
    {
    case Qt::LeftButton :
        myLeftBt.bt = realButton;
        myLeftBt.mod = mod;
        break;
    case Qt::MidButton :
        myMidBt.bt = realButton;
        myMidBt.mod = mod;
        break;
    case Qt::RightButton :
        myRightBt.bt = realButton;
        myRightBt.mod = mod;
        break;
    case Qt::NoButton:
    case Qt::XButton1:
    case Qt::XButton2 :
    case Qt::MouseButtonMask:
        break;
    default:
        break;
    }
    return true;
}

bool QicsMouseMap::checkButton( Qt::MouseButton buttonID,
                               QMouseEvent * mouseEvent)
{

    switch (buttonID)
    {
    case Qt::LeftButton: {
            if ((mouseEvent->button() == myLeftBt.bt )||(mouseEvent->buttons() & myLeftBt.bt)) {
                if (myLeftBt.mod != Qt::UNICODE_ACCEL) {
                    if (mouseEvent->modifiers() & myLeftBt.mod)
                        return true;

                    return false;
                }
                return true;
            }
        }
        break;
    case Qt::MidButton: {
            if (mouseEvent->buttons() & myMidBt.bt ) {
                if (myMidBt.mod != Qt::UNICODE_ACCEL) {
                    if (mouseEvent->modifiers() & myMidBt.mod)
                        return true;
                    return false;
                }
                return true;
            }
        }
        break;
    case Qt::RightButton: {
            if (myRightBt.bt == mouseEvent->button()) {
                if (myRightBt.mod != Qt::UNICODE_ACCEL) {
                    if (mouseEvent->modifiers() & myRightBt.mod)
                        return true;
                    return false;
                }
                return true;
            }
        }
        break;
    case Qt::NoButton:
    case Qt::XButton1:
    case Qt::XButton2 :
    case Qt::MouseButtonMask:
        break;
    default:
        break;
    }
    return false;
}

Qt::MouseButton QicsMouseMap::currentLeftButton()
{
    return myLeftBt.bt;
}

Qt::MouseButton QicsMouseMap::currentRightButton()
{
    return myRightBt.bt;
}

Qt::MouseButton QicsMouseMap::currentMiddleButton()
{
    return myMidBt.bt;
}


