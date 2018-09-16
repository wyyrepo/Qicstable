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

#ifndef QICSPUSHBUTTONCELLDISPLAY_H
#define QICSPUSHBUTTONCELLDISPLAY_H

#include <QPushButton>
#include <QPixmap>
#include "QicsCellDisplay.h"

class QFocusEvent;
class QKeyEvent;
class QMouseEvent;

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

/*!
* \class QicsPushButtonCellDisplay QicsPushButtonCellDisplay.h
* \brief A cell displayer for push buttons
*
* QicsPushButtonCellDisplay is a cell displayer for QicsTable that displays push
* buttons in cells.  This cell display class is different from QicsTextCellDisplay
* in that QicsPushButtonCellDisplay inherits from both QicsMovableEntryWidgetCellDisplay
* \b and QPushButton.  This means that the displayer object is also a push button.
* This allows the programmer to easily use any QPushButton methods or modify any
* QPushButton property.
*/

class QICS_EXPORT QicsPushButtonCellDisplay: public QPushButton,
    public QicsMovableEntryWidgetCellDisplay
{
    Q_OBJECT
public:

    /*!
    * Constructor for use by programmers.
    */
    QicsPushButtonCellDisplay( QWidget * parent = 0);

    /*!
    * Constructor for use by programmers.
    */
    QicsPushButtonCellDisplay( const QString & text, QWidget * = 0 );

    virtual ~QicsPushButtonCellDisplay();

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm, QRect &rect,
        QPainter *painter);

    virtual void startEdit(QicsScreenGrid *, int row, int col,
        const QicsDataItem *itm);

    inline virtual bool isEmpty(QicsGridInfo *, int, int,
        const QicsDataItem *) const { return false; }

    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    /*!
    * Handles mouse events from screen grid.
    */
    bool handleMouseEvent(QicsScreenGrid *grid, int row, int col, QMouseEvent *me);

    /// String name of cell displayer.
    static const QString PushButtonCellDisplayName;

    /// Returns cell displayer name string.
    virtual QString cellDisplayName() const;

    /*!
    * Returns widget that cell displayer uses.
    */
    virtual QWidget* widget(){return static_cast<QPushButton*>(this);}

    virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc);
    virtual void configureFromDomXml(const QDomElement& e);
    bool isChecked(QicsGridInfo *info, int row, int col,const QicsDataItem *itm);

    /*!
    * This method is called when the state of the button for cell
    * (\a row, \a col ) is changed.  The new state is \a set.
    *
    * This method is intended to be reimplemented by subclasses
    * of QicsPushButtonCellDisplay.  The programmer should implement this
    * method so that it performs the appropriate action(s).  Possible
    * actions could include setting a new value in the QicsDataModel or
    * performing some other action based on the progress value.
    *
    * If you use this method to set a new value in the data model, it
    * is generally preferrable to use QicsGridInfo::setCurrentCellValue()
    * or QicsGridInfo::setCellValue(), as these methods will ensure that
    * the \link QicsTable::valueChanged valueChanged\endlink signal of
    * QicsTable is emitted.
    *
    * \param info grid info object
    * \param row the \a visual row index of the cell whose value changed
    * \param col the \a visual column index of the cell whose value changed
    * \param set the new state of the button
    */
    virtual void valueChanged(QicsGridInfo *info, int row, int col, bool set);

signals:
    // Emits when button is clicked
    void clicked(int row,int col);

protected:
    // Creates an instance of QPushButton.
    virtual QWidget *newEntryWidget(QicsScreenGrid *grid);

    virtual void focusOutEvent(QFocusEvent* fe);

    virtual void keyPressEvent(QKeyEvent *ke);

    virtual bool eventFilter(QObject *obj, QEvent *event);

    bool handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *);

    virtual QString textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

    void celldisplay_init();

public slots:
    void setCheckState();

protected slots:
    void handleMouseClick();
};


class QICS_EXPORT QicsPushButtonCellDisplayConfigurationWidget : public QicsCellDisplayConfigurationWidget
{
public:
    QicsPushButtonCellDisplayConfigurationWidget(QWidget* parent=0);

    virtual void readConfiguration(QicsCellDisplay* cellDisplay);
    virtual void writeConfiguration(QicsCellDisplay* cellDisplay);
};


class QICS_EXPORT QicsPushButtonCellDisplayFactory: public QicsCellDisplayFactory
{
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};

#endif //QICSPUSHBUTTONCELLDISPLAY_H


