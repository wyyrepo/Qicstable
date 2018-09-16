/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef CONSULTANT_H
#define CONSULTANT_H

#include <QValidator>
#include <QMainWindow>
#include <QicsNamespace.h>

////////////////////////////////////////////////////////////////////////
//
// This example program demonstrates the use of a number of QicsTable
// features, including custom cell displayers, validators, and formatters.
//
// The program displays a table when can be loaded with data concerning
// "consultants".  Data can be loaded from, and saved to, a text file.
// Columns in the table are:
//
// First Name:         (string) default cell displayer
// Last Name:          (string) default cell displayer
// SSN:                (int) default cell displayer, custom validator
//                           and formatter
// Date of Birth:      (date) default cell displayer
// Office:             (string) combo cell displayer
// Security Clearance: (string) subclassed check box displayer
// Billable Rate:      (float) default cell displayer, custom formatter
//
////////////////////////////////////////////////////////////////////////

/*
* SSNValidator - A subclass of QValidator that restricts input to
* numeral and '-' chars.   Once the user presses enter, fixup() will
* strip the dashed from the input in order to keep the data consistant.
* A formatter will be used to display the value in the table in the
* common manner (XXX-XX-XXXX).
*/

class SSNValidator : public QValidator
{
public:
    SSNValidator() : QValidator( 0 ) {};
    virtual State validate( QString&, int& ) const;
    virtual void fixup( QString& ) const;
};

////////////////////////////////////////////////////////////////////////

/*
* Consultant - The primary class.  Subclassed from QMainWindow, this
* class contains both the table and the data model, as well as all
* the methods to implement the UI behavior.
*/

class QicsDataModel;
class QicsTable;

class Consultant : public QMainWindow
{
    Q_OBJECT
public:
    Consultant(QWidget*parent=0);

protected slots:
    void addRow();
    void deleteRow();
    void sortAscending();
    void sortDescending();
    void fileOpen();
    void fileSave();
    void about();
    void autoSort(int, int, int, const QPoint&);

private:
    QicsDataModel *m_dataModel;
    QicsTable *m_table;
    Qics::QicsSortOrder m_autoSortOrder;
    int m_autoSortCol;
};

#endif //CONSULTANT_H


