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

#include "QicsCellCommon.h"

#include <QDir>
#include <QFileInfo>


QicsCellCommon::QicsCellCommon(QObject *parent)
    : QObject(parent), m_info(0)
{
}

QicsCellCommon::QicsCellCommon(QicsGridInfo *info, QObject *parent)
    : QObject(parent), m_info(info)
{
}

void QicsCellCommon::setInfo(QicsGridInfo *info)
{
    if (!info || m_info == info)
        return;

    m_info = info;
}

int QicsCellCommon::margin() const
{
    return (* static_cast<int *> (getAttr(QicsCellStyle::CellMargin)));
}

void QicsCellCommon::setMargin(int margin)
{
    m_info->setGlobalRepaintBehavior(Qics::RepaintOff);

    // update the dimension manager with the new margin
    setDMMargin(margin);

    setAttr(QicsCellStyle::CellMargin, static_cast<const void *> (&margin));

    m_info->revertGlobalRepaintBehavior();
}

QFont QicsCellCommon::font() const
{
    return (* static_cast<QFont *> (getAttr(QicsCellStyle::Font)));
}

void QicsCellCommon::setFont(const QFont &font)
{
    // update the dimension manager with the new font
    setDMFont(font);

    setAttr(QicsCellStyle::Font, static_cast<const void *>(&font));
}

QFont QicsCellCommon::selectedFont() const
{
    return (* static_cast<QFont *> (getAttr(QicsCellStyle::SelectedFont)));
}

void QicsCellCommon::setSelectedFont(const QFont &font)
{
    // update the dimension manager with the new font
    setAttr(QicsCellStyle::SelectedFont, static_cast<const void *>(&font));
}

QPalette QicsCellCommon::palette() const
{
    QPalette pal;
    pal.setColor(QPalette::Active, QPalette::Text, foregroundColor());
    pal.setColor(QPalette::Active, QPalette::Base, backgroundColor());
    pal.setColor(QPalette::Active, QPalette::HighlightedText, selectedForegroundColor());
    pal.setColor(QPalette::Active, QPalette::Highlight, selectedBackgroundColor());
    pal.setColor(QPalette::Active, QPalette::WindowText, windowTextColor());
    pal.setColor(QPalette::Active, QPalette::Window, windowColor());
    return pal;
}

void QicsCellCommon::setPalette(const QPalette &pal)
{
    QColor color;

    color = pal.text().color();
    setAttr(QicsCellStyle::ForeColor, static_cast<const void *> (&color));
    color = pal.base().color();
    setAttr(QicsCellStyle::BackColor, static_cast<const void *> (&color));
    color = pal.highlightedText().color();
    setAttr(QicsCellStyle::SelForeColor, static_cast<const void *> (&color));
    color = pal.highlight().color();
    setAttr(QicsCellStyle::SelBackColor, static_cast<const void *> (&color));
    color = pal.window().color();
    setAttr(QicsCellStyle::WindowColor, static_cast<const void *> (&color));
    color = pal.windowText().color();
    setAttr(QicsCellStyle::WindowTextColor, static_cast<const void *> (&color));
}

void QicsCellCommon::setWindowTextColor(const QColor &color)
{
    setAttr(QicsCellStyle::WindowTextColor, static_cast<const void *> (&color));
}

QColor QicsCellCommon::windowTextColor() const
{
    return *(QColor*) getAttr(QicsCellStyle::WindowTextColor);
}

void QicsCellCommon::setWindowColor(const QColor &color)
{
    setAttr(QicsCellStyle::WindowColor, static_cast<const void *> (&color));
}

QColor QicsCellCommon::windowColor() const
{
    return *(QColor*) getAttr(QicsCellStyle::WindowColor);
}

QColor QicsCellCommon::foregroundColor() const
{
    return *(QColor*) getAttr(QicsCellStyle::ForeColor);
}

void QicsCellCommon::setForegroundColor(const QColor &color)
{
    setAttr(QicsCellStyle::ForeColor, static_cast<const void *> (&color));
}

QColor QicsCellCommon::backgroundColor() const
{
    return *(QColor*) getAttr(QicsCellStyle::BackColor);
}

void QicsCellCommon::setBackgroundColor(const QColor &color)
{
    setAttr(QicsCellStyle::BackColor, static_cast<const void *> (&color));
}

QColor QicsCellCommon::selectedForegroundColor() const
{
    return *(QColor*) getAttr(QicsCellStyle::SelForeColor);
}

void QicsCellCommon::setSelectedForegroundColor(const QColor &color)
{
    setAttr(QicsCellStyle::SelForeColor, static_cast<const void *> (&color));
}

QColor QicsCellCommon::selectedBackgroundColor() const
{
    return *(QColor*) getAttr(QicsCellStyle::SelBackColor);
}

void QicsCellCommon::setSelectedBackgroundColor(const QColor &color)
{
    setAttr(QicsCellStyle::SelBackColor, static_cast<const void *> (&color));
}

bool QicsCellCommon::readOnly() const
{
    return (* static_cast<bool *> (getAttr(QicsCellStyle::ReadOnly)));
}

