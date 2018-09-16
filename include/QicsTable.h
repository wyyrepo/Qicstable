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

#ifndef QICSTABLE_H
#define QICSTABLE_H

#include <QVector>
#include <QPixmap>
#include <QPrinter>

#include "QicsTableCommon.h"
#include "QicsRegion.h"
#include "QicsSpan.h"
#include "QicsHeaderGrid.h"
#include "QicsTableGrid.h"
#include "QicsGridGeometry.h"
#include "QicsKeyboardManager.h"
#include "QicsDimensionManager.h"
#include "QicsDataModel.h"
#include "QicsFilter.h"
// These files must be included for every application
// so we'll reduce number of includes - only QicsTable.h would be enought
#include "QicsSelection.h"
#include "QicsRepeatingRow.h"
#include "QicsCell.h"
#include "QicsRow.h"
#include "QicsColumn.h"
#include "QicsCellRegion.h"
#include "QicsColumnHeader.h"
#include "QicsRowHeader.h"
#include "QicsRubberBand.h"


// Forwards to reduce dependencies..
class QicsSelection;
class QicsCellRegion;
class QEvent;
class QAbstractSlider;
class QicsScrollManager;
class QicsScroller;
class QicsStyleManager;
class QicsSelectionManager;
class QicsCellDisplay;
class QicsGridLayout;
class QicsNavigator;
class QicsAbstractAttributeController;
class QicsCellDisplayFactory;
class QicsCellDisplayConfigurationWidget;
class QicsAbstractClipboardDelegate;
class QicsAbstractFilterDelegate;
class QicsAbstractSorterDelegate;

