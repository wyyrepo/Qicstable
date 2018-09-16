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

#ifndef QICSGRIDCOMMON_H
#define QICSGRIDCOMMON_H

#include <QPixmap>
#include "QicsCellCommon.h"
#include "QicsGridStyle.h"
#include "QicsSpan.h"
#include "QicsRubberBand.h"

class QicsMouseMap;

/////////////////////////////////////////////////////////////////////////////////

/*! \class QicsGridCommon QicsGridCommon.h
* \nosubgrouping
*  \brief base class for setting grid and cell attributes
*
* QicsGridCommon is a base class that allows setting and retrieval
* of common grid attributes.  It is not intended to be directly instantiated.
* Rather, one of its subclasses should be used.
*
* New grid attributes should be added here so that all subclasses can use them.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsGridCommon: public QicsCellCommon
{
    Q_OBJECT
public:
    Q_ENUMS( QicsCurrentCellStyle )
    Q_ENUMS( QicsGridCellClipping )

    /** @name Common Grid Properties
    */

    //@{
    /*!
    * Specifies the line width of the frame bordering the table grid and all
    * headers in the table.
    *
    * The default value of this property is \b 2.
    */
    Q_PROPERTY( int frameLineWidth READ frameLineWidth WRITE setFrameLineWidth )

    /*!
    * Specifies the style of the frame bordering the table grid.
    * This value has the same possible values as QFrame::frameStyle().
    *
    * The default value of this property is <b> (QFrame::StyledPanel | QFrame::Sunken </b>
    */
    Q_PROPERTY( int frameStyle READ frameStyle WRITE setFrameStyle )

    /*!
    * Specifies whether horizontal separator lines between cells are drawn
    *
    * The default value of this property is \b true.
    */
    Q_PROPERTY( bool horizontalGridLinesVisible  READ horizontalGridLinesVisible WRITE setHorizontalGridLinesVisible )

    /*!
    * Specifies whether vertical separator lines between cells are drawn
    *
    * The default value of this property is \b true.
    */
    Q_PROPERTY( bool verticalGridLinesVisible  READ verticalGridLinesVisible WRITE setVerticalGridLinesVisible )

    /*!
    * Specifies the width of the grid's horizontal grid lines.
    *
    * The default value of this property is 1.
    */
    Q_PROPERTY( int horizontalGridLineWidth READ horizontalGridLineWidth WRITE setHorizontalGridLineWidth )

    /*!
    * Specifies the width of the grid's vertical grid lines.
    *
    * The default value of this property is 1.
    */
    Q_PROPERTY( int verticalGridLineWidth READ verticalGridLineWidth WRITE setVerticalGridLineWidth )

    /*!
    * Specifies the palette used when drawing grid lines and shadows.
    * The default value of this property is the palette of the table widget.
    */
    Q_PROPERTY( QPalette gridPalette READ gridPalette WRITE setGridPalette )

    /*!
    * Specifies the style of the horizontal grid lines in the grid.
    * See QicsLineStyle for details on the available styles.
    *
    * The default value of this property is \b Plain.
    */
    Q_PROPERTY( QicsLineStyle horizontalGridLineStyle  READ horizontalGridLineStyle WRITE setHorizontalGridLineStyle )

    /*!
    * Specifies the pen of the horizontal grid lines in the grid.
    *
    * The default value of property is \b transparent \b solid pen.
    */
    Q_PROPERTY( QPen horizontalGridLinePen  READ horizontalGridLinePen WRITE setHorizontalGridLinePen )

    /*!
    * Specifies the pen of the horizontal grid lines in the grid.
    * Default value is transparent solid pen.
    *
    * The default value of property is \b transparent \b solid pen.
    */
    Q_PROPERTY( QPen verticalGridLinePen  READ verticalGridLinePen WRITE setVerticalGridLinePen )

    /*!
    * Specifies the style of the vertical grid lines in the grid.
    * See QicsLineStyle for details on the available styles.
    *
    * The default value of this property is \b Plain.
    */
    Q_PROPERTY( QicsLineStyle verticalGridLineStyle  READ verticalGridLineStyle WRITE setVerticalGridLineStyle )

    /*!
    * Specifies the overflow behavior of the grid.  See Qics::QicsCellOverflowBehavior
    * for details.
    *
    * The default value of this property is \b Clip.
    */
    Q_PROPERTY( QicsCellOverflowBehavior cellOverflowBehavior READ cellOverflowBehavior WRITE setCellOverflowBehavior )

    /*!
    * Specifies the maximum number of cells that a given cell can overflow into
    * (when the cellOverflowBehavior property is set to \b Overflow ).
    *
    * The default value of this property is \b 10.
    */
    Q_PROPERTY( int maxOverflowCells READ maxOverflowCells WRITE setMaxOverflowCells )

    /*!
    * Specifies whether cells that would only partially be shown at the right or
    * bottom edge of the grid should be drawn.  If this value is \a false, a cell
    * will only be drawn if it can be completely shown.
    */
    Q_PROPERTY( bool drawPartialCells READ drawPartialCells WRITE setDrawPartialCells )

    /*!
    * Specifies the policy for text that is too large to fit into a cell.
    * If the value is \b AllowPartial, as much of the text as possible will be
    * displayed.  If the value is \b NoDisplayOnPartial, none of the text will be
    * displayed.  If the value is \b UseClippedSymbol, some text will be displayed,
    * and a clipped symbol will then be displayed, signifying that not all of the
    * text could be displayed.
    *
    * The default value of this property is \b AllowPartial.
    */
    Q_PROPERTY( QicsGridCellClipping gridCellClipping READ gridCellClipping WRITE setGridCellClipping )

    /*!
    * Specifies the style of the current cell in the grid.  See
    * \link Qics::QicsCurrentCellStyle QicsCurrentCellStyle\endlink
    * for details.
    *
    * The default value of this property is \b Spreadsheet.
    */
    Q_PROPERTY( QicsCurrentCellStyle currentCellStyle READ currentCellStyle WRITE setCurrentCellStyle )

    /*!
    * Specifies the style of selection.
    * The default value of this property is \b NORMAL (The selection will be drawn by
    * the normal way.) When selection style is \b EXCLUSIVE the selection will be drawn
    * with border like in MS Excell.
    */
    Q_PROPERTY( QicsSelectionStyle selectionStyle READ selectionStyle WRITE setSelectionStyle )

    /*!
    * Specifies the width of the border that is drawn around the current cell
    * in the grid.
    *
    * The default value of this property is \b 3.
    */
    Q_PROPERTY( int currentCellBorderWidth READ currentCellBorderWidth WRITE setCurrentCellBorderWidth )

    /*!
    * Specifies whether the user must first click in a cell before being able
    * to edit the cell by typing.  If \b true, the user must left click in the
    * cell before keystrokes will be accepted.  If \b false, keystrokes will
    * immediately be accepted.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool clickToEdit READ clickToEdit WRITE setClickToEdit )

    /*!
    * Specifies the cell decoration style for headers.
    *
    * The default value of this property is \b Flat.
    */
    Q_PROPERTY( QicsCellDecorationStyle cellDecorationStyle READ cellDecorationStyle WRITE setCellDecorationStyle )

    /*!
    * Specifies whether the contents of a cell are automatically selected
    * when the table begins a cell edit operation.  If \b true, the contents
    * of each cell will be selected when beginning an edit (and thus typing
    * any character will erase the previous contents).  If \b false, the
    * contents of each sell will not be selected (and thus typing
    * any character will append to the previous contents).
    *
    * The default value of this property is \b true.
    */
    Q_PROPERTY( bool autoSelectCellContents READ autoSelectCellContents WRITE setAutoSelectCellContents )

    /*!
    * Specifies the behavior of traversal keys (Left, Right, Up, Down, etc)
    * within an edited cell (using QicsTextCellDisplay).  If
    * \b TraverseWithinCell, traversal keys will move the text cursor within
    * the edited cell.  If \b TraverseBetweenCells, traversal keys move the
    * current cell highlight between cells in the table.
    *
    * The default value of this property is \b TraverseWithinCell.
    */
    Q_PROPERTY( QicsTraversalKeyEditBehavior traversalKeyEditBehavior READ traversalKeyEditBehavior WRITE setTraversalKeyEditBehavior )

    /*!
    * Specifies the direction that the grid will traverse when the user
    * types \a Enter or \a Shift-Enter.  If the property is \b Vertical,
    * \a Enter will traverse down one cell, and \a Shift-Enter
    * will traverse up one cell.  If the property is \b Horizontal,
    * \a Enter will traverse right one cell, and \a Shift-Enter
    * will traverse left one cell.
    *
    * The default value of this property is \b Vertical.
    */
    Q_PROPERTY( Qt::Orientation enterTraversalDirection READ enterTraversalDirection WRITE setEnterTraversalDirection )

    /*!
    * Specifies the direction that the grid will traverse when the user
    * types \a Tab or \a Shift-Tab.  If the property is \b Vertical,
    * \a Tab will traverse down one cell, and \a Shift-Tab
    * will traverse up one cell.  If the property is \b Horizontal,
    * \a Tab will traverse right one cell, and \a Shift-Tab
    * will traverse left one cell.
    *
    * The default value of this property is \b Horizontal.
    */
    Q_PROPERTY( Qt::Orientation tabTraversalDirection READ tabTraversalDirection WRITE setTabTraversalDirection )

    /*!
    * Specifies traversal cell behavior (for Tab key) when it reaches the end of row/column.
    * If \a true it jumps to the first cell of next row/column, else does nothing.
    */
    Q_PROPERTY( bool tabTraverseToBegin READ tabTraverseToBegin WRITE setTabTraverseToBegin )

    /*!
    * Specifies traversal cell behavior (for Enter key) when it reaches the end of row/column.
    * If \a true it jumps to the first cell of next row/column, else does nothing.
    */

    Q_PROPERTY( bool enterTraverseToBegin READ enterTraverseToBegin WRITE setEnterTraverseToBegin )

    /*!
    * Specifies the pixmap that is used in this grid to show that text
    * in a cell has been clipped (i.e. it could not completely fit in the cell).
    *
    * The default value of this property is a "right arrow" pixmap.
    */
    Q_PROPERTY( QPixmap moreTextPixmap READ moreTextPixmap WRITE setMoreTextPixmap )

    /*!
    * Specifies whether the user is allowed to drag cell contents to other
    * cells, tables, and/or other applications.
    *
    * The default value of this property is \b true.
    */
    Q_PROPERTY( bool dragEnabled READ dragEnabled WRITE setDragEnabled )

    /*!
    * This property set the layout direction for a grid
    *
    * The default value of this property is \b Qt::LeftToRight.
    */
    Q_PROPERTY( Qt::LayoutDirection layoutDirection READ layoutDirection WRITE setLayoutDirection )

    /*!
    * Setting FakeCellsDrawingIndex forcing table to draw empty not
    * editable cells witch are using default style
    * default value of this property is Qics::NoIndex
    */
    Q_PROPERTY(QicsIndexType fakeCellsDrawingIndex READ fakeCellsDrawingIndex WRITE setFakeCellsDrawingIndex)

    /*!
    * Specifies the behavior of drag&drop operations when it has began
    * from current cell.
    * The default value of this property is \b Qics::DragCurrent.
    */
    Q_PROPERTY(QicsCurrentCellDraggingPolicy currentCellDraggingPolicy READ currentCellDraggingPolicy
    WRITE setCurrentCellDraggingPolicy)

    /*!
    * This property holds whether the fill handle is visible.
    */
    Q_PROPERTY( bool fillHandleVisible READ fillHandleVisible WRITE setFillHandleVisible )
    //@}

