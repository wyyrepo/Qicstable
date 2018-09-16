/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "form.h"

#include <QicsDataModel.h>
#include "tetris.h"


Form::Form()
{
    setShape(NoShape);
    m_arrPoints = new QPoint[diameterForm];
    for (int i = 0; i < diameterForm; ++i) {
        m_arrPoints[i].setX(-1);
        m_arrPoints[i].setY(-1);
    }
}

Form::~Form()
{
    delete m_arrPoints;
    for (int i = 0; i < diameterForm; ++i)
        delete m_coords[i];

    delete m_coords;

    for (int i = 0; i < diameterForm; ++i)
        delete m_tmpRotateCoords[i];

    delete m_tmpRotateCoords;
}

// protected functions
void Form::setShape(Shape shape)
{
    m_tmpRotateCoords = new int *[diameterForm];
    for (int i = 0; i < diameterForm; ++i)
        m_tmpRotateCoords[i] = new int [2];

    m_coords = new int *[diameterForm];

    for (int i = 0; i < diameterForm; ++i)
        m_coords[i] = new int [2];

    static const int coordsTable[8][diameterForm][2] = {
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } },
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } },
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } },
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } },
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } }};

        for (int i = 0; i < diameterForm; i++)
            for (int j = 0; j < 2; ++j)
                m_coords[i][j] = coordsTable[shape][i][j];

        m_pieceShape = shape;
}

void Form::operator = (const Form& newForm)
{
    // think more and fix
    this->m_colorForm = newForm.m_colorForm;
    for (int i = 0; i < diameterForm; ++i) {
        this->m_coords[i][0] = newForm.m_coords[i][0];
        this->m_coords[i][1] = newForm.m_coords[i][1];
    }
}

int Form::minX() const
{
    int min = m_coords[0][0];
    for (int i = 1; i < diameterForm; ++i)
        min = qMin(min, m_coords[i][0]);

    return min;
}

int Form::maxX() const
{
    int max = m_coords[0][0];
    for (int i = 1; i < diameterForm; ++i)
        max = qMax(max, m_coords[i][0]);

    return max;
}

int Form::minY() const
{
    int min = m_coords[0][1];
    for (int i = 1; i < diameterForm; ++i)
        min = qMin(min, m_coords[i][1]);

    return min;
}

int Form::maxY() const
{
    int max = m_coords[0][1];
    for (int i = 1; i < diameterForm; ++i)
        max = qMax(max, m_coords[i][1]);

    return max;
}

// public functions
void Form::setRandomShape()
{
    setShape(Shape(qrand() % 7 + 1));
}

void Form::firstLocate()
{
    m_curX = countColumnsTableField / 2 - 1;
    m_curY = countRowsTableField - 1 + minY();
    fillArr();
}

void Form::firstLocateNextForm()
{
    m_curX = diameterForm / 2 - 1;
    m_curY = countRowsTableField - 1 + minY();
    fillArr();
}

bool Form::rotatedLeft(QicsDataModel *dataModelField)
{
    for (int i = 0; i < diameterForm; ++i) {
        // write rotated coords in tmp array
        m_tmpRotateCoords[i][0] = y(i);
        m_tmpRotateCoords[i][1] = -x(i);
    }
    // check rotate
    if (!checkOnMoveForm(dataModelField, false))
        return false;

    for (int i = 0; i < diameterForm; ++i) {
        // rotate real coords
        m_coords[i][0] = m_tmpRotateCoords[i][0];
        m_coords[i][1] = m_tmpRotateCoords[i][1];
    }
    fillArr();
    return true;
}

bool Form::rotatedRight(QicsDataModel *dataModelField)
{
    for (int i = 0; i < diameterForm; ++i) {
        // write rotated coords in tmp array
        m_tmpRotateCoords[i][0] = -y(i);
        m_tmpRotateCoords[i][1] = x(i);
    }
    // check rotate
    if (!checkOnMoveForm(dataModelField, false))
        return false;

    for (int i = 0; i < diameterForm; ++i) {
        // rotate real coords
        m_coords[i][0] = m_tmpRotateCoords[i][0];
        m_coords[i][1] = m_tmpRotateCoords[i][1];
    }
    fillArr();
    return true;
}

bool Form::moveDown(QicsDataModel *dataModelField)
{
    m_curY--;
    // check rotate
    if (!checkOnMoveForm(dataModelField, true)) {
        // rollback
        m_curY++;
        return false;
    }
    fillArr();
    return true;
}

bool Form::moveLeft(QicsDataModel *dataModelField)
{
    m_curX--;
    if (!checkOnMoveForm(dataModelField, true)) {
        // rollback
        m_curX++;
        return false;
    }
    fillArr();
    return true;
}

bool Form::moveRight(QicsDataModel *dataModelField)
{
    m_curX++;
    if (!checkOnMoveForm(dataModelField, true)) {
        // rollback
        m_curX--;
        return false;
    }
    fillArr();
    return true;
}

bool Form::checkOnMoveForm(QicsDataModel *dataModelField, bool whatCoords)
{
    fillArrForCheck(whatCoords);
    int x, y;
    for (int i = 0; i < diameterForm; ++i) {
        x = m_arrForCheck[i].x();
        y = m_arrForCheck[i].y();
        if (x >= countColumnsTableField || x < 0 || y >= countRowsTableField || y < 0 ||
            dataModelField->item(y, x)->string().toInt() == BUSY_CELL)
            return false;
    }
    return true;
}

void Form::fillArrForCheck(bool whatCoords)
{
    if (whatCoords) {
        for (int i = 0; i < diameterForm; ++i) {
            int col = m_curX + m_coords[i][0];
            int row = m_curY - m_coords[i][1];
            m_arrForCheck[i].setY(countRowsTableField - row - 1);
            m_arrForCheck[i].setX(col);
        }
    }
    else {
        for (int i = 0; i < diameterForm; ++i) {
            int col = m_curX + m_tmpRotateCoords[i][0];
            int row = m_curY - m_tmpRotateCoords[i][1];
            m_arrForCheck[i].setY(countRowsTableField - row - 1);
            m_arrForCheck[i].setX(col);
        }
    }
}

void Form::fillArr()
{
    for (int i = 0; i < diameterForm; ++i) {
        int col = m_curX + m_coords[i][0];
        int row = m_curY - m_coords[i][1];
        m_arrPoints[i].setY(countRowsTableField - row - 1);
        m_arrPoints[i].setX(col);
    }
}

void Form::setRandomColor(QColor* backColor)
{
    int r, g, b;
    do {
        r = qrand() % 256;
        g = qrand() % 256;
        b = qrand() % 256;
        m_colorForm.setRgb(r, g, b);
    }
    while (m_colorForm == *backColor);
}


