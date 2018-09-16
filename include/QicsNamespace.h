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

#ifndef QICSNAMESPACE_H
#define QICSNAMESPACE_H

#include <QtGlobal>
#include <QtDebug>
#include <limits.h>

// Note: qicstable_version mask = N.N.N (no more, no less, N-number)
// Else brakes PyQicsTable
#define QICSTABLE_VERSION "qicstable_version: 3.0.0 (07-Feb-14)"

// Note: numeric version to be able to use conditional compilation (using #if directive).
// This macro expands a numeric value of the form 0xMMNNPP (MM = major, NN = minor, PP = patch) that
// specifies QicsTable's version number
#define QICSTABLE_VERSION_NUMERIC 0x030000

// Note: QicsTable's version number as string
#define QICSTABLE_VERSION_STR  "3.0.0"

// Note: packaging date string
#define QICSTABLE_PACKAGEDATE  "07-Feb-2014"

// Note: numeric date definition to be able to use conditional compilation (using #if directive).
// This macro expands a numeric value of the form 0xYYMMDD (YY = year, MM = month, DD = day) that
// specifies QicsTable's packaging date.

#define QICSTABLE_PACKAGEDATE_NUMERIC 0x140207

#ifdef QICS_STATICLIB
#  undef QICS_SHAREDLIB
#  define QICS_EXPORT
#else
#  ifdef QICS_MAKEDLL
#   define QICS_EXPORT Q_DECL_EXPORT
#  else
#   define QICS_EXPORT Q_DECL_IMPORT
#  endif
#endif

#ifndef QICSTABLE_EVAL
#  if !defined(QICS_EXPORT)
#    define QICS_EXPORT
#  endif
#endif

////////////////////////////////////////////////////////////////////////
/*! \file */
////////////////////////////////////////////////////////////////////////

/*!
* \class Qics QicsNamespace.h
* \brief A common superclass for all Qics classes
*
* Qics is a common superclass for all Qics classes.  The Qics class
* contains mostly type declarations at the moment.
*/

class QICS_EXPORT Qics
{
public:

    static const int QicsLAST_ROW = INT_MAX-1;
    static const int QicsLAST_COLUMN = INT_MAX-1;

    /*! Denotes global key actions available for the table grid.
    * \arg \b Del_DeleteSelected Pressing Delete removes data from the selected cells.
    * \arg \b CtrlA_SelectAll Pressing Control+A selects all the cells in the grid.
    */
    enum QicsWideKeyAction {
        Del_DeleteSelected,
        CtrlA_SelectAll,
        CtrlC_Copy,
        CtrlV_Paste,
        CtrlX_Cut
    };

    /*!
    * Denotes the box borders of cell region.
    * \arg \b TopBorder Top border of the region.
    * \arg \b BottomBorder Bottom border of the region.
    * \arg \b LeftBorder Left border of the region.
    * \arg \b RightBorder Right border of the region.
    */
    enum QicsBoxBorders {
        TopBorder = 0x01,
        BottomBorder = 0x02,
        LeftBorder = 0x04,
        RightBorder = 0x08,
        AllBorders = TopBorder | BottomBorder | LeftBorder | RightBorder
    };

    /*!
    * Denotes the copying policy during copy/paste and drag/drop operations.
    * \arg \b CopyData Copy data from cells.
    * \arg \b CopyAttributes Copy cell attributes.
    * \arg \b CopyDimensions Copy cell dimensions (width/height).
    * \arg \b CopySpans Copy spanned cells.
    * \arg \b CopyTopHeaderData Copy data from Top Header.
    * \arg \b CopyBottomHeaderData Copy data from Bottom Header.
    * \arg \b CopyLeftHeaderData Copy data from Left Header.
    * \arg \b CopyRightHeaderData Copy data from Right Header.
    */
    enum QicsCopyPolicy {
        CopyData = 0x01,
        CopyAttributes = 0x02,
        CopyDimensions = 0x04,
        CopySpans = 0x08,
        CopyTopHeaderData = 0x10,
        CopyBottomHeaderData = 0x20,
        CopyLeftHeaderData = 0x30,
        CopyRightHeaderData = 0x40,
        CopyAll = CopyData | CopyAttributes | CopyDimensions | CopySpans | CopyTopHeaderData | CopyBottomHeaderData | CopyLeftHeaderData | CopyRightHeaderData
    };

    /*!
    * Denotes the clearing policy during cut and delete operations.
    * \arg \b ClearData Clear data from cells.
    * \arg \b ClearAttributes Clear cell attributes.
    * \arg \b ClearSpans Clear spanned cells.
    */
    enum QicsClearPolicy {
        ClearData = 1,
        ClearAttributes = 2,
        ClearSpans = 4,
        ClearAll = 1+2+4
    };

    /*!
    * Denotes the sort algorithm which is used.
    * \arg \b QicsStableSort Used non-destructive, stable sort algorithm.
    * \arg \b QicsQuickSort Default quick sort algorithm.
    */
    enum QicsSortMode {
        QicsStableSort = 0,
        QicsQuickSort
    };