public:
    /*!
    * \internal
    * default constructor
    */
    QicsGridCommon(QObject *parent = 0, bool forward_signals = false);

    /*!
    * \internal
    * Constructor for the grid.
    */
    QicsGridCommon(QicsGridInfo *info, QObject *parent = 0,
        bool forward_signals = false);

    /*!
    * Destructor
    */
    virtual ~QicsGridCommon();

    virtual void setInfo(QicsGridInfo *info);

    /*!
    * Returns the current viewport of the grid.  The viewport
    * specifies an area of the data model that is allowed to be
    * displayed.
    * \sa setViewport()
    */
    virtual QicsRegion viewport() const;

    /*!
    * Adds a cell spanning region to the grid.  The contents of the cell
    * at the top, left of \a span will be drawn across all cells in \a span.
    * Two cell spanning regions cannot overlap.  If addCellSpan is called with
    * a spanning region that overlaps an existing spanning region, the method
    * will return \b false.  If the new spanning region is accepted, the method
    * will return \b true.
    * If \a visual is true (default), it is assumed that the span is given in visual coordinates,
    * else model coordinates are used.
    * \sa removeCellSpan()
    * \sa cellSpanList()
    */
    bool addCellSpan(QicsSpan span, bool visual = true);

    /*!
    * Returns the grid's cell spanning list.  The returned value
    * is a \b copy of the grid's internal list, and it is the responsibility
    * of the caller to delete it when it is no longer used.  Modifications
    * to this list will not affect the grid -- use addCellSpan() and
    * removeCellSpan() instead.
    * \sa removeCellSpan()
    * \sa cellSpanList()
    */
    QicsSpanList *cellSpanList();

    /*!
    * Returns the current horizontalGridLinesVisible property setting.
    * See the \link #horizontalGridLinesVisible "horizontalGridLinesVisible" \endlink
    * property for details.
    * \sa setHorizontalGridLinesVisible()
    */
    bool horizontalGridLinesVisible() const;

    /*!
    * Returns the current verticalGridLinesVisible property setting.
    * See the \link #verticalGridLinesVisible "verticalGridLinesVisible" \endlink
    * property for details.
    * \sa setVerticalGridLinesVisible()
    */
    bool verticalGridLinesVisible() const;

    /*!
    * Returns true if this fill handle visible;
    * otherwise returns false.
    */
    bool fillHandleVisible() const;

    /*!
    * Returns the value of the width of the grid's horizontal grid lines.
    * See the \link #horizontalGridLineWidth "horizontalGridLineWidth" \endlink
    * property for details.
    * \sa setHorizontalGridLineWidth()
    */
    int horizontalGridLineWidth(void) const;

    /*!
    * Returns the value of the width of the grid's vertical grid lines.
    * See the \link #verticalGridLineWidth "verticalGridLineWidth" \endlink
    * property for details.
    * \sa setVerticalGridLineWidth()
    */
    int verticalGridLineWidth(void) const;

    /*!
    * Returns the horizontal grid line style for the grid.
    * See the \link #horizontalGridLineStyle "horizontalGridLineStyle" \endlink
    * property for details.
    * \sa setHorizontalGridLineStyle()
    */
    QicsLineStyle horizontalGridLineStyle(void) const;

    /*!
    * Returns the vertical grid line style for the grid.
    * See the \link #verticalGridLineStyle "verticalGridLineStyle" \endlink
    * property for details.
    * \sa setVerticalGridLineStyle()
    */
    QicsLineStyle verticalGridLineStyle(void) const;

    /*!
    * Returns the pen used to draw the grid's horizontal grid lines.
    * See the \link #horizontalGridLinePen "horizontalGridLinePen" \endlink
    * property for details.
    * \sa setHorizontalGridLinePen()
    */
    QPen horizontalGridLinePen() const;

    /*!
    * Returns the pen used to draw the grid's vertical grid lines.
    * See the \link #verticalGridLinePen "verticalGridLinePen" \endlink
    * property for details.
    * \sa setVerticalGridLinePen()
    */
    QPen verticalGridLinePen() const;

    /*!
    * Returns the partial cell display policy.
    * See the \link #drawPartialCells "drawPartialCells" \endlink
    * property for details.
    * \sa setDrawPartialCells()
    */
    bool drawPartialCells() const;

    /*!
    * Returns the cell clipping display policy.
    * See the \link #gridCellClipping "gridCellClipping" \endlink
    * property for details.
    * \sa setGridCellClipping()
    */
    QicsGridCellClipping gridCellClipping() const;

    /*!
    * Returns the grid's cell overflow behavior.
    * See the \link #cellOverflowBehavior "cellOverflowBehavior" \endlink
    * property for details.
    * \sa setCellOverflowBehavior()
    */
    QicsCellOverflowBehavior cellOverflowBehavior() const;

    /*!
    * Returns tooltip offset from point of interest
    * \sa setCellToolTipOffset()
    */
    QPoint cellToolTipOffset() const;

    /*!
    * Returns the maximum number of overflow cells for the grid.
    * See the \link #maxOverflowCells "maxOverflowCells" \endlink
    * property for details.
    * \sa setMaxOverflowCells()
    */
    int maxOverflowCells() const;

    /*!
    * Returns the frame line width of the grid.
    * See the \link #frameLineWidth "frameLineWidth" \endlink
    * property for details.
    * \sa setFrameLineWidth()
    */
    int frameLineWidth() const;

    /*!
    * Returns the frame style of the grid.
    * See the \link #frameStyle "frameStyle" \endlink property for details.
    * \sa setFrameStyle()
    */
    int frameStyle() const;

    /*!
    * Returns the style of the current cell in the table.
    * See the \link #currentCellStyle "currentCellStyle" \endlink
    * property for details.
    * \sa  setCurrentCellStyle()
    */
    QicsCurrentCellStyle currentCellStyle() const;

    /*!
    * Returns the style of the selection in the table.
    * See the \link #selectionStyle "selectionStyle" \endlink
    * property for details.
    * \sa  setSelectionStyle()
    */
    QicsSelectionStyle selectionStyle() const;

    /*!
    * Returns true if selection is highlighted on headers.
    * See the \link #highlightSelection "highlightSelection" \endlink
    * property for details.
    * \sa  setHighlightSelection(),
    * QicsCellCommon::highlightForegroundColor(), QicsCellCommon::highlightBackgroundColor()
    * \since 2.3
    */
    bool highlightSelection() const;

    /*!
    * Returns the width of the border that is drawn around the current cell
    * in the table.
    * See the \link #currentCellBorderWidth "currentCellBorderWidth" \endlink
    * property for details.
    * \sa  setCurrentCellBorderWidth()
    */
    int currentCellBorderWidth() const;

    /*!
    * Returns whether the user must first click in a cell before being able
    * to edit the cell by typing.
    * See the \link #clickToEdit "clickToEdit" \endlink property
    * for details.
    * \sa setClickToEdit()
    */
    bool clickToEdit() const;

    /*!
    * Returns whether the contents of a cell are automatically selected
    * when the table begins a cell edit operation.
    * See the \link #autoSelectCellContents "autoSelectCellContents" \endlink
    * property for details.
    * \sa setAutoSelectCellContents()
    */
    bool autoSelectCellContents() const;

    /*!
    * Returns the behavior of traversal keys within an edited cell.
    * See the \link #traversalKeyEditBehavior "traversalKeyEditBehavior" \endlink
    * property for details.
    *
    * \sa setTraversalKeyEditBehavior
    */
    QicsTraversalKeyEditBehavior traversalKeyEditBehavior() const;

    /*!
    * Returns the direction that the table will traverse when the user
    * types \a Enter or \a Shift-Enter.
    * See the \link #enterTraversalDirection "enterTraversalDirection" \endlink
    * property for details.
    * \sa setEnterTraversalDirection()
    */
    Qt::Orientation enterTraversalDirection() const;

    /*!
    * Returns the direction that the table will traverse when the user
    * types \a Tab or \a Shift-Tab.
    * See the \link #tabTraversalDirection "tabTraversalDirection" \endlink
    * property for details.
    * \sa setTabTraversalDirection()
    */
    Qt::Orientation tabTraversalDirection() const;

    /*!
    * Returns traversal cell behavior (for Tab key) when it reaches the end of row/column.
    * If \a true it jumps to the first cell of next row/column, else does nothing.
    * \sa setTabTraverseToBegin(bool)
    */
    bool tabTraverseToBegin() const;

    /*!
    * Returns traversal cell behavior (for Enter key) when it reaches the end of row/column.
    * If \a true it jumps to the first cell of next row/column, else does nothing.
    * \sa setTabTraverseToBegin(bool)
    */
    bool enterTraverseToBegin() const;

    /*!
    * Returns the pixmap that is used in this grid to show that text
    * in a cell has been clipped (i.e. it could not completely fit in the cell).
    * See the \link #moreTextPixmap "moreTextPixmap" \endlink
    * property for details.
    * \sa setMoreTextPixmap()
    */
    QPixmap moreTextPixmap() const;

    /*!
    * Returns the palette that is used to draw this grid's grid lines.
    * See the \link #gridPalette "gridPalette" \endlink
    * property for details.
    * \sa setGridPalette()
    */
    QPalette gridPalette() const;

    /*!
    * Returns whether the user is allowed to drag from this table.
    * See the \link #dragEnabled "dragEnabled" \endlink
    * property for details.
    * \sa setDragEnabled
    */
    bool dragEnabled() const;
    /*!
    * Returns layout direction of the grid.
    * See the \link #layoutDirection "layoutDirection" \endlink
    * property for details.
    * \sa setLayoutDirection
    */
    Qt::LayoutDirection layoutDirection(void) const;

    /*!
    * Sets the table's FakeCellsDrawingIndex for drawing fake cells
    * See the \link #fakeCellsDrawingIndex "fakeCellsDrawingIndex" \endlink
    * property for details.
    */
    void setFakeCellsDrawingIndex(Qics::QicsIndexType type);

    /*!
    * Returns the FakeCellsDrawingIndex for drawing fake cells.
    * See the \link #fakeCellsDrawingIndex "fakeCellsDrawingIndex" \endlink
    * property for details.
    */
    Qics::QicsIndexType fakeCellsDrawingIndex() const;

    /*!
    * Returns behavior of drag&drop operations when it has began
    * from current cell.
    * See the \link #currentCellDraggingPolicy "currentCellDraggingPolicy" \endlink
    * property for details.
    */
    Qics::QicsCurrentCellDraggingPolicy currentCellDraggingPolicy()const;

    /*!
    * Sets behavior of drag&drop operations when it has began
    * from current cell.
    * See the \link #currentCellDraggingPolicy "currentCellDraggingPolicy" \endlink
    * property for details.
    */
    void setCurrentCellDraggingPolicy(Qics::QicsCurrentCellDraggingPolicy policy);

    /*!
    * Returns border width of the exclusive selection frame.
    * See the \link #exclusiveSelectionBorderWidth "exclusiveSelectionBorderWidth" \endlink
    * property for details.
    * \since 2.3
    */
    int exclusiveSelectionBorderWidth()const;

    /*!
    * Sets border width of the exclusive selection frame.
    * See the \link #exclusiveSelectionBorderWidth "exclusiveSelectionBorderWidth" \endlink
    * property for details.
    * \since 2.3
    */
    void setExclusiveSelectionBorderWidth(int w);

    /*!
    * Returns color of the exclusive selection frame.
    * See the \link #exclusiveSelectionBorderColor "exclusiveSelectionBorderColor" \endlink
    * property for details.
    * \since 2.3
    */
    QColor exclusiveSelectionBorderColor()const;

    /*!
    * Sets color of the exclusive selection frame.
    * See the \link #exclusiveSelectionBorderColor "exclusiveSelectionBorderColor" \endlink
    * property for details.
    * \since 2.3
    */
    void setExclusiveSelectionBorderColor(const QColor &c);

    /*!
    * Returns handle's color of the exclusive selection frame.
    * See the \link #exclusiveSelectionHandleColor "exclusiveSelectionHandleColor" \endlink
    * property for details.
    * \since 2.3
    */
    QColor exclusiveSelectionHandleColor()const;

    /*!
    * Sets handle's color of the exclusive selection frame.
    * See the \link #exclusiveSelectionHandleColor "exclusiveSelectionHandleColor" \endlink
    * property for details.
    * \since 2.3
    */
    void setExclusiveSelectionHandleColor(const QColor &c);

    /*!
    * Returns handle's background color of the exclusive selection frame.
    * See the \link #exclusiveSelectionHandleBackColor "exclusiveSelectionHandleBackColor" \endlink
    * property for details.
    * \since 2.3
    */
    QColor exclusiveSelectionHandleBackColor()const;

    /*!
    * Sets handle's background color of the exclusive selection frame.
    * See the \link #exclusiveSelectionHandleBackColor "exclusiveSelectionHandleBackColor" \endlink
    * property for details.
    * \since 2.3
    */
    void setExclusiveSelectionHandleBackColor(const QColor &c);

    /*!
    * Returns background color of the exclusive selection frame during dragging by fill handle.
    * See the \link #exclusiveSelectionDragBackColor "setExclusiveSelectionDragBackColor" \endlink
    * property for details.
    * \since 2.3
    */
    QColor exclusiveSelectionDragBackColor()const;

    /*!
    * Sets background color of the exclusive selection frame during dragging by fill handle.
    * See the \link #exclusiveSelectionDragBackColor "setExclusiveSelectionDragBackColor" \endlink
    * property for details.
    * \since 2.3
    */
    void setExclusiveSelectionDragBackColor(const QColor &c);

    /*!
    * Returns pen of the exclusive selection frame during dragging by fill handle.
    * See the \link #exclusiveSelectionDragPen "setExclusiveSelectionDragPen" \endlink
    * property for details.
    * \since 2.3
    */
    QPen exclusiveSelectionDragPen()const;

    /*!
    * Sets pen of the exclusive selection frame during dragging by fill handle.
    * See the \link #exclusiveSelectionDragPen "setExclusiveSelectionDragPen" \endlink
    * property for details.
    * \since 2.3
    */
    void setExclusiveSelectionDragPen(const QPen &p);

    /*!
    * Returns opacity of the exclusive selection frame during dragging by fill handle.
    * See the \link #exclusiveSelectionDragOpacity "setExclusiveSelectionDragOpacity" \endlink
    * property for details.
    * \since 2.3
    */
    float exclusiveSelectionDragOpacity()const;

    /*!
    * Sets opacity of the exclusive selection frame during dragging by fill handle.
    * \a op must be in ranges 0.0 (full transparency) to 1.0 (full opacity).
    * Default value is 0.5.
    * See the \link #exclusiveSelectionDragOpacity "setExclusiveSelectionDragOpacity" \endlink
    * property for details.
    * \since 2.3
    */
    void setExclusiveSelectionDragOpacity(float op);

    /*!
    * Returns drawing mode of the exclusive selection frame during dragging by fill handle.
    * See the \link #exclusiveSelectionDragMode "setExclusiveSelectionDragMode" \endlink
    * property for details.
    * \since 2.3
    */
    QicsRubberBand::DrawingMode exclusiveSelectionDragMode()const;

    /*!
    * Sets drawing mode of the exclusive selection frame during dragging by fill handle.
    * Default value is QicsRubberBand::DM_CUSTOM.
    * See the \link #exclusiveSelectionDragMode "setExclusiveSelectionDragMode" \endlink
    * property for details.
    * \since 2.3
    */
    void setExclusiveSelectionDragMode(QicsRubberBand::DrawingMode mode);

    /*!
    * Returns cell decoration style.
    * See the \link #cellDecorationStyle "cellDecorationStyle" \endlink
    * property for details.
    * \sa setCellDecorationStyle();
    * \since 2.3
    */
    Qics::QicsCellDecorationStyle cellDecorationStyle() const;

    /*!
    * Sets cell decoration style to \a s.
    * See the \link #cellDecorationStyle "cellDecorationStyle" \endlink
    * property for details.
    * \since 2.3
    */
    void setCellDecorationStyle(Qics::QicsCellDecorationStyle s);

    /*!
    * Sets the autofit mode.
    * \since 2.4.1
    */
    inline void setAutoFitMode(Qics::QicsAutoFitMode mode) {m_autoFitMode = mode;}

    /*!
    * Returns current autofit mode.
    * \since 2.4.1
    */
    inline Qics::QicsAutoFitMode autoFitMode() const {return m_autoFitMode;}

    /*!
    * This method set mouse button mapping inside \class QicsGridCommon.
    * Usefull for Apple mouse, 2-button mouse or for individual
    * remapping.
    * First paramater action button, second keyboard modifier, third result
    * button (which emulate).
    */
    bool setMouseButton( Qt::MouseButton, Qt::Modifier, Qt::MouseButton );

    QicsMouseMap* mouseButtonRef() const;

