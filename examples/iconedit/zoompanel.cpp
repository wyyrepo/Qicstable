/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "zoompanel.h"

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>


ZoomPanel::ZoomPanel(QWidget *parent)
    : QDockWidget(parent)
{
    setWindowTitle(tr("Zoom Panel"));
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);

    QWidget *w = new QWidget(this);
    setWidget(w);

    QGridLayout *gl = new QGridLayout(w);

    gl->addWidget(new QLabel(tr("Actual Size:"), this), 0, 0);
    gl->addWidget(m_actualView = new QLabel(this), 0, 1);

    gl->addWidget(m_size = new QLabel(this), 1, 1);


    gl->addWidget(new QLabel(tr("Zoom Factor:"), this), 2, 0);

    m_zoomBox = new QComboBox(this);
    gl->addWidget(m_zoomBox, 2, 1);
    m_zoomBox->addItem("4x", 4);
    m_zoomBox->addItem("8x", 8);
    m_zoomBox->addItem("12x", 12);
    m_zoomBox->addItem("16x", 16);
    m_zoomBox->addItem("24x", 24);
    m_zoomBox->addItem("32x", 32);
    m_zoomBox->addItem("64x", 64);
    m_zoomBox->setCurrentIndex(3);	// 16x
    connect(m_zoomBox, SIGNAL(activated(int)), this, SLOT(emitZoom(int)));
}

ZoomPanel::~ZoomPanel()
{
}

int ZoomPanel::currentZoom() const
{
    return m_zoomBox->itemData(m_zoomBox->currentIndex()).toInt();
}

void ZoomPanel::setIcon(const QImage& icon)
{
    m_actualView->setPixmap(QPixmap::fromImage(icon));
    m_size->setText(QString("%1 x %2").arg(icon.width()).arg(icon.height()));
}

void ZoomPanel::emitZoom(int i)
{
    emit zoomChanged(m_zoomBox->itemData(i).toInt());
}


