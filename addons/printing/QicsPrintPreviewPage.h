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

#ifndef QICSPRINTPREVIEWPAGE_H
#define QICSPRINTPREVIEWPAGE_H

#include <QFrame>
#include <QicsNamespace.h>

class QicsPrintPreviewWidget;

///////////////////////////////////////////////////////////////////////////////
// QicsPrintPreviewPage class
///////////////////////////////////////////////////////////////////////////////

/*!
* \class QicsPrintPreviewPage QicsPrintPreviewPage.h
* \nosubgrouping
* \brief Page class used for WYSIWYG printing.

%QicsPrintPreviewPage represents single page of the document being previewed/printed.
Every page is responcible to paint its common parts (such as frame, logical number,
and so on) and its content by emitting drawPage() signal which is handled by
QicsPrintPreviewWidget and passed to the own preview program.

A page has its logical number which is 0-based and always in the
range of 0...QicsPrintPreviewWidget::pagesCount()-1. However, method number() will
return 1-based number as it is more convenient for the user.

Also, a page keeps its metrics, returned by pageRect(), realPageRect() and realPaperRect()
methods in pixels, and also widthMM() and heightMM() in metrical units.

Page can have active (returned by isActive()), selected (isSelected()) and enabled (isEnabled())
state set to true or false. These states are used mostly during printing, see QicsPrintPreviewWidget
description for the reference.

Typically, there is no need to explicitly create %QicsPrintPreviewPage objects as they are always
created by the framework.
*/

class QICS_EXPORT QicsPrintPreviewPage : public QFrame
{
    Q_OBJECT
public:
    /*! Constructor.
    @param num              0-based logical number of the page.
    @param widget   Parent QicsPrintPreviewWidget.
    @param parent   Parent QWidget (it is the real parent widget of the page).
    */
    QicsPrintPreviewPage(int num, QicsPrintPreviewWidget *widget, QWidget *parent);
    /*! Destructor.
    */
    virtual ~QicsPrintPreviewPage();

    /*! Returns 1-based logical number of the page in the
    range of 1...QicsPrintPreviewWidget::pagesCount().
    */
    inline int number() const {return m_num+1;}

    /*! Returns visual dimensions of the page (in pixels) according to current scale
    (i.e. as it is currently visible on the screen).
    \sa realPageRect()
    */
    inline const QRect &pageRect() const {return m_pageRect;}

    /*! Returns actual dimensions of the page (in pixels) as it is will be sent to printer. See QPrinter::pageRect().
    \sa pageRect()
    */
    inline const QRect &realPageRect() const {return m_realPageRect;}

    /*! Returns actual dimensions of the paper (in pixels). See QPrinter::paperRect().
    \sa realPageRect()
    */
    inline const QRect &realPaperRect() const {return m_realPaperRect;}

    /*! Returns \a true if the page is active, \a false if not. See QicsPrintPreviewWidget::activePage().
    */
    bool isActive() const;

    /*! Sets page selected state to \a yes. See QicsPrintPreviewWidget::selectAll().
    \sa isSelected()
    */
    void setSelected(bool yes = true);
    /*! Returns \a true if the page is selected, \a false if not.
    \sa setSelected()
    */
    inline bool isSelected() const {return m_selected;}

    /*! Sets page enabled state to \a yes. See QicsPrintPreviewWidget::enableAll().
    \sa isEnabled()
    */
    void setEnabled(bool yes = true);
    /*! Returns \a true if the page is enabled, \a false if not.
    \sa setEnabled()
    */
    inline bool isEnabled() const {return m_enabled;}

    /*! Returns width of the page in millimeters.
    \sa heightMM()
    */
    inline double widthMM() const {return mmWidth;}

    /*! Returns height of the page in millimeters.
    \sa widthMM()
    */
    inline double heightMM() const {return mmHeight;}

    /*! Reinitialized page dimensions from the QicsPrintPreviewWidget's current printer.
    Called internally when printer/paper parameters were changed.
    */
    void reinit();

signals:
    /*! Emitted when page needs to be drawn. Handled in QicsPrintPreviewWidget.
    */
    void drawPage(QicsPrintPreviewPage *page);

protected:
    virtual void paintEvent ( QPaintEvent * event );
    virtual void resizeEvent ( QResizeEvent * event );

    QRect m_pageRect;
    QRect m_realPageRect;
    QRect m_realPaperRect;

    int m_num;
    bool m_selected;
    bool m_enabled;

    QicsPrintPreviewWidget *m_widget;

    double mmWidth;
    double mmHeight;
};

#endif //QICSPRINTPREVIEWPAGE_H


