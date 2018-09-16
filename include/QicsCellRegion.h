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

#ifndef QICSCELLREGION_H
#define QICSCELLREGION_H

#include <QFont>
#include <QColor>
#include <QPen>

#include "QicsNamespace.h"
#include "QicsRegion.h"
#include "QicsCellStyle.h"

class QValidator;
class QicsStyleManager;
class QicsDimensionManager;
class QicsCellDisplay;
class QicsDataItemFormatter;
class QicsPasteValidator;
class QicsCell;
class QicsHeader;

/*!
* \class QicsCellRegion QicsCellRegion.h
* \brief Flyweight object for setting attributes for a region of cells.
*
* QicsCellRegion object can be obtained from table or header object using corresponding
* QicsTable::cellRegionRef() of QicsHeader::cellRegionRef() methods. Then it is possible to
* set new attributes for this region by calling appropriate QicsCellRegion's methods.
*
* Example of usage:
*
* \code
* table->cellRegionRef(4,10,100,200).setBackgroundColor(Qt::yellow);
* \endcode
*
* QicsCellRegion provides a wide set of methods that allow to change attributes of rectangular region of cells by one call.
* These methods are optimized for high performance and convenience.
* Especially effective when using with external attribute controller;
* see QicsAbstractAttributeController class.
*
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////


class QICS_EXPORT QicsCellRegion : public Qics
{
protected:
    /*!
    * Constructs a new cell region object.  The region extends from
    * row \a begin_row, column \a begin_col to
    * row \a end_row, column \a end_col.
    */
    QicsCellRegion(int begin_row, int begin_col, int end_row, int end_col);

    /*!
    * Constructs a new cell region object from /a region.
    */
    QicsCellRegion(const QicsRegion &region);

    /*!
    * Constructs a new invalid cell region object.
    */
    QicsCellRegion() {};

