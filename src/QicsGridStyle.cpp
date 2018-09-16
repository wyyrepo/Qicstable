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

#include "QicsGridStyle.h"

#include <QFrame>
#include <QBitmap>
#include <QPen>
#include <QApplication>
#include "QicsRegion.h"
#include "QicsUtil.h"
#include "QicsMouseMap.h"
#include "QicsRubberBand.h"

QVector<QicsStyle::QicsStylePropertyType> QicsGridStyle::myGridStyleTypeList;
QStringList QicsGridStyle::myPropertyNames;


QicsGridStyle::QicsGridStyle(QicsGridType type, bool create_defaults)
{
    Q_UNUSED(type);

    if (myGridStyleTypeList.isEmpty()) {
        myGridStyleTypeList << QicsT_Boolean;           // HorizontalGridLinesVisible
        myGridStyleTypeList << QicsT_Boolean;           // VerticalGridLinesVisible
        myGridStyleTypeList << QicsT_Int;               // HorizontalGridLineWidth
        myGridStyleTypeList << QicsT_Int;               // VerticalGridLineWidth
        myGridStyleTypeList << QicsT_Int;               // HorizontalGridLineStyle
        myGridStyleTypeList << QicsT_Int;               // VerticalGridLineStyle
        myGridStyleTypeList << QicsT_QPen;              // HorizontalGridLinePen
        myGridStyleTypeList << QicsT_QPen;              // VerticalGridLinePen
        myGridStyleTypeList << QicsT_Boolean;           // FillHandleVisible
        myGridStyleTypeList << QicsT_Int;               // GridCellClipping
        myGridStyleTypeList << QicsT_Boolean;           // DrawPartialCells
        myGridStyleTypeList << QicsT_Boolean;           // AllowUserResize
        myGridStyleTypeList << QicsT_Boolean;           // AllowHeaderResize
        myGridStyleTypeList << QicsT_Boolean;           // AllowUserMove
        myGridStyleTypeList << QicsT_Boolean;           // AllowUserEdit
        myGridStyleTypeList << QicsT_Boolean;           // ClickToEdit
        myGridStyleTypeList << QicsT_QCursor;           // ResizeCursor
        myGridStyleTypeList << QicsT_Boolean;           // AutoSelectCellContents,
        myGridStyleTypeList << QicsT_Int;               // TraversalKeyEditBehavior,
        myGridStyleTypeList << QicsT_Int;               // EnterTraversalDirection,
        myGridStyleTypeList << QicsT_Int;               // TabTraversalDirection
        myGridStyleTypeList << QicsT_Boolean;           // EnterTraverseToBegin,
        myGridStyleTypeList << QicsT_Boolean;           // TabTraverseToBegin
        myGridStyleTypeList << QicsT_Int;               // GridRepaintBehavior
        myGridStyleTypeList << QicsT_Int;               // CellOverflowBehavior
        myGridStyleTypeList << QicsT_QPoint;            // CellToolTipOffset
        myGridStyleTypeList << QicsT_Int;               // MaxOverflowCells
        //myGridStyleTypeList << QicsT_Boolean;         // Current
        myGridStyleTypeList << QicsT_Int;               // CurrentCellStyle
        myGridStyleTypeList << QicsT_Int;               // CurrentCellBorderWidth
        myGridStyleTypeList << QicsT_Int;               // FrameLineWidth
        myGridStyleTypeList << QicsT_Int;               // FrameStyle
        myGridStyleTypeList << QicsT_QPalette;
        myGridStyleTypeList << QicsT_QicsRegion;        // Viewport
        myGridStyleTypeList << QicsT_QPixmap;           // MoreTextPixmap
        myGridStyleTypeList << QicsT_Boolean;           // DragEnabled
        myGridStyleTypeList << QicsT_Int;               // LayoutDirection
        myGridStyleTypeList << QicsT_QicsMouseMap;      // mapMouseButton
        myGridStyleTypeList << QicsT_Int;               // FakeCellsDrawingIndex
        myGridStyleTypeList << QicsT_Int;               // SelectionStyle
        myGridStyleTypeList << QicsT_Int;               // ÑurrentCellDraggingPolicy
        myGridStyleTypeList << QicsT_Int;               // CellDecorationStyle
        myGridStyleTypeList << QicsT_Int;               // ExclusiveSelectionBorderWidth
        myGridStyleTypeList << QicsT_QColor;            // ExclusiveSelectionBorderColor
        myGridStyleTypeList << QicsT_QColor;            // ExclusiveSelectionHandleBackColor
        myGridStyleTypeList << QicsT_QColor;            // ExclusiveSelectionHandleColor
        myGridStyleTypeList << QicsT_QColor;            // ExclusiveSelectionDragBackColor
        myGridStyleTypeList << QicsT_QPen;              // ExclusiveSelectionDragPen
        myGridStyleTypeList << QicsT_Int;               // ExclusiveSelectionDragMode
        myGridStyleTypeList << QicsT_Float;             // ExclusiveSelectionDragOpacity
        myGridStyleTypeList << QicsT_Boolean;           // HighlightSelection
    }

    myStyleTypeList = &QicsGridStyle::myGridStyleTypeList;
    myNumProperties = QicsGridStyle::LastProperty;

    if (myPropertyNames.isEmpty()) {
        myPropertyNames << "horizontalGridLinesVisible";    // HorizontalGridLinesVisible
        myPropertyNames << "verticalGridLinesVisible";      // VerticalGridLinesVisible
        myPropertyNames << "horizontalGridLineWidth";       // HorizontalGridLineWidth
        myPropertyNames << "verticalGridLineWidth";         // VerticalGridLineWidth
        myPropertyNames << "horizontalGridLineStyle";       // HorizontalGridLineStyle
        myPropertyNames << "vertivcalGridLineStyle";        // VerticalGridLineStyle
        myPropertyNames << "horizontalGridLinePen";         // HorizontalGridLinePen
        myPropertyNames << "verticalGridLinePen";           // VerticalGridLinePen
        myPropertyNames << "fillHandleVisible";             // FillHandleVisible
        myPropertyNames << "gridCellClipping";              // GridCellClipping
        myPropertyNames << "drawPartialCells";              // DrawPartialCells
        myPropertyNames << "allowUserResize";               // AllowUserResize
        myPropertyNames << "allowHeaderResize";             // allowHeaderResize
        myPropertyNames << "allowUserMove";                 // AllowUserMove
        myPropertyNames << "allowUserEdit";                 // AllowUserEdit
        myPropertyNames << "clickToEdit";                   // ClickToEdit
        myPropertyNames << "resizeCursor";                  // ResizeCursor
        myPropertyNames << "autoSelectCellContents";        // AutoSelectCellContents,
        myPropertyNames << "traversalKeyEditBehavior";      // TraversalKeyEditBehavior,
        myPropertyNames << "enterTraversalDirection";       // EnterTraversalDirection,
        myPropertyNames << "tabTraversalDirection";         // TabTraversalDirection
        myPropertyNames << "enterTraverseToBegin";          // EnterTraverseToBegin
        myPropertyNames << "tabTraverseToBegin";            // TabTraverseToBegin
        myPropertyNames << "gridRepaintBehavior";           // GridRepaintBehavior
        myPropertyNames << "cellOverFlowBehavior";          // CellOverflowBehavior
        myPropertyNames << "cellToolTipOffset";             // CellToolTipOffset
        myPropertyNames << "maxOverflowCells";              // MaxOverflowCells
        //myPropertyNames << "current";                     // Current
        myPropertyNames << "currentCellStyle";              // CurrentCellStyle
        myPropertyNames << "currentCellBorderWidth";        // CurrentCellBorderWidth
        myPropertyNames << "frameLineWidth";                // FrameLineWidth
        myPropertyNames << "frameStyle";                    // FrameStyle
        myPropertyNames << "gridPalette";
        myPropertyNames << "viewport";                      // Viewport
        myPropertyNames << "moreTextPixmap";                // MoreTextPixmap
        myPropertyNames << "dragEnabled";                   // DragEnabled
        myPropertyNames << "layoutDirection";               // LayoutDirection
        myPropertyNames << "mapMouseButton";
        myPropertyNames << "fakeCellsDrawingIndex";
        myPropertyNames << "selectionStyle";
        myPropertyNames << "currentCellDraggingPolicy";     // ÑurrentCellDraggingPolicy
        myPropertyNames << "cellDecorationStyle";
        myPropertyNames << "exclusiveSelectionBorderWidth";
        myPropertyNames << "exclusiveSelectionBorderColor";
        myPropertyNames << "exclusiveSelectionHandleBackColor";
        myPropertyNames << "exclusiveSelectionHandleColor";
        myPropertyNames << "exclusiveSelectionDragBackColor";
        myPropertyNames << "exclusiveSelectionDragPen";
        myPropertyNames << "exclusiveSelectionDragMode";
        myPropertyNames << "exclusiveSelectionDragOpacity";
        myPropertyNames << "highlightSelection";
    }

    init();

    // Initialize the style values

    if (create_defaults) {
        bool t = true;
        bool f = false;
        int intval;

        QPalette pal = QApplication::palette();
        setValue(GridPalette, &pal);

        QicsGridCellClipping ap = Qics::AllowPartial;
        setValue(GridCellClipping, &ap);

        setValue(HorizontalGridLinesVisible, &t);
        setValue(VerticalGridLinesVisible, &t);

        setValue(FillHandleVisible, &t);

        intval = 1;
        setValue(HorizontalGridLineWidth, &intval);
        setValue(VerticalGridLineWidth, &intval);

        QicsLineStyle plain = Qics::Plain;
        setValue(HorizontalGridLineStyle, &plain);
        setValue(VerticalGridLineStyle, &plain);

        QPen pen;
        setValue(HorizontalGridLinePen, &pen);
        setValue(VerticalGridLinePen, &pen);

        intval = 1;
        setValue(CellDecorationStyle, &intval);

        setValue(DrawPartialCells, &t);

        setValue(AllowUserResize, &t);
        setValue(AllowHeaderResize,&t);
        setValue(AllowUserMove, &f);
        setValue(AllowUserEdit, &f);
        setValue(ClickToEdit, &f);

        QCursor c;
        setValue(ResizeCursor, &c);

        setValue(AutoSelectCellContents, &t);

        QicsTraversalKeyEditBehavior eb = Qics::TraverseWithinCell;
        setValue(TraversalKeyEditBehavior, &eb);

        Qt::Orientation vert = Qt::Vertical;
        setValue(EnterTraversalDirection, &vert);

        Qt::Orientation horiz = Qt::Horizontal;
        setValue(TabTraversalDirection, &horiz);

        setValue(EnterTraverseToBegin,&t);
        setValue(TabTraverseToBegin,&t);

        Qics::QicsRepaintBehavior ro = Qics::RepaintOn;
        setValue(GridRepaintBehavior, &ro);

        Qics::QicsCellOverflowBehavior clip = Qics::Clip;
        setValue(CellOverflowBehavior, &clip);

        QPoint pos(0,0);
        setValue(CellToolTipOffset, &pos);

        intval = 10;
        setValue(MaxOverflowCells, &intval);

        Qics::QicsCurrentCellStyle cstyle = Qics::Spreadsheet;
        setValue(CurrentCellStyle, &cstyle);

        QicsSelectionStyle style = Qics::Normal;
        setValue(SelectionStyle, &style);

        intval = 2;
        setValue(CurrentCellBorderWidth, &intval);

        intval = 0; //2?
        setValue(FrameLineWidth, &intval);

        //intval = QFrame::StyledPanel | QFrame::Sunken;
        intval = QFrame::NoFrame;
        //if (type != Qics::TableGrid)
        //    intval = QFrame::NoFrame;// | QFrame::Sunken;
        setValue(FrameStyle, &intval);

        QicsRegion maxreg(0, 0, Qics::QicsLAST_ROW, Qics::QicsLAST_COLUMN);

        setValue(Viewport, &maxreg);

        QPixmap mtp(Qics_arrow_xpm);
        mtp.setMask(mtp.createHeuristicMask(true));
        setValue(MoreTextPixmap, &mtp);

        setValue(DragEnabled, &t);

        Qt::LayoutDirection layoutDirect = Qt::LeftToRight;
        setValue(LayoutDirection, &layoutDirect);

        setValue( MapMouseButton, new QicsMouseMap() );

        Qics::QicsIndexType type = Qics::NoIndex;
        setValue(FakeCellsDrawingIndex, &type);

        QicsCurrentCellDraggingPolicy policy = Qics::DragCurrent;
        setValue(CurrentCellDraggingPolicy, &policy);

        intval = 2;
        setValue(ExclusiveSelectionBorderWidth, &intval);

        QColor colorval;
        colorval = Qt::black;
        setValue(ExclusiveSelectionBorderColor, &colorval);
        setValue(ExclusiveSelectionHandleColor, &colorval);

        colorval = Qt::white;
        setValue(ExclusiveSelectionHandleBackColor, &colorval);

        colorval = Qt::blue;
        setValue(ExclusiveSelectionDragBackColor, &colorval);

        pen = QPen(Qt::black, 3);
        setValue(ExclusiveSelectionDragPen, &pen);

        float floatval = 0.5;
        setValue(ExclusiveSelectionDragOpacity, &floatval);

        intval = (int)QicsRubberBand::DM_CUSTOM;
        setValue(ExclusiveSelectionDragMode, &intval);

        setValue(HighlightSelection, &f);
    }
}

