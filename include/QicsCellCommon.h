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

#ifndef QICSCELLCOMMON_H
#define QICSCELLCOMMON_H

#include <QObject>
#include <QPointer>
#include <QPen>
#include <QFont>
#include <QPalette>
#include <QCursor>
#include <QPixmap>

#include "QicsGridInfo.h"
#include "QicsCellStyle.h"

class QValidator;
class QicsPasteValidator;
class QicsDataItemFormatter;
class QicsCellDisplay;
class QicsGridInfo;

////////////////////////////////////////////////////////////////////////////////


/*! \class QicsCellCommon QicsCellCommon.h
* \nosubgrouping
* \brief Abstract class for setting and retrieving table attributes
*
* QicsCellCommon is an abstract class that defines a number of cell
* attributes and partially implements the setting and retrieval operations
* for those attributes.  Ancestor classes that inherit from QicsCellCommon
* will complete the implementation so that setting an attribute results
* in a cell, row, column, or grid-wide change, depending on the class.  New
* cell attributes should be added here so that all subclasses can use them.
*/

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////


class QICS_EXPORT QicsCellCommon: public QObject, public Qics
{
    Q_OBJECT
public:

    /*!
    * Specifies the label of the cell(s).  This string may be used
    * by the cell displayer when rendering the contents of the cell(s).
    *
    * The default value of this property is the null string.
    */
    Q_PROPERTY( QString label READ label WRITE setLabel )
    /*!
    * Specifies the enabled attribute of the cell(s).  If the value of
    * this property is \b false, the cell cannot be traversed to.
    *
    * The default value of this property is \b true.
    */
    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled )
    /*!
    * Specifies if the cell(s) is/are not allowed to be modified.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool readOnly READ readOnly  WRITE setReadOnly )
    /*!
    * Specifies the current selection state of this cell/row/column.
    *
    * The default value of this property is \b false.
    */
    Q_PROPERTY( bool selected READ selected )
    /*!
    * Specifies the font that will be used to draw text in the cell(s).
    *
    * The default value of this property is the default font of
    * the QicsTable widget.
    */
    Q_PROPERTY( QFont font READ font WRITE setFont )
    /*!
    * Specifies the palette that will be used when drawing in the cell(s).
    *
    * The default value of this property is the default palette of
    * the QicsTable widget.
    */
    Q_PROPERTY( QPalette palette READ palette WRITE setPalette )
    /*!
    * This is convenience property which specifies the foreground
    * color of the cell(s) by changing palette property.  This color
    * will be used when drawing any text in the cell(s) when it is
    * not selected.
    *
    * The default value of this property is the default foreground color
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
    * This is convenience property which specifies the background
    * color of the cell(s) by changing palette property.  This color
    * will be used when the cell is not selected.
    *
    * The default value of this property is the default background color
    * of the QicsTable widget.
    *
    * Note: Since setting property on row/column overrides cells property
    * as well as setting table's property overrides rows/columns property,
    * and background color is really part of palette property, setting
    * background color on row/column will override any other color
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
    * This is convenience property which specifies the selected
    * foreground color of the cell(s) by changing palette property.
    * This color will be used when drawing any text in the cell
    * when the cell is selected.
    *
    * The default value of this property is the default selectedForeground color
    * of the QicsTable widget.
    *
    * Note: Since setting property on row/column overrides cells property
    * as well as setting table's property overrides rows/columns property,
    * and selected foreground color is really part of palette property,
    * setting selected foreground color on row/column will override any
    * other color settings of cells in that row/column.
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
    * This is convenience property which specifies the selected
    * background color of the cell(s) by changing palette property.
    * This color will be used when the cell is selected.
    *
    * The default value of this property is the default selectedBackground color
    * of the QicsTable widget.
    *
    * Note: Since setting property on row/column overrides cells property
    * as well as setting table's property overrides rows/columns property,
    * and selected background color is really part of palette property,
    * setting selected background color on row/column will override any
    * other color settings of cells in that row/column.
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
    * Specifies the cursor for the cell(s).  This cursor will be displayed
    * whenever the mouse pointer is within the cell.
    *
    * The default value of this property is the null cursor, which
    * means that the default cursor for the table widget will be displayed.
    */
    Q_PROPERTY( QCursor cursor READ cursor WRITE setCursor )
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
    * Specifies the border width of the cell(s).
    *
    * The default value of this property is \b 1.
    */
    Q_PROPERTY( int borderWidth READ borderWidth WRITE setBorderWidth )
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
    // END REMOVE THIS if building with Qt 3.0.x
    /*!
    * Specifies text to be shown as a tool tip when the mouse pointer
    * is hovered over the cell(s).
    *
    * The default value of this property is the null string, which will
    * cause no tool tip to be displayed.
    */
    Q_PROPERTY( QString toolTipText READ toolTipText WRITE setToolTipText )

    /*!
    * Specifies the edit foreground color of the cell(s).
    * This color will be used when the cell is being edited.
    *
    * The default value of this property is the default text color
    * of the QicsTable widget.
    */
    Q_PROPERTY( QColor editForegroundColor READ editForegroundColor WRITE setEditForegroundColor )
    /*!
    * Specifies the edit background color of the cell(s).
    * This color will be used when the cell is being edited.
    *
    * The default value of this property is the default base color
    * of the QicsTable widget.
    */
    Q_PROPERTY( QColor editBackgroundColor READ editBackgroundColor WRITE setEditBackgroundColor )
    /*!
    * Specifies the style of input data in cell. If set's true
    * the QicsTable do recognizing of input data into cell (QicsDataInt,
    * QicsDataString, QicsDataDate, QicsDataDateTime, QicsDataTime).
    */
    Q_PROPERTY( bool autoRecognition READ autoRecognition WRITE setAutoRecognition )

    /*!
    * Specifies validator object that will be used for cell input validation.
    * This validator will be used when data pasted in cell/cells(through drag&drop
    * or QicsTable::paste() method).
    */
    Q_PROPERTY(QicsPasteValidator* pasteValidator READ pasteValidator WRITE
    setPasteValidator)
    /*!
    * Specifies validator object that will be used for cell input validation
    * This validator will be used when this cell is edited.
    */
    Q_PROPERTY(QValidator* validator READ validator WRITE
    setValidator)
    /*!
    * Specifies how cell object react on reciving drag-data. By default all cell's of QicsTable
    * hase allow to use Drag'n'Drop feature.
    */
    Q_PROPERTY( bool canAcceptDrops READ canAcceptDrops WRITE setCanAcceptDrops )
    //@}

