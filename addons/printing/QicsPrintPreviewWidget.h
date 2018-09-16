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

#ifndef QICSPRINTPREVIEWWIDGET_H
#define QICSPRINTPREVIEWWIDGET_H

#include <QScrollArea>
#include <QicsNamespace.h>

class QicsPrintPreviewPage;
class QicsRuler;
class QPrinter;

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Print parameters
///////////////////////////////////////////////////////////////////////////////

#define COPIES_PRINTER	0
/*! \enum PrintCopyOrder
Defines the order in which copies should be printed. See QicsPrintPreviewWidget::setPrintCopyOrder() method for reference.
*/
enum PrintCopyOrder		{ CO_EVERYPAGE, CO_PAGESET };
/*! \enum PrintPageOrder
Defines the order in which pages should be printed. See QicsPrintPreviewWidget::setPrintPageOrder() method for reference.
*/
enum PrintPageOrder		{ PO_FIRSTPAGE, PO_LASTPAGE, PO_PRINTER };
/*! \enum PrintPageSet
Defines the subset of the pages to be printed. See QicsPrintPreviewWidget::setPrintPageSet() method for reference.
*/
enum PrintPageSet		{ PS_ALL, PS_ENABLED, PS_SELECTED, PS_PRINTER, PS_CUSTOM };
/*! \enum PrintPageSequence
Defines the sequence of the pages in the subset to be printed. See QicsPrintPreviewWidget::setPrintPageSequence() method for reference.
*/
enum PrintPageSequence	{ PR_ALL, PR_EVEN, PR_ODD };

///////////////////////////////////////////////////////////////////////////////
// Layout parameters
///////////////////////////////////////////////////////////////////////////////

/*! \enum PageLayoutDirection
The direction in which pages will be layed out. See PageLayoutData for reference.
*/
enum PageLayoutDirection	{ LD_HORIZONTAL, LD_VERTICAL };

/*! \enum PageLayoutOrdering
The direction of pages numeration. See PageLayoutData for reference.
*/
enum PageLayoutOrdering		{ LO_HORIZONTAL, LO_VERTICAL };

/*! \enum PageLayoutScaling
Scaling policy. See PageLayoutData for reference.
*/
enum PageLayoutScaling		{ LS_FIXED, LS_FIT };

/*! \enum PageLayoutCounting
Counting policy. See PageLayoutData for reference.
*/
enum PageLayoutCounting		{ LC_FIXED, LC_FIT };

///////////////////////////////////////////////////////////////////////////////
// PageLayoutData struct - holds page layout parameters
///////////////////////////////////////////////////////////////////////////////

