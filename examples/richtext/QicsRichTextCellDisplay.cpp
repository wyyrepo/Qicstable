/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "QicsRichTextCellDisplay.h"

#include <QApplication>
#include <QPainter>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include "QicsStyleManager.h"
#include "QicsScreenGrid.h"
#include "QicsCell.h"
#include "QicsMainGrid.h"
#include "QicsCellDisplay_p.h"


const QString QicsRichTextCellDisplay::RichTextCellDisplayName = "RichTextCellDisplay";
int g_escape_counter = 0;


QicsRichTextCellDisplay::QicsRichTextCellDisplay(QWidget *parent)
    : QTextEdit(parent), QicsMovableEntryWidgetCellDisplay()
{
    connect(this, SIGNAL(textChanged()), SLOT(textChanged()));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    installEventFilter(this);
    hide();

    m_document = new QTextDocument(this);
    m_editWhenCurrent = false;
}

QicsRichTextCellDisplay::~QicsRichTextCellDisplay()
{
}

void QicsRichTextCellDisplay::displayCell(QicsGrid *grid, int row, int col,
                                          const QicsDataItem *itm,
                                          QRect &rect, QPainter *painter)
{
    // init statics
    commonInit(grid, row, col, itm, rect, painter, this, true, false);

    m_document->setHtml(d->qs);
    painter->save();
    painter->translate(d->cr.topLeft());
    painter->setClipRect(d->cr.translated(-d->cr.x(), -d->cr.y()));
    m_document->drawContents(painter);
    painter->restore();
}

void QicsRichTextCellDisplay::startEdit(QicsScreenGrid *grid, int row, int col,
                                        const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QTextEdit *widget = static_cast<QTextEdit *> (info->widget());
    widget->setParent(grid);

    info->setRow(ginfo->modelRowIndex(row));
    info->setColumn(ginfo->modelColumnIndex(col));

    QicsDataItem *item = itm ? itm->clone() : 0;

    // Setup widget (colors, fonts, values etc) for this cell

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QPalette pal = myCell->palette();
    QColor efg = myCell->editForegroundColor();
    QColor ebg = myCell->editBackgroundColor();
    pal.setColor(QPalette::Text, efg);
    pal.setColor(QPalette::Base, ebg);
    widget->setPalette(pal);

    bool blocked = widget->blockSignals(true);
    widget->document()->clear();
    QString valString = item ? item->string() : QString();
    widget->setHtml(valString);
    m_prevValue = valString;
    widget->blockSignals(blocked);

    delete item;

    QicsCellDisplay::startEdit(grid,row,col,itm);
}

void QicsRichTextCellDisplay::moveEdit(QicsScreenGrid *grid, int row, int col,
                                       const QRect &rect)
{
    QicsEntryWidgetInfo* entry_info = getInfoFromGrid(grid);
    QTextEdit* widget = qobject_cast<QTextEdit *>(entry_info->widget());

    QicsGridInfo *ginfo = &(grid->gridInfo());

    QRect r = entryWidgetRect(ginfo, row, col, rect);
    widget->setGeometry(r);

    widget->show();
    widget->setFocus();
}

void QicsRichTextCellDisplay::endEdit(QicsScreenGrid *grid, int row, int col)
{
    QicsEntryWidgetInfo *info = takeInfoFromGrid(grid);
    if (!info) return;

    QTextEdit* widget = qobject_cast<QTextEdit *>(info->widget());
    widget->hide();

    m_prevValue.clear();

    QicsGridInfo &ginfo = grid->gridInfo();

    valueChanged(&ginfo, ginfo.visualRowIndex(info->row()),
        ginfo.visualColumnIndex(info->column()), info->row(),
        info->column(), widget->toHtml());

    QicsCellDisplay::endEdit(grid,row,col);
}

bool QicsRichTextCellDisplay::isEmpty(QicsGridInfo *info, int row, int col,
                                      const QicsDataItem *itm) const
{
    myCell->setInfo(info);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QString text = textToDisplay(info, row, col, itm);

    return text.isEmpty();
}

QSize QicsRichTextCellDisplay::sizeHint(QicsGrid *grid, int row, int col,
                                        const QicsDataItem *itm)
{
    QicsGridInfo *ginfo = &(grid->gridInfo());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(row);
    myCell->setColumnIndex(col);

    QString text = textToDisplay(ginfo, row, col, itm);
    m_document->setHtml(text);

    return m_document->size().toSize();
}

QString QicsRichTextCellDisplay::cellDisplayName() const
{
    return RichTextCellDisplayName;
}

QWidget* QicsRichTextCellDisplay::widget()
{
    return qobject_cast<QTextEdit *>(this);
}

void QicsRichTextCellDisplay::textChanged()
{
    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(parent());
    if (!grid) return;

    QicsEntryWidgetInfo *info = getInfoFromGrid(grid);
    QicsGridInfo *ginfo = &(grid->gridInfo());
    QTextEdit* widget = qobject_cast<QTextEdit *>(info->widget());

    myCell->setInfo(ginfo);
    myCell->setRowIndex(ginfo->visualRowIndex(info->row()));
    myCell->setColumnIndex(ginfo->visualColumnIndex(info->column()));

    bool blocked = ginfo->blockSignals(true);
    valueChanged(ginfo, ginfo->visualRowIndex(info->row()),
        ginfo->visualColumnIndex(info->column()), info->row(),
        info->column(), widget->toHtml());
    ginfo->blockSignals(blocked);
}