    /*!
    * Denotes the behavior of grids when handling cells whose contents
    * cannot be displayed in the boundaries of the cell.
    * \arg \b Clip Cell contents are clipped at the edges of the cell.
    * \arg \b Overflow The grid will attempt to overflow into adjacent
    *                  cells.
    * \arg \b ToolTip The full contents of the cell will be displayed
    *                 in a tooltip when the mouse pointer is hovered
    *                 over the cell.
    */
    enum QicsCellOverflowBehavior {
        Clip = 0,
        Overflow,
        ToolTip
    };

    /*!
    * Denotes the visual style that the current cell of the table will
    * be drawn with.
    * \arg \b Spreadsheet The current cell will be drawn with a solid border
    *                     and will always use the cell's background color.
    * \arg \b NormalSelected The current cell will be drawn the same as all
    *                        other selected cells.  The border will be as
    *                        specified in the cell's border properties, and
    *                        the background will be the cell's selected color.
    */
    enum QicsCurrentCellStyle {
        Spreadsheet = 0,
        NormalSelected
    };

    /*!
    * \arg \b Normal         The selection will be drawn by the normal way.
    * \arg \b Exclusive      The selection will be drawn in ICS way (use NativeStyle insted).
    * \arg \b NativeStyle    The selection will be drawn in ICS way.
    * \arg \b ExcelStyle     The selection will be drawn
    *                        with border like in MS Excel.
    */
    enum QicsSelectionStyle {
        Normal = 0,
        Exclusive,                  //#### TODO: it should be removed in 2.5
        NativeStyle = Exclusive,
        ExcelStyle
    };

    /*!
    * \arg \b Follow    The current cell will follow the selection (QicsTable style).
    * \arg \b Static   The current cell will stay on selection anchor (MS Excel style).
    * \since 2.3
    */
    enum QicsSelectCurrentCellPolicy {
        Follow = 0,
        Static
    };

    /*! \internal
    * Denotes the type of fill handle event.
    * \since 2.3
    */
    enum QicsFillHandleEventType {
        FillHandlePressed = 0,
        FillHandleReleased,
        FillHandleDoubleClicked,
        FillHandleDragged
    };

    /*!
    * Denotes the policy for displaying text that is too long to
    * fit in its cell.
    * \arg \b AllowPartial Shows as much of the text as possible.
    * \arg \b NoDisplayOnPartial Do not display text if it cannot
    *         fit in the cell.
    * \arg \b UseClippedSymbol Display a clipped symbol at the end
    *         of the partial text.
    */
    enum QicsGridCellClipping {
        AllowPartial = 0,
        NoDisplayOnPartial,
        UseClippedSymbol
    };

    /*!
    * Specifies a type of grid object.
    * \arg \b TableGrid A grid used to display normal table data.
    * \arg \b RowHeaderGrid A grid used to display row header data.
    * \arg \b ColumnHeaderGrid A grid used to display column header data.
    */
    enum QicsGridType {
        TableGrid = 0,
        RowHeaderGrid,
        ColumnHeaderGrid
    };

    /*!
    * Specifies the type of table header.
    *
    * - \b HeaderRow A row header.
    * - \b HeaderColumn A column header.
    */
    enum QicsHeaderType {
        RowHeader = 0,
        ColumnHeader
    };

    /*!
    * Specifies the type of table index.
    *
    * - \b RowIndex A row index.
    * - \b ColumnIndex A column index.
    */
    enum QicsIndexType {
        NoIndex = 0,
        RowIndex,
        ColumnIndex,
        RowAndColumnIndex
    };

    /*!
    * Denotes the style of lines and borders in the table widget.
    * \arg \b None No line or border.
    * \arg \b Plain A simple, plain line or border.
    * \arg \b Raised A raised line (using shadows).
    * \arg \b Sunken A sunken line (using shadows).
    */
    enum QicsLineStyle {
        None = 0,
        Plain,
        Raised,
        Sunken
    };

    /*!
    * Specifies the recalculation and repainting behavior of tables and grids.
    *
    * - \b RepaintOff Do not perform any drawing or dimension calculations.
    * - \b RepaintOn Draw as needed, keep all dimensions up to date.
    * - \b RepaintCalcOnly Do not draw, but keep all dimensions up to date.
    */
    enum QicsRepaintBehavior {
        RepaintOff = 0,
        RepaintOn = 1,
        RepaintCalcOnly = 2
    };

    /*!
    * Specifies the various modes of QicsTable's scroll bars.
    *
    * - \b Auto QicsTable shows a scroll bar when the content is too large
    *           to fit and not otherwise.
    * - \b AlwaysOff QicsTable never shows a scroll bar.
    * - \b AlwaysOn QicsTable always shows a scroll bar.
    */
    enum QicsScrollBarMode {
        Auto = 0,
        AlwaysOff,
        AlwaysOn
    };