public:
    /*! \internal
    * Constructor for the class.
    * \param parent parent object
    */
    QicsCellCommon(QObject *parent = 0);

    /*! \internal
    * Constructor for the class.
    * \param info grid info to be used during attribute operations
    * \param parent parent object
    */
    QicsCellCommon(QicsGridInfo *info, QObject *parent = 0);

    /*!
    * Returns auto recognition flag for cell.
    * See the \link #autoRecognition "autoRecognition" \endlink property for details.
    * \sa setAutoRecognition()
    */
    bool autoRecognition();

    /*!
    * This method returns true is cell can recive any drag data, othervise
    * returns false. Se also \link #setCanAcceptDrops "setCanAcceptDrops" \endlink function.
    */
    bool canAcceptDrops() const;

    /*! \internal
    * Sets the grid info object that will be used during attribute operations.
    */
    virtual void setInfo(QicsGridInfo *info);

    /*!
    * Returns the data model associated with this object.
    */
    inline QicsDataModel *dataModel() const;

    /*!
    * Returns the margin of the cell (in pixels).
    * See the \link #margin "margin" \endlink property for details.
    * \sa setMargin()
    */
    int margin() const;

    /*!
    * Returns the read-only attribute of the cell.
    * See the \link #readOnly "readOnly" \endlink property for details.
    * \sa setReadOnly()
    */
    bool readOnly() const;

    /*!
    * Returns the font attribute of the cell.
    * See the \link #font "font" \endlink property for details.
    * \sa setFont()
    */
    QFont font() const;

    /*!
    * Returns the selected font attribute of the cell.
    * See the \link #selectedFont "selectedFont" \endlink property for details.
    * \sa setSelectedFont()
    */
    QFont selectedFont() const;

    /*!
    * Returns the palette attribute of the cell.
    * See the \link #palette "palette" \endlink property for details.
    * \sa setPalette()
    */
    QPalette palette() const;

    /*!
    * Returns the cell's foreground color.
    * See the \link #foregroundColor "foregroundColor" \endlink
    * property for details.
    * \sa setForegroundColor()
    */
    QColor foregroundColor() const;

    /*!
    * Returns the cell's window color.
    * See the \link #windowColor "windowColor" \endlink
    * property for details.
    * \sa setWindowColor()
    */
    QColor windowColor() const;

    /*!
    * Returns the cell's window text color.
    * See the \link #windowTextColor "windowTextColor" \endlink
    * property for details.
    * \sa setWindowTextColor()
    */
    QColor windowTextColor() const;

    /*!
    * Returns the cell's background color.
    * See the \link #backgroundColor "backgroundColor" \endlink
    * property for details.
    * \sa setBackgroundColor()
    */
    QColor backgroundColor() const;

    /*!
    * Returns the cell's selected foreground color.
    * See the \link #selectedForegroundColor "selectedForegroundColor" \endlink
    * property for details.
    * \sa setSelectedForegroundColor()
    */
    QColor selectedForegroundColor() const;

    /*!
    * Returns the cell's selected background color.
    * See the \link #selectedBackgroundColor "selectedBackgroundColor" \endlink
    * property for details.
    * \sa setSelectedBackgroundColor()
    */
    QColor selectedBackgroundColor() const;

    /*!
    * Returns the cell's edit foreground color.
    * See the \link #editForegroundColor "editForegroundColor" \endlink
    * property for details.
    * \sa setEditForegroundColor()
    */
    QColor editForegroundColor() const;

    /*!
    * Returns the cell's edit background color.
    * See the \link #editBackgroundColor "editBackgroundColor" \endlink
    * property for details.
    * \sa setEditBackgroundColor()
    */
    QColor editBackgroundColor() const;

    /*!
    * Returns the cell's highlight foreground color.
    * See the \link #highlightForegroundColor "highlightForegroundColor" \endlink
    * property for details.
    * \sa setHighlightForegroundColor()
    * \since 2.3
    */
    QColor highlightForegroundColor() const;

    /*!
    * Returns the cell's highlight background color.
    * See the \link #highlightBackgroundColor "highlightBackgroundColor" \endlink
    * property for details.
    * \sa setHighlightBackgroundColor()
    * \since 2.3
    */
    QColor highlightBackgroundColor() const;

    /*!
    * Returns the cell's pixmap.
    * See the \link #pixmap "pixmap" \endlink property for details.
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
    * Returns the QicsCellDisplay object for this cell.  The cell display object
    * controls aspects of displaying and editing the cell.  See QicsCellDisplay
    * and its subclasses for details.
    * \sa setDisplayer()
    */
    QicsCellDisplay *displayer() const;

    /*!
    * Returns the QicsDataItemFormatter object for this cell.
    * The formatter object controls the formatting of data values
    * that are displayed in this cell.
    * See QicsDataItemFormatter and its subclasses for details.
    * \sa setFormatter()
    */
    QicsDataItemFormatter *formatter() const;

    /*!
    * Returns the alignment options for this cell.
    * See the \link #alignment "alignment" \endlink property for details.
    * \sa Qt::Alignment
    * \sa setAlignment()
    */
    int alignment() const;

    /*!
    * Returns the enabled attribute of the cell.
    * See the \link #enabled "enabled" \endlink property for details.
    * \sa setEnabled()
    */
    bool enabled() const;

    /*!
    * Returns the selected attribute of the cell.
    * See the \link #selected "selected" \endlink property for details.
    */
    virtual bool selected() const;

    /*!
    * Returns the text flags attribute of the cell.
    * See the \link #textFlags "textFlags" \endlink property for details.
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
    * Returns the label attribute of this cell.  This string may be used
    * by the cell displayer when rendering the contents of this cell.
    * See the \link #label "label" \endlink property for details.
    * \sa setLabel()
    */
    QString label() const;

    /*!
    * Returns the maximum length of the value in this cell.
    * See the \link #maxLength "maxLength" \endlink property for details.
    * \sa setMaxLength()
    */
    int maxLength() const;

    /*!
    * Returns the cursor for this cell.
    * See the \link #cursor "cursor" \endlink property for details.
    * \sa setCursor()
    * \sa unsetCursor()
    */
    const QCursor &cursor() const;

    /*!
    * Returns the border width of this cell.
    * See the \link #borderWidth "borderWidth" \endlink property for details.
    * \sa setBorderWidth()
    */
    int borderWidth() const;

    /*!
    * Returns the pen used to draw the top border of this cell.
    * See the \link #topBorderPen "topBorderPen" \endlink property for details.
    * \sa setTopBorderPen()
    */
    QPen topBorderPen() const;

    /*!
    * Returns the pen used to draw the left border of this cell.
    * See the \link #leftBorderPen "leftBorderPen" \endlink property for details.
    * \sa setLeftBorderPen()
    */
    QPen leftBorderPen() const;

    /*!
    * Returns the pen used to draw the right border of this cell.
    * See the \link #rightBorderPen "rightBorderPen" \endlink property for details.
    * \sa setRightBorderPen()
    */
    QPen rightBorderPen() const;

    /*!
    * Returns the pen used to draw the bottom border of this cell.
    * See the \link #bottomBorderPen "bottomBorderPen" \endlink property for details.
    * \sa setBottomBorderPen()
    */
    QPen bottomBorderPen() const;

    /*!
    * Returns a user-supplied pointer for this cell.
    */
    void *userData() const;

    /*!
    * Returns the tool tip attribute of this cell.  This string will be
    * displayed in a tooltip when the mouse is hovered over the cell.
    * See the \link #toolTipText "toolTipText" \endlink property for details.
    * \sa setToolTipText()
    */
    QString toolTipText() const;

    inline QicsGridInfo& gridInfo() const {return *m_info;}