void QicsCellCommon::setReadOnly(bool b)
{
    setAttr(QicsCellStyle::ReadOnly, static_cast<const void *> (&b));
}

QPixmap QicsCellCommon::pixmap() const
{
    QPixmap *pix = static_cast<QPixmap *> (getAttr(QicsCellStyle::Pixmap));

    if (pix)
        return (*pix);

    return QPixmap();
}

void QicsCellCommon::setPixmap(const QPixmap &p)
{
    if (p.isNull())
        clearAttr(QicsCellStyle::Pixmap);
    else {
        // fit the pixmap into the cell
        int mr = margin() * 4;
        setDMFitSize(p.width() + mr, p.height() + mr);

        setAttr(QicsCellStyle::Pixmap, static_cast<const void *> (&p));
    }
}

void QicsCellCommon::setPixmap(const QString &file)
{
    setPixmap(QPixmap(file));

    QFileInfo fi(file);
    QString s = fi.canonicalFilePath();
    if (s.startsWith(m_info->defaultImagePath()))
        s = s.remove(m_info->defaultImagePath());
    else
        s = fi.fileName();
    setAttr(QicsCellStyle::PixmapName, static_cast<const void *> (&s));
}

int QicsCellCommon::pixmapSpacing() const
{
    return (* static_cast<int *> (getAttr(QicsCellStyle::PixmapSpacing)));
}

void QicsCellCommon::setPixmapSpacing(int sp)
{
    setAttr(QicsCellStyle::PixmapSpacing, static_cast<const void *> (&sp));
}

QicsCellDisplay *QicsCellCommon::displayer() const
{
    return (static_cast<QicsCellDisplay *>(getAttr(QicsCellStyle::CellDisplayer)));
}

void QicsCellCommon::setDisplayer(QicsCellDisplay *dsp)
{
    setAttr(QicsCellStyle::CellDisplayer, static_cast<const void *> (dsp));
}

QicsDataItemFormatter *QicsCellCommon::formatter() const
{
    return (static_cast<QicsDataItemFormatter *>(getAttr(QicsCellStyle::Formatter)));
}

void QicsCellCommon::setFormatter(QicsDataItemFormatter *fmt)
{
    setAttr(QicsCellStyle::Formatter, static_cast<const void *>(fmt));
}

int QicsCellCommon::alignment() const
{
    return (* static_cast<int *> (getAttr(QicsCellStyle::Alignment)));
}

void QicsCellCommon::setAlignment(int flags)
{
    setAttr(QicsCellStyle::Alignment, static_cast<const void *> (&flags));
}

bool QicsCellCommon::enabled() const
{
    return (* static_cast<bool *> (getAttr(QicsCellStyle::Enabled)));
}

void QicsCellCommon::setEnabled(bool b)
{
    setAttr(QicsCellStyle::Enabled, static_cast<const void *> (&b));
}

bool QicsCellCommon::selected() const
{
    return false;
    //### TODO: return (* static_cast<bool *> (getAttr(QicsCellStyle::Selected)));
}

int QicsCellCommon::textFlags() const
{
    void *flag = getAttr(QicsCellStyle::TextFlags);

    if (flag)
        return (* static_cast<int *> (flag));

    return 0;
}

void QicsCellCommon::setTextFlags(int flags)
{
    setAttr(QicsCellStyle::TextFlags, static_cast<const void *> (&flags));
}

QValidator *QicsCellCommon::validator() const
{
    return (static_cast<QValidator *> (getAttr(QicsCellStyle::Validator)));
}

void QicsCellCommon::setValidator(QValidator *v)
{
    setAttr(QicsCellStyle::Validator, static_cast<const void *> (v));
}

QicsPasteValidator *QicsCellCommon::pasteValidator() const
{
    return (static_cast<QicsPasteValidator *> (getAttr(QicsCellStyle::PasteValidator)));
}

void QicsCellCommon::setPasteValidator(QicsPasteValidator *v)
{
    setAttr(QicsCellStyle::PasteValidator, static_cast<const void *> (v));
}

QString QicsCellCommon::label() const
{
    return (* static_cast<QString *> (getAttr(QicsCellStyle::Label)));
}

void QicsCellCommon::setLabel(const QString &label)
{
    setAttr(QicsCellStyle::Label, static_cast<const void *> (&label));
}

int QicsCellCommon::maxLength() const
{
    return (* static_cast<int *> (getAttr(QicsCellStyle::MaxLength)));
}

void QicsCellCommon::setMaxLength(int len)
{
    setAttr(QicsCellStyle::MaxLength, static_cast<const void *> (&len));
}

const QCursor &QicsCellCommon::cursor() const
{
    return (* static_cast<QCursor *> (getAttr(QicsCellStyle::Cursor)));
}

void QicsCellCommon::setCursor(const QCursor &c)
{
    setAttr(QicsCellStyle::Cursor, static_cast<const void *> (&c));
}

void QicsCellCommon::unsetCursor()
{
    clearAttr(QicsCellStyle::Cursor);
}