public slots:
    /*!
    * Restricts the grid to show a subset of the data model values.
    * \param vp the area of the data model that is allowed to be
    *           displayed.
    * \sa viewport()
    */
    virtual void setViewport(const QicsRegion &vp);

    /*!
    * Removes a cell spanning region from the grid.  Because cell spanning regions
    * cannot overlap, it is sufficient to specify the existing span by giving
    * its starting cell.
    * If \a visual is true (default), it is assumed that the span is given in visual coordinates,
    * else model coordinates are used.
    * \sa addCellSpan()
    * \sa cellSpanList()
    */
    void removeCellSpan(int start_row, int start_col, bool visual = true);

    /*!
    * Controls the display of horizontal grid lines in the grid.
    * See the \link #horizontalGridLinesVisible "horizontalGridLinesVisible" \endlink
    * property for details.
    * \sa horizontalGridLinesVisible()
    */
    void setHorizontalGridLinesVisible(bool b);

    /*!
    * Controls the display of vertical grid lines in the grid.
    * See the \link #verticalGridLinesVisible "verticalGridLinesVisible" \endlink
    * property for details.
    * \sa verticalGridLinesVisible()
    */
    void setVerticalGridLinesVisible(bool b);

    /*!
    * Sets visibility of fill handle
    */
    void setFillHandleVisible(bool b);

    /*!
    * Sets the value of the width of the grid's horizontal grid lines.
    * See the \link #horizontalGridLineWidth "horizontalGridLineWidth" \endlink
    * property for details.
    * \sa horizontalGridLineWidth()
    */
    void setHorizontalGridLineWidth(int w);

    /*!
    * Sets the value of the width of the grid's vertical grid lines.
    * See the \link #verticalGridLineWidth "verticalGridLineWidth" \endlink
    * property for details.
    * \sa verticalGridLineWidth()
    */
    void setVerticalGridLineWidth(int w);

    /*!
    * Sets the horizontal grid line style for the grid.
    * See the \link #horizontalGridLineStyle "horizontalGridLineStyle" \endlink
    * property for details.
    * \sa horizontalGridLineStyle()
    */
    void setHorizontalGridLineStyle(QicsLineStyle style);

    /*!
    * Sets the vertical grid line style for the grid.
    * See the \link #verticalGridLineStyle "verticalGridLineStyle" \endlink
    * property for details.
    * \sa verticalGridLineStyle()
    */
    void setVerticalGridLineStyle(QicsLineStyle style);

    /* Sets the pen used to draw the grid's horizontal grid lines.
    * See the \link #horizontalGridLinePen "horizontalGridLinePen" \endlink
    * property for details.
    * \sa horizontalGridLinePen()
    */
    void setHorizontalGridLinePen(const QPen &pen);

    /*!
    * Sets the pen used to draw the grid's vertical grid lines.
    * See the \link #verticalGridLinePen "verticalGridLinePen" \endlink
    * property for details.
    * \sa verticalGridLinePen()
    */
    void setVerticalGridLinePen(const QPen &pen);

    /*!
    * Sets the partial cell display policy.
    * See the \link #drawPartialCells "drawPartialCells" \endlink
    * property for details.
    * \sa drawPartialCells()
    */
    void setDrawPartialCells(bool b);

    /*!
    * Sets the cell clipping display policy.
    * See the \link #gridCellClipping "gridCellClipping" \endlink
    * property for details.
    * \sa gridCellClipping()
    */
    void setGridCellClipping(QicsGridCellClipping c);

    /*!
    * Sets the grid's cell overflow behavior.
    * See the \link #cellOverflowBehavior "cellOverflowBehavior" \endlink
    * property for details.
    * \sa cellOverflowBehavior()
    */
    void setCellOverflowBehavior(QicsCellOverflowBehavior b);

    /*!
    * Sets tooltip offset from point of interest.
    * \sa cellToolTipOffset()
    */
    void setCellToolTipOffset(const QPoint &pos);

    /*!
    * Sets the maximum number of overflow cells for the grid.
    * See the \link #maxOverflowCells "maxOverflowCells" \endlink
    * property for details.
    * \sa maxOverflowCells()
    */
    void setMaxOverflowCells(int num);

    /*!
    * Sets the frame style of the grid.
    * See the \link #frameStyle "frameStyle" \endlink property for details.
    * \sa frameStyle()
    */
    void setFrameStyle(int style);

    /*!
    * Sets the frame line width of the grid.
    * See the \link #frameLineWidth "frameLineWidth" \endlink
    * property for details.
    * \sa frameLineWidth()
    */
    void setFrameLineWidth(int lw);

    /*!
    * Sets the style of the current cell in the table.
    * See the \link #currentCellStyle "currentCellStyle" \endlink
    * property for details.
    * \sa  currentCellStyle()
    */
    void setCurrentCellStyle(QicsCurrentCellStyle s);

    /*!
    * Sets the style of the selection in the table.
    * See the \link #selectionStyle "selectionStyle" \endlink
    * property for details.
    * \sa  selectionStyle()
    */
    void setSelectionStyle(QicsSelectionStyle s);

    /*!
    * Sets selection highlighting on headers.
    * See the \link #highlightSelection "highlightSelection" \endlink
    * property for details.
    * \sa highlightSelection(),
    * QicsCellCommon::highlightForegroundColor(), QicsCellCommon::highlightBackgroundColor()
    * \since 2.3
    */
    void setHighlightSelection(bool s);

    /*!
    * Sets the width of the border that is drawn around the current cell
    * in the table.
    * See the \link #currentCellBorderWidth "currentCellBorderWidth" \endlink
    * property for details.
    * \sa  currentCellBorderWidth()
    */
    void setCurrentCellBorderWidth(int w);

    /*!
    * Sets whether the user must first click in a cell before being able
    * to edit the cell by typing.
    * See the \link #clickToEdit "clickToEdit" \endlink property
    * for details.
    * \sa clickToEdit()
    */
    void setClickToEdit(bool b);

    /*!
    * Sets whether the contents of a cell are automatically selected
    * when the table begins a cell edit operation.
    * See the \link #autoSelectCellContents "autoSelectCellContents" \endlink
    * property for details.
    * \sa autoSelectCellContents()
    */
    void setAutoSelectCellContents(bool b);

    /*!
    * Sets the behavior of traversal keys within an edited cell.
    * See the \link #traversalKeyEditBehavior "traversalKeyEditBehavior" \endlink
    * property for details.
    *
    * \sa traversalKeyEditBehavior
    */
    void setTraversalKeyEditBehavior(QicsTraversalKeyEditBehavior beh);

    /*!
    * Sets the direction that the table will traverse when the user
    * types \a Enter or \a Shift-Enter.
    * See the \link #enterTraversalDirection "enterTraversalDirection" \endlink
    * property for details.
    * \sa enterTraversalDirection()
    */
    void setEnterTraversalDirection(Qt::Orientation dir);

    /*!
    * Sets the direction that the table will traverse when the user
    * types \a Tab or \a Shift-Tab.
    * See the \link #tabTraversalDirection "tabTraversalDirection" \endlink
    * property for details.
    * \sa tabTraversalDirection()
    */
    void setTabTraversalDirection(Qt::Orientation dir);

    /*!
    * Sets traversal cell behavior (for Tab key) when it reaches the end of row/column.
    * If \a traverse is true it jumps to the first cell of next row/column, else does nothing.
    * \sa tabTraverseToBegin()
    */
    void setTabTraverseToBegin(bool traverse);

    /*!
    * Sets traversal cell behavior (for Enter key) when it reaches the end of row/column.
    * If \a traverse is true it jumps to the first cell of next row/column, else does nothing.
    * \sa tabTraverseToBegin()
    */
    void setEnterTraverseToBegin(bool traverse);

    /*!
    * Sets the pixmap that is used in this grid to show that text
    * in a cell has been clipped (i.e. it could not completely fit in the cell).
    * See the \link #moreTextPixmap "moreTextPixmap" \endlink
    * property for details.
    * \sa moreTextPixmap()
    */
    void setMoreTextPixmap(const QPixmap &pix);

    /*!
    * Sets the palette that is used to draw this grid's grid lines.
    * See the \link #gridPalette "gridPalette" \endlink
    * property for details.
    * \sa gridPalette()
    */
    void setGridPalette(const QPalette &pal);

    /*!
    * Sets whether the user is allowed to drag from this table.
    * See the \link #dragEnabled "dragEnabled" \endlink
    * property for details.
    * \sa setDragEnabled
    */
    void setDragEnabled(bool b);

    /*!
    * Set the layout direction for a table
    * See the \link #layoutDirection "layoutDirection" \endlink
    * property for details.
    * \sa setLayoutDirection
    */
    void setLayoutDirection(Qt::LayoutDirection layoutDirection);

