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

#ifndef QICSSCREENGRID_H
#define QICSSCREENGRID_H

#include <QFrame>
#include <QTimer>
#include <QVector>
#include <QPixmap>
#include "QicsGrid.h"
#include "QicsSpan.h"
#include "QicsCellStyle.h"
#include "QicsGridStyle.h"

class QKeyEvent;
class QDragMoveEvent;
class QDropEvent;
class QResizeEvent;
class QMouseEvent;
class QPaintEvent;
class QRubberBand;
/*!
* \class QicsScreenGrid QicsScreenGrid.h
* \brief Base widget class for on-screen grids
*
* QicsScreenGrid is the base widget class for all on-screen grids.  It uses
* the layout and drawing routines from its superclass, QicsGrid.  QicsScreenGrid
* also provides the infrastructure for cell traversal, cell editing, and drag and
* drop.
*
* Most users of the %QicsTable product should not need to create or modify
* QicsScreenGrid objects directly.  The QicsTable widget automatically creates
* the necessary grid objects when needed.  One instance where a user may
* need to modify a grid is to change the mouse and/or keyboard behavior of
* the grids.
*
* QicsScreenGrid does not specify any  user interaction semantics.  It is the
* responsibility of subclasses to handle user interaction,
* such as mouse and keyboard events on its window.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsScreenGrid: public QFrame, public QicsGrid
{
    Q_OBJECT
public:
    /*!
    * Constructor for the class.
    * \param w the parent of this widget
    * \param info used for style and dimension information, as well as
    *             the associated data model.
    * \param top_row initial top row to display in the grid
    * \param left_column initial left column to display in the grid
    */
    QicsScreenGrid(QWidget *w,
        QicsGridInfo &info,
        int top_row = 0,
        int left_column = 0);

    /*!
    * The destructor
    */
    virtual ~QicsScreenGrid();

    inline bool isMainGrid() const { return m_isMainGrid; }

    void setViewport(const QicsRegion &reg);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    /*!
    * Returns the index of the row that would be displayed at the top
    * of the grid
    * if the last displayable cell was placed at the bottom right
    * of the grid.  This value is used by the QicsTable to set the
    * appropriate values on the table's scrollbars.
    */
    int lastPageStartRow();
    /*!
    * Returns the index of the column that would be displayed at the left
    * of the grid
    * if the last displayable cell was placed at the bottom right
    * of the grid.  This value is used by the QicsTable to set the
    * appropriate values on the table's scrollbars.
    */
    int lastPageStartColumn();

    /*!
    * Returns the index of the row that is currently displayed at the
    * top of the grid widget.
    */
    inline int topRow() const { return m_topRow; }
    /*!
    * Returns the index of the row that is currently displayed at the
    * bottom of the grid widget.
    */
    inline int bottomRow() const { return m_bottomRow; }

    /*!
    * Returns the index of the leftmost column that is currently displayed
    * in the grid widget.
    */
    inline int leftColumn() const { return m_leftColumn; }
    /*!
    * Returns the index of the rightmost column that is currently displayed
    * in the grid widget.
    */
    inline int rightColumn() const { return m_rightColumn; }

    /*!
    * Returns the index of the row that is currently full displayed at the
    * bottom of the grid widget.
    */
    int fullyVisibleBottomRow();

    /*!
    * Returns the index of the rightmost column that is currently full displayed
    * in the grid widget.
    */
    int fullyVisibleRightColumn();

    /*!
    * Computes row and column values (given the current topRow and
    * leftColumn) for the window coordinates (\a x, \a y ), and
    * returns those values as a QicsICell.  If the coordinates
    * do not correspond to a cell, behavior is dependent on the value
    * of \a nearest.  If \a nearest is \b true, this method will return
    * the nearest cell to the coordinates.  If it is \b false,
    * it will return an invalid cell.
    *
    * \sa rowAt()
    * \sa columnAt()
    */
    QicsICell cellAt(int x, int y, bool nearest) const;

    /*!
    * Returns a row index (given the current topRow and
    * leftColumn) for the window coordinate \a y.  If the coordinate
    * does not correspond to a cell, behavior is dependent on the value
    * of \a nearest.  If \a nearest is \b true, this method will return
    * the nearest row to the coordinate.  If it is \b false,
    * it will return -1.
    */
    int rowAt(int y, bool nearest) const;

    /*!
    * Returns a column index (given the current topRow and
    * leftColumn) for the window coordinate \a x.  If the coordinate
    * does not correspond to a cell, behavior is dependent on the value
    * of \a nearest.  If \a nearest is \b true, this method will return
    * the nearest row to the coordinate.  If it is \b false,
    * it will return -1.
    */
    int columnAt(int x, bool nearest) const;

    /*!
    * Request that the table scroll left \a num columns.
    */
    void scrollLeft(int num);
    /*!
    * Request that the table scroll right \a num columns.
    */
    void scrollRight(int num);
    /*!
    * Request that the table scroll up \a num rows.
    */
    void scrollUp(int num);
    /*!
    * Request that the table scroll down \a num rows.
    */
    void scrollDown(int num);

    /*!
    * Returns \b true if the grid has a fixed height corresponding
    * to the height of the viewport (i.e. it cannot scroll vertically),
    * \b false otherwise.
    */
    inline bool isHeightFixed() const { return m_heightFixed; }
    /*!
    * Returns \b true if the grid has a fixed width corresponding
    * to the width of the viewport (i.e. it cannot scroll horizontally),
    * \b false otherwise.
    */
    inline bool isWidthFixed() const { return m_widthFixed; }

    /*!
    * If \a set is \b true, fixes the height of the grid to the height
    * of the viewport (i.e. it cannot scroll).  If \a set is \b false,
    * the grid is not fixed and may scroll vertically.  This is commonly
    * used when using a grid to show frozen rows.
    */
    void fixHeightToViewport(bool set);
    /*!
    * If \a set is \b true, fixes the width of the grid to the width
    * of the viewport (i.e. it cannot scroll).  If \a set is \b false,
    * the grid is not fixed and may scroll horizontally.  This is commonly
    * used when using a grid to show frozen columns.
    */
    void fixWidthToViewport(bool set);

    /*!
    * Returns the number of visible rows in the grid.
    */
    int visibleRows();
    /*!
    * Returns the number of visible columns in the grid.
    */
    int visibleColumns();

    /*!
    * Returns the current cell of the grid widget. Note that if there
    * is no current cell (i.e. the user has not selected a cell), the
    * returned cell will be invalid.
    */
    virtual inline QicsICell currentCell() const
    { return gridInfo().currentCell(); }

    /*!
    * Returns the now editing cell of the grid widget. Note that if there
    * is no editing cell now, the returned cell will be invalid.
    */
    inline QicsICell editingCell() const {return myEditingCell;}

    /*!
    * Returns the text that should be displayed in a tooltip
    * when the user hovers the mouse pointer over this cell.
    * This method will return an empty string if the grid property
    * CellOverflowBehavior is not set to ToolTip.
    */
    QString tooltipText(const QicsICell &cell) const;

    virtual void orderRowsBy(const QVector<int> &columns,
        Qics::QicsSortOrder order = Qics::Ascending,
        DataItemComparator func = 0);

    virtual void orderColumnsBy(const QVector<int> &rows,
        Qics::QicsSortOrder order = Qics::Ascending,
        DataItemComparator func = 0);

    ///////// Traversal Methods ///////////

    /*!
    * If the key event in \a ke corresponds to a traversal action,
    * this method performs the appropriate traversal action and returns
    * \b true.  If the key event does not correspond to a traversal
    * action, this method returns \b false.
    */
    virtual bool handleTraversalKeys(QKeyEvent *ke);

    /*!
    * Returns \b true is grid is frozen otherwise returns \b false;
    */
    virtual bool isFrozen() {return !m_isMainGrid;}

    /*!
    * Focus next widget in focus chain.
    */
    bool focusNextWidget();

    /*!
    * Focus previous widget in focus chain.
    */
    bool focusPreviousWidget();

    /*!
    * Should cell be selected on traversing
    */
    inline bool selectOnTraverse() const {return m_selectOnTraverse;}

    /*!
    *
    */
    inline void setSelectOnTraverse(bool select) {m_selectOnTraverse = select;}

