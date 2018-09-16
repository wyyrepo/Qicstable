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

#include <QicsPrintPreviewWidget.h>

#include <QScrollBar>
#include <QKeyEvent>
#include <QLayout>
#include <QPrinter>
#include <QApplication>
#include <QTimer>
#include <QPainter>

#include <QicsPrintPreviewPage.h>
#include <QicsRuler.h>


QicsPrintPreviewWidget::QicsPrintPreviewWidget(QWidget *parent, PageCountMode pcm)
    : QScrollArea(parent)
{
    m_printer = 0;

    m_copies = 1;
    m_printCopyOrder = CO_EVERYPAGE;
    m_printPageOrder = PO_FIRSTPAGE;
    m_printPageSet = PS_ENABLED;
    m_printPageSequence = PR_ALL;

    m_cm = CM_SELECT;
    m_pcm = pcm;

    m_activePage = 0;
    m_showNumbers = true;
    m_activePageJump = false;
    m_zoomOn = true;
    m_showRulers = false;

    m_preview = new QWidget(this);
    setWidget(m_preview);

    m_rulerX = new QicsRuler(Qt::Horizontal, this);
    m_rulerX->setFixedHeight(20);

    m_rulerY = new QicsRuler(Qt::Vertical, this);
    m_rulerY->setFixedWidth(20);

    m_corner = 0;
    setCornerWidget(0);

    showRulers(true);

    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateXRuler(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateYRuler(int)));

    setBackgroundRole(QPalette::Dark);

    m_params.m_ld = LD_HORIZONTAL;
    m_params.m_lo = LO_HORIZONTAL;
    m_params.m_ls = LS_FIXED;
    m_params.m_lc = LC_FIT;
    m_params.m_fitX = 3;
    m_params.m_fitY = 4;
    m_params.m_zoomP = 25;
}

QicsPrintPreviewWidget::~QicsPrintPreviewWidget()
{
    emit terminateCount();

    doDeletePages();
}

void QicsPrintPreviewWidget::setCornerWidget(QWidget *w)
{
    delete m_corner;

    if (w)
        m_corner = w;
    else {
        m_corner = new QFrame(this);
        (qobject_cast<QFrame *>(m_corner))->setFrameShape(QFrame::StyledPanel);
    }

    m_corner->setFixedSize(22,22);
    m_corner->move(0,0);
}

void QicsPrintPreviewWidget::setControlMode(ControlMode cm)
{
    if (m_cm == cm) return;

    m_cm = cm;
    m_modeState = false;

    switch (m_cm)
    {
    case CM_PAN:
        m_preview->setCursor(Qt::OpenHandCursor);
        break;
    default:
        m_preview->setCursor(Qt::ArrowCursor);
    }
}

void QicsPrintPreviewWidget::setPageCountMode(PageCountMode pcm)
{
    m_pcm = pcm;
}

void QicsPrintPreviewWidget::keyPressEvent ( QKeyEvent * event )
{
    if (event->key() == Qt::Key_A && event->modifiers() & Qt::ControlModifier)
        selectAll(true);

    QScrollArea::keyPressEvent(event);
}

void QicsPrintPreviewWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    emit contextMenuRequest(0, event);
}

void QicsPrintPreviewWidget::hideEvent ( QHideEvent * event )
{
    emit terminateCount();

    QScrollArea::hideEvent(event);
}

