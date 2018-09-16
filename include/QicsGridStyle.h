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

#ifndef QICSGRIDSTYLE_H
#define QICSGRIDSTYLE_H

#include <QVector>
#include <QDomElement>
#include "QicsStyle.h"

class QWidget;

////////////////////////////////////////////////////////////////////////////////

/*! \internal
* \class QicsGridStyle
* \brief Represents properties of a row, column, region or grid.
*
* QicsGridStyle contains settings for all properties that pertain to a grid.
*
* The interface of QicsGridStyle is very simple.  You can set a property
* (QicsGridStyle::setValue), get a property (QicsGridStyle::getValue), or clear
* a property (QicsGridStyle::clear).  A property is defined by its name,
* an enumerated value that allows for fast retrieval.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsGridStyle: public QicsStyle
{
public:
    /*! \internal
    * The names of the different properties that can be set.
    *
    * \b IMPORTANT:  Make sure that LastProperty is always the last
    * property name in the enumeration.
    */
    enum QicsGridStyleProperty {
        HorizontalGridLinesVisible = 0,
        VerticalGridLinesVisible,
        HorizontalGridLineWidth,
        VerticalGridLineWidth,
        HorizontalGridLineStyle,
        VerticalGridLineStyle,
        HorizontalGridLinePen,
        VerticalGridLinePen,
        FillHandleVisible,
        GridCellClipping,
        DrawPartialCells,
        AllowUserResize,
        AllowHeaderResize,
        AllowUserMove,
        AllowUserEdit,
        ClickToEdit,
        ResizeCursor,
        AutoSelectCellContents,
        TraversalKeyEditBehavior,
        EnterTraversalDirection,
        TabTraversalDirection,
        EnterTraverseToBegin,
        TabTraverseToBegin,
        GridRepaintBehavior,
        CellOverflowBehavior,
        CellToolTipOffset,
        MaxOverflowCells,
        //Current,
        CurrentCellStyle,
        CurrentCellBorderWidth,
        FrameLineWidth,
        FrameStyle,
        GridPalette,
        Viewport,
        MoreTextPixmap,
        DragEnabled,
        LayoutDirection,
        MapMouseButton,
        FakeCellsDrawingIndex,
        SelectionStyle,
        CurrentCellDraggingPolicy,
        CellDecorationStyle,
        ExclusiveSelectionBorderWidth,
        ExclusiveSelectionBorderColor,
        ExclusiveSelectionHandleBackColor,
        ExclusiveSelectionHandleColor,
        ExclusiveSelectionDragBackColor,
        ExclusiveSelectionDragPen,
        ExclusiveSelectionDragMode,
        ExclusiveSelectionDragOpacity,
        HighlightSelection,
        LastProperty
    };

    /*! \internal
    * Creates a new QicsGridStyle object.
    */
    QicsGridStyle(QicsGridType type, bool create_defaults = false);

    /*! \internal
    * Returns the value of the property specified by \a name.
    */
    inline void *getValue(QicsGridStyleProperty name) const;

    /*! \internal
    * Sets the new value of the property specified by \a name.
    */
    inline void setValue(QicsGridStyleProperty name, const void *val);

    /*! \internal
    * Unsets the value of the property \a prop.  Clearing a property
    * also frees the memory associated with the value.
    */
    inline void clear(QicsGridStyleProperty prop);

    /*!
    * Store settings to \a QDoomElement.
    */
    QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;

    /*!
    * Restore settings from \a QDoomElement.
    */
    void configureFromDomXml(const QDomElement& e);

protected:
    /*! \internal
    * A static const array of \a QicsGridStylePropertyType values which are
    * listed in the SAME order as the QicsGridStyleProperty list. These can
    * be used to determine the type of the property at position \a N in
    * the list of properties \a myProperties.
    */
    static QVector<QicsStylePropertyType> myGridStyleTypeList;
    static QStringList myPropertyNames;
};

inline void *QicsGridStyle::getValue(QicsGridStyleProperty name) const
{
    return QicsStyle::getValue((int)name);
}

inline void QicsGridStyle::setValue(QicsGridStyleProperty name, const void *val)
{
    QicsStyle::setValue((int) name, val);
}

inline void QicsGridStyle::clear(QicsGridStyleProperty prop)
{
    QicsStyle::clear((int) prop);
}

typedef QVector<QicsGridStyle *> QicsGridStylePV;
typedef QVector<QicsGridStylePV *> QicsGridStylePVPV;

#endif //QICSGRIDSTYLE_H


