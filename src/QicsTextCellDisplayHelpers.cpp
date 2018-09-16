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

#include "QicsTextCellDisplayHelpers.h"

#include <QTextCursor>
#include <QFocusEvent>
#include <QApplication>
#include <QValidator>
#include <QScrollBar>
#include <QHBoxLayout>
#include "QicsScreenGrid.h"


QicsLineEdit::QicsLineEdit(QWidget* parent)
    : QLineEdit(parent),
    m_blockSignals(false)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void QicsLineEdit::focusOutEvent(QFocusEvent* fe)
{
    emit focusReasonChanged(fe->reason());

    if(!m_blockSignals)
        emit lostFocus();
}

void QicsLineEdit::contextMenuEvent ( QContextMenuEvent * event )
{
    m_blockSignals = true;
    QLineEdit::contextMenuEvent(event);
    m_blockSignals = false;
}


QicsTextEdit::QicsTextEdit(QWidget* parent)
    : QTextEdit(parent),
    m_blockSignals(false),
    m_validator(0),
    m_maxLength(INT_MAX)
{
    setAcceptRichText(false);
    setLineWrapMode(QTextEdit::NoWrap);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setUndoRedoEnabled(false);
    setOverwriteMode(false);
    setLineWrapMode(QTextEdit::NoWrap);

    connect(this, SIGNAL(textChanged()), SLOT(handleTextChanged()));
}

void QicsTextEdit::handleTextChanged()
{
    QString str = toPlainText();
    int pos = textCursor().position();

    if((m_validator && !m_validator->validate(str,pos)) || str.size() > m_maxLength) {
        str.chop(1);
        bool blocked = blockSignals(true);
        setPlainText(str);
        blockSignals(blocked);
        moveCursor(QTextCursor::EndOfLine);
        return;
    }

    emit textEdited(str);
}

void QicsTextEdit::focusOutEvent(QFocusEvent* fe)
{
    emit focusReasonChanged(fe->reason());

    if(!m_blockSignals)
        emit lostFocus();
}

void QicsTextEdit::keyPressEvent(QKeyEvent* ke)
{
    if(ke->key() == Qt::Key_Enter|| ke->key() == Qt::Key_Return)
        return;

    QTextEdit::keyPressEvent(ke);
}

void QicsTextEdit::contextMenuEvent ( QContextMenuEvent * event )
{
    m_blockSignals = true;
    QTextEdit::contextMenuEvent(event);
    m_blockSignals = false;
}

QicsTextCellDisplayWidget::QicsTextCellDisplayWidget(QWidget* parent)
    : QWidget(parent, Qt::FramelessWindowHint),
    m_state(0)
{
    setAttribute(Qt::WA_DeleteOnClose);

    m_layout = new QHBoxLayout;
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    m_lineEditState =  new QicsLineEdit(this);
    connect(m_lineEditState, SIGNAL(lostFocus()), this, SIGNAL(lostFocus()));
    connect(m_lineEditState, SIGNAL(focusReasonChanged(Qt::FocusReason)),
        this, SIGNAL(focusReasonChanged(Qt::FocusReason)));
    m_lineEditState->hide();

    m_textEditState =  new QicsTextEdit(this);
    connect(m_textEditState, SIGNAL(lostFocus()), this, SIGNAL(lostFocus()));
    connect(m_textEditState, SIGNAL(focusReasonChanged(Qt::FocusReason)),
        this, SIGNAL(focusReasonChanged(Qt::FocusReason)));
    m_textEditState->hide();

    setLayout(m_layout);

    switchState(QicsTextCellDisplayWidget::LineEdit);
}

void QicsTextCellDisplayWidget::installFilters(QObject* obj)
{
    connect(m_lineEditState, SIGNAL(textEdited(const QString &)), obj, SLOT(handleTextChanged(const QString &)));
    connect(m_textEditState, SIGNAL(textEdited(const QString &)), obj, SLOT(handleTextChanged(const QString &)));

    m_lineEditState->installEventFilter(obj);
    m_textEditState->installEventFilter(obj);
}