public slots:
    /*!
    * Sets the margin of the cell (in pixels).
    * See the \link #margin "margin" \endlink property for details.
    * \sa margin()
    */
    void setMargin(int margin);

    /*!
    * Sets the read-only attribute of the cell.
    * See the \link #readOnly "readOnly" \endlink property for details.
    * \sa readOnly()
    */
    void setReadOnly(bool b);

    /*!
    * Sets the font attribute of the cell.
    * See the \link #font "font" \endlink property for details.
    * \sa font()
    */
    void setFont(const QFont &font);

    /*!
    * Sets the selected font attribute of the cell.
    * See the \link #selectedFont "selectedFont" \endlink property for details.
    * \sa selectedFont()
    */
    void setSelectedFont(const QFont &font);

    /*!
    * Sets the palette attribute of the cell.
    * See the \link #palette "palette" \endlink property for details.
    * \sa palette()
    */
    void setPalette(const QPalette &pal);

    /*!
    * Sets the cell's foreground color.
    * See the \link #foregroundColor "foregroundColor" \endlink
    * property for details.
    * \sa foregroundColor()
    */
    void setForegroundColor(const QColor &p);

    /*!
    * Sets the cell's window color.
    * See the \link #setWindowColor "setWindowColor" \endlink
    * property for details.
    * \sa windowColor()
    */
    void setWindowColor(const QColor &color);

    /*!
    * Sets the cell's window text color.
    * See the \link #setWindowTextColor "setWindowTextColor" \endlink
    * property for details.
    * \sa windowTextColor()
    */
    void setWindowTextColor(const QColor &color);

    /*!
    * Sets the cell's background color.
    * See the \link #backgroundColor "backgroundColor" \endlink
    * property for details.
    * \sa backgroundColor()
    */
    void setBackgroundColor(const QColor &p);

    /*!
    * Sets the cell's selected foreground color.
    * See the \link #selectedForegroundColor "selectedForegroundColor" \endlink
    * property for details.
    * \sa selectedForegroundColor()
    */
    void setSelectedForegroundColor(const QColor &p);

    /*!
    * Sets the cell's selected background color.
    * See the \link #selectedBackgroundColor "selectedBackgroundColor" \endlink
    * property for details.
    * \sa selectedBackgroundColor()
    */
    void setSelectedBackgroundColor(const QColor &p);

    /*!
    * Sets the cell's edit foreground color.
    * See the \link #editForegroundColor "editForegroundColor" \endlink
    * property for details.
    * \sa editForegroundColor()
    */
    void setEditForegroundColor(const QColor &p);

    /*!
    * Sets the cell's edit background color.
    * See the \link #editBackgroundColor "editBackgroundColor" \endlink
    * property for details.
    * \sa editBackgroundColor()
    */
    void setEditBackgroundColor(const QColor &p);

    /*!
    * Sets the cell's highlight foreground color.
    * See the \link #highlightForegroundColor "highlightForegroundColor" \endlink
    * property for details.
    * \sa highlightForegroundColor(), QicdGridCommon::highlightSelection()
    * \since 2.3
    */
    void setHighlightForegroundColor(const QColor &p);

    /*!
    * Sets the cell's highlight background color.
    * See the \link #highlightBackgroundColor "highlightBackgroundColor" \endlink
    * property for details.
    * \sa highlightBackgroundColor(), QicdGridCommon::setHighlightSelection()
    * \since 2.3
    */
    void setHighlightBackgroundColor(const QColor &p);

    /*!
    * Sets the cell's pixmap attribute.
    * See the \link #pixmap "pixmap" \endlink property for details.
    * \sa pixmap()
    */
    void setPixmap(const QPixmap &p);

    /*!
    * Sets the cell's pixmap attribute from a file.
    * See the \link #pixmap "pixmap" \endlink property for details.
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
    * Sets the QicsCellDisplay object for this cell.  The cell display object
    * controls aspects of displaying and editing the cell.  See QicsCellDisplay
    * and its subclasses for details.  Setting this value to 0 will cause
    * the table to use the default cell display object for this cell.
    * \sa displayer()
    */
    void setDisplayer(QicsCellDisplay *d);

    /*!
    * Sets the QicsDataItemFormatter object for this cell.
    * The formatter object controls the formatting of data values
    * that are displayed in this cell.
    * See QicsDataItemFormatter and its subclasses for details.
    * \sa formatter()
    */
    void setFormatter(QicsDataItemFormatter *d);

    /*!
    * Sets the alignment options for this cell.
    * See the \link #alignment "alignment" \endlink property for details.
    * \sa Qt::Alignment
    * \sa alignment()
    */
    void setAlignment(int flags);

    /*!
    * Sets the enabled attribute of the cell.
    * See the \link #enabled "enabled" \endlink property for details.
    * \sa enabled()
    */
    void setEnabled(bool b);

    /*!
    * Sets the text flags attribute of the cell.
    * See the \link #textFlags "textFlags" \endlink property for details.
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
    * Sets the label attribute of this cell.  This string may be used
    * by the cell displayer when rendering the contents of this cell.
    * See the \link #label "label" \endlink property for details.
    * \sa label()
    */
    void setLabel(const QString &label);

    /*!
    * Sets the maximum length of the value in this cell.
    * See the \link #maxLength "maxLength" \endlink property for details.
    * \sa maxLength()
    */
    void setMaxLength(int len);

    /*!
    * Sets the cursor for this cell.
    * See the \link #cursor "cursor" \endlink property for details.
    * \sa cursor()
    * \sa unsetCursor()
    */
    void setCursor(const QCursor &c);
    /*!
    * Unsets the cursor for this cell.
    * See the \link #cursor "cursor" \endlink property for details.
    * \sa cursor()
    * \sa setCursor()
    */
    void unsetCursor();

    /*!
    * Sets the border width of this cell.
    * See the \link #borderWidth "borderWidth" \endlink property for details.
    * \sa borderWidth()
    */
    void setBorderWidth(int bw);

    /*!
    * Sets the pen used to draw the top border of this cell.
    * See the \link #topBorderPen "topBorderPen" \endlink property for details.
    * \sa topBorderPen()
    */
    void setTopBorderPen(const QPen &pen);

    /*!
    * Sets the pen used to draw the left border of this cell.
    * See the \link #leftBorderPen "leftBorderPen" \endlink property for details.
    * \sa leftBorderPen()
    */
    void setLeftBorderPen(const QPen &pen);

    /*!
    * Sets the pen used to draw the right border of this cell.
    * See the \link #rightBorderPen "rightBorderPen" \endlink property for details.
    * \sa rightBorderPen()
    */
    void setRightBorderPen(const QPen &pen);

    /*!
    * Sets the pen used to draw the bottom border of this cell.
    * See the \link #bottomBorderPen "bottomBorderPen" \endlink property for details.
    * \sa bottomBorderPen()
    */
    void setBottomBorderPen(const QPen &pen);

    /*!
    * Sets a user-supplied pointer for this cell.  The table will not
    * use or delete this pointer.  It remains under the programmer's control.
    */
    void setUserData(void *data);

    /*!
    * Sets the tool tip attribute of this cell.  This string will be
    * displayed in a tooltip when the mouse is hovered over the cell.
    * See the \link #toolTipText "toolTipText" \endlink property for details.
    * \sa toolTip()
    */
    void setToolTipText(const QString &txt);
    /*!
    * Sets automatical recognition for cell.
    * See the \link #autoRecognition "autoRecognition" \endlink property for details.
    * \sa autoRecognition()
    */
    void setAutoRecognition(const bool flag);

    /*!
    * Sets allowing cell to recive drag and drop data.
    * If you sets to false, cell will not recive and cannot send any
    * drag data. By default all cells can recive and send/move they
    * dataItems. Se get method \link #canAcceptDrops "canAcceptDrops" \endlink
    */
    void setCanAcceptDrops(const bool);

