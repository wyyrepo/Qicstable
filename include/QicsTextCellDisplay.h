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

#ifndef QICSTEXTCELLDISPLAY_H
#define QICSTEXTCELLDISPLAY_H

#include <QLineEdit>
#include "QicsCellDisplay.h"

class QicsTextCellDisplayWidget;
class QicsTextCellDisplayData;
class QicsGrid;
class QicsGridInfo;
class QPainter;
class QKeyEvent;
class QEvent;
class QPixmap;

/*!
* \class QicsTextCellDisplay QicsTextCellDisplay.h
* \brief A cell displayer for text and pixmaps
*
* The text cell display object draws text and/or a pixmap for each cell
* that is it assigned to.  It uses a QLineEdit widget for data entry.
*
* QicsTextCellDisplay displays text and pixmaps, taking into consideration
* a number of cell and grid properties.  These properties are:
* - \link QicsCellCommon::enabled enabled \endlink
* - \link QicsCell::isCurrent isCurrent() \endlink
* - \link QicsCellCommon::selected selected() \endlink
* - \link QicsCellCommon::borderWidth borderWidth\endlink
* - \link QicsCellCommon::borderStyle borderStyle\endlink
* - \link QicsCellCommon::topBorderPen topBorderPen\endlink
* - \link QicsCellCommon::margin margin\endlink
* - \link QicsCellCommon::alignment alignment\endlink
* - \link QicsCellCommon::textFlags textFlags\endlink
* - \link QicsCellCommon::font font\endlink
* - \link QicsCellCommon::palette palette\endlink

* - \link QicsGridCommon::gridCellClipping gridCellClipping\endlink
* - \link QicsGridCommon::moreTextPixmap moreTextPixmap\endlink
* - \link QicsGridCommon::autoSelectCellContents\endlink
* - \link QicsGridCommon::currentCellBorderWidth currentCellBorderWidth\endlink
*
* See \ref cell_display for more information on creating or modifying
* a cell display class.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////
class QICS_EXPORT QicsTextCellDisplay: public QObject, public QicsMovableEntryWidgetCellDisplay
{
    Q_OBJECT
public:
    /*!
    * Constructor for use by programmers.
    */
    QicsTextCellDisplay(QWidget *parent = 0);
    virtual ~QicsTextCellDisplay();

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    virtual QRect contentRect(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    virtual void startEdit(QicsScreenGrid *, int row, int col,
        const QicsDataItem *itm);

    virtual void moveEdit(QicsScreenGrid *grid,
        int row, int col,
        const QRect &rect);

    virtual void endEdit(QicsScreenGrid *grid, int row, int col);

    inline virtual bool editWhenCurrent() const {return m_editWhenCurrent;}

    virtual void setEditWhenCurrent(bool yes) {m_editWhenCurrent = yes;}

    virtual bool isEmpty(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

    inline virtual bool contentScrollEnabled() const {return m_contentScrollEnabled;}

    virtual void setContentScrollEnabled(bool yes) {m_contentScrollEnabled = yes;}

    /*!
    * Returns state of text shifting when icon is present.
    * \sa setIconShiftsText
    * \since 2.3
    */
    inline bool iconShiftsText() const {return m_iconShiftsText;}

    /*!
    * Sets state of text shifting when icon is present to \a yes.
    * If set, text will be shifted to the left if icon is present (default).
    * \sa iconShiftsText
    * \since 2.3
    */
    inline void setIconShiftsText(bool yes) {m_iconShiftsText = yes;}

    /*!
    * Sets input mask \a inputMask for the cell text display.
    * Input mask works in QLineEdit mode only.
    * \since 2.4.2
    */
    inline void setInputMask(const QString &inputMask) {m_inputMask = inputMask;}

    /*!
    * Sets echo mode \a mode for the cell text display.
    * Echo mode works in QLineEdit mode only.
    * \since 2.4.2
    */
    inline void setEchoMode(QLineEdit::EchoMode mode) {m_echomode = mode;}

    /*!
    * Handler for keyboard events that occur within cell (\a row, \a col ).
    * This handler will forward key events to the text widget if the
    * text widget is currently being used to edit cell (\a row, \a col ).
    * Note that \a row and \a col are in \b visual coordinates.
    */
    virtual bool handleKeyEvent(QicsScreenGrid *grid, int row, int col,
        QKeyEvent *ke);

    virtual bool handleMouseEvent(QicsScreenGrid *grid, int row, int col,
                                            QMouseEvent *me);

    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    virtual QString tooltipText(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm, const QRect &rect) const;

    static const QString TextCellDisplayName;

    virtual QString cellDisplayName() const;

    /*!
    * Returns widget that cell displayer uses.
    */
    virtual QWidget* widget();

    virtual QDomElement toDomXml(const QString &tag, QDomDocument *doc);

    virtual void configureFromDomXml(const QDomElement &e);

signals:
    /*!
    * This signal is emitted whenever the text is edited.
    * The \b str argument is the next text, \b row and \b col -
    * row and column where text has been edited.
    */
    void textEdited(int row,int col,const QString &str);

protected slots:
    void handleTextChanged(const QString &str);

protected:
    QWidget *newEntryWidget(QicsScreenGrid *grid);

    /*!
    * Returns \b true if the pixmap (\a pix) and text (\a text, using
    * \a text_flags and \a font) can be completely displayed in the area
    * specified by \a rect.
    * \param info grid info object
    * \param rect area to display into
    * \param row row index in \a visual coordinates
    * \param col column index in \a visual coordinates
    * \param text text to display
    * \param text_flags flags for displaying text
    * \param font font to display text in
    * \param pix pixmap to display
    */
    virtual bool canDisplayAll(QicsGridInfo *info,
        const QRect &rect, int row, int col,
        const QString &text, int text_flags,
        const QFont &font,
        const QPixmap &pix) const;

    /*!
    * Event handler for the lineedit widget.
    */
    virtual bool eventFilter(QObject *obj, QEvent *evnt);

    /*!
    * Attempts to retrieve text from the lineedit widget, validate it,
    * convert the text to a QicsDataItem, and set the new value in the
    * data model.  Returns \b true if successful, \b false otherwise.
    */
    virtual bool setValue(QicsEntryWidgetInfo *info);

    /*!
    * Resets the value in the lineedit widget to the value in the data model
    * that is being displayed.
    */
    virtual void resetValue(QicsEntryWidgetInfo *info);

protected slots:
    inline void setLastFocusReason(Qt::FocusReason r) {myLastFocusReason = r;}

protected:
    /*!
    * \internal
    * the last event that was seen by the event filter.
    */
    QEvent *m_lastEvent;
    QicsTextCellDisplayWidget *m_widget;
    QString	m_inputMask;
    QString m_prevValue;
    QLineEdit::EchoMode m_echomode;
    static QicsTextCellDisplayData *td;
    bool m_editWhenCurrent;
    bool m_contentScrollEnabled;
    bool m_iconShiftsText;
};


class QICS_EXPORT QicsTextCellDisplayConfigurationWidget : public QicsCellDisplayConfigurationWidget
{
public:
    QicsTextCellDisplayConfigurationWidget(QWidget* parent=0);

    virtual void readConfiguration(QicsCellDisplay* cellDisplay);
    virtual void writeConfiguration(QicsCellDisplay* cellDisplay);
};


class QICS_EXPORT QicsTextCellDisplayFactory: public QicsCellDisplayFactory
{
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};


#endif //QICSTEXTCELLDISPLAY_H


