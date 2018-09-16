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

#include <QicsSortBar.h>

#include <QHBoxLayout>
#include <QListWidget>
#include <QStylePainter>

#include <QicsTreeTable.h>
#include <QicsRowColumnDrag.h>

/////////////////////////////////////////////////////////////////////////////////
// QicsSortBar
/////////////////////////////////////////////////////////////////////////////////

QicsSortBar::QicsSortBar(QicsTreeTable *table, QWidget *parent)
    : QWidget(parent)
{
    setAcceptDrops(true);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    QHBoxLayout *hbl = new QHBoxLayout(this);
    hbl->setMargin(0);

    m_bar = new QicsSortBarHeader(table, this);
    hbl->addWidget(m_bar);

    btnOrder = new QicsSortArrowButton(this);
    btnOrder->setFixedSize(24,24);
    btnOrder->setToolTip(tr("Change Sort Order"));
    hbl->addWidget(btnOrder);

    connect(btnOrder, SIGNAL(clicked()), this, SLOT(changeOrder()));

    btnClose = new QPushButton(this);
    btnClose->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    btnClose->setFixedSize(24,24);
    btnClose->setToolTip(tr("Unsort All"));
    hbl->addWidget(btnClose);

    connect(btnClose, SIGNAL(clicked()), table, SLOT(unsortRows()));

    connect(table, SIGNAL(rowsSorted()), this, SLOT(onSortingChanged()));
}

void QicsSortBar::onSortingChanged()
{
    btnOrder->setState( m_bar->table()->columnSortOrder() );
}

void QicsSortBar::changeOrder()
{
    Qics::QicsSortOrder ord = (m_bar->table()->columnSortOrder() == Qics::Ascending) ?
        Qics::Descending : Qics::Ascending;

    m_bar->table()->sortRows(m_bar->table()->columnSortVector(), ord);
}

void QicsSortBar::dragEnterEvent ( QDragEnterEvent * event )
{
    if (event->mimeData()->hasFormat(QICS_MIME_COLUMNLIST)) {
        event->accept();
        return;
    }

    event->ignore();
}

void QicsSortBar::dropEvent ( QDropEvent * event )
{
    if (event->mimeData()->hasFormat(QICS_MIME_COLUMNLIST)) {
        QVector<int> cols;
        QVector<int> group = m_bar->table()->columnSortVector();
        QicsRowColumnDrag::decode(event->mimeData(), Qics::ColumnIndex, cols);
        for (int i = 0; i < cols.count(); i++) {
            group.append(cols.at(i));
        }
        m_bar->table()->sortRows(group);

        event->accept();
        return;
    }

    event->ignore();
}


/////////////////////////////////////////////////////////////////////////////////
// QicsSortBarHeader
/////////////////////////////////////////////////////////////////////////////////

QicsSortBarHeader::QicsSortBarHeader(QicsTreeTable *table, QWidget *parent)
    : QicsSectionBar(table, parent)
{
    connect(m_table, SIGNAL(rowsSorted()), this, SLOT(onSortingChanged()));

    connect(this, SIGNAL(sectionDoubleClicked(int)), this, SLOT(onSectionClosed(int)));
    connect(this, SIGNAL(sectionMoved(int,int,int)), this, SLOT(doResort(int,int,int)));

    setToolTip(tr("Right click to alter sorting"));
}

QicsSortBarHeader::~QicsSortBarHeader()
{
}

void QicsSortBarHeader::onSectionClosed(int num)
{
    m_model.removeColumns(num, 1);

    doResort(0,0,0);
}

void QicsSortBarHeader::onSortingChanged()
{
    if (in_progress) return;

    setModel(0);
    m_model.removeColumns(0, m_model.columnCount());

    const QVector<int> &v = m_table->columnSortVector();
    for (int i = 0; i < v.count(); ++i) {
        onAdded(v[i]);
    }

    setModel(&m_model);
}

void QicsSortBarHeader::doResort(int,int,int)
{
    if (!count()) {
        m_table->unsortRows();
        return;
    }

    QVector<int> v(count());

    for (int i = 0; i < count(); ++i)
        v[i] = m_model.headerData(logicalIndex(i), Qt::Horizontal, Qt::UserRole).toInt();

    in_progress = true;
    m_table->sortRows(v, m_table->columnSortOrder());
    in_progress = false;
}

void QicsSortBarHeader::dialogAccepted()
{
    QVector<int> v;

    for (int i = 0; i < m_dialog->list()->count(); ++i) {
        QListWidgetItem *wi = m_dialog->list()->item(i);
        if (wi->checkState() == Qt::Checked) {
            v.append(wi->data(Qt::UserRole).toInt());
        }
    }

    m_table->sortRows(v, m_table->columnSortOrder());
}


/////////////////////////////////////////////////////////////////////////////////
// QicsSortArrowButton
/////////////////////////////////////////////////////////////////////////////////

QicsSortArrowButton::QicsSortArrowButton(QWidget *parent)
    : QPushButton(parent)
{
    m_state = Qics::Ascending;
}

void QicsSortArrowButton::setState(Qics::QicsSortOrder state)
{
    m_state = state;

    update();
}

void QicsSortArrowButton::paintEvent ( QPaintEvent * event )
{
    QPushButton::paintEvent(event);

    QStylePainter p(this);

    QStyleOptionHeader opt;
    opt.rect = rect().adjusted(8,8,-8,-8);
    if (isEnabled())
        opt.state |= (QStyle::State_Enabled | QStyle::State_Active);
    opt.sortIndicator = (m_state == Qics::Ascending) ? QStyleOptionHeader::SortUp : QStyleOptionHeader::SortDown;
    p.drawPrimitive(QStyle::PE_IndicatorHeaderArrow, opt);
}