/*!
* \struct PageLayoutData
* \nosubgrouping

This struct is used to setup QicsPrintPreviewWidget's page layout options.
See setPreviewOptions().

There are enumerations for setting current layout policy: #PageLayoutDirection, #PageLayoutOrdering,
#PageLayoutScaling and #PageLayoutCounting. Their combination will inform QicsPrintPreviewWidget about
the way how to layout the pages.

#PageLayoutDirection is used to set up the direction in which pages will be layed out. This can be set
via PageLayoutData::m_ld member.
There are two variants: LD_HORIZONTAL and LD_VERTICAL. LD_HORIZONTAL assumes that pages will be layed out
from left to right, fitting the height of the preview area (viewport). The number of pages to fit is
set via PageLayoutData::m_fitY member.
LD_VERTICAL assumes laying out from top to bottom, fitting the width of the viewport.
The number of pages to fit is set via PageLayoutData::m_fitX member.

#PageLayoutOrdering sets the direction of pages numeration. This can be set via PageLayoutData::m_lo member.
There are two variants: LO_HORIZONTAL and LO_VERTICAL. LO_HORIZONTAL will enumerate pages from left to right,
and LO_VERTICAL will enumerate pages from top to bottom, respectively.

#PageLayoutScaling sets scaling policy. This can be set via PageLayoutData::m_ls member.
It can be one of LS_FIXED or LS_FIT. LS_FIXED means that the current scale always be fixed,
and can be changed via PageLayoutData::m_zoomP member method.
LS_FIT allows the QicsPrintPreviewWidget to set the scale automatically, depending of current viewport
size and fixed pages count.

#PageLayoutCounting sets layout counting policy. This can be set	via PageLayoutData::m_lc member.
It can be one of LC_FIXED or LC_FIT. LC_FIXED means that the fixed number of pages will be placed
horizontally or vertically (depending of current #PageLayoutDirection and corresponding number
of pages set). LC_FIT allows the QicsPrintPreviewWidget to count the number of pages able to
fit into the current viewport and lay pages out correspondingly to this amount.


The images below illustrating how these policies impact on pages layout.

<table align=left>
<tr><td>

\image html layout1.png

</td><td>

Assuming that we have totally 23 pages.

Direction is set to LD_HORIZONTAL, and Counting is LC_FIXED, so
the value of fitY is used to tell QicsPrintPreviewWidget how many horizontal rows of pages to create.

Ordering is LO_HORIZONTAL, so pages are placed and enumerated from left to right.

Scaling is set to LS_FIXED, so all the pages have fixed scale.

</td><tr><td>

\image html layout2.png

</td><td>

Assuming that we have totally 23 pages.

Direction is set to LD_HORIZONTAL, and Counting is LC_FIXED again, so
the value of fitY is used to tell QicsPrintPreviewWidget how many horizontal rows of pages to create.

Ordering is LO_VERTICAL however, so pages are placed and enumerated from top to bottom.

Scaling is set to LS_FIXED, so all the pages have fixed scale.

</td><tr><td>

\image html layout3.png

</td><td>

Assuming that we have totally 23 pages.

Direction is set to LD_VERTICAL, and Ordering is LO_VERTICAL.

But Counting is LC_FIT, so the value used to tell QicsPrintPreviewWidget how many vertical
rows of pages to create is calculated depending of viewport size. Resizing the viewport
will result in recalculation of number of pages to fit and layouting them again.

Scaling is set to LS_FIXED, so all the pages have fixed scale.

</td><tr><td>

\image html layout4.png

</td><td>

Direction is set to LD_VERTICAL, and Ordering is LO_HORIZONTAL.

Counting is LC_FIXED, so the value of fitX is used to tell QicsPrintPreviewWidget how many vertical
rows of pages to create.

Scaling is set to LS_FIT, so the scale of the pages is calculated automatically to fit the
viewport size.

</td></table>
*/

struct QICS_EXPORT PageLayoutData
{
    PageLayoutDirection	m_ld;			//!<	Direction policy
    PageLayoutOrdering	m_lo;			//!<	Ordering policy
    PageLayoutScaling	m_ls;			//!<	Scaling policy
    PageLayoutCounting	m_lc;			//!<	Counting policy
    int m_fitX;             			//!<	Columns of pages to set
    int m_fitY;	                		//!<	Rows of pages to set
    double m_zoomP;	                	//!<	Scale of pages to set
};

///////////////////////////////////////////////////////////////////////////////
// Control modes
///////////////////////////////////////////////////////////////////////////////

/*! \enum ControlMode
Defines user control mode. See QicsPrintPreviewWidget::setControlMode() method for reference.
*/
enum ControlMode		{ CM_SELECT, CM_PAN };
/*! \enum PageCountMode
Defines page counting mode. See QicsPrintPreviewWidget::setPageCountMode() method
and <a href="print_preview.html#counting">Counting the pages</a> topic for reference.
*/
enum PageCountMode		{ PCM_STATIC, PCM_DYNAMIC };

///////////////////////////////////////////////////////////////////////////////
// QicsPrintPreviewWidget class
///////////////////////////////////////////////////////////////////////////////