public slots:
    /*!
    * Sets the top row that will be displayed in the grid.
    *
    * This method causes a layout computation and a redraw.
    */
    void setTopRow(int value);

    /*!
    * Sets the left column that will be displayed in the grid.
    *
    * This method causes a layout computation and a redraw.
    */
    void setLeftColumn(int value);

    /*!
    * Sets whether this grid is the main grid of the table.
    */
    inline void setIsMainGrid(bool yes) { m_isMainGrid = yes; }

    /*!
    * Reset any internal state (selections, editing, etc) for the grid
    */
    virtual void reset();

    /*!
    * Causes a recompute cell positions.
    */
    virtual void recomputeCellPositions(Qics::QicsIndexType indexType = Qics::RowAndColumnIndex);

    /*!
    * Causes a layout computation and a redraw.
    */
    virtual void recomputeAndDraw(Qics::QicsIndexType indexType = Qics::RowAndColumnIndex);

    /*!
    * Causes a layout computation and a redraw.
    */
    virtual void recomputeAndDraw(Qics::QicsIndexType indexType, int startIndex, int endIndex);

    /*!
    * Causes a complete reset of the grid wiget,
    * a layout computation and a redraw.
    */
    virtual void resetAndDraw();

    /*!
    * Causes a reset of the grid wiget,
    * a layout computation and a redraw if it needed.
    */
    virtual void resetAndDraw(int position, int num_items_added_removed, Qics::QicsIndexType row_or_col);

    /*!
    * Causes a redraw of the grid, but not a layout computation.
    *
    * This method will redraw the entire grid.
    */
    virtual void redraw();
    /*!
    * Causes a redraw of the grid, but not a layout computation.
    *
    * This method will redraw only the portion of the grid
    * specified in \a region.
    */
    virtual void redraw(const QicsRegion &region);

    /*!
    * Causes a redraw of the grid, but not a layout computation.
    *
    * This method will redraw only the portion of the grid
    * specified in \a span.
    */
    virtual void redraw(const QicsSpan &span);

    /*!
    * Sets the number of visible rows in the grid.
    * function sets value independently of scroll bar.
    */
    void setVisibleRows(int num);
    /*!
    * Sets the number of visible columns in the grid.
    */
    void setVisibleColumns(int num);

    /*!
    * Slot for the QicsStyleManager object to use to signal a change
    * of cell property \a prop in the table.  Cells in the specified
    * \a region have been changed.
    *
    * This method may cause both a layout computation and a redraw of
    * the grid.
    */
    virtual void handleCellPropertyChange(const QicsRegion &region,
        QicsCellStyle::QicsCellStyleProperty prop,
        bool visual_coords);

    /*!
    * Slot for the QicsStyleManager object to use to signal a change
    * of grid property \a prop in the table.
    *
    * This method may cause both a layout computation and a redraw of
    * the grid.
    */
    virtual void handleGridPropertyChange(QicsGridStyle::QicsGridStyleProperty prop);

    /*!
    * Draws a resizing bar for row/column \a idx of a header of type \a type.
    * The resizing bar is drawn at x/y value \a pos.
    */
    void drawHeaderResizeBar(int idx, int pos, Qics::QicsHeaderType type);

    /*!
    * Slot for various QicsTable subwidgets to use to request a traversal
    * in a certain direction.
    */
    void traverse(Qics::QicsScrollDirection dir);

    /*!
    * Traverse to cell (\a row, \a col ).  This will cause the
    * specified cell to become visible in the grid, if it is not
    * already.  If \a select_cell is \b true, the cell will become
    * the sole contents of the selection list.
    */
    virtual bool traverseToCell(int row, int col, bool select_cell = true);

    /*!
    * Returns true is it is possible to move the cursor to cell (\a row, \a col ).
    * Returns false if this cell is disabled, or coordinates are wrong.
    */
    virtual bool canTraverseToCell(int row, int col);

    /*!
    * Moves the current cell to the top-left cell in the grid's
    * current viewport.
    */
    virtual bool traverseToBeginningOfTable();
    /*!
    * Moves the current cell to the bottom-right cell in the grid's
    * current viewport.
    */
    virtual bool traverseToEndOfTable();
    /*!
    * Moves the current cell to the leftmost cell of the current row
    * in the grid's current viewport.
    */
    virtual bool traverseToBeginningOfRow();
    /*!
    * Moves the current cell to the rightmost cell of the current row
    * in the grid's current viewport.
    */
    virtual bool traverseToEndOfRow();

    /*!
    * Moves the current cell to the bottommost cell of the current column
    * in the grid's current viewport.
    */
    virtual bool traverseToEndOfColumn();

    /*!
    * Moves the current cell to the upmost cell of the current column
    * in the grid's current viewport.
    */
    virtual bool traverseToBeginningOfColumn();

    /*!
    * Moves the current cell to the nearest cell to the left of the
    * current cell that is enabled.
    */
    virtual bool traverseLeft();
    /*!
    * Moves the current cell to the nearest cell to the right of the
    * current cell that is enabled.
    */
    virtual bool traverseRight();
    /*!
    * Moves the current cell to the nearest cell above the
    * current cell that is enabled.
    */
    virtual bool traverseUp();
    /*!
    * Moves the current cell to the nearest cell below the
    * current cell that is enabled.
    */
    virtual bool traverseDown();

    /*!
    * Moves the current cell to one page up.
    */
    virtual bool traversePageUp();

    /*!
    * Moves the current cell to one page down.
    */
    virtual bool traversePageDown();

    /*!
    * Slot for various QicsTable subwidgets to use to request that
    * the grid should cause the cell (\a row, \a col ) to be edited.
    */
    bool editCell(int row, int col);

    /*!
    * Slot for various QicsTable subwidgets to use to request that
    * the grid should cause the current cell to be edited.
    */
    virtual bool editCurrentCell();

    /*!
    * Slot for various QicsTable subwidgets to use to request that
    * the grid should cause the current cell to not be able to be edited.
    */
    void uneditCurrentCell();

    /*!
    * Returns whether the grid allows editing.
    */
    inline bool editable() const { return m_editable; }

    /*!
    * Sets whether the grid allows editing.
    */
    void setEditable(bool b);