    /*!
    * Denotes a direction for scrolling operations, or that an
    * absolute scroll should occur.
    * \arg \b ScrollNone no scroll allowed.
    * \arg \b ScrollUp scrolling up on a certain number of rows.
    * \arg \b ScrollDown scrolling down on a certain number of rows.
    * \arg \b ScrollLeft scrolling left on a certain number of columns.
    * \arg \b ScrollRight scrolling right on a certain number of columns.
    * \arg \b ScrollToRow scrolling up or down to a certain row.
    * \arg \b ScrollToColumn scrolling left or right to a certain column.
    */
    enum QicsScrollDirection {
        ScrollNone = 0,
        ScrollUp = 1,
        ScrollDown = 2,
        ScrollLeft = 4,
        ScrollRight = 8,
        ScrollToRow = 16,
        ScrollToColumn = 32
    };

    /*!
    * Denotes the selection policy for the table.
    * \arg \b SelectNone no selection allowed.
    * \arg \b SelectSingle single cell selection.
    * \arg \b SelectMultiple multiple cell selection (including
    *         non-contiguous regions).
    * \arg \b SelectSingleRow selecting a cell selects the entire row,
    *         only one row can be selected.
    * \arg \b SelectMultipleRow selecting a cell selects the entire row,
    *         multiple rows can be selected.
    */
    enum QicsSelectionPolicy {
        SelectNone = 0,
        SelectSingle,
        SelectMultiple,
        SelectSingleRow,
        SelectMultipleRow
    };

    /*! \internal
    * Specifies the selection action that took place in the grid widget.
    * These actions are signaled to the QicsSelectionManager when
    * the user performs any selection action.
    *
    * - \b SelectionNone No selection.
    * - \b SelectionBegin The user has begun a new selection.
    * - \b SelectionDrag The user is modifying a selection by clicking
    *      and dragging the mouse.
    * - \b SelectionEnd The user has ended a selection.
    * - \b SelectionExtend The user has extended a selection.
    * - \b SelectionAdd The user has added a new selection.
    */
    enum QicsSelectionType {
        SelectionNone = 0,
        SelectionBegin,
        SelectionDrag,
        SelectionEnd,
        SelectionExtend,
        SelectionAdd,
        SelectionReplace
    };

    /*! Specifies whenever ordering should be ascending or descending.
    *
    * - \b Ascending	Ascending order.
    * - \b Descending	Descending order.
    */
    enum QicsSortOrder {
        Ascending = 0,
        Descending
    };

    /*!
    * Specifies a printing display policy for elements of the table.
    * \arg \b DisplayNever Do not display the element when printing.
    * \arg \b DisplayAlways Always display the element when printing.
    * \arg \b DisplayFirstPage Display the element only when printing
    *         the first page of the table.
    */
    enum QicsTableDisplayOption {
        DisplayNever = 0,
        DisplayAlways,
        DisplayFirstPage
    };

    /*!
    * Specifies the behavior of the traversal keys.
    * (left, right, up, down, etc) \b while \b editing a cell.
    * \arg \b TraverseWithinCell Traversal keys move the text cursor
    *          within the edited cell.
    * \arg \b TraverseBetweenCells Traversal keys move the current cell
    *          highlight between cells in the table.
    */
    enum QicsTraversalKeyEditBehavior {
        TraverseWithinCell = 0,
        TraverseBetweenCells
    };

    /*!
    * Specifies the behavior of drag&drop operations when it has began
    * from current cell.
    * \arg \b DragCurrent - only current cell is dragging.
    * \arg \b DragSelection - whole selection is dragging.
    */
    enum QicsCurrentCellDraggingPolicy {
        DragCurrent = 0,
        DragSelection
    };

    /*!
    * Specifies the behavior of set cell font.
    *
    * \arg \b NoChange - does not change size of cells.
    * \arg \b ChangeOnlyHeight - changing only height of the cell when new font is applies (cell width is not changed).
    * \arg \b ChangeHeightWidth - changing height and width.
    */
    enum QicsCellWidthMode {
        NoChange = 0,
        ChangeOnlyHeight,
        ChangeHeightWidth
    };

    /*!
    * Specifies the cell decoration style for headers.
    *
    * \arg \b Flat Header cells are drawn flat, just like as ordinal cells.
    * \arg \b Styled Header cells are drawn with different look.
    */
    enum QicsCellDecorationStyle {
        Flat = 0,
        Styled = 1
    };

    /*!
    * Specifies row/column drag style for headers.
    *
    * \arg \b SimpleDrag Only drag cursor is used to indicate drag process.
    * \arg \b StyledDrag Extended image is used to indicate drag process.
    */
    enum QicsHeaderDragStyle {
        SimpleDrag = 0,
        StyledDrag = 1
    };

    /*!
    * Specifies autofit modes.
    *
    * \arg \b Native It toggles between the user defined and minimal width\height (default).
    * \arg \b Excel It is just setting the minimal width\height.
    */
    enum QicsAutoFitMode {
        Native = 0,
        Excel
    };
};

#endif //QICSNAMESPACE_H