/*!
* \class QicsPrintPreviewWidget QicsPrintPreviewWidget.h
* \nosubgrouping
* \brief The base class for WYSIWYG printing.

QicsPrintPreviewWidget is the core of \link print_preview Print Preview Framework \endlink
used for WYSIWYG printing. This class allows to display a set of pages before they will be
sent to printer, using several layout policies, to choose printer/paper options, to
customize printing order and number of copies, and more others.

QicsPrintPreviewWidget inherits itself from QScrollArea. It holds QicsPrintPreviewPage
objects each of them representing single page to preview/print. Also, it provides
vertical and horizontal QicsRuler objects used as metrical rulers to view original page
size in millimeters. It provides user interaction to select range of pages, quickly zoom
the current page and so on.


<h3>Designing own preview</h3>

When designing own print preview dialog, QicsPrintPreviewWidget is supposed to be a main
widget. It provides a couple of methods to set page layout policy, numeration, printing
order and so on. Dialog's GUI elements should be responcible to get/set appropriate values
from/to QicsPrintPreviewWidget. \link stocks2_demo Stocks2 \endlink demo shows how to
set up its own preview & printing options functionality.

QicsPrintPreviewWidget contains a QPrinter object which is used for printing and
can be obtained with printer() method. To change current printer and respectively update
the pages, use updatePages(QPrinter *printer) method. The typical usage
is to provide the user with standard QPrintDialog/QPageSetupDialog passing printer()
to its constructor,
and then update pages passing QPrintDialog::printer()/QPageSetupDialog::printer()
back to updatePages() when the user has changed something.

The signals countPages(), drawPage() and printPage() emitted from the widget are obligatory
to handle in the program. countPages() is emitted every time when printer/paper settings
were changed; the handler should count the number of pages fits current settings.
There are two page count modes - \b static and \b dynamic - are used to organize page counting.
Current mode can be set in the constructor or
via setPageCountMode() method. In the dynamic mode, terminateCount() must be also
handled. See <a href="print_preview.html#counting">Counting the pages</a> topic for reference.

drawPage() and printPage() are emitted when a page should be previewed or printed
accordingly; the handlers should perform actual drawing of page's content. See
<a href="print_preview.html#painting">Painting page content</a> topic for reference.

updatePerformed(const PageLayoutData &opt) signal is emitted every time when
the page layout was updated. Handle it to provide corresponding GUI controls update.

To invoke own popup menu, handle contextMenuRequest(QicsPrintPreviewPage *page, QContextMenuEvent* event)
signal. This signal is emitted accordingly to current context menu policy set for
QicsPrintPreviewWidget; see QWidget::customContextMenuRequested() for reference.


<h3>Layouting capabilities</h3>

QicsPrintPreviewWidget has a wide set of options to control automatic page layouting.
There are two ways in which these options could be set/retrieved: using individual methods to
set every option separately (convenient while performing interaction with dialog's control
elements, such as choosing the scale with combobox, and so on), or impact all the options by
one call.

setPreviewOptions(const PageLayoutData &opt) method allows to set all the layout options passed
as \opt parameter; they could be obtained in the same way by calling previewOptions().

There are enumerations for setting current layout policy: #PageLayoutDirection, #PageLayoutOrdering,
#PageLayoutScaling and #PageLayoutCounting. Their combination will inform QicsPrintPreviewWidget about
the way how to layout the pages.

See PageLayoutData description to realize how to setup desired layout policy.





<h3>User interaction</h3>

There are two user control modes available - select and pan - which can be set with setControlMode() method.

In the \b select mode, pages can be
selected with the mouse (single left click). The page which was clicked becomes "active" and
drawn with the green border. To add/remove others pages to/from selection, use Ctrl + left mouse click.
Selected pages are drawn with blue border.

Also, each page can be quickly zoomed to 100% size and unzoomed back to previous state with double-clicking
left mouse button. This behavior can be set with enableClickZoom() method.

Some keyboard combinations provided in this mode. \b Ctrl-A will select all the pages. \b Ctrl-E will
enable selected pages, and \b Ctrl-D will disable them. Disabled pages are marked as crossed-out.
This feature can be useable while choosing which pages to exclude from printing.

See also setActivePage(), setActivePageIndex(), selectAll(), deselectAll(), enableAll(), disableAll(),
enableSelected() and disableSelected() methods.

In the \b pan mode, a "hand" cursor is shown, and you can drag the whole layout to quickly
browse/preview the desired page(s).



<h3>Metrical rulers</h3>

QicsPrintPreviewWidget provides vertical and horizontal metrical rulers to view original page
size in millimeters. It is possible to show/hide these rulers via showRulers() method.


<h3>Page numbers</h3>

Every page has its logical page number used to locate it and make available/not available for
printing. These numbers are shown in the right-bottom corner of every page, independent of
current scale settings. It is possible to show/hide these numbers via showPageNumbers() method.


<h3>Printing</h3>

Generally, actual printing is performed when calling print() or printAllPages() methods.
printAllPages() is intended for the simple printing of all the pages available, one copy,
one-by-one order starting by first page. It does not need any setup.

print() performs more customizable printing. It uses the current printing settings. There are:
- subset of available pages to print;
- order of pages;
- sequence of pages;
- number of copies;
- order of pages in the set (when number of copies > 1).

A subset of pages is defined by #PrintPageSet enum passed to setPrintPageSet() method.

Order in which pages will be sent to printer is defined by #PrintPageOrder enum passed to setPrintPageOrder() method.

Sequence of the subset which will be sent to printer is defined by #PrintPageSequence enum passed to setPrintPageSequence() method.

If the number of copies (set by setPrintCopies()) is more than 1, then order of copies also will be applied.
It is defined by #PrintCopyOrder enum passed to setPrintCopyOrder() method.


<h3>Feedback from print process</h3>

There are signals which should be handled to get feedback from print process.
After print() method was called, printingStarted(int totalPages) signal will be emitted
first. It can be used to set up the GUI indicating how many pages were selected to be printed.
Then, for each page, pagePrinting(int i, QicsPrintPreviewPage *page) and
pagePrinted(int i, QicsPrintPreviewPage *page) signals are emitted before and after
the page was printed, respectively. The signals printingFinished(), printingAborted() or
printingError() will be emitted to indicate if the printing is finished with no issues,
aborted by the user, or some issue while printing was happened, respectively.
*/

