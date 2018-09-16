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

#include <QicsSectionBar.h>

#include <QListWidget>
#include <QPainter>

#include <QicsTreeTable.h>
#include <QicsColumnHeader.h>

/////////////////////////////////////////////////////////////////////////////////
// QicsSectionBar
/////////////////////////////////////////////////////////////////////////////////

QicsSectionBar::QicsSectionBar(QicsTreeTable *table, QWidget *parent)
    : QHeaderView(Qt::Horizontal, parent),
        m_table(table),
        in_progress(false),
        m_closeActive(false)
{
    m_model.insertRow(0);
    setModel(&m_model);

#if QT_VERSION < 0x050000
    setResizeMode(QHeaderView::Stretch);
    setMovable(true);
#else
    setSectionResizeMode(QHeaderView::Stretch);
    setSectionsMovable(true);
#endif
    setHighlightSections(true);
    setDropIndicatorShown(true);
    setFixedHeight(24);

    m_dialog = new QicsCheckPopupDialog(true, false);
    connect(m_dialog, SIGNAL(accepted()), this, SLOT(dialogAccepted()));

    setFrameStyle(QFrame::StyledPanel);
    setFrameShadow(QFrame::Sunken);
}

QicsSectionBar::~QicsSectionBar()
{
    delete m_dialog;
}

void QicsSectionBar::onAdded(int id)
{
    if (in_progress) return;

    int index = m_model.columnCount();
    m_model.insertColumn(index);
    m_model.setHeaderData(index, Qt::Horizontal, id, Qt::UserRole);
    m_model.setHeaderData(index, Qt::Horizontal, m_table->groupText(id), Qt::DisplayRole);
}

void QicsSectionBar::onRemoved(int id)
{
    if (in_progress) return;

    for (int i = 0; i < count(); ++i)
        if (m_model.headerData(i, Qt::Horizontal, Qt::UserRole) == id) {
            m_model.removeColumn(i);
            break;
        }
}

// paint & control facilities
void QicsSectionBar::mouseMoveEvent ( QMouseEvent * event )
{
    m_sect = logicalIndexAt(event->pos());
    if (m_sect != -1) {
        QStyleOptionHeader opt;
        opt.rect.setWidth(sectionSize(m_sect));
        QRect rect = style()->subElementRect(QStyle::SE_HeaderLabel, &opt, this);

        bool b = false, tmp;
        int dx = event->x() - sectionPosition(m_sect);

        // close button state
        int w = dx - rect.x();
        int h = event->y() - rect.y();
        tmp = (w >=0 && w <= 14) && (h >= 4 && h <= 14);
        if (tmp != m_closeActive) {
            m_closeActive = tmp;
            b = true;
        }

        if (extendMouseMove(event, rect, dx))
            b = true;

        if (b) updateSection(m_sect);
    }

    QHeaderView::mouseMoveEvent(event);
}

void QicsSectionBar::mousePressEvent ( QMouseEvent * event )
{
    if (event->button() == Qt::LeftButton) {

        mouseMoveEvent(event);	// need to call, but event - ?

        if (m_closeActive) {
            updateSection(m_sect);
            onSectionClosed(m_sect);
            return;
        }
    }

    QHeaderView::mousePressEvent(event);
}

void QicsSectionBar::leaveEvent ( QEvent * event )
{
    if (m_closeActive) {
        m_closeActive = false;
        updateSection(m_sect);
    }

    QHeaderView::leaveEvent(event);
}

void QicsSectionBar::contextMenuEvent ( QContextMenuEvent * event )
{
    m_dialog->list()->clear();

    QList<int> l;

    for (int i = 0; i < count(); ++i)
        l.append(m_model.headerData(logicalIndex(i), Qt::Horizontal, Qt::UserRole).toInt());

    for (int i = 1; i < m_table->columnCount(); ++i)
        if (!(l.contains(i))) l.append(i);

    for (int i = 0; i < l.count(); ++i) {
        QListWidgetItem *wi = new QListWidgetItem(m_table->groupText(l[i]), m_dialog->list());
        wi->setCheckState(i < count() ?  Qt::Checked : Qt::Unchecked);
        wi->setData(Qt::UserRole, l[i]);
    }

    m_dialog->show(event->globalPos());
}

QSize QicsSectionBar::sectionSizeFromContents(int logicalIndex) const
{
    QSize s = QHeaderView::sectionSizeFromContents(logicalIndex);

    // add size for close button
    s.setWidth(s.width() + 14 + 4);
    return s;
}

void QicsSectionBar::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    QHeaderView::paintSection(painter, rect, logicalIndex);

    QStyleOptionHeader opt;
    opt.initFrom(this);
    opt.state &= ~QStyle::State_MouseOver;

    const QStyleOptionHeader *header = &opt;

    QRegion clipRegion = painter->clipRegion();
    painter->setClipRect(opt.rect);
    QStyleOptionHeader subopt = *header;
    subopt.rect = rect.adjusted(4,4,0,0);

    // should be reimplemented
    drawSection(painter, rect, logicalIndex, opt, subopt);

    if (header->sortIndicator != QStyleOptionHeader::None) {
        subopt.rect = style()->subElementRect(QStyle::SE_HeaderArrow, &opt, this);
        subopt.rect.setRight(subopt.rect.right()-14);
        style()->drawPrimitive(QStyle::PE_IndicatorHeaderArrow, &subopt, painter, this);
    }
    painter->setClipRegion(clipRegion);
}

void QicsSectionBar::drawSection(QPainter *painter, const QRect &rect, int logicalIndex,
                                 QStyleOptionHeader opt, QStyleOptionHeader subopt) const
{
    Q_UNUSED(rect);

    // close button
    QStyleOptionButton button_opt;
    button_opt.state = opt.state;

    QStyleOptionDockWidget dw_opt;
    dw_opt.state = opt.state;

    button_opt.icon = style()->standardIcon(QStyle::SP_TitleBarCloseButton, &dw_opt);
    button_opt.iconSize = QSize(8,8);
    button_opt.rect = subopt.rect;
    button_opt.rect.setSize(QSize(14,14));

    if (logicalIndex == m_sect && m_closeActive)
        button_opt.state |= QStyle::State_MouseOver;

    style()->drawControl(QStyle::CE_PushButton, &button_opt, painter, this);
}


