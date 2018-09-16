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

#include "QicsScrollBarScroller.h"


QicsScrollBarScroller::QicsScrollBarScroller(Qics::QicsIndexType type,
                                             QWidget *parent)
{
    myType = type;
    myInSetIndex = false;
    myMode = Auto;
    myIsSliderPressed = false;
    oldVal = -1;

    Qt::Orientation orientation = (type == RowIndex ? Qt::Vertical : Qt::Horizontal);
    myScrollBar = new QScrollBar(orientation, parent);

    connect(myScrollBar, SIGNAL(valueChanged(int)),
        this, SLOT(valueChanged(int)));
    connect(myScrollBar, SIGNAL(sliderPressed()),
        this, SLOT(handleSliderPressed()));
    connect(myScrollBar, SIGNAL(sliderReleased()),
        this, SLOT(handleSliderReleased()));
    connect(myScrollBar, SIGNAL(actionTriggered(int)),
        this, SLOT(handleActionTriggered(int)));
}

QicsScrollBarScroller::~QicsScrollBarScroller()
{
    delete myScrollBar;
    myScrollBar = 0;
}

void QicsScrollBarScroller::setIndex(Qics::QicsIndexType type, int idx)
{
    if (type == myType) {
        // We use this flag so we don't emit the valueChanged() signal
        // in this case.  The scroll manager already knows about this
        // index changed, because it's the one that called this method.
        // myInSetIndex = true;
        if (idx > myScrollBar->maximum())
            idx = myScrollBar->maximum();

        myInSetIndex = true;

        bool blocked = myScrollBar->blockSignals(true);

        if (myIsSliderPressed)
            myScrollBar->setTracking(false);

        myScrollBar->setValue(idx);

        if (myIsSliderPressed)
            myScrollBar->setTracking(true);

        myScrollBar->blockSignals(blocked);

        // This is temporary workaround for bug #592 for forced repainting of scrollbar.
#ifdef Q_OS_WIN32
        myScrollBar->repaint();
#endif
        myInSetIndex = false;
    }
}

void QicsScrollBarScroller::setMinIndex(Qics::QicsIndexType type, int idx)
{
    if (type == myType) {
        myScrollBar->setMinimum(idx);
        checkBounds();
    }
}

void QicsScrollBarScroller::setMaxIndex(Qics::QicsIndexType type, int idx)
{
    if (type == myType) {
        myScrollBar->setMaximum(idx);
        checkBounds();
    }
}

void QicsScrollBarScroller::setMode(QicsScrollBarMode m)
{
    myMode = m;
    checkBounds();
}

void QicsScrollBarScroller::valueChanged(int val)
{
    if (!myInSetIndex && (val >= 0))
        emit indexChanged(myType, val);
}

void QicsScrollBarScroller::checkBounds()
{
    bool show;

    if (myMode == AlwaysOn)
        show = true;
    else if (myMode == AlwaysOff)
        show = false;
    else
        show = (myScrollBar->minimum() < myScrollBar->maximum());

    if (show)
        myScrollBar->show();
    else
        myScrollBar->hide();
}

void QicsScrollBarScroller::setWidget(QAbstractSlider* widget)
{
    if(myScrollBar && myScrollBar != widget) {
        delete myScrollBar;
        myScrollBar = widget;
    }
    myScrollBar->hide();
    connect(myScrollBar, SIGNAL(valueChanged(int)),
        this, SLOT(valueChanged(int)));
}

void QicsScrollBarScroller::handleActionTriggered(int action)
{
    switch (action) {
    case QAbstractSlider::SliderPageStepAdd:
        myScrollBar->setSliderPosition(myScrollBar->value());
        emit scrollPageAdd(myType, myScrollBar->value());
        break;
    case QAbstractSlider::SliderPageStepSub:
        myScrollBar->setSliderPosition(myScrollBar->value());
        emit scrollPageSub(myType, myScrollBar->value());
        break;
    default:
        break;
    }
}