public:
    virtual ~QicsCellRegion() {};

    /*!
    Sets value \a val of attribute named \a prop in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after attributes are set.

    This method is provided for convenience and results in much more faster setting of cell attributes than
    calling appropriate methods for every cell explicitly.

    In general, you should probably use this method very rarely - as there are more specialized methods for setting
    attributes of region of cell like setColors(), setAlignment(), setFont() etc.

    \sa QicsTable::setRepaintBehavior()
    */
    inline void setRegionAttr(QicsCellStyle::QicsCellStyleProperty prop, const void *val);

    /*!
    Updates font of dimension manager in rectangular region of cells.
    */
    inline void updateFontInRegion();

    /*!
    Sets foreground (\a fc), background (\a bc), selected foreground (\a sfc), and selected background (\a sbc)
    colors in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after colors are set.

    Non-valid (default constructed) color values will be set to defaults.

    This method is provided for convenience and results in much more faster setting of colors than
    calling appropriate methods for every cell explicitly.

    If you want to set particular colors and do not affect others, use more specialized methods:
    setForegroundColor(), setBackgroundColor(),
    setSelectedForegroundColor(), setSelectedBackgroundColor(),
    setEditForegroundColor(), setEditBackgroundColor().
    */
    void setColors(const QColor &fc = QColor(), const QColor &bc = QColor(),
        const QColor &sfc = QColor(), const QColor &sbc = QColor());

    /*!
    Sets foreground color to \a color in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after colors are set.

    If region or color are invalid, nothing happens.

    This method is provided for convenience and results in much more faster setting of color than
    calling appropriate methods for every cell explicitly.

    \sa setColors(), setForegroundColor()
    */
    void setForegroundColor(const QColor &color);

    /*!
    Sets background color to \a color in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after colors are set.

    If region or color are invalid, nothing happens.

    This method is provided for convenience and results in much more faster setting of color than
    calling appropriate methods for every cell explicitly.

    \sa setColors(), setBackgroundColor()
    */
    void setBackgroundColor(const QColor &color);

    /*!
    Sets selected foreground color to \a color in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after colors are set.

    If region or color are invalid, nothing happens.

    This method is provided for convenience and results in much more faster setting of color than
    calling appropriate methods for every cell explicitly.

    \sa setColors(), setSelectedForegroundColor()
    */
    void setSelectedForegroundColor(const QColor &color);

    /*!
    Sets selected background color to \a color in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after colors are set.

    If region or color are invalid, nothing happens.

    This method is provided for convenience and results in much more faster setting of color than
    calling appropriate methods for every cell explicitly.

    \sa setColors(), setSelectedBackgroundColor()
    */
    void setSelectedBackgroundColor(const QColor &color);

    /*!
    Sets edit foreground color to \a color in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after colors are set.

    If region or color are invalid, nothing happens.

    This method is provided for convenience and results in much more faster setting of color than
    calling appropriate methods for every cell explicitly.

    \sa setColors(), setEditForegroundColor()
    */
    void setEditForegroundColor(const QColor &color);

    /*!
    Sets edit background color to \a color in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after colors are set.

    If region or color are invalid, nothing happens.

    This method is provided for convenience and results in much more faster setting of color than
    calling appropriate methods for every cell explicitly.

    \sa setColors(), setEditBackgroundColor()
    */
    void setEditBackgroundColor(const QColor &color);

    /*!
    Sets text alignment to \a align in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after alignment is set.

    This method is provided for convenience and results in much more faster setting of alignment than
    calling QicsCell::setAlignment() for every cell explicitly.

    \sa setAlignment()
    */
    inline void setAlignment(int align);

    /*!
    Sets text flags to \a flags in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after text flags are set.

    This method is provided for convenience and results in much more faster setting of text flags than
    calling QicsCell::setTextFlags() for every cell explicitly.

    \sa setTextFlags()
    */
    inline void setTextFlags(int flags);

    /*!
    Sets enable state to \a b in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after enable state is set.

    This method is provided for convenience and results in much more faster setting of enable state than
    calling QicsCell::setEnabled() for every cell explicitly.

    \sa QicsCell::setEnabled()
    */
    inline void setEnabled(bool b);

    /*!
    Sets read-only state to \a b in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after read-only state is set.

    This method is provided for convenience and results in much more faster setting of read-only state than
    calling QicsCell::setReadOnly() for every cell explicitly.

    \sa setReadOnly()
    */
    inline void setReadOnly(bool b);

    /*!
    Sets drop acception to \a b in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after drop acception is set.

    This method is provided for convenience and results in much more faster setting of drop acception than
    calling QicsCell::setCanAcceptDrops() for every cell explicitly.

    \sa QicsCell::setCanAcceptDrops()
    */
    inline void setCanAcceptDrops(bool b);

    /*!
    Sets auto recognition to \a b in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after auto recognition is set.

    This method is provided for convenience and results in much more faster setting of auto recognition than
    calling QicsCell::setAutoRecognition() for every cell explicitly.

    \sa QicsCell::setAutoRecognition()
    */
    inline void setAutoRecognition(bool b);

    /*!
    Sets maximum cell value length to \a value in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after length is set.

    This method is provided for convenience and results in much more faster setting of maximum cell value length than
    calling QicsCell::setMaxLength() for every cell explicitly.

    \sa setMaxLength()
    */
    inline void setMaxLength(int value);

    /*!
    Sets cell margin to \a value in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after margins are set.

    This method is provided for convenience and results in much more faster setting of cell margin than
    calling QicsCell::setMargin() for every cell explicitly.

    \sa setMargin()
    */
    inline void setMargin(int value);

    /*!
    Sets pixmap spacing to \a value in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after pixmap spacings are set.

    This method is provided for convenience and results in much more faster setting of pixmap spacing than
    calling QicsCell::setPixmapSpacing() for every cell explicitly.

    \sa setPixmapSpacing()
    */
    inline void setPixmapSpacing(int value);

    /*!
    Sets cell border width to \a value in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after border width is set.

    This method is provided for convenience and results in much more faster setting of cell border width than
    calling QicsCell::setBorderWidth() for every cell explicitly.

    \sa QicsCell::setBorderWidth()
    */
    inline void setBorderWidth(int value);

    /*!
    Sets cell font to \a font in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after font is set.

    If \a updateCellSizes is set to true, then dimensions of cells will be
    updated to match new font value.

    This method is provided for convenience and results in much more faster setting of cell font than
    calling QicsCell::setFont() for every cell explicitly.

    There are a couple of more specialized methods which allow to set individual font attributes
    without changing other ones. See setFontAttribute(), setFontSize(),
    setFontWeight(), setFontStretch() and setFontFamily().

    \sa QicsCell::setFont()
    */
    virtual void setFont(const QFont &font, bool updateCellSizes = true) = 0;

    /*!
    Sets font for selected cells to \a font in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after font is set.

    This method is provided for convenience and results in much more faster setting of cell font than
    calling QicsCell::setSelectedFont() for every cell explicitly.

    \sa QicsCell::setSelectedFont()
    \since 2.3
    */
    virtual void setSelectedFont(const QFont &font) = 0;

    /*!
    Sets boolean font attribute \a prop to \a value in rectangular region of cells in the grid.
    Other font attributes remain unchanged. The following attributes can be set with this method:
    - QicsCellStyle::FontBold
    - QicsCellStyle::FontItalic
    - QicsCellStyle::FontUnderline
    - QicsCellStyle::FontOverline
    - QicsCellStyle::FontStrikeOut
    - QicsCellStyle::FontFixed
    - QicsCellStyle::FontKerning

    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after font attribute are set.

    If \a updateCellSizes is set to true, then dimensions of cells will be
    updated to match new font value.

    This method is provided for convenience and results in much more faster setting of font attribute than
    calling QicsCell::setFont() for every cell explicitly.

    \sa setFont(), QicsCell::setFont()
    */
    void setFontAttribute(QicsCellStyle::QicsCellStyleProperty prop,
        bool value, bool updateCellSizes = true);

    /*!
    Sets font size to \a size in rectangular region of cells in the grid.
    Other font properties remain unchanged. Size is given in points (see QFont::setPointSize()).

    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after font sizes are set.

    If \a updateCellSizes is set to true, then dimensions of cells will be
    updated to match new font value.

    This method is provided for convenience and results in much more faster setting of font size than
    calling QicsCell::setFont() for every cell explicitly.

    \sa setFont(), QicsCell::setFont(), QFont::setPointSize()
    */
    void setFontSize(int size, bool updateCellSizes = true);

    /*!
    Sets font weight to \a weight in rectangular region of cells in the grid.
    Other font properties remain unchanged. Weight is given according to QFont::Weight enumeration
    (see QFont::setWeight()).

    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after font weights are set.

    If \a updateCellSizes is set to true, then dimensions of cells will be
    updated to match new font value.

    This method is provided for convenience and results in much more faster setting of font weight than
    calling QicsCell::setFont() for every cell explicitly.

    \sa setFont(), QicsCell::setFont(), QFont::setWeight()
    */
    void setFontWeight(int weight, bool updateCellSizes = true);

    /*!
    Sets font stretch factor to \a factor in rectangular region of cells in the grid.
    Other font properties remain unchanged. Stretch factor is given according to QFont::Stretch enumeration
    (see QFont::setStretch()).

    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after font stretch factor is set.

    If \a updateCellSizes is set to true, then dimensions of cells will be
    updated to match new font value.

    This method is provided for convenience and results in much more faster setting of font factor than
    calling QicsCell::setFont() for every cell explicitly.

    \sa setFont(), QicsCell::setFont(), QFont::setStretch()
    */
    void setFontStretch(int factor, bool updateCellSizes = true);

    /*!
    Sets font family to \a family in rectangular region of cells in the grid.
    Other font properties remain unchanged. Family is given according to QFont::setFamily() parameters.

    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after font family is set.

    If \a updateCellSizes is set to true, then dimensions of cells will be
    updated to match new font value.

    This method is provided for convenience and results in much more faster setting of font family than
    calling QicsCell::setFont() for every cell explicitly.

    \sa setFont(), QicsCell::setFont(), QFont::setFamily()
    */
    void setFontFamily(const QString &family, bool updateCellSizes = true);


    /*!
    Sets mouse cursor to \a cursor in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after cursor is set.

    This method is provided for convenience and results in much more faster setting of cursor than
    calling QicsCell::setCursor() for every cell explicitly.

    \sa setCursor()
    */
    inline void setCursor(const QCursor &cursor);

    // there is no sense to set region with pixmaps, labels, user data...
    // setLabel
    // setPixmap
    // setUserData


    /*!
    Sets top border pen to \a pen in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after top border pen is set.

    This method is provided for convenience and results in much more faster setting of pen than
    calling QicsCell::setTopBorderPen() for every cell explicitly.

    To set a pen for several borders around the region, see setBoxPen().

    \sa setTopBorderPen()
    */
    inline void setTopBorderPen(const QPen &pen);

    /*!
    Sets bottom border pen to \a pen in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after bottom border pen is set.

    This method is provided for convenience and results in much more faster setting of pen than
    calling QicsCell::setBottomBorderPen() for every cell explicitly.

    To set a pen for several borders around the region, see setBoxPen().

    \sa setBottomBorderPen()
    */
    inline void setBottomBorderPen(const QPen &pen);

    /*!
    Sets left border pen to \a pen in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after left border pen is set.

    This method is provided for convenience and results in much more faster setting of pen than
    calling QicsCell::setLeftBorderPen() for every cell explicitly.

    To set a pen for several borders around the region, see setBoxPen().

    \sa setLeftBorderPen()
    */
    inline void setLeftBorderPen(const QPen &pen);

    /*!
    Sets right border pen to \a pen in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after right border pen is set.

    This method is provided for convenience and results in much more faster setting of pen than
    calling QicsCell::setRightBorderPen() for every cell explicitly.

    To set a pen for several borders around the region, see setBoxPen().

    \sa setRightBorderPen()
    */
    inline void setRightBorderPen(const QPen &pen);

    /*!
    Sets border pen to \a pen in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after border pen is set.

    The main difference of methods like setRightBorderPen() and others is that those methods are setting
    pen for EVERY cell in the region. setBoxPen() allows to draw a box AROUND the region. It automatically
    decides which cells should be updated with a pen value, using combination of \a borders.
    By default, it is set to QicsBoxBorders::All, which results in making closed rectangle
    around the region.

    This method is provided for convenience and results in much more faster setting of pen than
    setting pen for every cell explicitly.

    \sa QicsBoxBorders
    */
    void setBoxPen(const QPen &pen, QicsBoxBorders borders = AllBorders);

    /*!
    Sets cell display object to \a d in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after display object is set.

    This method is provided for convenience and results in much more faster setting of QicsCellDisplay object than
    calling QicsCell::setDisplayer() for every cell explicitly.

    \sa setDisplayer()
    */
    inline void setDisplayer(QicsCellDisplay *d);

    /*!
    Sets formatter object to \a df in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after formatter is set.

    This method is provided for convenience and results in much more faster setting of QicsDataItemFormatter object than
    calling QicsCell::setFormatter() for every cell explicitly.

    \sa setFormatter()
    */
    inline void setFormatter(QicsDataItemFormatter *df);

    /*!
    Sets cell validator object to \a v in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after validator is set.

    This method is provided for convenience and results in much more faster setting of QValidator object than
    calling QicsCell::setValidator() for every cell explicitly.

    \sa setValidator()
    */
    inline void setValidator(QValidator *v);

    /*!
    Sets paste validator object to \a v in rectangular region of cells in the grid.
    If repaint behavior is Qics::QicsRepaintOn, then
    the grid will be redrawn automatically after paste validator is set.

    This method is provided for convenience and results in much more faster setting of QicsPasteValidator object than
    calling QicsCell::setPasteValidator() for every cell explicitly.

    \sa setPasteValidator()
    */
    inline void setPasteValidator(QicsPasteValidator *v);