QDomElement QicsGridStyle::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement gridElement = doc->createElement(tag);

    // Let do this via integer.  NO COMPAT WITH FUTURE VERSIONS!!!!
    for(int i=0; i < myNumProperties; ++i) {
        void* voidPtr=0;
        QVariant value;
        QicsGridStyle::QicsGridStyleProperty prop = (QicsGridStyle::QicsGridStyleProperty) i;
        QStringList listOfProperties;

        switch(myStyleTypeList->at(i))
        {
        case QicsT_Int:
            voidPtr = getValue(prop);
            if(voidPtr)
                value = QVariant(*(static_cast<const int *>(voidPtr)));
            break;
        case QicsT_Float:
            voidPtr = getValue(prop);
            if(voidPtr)
                value = QVariant(*(static_cast<const float *>(voidPtr)));
            break;
        case QicsT_QPoint:
            voidPtr = getValue(prop);
            if(voidPtr)
                value = QVariant(*(static_cast<const QPoint *>(voidPtr)));
            break;
        case QicsT_QString:
            voidPtr = getValue(prop);
            if(voidPtr)
                value = QVariant(*(static_cast<const QString*>(voidPtr)));
            break;
        case QicsT_QColor:
            voidPtr = getValue(prop);
            if(voidPtr)
                value = QVariant(*(static_cast<const QColor*>(voidPtr)));
            break;
        case QicsT_Boolean:
            voidPtr = getValue(prop);
            if(voidPtr)
                value = QVariant(*(static_cast<const bool*>(voidPtr)));
            break;
        case QicsT_QWidget:
            // We don't copy this
            break;
        case QicsT_QFont:
            voidPtr = getValue(prop);
            if(voidPtr)
                value = QVariant(*(static_cast<const QFont*>(voidPtr)));
            break;
        case QicsT_QPalette:
            voidPtr = getValue(prop);
            if(voidPtr)
                value = QVariant(paletteToString(static_cast<const QPalette*>(voidPtr)));
            break;
        case QicsT_QCursor:
            // Don't Save
            break;
        case QicsT_QicsCellDisplay:
            // We don't copy this
            break;
        case QicsT_QicsDataItemFormatter:
            // We don't copy this
            break;
        case QicsT_QPixmap:
            //Dont Save
            break;
        case QicsT_QValidator:
            // We don't copy this
            break;
        case QicsT_QPen:
            voidPtr = getValue(prop);
            if(voidPtr) {
                const QPen * pen = static_cast<const QPen*>(voidPtr);
                value = QVariant(penToString(pen));
            }
            break;
        case QicsT_QicsRegion:
            // Dont Save
            break;
        case QicsT_Pointer:
            // We don't copy this
            break;
        default:
            break;
        }

        if( value.isValid() ) {
            QString attrName = myPropertyNames.at(i);
            QString attrValue = value.toString();
            gridElement.setAttribute(attrName, attrValue);
        }
    }

    return gridElement;
}

