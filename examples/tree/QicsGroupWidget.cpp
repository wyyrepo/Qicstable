/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "QicsGroupWidget.h"

#include "QicsTreeTable.h"
#include "QicsFilterGroupWidget.h"

#include <QicsRowColumnDrag.h>


QicsGroupWidget::QicsGroupWidget(QicsTreeTable *table, QWidget *parent)
    : QLabel(parent), m_table(table)
{
    setAcceptDrops(true);

    setFrameStyle(QFrame::StyledPanel);
    setFrameShadow(QFrame::Sunken);

    connect(m_table, SIGNAL(groupingChanged()), this, SLOT(updateView()));

    m_dialog = new QicsFilterGroupWidget(m_table, this);

    updateView();
}

QicsGroupWidget::~QicsGroupWidget()
{
}

void QicsGroupWidget::updateView()
{
    const QList<int> &groups = m_table->groups();
    int size = groups.size();
    setText(tr("Columns grouped: %1").arg(size));

    //QicsDataModel* model = m_table->columnHeaderRef().gridInfo().dataModel();

    QString tip = QString("<b>Columns grouped: %1</b><hr><ul>").arg(size);

    for (int i = 0; i < size; ++i)
        tip += "<li>" + m_table->groupText(groups.at(i));
    tip += "</li>";

    setToolTip(tip);
}

void QicsGroupWidget::mousePressEvent ( QMouseEvent * event )
{
    event->accept();

    if (event->button() == Qt::RightButton)
        m_dialog->show(event->globalPos());
}

void QicsGroupWidget::mouseDoubleClickEvent ( QMouseEvent * event )
{
    event->accept();

    if (event->button() == Qt::LeftButton)
        m_dialog->show(event->globalPos());
}

void QicsGroupWidget::dragEnterEvent ( QDragEnterEvent * event )
{
    if (event->mimeData()->hasFormat(QICS_MIME_COLUMNLIST)) {
        event->accept();
        return;
    }

    event->ignore();
}

void QicsGroupWidget::dropEvent ( QDropEvent * event )
{
    if (event->mimeData()->hasFormat(QICS_MIME_COLUMNLIST)) {
        QVector<int> cols;
        QList<int> group = m_table->groups();
        QicsRowColumnDrag::decode(event->mimeData(), Qics::ColumnIndex, cols);

        for (int i = 0; i < cols.count(); ++i)
            group.append(cols.at(i));
        m_table->groupColumns(group);

        event->accept();
        return;
    }

    event->ignore();
}