protected:
    /*! \internal
    Updates font of dimension manager in rectangular \a region of cells.
    Region is given in visual coordinates.
    */
    virtual void updateFontInRegion(const QicsRegion &region) = 0;

    virtual void prepareFontInRegion(const QicsRegion &region) = 0;

    /*! \internal
    Sets value \a val of attribute named \a prop in rectangular \a region of cells in the grid.
    Region is given in visual coordinates.
    */
    virtual void setRegionAttr(const QicsRegion &region, QicsCellStyle::QicsCellStyleProperty prop, const void *val);

    /*! \internal
    Sets virtual attribute \a prop in a cell \a row, \a col to \a val.
    */
    void setVirtualCellAttr(int row, int col, QicsCellStyle::QicsCellStyleProperty prop, const void *val);

    /*! \internal
    Returns default palette.
    */
    virtual QPalette palette() = 0;

    /*! \internal
    */
    virtual QicsCell& cellRef(int r, int c) = 0;


    QicsRegion  myRegion;
    QicsGridType myGridType;
    QicsStyleManager        *mySM;
    QicsDimensionManager    *myDM;
    QVector<int> modelRows, modelCols;
    bool myInitCoords;
};


inline void QicsCellRegion::setRegionAttr(QicsCellStyle::QicsCellStyleProperty prop, const void *val)
{
    myInitCoords = true; setRegionAttr(myRegion, prop, val);
}

