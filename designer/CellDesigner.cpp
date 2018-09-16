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

#include "CellDesigner.h"

#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowCursorInterface>
#include <QicsTextCellDisplay.h>
#include <QicsPushButtonCellDisplay.h>
#include <QicsDateTimeCellDisplay.h>
#include <QicsComboCellDisplay.h>
#include <QicsCheckCellDisplay.h>
#include <QicsRow.h>
#include <QicsColumn.h>
#include <QicsCell.h>
#include <QicsSelection.h>
#include <QicsMainGrid.h>
#include <QicsTable.h>

#define CH "w"


CellDesigner::CellDesigner(QicsTable* designersTable, QicsTable* table, CoordType coords, ConfigType type,
                           int row, int col, QWidget* parent)
    : QDialog(parent), m_table(table)
{
    m_designersTable = designersTable;
    setupUi(this);

    m_coords = coords;
    m_type = type;
    m_row = row;
    m_col = col;

    loadCurrentData();
    loadWidgetData();
    connectWidgets();

    connect( cancelButton, SIGNAL(clicked()), this, SLOT(resetTable()));
    connect(displayerBox,SIGNAL(activated(const QString&)), this,SLOT(handleDisplayerBoxActivated(const QString&)));
    connect(editWhenCurrentCheck,SIGNAL(toggled(bool)),this,SLOT(handleTEEditWhenCurrentToggled(bool)));
    connect(addNewButton,SIGNAL(clicked()),this,SLOT(handleCBAddNewClicked()));
    connect(upButton,SIGNAL(clicked()),this,SLOT(handleCBUpClicked()));
    connect(downButton,SIGNAL(clicked()),this,SLOT(handleCBDownClicked()));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(handleCBRemoveClicked()));
    connect(editItemButton,SIGNAL(clicked()),this,SLOT(handleCBEditButtonClicked()));

    connect(comboItemsList,SIGNAL(itemChanged(QListWidgetItem *)), SLOT(setComboItems()));

    connect(displayFormatEdit,SIGNAL(textEdited(const QString&)), SLOT(handleDTSectionsToggled(const QString&)));

    connect(minDateEdit,SIGNAL(dateChanged(const QDate&)), SLOT(handleDTDateChanged(const QDate&)));
    connect(maxDateEdit,SIGNAL(dateChanged(const QDate&)), SLOT(handleDTDateChanged(const QDate&)));
    connect(minTimeEdit,SIGNAL(timeChanged(const QTime&)), SLOT(handleDTTimeChanged(const QTime&)));
    connect(maxTimeEdit,SIGNAL(timeChanged(const QTime&)), SLOT(handleDTTimeChanged(const QTime&)));

    connect(labelEdit,SIGNAL(textChanged(const QString&)), SLOT(handleLabelChanged(const QString&)));
}

