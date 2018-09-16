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

#ifndef TABLECONFIGDESIGNER_H
#define TABLECONFIGDESIGNER_H

#include "ui_TableConfigDesigner.h"

class QicsTable;

class TableConfigDesigner : public QDialog, private Ui::TableConfigDesigner
{
    Q_OBJECT
public:
    TableConfigDesigner(QicsTable* table, QWidget* parent=0);

public slots:
    void handleVerticalGridLineColorChanged(QColor);
    void handleHorizontalGridLineColorChanged(QColor);

    void handleVerticalGridLineStyleChanged(int);
    void handleHorizontalGridLineStyleChanged(int);

    void handleVerticalGridLineWidthChanged(int);
    void handleHorizontalGridLineWidthChanged(int);

    void handleHorizontalGridLinesVisibleChanged(bool);
    void handleVerticalGridLinesVisibleChanged(bool);

    void handleFrameShapeChanged(int);
    void handleFrameShadowChanged(int);

    void handleAllowUserRowResizeChanged(bool);
    void handleAllowUserColumnResizeChanged(bool);

    void handleAllowUserRowMoveChanged(bool);
    void handleAllowUserColumnMoveChanged(bool);

    void handleGridCellClippingChanged(int);

    void handleCellOverflowBehaviorChanged(int);

    void handleTraversalEditBehaviorChanged(int);

    void handleEnterTraversalDirectionChanged(int);
    void handleTabTraversalDirectionChanged(int);

    void handleMoreTextPixmapClicked();

protected:
    void saveProperty(QString prop_name,QVariant value);

private:
    QicsTable* m_designersTable;
    QString m_moreTextPixmapString;
};

#endif //TABLECONFIGDESIGNER_H


