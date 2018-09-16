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

#ifndef QICSCHECKCELLDISPLAY_H
#define QICSCHECKCELLDISPLAY_H

#include <QCheckBox>
#include <QPixmap>
#include "QicsCellDisplay.h"

class QFocusEvent;
class QKeyEvent;
class QMouseEvent;
/*!
* \class QicsCheckCellDisplay QicsCheckCellDisplay.h
* \brief A cell displayer for check boxes
*
* QicsCheckCellDisplay is a cell displayer for QicsTable that displays check
* boxes in cells.  This cell display class is different from QicsTextCellDisplay
* in that QicsCheckCellDisplay inherits from both QicsMovableEntryWidgetCellDisplay
* \b and QCheckBox.  This means that the displayer object is also a check box.
* This allows the programmer to easily use any QCheckBox method or modify any
* QCheckBox property.
*
* The check box state of a cell is determined by the method #checkState().
* The default implementation of this method works for all standard Qics data types
* (QicsDataInt, QicsDataString etc.). In case of using custom data type
* you must reimplement this method.
*
* The text that is displayed in the cell is determined by the method
* #textToDisplay().  The default implementation of this method returns
* the value of the \link QicsCell::label label\endlink property
* for the cell.  This method may be reimplemented in a subclass by the
* programmer.
*
* The pixmap that is displayed in the cell is determined by the method
* #pixmapToDisplay().  The default implementation of this method returns
* the value of the \link QicsCell::pixmap pixmap\endlink property
* for the cell.  This method may be reimplemented in a subclass by the
* programmer.
*
* When the check box state of a cell is changed by the user, the  #valueChanged()
* method is called.  The default implementation of this method works for all
* standard Qics data types (QicsDataInt, QicsDataString etc.).
* In case of using custom data types you must reimplement this method.
*/


////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsCheckCellDisplay: public QCheckBox,
    public QicsMovableEntryWidgetCellDisplay
{
    Q_OBJECT

public:

    /*!
    * Constructor for use by programmers.
    */
    QicsCheckCellDisplay( QWidget * parent = 0);

    /*!
    * Constructor for use by programmers.
    */
    QicsCheckCellDisplay( const QString & text, QWidget * = 0 );

    virtual ~QicsCheckCellDisplay();

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm, QRect &rect,
        QPainter *painter);

    virtual void startEdit(QicsScreenGrid *, int row, int col,
        const QicsDataItem *itm);

    virtual void moveEdit(QicsScreenGrid *grid, int row, int col,
        const QRect &rect);

    inline virtual bool isEmpty(QicsGridInfo *, int, int,
        const QicsDataItem *) const { return false; }

    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    bool handleMouseEvent(QicsScreenGrid *grid, int row, int col, QMouseEvent *me);

    static const QString CheckCellDisplayName;
    virtual QString cellDisplayName() const;

    /*!
    * If whole if true, the whole cell will be clickable and change check state
    */
    inline void setWholeCellClickable(bool whole) {m_isWholeCellClickable = whole;}
    /*!
    * Returns true if the whole cell is clickable and change check state
    */
    inline bool isWholeCellClickable() const {return m_isWholeCellClickable;}

    virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc);
    virtual void configureFromDomXml(const QDomElement& e);

protected:
    virtual QWidget *newEntryWidget(QicsScreenGrid *grid);

    virtual void focusOutEvent(QFocusEvent* fe);

    bool handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *);

    /*!
    * Returns the check box state for cell (\a row, \a col ).
    *
    * This method can be reimplemented by subclasses
    * of QicsCheckCellDisplay.
    *
    * For example, a subclass might implement the following method that
    * sets the check box when the value of the cell is "Y":
    *
    * \code
    * bool MyCheckBoxSubclass::checkState(QicsGridInfo *info, int row, int col, const QicsDataItem *itm)
    * {
    *    return (itm->string() == QString("Y"));
    * }
    * \endcode
    *
    * \param info grid info object
    * \param row the \a visual row index of the cell whose value changed
    * \param col the \a visual column index of the cell whose value changed
    * \param itm the data item for this cell (this value may be 0)
    */
    virtual bool checkState(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm);

    /*!
    * Returns the full check box state for cell (\a row, \a col ).
    *
    * Same as above but returns one of Qt::CheckState values according to
    * cell value and isTristate().
    *
    * \since 2.4
    */
    virtual Qt::CheckState fullCheckState(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm);

    virtual void setCheckState (Qt::CheckState state)
    { QCheckBox::setCheckState(state); }

    /*!
    * This method is called when the checkbox state for cell (\a row, \a col )
    * is changed.  The new checkbox state is \a set.
    *
    * This method works for all standard Qics data types. In case of using
    * custom data type you must reimplement this virtual method.
    * Possible actions could include setting a new value in the data model or
    * performing some other action based on the state of the checkbox.
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
    * \param set the new state of the checkbox
    */
    virtual void valueChanged(QicsGridInfo *info, int row, int col, Qt::CheckState set);

    virtual QWidget* widget() {return static_cast<QCheckBox*>(this);}

    virtual QString textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

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

    void celldisplay_init();

protected slots:
    /*!
    * \internal
    * Called when state changes in the entry widget
    */
    virtual void checkStateChanged(int state);

private:
    bool m_isWholeCellClickable;
};


class QICS_EXPORT QicsCheckCellDisplayConfigurationWidget : public QicsCellDisplayConfigurationWidget
{
public:
    QicsCheckCellDisplayConfigurationWidget(QWidget* parent=0);

    virtual void readConfiguration(QicsCellDisplay* cellDisplay);
    virtual void writeConfiguration(QicsCellDisplay* cellDisplay);
};


class QICS_EXPORT QicsCheckCellDisplayFactory: public QicsCellDisplayFactory
{
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};

#endif //QICSCHECKCELLDISPLAY_H


