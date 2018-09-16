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

#include "QicsCellStyle.h"

#include <QApplication>
#include "QicsTextCellDisplay.h"
#include "QicsWidgetCellDisplay.h"
#include "QicsTable.h"

QVector<QicsStyle::QicsStylePropertyType> QicsCellStyle::myCellStyleTypeList;
QStringList QicsCellStyle::myPropertyNames;

/////////////////////////////////////////////////////////////////////////////////

QicsCellStyle::QicsCellStyle(const QicsCellStyle& style)
    : QicsStyle()
{
    myStyleTypeList = style.myStyleTypeList;
    myNumProperties = style.myNumProperties;
    init();

    // copy properties via operator=
    *this = style;
}

/////////////////////////////////////////////////////////////////////////////////

QicsCellStyle& QicsCellStyle::operator=(const QicsCellStyle& style)
{
    for (int i = 0; i < myNumProperties; ++i)
        setValue((QicsCellStyleProperty)i, style.getValue((QicsCellStyleProperty)i));

    return *this;
}

QicsCellStyle::QicsCellStyle(Qics::QicsGridType type, bool create_defaults)
{
    if (myCellStyleTypeList.isEmpty()) {
        //myCellStyleTypeList << QicsT_QPalette;                // Palette
        myCellStyleTypeList << QicsT_Int;                       // BorderWidth
        myCellStyleTypeList << QicsT_QPen;                      // TopBorderPen
        myCellStyleTypeList << QicsT_QPen;                      // LeftBorderPen
        myCellStyleTypeList << QicsT_QPen;                      // RightBorderPen
        myCellStyleTypeList << QicsT_QPen;                      // BottomBorderPen
        myCellStyleTypeList << QicsT_Int;                       // CellMargin
        myCellStyleTypeList << QicsT_Boolean;                   // Enabled
        //myCellStyleTypeList << QicsT_Boolean;                 // Current
        myCellStyleTypeList << QicsT_Boolean;                   // Selected
        myCellStyleTypeList << QicsT_Boolean;                   // ReadOnly
        myCellStyleTypeList << QicsT_Int;                       // Alignment
        myCellStyleTypeList << QicsT_Int;                       // TextFlags
        myCellStyleTypeList << QicsT_QFont;                     // Font
        myCellStyleTypeList << QicsT_QCursor;                   // Cursor
        myCellStyleTypeList << QicsT_QicsCellDisplay;           // CellDisplayer
        myCellStyleTypeList << QicsT_QicsDataItemFormatter;     // Formatter
        myCellStyleTypeList << QicsT_QPixmap;                   // Pixmap
        myCellStyleTypeList << QicsT_QString;                   // PixmapName
        myCellStyleTypeList << QicsT_Int;                       // PixmapSpacing
        myCellStyleTypeList << QicsT_QValidator;                // Validator
        myCellStyleTypeList << QicsT_QString;                   // Label
        myCellStyleTypeList << QicsT_Int;                       // MaxLength
        myCellStyleTypeList << QicsT_Pointer;                   // UserData
        myCellStyleTypeList << QicsT_Boolean;                   // Hidden
        myCellStyleTypeList << QicsT_QString;                   // ToolTipText
        myCellStyleTypeList << QicsT_QColor;                    // EditBackgroundColor
        myCellStyleTypeList << QicsT_QColor;                    // EditForegroundColor
        myCellStyleTypeList << QicsT_PasteValidator;            // PasteValidator
        myCellStyleTypeList << QicsT_Boolean;                   // autorecognition
        myCellStyleTypeList << QicsT_Boolean;                   // canAcceptDrops
        myCellStyleTypeList << QicsT_QColor;                    // ForegroundColor
        myCellStyleTypeList << QicsT_QColor;                    // BackgroundColor
        myCellStyleTypeList << QicsT_QColor;                    // WindowTextColor
        myCellStyleTypeList << QicsT_QColor;                    // WindowColor
        myCellStyleTypeList << QicsT_QColor;                    // SelectedForegroundColor
        myCellStyleTypeList << QicsT_QColor;                    // SelectedBackgroundColor
        myCellStyleTypeList << QicsT_QFont;                     // SelectedFont
        myCellStyleTypeList << QicsT_QColor;                    // HighlightedForegroundColor
        myCellStyleTypeList << QicsT_QColor;                    // HighlightedBackgroundColor
    }

    if (myPropertyNames.isEmpty()) {
        //myPropertyNames << "palette";                         // Palette
        myPropertyNames << "borderWidth";                       // BorderWidth
        myPropertyNames << "topBorderPen";                      // TopBorderPen
        myPropertyNames << "leftBorderPen";                     // LeftBorderPen
        myPropertyNames << "rightBorderPen";                    // RightBorderPen
        myPropertyNames << "bottomBorderPen";                   // BottomBorderPen
        myPropertyNames << "cellMargin";                        // CellMargin
        myPropertyNames << "enabled";                           // Enabled
        //myPropertyNames << "current";                         // Current
        myPropertyNames << "selected";                          // Selected
        myPropertyNames << "readOnly";                          // ReadOnly
        myPropertyNames << "alignment";                         // Alignment
        myPropertyNames << "textFlags";                         // TextFlags
        myPropertyNames << "font";                              // Font
        myPropertyNames << "cursor";                            // Cursor
        myPropertyNames << "cellDisplayer";                     // CellDisplayer
        myPropertyNames << "formatter";                         // Formatter
        myPropertyNames << "pixmap";                            // Pixmap
        myPropertyNames << "pixmapName";                        // PixmapName
        myPropertyNames << "pixmapSpacing";                     // PixmapSpacing
        myPropertyNames << "validator";                         // Validator
        myPropertyNames << "label";                             // Label
        myPropertyNames << "maxLength";                         // MaxLength
        myPropertyNames << "userData";                          // UserData
        myPropertyNames << "hidden";                            // Hidden
        myPropertyNames << "toolTipText";                       // ToolTipText
        myPropertyNames << "editBackground";                    // EditBackgroundColor
        myPropertyNames << "editForeground";                    // EditForegroundColor
        myPropertyNames << "pasteValidator";                    // paste validator
        myPropertyNames << "autoRecognition";                   // Auto recognition
        myPropertyNames << "canAcceptDrops";                    // canAccpetDrops
        myPropertyNames << "foreground";                        // ForegroundColor
        myPropertyNames << "background";                        // BackgroundColor
        myPropertyNames << "windowtext";                        // WindowTextColor
        myPropertyNames << "window";                            // WindowColor
        myPropertyNames << "selectedForeground";                // SelectedForegroundColor
        myPropertyNames << "selectedBackground";                // SelectedBackgroundColor
        myPropertyNames << "selectedFont";                      // SelectedFont
        myPropertyNames << "highlightedForeground";             // HighlightedForegroundColor
        myPropertyNames << "highlightedBackground";             // HighlightedBackgroundColor
    }

    myStyleTypeList = &QicsCellStyle::myCellStyleTypeList;
    myNumProperties = QicsCellStyle::LastProperty;

    init();

    if (create_defaults) {
        bool t = true;
        bool f = false;
        int intval;

        QPalette pal = QApplication::palette();

        if (type != Qics::TableGrid)
            pal.setColor(QPalette::Base, pal.window().color());

        //setValue(Palette, &pal);
        QColor color = pal.text().color();
        setValue(ForeColor, &color);
        color = pal.base().color();
        setValue(BackColor, &color);
        color = pal.highlightedText().color();
        setValue(SelForeColor, &color);
        color = pal.highlight().color();
        setValue(SelBackColor, &color);
        color = pal.windowText().color();
        setValue(WindowTextColor, &color);
        color = pal.window().color();
        setValue(WindowColor, &color);

        QFont fnt = QApplication::font();
        setValue(Font, &fnt);
        fnt.setFamily("");
        setValue(SelectedFont, &fnt);

        QCursor cursor;
        setValue(Cursor, &cursor);

        intval = 1;
        setValue(BorderWidth, &intval);

        QPen pen;
        pen.setStyle(Qt::NoPen);
        setValue(TopBorderPen, &pen);
        setValue(LeftBorderPen, &pen);
        setValue(RightBorderPen, &pen);
        setValue(BottomBorderPen, &pen);

        intval = 1;
        setValue(CellMargin, &intval);

        setValue(Enabled, &t);
        //        setValue(Current, &f);
        setValue(Selected, &f);
        setValue(ReadOnly, &f);

        intval = Qt::AlignLeft;
        setValue(Alignment, &intval);
#ifdef NOTDEF
        intval = 0;
        setValue(Qt::TextFlag, &intval);
#endif

        setValue(Pixmap, 0);

        intval = 6;
        setValue(PixmapSpacing, &intval);

        setValue(Formatter, 0);
        setValue(Validator, 0);

        setValue(PasteValidator,0);

        QString str;
        setValue(Label, &str);

        intval = 32767;
        setValue(MaxLength, &intval);

        setValue(UserData, 0);

        setValue(Hidden, &f);

        setValue(ToolTipText, &str);
        setValue(AutoRecognition, &f );

        setValue(EditBackgroundColor, 0);
        setValue(EditForegroundColor, 0);

        setValue(HighlightBackColor, 0);
        setValue(HighlightForeColor, 0);

        setValue(CanAcceptDrops, &t);
    }
}