bool QicsPrintPreviewWidget::eventFilter(QObject *obj, QEvent *event)
{
    QicsPrintPreviewPage *page = dynamic_cast<QicsPrintPreviewPage*>(obj);

    if (event->type() == QEvent::ContextMenu) {
        emit contextMenuRequest(page, (QContextMenuEvent*)event);
        return true;
    }

    if (!page) return false;

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);

        if (me->button() == Qt::RightButton) return true;

        if (m_cm == CM_PAN) {
            m_modeState = true;
            m_modePos = me->globalPos();
            m_preview->setCursor(Qt::ClosedHandCursor);
            return true;
        }

        if (me->modifiers() & Qt::ControlModifier) {
            if (!m_activePage)
                setActivePage(page);
            else {
                if (page->isActive())
                    setActivePage(0);
                page->setSelected(!page->isSelected());
            }
        }
        else {
            selectAll(false);
            setActivePage(page);
        }
        return true;
    }

    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::RightButton) return true;

        if (m_cm == CM_PAN) {
            m_modeState = false;
            m_preview->setCursor(Qt::OpenHandCursor);
            return true;
        }
    }

    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::RightButton) return true;

        if (m_cm == CM_PAN && m_modeState) {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() + m_modePos.x() - me->globalPos().x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() + m_modePos.y() - me->globalPos().y());
            m_modePos = me->globalPos();
            return true;
        }
    }

    if (event->type() == QEvent::MouseButtonDblClick && m_zoomOn) {
        if (m_params.m_zoomP == 100) {
            setPreviewOptions(m_storedParams);
            ensurePageVisible(page);
        }
        else {
            // zoom to 100%
            if (m_params.m_zoomP < 100)
                m_storedParams = m_params;

            m_params.m_ls = LS_FIXED;
            m_params.m_lc = LC_FIT;
            m_params.m_zoomP = 100;
            setPageDirection(m_params.m_ld, 1);

            ensurePageVisible(page);
        }
        return true;
    }

    return QObject::eventFilter(obj, event);
}

void QicsPrintPreviewWidget::resizeEvent ( QResizeEvent * event )
{
    QScrollArea::resizeEvent(event);

    doFullUpdate(HINT_NoZoom | HINT_LayoutOnFitChanged);
}

int QicsPrintPreviewWidget::doCalculateHorizontalFit()
{
    int margin = m_preview->layout() ? m_preview->layout()->margin() : 5;

    int fit = 1;
    int num = m_pages.size();

    int wmax;

    do {
        fit++;
        if (fit > num) break;

        int mx = fit;
        int my = num / fit + !!(num % fit);

        int w = 0;
        wmax = margin;
        int pn = 0;

        for (int i = 0; i < my; ++i) {
            w = margin;
            for (int j = 0; j < mx; ++j) {
                if (m_params.m_lo == LO_HORIZONTAL)
                    pn = i*mx+j;
                else
                    pn = j*my+i;

                if (pn >= num) break;

                w += m_pages[pn]->width() + margin;
            }
            if (w >= wmax) wmax = w;
        }
    }
    while (wmax < viewport()->width());

    return --fit;
}

int QicsPrintPreviewWidget::doCalculateVerticalFit()
{
    int margin = m_preview->layout() ? m_preview->layout()->margin() : 5;

    int fit = 1;
    int num = m_pages.size();

    int hmax;

    do {
        fit++;
        if (fit > num) break;

        int my = fit;
        int mx = num / fit + !!(num % fit);

        int h = 0;
        hmax = margin;
        int pn = 0;

        for (int i = 0; i < mx; ++i) {
            h = margin;
            for (int j = 0; j < my; ++j) {
                if (m_params.m_lo == LO_VERTICAL)
                    pn = i*my+j;
                else
                    pn = j*mx+i;

                if (pn >= num) break;

                h += m_pages[pn]->height() + margin;
            }
            if (h >= hmax) hmax = h;
        }
    }
    while (hmax < viewport()->height());

    return --fit;
}

void QicsPrintPreviewWidget::doZoom()
{
    QSize s(m_printer->paperRect().size());
    s.setWidth(int(m_params.m_zoomP * s.width() / 100));
    s.setHeight(int(m_params.m_zoomP * s.height() / 100));

    int num = m_pages.size();
    for (int i = 0; i < num; ++i)
        m_pages[i]->setFixedSize(s);
}

