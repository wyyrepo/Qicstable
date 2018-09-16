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

#ifndef QICSHEADERGRID_H
#define QICSHEADERGRID_H

#include <QDragMoveEvent>
#include <QKeyEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include "QicsScreenGrid.h"
#include "QicsDataItem.h"
#include "QicsICell.h"


/*!
* \class QicsHeaderGrid QicsHeaderGrid.h
* \brief Header object for the QicsTable
*
* QicsHeaderGrid is a widget used internally by the QicsTable widget.  The headerGrid
* widget is responsible for laying out the header cells based on the size of the
* widget and based on various properties that are stored in the QicsStyleManager.
* The headerGrid does not actually draw anyting, rather it determines where each
* cell should be drawn and calls the appropriate cell display object for
* the actual display of the widget.
*
* The headerGrid widget is also largely responsible for managing user interaction
* with the table.  The headerGrid handles mouse and keyboard events on its window,
* and performs the appropriate action (selection and traversal, primarily).
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsHeaderGrid: public QicsScreenGrid
{
    Q_OBJECT
public:
    /*!
    * \internal
    * Constructor for the class.  \a w is the grid widget's parent.
    * \a info is used for style and dimension information, as well as
    * the associated data model.
    * Optional arguments \a top_row and \a left_column specify the
    * starting location for the top left cell of the grid.
    */
    QicsHeaderGrid(QWidget *w, QicsGridInfo &info, Qics::QicsHeaderType type);

    /*!
    * The destructor (virtual, in case we subclass later).
    */
    virtual ~QicsHeaderGrid();

    /*!
    * Returns the type of this headerGrid object.
    */
    inline Qics::QicsHeaderType type() const { return m_type; }

    virtual QicsRegion currentViewport() const;

    /*!
    * Returns the maximum distance (in pixels) that the cursor can be
    * away from a cell margin and still be able to resize a row/column.
    */
    inline int gripThreshold() const { return m_gripThreshold; }
    /*!
    * Sets the maximum distance (in pixels) that the cursor can be
    * away from a cell margin and still be able to resize a row/column.
    */
    inline void setGripThreshold(int gt) { m_gripThreshold = gt; }

    /*!
    * Sets current header grid property \b myIsFrozenHeader to true
    * if it belongs frozen part of table. See the \link #myIsFrozenHeader
    * "myIsFrozenHeader" \endlink property for details.
    */
    inline void setIsFrozenHeader(bool flag) {m_isFrozenHeader = flag;}

    /*!
    * Foundry method to create new instances of QicsHeaderGrid (or a
    * subclass of QicsHeaderGrid).  Subclasses
    * of QicsHeaderGrid should implement a similar method.  Methods
    * of this type can be passed to a QicsTable constructor in order
    * to create custom grids.
    */
    typedef QicsHeaderGrid *(*Foundry)(QWidget *w, QicsGridInfo &info,
        Qics::QicsHeaderType type);

    /*!
    * Internal foundry method to create new instances of QicsHeaderGrid
    * (or a subclass of QicsHeaderGrid).  Methods of this type can be
    * passed to a QicsTable constructor in order to create custom grids.
    *  A pointer to a previously provided "key" is passed as a parameter.
    * This typedef is not exposed as part of the public API.  It has
    * been added because it's necessary for the Python bindings.
    */
    typedef QicsHeaderGrid *(*FoundryWithKey)(QWidget *w, QicsGridInfo &info,
        Qics::QicsHeaderType type,
        void *key);

    /*!
    * Foundry method to create new instances of QicsHeaderGrid.  Subclasses
    * of QicsHeaderGrid should implement a similar method.  Methods
    * of this type can be passed to a QicsTable constructor in order
    * to create custom grids.
    */
    static QicsHeaderGrid *createGrid(QWidget *w, QicsGridInfo &info,
        Qics::QicsHeaderType type);

    virtual QSize sizeHint() const;

signals:
    /*!
    * Signals when the user has finished resizing a row or column.
    * \param idx the index of the row or column that was resized
    * \param old_size the old height/width of the row/column
    * \param new_size the new height/width of the row/column
    * \param type row or column header
    */
    void sizeChange(int idx, int old_size, int new_size, Qics::QicsHeaderType type);

    /*!
    * Signals when the user is in the process of interactively resizing
    * a cell by dragging the mouse.
    * \param idx the index of the row or column that is being resized
    * \param position the X (if a column header) or Y (if a row header) location of the
    * cursor in the grid
    * \param type row or column header
    */
    void resizeInProgress(int idx, int position, Qics::QicsHeaderType type);

    /*!
    * Signals when the user double clicks in the "grip" area between two cells.
    * \param idx the index of the row or column to the top/left of the grip
    *  that was double clicked in
    * \param button the button that was clicked
    * \param type row or column header
    */
    void gripDoubleClicked(int idx, int button, Qics::QicsHeaderType type);

    /*!
    * Signals when the user resizes row header horizontally or column
    * header vertically.
    * \param size value of difference of new size and old size
    * \param type type of header(RowHeader or ColumnHeader)
    */
    void headerResized(int size, int pos, Qics::QicsHeaderType type);

public slots:
    virtual void handleGridPropertyChange(QicsGridStyle::QicsGridStyleProperty prop);
    virtual void handleFrameStyleUpdated();

