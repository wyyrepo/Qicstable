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

#ifndef CELLDESIGNER_H
#define CELLDESIGNER_H

#include "ui_CellDesigner.h"
#include <QPen>

class QicsTable;
class QicsCellCommon;
class QicsCellDisplay;

class CellDesigner : public QDialog, private Ui::CellDesigner
{
    Q_OBJECT
public:
    enum ConfigType {Table=0, Row, Column, Cell};
    enum CoordType {Model=0, Visual };

    CellDesigner(QicsTable* designersTable, QicsTable* table, CoordType coords, ConfigType type,
        int row, int col, QWidget* parent=0);

public slots:
    void resetTable();
    void clearTable();

protected:
    void loadWidgetData();
    void loadCurrentData();
    void connectWidgets();
    int row();				//visual coords
    int col();				//visual coords
    void saveProperty(QString prop_name,QVariant value);

    QicsCellCommon* cellCommonPtr();

protected slots:
    void handleFontChanged(const QFont&);
    void handleEnabledToggled(bool enabled);
    void handleReadOnlyToggled(int stateCheckBox);
    void handleSelectedChanged(bool selected);
    void handleForegroundColorChanged(const QColor&);
    void handleBackgroundColorChanged(const QColor&);
    void handleEditingForegroundColorChanged(const QColor&);
    void handleEditingBackgroundColorChanged(const QColor&);
    void handleSelectedForegroundColorChanged(const QColor&);
    void handleSelectedBackgroundColorChanged(const QColor&);
    void handleVAlignmentChanged(const QString&);
    void handleHAlignmentChanged(const QString&);
    void handleNewAlignment( const QString& hAlign, const QString& vAlign);
    void handleToolTipChanged( const QString&);
    //    Border Pen
    void handleBorderSideChanged(int newSide);
    void handleBorderWidthChanged(int newBorder);
    void handleBorderStyleChanged(int newStyle);
    void handleBorderColorChanged(const QColor&);
    //    Spann
    void handleSpannedRowsChanged(int);
    void handleSpannedColumnsChanged(int);
    //    Height
    void handleHeightChanged(int);
    void handleWidthChanged(int);
    void handleDisplayerBoxActivated(const QString&);
    void handleTEEditWhenCurrentToggled(bool);
    void handleCBAddNewClicked();
    void handleCBUpClicked();
    void handleCBDownClicked();
    void handleCBRemoveClicked();
    void handleCBEditButtonClicked();
    void handleDTSectionsToggled(const QString &);
    void setComboItems();
    void setComboItemsList();
    void handleDTDateChanged(const QDate&);
    void handleDTTimeChanged(const QTime&);
    void handleLabelChanged(const QString&);

    virtual void accept();

private:
    QicsTable* m_table;
    QicsTable* m_designersTable;
    CoordType m_coords;
    ConfigType m_type;
    int m_row;
    int m_col;

    struct InitalCellProps
    {
        int heightInChars;
        int widthInChars;
        int cellMargin;
        //spaning
        int spannedRows;
        int spannedColumns;

        QString label;
        bool enabled;
        bool readOnly;
        bool selected;
        QFont font;
        QPalette pal;
        QColor foregroundColor;
        QColor backgroundColor;
        QColor selectedForegroundColor;
        QColor selectedBackgroundColor;
        int pixmapSpacing;
        int alignment;
        int textFlags;
        int maxLength;
        int margin;
        int borderWidth;
        QPen topBorderPen;
        QPen leftBorderPen;
        QPen rightBorderPen;
        QPen bottomBorderPen;
        QString tooltipText;
        QColor editForegroundColor;
        QColor editBackgroundColor;
        QicsCellDisplay* displayer;
    } m_initalCellProps;
};

#endif //CELLDESIGNER_H


