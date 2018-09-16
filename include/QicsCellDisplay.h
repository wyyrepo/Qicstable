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

#ifndef QICSCELLDISPLAY_H
#define QICSCELLDISPLAY_H

#include <QWidget>
#include <QList>
#include <QPointer>
#include <QDomElement>
#include <QStyle>

#include "QicsNamespace.h"
#include "QicsDataItem.h"

class QKeyEvent;
class QMouseEvent;
class QPainter;
class QStyle;
class QicsGridInfo;
class QicsGrid;
class QicsScreenGrid;
class QicsCell;
class QicsRow;
class QicsColumn;
class QicsMainGrid;
class QicsCellDisplayData;

// The abstract base class for all cell display objects

/*!
* \class QicsCellDisplay QicsCellDisplay.h
* \brief Abstract class defining the interface for all cell display objects
*
* QicsCellDisplay is an abstract base class defining the interface that all
* cell display objects must implement.
*
* Please note that all row and column index parameters are given in \b visual
* coordinates.  If model coordinates are needed, use QicsGridInfo::modelRowIndex()
* and QicsGridInfo::modelColumnIndex().
*
* See \ref cell_display for more information on creating a cell display class.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////


class QICS_EXPORT QicsCellDisplay: public Qics
{
public:
    /*!
    * Constructor for the class.  No arguments are passed.
    */
    QicsCellDisplay();

    /*!
    * Destructor for the class.
    */
    virtual ~QicsCellDisplay();

    virtual QicsCellDisplay* clone() { return 0; }

    /*!
    * Displays the cell (\a row, \a col ) in grid widget \a grid.  The
    * cell will be displayed in region \a rect, using \a painter.
    * \param grid the controlling grid
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param itm the value to display.  This value may be 0.
    * \param rect the rectangle to paint into
    * \param painter a painting context
    */
    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter) = 0;

    /*!
    * Begin an edit operation in cell (\a row, \a col ).  This method is
    * intended to be used as a "preparation" for an edit operation.
    * It will be called by the grid in response to a user action or a
    * programmatic call.  Note that this method does not specify a location
    * in the grid widget for the edit operation to take place.  The location
    * will be specified in a subsequent call to #moveEdit().
    * \param grid the controlling grid
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param itm the value to edit.  This value may be 0.
    */
    virtual void startEdit(QicsScreenGrid *, int , int ,
        const QicsDataItem *){myIsEditing = true;}

    /*!
    * This method is called by the grid widget when the cell currently
    * being edited has changed location in the grid.  This would
    * most commonly occur when the table is scrolled while an edit
    * operation is underway.
    * \param grid the controlling grid
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param rect the new location of the cell
    */
    virtual void moveEdit(QicsScreenGrid *grid, int row, int col,
        const QRect &rect) = 0;


    virtual Qt::FocusReason lastFocusReason(){return myLastFocusReason;}

    /*!
    * This method is called by the grid widget when an edit operation
    * of a cell should be ended.  This would most commonly occur when
    * the user traverses out of the cell via keyboard or mouse action.
    * \param grid the controlling grid
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    */
    virtual void endEdit(QicsScreenGrid *, int , int )
        {myIsEditing = false;}

    /*!
    * Returns \b true if cell displayer is editing now, otherwise returns \b false.
    */
    inline bool isEditing() {return myIsEditing;}

    /*!
    * Informs that it is row displayer (which handles drawing of the whole row).
    * \sa setSpanned()
    * \since 2.4
    */
    virtual bool useWholeRow() const { return mySpanned; }

    /*!
    *  Spans/unspans all the cells according to \a span.
    *	\sa useWholeRow()
    *	\since 2.4
    */
    inline void setSpanned(bool span) { mySpanned = span;}

    /*!
    * Handler for mouse events that occur within cell (\a row, \a col ).
    * The handler must return \b true if the event was handled by the
    * cell displayer (and thus should not be handled by the grid), or \b false
    * otherwise.
    * \param grid the controlling grid
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param me the mouse event
    */
    virtual bool handleMouseEvent(QicsScreenGrid *grid, int row, int col,
        QMouseEvent *me);

    /*!
    * Handler for keyboard events that occur within cell (\a row, \a col ).
    * The handler must return \b true if the event was handled by the
    * cell displayer (and thus should not be handled by the grid), or \b false
    * otherwise.
    * \param grid the controlling grid
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param ke the keyboard event
    */
    virtual bool handleKeyEvent(QicsScreenGrid *grid, int row, int col,
        QKeyEvent *ke);

    /*!
    * Returns the size that cell (\a row, \a col ) needs to be if it
    * is to completely display \a itm.
    * \param grid the controlling grid
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param itm the value to display.  This value may be 0.
    */
    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm) = 0;

    /*!
    * Returns \b true if cells using this displayer should automatically be
    * put into edit mode when they become the current cell in the table.
    */
    virtual bool editWhenCurrent() const = 0;

    /*!
    * Returns \b true if this displayer should be notified when its cells
    * are no longer visible in the grid, \b false otherwise.
    */
    inline virtual bool needsVisibilityNotification() const { return false; }

    /*!
    * Returns \b true if the contents of cell (\a row, \a col ) (given data
    * item \a itm ) are empty, \b false otherwise.
    * \param info grid info object
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param itm item that will be displayed in the cell
    */
    virtual bool isEmpty(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const = 0;

    /*!
    * Returns the text that a grid widget can display in a tooltip when
    * the user hovers the mouse pointer over this cell.
    * \param info grid info object
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param itm item that will be displayed in the cell
    * \param rect size of the cell
    */
    virtual QString tooltipText(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm, const QRect &rect) const;

    /*!
    * Returns widget that cell displayer uses or '0' otherwise.
    */
    virtual QWidget* widget() {return 0;}

    static const QString BasicCellDisplayName;

    /*!
    * Returns displayer string name.
    */
    virtual QString cellDisplayName() const;

    /*!
    * Helper function. Save displayer state to QDomDocument.
    */
    virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc);

    /*!
    * Helper function. Restore displayer state from QDomElement.
    */
    virtual void configureFromDomXml(const QDomElement& e);

    /*!
    * Draws the borders of cell (\a row, \a col )  (based on cell
    * borders properties contained in \a info at the location specified by \a rect.
    * \param info grid info object
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param rect location to draw in
    * \param painter painter to draw with
    */
    virtual void drawCellBorders(QicsGridInfo *info, int row, int col,
        const QRect &rect, QPainter *painter);

    /*!
    * Returns the full rect of the content of the displayer.
    */
    virtual QRect contentRect(QicsGrid * /*grid*/, int /*row*/, int /*col*/,
        const QicsDataItem * /*itm*/,
        QRect &rect, QPainter * /*painter*/)
    { return QRect(0,0,rect.width(),rect.height()); }

    /*!
    * Returns the active look state. If true, cells will not be drawn differently even if disabled.
    * \since 2.4
    */
    inline bool isActiveLook() const {return myActiveLook;}

    /*!
    * Sets the active look state to \a on. If true, cells will not be drawn differently even if disabled.
    * \since 2.4
    */
    inline void setActiveLook(bool on) {myActiveLook = on;}

    /*!
    * Returns the default active look state. If true, cells will not be drawn differently even if disabled.
    * \since 2.4
    */
    static bool defaultActiveLook() {return active_look;}

    /*!
    * Sets the default active look state to \a on.
    * If \a on is true, cells will not be drawn differently even if disabled.
    * All the newly created cell displayers will be initialized with this default state of active look.
    * \since 2.4
    */
    static void setDefaultActiveLook(bool on) {active_look = on;}

    /*!
    * Called internally when the displayer is about to be cleared from the cell at \a row, \a col.
    * \since 2.4
    */
    virtual void aboutToClear(QicsGridInfo *info, int row, int col);

