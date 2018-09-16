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

#ifndef QICSTEXTCELLDISPLAYHELPERS_H
#define QICSTEXTCELLDISPLAYHELPERS_H

#include <QLineEdit>
#include <QTextEdit>
#include "QicsNamespace.h"

/*! \internal
*  \class QicsLineEdit QicsTextCellDisplayHelpers.h
*  \brief helper class for QicsTextCellDisplayWidget
*/

class QicsTextCellDisplayWidget;
class QLayout;
class QicsScreenGrid;

class QICS_EXPORT QicsLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QicsLineEdit(QWidget*);
    virtual void focusOutEvent(QFocusEvent*);
    virtual void contextMenuEvent(QContextMenuEvent *event);

signals:
    void lostFocus();
    void focusReasonChanged(Qt::FocusReason);

protected:
    bool m_blockSignals;
};

/*! \internal
*  \class QicsTextEdit QicsTextCellDisplayHelpers.h
*  \brief helper class for QicsTextCellDisplayWidget
*/
class QICS_EXPORT QicsTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    QicsTextEdit(QWidget*);
    virtual void focusOutEvent(QFocusEvent*);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void keyPressEvent(QKeyEvent* );

    // Helper methods to simulate QLineEdit behavior
    inline void setValidator(QValidator* v) {m_validator = v;}
    inline const QValidator* validator() const {return m_validator;}
    inline void setMaxLength(int length) {m_maxLength = length;}
    inline int maxLength() const {return m_maxLength;}

protected slots:
    void handleTextChanged();

signals:
    void lostFocus();
    void focusReasonChanged(Qt::FocusReason);
    void textEdited(const QString &);

protected:
    bool m_blockSignals;
    QValidator* m_validator;
    int m_maxLength;
};

/*! \internal
*  \class QicsTextEdit QicsTextCellDisplayHelpers.h
*  \brief Widget class for QicsTextCellDisplay
*
*  Widget class for QicsTextCellDisplay.
*  Instance of QicsTextCellDisplayWidget is using in QicsTextCellDisplay class.
*  It contains QicsLineEdit and QicsTextEdit inside.
*  This allows it behave in different manner in different situations - when
*  width of displayer more than viewport width it uses QicsTextEdit with
*  additional scrollbar to be able to scroll all contetns. In normal situations
*  it uses QicsLineEdit.
*/

class QICS_EXPORT QicsTextCellDisplayWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
    * Widget types
    */
    enum WidgetType {
        LineEdit,
        TextEdit
    };

    /*!
    * Default constructor
    */
    QicsTextCellDisplayWidget(QWidget* parent = 0);

    /*!
    * Sets alignment \a to widget
    */
    void setAlignment(Qt::Alignment a);

    /*!
    * Return alignment of widget
    */
    Qt::Alignment alignment() const;

    /*!
    *  Set string \b str to widget
    */
    void setText(const QString& str);

    /*!
    * Returns the string that widget displays
    */
    QString text() const;

    /*!
    * Sets validator \b v to widget
    */
    void setValidator(QValidator *v);
    /*!
    * Returns the validator that widget has.
    */
    const QValidator *validator() const;

    /*!
    * Sets max length \b l of string that widget can have
    */
    void setMaxLength(int l);
    /*!
    * Returns max length of string that widget can have
    */
    int maxLength() const;

    /*!
    * Sets input mask \b mask for line edit; does nothing for text edit.
    */
    void setInputMask(const QString &inputMask);
    /*!
    * Returns input mask for line edit; QString::null for text edit.
    */
    QString inputMask() const;

    /*!
    * Sets echo mode \b mode for line edit; does nothing for text edit.
    */
    void setEchoMode(QLineEdit::EchoMode mode);

    /*!
    * Returns echo mode for line edit; QLineEdit::Normal for text edit.
    */
    QLineEdit::EchoMode echoMode() const;

    /*!
    * Returns the string that widget displays when mask is applied.
    */
    QString displayText() const;

    /*!
    * Enables frame when \b on = true for line edit; does nothing for text edit.
    */
    void setFrame( bool on );
    /*!
    * Returns is the frame was enabled for line edit; true for text edit.
    */
    bool hasFrame() const;

    /*!
    * Enables drag when \b b = true for line edit; does nothing for text edit.
    */
    void setDragEnabled( bool b );
    /*!
    * Returns is the drag was enabled for line edit; false for text edit.
    */
    bool dragEnabled() const;

    /*!
    * Selects all contents of widget
    */
    void selectAll();

    /*!
    * Clears contents of widget
    */
    void clear();

    /*!
    * Used by QicsTextCellDisplay to send needful events to widget.
    */
    bool sendEvent(QEvent *event);

    /*!
    * Used by QicsTextCellDisplay to install event filters and other initialization.
    */
    void installFilters(QObject *obj);

    /*!
    * Returns rect that required to proper widget displaying.
    * \param fullRect full rect of cell where displayer is placed
    * \param helper helper widget which holds the viewport width/height
    */
    QRect requiredRect(const QRect &fullRect,QWidget *helper = 0)const;

    /*!
    * Switches widget between two states - LINEEDIT and TEXTEDIT
    */
    void switchState(WidgetType type);

    /*!
    * Returns current state of widget.
    */
    inline WidgetType currentState() const {return m_currentState;}

signals:
    void lostFocus();
    void focusReasonChanged(Qt::FocusReason);

protected:
    QLayout *m_layout;
    QWidget *m_state;
    QicsLineEdit *m_lineEditState;
    QicsTextEdit *m_textEditState;
    WidgetType m_currentState;
};

#endif //QICSTEXTCELLDISPLAYHELPERS_H