class QICS_EXPORT QicsPrintPreviewWidget : public QScrollArea
{
    Q_OBJECT
public:
    /*! Constructor.
    @param parent	Parent widget.
    @param pcm		Page control mode. See setPageCountMode().
    */
    QicsPrintPreviewWidget(QWidget *parent, PageCountMode pcm = PCM_DYNAMIC);

    ~QicsPrintPreviewWidget();

    // common

    /*! Returns current printer object.
    \sa updatePages()
    */
    inline QPrinter *printer() const {return m_printer;}

    /*! Updates pages layout according to the \a printer parameter which will be set as current printer.
    Typical usage is to call this function after showing QPrintDialog or QPageSetupDialog with their
    %printer() value.
    \sa printer()
    */
    void updatePages(QPrinter *printer);

    /*! Returns total numbers of pages shown.
    */
    inline int pagesCount() const { return m_pages.size(); }

    // printing

    /*! Sets \a num number of copies to print with print() method.
    If the number of copies is more than 1, then order of copies also will be applied (set by setPrintCopyOrder()).

    \sa printCopies(), setPrintCopyOrder(), print()
    */
    void setPrintCopies(int num);
    /*! Returns number of copies to print with print() method.
    \sa setPrintCopies()
    */
    inline int printCopies() const { return m_copies; }

    /*! Sets \a order of copies to print with print() method.
    It can be one of the following:
    - CO_EVERYPAGE - every copy is duplicated one-after-one (i.e. 1,1,1,2,2,2,3,3,3...).
    - CO_PAGESET - every subset is duplicated (i.e. 1,2,3,1,2,3,1,2,3...).

    \sa printCopyOrder(), setPrintCopies(), print()
    */
    inline void setPrintCopyOrder(PrintCopyOrder order) {m_printCopyOrder = order;}
    /*! Returns order of copies to print with print() method.
    \sa setPrintCopyOrder()
    */
    inline PrintCopyOrder printCopyOrder() const { return m_printCopyOrder; }

