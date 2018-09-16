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

#include "QicsGroupBar.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>

#include <QicsTreeTable.h>
#include <QicsColumnHeader.h>
#include <QicsRowColumnDrag.h>
#include <QicsRegexpFilterDelegate.h>
#include <QicsListFilterDelegate.h>



QicsGroupBar::QicsGroupBar(QicsTreeTable *table, QWidget *parent)
    : QWidget(parent)
{
    setAcceptDrops(true);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    QHBoxLayout *hbl = new QHBoxLayout(this);
    hbl->setMargin(0);

    m_bar = new QicsGroupBarHeader(table, this);
    hbl->addWidget(m_bar);

    btnClose = new QPushButton(this);
    btnClose->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    btnClose->setFixedSize(24,24);
    btnClose->setToolTip(tr("Ungroup All"));
    hbl->addWidget(btnClose);

    connect(btnClose, SIGNAL(clicked()), table, SLOT(ungroup()));
}

void QicsGroupBar::dragEnterEvent ( QDragEnterEvent * event )
{
    if (event->mimeData()->hasFormat(QICS_MIME_COLUMNLIST)) {
        event->accept();
        return;
    }

    event->ignore();
}

void QicsGroupBar::dropEvent ( QDropEvent * event )
{
    if (event->mimeData()->hasFormat(QICS_MIME_COLUMNLIST)) {
        QVector<int> cols;
        QList<int> group = m_bar->table()->groups();
        QicsRowColumnDrag::decode(event->mimeData(), Qics::ColumnIndex, cols);
        for (int i = 0; i < cols.count(); i++) {
            group.append(cols.at(i));
        }
        m_bar->table()->groupColumns(group);

        event->accept();
        return;
    }

    event->ignore();
}

/////////////////////////////////////////////////////////////////////////////////
// QicsGroupBarHeader
/////////////////////////////////////////////////////////////////////////////////

QicsGroupBarHeader::QicsGroupBarHeader(QicsTreeTable *table, QWidget *parent)
    : QicsSectionBar(table, parent)
{
    m_dropActive = m_dropPressed = false;

    connect(m_table, SIGNAL(groupAdded(int)), this, SLOT(onAdded(int)));
    connect(m_table, SIGNAL(groupRemoved(int)), this, SLOT(onRemoved(int)));

    connect(m_table, SIGNAL(filterChanged(int)), this, SLOT(onFilterChanged(int)));

    connect(this, SIGNAL(sectionDoubleClicked(int)), this, SLOT(onSectionClosed(int)));
    connect(this, SIGNAL(sectionMoved(int,int,int)), this, SLOT(doRegroup(int,int,int)));

    m_filterDialog = new QicsGroupBarFilterDialog(m_table);

    setToolTip(tr("Right click to alter grouping"));
}

QicsGroupBarHeader::~QicsGroupBarHeader()
{
    delete m_filterDialog;
}

void QicsGroupBarHeader::onAdded(int column)
{
    if (in_progress) return;

    // !!!!!!! workaround due to a bug in QHeaderView when deleting a section after it was moved
    // (falls with exception on drawing, since Qt 4.1.x)
    if (sectionsMoved()) {
        doWorkAround();
        return;
    }
    // !!!!!!! end of workaround

    QicsSectionBar::onAdded(column);

    // update filter state
    onFilterChanged(column);
}

void QicsGroupBarHeader::onRemoved(int column)
{
    if (in_progress) return;

    // !!!!!!! workaround due to a bug in QHeaderView when deleting a section after it was moved
    // (falls with exception on drawing, since Qt 4.1.x)
    if (sectionsMoved()) {
        doWorkAround();
        return;
    }
    // !!!!!!! end of workaround

    QicsSectionBar::onRemoved(column);
}

void QicsGroupBarHeader::doWorkAround()
{
    setModel(0);
    m_model.removeColumns(0, m_model.columnCount());

    QList<int> l = m_table->groups();
    for (int i = 0; i < l.count(); ++i)
        onAdded(l[i]);

    setModel(&m_model);
}

void QicsGroupBarHeader::doRegroup(int,int,int)
{
    if (in_progress) return;

    QList<int> v;

    for (int i = 0; i < count(); ++i)
        v.append(m_model.headerData(logicalIndex(i), Qt::Horizontal, Qt::UserRole).toInt());

    in_progress = true;
    m_table->groupColumns(v);
    in_progress = false;
}