protected:
    /*!
    * Draws the background of cell (\a row, \a col ) using \a painter
    * (based on cell and grid properties contained \a info)
    * at the location specified by \a rect.
    * \param info grid info object
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param rect location to draw in
    * \param pal color group to draw with
    * \param painter painter to draw with
    * \param is_current is this cell the current cell?
    * \param is_selected is this cell selected?
    */
    virtual void drawBackground(QicsGridInfo *info, int row, int col,
        const QRect &rect, const QPalette &pal,
        QPainter *painter,
        bool is_current = false, bool is_selected = false);

    /*!
    * Draws the border of cell (\a row, \a col )  (based on cell and
    * grid properties contained in \a info at the location specified by \a rect.
    * \param info grid info object
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param rect location to draw in
    * \param pal color group to draw with
    * \param painter painter to draw with
    * \param is_current is this cell the current cell?
    * \param is_selected is this cell selected?
    */
    virtual void drawBorder(QicsGridInfo *info, int row, int col,
        const QRect &rect, const QPalette &pal,
        QPainter *painter,
        bool is_current = false, bool is_selected = false);

    /*!
    * Returns \b true if cell (\a row, \a col ) is selected in grids
    * described by \a info.  If the grid(s) are main table grids, this
    * method simply determines if the cell is contained in the current
    * selection list.  If the grid(s) are header grids, this method
    * determines if all the cells in the header cell's row or column
    * are selected.
    * \param info grid info object
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    */
    virtual bool isCellSelected(QicsGridInfo *info, int row, int col);

    /*!
    * Returns the palette to be used when drawing in cell (\a row, \a col ),
    * using grid information from \a info.  If
    * \a for_printer is \b true, the palette will have a white background color
    * substituted for any background colors that are the same as the default
    * background color of the grid.
    * \param info grid info object
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param for_printer will this palette be used to print?
    */
    virtual QPalette &cellPalette(bool enabled, bool for_printer = false);
    /*!
    * Returns a rectangle specifying the area of cell (\a row, \a col ) that can
    * be used to display information.  This rectangle is based on the full area
    * of the cell (contained in \a cr_full ), minus the area reserved for the cell's
    * border and margin.
    * \param info grid info object
    * \param row the row in \a visual coordinates
    * \param col the column in \a visual coordinates
    * \param cr_full the full area of the cell
    * \param consider_margin should the returned area exclude the margin of the cell?
    * \param consider_border should the returned area exclude the border of the cell?
    */
    virtual QRect displayableCellArea(QicsGridInfo *ginfo,
        int row, int col,
        const QRect &cr_full,
        bool consider_margin = true,
        bool consider_border = true) const;

    /*!
    * Returns the label string for cell (\a row, \a col ).
    *
    * This default method returns the value of the \b label property
    * of the cell.
    *
    * This method is intended to be reimplemented by subclasses
    * of QicsCheckCellDisplay.  The programmer should implement this
    * method so that it returns the appropriate value for the given
    * cell.
    *
    * \param info grid info object
    * \param row the \a visual row index of the cell
    * \param col the \a visual column index of the cell
    * \param itm the data item for this cell (this value may be 0)
    */
    virtual QString textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

    /*!
    * Returns the pixmap for cell (\a row, \a col ).
    *
    * This default method returns the pixmap that was set for this
    * cell via QicsCell::setPixmap , or 0 if no such pixmap exists.
    *
    * This method is intended to be reimplemented by subclasses
    * of QicsCheckCellDisplay.  The programmer should implement this
    * method so that it returns the appropriate value for the given
    * cell.  If no pixmap should be displayed, return the null pixmap.
    *
    * \param info grid info object
    * \param row the \a visual row index of the cell
    * \param col the \a visual column index of the cell
    * \param itm the data item for this cell (this value may be 0)
    */
    virtual QPixmap pixmapToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