    /*! Sets \a order of pages to print with print() method. It can be one of the following:
    - PO_FIRSTPAGE - printing will be started from the first page in the subset (to last).
    - PO_LASTPAGE - printing will be started from the last page in the subset (to first).
    - PO_PRINTER - order is determined by current printer's settings (see printer() and QPrinter::pageOrder()).

    \sa printPageOrder()
    */
    inline void setPrintPageOrder(PrintPageOrder order) {m_printPageOrder = order;}
    /*! Returns order of pages to print with print() method.
    \sa setPrintPageOrder(), print()
    */
    inline PrintPageOrder printPageOrder() const { return m_printPageOrder; }

    /*! Sets \a set of pages to print with print() method. It can be one of	the following:
    - PS_ALL - all the pages will be sent to printer.
    - PS_ENABLED - only enabled pages will be sent to printer.
    - PS_SELECTED - only selected pages will be sent to printer.
    - PS_PRINTER - subset is determined by current printer's settings (see printer() and QPrinter::printRange()).
    - PS_CUSTOM - subset must be set explicitly with setPagesToPrint() method.

    \sa printPageSet(), print()
    */
    inline void setPrintPageSet(PrintPageSet set) {m_printPageSet = set;}
    /*! Returns set of pages to print with print() method.
    \sa setPrintPageSet()
    */
    inline PrintPageSet printPageSet() const { return m_printPageSet; }

    /*! Sets \a sequence of pages to print with print() method. It can be one of the following:
    - PR_ALL - all the pages in the subset will be sent to printer.
    - PR_EVEN - only even pages in the subset will be sent to printer.
    - PR_ODD - only odd pages in the subset will be sent to printer.

    \sa printPageSequence(), print()
    */
    inline void setPrintPageSequence(PrintPageSequence sequence) {m_printPageSequence = sequence;}
    /*! Returns sequence of pages to print with print() method.
    \sa setPrintPageSequence()
    */
    inline PrintPageSequence printPageSequence() const { return m_printPageSequence; }

    /*! Sets explicitly \a numbers of pages to print with print() method. Logical numbers (from 1 to pagesCount()) must be used.
    \sa pagesToPrint()
    */
    void setPagesToPrint(const QList<int>& numbers);
    /*! Returns numbers of pages to print with print() method.
    \sa setPagesToPrint()
    */
    inline const QList<int> &pagesToPrint() const { return m_pagesToPrint; }

    // pages appearance

    /*! Sets visibility of logical page numbers to \a show.
    \sa isPageNumbersVisible()
    */
    void showPageNumbers(bool show = true);
    /*! Returns visibility of logical page numbers.
    \sa showPageNumbers()
    */
    inline bool isPageNumbersVisible() const { return m_showNumbers; }

    /*! Sets visibility of metrical rulers to \a show.
    \sa isRulersVisible()
    */
    void showRulers(bool show = true);
    /*! Returns visibility of metrical rulers.
    \sa showRulers()
    */
    inline bool isRulersVisible() const { return m_showRulers; }

    /*! Sets jump to active page to \a yes. If enabled, then after each layout update viewport will
    be scrolled to show currently active page.
    \sa isAutoShowActivePage()
    */
    inline void autoShowActivePage(bool yes = true) {m_activePageJump = yes;}
    /*! Returns \a true if jump to active page is enabled. \a false if not.
    \sa autoShowActivePage()
    */
    inline bool isAutoShowActivePage() const { return m_activePageJump; }