QDomElement QicsCellStyle::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement cellElement = doc->createElement(tag);

    // Let do this via integer.  NO COMPAT WITH FUTURE VERSIONS!!!!
    for (int i = 0; i < myNumProperties; ++i) {
        void* voidPtr = 0;
        QVariant value;
        QicsCellStyle::QicsCellStyleProperty prop = (QicsCellStyle::QicsCellStyleProperty) i;

        //Hacks
        QStringList listOfColors;

        switch (myStyleTypeList->at(i))
        {
        case QicsT_Int:
            voidPtr = getValue(prop);
            if (voidPtr)
                value = QVariant(*(static_cast<const int *>(voidPtr)));
            break;

        case QicsT_Float:
            voidPtr = getValue(prop);
            if (voidPtr)
                value = QVariant(*(static_cast<const float *>(voidPtr)));
            break;

        case QicsT_QString:
            voidPtr = getValue(prop);
            if (voidPtr)
                value = QVariant(*(static_cast<const QString*>(voidPtr)));
            break;

        case QicsT_QColor:
            voidPtr = getValue(prop);
            if (voidPtr)
                value = QVariant(*(static_cast<const QColor*>(voidPtr)));
            break;

        case QicsT_Boolean:
            voidPtr = getValue(prop);
            if (voidPtr)
                value = QVariant(*(static_cast<const bool*>(voidPtr)));
            break;

        case QicsT_QFont:
            voidPtr = getValue(prop);
            if (voidPtr)
                value = QVariant(*(static_cast<const QFont*>(voidPtr)));
            break;

        case QicsT_QPalette:
            voidPtr = getValue(prop);
            if (voidPtr)
                value = QVariant(paletteToString(static_cast<const QPalette*>(voidPtr)));
            break;

        case QicsT_QicsCellDisplay:
            voidPtr = getValue(prop);
            if (voidPtr) {
                QicsCellDisplay* disp = static_cast<QicsCellDisplay *>(voidPtr);
                QDomElement dispE = disp->toDomXml("cellDisplay", doc);
                cellElement.appendChild(dispE);
            }
            break;

        case QicsT_QPen:
            voidPtr = getValue(prop);
            if (voidPtr) {
                const QPen * pen = static_cast<const QPen*>(voidPtr);
                value = QVariant(penToString(pen));
            }
            break;

        default:
            break;
        }

        // Check to see if we need to catalogue an attribute
        if (value.isValid()) {
            QString attrName = myPropertyNames[i];
            QString attrValue = value.toString();
            if(i != Selected)
                cellElement.setAttribute(attrName, attrValue);
        }
    }

    return cellElement;
}