void QicsPrintPreviewWidget::doCalculateHorizontalZoom()
{
    int margin = m_preview->layout() ? m_preview->layout()->margin() : 5;

    QSize s(m_printer->paperRect().size());

    int fitX = m_params.m_fitX;
    int num = m_pages.size();

    if (m_params.m_lo == LO_VERTICAL) {
        int fitY = (num / fitX) + !!(num % fitX);
        fitX = num / fitY + !!(num % fitY);
    }

    int min = qMin(fitX, num);
    int w = s.width() * min;

    m_params.m_zoomP = (double)(viewport()->width() - margin*(min+1)) / w * 100;
    doZoom();
}


void QicsPrintPreviewWidget::doCalculateVerticalZoom()
{
    int margin = m_preview->layout() ? m_preview->layout()->margin() : 5;

    QSize s(m_printer->paperRect().size());

    int fitY = m_params.m_fitY;
    int num = m_pages.size();

    if (m_params.m_lo == LO_HORIZONTAL) {
        int fitX = (num / fitY) + !!(num % fitY);
        fitY = num / fitX + !!(num % fitX);
    }

    int min = qMin(fitY, m_pages.size());
    int h = s.height() * min;

    m_params.m_zoomP = (double)(viewport()->height() - margin*(min+1)) / h * 100;
    doZoom();
}

void QicsPrintPreviewWidget::doLayoutPages()
{
    // lay out
    QLayout *l = m_preview->layout();
    if (l) delete l;
    l = new QGridLayout(m_preview);

    int x = 0;
    int y = 0;
    int num = m_pages.size();

    int mx = (m_params.m_ld == LD_HORIZONTAL) ? m_params.m_fitX : num / m_params.m_fitY + !!(num % m_params.m_fitY);
    int my = (m_params.m_ld == LD_VERTICAL) ? m_params.m_fitY : num / m_params.m_fitX + !!(num % m_params.m_fitX);

    for (int i = 0; i < num; ++i) {
        ((QGridLayout*)l)->addWidget(m_pages[i], y, x);

        if (m_params.m_lo == LO_HORIZONTAL) {
            if (++x == mx) {
                x = 0;
                y++;
            }
        }
        else {
            if (++y == my) {
                y = 0;
                x++;
            }
        }
        m_pages[i]->show();
    }

    m_preview->setLayout(l);
    m_preview->show();

    m_preview->adjustSize();

    qApp->processEvents();
}

void QicsPrintPreviewWidget::setPageOrdering(PageLayoutOrdering lo)
{
    m_params.m_lo = lo;

    doFullUpdate();
}

void QicsPrintPreviewWidget::setScale(double percent)
{
    if (m_params.m_zoomP == percent) return;

    m_params.m_zoomP = percent;

    doFullUpdate();
}

void QicsPrintPreviewWidget::setPageDirection(PageLayoutDirection dir, int count)       // <=0 = default; >0 = fixed (number)
{
    m_params.m_ld = dir;

    if (count > 0 && m_params.m_lc == LC_FIXED) {
        if (m_params.m_ld == LD_HORIZONTAL)
            m_params.m_fitX = count;
        else
            m_params.m_fitY = count;
    }

    doFullUpdate();
}

void QicsPrintPreviewWidget::setPageScaling(PageLayoutScaling ls)
{
    m_params.m_ls = ls;

    doFullUpdate();
}

void QicsPrintPreviewWidget::setPageCounting(PageLayoutCounting lc)
{
    m_params.m_lc = lc;

    doFullUpdate();
}

void QicsPrintPreviewWidget::setPreviewOptions(const PageLayoutData &opt)
{
    m_params = opt;

    doFullUpdate();
}


void QicsPrintPreviewWidget::updatePages(QPrinter *printer)
{
    if (m_updateInProgress) {
        emit terminateCount();
        m_updateInProgress = false;
    }

    m_printer = printer;

    if (!printer) {
        doDeletePages();
        return;
    }

    m_updateInProgress = true;

    m_storedParams = m_params;

    if (m_pcm == PCM_STATIC) {
        doStaticUpdate();
        m_updateInProgress = false;
    }
    else
        QTimer::singleShot(100, this, SLOT(doDynamicUpdate()));
}