    /*! Sets double click zoom enabled \a yes. If enabled, then double-click on the page in select mode
    will zoom it to 100%, and the next double-click will revert zoom back.
    \sa isClickZoomEnabled()
    */
    inline void enableClickZoom(bool yes = true) {m_zoomOn = yes;}
    /*! Returns \a true if double click zoom is enabled. \a false if not.
    \sa enableClickZoom()
    */
    inline bool isClickZoomEnabled() const { return m_zoomOn; }

    /*! Scrolls viewport to the page with logical index \a index if exists.
    \sa ensurePageVisible(QicsPrintPreviewPage* page)
    */
    void ensurePageVisibleIndex(int index);
    /*! Scrolls viewport to the page \a page if exists.
    \sa ensurePageVisible(int index)
    */
    void ensurePageVisible(QicsPrintPreviewPage* page);

    /*! Sets active page with index \a index if exists.
    \sa setActivePage(QicsPrintPreviewPage* page), activePage()
    */
    void setActivePageIndex(int index);
    /*! Sets active page \a page if exists.
    \sa setActivePageIndex(int index), activePage()
    */
    void setActivePage(QicsPrintPreviewPage* page);
    /*! Returns active page if exists.
    \sa setActivePageIndex(int index), setActivePage(QicsPrintPreviewPage* page)
    */
    inline QicsPrintPreviewPage *activePage() const { return m_activePage; }

    /*! Sets current user control mode to \a cm. It can be one of the following:
    - CM_SELECT - in this mode, pages can be selected with the mouse.
    - CM_PAN - in this mode, viewport can de dragged freely to navigate through the whole page set.

    \sa controlMode()
    */
    void setControlMode(ControlMode cm);
    /*! Returns current user control mode.
    \sa setControlMode()
    */
    inline ControlMode controlMode() const { return m_cm; }

    /*! Sets current page count mode to \a pcm. It can be one of the following:
    - PCM_STATIC - static counting mode.
    - PCM_DYNAMIC - dynamic counting mode (good when there are a lot of pages to preview).

    See <a href="print_preview.html#counting">Counting the pages</a> topic for reference.

    \sa pageCountMode()
    */
    void setPageCountMode(PageCountMode pcm);
    /*! Returns current page count mode.
    \sa setPageCountMode()
    */
    inline PageCountMode pageCountMode() const { return m_pcm; }

    // pages arrangement

    /*! Sets preview options to \a opt. See PageLayoutData description for reference.
    \sa previewOptions()
    */
    void setPreviewOptions(const PageLayoutData &opt);
    /*! Returns current preview options.
    \sa setPreviewOptions()
    */
    inline const PageLayoutData& previewOptions() const { return m_params; }

    /*! Sets page ordering to \a lo. See PageLayoutData description for reference.
    \sa pageOrdering()
    */
    void setPageOrdering(PageLayoutOrdering lo);
    /*! Returns current page ordering.
    \sa setPageOrdering()
    */
    inline PageLayoutOrdering pageOrdering() const { return m_params.m_lo; }

    /*! Sets current scale (in percents) to \a percent.
    \sa scale()
    */
    void setScale(double percent);
    /*! Returns current scale in percents.
    \sa setScale()
    */
    inline double scale() const { return m_params.m_zoomP; }

    /*! Sets page direction to \a dir and number of pages to fit to \a count. If count <= 0 (by default), it will remain unchanged.
    See PageLayoutData description for reference.
    \sa pageDirection()
    */
    void setPageDirection(PageLayoutDirection dir, int count = -1);	// <=0 = default; >0 = fixed (number)
    /*! Returns current page direction.
    \sa setPageDirection()
    */
    inline PageLayoutDirection	pageDirection() const { return m_params.m_ld; }

    /*! Returns current page number to fit horizontally.
    \sa setPageDirection()
    */
    inline int pagesX() const { return m_params.m_fitX;}
    /*! Returns current page number to fit vertically.
    \sa setPageDirection()
    */
    inline int pagesY() const { return m_params.m_fitY;}

