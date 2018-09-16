/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef PROGRESS_H
#define PROGRESS_H

#include <QMainWindow>
#include <QSlider>
#include <QicsWidgetCellDisplay.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    /*!
    *  Slot to process sliders value changes.
    */
    void on_sliderValueChanged(int);

private:
    class QicsTable *m_table;
};

/*!
 *  Subclassed class from QicsWidgetCellDisplay.
 *  Displays QSlider in the cell.
 */
class SliderDisplayer : public QicsWidgetCellDisplay
{
    Q_OBJECT

public:
    /*!
    *  Constructs a new SliderDisplayer object.
    */
    SliderDisplayer(QObject *parent, int min = 0, int max = 100, bool tracking = true)
    : QicsWidgetCellDisplay(m_slider = new QSlider(Qt::Horizontal), parent)
    {
        m_slider->setRange(min, max);
        m_slider->setTracking(tracking);
    }

    /*!
    *  Returns pointer to QSlider for this display object.
    */
    inline QSlider *slider() { return m_slider; }

private:
    QSlider *m_slider;
};


#endif //PROGRESS_H


