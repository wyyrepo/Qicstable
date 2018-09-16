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

#ifndef ROWCOLDESIGNER_H
#define ROWCOLDESIGNER_H

#include "ui_RowColDesigner.h"

class QicsTable;

class RowColDesigner : public QDialog, private Ui::RowColDesigner
{
    Q_OBJECT
public:
    enum RowColType { Columns=0, Rows };

    RowColDesigner(QicsTable* table, RowColType type, QWidget* parent=0);

public slots:
    void handleHideToggled(bool);
    void handleFreezeToggled(bool);
    void handleFreezeAltToggled(bool);
    void upButtonClicked();
    void downButtonClicked();
    void handleCurrentCellChanged(int,int);

protected:
    void saveProperty(QString prop_name,QVariant value);

private:
    QicsTable* m_designersTable;
    RowColType m_rowColType;
};

#endif //ROWCOLDESIGNER_H