protected:
    QPointer<QicsCell> myCell;
    QPointer<QicsCell> myEditCell;
    QPointer<QicsRow> myRow;
    QPointer<QicsColumn> myColumn;
    QPointer<QicsMainGrid> myGrid;
    QPointer<QStyle> myStyle;
    bool myIsEditing;
    bool myActiveLook;
    bool mySpanned;

    Qt::FocusReason myLastFocusReason;

    // default static stuff
    static bool active_look;

    // internal static stuff common for all widgets
    static QicsCellDisplayData *d;

    void commonInit(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter,
        QWidget *wdg,
        bool draw_bg,
        bool consider_frame);
};

////////////////////////////////////////////////////////////////////////////////////


/*!
* \class QicsNoWidgetCellDisplay QicsCellDisplay.h
* \brief Abstract class for cell display objects with a single entry widget
*
* QicsNoWidgetCellDisplay is an abstract class that implements portions
* of the interface
* defined by QicsCellDisplay.  QicsNoWidgetCellDisplay contains internal
* methods and data to help implement a cell display class that handles
* all aspects of the cell, including any editing and user interaction, as
* well as the actual drawing of cell contents.
*
* See \ref cell_display for more information on creating a cell display class.
*/

// An abstract class for all cell display objects that draw each cell,
// and use a single entry widget