inline void QicsCellRegion::updateFontInRegion()
{
    updateFontInRegion(myRegion);
}

inline void QicsCellRegion::setAlignment(int align)
{
    setRegionAttr(myRegion, QicsCellStyle::Alignment, static_cast<const void *> (&align));
}

inline void QicsCellRegion::setTextFlags(int flags)
{
    setRegionAttr(myRegion, QicsCellStyle::TextFlags, static_cast<const void *> (&flags));
}

inline void QicsCellRegion::setEnabled(bool b)
{
    setRegionAttr(myRegion, QicsCellStyle::Enabled, static_cast<const void *> (&b));
}

inline void QicsCellRegion::setReadOnly(bool b)
{
    setRegionAttr(myRegion, QicsCellStyle::ReadOnly, static_cast<const void *> (&b));
}

inline void QicsCellRegion::setCanAcceptDrops(bool b)
{
    setRegionAttr(myRegion, QicsCellStyle::CanAcceptDrops, static_cast<const void *> (&b));
}

inline void QicsCellRegion::setAutoRecognition(bool b)
{
    setRegionAttr(myRegion, QicsCellStyle::AutoRecognition, static_cast<const void *> (&b));
}

inline void QicsCellRegion::setMaxLength(int value)
{
    setRegionAttr(myRegion, QicsCellStyle::MaxLength, static_cast<const void *> (&value));
}