void QicsPrintPreviewWidget::doDynamicUpdate()
{
    doDeletePages();

    int num = 0;
    emit countPages(&num, m_printer);

    m_updateInProgress = false;
}

void QicsPrintPreviewWidget::pageCounted()
{
    if (m_pcm != PCM_DYNAMIC) return;

    QicsPrintPreviewPage *page = new QicsPrintPreviewPage(m_pages.size(), this, m_preview);

    page->installEventFilter(this);

    connect(page, SIGNAL(drawPage(QicsPrintPreviewPage*)),
        this, SIGNAL(drawPage(QicsPrintPreviewPage*)));

    m_pages.append(page);

    doFullUpdate();
}

void QicsPrintPreviewWidget::doStaticUpdate()
{
    int num = 0;
    emit countPages(&num, m_printer);

    if (!num) {
        doDeletePages();
        return;
    }

    if (num == m_pages.count()) {
        // update pages
        for (int i = 0; i < num; ++i)
            m_pages[i]->reinit();

    }
    else {
        // create pages
        doDeletePages();
        m_pages.resize(num);

        for (int i = 0; i < num; ++i) {
            QicsPrintPreviewPage *page = new QicsPrintPreviewPage(i, this, m_preview);

            page->installEventFilter(this);

            connect(page, SIGNAL(drawPage(QicsPrintPreviewPage*)),
                this, SIGNAL(drawPage(QicsPrintPreviewPage*)));

            m_pages[i] = page;
        }
    }

    // total update
    doFullUpdate();
}

void QicsPrintPreviewWidget::doDeletePages()
{
    m_activePage = 0;

    qDeleteAll(m_pages);
    m_pages.clear();

    m_pages.resize(0);
    m_pagesToPrint.clear();
}

void QicsPrintPreviewWidget::doFullUpdate(uint hint)
{
    if (!m_printer) return;

    bool doLayout = true;

    // resize and fit
    if (m_params.m_ls == LS_FIXED) {

        if (!(hint & HINT_NoZoom))
            doZoom();

        if (m_params.m_lc == LC_FIT) {
            if (m_params.m_ld == LD_HORIZONTAL) {
                int fit = m_params.m_fitX;
                m_params.m_fitX = doCalculateHorizontalFit();

                if (hint & HINT_LayoutOnFitChanged && m_params.m_fitX == fit)
                    doLayout = false;
            }
            else
                if (m_params.m_ld == LD_VERTICAL) {
                    int fit = m_params.m_fitY;
                    m_params.m_fitY = doCalculateVerticalFit();

                    if (hint & HINT_LayoutOnFitChanged && m_params.m_fitY == fit)
                        doLayout = false;
                }
        }
    }
    else

        if (m_params.m_ls == LS_FIT && m_params.m_lc == LC_FIXED) {
            if (m_params.m_ld == LD_HORIZONTAL)
                doCalculateHorizontalZoom();
            else
                if (m_params.m_ld == LD_VERTICAL)
                    doCalculateVerticalZoom();
        }

    // layout
    if (doLayout) doLayoutPages();

    // jump to active page
    if (m_activePage && m_activePageJump)
        ensurePageVisible(m_activePage);

    // rulers
    doUpdateRulers();

    // emit
    emit updatePerformed(m_params);
}

void QicsPrintPreviewWidget::setActivePageIndex(int index)
{
    if (index < 0 || index >= m_pages.size()) return;
    setActivePage(m_pages[index]);
}

void QicsPrintPreviewWidget::setActivePage(QicsPrintPreviewPage* page)
{
    if (page == m_activePage) return;

    QicsPrintPreviewPage *old = m_activePage;
    m_activePage = m_pages.contains(page) ? page : 0;
    if (m_activePage) {
        m_activePage->setSelected(true);
        m_activePage->update();
    }
    if (old) old->update();

    doUpdateRulers();
}