    /*! Sets page scaling to \a ls. See PageLayoutData description for reference.
    \sa pageScaling()
    */
    void setPageScaling(PageLayoutScaling ls);
    /*! Returns current page scaling.
    \sa setPageScaling()
    */
    inline PageLayoutScaling pageScaling() const { return m_params.m_ls; }

    /*! Sets page counting to \a lc. See PageLayoutData description for reference.
    \sa pageCounting()
    */
    void setPageCounting(PageLayoutCounting lc);
    /*! Returns current page counting.
    \sa setPageCounting()
    */
    inline PageLayoutCounting pageCounting() const { return m_params.m_lc; }

    // ui

    /*! Sets corner widget (top-left between rulers) to w, or placeholder if w is 0.
    Corner widget always has fixed side, and is visible only if rulers set to visible
    (i.e. isRulersVisible() is true).
    \sa cornerWidget()
    \since 2.2
    */
    void setCornerWidget(QWidget *w);

    /*! Returns corner widget (top-left between rulers).
    \sa setCornerWidget()
    \since 2.2
    */
    inline QWidget* cornerWidget() { return m_corner; }

public slots:
    // printing

    /*! Starts printing, using current printer and paper settings, current set of pages, number of copies and order.
    \sa setPrintCopies(), setPrintCopyOrder(), setPrintPageOrder(), setPrintPageSet(), setPrintPageSequence(), setPagesToPrint()
    */
    void print();
    /*! Simply prints single copy of all the pages using current printer and paper settings.
    May be deprecated in the future releases; use print() instead.
    */
    void printAllPages();

    // pages appearance
    /*! Sets selection state of all the pages to \a yes.
    \sa deselectAll()
    */
    void selectAll(bool yes = true);
    /*! Sets selection state of all the pages to \a false.
    \sa selectAll()
    */
    inline void deselectAll() { selectAll(false); }

    /*! Sets enabled state of all the pages to \a yes.
    \sa disableAll()
    */
    void enableAll(bool yes = true);
    /*! Sets enabled state of all the pages to \a false.
    \sa enableAll()
    */
    inline void disableAll() { enableAll(false); }
    /*! Sets enabled state of all the selected pages to \a yes.
    \sa disableSelected()
    */
    void enableSelected(bool yes = true);
    /*! Sets enabled state of all the selected pages to \a false.
    \sa enableSelected()
    */
    inline void disableSelected() { enableSelected(false); }

    // callback from page counter

    /*! In dynamic page count mode, it should be called after a page has been counted.
    It is obligatory to call when building own preview.
    See <a href="print_preview.html#counting">Counting the pages</a> topic for reference.
    \sa countPages(), terminateCount(), setPageCountMode()
    */
    void pageCounted();

signals:
    // obligatory to handle

    /*! Emitted when printer/paper options was changed so number of pages should be recalculated.
    This signal is obligatory to handle when building own preview.
    See <a href="print_preview.html#counting">Counting the pages</a> topic for reference.
    \sa pageCounted(), terminateCount(), setPageCountMode()
    */
    void countPages(int *pages, QPrinter *printer);
    /*! Emitted when a page must be redrawn during preview.
    This signal is obligatory to handle when building own preview.
    See <a href="print_preview.html#painting">Painting page content</a> topic for reference.
    \sa printPage()
    */
    void drawPage(QicsPrintPreviewPage *page);
    /*! Emitted when a page must be redrawn during printing.
    This signal is obligatory to handle when building own preview.
    See <a href="print_preview.html#painting">Painting page content</a> topic for reference.
    \sa drawPage()
    */
    void printPage(QicsPrintPreviewPage *page, QPainter *painter);

    // handle to terminate page counting in dynamic mode

    /*! Emitted in dynamic mode when page counting was restarted, so older counter should be terminated.
    This signal is obligatory to handle when building own preview with dynamic mode support.
    See <a href="print_preview.html#counting">Counting the pages</a> topic for reference.
    \sa countPages(), pageCounted(), setPageCountMode()
    */
    void terminateCount();

    // handle to get feedback from print process

