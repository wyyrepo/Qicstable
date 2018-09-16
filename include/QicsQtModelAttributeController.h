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

#ifndef QICSQTMODELATTRIBUTECONTROLLER_H
#define QICSQTMODELATTRIBUTECONTROLLER_H

#include <QAbstractItemModel>
#include "QicsNamespace.h"
#include "QicsCommonAttributeController.h"

/*!
* \class QicsQtModelAttributeController QicsQtModelAttributeController.h
* \brief QicsQtModelAttributeController is a controller for standard Qt models
based on QAbstractItemModel and its descendants.
It supports retrieving and storing of common Qt
data model attributes of a single cell for the native QAbstractItemModel.
Those attributes are:
- foreground and background colors;
- text alignment;
- tooltip;
- decoration (i.e. icon).
When these attributes are set/read, the controller performs lookups and changes in the
QAbstractItemModel. All the rest of QicsCellStyle attributes (such as cell grid
pen, displayers, etc.) are passed through and are
managed by standard QicsStyleManager facilities; they are not stored in the
compound QAbstractItemModel and are exist in the table view only.

The controller should be used in parallel with QicsDataModelQtModelAdapter which is the
interface between Qt and QicsTable data models. It operates directly with the
QicsDataModelQtModelAdapter set as the model for QicsTable.
Here's the example how to work with simple QAbstractTableModel.

\code

// create Qt table data model
QAbstractItemModel *qtmodel = new QAbstractTableModel();

// create an adapter to QicsDataModel
QicsDataModelQtModelAdapter *model = new QicsDataModelQtModelAdapter();
// set qtmodel for the adapter
model->setModel(qtmodel);

// create the table
QicsTable *table = new QicsTable();
// set adapted model for the table
table->setDataModel(model);

// install the controller
QicsQtModelAttributeController controller;
table->setExternalAttributeController(controller);

\endcode
*
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsDataModelQtModelAdapter;


class QICS_EXPORT QicsQtModelAttributeController : public QicsCommonAttributeController
{
public:
    QicsQtModelAttributeController();

    virtual ~QicsQtModelAttributeController();

    virtual QicsAbstractAttributeController* createController()
    { return new QicsQtModelAttributeController(); }

    /*!
    Returns pointer to a property \a name at cell with row \a row and column \a col, 0 if not found.
    */
    virtual void* cellProperty(int row, int col, int name);

    /*!
    Sets a property \a name at cell with row \a row and column \a col to a value of \a val.
    Returns \a true if ok, \a false if such property cannot be set (i.e. not supported).
    */
    virtual bool setCellProperty(int row, int col, int name, const void *val);

    /*!
    Clears property \a name at cell with row \a row and column \a col.
    Returns \a true if ok, \a false if such property cannot be cleared (i.e. not supported).
    */
    virtual bool clearCellProperty(int row, int col, int name);

    virtual bool isPropertySupported(int name);

    virtual void reinit(int rows, int columns);

protected:
    QAbstractItemModel *m_model;
    QicsDataModelQtModelAdapter *m_adapter;

    // flyweights
    QVariant variant;
    bool boolean;
    uint uinteger;
    quintptr pointer;
    QColor clr;
    QFont font;
    QString string;
    QCursor cursor;
    QPen pen;
    QPixmap pixmap;
};

#endif //QICSQTMODELATTRIBUTECONTROLLER_H


