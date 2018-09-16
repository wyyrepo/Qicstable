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

#ifndef QICSDATETIMECELLDISPLAY_H
#define QICSDATETIMECELLDISPLAY_H

#include <QDateTimeEdit>
#include "QicsCellDisplay.h"


class QFocusEvent;
class QKeyEvent;
class QMouseEvent;

/*!
* \class QicsDateTimeCellDisplay QicsDateTimeCellDisplay.h
* \brief A cell displayer for DateTime edit
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsDateTimeCellDisplay: public QDateTimeEdit, public QicsMovableEntryWidgetCellDisplay
{
    Q_OBJECT
public:
    /*!
    * Constructor for use by programmers.
    */
    QicsDateTimeCellDisplay( QWidget *parent = 0);

    virtual ~QicsDateTimeCellDisplay();
    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    virtual void startEdit(QicsScreenGrid *, int row, int col,
        const QicsDataItem *itm);

    QString cellDisplayName() const;

    /*!
    * Returns widget that cell displayer uses.
    */
    virtual QWidget* widget(){return static_cast<QDateTimeEdit*>(this);}

    inline virtual bool isEmpty(QicsGridInfo *, int, int,
        const QicsDataItem *) const { return false; }

    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    virtual bool handleMouseEvent(QicsScreenGrid *grid, int row, int col, QMouseEvent *me);
    virtual bool handleKeyEvent(QicsScreenGrid *grid, int row, int col, QKeyEvent *ke);

    virtual void focusOutEvent(QFocusEvent*);

    void contextMenuEvent ( QContextMenuEvent * event );

    QDomElement toDomXml(const QString &tag, QDomDocument* doc);
    void configureFromDomXml(const QDomElement &e);

    static const QString DateTimeCellDisplayName;

protected:
    virtual QWidget *newEntryWidget(QicsScreenGrid *grid);
    virtual void keyPressEvent(QKeyEvent *ke);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual QString textToDisplay(QicsGridInfo *ginfo, int row, int col, const QicsDataItem *itm) const;

protected slots:
    /*!
    * Called when an item is selected in the DateTime edit
    */
    virtual void setValue(const QDateTime &val);
    virtual void setValue(const QDate &val);
    virtual void setValue(const QTime &val);

private:
    bool myBlockSignals;
};


class QICS_EXPORT QicsDateTimeCellDisplayConfigurationWidget : public QicsCellDisplayConfigurationWidget
{
public:
    QicsDateTimeCellDisplayConfigurationWidget(QWidget* parent=0);

    virtual void readConfiguration(QicsCellDisplay* cellDisplay);
    virtual void writeConfiguration(QicsCellDisplay* cellDisplay);
};


class QICS_EXPORT QicsDateTimeCellDisplayFactory: public QicsCellDisplayFactory
{
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};


#endif //QICSDATETIMECELLDISPLAY_H