signals:
    /*!
    * This signal is emitted when the user presses a mouse button
    * in the grid.  The indices of the cell, the button that was pressed,
    * and the position of the mouse pointer in the table are passed as
    * parameters.
    * \sa clicked(), doubleClicked()
    */
    void pressed(int row, int col, int button, const QPoint &pos);

    /*!
    * This signal is emitted when the user clicks (presses and releases
    * in the same cell) a mouse button in the grid.  The indices of
    * the cell, the button that was pressed, and the position of the mouse
    * pointer in the table are passed as parameters.
    * \sa pressed(), doubleClicked()
    */
    void clicked(int row, int col, int button, const QPoint &pos);

    /*!
    * This signal is emitted when the user double clicks a mouse button
    * in the grid.  The indices of the cell, the button that was pressed,
    * and the position of the mouse pointer in the table are passed as parameters.
    * \sa clicked()
    */
    void doubleClicked(int row, int col, int button, const QPoint &pos);

    /*!
    * This signal is emitted when a the value of a cell in the grid changes.
    * The cell (\a row, \a col ) is expressed in \b visual coordinates.
    */
    void valueChanged(int row, int col);

protected:
    virtual void setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val);
    virtual void *getAttr(QicsCellStyle::QicsCellStyleProperty attr) const;
    virtual void clearAttr(QicsCellStyle::QicsCellStyleProperty attr);

    /*!
    * \internal
    * Method used to actually set a grid attribute's value.
    */
    virtual void setGridAttr(QicsGridStyle::QicsGridStyleProperty attr, const void *val);
    /*!
    * \internal
    * Method used to actually retrieve a grid attribute's value.
    */
    virtual void *getGridAttr(QicsGridStyle::QicsGridStyleProperty attr) const;
    /*!
    * \internal
    * Method method used to actually clear a grid attribute's value.
    */
    virtual void clearGridAttr(QicsGridStyle::QicsGridStyleProperty attr);

    virtual void setDMMargin(int margin);
    virtual void setDMFont(const QFont &font);
    virtual void setDMBorderWidth(int bw);

    /*!
    * \internal
    * Connect signals from grid widgets to our signals.
    */
    void initSignals();

    int horizontalShadeLineWidth() const;

    int verticalShadeLineWidth() const;

    /*!
    * \internal
    * Should this object forward signals from grids, etc?
    */
    bool myForwardSignals;

    Qics::QicsAutoFitMode m_autoFitMode;