class QICS_EXPORT QicsNoWidgetCellDisplay: public QicsCellDisplay
{
public:
    /*!
    * Constructor for the abstract class.  Should never be called by the programmer.
    * This is intended only for use by subclass constructors.
    */
    QicsNoWidgetCellDisplay();

    /*!
    * Destructor for the class.
    */
    virtual ~QicsNoWidgetCellDisplay();

    /*!
    * This method is a no-op in this abstract class.
    */
    virtual void startEdit(QicsScreenGrid *, int row, int col,
        const QicsDataItem *);
    /*!
    * This method is a no-op in this abstract class.
    */
    virtual void moveEdit(QicsScreenGrid *, int row, int col, const QRect &rect);

    /*!
    * This method is a no-op in this abstract class.
    */
    virtual void endEdit(QicsScreenGrid *grid, int row, int col);

    inline virtual bool editWhenCurrent() const {return false;}
};


/*!
* \class QicsMovableEntryWidgetCellDisplay QicsCellDisplay.h
* \brief Abstract class for cell display objects with a single entry widget
*
* QicsMovableEntryWidgetCellDisplay is an abstract class that implements portions
* of the interface defined by QicsCellDisplay.  QicsMovableEntryWidgetCellDisplay
* contains internal methods and data to help implement a cell display class that
* uses a single widget to facilitate data entry.  This widget is moved from cell
* to cell as the user edits different cells.
*
* When deriving from this class, you must implement all the necessary methods
* in QicsCellDisplay, as well as the #newEntryWidget() method of this class.
*
* See \ref cell_display for more information on creating a cell display class.
*/

// An abstract class for all cell display objects that draw each cell,
// and use a single entry widget

class QICS_EXPORT QicsMovableEntryWidgetCellDisplay: public QicsCellDisplay
{
public:
    /*!
    * Constructor for the abstract class.  Should never be called by the programmer.
    * This is intended only for use by subclass constructors.
    */
    QicsMovableEntryWidgetCellDisplay();

    /*!
    * Destructor for the class.
    */
    virtual ~QicsMovableEntryWidgetCellDisplay();

    virtual void moveEdit(QicsScreenGrid *, int row, int col, const QRect &rect);
    virtual void endEdit(QicsScreenGrid *, int row, int col);

    inline virtual bool editWhenCurrent() const { return true; }

protected:
    // A helper class for QicsMovableEntryWidgetCellDisplay

    /*!
    * \class QicsEntryWidgetInfo QicsCellDisplay.h
    * \brief A helper class for QicsMovableEntryWidgetCellDisplay
    *
    * A helper class for QicsMovableEntryWidgetCellDisplay.  An instance of this
    * class contains information about a single entry widget.  (A subclass
    * of QicsMovableEntryWidgetCellDisplay can have multiple entry widgets, one
    * for each grid in which it is used.  Information available includes the
    * entry widget, the grid widget associated with it, the current cell that
    * the entry widget is in, and the data item that the entry widget is editing.
    */
    class QicsEntryWidgetInfo
    {
    public:
        QicsEntryWidgetInfo() { myWidget = 0; myGrid = 0; myItem = 0;
        myRow = -1; myCol = -1; }
        ~QicsEntryWidgetInfo() { delete myItem; myItem = 0; }

