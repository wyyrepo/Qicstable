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

#ifndef QICSSHARED_H
#define QICSSHARED_H

/*!
* The QicsShared class is internally used for implementing shared classes.
*
* It only contains a reference count and member functions to increment and
* decrement it.
*
* Shared classes normally have internal classes that inherit QShared and add
* the shared data.
*/

class QicsShared
{
public:
    QicsShared() : m_count(1) {}
    ~QicsShared() {}

    void ref() { m_count++; }
    bool deref() { return !--m_count; }

    unsigned int m_count;
};

#endif //QICSSHARED_H


