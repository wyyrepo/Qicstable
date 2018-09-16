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

#ifndef TABLEDESIGNER_H
#define TABLEDESIGNER_H

#include <QDialog>

class QMenu;
class QPushButton;
class QicsTable;

class TableDesigner : public QDialog
{
    Q_OBJECT
public:
    TableDesigner(QicsTable* table, QWidget* parent=0);

protected slots:
    void handleClicked(int row, int col, int button, const QPoint &pos);
    void configureDefaultCells();
    void configureModelRowCells();
    void configureModelColumnCells();
    void configureModelCell();
    void clearModelRowCells();
    void clearModelColumnCells();
    void clearModelCell();
    void configureVisualRowCells();
    void configureVisualColumnCells();
    void configureVisualCell();
    void clearVisualRowCells();
    void clearVisualColumnCells();
    void clearVisualCell();
    void saveChanges();

private:
    QicsTable* m_table;
    QicsTable* m_designersTable;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
    QMenu* m_popup;
    int m_lastRow;
    int m_lastColumn;
};

#endif //TABLEDESIGNER_H