void CellDesigner::loadCurrentData()
{
    QicsCellCommon* cellProps  = cellCommonPtr();

    //Get the properties
    m_initalCellProps.heightInChars = m_table->row(row())->heightInChars();
    m_initalCellProps.widthInChars = m_table->column(col())->widthInChars();

    //Span
    QicsRegion span_reg(m_table->cell(m_row, m_col, (m_coords==Model)?true:false)->spannedRegion());
    m_initalCellProps.spannedRows = span_reg.width() ;
    m_initalCellProps.spannedColumns = span_reg.height() ;

    m_initalCellProps.label = cellProps->label();
    m_initalCellProps.enabled = cellProps->enabled();
    m_initalCellProps.readOnly = cellProps->readOnly();
    m_initalCellProps.selected = cellProps->selected();
    m_initalCellProps.font = cellProps->font();
    m_initalCellProps.pal = cellProps->palette();
    m_initalCellProps.foregroundColor = cellProps->foregroundColor();
    m_initalCellProps.backgroundColor = cellProps->palette().color(QPalette::Normal, QPalette::Base);
    m_initalCellProps.selectedForegroundColor = cellProps->selectedForegroundColor();
    m_initalCellProps.selectedBackgroundColor = cellProps->selectedBackgroundColor();
    m_initalCellProps.pixmapSpacing = cellProps->pixmapSpacing();
    m_initalCellProps.alignment = cellProps->alignment();
    m_initalCellProps.textFlags = cellProps->textFlags();
    m_initalCellProps.maxLength = cellProps->maxLength();
    m_initalCellProps.margin = cellProps->margin();

    //_initalCellProps.borderWidth = cellProps->borderWidth();
    //  _initalCellProps.borderStyle = cellProps->borderStyle();
    m_initalCellProps.topBorderPen = cellProps->topBorderPen();
    m_initalCellProps.leftBorderPen = cellProps->leftBorderPen();
    m_initalCellProps.rightBorderPen = cellProps->rightBorderPen();
    m_initalCellProps.bottomBorderPen = cellProps->bottomBorderPen();

    m_initalCellProps.tooltipText = cellProps->toolTipText();
    m_initalCellProps.editForegroundColor = cellProps->editForegroundColor();
    m_initalCellProps.editBackgroundColor = cellProps->editBackgroundColor();

    //        Borde Pen
    m_initalCellProps.topBorderPen = cellProps->topBorderPen();
    m_initalCellProps.leftBorderPen = cellProps->leftBorderPen();
    m_initalCellProps.rightBorderPen = cellProps->rightBorderPen();
    m_initalCellProps.bottomBorderPen = cellProps->bottomBorderPen();

    m_initalCellProps.displayer = cellProps->displayer();
}

void CellDesigner::loadWidgetData()
{
    // General Section
    if ( m_type == Cell ) {
        spanRowsBox->setEnabled(true);
        spanColumnsBox->setEnabled(true);
        spanRowsBox->setValue(m_initalCellProps.spannedRows);
        spanColumnsBox->setValue(m_initalCellProps.spannedColumns);
    }
    else {
        spanRowsBox->setEnabled(false);
        spanColumnsBox->setEnabled(false);
    }

    //Height
    heightBox->setValue(m_initalCellProps.heightInChars);    //Width
    widthBox->setValue(m_initalCellProps.widthInChars);
    fontSelector->setFont(m_initalCellProps.font);

    // Alignments are hard because thye are OR'd values
    // In the Horiz combo 0=Left 1=Right 2=Center
    if( m_initalCellProps.alignment & Qt::AlignHCenter )
        hAlignmentBox->setCurrentIndex(2);
    else if(m_initalCellProps.alignment & Qt::AlignRight )
        hAlignmentBox->setCurrentIndex(1);
    else //Default No alignment
        hAlignmentBox->setCurrentIndex(0);

    // In the Vert combo 0=Bottom 1=Top 2=Center
    if( m_initalCellProps.alignment & Qt::AlignVCenter )
        vAlignmentBox->setCurrentIndex(2);
    else if(m_initalCellProps.alignment & Qt::AlignBottom )
        vAlignmentBox->setCurrentIndex(0);
    else // Default No Alignment
        vAlignmentBox->setCurrentIndex(1);

    toolTipEdit->setText(m_initalCellProps.tooltipText);

    enabledBox->setChecked(m_initalCellProps.enabled);
    readOnlyBox->setChecked(m_initalCellProps.readOnly);
    selectedBox->setChecked(m_initalCellProps.selected);

    //Color Section
    foregroundColorSelector->setColor(m_initalCellProps.foregroundColor);
    backgroundColorSelector->setColor(m_initalCellProps.backgroundColor);
    editingForegroundColorSelector->setColor(m_initalCellProps.editForegroundColor);
    editingBackgroundColorSelector->setColor(m_initalCellProps.editBackgroundColor);
    selectedForegroundColorSelector->setColor(m_initalCellProps.selectedForegroundColor);
    selectedBackgroundColorSelector->setColor(m_initalCellProps.selectedBackgroundColor);

    //    Border Pen
    sideBox->setCurrentIndex(0);
    borderWidthBox->setValue(m_initalCellProps.topBorderPen.width());
    borderStyleBox->setCurrentIndex(m_initalCellProps.topBorderPen.style());
    borderColorSelector->setColor(m_initalCellProps.topBorderPen.color());

    editWhenCurrentCheck->setChecked(m_initalCellProps.displayer->editWhenCurrent());
    QicsCellDisplay* cd = m_initalCellProps.displayer;

    if(cd) {
        if(cd->cellDisplayName() == QicsTextCellDisplay::TextCellDisplayName) {
            displayerBox->setCurrentIndex(displayerBox->findText("Text Display"));
            displayerOptionsStack->setCurrentIndex(displayerBox->findText("Text Display"));
        }
        else if(cd->cellDisplayName() == QicsCheckCellDisplay::CheckCellDisplayName) {
            displayerBox->setCurrentIndex(displayerBox->findText("CheckBox Display"));
            displayerOptionsStack->setCurrentIndex(displayerBox->findText("CheckBox Display"));
        }
        else if(cd->cellDisplayName() == QicsComboCellDisplay::ComboCellDisplayName) {
            displayerBox->setCurrentIndex(displayerBox->findText("ComboBox Display"));
            displayerOptionsStack->setCurrentIndex(displayerBox->findText("ComboBox Display"));
        }
        else if(cd->cellDisplayName() == QicsDateTimeCellDisplay::DateTimeCellDisplayName) {
            displayerBox->setCurrentIndex(displayerBox->findText("DateTime Display"));
            displayerOptionsStack->setCurrentIndex(displayerBox->findText("DateTime Display"));
            displayFormatEdit->setText(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->displayFormat());
            minDateEdit->setDate(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->minimumDate());
            maxDateEdit->setDate(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->maximumDate());
            minTimeEdit->setTime(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->minimumTime());
            maxTimeEdit->setTime(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->maximumTime());
        }
        else if(cd->cellDisplayName() == QicsPushButtonCellDisplay::PushButtonCellDisplayName) {
            displayerBox->setCurrentIndex(displayerBox->findText("PushButton Display"));
            displayerOptionsStack->setCurrentIndex(displayerBox->findText("PushButton Display"));
        }
    }
    setComboItemsList();
    labelEdit->setText(m_initalCellProps.label);

}