protected slots:
    /*!
    * \internal
    * Handler for rows deletion.
    */
    void handleRowsChanges(int num, int start_position);

    /*!
    * \internal
    * Handler for columns deletion.
    */
    void handleColumnsChanges(int num, int start_position);

    void hideResizeBar();

    virtual void timerScrollingEvent();

protected:
    /*!
    * Get the drop event, convert the position to a cell and call dropAt.
    * Subclasses may override this if they want to do special processing
    */
    virtual void dropEvent(QDropEvent *event);

    virtual void dragEnterEvent(QDragEnterEvent *event);

    /*!
    * Called from dropEvent to actually do the drop processing.
    * The default behavior is to ignore the drop.
    * Subclasses should override this method if they want
    * to accept drops.
    */
    virtual void dropAt(QDropEvent *event, const QicsICell &cell);

signals:
    /*!
    * This signal is emitted when the user presses a mouse button
    * in the grid.  The indices of the cell, the button that was pressed,
    * and the position of the mouse pointer in the grid are passed as
    * parameters.
    * \sa clicked(), doubleClicked()
    */
    void pressed(int row, int col, int button, const QPoint &pos);

    /*!
    * This signal is emitted when the user clicks (presses and releases
    * in the same cell) a mouse button in the grid.  The indices of
    * the cell, the button that was pressed, and the position of the mouse
    * pointer in the grid are passed as parameters.
    * \sa pressed(), doubleClicked()
    */
    void clicked(int row, int col, int button, const QPoint &pos);

    /*!
    * This signal is emitted when the user double clicks a mouse button
    * in the grid.  The indices of the cell, the button that was pressed,
    * and the position of the mouse pointer in the grid are passed as parameters.
    * \sa clicked()
    */
    void doubleClicked(int row, int col, int button, const QPoint &pos);

    /*!
    * This signal is emitted when the grid wishes to request a scroll
    * in direction \a direction for \a num number of rows or columns.
    */
    void scrollRequest(Qics::QicsScrollDirection direction, int num);

    /*!
    * This signal is emitted when the grid recomputes its last page
    * information.  The QicsTable widget receives this signal and should
    * set the scrollbar min and max values appropriately.
    */
    void newBoundsInfo();

    /*!
    * This signal is emitted when traversed to other cell.
    */
    void uneditRequest();

    /*!
    * This signal is emitted when cell edit just've been started.
    */
    void cellEditStarted(int row, int col);

    /*!
    * This signal is emitted when cell edit ended.
    */
    void cellEditEnded(int row, int col);

    void focusNextPrevGridRequest(const QicsScreenGrid*,bool next);

    void hideResizeBarRequest();

    /*!
    * Signals when table-wide control key was pressed.
    * \since 2.2
    */
    void wideKeyPressed(QKeyEvent *event);

    /*!
    * Signals when frame style was updated.
    * \since 2.4.4
    */
    void frameStyleUpdated();

