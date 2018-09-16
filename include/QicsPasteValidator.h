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

#ifndef QICSPASTEVALIDATOR_H
#define QICSPASTEVALIDATOR_H

#include "QicsNamespace.h"

class QicsICell;
class QicsSelectionList;
class QicsDataModel;
/*! \class QicsPasteValidator QicsPasteValidator.h
*  \brief Abstract class for paste validator.
*
* Abstract class for paste validator.
* This validator will be used when data is pasted into cell/cells(through drag&drop
* or QicsTable::paste() method).To use it you must subclass it
* and reimplement pure virtual method validate(const QicsDataModel&).
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsPasteValidator
{
public:
    QicsPasteValidator();
    virtual ~QicsPasteValidator();

    /*! Reimplement this method for your own paste validator.
    *  \a dest_cell - destination cell where data is going to be pasted.
    *  \a source_selection - selection from what data is going to be pasted(through drag&drop).
    *   When you are pasting data through QicsTable::copy() and QicsTable::paste() methods or
    *   data from external applications \a source_selection will be a NULL pointer.
    *  \a source_items - const reference to data model that contains items to be pasted.
    *  \a main_dm - pointer to main table's data model.
    */
    virtual bool validate(const QicsICell& dest_cell,const QicsSelectionList* source_selection,
        const QicsDataModel& source_items,const QicsDataModel* main_dm) = 0;
};

#endif //QICSPASTEVALIDATOR_H