void CellDesigner::connectWidgets()
{
    connect(fontSelector, SIGNAL(fontChanged(QFont)), SLOT(handleFontChanged(QFont)));

    connect(enabledBox, SIGNAL(toggled(bool)), SLOT(handleEnabledToggled(bool)));

    connect(foregroundColorSelector, SIGNAL(colorChanged(QColor)), SLOT(handleForegroundColorChanged(QColor)));
    connect(backgroundColorSelector, SIGNAL(colorChanged(QColor)), SLOT(handleBackgroundColorChanged(QColor)));
    connect(editingForegroundColorSelector, SIGNAL(colorChanged(QColor)), SLOT(handleEditingForegroundColorChanged(QColor)));
    connect(editingBackgroundColorSelector, SIGNAL(colorChanged(QColor)), SLOT(handleEditingBackgroundColorChanged(QColor)));
    connect(selectedForegroundColorSelector, SIGNAL(colorChanged(QColor)), SLOT(handleSelectedForegroundColorChanged(QColor)));
    connect(selectedBackgroundColorSelector, SIGNAL(colorChanged(QColor)), SLOT(handleSelectedBackgroundColorChanged(QColor)));

    connect(hAlignmentBox, SIGNAL(activated(QString)), SLOT(handleHAlignmentChanged(QString)));
    connect(vAlignmentBox, SIGNAL(activated(QString)), SLOT(handleVAlignmentChanged(QString)));

    connect(toolTipEdit, SIGNAL(textChanged(QString)), SLOT(handleToolTipChanged(QString)));

    //    Height
    connect(heightBox, SIGNAL(valueChanged(int)), SLOT(handleHeightChanged(int)));
    connect(widthBox, SIGNAL(valueChanged(int)), SLOT(handleWidthChanged(int)));

    //    Border Pen
    connect(sideBox, SIGNAL(currentIndexChanged(int)), SLOT(handleBorderSideChanged(int)));
    connect(borderWidthBox, SIGNAL(valueChanged(int)), SLOT(handleBorderWidthChanged(int)));
    connect(borderStyleBox, SIGNAL(currentIndexChanged(int)), SLOT(handleBorderStyleChanged(int)));
    connect(borderColorSelector, SIGNAL(colorChanged(QColor)), SLOT(handleBorderColorChanged(QColor)));

    //     Spann
    connect(spanRowsBox, SIGNAL(valueChanged(int)), SLOT(handleSpannedRowsChanged(int)));
    connect(spanColumnsBox, SIGNAL(valueChanged(int)), SLOT(handleSpannedColumnsChanged(int)));
    connect(readOnlyBox, SIGNAL(stateChanged(int)), SLOT(handleReadOnlyToggled(int) ));
}

