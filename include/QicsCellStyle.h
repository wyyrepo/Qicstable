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

#ifndef QICSCELLSTYLE_H
#define QICSCELLSTYLE_H

#include <QVector>
#include <QDomElement>
#include <QicsStyle.h>

class QWidget;
class QicsTable;

/*! \internal
* \class QicsCellStyle
* \brief Represents properties of a row, column, region or cell.
*
* QicsCellStyle contains settings for all properties that pertain to a single
* row, column, region or cell.  These properties are used by QicsTable and
* its subwidgets to specify different aspects of the table's display and
* behavior.
*
* The interface of QicsCellStyle is very simple.  You can set a property
* (QicsCellStyle::setValue), get a property (QicsCellStyle::getValue), or clear
* a property (QicsCellStyle::clear).  A property is defined by its name,
* an enumerated value that allows for fast retrieval.
*/

////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QicsCellStyle: public QicsStyle
{
public:
    /*! \internal
    * The names of the different properties that can be set.
    *
    * \b IMPORTANT:  Make sure that LastProperty is always the last
    * property name in the enumeration.
    */
    enum QicsCellStyleProperty {
        //Palette = 0,

        BorderWidth,
        TopBorderPen,
        LeftBorderPen,
        RightBorderPen,
        BottomBorderPen,
        CellMargin,
        Enabled,
        //Current,
        Selected,
        ReadOnly,
        Alignment,
        TextFlags,
        Font,
        Cursor,
        CellDisplayer,
        Formatter,
        Pixmap,
        PixmapName,
        PixmapSpacing,
        Validator,
        Label,
        MaxLength,
        UserData,
        Hidden,				// do - row/col only ?
        ToolTipText,
        EditBackgroundColor,
        EditForegroundColor,
        PasteValidator,
        AutoRecognition,
        CanAcceptDrops,

        ForeColor,
        BackColor,
        WindowTextColor,
        WindowColor,
        SelForeColor,
        SelBackColor,
        SelectedFont,
        HighlightForeColor,
        HighlightBackColor,

        LastProperty,

        // virtual properties start from here
        FontFamily,
        FontSize,
        FontWeight,
        FontBold,
        FontItalic,
        FontUnderline,
        FontOverline,
        FontStrikeOut,
        FontFixed,
        FontKerning,
        FontStretch,

        LastVirtualProperty
    };


    /*! \internal
    * Creates a new QicsCellStyle object of type \a type.
    * If \a create_default = \b false,
    * all property pointers are intitialized to 0.  If
    * \a create_default = \b true, default values will be set for each
    * property.  This is really only necessary when creating the
    * default style for the entire table.  Note that it is NOT
    * true that every property in the default style will have a non-zero
    * value.  Some of the object pointer properties will still be 0.
    */
    QicsCellStyle(QicsGridType type, bool create_default = false);

    /*! \internal
    * Copy Constructor.
    * \since 2.2
    */
    QicsCellStyle(const QicsCellStyle&);

    /*! \internal
    * Copy Operator.
    * \since 2.2
    */
    QicsCellStyle& operator=(const QicsCellStyle&);

    QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;

    void configureFromDomXml(const QDomElement&, QicsTable* table);

    void encode(QDataStream &ds) const;

    static QicsCellStyle* decode(QDataStream &ds, bool sameGrid);

protected:
    /*! \internal
    * A static const array of \a QicsCellStylePropertyType values which are
    * listed in the SAME order as the QicsCellStyleProperty list. These can
    * be used to determine the type of the property at position \a N in
    * the list of properties \a myProperties.
    */
    static QVector<QicsStylePropertyType> myCellStyleTypeList;
    static QStringList myPropertyNames;
};

typedef QVector<QicsCellStyle *> QicsCellStylePV;
typedef QVector<QicsCellStylePV *> QicsCellStylePVPV;

#endif //QICSCELLSTYLE_H


