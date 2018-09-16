/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef FROZEN_H
#define FROZEN_H

#include <QMainWindow>
#include <QDialog>

// The main UI of the example program

class QicsFrozenConfigDialog;
class QicsTable;
class QicsDataModel;
class MoveDialog;

class Frozen: public QMainWindow
{
    Q_OBJECT
public:
    Frozen(QicsDataModel *dm, QWidget *parent = 0);

    inline QicsTable *table() const { return m_table; }

    protected slots:
        void doFrozenConfig();
        void doMoveConfig();
        void about();

protected:
    QicsTable *m_table;
    QicsFrozenConfigDialog *m_frozenDialog;
    MoveDialog *m_moveDialog;
};


// The dialog that allows the user to move rows and columns

class QRadioButton;

class MoveDialog : public QDialog
{
    Q_OBJECT
public:
    MoveDialog(QWidget *parent=0);

    // These methods return the specific type of move that was requested
    inline bool moveToTop() const { return m_moveToTop; }
    inline bool moveToBottom() const { return m_moveToBottom; }
    inline bool moveToLeft() const { return m_moveToLeft; }
    inline bool moveToRight() const { return m_moveToRight; }

public slots:
    // Pops up the dialog and waits until the user pops it down
    virtual int exec();

private:
    QRadioButton *m_top;
    QRadioButton *m_bottom;
    QRadioButton *m_left;
    QRadioButton *m_right;
    bool m_moveToTop;
    bool m_moveToBottom;
    bool m_moveToLeft;
    bool m_moveToRight;
};

#endif //FROZEN_H


