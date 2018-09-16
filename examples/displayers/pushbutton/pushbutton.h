/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    /*!
    *  Slot to process cell's value changes.
    */
    void on_cellValueChanged(int row, int col);

private:
    /*!
    *  Returns a QRgb color value for the given predefined index.
    */
    QRgb colorValue(int index);

    class QicsTable *m_table;
    QStringList m_colors;
};


#endif //PUSHBUTTON_H


