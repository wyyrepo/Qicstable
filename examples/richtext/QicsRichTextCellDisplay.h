/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef QICSRICHTEXTCELLDISPLAY_H
#define QICSRICHTEXTCELLDISPLAY_H

#include <QTextEdit>
#include "QicsCellDisplay.h"


class QTextDocument;

class QicsRichTextCellDisplay : public QTextEdit, public QicsMovableEntryWidgetCellDisplay
{
    Q_OBJECT
public:
    QicsRichTextCellDisplay(QWidget *parent = 0);
    virtual ~QicsRichTextCellDisplay();

    virtual void displayCell(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm,
        QRect &rect, QPainter *painter);

    virtual void startEdit(QicsScreenGrid *, int row, int col,
        const QicsDataItem *itm);

    virtual void moveEdit(QicsScreenGrid *grid,
        int row, int col,
        const QRect &rect);

    virtual void endEdit(QicsScreenGrid *, int row, int col);

    inline virtual bool editWhenCurrent() const {return m_editWhenCurrent;}

    virtual void setEditWhenCurrent(bool yes) {m_editWhenCurrent = yes;}

    virtual bool isEmpty(QicsGridInfo *info, int row, int col,
        const QicsDataItem *itm) const;

    virtual QSize sizeHint(QicsGrid *grid, int row, int col,
        const QicsDataItem *itm);

    virtual QWidget* widget();

    static const QString RichTextCellDisplayName;

    virtual QString cellDisplayName() const;

    virtual QDomElement toDomXml(QString tag, QDomDocument* doc);

    virtual void configureFromDomXml(const QDomElement& e);

    virtual bool eventFilter(QObject *o, QEvent *e);

protected:
    virtual void focusOutEvent(QFocusEvent*);

    virtual QWidget *newEntryWidget(QicsScreenGrid *grid);

    virtual void valueChanged(QicsGridInfo *info, int row, int col,
        int model_row, int model_col, const QString &val);

    virtual void resetValue(QicsEntryWidgetInfo *info);

protected slots:
    void textChanged();

private:
    QTextDocument *m_document;
    QString m_prevValue;
    bool m_editWhenCurrent;
};

class QicsRichTextCellDisplayConfigurationWidget : public QicsCellDisplayConfigurationWidget
{
public:
    QicsRichTextCellDisplayConfigurationWidget(QWidget* parent=0);

    virtual void readConfiguration(QicsCellDisplay* cellDisplay);
    virtual void writeConfiguration(QicsCellDisplay* cellDisplay);
};


class QicsRichTextCellDisplayFactory: public QicsCellDisplayFactory
{
public:
    virtual QicsCellDisplay* createCellDisplay();
    virtual QicsCellDisplayConfigurationWidget* configurationWidget();
};

#endif //QICSRICHTEXTCELLDISPLAY_H


