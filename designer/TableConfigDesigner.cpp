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

#include "TableConfigDesigner.h"

#include <QFileDialog>
#include <QDesignerFormWindowCursorInterface>
#include <QDesignerFormWindowInterface>

#include <QicsRowHeader.h>
#include <QicsColumnHeader.h>
#include <QicsTable.h>


TableConfigDesigner::TableConfigDesigner(QicsTable* table, QWidget* parent)
    : QDialog(parent),
        m_designersTable(table)
{
    setupUi(this);

    m_moreTextPixmapString = "rightarrow";

    categoryList->setCurrentRow(0);

    resizeableRowsBox->setChecked(m_designersTable->rowHeaderRef().allowUserResize());
    resizeableColumnsBox->setChecked(m_designersTable->columnHeaderRef().allowUserResize());

    connect(resizeableRowsBox,SIGNAL(toggled(bool)), SLOT(handleAllowUserRowResizeChanged(bool)));
    connect(resizeableColumnsBox,SIGNAL(toggled(bool)), SLOT(handleAllowUserColumnResizeChanged(bool)));

    drawPartialCellsBox->setChecked(m_designersTable->drawPartialCells());
    connect(drawPartialCellsBox,SIGNAL(toggled(bool)), m_designersTable,SLOT(setDrawPartialCells(bool)));

    gridCellClippingCombo->setCurrentIndex((int)m_designersTable->gridCellClipping());
    connect(gridCellClippingCombo,SIGNAL(activated(int)), SLOT(handleGridCellClippingChanged(int)));

    movableRowsBox->setChecked(m_designersTable->rowHeaderRef().allowUserMove());
    movableColumnsBox->setChecked(m_designersTable->columnHeaderRef().allowUserMove());

    connect(movableRowsBox,SIGNAL(toggled(bool)), SLOT(handleAllowUserRowMoveChanged(bool)));
    connect(movableColumnsBox,SIGNAL(toggled(bool)), SLOT(handleAllowUserColumnMoveChanged(bool)));

    frameWidthSpin->setValue(m_designersTable->frameLineWidth());
    connect(frameWidthSpin,SIGNAL(valueChanged(int)), m_designersTable,SLOT(setFrameLineWidth(int)));

    connect(frameShadowCombo,SIGNAL(activated(int)), SLOT(handleFrameShadowChanged(int)));
    connect(frameShapeCombo,SIGNAL(activated(int)), SLOT(handleFrameShapeChanged(int)));

    vertColorButton->setColor(m_designersTable->verticalGridLinePen().color());
    horizColorButton->setColor(m_designersTable->horizontalGridLinePen().color());

    connect(vertColorButton, SIGNAL(colorChanged(QColor)), SLOT(handleVerticalGridLineColorChanged(QColor)));
    connect(horizColorButton, SIGNAL(colorChanged(QColor)), SLOT(handleHorizontalGridLineColorChanged(QColor)));

    vertStyleCombo->setCurrentIndex(m_designersTable->verticalGridLineStyle());
    horizStyleCombo->setCurrentIndex(m_designersTable->horizontalGridLineStyle());

    connect(vertStyleCombo, SIGNAL(activated(int)), SLOT(handleVerticalGridLineStyleChanged(int)));
    connect(horizStyleCombo, SIGNAL(activated(int)), SLOT(handleHorizontalGridLineStyleChanged(int)));

    vertWidthSpin->setValue(m_designersTable->verticalGridLineWidth());
    horizWidthSpin->setValue(m_designersTable->horizontalGridLineWidth());

    connect(vertWidthSpin, SIGNAL(valueChanged(int)), SLOT(handleVerticalGridLineWidthChanged(int)));
    connect(horizWidthSpin, SIGNAL(valueChanged(int)), SLOT(handleHorizontalGridLineWidthChanged(int)));

    verticalGridLinesBox->setChecked(m_designersTable->verticalGridLinesVisible());
    horizontalGridLinesBox->setChecked(m_designersTable->horizontalGridLinesVisible());

    connect(verticalGridLinesBox,SIGNAL(toggled(bool)), SLOT(handleVerticalGridLinesVisibleChanged(bool)));
    connect(horizontalGridLinesBox,SIGNAL(toggled(bool)), SLOT(handleHorizontalGridLinesVisibleChanged(bool)));

    cellOverflowCombo->setCurrentIndex(m_designersTable->cellOverflowBehavior());
    connect(cellOverflowCombo,SIGNAL(activated(int)), SLOT(handleCellOverflowBehaviorChanged(int)));

    maxOverflowCellsSpin->setValue(m_designersTable->maxOverflowCells());
    connect(maxOverflowCellsSpin,SIGNAL(valueChanged(int)), m_designersTable,SLOT(setMaxOverflowCells(int)));

    autoSelectContentsBox->setChecked(m_designersTable->autoSelectCellContents());

    connect(autoSelectContentsBox,SIGNAL(toggled(bool)), m_designersTable,SLOT(setAutoSelectCellContents(bool)));

    traversalEditBehaviorCombo->setCurrentIndex(m_designersTable->traversalKeyEditBehavior());

    connect(traversalEditBehaviorCombo,SIGNAL(activated(int)), SLOT(handleTraversalEditBehaviorChanged(int)));

    if(m_designersTable->enterTraversalDirection() == Qt::Vertical)
        enterTraversalCombo->setCurrentIndex(0);
    else
        enterTraversalCombo->setCurrentIndex(1);

    connect(enterTraversalCombo,SIGNAL(activated(int)), SLOT(handleEnterTraversalDirectionChanged(int)));

    if(m_designersTable->tabTraversalDirection() == Qt::Horizontal)
        tabTraversalCombo->setCurrentIndex(0);
    else
        tabTraversalCombo->setCurrentIndex(1);

    connect(tabTraversalCombo,SIGNAL(activated(int)), SLOT(handleTabTraversalDirectionChanged(int)));

    moreTextPixmapEdit->setText(m_moreTextPixmapString);
    connect(moreTextPixmapButton,SIGNAL(clicked()), SLOT(handleMoreTextPixmapClicked()));
}

