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

#ifndef QICSTABLEGRID_H
#define QICSTABLEGRID_H

#include "QicsScreenGrid.h"

class QMouseEvent;
class QFocusEvent;
class QDragMoveEvent;
class QEvent;
class QDropEvent;
class QTimer;

class QicsRubberBand;
class QicsSelection;

/*!
* \class QicsTableGrid QicsTableGrid.h
* \brief Internal widget that manages the layout and display
*        of cells, as well as user interaction
*
* QicsTableGrid is a private widget used internally by the QicsTable widget.
* The grid widget is responsible for laying out the cells based on the size of
* the widget and based on various properties that are stored in the
* QicsStyleManager.  The grid does not actually draw anyting, rather it
* determines where each cell should be drawn and calls the appropriate cell
* display object for the actual display of the widget.
*
* Most users of the %QicsTable product should not need to create or modify
* QicsTableGrid objects directly.  The QicsTable widget automatically creates
* the necessary grid objects when needed.  One instance where a user may
* need to modify a grid is to change the mouse and/or keyboard behavior of
* the grids.
*
* The grid widget is also largely responsible for managing user interaction
* with the table.  The grid handles mouse and keyboard events on its window,
* and performs the appropriate action (selection and traversal, primarily).
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsTableGrid: public QicsScreenGrid
{
    Q_OBJECT
public:
    /*!
    * Constructor for the class.  \a w is the grid widget's parent.
    * \a info is used for style and dimension information, as well as
    * the associated data model.
    * Optional arguments \a top_row and \a left_column specify the
    * starting location for the top left cell of the grid.
    */
    QicsTableGrid(QWidget *w, QicsGridInfo &info, int top_row = 0,
        int left_column = 0);

    /*! \internal
    * The destructor (virtual, in case we subclass later).
    */
    virtual ~QicsTableGrid();

    /*!
    * Resets the state of the table grid.  Clears all selections,
    * stops editing of any cells, and causes there to be no current cell.
    */
    virtual void reset();

    /*!
    * Foundry method to create new instances of QicsTableGrid (or a
    * subclass of QicsTableGrid).  Subclasses
    * of QicsTableGrid should implement a similar method.  Methods
    * of this type can be passed to a QicsTable constructor in order
    * to create custom grids.
    */
    typedef QicsTableGrid *(*Foundry)(QWidget *w, QicsGridInfo &info,
        int top_row, int left_column);

    /*!
    * Internal foundry method to create new instances of QicsTableGrid
    * (or a subclass of QicsTableGrid).  Methods of this type can be
    * passed to a QicsTable constructor in order to create custom grids.
    *  A pointer to a previously provided "key" is passed as a parameter.
    * This typedef is not exposed as part of the public API.  It has
    * been added because it's necessary for the Python bindings.
    */
    typedef QicsTableGrid *(*FoundryWithKey)(QWidget *w, QicsGridInfo &info,
        int top_row, int left_column,
        void *key);

    /*!
    * Foundry method to create new instances of QicsTableGrid.  Subclasses
    * of QicsTableGrid should implement a similar method.  Methods
    * of this type can be passed to a QicsTable constructor in order
    * to create custom grids.
    */
    static QicsTableGrid *createGrid(QWidget *w, QicsGridInfo &info,
        int top_row = 0, int left_column = 0);

public slots:
    virtual void handleGridPropertyChange(QicsGridStyle::QicsGridStyleProperty prop);

protected:
    virtual void dropEvent(QDropEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);

    /*!
    * \internal
    * The widget's main event handler.  Reimplemented so the Tab key
    * can be handled before the usual Qt focus handling code
    * swallows it.
    */
    virtual bool event( QEvent *e );

    virtual void handleMousePressEvent(const QicsICell &cell, QMouseEvent *m);
    virtual void handleMouseReleaseEvent(const QicsICell &cell, QMouseEvent *m);
    virtual void handleMouseDoubleClickEvent(const QicsICell &cell, QMouseEvent *m);
    virtual void handleMouseMoveEvent(const QicsICell &cell, QMouseEvent *m);

    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);

    virtual void dragMoveEvent(QDragMoveEvent *event);

    virtual bool canDropAt(QDragMoveEvent *event, const QicsICell &cell) const;
    virtual void dropAt(QDropEvent *event, const QicsICell &cell);

    virtual void selectCell(int row, int col);

    virtual QRect drawRegion(const QicsRegion &region, QPainter *painter, QicsGrid::DrawCellMode mode);

    /*!
    * Sets the grid's anchor selection cell, deleting the storage
    * for the previous anchor cell.
    */
    void setSelectionAnchorCell(QicsICell *cell);

    /*!
    * Sets the grid's current selected cell, deleting the storage
    * for the previous selected cell.
    */
    void setSelectionCurrentCell(QicsICell *cell);

    /*!
    * Perform the beginning of a user-intiated selection.
    */
    void beginSelection(int row, int col);
    /*!
    * Perform the end of a user-intiated selection.
    */
    void endSelection(int row, int col);
    /*!
    * Continue a drag selection to the given cell (\a row, \a col )
    */
    void dragSelection(int row, int col);
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
    * Report an action of a user-intiated selection.
    */
    void reportSelection(int row, int col, Qics::QicsSelectionType stype);

    virtual void doScrollTraverse();

    QRect fillingRect(const QicsICell &cell);

    QicsSelection fillingSelection(const QicsICell &cell);

    /*!
    * \internal
    * Holds the current selection cell.  This cell is the cell which
    * was under the mouse pointer when the last selection action was
    * taken.
    */
    QicsICell *m_selectionCurrentCell;

    /*!
    * \internal
    * Holds the anchor selection cell.  This cell is the cell which
    * was under the mouse pointer when the last selection action was
    * begun.
    */
    QicsICell *m_selectionAnchorCell;

    bool m_exclusiveMode;
    bool m_exclusiveDrag;
    QRect m_fillHandlePickRect;
    QRect m_updateRect;
    QicsRegion m_oldRegion;
    QicsRubberBand *m_fillBand;
};

#endif //QICSTABLEGRID_H


