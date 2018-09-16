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

#ifndef QICSRULER_H
#define QICSRULER_H

#include <QFrame>

#include <QicsNamespace.h>

///////////////////////////////////////////////////////////////////////////////
// QicsRuler
///////////////////////////////////////////////////////////////////////////////

/*! \class QicsRuler QicsRuler.h
* \nosubgrouping
* \brief Ruler widget is intended to show size of some object/part of screen in metric system.

Ruler is visual-only class which helps identify a size in metrical units rather then in pixels.
It can be helpful when dealing with objects which should be measured in millimeters/centimeters, for
exapmle, page size while previewing printer output.

\image html rulers.png "Rulers used to show real page size in centimeters"

To use the ruler, set its appropriate screen geometry in pixels and then set its size in metrical
units by #setMetrix method . Ruler's scale will be repainted accordingly to match metrical and pixel sizes.

You can choose ruler's #orientation while constructing it or by #setOrientation method.
Horizontal and vertical orientations are supported.
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsRuler : public QFrame
{
    Q_OBJECT
public:
    /*! Constructor.
    Specify the ruler's orientation in \a orient parameter.
    It can be set to \b Qt::Horizontal or \b Qt::Vertical.
    */
    QicsRuler(Qt::Orientation orient, QWidget *parent = 0);
    ~QicsRuler();

    /*! Returns ruler's orientation (\b Qt::Horizontal or \b Qt::Vertical).
    */
    inline Qt::Orientation orientation() const { return m_orient; }

    /*! Sets ruler's orientation (\b Qt::Horizontal or \b Qt::Vertical).
    Ruler will be rotated accordingly to match current orientation.
    */
    void setOrientation(Qt::Orientation orient);

    /*! Makes the ruler to represent metrical scale in the range from 0 to \a mm milimeters.
    */
    void setMetrix(double mm);

protected:
    /*! Paints ruler and its scale. You can reimplement this method to create your own scale.
    */
    virtual void paintEvent ( QPaintEvent * event );

    Qt::Orientation	m_orient;
    double	m_mm;
};

#endif //QICSRULER_H