void CellDesigner::handleToolTipChanged( const QString& newTip)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setToolTipText(newTip);
}

void CellDesigner::handleVAlignmentChanged(const QString& vAlign)
{
    QString hAlign = hAlignmentBox->currentText();
    handleNewAlignment(hAlign, vAlign);
}

void CellDesigner::handleHAlignmentChanged(const QString& hAlign)
{
    QString vAlign = vAlignmentBox->currentText();
    handleNewAlignment(hAlign, vAlign);

}

void CellDesigner::handleNewAlignment( const QString& hAlign, const QString& vAlign)
{
    Qt::Alignment a = 0;
    // In the Vert combo 0=Bottom 1=Top 2=Center
    // In the Horiz combo 0=Left 1=Right 2=Center
    if ( hAlign == "Left" )
        a |= Qt::AlignLeft;
    else if( hAlign == "Right" )
        a |= Qt::AlignRight;
    else
        a |= Qt::AlignHCenter;

    if ( vAlign == "Top" )
        a |= Qt::AlignTop;
    else if( vAlign == "Bottom" )
        a |= Qt::AlignBottom;
    else
        a |= Qt::AlignVCenter;

    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setAlignment(a);
}

void CellDesigner::handleForegroundColorChanged(const QColor& newColor)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setForegroundColor(newColor);
}

void CellDesigner::handleBackgroundColorChanged(const QColor& newColor)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setBackgroundColor(newColor);
}

void CellDesigner::handleEditingForegroundColorChanged(const QColor& newColor)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setEditForegroundColor(newColor);
}

void CellDesigner::handleEditingBackgroundColorChanged(const QColor& newColor)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setEditBackgroundColor(newColor);
}

void CellDesigner::handleSelectedForegroundColorChanged(const QColor& newColor)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setSelectedForegroundColor(newColor);
}

void CellDesigner::handleSelectedBackgroundColorChanged(const QColor& newColor)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setSelectedBackgroundColor(newColor);
}

void CellDesigner::handleFontChanged(const QFont& font)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setFont(font);
}

void CellDesigner::handleEnabledToggled(bool enabled)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setEnabled(enabled);
}

void CellDesigner::handleReadOnlyToggled(int intState)
{
    bool state = false;
    if (intState == 2)
        state = true;
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setReadOnly(state);
}

void CellDesigner::handleSelectedChanged(bool selected)
{
    Q_UNUSED(selected);
    QicsCellCommon* cellProps = cellCommonPtr();

    if (cellProps->selected()) {
        QicsSelection sel(m_row, m_col, 0, 0, false);
        m_table->addSelection(sel);
    }
    else {
        QicsSelection sel(m_row, m_col, 0, 0, true);
        m_table->addSelection(sel);
    }
}

// -----------------BORDER PEN-----------------------------------------------

void CellDesigner::handleBorderSideChanged(int newSide)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    //   updating buttons for correct side
    QPen pen;

    switch (newSide)
    {
    case 0:
        pen = cellProps->topBorderPen();
        break;
    case 1:
        pen = cellProps->leftBorderPen();
        break;
    case 2:
        pen = cellProps->rightBorderPen();
        break;
    case 3:
        pen = cellProps->bottomBorderPen();
        break;
    default:
        pen = cellProps->topBorderPen();
    }

    borderWidthBox->blockSignals(true);
    borderWidthBox->setValue(pen.width());
    borderWidthBox->blockSignals(false);

    borderStyleBox->blockSignals(true);
    borderStyleBox->setCurrentIndex(pen.style());
    borderStyleBox->blockSignals(false);

    borderColorSelector->blockSignals(true);
    borderColorSelector->setColor(pen.color());
    borderColorSelector->blockSignals(false);
}

