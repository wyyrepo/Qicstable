/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "setupdialog.h"

#include <QStyleFactory>

#include <QicsTable.h>


QString SetupDialog::m_lastStyle;


SetupDialog::SetupDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    // set available styles
    QStringList styles = QStyleFactory::keys();
    for (int i = 0; i < styles.size(); ++i) {
        const QString &name = styles.at(i);
        cbStyle->addItem(name, name);
    }
}

SetupDialog::~SetupDialog()
{
}

int SetupDialog::exec(const QList<QicsTable*>& tables)
{
    // init
    if (tables.isEmpty()) return false;

    m_tables = tables;
    QicsTable *table = m_tables.first();

    // style
    cbStyle->setCurrentIndex( cbStyle->findData(m_lastStyle) );

    // overflowing
    cbOverflow->setCurrentIndex(table->cellOverflowBehavior());

    // grid
    cbHGrid->setChecked(table->horizontalGridLinesVisible());
    cbVGrid->setChecked(table->verticalGridLinesVisible());

    // copy policy
    int cp = table->copyPolicy();
    cbCopyData->setChecked(cp & Qics::CopyData);
    cbCopyAttr->setChecked(cp & Qics::CopyAttributes);
    cbCopySize->setChecked(cp & Qics::CopyDimensions);
    cbCopySpan->setChecked(cp & Qics::CopySpans);

    // clear policy
    int clp = table->clearPolicy();
    cbClearData->setChecked(clp & Qics::ClearData);
    cbClearAttr->setChecked(clp & Qics::ClearAttributes);
    cbClearSpan->setChecked(clp & Qics::ClearSpans);

    // selection policy
    cbSelectionPolicy->setCurrentIndex(table->selectionPolicy());
    cbExclusiveSelection->setChecked(table->selectionStyle() == Qics::Exclusive);

    // headers
    cbRHMovable->setChecked(table->rowHeaderRef().allowUserMove());
    cbCHMovable->setChecked(table->columnHeaderRef().allowUserMove());
    cbRHUserResizable->setChecked(table->rowHeaderRef().allowUserResize());
    cbCHUserResizable->setChecked(table->columnHeaderRef().allowUserResize());
    cbRHResizable->setChecked(table->rowHeaderRef().allowHeaderResize());
    cbCHResizable->setChecked(table->columnHeaderRef().allowHeaderResize());
    cbExtDrag->setChecked(table->headerDragStyle() == Qics::StyledDrag);

    if (QDialog::exec()) {
        on_bApply_clicked();
        return true;
    }

    return false;
}

void SetupDialog::on_bApply_clicked()
{
    // set style
    m_lastStyle = cbStyle->itemData( cbStyle->currentIndex() ).toString();
    QApplication::setStyle(m_lastStyle);

    // copy policy
    int cp = 0;
    if (cbCopyData->isChecked()) cp |= Qics::CopyData;
    if (cbCopyAttr->isChecked()) cp |= Qics::CopyAttributes;
    if (cbCopySize->isChecked()) cp |= Qics::CopyDimensions;
    if (cbCopySpan->isChecked()) cp |= Qics::CopySpans;

    // clear policy
    int clp = 0;
    if (cbClearData->isChecked()) clp |= Qics::ClearData;
    if (cbClearAttr->isChecked()) clp |= Qics::ClearAttributes;
    if (cbClearSpan->isChecked()) clp |= Qics::ClearSpans;

    // selection policy
    Qics::QicsSelectionPolicy selp = (Qics::QicsSelectionPolicy)cbSelectionPolicy->currentIndex();
    Qics::QicsSelectionStyle sels = (cbExclusiveSelection->isChecked() ? Qics::Exclusive : Qics::Normal);

    // overflowing
    Qics::QicsCellOverflowBehavior over = (Qics::QicsCellOverflowBehavior)cbOverflow->currentIndex();

    for (int i = 0; i < m_tables.size(); ++i) {
        QicsTable *table = m_tables.at(i);

        table->setCopyPolicy(static_cast<Qics::QicsCopyPolicy>(cp));
        table->setClearPolicy(clp);

        table->setSelectionPolicy(selp);
        table->setSelectionStyle(sels);

        // headers
        table->rowHeaderRef().setAllowUserMove(cbRHMovable->isChecked());
        table->columnHeaderRef().setAllowUserMove(cbCHMovable->isChecked());
        table->rowHeaderRef().setAllowUserResize(cbRHUserResizable->isChecked());
        table->columnHeaderRef().setAllowUserResize(cbCHUserResizable->isChecked());
        table->rowHeaderRef().setAllowHeaderResize(cbRHResizable->isChecked());
        table->columnHeaderRef().setAllowHeaderResize(cbCHResizable->isChecked());
        table->setHeaderDragStyle(cbExtDrag->isChecked() ? Qics::StyledDrag : Qics::SimpleDrag);

        // overflowing
        table->setCellOverflowBehavior(over);

        // grid
        table->setHorizontalGridLinesVisible(cbHGrid->isChecked());
        table->setVerticalGridLinesVisible(cbVGrid->isChecked());
    }
}


