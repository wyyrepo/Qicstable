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

#include "RowColDesigner.h"

#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowCursorInterface>

#include <QicsSelection.h>
#include <QicsColumn.h>
#include <QicsRow.h>


RowColDesigner::RowColDesigner(QicsTable* table, RowColType type, QWidget* parent)
    : QDialog(parent),
        m_designersTable(table),
        m_rowColType(type)
{
    setupUi(this);

    previewTable->setDataModel( m_designersTable->dataModel() );
    previewTable->configureFromXml( m_designersTable->configurationToXml() );

    if(type == Columns) {
        freezeCheck->setText("Freeze Left");
        freezeCheckAlt->setText("Freeze Right");
    }

    connect(previewTable,SIGNAL(currentCellChanged(int,int)),this,SLOT(handleCurrentCellChanged(int,int)));
    connect(hideCheck,SIGNAL(toggled(bool)),this,SLOT(handleHideToggled(bool)));
    connect(freezeCheck,SIGNAL(toggled(bool)),this,SLOT(handleFreezeToggled(bool)));
    connect(freezeCheckAlt,SIGNAL(toggled(bool)),this,SLOT(handleFreezeToggled(bool)));
}

void RowColDesigner::handleHideToggled(bool checked)
{
    QicsSelectionList* list = previewTable->selectionList();
    if(!list || list->isEmpty())
        return;

    if(checked) {
        if(m_rowColType == Columns) {
            foreach(QicsSelection sel, *list) {
                int endColumn = sel.endColumn();
                if(sel.endColumn() == Qics::QicsLAST_COLUMN)
                    endColumn = m_designersTable->dataModel()->numColumns();
                for(int i=sel.leftColumn();i<=endColumn;++i) {
                    m_designersTable->columnRef(i).hide();
                    previewTable->columnRef(i).setLabel("hidden");
                }
            }
        }
        else {
            foreach(QicsSelection sel, *list) {
                int endRow = sel.endRow();
                if(sel.endRow() == Qics::QicsLAST_ROW)
                    endRow = m_designersTable->dataModel()->numRows();
                for(int i=sel.topRow();i<=endRow;++i) {
                    m_designersTable->rowRef(i).hide();
                    previewTable->rowRef(i).setLabel("hidden");
                }
            }
        }
    }
    else {
        if(m_rowColType == Columns) {
            foreach(QicsSelection sel, *list) {
                int endColumn = sel.endColumn();
                if(sel.endColumn() == Qics::QicsLAST_COLUMN)
                    endColumn = m_designersTable->dataModel()->numColumns();
                for(int i=sel.leftColumn();i<=endColumn;++i) {
                    m_designersTable->columnRef(i).show();
                    previewTable->columnRef(i).setLabel(QString());
                }
            }
        }
        else {
            foreach(QicsSelection sel, *list) {
                int endRow = sel.endRow();
                if(sel.endRow() == Qics::QicsLAST_ROW)
                    endRow = m_designersTable->dataModel()->numRows();
                for(int i=sel.topRow();i<=endRow;++i) {
                    m_designersTable->rowRef(i).show();
                    previewTable->rowRef(i).setLabel(QString());
                }
            }
        }
    }

    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
    m_designersTable->repaint();
}

void RowColDesigner::handleFreezeToggled(bool checked)
{
    QicsSelectionList* list = previewTable->selectionList();

    bool flag = (sender() == freezeCheck) ? true : false;

    if(checked && list && !list->isEmpty()) {
        if(m_rowColType == Rows ) {
            int count_to_freeze = 0;
            QVector<int>rows_to_move;

            foreach(QicsSelection sel,*list) {
                count_to_freeze += sel.numRows();

                for(int i = sel.topRow();i<=sel.endRow();++i)
                    rows_to_move.append(i);
            }

            if(flag) {
                previewTable->moveRows(0,rows_to_move);
                previewTable->freezeTopRows(count_to_freeze);

                m_designersTable->moveRows(0,rows_to_move);
                m_designersTable->freezeTopRows(count_to_freeze);
            }
            else {
                previewTable->moveRows(m_designersTable->dataModel()->numRows(),rows_to_move);
                previewTable->freezeBottomRows(count_to_freeze);

                previewTable->moveRows(m_designersTable->dataModel()->numRows(),rows_to_move);
                m_designersTable->freezeBottomRows(count_to_freeze);
            }
            previewTable->clearSelectionList();
            m_designersTable->clearSelectionList();
        }
        else {
            int count_to_freeze = 0;
            QVector<int>columns_to_move;

            foreach(QicsSelection sel,*list) {
                count_to_freeze += sel.numColumns();

                for(int i = sel.leftColumn();i<=sel.rightColumn();++i)
                    columns_to_move.append(i);
            }

            if(flag) {
                previewTable->moveColumns(0,columns_to_move);
                previewTable->freezeLeftColumns(count_to_freeze);

                m_designersTable->moveColumns(0,columns_to_move);
                m_designersTable->freezeLeftColumns(count_to_freeze);
            }
            else {
                previewTable->moveColumns(m_designersTable->dataModel()->numColumns(),columns_to_move);
                previewTable->freezeRightColumns(count_to_freeze);

                m_designersTable->moveColumns(m_designersTable->dataModel()->numColumns(),columns_to_move);
                m_designersTable->freezeRightColumns(count_to_freeze);
            }

            previewTable->clearSelectionList();
            m_designersTable->clearSelectionList();
        }
    }
    else {
        if(m_rowColType == Rows ) {
            if(flag) {
                previewTable->unfreezeTopRows();
                m_designersTable->unfreezeTopRows();
            }
            else {
                previewTable->unfreezeBottomRows();
                m_designersTable->unfreezeBottomRows();
            }
        }
        else {
            if(flag) {
                previewTable->unfreezeLeftColumns();
                m_designersTable->unfreezeLeftColumns();
            }
            else {
                previewTable->unfreezeRightColumns();
                m_designersTable->unfreezeRightColumns();
            }
        }
        previewTable->clearSelectionList();
        m_designersTable->clearSelectionList();
    }

    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
    m_designersTable->repaint();

}
void RowColDesigner::handleFreezeAltToggled(bool)
{
}

void RowColDesigner::upButtonClicked()
{
}

void RowColDesigner::downButtonClicked()
{
}

void RowColDesigner::handleCurrentCellChanged(int row ,int col)
{
    if(m_rowColType == Rows)
        hideCheck->setChecked(m_designersTable->rowRef(row).isHidden()? true : false);
    else
        hideCheck->setChecked(m_designersTable->columnRef(col).isHidden()? true : false);
}

void RowColDesigner::saveProperty(QString prop_name,QVariant value)
{
    QDesignerFormWindowInterface *formWindow = QDesignerFormWindowInterface::findFormWindow(m_designersTable);
    if (formWindow)
        formWindow->cursor()->setProperty(prop_name, value);
}