void CellDesigner::handleBorderWidthChanged(int newBorder)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    QPen pen;

    switch(sideBox->currentIndex())
    {
    case 0: { //topBorderPen
        pen = cellProps->topBorderPen();
        pen.setWidth(newBorder);
        cellProps->setTopBorderPen(pen);
        m_table->repaint();
        break;
            }
    case 1: { //leftBorderPen
        pen = cellProps->leftBorderPen();
        pen.setWidth(newBorder);
        cellProps->setLeftBorderPen(pen);
        m_table->repaint();
        break;
            }
    case 2: { //rightBorderPen
        pen = cellProps->rightBorderPen();
        pen.setWidth(newBorder);
        cellProps->setRightBorderPen(pen);
        m_table->repaint();
        break;
            }
    case 3: { //bottomBorderPen
        pen = cellProps->bottomBorderPen();
        pen.setWidth(newBorder);
        cellProps->setBottomBorderPen(pen);
        m_table->repaint();
        break;
            }
    }
}

void CellDesigner::handleBorderStyleChanged(int newStyle)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    QPen pen;

    switch(sideBox->currentIndex())
    {
    case 0: { //topBorderPen
        pen = cellProps->topBorderPen();
        pen.setStyle(Qt::PenStyle(newStyle));
        cellProps->setTopBorderPen(pen);
        m_table->repaint();
        break;
            }
    case 1: { //leftBorderPen
        pen = cellProps->leftBorderPen();
        pen.setStyle(Qt::PenStyle(newStyle));
        cellProps->setLeftBorderPen(pen);
        m_table->repaint();
        break;
            }
    case 2: { //rightBorderPen
        pen = cellProps->rightBorderPen();
        pen.setStyle(Qt::PenStyle(newStyle));
        cellProps->setRightBorderPen(pen);
        m_table->repaint();
        break;
            }
    case 3: { //bottomBorderPen
        pen = cellProps->bottomBorderPen();
        pen.setStyle(Qt::PenStyle(newStyle));
        cellProps->setBottomBorderPen(pen);
        m_table->repaint();
        break;
            }
    }
}

void CellDesigner::handleBorderColorChanged(const QColor& newColor)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    QPen pen;

    switch(sideBox->currentIndex())
    {
    case 0: { //topBorderPen
        pen = cellProps->topBorderPen();
        pen.setColor(newColor);
        cellProps->setTopBorderPen(pen);
        m_table->repaint();
        break;
            }
    case 1: { //leftBorderPen
        pen = cellProps->leftBorderPen();
        pen.setColor(newColor);
        cellProps->setLeftBorderPen(pen);
        m_table->repaint();
        break;
            }
    case 2: { //rightBorderPen
        pen = cellProps->rightBorderPen();
        pen.setColor(newColor);
        cellProps->setRightBorderPen(pen);
        m_table->repaint();
        break;
            }
    case 3: { //bottomBorderPen
        pen = cellProps->bottomBorderPen();
        pen.setColor(newColor);
        cellProps->setBottomBorderPen(pen);
        m_table->repaint();
        break;
            }
    }
}

// -----------------SPAN-----------------------------------------------

void CellDesigner::handleSpannedRowsChanged(int numRows)
{
    m_table->setRepaintBehavior(Qics::RepaintOff);
    m_table->cell(m_row, m_col, (m_coords==Model)?true:false)->removeSpan();
    m_table->cell(m_row, m_col, (m_coords==Model)?true:false)->addSpan(numRows, spanColumnsBox->value());
    m_table->setRepaintBehavior(Qics::RepaintOn);
}