void QicsGridStyle::configureFromDomXml(const QDomElement& e)
{
    // #### Ask Chris about Memory Leaks here
    QVariant value;
    QicsGridStyle::QicsGridStyleProperty prop;

    for(int i=0; i < myNumProperties; ++i) {
        QString attrName = myPropertyNames.at(i);

        prop = static_cast<QicsGridStyle::QicsGridStyleProperty>(i);

        QString attrValue = e.attribute(attrName);
        if( !attrValue.isNull() ) {
            switch(myStyleTypeList->at(i))
            {
            case QicsT_Int:
                setValue(prop, (void*) new int(attrValue.toInt()));
                break;
            case QicsT_Float:
                setValue(prop, (void*) new float(attrValue.toFloat()));
                break;
            case QicsT_QPoint:
                value = QVariant(attrValue);
                setValue(prop, (void*) new QPoint(value.value<QPoint>()));
                break;
            case QicsT_QString:
                setValue(prop, (void*) new QString(attrValue));
                break;
            case QicsT_QColor:
                value = QVariant(attrValue);
                setValue(prop, (void*) new QColor(value.value<QColor>()));
                break;
            case QicsT_Boolean:
                value = QVariant(attrValue);
                setValue(prop, (void*) new bool(value.toBool()));
                break;
            case QicsT_QWidget:
                // We don't save this
                break;
            case QicsT_QFont:
                value = QVariant(attrValue);
                setValue(prop, (void*) new QFont(value.value<QFont>()));
                break;
            case QicsT_QPalette:
                setValue(prop,(void*) stringToPalette(attrValue));
                break;
            case QicsT_QCursor:
                // Don't Save
                break;
            case QicsT_QicsCellDisplay:
                // We don't copy this
                break;
            case QicsT_QicsDataItemFormatter:
                // We don't copy this
                break;
            case QicsT_QPixmap:
                //Dont Save
                break;
            case QicsT_QValidator:
                // We don't copy this
                break;
            case QicsT_QPen:
                setValue(prop,(void*) stringToPen(attrValue));
                break;
            case QicsT_QicsRegion:
                // We don't save this
                break;
            case QicsT_Pointer:
                // We don't copy this
                break;
            default:
                break;
            }
        }
    }
}