void QicsPrintPreviewWidget::doUpdateRulers()
{
    if (m_activePage && m_showRulers) {
        m_rulerX->setFixedWidth(m_activePage->width());
        m_rulerX->setMetrix(m_activePage->widthMM());
        updateXRuler();
        m_rulerX->show();

        m_rulerY->setFixedHeight(m_activePage->height());
        m_rulerY->setMetrix(m_activePage->heightMM());
        updateYRuler();
        m_rulerY->show();
    }
    else {
        m_rulerX->hide();
        m_rulerY->hide();
    }
}

void QicsPrintPreviewWidget::updateXRuler(int)
{
    if (m_activePage && m_showRulers)
        m_rulerX->move(m_activePage->pos().x()+22+m_preview->x()+1, 2);
}

void QicsPrintPreviewWidget::updateYRuler(int)
{
    if (m_activePage && m_showRulers)
        m_rulerY->move(2, m_activePage->pos().y()+22+m_preview->y()+1);
}

void QicsPrintPreviewWidget::ensurePageVisibleIndex(int index)
{
    if (index < 0 || index >= m_pages.size()) return;
    ensurePageVisible(m_pages[index]);
}

void QicsPrintPreviewWidget::ensurePageVisible(QicsPrintPreviewPage* page)
{
    if (!page) return;

    qApp->processEvents();      // make sure that layout complete

    int mx = viewport()->width()-page->width();
    if (mx > 0) mx /= 2;

    int my = viewport()->height()-page->height();
    if (my > 0) my /= 2;

    ensureWidgetVisible(page, mx, my);
}

void QicsPrintPreviewWidget::showPageNumbers(bool show)
{
    if (m_showNumbers == show) return;
    m_showNumbers = show;
    m_preview->update();
}

void QicsPrintPreviewWidget::showRulers(bool show)
{
    if (m_showRulers == show) return;
    m_showRulers = show;

    if (show) {
        setViewportMargins(22,22,0,0);
        m_corner->show();
    } else {
        setViewportMargins(0,0,0,0);
        m_corner->hide();
    }

    doUpdateRulers();
}

void QicsPrintPreviewWidget::selectAll(bool yes)
{
    for (int i = 0; i < m_pages.count(); ++i)
        m_pages[i]->setSelected(yes);
    m_preview->update();
}

void QicsPrintPreviewWidget::enableAll(bool yes)
{
    for (int i = 0; i < m_pages.count(); ++i)
        m_pages[i]->setEnabled(yes);
    m_preview->update();
}

void QicsPrintPreviewWidget::enableSelected(bool yes)
{
    for (int i = 0; i < m_pages.count(); ++i)
        if (m_pages[i]->isSelected())
            m_pages[i]->setEnabled(yes);
    m_preview->update();
}

void QicsPrintPreviewWidget::printAllPages()
{
    if (!m_printer || !m_pages.size()) return;

    QPainter p(m_printer);

    if (m_printer->printerState() != QPrinter::Active) {
        emit printingAborted();
        return;
    }

    for (int i = 0; i < m_pages.size(); ++i) {
        emit pagePrinting(i, m_pages[i]);

        if (i) m_printer->newPage();

        emit printPage(m_pages[i], &p);

        switch (m_printer->printerState())
        {
        case QPrinter::Idle:
        case QPrinter::Aborted:
            emit printingAborted();
            return;
        case QPrinter::Error:
            emit printingError();
            return;
        default:
            emit pagePrinted(i, m_pages[i]);
        }
    }
}

void QicsPrintPreviewWidget::setPagesToPrint(const QList<int>& numbers)
{
    if (numbers.isEmpty()) return;

    setPrintPageSet(PS_CUSTOM);
    m_pagesToPrint = numbers;
}