QRect QicsTextCellDisplayWidget::requiredRect(const QRect &fullRect, QWidget *helper)const
{
    QRect full_rect(fullRect);

    // QLineEdit always centers edit - so it will be hidden cell height is too high
    // so we're reducing the helper height to fit it into the cell
    // the same for width
    if(helper->height() < full_rect.height())
        full_rect.setHeight(helper->height());

    if(helper->width() < full_rect.width())
        full_rect.setWidth(helper->width());

    if(m_currentState == QicsTextCellDisplayWidget::TextEdit)
        full_rect.setHeight(m_textEditState->horizontalScrollBar()->height() + full_rect.height() + 4);

    return full_rect;
}

bool QicsTextCellDisplayWidget::sendEvent(QEvent* ke)
{
    return QApplication::sendEvent(m_state, ke);
}

void QicsTextCellDisplayWidget::switchState(WidgetType type)
{
    if(type == QicsTextCellDisplayWidget::LineEdit) {
        m_state = m_lineEditState;
        m_currentState = QicsTextCellDisplayWidget::LineEdit;
    }
    else {
        m_state = m_textEditState;
        m_currentState = QicsTextCellDisplayWidget::TextEdit;
    }

    setFocusProxy(m_state);

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        QWidget *widget = child->widget();
        if (widget)
            widget->hide();
    }

    m_layout->addWidget(m_state);
    m_layout->update();
    m_state->show();
}

/*
* Widget attribute methods
*/
void QicsTextCellDisplayWidget::setAlignment(Qt::Alignment a)
{
    bool blocked = m_state->blockSignals( true );
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->setAlignment(a);
    else
        m_textEditState->setAlignment(a);
    m_state->blockSignals( blocked );
}

Qt::Alignment QicsTextCellDisplayWidget::alignment()const
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        return m_lineEditState->alignment();
    else
        return m_textEditState->alignment();
}

void QicsTextCellDisplayWidget::clear()
{
    bool blocked = m_state->blockSignals( true );
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->clear();
    else
        m_textEditState->clear();
    m_state->blockSignals( blocked );
}

void QicsTextCellDisplayWidget::setText(const QString &str)
{
    bool blocked = m_state->blockSignals( true );
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->setText(str);
    else
        m_textEditState->setPlainText(str);
    m_state->blockSignals( blocked );
}

QString QicsTextCellDisplayWidget::text()const
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        return m_lineEditState->text();
    else
        return m_textEditState->toPlainText();
}

QString QicsTextCellDisplayWidget::displayText() const
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        return m_lineEditState->displayText();
    else
        return m_textEditState->toPlainText();
}

void QicsTextCellDisplayWidget::setValidator(QValidator *v)
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->setValidator(v);
    else
        m_textEditState->setValidator(v);
}

const QValidator* QicsTextCellDisplayWidget::validator()const
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        return m_lineEditState->validator();
    else
        return m_textEditState->validator();
}

void QicsTextCellDisplayWidget::setMaxLength(int l)
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->setMaxLength(l);
    else
        m_textEditState->setMaxLength(l);
}

int QicsTextCellDisplayWidget::maxLength()const
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        return m_lineEditState->maxLength();
    else
        return m_textEditState->maxLength();
}

void QicsTextCellDisplayWidget::selectAll()
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->selectAll();
    else
        m_textEditState->selectAll();
}

void QicsTextCellDisplayWidget::setInputMask(const QString &inputMask)
{
    bool blocked = m_state->blockSignals(true);
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->setInputMask(inputMask);
    m_state->blockSignals(blocked);
}

QString QicsTextCellDisplayWidget::inputMask() const
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        return m_lineEditState->inputMask();
    return QString();
}

void QicsTextCellDisplayWidget::setEchoMode(QLineEdit::EchoMode mode)
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->setEchoMode(mode);
}

QLineEdit::EchoMode QicsTextCellDisplayWidget::echoMode() const
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        return m_lineEditState->echoMode();
    return QLineEdit::Normal;
}

void QicsTextCellDisplayWidget::setFrame(bool on)
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->setFrame(on);
}

bool QicsTextCellDisplayWidget::hasFrame() const
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        return m_lineEditState->hasFrame();
    return true;
}

void QicsTextCellDisplayWidget::setDragEnabled (bool b)
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        m_lineEditState->setDragEnabled(b);
}

bool QicsTextCellDisplayWidget::dragEnabled() const
{
    if(m_currentState == QicsTextCellDisplayWidget::LineEdit)
        return m_lineEditState->dragEnabled();
    return true;
}


