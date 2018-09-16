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

#ifndef QICSCOMBOCHECKCELLDISPLAY_H
#define QICSCOMBOCHECKCELLDISPLAY_H

#include <QStandardItemModel>
#include "QicsComboCellDisplay.h"

class QicsCheckComboModel;

/*!
* \class QicsComboCheckCellDisplay QicsComboCheckCellDisplay.h
* \brief A cell displayer for dropdown checklist combo boxes
*
* QicsComboCheckCellDisplay is a cell displayer for QicsTable that displays
* dropdown checklist boxes in cells.  This cell display class is inherits from
* QicsComboCellDisplay and behaves in a similar way.
*
* The value of the cell with QicsComboCheckCellDisplay displayer is set to
* a checked items list joined with a \a separator. If there is no checked items
* in the list the cell value is set to a default text string.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsComboCheckCellDisplay: public QicsComboCellDisplay
{
    Q_OBJECT
public:
    /*!
    * Constructor for use by programmers.
    */
    QicsComboCheckCellDisplay(QWidget *parent = 0);
    virtual ~QicsComboCheckCellDisplay() {};

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    /*!
    * Returns the check state of the item \a index in the list.
    */
    Qt::CheckState itemCheckState(int index) const;

    /*!
    * Sets the check state of the item \a index in the list
    * to \a state.
    */
    void setItemCheckState(int index, Qt::CheckState state);

    /*!
    * Returns the string \a separator which is used to join the checked
    * items into a string. By default the separator is comma.
    */
    inline QString separator() const {return m_separator;}

    /*!
    * Sets the string \a separator which is used to join the checked
    * items into a string. By default the separator is comma.
    */
    void setSeparator(const QString& separator);

    /*!
    * Returns the default text string which is displayed when no item is
    * checked in the list. This string is empty by default.
    */
    inline QString defaultText() const {return m_defaultText;}

    /*!
    * Sets the default text string which is displayed when no item is
    * checked in the list. This string is empty by default.
    */
    void setDefaultText(const QString& text);

    /*!
    * Returns a string list of checked items.
    */
    QStringList checkedItems() const;

    /*!
    * Sets \a items in the checklist to checked state.
    * If \a douncheck is true, this function will uncheck
    * all items in the checklist before setting checked items.
    * By default it won't do uncheck.
    */
    void setCheckedItems(const QStringList& items, bool douncheck = false);

    static const QString ComboCheckCellDisplayName;
    virtual QString cellDisplayName() const;

    virtual void endEdit(QicsScreenGrid *grid, int, int);

protected slots:
    /*!
    * This slot is called when checked items values have been changed.
    */
    void updateCheckedItems();

    /*!
    * Toggles the state of \a index item in the checklist.
    */
    void toggleCheckState(int index);

    /*!
    * Reimplemented virtual function which shows the dropdown checklist.
    */
    virtual void popup();

protected:
    /*!
    * This method is called when the value in the combobox for cell
    * (\a row, \a col ) is changed.  The new combobox value is \a val.
    *
    * This method is intended to be reimplemented by subclasses
    * of QicsComboCheckCellDisplay.  The programmer should implement this
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
    * \param val the new state of the checkbox
    */
    virtual void valueChanged(QicsGridInfo *info, int row, int col,
        const QString &val);

    virtual QString textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

private:
    QString	m_defaultText;
    QString	m_separator;
};


class QICS_EXPORT QicsComboCheckCellDisplayConfigurationWidget : public QicsComboCellDisplayConfigurationWidget
{
    Q_OBJECT
public:
    QicsComboCheckCellDisplayConfigurationWidget(QWidget* parent=0);
};


class QICS_EXPORT QicsComboCheckCellDisplayFactory: public QicsComboCellDisplayFactory {
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};


/*!
* \class QicsCheckComboModel QicsComboCheckCellDisplay.h
* \brief A model that is used in dropdown checklist combobox
*
* QicsCheckComboModel is a model intended to be used in dropdown
* checklist combobox. It is inherits from QStandardItemModel
* and overwrites a few virtual methods.
*/

class QICS_EXPORT QicsCheckComboModel : public QStandardItemModel
{
    Q_OBJECT
public:
    QicsCheckComboModel(QObject* parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

signals:
    void checkStateChanged();
};

#endif // QICSCOMBOCHECKCELLDISPLAY_H