void QicsGroupBarHeader::onFilterChanged(int column)
{
    int index = -1;
    for (int i = 0; i < count(); ++i)
        if (m_model.headerData(i, Qt::Horizontal, Qt::UserRole) == column) {
            index = i;
            break;
        }

    QString s = m_table->groupText(column);

    if (m_table->hasRowFilter(column))
        s += " *";

    m_model.setHeaderData(index, Qt::Horizontal, s, Qt::DisplayRole);
}

void QicsGroupBarHeader::onSectionClosed(int column)
{
    int col = m_model.headerData(column, Qt::Horizontal, Qt::UserRole).toInt();

    m_table->ungroupColumn(col);
}

void QicsGroupBarHeader::dialogAccepted()
{
    QList<int> v;

    for (int i = 0; i < m_dialog->list()->count(); ++i) {
        QListWidgetItem *wi = m_dialog->list()->item(i);
        if (wi->checkState() == Qt::Checked) {
            v.append(wi->data(Qt::UserRole).toInt());
        }
    }

    m_table->groupColumns(v);
}

// paint & control facilities

bool QicsGroupBarHeader::extendMouseMove(QMouseEvent * event, const QRect &rect, int dx)
{
    // drop button state
    int w = rect.right() - dx;
    int h = event->y() - rect.y();
    bool tmp = (w >=0 && w <= 14) && (h >= 2 && h <= 14);

    if (tmp != m_dropActive) {
        m_dropActive = tmp;
        return true;
    }

    return false;
}

void QicsGroupBarHeader::leaveEvent ( QEvent * event )
{
    if (m_closeActive || m_dropActive) {
        m_closeActive = m_dropActive = false;
        updateSection(m_sect);
    }

    QHeaderView::leaveEvent(event);
}

void QicsGroupBarHeader::mousePressEvent ( QMouseEvent * event )
{
    if ((m_sect != -1) && (event->button() == Qt::LeftButton)) {

        mouseMoveEvent(event);	// need to call, but event - ?

        if (m_dropActive) {
            int px = sectionPosition(m_sect) + sectionSize(m_sect);
            int py = height();

            m_filterDialog->show(mapToGlobal(QPoint(px, py)),
                m_model.headerData(m_sect, Qt::Horizontal, Qt::UserRole).toInt());

            m_dropActive = false;
            updateSection(m_sect);
            return;
        }
    }

    QicsSectionBar::mousePressEvent(event);
}

QSize QicsGroupBarHeader::sectionSizeFromContents(int logicalIndex) const
{
    QSize s = QHeaderView::sectionSizeFromContents(logicalIndex);

    // add size for close button and drop down arrow
    s.setWidth(s.width() + 14 + 14 + 4);
    return s;
}

void QicsGroupBarHeader::drawSection(QPainter *painter, const QRect &rect, int logicalIndex,
                                     QStyleOptionHeader opt, QStyleOptionHeader subopt) const
{
    QicsSectionBar::drawSection(painter, rect, logicalIndex, opt, subopt);

    // drop down button
    QStyleOptionComboBox co;
    co.state = opt.state;
    co.rect = subopt.rect.adjusted(0,-2,-4,-2);
    co.rect.setLeft(co.rect.right()-14);
    co.subControls = co.activeSubControls = QStyle::SC_ComboBoxArrow;

    if (logicalIndex == m_sect && m_dropActive) {
        co.state |= QStyle::State_MouseOver;
        if (m_dropPressed)
            co.state |= QStyle::State_Sunken;
    }

    style()->drawComplexControl(QStyle::CC_ComboBox, &co, painter, this);

    subopt.rect.setLeft(subopt.rect.left()+14);
    subopt.rect.setRight(subopt.rect.right()-14);
}

/////////////////////////////////////////////////////////////////////////////////
// QicsGroupBarFilterDialog
/////////////////////////////////////////////////////////////////////////////////

