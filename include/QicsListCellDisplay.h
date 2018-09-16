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

#ifndef QICSLISTCELLDISPLAY_H
#define QICSLISTCELLDISPLAY_H

#include <QListWidget>
#include "QicsCellDisplay.h"

class QKeyEvent;
class QMouseEvent;
class QFocusEvent;
class QPushButton;

/*!
* \class QicsListCellDisplay QicsListCellDisplay.h
* \brief A cell displayer for list widget
*
* QicsListCellDisplay is a cell displayer for QicsTable that displays list
* widgets in cells.  This cell display class is different from QicsTextCellDisplay
* in that QicsListCellDisplay inherits from both QicsMovableEntryWidgetCellDisplay
* \b and QListWidget.  This means that the displayer object is also a list widget.
* This allows the programmer to easily use any QListWidget method or modify any
* QListWidget property.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsListCellDisplay: public QListWidget, public QicsMovableEntryWidgetCellDisplay
{
    Q_OBJECT
public:
    /*!
    * Constructor for use by programmers.
    */
    QicsListCellDisplay( QWidget *parent = 0);
    virtual ~QicsListCellDisplay() {};

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    virtual void startEdit(QicsScreenGrid *, int row, int col,
        const QicsDataItem *itm);

    virtual void moveEdit(QicsScreenGrid *grid, int row, int col,
        const QRect &rect);

    void endEdit(QicsScreenGrid *grid, int, int);

    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    virtual bool handleMouseEvent(QicsScreenGrid *grid, int row, int col,
        QMouseEvent *me);

    virtual bool handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *ke);

    inline virtual bool isEmpty(QicsGridInfo *, int, int,
        const QicsDataItem *) const { return false; }

    /*!
    * Sets the index of the current item in the list widget
    */
    void setCurrentRow(int row);

    /*!
    * Sets the text of the current item in the list widget.
    */
    void setCurrentText(const QString&);

    static const QString ListCellDisplayName;
    virtual QString cellDisplayName() const;

    /*!
    * Returns widget that cell displayer uses.
    */
    virtual QWidget* widget(){return (static_cast<QListWidget*>(this));}

    virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc);
    virtual void configureFromDomXml(const QDomElement& e);

protected slots:
    /*!
    * Called when the current row changes in the list widget
    */
    virtual void rowChanged(int);

protected:
    virtual QWidget *newEntryWidget(QicsScreenGrid *grid);

    virtual void focusOutEvent(QFocusEvent* fe);

    virtual QString textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

    /*!
    * This method is called when the value in the listwidget for cell
    * (\a row, \a col ) is changed.  The new listwidget value is \a val.
    *
    * This method is intended to be reimplemented by subclasses
    * of QicsListCellDisplay.  The programmer should implement this
    * method so that it performs the appropriate action(s).  Possible
    * actions could include setting a new value in the QicsDataModel or
    * performing some other action based on the state of the listwidget.
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
    * \param index the new state of the listwidget
    */
    virtual void valueChanged(QicsGridInfo *info, int row, int col, int index);
};


class QICS_EXPORT QicsListCellDisplayConfigurationWidget : public QicsCellDisplayConfigurationWidget
{
    Q_OBJECT
public:
    QicsListCellDisplayConfigurationWidget(QWidget* parent=0);

    virtual void readConfiguration(QicsCellDisplay* cellDisplay);
    virtual void writeConfiguration(QicsCellDisplay* cellDisplay);

protected slots:
    void addItem();
    void removeItem();
    void moveItemUp();
    void moveItemDown();

private:
    QListWidget* _itemList;
    QPushButton* _add;
    QPushButton* _remove;
    QPushButton* _up;
    QPushButton* _down;

};

class QICS_EXPORT QicsListCellDisplayFactory: public QicsCellDisplayFactory {
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};


#endif // QICSLISTCELLDISPLAY_H