        /*!
        * Returns the entry widget.
        */
        inline QWidget *widget() const { return myWidget; }
        /*!
        * Sets the entry widget.
        */
        inline void setWidget(QWidget *w) { myWidget = w; }
        /*!
        * Returns the grid widget.
        */
        inline QicsScreenGrid *grid() const { return myGrid; }
        /*!
        * Sets the grid widget.
        */
        inline void setGrid(QicsScreenGrid *grid) { myGrid = grid; }
        /*!
        * Returns the row index of the cell that the entry widget is in.
        */
        inline int row() const { return myRow; }
        /*!
        * Sets the row index of the cell that the entry widget is in.
        */
        inline void setRow(int row) { myRow = row; }
        /*!
        * Returns the column index of the cell that the entry widget is in.
        */
        inline int column() const { return myCol; }
        /*!
        * Sets the column index of the cell that the entry widget is in.
        */
        inline void setColumn(int col) { myCol = col; }
        /*!
        * Returns the item that is currently being edited by this entry widget.
        */
        inline QicsDataItem *item() const { return myItem; }
        /*!
        * Sets the item that is currently being edited by this entry widget.
        */
        inline void setItem(QicsDataItem *item) { if (myItem) delete myItem; myItem = item; }

    protected:
        QWidget *myWidget;
        QicsScreenGrid *myGrid;
        int myRow;
        int myCol;
        QicsDataItem *myItem;
    };

    typedef QMap<QicsScreenGrid *, QicsEntryWidgetInfo*> QicsEntryWidgetInfoPL;

    /*!
    * Creates a new entry widget as a child of \a grid.  When deriving from
    * this class, you should implement this method so that it returns
    * a new instance of the type of entry widget you want to use.
    */
    virtual QWidget *newEntryWidget(QicsScreenGrid *grid) = 0;
    /*!
    * Returns the entry widget information for the specified grid widget.
    */
    QicsEntryWidgetInfo *getInfoFromGrid(QicsScreenGrid *grid);
    /*!
    * Returns the entry widget information for the specified entry widget.
    */
    QicsEntryWidgetInfo *getInfoFromEntry(const QWidget *widget);

    QicsEntryWidgetInfo *takeInfoFromGrid(QicsScreenGrid *grid);

    /*!
    * Returns the rectangle which specifies the location and size
    * for the entry widget to be placed.
    *
    * \param ginfo grid info object
    * \param row the cell's row index
    * \param col the cell's column index
    * \param cell_rect the location and size of the cell to be edited
    */
    virtual QRect entryWidgetRect(QicsGridInfo *ginfo, int row, int col,
        QRect cell_rect);

    void celldisplay_init() {}

    /*!
    * \internal
    * the list of entry info objects
    */
    QicsEntryWidgetInfoPL myEntryList;
};


class QICS_EXPORT QicsCellDisplayConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    QicsCellDisplayConfigurationWidget(QWidget* parent=0) : QWidget(parent) {};
    virtual ~QicsCellDisplayConfigurationWidget() {};

    virtual void readConfiguration(QicsCellDisplay* cellDisplay) = 0;
    virtual void writeConfiguration(QicsCellDisplay* cellDisplay) = 0;

signals:
    void configurationChanged();
};


class QICS_EXPORT QicsCellDisplayFactory: public Qics
{
public:
    QicsCellDisplayFactory() {};
    virtual ~QicsCellDisplayFactory() {};
    virtual QicsCellDisplay* createCellDisplay() = 0;
    virtual QicsCellDisplayConfigurationWidget* configurationWidget() = 0;
};

#endif //QICSCELLDISPLAY_H