void QicsCellStyle::configureFromDomXml(const QDomElement& e, QicsTable* table)
{
    // #### TODO: Ask Chris about Memor Leaks here
    QVariant value;

    // Hacks
    QStringList listOfColors;
    QColor color;

    QicsCellStyle::QicsCellStyleProperty prop;

    for(int i = 0; i < myNumProperties; ++i) {
        // skip unstorable properties
        switch ((QicsCellStyle::QicsCellStyleProperty) i) {
            case Selected:
                continue;
            default: break;
        }

        QString attrName = myPropertyNames[i];
        QString attrValue = e.attribute(attrName);

        if( !attrValue.isNull()) {

            prop = static_cast<QicsCellStyle::QicsCellStyleProperty>(i);

            switch (myStyleTypeList->at(i)) {
            case QicsT_Int:
                setValue(prop, (void*) new int(attrValue.toInt()));
                break;
            case QicsT_Float:
                setValue(prop, (void*) new float(attrValue.toFloat()));
                break;
            case QicsT_QString:
                setValue(prop, (void*) new QString(attrValue));

                // if this is pixmap path - load pixmap as well
                if (i == PixmapName)
                    setValue(QicsCellStyle::Pixmap, (void*) new QPixmap(table->defaultImagePath() + attrValue));
                break;
            case QicsT_QColor: {
                    value = QVariant(attrValue);
                    setValue(prop, (void*) new QColor(value.value<QColor>()));
                    break;
                }
            case QicsT_Boolean: {
                    value = QVariant(attrValue);
                    setValue(prop, (void*) new bool(value.toBool()));
                    break;
                }
            case QicsT_QFont: {
                    value = QVariant(attrValue);
                    setValue(prop, (void*) new QFont(value.value<QFont>()));
                    break;
                }
            case QicsT_QPen: {
                    value = QVariant(attrValue);
                    setValue(prop, (void*) stringToPen(attrValue));
                    break;
                }
            case QicsT_QPalette:
                // There is realy only 4 values of use for us in here.
                // Background/Foregound stuff
                // Regular and Selected
                // We expect this to be a csv of colors
                // Shold We Get current palette instead of a new one?
                setValue(prop, static_cast<void *>(stringToPalette(attrValue)));
                break;

            default:
                break;
            }
        }

        if (table) {
            QDomElement dispE = e.firstChildElement("cellDisplay");

            QicsCellDisplay* disp = table->createCellDisplay(dispE.attribute("name"));
            if(disp) {
                disp->configureFromDomXml(dispE);
                clear(CellDisplayer);
                setValue(CellDisplayer, static_cast<void *>(disp));
            }
        }
    }
}