/*!
* \class QicsTable QicsTable.h
* \nosubgrouping
* \brief A full-featured table widget supporting a Model-View-Controller
* architecture

QicsTable is a full-featured, high-performance table widget suited for use
in industrial-strength applications.  It incorporates a model-view-controller
architecture, so that multiple table widgets can display the same data.

Please see \ref arch for an overview of the table's design.

The table consists of a <A HREF="#MainGrid">main grid</A>, two optional row
<A HREF="#Headers">headers</A> (left and right)
and two optional column <A HREF="#Headers">headers</A> (top and bottom).
There are spaces for four optional title widgets, one on each side of the table.
Four "corner widgets" are also supported, one in each corner of the table.
There are optional scrollbars on the right and bottom of the table to support
table scrolling.

The table supports frozen rows and columns, where a programmer-specified
number of rows and/or columns can be "frozen" at the top/bottom/left/right
of the table.  These frozen rows and columns do not move when the table is
scrolled.

The table supports intra-table and inter-table
<A HREF="#DragAndDrop">drag and drop</A>, as well as
<A HREF="#CutAndPaste">cut and paste</A>. Also there are
<A HREF="#WideKeys">global keyboard actions</A> available.

The table has three types of attributes:
- <A HREF="#Cells">Cell Attributes</A> These attributes may be set on a single cell,
a row, a column, or the entire grid.
- <A HREF="#Grids">Grid Attributes</A> These attributes may be set on grids, either
the main grid or one of the headers.
- <A HREF="#Tables">Table Attributes</A> These attributes may be set on the entire table.

Also, table provides high-performance and convenient methods for setting
<A HREF="#Regions">Region Attributes</A> of a region of cells in the grid.

Cell attributes can be also controlled externally using
<A HREF="#ExtAttr">External Attribute Controllers</A>.

Content of the table can be sorted and filtered. Also, it can be grouped
into tree-like structure; see QicsTreeTable class.

The QicsTable widget has extensive <A HREF="#Printing">printing</A>
capabilities, including the ability to print the table with different
visual characteristics than the on-screen table display. A powerful
Print Preview Framework provides easy-to-use
\link print_preview WYSIWYG printing \endlink. Also, the content of
the table can be <A HREF="#Export">exported</A> to the external file.


<H3>Data Model</H3>

QicsTable uses the data model interface that is defined in QicsDataModel.
The programmer should begin by creating a data model by instantiating one
of the subclasses of QicsDataModel, and then filling the data model
with QicsDataItem objects.

\code
QicsDataModel *data_model = new QicsDataModelDefault(100, 100);

for(int row = 0; row < 100; ++row)
{
    for(int col = 0; col < 100; ++col)
    {
        QString str("R%1C%2").arg(row).arg(col);
        data_model->setItem(row, col,  QicsDataString(str));
    }
}
\endcode

QicsTable also supports <A HREF="#QtModel">native Qt models</A> via corresponding
wrappers.

<H3>Creating the Table Widget</H3>

The table widget is created by passing a data model pointer (and other
optional arguments) to the QicsTable constructor.

\code
QicsTable *table = new QicsTable(data_model, this);
\endcode

If the data model argument is 0, no cells will be displayed.

An alternate QicsTable constructor allows you to use customized grids
and/or row/column header data model objects in the table.
\a Foundry \a methods are passed into the table's constructor, which
uses the methods to create the grids and data models.

\code
QicsTable *table = new QicsTable(MyCustomDataModel::create, // row header data model
MyCustomDataModel::create, // column header data model
MyCustomTableGrid::create,
MyCustomHeaderGrid::create,
data_model,
parent);
\endcode

For more information on creating custom data models, see
\ref extend_model.

For more information on creating custom grids, see
\ref keymouse_behavior.

<A NAME="Tables">
<H3>Setting Table Attributes</H3>
</A>

The table widget has many attributes that control its behavior.

The table has a #viewport(), which limits the displayable cells in the
table to a subset of the data in the model.  To specifiy the number of
rows and columns that are displayed on the screen, use #setVisibleRows()
and #setVisibleColumns().  These methods will cause the table to set a
preferred size that will be large enough to display that many items.
To control the part of the data model that is currently displayed, use
#setTopRow() and #setLeftColumn().

You can temporarily stop the QicsTable and all of its grids and headers
from resizing and/or redrawing by calling #setRepaintBehavior().  This
is especially useful if you are making a number of attribute changes at
once.

Header visibility
can be controlled by #setTopHeaderVisible(), #setBottomHeaderVisible(),
#setLeftHeaderVisible(), and #setRightHeaderVisible().

Normally, the table displays row and column indices in the headers.  If you
wish to have the headers display data from the data model, use
#setRowHeaderUsesModel() and #setColumnHeaderUsesModel().

The table has reserved areas that may be used to display arbitrary widgets.
There are four title areas, one each at the top, bottom, left, and right
edges of the table widget.  Widgets can be placed in these areas by calling
#setTopTitleWidget(), #setBottomTitleWidget(), #setLeftTitleWidget(), and
#setRightTitleWidget().  In addition, there are four corner areas, at each
corner of the table.  Widgets can be placed in these areas by calling
#setTopLeftCornerWidget(), #setTopRightCornerWidget(),
#setBottomLeftCornerWidget(), and #setBottomRightCornerWidget().

QicsTable supports frozen rows and columns at all four edges of the table.
These frozen rows and columns do not move when the table is scrolled.
To freeze rows, first ensure that the rows you want to freeze are at the
top (or bottom, if you want to freeze them at the bottom) of the table using
#moveRows(), then, call #freezeTopRows() or #freezeBottomRows().
To freeze columns, first ensure that the
columns you want to freeze are at the left (or right, if you want to freeze
them at the right) edge of the table using moveColumns(), then, call
#freezeLeftColumns(), or freezeRightColumns().

The table has optional scrollbars on the right (row scrolling) and the bottom
(column scrolling) of the table.  These scrollbars can be configured to
always be shown, never be shown, or shown only when needed.
See #hScrollBarMode() and #vScrollBarMode() for details.

<A NAME="Grids">
<H3>Grid Attributes</H3>
</A>

There are a number of attributes that can be set on the main grid of the table,
as well as the row and column header grids.  It is important to remember that header
grids and table grids are both derived from a common object, so they share many
methods and attributes.

Some of the attributes that can be set on a grid are:
- \link QicsGridCommon::frameLineWidth frame width \endlink
- \link QicsGridCommon::frameStyle frame style \endlink
- \link QicsGridCommon::horizontalGridLineWidth horizontal \endlink and \link QicsGridCommon::verticalGridLineWidth vertical \endlink grid line width
- \link QicsGridCommon::horizontalGridLinePen horizontal \endlink and \link QicsGridCommon::verticalGridLinePen vertical \endlink grid line pen
- \link QicsGridCommon::horizontalGridLineStyle horizontal \endlink and \link QicsGridCommon::verticalGridLineStyle vertical \endlink grid line style
- \link QicsGridCommon::gridCellClipping grid cell clipping \endlink
- \link QicsGridCommon::drawPartialCells \endlink partial cell drawing policy
- \link QicsGridCommon::currentCellBorderWidth current cell border width \endlink
- editing policies

All grid types support cell spanning.  Cell spanning is a way of specifying a
region of the grid that should appear as one cell.  The contents of the first
(top and left) cell in the span is displayed in the entire spanned region.
See #addCellSpan() for details.

All grid types also support cell overflow.  Cell overflow is a way of allowing
cells that cannot fully display their contents in the cell's normal area.
By default, the text is simply clipped and a symbol is displayed indicating
that the text was clipped (see #moreTextPixmap()).
Using #setCellOverflowBehavior(), cells may overflow into adjacent cells if those
cells are empty.  A maximum number of overflowed cells can be set using
setMaxOverflowCells().
Also, using #setCellOverflowBehavior(), cells can display a tooltip containing
the full contents of the cell when the mouse pointer is hovered over the cell.

<A NAME="Cells">
<H3>Cell Attributes</H3>
</A>
Cell attributes are retrieved and set by using the QicsCell class.
This is a flyweight object that represents a single cell in the table.
There are two different ways that a programmer can use a QicsCell object.
If the programmer's intent is simply to call one or two cell methods,
it is usually best to use the #cellRef() method of QicsTable.  This method
returns a reference to a QicsCell that cannot be assigned to a variable.
Rather, the programmer should call the method directly from the reference.

\code
table->cellRef(5,4).setMargin(2);
\endcode

This way of using QicsCell has the advantage of being very fast and convenient.

If the intent is to use the cell object many times, or if you intend to
connect a signal to one of QicsCell's slots, you should create a new QicsCell
object.  This object is persistent, and will continue to be valid until it
is destroyed or the table itself is destroyed.

\code
// you can create a cell object this way...
QicsCell *cell = table->cell(5,4);

// ...or, you can do it this way
QicsCell *cell = new QicsCell(5,4, table);
\endcode

When a new, persistent QicsCell object is created, the programmer must specify
whether the cell object should always refer to the same visual location in the
table (even if the data model has been modified or the table has been sorted),
or whether the cell should always refer to the model cell that is currently
in the location that the cell was created with.  This is done by specifying
the \a follow_model flag when creating the cell.  The default for this flag is
\b true, which means that the cell object will always follow the model.

Any attribute settings that are made while using a cell object that follows
the model are considered to be <i>model attribute settings</i>.  These
settings will always pertain to this model cell, even if rows and columns
are reordered.  Attribute settings that are made while using a cell object
that does not follow the model are considered to be
<i>visual attribute settings</i>.  These settings will always pertain to
the visual cell, regardless of what model cell is being displayed in that
cell.

Data from the data model is displayed in table cells.  Generally, one value in
the model is displayed in a single cell in the table, although cell spanning
and cell overflowing can change this.  Each cell has associated with it a cell
display object.  A cell display object, whose interface is defined in the
QicsCellDisplay class, handles all aspects of the display and editing of a cell.
The default cell display object used in the table is QicsTextCellDisplay.  This
object displays text and/or pixmaps in cells.  Other cell display objects
currently available are QicsCheckCellDisplay (check boxes), QicsComboCellDisplay
(combo boxes), and QicsWidgetCellDisplay (any widget).  Cell display objects
can be set on a cell, row, column, or grid-wide basis.

Some of the attributes that can be set on a cell are:
- \link QicsCellCommon::font font \endlink
- \link QicsCellCommon::foregroundColor foreground \endlink and \link QicsCellCommon::backgroundColor background \endlink colors
- \link QicsCellCommon::label label \endlink
- \link QicsCellCommon::pixmap pixmap \endlink
- \link QicsCellCommon::alignment alignment \endlink
- \link QicsCellCommon::textFlags text flags \endlink
- \link QicsCellCommon::topBorderPen top border pen \endlink
- \link QicsCellCommon::leftBorderPen left border pen \endlink
- \link QicsCellCommon::rightBorderPen right border pen \endlink
- \link QicsCellCommon::bottomBorderPen bottom border pen \endlink
- \link QicsCellCommon::margin margin size \endlink
- \link QicsCellCommon::displayer cell displayer \endlink
- editing - validators, maxLength

<A NAME="Regions">
<H3>Region Attributes</H3>
</A>

Region attributes can be set by using the QicsCellRegion class.
These is flyweight object that represents a rectangular region of cells in the table and headers.
Its methods are optimized for high performance and convenience.

An older style to set, say, background color of cells in region (4,10)-(100,200) was to
iterate over all the cells and explicitly call QicsCell::setBackgroundColor() every time,
similar like:

\code
table->setRepaintBehavior(Qics::RepaintOff);

QicsRegion r(4,10,100,200);
QColor color(Qt::yellow);

for(int i = r.startColumn(); i <= r.endColumn();i++)
    for(int j = r.startRow(); j <= r.endRow();j++)
        table->cellRef(j,i).setBackgroundColor(color);

table->setRepaintBehavior(Qics::RepaintOn);
\endcode

A new approach is to obtain QicsCellRegion object from the table
and use its QicsCellRegion::setBackgroundColor() method:

\code
table->cellRegionRef(4,10,100,200).setBackgroundColor(Qt::yellow);
\endcode

It is simpler, shorter and much more faster than iteration in previous example.
Also, you do not need to control repaint behavior anymore as region methods do this
automatically.

Another example is drawing of a visual box around a region of cells, like this
done in popular spreadsheets. Older style forces you to obtain four regions from a
desired one - top, bottom, left and right - and perform iterations on each of them
to setup appropriate border pen. Now, QicsCellRegion introduces QicsCellRegion::setBoxPen()
which allows to automatically performs all the necessary calculations and calls.

\code
table->cellRegionRef(4,10,100,200).setBoxPen(QPen(Qt::black), QicsBoxBorders::All);
\endcode

Here, all the four borders around the region will be set with given pen in one call.


<A NAME="Rows">
<H3>Rows and Columns</H3>
</A>

Row and column attributes are retrieved and set by using the QicsRow
and QicsColumn classes.  These are flyweight objects that represent a single
row or column in the table.
There are two different ways that a programmer can use a QicsRow
or QicsColumn object.
If the programmer's intent is simply to call one or two row/column methods,
it is usually best to use the #rowRef() and #columnRef() methods
of QicsTable.  These methods return a reference to a QicsRow or
QicsColumn object that cannot be assigned to a variable.
Rather, the programmer should call the method directly from the reference.

\code
table->rowRef(5).setMargin(2);
\endcode

This way of using QicsRow and QicsColumn has the advantage of being very fast
and convenient.

If the intent is to use the row or column object many times, or if you intend to
connect a signal to one of the object's slots, you should create a new QicsRow
or QicsColumn object.  This object is persistent, and will continue to be valid
until it is destroyed or the table itself is destroyed.

\code
// you can create a column object this way...
QicsColumn *col = table->column(4);

// ...or, you can do it this way
QicsColumn *col = new QicsColumn(4, table);
\endcode

When a new, persistent QicsRow or QicsColumn object is created, the programmer
must specify whether the object should always refer to the same visual location
in the table (even if the data model has been modified or the table has been sorted),
or whether the cell should always refer to the model row or column that is currently
in the location that the object was created with.  This is done by specifying
the \a follow_model flag when creating the row or column.  The default for this flag
is \b true, which means that the row or column object will always follow the model.

Any attribute settings that are made while using a row or column object
that follows the model are considered to be <i>model attribute settings</i>.
These settings will always pertain to this model row or column,
even if rows and columns are reordered.  Attribute settings that are made
while using a row or column object that does not follow the model are
considered to be <i>visual attribute settings</i>.  These settings will
always pertain to the visual row or column, regardless of the current row and
column ordering of the table.

It is important to note that any attribute that can be set on an individual cell
can also be set on a row or a column.  Setting the attribute on an entire row
or column will override any previous settings to all cells in the row or column.

Row heights and column widths can be set in pixel dimensions or in character/font
dimensions.  See QicsRow and QicsColumn for details.

Because the table is a really a view of a data model, rows and columns can be
moved, hidden, and sorted in the table without affecting the underlying data.
See QicsRow and QicsColumn for details.

By default, when the table grows or shrinks in size, the table adjusts the
number of visible rows and/or columns to fit the new size.  If you
would rather have the number of visible rows and columns remain fixed and
instead adjust the size of some or all of the rows and columns to fit the
new table size, you can set these rows and columns to be stretchable.
When the table's width changes, the widths of any stretchable columns that
are currently visible in the table are modified to account for the change
in width.  All stretchable and visible columns have the same "priority", so
the extra (or lost) width is spread evenly among the stretchable columns.
(This is also true of a change in height affecting all stretchable and
visible rows.)  See QicsRow and QicsColumn for more details.

<A NAME="Repeating">
<H3>Repeating Rows and Columns</H3>
</A>
QicsRepeatingRow and QicsRepeatingColumn are specialized flyweight objects
that represent a repeating sequence of row and column settings.  These
objects are very similar to QicsRow and QicsColumn, except that they
have a starting index and an interval, which specifies how often the
settings should repeat.  Attribute settings using repeating row and column
objects are always considered <i>visual attribute settings</i>.
These settings will always pertain to visual rows or columns, regardless
of the current row and column ordering of the table.

There are two different ways that a programmer can use a QicsRepeatingRow
or QicsRepeatingColumn object.
If the programmer's intent is simply to call one or two row/column methods,
it is usually best to use the #repeatingRowRef() and
#repeatingColumnRef() methods of QicsTable.  These methods return a
reference to a QicsRepeatingRow or QicsRepeatingColumn object that cannot
be assigned to a variable.  Rather, the programmer should call the method
directly from the reference.

\code
// Sets every other row's background color to yellow.
table->repeatingRowRef(0, 2).setBackgroundColor(Qt::yellow);
\endcode

This way of using QicsRepeatingRow and QicsRepeatingColumn has the advantage
of being very fast and convenient.

If the intent is to use the repeating row or column object many times,
or if you intend to connect a signal to one of the object's slots,
you should create a new QicsRepeatingRow or QicsRepeatingColumn object.
This object is persistent, and will continue to be valid until it is
destroyed or the table itself is destroyed.

\code
// you can create a repeating column object this way...
QicsRepeatingColumn *col = table->repeatingColumn(0, 5);

// ...or, you can do it this way
QicsRepeatingColumn *col = new QicsRepeatingColumn(0, 5, table);
\endcode

<A NAME="MainGrid">
<H3>Main Grid</H3>
</A>

Attributes from the main grid are retrieved and set by using the
QicsMainGrid class.  This is a flyweight object that represents the
main grid of the table.  There are two different
ways that a programmer can use the QicsMainGrid object.
If the programmer's intent is simply to call one or two grid methods,
it is usually best to use the #mainGridRef() method of
QicsTable.  This method returns a reference to a QicsMainGrid object that
cannot be assigned to a variable.  Rather, the programmer should call the
method directly from the reference.

\code
table->mainGridRef().setMargin(2);
\endcode

This way of using QicsMainGrid has the advantage of
being very fast and convenient.

If the intent is to use the grid object many times, or if you intend to
connect a signal to one of the object's slots, you should create a new
QicsMainGrid object.  This object is persistent, and will continue to be valid
until it is destroyed or the table itself is destroyed.

\code
// you can create a main grid object object this way...
QicsMainGrid *mainGrid = table->mainGrid();

// ...or, you can do it this way
QicsMainGrid *mainGrid = new QicsMainGrid(table);
\endcode

It is important to note that any attribute that can be set on a table cell,
row, or column can also be set on the entire main grid.

<A NAME="Headers">
<H3>Headers</H3>
</A>

Rows and column header attributes are retrieved and set by using the
QicsRowHeader and QicsColumnHeader classes.  These are flyweight objects
that represent row and column headers in the table.  There are two different
ways that a programmer can use a QicsRowHeader or QicsColumnHeader object.
If the programmer's intent is simply to call one or two header methods,
it is usually best to use the #rowHeaderRef() and #columnHeaderRef() methods of
QicsTable.  These methods return a reference to a QicsRowHeader or
QicsColumnHeader object that cannot be assigned to a variable.
Rather, the programmer should call the method directly from the
reference.

\code
table->rowHeaderRef().setMargin(2);
\endcode

This way of using QicsRowHeader and QicsColumnHeader has the advantage of
being very fast and convenient.

If the intent is to use the header object many times, or if you intend to
connect a signal to one of the object's slots, you should create a new QicsRowHeader
or QicsColumnHeader object.  This object is persistent, and will continue to be valid
until it is destroyed or the table itself is destroyed.

\code
// you can create a column header object this way...
QicsColumnHeader *colHdr = table->columnHeader();

// ...or, you can do it this way
QicsColumnHeader *colHdr = new QicsColumnHeader(table);
\endcode

It is important to note that any attribute that can be set on a table cell,
row, or column can also be set on a header cell, row, or column.
Also, any attribute that can be set on the main table grid can also be set on
a header.

By default, QicsTable displays row or column numbers in the cells of
row and column headers.  These numbers are generated "on-the-fly" and
do not cause any memory to be allocated.

If you want to change what
is displayed in the header cells, you may do one of the following:

- Set the \link QicsCellCommon::label "label"\endlink property on one
or more cells in the header.
\code table->rowHeaderRef().cellRef(4,0).setLabel(QString("New Label"));\endcode
- Change the value of a cell in the header's data model.  (Each header
has it's own data model object that is automatically created by the
table widget.  The models are initially empty, but can be modified.)
You can change the value by doing the following:
\code table->rowHeaderRef().cellRef(4,0).setDataValue(&newvalue);\endcode
- Create a custom data model object for the headers.  Using your own
data model, you can completely control the data that is displayed in
the headers.  Create your QicsTable widget with the alternate constructor
(QicsTable::QicsTable), passing your custom
data model's foundry method as a parameter.  See \ref
extend_model for details on creating a custom data model.

<H3>Traversal and Cell Editing</H3>

QicsTable allows the programmer to have full control over cell traversal.
The table has a current cell, which is the cell to which any keyboard input
will be directed.  This cell is drawn with a special border
(see #currentCellBorderWidth and #setCurrentCellBorderWidth).  Traversing
to a cell causes the cell to become the current cell.  Cells that are
not #enabled cannot be traversed to.

The following methods control cell traversal:

- #traverseToCell()
- #traverseToBeginningOfTable()
- #traverseToEndOfTable()
- #traverseToBeginningOfRow()
- #traverseToEndOfRow()
- #traverseToBeginningOfColumn()
- #traverseToEndOfColumn()
- #traverseLeft()
- #traverseRight()
- #traverseUp()
- #traverseDown()
- #traversePageUp()
- #traversePageDown()

Once the user has traversed to a cell, s/he may edit the cell
if the cell property #readOnly is \b false.

If the #clickToEdit property is \b false, simply typing a character
will begin the editing process.  If #clickToEdit is \b true, the user
must click in the cell with the mouse before the cell can be edited.

The #autoSelectCellContents property controls the initial selection of
the contents of the current cell when editing is initiated.  If the
property is \b true, the entire contents of the cell will be selected
(so that any typing will cause the old contents to be replaced).  If
the property is \b false, the contents will not be selected.

The #traversalKeyEditBehavior property controls the behavior of the
traversal keys while the user is editing a cell.  If the property
is \b TraverseWithinCell, traversal keys will move the text cursor
within the edited cell.  If the property is \b TraverseBetweenCells,
traversal keys move the current cell highlight between cells in the
table.

To control the direction of traversal when the user presses \b Enter,
use the #enterTraversalDirection property.  To control the direction
of traversal when the user presses \b Tab, use the #enterTraversalDirection
property.

<A NAME="Selections">
<H3>Selections</H3>

QicsTable supports several different selection policies.  See
Qics::QicsSelectionPolicy for details on the different choices.
The default mouse/key bindings for selection are:
- \b LeftButton \b click - select a single cell (or row/column,
if clicking in a header)
- \b LeftButton \b drag - select a range of cells (or rows/columns,
if clicking in a header)
- \b Shift \b LeftButton \b click - extend the last selection to
to the clicked-on cell (or row/column, if clicking in a header)
- \b Ctrl \b LeftButton \b click - add or remove a cell (or row/column,
if clicking in a header) selection.
- \b Ctrl \b LeftButton \b drag - add or remove a cell (or row/column,
if clicking in a header) range selection.

A copy of the current selection list can be obtained using #selectionList().
The selection list may contain more than one QicsSelection object if the
selection policy allows discontiguous selections.  If some cells in a
selection are unselected by the user, the original selection will
split into as many as four new selections to describe the new selection state.

A copy of the current selection action list can be obtained using
#selectionActionList().  The selection action list logs the exact actions that
were taken by the user to get to the current selection state.  Cells that
are unselected appear in the selection action list as selection objects
whose \link QicsSelection::selected selected()\endlink methods return \b false.

The programmer can set a new selection list by calling #setSelectionList().
If the selection policy allows multiple, discontiguous selections, the
programmmer may add a selection to the selection list using #addSelection().
Selection objects that represent unselections are allowed in the list.

To modify the selection list, retrieve a copy of it using #selectionList(),
then add, remove, or modify individual QicsSelection objects in the list.
When the new list is complete, call #setSelectionList() to set it on the table.
Selection objects that represent unselections are allowed in the list.

To clear the entire selection list, call #clearSelectionList().

<A NAME="DragAndDrop">
<H3>Drag and Drop</H3>
</A>
QicsTable supports intra-table drag and drop, inter-table drag and drop,
and has limited support for dragging data from the table to other non-table
drop sites.  Drags between tables will move or copy data items in their original
formats to the receiving cells.  Dragging cells to non-QicsTable drop sites
will cause only the text representations of the cells to be moved or copied.

Drag operations are performed using the middle mouse button.  Clicking in a cell
with the middle mouse button and then moving the mouse pointer while
holding the button down initiates a drag.  If the Control modifier key
is held down when the drag is started, the operation is considered a
"move".  Otherwise, the operation is considered a "copy".
If the starting drag cell is selected, the entire selection will be moved or
copied to the drop location.  If the starting drag cell is not selected,
only the cell will be dragged.

Drag operations complete when the user releases the middle mouse button.
Data items that were dragged are moved or copied are placed into the table,
beginning at the cell containing the mouse pointer.

<A NAME="CutAndPaste">
<H3>Cut and Paste</H3>
</A>

QicsTable supports cut and paste through the Qt Clipboard.  Call #cut() or #copy()
to place the contents of the cells in the #selectionList on the clipboard.
Call #paste() to place the contents of the clipboard into the table.  If the
contents of the clipboard is data from a QicsTable, the items will be placed
into the table beginning at the specified cell.  If the contents of the clipboard
is a string, that value will be placed into the table at the specified cell.
No other Mime formats are supported at the present time.

Since 2.3, QicsTable supports reimplementation of standard clipboard operations via
QicsAbstractClipboardDelegate. You can create your own delegate
which will handle needed operations and install it with setClipboardDelegate() method.

<A NAME="Filtering">
<H3>Filtering</H3>
</A>

Since 2.4, QicsTable introduces a new approach for filtering its contents.
A QicsAbstractFilterDelegate subclass which implements desired filtering
functionality can be installed for a column. Once installed, it will hide all the rows
which data do not match filter pattern.

Via setRowFilter() method, a filter delegate can be set to a column, and removed via
removeRowFilter(). All filters can be removed via removeAllRowFilters().
To obtain a filter installed for a particular column, use rowFilter() method. hasRowFilter()
method allows to know if any filter was installed for a column.
isRowFiltered() is helper method which retrieves current state of a particular row.

Currently two basic filter delegates available: QicsListFilterDelegate which allows
to specify a QStringList of valid items and QicsRegexpFilterDelegate which uses
regular expressions based on QRegExp.

Installation of a filter delegate is pretty easy.
First, you should create a filter delegate.
Second, set up the filtering pattern for the delegate.
And third, install the delegate for a column you wish to filter rows by.

There is an example how to use QicsListFilterDelegate.

\code
QStringList filters;
filters << "John" << "Mary" << "Dan";	// Fill QStringList with items which should be visible.
// The rest will be filtered off.

table->setRowFilter(2, new QicsListFilterDelegate(filters));	// Create and install the delegate to column 2 of the table.
\endcode

After executing this code, all the rows which data in column 2 do not match the given pattern
(i.e. not "John", "Mary" or "Dan") will be hidden. So the table

<table>
<tr><td><b>N row</b></td><td><b>Column 1</b></td><td><b>Column 2</b></td>
<tr><td>1</td><td>1</td><td>John</td>
<tr><td>2</td><td>2</td><td>Jack</td>
<tr><td>3</td><td>3</td><td>Gabrielle</td>
<tr><td>4</td><td>4</td><td>Mary</td>
<tr><td>5</td><td>5</td><td>Warwick</td>
<tr><td>6</td><td>6</td><td>Dan</td>
</table>

after setting the filter would result in

<table>
<tr><td><b>N row</b></td><td><b>Column 1</b></td><td><b>Column 2</b></td>
<tr><td>1</td><td>1</td><td>John</td>
<tr><td>4</td><td>4</td><td>Mary</td>
<tr><td>6</td><td>6</td><td>Dan</td>
</table>


<A NAME="Grouping">
<H3>Grouping</H3>
</A>

Table's content can be organized into tree-like structure by representing a data
from grouped columns as tree nodes, and rest of rows as leafs. This is provided by
QicsTreeTable class, a subclass of %QicsTable which uses different data representation.

To work with a table like with a tree, you should use QicsTreeTable as the base class,
not %QicsTable.

<A NAME="Printing">
<H3>Printing</H3>
</A>

QicsTable supports high quality printed output.  Programmers can use #print() to
print the entire table or only a region of the table.  The printed version of
the table will use the same attribute settings as the on-screen table.  If you
wish to change some of the settings before printing, create a QicsTablePrint
object.  The QicsTablePrint object is basically a copy of the current state
of the attribute settings in the table object that was passed to the
constructor.   After creation, this table print object will contain the exact
same attribute settings as the original QicsTable object.  You can now make
any attribute changes on the new QicsTablePrint object -- these changes will
be refelcted only in the printed output, not the on-screen table.  After
the desired changes are made, you can print the new object by calling
QicsTablePrint::print().

A powerful \link print_preview Print Preview Framework \endlink provides easy-to-use
WYSIWYG printing.  To make printing with preview easy, \a addons folder contains helper class
QicsPrintPreviewDialog. It provides static method \b previewAndPrint(QicsTable *table)
which invokes preview of the \a table.

See QicsTablePrint::setPreviewWidget() and
QicsTablePrint::setPreviewRegion(), and \link stocks2_demo Stocks2 \endlink example.

<A NAME="Export">
<H3>Export</H3>
</A>

QicsTable supports export of its content to several popular data formats.

Table content can be exported as the \b hypertext \b (html) file.
QicsHTMLExport class provides powerful capabilities to export not only textual
content, but also cell coloring, fonts, graphics and so on. Export is fully
customizable, so you can specify which attributes to export and which not.
QicsHTMLExport::exportToFile() method exports current table content to HTML file.

To make the customization easy, \a addons folder contains helper class
\b QicsHTMLExportDialog. It provides static method \b exportTable(QicsTable *table, const QString &name)
which invokes export of the \a table to a file with \a name.

Also, model data can be exported/imported to/from \b comma \b separated \b (CSV) text
file. See methods QicsDataModel::readASCII() and QicsDataModel::writeASCII().

<A NAME="ExtAttr">
<H3>External Attribute Controllers</H3>
</A>

QicsTable allows to control cell attributes from outside, using special
External Attribute Controllers. A programmer can implement his own attribute controller
which will be responsible for setting, keeping and retrieving certain attributes of
cells, rows and columns.

Class QicsAbstractAttributeController provides basic interface to implement
own external controller. Currently there is QicsRegionalAttributeController available.
It is very fast and consumes as few memory as possible.

To install own controller, use QicsTable::setExternalAttributeController() method.
For example, installing QicsRegionalAttributeController would be

\code
QicsRegionalAttributeController control;
table->setExternalAttributeController(control);
\endcode

To remove contoller previously installed, use QicsTable::removeExternalAttributeController().

<A NAME="WideKeys">
<H3>Global Key Actions</H3>
</A>

QicsTable introduces global (table-wide) keyboard actions used to perform most common
tasks with the grid (such as select all cells by pressing Ctrl-A, or delete data from
selected cells by pressing Delete).

Available actions are defined as Qics::QicsWideKeyAction enumeration
and could be enabled/disabled using setWideKeyAction() and setWideKeyActions() methods.

By default, there are no global actions set. To enable all the defined actions, just use

\code
table->setWideKeyActions();
\endcode

<A NAME="QtModel">
<H3>Support of Qt models</H3>
</A>

Native Qt models are supported by QicsTable via QicsDataModelQtModelAdapter and
QicsQtModelAttributeController. These classes used for retrieve data and attributes from
Qt item model.

To use data from Qt model, use QicsDataModelQtModelAdapter class:

\code
// create Qt table data model
QAbstractItemModel *qtmodel = new QAbstractTableModel();

// create an adapter to QicsDataModel
QicsDataModelQtModelAdapter *model = new QicsDataModelQtModelAdapter();
// set qtmodel for the adapter
model->setModel(qtmodel);

// create the table
QicsTable *table = new QicsTable();
// set adapted model for the table
table->setDataModel(model);
\endcode

To handle attributes stored in Qt model, install QicsQtModelAttributeController.
QicsDataModelQtModelAdapter should be used as the model in order to make the controller work.

\code
QicsQtModelAttributeController controller;
table->setExternalAttributeController(controller);
\endcode

<A NAME="KDChart">
<H3>Support of KDChart charts</H3>
</A>

Since 2.4, QicsTable allows to integrate with KDChart 2.x via kdchart addon.
This addon makes easy to bind KDChart charts with QicsTable tabular data,
providing on-the-fly data synchronization between table and chart.

It consists of two classes, QicsKDChartDataModelAdapter and QicsKDChartTableObject.
QicsKDChartDataModelAdapter is the wrapper of QicsDataModel which adapts it to
the KDChart datamodel, and QicsKDChartTableObject is the special displayer which
allows to place charts into the table's field.

The common way to use KDChart is:

\code
//create the table
QicsTable *table = new QicsTable();

// create KDChart bar diagram
KDChart::BarDiagram *bars = new KDChart::BarDiagram;

// install special displayer
QicsKDChartTableObject *kdview = new QicsKDChartTableObject(
table,					// parent table
bars,					// diagram to integrate with
QicsRegion(13,1,23,6),	// cell region where to put the chart itself
QicsRegion(1,1,10,6)	// cell region where to read data from
);
\endcode

After this, a KDChart::BarDiagram will be inserted into QicsTable's region
(13,1,23,6), taking data from cells in the region (1,1,10,6).

There are two demos, advanced_spreadsheet and kdchart, which show how to use
KDChart within QicsTable.

<B>Please note:</B> in order to use kdchart addon, you should build QicsTable library
with KDChart support; see INSTALL file for details.

<B>Another note:</B> please make sure that you added all the files from
<QICSTABLE>/addons/kdchart to your project file (.h and .cpp). This is because
QicsTable library does not integrate this addon. It is about to be changed in
the future releases.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsTable:  public QFrame, public Qics
{
    Q_OBJECT
public:
    friend class QicsCell;
    friend class QicsRow;
    friend class QicsColumn;
    friend class QicsRepeatingRow;
    friend class QicsRepeatingColumn;
    friend class QicsMainGrid;
    friend class QicsRowHeader;
    friend class QicsColumnHeader;
#ifndef QICSTABLE_GPL
    friend class QicsTablePrint;
#endif

    /** @name Table Properties
    */

    //@{
    /*!
    * Specifies the topmost visible row in the table.
    * The default value of this property is \b 0.
    */
    Q_PROPERTY( int topRow READ topRow WRITE setTopRow )

    /*!
    * Returns/sets default column width in character units.
    */
    Q_PROPERTY( int defaultColumnWidthInChars READ defaultColumnWidthInChars WRITE setDefaultColumnWidthInChars )

    /*!
    * Returns/sets default column width in pixels.
    */
    Q_PROPERTY( int defaultColumnWidthInPixels READ defaultColumnWidthInPixels WRITE setDefaultColumnWidthInPixels )

    /*!
    * Returns/sets default row height in chars.
    */
    Q_PROPERTY( int defaultRowHeightInChars READ defaultRowHeightInChars WRITE setDefaultRowHeightInChars )

    /*!
    * Returns/sets default row height in pixels.
    */
    Q_PROPERTY( int defaultRowHeightInPixels READ defaultRowHeightInPixels WRITE setDefaultRowHeightInPixels )

    /*!
    * Specifies the bottommost visible row in the table.  This
    * value cannot be set directly by the programmer (use #setVisibleRows()
    * and #setViewport()).
    * The default value of this property is \b QicsLAST_ROW.
    */
    Q_PROPERTY( int bottomRow READ bottomRow )

    /*!
    * Specifies the leftmost visible column in the table.
    * The default value of this property is \b 0.
    */
    Q_PROPERTY( int leftColumn READ leftColumn WRITE setLeftColumn )

    /*!
    * Specifies the rightmost visible column in the table.  This
    * value cannot be set directly by the programmer (use #setVisibleColumns()
    * and #setViewport()).
    * The default value of this property is \b QicsLAST_COLUMN.
    */
    Q_PROPERTY( int rightColumn READ rightColumn )

    /*!
    * Specifies the current number of visible rows in the table.
    * This value does not include any frozen rows that may be visible.
    * The default value of this property is \b 10.
    */
    Q_PROPERTY( int visibleRows READ visibleRows WRITE setVisibleRows )

    /*!
    * Specifies the current number of visible columns in the table.
    * This value does not include any frozen columns that may be visible.
    * The default value of this property is \b 10.
    */
    Q_PROPERTY( int visibleColumns READ visibleColumns WRITE setVisibleColumns )

    /*!
    * Specifies whether the top header is visible.  The default value
    * of this property is \b true.
    */
    Q_PROPERTY( bool topHeaderVisible READ topHeaderVisible WRITE setTopHeaderVisible )
    /*!
    * Specifies whether the bottom header is visible. The default value
    * of this property is \b false.
    */
    Q_PROPERTY( bool bottomHeaderVisible READ bottomHeaderVisible WRITE setBottomHeaderVisible )
    /*!
    * Specifies whether the left header is visible. The default value
    * of this property is \b true.
    */
    Q_PROPERTY( bool leftHeaderVisible READ leftHeaderVisible WRITE setLeftHeaderVisible )
    /*!
    * Specifies whether the right header is visible. The default value
    * of this property is \b false.
    */
    Q_PROPERTY( bool rightHeaderVisible READ rightHeaderVisible WRITE setRightHeaderVisible )

    /*!
    * Specifies the number of pixels between the edge of the table widget and
    * its subwidgets (title widgets, headers, etc).
    * The default value of this property is \b 10.
    */
    Q_PROPERTY( int tableMargin READ tableMargin WRITE setTableMargin )

    /*!
    * Specifies the number of pixels between each of the table widget's
    * elements (title widgets, corner widgets, etc).  This property does
    * not control the spacing between grids in the table.  Use
    * \link #gridSpacing "gridSpacing"\endlink in that case.
    * The default value of this property is \b 10.
    */
    Q_PROPERTY( int tableSpacing READ tableSpacing WRITE setTableSpacing )

    /*!
    * Specifies the number of pixels between each of the grids in the
    * table widget.  This property does not control the spacing between
    * other element in the table.  Use
    * \link #tableSpacing "tableSpacing"\endlink in that case.
    * The default value of this property is \b 0.
    */
    Q_PROPERTY( int gridSpacing READ gridSpacing WRITE setGridSpacing )

    /*!
    * Specifies the selection policy for the table.
    *
    * The default value of this property is \b QicsSelectMultiple.
    */
    Q_PROPERTY( QicsSelectionPolicy  selectionPolicy  READ selectionPolicy WRITE setSelectionPolicy )

    /*!
    * Specifies the policy of changing active cell during selection for the table.
    *
    * The default value of this property is \b Follow.
    */
    Q_PROPERTY( QicsSelectCurrentCellPolicy  selectCurrentCellPolicy  READ selectCurrentCellPolicy WRITE setSelectCurrentCellPolicy )

    /*!
    * Specifies whether the values in the first row of the table's
    * associated QicsDataModel should be interpreted as column header data.  If this
    * value is \a true, the value of cell(\a 0, \a colnum) will be shown as the column
    * header for column \a colnum, and row 0 will never be shown as data in the table.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool columnHeaderUsesModel READ columnHeaderUsesModel WRITE setColumnHeaderUsesModel )

    /*!
    * Specifies whether the values in the first column of the table's
    * associated QicsDataModel should be interpreted as row header data.  If this
    * value is \a true, the value of cell(\a rownum, \a 0) will be shown as the row
    * header for row \a rownum, and column 0 will never be shown as data in the table.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool rowHeaderUsesModel READ rowHeaderUsesModel WRITE setRowHeaderUsesModel )

    /*!
    * Specifies the drawing and dimension calculation behavior
    * of the table widget.
    *
    * The default value of this property is \b RepaintOn.
    */
    Q_PROPERTY( QicsRepaintBehavior repaintBehavior READ repaintBehavior WRITE setRepaintBehavior )

    /*!
    * Specifies the mode for the table's horizontal scrollbar.
    *
    * The default value of this property is \b Auto.
    */
    Q_PROPERTY( QicsScrollBarMode hScrollBarMode READ hScrollBarMode WRITE setHScrollBarMode )
    /*!
    * Specifies the mode for the table's vertical scrollbar.
    *
    * The default value of this property is \b Auto.
    */
    Q_PROPERTY( QicsScrollBarMode vScrollBarMode READ vScrollBarMode WRITE setVScrollBarMode )

    //@}

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
    * The default value of this property is
    * <b> (QFrame::StyledPanel | QFrame::Sunken </b> for table grids, and
    * <b> (QFrame::NoFrame | QFrame::Plain) </b> for header grids.
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
    * Specifies the width of the grid's horizontal grid lines.
    *
    * The default value of this property is \b 1.
    */
    Q_PROPERTY( int horizontalGridLineWidth READ horizontalGridLineWidth WRITE setHorizontalGridLineWidth )

    /*!
    * Specifies the width of the grid's vertical grid lines.
    *
    * The default value of this property is \b 1.
    */
    Q_PROPERTY( int verticalGridLineWidth READ verticalGridLineWidth WRITE setVerticalGridLineWidth )

    /*!
    * Specifies the style of the horizontal grid lines in the grid.
    * See QicsLineStyle for details on the available styles.
    *
    * The default value of this property is \b Plain.
    */
    Q_PROPERTY( QicsLineStyle horizontalGridLineStyle  READ horizontalGridLineStyle WRITE setHorizontalGridLineStyle )

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
    *
    * The default value of this property is \b true.
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
    * Specifies the selection style of the table.  See
    * \link Qics::QicsSelectionStyle QicsSelectionStyle\endlink
    * for details.
    *
    * The default value of this property is \b Normal.
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
    * Specifies traversal cell behavior (for Tab key) when it achieves the end of row/column.
    * If \a true it jumps to the first cell of next row/column, else does nothing.
    */
    Q_PROPERTY( bool tabTraverseToBegin READ tabTraverseToBegin WRITE setTabTraverseToBegin )

    /*!
    * Specifies traversal cell behavior (for Enter key) when it achieves the end of row/column.
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
    * Specifies property set the layout direction for a grid
    *
    * The default value of this property is \b Qt::LeftToRight
    */
    Q_PROPERTY( Qt::LayoutDirection layoutDirection READ layoutDirection WRITE setLayoutDirection )

    /*!
    * This property holds whether the fill handle is visible.
    */
    Q_PROPERTY( bool fillHandleVisible READ fillHandleVisible WRITE setFillHandleVisible )

    //@}

    /** @name Common Cell Properties
    */

    //@{
    /*!
    * Specifies the label of the cell(s).  This string may be used
    * by the cell displayer when rendering the contents of the cell(s).
    *
    * The default value of this property is the null string.
    */
    Q_PROPERTY( QString label READ label WRITE setLabel )
    /*!
    * Specifies if the cell(s) is/are not allowed to be modified.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool readOnly READ readOnly  WRITE setReadOnly )
    /*!
    * This is convenience property which specifies the foreground color
    * of the cell(s) by changing palette property.
    * This color will be used when drawing any text in the cell
    * when the cell is not selected or being edited.
    *
    * The default value of this property is the default selectedForeground color
    * of the QicsTable widget.
    *
    * Note: Since setting property on row/column overrides cells property
    * as well as setting table's property overrides rows/columns property,
    * and foreground color is really part of palette property, setting
    * foreground color on row/column will override any other color
    * settings of cells in that row/column.
    * For example, if you set the foreground on a cell, and then set
    * the background on a row, the foreground of the cell will also
    * be changed to default palette.
    * To obtain desired behavior user should set colors in next sequence:
    * 1. Set the table colors
    * 2. Set the row/column colors
    * 3. Set the cell colors
    */
    Q_PROPERTY( QColor foregroundColor READ foregroundColor WRITE setForegroundColor )
    /*!
    * This is convenience property which specifies the background color
    * of the cell(s) by changing palette property.
    * This color will be used when drawing any text in the cell
    * when the cell is not selected or being edited.
    *
    * The default value of this property is the default selectedBackground color
    * of the QicsTable widget.
    *
    * Note: Since setting property on row/column overrides cells property
    * as well as setting table's property overrides rows/columns property,
    * and foreground color is really part of palette property, setting
    * foreground color on row/column will override any other color
    * settings of cells in that row/column.
    * For example, if you set the foreground on a cell, and then set
    * the background on a row, the foreground of the cell will also
    * be changed to default palette.
    * To obtain desired behavior user should set colors in next sequence:
    * 1. Set the table colors
    * 2. Set the row/column colors
    * 3. Set the cell colors
    */
    Q_PROPERTY( QColor backgroundColor READ backgroundColor WRITE setBackgroundColor )
    /*!
    * This is convenience property which specifies the selected foreground
    * color of the cell(s) by changing palette property.
    * This color will be used when drawing any text in the cell
    * when the cell is selected but not being edited.
    *
    * The default value of this property is the default selectedForeground color
    * of the QicsTable widget.
    *
    * Note: Since setting property on row/column overrides cells property
    * as well as setting table's property overrides rows/columns property,
    * and foreground color is really part of palette property, setting
    * foreground color on row/column will override any other color
    * settings of cells in that row/column.
    * For example, if you set the foreground on a cell, and then set
    * the background on a row, the foreground of the cell will also
    * be changed to default palette.
    * To obtain desired behavior user should set colors in next sequence:
    * 1. Set the table colors
    * 2. Set the row/column colors
    * 3. Set the cell colors
    */
    Q_PROPERTY( QColor selectedForegroundColor READ selectedForegroundColor WRITE setSelectedForegroundColor )
    /*!
    * This is convenience property which specifies the selected background
    * color of the cell(s) by changing palette property.
    * This color will be used when the cell is selected but not being edited.
    *
    * The default value of this property is the default selectedBackground color
    * of the QicsTable widget.
    *
    * Note: Since setting property on row/column overrides cells property
    * as well as setting table's property overrides rows/columns property,
    * and foreground color is really part of palette property, setting
    * foreground color on row/column will override any other color
    * settings of cells in that row/column.
    * For example, if you set the foreground on a cell, and then set
    * the background on a row, the foreground of the cell will also
    * be changed to default palette.
    * To obtain desired behavior user should set colors in next sequence:
    * 1. Set the table colors
    * 2. Set the row/column colors
    * 3. Set the cell colors
    */
    Q_PROPERTY( QColor selectedBackgroundColor READ selectedBackgroundColor WRITE setSelectedBackgroundColor )
    /*!
    * Specifies the edit foreground color of the cell(s).
    * This color will be used when editing text in the cell.
    *
    * The default value of this property is the default text color
    * of the QicsTable widget.
    */
    Q_PROPERTY( QColor editForegroundColor READ editForegroundColor WRITE setEditForegroundColor )
    /*!
    * Specifies the edit background color of the cell(s).
    * This color will be used when editing text in the cell.
    *
    * The default value of this property is the default text color
    * of the QicsTable widget.
    */
    Q_PROPERTY( QColor editBackgroundColor READ editBackgroundColor WRITE setEditBackgroundColor )
    /*!
    * Specifies the pixmap to use when drawing the cell(s).
    * If set, the pixmap is drawn in the cell.
    * If the cell also has a value in the QicsDataModel,
    * the pixmap will be drawn at the left of the cell, and the
    * value will be drawn to the right of the pixmap. If the pixmap is a
    * null pixmap, no pixmap will be shown.
    *
    * The default value of this property is the null pixmap.
    */
    Q_PROPERTY( QPixmap pixmap READ pixmap WRITE setPixmap )
    /*!
    * Specifies the number of pixels between the pixmap and the text
    * in the cell(s).
    *
    * The default value of this property is \b 6.
    */
    Q_PROPERTY( int pixmapSpacing READ pixmapSpacing WRITE setPixmapSpacing )
    /*!
    * Specifies the alignment setting used when drawing the cell(s).
    *
    * The default value of this property is \b Qt::AlignLeft.
    *
    * \sa Qt::Alignment
    */
    Q_PROPERTY( int alignment READ alignment WRITE setAlignment )
    /*!
    * Specifies the text flag setting used when drawing the cell(s).
    * The Qt::DontClip modifier will be ignored.
    * The default value of this property is \b 0.
    *
    * \sa Qt::TextFlags
    */
    Q_PROPERTY( int textFlags READ textFlags WRITE setTextFlags )
    /*!
    * Specifies the maximum length of the value in the cell(s).
    *
    * The default value of this property is \b 32767.
    */
    Q_PROPERTY( int maxLength READ maxLength WRITE setMaxLength )
    /*!
    * Specifies the cell margin (in pixels).
    * The margin is the area between the cell border and the
    * body of the cell that is not used
    * for information display.  It will always be filled with the
    * background color of the cell(s).
    *
    * The default value of this property is \b 1.
    */
    Q_PROPERTY( int margin READ margin WRITE setMargin )
    /*!
    * Specifies the pen that will be used to draw the top border of the cell(s).
    *
    * The default value of this property is the default QPen.
    */
    Q_PROPERTY( QPen topBorderPen READ topBorderPen WRITE setTopBorderPen )

    /*!
    * Specifies the pen that will be used to draw the left border of the cell(s).
    *
    * The default value of this property is the default QPen.
    */
    Q_PROPERTY( QPen leftBorderPen READ leftBorderPen WRITE setLeftBorderPen )

    /*!
    * Specifies the pen that will be used to draw the right border of the cell(s).
    *
    * The default value of this property is the default QPen.
    */
    Q_PROPERTY( QPen rightBorderPen READ rightBorderPen WRITE setRightBorderPen )

    /*!
    * Specifies the pen that will be used to draw the bottom border of the cell(s).
    *
    * The default value of this property is the default QPen.
    */
    Q_PROPERTY( QPen bottomBorderPen READ bottomBorderPen WRITE setBottomBorderPen )

    /*!
    * Specifies whether the table should ignore changes to the size of the model.
    * This can be very useful when you are clearing all rows and columns from
    * the table, but you wish to retain model attribute settings on the table
    * display.  If this value is \b true, these settings will be retained.
    * If this value is \b false, row and column addition and deletion will
    * result in the appropriate changes to the attribute settings and row and
    * column widths of the table.
    */
    Q_PROPERTY( bool ignoreModelSizeChanges READ ignoreModelSizeChanges WRITE setIgnoreModelSizeChanges )

    /*!
    * Specifies whether the table should allow propagating changes from
    * editing cell to whole selection. You can do this by selecting cells
    * you want to change to same value and double click with CTRL key
    * on any cell within selection and entering value you want - after
    * that all selected cells will also change their value to same value.
    *
    * Note: The selection policy shouldn't be SelectSingleRow or
    * SelectMultipleRow to allow this feature.
    *
    * Attention: To use this feature for changing value of the comboboxes
    * you will need to select them using CTRL key, release CTRL and click
    * on last selected combobox holding SHIFT key.
    */
    Q_PROPERTY( bool allowPropagateChanges READ allowPropagateChanges WRITE setAllowPropagateChanges )


    Q_PROPERTY(QString globalXmlConfiguration READ configurationToXml WRITE configureFromXml)

    /*!
    * Setting FakeCellsDrawingIndex forcing table to draw empty not
    * editable cells witch are using default style
    * default value of this property is Qics::NoIndex
    */
    Q_PROPERTY(QicsIndexType fakeCellsDrawingIndex READ fakeCellsDrawingIndex WRITE setFakeCellsDrawingIndex)

    //@}

public:
    ////////////////// CONST/DEST
    /*!
    * Constructs a QicsTable widget that will display data contained in \a model,
    * or, an empty table if \a model is 0.
    */
    QicsTable(QicsDataModel *model = 0, QWidget *parent = 0);

    /*!
    * Constructs an empty table.
    */
    QicsTable(QWidget *parent);

    /*!
    * Constructs a QicsTable widget with one or more custom components.
    * Any foundry method parameter that is 0 will be replaced by the
    * appropriate builtin foundry method (QicsDataModelDefault::create(),
    * QicsTableGrid::create(), or QicsHeaderGrid::create()).
    * \param rhdmf foundry method to create a custom row header data model
    * \param chdmf foundry method to create a custom column header data model
    * \param tf foundry method to create custom table grids
    * \param hf foundry method to create custom header grids
    * \param model data model to display
    * \param parent parent widget
    */
    QicsTable(QicsDataModel::Foundry rhdmf,
        QicsDataModel::Foundry chdmf,
        QicsTableGrid::Foundry tf,
        QicsHeaderGrid::Foundry hf,
        QicsDataModel *model = 0,
        QWidget *parent = 0);

    /*!
    * \internal
    * Constructs a QicsTable widget with one or more custom components.
    * Any foundry method parameter that is 0 will be replaced by the
    * appropriate builtin foundry method (QicsDataModelDefault::create(),
    * QicsTableGrid::create(), or QicsHeaderGrid::create()).
    * This constructor is not exposed as part of the public API.  It has
    * been added because it's necessary for the Python bindings.
    * \param key value that will be passed to each of the special foundry methods
    * \param rhdmf_wk special foundry method to create a custom row header data model
    * \param chdmf_wk special foundry method to create a custom column header data model
    * \param tf_wk special foundry method to create custom table grids
    * \param hf_wk special foundry method to create custom header grids
    * \param model data model to display
    * \param parent parent widget
    */
    QicsTable(void *key,
        QicsDataModel::FoundryWithKey rhdmf_wk,
        QicsDataModel::FoundryWithKey chdmf_wk,
        QicsTableGrid::FoundryWithKey tf_wk,
        QicsHeaderGrid::FoundryWithKey hf_wk,
        QicsDataModel *model = 0,
        QWidget *parent = 0);

    /*!
    * Destructor for the class
    */
    virtual ~QicsTable();

    ///////////////// Data Model

    /*!
    * Returns the data model object that is being displayed by the table widget.
    * \sa setDataModel()
    */
    virtual QicsDataModel *dataModel() const {return m_tableCommon->gridInfo().dataModel();}

    /*!
    * Sets the data model object for the table widget to display.
    * \sa dataModel()
    */
    virtual void setDataModel(QicsDataModel *dm);

    /////////////////// Flyweight class accessors

    /*!
    * Returns a reference to an internal flyweight QicsRow object
    * that refers to row \a rownum.
    * \sa row()
    */
    QicsRow &rowRef(int rownum);
    /*!
    * Returns a \b const reference to an internal flyweight QicsRow object
    * that refers to row \a rownum.
    * \sa row()
    */
    const QicsRow &rowRef(int rownum) const;

    /*!
    * Returns a newly allocated QicsRow object for row \a rownum.
    * If \a follow_model is \b true, the row object will always refer to the
    * row in the data model that is currently being displayed in
    * row \a rownum, even if rows of the table are reordered.
    * If follow_model is \b false, the row will always refer to
    * the visual row \a rownum, but the underlying row in the
    * data model may change.
    * \sa rowRef()
    */
    QicsRow *row(int rownum, bool follow_model = true);
    /*!
    * Returns a newly allocated \b const QicsRow object for row \a rownum.
    * If \a follow_model is \b true, the row object will always refer to the
    * row in the data model that is currently being displayed in
    * row \a rownum, even if rows of the table are reordered.
    * If follow_model is \b false, the row will always refer to
    * the visual row \a rownum, but the underlying row in the
    * data model may change.
    * \sa rowRef()
    */
    const QicsRow *row(int rownum, bool follow_model = true) const;

    /*!
    * Returns a reference to an internal flyweight QicsColumn object
    * that refers to column \a colnum.
    * \sa column()
    */
    QicsColumn &columnRef(int colnum);
    /*!
    * Returns a \b const reference to an internal flyweight QicsColumn object
    * that refers to column \a colnum.
    * \sa column()
    */
    const QicsColumn &columnRef(int colnum) const;

    /*!
    * Returns a newly allocated QicsColumn object for column \a colnum.
    * If \a follow_model is \b true, the column object will always refer to the
    * column in the data model that is currently being displayed in
    * column \a colnum, even if columns of the table are reordered.
    * If follow_model is \b false, the column will always refer to
    * the visual column \a colnum, but the underlying column in the
    * data model may change.
    * \sa columnRef()
    */
    QicsColumn *column(int colnum, bool follow_model = true);
    /*!
    * Returns a newly allocated \b const QicsColumn object for column \a colnum.
    * If \a follow_model is \b true, the column object will always refer to the
    * column in the data model that is currently being displayed in
    * column \a colnum, even if columns of the table are reordered.
    * If follow_model is \b false, the column will always refer to
    * the visual column \a colnum, but the underlying column in the
    * data model may change.
    * \sa columnRef()
    */
    const QicsColumn *column(int colnum, bool follow_model = true) const;

    /*!
    * Returns a reference to an internal flyweight QicsRepeatingRow object
    * that refers to a repeating sequence starting at \a start_row and
    * repeating each \a interval rows.
    * \sa repeatingRow()
    */
    QicsRepeatingRow &repeatingRowRef(int start_row, unsigned int interval);
    /*!
    * Returns a \b const reference to an internal flyweight QicsRepeatingRow
    * object that refers to a repeating sequence starting at \a start_row and
    * repeating each \a interval rows.
    * \sa repeatingRow()
    */
    const QicsRepeatingRow &repeatingRowRef(int start_row,
        unsigned int interval) const;

    /*!
    * Returns a newly allocated QicsRepeatingRow object for
    * a repeating sequence starting at \a start_row and repeating each
    * \a interval rows.
    * \sa repeatingRowRef()
    */
    QicsRepeatingRow *repeatingRow(int start_row, unsigned int interval);
    /*!
    * Returns a newly allocated \b const QicsRepeatingRow object for
    * a repeating sequence starting at \a start_row and repeating each
    * \a interval rows.
    * \sa repeatingRowRef()
    */
    const QicsRepeatingRow *repeatingRow(int start_row,
        unsigned int interval) const;

    /*!
    * Returns a reference to an internal flyweight QicsRepeatingColumn object
    * that refers to a repeating sequence starting at \a start_col and
    * repeating each \a interval columns.
    * \sa repeatingColumn()
    */
    QicsRepeatingColumn &repeatingColumnRef(int start_col,
        unsigned int interval);
    /*!
    * Returns a \b const reference to an internal flyweight QicsRepeatingColumn
    * object that refers to a repeating sequence starting at \a start_col and
    * repeating each \a interval columns.
    * \sa repeatingColumn()
    */
    const QicsRepeatingColumn &repeatingColumnRef(int start_col,
        unsigned int interval) const;

    /*!
    * Returns a newly allocated QicsRepeatingColumn object for
    * a repeating sequence starting at \a start_col and repeating each
    * \a interval columns.
    * \sa repeatingColumnRef()
    */
    QicsRepeatingColumn *repeatingColumn(int start_col,
        unsigned int interval);
    /*!
    * Returns a newly allocated \b const QicsRepeatingColumn object for
    * a repeating sequence starting at \a start_col and repeating each
    * \a interval columns.
    * \sa repeatingColumnRef()
    */
    const QicsRepeatingColumn *repeatingColumn(int start_col,
        unsigned int interval) const;

    /*!
    * Returns a reference to an internal flyweight QicsCell object
    * that refers to cell (\a rownum, \a colnum).
    * \sa cell()
    */
    QicsCell &cellRef(int rownum, int colnum);
    /*!
    * Returns a \b const reference to an internal flyweight QicsCell object
    * that refers to cell (\a rownum, \a colnum).
    * \sa cell()
    */
    const QicsCell &cellRef(int rownum, int colnum) const;

    /*!
    * Returns a newly allocated QicsCell object for cell (\a rownum, \a colnum).
    * If \a follow_model is \b true, the cell object will always refer to the
    * cell in the data model that is currently being displayed in
    * cell (\a rownum, \a colnum), even if rows and columns of the table are
    * reordered.  If follow_model is \b false, the cell will always refer to
    * the visual cell (\a rownum, \a colnum), but the underlying cell in the
    * data model may change.
    * \sa cellRef()
    */
    QicsCell *cell(int rownum, int colnum, bool follow_model = true);
    /*!
    * Returns a newly allocated \b const QicsCell object for cell (\a rownum, \a colnum).
    * If \a follow_model is \b true, the cell object will always refer to the
    * cell in the data model that is currently being displayed in
    * cell (\a rownum, \a colnum), even if rows and columns of the table are
    * reordered.  If follow_model is \b false, the cell will always refer to
    * the visual cell (\a rownum, \a colnum), but the underlying cell in the
    * data model may change.
    * \sa cellRef()
    */
    const QicsCell *cell(int rownum, int colnum, bool follow_model = true) const;

    /*!
    * Returns a reference to an internal flyweight QicsMainGrid object.
    * \sa mainGrid()
    */
    QicsMainGrid &mainGridRef();
    /*!
    * Returns a \b const reference to an internal flyweight QicsMainGrid object.
    * \sa mainGrid()
    */
    const QicsMainGrid &mainGridRef() const;

    /*!
    *  Returns a newly allocated main grid object.
    * \sa mainGridRef()
    */
    QicsMainGrid *mainGrid();
    /*!
    *  Returns a newly allocated \b const main grid object.
    * \sa mainGridRef()
    */
    const QicsMainGrid *mainGrid() const;

    /*!
    * Returns a reference to an internal flyweight QicsRowHeader object.
    * \sa rowHeader()
    */
    QicsRowHeader &rowHeaderRef();
    /*!
    * Returns a \b const reference to an internal flyweight QicsRowHeader object.
    * \sa rowHeader()
    */
    const QicsRowHeader &rowHeaderRef() const;

    /*!
    *  Returns a newly allocated row header object.
    * \sa rowHeaderRef()
    */
    QicsRowHeader *rowHeader();
    /*!
    *  Returns a newly allocated \b const row header object.
    * \sa rowHeaderRef()
    */
    const QicsRowHeader *rowHeader() const;

    /*!
    * Returns a reference to an internal flyweight QicsColumnHeader object.
    * \sa columnHeader()
    */
    QicsColumnHeader &columnHeaderRef();
    /*!
    * Returns a \b const reference to an internal flyweight QicsColumnHeader object.
    * \sa columnHeader()
    */
    const QicsColumnHeader &columnHeaderRef() const;

    /*!
    *  Returns a newly allocated column header object.
    * \sa columnHeaderRef()
    */
    QicsColumnHeader *columnHeader();
    /*!
    *  Returns a newly allocated \b const column header object.
    * \sa columnHeaderRef()
    */
    const QicsColumnHeader *columnHeader() const;

    /*!
    * Clears table deleting all displayers and styles
    */
    void clearTable();

    /////////////////// Selection methods

    /*!
    * If \a nocopy is true, returns pointer to internal selection list.
    * Otherwise, returns a copy of the selection manager's current selection list.
    * It is the programmer's responsibility to delete this list when
    * it is no longer needed.  Modifying the copy will not affect the
    * selection list.  You must call #setSelectionList() with the
    * modified list in order to change the selection.
    * \sa setSelectionList() clearSelectionList()
    */
    QicsSelectionList *selectionList(bool nocopy=false) const;

    /*!
    * Returns a copy of the table's current selection action list.
    * The selection action list is a direct log of the user's selection
    * actions. (For more information on the selection action list, see
    * the <A HREF="#Selections">Selection</A> section of this page.)
    * It is the programmer's responsibility to delete this list when
    * it is no longer needed.  Modifying the copy will not affect the
    * selection list.  You must call #setSelectionList() with the
    * modified list in order to change the selection.
    * \sa selectionList() setSelectionList() clearSelectionList()
    */
    QicsSelectionList *selectionActionList() const;

    /*!
    * Sets the selection list for the table to \a sel_list.  The list will
    * be inspected to assure that it conforms to the current \a selectionPolicy
    * for the table.
    * \sa selectionList() addSelection() clearSelectionList()
    */
    void setSelectionList(QicsSelectionList &sel_list);
    /*!
    * Clears the selection list for the table
    * \sa setSelectionList()
    */
    void clearSelectionList();
    /*!
    * Adds \a selection to the table's selection list.  If the current
    * \a selectionPolicy for the table does not allow multiple selection,
    * this new selection will be ignored.
    * \sa setSelectionList()
    */
    void addSelection(QicsSelection &selection);

    /////////////////// PROPERTY GETS

    ////////// Cell Attribute Gets

    /*!
    * Returns the default cell margin (in pixels).
    * See the \link #margin "margin" \endlink
    * property for details.
    * \sa setMargin()
    */
    int margin() const;

    /*!
    * Returns the default read-only attribute of cells in the table.
    * See the \link #readOnly "readOnly" \endlink
    * property for details.
    * \sa setReadOnly()
    */
    bool readOnly() const;

    /*!
    * Returns the default cell foreground color.
    * See the \link #foregroundColor "foregroundColor" \endlink
    * property for details.
    * \sa setForegroundColor()
    */
    QColor foregroundColor() const;
    /*!
    * Returns the default cell background color.
    * See the \link #backgroundColor "backgroundColor" \endlink
    * property for details.
    * \sa setBackgroundColor()
    */
    QColor backgroundColor() const;

    /*!
    * Returns the default cell selected foreground color.
    * See the \link #selectedForegroundColor "selectedForegroundColor" \endlink
    * property for details.
    * \sa setSelectedForegroundColor()
    */
    QColor selectedForegroundColor() const;

    /*!
    * Returns the default cell selected background color.
    * See the \link #selectedBackgroundColor "selectedBackgroundColor" \endlink
    * property for details.
    * \sa setSelectedBackgroundColor()
    */
    QColor selectedBackgroundColor() const;

    /*!
    * Returns the default cell edit foreground color.
    * See the \link #editForegroundColor "editForegroundColor" \endlink
    * property for details.
    * \sa setEditForegroundColor()
    */
    QColor editForegroundColor() const;

    /*!
    * Returns the default cell edit background color.
    * See the \link #editBackgroundColor "editBackgroundColor" \endlink
    * property for details.
    * \sa setEditBackgroundColor()
    */
    QColor editBackgroundColor() const;

    /*!
    * Returns the default cell pixmap.
    * See the \link #pixmap "pixmap" \endlink
    * property for details.
    * \sa setPixmap()
    */
    QPixmap pixmap() const;

    /*!
    * Returns the cell's pixmap spacing
    * See the \link #pixmap "pixmapSpacing" \endlink property for details.
    * \sa setPixmap()
    */
    int pixmapSpacing() const;

    /*!
    * Returns the default QicsCellDisplay object. The cell display object
    * controls aspects of displaying and editing of cells.  See QicsCellDisplay
    * and its subclasses for details.
    * \sa setDisplayer()
    */
    QicsCellDisplay *displayer() const;

    /*!
    * Returns the default QicsDataItemFormatter object.
    * The formatter object controls the formatting of data values.
    * See QicsDataItemFormatter and its subclasses for details.
    * \sa setFormatter()
    */
    QicsDataItemFormatter *formatter() const;

    /*!
    * Returns the default cell alignment options.
    * See the \link #alignment "alignment" \endlink
    * property for details.
    * \sa Qt::Alignment
    * \sa setAlignment()
    */
    int alignment() const;

    /*!
    * Returns the default cell text flags attribute.
    * See the \link #textFlags "textFlags" \endlink
    * property for details.
    * \sa Qt::TextFlags
    * \sa setTextFlags()
    */
    int textFlags() const;

    /*!
    * Returns the default cell validator object.
    * This validator will be used when cells are edited.
    * See the \link #validator "validator" \endlink
    * property for details.
    * \sa setValidator()
    */
    QValidator *validator() const;

    /*!
    * Returns the paste validator object for this cell.
    * See the \link #pasteValidator "pasteValidator" \endlink
    * property for details.
    * \sa setPasteValidator()
    */
    QicsPasteValidator *pasteValidator() const;

    /*!
    * Returns the default cell label attribute.
    * See the \link #label "label" \endlink
    * property for details.
    * \sa setLabel()
    */
    QString label() const;

    /*!
    * Returns the default maximum cell value length.
    * See the \link #maxLength "maxLength" \endlink
    * property for details.
    * \sa setMaxLength()
    */
    int maxLength() const;

    /*!
    * Returns the default cell top border pen.
    * See the \link #topBorderPen "borderPen" \endlink
    * property for details.
    * \sa setBorderPen()
    */
    QPen topBorderPen() const;

    /*!
    * Returns the default cell left border pen.
    * See the \link #leftBorderPen "borderPen" \endlink
    * property for details.
    * \sa setBorderPen()
    */
    QPen leftBorderPen() const;

    /*!
    * Returns the default cell right border pen.
    * See the \link #topBorderPen "topBorderPen" \endlink
    * property for details.
    * \sa setTopBorderPen()
    */
    QPen rightBorderPen() const;

    /*!
    * Returns the default cell bottom border pen.
    * See the \link #bottomBorderPen "borderPen" \endlink
    * property for details.
    * \sa setBorderPen()
    */
    QPen bottomBorderPen() const;

    ////////// Grid Attribute Gets

    /*!
    * Returns the current viewport of the table.  The viewport
    * specifies an area of the data model that is allowed to be
    * displayed.  This viewport does not take into account any
    * rows or columns that may be frozen.  To get the viewport of
    * the main grid (and thus excluding any frozen rows and columns),
    * use #mainGridViewport().
    *
    * \sa setViewport()
    */
    virtual QicsRegion viewport() const;

    /*!
    * Returns the current viewport of the main grid of the table.
    * The viewport specifies an area of the data model that is allowed to be
    * displayed, excluding any rows or columns that may be frozen.
    */
    virtual QicsRegion mainGridViewport() const;

    /*!
    * Returns the intersection of the table's #viewport and
    * the dimensions of the table's data model.
    */
    virtual QicsRegion currentViewport() const;

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
    * Returns \b true if horizontal grid lines are being drawn in the grid,
    * \b false otherwise.  These lines separate each row.
    * See the
    * \link #horizontalGridLinesVisible "horizontalGridLinesVisible" \endlink
    * property for details.
    * \sa setHorizontalGridLinesVisible()
    */
    bool horizontalGridLinesVisible() const;

    /*!
    * Returns \b true if vertical grid lines are being drawn, \b false otherwise.
    * See the
    * \link #verticalGridLinesVisible "verticalGridLinesVisible" \endlink
    * property for details.
    * \sa setVerticalGridLinesVisible()
    */
    bool verticalGridLinesVisible() const;

    /*!
    * Returns the value of the width of the table's horizontal grid lines.
    * See the \link #horizontalGridLineWidth "horizontalGridLineWidth" \endlink
    * property for details.
    * \sa setHorizontalGridLineWidth()
    */
    int horizontalGridLineWidth() const;

    /*!
    * Returns the value of the width of the table's vertical grid lines.
    * See the \link #verticalGridLineWidth "verticalGridLineWidth" \endlink
    * property for details.
    * \sa setVerticalGridLineWidth()
    */
    int verticalGridLineWidth() const;

    /*!
    * Returns the horizontal grid line style.
    * See the \link #horizontalGridLineStyle "horizontalGridLineStyle" \endlink
    * property for details.
    * \sa setHorizontalGridLineStyle()
    */
    QicsLineStyle horizontalGridLineStyle() const;

    /*!
    * Returns the vertical grid line style.
    * See the \link #verticalGridLineStyle "verticalGridLineStyle" \endlink
    * property for details.
    * \sa setVerticalGridLineStyle()
    */
    QicsLineStyle verticalGridLineStyle() const;

    /*!
    * Returns the pen used to draw the table's horizontal grid lines.
    * See the \link #horizontalGridLinePen "horizontalGridLinePen" \endlink
    * property for details.
    * \sa setHorizontalGridLinePen()
    */
    QPen horizontalGridLinePen() const;

    /*!
    * Returns the pen used to draw the table's vertical grid lines.
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
    * Returns the frame line width of the table's main grid.
    * See the \link #frameLineWidth "frameLineWidth" \endlink
    * property for details.
    * \sa setFrameLineWidth()
    */
    int frameLineWidth() const;

    /*!
    * Returns the frame style of the table's main grid.
    * See the \link #frameStyle "frameStyle" \endlink
    * property for details.
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
    * Returns current selection style of the table.
    * See the \link #selectionStyle "selectionStyle" \endlink
    * property for details.
    * \sa  setSelectionStyle()
    */
    QicsSelectionStyle selectionStyle() const;

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
    * for details
    * \sa setClickToEdit()
    */
    bool clickToEdit() const;

    /*!
    * Returns whether the contents of a cell are automatically selected
    * when the table begins a cell edit operation.
    * See the \link #autoSelectCellContents "autoSelectCellContents" \endlink
    * property for details
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
    * property for details
    * \sa setEnterTraversalDirection()
    */
    Qt::Orientation enterTraversalDirection() const;

    /*!
    * Returns the direction that the table will traverse when the user
    * types \a Tab or \a Shift-Tab.
    * See the \link #tabTraversalDirection "tabTraversalDirection" \endlink
    * property for details
    * \sa setTabTraversalDirection()
    */
    Qt::Orientation tabTraversalDirection() const;

    /*!
    *  Returns key action defined for \b combo key combination.
    */
    QicsKeyboardManager::Action keyAction(const QicsKeyCombination& combo);

    /*!
    * Sets the key combination \b combo for \b action key action.
    */
    void setKeyAction(const QicsKeyCombination& combo, QicsKeyboardManager::Action action);

    /*!
    * Returns traversal cell behavior (for Tab key) when it achieves the end of row/column.
    * See the \link #tabTraverseToBegin "tabTraverseToBegin" \endlink property
    * for details.
    * \sa setTabTraverseToBegin(bool)
    */
    bool tabTraverseToBegin() const;

    /*!
    * Returns traversal cell behavior (for Enter key) when it achieves
    * the end of row/column.
    * See the \link #enterTraverseToBegin "enterTraverseToBegin" \endlink property
    * for details.
    * \sa setEnterTraverseToBegin(bool)
    */
    bool enterTraverseToBegin() const;

    /*!
    * Returns the pixmap that is used in this grid to show that text
    * in a cell has been clipped (i.e. it could not completely fit in the cell).
    * \sa setMoreTextPixmap()
    */
    QPixmap moreTextPixmap() const;

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
    Qt::LayoutDirection layoutDirection() const;

    /*!
    * Returns true if this fill handle visible;
    * otherwise returns false.
    * \sa setFillHandleVisible()
    */
    bool fillHandleVisible() const;

    ////////// Table Common Gets

    /*!
    * Returns \b true if the top header is visible, \b false otherwise.
    * See the \link #topHeaderVisible "topHeaderVisible" \endlink
    * property for details.
    */
    bool topHeaderVisible() const;
    /*!
    * Returns \b true if the bottom header is visible, \b false otherwise.
    * See the \link #bottomHeaderVisible "bottomHeaderVisible" \endlink
    * property for details.
    */
    bool bottomHeaderVisible() const;
    /*!
    * Returns \b true if the left header is visible, \b false otherwise.
    * See the \link #leftHeaderVisible "leftHeaderVisible" \endlink
    * property for details.
    */
    bool leftHeaderVisible() const;
    /*!
    * Returns \b true if the right header is visible, \b false otherwise.
    * See the \link #rightHeaderVisible "rightHeaderVisible" \endlink
    * property for details.
    */
    bool rightHeaderVisible() const;

    /*!
    * Returns the margin of the table widget.
    * See the \link #tableMargin "tableMargin" \endlink
    * property for details.
    */
    int tableMargin() const;

    /*!
    * Returns the spacing of the table widget.
    * See the \link #tableSpacing "tableSpacing" \endlink
    * property for details.
    */
    int tableSpacing() const;

    /*!
    * Returns the spacing of the grids in the table widget.
    * See the \link #gridSpacing "gridSpacing" \endlink
    * property for details.
    */
    int gridSpacing() const;

    /*!
    * Returns whether the table should ignore size change signals from
    * the data model.
    * See the \link #ignoreModelSizeChanges "ignoreModelSizeChanges" \endlink
    * property for details.
    * \sa setIgnoreModelSizeChanges()
    */
    bool ignoreModelSizeChanges() const;

    /*!
    * Returns whether the table should allow propagate changes from editing
    * cell to whole selection.
    * See the \link #allowPropagateChanges "allowPropagateChanges" \endlink
    * property for details.
    * \sa setAllowPropagateChanges()
    */
    bool allowPropagateChanges() const;

    /*!
    * Returns the table's current selection policy.
    * See the \link #selectionPolicy "selectionPolicy" \endlink
    * property for details.
    */
    QicsSelectionPolicy selectionPolicy() const;
    /*!
    * Sets the table's selection policy.  Calling this method will also
    * clear the table's current selection.
    * See the \link #selectionPolicy "selectionPolicy" \endlink
    * property for details.
    */
    void setSelectionPolicy(QicsSelectionPolicy policy);

    /*!
    * Returns the table's current cell selection policy.
    * See the \link #selectCurrentCellPolicy "selectCurrentCellPolicy" \endlink
    * property for details.
    * \since 2.3
    */
    QicsSelectCurrentCellPolicy selectCurrentCellPolicy() const;
    /*!
    * Sets the table's current cell selection policy.
    * See the \link #selectCurrentCellPolicy "selectCurrentCellPolicy" \endlink
    * property for details.
    * \since 2.3
    */
    void setSelectCurrentCellPolicy(QicsSelectCurrentCellPolicy policy);

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
    * Sets the table's FakeCellsDrawingIndex for drawing fake cells
    * See the \link #fakeCellsDrawingIndex "fakeCellsDrawingIndex" \endlink
    * property for details.
    */
    void setFakeCellsDrawingIndex(Qics::QicsIndexType type);

    /*!
    * Returns the repaint behavior of the table widget.
    * See the \link #repaintBehavior "repaintBehavior" \endlink
    * property for details
    */
    QicsRepaintBehavior repaintBehavior() const;

    /*!
    * Returns the mode of the horizontal scrollbar.
    * See the \link #hScrollBarMode "hScrollBarMode" \endlink
    * property for details
    */
    QicsScrollBarMode hScrollBarMode() const;

    /*!
    * Returns the mode of the vertical scrollbar.
    * See the \link #vScrollBarMode "vScrollBarMode" \endlink
    * property for details
    */
    QicsScrollBarMode vScrollBarMode() const;

    /*!
    * Sets the mode of the horizontal scrollbar.
    * See the \link #hScrollBarMode "hScrollBarMode" \endlink
    * property for details
    */
    void setHScrollBarMode(QicsScrollBarMode m);

    /*!
    * Sets the mode of the vertical scrollbar.
    * See the \link #vScrollBarMode "vScrollBarMode" \endlink
    * property for details
    */
    void setVScrollBarMode(QicsScrollBarMode m);

    /*!
    * Sets the autofit mode.
    * \since 2.4.1
    */
    void setAutoFitMode(Qics::QicsAutoFitMode mode);

    /*!
    * Returns current autofit mode.
    * \since 2.4.1
    */
    Qics::QicsAutoFitMode autoFitMode() const;

    /*!
    * Returns current row order as QVector<int>.
    * \since 2.3.1
    */
    const QVector<int>& currentRowOrder() const
    { return gridInfo().rowOrdering()->currentOrder(); }

    /*!
    * Sets current row order to \a order.
    * \since 2.3.1
    */
    void setRowOrder(const QVector<int> &order);

    /*!
    * Returns current column order as QVector<int>.
    * \since 2.3.1
    */
    const QVector<int>& currentColumnOrder() const
    { return gridInfo().columnOrdering()->currentOrder(); }

    /*!
    * Sets current column order to \a order.
    * \since 2.3.1
    */
    void setColumnOrder(const QVector<int> &order);

    /*!
    * Sort the rows
    * \param column the data model columns to sort on
    * \param order sort order <i>(default: Qics::Ascending)</i>
    * \param from only sort the rows between \a from and \a to
    * \param to only sort the rows between \a from and \a to.  The
    * default of -1 will cause all rows through the end to be sorted
    * \param func \a optional a comparator function.  Specifying
    * 0 will use a default comparator which should work for almost
    * every case.   Applications which add new subclasses of QicsDataItem
    * to their tables, should provide type specific comparators with their
    * data elements.
    * \sa multicolumn sort the rows
    */
    void sortRows(int column,
        QicsSortOrder order = Qics::Ascending,
        int from = 0, int to = -1,
        DataItemComparator func = 0);

    /*!
    * Sort the columns
    * \param row the data model rows to sort on
    * \param order sort order <i>(default: Qics::Ascending)</i>
    * \param from only sort the columns between \a from and \a to
    * \param to only sort the columns between \a from and \a to.  The
    * default of -1 will cause all columns through the end to be sorted
    * \param func \a optional a comparator function.  Specifying
    * 0 will use a default comparator which should work for almost
    * every case.   Applications which add new subclasses of QicsDataItem
    * to their tables, should provide type specific comparators with their
    * data elements.
    * \sa multirow sort
    */
    void sortColumns(int row,
        QicsSortOrder order = Qics::Ascending,
        int from = 0, int to = -1,
        DataItemComparator func = 0);

    /*!
    * Multicolumn sort the rows
    * \param columns the data model columns to sort on
    * \param order sort order <i>(default: Qics::Ascending)</i>
    * \param from only sort the rows between \a from and \a to
    * \param to only sort the rows between \a from and \a to.  The
    * default of -1 will cause all rows through the end to be sorted
    * \param func \a optional a comparator function.  Specifying
    * 0 will use a default comparator which should work for almost
    * every case.   Applications which add new subclasses of QicsDataItem
    * to their tables, should provide type specific comparators with their
    * data elements.
    */
    void sortRows(const QVector<int> &columns,
        QicsSortOrder order = Qics::Ascending,
        int from = 0, int to = -1,
        DataItemComparator func = 0);

    /*!
    * Multirow sort the columns
    * \param rows the data model rows to sort on
    * \param order sort order <i>(default: Qics::Ascending)</i>
    * \param from only sort the columns between \a from and \a to
    * \param to only sort the columns between \a from and \a to.  The
    * default of -1 will cause all columns through the end to be sorted
    * \param func \a optional a comparator function.  Specifying
    * 0 will use a default comparator which should work for almost
    * every case.   Applications which add new subclasses of QicsDataItem
    * to their tables, should provide type specific comparators with their
    * data elements.
    */
    void sortColumns(const QVector<int> &rows,
        QicsSortOrder order = Qics::Ascending,
        int from = 0, int to = -1,
        DataItemComparator func = 0);

    /*!
    * Sets case sensitivity while sorting to \a cs.
    * \sa sortColumns(), sortRows()
    * \since 2.3
    */
    void setSortingSensitivity(Qt::CaseSensitivity cs);

    /*!
    * Moves a set of (possibly discontiguous) rows to a new location.
    * \param target_row the row before which the moved rows will be placed
    * \param rows a list of row indices to be moved
    */
    void moveRows(int target_row, const QVector<int> &rows);

    /*!
    * Moves a set of (possibly discontiguous) columns to a new location.
    * \param target_col the column before which the moved columns will be placed
    * \param cols a list of column indices to be moved
    */
    void moveColumns(int target_col, const QVector<int> &cols);

    /*!
    * Returns whether the first column of data in the table's data model
    * is used as row header data.
    * See the \link #rowHeaderUsesModel "rowHeaderUsesModel" \endlink
    * property for details.
    */
    bool rowHeaderUsesModel() const;

    /*!
    * Returns whether the first row of data in the table's data model
    * is used as column header data.
    * See the \link #columnHeaderUsesModel "columnHeaderUsesModel" \endlink
    * property for details.
    */
    bool columnHeaderUsesModel() const;

    /////////////////////////////////////////////////////////////////

    /*!
    * Returns the index of the row that is currently displayed at the
    * top of the table widget.
    * See the \link #topRow "topRow" \endlink
    * property for details.
    * \sa setTopRow()
    */
    int topRow() const;
    /*!
    * Returns the index of the row that is currently displayed at the
    * bottom of the table widget.
    * See the \link #bottomRow "bottomRow" \endlink
    * property for details.
    */
    int bottomRow() const;
    /*!
    * Returns the index of the leftmost column that is currently displayed
    * in the table widget.
    * See the \link #leftColumn "leftColumn" \endlink
    * property for details.
    * \sa setLeftColumn()
    */
    int leftColumn() const;
    /*!
    * Returns the index of the rightmost column that is currently displayed
    * in the table widget.
    * See the \link #rightColumn "rightColumn" \endlink
    * property for details.
    */
    int rightColumn() const;

    /*!
    * Returns the number of currently visible rows in the main grid area
    * of the table.
    * See the \link #visibleRows "visibleRows" \endlink
    * property for details.
    * \sa setVisibleRows()
    */
    int visibleRows() const;
    /*!
    * Returns the number of currently visible columns in the main grid area
    * of the table.
    * See the \link #visibleColumns "visibleColumns" \endlink
    * property for details.
    * \sa setVisibleColumns()
    */
    int visibleColumns() const;

    /*!
    * Returns the index of bottom fully visible row in the main grid area
    * of the table.
    */
    int fullyVisibleBottomRow() const;

    /*!
    * Returns the index of right fully visible column in the main grid area
    * of the table.
    */
    int fullyVisibleRightColumn() const;

    /*!
    * Returns the current cell of the table widget.  Note that if there
    * is no current cell (i.e. the user has not selected a cell) the
    * returned cell will be invalid.  Check a cell's validity with
    * QicsCell::isValid().
    * \sa setCurrentCell()
    */
    inline QicsCell *currentCell() const {return myCurrentCell;}

    /*!
    * Begins an edit of the current cell.  Returns \b true if
    * the cell successfully begins an edit operation, \b false otherwise.
    */
    bool editCurrentCell();

    /*!
    * Stops an edit operation on the current cell.
    */
    void uneditCurrentCell();

    /*!
    * Begins an edit of cell (\a row, \a col ).  Returns \b true if
    * the cell successfully begins an edit operation, \b false otherwise.
    */
    bool editCell(int row, int col);

    /*!
    * Returns index of last row with data, -1 if there are no data.
    * \since 2.2
    */
    int lastRowWithData() const;

    /*!
    * Returns index of last column with data, -1 if there are no data.
    * \since 2.2
    */
    int lastColumnWithData() const;

    /*!
    * Returns index of first row with data, -1 if there are no data.
    * \since 2.2
    */
    int firstRowWithData() const;

    /*!
    * Returns index of first column with data, -1 if there are no data.
    * \since 2.2
    */
    int firstColumnWithData() const;

    /*!
    * Returns region covering all the data cells, or invalid region if there are no data.
    * \since 2.2
    */
    QicsRegion dataRegion() const;

    /*!
    * Returns region covering all the selections, or invalid region if there are no selections.
    * \since 2.2
    */
    QicsRegion selectedRegion() const;

    /*!
    * Returns \b true if the \link #QicsNavigator navigator \endlink is allowed, else returns \b false.
    * \sa setNavigatorAllowed(), navigator()
    */
    inline bool isNavigatorAllowed() const {return m_navAllowed;}

    /*!
    * Returns pointer to the navigator (to modify its properties etc.)
    * \since 2.2
    * \sa setNavigatorAllowed(), isNavigatorAllowed()
    */
    inline QicsNavigator* navigator() const {return m_navButton;}

    /*!
    * Sets enable state of the global key \a action to \a on.
    * \since 2.2
    * \sa isWideKeyAction(), setWideKeyActions()
    */
    void setWideKeyAction(QicsWideKeyAction action, bool on = true);

    /*!
    * Returns enable state of the global key \a action.
    * \since 2.2
    * \sa setWideKeyAction(), setWideKeyActions()
    */
    inline bool isWideKeyAction(QicsWideKeyAction action) const {return myWideKeys.contains(action);}

    /*!
    * Sets all the global key actions to \a on.
    * \since 2.2
    * \sa setWideKeyAction(), isWideKeyAction()
    */
    void setWideKeyActions(bool on = true);

    /*!
    * Sets copying policy during copy/paste and drag/drop operations to \a policy.
    * \a policy should be bitwise 'or' of QicsCopyPolicy values.
    * \since 2.2
    * \sa copyPolicy()
    */
    inline void setCopyPolicy(Qics::QicsCopyPolicy policy) {gridInfo().setCopyPolicy(policy);}

    /*!
    * Returns current copying policy during copy/paste and drag/drop operations
    * as bitwise 'or' of QicsCopyPolicy values.
    * \since 2.2
    * \sa setCopyPolicy()
    */
    inline Qics::QicsCopyPolicy copyPolicy() const {return gridInfo().copyPolicy();}

    /*!
    * Sets clearing policy during cut and delete operations to \a policy.
    * \a policy should be bitwise 'or' of QicsClearPolicy values.
    * \since 2.2
    * \sa clearPolicy()
    */
    inline void setClearPolicy(int policy) {gridInfo().setClearPolicy(policy);}

    /*!
    * Returns current clearing policy during cut and delete operations
    * as bitwise 'or' of QicsClearPolicy values.
    * \since 2.2
    * \sa setClearPolicy()
    */
    inline int clearPolicy() const {return gridInfo().clearPolicy();}

    /*!
    * Sets header drag style to \a style
    * \since 2.4
    * \sa headerDragStyle()
    */
    void setHeaderDragStyle (const QicsHeaderDragStyle &style);

    /*!
    * Returns current header drag style
    * \since 2.4
    * \sa setHeaderDragStyle()
    */
    QicsHeaderDragStyle headerDragStyle() const;

    /*!
    * Returns the horizontal scrollbar for the table.  The table's behavior
    * is undefined if the programmer changes the scrollbar's minValue or
    * maxValue properties.  Use QTable::setViewport instead.
    */
    QAbstractSlider *horizontalScrollBar() const;

    /*!
    * Sets new horizontal scroll bar. New scroll bar class
    * must be implementation of \b QicsScroller or \b QicsScrollBarScroller.
    * For example if you want create custom scroll bar by deriving
    * \b QicsScrollBarScroller reimplement constructor like that:
    *
    * \code
    *		myScroller::myScroller(Qics::QicsIndexType type,
    *		QWidget *parent) : QicsScrollBarScroller(type,parent)
    *		{
    *			Qt::Orientation orientation = (type == RowIndex ? Qt::Vertical :
    *															Qt::Horizontal);
    *			QSlider *slider = new QSlider(orientation, parent);
    *			slider->setCursor(Qt::PointingHandCursor);
    *			slider->setToolTip("Custom ScrollBar");
    *			setWidget(slider);
    *		}
    * \endcode
    *
    *	\b QicsScrollBarScroller::setWidget method removes if it does need old widget and
    *	sets new widget.
    *	In case of deriving custom scroll bar from \b QicsScroller class you must
    *	reimplement all virtual methods by yourself.
    */
    void setHorizontalScrollBar(QicsScroller *newScroller);

    /*!
    * Sets new vertical scroll bar. New scroll bar class
    * must be implementation of QicsScroller or QicsScrollBarScroller.
    */
    void setVerticalScrollBar(QicsScroller *newScroller);

    /*!
    * Returns the vertical scrollbar for the table.  The table's behavior
    * is undefined if the programmer changes the scrollbar's minValue or
    * maxValue properties.  Use QTable::setViewport instead.
    */
    QAbstractSlider *verticalScrollBar() const;

    /*!
    * Returns the title widget placed at the top of the table.
    * \sa setTopTitleWidget
    */
    inline QWidget *topTitleWidget() const {return myTopTitleWidget;}

    /*!
    * Returns the title widget placed at the bottom of the table.
    * \sa setBottomTitleWidget
    */
    inline QWidget *bottomTitleWidget() const {return myBottomTitleWidget;}

    /*!
    * Returns the title widget placed at the left of the table.
    * \sa setLeftTitleWidget
    */
    inline QWidget *leftTitleWidget() const {return myLeftTitleWidget;}

    /*!
    * Returns the title widget placed at the right of the table.
    * \sa setRightTitleWidget
    */
    inline QWidget *rightTitleWidget() const {return myRightTitleWidget;}

    /*!
    * Returns the widget placed in the top left corner of the table,
    * above the left header and to the left of the top header.
    * \sa setTopLeftCornerWidget()
    */
    inline QWidget *topLeftCornerWidget() const {return myTopLeftCornerWidget;}

    /*!
    * Returns the widget placed in the top right corner of the table,
    * above the right header and to the right of the top header.
    * \sa setTopRightCornerWidget()
    */
    inline QWidget *topRightCornerWidget() const {return myTopRightCornerWidget;}

    /*!
    * Returns the widget placed in the bottom left corner of the table,
    * below the left header and to the left of the bottom header.
    * \sa setBottomLeftCornerWidget()
    */
    inline QWidget *bottomLeftCornerWidget() const {return myBottomLeftCornerWidget;}

    /*!
    * Returns the widget placed in the bottom right corner of the table,
    * below the right header and to the right of the bottom header.
    * \sa setBottomRightCornerWidget()
    */
    inline QWidget *bottomRightCornerWidget() const {return myBottomRightCornerWidget;}

    /*!
    * Returns the grid geometry object of the table.
    * \since 2.3.1
    */
    inline QicsGridGeometry& gridGeometry() {return myGridGeometry;}

    /*!
    * Returns the height of the row header.
    * \since 2.3.1
    */
    int rowHeaderHeight();

    /*!
    * Returns the width of the column header.
    * \since 2.3.1
    */
    int columnHeaderWidth();

    bool hasHiddenRows() const;

    bool hasHiddenColumns() const;

    // Filter

    /*! Returns \link #QicsRowFilter filter \endlink installed for \a column column, or 0 if none.
    \sa setRowFilter(), removeRowFilter(), removeAllRowFilters()
    */
    inline QicsAbstractFilterDelegate* rowFilter(int column) const
    { return gridInfo().rowFilter()->filter(column); }

    /*!
    *  Sets \link #QicsRowFilter filter \endlink to \a column column. Rows that don't match \a filter
    *  in \a column column will be hidden.
    *  If \a deleteOld is true, previous filter will be destroyed.
    *  \sa rowFilter(), removeRowFilter(), removeAllRowFilters()
    */
    void setRowFilter(int column, QicsAbstractFilterDelegate *filter, bool deleteOld = false);

    /*! Removes \link #QicsRowFilter filter \endlink from \a column column.
    *  If \a deleteOld is true, previous filter will be destroyed.
    *  \sa rowFilter(), setRowFilter(), removeAllRowFilters()
    */
    void removeRowFilter(int column, bool deleteOld = false);

    /*! Removes all \link #QicsRowFilter row filters \endlink from all columns.
    \sa rowFilter(), setRowFilter(), removeRowFilter()
    */
    void removeAllRowFilters();

    /*!
    * Returns \a true if row with index \a row is filtered off
    * (i.e. not visible), \a false otherwise.
    * \since 2.4
    */
    inline bool isRowFiltered(int row) const
    { return gridInfo().rowFilter()->isFiltered(row); }

    /*!
    * Returns true if \a column has any filter installed.
    * \since 2.4
    */
    inline bool hasRowFilter(int column) const
    { return gridInfo().rowFilter()->hasFilter(column); }

    /*!
    * Returns list of unique keys for \a column (model coords).
    * If \a noEmpty is set (default), then list will not include emtpy items.
    * \since 2.4
    */
    QStringList	uniqueKeysForColumn(int column, bool noEmpty = true) const;

    // Sorting

    /**
    * Sets the sorting algorithm for columns.
    * @param mode  sort mode sets to.
    */
    inline void setColumnsSortingMode(Qics::QicsSortMode mode)
    { gridInfo().columnOrdering()->setSortMode(mode); }

    /**
    * Gets current columns sorting mode.
    * @return current sorting mode.
    */
    inline Qics::QicsSortMode columnsSortingMode()
    { return gridInfo().columnOrdering()->sortMode(); }

    /**
    * Sets the sorting algorithm for rows.
    * @param mode  sort mode sets to.
    */
    inline void setRowsSortingMode(Qics::QicsSortMode mode)
    { gridInfo().rowOrdering()->setSortMode(mode); }

    /**
    * Gets current rows sorting mode.
    * @return current sorting mode.
    */
    inline Qics::QicsSortMode rowsSortingMode()
    { return gridInfo().rowOrdering()->sortMode(); }

    inline void setSorterDelegate(QicsAbstractSorterDelegate *sorter)
    {gridInfo().setSorterDelegate(sorter);}

    inline QicsAbstractSorterDelegate *sorterDelegate() const
    {return gridInfo().sorterDelegate();}

    inline void setRowSorterDelegate(int index, QicsAbstractSorterDelegate *sorter)
    {gridInfo().rowOrdering()->setSorterDelegate(index, sorter);}

    inline QicsAbstractSorterDelegate *rowSorterDelegate(int index) const
    {return gridInfo().rowOrdering()->sorterDelegate(index);}

    inline void setColumnSorterDelegate(int index, QicsAbstractSorterDelegate *sorter)
    {gridInfo().columnOrdering()->setSorterDelegate(index, sorter);}

    inline QicsAbstractSorterDelegate *columnSorterDelegate(int index) const
    {return gridInfo().columnOrdering()->sorterDelegate(index);}

    /*!
    * This method set mouse button mapping inside \sa QicsTable.
    * Usefull for Apple mouse, 2-button mouse or for individual
    * remapping.
    * First paramater action button, second keyboard modifier, third result
    * button (which emulate).
    */
    bool setMouseButton( Qt::MouseButton, Qt::Modifier, Qt::MouseButton );

    void registerDisplayFactory(const QString& name, QicsCellDisplayFactory* factory);

    QStringList registeredCellDisplays();

    QicsCellDisplay* createCellDisplay(const QString& name);

    QicsCellDisplayConfigurationWidget* createCellDisplayConfigurationWidget(const QString& name);

    /*!
    * Returns the FakeCellsDrawingIndex for drawing fake cells.
    * See the \link #fakeCellsDrawingIndex "fakeCellsDrawingIndex" \endlink
    * property for details.
    */
    Qics::QicsIndexType fakeCellsDrawingIndex() const;

    /*!
    Returns current selection or 0 if nothing selected.
    This selection is NOT included into selection list while it is
    in progress (i.e. being performed with mouse etc.). Hovewer, sometimes it is usable to get
    unfinished selection.

    \sa selectionList()
    \since 2.2
    */
    const QicsSelection* currentSelection();

    /*!
    * Returns a reference to an internal flyweight QicsCellRegion object
    * that refers to cells in \a region of the grid.
    */
    inline QicsCellRegion& cellRegionRef(const QicsRegion &region)
    { return *cellRegion( region ); }

    /*!
    * Returns a reference to an internal flyweight QicsCellRegion object
    * that refers to cells in region \a begin_row, \a begin_col, \a end_row, \a end_col of the grid.
    */
    inline QicsCellRegion& cellRegionRef(int begin_row, int begin_col, int end_row, int end_col)
    { return cellRegionRef(QicsRegion(begin_row, begin_col, end_row, end_col)); }

    /*!
    * Returns a pointer to an internal flyweight QicsCellRegion object
    * that refers to cells in \a region of the grid.
    */
    QicsCellRegion* cellRegion(const QicsRegion &region);

    /*!
    * Returns a pointer to an internal flyweight QicsCellRegion object
    * that refers to cells in region \a begin_row, \a begin_col, \a end_row, \a end_col of the grid.
    */
    inline QicsCellRegion* cellRegion(int begin_row, int begin_col, int end_row, int end_col)
    { return cellRegion(QicsRegion(begin_row, begin_col, end_row, end_col)); }


    /*!
    * Sets external attribute controller to \a mac
    * (only controlling of model attributes is supported at this moment).
    * Really, this method makes a copy of the controller - so it can be
    * safely deleted after the method is called:
    *
    * \code
    *      table->setExternalAttributeController(QicsCommonAttributeController());
    * \endcode
    *
    * \sa removeExternalAttributeController()
    *
    * Please keep in mind that installing of a new controller will result in
    * full wipe-out of all attributes previously set.
    * \since 2.2
    * \sa removeExternalAttributeController(), setExternalAttributeControllers()
    */
    void setExternalAttributeController(QicsAbstractAttributeController& mac);

    /*!
    * Sets external attribute controller of main grid, row and column headers
    * respectively to \a macGrid, \a macRowHeader and \a macColumnHeader.
    * \since 2.3
    * \sa setExternalAttributeController()
    */
    void setExternalAttributeControllers(
        QicsAbstractAttributeController* macGrid,
        QicsAbstractAttributeController* macRowHeader,
        QicsAbstractAttributeController* macColumnHeader);

    /*!
    * Returns external attribute controller for main grid.
    * \since 2.3
    * \sa setExternalAttributeController(), setExternalAttributeControllers()
    */
    QicsAbstractAttributeController* gridAttributeController() const;
    /*!
    * Returns external attribute controller for row header.
    * \since 2.3
    * \sa setExternalAttributeController(), setExternalAttributeControllers()
    */
    QicsAbstractAttributeController* rowHeaderAttributeController() const;
    /*!
    * Returns external attribute controller for column header.
    * \since 2.3
    * \sa setExternalAttributeController(), setExternalAttributeControllers()
    */
    QicsAbstractAttributeController* columnHeaderAttributeController() const;

    /*!
    * Removes external attribute controller.
    * \since 2.2
    * \sa setExternalAttributeController()
    */
    void removeExternalAttributeController();

public slots:
    /////// Cell Attribute Sets

    /*!
    * Sets the default cell margin (in pixels).
    * See the \link #margin "margin" \endlink
    * property for details.
    * \sa margin()
    */
    void setMargin(int margin);

    /*!
    * Sets the default read-only attribute of cells in the table.
    * See the \link #readOnly "readOnly" \endlink
    * property for details.
    * \sa readOnly()
    */
    void setReadOnly(bool b);

    /*!
    * Sets the default cell foreground color.
    * See the \link #foregroundColor "foregroundColor" \endlink
    * property for details.
    * \sa foregroundColor()
    */
    void setForegroundColor(const QColor &p);

    /*!
    * Sets the default cell background color.
    * See the \link #backgroundColor "backgroundColor" \endlink
    * property for details.
    * \sa backgroundColor()
    */
    void setBackgroundColor(const QColor &p);

    /*!
    * Sets the default cell selected foreground color.
    * See the \link #selectedForegroundColor "selectedForegroundColor" \endlink
    * property for details.
    * \sa selectedForegroundColor()
    */
    void setSelectedForegroundColor(const QColor &p);

    /*!
    * Sets the default cell selected background color.
    * See the \link #selectedBackgroundColor "selectedBackgroundColor" \endlink
    * property for details.
    * \sa selectedBackgroundColor()
    */
    void setSelectedBackgroundColor(const QColor &p);

    /*!
    * Sets the default cell edit foreground color.
    * See the \link #editForegroundColor "editForegroundColor" \endlink
    * property for details.
    * \sa editForegroundColor()
    */
    void setEditForegroundColor(const QColor &p);

    /*!
    * Sets the default cell edit background color.
    * See the \link #editBackgroundColor "editBackgroundColor" \endlink
    * property for details.
    * \sa editBackgroundColor()
    */
    void setEditBackgroundColor(const QColor &p);

    /*!
    * Sets the default cell pixmap.
    * See the \link #pixmap "pixmap" \endlink
    * property for details.
    * \sa pixmap()
    */
    void setPixmap(const QPixmap &p);

    /*!
    * Sets the default cell pixmap from a file.
    * See the \link #pixmap "pixmap" \endlink
    * property for details.
    * \sa pixmap()
    */
    void setPixmap(const QString &file_name);

    /*!
    * Sets the cell's pixmap spacing.
    * See the \link #pixmapSpacing "pixmapSpacing" \endlink property for details.
    * \sa pixmap()
    */
    void setPixmapSpacing(int sp);

    /*!
    * Sets the default QicsCellDisplay object. The cell display object
    * controls aspects of displaying and editing of cells.  See QicsCellDisplay
    * and its subclasses for details.
    * \sa displayer()
    */
    void setDisplayer(QicsCellDisplay *d);

    /*!
    * Sets the default QicsDataItemFormatter object.
    * The formatter object controls the formatting of data values.
    * See QicsDataItemFormatter and its subclasses for details.
    * \sa formatter()
    */
    void setFormatter(QicsDataItemFormatter *d);

    /*!
    * Sets the default cell alignment options.
    * See the \link #alignment "alignment" \endlink
    * property for details.
    * \sa Qt::Alignment
    * \sa alignment()
    */
    void setAlignment(int flags);

    /*!
    * Sets the default cell text flags attribute.
    * See the \link #textFlags "textFlags" \endlink
    * property for details.
    * \sa Qt::TextFlags
    * \sa textFlags()
    */
    void setTextFlags(int flags);

    /*!
    * Sets the default cell validator object.
    * This validator will be used when cells are edited.
    * See the \link #validator "validator" \endlink
    * property for details.
    * \sa validator()
    */
    void setValidator(QValidator *v);

    /*!
    * Sets the paste validator object for this cell.
    * See the \link #pasteValidator "pasteValidator" \endlink
    * property for details.
    * \sa pasteValidator()
    */
    void setPasteValidator(QicsPasteValidator *v);

    /*!
    * Sets the default cell label attribute.
    * See the \link #label "label" \endlink
    * property for details.
    * \sa label()
    */
    void setLabel(const QString &label);

    /*!
    * Sets the default maximum cell value length.
    * See the \link #maxLength "maxLength" \endlink
    * property for details.
    * \sa maxLength()
    */
    void setMaxLength(int len);

    /*!
    * Sets the default cell border pen.
    * See the \link #topBorderPen "topBorderPen" \endlink
    * property for details.
    * \sa topBorderPen()
    */
    void setBorderPen(const QPen &pen);
    /*!
    * Sets the default cell top border pen.
    * See the \link #topBorderPen "borderPen" \endlink
    * property for details.
    * \sa topBorderPen()
    */
    void setTopBorderPen(const QPen &pen);
    /*!
    * Sets the default cell left border pen.
    * See the \link #topBorderPen "topBorderPen" \endlink
    * property for details.
    * \sa topBorderPen()
    */
    void setLeftBorderPen(const QPen &pen);
    /*!
    * Sets the default cell right border pen.
    * See the \link #topBorderPen "topBorderPen" \endlink
    * property for details.
    * \sa topBorderPen()
    */
    void setRightBorderPen(const QPen &pen);
    /*!
    * Sets the default cell bottom border pen.
    * See the \link #bottomBorderPen "borderPen" \endlink
    * property for details.
    * \sa bottomBorderPen()
    */
    void setBottomBorderPen(const QPen &pen);

    /////// Grid Attribute Sets

    /*!
    * Sets the viewport for the table.  Setting a viewport allows the programmer
    * to limit the table to showing a particular region of the table.  For example,
    * to only display rows 5-10 and columns 2-7, do the following:
    *
    * <tt>setViewport(QicsRegion(5, 2, 10, 7));</tt>
    *
    * The constants \b QicsLAST_ROW and \b QicsLAST_COLUMN can be used
    * when specifying the region.  For example, to only display rows 5-10,
    * do the following:
    *
    * setViewport(QicsRegion(5, 0, 10, QicsLAST_COLUMN));
    *
    * \param vp the area of the data model that is allowed to be
    *           displayed.
    * /sa viewport()
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
    * Controls the display of horizontal grid lines.
    * See the
    * \link #horizontalGridLinesVisible "horizontalGridLinesVisible" \endlink
    * property for details.
    * \sa horizontalGridLinesVisible()
    */
    void setHorizontalGridLinesVisible(bool b);

    /*!
    * Controls the display of vertical grid lines.
    * See the
    * \link #verticalGridLinesVisible "verticalGridLinesVisible" \endlink
    * property for details.
    * \sa verticalGridLinesVisible()
    */
    void setVerticalGridLinesVisible(bool b);

    /*!
    * Sets the value of the width of the table's horizontal grid lines.
    * See the \link #horizontalGridLineWidth "horizontalGridLineWidth" \endlink
    * property for details.
    * \sa horizontalGridLineWidth()
    */
    void setHorizontalGridLineWidth(int w);

    /*!
    * Sets the value of the width of the table's vertical grid lines.
    * See the \link #verticalGridLineWidth "verticalGridLineWidth" \endlink
    * property for details.
    * \sa verticalGridLineWidth()
    */
    void setVerticalGridLineWidth(int w);

    /*!
    * Sets the horizontal grid line style.
    * See the \link #horizontalGridLineStyle "horizontalGridLineStyle" \endlink
    * property for details.
    * \sa horizontalGridLineStyle()
    */
    void setHorizontalGridLineStyle(QicsLineStyle style);

    /*!
    * Sets the vertical grid line style.
    * See the \link #verticalGridLineStyle "verticalGridLineStyle" \endlink
    * property for details.
    * \sa verticalGridLineStyle()
    */
    void setVerticalGridLineStyle(QicsLineStyle style);

    /*!
    * Sets the pen used to draw the table's horizontal grid lines.
    * See the \link #horizontalGridLinePen "horizontalGridLinePen" \endlink
    * property for details.
    * \sa horizontalGridLinePen()
    */
    void setHorizontalGridLinePen(const QPen &pen);

    /*!
    * Sets the pen used to draw the table's vertical grid lines.
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
    * Sets the frame style of the table's main grid.
    * See the \link #frameStyle "frameStyle" \endlink
    * property for details.
    * \sa frameStyle()
    */
    void setFrameStyle(int style);

    /*!
    * Sets the frame line width of the table's main grid.
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
    * Sets selection style of the table.
    * See the \link #selectionStyle "selectionStyle" \endlink
    * property for details.
    * \sa  selectionStyle()
    */
    void setSelectionStyle(QicsSelectionStyle s);

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
    * for details
    * \sa clickToEdit()
    */
    void setClickToEdit(bool b);

    /*!
    * Sets whether the contents of a cell are automatically selected
    * when the table begins a cell edit operation.
    * See the \link #autoSelectCellContents "autoSelectCellContents" \endlink
    * property for details
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
    * property for details
    * \sa enterTraversalDirection()
    */
    void setEnterTraversalDirection(Qt::Orientation dir);

    /*!
    * Sets the direction that the table will traverse when the user
    * types \a Tab or \a Shift-Tab.
    * See the \link #tabTraversalDirection "tabTraversalDirection" \endlink
    * property for details
    * \sa tabTraversalDirection()
    */
    void setTabTraversalDirection(Qt::Orientation dir);

    /*!
    * Sets traversal cell behavior (for Tab key) when it achieves the end of row/column.
    * See the \link #tabTraverseToBegin "tabTraverseToBegin" \endlink
    * property for details.
    * \sa tabTraverseToBegin()
    */
    void setTabTraverseToBegin(bool traverse);

    /*!
    * Sets traversal cell behavior (for Enter key) when it achieves the end of row/column.
    * See the \link #enterTraverseToBegin "enterTraverseToBegin" \endlink property
    * for details.
    * \sa enterTraverseToBegin()
    */
    void setEnterTraverseToBegin(bool traverse);

    /*!
    * Sets the pixmap that is used in this grid to show that text
    * in a cell has been clipped (i.e. it could not completely fit in the cell).
    * \sa moreTextPixmap()
    */
    void setMoreTextPixmap(const QPixmap &pix);

    /*!
    * Sets whether the user is allowed to drag from this table.
    * See the \link #dragEnabled "dragEnabled" \endlink
    * property for details.
    * \sa setDragEnabled
    */
    void setDragEnabled(bool b);

    /*!
    * Set the layout direction for a grid
    * See the \link #layoutDirection "layoutDirection" \endlink
    * property for details.
    * \sa setLayoutDirection
    */
    void setLayoutDirection(Qt::LayoutDirection layoutDirection);

    /*!
    * Sets visibility of fill handle
    * \sa fillHandleVisible()
    */
    void setFillHandleVisible(bool b);

    /////// Table Attribute Sets

    // Headers
    /*!
    * Sets the value of the topHeaderVisible property.
    * See the \link #topHeaderVisible "topHeaderVisible" \endlink
    * property for details.
    * \sa topHeaderVisible()
    */
    void setTopHeaderVisible(bool);
    /*!
    * Sets the value of the bottomHeaderVisible property.
    * See the \link #bottomHeaderVisible "bottomHeaderVisible" \endlink
    * property for details.
    * \sa bottomHeaderVisible()
    */
    void setBottomHeaderVisible(bool);
    /*!
    * Sets the value of the leftHeaderVisible property.
    * See the \link #leftHeaderVisible "leftHeaderVisible" \endlink
    * property for details.
    * \sa leftHeaderVisible()
    */
    void setLeftHeaderVisible(bool);
    /*!
    * Sets the value of the rightHeaderVisible property.
    * See the \link #rightHeaderVisible "rightHeaderVisible" \endlink
    * property for details.
    * \sa rightHeaderVisible()
    */
    void setRightHeaderVisible(bool);

    /*!
    * Sets the margin of the table widget.
    * See the \link #tableMargin "tableMargin" \endlink
    * property for details.
    * \sa tableMargin()
    */
    void setTableMargin(int margin);

    /*!
    * Sets the spacing of the table widget.
    * See the \link #tableSpacing "tableSpacing" \endlink
    * property for details.
    * \sa tableSpacing()
    */
    void setTableSpacing(int spacing);

    /*!
    * Sets the spacing of the grids in the table widget.
    * See the \link #gridSpacing "gridSpacing" \endlink
    * property for details.
    * \sa gridSpacing()
    */
    void setGridSpacing(int spacing);

    /*!
    * Controls whether the first column of data in the table's data model
    * is used as row header data.
    * See the \link #rowHeaderUsesModel "rowHeaderUsesModel" \endlink
    * property for details.
    * \sa rowHeaderUsesModel()
    */
    void setRowHeaderUsesModel(bool b);

    /*!
    * Controls whether the first row of data in the table's data model
    * is used as column header data.
    * See the \link #columnHeaderUsesModel "columnHeaderUsesModel" \endlink
    * property for details.
    * \sa columnHeaderUsesModel()
    */
    void setColumnHeaderUsesModel(bool b);

    /*!
    * Controls whether the table should ignore size change signals from
    * the data model.
    * See the \link #ignoreModelSizeChanges "ignoreModelSizeChanges" \endlink
    * property for details.
    * \sa ignoreModelSizeChanges()
    */
    void setIgnoreModelSizeChanges(bool b);

    /*!
    * Controls whether the table should allow propagating changes from
    * editing cell to whole selection.
    * See the \link #allowPropagateChanges "allowPropagateChanges" \endlink
    * property for details.
    * \sa allowPropagateChanges()
    */
    void setAllowPropagateChanges(bool b);

    // Titles
    /*!
    * Sets the title widget placed at the top of the table.
    * \sa topTitleWidget()
    */
    void setTopTitleWidget(QWidget *w);

    /*!
    * Sets the title widget placed at the bottom of the table.
    * \sa bottomTitleWidget()
    */
    void setBottomTitleWidget(QWidget *w);

    /*!
    * Sets the title widget placed at the left of the table.
    * \sa leftTitleWidget()
    */
    void setLeftTitleWidget(QWidget *w);

    /*!
    * Sets the title widget placed at the right of the table.
    * \sa rightTitleWidget()
    */
    void setRightTitleWidget(QWidget *w);

    // Corners

    /*!
    * Sets the widget placed in the top left corner of the table
    * (above the left header and to the left of the top header) to \a w.
    * The widget must be a child of the table, and the table will
    * take control of \a w when this method is called.  If a widget
    * was previously being displayed in this spot, it will be destroyed.
    * \sa topLeftCornerWidget()
    */
    void setTopLeftCornerWidget(QWidget *w);

    /*!
    * Sets the widget placed in the top right corner of the table
    * (above the right header and to the right of the top header) to \a w.
    * The widget must be a child of the table, and the table will
    * take control of \a w when this method is called.  If a widget
    * was previously being displayed in this spot, it will be destroyed.
    * \sa topRightCornerWidget()
    */
    void setTopRightCornerWidget(QWidget *w);

    /*!
    * Sets the widget placed in the bottom left corner of the table
    * (below the left header and to the left of the bottom header) to \a w.
    * The widget must be a child of the table, and the table will
    * take control of \a w when this method is called.  If a widget
    * was previously being displayed in this spot, it will be destroyed.
    * \sa bottomLeftCornerWidget()
    */
    void setBottomLeftCornerWidget(QWidget *w);

    /*!
    * Sets the widget placed in the bottom right corner of the table
    * (below the right header and to the right of the bottom header) to \a w.
    * The widget must be a child of the table, and the table will
    * take control of \a w when this method is called.  If a widget
    * was previously being displayed in this spot, it will be destroyed.
    * \sa bottomRightCornerWidget()
    */
    void setBottomRightCornerWidget(QWidget *w);

    ////

    /*!
    * Sets default path for pixmaps to \a path. Pixmaps will be searched at this path on load.
    * \sa defaultImagePath();
    * \since 2.3
    */
    void setDefaultImagePath(const QString &path);
    /*!
    * Returns default path for pixmaps to \a path.
    * \sa setDefaultImagePath();
    * \since 2.3
    */
    const QString& defaultImagePath() const { return gridInfo().defaultImagePath(); }

    ////

    /*!
    * Sets the current cell of the table.  This method will modify
    * the table's selection list to contain only the specified cell.
    * \sa currentCell()
    */
    void setCurrentCell(int row, int col);

    /*!
    * Sets the current cell of the table.  This method will modify
    * the table's selection list to contain only the specified cell.
    * \sa currentCell()
    */
    void setCurrentCell(const QicsICell &cell);

    /*!
    * Scrolls the table \a num number of rows or columns in direction \a dir.
    */
    void scroll(Qics::QicsScrollDirection dir, int num);

    /*!
    * Freezes the first \a num_rows rows in the table's current viewport.
    * These rows will be displayed "frozen" at the top of the table.
    * If \a num_rows is 0, any frozen top rows will be unfrozen.
    *
    * \sa unfreezeTopRows()
    */
    void freezeTopRows(int num_rows);

    /*!
    * Freezes the last \a num_rows rows in the table's current viewport.
    * These rows will be displayed "frozen" at the bottom of the table.
    * If \a num_rows is 0, any frozen bottom rows will be unfrozen.
    *
    * \sa unfreezeBottomRows()
    */
    void freezeBottomRows(int num_rows);

    /*!
    * Unfreezes any rows that have been frozen at the top of the table.
    *
    * \sa freezeTopRows()
    */
    void unfreezeTopRows();

    /*!
    * void any rows that have been frozen at the bottom of the table.
    *
    * \sa freezeBottomRows()
    */
    void unfreezeBottomRows();

    /*!
    * Freezes the first \a num_cols columns in the table's current viewport.
    * These rows will be displayed "frozen" on the left side of the table.
    * If \a num_rows is 0, any frozen left columns will be unfrozen.
    *
    * \sa unfreezeLeftColumns()
    */
    void freezeLeftColumns(int num_cols);

    /*!
    * Freezes the last \a num_cols columns in the table's current viewport.
    * These rows will be displayed "frozen" on the right side of the table.
    * If \a num_rows is 0, any frozen right columns will be unfrozen.
    *
    * \sa unfreezeRightColumns()
    */
    void freezeRightColumns(int num_cols);

    /*!
    * Unfreezes any columns that have been frozen on the left side
    *  of the table.
    *
    * \sa freezeLeftColumns()
    */
    void unfreezeLeftColumns();

    /*!
    * Unfreezes any columns that have been frozen on the right side
    *  of the table.
    *
    * \sa freezeRightColumns()
    */
    void unfreezeRightColumns();

    /*!
    * Returns number of top frozen rows.
    * \since 2.3
    */
    inline int numFreezeTopRows () const {return m_topFrozenRows;}
    /*!
    * Returns number of bottom frozen rows.
    * \since 2.3
    */
    inline int numFreezeBottomRows () const {return m_bottomFrozenRows;}
    /*!
    * Returns number of right frozen columns.
    * \since 2.3
    */
    inline int numFreezeRightColumns() const {return m_rightFrozenColumns;}
    /*!
    * Returns number of  left frozen columns.
    * \since 2.3
    */
    inline int numFreezeLeftColumns() const {return m_leftFrozenColumns;}
    /*!
    * Set the space between main and frozen tables.
    * \since 2.4
    */
    inline void setFrozenLineWidth(int width) {m_frozenLineWidth = width;}
    /*!
    * Returns the space between main and frozen tables.
    * \since 2.4
    */
        inline int frozenLineWidth() const {return m_frozenLineWidth;}

    /*!
    * Expands the height of row \a row to the number of pixels necessary
    * to completely display all items in the row.  Calling this method
    * a second time will cause the row to revert to its previous height
    * or does nothing (depends from autofit mode).
    */
    void toggleRowHeightExpansion(int row, bool setOverrideCursor = false);

    /*!
    * Expands the width of column \a col to the number of pixels necessary
    * to completely display all items in the column.  Calling this method
    * a second time will cause the column to revert to its previous width
    * or does nothing (depends from autofit mode).
    */
    void toggleColumnWidthExpansion(int col, bool setOverrideCursor = false);

    /*!
    * Scrolls the table so that the top displayed row is \a row.
    * If visible area contains all rows, function do nothing
    * See the \link #topRow "topRow" \endlink
    * property for details.
    * \sa topRow()
    */
    void setTopRow(int row);
    /*!
    * Scrolls the table so that the leftmost displayed column is \a col.
    * If visible area contains all columns, function do nothing
    * See the \link #leftColumn "leftColumn" \endlink
    * property for details.
    * \sa leftColumn()
    */
    void setLeftColumn(int col);

    /*!
    * Sets the repaint behavior of the table widget.
    * See the \link #repaintBehavior "repaintBehavior" \endlink
    * property for details.
    * \sa repaintBehavior()
    */
    void setRepaintBehavior(QicsRepaintBehavior r);

    /*!
    * Sets the number of visible rows in the main grid area of the table.
    * This value does not include any frozen rows that may be visible.
    * See the \link #visibleRows "visibleRows" \endlink
    * property for details.
    * \sa visibleRows()
    */
    void setVisibleRows(int num);
    /*!
    * Sets the number of visible columns in the main grid area of the table.
    * This value does not include any frozen columns that may be visible.
    * See the \link #visibleColumns "visibleColumns" \endlink
    * property for details.
    * \sa visibleColumns()
    */
    void setVisibleColumns(int num);

    /*!
    * Traverse to cell (\a row, \a col ).  This will cause the
    * specified cell to become visible in the main grid, if it is not
    * already.
    */
    bool traverseToCell(int row, int col);

    /*!
    * Moves the current cell to the top-left cell in the table's
    * current viewport.
    */
    void traverseToBeginningOfTable();
    /*!
    * Moves the current cell to the bottom-right cell in the table's
    * current viewport.
    */
    void traverseToEndOfTable();
    /*!
    * Moves the current cell to the leftmost cell of the current row
    * in the table's current viewport.
    */
    void traverseToBeginningOfRow();
    /*!
    * Moves the current cell to the rightmost cell of the current row
    * in the table's current viewport.
    */
    void traverseToEndOfRow();

    /*!
    * Moves the current cell to the topmost cell of the current column
    * in the table's current viewport.
    */
    void traverseToBeginningOfColumn();

    /*!
    * Moves the current cell to the bottommost cell of the current column
    * in the table's current viewport.
    */
    void traverseToEndOfColumn();

    /*!
    * Moves the current cell to the nearest cell to the left of the
    * current cell that is enabled.
    */
    void traverseLeft();
    /*!
    * Moves the current cell to the nearest cell to the right of the
    * current cell that is enabled.
    */
    void traverseRight();
    /*!
    * Moves the current cell to the nearest cell above the
    * current cell that is enabled.
    */
    void traverseUp();
    /*!
    * Moves the current cell to the nearest cell below the
    * current cell that is enabled.
    */
    void traverseDown();

    /*!
    * Moves the current cell to one page up.
    */
    void traversePageUp();
    /*!
    * Moves the current cell to one page down.
    */
    void traversePageDown();

    /*!
    * Delete a column from the table
    */
    void deleteColumn(int column);
    /*!
    * Deletes \a num columns, starting at \a start_position.
    */
    void deleteColumns(int num, int start_position);
    /*!
    * Append a column or more to the table
    */
    void addColumns(int howMany);
    /*!
    * Insert a column into the table before \a column
    */
    void insertColumn(int column);
    /*!
    * Delete a row from the table
    */
    void deleteRow(int row);
    /*!
    * Deletes \a num rows, starting at \a start_position.
    */
    void deleteRows(int num, int start_position);
    /*!
    * Append a row or more to the table
    */
    void addRows(int rows);
    /*!
    * Insert a row into the table before \a row
    */
    void insertRow(int row);

#ifndef QICSTABLE_GPL
    /*!
    * Prints the table to the specified printer device.
    */
    virtual void print(QPrinter *printer);
    /*!
    * Prints the region of the table specified in \a region  to
    * the specified printer device.
    */
    virtual void print(QPrinter *printer, const QicsRegion &region);
#endif

    /*!
    * Sets clipboard delegate to \a Delegate. Passing 0 will remove the delegate.
    * \sa clipboardDelegate()
    * \since 2.3
    */
    inline void setClipboardDelegate(QicsAbstractClipboardDelegate *delegate) {myClipboardDelegate = delegate;}

    /*!
    * Returns pointer to the current clipboard delegate, or 0 if no delegate installed.
    * \sa setClipboardDelegate()
    * \since 2.3
    */
    inline QicsAbstractClipboardDelegate* clipboardDelegate() const {return myClipboardDelegate;}

    /*!
    * Performs a cut operation.  The data in the current selection list
    * is placed in the application's clipboard, and the cells in the
    * current selection list are cleared.
    */
    virtual void cut();

    /*!
    * Performs a copy operation.  The data in the current selection list
    * is placed in the application's clipboard.
    */
    virtual void copy();

    /*!
    * Performs a paste operation.  This method attempts to place data
    * from the application's clipboard in the data model specified in this
    * object.
    */
    virtual void paste();

    /*!
    * Sets the default row height to \a height pixels
    * for the entire table.
    */
    void setDefaultRowHeightInPixels(int height);

    /*!
    * Returns default \a height (in pixels) for the entire table.
    */
    inline int defaultRowHeightInPixels() const { return gridInfo().dimensionManager()->defaultRowHeightInPixels(); }

    /*!
    * Sets the default row height to \a height character units (i.e. lines)
    * for the entire table.
    */
    void setDefaultRowHeightInChars(int height);
    /*!
    * Returns default \a height (in character unit) for the entire table.
    */
    inline int defaultRowHeightInChars() const { return dimensionManager()->defaultRowHeightInChars(); }

    /*!
    * Sets the default column width to \a width pixels
    * for the entire table.
    */
    void setDefaultColumnWidthInPixels(int width);
    /*!
    * Returns default column \a width in pixels.
    */
    inline int defaultColumnWidthInPixels() const { return dimensionManager()->defaultColumnWidthInPixels(); }

    /*!
    * Sets the default column width to \a width character units
    * for the entire table.
    */
    void setDefaultColumnWidthInChars(int width);
    /*!
    * Returns default column width in chars.
    */
    inline int defaultColumnWidthInChars() const { return dimensionManager()->defaultColumnWidthInChars(); }

    /*!
    * Sets the behavior of auto fitting cell width to \a mode.
    * \arg \b ChangeOnlyHeight - fit only Height
    * \arg \b ChangeHeightWidth - fit Height and Widhth
    * \since 2.3.1
    * \sa QicsCellWidthMode
    */
    void setCellWidthMode(Qics::QicsCellWidthMode mode);
    /*!
    * Returns current cell width mode.
    * \since 2.3.1
    */
    inline Qics::QicsCellWidthMode cellWidthMode() const { return dimensionManager()->cellWidthMode(); }

    /*!
    * Enables (\b allow = true) or disables (\b allow = false) table \link #QicsNavigator navigator \endlink.
    \sa isNavigatorAllowed()
    */
    void setNavigatorAllowed(bool allow);

    /*!
    * Repaints whole table.
    */
    void repaint();

    /*!
    * Selects all the cells in the main grid.
    * \since 2.2
    */
    void selectAll();

    /*!
    * Deletes data from selected cells.
    * \since 2.2
    */
    void deleteSelected();

    /*!
    * Selects entire row \a row. If \a exclusive is true, then previous selection is cleared.
    * \since 2.3
    */
    void selectRow(int row, bool exclusive = true);

    /*!
    * Selects entire column \a col. If \a exclusive is true, then previous selection is cleared.
    * \since 2.3
    */
    void selectColumn(int col, bool exclusive = true);

    /*!
    * Displays a simple message box about Qt
    * \since 2.4.2
    */
    static void aboutQicsTable();

public:
    /*!
    * Loads table attributes from \a xmlString.
    * \sa configureFromXmlFile(), configureFromXmlFile()
    */
    virtual void configureFromXml(const QString& xmlString);
    /*!
    * Loads table attributes from xml file \a fileName.
    * Returns true if configuration was successful, false otherwise.
    * \sa configureFromXml(), configurationToXml()
    * \since 2.3
    */
    virtual bool configureFromXmlFile(const QString& fileName);
    /*!
    * Stores table attributes to \a xmlString.
    * \sa configureFromXml(), configureFromXmlFile()
    */
    virtual QString configurationToXml();

    virtual void configureFromDomXml(const QDomElement& e);

    virtual QDomElement configurationToDomXml(QDomDocument* doc);

    /*! Sets cursor for entire table
    */
    void setCursor(const QCursor&);

signals:
    /*!
    * This signal is emitted when the user presses a mouse button
    * in the main grid of the table.  The indices of the cell,
    * the button that was pressed, and the position of the mouse pointer
    * in the table are passed as parameters.
    * \sa clicked(), doubleClicked()
    */
    void pressed(int row, int col, int button, const QPoint &pos);

    /*!
    * This signal is emitted when the user clicks (presses and releases
    * in the same cell) a mouse button in the main grid of the table.
    * The indices of the cell, the button that was pressed, and the position
    * of the mouse pointer in the table are passed as parameters.
    * \sa pressed(), doubleClicked()
    */
    void clicked(int row, int col, int button, const QPoint &pos);

    /*!
    * This signal is emitted when the user double clicks a mouse button
    * in the main grid of the table.  The indices of the cell, the button
    * that was pressed, and the position of the mouse pointer in the table
    * are passed as parameters.
    * \sa clicked()
    */
    void doubleClicked(int row, int col, int button, const QPoint &pos);

    /*!
    * This signal is emitted when the current cell of the table changes.
    * The cell (\a new_row, \a new_col ) is the new current cell
    * (in \b visual coordinates ).
    * \sa currentCell()
    */
    void currentCellChanged(int new_row, int new_col);

    /*!
    * This signal is emitted when the selection list of the table changes.
    * The parameter \a in_progress signifies if the selection action that
    * caused the change is still in progress.
    *
    * \sa selectionList()
    */
    void selectionListChanged(bool in_progress);

    /*!
    * This signal is emitted when the value of a cell in the main grid
    * changes.  The cell (\a row, \a col ) is expressed in \b visual
    * coordinates.
    */
    void valueChanged(int row, int col);

    /*!
    * Signal emitted when the row selection changes. The \b row parameter is
    * row index, \b selected is \b true when row was selected and \b false
    * when row was deselected.
    */
    void rowSelected(int row,bool selected);


    /*!
    * Signal emitted when the column selection changes. The \b col parameter is
    * column index, \b selected is \b true when column was selected and \b false
    * when column was deselected.
    */
    void columnSelected(int col,bool selected);

    /*!
    * This signal is emitted whenever a row dimension has changed
    * in the dimension manager.
    */
    void rowResized(int row,int old_height,int new_height);

    /*!
    * This signal is emitted whenever a column dimension has changed
    * in the dimension manager.
    */
    void columnResized(int col,int old_width,int new_width);

    /*!
    * This signal is emitted whenever a row header width has changed.
    * If header has several rows then old_width and
    * new_width are only valid for the top row.
    */
    void rowHeaderResized(int old_width,int new_width);

    /*!
    * This signal is emitted whenever a column header height has changed.
    * If header has several columns then old_height and
    * new_height are only valid for the left column.
    */
    void columnHeaderResized(int old_height,int new_height);

    /*!
    * This signal is emitted whenever a row visibility is changed.
    * @row parameter is row index.
    * @visible parameter holds whether the row is visible
    */
    void rowVisibilityChanged(int row, bool visible);

    /*!
    * This signal is emitted whenever a column visibility is changed.
    * @column parameter is column index.
    * @visible parameter holds whether the column is visible
    */
    void columnVisibilityChanged(int column, bool visible);

    /*!
    * This signal is emitted when cell edit just've been started.
    */
    void cellEditStarted(int row, int col);

    /*!
    * This signal is emitted when cell edit ended.
    */
    void cellEditEnded(int row, int col);

    /*!
    * This signal is emitted when rows moved.
    * See \b moveRows() for more information.
    */

    void rowsMoved(int target, const QVector<int> &rows);

    /*!
    * This signal is emitted when columns moved.
    * See \b moveColumns() method for more information.
    */

    void columnsMoved(int target, const QVector<int> &columns);

    /*!
    * This signal is emitted when the table is in exclusive selection mode,
    * and fill handle was pressed by mouse.
    * @param m Mouse event object.
    * @param initialSel A pointer to initial selection.
    * \since 2.3
    */
    void fillHandlePressed(QMouseEvent *m, const QicsSelection *initialSel);
    /*!
    * This signal is emitted when the table is in exclusive selection mode,
    * and fill handle was released (after drag operation).
    * @param m Mouse event object.
    * @param initialSel A pointer to initial selection.
    * @param currentSel A pointer to current selection (expanded while dragging).
    * \since 2.3
    */
    void fillHandleReleased(QMouseEvent *m, const QicsSelection *initialSel, const QicsSelection *currentSel);
    /*!
    * This signal is emitted when the table is in exclusive selection mode,
    * and fill handle was double-clicked.
    * @param m Mouse event object.
    * @param initialSel A pointer to initial selection.
    * \since 2.3
    */
    void fillHandleDoubleClicked(QMouseEvent *m, const QicsSelection *initialSel);
    /*!
    * This signal is emitted when the table is in exclusive selection mode,
    * and fill handle dragging is in progress.
    * @param m Mouse event object.
    * @param initialSel A pointer to initial selection.
    * @param currentSel A pointer to current selection (expanded while dragging).
    * \since 2.3
    */
    void fillHandleDragged(QMouseEvent *m, const QicsSelection *initialSel, const QicsSelection *currentSel);

    /*!
    * This signal is emitted when widget layout became deformed.
    * It is happens often when table has fixed size and columns
    * or rows are freezen. In this case summ of inner widgets sizes
    * can be more then size of QicsTable.
    * \b orientation is \b Qt::Vertical when layout deformed in
    * vertical direction(rows are freezen), and \b Qt::Horizontal
    * when layout deformed in horizontal direction(columns are freezen).
    */
    void layoutFailed(int orientation);

    void filterChanged(int index, bool set);

protected slots:
    /*!
    * \internal
    * Resets the repaint behavior of the table widget
    * to the state that it was in prior to the last setRepaintBehavior
    * call.
    */
    void revertRepaintBehavior();

    /*!
    * \internal
    * If button == LeftButton, toggles the row height or column width
    * expansion feature.
    * \sa toggleRowHeightExpansion(), toggleColumnWidthExpansion()
    */
    void handleHeaderDoubleClick(int idx, int button, Qics::QicsHeaderType type);

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
    * Converts model coordinates from data model signal to
    * visual coordinates and emits signal valueChanged()
    */
    void modelToVisualValueChanged(int row, int col);

    /*!
    * \internal
    * Tracks change of current cell coordinates improving performance of
    * currentCell() function
    */
    void handleCurrentCellChange(int row, int col);

    /*!
    * \internal
    * Resizes row header horizontally and column header vertically.
    */
    void resizeHeader(int val, int pos, Qics::QicsHeaderType type);

    /*!
    * \internal
    * Configure the table according to frozen row/column settings.
    */
    void configureFrozen();

    void handleFilterChanged(int index, bool set);

    ////////////////////////////

    void checkFrozenRegCorner(int FrozenColumns,int FrozenRows,QicsTableGrid* &GRID,
        int GRID_IDX1,int GRID_IDX2,
        QicsGridGeometry::TableRegion region);


    void checkFrozenRegMiddle_T(int FrozenRows,QicsHeaderGrid* &HEADER,bool HeaderVisible,
        Qics::QicsHeaderType hederType,
        int IDX1,int IDX2,int Align,
        QicsGridGeometry::HeaderPosition position);


    void checkFrozenRegMiddle_M(int FrozenRows,QicsTableGrid* &GRID,
        int IDX1,int IDX2,
        QicsGridGeometry::TableRegion region,
        bool control);

    void configResizeConnects(QicsHeaderGrid* &HEADER,QicsTableGrid* &GRID1,
        QicsTableGrid* &GRID2,QicsTableGrid* &GRID3);

    void confConn (bool Visible,QicsHeaderGrid* &HEADER,Qics::QicsHeaderType hederType,
        int IDX1,int IDX2,int Align,
        QicsGridGeometry::HeaderPosition position,bool control);

    void constrainFrozen (int FrozenRows,int IDX,bool vr,bool vr2);

    void constrainFrozen_H (int FrozenRows,int IDX,bool vr,bool vr2);

    /*!
    * \internal
    */
    void configureFrozen(Qics::QicsIndexType indexType, int startIndex, int endIndex);

    void focusNextPrevGrid(const QicsScreenGrid*,bool next);

    /*!
    * \internal
    * Store global table properties to \a doc.
    */
    QDomElement propertiesToDomXml(QDomDocument* doc) const;
    /*!
    * \internal
    * Load global table properties from \a e.
    */
    void propertiesFromDomXml(const QDomElement& e, QicsTable * table);

    /*!
    *\internal
    * Show navigator according current table size.
    */
    void showNavigator();
    /*!
    *\internal
    * Navigate table to rect.left() and rect.top()
    */
    void navigate(const QRect& rect);

    /*!
    * \internal
    * Handles global key press \a event (such as Delete etc.).
    * \since 2.2
    */
    void handleWideKeyPressed(QKeyEvent *event);

    /*!
    * \internal
    * Handles fill handle events in exclusive selection mode.
    * \since 2.3
    */
    void handleFillHandleEvent(QicsFillHandleEventType type,
        QMouseEvent *event, const QicsSelection *initialSel, const QicsSelection *currentSel);

    /*!
    * \internal
    * Handles selection changes.
    * \since 2.3
    */
    void handleSelectionListChanged(bool progress);

protected:
    void init(QicsDataModel *model,
        QicsDataModel::Foundry rhdmf,
        QicsDataModel::Foundry chdmf,
        QicsDataModel::FoundryWithKey rhdmf_wk,
        QicsDataModel::FoundryWithKey chdmf_wk,
        QicsTableGrid::Foundry tf,
        QicsHeaderGrid::Foundry hf,
        QicsTableGrid::FoundryWithKey tf_wk,
        QicsHeaderGrid::FoundryWithKey hf_wk,
        void *key);

    virtual void initDataModels(QicsDataModel *dm);
    virtual void initObjects();
    virtual void initGridInfoObjects();

    inline QicsGridInfo &gridInfo() const {return m_tableCommon->gridInfo();}

    inline QicsGridInfo &rhGridInfo() const {return m_tableCommon->rhGridInfo();}

    inline QicsGridInfo &chGridInfo() const {return m_tableCommon->chGridInfo();}

    inline QicsStyleManager *styleManager() const {return gridInfo().styleManager();}

    inline QicsStyleManager *rhStyleManager() const {return rhGridInfo().styleManager();}

    inline QicsStyleManager *chStyleManager() const {return chGridInfo().styleManager();}

    inline void setStyleManager(QicsStyleManager *sm) {gridInfo().setStyleManager(sm); }

    inline void setRHStyleManager(QicsStyleManager *sm) {rhGridInfo().setStyleManager(sm);}

    inline void setCHStyleManager(QicsStyleManager *sm) {chGridInfo().setStyleManager(sm);}

    inline QicsDimensionManager *dimensionManager() const {return gridInfo().dimensionManager();}

    inline QicsDimensionManager *rhDimensionManager() const {return rhGridInfo().dimensionManager();}

    inline QicsDimensionManager *chDimensionManager() const {return chGridInfo().dimensionManager();}

    inline void setDimensionManager(QicsDimensionManager *sm) {gridInfo().setDimensionManager(sm);}

    inline void setRHDimensionManager(QicsDimensionManager *sm) {rhGridInfo().setDimensionManager(sm);}

    inline void setCHDimensionManager(QicsDimensionManager *sm) {chGridInfo().setDimensionManager(sm);}

    inline QicsGridLayout *gridLayout() const {return m_gridLayout;}

    virtual bool event ( QEvent * event );

    virtual bool eventFilter (QObject*,QEvent *);

    void connectGrid(QicsTableGrid* grid,QicsHeaderGrid* header);

    /*!
    * \internal
    * Sets the viewport of the main grid.
    */
    virtual void setMainGridViewport(const QicsRegion &vp);

    /*!
    * \internal
    * Creates the display widgets
    */
    void initDisplay();

    /*!
    * \internal
    * Creates a grid widget and stores it in location
    * [\a grid_row ][\a grid_col ] in the grid array.
    */
    QicsTableGrid *createGrid(int grid_row, int grid_col);

    /*!
    * \internal
    * Creates a header grid object, stores it in the grid array, and adds
    * it to the grid layout object.
    * \param type type of header to create
    * \param grid_row row location of the grid in the grid array
    * \param grid_col column location of the grid in the grid array
    * \param alignment alignment of the grid in the grid layout object
    */
    QicsHeaderGrid *createHeader(Qics::QicsHeaderType type,
        int grid_row, int grid_col,
        int alignment = 0);


    void registerBuiltInCellDisplays();

    /*!
    * \internal
    * Checking for a layout deformation
    */
    void checkLayoutOverlap();

    /*!
    * \internal
    * common table info
    */
    QPointer<QicsTableCommon> m_tableCommon;

    /*!
    * \internal
    * the table's selection manager
    */
    QPointer<QicsSelectionManager> m_selectionManager;

    /*!
    * \internal
    * Qt grid object for layout
    * the master grid layout
    */
    QicsGridLayout *m_gridLayout;

    /*!
    * \internal
    * scroll manager object
    */
    QPointer<QicsScrollManager> m_scrollManager;

    /*!
    * \internal
    * row scroller
    */
    QPointer<QicsScroller> m_rowScroller;

    /*!
    * \internal
    * navigator through grid
    */
    QicsNavigator *m_navButton;

    /*!
    * \internal
    * column scroller
    */
    QPointer<QicsScroller> m_columnScroller;

    /*!
    * \internal
    * header grid foundry method
    */
    QicsHeaderGrid::Foundry myHeaderGridFoundry;

    /*!
    * \internal
    * table grid foundry method
    */
    QicsTableGrid::Foundry myTableGridFoundry;

    /*!
    * \internal
    * header grid foundry method for Python
    */
    QicsHeaderGrid::FoundryWithKey myHeaderGridFoundryWK;

    /*!
    * \internal
    * table grid foundry method for Python
    */
    QicsTableGrid::FoundryWithKey myTableGridFoundryWK;

    /*!
    * \internal
    * row header data model foundry method
    */
    QicsDataModel::Foundry myRowHeaderDMFoundry;

    /*!
    * \internal
    * column header data model foundry method
    */
    QicsDataModel::Foundry myColumnHeaderDMFoundry;

    /*!
    * \internal
    * row header data model foundry method for Python
    */
    QicsDataModel::FoundryWithKey myRowHeaderDMFoundryWK;

    /*!
    * \internal
    * column header data model foundry method for Python
    */
    QicsDataModel::FoundryWithKey myColumnHeaderDMFoundryWK;

    /*!
    * \internal
    * \brief all the grid widgets
    *
    * The main grid (mygrids[1][1]) is surrounded by other grids and headers
    * which look like this:
    *
    * \code
    *                  | myHHeaders[0][0] | myHHeaders[0][1] | myHHeaders[0][2] |
    * --------------------------------------------------------------------------------------------
    * myVHeaders[0][0] | myGrids[0][0]    | myGrids[0][1]    | myGrids[0][2]    | myVHeaders[0][1]
    * --------------------------------------------------------------------------------------------
    * myVHeaders[1][0] | myGrids[1][0]    | myGrids[1][1]    | myGrids[1][2]    | myVHeaders[1][1]
    * --------------------------------------------------------------------------------------------
    * myVHeaders[2][0] | myGrids[2][0]    | myGrids[2][1]    | myGrids[2][2]    | myVHeaders[2][1]
    * --------------------------------------------------------------------------------------------
    *                  | myHHeaders[1][0] | myHHeaders[1][1] | myHHeaders[1][2] |
    * \endcode
    *
    * Only the grids and headers that are needed (based on the current table settings
    * for header location and frozen rows and columns) are created and shown.
    */
    QicsTableGrid *myGrids[3][3];
    /*!
    * \internal
    * \brief horizontal header widgets
    *
    * See \link #myGrids myGrids \endlink for details on the widget layout.
    */
    QicsHeaderGrid *myHHeaders[2][3];
    /*!
    * \internal
    * \brief vertical header widgets
    *
    * See \link #myGrids myGrids \endlink for details on the widget layout.
    */
    QicsHeaderGrid *myVHeaders[3][2];

    /*!
    * \internal
    * number of currently frozen rows at the top of the table
    */
    int m_topFrozenRows;
    /*!
    * \internal
    * number of currently frozen rows at the bottom of the table
    */
    int m_bottomFrozenRows;
    /*!
    * \internal
    * number of currently frozen columns on the left of the table
    */
    int m_leftFrozenColumns;
    /*!
    * \internal
    * number of currently frozen columns on the right of the table
    */
    int m_rightFrozenColumns;

    /*!
    * \internal
    * user-supplied top title widget
    */
    QWidget *myTopTitleWidget;
    /*!
    * \internal
    * user-supplied bottom title widget
    */
    QWidget *myBottomTitleWidget;
    /*!
    * \internal
    * user-supplied left title widget
    */
    QWidget *myLeftTitleWidget;
    /*!
    * \internal
    * user-supplied right title widget
    */
    QWidget *myRightTitleWidget;

    /*!
    * \internal
    * user-supplied top left corner widget
    */
    QWidget *myTopLeftCornerWidget;
    /*!
    * \internal
    * user-supplied top right corner widget
    */
    QWidget *myTopRightCornerWidget;
    /*!
    * \internal
    * user-supplied bottom left corner widget
    */
    QWidget *myBottomLeftCornerWidget;
    /*!
    * \internal
    * user-supplied bottom right corner widget
    */
    QWidget *myBottomRightCornerWidget;

    /*!
    * \internal
    * user-supplied viewport for the table
    */
    QicsRegion myFullViewport;

    /*!
    * \internal
    * default cell display object
    */
    QicsCellDisplay *myDefaultCellDisplayer;

    QMap<QString, QicsCellDisplayFactory*> displayFactories;

    QList<QicsCellDisplay*> createdDisplays;

    /*!
    * \internal
    * foundry key value
    */
    void *myFoundryKey;

    /*!
    * \internal
    * Current cell for performance improve
    */
    QicsCell *myCurrentCell;

    bool myUnfreezingFlag;

    QicsGridGeometry myGridGeometry;

    QicsKeyboardManager myKeyboardManager;

    bool m_navAllowed;

    QicsCellRegion *myCellRegion;

    QSet<QicsWideKeyAction> myWideKeys;

    QicsAbstractClipboardDelegate *myClipboardDelegate;

    int m_frozenLineWidth;
};

#endif //QICSTABLE_H