void TableConfigDesigner::handleAllowUserRowResizeChanged(bool yes)
{
    m_designersTable->rowHeaderRef().setAllowUserResize(yes);
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
}

void TableConfigDesigner::handleAllowUserColumnResizeChanged(bool yes)
{
    m_designersTable->columnHeaderRef().setAllowUserResize(yes);
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
}

void TableConfigDesigner::handleAllowUserRowMoveChanged(bool yes)
{
    m_designersTable->rowHeaderRef().setAllowUserMove(yes);
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
}

void TableConfigDesigner::handleAllowUserColumnMoveChanged(bool yes)
{
    m_designersTable->columnHeaderRef().setAllowUserMove(yes);
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
}

void TableConfigDesigner::handleVerticalGridLineColorChanged(QColor color)
{
    QPen pen = m_designersTable->verticalGridLinePen();
    pen.setColor(color);
    m_designersTable->setVerticalGridLinePen(pen);
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
}

void TableConfigDesigner::handleHorizontalGridLineColorChanged(QColor color)
{
    QPen pen = m_designersTable->horizontalGridLinePen();
    pen.setColor(color);
    m_designersTable->setHorizontalGridLinePen(pen);
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
}

void TableConfigDesigner::handleVerticalGridLineStyleChanged(int val)
{
    m_designersTable->setVerticalGridLineStyle(static_cast<Qics::QicsLineStyle>(val));
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
}

void TableConfigDesigner::handleHorizontalGridLineStyleChanged(int val)
{
    m_designersTable->setHorizontalGridLineStyle(static_cast<Qics::QicsLineStyle>(val));
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
}

void TableConfigDesigner::handleVerticalGridLineWidthChanged(int w)
{
    m_designersTable->setVerticalGridLineWidth(w);
    saveProperty( "verticalGridLineWidth",w );
}

void TableConfigDesigner::handleHorizontalGridLineWidthChanged(int w)
{
    m_designersTable->setHorizontalGridLineWidth(w);
    saveProperty( "horizontalGridLineWidth",w );
}

