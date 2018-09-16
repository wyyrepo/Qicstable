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

#ifndef QICSCOMBOCELLDISPLAY_H
#define QICSCOMBOCELLDISPLAY_H

#include <QComboBox>
#include "QicsCellDisplay.h"

/*!
* \class QicsComboCellDisplay QicsComboCellDisplay.h
* \brief A cell displayer for combo boxes
*
* QicsComboCellDisplay is a cell displayer for QicsTable that displays combo
* boxes in cells.  This cell display class is different from QicsTextCellDisplay
* in that QicsComboCellDisplay inherits from both QicsMovableEntryWidgetCellDisplay
* \b and QComboBox.  This means that the displayer object is also a combo box.
* This allows the programmer to easily use any QComboBox method or modify any
* QComboBox property.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QFocusEvent;
class QKeyEvent;
class QMouseEvent;
class QListWidget;
class QPushButton;

class QICS_EXPORT QicsComboCellDisplay: public QComboBox, public QicsMovableEntryWidgetCellDisplay
{
    Q_OBJECT
public:
    /*!
    * Constructor for use by programmers.
    */
    QicsComboCellDisplay( QWidget *parent = 0);
    virtual ~QicsComboCellDisplay();

    /*!
    * Set the index of the current item in the combobox
    */
    void setCurrentIndex( int index );

    /*!
    * Set the text of the current item in the combobox
    */
    void setCurrentText( const QString& );

    /*!
    * holds whether the combobox can be edited by the user
    */
    void setEditable( bool );

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    virtual void startEdit(QicsScreenGrid *, int row, int col,
        const QicsDataItem *itm);

    virtual void moveEdit(QicsScreenGrid *grid, int row, int col,
        const QRect &rect);

    inline virtual bool isEmpty(QicsGridInfo *, int, int,
        const QicsDataItem *) const { return false; }

    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    virtual bool handleMouseEvent(QicsScreenGrid *grid, int row, int col,
        QMouseEvent *me);

    virtual bool handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *ke);
    virtual void focusOutEvent(QFocusEvent*);

    virtual void endEdit(QicsScreenGrid *grid, int, int);

    /*!
    * Returnss whether data values that are not in the combo box's list
    * should be added to the list.  If \b true, any value that this
    * object attempts to display will be added to the combo box's list
    * of choices, if it does not already appear in the list.  If \b false,
    * no values will be added to the list.
    */
    inline bool addValueToList() const {return myAddValueToList;}

    /*!
    * Controls whether data values that are not in the combo box's list
    * should be added to the list.  If \b true, any value that this
    * object attempts to display will be added to the combo box's list
    * of choices, if it does not already appear in the list.  If \b false,
    * no values will be added to the list.
    */
    inline void setAddValueToList(bool set) {myAddValueToList = set;}

    static const QString ComboCellDisplayName;

    virtual QString cellDisplayName() const;

    /*!
    * Returns widget that cell displayer uses.
    */
    virtual QWidget* widget(){return (static_cast<QComboBox*>(this));}

    virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc);

    virtual void configureFromDomXml(const QDomElement& e);

protected:
    virtual QWidget *newEntryWidget(QicsScreenGrid *grid);
    virtual void keyPressEvent(QKeyEvent *ke);
    virtual bool eventFilter(QObject *obj, QEvent *event);

    /*!
    * This method is called when the value in the combobox for cell
    * (\a row, \a col ) is changed.  The new combobox value is \a val.
    *
    * This method is intended to be reimplemented by subclasses
    * of QicsComboCellDisplay.  The programmer should implement this
    * method so that it performs the appropriate action(s).  Possible
    * actions could include setting a new value in the QicsDataModel or
    * performing some other action based on the state of the combobox.
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
    * \param model_row the \a model row index of the cell whose value changed
    * \param model_col the \a model column index of the cell whose value changed
    * \param val the new state of the checkbox
    */
    virtual void valueChanged(QicsGridInfo *info, int row, int col,
        int model_row, int model_col, const QString &val);

    virtual QString textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

    /*!
    * \internal
    * do we add new values to the list?
    */
    bool myAddValueToList;

protected slots:
    /*!
    * Called when an item is selected in the combo box
    */
    virtual void itemSelected(const QString &val);

    virtual void popup() { showPopup(); }
};


class QICS_EXPORT QicsComboCellDisplayConfigurationWidget : public QicsCellDisplayConfigurationWidget
{
    Q_OBJECT
public:
    QicsComboCellDisplayConfigurationWidget(QWidget* parent=0);

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


class QICS_EXPORT QicsComboCellDisplayFactory: public QicsCellDisplayFactory
{
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};

#endif //QICSCOMBOCELLDISPLAY_H


