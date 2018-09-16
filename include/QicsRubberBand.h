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

#ifndef QICSRUBBERBAND_H
#define QICSRUBBERBAND_H

#include <QRubberBand>
#include <QPen>

///////////////////////////////////////////////////////////////////////////

/*! \class QicsRubberBand QicsRubberBand.h
 * \nosubgrouping
 * \brief QRubberBand with configurable attributes.
 * QicsRubberBand is the subclass of QRubberBand which allows to configure
 * color, pen and opacity of the band.

 * QicsRubberBand can be used also as standalone widget inside your own
 * application.

 * There are two modes which can be set via setDrawingMode() method:
 * with DM_STANDARD QicsRubberBand will act exactly as standard QRubberBand,
 * being drawn with current system selection style. In DM_CUSTOM mode,
 * drawing can be customized.

 * To change frame's pen, use setPen(). To change brush of the frame (i.e. filling),
 * use setBrush(). To change frame's opacity, use setOpacity() method.

 */

 ////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QicsRubberBand : public QRubberBand
{
    Q_OBJECT
public:
    /*! Denotes drawing mode of the frame.
     * \arg \b DM_STANDARD QicsRubberBand will act exactly as standard QRubberBand,
     * being drawn with current system selection style.
     * \arg \b DM_CUSTOM All the drawing can be customized.
     */
    enum DrawingMode {
        DM_STANDARD,
        DM_CUSTOM
    };

    QicsRubberBand(Shape s, QWidget *parent = 0);
    virtual ~QicsRubberBand();

    /*! Sets \a pen of the frame.
    */
    inline void setPen(const QPen &pen) {m_pen = pen;}
    /*! Returns pen of the frame.
    */
    inline QPen pen() const { return m_pen; }

    /*! Sets \a brush of the frame.
    */
    inline void setBrush(const QBrush &brush) {m_brush = brush;}
    /*! Returns brush of the frame.
    */
    inline QBrush brush() const { return m_brush; }

    /*! Sets opacity of the frame.
    * \a op must be in ranges 0.0 (full transparency) to 1.0 (full opacity).
    * Default value is 0.5.
    */
    void setOpacity(qreal op);
    /*! Returns opacity of the frame.
    */
    inline qreal opacity() const { return m_opacity; }

    /*!
    * Sets drawing mode of the frame to \a mode.
    * Default value is QicsRubberBand::DM_CUSTOM.
    */
    inline void setDrawingMode(DrawingMode mode) { m_dm = mode; }
    /*!
    * Returns drawing mode of the frame.
    */
    inline DrawingMode drawingMode() const { return m_dm; }

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QPen m_pen;
    QBrush m_brush;
    qreal m_opacity;
    DrawingMode m_dm;
};

#endif //QICSRUBBERBAND_H