void CellDesigner::handleSpannedColumnsChanged(int numColumns)
{
    m_table->setRepaintBehavior(Qics::RepaintOff);
    m_table->cell(m_row, m_col, (m_coords==Model)?true:false)->removeSpan();
    m_table->cell(m_row, m_col, (m_coords==Model)?true:false)->addSpan(spanRowsBox->value(), numColumns);
    m_table->setRepaintBehavior(Qics::RepaintOn);
}

// -----------------HEIGHT-----------------------------------------------

void CellDesigner::handleWidthChanged(int newWidth)
{
    m_table->setDefaultColumnWidthInChars( newWidth );
}

void CellDesigner::handleHeightChanged(int newHeight)
{
    m_table->setDefaultRowHeightInChars( newHeight );
}

// -----------------ADDONS-----------------------------------------------

int CellDesigner::row()
{
    return m_row;
}

int CellDesigner::col()
{
    return m_col;
}

QicsCellCommon *CellDesigner::cellCommonPtr()
{
    QicsCellCommon* cellProps = 0;

    bool follow_model = true;

    if (m_coords == Visual)
        follow_model=false;
    else
        follow_model=true;

    if(m_type == Table)
        cellProps = m_table->mainGrid();
    else if(m_type == Row)
        cellProps = m_table->row(m_row, follow_model);
    else if(m_type == Column)
        cellProps = m_table->column(m_col, follow_model);
    else if(m_type == Cell)
        cellProps = m_table->cell(m_row,m_col, follow_model);
    else {
        //Shouldn't get there
        return 0;
    }

    return cellProps;
}

void CellDesigner::resetTable()
{
    QicsCellCommon* cellProps  = cellCommonPtr();

    //Get the properties
    cellProps->setLabel(m_initalCellProps.label);
    cellProps->setEnabled(m_initalCellProps.enabled);
    cellProps->setReadOnly(m_initalCellProps.readOnly);
    //cellProps->setSelected(m_initalCellProps.selected); NO METHOD
    cellProps->setFont(m_initalCellProps.font);
    cellProps->setPalette(m_initalCellProps.pal);
    cellProps->setForegroundColor(m_initalCellProps.foregroundColor);
    cellProps->setBackgroundColor(m_initalCellProps.backgroundColor);
    cellProps->setSelectedForegroundColor(m_initalCellProps.selectedForegroundColor);
    cellProps->setSelectedBackgroundColor(m_initalCellProps.selectedBackgroundColor);
    cellProps->setPixmapSpacing(m_initalCellProps.pixmapSpacing);
    cellProps->setAlignment(m_initalCellProps.alignment);
    //cellProps->setTextFlags(m_initalCellProps.textFlags);
    cellProps->setMaxLength(m_initalCellProps.maxLength);
    cellProps->setMargin(m_initalCellProps.margin);
    //cellProps->setBorderWidth(m_initalCellProps.borderWidth);
    //cellProps->setBorderStyle(m_initalCellProps.borderStyle);

    // border pen
    cellProps->setTopBorderPen(m_initalCellProps.topBorderPen);
    cellProps->setLeftBorderPen(m_initalCellProps.leftBorderPen);
    cellProps->setRightBorderPen(m_initalCellProps.rightBorderPen);
    cellProps->setBottomBorderPen(m_initalCellProps.bottomBorderPen);

    cellProps->setToolTipText(m_initalCellProps.tooltipText);
    cellProps->setEditForegroundColor(m_initalCellProps.editForegroundColor);
    cellProps->setEditBackgroundColor(m_initalCellProps.editBackgroundColor);

    QString xml_string = m_table->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
}