inline void QicsCellRegion::setMargin(int value)
{
    setRegionAttr(myRegion, QicsCellStyle::CellMargin, static_cast<const void *> (&value));
}

inline void QicsCellRegion::setPixmapSpacing(int value)
{
    setRegionAttr(myRegion, QicsCellStyle::PixmapSpacing, static_cast<const void *> (&value));
}

inline void QicsCellRegion::setBorderWidth(int value)
{
    setRegionAttr(myRegion, QicsCellStyle::BorderWidth, static_cast<const void *> (&value));
}

inline void QicsCellRegion::setCursor(const QCursor &cursor)
{
    setRegionAttr(myRegion, QicsCellStyle::Cursor, static_cast<const void *> (&cursor));
}

inline void QicsCellRegion::setTopBorderPen(const QPen &pen)
{
    setRegionAttr(myRegion, QicsCellStyle::TopBorderPen, (pen.style() == Qt::NoPen) ? 0 : static_cast<const void *> (&pen));
}

inline void QicsCellRegion::setBottomBorderPen(const QPen &pen)
{
    setRegionAttr(myRegion, QicsCellStyle::BottomBorderPen, (pen.style() == Qt::NoPen) ? 0 : static_cast<const void *> (&pen));
}

inline void QicsCellRegion::setLeftBorderPen(const QPen &pen)
{
    setRegionAttr(myRegion, QicsCellStyle::LeftBorderPen, (pen.style() == Qt::NoPen) ? 0 : static_cast<const void *> (&pen));
}

