/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "viewOptionsDialog.h"

#include <QColorDialog>
#include <QPainter>
#include "viewSettings.h"


ViewOptionsDialog::ViewOptionsDialog(ViewSettings* viewSettings, QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);
    m_viewSettings = viewSettings;

    connect (gridColorPushButton, SIGNAL(clicked()), this, SLOT(gridColorPushButton_Click()));
    connect (fieldColorPushButton, SIGNAL(clicked()), this, SLOT(fieldColorPushButton_Click()));
    connect (figureColorPushButton, SIGNAL(clicked()), this, SLOT(figureColorPushButton_Click()));
    connect (randomCheckBoc, SIGNAL(stateChanged(int)), this, SLOT(randomCheckBoc_stateChanged(int)));
    connect (buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonBox_Clicked(QAbstractButton*)));

    m_gridColor = m_viewSettings->getGridColor();
    m_fieldColor = m_viewSettings->getFieldColor();
    m_figureColor = m_viewSettings->getFigureColor();

    if (m_viewSettings->getIsRandom())
        randomCheckBoc->setChecked(true);
    else
        randomCheckBoc->setChecked(false);

    if (m_viewSettings->getDisplayerForm() == ViewSettings::TEXT)
        textRadioButton->setChecked(true);
    else if (m_viewSettings->getDisplayerForm() == ViewSettings::CHECK)
        checkRadioButton->setChecked(true);
    else if (m_viewSettings->getDisplayerForm() == ViewSettings::COMBO)
        comboRadioButton->setChecked(true);
    else if (m_viewSettings->getDisplayerForm() == ViewSettings::BUTTON)
        buttonRadioButton->setChecked(true);
}

void ViewOptionsDialog::gridColorPushButton_Click()
{
    m_gridColor = QColorDialog::getColor(m_gridColor);
}

void ViewOptionsDialog::fieldColorPushButton_Click()
{
    m_fieldColor = QColorDialog::getColor(m_fieldColor);
}

void ViewOptionsDialog::figureColorPushButton_Click()
{
    m_figureColor = QColorDialog::getColor(m_figureColor);
}

void ViewOptionsDialog::buttonBox_Clicked(QAbstractButton* btn)
{
    if (btn->text() == "OK") {
        m_viewSettings->setGridColor(m_gridColor);
        m_viewSettings->setFieldColor(m_fieldColor);
        m_viewSettings->setFigureColor(m_figureColor);

        if (randomCheckBoc->isChecked())
            m_viewSettings->setIsRandom(true);
        else
            m_viewSettings->setIsRandom(false);

        if (buttonRadioButton->isChecked())
            m_viewSettings->setDisplayerForm(ViewSettings::BUTTON);
        else if (comboRadioButton->isChecked())
            m_viewSettings->setDisplayerForm(ViewSettings::COMBO);
        else if (checkRadioButton->isChecked())
            m_viewSettings->setDisplayerForm(ViewSettings::CHECK);
        else if (textRadioButton->isChecked())
            m_viewSettings->setDisplayerForm(ViewSettings::TEXT);
    }
}

void ViewOptionsDialog::randomCheckBoc_stateChanged(int state)
{
    Q_UNUSED ( state )
        if (randomCheckBoc->isChecked())
            figureColorPushButton->setEnabled(false);
        else
            figureColorPushButton->setEnabled(true);
}

void ViewOptionsDialog::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);
    QBrush b(m_gridColor);
    QRect rect = colorGridGroupBox->rect();
    QPoint pt = mapTo(this, colorGridGroupBox->pos());
    rect.setX(colorGridGroupBox->x() + colorGroupBox->x());
    rect.setY(colorGridGroupBox->y() + colorGroupBox->y());
    rect.setWidth(colorGridGroupBox->rect().width());
    rect.setHeight(colorGridGroupBox->rect().height());
    p.fillRect(rect, b);

    b.setColor(m_fieldColor);
    rect = colorGridGroupBox->rect();
    pt = mapTo(this, fieldColoGroupBox->pos());
    rect.setX(fieldColoGroupBox->x() + colorGroupBox->x());
    rect.setY(fieldColoGroupBox->y() + colorGroupBox->y());
    rect.setWidth(fieldColoGroupBox->rect().width());
    rect.setHeight(fieldColoGroupBox->rect().height());
    p.fillRect(rect, b);

    b.setColor(m_figureColor);
    rect = colorGridGroupBox->rect();
    pt = mapTo(this, colorGridGroupBox->pos());
    rect.setX(figureColoGroupBox->x() + colorGroupBox->x());
    rect.setY(figureColoGroupBox->y() + colorGroupBox->y());
    rect.setWidth(figureColoGroupBox->rect().width());
    rect.setHeight(figureColoGroupBox->rect().height());
    p.fillRect(rect, b);
    p.end();
}