void CellDesigner::clearTable()
{
    QicsCellCommon* cellProps  = cellCommonPtr();

    // Creating table with default values.
    QicsTable* init_table = new QicsTable(0,this);

    cellProps->setLabel(init_table->label());
    cellProps->setEnabled(init_table->mainGridRef().enabled());
    cellProps->setReadOnly(init_table->readOnly());
    cellProps->setFont(init_table->font());
    cellProps->setPalette(init_table->mainGridRef().palette());
    cellProps->setPixmapSpacing(init_table->pixmapSpacing());
    cellProps->setAlignment(init_table->alignment());
    cellProps->setMaxLength(init_table->maxLength());
    cellProps->setMargin(init_table->margin());

    cellProps->setTopBorderPen(init_table->topBorderPen());
    cellProps->setLeftBorderPen(init_table->leftBorderPen());
    cellProps->setRightBorderPen(init_table->rightBorderPen());
    cellProps->setBottomBorderPen(init_table->bottomBorderPen());

    cellProps->setToolTipText(init_table->mainGridRef().toolTipText());
    cellProps->setEditForegroundColor(init_table->editForegroundColor());
    cellProps->setEditBackgroundColor(init_table->editBackgroundColor());

    cellProps->setDisplayer(new QicsTextCellDisplay);
    m_table->clearSelectionList();

    QString xml_string = m_table->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
    delete init_table;
}


void CellDesigner::handleDisplayerBoxActivated(const QString& str)
{
    QicsCellCommon* cellProps = cellCommonPtr();

    if(str == "Text Display") {
        if(cellProps->displayer()->cellDisplayName() != QicsTextCellDisplay::TextCellDisplayName)
            cellProps->setDisplayer(new QicsTextCellDisplay);
    }
    else if(str == "ComboBox Display") {
        if(cellProps->displayer()->cellDisplayName() != QicsComboCellDisplay::ComboCellDisplayName)
            cellProps->setDisplayer(new QicsComboCellDisplay);
    }
    else if(str == "CheckBox Display") {
        if(cellProps->displayer()->cellDisplayName() != QicsCheckCellDisplay::CheckCellDisplayName)
            cellProps->setDisplayer(new QicsCheckCellDisplay);
    }
    else if(str == "DateTime Display") {
        if(cellProps->displayer()->cellDisplayName() != QicsDateTimeCellDisplay::DateTimeCellDisplayName) {
            QicsDateTimeCellDisplay* cd = new QicsDateTimeCellDisplay;
            cellProps->setDisplayer(cd);
            displayFormatEdit->setText(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->displayFormat());
            minDateEdit->setDate(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->minimumDate());
            maxDateEdit->setDate(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->maximumDate());
            minTimeEdit->setTime(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->minimumTime());
            maxTimeEdit->setTime(((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd))->maximumTime());
        }
    }
    else if(str == "PushButton Display") {
        if(cellProps->displayer()->cellDisplayName() != QicsPushButtonCellDisplay::PushButtonCellDisplayName)
            cellProps->setDisplayer(new QicsPushButtonCellDisplay);
    }
}

void CellDesigner::handleTEEditWhenCurrentToggled(bool toggled)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    QicsCellDisplay* cd = cellProps->displayer();
    if(cd && cd->cellDisplayName() == QicsTextCellDisplay::TextCellDisplayName) {
        QicsTextCellDisplay* tcd = new QicsTextCellDisplay;
        tcd->setEditWhenCurrent(toggled);
        cellProps->setDisplayer(tcd);
        QString xml_string = m_table->configurationToXml();
        saveProperty( "globalXmlConfiguration",xml_string);
    }
}

void CellDesigner::handleCBAddNewClicked()
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setText("New Item");
    item->setFlags(Qt::ItemIsSelectable |
        Qt::ItemIsEnabled |
        Qt::ItemIsEditable);
    comboItemsList->addItem(item);
    setComboItems();
}

void CellDesigner::handleCBRemoveClicked()
{
    foreach( QListWidgetItem* item, comboItemsList->selectedItems() )
        delete item;

    setComboItems();
}

void CellDesigner::handleCBUpClicked()
{
    QListWidget* _itemList = comboItemsList;
    foreach( QListWidgetItem* item, _itemList->selectedItems() ) {
        _itemList->selectionModel()->clear();
        int index = _itemList->row(item);
        QListWidgetItem* taken = _itemList->takeItem(index);
        _itemList->insertItem(--index, taken);
        _itemList->setItemSelected(taken, true);
    }
    setComboItems();
}

