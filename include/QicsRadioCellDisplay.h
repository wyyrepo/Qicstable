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

#ifndef QICSRADIOCELLDISPLAY_H
#define QICSRADIOCELLDISPLAY_H

#include <QButtonGroup>
#include "QicsCellDisplay.h"

class QKeyEvent;
class QMouseEvent;
class QFocusEvent;
class QPushButton;
class QListWidget;
class QVBoxLayout;
class QRadioButton;
class QAbstractButton;


class QICS_EXPORT QicsRadioWidget: public QWidget
{
    Q_OBJECT
public:
    /*!
    * Class QicsRadioWidget used internally by QicsRadioCellDisplay.
    */
    QicsRadioWidget(QWidget *parent);
    virtual ~QicsRadioWidget() {};

    /*!
    * Adds radio button to the widget.
    */
    void addButton(QRadioButton * button);

    /*!
    * This is overloaded function, adds radio button to the widget.
    */
    void addButton(const QString &text);

    /*!
    * Adds several radio buttons to the widget.
    */
    void addItems(const QStringList &labels);

    /*!
    * Returns radio button with index \a id from radio widget.
    */
    QRadioButton *button(int id) const;

    /*!
    * Returns index of the given radio button or -1 if button
    * is not on the widget.
    */
    int id(QRadioButton *button) const;

    /*!
    * Returns checked radio button.
    */
    QRadioButton *checkedButton() const;

    /*!
    * Returns checked radio button index or -1 if none of the buttons is checked.
    */
    int checkedId() const;

    /*!
    * Returns a list of all buttons on widget.
    */
    inline QList<QAbstractButton *> buttons () const
    { return m_bg->buttons(); }

    /*!
    * Clears the checked state for all buttons on widget.
    */
    void uncheckAll();

    /*!
    * Removes radio button from widget.
    */
    void removeButton(QRadioButton *button);

    /*!
    * Removes all radio buttons from widget.
    */
    void clear();

    /*!
    * Returns radio buttons count.
    */
    int count() const;

signals:
    void buttonClicked(int id);
    void buttonPressed(int id);
    void buttonReleased(int id);

protected:
    QButtonGroup *m_bg;
    QVBoxLayout *m_vbox;
};

/*!
* \class QicsRadioCellDisplay QicsRadioCellDisplay.h
* \brief A cell displayer for radio buttons group
*
* QicsRadioCellDisplay is a cell displayer for QicsTable that displays radio
* buttons in cells.  This cell display class is different from QicsTextCellDisplay
* in that QicsRadioCellDisplay inherits from both QicsMovableEntryWidgetCellDisplay
* and QicsRadioWidget.  This means that the displayer object is also a radio buttons
* widget.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsRadioCellDisplay: public QicsRadioWidget, public QicsMovableEntryWidgetCellDisplay
{
    Q_OBJECT
public:
    /*!
    * Constructor for use by programmers.
    */
    QicsRadioCellDisplay( QWidget *parent = 0);
    virtual ~QicsRadioCellDisplay() {};

    void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    void startEdit(QicsScreenGrid *, int row, int col, const QicsDataItem *itm);

    void moveEdit(QicsScreenGrid *grid, int row, int col, const QRect &rect);

    void endEdit(QicsScreenGrid *grid, int, int);

    inline virtual bool isEmpty(QicsGridInfo *, int, int, const QicsDataItem *) const
    { return false; }

    QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    bool handleMouseEvent(QicsScreenGrid *grid, int row, int col,
        QMouseEvent *me);

    bool handleKeyEvent(QicsScreenGrid *, int, int, QKeyEvent *ke);

    /*!
    * Sets the radio button with index \a id to the checked state.
    */
    void setChecked(int id);

    /*!
    * Returns widget that cell displayer uses.
    */
    inline virtual QWidget* widget()
    { return (static_cast<QWidget*>(this)); }

    virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc);
    virtual void configureFromDomXml(const QDomElement& e);

    static const QString RadioCellDisplayName;
    virtual QString cellDisplayName() const;

protected slots:
    /*!
    * Called when the index of the checked button is changed.
    */
    virtual void changeIndex(int);

protected:
    virtual QWidget *newEntryWidget(QicsScreenGrid *grid);

    virtual void focusOutEvent(QFocusEvent* fe);

    /*!
    * This method is called when the checked button index in the radio button
    * group for cell (\a row, \a col ) is changed.  The new checked button
    * index is \a index.
    *
    * This method is intended to be reimplemented by subclasses
    * of QicsRadioCellDisplay.  The programmer should implement this
    * method so that it performs the appropriate action(s).  Possible
    * actions could include setting a new value in the QicsDataModel or
    * performing some other action based on the state of the radio buttons.
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
    * \param index the new state of the radio buttons.
    */
    virtual void valueChanged(QicsGridInfo *info, int row, int col, int index);
};


class QICS_EXPORT QicsRadioCellDisplayConfigurationWidget: public QicsCellDisplayConfigurationWidget
{
    Q_OBJECT
public:
    QicsRadioCellDisplayConfigurationWidget(QWidget* parent=0);

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

class QICS_EXPORT QicsRadioCellDisplayFactory: public QicsCellDisplayFactory
{
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};

#endif // QICSRADIOCELLDISPLAY_H