protected:
    /*! \internal
    * Abstract method used to actually set a cell attribute's value.
    * This method must be implemented by any subclass.
    */
    virtual void setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val) = 0;
    /*! \internal
    * Abstract method used to actually retrieve a cell attribute's value.
    * This method must be implemented by any subclass.
    */
    virtual void *getAttr(QicsCellStyle::QicsCellStyleProperty attr) const = 0;
    /*! \internal
    * Abstract method used to actually clear a cell attribute's value.
    * This method must be implemented by any subclass.
    */
    virtual void clearAttr(QicsCellStyle::QicsCellStyleProperty attr) = 0;

    /*! \internal
    * Abstract method used to actually set the cell margin in the
    * dimension manager.  This method must be implemented by any subclass.
    */
    virtual void setDMMargin(int margin) = 0;
    /*! \internal
    * Abstract method used to actually set the cell font in the
    * dimension manager.  This method must be implemented by any subclass.
    */
    virtual void setDMFont(const QFont &font) = 0;
    /*! \internal
    * Abstract method used to actually set the cell margin in the
    * dimension manager.  This method must be implemented by any subclass.
    */
    virtual void setDMBorderWidth(int bw) = 0;
    /*! \internal
    * Abstract method used to check if a row/column has enought wize to fit height h/width w in the
    * dimension manager.  This method must be implemented by any subclass.
    * \since 2.2
    */
    virtual void setDMFitSize(int w, int h);

    /*! \internal
    * Returns the style manager associated with this object.
    */
    inline QicsStyleManager &styleManager() const;

    /*! \internal
    * Returns the dimension manager associated with this object.
    */
    inline QicsDimensionManager &dimensionManager() const;

    /*! \internal
    * the grid info for this object
    */
    QPointer<QicsGridInfo> m_info;

    /// \internal should the indices change when model changes
    bool myFollowModel;

private:
#ifdef Q_DISABLE_COPY
    QicsCellCommon(const QicsCellCommon &cc);
    QicsCellCommon &operator=(const QicsCellCommon &cc);
#endif
    friend class QicsHTMLExport;
    friend class QicsTableRegionDrag;
};


inline QicsDataModel *QicsCellCommon::dataModel() const
{
    return (m_info ? m_info->dataModel() : 0);
}

inline QicsStyleManager &QicsCellCommon::styleManager() const
{
    return (*(m_info->styleManager()));
}

inline QicsDimensionManager &QicsCellCommon::dimensionManager() const
{
    return (*(m_info->dimensionManager()));
}

#endif // QICSCELLCOMMON_H