void QicsPrintPreviewWidget::setPrintCopies(int num)
{
    if (num < 0) num = 0;
    m_copies = num;
}

void QicsPrintPreviewWidget::print()
{
    if (!m_printer || !m_pages.size()) return;

    QPainter p(m_printer);

    if (m_printer->printerState() != QPrinter::Active) {
        emit printingAborted();
        return;
    }

    // make page list
    QList<int> list;
    PrintPageSet set = m_printPageSet;

    switch (set)
    {
    case PS_PRINTER:
        if (m_printer->printRange() == QPrinter::AllPages)
            set = PS_ALL;
        else if (m_printer->printRange() == QPrinter::Selection)
            set = PS_SELECTED;
        else {
            for (int i = m_printer->fromPage(); i <= m_printer->toPage() && i <= m_pages.size(); ++i)
                list.append(i);
            break;
        }
    // fall through
    case PS_ALL:
    case PS_ENABLED:
    case PS_SELECTED:
        for (int i = 1; i <= m_pages.size(); ++i)
            if (set == PS_ALL ||
                (set == PS_ENABLED && m_pages[i-1]->isEnabled()) ||
                (set == PS_SELECTED && m_pages[i-1]->isSelected()) )
                list.append(i);
        break;

    default:
        list = m_pagesToPrint;
    }

    // set pages sequence
    if (m_printPageSequence == PR_ODD) {
        for (int i = list.size()-1; i >= 0; --i)
            if (list[i] & 1) list.removeAt(i);
    }
    else
        if (m_printPageSequence == PR_EVEN) {
            for (int i = list.size()-1; i >= 0; --i)
                if (!(list[i] & 1)) list.removeAt(i);
        }

    // count number of copies
    int copies = m_copies > 0 ? m_copies : m_printer->numCopies();

    int totalPages = copies * list.size();
    if (!totalPages) return;

    // set page print order
    PrintPageOrder order = m_printPageOrder;
    if (m_printPageOrder == PO_PRINTER)
        order = m_printer->pageOrder() == QPrinter::FirstPageFirst ? PO_FIRSTPAGE : PO_LASTPAGE;

    // do printing
    m_newPage = false;
    m_printPageIndex = 0;

    emit printingStarted(totalPages);

    if (order == PO_FIRSTPAGE && m_printCopyOrder == CO_EVERYPAGE) {
        for (int i = 0; i < list.size(); ++i)
            for (int j = 0; j < copies; ++j)
                if (!doPrintPage(list[i], p)) return;
    }
    else
        if (order == PO_FIRSTPAGE && m_printCopyOrder == CO_PAGESET) {
            for (int j = 0; j < copies; ++j)
                for (int i = 0; i < list.size(); ++i)
                    if (!doPrintPage(list[i], p)) return;
        }
        else
            if (order == PO_LASTPAGE && m_printCopyOrder == CO_EVERYPAGE) {
                for (int i = list.size()-1; i >= 0; --i)
                    for (int j = 0; j < copies; ++j)
                        if (!doPrintPage(list[i], p)) return;
            }
            else {
                for (int j = 0; j < copies; ++j)
                    for (int i = list.size()-1; i >= 0; --i)
                        if (!doPrintPage(list[i], p)) return;
            }

    emit printingFinished();
}

bool QicsPrintPreviewWidget::doPrintPage(int number, QPainter &p)
{
    m_printPageIndex++;
    QicsPrintPreviewPage *page = m_pages[number-1];

    emit pagePrinting(m_printPageIndex, page);

    if (m_newPage) m_printer->newPage();
    m_newPage = true;

    emit printPage(page, &p);

    switch (m_printer->printerState())
    {
    case QPrinter::Idle:
    case QPrinter::Aborted:
        emit printingAborted();
        return false;
    case QPrinter::Error:
        emit printingError();
        return false;
    default:
        emit pagePrinted(m_printPageIndex, page);
    }

    return true;
}


