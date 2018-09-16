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

#include <QicsPopupDialog.h>

#include <QSizeGrip>
#include <QMouseEvent>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QPointer>

/////////////////////////////////////////////////////////////////////////////////
// QicsSizeGrip
/////////////////////////////////////////////////////////////////////////////////

class QicsSizeGrip : public QSizeGrip
{
public:
    QicsSizeGrip(QWidget *w)
        : QSizeGrip(w), m_w(w)
    {
    }

protected:
    virtual void mousePressEvent(QMouseEvent *e)
    {
        if (e->button() == Qt::LeftButton) {
            m_drag = true;
            m_pos = e->pos();
        }
    }

    virtual void mouseReleaseEvent(QMouseEvent *e)
    {
        if (e->button() == Qt::LeftButton) {
            m_drag = false;
        }
    }

    virtual void mouseMoveEvent(QMouseEvent *e)
    {
        if (m_drag) {
            QPoint pos = e->pos() - m_pos;
            m_w->resize(m_w->width() + pos.x(), m_w->height() + pos.y());
        }
    }

    QWidget *m_w;
    bool m_drag;
    QPoint m_pos;
};

/////////////////////////////////////////////////////////////////////////////////
// QicsPopupDialog
/////////////////////////////////////////////////////////////////////////////////

QicsPopupDialog::QicsPopupDialog(QWidget *parent, QDialogButtonBox::StandardButtons buttons)
    : QFrame(parent, Qt::Popup), m_gripOn(true), m_eventLoop(0), m_widget(0)
{
    setFrameShape(QFrame::Box);

    m_status = new QStatusBar(this);

    m_lastButton = QDialogButtonBox::NoButton;
    m_buttons = new QDialogButtonBox(buttons, Qt::Horizontal, m_status);
    m_status->addPermanentWidget(m_buttons);
    connect(m_buttons, SIGNAL(accepted()), this, SIGNAL(accepted()));
    connect(m_buttons, SIGNAL(rejected()), this, SIGNAL(rejected()));
    connect(m_buttons, SIGNAL(clicked(QAbstractButton *)), this, SLOT(onClicked(QAbstractButton *)));

    connect(m_buttons, SIGNAL(accepted()), this, SLOT(onAccepted()));
    connect(m_buttons, SIGNAL(rejected()), this, SLOT(close()));

#ifndef Q_OS_WIN32
    m_status->setSizeGripEnabled(false);
    m_sizeGrip = new QicsSizeGrip(this);
    m_status->addPermanentWidget(m_sizeGrip);
    m_sizeGrip->setFixedHeight(m_status->height());
#endif

    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(2);

    setLayout(m_layout);

    m_layout->addWidget(m_status);
}

QicsPopupDialog::~QicsPopupDialog()
{
}

void QicsPopupDialog::setSizeGripEnabled ( bool on )
{
    if (m_gripOn != on) return;

    m_gripOn = on;

#ifndef Q_OS_WIN32
    m_sizeGrip->setVisible(on);
#else
    m_status->setSizeGripEnabled(m_gripOn);
#endif
}

void QicsPopupDialog::onClicked ( QAbstractButton * button )
{
    m_lastButton = m_buttons->standardButton(button);
    emit clicked(m_lastButton);
}

void QicsPopupDialog::onAccepted()
{
    accept();
}

void QicsPopupDialog::show(const QPoint &pos)
{
    m_lastButton = QDialogButtonBox::NoButton;

    // we should check if any screen bounds have been violated
    QRect srect(QDesktopWidget().availableGeometry());

    QPoint p(pos);
    QRect r(rect());
    r.moveTopLeft(p);
    if (r.top() < 0) p.setY(0);
    if (r.left() < 0) p.setX(0);
    if (r.bottom() > srect.bottom()) p.setY(srect.bottom() - height());
    if (r.right() > srect.right()) p.setX(srect.right() - width());

    move(p);

    raise();
    QFrame::show();
}

int QicsPopupDialog::exec(const QPoint &pos)
{
    if (m_eventLoop) {
        qWarning("QicsPopupDialog::exec: Recursive call detected");
        return -1;
    }

    bool deleteOnClose = testAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_DeleteOnClose, false);

    bool wasShowModal = testAttribute(Qt::WA_ShowModal);
    setAttribute(Qt::WA_ShowModal, true);

    show(pos);

    QEventLoop eventLoop;
    m_eventLoop = &eventLoop;
    QPointer<QicsPopupDialog> guard = this;
    eventLoop.exec();
    if (guard.isNull())
        return -1;
    m_eventLoop = 0;

    setAttribute(Qt::WA_ShowModal, wasShowModal);

    int res = static_cast<int>(m_lastButton);
    if (deleteOnClose)
        delete this;

    return res;
}

void QicsPopupDialog::setWidget(QWidget *mainWidget, bool deleteOld)
{
    if (m_widget) {
        m_widget->setParent(0);
        if (deleteOld)
            delete m_widget;
    }

    m_widget = mainWidget;
    m_layout->insertWidget(0, m_widget);
}

void QicsPopupDialog::setVisible(bool visible)
{
    // Reimplemented to exit a modal event loop when the dialog is hidden.
    QFrame::setVisible(visible);
    if (!visible) {
        if (m_eventLoop)
            m_eventLoop->exit();
    }
}