void QicsCellStyle::encode(QDataStream &ds) const
{
    for (int i = 0; i < myNumProperties; ++i) {
        // skip unstorable properties
        switch ((QicsCellStyle::QicsCellStyleProperty) i) {
            case Selected:
            case Hidden:
                continue;
            default: break;
        }

        void* voidPtr = getValue((QicsCellStyle::QicsCellStyleProperty) i);

        if (!voidPtr) {
            ds << int(-1);
            continue;
        }

        QicsStylePropertyType pt = myStyleTypeList->at(i);
        ds << static_cast<int>(pt);

        switch (pt)
        {
        case QicsT_Int:
            ds << *static_cast<int *>(voidPtr);
            break;
        case QicsT_Float:
            ds << *static_cast<float *>(voidPtr);
            break;
        case QicsT_QString:
            ds << *static_cast<QString *>(voidPtr);
            break;
        case QicsT_QColor:
            ds << *static_cast<QColor *>(voidPtr);
            break;
        case QicsT_Boolean:
            ds << *static_cast<bool *>(voidPtr);
            break;
        case QicsT_QFont:
            ds << *static_cast<QFont *>(voidPtr);
            break;
        case QicsT_QPalette:
            ds << *static_cast<QPalette *>(voidPtr);
            break;
        case QicsT_QCursor:
            ds << *static_cast<QCursor *>(voidPtr);
            break;
        case QicsT_QPixmap:
            ds << *static_cast<QPixmap *>(voidPtr);
            break;
        case QicsT_QPen:
            ds << *static_cast<QPen *>(voidPtr);
            break;
        case QicsT_QicsCellDisplay:
        case QicsT_QicsDataItemFormatter:
        case QicsT_QValidator:
        case QicsT_PasteValidator:
        case QicsT_Pointer:
            ds << quintptr(voidPtr);
            break;
        default:
            break;
        }
    }
}

QicsCellStyle* QicsCellStyle::decode(QDataStream &ds, bool sameGrid)
{
    QicsCellStyle *cs = new QicsCellStyle(Qics::TableGrid);
    int pt;
    QicsCellStyleProperty it;

    for (int i = 0; i < cs->myNumProperties; ++i) {
        // skip unstorable properties
        switch ((QicsCellStyle::QicsCellStyleProperty) i) {
            case Selected:
            case Hidden:
                continue;
            default: break;
        }

        ds >> pt;

        if (pt == -1) continue;
        it = static_cast<QicsCellStyle::QicsCellStyleProperty>(i);

        switch (pt)
        {
        case QicsT_Int: {
                int i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_Float: {
                float i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_QString: {
                QString i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_QColor: {
                QColor i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_Boolean: {
                bool i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_QFont: {
                QFont i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_QPalette: {
                QPalette i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_QCursor: {
                QCursor i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_QPixmap: {
                QPixmap i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_QPen: {
                QPen i;
                ds >> i;
                cs->setValue(it, static_cast<void *>(&i));
                break;
            }
        case QicsT_QicsCellDisplay: {
                quintptr i;
                ds >> i;
                if (sameGrid) {
                    QicsCellDisplay *cd = reinterpret_cast<QicsCellDisplay *>(i);
                    if (cd) {
                        cd = cd->clone();
                        cs->setValue(it, static_cast<void *>(cd));
                    } else
                        cs->setValue(it, reinterpret_cast<void *>(i));
                }
                break;
            }
        case QicsT_QicsDataItemFormatter:
        case QicsT_QValidator:
        case QicsT_PasteValidator:
        case QicsT_Pointer: {
                quintptr i;
                ds >> i;
                if (sameGrid)
                    cs->setValue(it, reinterpret_cast<void *>(i));
                break;
            }
        default:
            continue;
        }
    }

    return cs;
}


