/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef _QICSFROZENCONFIGDIALOG_H
#define _QICSFROZENCONFIGDIALOG_H

#include <QDialog>

//////////////////////////////////////////////////////////////////////////////

// The dialog that allows the user to move rows and columns

class QLineEdit;
class QCheckBox;

class QicsFrozenConfigDialog : public QDialog
{
    Q_OBJECT
public:
    QicsFrozenConfigDialog(QWidget *parent=0);

    // These methods return the specific type of freeze that was requested
    inline bool freezeTopRows(void) const
	{ return _freezeTopRows; }
    inline bool freezeBottomRows(void) const
	{ return _freezeBottomRows; }
    inline bool freezeLeftColumns(void) const
	{ return _freezeLeftColumns; }
    inline bool freezeRightColumns(void) const
	{ return _freezeRightColumns; }

    // These methods return the number of frozen rows/columns that were requested
    int numTopFrozenRows(void) const;
    int numBottomFrozenRows(void) const;
    int numLeftFrozenCols(void) const;
    int numRightFrozenCols(void) const;

public slots:
    // Pops up the dialog and waits until the user pops it down
    int exec();

private:
    QLineEdit *_top;
    QLineEdit *_bottom;

    QLineEdit *_left;
    QLineEdit *_right;

    QCheckBox *_frTop;
    QCheckBox *_frBottom;
    QCheckBox *_frLeft;
    QCheckBox *_frRight;

    bool _freezeTopRows;
    bool _freezeBottomRows;
    bool _freezeLeftColumns;
    bool _freezeRightColumns;
};

#endif /* _QICSFROZENCONFIGDIALOG_H */