protected slots:
    /*!
    * \internal
    * Connects signals from \a grid to this object.
    */
    virtual void connectGrid(QicsScreenGrid *grid);

    /*!
    * \internal
    * Disonnects signals from \a grid to this object.
    */
    virtual void disconnectGrid(QicsScreenGrid *grid);

    /*!
    * \internal
    * Translates \a pos to table coordinates and "re-emits" the signal.
    */
    void handleGridPress(int row, int col, int button, const QPoint &pos);

    /*!
    * \internal
    * Translates \a pos to table coordinates and "re-emits" the signal.
    */

    void handleGridClick(int row, int col, int button, const QPoint &pos);

    /*!
    * \internal
    * Translates \a pos to table coordinates and "re-emits" the signal.
    */
    void handleGridDoubleClick(int row, int col, int button, const QPoint &pos);

    /*!
    * \internal
    * Disconect from grid.
    */
    //void gridDeleted();

#ifdef Q_DISABLE_COPY
private:
    QicsGridCommon(const QicsGridCommon& gc);
    QicsGridCommon &operator=(const QicsGridCommon& gc);
#endif

private:
    friend class QicsGrid;
    friend class QicsScreenGrid;
    friend class QicsPrintGrid;
    friend class QicsHeaderGrid;
    friend class QicsTable;
    friend class QicsCellDisplay;
};

#endif //QICSGRIDCOMMON_H


