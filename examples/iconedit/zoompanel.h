/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef ZOOMPANEL_H
#define ZOOMPANEL_H

#include <QDockWidget>

class QLabel;
class QComboBox;

class ZoomPanel : public QDockWidget
{
    Q_OBJECT
public:
    ZoomPanel(QWidget *parent = 0);
    ~ZoomPanel();

    int currentZoom() const;

public slots:
    void setIcon(const QImage& icon);

signals:
        void zoomChanged(int);

private:
    QLabel *m_actualView;
    QLabel *m_size;
    QComboBox *m_zoomBox;

private slots:
    void emitZoom(int);
};

#endif // ZOOMPANEL_H


