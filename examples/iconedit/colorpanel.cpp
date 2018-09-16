/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "colorpanel.h"

#include <QValidator>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QColorDialog>

#include <QicsMainGrid.h>
#include "rubbergrid.h"


class ColorValidator : public QValidator
{
public:
    ColorValidator(QObject * parent ) : QValidator(parent) {}

    virtual State validate ( QString & input, int & pos ) const
    {
        Q_UNUSED(pos);
        QColor c(input);
        if (c.isValid())
            return QValidator::Acceptable;
        return QValidator::Intermediate;
    }
};


ColorPanel::ColorPanel(QWidget *parent)
    : QDockWidget(parent)
{
    setWindowTitle(tr("Color Panel"));
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QWidget *w = new QWidget(this);
    QVBoxLayout *vbl = new QVBoxLayout(w);
    setWidget(w);

    // create the data model
    m_dm = new QicsDataModelDefault(13,17);

    // create the table
    m_table = new QicsTable(0, 0, RubberGrid::createGrid, 0, m_dm, w);
    vbl->addWidget(m_table);
    m_table->setDefaultColumnWidthInPixels(12);
    m_table->setDefaultRowHeightInPixels(12);
    m_table->mainGridRef().setReadOnly(true);
    m_table->setSelectionPolicy(Qics::SelectNone);
    m_table->setTopHeaderVisible(false);
    m_table->setLeftHeaderVisible(false);

    m_table->setVisibleRows(13);
    m_table->setVisibleColumns(17);

    connect(m_table, SIGNAL(currentCellChanged(int,int)), this, SLOT(cellChanged(int,int)));
    connect(m_table, SIGNAL(pressed(int,int,int,const QPoint&)), this, SLOT(cellClicked(int,int,int,const QPoint&)));

    m_table->show();

    QGridLayout *gl = new QGridLayout();
    vbl->addLayout(gl);

    ColorValidator *cv = new ColorValidator(this);
    QLabel *l = new QLabel(tr("Foreground Color: "), this);
    gl->addWidget(l,0,0);

    m_leColor = new QLineEdit(this);
    m_leColor->setValidator(cv);
    connect(m_leColor, SIGNAL( editingFinished () ), this, SLOT( textEditedColor () ));
    gl->addWidget(m_leColor,0,1);

    m_tbColor = new QToolButton(this);
    gl->addWidget(m_tbColor,0,2);
    connect(m_tbColor, SIGNAL(clicked()), this, SLOT(pickColor()));

    QLabel *l1 = new QLabel(tr("Background Color: "), this);
    gl->addWidget(l1,1,0);

    m_leColorBG = new QLineEdit(this);
    gl->addWidget(m_leColorBG,1,1);
    m_leColorBG->setValidator(cv);
    connect(m_leColorBG, SIGNAL( editingFinished () ), this, SLOT( textEditedColorBG () ));

    m_tbColorBG = new QToolButton(this);
    gl->addWidget(m_tbColorBG,1,2);
    connect(m_tbColorBG, SIGNAL(clicked()), this, SLOT(pickColorBG()));

    fillColors();
    setColor(Qt::black);
    setColorBG(Qt::white);
}

ColorPanel::~ColorPanel()
{
}

void ColorPanel::fillColors()
{
    m_table->setRepaintBehavior(Qics::RepaintOff);

    int x = 0;
    for (int r = -1; r < 256; r += 16, x++) {
        int c = r==-1?0:r;
        int y = 0;

        m_table->cell(y++,x)->setBackgroundColor(QColor(c,c,c));
        m_table->cell(y++,x)->setBackgroundColor(QColor(0,c,0));
        m_table->cell(y++,x)->setBackgroundColor(QColor(0,c/2,c/2));
        m_table->cell(y++,x)->setBackgroundColor(QColor(c/2,c/2,0));

        m_table->cell(y++,x)->setBackgroundColor(QColor(c,c,0));
        m_table->cell(y++,x)->setBackgroundColor(QColor(c,c/2,0));
        m_table->cell(y++,x)->setBackgroundColor(QColor(c,0,0));
        m_table->cell(y++,x)->setBackgroundColor(QColor(c,0,c/2));

        m_table->cell(y++,x)->setBackgroundColor(QColor(c,0,c));
        m_table->cell(y++,x)->setBackgroundColor(QColor(c/2,0,c));
        m_table->cell(y++,x)->setBackgroundColor(QColor(0,0,c));
        m_table->cell(y++,x)->setBackgroundColor(QColor(0,c/2,c));
        m_table->cell(y++,x)->setBackgroundColor(QColor(0,c,c));
    }

    m_table->setRepaintBehavior(Qics::RepaintOn);
}

void ColorPanel::setColor(const QColor &color)
{
    QPixmap pm(16,16);
    pm.fill(color);
    m_tbColor->setIcon(QIcon(pm));
    m_leColor->setText(color.name());
    m_color = color;
}

void ColorPanel::setColorBG(const QColor &color)
{
    QPixmap pm(16,16);
    pm.fill(color);
    m_tbColorBG->setIcon(QIcon(pm));
    m_leColorBG->setText(color.name());
    m_colorBG = color;
}

void ColorPanel::cellClicked(int r, int c, int button, const QPoint &pos)
{
    Q_UNUSED(pos);
    if (button == Qt::RightButton)
        setColorBG(m_table->cell(r,c)->backgroundColor());
}

void ColorPanel::cellChanged(int r,int c)
{
    setColor(m_table->cell(r,c)->backgroundColor());
}

void ColorPanel::pickColor()
{
    QColor c = QColorDialog::getColor(m_color, this);
    if (c.isValid())
        setColor(c);
}

void ColorPanel::pickColorBG()
{
    QColor c = QColorDialog::getColor(m_colorBG, this);
    if (c.isValid())
        setColorBG(c);
}

void ColorPanel::textEditedColor()
{
    setColor(QColor(m_leColor->text()));
}

void ColorPanel::textEditedColorBG()
{
    setColorBG(QColor(m_leColorBG->text()));
}