    /*! Emitted when the page \a page with order \a i is about to be printed.
    Handle this signal to perform GUI interaction.
    \a i is the order in which this page is sent to printer, and it is always in range [1...totalPages].
    For every next page printed, \a i will be increased	by 1.
    \sa printingStarted(), pagePrinted()
    */
    void pagePrinting(int i, QicsPrintPreviewPage *page);
    /*! Emitted after the page \a page with order \a i has been printed.
    Handle this signal to perform GUI interaction.
    \a i is the order in which this page is sent to printer, and it is always in range [1...totalPages].
    For every next page printed, \a i will be increased	by 1.
    \sa printingStarted(), pagePrinting()
    */
    void pagePrinted(int i, QicsPrintPreviewPage *page);
    /*! Emitted just before the actual printing starts.
    Handle this signal to perform GUI interaction.
    \a totalPages is the total amount of pages to print in this session.
    \sa pagePrinting(), pagePrinted(), printingFinished()
    */
    void printingStarted(int totalPages);
    /*! Emitted after the actual printing has finished normally (all pages have been printed).
    Handle this signal to perform GUI interaction.
    \sa printingAborted(), printingError()
    */
    void printingFinished();
    /*! Emitted when the actual printing was aborted (i.e. by the user etc.).
    Handle this signal to perform GUI interaction.
    \sa printingFinished(), printingError()
    */
    void printingAborted();
    /*! Emitted when the actual printing was aborted or was not started because some error was happened.
    Handle this signal to perform GUI interaction.
    \sa printingFinished(), printingAborted()
    */
    void printingError();

    // handle to GUI interact

    /*! Emitted when page layout was updated.
    Handle this signal to perform GUI interaction.
    \a opt is the current preview layout data.
    \sa setPreviewOptions()
    */
    void updatePerformed(const PageLayoutData &opt);
    /*! Emitted when a context menu has been requested on page \a page.
    If \a page is 0, then the menu has been requested on viewport.
    \a event is QContextMenuEvent passed from viewport.
    Handle this signal to organize own context menu.
    */
    void contextMenuRequest(QicsPrintPreviewPage *page, QContextMenuEvent* event);

protected slots:
    void updateXRuler(int = 0);
    void updateYRuler(int = 0);

    void doStaticUpdate();
    void doDynamicUpdate();

protected:
    virtual void resizeEvent ( QResizeEvent * event );
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void contextMenuEvent ( QContextMenuEvent * event );
    virtual void hideEvent ( QHideEvent * event );

    void doZoom();
    int doCalculateHorizontalFit();
    int doCalculateVerticalFit();

    void doCalculateHorizontalZoom();
    void doCalculateVerticalZoom();

    void doLayoutPages();
    void doDeletePages();

    enum UpdateHints {
        HINT_NoZoom = 1,
        HINT_LayoutOnFitChanged = 2
    };

    bool doPrintPage(int number, QPainter &p);

    void doFullUpdate(uint hint = 0);

    void doUpdateRulers();

    QPrinter *m_printer;

    QWidget *m_preview;
    QicsRuler *m_rulerX;
    QicsRuler *m_rulerY;
    QWidget *m_corner;
    QVector<QicsPrintPreviewPage*>	m_pages;

    QicsPrintPreviewPage *m_activePage;
    bool m_showNumbers;
    bool m_activePageJump;
    bool m_zoomOn;
    bool m_showRulers;

    PageLayoutData	m_params;
    PageLayoutData m_storedParams;

    QList<int> m_pagesToPrint;
    int m_copies;
    PrintCopyOrder m_printCopyOrder;
    PrintPageOrder m_printPageOrder;
    PrintPageSet m_printPageSet;
    PrintPageSequence m_printPageSequence;

    bool m_newPage;
    int m_printPageIndex;

    ControlMode m_cm;
    bool m_modeState;
    QPoint m_modePos;

    PageCountMode m_pcm;
    bool m_updateInProgress;
};

#endif //QICSPRINTPREVIEWWIDGET_H


