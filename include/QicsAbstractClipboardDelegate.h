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

#ifndef QICSABSTRACTCLIPBOARDDELEGATE_H
#define QICSABSTRACTCLIPBOARDDELEGATE_H

/*!
* \class QicsAbstractClipboardDelegate QicsAbstractClipboardDelegate.h
* \brief QicsAbstractClipboardDelegate is a delegate class which can be subclassed and
* installed on QicsTable in order to customize clipboard operations.
*
* QicsAbstractClipboardDelegate provides an interface for cut, copy and paste operation.
* Each of its methods can be reimplemented in own class to achieve the desired behavior.
*
* To install the delegate, use QicsTable::setClipboardDelegate() method.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

#include <QObject>
#include "QicsNamespace.h"


class QICS_EXPORT QicsAbstractClipboardDelegate : public QObject
{
public:
    QicsAbstractClipboardDelegate(QObject *parent = 0);
    virtual ~QicsAbstractClipboardDelegate();

    /*!
    * Reimplement this method to redefine standard behavior of cut operation.
    * Reimplemented method must return true to indicate that it handles the action.
    * @param table is the pointer to QicsTable.
    * @param current is current selection list.
    */
    virtual bool cut(class QicsTable* table, class QicsSelectionList* current)
    {
        Q_UNUSED(table);
        Q_UNUSED(current);
        return false;
    }

    /*!
    * Reimplement this method to redefine standard behavior of copy operation.
    * Reimplemented method must return true to indicate that it handles the action.
    * @param table is the pointer to QicsTable.
    * @param current is current selection list.
    */
    virtual bool copy(class QicsTable* table, class QicsSelectionList* current)
    {
        Q_UNUSED(table);
        Q_UNUSED(current);
        return false;
    }

    /*!
    * Reimplement this method to redefine standard behavior of paste operation.
    * Reimplemented method must return true to indicate that it handles the action.
    * @param table is the pointer to QicsTable.
    * @param current is current selection list.
    */
    virtual bool paste(class QicsTable* table, class QicsSelectionList* current)
    {
        Q_UNUSED(table);
        Q_UNUSED(current);
        return false;
    }
};

#endif //QICSABSTRACTCLIPBOARDDELEGATE_H