void QicsRichTextCellDisplay::valueChanged(QicsGridInfo *info, int row, int col,
        int model_row, int model_col, const QString &val)
{
    Q_UNUSED(model_row);
    Q_UNUSED(model_col);

    const QicsDataItem* item = info->cellValue(row, col);
    if (item && val == item->string())
        return;

    QicsDataString itm(val);
    info->setCellValue(row, col, itm);
}

QWidget *QicsRichTextCellDisplay::newEntryWidget(QicsScreenGrid *grid)
{
    Q_UNUSED(grid);
    return static_cast<QTextEdit *> (this);
}

void QicsRichTextCellDisplay::focusOutEvent(QFocusEvent* e)
{
    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid*>(parentWidget());

    if(grid && !hasFocus())
        grid->uneditCurrentCell();

    g_escape_counter = 0;

    QTextEdit::focusOutEvent(e);
}

bool QicsRichTextCellDisplay::eventFilter( QObject *o, QEvent *e )
{
    QicsEntryWidgetInfo *info = getInfoFromEntry(qobject_cast<QWidget*>(o));

    if(!info) return false;

    QicsScreenGrid* grid = info->grid();

    bool eatEvent = false;

    if ( e->type() == QEvent::KeyPress ) {
        QKeyEvent *k = static_cast<QKeyEvent *> (e);

        //if ((k->key() == Qt::Key_Return || k->key() == Qt::Key_Enter) && (k->modifiers() == Qt::AltModifier)) {
        // #### TODO: #962
        //}

        myGrid->setInfo(&(grid->gridInfo()));

        if (myGrid->traversalKeyEditBehavior() == Qics::TraverseBetweenCells ||
            k->key() == Qt::Key_Tab ||
            k->key() == Qt::Key_Backtab) {
            // special processing for key press
            eatEvent = grid->handleTraversalKeys(k);
        }
        // Handling for QicsTextEdit.
        if((k->key() == Qt::Key_Enter ||
            k->key() == Qt::Key_Return ||
            k->key() == Qt::Key_Up ||
            k->key() == Qt::Key_Down))
            eatEvent = grid->handleTraversalKeys( k );

        if (!eatEvent) {
            switch (k->key())
            {
            case Qt::Key_Escape:
                if (++g_escape_counter == 2) {
                    grid->uneditCurrentCell();
                    g_escape_counter = 0;
                }
                else
                    resetValue(info);
                eatEvent = true;
                break;
            default:
                g_escape_counter = 0;
                break;
            }
        }
    }

    if (eatEvent) return true;

    return QTextEdit::eventFilter(o, e);
}

void QicsRichTextCellDisplay::resetValue(QicsEntryWidgetInfo *info)
{
    if (!info) return;

    QicsScreenGrid *grid = qobject_cast<QicsScreenGrid *>(parent());
    if (!grid) return;

    QicsGridInfo *ginfo = &(grid->gridInfo());
    QTextEdit* widget = qobject_cast<QTextEdit *>(info->widget());

    bool blocked = widget->blockSignals(true);
    widget->setHtml(m_prevValue);
    widget->blockSignals(blocked);

    valueChanged(ginfo, ginfo->visualRowIndex(info->row()),
        ginfo->visualColumnIndex(info->column()), info->row(),
        info->column(), widget->toHtml());

    QicsStyleManager *sm = info->grid()->gridInfo().styleManager();
    const bool auto_select = * static_cast<bool*>
        (sm->getGridProperty(QicsGridStyle::AutoSelectCellContents));

    if (auto_select)
        widget->selectAll();
}

QDomElement QicsRichTextCellDisplay::toDomXml(QString tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    e.setAttribute("name", cellDisplayName());
    e.setAttribute("editWhenCurrent",QString::number((int)editWhenCurrent()));
    return e;
}

void QicsRichTextCellDisplay::configureFromDomXml(const QDomElement& e)
{
    setEditWhenCurrent((bool)e.attribute("editWhenCurrent").toInt());
}


QicsRichTextCellDisplayConfigurationWidget::QicsRichTextCellDisplayConfigurationWidget(QWidget* parent)
    : QicsCellDisplayConfigurationWidget(parent)
{
    QLabel* label = new QLabel("No Configuration Options");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);

    setLayout(layout);
}

void QicsRichTextCellDisplayConfigurationWidget::readConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

void QicsRichTextCellDisplayConfigurationWidget::writeConfiguration(QicsCellDisplay *cellDisplay)
{
    Q_UNUSED(cellDisplay);
}

QicsCellDisplay *QicsRichTextCellDisplayFactory::createCellDisplay()
{
    return new QicsRichTextCellDisplay();
}

QicsCellDisplayConfigurationWidget *QicsRichTextCellDisplayFactory::configurationWidget()
{
    return new QicsRichTextCellDisplayConfigurationWidget();
}