inline void QicsCellRegion::setRightBorderPen(const QPen &pen)
{
    setRegionAttr(myRegion, QicsCellStyle::RightBorderPen, (pen.style() == Qt::NoPen) ? 0 : static_cast<const void *> (&pen));
}

inline void QicsCellRegion::setDisplayer(QicsCellDisplay *d)
{
    setRegionAttr(myRegion, QicsCellStyle::CellDisplayer, static_cast<const void *> (d));
}

inline void QicsCellRegion::setFormatter(QicsDataItemFormatter *df)
{
    setRegionAttr(myRegion, QicsCellStyle::Formatter, static_cast<const void *> (df));
}

inline void QicsCellRegion::setValidator(QValidator *v)
{
    setRegionAttr(myRegion, QicsCellStyle::Validator, static_cast<const void *> (v));
}

inline void QicsCellRegion::setPasteValidator(QicsPasteValidator *v)
{
    setRegionAttr(myRegion, QicsCellStyle::PasteValidator, static_cast<const void *> (v));
}


/*! \internal
* \class QicsCellRegionHeader QicsCellRegion.h
* \brief Flyweight object for setting attributes for a region of cells in the header grid.
*
*/

class QICS_EXPORT QicsCellRegionHeader : public QicsCellRegion
{
    friend class QicsHeader;
public:
    QicsCellRegionHeader(const QicsRegion &region, QicsHeader *header, QicsGridType gt, QicsStyleManager *sm, QicsDimensionManager *dm);
    virtual ~QicsCellRegionHeader() {};

    virtual void setFont(const QFont &font, bool updateCellSizes = true);
    virtual void setSelectedFont(const QFont &font);

protected:
    virtual void updateFontInRegion(const QicsRegion &region);
    virtual void prepareFontInRegion(const QicsRegion &region);

    virtual QPalette palette();
    virtual QicsCell& cellRef(int r, int c);

private:
    QicsHeader  *myHeader;
};


/*! \internal
* \class QicsCellRegionTable QicsCellRegion.h
* \brief Flyweight object for setting attributes for a region of cells in the table grid.
*
*/

class QICS_EXPORT QicsCellRegionTable : public QicsCellRegion
{
    friend class QicsTable;
public:
    QicsCellRegionTable(const QicsRegion &region, QicsTable *table, QicsGridType gt, QicsStyleManager *sm, QicsDimensionManager *dm);
    virtual ~QicsCellRegionTable() {};

    virtual void setFont(const QFont &font, bool updateCellSizes = true);
    virtual void setSelectedFont(const QFont &font);

protected:
    virtual void setRegionAttr(const QicsRegion &region, QicsCellStyle::QicsCellStyleProperty prop, const void *val);
    virtual void updateFontInRegion(const QicsRegion &region);
    virtual void prepareFontInRegion(const QicsRegion &region);

    virtual QPalette palette();
    virtual QicsCell& cellRef(int r, int c);

private:
    QicsTable   *myTable;
};

#endif //QICSCELLREGION_H