int QicsCellCommon::borderWidth() const
{
    return (* static_cast<int *> (getAttr(QicsCellStyle::BorderWidth)));
}

void QicsCellCommon::setDMFitSize(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void QicsCellCommon::setBorderWidth(int bw)
{
    m_info->setGlobalRepaintBehavior(RepaintOff);

    // update the dimension manager with the new border width
    setDMBorderWidth(bw);

    setAttr(QicsCellStyle::BorderWidth, static_cast<const void *> (&bw));

    m_info->revertGlobalRepaintBehavior();
}

QPen QicsCellCommon::topBorderPen() const
{
    return (* static_cast<QPen *> (getAttr(QicsCellStyle::TopBorderPen)));
}

QPen QicsCellCommon::leftBorderPen() const
{
    return (* static_cast<QPen *> (getAttr(QicsCellStyle::LeftBorderPen)));
}

QPen QicsCellCommon::rightBorderPen() const
{
    return (* static_cast<QPen *> (getAttr(QicsCellStyle::RightBorderPen)));
}

QPen QicsCellCommon::bottomBorderPen() const
{
    return (* static_cast<QPen *> (getAttr(QicsCellStyle::BottomBorderPen)));
}

void QicsCellCommon::setTopBorderPen(const QPen &pen)
{
    setAttr(QicsCellStyle::TopBorderPen, static_cast<const void *> (&pen));
}

void QicsCellCommon::setLeftBorderPen(const QPen &pen)
{
    setAttr(QicsCellStyle::LeftBorderPen, static_cast<const void *> (&pen));
}

void QicsCellCommon::setRightBorderPen(const QPen &pen)
{
    setAttr(QicsCellStyle::RightBorderPen, static_cast<const void *> (&pen));
}

void QicsCellCommon::setBottomBorderPen(const QPen &pen)
{
    setAttr(QicsCellStyle::BottomBorderPen, static_cast<const void *> (&pen));
}

void *QicsCellCommon::userData() const
{
    return getAttr(QicsCellStyle::UserData);
}

void QicsCellCommon::setUserData(void *data)
{
    setAttr(QicsCellStyle::UserData, static_cast<const void *> (data));
}

QString QicsCellCommon::toolTipText() const
{
    return (* static_cast<QString *> (getAttr(QicsCellStyle::ToolTipText)));
}

void QicsCellCommon::setToolTipText(const QString &txt)
{
    setAttr(QicsCellStyle::ToolTipText, static_cast<const void *> (&txt));
}

QColor QicsCellCommon::editForegroundColor() const
{
    QColor *color = static_cast<QColor *>(getAttr(QicsCellStyle::EditForegroundColor));

    return (color ? *color : foregroundColor());
}

void QicsCellCommon::setEditForegroundColor(const QColor &color)
{
    setAttr(QicsCellStyle::EditForegroundColor, static_cast<const void *> (&color));
}

QColor QicsCellCommon::editBackgroundColor() const
{
    QColor *color = static_cast<QColor *>
        (getAttr(QicsCellStyle::EditBackgroundColor));

    return (color ? *color : backgroundColor());
}

void QicsCellCommon::setEditBackgroundColor(const QColor &color)
{
    setAttr(QicsCellStyle::EditBackgroundColor, static_cast<const void *> (&color));
}

QColor QicsCellCommon::highlightForegroundColor() const
{
    QColor *color = static_cast<QColor *>
        (getAttr(QicsCellStyle::HighlightForeColor));

    return (color ? *color : foregroundColor());
}

void QicsCellCommon::setHighlightForegroundColor(const QColor &color)
{
    setAttr(QicsCellStyle::HighlightForeColor,
        static_cast<const void *> (&color));
}

QColor QicsCellCommon::highlightBackgroundColor() const
{
    QColor *color = static_cast<QColor *>
        (getAttr(QicsCellStyle::HighlightBackColor));

    return (color ? *color : backgroundColor());
}

void QicsCellCommon::setHighlightBackgroundColor(const QColor &color)
{
    setAttr(QicsCellStyle::HighlightBackColor,
        static_cast<const void *> (&color));
}

void QicsCellCommon::setAutoRecognition(const bool flag)
{
    setAttr( QicsCellStyle::AutoRecognition,
        static_cast <const void *> (&flag));
}

bool QicsCellCommon::autoRecognition()
{
    return (*(static_cast <bool *>
        (getAttr(QicsCellStyle::AutoRecognition))));
}

bool QicsCellCommon::canAcceptDrops() const
{
    bool * isCan = (static_cast <bool*> (getAttr(QicsCellStyle::CanAcceptDrops)));
#ifdef notdef
    qDebug("QicsCellCommon::canAcceptDrops: %d", *isCan);
#endif
    return *isCan;
}

void QicsCellCommon::setCanAcceptDrops( const bool val )
{
#ifdef notdef
    qDebug("QicsCellCommon::setCanAcceptDrops: %d", val);
#endif
    setAttr(QicsCellStyle::CanAcceptDrops,
        static_cast<const void *> (&val));
}