void CellDesigner::setComboItems()
{
    QicsCellCommon* cellProps = cellCommonPtr();
    QicsCellDisplay* cd = cellProps->displayer();
    if(cd && cd->cellDisplayName() == QicsComboCellDisplay::ComboCellDisplayName) {
        QComboBox* combo = dynamic_cast<QComboBox*>(cd);
        if( NULL != combo ) {
            combo->clear();
            for(int i=0; i<comboItemsList->count(); ++i) {
                QString str = comboItemsList->item(i)->text();
                combo->addItem(str);
            }
        }
    }
}

void CellDesigner::setComboItemsList()
{
    comboItemsList->clear();

    QicsCellCommon* cellProps = cellCommonPtr();
    QicsCellDisplay* cellDisplay = cellProps->displayer();
    if(!cellDisplay)
        return;

    QComboBox* combo = dynamic_cast<QComboBox*>(cellDisplay);

    if( NULL != combo ) {
        for(int i=0; i<combo->count(); ++i) {
            QAbstractItemModel* model = combo->model();
            QModelIndex index = model->index(i,0,QModelIndex());
            QString itemStr = model->data(index, Qt::DisplayRole).toString();
            comboItemsList->addItem(itemStr);
        }
    }
}

void CellDesigner::handleCBDownClicked()
{
    QListWidget* _itemList = comboItemsList;
    foreach( QListWidgetItem* item, _itemList->selectedItems() ) {
        _itemList->selectionModel()->clear();
        int index = _itemList->row(item);
        QListWidgetItem* taken = _itemList->takeItem(index);
        _itemList->insertItem(++index, taken);
        _itemList->setItemSelected(taken, true);
    }
    setComboItems();
}

void CellDesigner::handleCBEditButtonClicked()
{
    foreach( QListWidgetItem* item, comboItemsList->selectedItems() ) {
        item->setFlags(Qt::ItemIsSelectable |
            Qt::ItemIsEnabled |
            Qt::ItemIsEditable);
        comboItemsList->editItem(item);
    }
}

void CellDesigner::handleDTSectionsToggled(const QString & format)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    QicsCellDisplay* cd = cellProps->displayer();
    if(cd && cd->cellDisplayName() == QicsDateTimeCellDisplay::DateTimeCellDisplayName) {
        QDateTimeEdit* de = ((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd));
        de->setDisplayFormat(format);
    }
}

void CellDesigner::saveProperty(QString prop_name,QVariant value)
{
    QDesignerFormWindowInterface *formWindow = QDesignerFormWindowInterface::findFormWindow(m_designersTable);
    if (formWindow)
        formWindow->cursor()->setProperty(prop_name, value);
}

void CellDesigner::handleDTDateChanged(const QDate& date)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    QicsCellDisplay* cd = cellProps->displayer();

    if(cd && cd->cellDisplayName() == QicsDateTimeCellDisplay::DateTimeCellDisplayName) {
        if(sender() == minDateEdit) {
            QDateTimeEdit* de = ((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd));
            de->setMinimumDate(date);
        }
        else if(sender() == maxDateEdit) {
            QDateTimeEdit* de = ((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd));
            de->setMaximumDate(date);
        }
    }
}

void CellDesigner::handleDTTimeChanged(const QTime& time)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    QicsCellDisplay* cd = cellProps->displayer();

    if(cd && cd->cellDisplayName() == QicsDateTimeCellDisplay::DateTimeCellDisplayName) {
        if(sender() == minTimeEdit) {
            QDateTimeEdit* de = ((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd));
            de->setMinimumTime(time);
        }
        else if(sender() == maxTimeEdit) {
            QDateTimeEdit* de = ((QDateTimeEdit*)((QicsDateTimeCellDisplay*)cd));
            de->setMaximumTime(time);
        }
    }
}

void CellDesigner::handleLabelChanged(const QString& str)
{
    QicsCellCommon* cellProps = cellCommonPtr();
    cellProps->setLabel(str);
}

void CellDesigner::accept()
{
    QString xml_string = m_table->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
    QDialog::accept();
}