QicsGroupBarFilterDialog::QicsGroupBarFilterDialog(QicsTreeTable *table, QWidget *parent)
    : QicsPopupDialog(parent, QDialogButtonBox::Ok | QDialogButtonBox::Cancel),
        m_table(table), m_group(-1)
{
    twFilter = new QTabWidget(this);
    twFilter->setObjectName(QString::fromUtf8("twFilter"));

    m_checks = new QicsCheckPopup(false, true, twFilter);
    twFilter->addTab(m_checks, tr("List Filter"));

    tab_2 = new QWidget(twFilter);
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    gridLayout = new QGridLayout(tab_2);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    cbRegexpCase = new QCheckBox(tr("Case Sensitive"), tab_2);
    cbRegexpCase->setObjectName(QString::fromUtf8("cbRegexpCase"));

    gridLayout->addWidget(cbRegexpCase, 0, 0, 1, 2);

    cbRegexpMinimal = new QCheckBox(tr("Minimal"), tab_2);
    cbRegexpMinimal->setObjectName(QString::fromUtf8("cbRegexpMinimal"));

    gridLayout->addWidget(cbRegexpMinimal, 0, 2, 1, 1);

    label = new QLabel(tr("Syntax"), tab_2);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 1, 0, 1, 1);

    cbRegexpSyntax = new QComboBox(tab_2);
    cbRegexpSyntax->setObjectName(QString::fromUtf8("cbRegexpSyntax"));

    gridLayout->addWidget(cbRegexpSyntax, 1, 1, 1, 2);

    teRegexpPattern = new QTextEdit(tab_2);
    teRegexpPattern->setObjectName(QString::fromUtf8("teRegexpPattern"));

    gridLayout->addWidget(teRegexpPattern, 2, 0, 1, 3);

    twFilter->addTab(tab_2, tr("Regexp Filter"));
    tab_3 = new QWidget(twFilter);
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    twFilter->addTab(tab_3, tr("No Filter"));

    setWidget(twFilter);
}

void QicsGroupBarFilterDialog::show(QPoint pos, int group)
{
    m_group = group;

    QicsAbstractFilterDelegate *filter = m_table->rowFilter(group);

    if (!filter) {
        twFilter->setCurrentIndex(2);
    }
    else {
        QicsRegexpFilterDelegate *rfd = dynamic_cast<QicsRegexpFilterDelegate*>(filter);
        if (rfd) {
            const QRegExp &regexp = rfd->regexp();
            teRegexpPattern->setText(regexp.pattern());
            cbRegexpCase->setChecked(regexp.caseSensitivity() == Qt::CaseSensitive);
            cbRegexpMinimal->setChecked(regexp.isMinimal());
            cbRegexpSyntax->setCurrentIndex((int)regexp.patternSyntax());

            twFilter->setCurrentIndex(1);
        }
    }

    m_checks->list()->blockSignals(true);
    m_checks->list()->clear();

    QStringList keys = m_table->uniqueKeysForColumn(m_group - !(m_table->treeInHeader()), false);
    QicsListFilterDelegate *lf = dynamic_cast<QicsListFilterDelegate*>(filter);
    QStringList filterList;
    if (lf) {
        twFilter->setCurrentIndex(0);
        filterList = lf->list();
    }
    QStringList::const_iterator it, it_end = keys.end();
    for (it = keys.begin(); it != it_end; ++it) {
        const QString &key = *it;
        QListWidgetItem *wi = new QListWidgetItem(key, m_checks->list());
        wi->setCheckState(!lf || filterList.contains(key) ? Qt::Checked : Qt::Unchecked);
    }

    m_checks->list()->blockSignals(false);
    m_checks->updateCheckBox();

    QicsPopupDialog::show(QPoint(pos.x()-28, pos.y()));
}

void QicsGroupBarFilterDialog::accept()
{
    switch (twFilter->currentIndex()) {
    case 2:
        m_table->removeRowFilter(m_group);
        break;
    case 1: {
            QString s(teRegexpPattern->toPlainText());
            if (s.isEmpty()) {
                m_table->removeRowFilter(m_group);
                break;
            }

            QRegExp regexp(s,
                cbRegexpCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive,
                (QRegExp::PatternSyntax)cbRegexpSyntax->currentIndex());
            regexp.setMinimal(cbRegexpMinimal->isChecked());

            m_table->setRowFilter(m_group, new QicsRegexpFilterDelegate(regexp), true);
        }
        break;
    case 0: {
            QStringList sl;
            for (int i = 0; i < m_checks->list()->count(); ++i) {
                QListWidgetItem *wi = m_checks->list()->item(i);
                if (wi->checkState() == Qt::Checked)
                    sl.append(wi->text());
            }

            if (sl.count() == m_checks->list()->count())
                m_table->removeRowFilter(m_group, true);
            else
                m_table->setRowFilter(m_group, new QicsListFilterDelegate(sl), true);
        }
        break;
    }

    close();
}


