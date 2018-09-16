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

#ifndef QICSPROGRESSCELLDISPLAY_H
#define QICSPROGRESSCELLDISPLAY_H

#include <QProgressBar>
#include "QicsCellDisplay.h"

/*!
* \class QicsProgressCellDisplay QicsProgressCellDisplay.h
* \brief A cell displayer for progress bar
*
* QicsProgressCellDisplay is a cell displayer for QicsTable that displays
* progress bars in cells. This cell display class inherits from both
* QicsMovableEntryWidgetCellDisplay and QProgressBar. This means that the
* displayer object is also a progress bar.
* This allows the programmer to easily use any QProgressBar method or
* modify any QProgressBar property.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsProgressCellDisplay: public QProgressBar, public QicsMovableEntryWidgetCellDisplay
{
    Q_OBJECT
public:
    /*!
    * Constructor for use by programmers.
    */
    QicsProgressCellDisplay( QWidget *parent = 0);
    virtual ~QicsProgressCellDisplay() {};

    void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    void startEdit(QicsScreenGrid *, int, int, const QicsDataItem *)  {};

    void moveEdit(QicsScreenGrid *, int, int, const QRect&) {};

    void endEdit(QicsScreenGrid *, int, int) {};

    inline virtual bool isEmpty(QicsGridInfo *, int, int, const QicsDataItem *) const
    { return false; }

    QSize sizeHint(QicsGrid *grid, int row, int col, const QicsDataItem *itm);

    /*!
    * Sets the current value of the progress bar
    */
    void setValue(int value);

    /*!
    * Returns widget that cell displayer uses.
    */
    virtual QWidget* widget(){return (static_cast<QProgressBar*>(this));}

    virtual QDomElement toDomXml(const QString &tag, QDomDocument* doc);
    virtual void configureFromDomXml(const QDomElement& e);

    static const QString ProgressCellDisplayName;
    virtual QString cellDisplayName() const;

protected:
    virtual QWidget *newEntryWidget(QicsScreenGrid *grid);

    /*!
    * This method is called when the value of the progress for cell
    * (\a row, \a col ) is changed.  The new progress value is \a val.
    *
    * This method is intended to be reimplemented by subclasses
    * of QicsProgressCellDisplay.  The programmer should implement this
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
    * \param val the new value of the progress
    */
    virtual void valueChanged(QicsGridInfo *info, int row, int col, int val);
};

class QICS_EXPORT QicsProgressCellDisplayConfigurationWidget: public QicsCellDisplayConfigurationWidget
{
public:
    QicsProgressCellDisplayConfigurationWidget(QWidget* parent=0);

    virtual void readConfiguration(QicsCellDisplay* cellDisplay);
    virtual void writeConfiguration(QicsCellDisplay* cellDisplay);
};

class QICS_EXPORT QicsProgressCellDisplayFactory: public QicsCellDisplayFactory
{
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};


#endif // QICSPROGRESSCELLDISPLAY_H