void TableConfigDesigner::handleHorizontalGridLinesVisibleChanged(bool on)
{
    m_designersTable->setHorizontalGridLinesVisible(on);
    m_designersTable->repaint();
    saveProperty( "horizontalGridLinesVisible",on );
}

void TableConfigDesigner::handleVerticalGridLinesVisibleChanged(bool on)
{
    m_designersTable->setVerticalGridLinesVisible(on);
    saveProperty( "verticalGridLinesVisible",on );
    m_designersTable->repaint();
}

void TableConfigDesigner::handleFrameShapeChanged(int val)
{
    QFrame::Shape shape;
    if(val == 0) //NoFrame
        shape = QFrame::NoFrame;
    else if(val == 1) //Box
        shape = QFrame::Box;
    else if(val == 2) //Panel
        shape = QFrame::Panel;
    else if(val == 3) //StylePanel
        shape = QFrame::StyledPanel;
    else if(val == 4) //HLine
        shape = QFrame::HLine;
    else if(val == 5) //VLine
        shape = QFrame::VLine;
    else              //WinPanel
        shape = QFrame::WinPanel;

    m_designersTable->setFrameShape(shape);
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
    m_designersTable->repaint();
}

void TableConfigDesigner::handleFrameShadowChanged(int val)
{
    QFrame::Shadow shadow;
    if(val == 0) //Plain
        shadow = QFrame::Plain;
    else if(val == 1) //Raised
        shadow = QFrame::Raised;
    else        //Sunken
        shadow = QFrame::Sunken;

    m_designersTable->setFrameShadow(shadow);
    QString xml_string = m_designersTable->configurationToXml();
    saveProperty( "globalXmlConfiguration",xml_string);
    m_designersTable->repaint();
}

void TableConfigDesigner::handleGridCellClippingChanged(int val)
{
    m_designersTable->setGridCellClipping((Qics::QicsGridCellClipping)val);
    saveProperty( "gridCellClipping", (Qics::QicsGridCellClipping)val );
}

void TableConfigDesigner::handleCellOverflowBehaviorChanged(int val)
{
    m_designersTable->setCellOverflowBehavior((Qics::QicsCellOverflowBehavior)val);
    saveProperty( "cellOverFlowBehavior", (Qics::QicsCellOverflowBehavior)val);
}

void TableConfigDesigner::handleTraversalEditBehaviorChanged(int val)
{
    qWarning("traversalKeyEditBehavior" );
    m_designersTable->setTraversalKeyEditBehavior((Qics::QicsTraversalKeyEditBehavior)val);
    saveProperty( "traversalKeyEditBehavior", (Qics::QicsTraversalKeyEditBehavior)val);
}

void TableConfigDesigner::handleEnterTraversalDirectionChanged(int val)
{
    if(val == 0) {
        m_designersTable->setEnterTraversalDirection(Qt::Vertical);
        saveProperty( "enterTraversalDirection", Qt::Vertical);
    }
    else {
        m_designersTable->setEnterTraversalDirection(Qt::Horizontal);
        saveProperty( "enterTraversalDirection", Qt::Horizontal);
    }
}

void TableConfigDesigner::handleTabTraversalDirectionChanged(int val)
{
    if(val == 1) {
        m_designersTable->setTabTraversalDirection(Qt::Vertical);
        saveProperty( "tabTraversalDirection", Qt::Vertical);
    }
    else {
        m_designersTable->setTabTraversalDirection(Qt::Horizontal);
        saveProperty( "tabTraversalDirection", Qt::Horizontal);
    }
}

void TableConfigDesigner::handleMoreTextPixmapClicked()
{
    QString path = QFileDialog::getOpenFileName (this,"Choose a pixmap");

    if(path.isEmpty())
        return;
    QPixmap pixmap(path);

    m_moreTextPixmapString = path;
    moreTextPixmapEdit->setText(path);
    m_designersTable->setMoreTextPixmap(pixmap);
    saveProperty( "moreTextPixmap", pixmap);
}

void TableConfigDesigner::saveProperty(QString prop_name,QVariant value)
{
    if (QDesignerFormWindowInterface *formWindow
        = QDesignerFormWindowInterface::findFormWindow(m_designersTable))
        formWindow->cursor()->setProperty(prop_name, value);
}


