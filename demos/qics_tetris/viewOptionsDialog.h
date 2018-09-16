/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef VIEWOPTIONSDILOG_H
#define VIEWOPTIONSDILOG_H

#include "ui_OptionsDialog.h"

class ViewSettings;

class ViewOptionsDialog : public QDialog, private Ui::OptionsDialog
{
    Q_OBJECT
public:
    ViewOptionsDialog(ViewSettings* viewSettings, QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *);

protected slots:
    void gridColorPushButton_Click();
    void fieldColorPushButton_Click();
    void figureColorPushButton_Click();
    void randomCheckBoc_stateChanged(int state);
    void buttonBox_Clicked(QAbstractButton* btn);

private:
    ViewSettings* m_viewSettings;
    QColor m_gridColor;
    QColor m_fieldColor;
    QColor m_figureColor;
};

#endif //VIEWOPTIONSDILOG_H


