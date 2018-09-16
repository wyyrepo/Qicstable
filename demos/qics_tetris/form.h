/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef FORM_INCLUDED
#define FORM_INCLUDED

#include <QColor>
#include <QPoint>

class QicsDataModel;

class Form
{
public:
    Form();
    ~Form();

    void operator = (const Form&);

    enum Shape { NoShape, ZShape, SShape, LineShape, TShape, SquareShape,
        LShape, MirroredLShape };
    inline Shape shape() const {return m_pieceShape;}

    // public functions
    void setRandomShape();
    void firstLocate();
    void firstLocateNextForm();

    bool rotatedLeft(QicsDataModel *dataModelField);
    bool rotatedRight(QicsDataModel *dataModelField);
    bool moveDown(QicsDataModel *dataModelField);
    bool moveLeft(QicsDataModel *dataModelField);
    bool moveRight(QicsDataModel *dataModelField);

    void setRandomColor(QColor* backColor);

    inline void setColor(QColor* color) {m_colorForm = *color;}
    inline QColor color() const {return m_colorForm;}

    inline QPoint* GetArrPoints() const {return m_arrPoints;}

protected:
    void setShape(Shape shape);
    //Shape shape() const;
    inline int x(int index) const {return m_coords[index][0];}
    inline int y(int index) const {return m_coords[index][1];}
    int minX() const;
    int maxX() const;
    int minY() const;
    int maxY() const;
    void fillArr();
    void fillArrForCheck(bool);
    bool checkOnMoveForm(QicsDataModel *, bool);

private:
    QColor m_colorForm;
    QPoint *m_arrPoints;
    QPoint m_arrForCheck[4];
    Shape m_pieceShape;
    int **m_coords;
    int **m_tmpRotateCoords;
    int m_curX;
    int m_curY;
};

#endif //FORM_INCLUDED