protected:
    virtual void handleMousePressEvent(const QicsICell &cell, QMouseEvent *m);
    virtual void handleMouseReleaseEvent(const QicsICell &cell, QMouseEvent *m);
    virtual void handleMouseMoveEvent(const QicsICell &cell, QMouseEvent *m);
    virtual void handleMouseDoubleClickEvent(const QicsICell &, QMouseEvent *m);
    virtual void emitMouseSignals(const QicsICell&,QMouseEvent*);
    virtual void keyPressEvent (QKeyEvent *ke);

    virtual bool event( QEvent *e );

    void reset();

    //////////////// Drag & Drop /////////////////

    void dragMoveEvent(QDragMoveEvent *event);

    void dropEvent(QDropEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);

    /*!
    * \internal
    * Determines if the drag data is acceptable to drop
    * at this particular cell.  The rules are interesting.
    * If the cell is a row header, then you can drop
    *     - a bunch of rows (application/vnd.ics.rowlist) from this
    *       GridInfo
    *     - rows or columns from another table in the same application
    *     - vnd.ics.celldata from anywhere
    * The last two are unimplemented, but the idea is that you could
    * drag a few columns from one table and store them as rows in
    * another table, either in the same application or in a different
    * one.
    */
    virtual bool canDropAt(QDragMoveEvent *event, const QicsICell &cell) const;

    virtual void dropAt(QDropEvent *event, const QicsICell &cell);

    void startDrag(Qt::DropAction mode);

    //////////////// Resizing /////////////////

    /*!
    * Determines if the major axis position of the pointer (\a pos )
    * is within the grip threshold allowance.
    */
    bool isWithinResizeThreshold(int x, int y, int *in_row = 0, int *close_row = 0, int *in_column = 0, int *close_column = 0) const;

    /*!
    * Set or unset the headerGrid's resize cursor based on the position of
    * the mouse cursor.  Returns \b true if the resize cursor was set.
    */
    bool setResizeCursor( QMouseEvent *m );

    /*!
    * Handles the end of a cell resize operation
    */
    void finishResize();

    //////////////// Selection /////////////////

    /*!
    * Perform the beginning of a user-intiated selection.
    */
    void beginSelection(int row, int col);
    /*!
    * Perform the end of a user-intiated selection.
    */
    void endSelection(int row, int col);
    /*!
    * Extend a user-intiated selection.
    */
    void extendSelection(int row, int col);
    /*!
    * Add a new cell to a user-intiated selection.
    */
    void addSelection(int row, int col);
    /*!
    * Clear a user-intiated selection.
    */
    void clearSelection();
    /*!
    * Handle an in-progress selection drag
    */
    void dragSelection(QMouseEvent *m);
    /*!
    * Report an action of a user-intiated selection.
    */
    void reportSelection(int row, int col, Qics::QicsSelectionType stype);

    virtual void selectCell(int row, int col);

    //////////////// Traversing ///////////////////

    virtual void doScrollTraverse();

    virtual bool traverseToCell(int row, int col, bool select_cell = true);

    //////////////// Data Members /////////////////

    /*!
    * \internal
    * the type of this headerGrid object.
    */
    Qics::QicsHeaderType m_type;

    /*!
    * \internal
    * the threshold of the grip between the cells.
    */
    int m_gripThreshold;

    /*!
    * \internal
    * is the mouse pointer currently in a cell grip?
    */
    bool m_mouseInGrip;

    /*!
    * \internal
    * are we currently performing a resize operation?
    */
    bool m_doingResize;

    /*!
    * \internal
    * the cell which is expanding or contracting (valid only
    * when myCanResize=true)
    */
    QicsICell m_expandingCell;

    /*!
    * \internal
    * the original size of the cell that is being expanded
    * (valid only when myCanResize=true)
    */
    QSize m_expandingCellSize;

    /*!
    * \internal
    * the start position of the cell that is being expanded
    * (valid only when myCanResize=true)
    */
    QPoint m_expandingCellStartPosition;

    /*!
    * \internal
    * the current positon of the cell that is being expanded
    * (valid only when myCanResize=true)
    */
    QPoint m_expandingCellCurrentPosition;

    /*!
    * \internal
    * index of the anchor row/col of a selection
    */
    int m_anchorIndex;

    /*!
    * \internal
    * index of the current row/col of a selection
    */
    int m_selectionIndex;

    /*!
    * \internal
    * is current header locates over frozen part of table.
    * This flag needs to enable or disable cells resizing and moving
    * in frozen parts.
    */
    bool m_isFrozenHeader;
    /*!
    * \internal
    * is header began to resize(row header horizontally
    * and column header vertically)
    */
    bool m_isHeaderBeganResize;

    /*!
    * \internal
    * start position for headers resizing
    */
    int m_startResizingPos;

    /*!
    * \internal
    * have I drop row/column after target.
    */
    bool m_haveIdropAfter;

    /*!
    * \internal
    * Last mouse press coords.
    */
    QPoint m_pressedCoords;

    QPoint m_spotDelta;

    QPixmap m_dragPixmap;

    /*!
    * \internal
    * sets at double click, drops at release.
    */
    bool m_doubleClicked;
};


#endif //QICSHEADERGRID_H