protected:
    /*!
    * Internal overload
    */
    virtual bool event(QEvent *event);
    /*!
    * Sets the grid's current cell
    */
    inline void setCurrentCell(const QicsICell &cell)
    { gridInfo().setCurrentCell(cell); }

    /*!
    * Performs any needed layout operations (computing cell positions
    * and/or last page information).
    */
    virtual void layout();

    /*!
    * Computes cell positions based on QFrame::contentsRect and
    * the current top, left cell.
    */
    virtual void computeCellPositions(Qics::QicsIndexType indexType = Qics::RowAndColumnIndex);

    /*!
    * Draws the contents of the grid widget within the grid's frame border.
    */
    virtual void paintEvent(QPaintEvent *ev);

    /*!
    * Repaints the portion of the grid specified by \a rect that
    * is currently visible.
    * If \a painter is non-zero, it will be used to draw the grid.
    */
    virtual QRect paintRegion(const QRect &rect, QPainter *painter);

    /*!
    * Repaints the portion of the grid specified by \a region that is
    * currently visible.  If \a painter is non-zero, it will
    * be used to draw the grid.
    */
    virtual QRect paintRegion(const QicsRegion &region, QPainter *painter, bool compute_cell_possitions);

    /*!
    * Clear any unused space in the widget
    */
    void clearUnusedRect(QPainter *painter);

    virtual QRect drawCell(int row, int col, int x, int y,
        bool look_for_overflower, QPainter *painter, QicsGrid::DrawCellMode mode);

    virtual bool prepareToDraw(int row, int col, const QRect &rect,
        QPainter *painter);

    virtual void extendSelection(int,int){;}

    /*!
    * Computes how many rows and columns would be displayed if the
    * bottom-right cell was in the bottom-right spot on the grid.
    * This information will be used by the table to set the correct
    * values on the scrollbars.
    */
    virtual void computeLastPage(Qics::QicsIndexType indexType = Qics::RowAndColumnIndex);

    /*!
    * The grid widget's resize event handler.  It will cause a
    * layout computation and a redraw of the entire grid.
    */
    virtual void resizeEvent( QResizeEvent *r);

    /*!
    * The grid widget's mouse press event handler.  If the event
    * took place in a valid cell, this handler first calls the cell display
    * object of the appropriate cell to let it handle this event.
    * If the event is not handled, #handleMousePressEvent() is called.
    * In all valid cases, the #pressed() signal is emitted.
    */
    virtual void mousePressEvent( QMouseEvent *m );

    /*!
    * The grid widget's mouse release event handler.  If the event
    * took place in a valid cell, this handler first calls the cell display
    * object of the appropriate cell to let it handle this event.
    * If the event is not handled, #handleMouseReleaseEvent() is called.
    * In all valid cases, the #clicked() signal is emitted if the button
    * was released in the same cell in which it was originally pressed.
    */
    virtual void mouseReleaseEvent( QMouseEvent *m );

    /*!
    * The grid widget's mouse double click event handler.  If the event
    * took place in a valid cell, this handler first calls the cell display
    * object of the appropriate cell to let it handle this event.
    * If the event is not handled, #handleMousePressEvent() is called.
    * In all valid cases, the #doubleClicked() signal is emitted.
    */
    virtual void mouseDoubleClickEvent( QMouseEvent *m );

    /*!
    * The grid widget's mouse movement event handler.  If the event
    * took place in a valid cell, this handler first calls the cell display
    * object of the appropriate cell to let it handle this event.
    * If the event is not handled, #handleMouseMoveEvent() is called.
    */
    virtual void mouseMoveEvent( QMouseEvent *m );

    /*!
    * The grid widget's key press event handler.  If the grid has a
    * current cell, this handler first trys to handle the key press as
    * a traversal command.  If the key pressed was not a traversal command,
    * the cell display object of the current cell is called to let it
    * handle the event.  If the event is not handled, #handleKeyPressEvent()
    * is called.
    */
    virtual void keyPressEvent (QKeyEvent *ke);

    /*!
    * The grid widget's key release event handler.  If the grid has a
    * current cell, #handleKeyPressEvent() is called.
    */
    virtual void keyReleaseEvent (QKeyEvent *ke);

    /*!
    * Custom mouse press event handler.  This method is intended to be
    * reimplemented in subclasses to provide the desired user interaction.
    * Note that \a cell may not be valid (i.e. the event may have taken
    * place outside the grid).
    * \sa mousePressEventHandler()
    */
    virtual void handleMousePressEvent(const QicsICell &cell, QMouseEvent *m);

    /*!
    * Custom mouse release event handler.  This method is intended to be
    * reimplemented in subclasses to provide the desired user interaction.
    * Note that \a cell may not be valid (i.e. the event may have taken
    * place outside the grid).
    * \sa mouseReleaseEventHandler()
    */
    virtual void handleMouseReleaseEvent(const QicsICell &cell, QMouseEvent *m);

    /*!
    * Custom mouse double click event handler.  This method is intended to be
    * reimplemented in subclasses to provide the desired user interaction.
    * Note that \a cell may not be valid (i.e. the event may have taken
    * place outside the grid).
    * \sa mouseDoubleClickEventHandler()
    */
    virtual void handleMouseDoubleClickEvent(const QicsICell &cell, QMouseEvent *m);

    /*!
    * Custom mouse movement event handler.  This method is intended to be
    * reimplemented in subclasses to provide the desired user interaction.
    * \sa mouseMoveEventHandler()
    */
    virtual void handleMouseMoveEvent(const QicsICell &cell, QMouseEvent *m);

    /*!
    * Custom key press event handler.  This method is intended to be
    * reimplemented in subclasses to provide the desired user interaction.
    * \sa keyPressEventHandler()
    */
    virtual void handleKeyPressEvent(const QicsICell &cell, QKeyEvent *k);

    /*!
    * Custom key release event handler.  This method is intended to be
    * reimplemented in subclasses to provide the desired user interaction.
    * \sa keyReleaseEventHandler()
    */
    virtual void handleKeyReleaseEvent(const QicsICell &cell, QKeyEvent *k);

    /*!
    * Returns \b true if the cell (\a row, \a col ) is currently
    * visible in the grid, \b false otherwise.
    */
    bool isCellVisible(int row, int col) const;

    /*!
    * Scrolls the grid (if necessary) so that cell (\a row, \a col )
    * is completely visible.
    */
    virtual void makeCellFullyVisible(int row, int col);

    /*!
    * \internal
    * Updates the widget based on the current value of viewport().
    */
    void updateViewport();

    /*!
    * \internal
    * Updates the widget's internal lineWidth property with the current
    * value of the FrameLineWidth grid property.
    */
    void updateLineWidth();

    /*!
    * \internal
    * Updates the widget's internal frameStyle property with the current
    * value of the FrameStyle grid property.
    */
    void updateFrameStyle();

    /*!
    * Traverses in a manner indicated by \a orient, in the direction
    * indicated by \a forward.  For example, traverse(Qt::Vertical, true)
    * traverses down, while traverse(Qt::Horizontal, false) traverses left.
    */
    //void traverse(Qt::Orientation orient, bool forward);

    /*! \internal
    \since 2.2
    */
    virtual bool doTraverseHorizontal(bool leftwards);
    /*! \internal
    \since 2.2
    */
    virtual bool doTraverseVertical(bool upwards);
    /*! \internal
    \since 2.2
    */
    virtual bool doTraverseCellVertical(int row, bool upwards);
    /*! \internal
    \since 2.2
    */
    virtual bool doTraverseCellHorizontal(int col, bool leftwards);

    /*!
    * Select cell (\a row, \a col ).  In this class, this method is a
    * no-op.  It is intended that subclasses will reimplement this
    * method based on the selection paradigm for the class.
    */
    virtual void selectCell(int, int) {;}

    /*!
    * Place the current entry widget in the correct position on the grid.
    * Called after a grid layout has been performed.
    */
    void placeEntryWidget();

    /*!
    * Prepare for a drag operation from cell \a cell.
    */
    virtual void prepareDrag(const QicsICell &cell, Qt::DropAction mode);

    /*!
    * Start a drag operation with drag mode \a mode.
    */
    virtual void startDrag(Qt::DropAction mode);

    /*!
    * Finish a drag operation begun with drag mode \a mode.  If \a remove
    * is true and \a target is a widget with the same parent as this grid,
    * the dragged selection is cleared.
    */
    virtual void finishDrag(Qt::DropAction mode, bool remove, QWidget *target);

    /*!
    * Returns the drag object to be used in a drag operation with mode \a mode.
    */
    QMimeData *dragObject(Qt::DropAction mode);

    /*!
    * Returns \b true if the dragged object specified in \a event can be
    * accepted by the grid in cell \a cell.
    */
    virtual bool canDropAt(QDragMoveEvent *event, const QicsICell &cell) const;

    /*!
    * Emits grid signals \a pressed, \a clicked and \a doubleClicked
    * after \a event is occured in \a cell.
    */
    virtual void emitMouseSignals(const QicsICell &cell, QMouseEvent *event);

    /*! \internal
    \since 2.2
    */
    virtual void doScrollTraverse() {}

    /*! \internal
    \since 2.2
    */
    bool doCheckScrollUp(QMouseEvent *m);
    /*! \internal
    \since 2.2
    */
    bool doCheckScrollDown(QMouseEvent *m);
    /*! \internal
    \since 2.2
    */
    bool doCheckScrollLeft(QMouseEvent *m);
    /*! \internal
    \since 2.2
    */
    bool doCheckScrollRight(QMouseEvent *m);

    /*! \internal
    \since 2.4.4
    */
    void stopScrollingTimer() const;

    /*! \internal
    \since 2.4.4
    */
    void startScrollingTimer() const;

    //////////////// Data Members /////////////////

    /*!
    * \internal
    * Number of rows that have been requested to be visible.
    */
    int myReqVisibleRows;
    /*!
    * \internal
    * Number of columns that have been requested to be visible.
    */
    int myReqVisibleColumns;

    /*!
    * \internal
    * Keeps track of the last (XOR'd) drawn resize line
    */
    int myLastResizeLinePosition;

    /*!
    * \internal
    * Flag that is set when the grid needs to recompute its layout.
    * This is used when something happens that would cause a layout
    * computation to occur, except that the widget is not visible.
    * In this case, we delay the layout computation until necessary.
    */
    Qics::QicsIndexType myNeedsRecomputeCellsFlag;
    /*!
    * \internal
    * Flag that is set when the grid needs to recompute its last page
    * layout.  This is used when something happens that would cause
    * a last page layout computation to occur, except that the widget
    * is not visible. In this case, we delay the layout computation
    * until necessary.
    */
    Qics::QicsIndexType myNeedsRecomputeLastPageFlag;

    /*!
    * \internal
    * Flag that is set when the grid needs to be repainted, but cannot
    * because the global repaint behavior flag is set to RepaintOff.
    */
    bool myNeedsRepaintFlag;

    /*!
    * \internal
    * Stores the index of the row that would be displayed at the
    * top of the grid if the
    * bottom-right cell was in the bottom-right spot on the grid.
    */
    int myLastPageStartRow;
    /*!
    * \internal
    * Stores the index of the column that would be displayed at the
    * right of the grid if the
    * bottom-right cell was in the bottom-right spot on the grid.
    */
    int myLastPageStartColumn;

    /*!
    * \internal
    * List of cells that need to be displayed when they become not visible.
    * More precisely, it is a list of the *model coordinates* of
    * WidgetDisplay cells that we have drawn.   After scrolling or
    * sorting, ee can consult that list, map back to visual coordinates
    * and see if they are still visible.
    */
    QicsICellV m_cellsToNotify;

    /*!
    * \internal
    * Stores whether the grid's height is fixed.
    */
    bool m_heightFixed;
    /*!
    * \internal
    * Stores whether the grid's width is fixed.
    */
    bool m_widthFixed;

    /*!
    * \internal
    * Pointer to current cell is being edited. If no cell edited pointer is NULL.
    * (i.e. there is an entry widget currently displayed for that cell).
    */
    QicsCellDisplay *myEditingCurrentCell;

    /*!
    * \internal
    * Cell that now is editing.
    */
    QicsICell myEditingCell;

    /*!
    * \internal
    * Stores cell which no need to unedit.
    */
    QicsICell myUneditCell;

    /*!
    * \internal
    * Stores whether the grid allows editing.
    */
    bool m_editable;

    /*!
    * \internal
    * Flag set when placing an entry widget so we can ignore focus events
    */
    bool myPlacingEntryWidgetFlag;

    /*!
    * \internal
    * The cell that was most recently pressed.
    */
    QicsICell myPressedCell;

    /*!
    * \internal
    * The cell from which a drag operation was started.
    */
    QicsICell m_dragCell;

    /*!
    * \internal
    * Flag set when this grid is the main grid of the table.
    */
    bool m_isMainGrid;

    /*!
    * \internal
    * Flag set when whole grid area need to be repainted.
    */
    bool m_repaintAll;

    /*!
    * \internal
    * Flag indicates that grid currently in paintEvent().
    */
    bool m_gridInPaintEvent;

    /*!
    * \internal
    * Flag set when QFrame paints for the first time or after resizeEvent.
    */
    bool m_initialRepaint;

    /*!
    * \internal
    * Flag set when needs skip next paint event, for example: to avoid recursive Paint Event.
    */
    bool myForceSkipPaintEvent;

    /*!
    * \internal
    * Flag which indicates whether there is currently a user-initiated
    * selection taking place.
    */
    bool m_doingSelectionFlag;

    /*!
    * \internal
    * Flag set when QDragMoveEvent occur.
    */
    Qt::DropAction myDropAction;

    QRubberBand *m_resizeLine;

    int myLastPressedKey;

    bool m_selectOnTraverse;

    QTimer *timerScrolling;

    QicsICell cellToScroll;

    QicsICell lastFillCell;

    int m_scrollDirec;

    int speedOfScroll;

    QicsRegion m_paintRegion;

      /*!
    * \internal
    * Internal buffer image.
    */
    QPixmap m_imageBuffer;

private:
    friend class QicsKeyboardManager;
};

#endif //QICSSCREENGRID_H


