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

#ifndef QICSCOMBOTABCELLDISPLAY_H
#define QICSCOMBOTABCELLDISPLAY_H

#include "QicsComboCellDisplay.h"

class QTableWidget;

/*!
* \class QicsComboTabCellDisplay QicsComboTabCellDisplay.h
* \brief A cell displayer for combo box with tabbed dropdown list
*
* QicsComboTabCellDisplay is a cell displayer for QicsTable that displays
* the listbox entries through the combobox choice list in cells.  This cell
* display class is inherits from QicsComboCellDisplay and it behaves in similar way.
*
* QicsComboTabCellDisplay uses QTableWidget as a popup view, function
* \link tabWidget() returns a pointer to the table widget. After creation
* the table widget is empty, to set its dimension use \link setTabSize().
*
* The value of the cell is determined by active column and the current selected
* row. The active column can be changed by \link setActiveColumn().
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsComboTabCellDisplay: public QicsComboCellDisplay
{
    Q_OBJECT
public:
    /*!
    * Constructor for use by programmers.
    */
    QicsComboTabCellDisplay( QWidget *parent = 0);
    virtual ~QicsComboTabCellDisplay() {};

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    /*!
    * Returns table widget which is set as a combo box view.
    */
    inline QTableWidget* tabWidget() const
    { return reinterpret_cast<QTableWidget*>(view()); }

    /*!
    * Returns string value from \a row and \a column cell of the
    * tabbed data.
    */
    QString tabData(int row, int column) const;

    /*!
    * Sets string value for \a row and \a column cell of the
    * tabbed data.
    */
    void setTabData(int row, int column, const QString &data);

    /*!
    * Adds string data for the whole row \a row.
    */
    void addRowData(int row, const QStringList& items);

    /*!
    * Adds string data for the whole column \a column.
    */
    void addColumnData(int column, const QStringList& items);

    /*!
    * Returns row count of the tabbed view.
    */
    int  rowCount() const;

    /*!
    * Returns column count of the tabbed view.
    */
    int  columnCount() const;

    /*!
    * Inserts \a count rows into the tabbed view starting at position \a index.
    */
    bool insertRows(int count, int index = 0);

    /*!
    * Inserts \a count columns into the tabbed view starting at position \a index.
    */
    bool insertColumns(int count, int index = 0);

    /*!
    * Sets dimension of the tabbed view to \a rows and \a columns.
    */
    bool setTabSize(int rows, int columns);

    /*!
    * Returns active column of the tabbed view, i.e. the column which
    * defines the value will be displayed in the combo box when
    * user selects a row. By default its 0.
    */
    inline int activeColumn() const {return m_activecolumn;}

    /*!
    * Sets active column of the tabbed view, i.e. the column which
    * defines the value will be displayed in the combo box when
    * user selects a row. By default its 0.
    */
    void setActiveColumn(int column);

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

    static const QString ComboTabCellDisplayName;
    virtual QString cellDisplayName() const;

protected slots:
    /*!
    * Called when an item is selected in the combo box
    */
    virtual void itemSelected(int index);

    virtual void popup();

protected:
    /*!
    * This method is called when the value in the combobox for cell
    * (\a row, \a col ) is changed.  The new combobox value is \a val.
    *
    * This method is intended to be reimplemented by subclasses
    * of QicsComboTabCellDisplay.  The programmer should implement this
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
    virtual void valueChanged(QicsGridInfo *info, int row, int col, const QString &val);

    virtual QString textToDisplay(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

private:
    int m_activecolumn;
    QString	m_defaultText;
};


class QICS_EXPORT QicsComboTabCellDisplayConfigurationWidget : public QicsComboCellDisplayConfigurationWidget
{
    Q_OBJECT
public:
    QicsComboTabCellDisplayConfigurationWidget(QWidget* parent=0);
};


class QICS_EXPORT QicsComboTabCellDisplayFactory: public QicsComboCellDisplayFactory {
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};


#endif // QICSCOMBOTABCELLDISPLAY_H


