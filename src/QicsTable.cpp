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

#include "QicsTable.h"

#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "QicsDataModelDefault.h"
#include "QicsScrollManager.h"
#include "QicsSelectionManager.h"
#include "QicsTextCellDisplay.h"
#include "QicsScrollBarScroller.h"
#include "QicsTableRegionDrag.h"
#include "QicsGridLayout.h"
#include "QicsMouseMap.h"
#include "QicsRowHeader.h"
#include "QicsColumnHeader.h"
#include "QicsMainGrid.h"
#include "QicsCell.h"
#include "QicsCellRegion.h"
#include "QicsStyleManager.h"
#ifndef QICSTABLE_GPL
#include "QicsTablePrint.h"
#endif
#include "QicsPasteValidator.h"
#include "QicsCheckCellDisplay.h"
#include "QicsComboCellDisplay.h"
#include "QicsDateTimeCellDisplay.h"
#include "QicsPushButtonCellDisplay.h"
#include "QicsNavigator.h"
#include "QicsAbstractAttributeController.h"
#include "QicsAbstractClipboardDelegate.h"
#include "QicsStandardSorterDelegate.h"
#include "QicsUtil.h"
#include "QicsTable_p.h" //QicsTable logo
#ifdef EVALDIALOG
#include "QicsEvalDialog.h"
#endif

const QString QicsTable_version_string = QICSTABLE_VERSION;

#define LEFT_VHEADER_IDX    0
#define RIGHT_VHEADER_IDX   1
#define TOP_VHEADER_IDX     0
#define MIDDLE_VHEADER_IDX  1
#define BOTTOM_VHEADER_IDX  2

#define LEFT_TOP_HEADER     (myVHeaders[TOP_VHEADER_IDX][LEFT_VHEADER_IDX])
#define LEFT_HEADER         (myVHeaders[MIDDLE_VHEADER_IDX][LEFT_VHEADER_IDX])
#define LEFT_BOTTOM_HEADER  (myVHeaders[BOTTOM_VHEADER_IDX][LEFT_VHEADER_IDX])
#define RIGHT_TOP_HEADER    (myVHeaders[TOP_VHEADER_IDX][RIGHT_VHEADER_IDX])
#define RIGHT_HEADER        (myVHeaders[MIDDLE_VHEADER_IDX][RIGHT_VHEADER_IDX])
#define RIGHT_BOTTOM_HEADER (myVHeaders[BOTTOM_VHEADER_IDX][RIGHT_VHEADER_IDX])

#define LEFT_HHEADER_IDX    0
#define MIDDLE_HHEADER_IDX  1
#define RIGHT_HHEADER_IDX   2
#define TOP_HHEADER_IDX     0
#define BOTTOM_HHEADER_IDX  1

#define TOP_LEFT_HEADER     (myHHeaders[TOP_HHEADER_IDX][LEFT_HHEADER_IDX])
#define TOP_HEADER          (myHHeaders[TOP_HHEADER_IDX][MIDDLE_HHEADER_IDX])
#define TOP_RIGHT_HEADER    (myHHeaders[TOP_HHEADER_IDX][RIGHT_HHEADER_IDX])
#define BOTTOM_LEFT_HEADER  (myHHeaders[BOTTOM_HHEADER_IDX][LEFT_HHEADER_IDX])
#define BOTTOM_HEADER       (myHHeaders[BOTTOM_HHEADER_IDX][MIDDLE_HHEADER_IDX])
#define BOTTOM_RIGHT_HEADER (myHHeaders[BOTTOM_HHEADER_IDX][RIGHT_HHEADER_IDX])

// Indices for the myGrids 2-dimensional array
#define GRID_TOP_IDX     0
#define GRID_MIDDLE_IDX  1
#define GRID_BOTTOM_IDX  2
#define GRID_LEFT_IDX    0
#define GRID_RIGHT_IDX   2

// The grids
#define TOP_LEFT_GRID       (myGrids[GRID_TOP_IDX][GRID_LEFT_IDX])
#define TOP_MIDDLE_GRID     (myGrids[GRID_TOP_IDX][GRID_MIDDLE_IDX])
#define TOP_RIGHT_GRID      (myGrids[GRID_TOP_IDX][GRID_RIGHT_IDX])
#define MIDDLE_LEFT_GRID    (myGrids[GRID_MIDDLE_IDX][GRID_LEFT_IDX])
#define MAIN_GRID           (myGrids[GRID_MIDDLE_IDX][GRID_MIDDLE_IDX])
#define MIDDLE_RIGHT_GRID   (myGrids[GRID_MIDDLE_IDX][GRID_RIGHT_IDX])
#define BOTTOM_LEFT_GRID    (myGrids[GRID_BOTTOM_IDX][GRID_LEFT_IDX])
#define BOTTOM_MIDDLE_GRID  (myGrids[GRID_BOTTOM_IDX][GRID_MIDDLE_IDX])
#define BOTTOM_RIGHT_GRID   (myGrids[GRID_BOTTOM_IDX][GRID_RIGHT_IDX])


QicsTable::QicsTable(QicsDataModel *model, QWidget *parent)
    : QFrame(parent)
{
    init(model, 0,0, 0,0, 0,0, 0,0, 0);
}

QicsTable::QicsTable(QWidget *parent)
    : QFrame(parent)
{
    init(0, 0,0, 0,0, 0,0, 0,0, 0);
}

QicsTable::QicsTable(QicsDataModel::Foundry rhdmf,
                     QicsDataModel::Foundry chdmf,
                     QicsTableGrid::Foundry tf,
                     QicsHeaderGrid::Foundry hf,
                     QicsDataModel *model,
                     QWidget *parent)
    : QFrame(parent)
{
    init(model, rhdmf,chdmf, 0,0, tf,hf, 0,0, 0);
}

QicsTable::QicsTable(void *key,
                     QicsDataModel::FoundryWithKey rhdmf_wk,
                     QicsDataModel::FoundryWithKey chdmf_wk,
                     QicsTableGrid::FoundryWithKey tf_wk,
                     QicsHeaderGrid::FoundryWithKey hf_wk,
                     QicsDataModel *model,
                     QWidget *parent)
    : QFrame(parent)
{
    init(model, 0,0, rhdmf_wk,chdmf_wk, 0,0, tf_wk,hf_wk, key);
}

QicsTable::~QicsTable()
{
    delete styleManager();
    delete rhStyleManager();
    delete chStyleManager();

    delete dimensionManager();
    delete rhDimensionManager();
    delete chDimensionManager();

    delete m_selectionManager;

    delete m_rowScroller;
    delete m_columnScroller;

    delete myCurrentCell;

    delete m_tableCommon->myRHDataModel;
    delete m_tableCommon->myCHDataModel;

    qDeleteAll(displayFactories);
    qDeleteAll(createdDisplays);

    delete myCellRegion;
}

void QicsTable::init(QicsDataModel *model,
                QicsDataModel::Foundry rhdmf,
                QicsDataModel::Foundry chdmf,
                QicsDataModel::FoundryWithKey rhdmf_wk,
                QicsDataModel::FoundryWithKey chdmf_wk,
                QicsTableGrid::Foundry tf,
                QicsHeaderGrid::Foundry hf,
                QicsTableGrid::FoundryWithKey tf_wk,
                QicsHeaderGrid::FoundryWithKey hf_wk,
                void *key)
{
#ifdef EVALDIALOG
    QicsEvalDialog::checkEvalLicense("QicsTable");
#endif
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);

    myKeyboardManager.setTableGeometry(&myGridGeometry);
    myKeyboardManager.setTraverseIntoFrozen(true);
    myKeyboardManager.setTraverseIntoHeaders(false);

    m_topFrozenRows = 0;
    m_bottomFrozenRows = 0;
    m_leftFrozenColumns = 0;
    m_rightFrozenColumns = 0;
    myTopTitleWidget = 0;
    myBottomTitleWidget = 0;
    myLeftTitleWidget = 0;
    myRightTitleWidget = 0;
    myTopLeftCornerWidget = 0;
    myTopRightCornerWidget = 0;
    myBottomLeftCornerWidget = 0;
    myBottomRightCornerWidget = 0;
    myUnfreezingFlag = false;
    m_navAllowed = true;
    m_frozenLineWidth = 0;

    myClipboardDelegate = 0;

    myDefaultCellDisplayer = 0;
    myCellRegion = 0;

    m_tableCommon = new QicsTableCommon(this);

    if (rhdmf)
        myRowHeaderDMFoundry = rhdmf;
    else
        myRowHeaderDMFoundry = QicsDataModelDefault::create;

    if (chdmf)
        myColumnHeaderDMFoundry = chdmf;
    else
        myColumnHeaderDMFoundry = QicsDataModelDefault::create;

    if (tf)
        myTableGridFoundry = tf;
    else
        myTableGridFoundry = QicsTableGrid::createGrid;

    if (hf)
        myHeaderGridFoundry = hf;
    else
        myHeaderGridFoundry = QicsHeaderGrid::createGrid;

    myRowHeaderDMFoundryWK = rhdmf_wk;
    myColumnHeaderDMFoundryWK = chdmf_wk;

    myTableGridFoundryWK = tf_wk;
    myHeaderGridFoundryWK = hf_wk;

    myFoundryKey = key;

    initDataModels(model);
    initObjects();
    initGridInfoObjects();

    // we need to keep the three data models in sync so that when
    // rows and columns are inserted or deleted in the main data model,
    // the header data models do the same.
    if(model) {
        gridInfo().controlRowsOf(&rhGridInfo());
        gridInfo().controlColumnsOf(&chGridInfo());
    }

    // use the default main grid viewport as the initial value
    myFullViewport = mainGridViewport();

    // create the subwidgets for the table
    initDisplay();

    configureFrozen();

    if (model == 0)
        setViewport(QicsRegion(-1,-1,-1,-1));
}

bool QicsTable::configureFromXmlFile(const QString& fileName)
{
    if(!styleManager())
        return false;

    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    QTextStream ts(&f);
    QString s = ts.readAll();
    f.close();

    configureFromXml(s);
    return true;
}

void QicsTable::configureFromXml(const QString& xmlString)
{
    if(!styleManager())
        return;

    QDomDocument doc;
    QString errorMsg;
    int errorCol;
    int errorRow;
    bool ok = doc.setContent(xmlString, &errorMsg, &errorRow, &errorCol);

    if(!ok)
        qDebug("Parse Error: %s\tRow:%d\tCol%d", qPrintable(errorMsg), errorRow, errorCol );

    // Find "QicsTableProperties"
    //XXX Walk tree. Dont assume this will be a child
    QDomElement tableElement = doc.firstChildElement("QicsTableProperties");

    Qics::QicsRepaintBehavior old_rb = repaintBehavior();
    setRepaintBehavior(Qics::RepaintOff);

    configureFromDomXml(tableElement);

    setRepaintBehavior(old_rb);
}

QString QicsTable::configurationToXml()
{
    QDomDocument doc("QicsTableProperties");
    configurationToDomXml(&doc);
    return doc.toString();
}

void QicsTable::configureFromDomXml(const QDomElement& e)
{
    QDomElement generalElement = e.firstChildElement("GlobalTableProperties");
    if (!generalElement.isNull())
        propertiesFromDomXml(generalElement, this);

    QDomElement styleManagerElement = e.firstChildElement("StyleManager");
    if(!styleManagerElement.isNull())
        styleManager()->configureFromDomXml(styleManagerElement, this);

    QDomElement chStyleManagerElement = e.firstChildElement("ColumnHeaderStyleManager");
    if(!chStyleManagerElement.isNull())
        chStyleManager()->configureFromDomXml(chStyleManagerElement, this);

    QDomElement rhStyleManagerElement = e.firstChildElement("RowHeaderStyleManager");
    if(!rhStyleManagerElement.isNull())
        rhStyleManager()->configureFromDomXml(rhStyleManagerElement, this);

    QDomElement dimensionManagerElement = e.firstChildElement("DimensionManager");
    if(!dimensionManagerElement.isNull())
        dimensionManager()->configureFromDomXml(dimensionManagerElement);

    // update commons
    setVerticalGridLineWidth(verticalGridLineWidth());
    setVerticalGridLinesVisible(verticalGridLinesVisible());
    setHorizontalGridLineWidth(horizontalGridLineWidth());
    setHorizontalGridLinesVisible(horizontalGridLinesVisible());

    setFrameLineWidth(frameLineWidth());
    setFrameStyle(frameStyle());
}

QDomElement QicsTable::configurationToDomXml(QDomDocument* doc)
{
    QDomElement root = doc->createElement("QicsTableProperties");

    // Style Manager && Span Manager
    if(styleManager()) {
        QDomElement smElement = styleManager()->toDomXml("StyleManager",doc);
        root.appendChild(smElement);
    }

    // Column Header Style Manager && Span Manager
    if(chStyleManager()) {
        QDomElement smElement = chStyleManager()->toDomXml("ColumnHeaderStyleManager",doc);
        root.appendChild(smElement);
    }

    // Row Header Style Manager && Span Manager
    if(rhStyleManager()) {
        QDomElement smElement = rhStyleManager()->toDomXml("RowHeaderStyleManager", doc);
        root.appendChild(smElement);
    }
    // Dimension Manager
    if(dimensionManager()) {
        QDomElement dmElement = dimensionManager()->toDomXml(doc);
        root.appendChild(dmElement);
    }

    //Global Table Properties
    QDomElement gpElement = propertiesToDomXml(doc);
    root.appendChild(gpElement);

    doc->appendChild(root);
    return root;
}

QDomElement QicsTable::propertiesToDomXml(QDomDocument* doc) const
{
    QDomElement root = doc->createElement("GlobalTableProperties");

    QDomElement root1 = doc->createElement("Visual");
    root1.setAttribute("TopRow", topRow());
    root1.setAttribute("LeftColumn", leftColumn());

    root1.setAttribute("VisibleRows", (visibleRows()-1));
    root1.setAttribute("VisibleColumns", (visibleColumns()-1));

    root1.setAttribute("HScrollBarMode", int(hScrollBarMode()));
    root1.setAttribute("VScrollBarMode", int(vScrollBarMode()));
    root.appendChild(root1);

    QDomElement root2 = doc->createElement("Headers");
    root2.setAttribute("TopHeaderVisible", topHeaderVisible());
    root2.setAttribute("LeftHeaderVisible", leftHeaderVisible());
    root2.setAttribute("RightHeaderVisible", rightHeaderVisible());
    root2.setAttribute("BottomHeaderVisible", bottomHeaderVisible());
    root.appendChild(root2);

    QDomElement rootG = doc->createElement("General");
    rootG.setAttribute("TableMargin", tableMargin());
    rootG.setAttribute("TableSpacing", tableSpacing());
    rootG.setAttribute("GridSpacing", gridSpacing());
        rootG.setAttribute("FrozenLineWidth", frozenLineWidth());

    rootG.setAttribute("SelectionPolicy", int(selectionPolicy()));

    rootG.setAttribute("ColumnHeaderUsesModel", columnHeaderUsesModel());
    rootG.setAttribute("RowHeaderUsesModel", rowHeaderUsesModel());

    rootG.setAttribute("HorizontalGridLinesVisible", horizontalGridLinesVisible());
    rootG.setAttribute("VerticalGridLinesVisible", verticalGridLinesVisible());
    rootG.setAttribute("FillHandleVisible", fillHandleVisible());
    root.appendChild(rootG);

    QDomElement root3 = doc->createElement("Cells");
    root3.setAttribute("CellOverflowBehavior", int(cellOverflowBehavior()));
    QPoint offset = cellToolTipOffset();
    root3.setAttribute("CellToolTipOffset", QString("%1;%2").arg(offset.x()).arg(offset.y()));
    root3.setAttribute("MaxOverflowCells", maxOverflowCells());

    root3.setAttribute("DrawPartialCells", drawPartialCells());
    root3.setAttribute("GridCellClipping", gridCellClipping());
    root.appendChild(root3);

    QDomElement root4 = doc->createElement("EditingFeatures");
    root4.setAttribute("CurrentCellStyle", currentCellStyle());
    root4.setAttribute("CurrentCellBorderWidth", currentCellBorderWidth());
    root4.setAttribute("ClickToEdit", clickToEdit());
    root4.setAttribute("AutoSelectCellContents", autoSelectCellContents());
    root4.setAttribute("TraversalKeyEditBehavior", int(traversalKeyEditBehavior()));
    root4.setAttribute("EnterTraversalDirection", enterTraversalDirection());
    root4.setAttribute("TabTraversalDirection", tabTraversalDirection());

    root4.setAttribute("NumTopFrozenRows", m_topFrozenRows);
    root4.setAttribute("NumBottomFrozenRows", m_bottomFrozenRows);
    root4.setAttribute("NumLeftFrozenColumns", m_leftFrozenColumns);
    root4.setAttribute("NumRightFrozenColumns", m_rightFrozenColumns);
    // ?????  MoreTextPixmap
    root4.setAttribute("DragEnabled", dragEnabled());
    root4.setAttribute("Label", label());
    root4.setAttribute("ReadOnly", readOnly());

    root4.setAttribute("LayoutDirection", layoutDirection());
    root.appendChild(root4);

    doc->appendChild(root);
    return root;
}

void QicsTable::propertiesFromDomXml(const QDomElement& e, QicsTable*)
{
    setRepaintBehavior(Qics::RepaintOff);
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        QString tag = child.tagName();
        if( tag == "Visual" ) {
            setVisibleRows(  child.attribute("VisibleRows",
                QString::number( visibleRows()-1 ) ).toInt() );
            setVisibleColumns(  child.attribute("VisibleColumns",
                QString::number( visibleColumns() -1 ) ).toInt() );
            setTopRow( child.attribute("TopRow", QString::number(topRow()) ).toInt());
            setLeftColumn( child.attribute("LeftColumn", QString::number(leftColumn()) ).toInt());
            setHScrollBarMode( QicsScrollBarMode(child.attribute("HScrollBarMode",
                QString::number( hScrollBarMode() ) ).toInt()) );
            setVScrollBarMode( QicsScrollBarMode(child.attribute("VScrollBarMode",
                QString::number( vScrollBarMode() ) ).toInt()) );
        }
        else if( tag == "Headers" ) {
            setTopHeaderVisible( child.attribute("TopHeaderVisible",
                QString::number( topHeaderVisible() ) ).toInt() );
            setLeftHeaderVisible( child.attribute("LeftHeaderVisible",
                QString::number( leftHeaderVisible() ) ).toInt() );
            setRightHeaderVisible( child.attribute("RightHeaderVisible",
                QString::number( rightHeaderVisible() ) ).toInt() );
            setBottomHeaderVisible( child.attribute("BottomHeaderVisible",
                QString::number( bottomHeaderVisible() ) ).toInt() );
        }
        else if( tag == "General" ) {
            setTableMargin(child.attribute("TableMargin", QString::number( tableMargin() ) ).toInt());
            setTableSpacing(child.attribute("TableSpacing", QString::number( tableSpacing() ) ).toInt());
            setGridSpacing(child.attribute("GridSpacing", QString::number( gridSpacing() ) ).toInt());
                        setFrozenLineWidth(child.attribute("FrozenLineWidth", QString::number(frozenLineWidth())).toInt());

            setSelectionPolicy(QicsSelectionPolicy(child.attribute("SelectionPolicy",
                QString::number( int(selectionPolicy()) ) ).toInt()));

            setColumnHeaderUsesModel(child.attribute("ColumnHeaderUsesModel",
                QString::number( columnHeaderUsesModel() ) ).toInt());
            setRowHeaderUsesModel(child.attribute("RowHeaderUsesModel",
                QString::number( rowHeaderUsesModel() ) ).toInt());

            setVerticalGridLinesVisible( child.attribute("VerticalGridLinesVisible",
                QString::number( verticalGridLinesVisible() ) ).toInt() );
            setHorizontalGridLinesVisible ( child.attribute("HorizontalGridLinesVisible",
                QString::number( horizontalGridLinesVisible() ) ).toInt() );
            setFillHandleVisible ( child.attribute("FillHandleVisible",
                QString::number( fillHandleVisible() ) ).toInt() );

            QString offsetString = child.attribute("CellToolTipOffset", "0;0");
            QStringList points = offsetString.split(";");
            QPoint offset(points.at(0).toInt(), points.at(1).toInt());
            setCellToolTipOffset(offset);
        }
        else if( tag == "Cells" ) {
            setCellOverflowBehavior(QicsCellOverflowBehavior( child.attribute("CellOverflowBehavior",
                QString::number( int(cellOverflowBehavior()) ) ).toInt()));

            setMaxOverflowCells(child.attribute("MaxOverflowCells",
                QString::number( maxOverflowCells() ) ).toInt());

            setDrawPartialCells(child.attribute("DrawPartialCells",
                QString::number( drawPartialCells() ) ).toInt());
            setGridCellClipping(QicsGridCellClipping( child.attribute("GridCellClipping",
                QString::number( gridCellClipping() ) ).toInt()));
        }
        else if( tag == "EditingFeatures" ) {
            setCurrentCellStyle(QicsCurrentCellStyle(child.attribute("CurrentCellStyle",
                QString::number( currentCellStyle() ) ).toInt()));
            setCurrentCellBorderWidth(child.attribute("CurrentCellBorderWidth",
                QString::number( currentCellBorderWidth() ) ).toInt());

            setClickToEdit(child.attribute("ClickToEdit",
                QString::number(clickToEdit()) ).toInt());
            setAutoSelectCellContents(child.attribute("AutoSelectCellContents",
                QString::number(autoSelectCellContents()) ).toInt());

            setTraversalKeyEditBehavior(QicsTraversalKeyEditBehavior(
                child.attribute("TraversalKeyEditBehavior",
                QString::number(int(traversalKeyEditBehavior())) ).toInt()));
            setEnterTraversalDirection(Qt::Orientation(
                child.attribute("EnterTraversalDirection",
                QString::number(enterTraversalDirection()) ).toInt()));
            setTabTraversalDirection(Qt::Orientation(child.attribute("TabTraversalDirection",
                QString::number(tabTraversalDirection()) ).toInt()));
            // ?????  MoreTextPixmap

            setDragEnabled(child.attribute("DragEnabled",
                QString::number(dragEnabled()) ).toInt());

            setLabel(child.attribute("Label", label()));
            setReadOnly(child.attribute("ReadOnly",
                QString::number(readOnly()) ).toInt());

            setLayoutDirection((Qt::LayoutDirection)child.attribute("LayoutDirection",
                QString::number(layoutDirection()) ).toInt());

            // #### AM: this is really bad. Freezing makes viewport obscured - so there are issues
            // when inserting rows/columns.

            freezeTopRows(child.attribute("NumTopFrozenRows",QString::number(m_topFrozenRows)).toInt());
            freezeBottomRows(child.attribute("NumBottomFrozenRows",QString::number(m_bottomFrozenRows)).toInt());
            freezeLeftColumns(child.attribute("NumLeftFrozenColumns",QString::number(m_leftFrozenColumns)).toInt());
            freezeRightColumns(child.attribute("NumRightFrozenColumns",QString::number(m_rightFrozenColumns)).toInt());
        }

        child = child.nextSiblingElement();
    }

    setRepaintBehavior(Qics::RepaintOn);
}

void QicsTable::setExternalAttributeController(QicsAbstractAttributeController& mac)
{
    setExternalAttributeControllers(mac.createController(), mac.createController(), mac.createController());
}

void QicsTable::setExternalAttributeControllers(QicsAbstractAttributeController* macGrid,
                QicsAbstractAttributeController* macRowHeader,
                QicsAbstractAttributeController* macColumnHeader)
{
    styleManager()->setModelAttributeController(macGrid);
    rhStyleManager()->setModelAttributeController(macRowHeader);
    chStyleManager()->setModelAttributeController(macColumnHeader);

    // set the default cell displayer in the style managers
    if ( !myDefaultCellDisplayer )
        myDefaultCellDisplayer = createCellDisplay(QicsTextCellDisplay::TextCellDisplayName);

    styleManager()->setDefaultProperty(QicsCellStyle::CellDisplayer,
        static_cast<void *> (myDefaultCellDisplayer));
    rhStyleManager()->setDefaultProperty(QicsCellStyle::CellDisplayer,
        static_cast<void *> (myDefaultCellDisplayer));
    chStyleManager()->setDefaultProperty(QicsCellStyle::CellDisplayer,
        static_cast<void *> (myDefaultCellDisplayer));
}

void QicsTable::removeExternalAttributeController()
{
    styleManager()->setModelAttributeController(0);
    rhStyleManager()->setModelAttributeController(0);
    chStyleManager()->setModelAttributeController(0);
}

QicsAbstractAttributeController *QicsTable::gridAttributeController() const
{
    return styleManager()->modelAttributeController();
}

QicsAbstractAttributeController *QicsTable::rowHeaderAttributeController() const
{
    return rhStyleManager()->modelAttributeController();
}

QicsAbstractAttributeController *QicsTable::columnHeaderAttributeController() const
{
    return chStyleManager()->modelAttributeController();
}

void  QicsTable::showNavigator()
{
    QRect view( horizontalScrollBar()->value(), verticalScrollBar()->value(),
        horizontalScrollBar()->pageStep(), verticalScrollBar()->pageStep() );

    QRect total( horizontalScrollBar()->minimum(), verticalScrollBar()->minimum(),
        horizontalScrollBar()->maximum()+horizontalScrollBar()->pageStep(),
        verticalScrollBar()->maximum()+verticalScrollBar()->pageStep() );

    m_navButton->navigate(total, view);
}

void QicsTable::navigate(const QRect &r)
{
    horizontalScrollBar()->setValue(r.left());
    verticalScrollBar()->setValue(r.top());
}

void QicsTable::setNavigatorAllowed(bool allow)
{
    m_navAllowed = allow;
    m_navButton->setVisible(m_navAllowed);
}

void QicsTable::handleWideKeyPressed(QKeyEvent *event)
{
    event->ignore();

    switch (event->key())
    {
    case Qt::Key_Delete: {
        if (isWideKeyAction(Del_DeleteSelected)) {
            deleteSelected();
            event->accept();
            return;
        }
        break;
    }
    case Qt::Key_A: {
        if (event->modifiers() == Qt::ControlModifier)
            if (isWideKeyAction(CtrlA_SelectAll)) {
                selectAll();
                event->accept();
                return;
            }
        break;
    }
    case Qt::Key_C: {
        if (event->modifiers() == Qt::ControlModifier)
            if (isWideKeyAction(CtrlC_Copy)) {
                copy();
                event->accept();
                return;
            }
        break;
    }
    case Qt::Key_X: {
        if (event->modifiers() == Qt::ControlModifier)
            if (isWideKeyAction(CtrlX_Cut)) {
                cut();
                event->accept();
                return;
            }
        break;
    }
    case Qt::Key_V: {
        if (event->modifiers() == Qt::ControlModifier)
            if (isWideKeyAction(CtrlV_Paste)) {
                paste();
                event->accept();
                return;
            }
        break;
    }
    default:
        break;
    }
}

void QicsTable::setWideKeyActions(bool on)
{
    if (on) {
        myWideKeys.insert(Del_DeleteSelected);
        myWideKeys.insert(CtrlA_SelectAll);
        myWideKeys.insert(CtrlC_Copy);
        myWideKeys.insert(CtrlV_Paste);
        myWideKeys.insert(CtrlX_Cut);
    } else
        myWideKeys.clear();
}

void QicsTable::setWideKeyAction(QicsWideKeyAction action, bool on)
{
    if (on)
        myWideKeys.insert(action);
    else
        myWideKeys.remove(action);
}

void QicsTable::handleFillHandleEvent(QicsFillHandleEventType type,
                                 QMouseEvent *event, const QicsSelection *initialSel,
                                 const QicsSelection *currentSel)
{
    switch (type)
    {
    case Qics::FillHandlePressed:
        emit fillHandlePressed(event, initialSel);
        break;
    case Qics::FillHandleDoubleClicked:
        emit fillHandleDoubleClicked(event, initialSel);
        break;
    case Qics::FillHandleReleased:
        emit fillHandleReleased(event, initialSel, currentSel);
        break;
    case Qics::FillHandleDragged:
        emit fillHandleDragged(event, initialSel, currentSel);
        break;
    default:
        break;
    }
}

void QicsTable::handleSelectionListChanged(bool progress)
{
    Q_UNUSED(progress);

    if (rowHeaderRef().highlightSelection())
        rowHeaderRef().gridInfo().updateAllGrids(true);

    if (columnHeaderRef().highlightSelection())
        columnHeaderRef().gridInfo().updateAllGrids(true);
}

void QicsTable::initObjects()
{
    // the main style manager
    setStyleManager(new QicsStyleManager(TableGrid, &gridInfo()));
    // separate style managers for headers
    setRHStyleManager(new QicsStyleManager(RowHeaderGrid, &rhGridInfo()));
    setCHStyleManager(new QicsStyleManager(ColumnHeaderGrid, &chGridInfo()));

    // set the default cell displayer in the style managers
    if (!myDefaultCellDisplayer)
        myDefaultCellDisplayer = createCellDisplay(QicsTextCellDisplay::TextCellDisplayName);

    styleManager()->setDefaultProperty(QicsCellStyle::CellDisplayer,
        static_cast<void *> (myDefaultCellDisplayer));
    rhStyleManager()->setDefaultProperty(QicsCellStyle::CellDisplayer,
        static_cast<void *> (myDefaultCellDisplayer));
    chStyleManager()->setDefaultProperty(QicsCellStyle::CellDisplayer,
        static_cast<void *> (myDefaultCellDisplayer));

    // dimension managers - the main dimension manager first...
    setDimensionManager(new QicsDimensionManager(&gridInfo()));
    // followed by a row header DM that passes on row height queries
    // to the main DM...
    setRHDimensionManager(new QicsDimensionManager(&rhGridInfo()));
    rhDimensionManager()->setControllingRowDimensionManager(dimensionManager());
    // and finally a column header DM that passes on column width queries
    // to the main DM.
    setCHDimensionManager(new QicsDimensionManager(&chGridInfo()));
    chDimensionManager()->setControllingColumnDimensionManager(dimensionManager());

    // keeps track of selections in the table
    m_selectionManager = new QicsSelectionManager();
    m_selectionManager->setDataModel(dataModel());
    m_selectionManager->setStyleManager(styleManager());
    m_selectionManager->setGridInfo(&gridInfo());

    connect(m_selectionManager, SIGNAL(selectionListChangedExternal(bool)),
        this, SIGNAL(selectionListChanged(bool)));
    connect(m_selectionManager, SIGNAL(selectionListChangedExternal(bool)),
        this, SLOT(handleSelectionListChanged(bool)));

    connect(m_selectionManager, SIGNAL(rowSelected(int,bool)),
        this, SIGNAL(rowSelected(int,bool)));

    connect(m_selectionManager, SIGNAL(columnSelected(int,bool)),
        this, SIGNAL(columnSelected(int,bool)));

    connect(dimensionManager(),SIGNAL(rowResized(int,int,int)),this,SIGNAL(rowResized(int,int,int)));
    connect(dimensionManager(),SIGNAL(columnResized(int,int,int)),this,SIGNAL(columnResized(int,int,int)));
    connect(dimensionManager(),SIGNAL(rowHeaderResized(int,int)),this,SIGNAL(rowHeaderResized(int,int)));
    connect(dimensionManager(),SIGNAL(columnHeaderResized(int,int)),this,SIGNAL(columnHeaderResized(int,int)));
    connect(dimensionManager(),SIGNAL(rowVisibilityChanged(int,bool)),this,SIGNAL(rowVisibilityChanged(int,bool)));
    connect(dimensionManager(),SIGNAL(columnVisibilityChanged(int,bool)),this,SIGNAL(columnVisibilityChanged(int,bool)));

    connect(dimensionManager(),SIGNAL(rowVisibilityChanged(int,bool)),styleManager()->spanManager(),SLOT(rowVisibilityChanged(int,bool)));
    connect(dimensionManager(),SIGNAL(columnVisibilityChanged(int,bool)),styleManager()->spanManager(),SLOT(columnVisibilityChanged(int,bool)));
    connect(dimensionManager(),SIGNAL(rowVisibilityChanged(int,bool)),rhStyleManager()->spanManager(),SLOT(rowVisibilityChanged(int,bool)));
    connect(dimensionManager(),SIGNAL(columnVisibilityChanged(int,bool)),rhStyleManager()->spanManager(),SLOT(columnVisibilityChanged(int,bool)));
    connect(dimensionManager(),SIGNAL(rowVisibilityChanged(int,bool)),chStyleManager()->spanManager(),SLOT(rowVisibilityChanged(int,bool)));
    connect(dimensionManager(),SIGNAL(columnVisibilityChanged(int,bool)),chStyleManager()->spanManager(),SLOT(columnVisibilityChanged(int,bool)));

    connect(dimensionManager(),SIGNAL(dimensionChanged(Qics::QicsIndexType,int,int)),this,SLOT(configureFrozen(Qics::QicsIndexType,int,int)));
    connect(&gridInfo(), SIGNAL(filterChanged(int, bool)), this, SLOT(handleFilterChanged(int, bool)));

    // handles all the scrolling
    m_scrollManager = new QicsScrollManager(this);

    m_tableCommon->setLayoutDirection(QApplication::layoutDirection());
}

void QicsTable::initDataModels(QicsDataModel *dm)
{
    // separate data models for headers
    if (dm) {
        if (myRowHeaderDMFoundryWK) {
            m_tableCommon->myRHDataModel =
                (*myRowHeaderDMFoundryWK)(dm->numRows(), 1, myFoundryKey);
        }
        else {
            if (m_tableCommon->myRHDataModel)
                delete m_tableCommon->myRHDataModel;
            m_tableCommon->myRHDataModel =
                (*myRowHeaderDMFoundry)(dm->numRows(), 1, this);
        }

        if (myColumnHeaderDMFoundryWK) {
            m_tableCommon->myCHDataModel =
                (*myColumnHeaderDMFoundryWK)(1, dm->numColumns(),
                myFoundryKey);
        }
        else {
            if (m_tableCommon->myCHDataModel)
                delete m_tableCommon->myCHDataModel;
            m_tableCommon->myCHDataModel =
                (*myColumnHeaderDMFoundry)(1, dm->numColumns(), this);
        }
    }
    else {
        m_tableCommon->myRHDataModel = 0;
        m_tableCommon->myCHDataModel = 0;
    }

    gridInfo().setDataModel(dm);
    rhGridInfo().setDataModel(m_tableCommon->myRHDataModel);
    chGridInfo().setDataModel(m_tableCommon->myCHDataModel);
}

void QicsTable::initGridInfoObjects()
{
    // set up the grid info for the main table grid
    gridInfo().setSelectionManager(m_selectionManager);

    connect(&gridInfo(), SIGNAL(globalSetRepaintBehaviorRequest(QicsRepaintBehavior)),
        this, SLOT(setRepaintBehavior(QicsRepaintBehavior)));

    connect(&gridInfo(), SIGNAL(globalRevertRepaintBehaviorRequest()),
        this, SLOT(revertRepaintBehavior()));

    connect(&gridInfo(), SIGNAL(cellValueChanged(int, int)),
        this, SLOT(modelToVisualValueChanged(int, int)));

    connect(&gridInfo(),SIGNAL(rowsMoved(int, const QVector<int>&)),
        this,SIGNAL(rowsMoved(int, const QVector<int>&)));

    connect(&gridInfo(),SIGNAL(columnsMoved(int, const QVector<int>&)),
        this,SIGNAL(columnsMoved(int, const QVector<int>&)));

    connect(&gridInfo(),
        SIGNAL(fillHandleEvent(QicsFillHandleEventType, QMouseEvent *, const QicsSelection *, const QicsSelection *)),
        this,
        SLOT(handleFillHandleEvent(QicsFillHandleEventType, QMouseEvent *, const QicsSelection *, const QicsSelection *)));

    // now set up grid info for the row headers
    rhGridInfo().setSelectionManager(m_selectionManager);

    connect(&rhGridInfo(), SIGNAL(globalSetRepaintBehaviorRequest(QicsRepaintBehavior)),
        this, SLOT(setRepaintBehavior(QicsRepaintBehavior)));

    connect(&rhGridInfo(), SIGNAL(globalRevertRepaintBehaviorRequest()),
        this, SLOT(revertRepaintBehavior()));

    connect(&rhGridInfo(), SIGNAL(uneditRequest()),
        &gridInfo(), SIGNAL(uneditRequest()));

    // finally, set up grid info for the column headers
    chGridInfo().setSelectionManager(m_selectionManager);

    connect(&chGridInfo(), SIGNAL(globalSetRepaintBehaviorRequest(QicsRepaintBehavior)),
        this, SLOT(setRepaintBehavior(QicsRepaintBehavior)));

    connect(&chGridInfo(), SIGNAL(globalRevertRepaintBehaviorRequest()),
        this, SLOT(revertRepaintBehavior()));

    connect(&chGridInfo(), SIGNAL(uneditRequest()),
        &gridInfo(), SIGNAL(uneditRequest()));

    gridInfo().setKeyboardManager(&myKeyboardManager);
    chGridInfo().setKeyboardManager(&myKeyboardManager);
    rhGridInfo().setKeyboardManager(&myKeyboardManager);
}

void QicsTable::initDisplay()
{
    int r;

    for (r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            myGrids[r][c] = 0;

    for(r = 0; r < 3; ++r)
        for (int c = 0; c < 2; ++c) {
            // the r and C here is misleading as the dimensions are different.
            // the first [] is rows, the second [] is cols (as always)
            myHHeaders[c][r] = 0;
            myVHeaders[r][c] = 0;
        }

    m_gridLayout = new QicsGridLayout(this);
    m_gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
    m_gridLayout->setSpacing(m_tableCommon->gridSpacing());
    m_gridLayout->setMargin(m_tableCommon->tableMargin());
    m_gridLayout->setRowStretch(GRID_LAYOUT_MAIN_GRID_IDX, 1);
    m_gridLayout->setColumnStretch(GRID_LAYOUT_MAIN_GRID_IDX, 1);

    // Scrollers
    m_rowScroller = new QicsScrollBarScroller(Qics::RowIndex,this);
    m_columnScroller = new QicsScrollBarScroller(Qics::ColumnIndex,this);

    m_scrollManager->connectScroller(m_rowScroller);
    m_scrollManager->connectScroller(m_columnScroller);

    // create the main grid, this one never goes away..
    MAIN_GRID = createGrid(GRID_LAYOUT_MAIN_GRID_IDX,
        GRID_LAYOUT_MAIN_GRID_IDX);

    myGridGeometry.setMainTableGrid(MAIN_GRID);

    MAIN_GRID->setVisibleRows(10);
    MAIN_GRID->setVisibleColumns(10);
    MAIN_GRID->setIsMainGrid(true);

    m_scrollManager->setPrimaryGrid(MAIN_GRID);
    m_scrollManager->connectGrid(MAIN_GRID, true, true);

    myCurrentCell = new QicsCell(gridInfo().currentCell().row(),
        gridInfo().currentCell().column(),
        &gridInfo());

    connect(&gridInfo(), SIGNAL(currentCellChanged(int, int)),
        this, SLOT(handleCurrentCellChange(int, int)));

    m_gridLayout->addWidget(m_rowScroller->widget(),
        GRID_LAYOUT_MAIN_GRID_IDX,
        GRID_LAYOUT_RIGHT_SCROLLBAR_IDX);
    m_gridLayout->addWidget(m_columnScroller->widget(),
        GRID_LAYOUT_BOTTOM_SCROLLBAR_IDX,
        GRID_LAYOUT_MAIN_GRID_IDX);

    m_navButton = new QicsNavigator(this);
    m_navButton->setFixedSize(18,18);
    m_navButton->resizeWindow(100,100);
    m_navButton->hide();
    connect(m_navButton, SIGNAL(pressed()), this, SLOT(showNavigator()));
    connect(m_navButton, SIGNAL(changed(const QRect&)), this, SLOT(navigate(const QRect&)));
    m_gridLayout->addWidget(m_navButton, GRID_LAYOUT_BOTTOM_SCROLLBAR_IDX, GRID_LAYOUT_RIGHT_SCROLLBAR_IDX);

    m_rowScroller->widget()->installEventFilter(this);
    m_columnScroller->widget()->installEventFilter(this);
}

void QicsTable::setDataModel(QicsDataModel *dm)
{
    if(dataModel() == 0 && dm != 0 && !viewport().isValid())
        setViewport(QicsRegion(0,0,QicsLAST_ROW,QicsLAST_COLUMN));

    initDataModels(dm);
    m_selectionManager->setDataModel(dm);

    // we need to keep the three data models in sync so that when
    // rows and columns are inserted or deleted in the main data model,
    // the header data models do the same.
    gridInfo().setSelectionManager(m_selectionManager);
    rhGridInfo().setSelectionManager(m_selectionManager);
    chGridInfo().setSelectionManager(m_selectionManager);

    gridInfo().controlRowsOf(&rhGridInfo());
    gridInfo().controlColumnsOf(&chGridInfo());

    repaint();
}

QicsTableGrid *QicsTable::createGrid(int grid_row, int grid_col)
{
    QicsTableGrid *grid;

    if (myTableGridFoundryWK)
        grid = (*myTableGridFoundryWK)(this, gridInfo(), 0, 0, myFoundryKey);
    else
        grid = (*myTableGridFoundry)(this, gridInfo(), 0, 0);

    //grid->setCommonInfo(m_tableCommon);

    gridInfo().connectGrid(grid);

    connect(grid, SIGNAL(pressed(int, int, int, const QPoint &)),
        this, SLOT(handleGridPress(int, int, int, const QPoint &)));

    connect(grid, SIGNAL(clicked(int, int, int, const QPoint &)),
        this, SLOT(handleGridClick(int, int, int, const QPoint &)));

    connect(grid, SIGNAL(doubleClicked(int, int, int, const QPoint &)),
        this, SLOT(handleGridDoubleClick(int, int, int, const QPoint &)));

    connect(grid, SIGNAL(scrollRequest(Qics::QicsScrollDirection, int)),
        this, SLOT(scroll(Qics::QicsScrollDirection, int)));

    connect(grid, SIGNAL(cellEditStarted(int, int)),
        this, SIGNAL(cellEditStarted(int, int)));

    connect(grid, SIGNAL(cellEditEnded(int, int)),
        this, SIGNAL(cellEditEnded(int, int)));

    connect(grid, SIGNAL(focusNextPrevGridRequest(const QicsScreenGrid*,bool)),
        this, SLOT(focusNextPrevGrid(const QicsScreenGrid*,bool)));

    connect(grid, SIGNAL(wideKeyPressed(QKeyEvent *)), this, SLOT(handleWideKeyPressed(QKeyEvent *)));

    m_gridLayout->addWidget(grid, grid_row, grid_col);
    //grid->show();

    return grid;
}

QicsHeaderGrid *QicsTable::createHeader(Qics::QicsHeaderType type, int grid_row, int grid_col,
                        int alignment)
{
    QicsGridInfo &info = (type == RowHeader ? rhGridInfo() : chGridInfo());

    QicsHeaderGrid *hdr;

    if (myHeaderGridFoundryWK)
        hdr = (*myHeaderGridFoundryWK)(this, info, type, myFoundryKey);
    else
        hdr = (*myHeaderGridFoundry)(this, info, type);

    //hdr->setCommonInfo(m_tableCommon);

    if((grid_row == GRID_LAYOUT_LEFT_HDR_IDX && grid_col == GRID_LAYOUT_MAIN_GRID_IDX) ||
        (grid_row == GRID_LAYOUT_MAIN_GRID_IDX && grid_col == GRID_LAYOUT_LEFT_HDR_IDX))
        hdr->setIsFrozenHeader(false);
    else
        hdr->setIsFrozenHeader(true);

    info.connectGrid(hdr);

    connect(hdr,SIGNAL(headerResized(int, int, Qics::QicsHeaderType)),
        this, SLOT(resizeHeader(int, int, Qics::QicsHeaderType)));

    connect(hdr, SIGNAL(gripDoubleClicked(int, int, Qics::QicsHeaderType)),
        this, SLOT(handleHeaderDoubleClick(int, int, Qics::QicsHeaderType)));

    connect(hdr, SIGNAL(scrollRequest(Qics::QicsScrollDirection,int)),
        this, SLOT(scroll(Qics::QicsScrollDirection,int)));

    connect(hdr, SIGNAL(wideKeyPressed(QKeyEvent *)), this, SLOT(handleWideKeyPressed(QKeyEvent *)));

    connect(MAIN_GRID, SIGNAL(frameStyleUpdated()), hdr, SLOT(handleFrameStyleUpdated()));

    m_gridLayout->addWidget(hdr, grid_row, grid_col, static_cast<Qt::Alignment>(alignment));
    //hdr->show();

    return hdr;
}

void QicsTable::configureFrozen(Qics::QicsIndexType indexType, int startIndex, int endIndex)
{
    switch (indexType)
    {
    case RowIndex:
        if (m_topFrozenRows > 0 && endIndex <= currentViewport().startRow()+m_topFrozenRows)
            configureFrozen();
        if (m_bottomFrozenRows > 0 && startIndex >= currentViewport().endRow()-m_bottomFrozenRows)
            configureFrozen();
        break;
    case ColumnIndex:
        if (m_leftFrozenColumns > 0 && endIndex <= currentViewport().startColumn()+m_leftFrozenColumns)
            configureFrozen();
        if (m_rightFrozenColumns > 0 && startIndex >= currentViewport().endColumn()-m_rightFrozenColumns)
            configureFrozen();
        break;
    default:
        break;
    }
}

void QicsTable::checkFrozenRegCorner(int FrozenColumns,int FrozenRows,QicsTableGrid* &GRID,
                                     int GRID_IDX1,int GRID_IDX2,QicsGridGeometry::TableRegion region)
{

    if ((FrozenColumns > 0) && (FrozenRows > 0)) {
        if(!GRID) {
            GRID = createGrid(GRID_IDX1,GRID_IDX2);
            myGridGeometry.setTableGrid(region, GRID);
        }
    }
    else {
        if (GRID) {
            GRID->deleteLater();
            GRID = 0;
        }
    }
}

void QicsTable::checkFrozenRegMiddle_T(int FrozenRows,QicsHeaderGrid* &HEADER,bool HeaderVisible,
                                       Qics::QicsHeaderType hederType,
                                       int IDX1,int IDX2,int Align,
                                       QicsGridGeometry::HeaderPosition position)
{
    if (FrozenRows > 0) {
        if (!HEADER && HeaderVisible) {
            HEADER = createHeader(hederType,IDX1,IDX2,Align);
            myGridGeometry.setHeaderGrid(position, 1, HEADER);
        }
        else if (HEADER && !HeaderVisible) {
            delete HEADER;
            HEADER = 0;
        }
    }
    else {
        if (HEADER) {
            HEADER->deleteLater();
            HEADER = 0;
        }
    }
}

void QicsTable::checkFrozenRegMiddle_M(int FrozenRows,QicsTableGrid* &GRID,
                                       int IDX1,int IDX2,
                                       QicsGridGeometry::TableRegion region,bool control)
{
    if (FrozenRows > 0) {
        if (!GRID) {
            GRID = createGrid(IDX1,IDX2);
            m_scrollManager->connectGrid(GRID, control, !control);
            myGridGeometry.setTableGrid(region, GRID);
        }
    }
    else {
        if (GRID) {
            GRID->deleteLater();
            GRID = 0;
        }
    }
}

void QicsTable::configResizeConnects(QicsHeaderGrid* &HEADER,QicsTableGrid* &GRID1,
                                     QicsTableGrid* &GRID2,QicsTableGrid* &GRID3)
{
    if(HEADER) {
        if(GRID1)
            connectGrid(GRID1,HEADER);
        if(GRID2)
            connectGrid(GRID2,HEADER);
        if(GRID3)
            connectGrid(GRID3,HEADER);
    }
}

void QicsTable::confConn (bool Visible,QicsHeaderGrid* &HEADER,Qics::QicsHeaderType hederType,
                          int IDX1,int IDX2,int Align,
                          QicsGridGeometry::HeaderPosition position,bool control)
{
    if (Visible) {
        if (!HEADER) {
            HEADER = createHeader(hederType,IDX1,IDX2,Align);
            myGridGeometry.setHeaderGrid(position, 2, HEADER);

            m_scrollManager->connectGrid(HEADER, control,!control);
        }
    }
    else {
        if (HEADER) {
            HEADER->deleteLater();
            HEADER = 0;
        }
    }
}
void QicsTable::constrainFrozen (int FrozenRows,int IDX,bool vr, bool vr2)
{
    QicsRegion vp, cvp, tvp;

    if (FrozenRows > 0) {
        for(int i = 0; i < 3; ++i) {
            QicsTableGrid *grid;
            if (vr2)
                grid=myGrids[IDX][i];
            else
                grid=myGrids[i][IDX];

            if (grid) {
                if (i == GRID_MIDDLE_IDX)
                    vp = MAIN_GRID->viewport();
                else
                    vp = grid->viewport();

                tvp = viewport();

                if (vr && vr2) {
                    int inc = columnHeaderUsesModel();
                    vp.setStartRow(tvp.startRow() + inc);
                    vp.setEndRow(tvp.startRow() + FrozenRows - 1 + inc);
                }
                if (!vr && vr2) {
                    cvp = grid->currentViewport();

                    vp.setStartRow(cvp.endRow() - FrozenRows + 1);
                    vp.setEndRow(tvp.endRow());
                }
                if (vr && !vr2) {
                    int inc = rowHeaderUsesModel();
                    vp.setStartColumn(tvp.startColumn() + inc);
                    vp.setEndColumn(tvp.startColumn() + FrozenRows - 1 + inc);
                }
                if (!vr && !vr2) {
                    cvp = grid->currentViewport();

                    vp.setStartColumn(cvp.endColumn() - FrozenRows + 1);
                    vp.setEndColumn(tvp.endColumn());
                }

                grid->setViewport(vp);

                if (vr2)
                    grid->fixHeightToViewport(true);
                else
                    grid->fixWidthToViewport(true);

                grid->setLeftColumn(vp.startColumn());
                grid->setTopRow(vp.startRow());
            }
        }
    }
}

void QicsTable::constrainFrozen_H (int FrozenRows,int IDX,bool vr,bool vr2)
{
    QicsRegion vp, cvp, tvp;

    if (FrozenRows > 0) {
        for (int i = 0; i < 2; ++i) {
            QicsHeaderGrid *hdr;
            if (vr2)
                hdr = myVHeaders[IDX][i];
            else
                hdr = myHHeaders[i][IDX];

            if (hdr) {
                vp = hdr->viewport();
                tvp = viewport();

                if (vr && vr2) {    // top
                    int inc = columnHeaderUsesModel();
                    vp.setStartRow(tvp.startRow() + inc);
                    vp.setEndRow(tvp.startRow() + FrozenRows - 1 + inc);
                }
                if (!vr && vr2)	{   // bottom
                    cvp = MAIN_GRID->currentViewport();

                    vp.setStartRow(cvp.endRow() + 1);
                    vp.setEndRow(tvp.endRow());
                }
                if (vr && !vr2)	{   // left
                    int inc = rowHeaderUsesModel();
                    vp.setStartColumn(tvp.startColumn() + inc);
                    vp.setEndColumn(tvp.startColumn() + FrozenRows - 1 + inc);
                }
                if (!vr && !vr2) {  // right
                    cvp = MAIN_GRID->currentViewport();

                    vp.setStartColumn(cvp.endColumn() + 1);
                    vp.setEndColumn(tvp.endColumn());
                }

                hdr->setViewport(vp);

                if (vr2) {
                    hdr->fixHeightToViewport(true);
                    hdr->setTopRow(vp.startRow());
                }
                else {
                    hdr->fixWidthToViewport(true);
                    hdr->setLeftColumn(vp.startColumn());
                }
            }
        }
    }
}

void QicsTable::configureFrozen()
{
    // Go through the regions listed below. The center 0,0 doesn't
    // need to be checked, as it's always on. Various flags determine
    // what can be shown in any other region.

    //  frozen L             frozen R
    // ------------------------------
    // | Reg1  |   reg2    |  reg3  |  frozen Top
    // ------------------------------
    // | reg5  |           |  reg6  |
    // ------------------------------
    // | reg7  |  reg8     |  reg9  |  Frozen Bottom
    // ------------------------------

    QicsRepaintBehavior rb = repaintBehavior();
    setRepaintBehavior(RepaintOff);

    //check reg1
    checkFrozenRegCorner(m_leftFrozenColumns,m_topFrozenRows,TOP_LEFT_GRID,
        GRID_LAYOUT_TOP_FROZEN_GRID_IDX,GRID_LAYOUT_LEFT_FROZEN_GRID_IDX,
        QicsGridGeometry::TopLeft);

    // check reg 2
    checkFrozenRegMiddle_T(m_topFrozenRows,LEFT_TOP_HEADER,leftHeaderVisible(),
        RowHeader,
        GRID_LAYOUT_TOP_FROZEN_GRID_IDX,
        GRID_LAYOUT_LEFT_HDR_IDX,
        Qt::AlignRight,
        QicsGridGeometry::Left);

    checkFrozenRegMiddle_T(m_topFrozenRows,RIGHT_TOP_HEADER,rightHeaderVisible(),
        RowHeader,
        GRID_LAYOUT_TOP_FROZEN_GRID_IDX,
        GRID_LAYOUT_RIGHT_HDR_IDX,
        Qt::AlignLeft,
        QicsGridGeometry::Right);

    checkFrozenRegMiddle_M(m_topFrozenRows,TOP_MIDDLE_GRID,
        GRID_LAYOUT_TOP_FROZEN_GRID_IDX,
        GRID_LAYOUT_MAIN_GRID_IDX,
        QicsGridGeometry::TopMiddle,false);

    checkFrozenRegCorner(m_topFrozenRows,m_rightFrozenColumns,
        TOP_RIGHT_GRID,
        GRID_LAYOUT_TOP_FROZEN_GRID_IDX,
        GRID_LAYOUT_RIGHT_FROZEN_GRID_IDX,
        QicsGridGeometry::TopRight);

    // check Reg5
    checkFrozenRegMiddle_T(m_leftFrozenColumns,TOP_LEFT_HEADER,topHeaderVisible(),
        ColumnHeader,
        GRID_LAYOUT_TOP_HDR_IDX,
        GRID_LAYOUT_LEFT_FROZEN_GRID_IDX,
        Qt::AlignBottom,
        QicsGridGeometry::Top);

    checkFrozenRegMiddle_T(m_leftFrozenColumns,BOTTOM_LEFT_HEADER,bottomHeaderVisible(),
        ColumnHeader,
        GRID_LAYOUT_BOTTOM_HDR_IDX,
        GRID_LAYOUT_LEFT_FROZEN_GRID_IDX,
        Qt::AlignTop,
        QicsGridGeometry::Bottom);

    checkFrozenRegMiddle_M(m_leftFrozenColumns,MIDDLE_LEFT_GRID,
        GRID_LAYOUT_MAIN_GRID_IDX,
        GRID_LAYOUT_LEFT_FROZEN_GRID_IDX,
        QicsGridGeometry::MiddleLeft,true);

    checkFrozenRegMiddle_T(m_rightFrozenColumns ,TOP_RIGHT_HEADER,topHeaderVisible(),
        ColumnHeader,
        GRID_LAYOUT_TOP_HDR_IDX,
        GRID_LAYOUT_RIGHT_FROZEN_GRID_IDX,
        Qt::AlignBottom,
        QicsGridGeometry::Top);

    checkFrozenRegMiddle_T(m_rightFrozenColumns ,BOTTOM_RIGHT_HEADER,bottomHeaderVisible(),
        ColumnHeader,
        GRID_LAYOUT_BOTTOM_HDR_IDX,
        GRID_LAYOUT_RIGHT_FROZEN_GRID_IDX,
        Qt::AlignTop,
        QicsGridGeometry::Bottom);

    checkFrozenRegMiddle_M(m_rightFrozenColumns ,MIDDLE_RIGHT_GRID,
        GRID_LAYOUT_MAIN_GRID_IDX,
        GRID_LAYOUT_RIGHT_FROZEN_GRID_IDX,
        QicsGridGeometry::MiddleRight,true);

    checkFrozenRegCorner(m_leftFrozenColumns,m_bottomFrozenRows,
        BOTTOM_LEFT_GRID,
        GRID_LAYOUT_BOTTOM_FROZEN_GRID_IDX,
        GRID_LAYOUT_LEFT_FROZEN_GRID_IDX,
        QicsGridGeometry::BottomLeft);

    // check reg8
    checkFrozenRegMiddle_T(m_bottomFrozenRows ,LEFT_BOTTOM_HEADER,leftHeaderVisible(),
        RowHeader,
        GRID_LAYOUT_BOTTOM_FROZEN_GRID_IDX,
        GRID_LAYOUT_LEFT_HDR_IDX,
        Qt::AlignRight,
        QicsGridGeometry::Left);

    checkFrozenRegMiddle_T(m_bottomFrozenRows ,RIGHT_BOTTOM_HEADER,leftHeaderVisible(),
        RowHeader,
        GRID_LAYOUT_BOTTOM_FROZEN_GRID_IDX,
        GRID_LAYOUT_LEFT_HDR_IDX,
        Qt::AlignRight,
        QicsGridGeometry::Left);

    checkFrozenRegMiddle_M(m_bottomFrozenRows ,BOTTOM_MIDDLE_GRID,
        GRID_LAYOUT_BOTTOM_FROZEN_GRID_IDX,
        GRID_LAYOUT_MAIN_GRID_IDX,
        QicsGridGeometry::BottomMiddle,false);

    checkFrozenRegCorner(m_rightFrozenColumns,m_bottomFrozenRows,
        BOTTOM_RIGHT_GRID,
        GRID_LAYOUT_BOTTOM_FROZEN_GRID_IDX,
        GRID_LAYOUT_RIGHT_FROZEN_GRID_IDX,
        QicsGridGeometry::BottomRight);

    confConn (topHeaderVisible(),TOP_HEADER,ColumnHeader,
        GRID_LAYOUT_TOP_HDR_IDX,
        GRID_LAYOUT_MAIN_GRID_IDX,
        Qt::AlignBottom,
        QicsGridGeometry::Top,false);

    confConn (bottomHeaderVisible(),BOTTOM_HEADER,ColumnHeader,
        GRID_LAYOUT_BOTTOM_HDR_IDX,
        GRID_LAYOUT_MAIN_GRID_IDX,
        Qt::AlignTop,
        QicsGridGeometry::Bottom,false);

    confConn (leftHeaderVisible(),LEFT_HEADER,RowHeader,
        GRID_LAYOUT_MAIN_GRID_IDX,
        GRID_LAYOUT_LEFT_HDR_IDX,
        Qt::AlignRight,
        QicsGridGeometry::Left,true);

    confConn (rightHeaderVisible(),RIGHT_HEADER,RowHeader,
        GRID_LAYOUT_MAIN_GRID_IDX,
        GRID_LAYOUT_RIGHT_HDR_IDX,
        Qt::AlignLeft,
        QicsGridGeometry::Right,true);

    configResizeConnects(LEFT_TOP_HEADER,TOP_LEFT_GRID,
        TOP_MIDDLE_GRID,TOP_RIGHT_GRID);

    configResizeConnects(RIGHT_TOP_HEADER,TOP_LEFT_GRID,
        TOP_MIDDLE_GRID,TOP_RIGHT_GRID);

    configResizeConnects(LEFT_HEADER,MIDDLE_LEFT_GRID,
        MAIN_GRID,MIDDLE_RIGHT_GRID);

    configResizeConnects(RIGHT_HEADER,MIDDLE_LEFT_GRID,
        MAIN_GRID,MIDDLE_RIGHT_GRID);

    configResizeConnects(LEFT_BOTTOM_HEADER,BOTTOM_LEFT_GRID,
        BOTTOM_MIDDLE_GRID,BOTTOM_RIGHT_GRID);

    configResizeConnects(RIGHT_BOTTOM_HEADER,BOTTOM_LEFT_GRID,
        BOTTOM_MIDDLE_GRID,BOTTOM_RIGHT_GRID);

    configResizeConnects(TOP_LEFT_HEADER,TOP_LEFT_GRID,
        MIDDLE_LEFT_GRID,BOTTOM_LEFT_GRID);

    configResizeConnects(BOTTOM_LEFT_HEADER,TOP_LEFT_GRID,
        MIDDLE_LEFT_GRID,BOTTOM_LEFT_GRID);

    configResizeConnects(TOP_HEADER,TOP_MIDDLE_GRID,
        MAIN_GRID,BOTTOM_MIDDLE_GRID);

    configResizeConnects(BOTTOM_HEADER,TOP_MIDDLE_GRID,
        MIDDLE_LEFT_GRID,BOTTOM_MIDDLE_GRID);

    configResizeConnects(TOP_RIGHT_HEADER,TOP_RIGHT_GRID,
        MIDDLE_RIGHT_GRID,BOTTOM_RIGHT_GRID);


    configResizeConnects(BOTTOM_RIGHT_HEADER,TOP_RIGHT_GRID,
        MIDDLE_RIGHT_GRID,BOTTOM_RIGHT_GRID);

        if (m_topFrozenRows) {
                m_gridLayout->setRowMinimumHeight(GRID_LAYOUT_TOP_FROZEN_LINE_IDX, frozenLineWidth() + 2);
        } else {
                m_gridLayout->setRowMinimumHeight(GRID_LAYOUT_TOP_FROZEN_LINE_IDX, 0);
        }

        if (m_bottomFrozenRows) {
                m_gridLayout->setRowMinimumHeight(GRID_LAYOUT_BOTTOM_FROZEN_LINE_IDX, frozenLineWidth() + 2);
        } else {
                m_gridLayout->setRowMinimumHeight(GRID_LAYOUT_BOTTOM_FROZEN_LINE_IDX, 0);
        }

        if (m_leftFrozenColumns) {
                m_gridLayout->setColumnMinimumWidth(GRID_LAYOUT_LEFT_FROZEN_LINE_IDX, frozenLineWidth() + 2);
        } else {
                m_gridLayout->setColumnMinimumWidth(GRID_LAYOUT_LEFT_FROZEN_LINE_IDX, 0);
        }

        if (m_rightFrozenColumns) {
                m_gridLayout->setColumnMinimumWidth(GRID_LAYOUT_RIGHT_FROZEN_LINE_IDX, frozenLineWidth() + 2);
        } else {
                m_gridLayout->setColumnMinimumWidth(GRID_LAYOUT_RIGHT_FROZEN_LINE_IDX, 0);
        }

    //
    // Configure viewports
    //
    QicsRegion vp;

    bool isFrozen = (m_topFrozenRows || m_bottomFrozenRows || m_leftFrozenColumns || m_rightFrozenColumns);

    // Begin by setting all of the frozen grids' viewports to the
    // full viewport of the table

    if (isFrozen) {
        for(int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if ((i != GRID_MIDDLE_IDX) || (j != GRID_MIDDLE_IDX)) {
                    QicsGrid *g = myGrids[i][j];

                    if (g)
                        g->setViewport(viewport());
                }
            }
        }

        vp = MAIN_GRID->viewport();
        vp.setStartRow(m_topFrozenRows + columnHeaderUsesModel());
        vp.setEndRow(currentViewport().numRows() - m_bottomFrozenRows -1 );
        vp.setStartColumn(m_leftFrozenColumns + rowHeaderUsesModel());
        vp.setEndColumn(currentViewport().numColumns() - m_rightFrozenColumns -1 );
        MAIN_GRID->setViewport(vp);
        //MAIN_GRID->setTopRow(vp.startRow());
        //MAIN_GRID->setLeftColumn(vp.startColumn());

        setTopRow(vp.startRow());
        setLeftColumn(vp.startColumn());

        // Constrain the frozen grids' viewports based on the
        // number of frozen rows and columns

        constrainFrozen (m_topFrozenRows,GRID_TOP_IDX,true,true);
        constrainFrozen (m_bottomFrozenRows,GRID_BOTTOM_IDX,false,true);
        constrainFrozen (m_leftFrozenColumns,GRID_LEFT_IDX,true,false);
        constrainFrozen (m_rightFrozenColumns,GRID_RIGHT_IDX,false,false);

        // Configure headers

        // Begin by setting all of the frozen headers' viewports to the
        // full viewport of the appropriate header

        QicsRegion row_hdr_vp = rowHeaderRef().viewport();
        QicsRegion col_hdr_vp = columnHeaderRef().viewport();
        QicsHeaderGrid *hdr = 0;

        for (int i = 0; i < 2; ++i) {
            hdr = myVHeaders[TOP_VHEADER_IDX][i];
            if (hdr) hdr->setViewport(row_hdr_vp);

            hdr = myVHeaders[BOTTOM_VHEADER_IDX][i];
            if (hdr) hdr->setViewport(row_hdr_vp);
        }

        for (int i = 0; i < 2; ++i) {
            hdr = myHHeaders[i][LEFT_HHEADER_IDX];
            if (hdr) hdr->setViewport(col_hdr_vp);

            hdr = myHHeaders[i][RIGHT_HHEADER_IDX];
            if (hdr) hdr->setViewport(col_hdr_vp);
        }

        row_hdr_vp.setStartRow(m_topFrozenRows + columnHeaderUsesModel());
        row_hdr_vp.setEndRow(currentViewport().numRows() - m_bottomFrozenRows - 1 );
        col_hdr_vp.setStartColumn(m_leftFrozenColumns + rowHeaderUsesModel());
        col_hdr_vp.setEndColumn(currentViewport().numColumns() - m_leftFrozenColumns -1 );
        rowHeaderRef().setViewport(row_hdr_vp);
        columnHeaderRef().setViewport(col_hdr_vp);
        if (LEFT_HEADER) LEFT_HEADER->setTopRow(row_hdr_vp.startRow());
        if (RIGHT_HEADER) RIGHT_HEADER->setTopRow(row_hdr_vp.startRow());
        if (TOP_HEADER) TOP_HEADER->setLeftColumn(col_hdr_vp.startColumn());
        if (BOTTOM_HEADER) BOTTOM_HEADER->setLeftColumn(col_hdr_vp.startColumn());

        constrainFrozen_H (m_topFrozenRows,TOP_VHEADER_IDX,true,true);
        constrainFrozen_H (m_bottomFrozenRows,BOTTOM_VHEADER_IDX,false,true);
        constrainFrozen_H (m_leftFrozenColumns,LEFT_HHEADER_IDX,true,false);
        constrainFrozen_H (m_rightFrozenColumns,RIGHT_HHEADER_IDX,false,false);
    }
    else {	// no frozens here
        int inc_r = columnHeaderUsesModel();
        int inc_c = rowHeaderUsesModel();

        vp = MAIN_GRID->viewport();
        vp.setStartRow(inc_r);
        vp.setStartColumn(inc_c);
        MAIN_GRID->setViewport(vp);

        QicsRegion row_hdr_vp = rowHeaderRef().viewport();
        row_hdr_vp.setStartRow(inc_r);
        rowHeaderRef().setViewport(row_hdr_vp);
        setTopRow(inc_r);

        QicsRegion col_hdr_vp = columnHeaderRef().viewport();
        col_hdr_vp.setStartColumn(inc_c);
        columnHeaderRef().setViewport(col_hdr_vp);
        setLeftColumn(inc_c);
    }

    // This line no need any more
    //myGridLayout->activate();

    if(!myUnfreezingFlag)
        checkLayoutOverlap();       // checking for a layout deformation

    setRepaintBehavior(rb);

    //update frame style
    setFrameLineWidth(frameLineWidth());
    setFrameStyle(frameStyle());

    update();
}

void QicsTable::checkLayoutOverlap()
{
    for(int i = 0; i < 8; ++i) {
        QWidget* widget = m_gridLayout->widgetAt(i, 4);
        if(widget && widget->isVisible()) {
            for (int j = i + 1; j < 9; ++j) {
                QWidget *nextWidget = m_gridLayout->widgetAt(j, 4);
                if (nextWidget && nextWidget->isVisibleTo(this)) {  // check intersect widget only with next visible nextWidget
                    QRect widget_rect = widget->geometry();
                    QRect nextWidget_rect = nextWidget->geometry();
                    if (widget_rect.intersects(nextWidget_rect) || !this->rect().contains(widget_rect, true)
                        || !this->rect().contains(nextWidget_rect, true)) {
                        emit layoutFailed(Qt::Vertical);
                        break;
                    }
                    break;
                }
            }
        }
    }

    for(int i = 0; i < 8; ++i) {
        QWidget* widget = m_gridLayout->widgetAt(4, i);
        if(widget && widget->isVisible()) {
            for(int j = i + 1; j < 9; ++j) {
                QWidget *nextWidget = m_gridLayout->widgetAt(4, j);
                if (nextWidget && nextWidget->isVisibleTo(this)) {   // check intersect widget only with next visible nextWidget
                    QRect widget_rect = widget->geometry();
                    QRect nextWidget_rect = nextWidget->geometry();
                    if (widget_rect.intersects(nextWidget_rect) || !this->rect().contains(widget_rect, true)
                        || !this->rect().contains(nextWidget_rect, true)) {
                        emit layoutFailed(Qt::Horizontal);
                        break;
                    }
                    break;
                }
            }
        }
    }
}

void QicsTable::connectGrid(QicsTableGrid *grid,QicsHeaderGrid *header)
{
    disconnect(header, SIGNAL(resizeInProgress(int, int, Qics::QicsHeaderType)),
        grid, SLOT(drawHeaderResizeBar(int, int, Qics::QicsHeaderType)));
    disconnect(header, SIGNAL(hideResizeBarRequest()),
        grid, SLOT(hideResizeBar()));

    connect(header, SIGNAL(resizeInProgress(int, int, Qics::QicsHeaderType)),
        grid, SLOT(drawHeaderResizeBar(int, int, Qics::QicsHeaderType)));
    connect(header, SIGNAL(hideResizeBarRequest()),
        grid, SLOT(hideResizeBar()));
}

void QicsTable::setTopRow(int row)
{
    // Sets top row if row index larger than first visibleRows
    // and less than last visibleRows
    int possibleTopRow;
    const int bRow = currentViewport().bottom();
    const int rows = bottomRow() - topRow();

    if (bRow == MAIN_GRID->fullyVisibleBottomRow())
        possibleTopRow = bRow - rows;
    else
        possibleTopRow = bRow - rows + 1;

    if (row > possibleTopRow)
        row = possibleTopRow;

    if(row < m_topFrozenRows)
        m_scrollManager->setIndex(Qics::RowIndex, myGrids[1][1]->gridInfo().firstNonHiddenRow(m_topFrozenRows, topRow()));
    else
        m_scrollManager->setIndex(Qics::RowIndex, row);
}

void QicsTable::setLeftColumn(int col)
{
    int possibleRightColumn;
    const int rColumn = currentViewport().right();
    const int cols = rightColumn() - leftColumn();

    if (rColumn == MAIN_GRID->fullyVisibleRightColumn())
        possibleRightColumn = rColumn - cols;
    else
        possibleRightColumn = rColumn - cols + 1;

    if (col > possibleRightColumn)
        col = possibleRightColumn;

    if(col < m_leftFrozenColumns)
        m_scrollManager->setIndex(Qics::ColumnIndex, myGrids[1][1]->gridInfo().firstNonHiddenColumn(m_leftFrozenColumns,leftColumn()));
    else
        m_scrollManager->setIndex(Qics::ColumnIndex, col);
}

int QicsTable::visibleRows() const
{
    return MAIN_GRID->visibleRows();
}

int QicsTable::visibleColumns() const
{
    return MAIN_GRID->visibleColumns();
}

void QicsTable::setVisibleRows(int num)
{
    MAIN_GRID->setVisibleRows(num);
}

void QicsTable::setVisibleColumns(int num)
{
    MAIN_GRID->setVisibleColumns(num);
}

int QicsTable::fullyVisibleBottomRow() const
{
    return (MAIN_GRID->fullyVisibleBottomRow());
}

int QicsTable::fullyVisibleRightColumn() const
{
    return (MAIN_GRID->fullyVisibleRightColumn());
}

QAbstractSlider *QicsTable::horizontalScrollBar() const
{
    return m_columnScroller->widget();
}

QAbstractSlider *QicsTable::verticalScrollBar() const
{
    return m_rowScroller->widget();
}

void QicsTable::setTopTitleWidget(QWidget *w)
{
    if(!w)
        return;

    delete myTopTitleWidget;

    myTopTitleWidget = w;

    m_gridLayout->addWidget(w, GRID_LAYOUT_TOP_TITLE_IDX,
        GRID_LAYOUT_MAIN_GRID_IDX);
    w->setCursor(mainGridRef().cursor());
}

void QicsTable::setBottomTitleWidget(QWidget *w)
{
    if(!w)
        return;

    delete myBottomTitleWidget;

    myBottomTitleWidget = w;

    m_gridLayout->addWidget(w, GRID_LAYOUT_BOTTOM_TITLE_IDX,
        GRID_LAYOUT_MAIN_GRID_IDX);
    w->setCursor(mainGridRef().cursor());
}

void QicsTable::setLeftTitleWidget(QWidget *w)
{
    if(!w)
        return;

    delete myLeftTitleWidget;

    myLeftTitleWidget = w;

    m_gridLayout->addWidget(w, GRID_LAYOUT_MAIN_GRID_IDX,
        GRID_LAYOUT_LEFT_TITLE_IDX);
    w->setCursor(mainGridRef().cursor());
}

void QicsTable::setRightTitleWidget(QWidget *w)
{
    if(!w)
        return;

    delete myRightTitleWidget;

    myRightTitleWidget = w;

    m_gridLayout->addWidget(w, GRID_LAYOUT_MAIN_GRID_IDX,
        GRID_LAYOUT_RIGHT_TITLE_IDX);
    w->setCursor(mainGridRef().cursor());
}

void QicsTable::setTopLeftCornerWidget(QWidget *w)
{
    if(!w)
        return;

    delete myTopLeftCornerWidget;

    myTopLeftCornerWidget = w;

    m_gridLayout->addWidget(w, GRID_LAYOUT_TOP_HDR_IDX,
        GRID_LAYOUT_LEFT_HDR_IDX);
    w->setCursor(mainGridRef().cursor());
}

void QicsTable::setTopRightCornerWidget(QWidget *w)
{
    if(!w)
        return;

    delete myTopRightCornerWidget;

    myTopRightCornerWidget = w;

    m_gridLayout->addWidget(w, GRID_LAYOUT_TOP_HDR_IDX,
        GRID_LAYOUT_RIGHT_HDR_IDX);
    w->setCursor(mainGridRef().cursor());
}

void QicsTable::setBottomLeftCornerWidget(QWidget *w)
{
    if(!w)
        return;

    delete myBottomLeftCornerWidget;

    myBottomLeftCornerWidget = w;

    m_gridLayout->addWidget(w, GRID_LAYOUT_BOTTOM_HDR_IDX,
        GRID_LAYOUT_LEFT_HDR_IDX);
    w->setCursor(mainGridRef().cursor());
}

void QicsTable::setBottomRightCornerWidget(QWidget *w)
{
    if(!w)
        return;

    delete myBottomRightCornerWidget;

    myBottomRightCornerWidget = w;

    m_gridLayout->addWidget(w, GRID_LAYOUT_BOTTOM_HDR_IDX,
        GRID_LAYOUT_RIGHT_HDR_IDX);
    w->setCursor(mainGridRef().cursor());
}


// computes the intersection of the data model dimensions and the
// viewport dimensions
QicsRegion QicsTable::currentViewport() const
{
    if (dataModel()) {
        int endRow, endCol;

        QicsRegion vp = viewport();

        int lastRow = dataModel()->lastRow();
        int lastCol = dataModel()->lastColumn();

        if (vp.endRow() > lastRow)
            endRow = lastRow;
        else
            endRow = vp.endRow();

        if (vp.endColumn() > lastCol)
            endCol = lastCol;
        else
            endCol = vp.endColumn();

        return QicsRegion(vp.startRow(), vp.startColumn(),
            endRow, endCol);
    }
    else
        // return full viewport if no data model
        return viewport();
}

void QicsTable::toggleRowHeightExpansion(int row, bool setOverrideCursor)
{
    if (gridInfo().mappedDM()->overriddenRowHeight(row) >= 0 && autoFitMode() == Qics::Native) {
        gridInfo().mappedDM()->resetRowHeight(row);
        return;
    }

    if (setOverrideCursor)//#90147
        QApplication::setOverrideCursor(Qt::WaitCursor);

    QicsRegion vp = currentViewport();

    int h = 0;
    QicsCellDisplay *cd = 0;
    const QicsDataItem *itm = 0;

    QicsCell cell(row, 0, this, false);

    QicsRegion reg;
    QicsSpanManager *spanManager = columnHeaderRef().styleManager().spanManager();//#90009

    const int rowNumColumns = rowHeaderRef().numColumns();
    for (int i = 0; i < rowNumColumns; ++i) {
        if (spanManager->isSpanner(cell.gridInfo(), row, i, reg))// #90009
            continue;// #90009
        cd = rowHeaderRef().cellRef(row,i).displayer();
        itm = rowHeaderRef().cellRef(row,i).dataValue();

        h = qMax(h, cd->sizeHint(LEFT_HEADER, row, i, itm).height());
    }

    const int endColumn = vp.endColumn();
    for (int i = vp.startColumn(); i <= endColumn; ++i) {
        if (spanManager->isSpanner(cell.gridInfo(), row, i, reg))// #90009
            continue;// #90009
        cell.setColumnIndex(i);
        cd = cell.displayer();
        itm = cell.dataValue();

        h = qMax(h, cd->sizeHint(MAIN_GRID, row, i, itm).height());
    }

    h = qMax(h, gridInfo().mappedDM()->rowMinHeight(row));
    h = qMin(h, gridInfo().mappedDM()->rowMaxHeight(row));
    gridInfo().mappedDM()->overrideRowHeight(row, h);

    if (setOverrideCursor)//#90147
        QApplication::restoreOverrideCursor();
}

void QicsTable::toggleColumnWidthExpansion(int col, bool setOverrideCursor)
{
    if (gridInfo().mappedDM()->overriddenColumnWidth(col) >= 0 && autoFitMode() == Qics::Native) {
        gridInfo().mappedDM()->resetColumnWidth(col);
        return;
    }

    if (setOverrideCursor)//#90147
        QApplication::setOverrideCursor(Qt::WaitCursor);

    QicsRegion vp = currentViewport();

    int w = 0;
    QicsCellDisplay *cd = 0;
    const QicsDataItem *itm = 0;

    QicsCell cell(0, col, this, false);

    QicsRegion reg;
    QicsSpanManager *spanManager = columnHeaderRef().styleManager().spanManager();//#90009

    const int colNumRows = columnHeaderRef().numRows();
    for (int i = 0; i < colNumRows; ++i) {
        if (spanManager->isSpanner(cell.gridInfo(), i, col, reg))// #90009
            continue;// #90009
        cd = columnHeaderRef().cellRef(i,col).displayer();
        itm = columnHeaderRef().cellRef(i,col).dataValue();

        w = qMax(w, cd->sizeHint(TOP_HEADER, i, col, itm).width());
    }

    const int endRow = vp.endRow();
    for (int i = vp.startRow(); i <= endRow; ++i) {
        if (spanManager->isSpanner(cell.gridInfo(), i, col, reg))// #90009
            continue;// #90009
        cell.setRowIndex(i);
        cd = cell.displayer();
        itm = cell.dataValue();

        w = qMax(w, cd->sizeHint(MAIN_GRID, i, col, itm).width());
    }

    w = qMax(w, gridInfo().mappedDM()->columnMinWidth(col));
    w = qMin(w, gridInfo().mappedDM()->columnMaxWidth(col));
    gridInfo().mappedDM()->overrideColumnWidth(col, w);

    if (setOverrideCursor)//#90147
        QApplication::restoreOverrideCursor();
}

////////////////////////////////////////////////////////////////////
///////////       Grid Access Methods                 //////////////
////////////////////////////////////////////////////////////////////

QicsRow &QicsTable::rowRef(int rownum)
{
    return m_tableCommon->rowRef(rownum);
}

const QicsRow &QicsTable::rowRef(int rownum) const
{
    return m_tableCommon->rowRef(rownum);
}

QicsRow *QicsTable::row(int rownum, bool follow_model)
{
    return m_tableCommon->row(rownum, follow_model);
}

const QicsRow *QicsTable::row(int rownum, bool follow_model) const
{
    return m_tableCommon->row(rownum, follow_model);
}

QicsColumn &QicsTable::columnRef(int colnum)
{
    return m_tableCommon->columnRef(colnum);
}

const QicsColumn &QicsTable::columnRef(int colnum) const
{
    return m_tableCommon->columnRef(colnum);
}

QicsColumn *QicsTable::column(int colnum, bool follow_model)
{
    return m_tableCommon->column(colnum, follow_model);
}

const QicsColumn *QicsTable::column(int colnum, bool follow_model) const
{
    return m_tableCommon->column(colnum, follow_model);
}

QicsRepeatingRow &QicsTable::repeatingRowRef(int start_row, unsigned int interval)
{
    return (m_tableCommon->repeatingRowRef(start_row, interval));
}

const QicsRepeatingRow &QicsTable::repeatingRowRef(int start_row, unsigned int interval) const
{
    return (m_tableCommon->repeatingRowRef(start_row, interval));
}

QicsRepeatingRow *QicsTable::repeatingRow(int start_row, unsigned int interval)
{
    return (m_tableCommon->repeatingRow(start_row, interval));
}

const QicsRepeatingRow *QicsTable::repeatingRow(int start_row, unsigned int interval) const
{
    return (m_tableCommon->repeatingRow(start_row, interval));
}

QicsRepeatingColumn &QicsTable::repeatingColumnRef(int start_col, unsigned int interval)
{
    return (m_tableCommon->repeatingColumnRef(start_col, interval));
}

const QicsRepeatingColumn &QicsTable::repeatingColumnRef(int start_col, unsigned int interval) const
{
    return (m_tableCommon->repeatingColumnRef(start_col, interval));
}

QicsRepeatingColumn *QicsTable::repeatingColumn(int start_col, unsigned int interval)
{
    return (m_tableCommon->repeatingColumn(start_col, interval));
}

const QicsRepeatingColumn *QicsTable::repeatingColumn(int start_col, unsigned int interval) const
{
    return (m_tableCommon->repeatingColumn(start_col, interval));
}

QicsCell &QicsTable::cellRef(int rownum, int colnum)
{
    return m_tableCommon->cellRef(rownum, colnum);
}

const QicsCell &QicsTable::cellRef(int rownum, int colnum) const
{
    return m_tableCommon->cellRef(rownum, colnum);
}

QicsCell *QicsTable::cell(int rownum, int colnum, bool follow_model)
{
    return m_tableCommon->cell(rownum, colnum, follow_model);
}

const QicsCell *QicsTable::cell(int rownum, int colnum, bool follow_model) const
{
    return m_tableCommon->cell(rownum, colnum, follow_model);
}

QicsMainGrid &QicsTable::mainGridRef()
{
    return m_tableCommon->mainGridRef();
}

const QicsMainGrid &QicsTable::mainGridRef() const
{
    return m_tableCommon->mainGridRef();
}

QicsMainGrid *QicsTable::mainGrid()
{
    return m_tableCommon->mainGrid();
}

const QicsMainGrid *QicsTable::mainGrid() const
{
    return m_tableCommon->mainGrid();
}

QicsRowHeader &QicsTable::rowHeaderRef()
{
    return m_tableCommon->rowHeaderRef();
}

const QicsRowHeader &QicsTable::rowHeaderRef() const
{
    return m_tableCommon->rowHeaderRef();
}

QicsRowHeader *QicsTable::rowHeader()
{
    return m_tableCommon->rowHeader();
}

const QicsRowHeader *QicsTable::rowHeader() const
{
    return m_tableCommon->rowHeader();
}

QicsColumnHeader &QicsTable::columnHeaderRef()
{
    return m_tableCommon->columnHeaderRef();
}

const QicsColumnHeader &QicsTable::columnHeaderRef() const
{
    return m_tableCommon->columnHeaderRef();
}

QicsColumnHeader *QicsTable::columnHeader()
{
    return m_tableCommon->columnHeader();
}

const QicsColumnHeader *QicsTable::columnHeader() const
{
    return m_tableCommon->columnHeader();
}

void QicsTable::clearTable()
{
    bool state=ignoreModelSizeChanges();
    setIgnoreModelSizeChanges(false);
    dataModel()->clearModel();
    setIgnoreModelSizeChanges(state);
}

int QicsTable::lastRowWithData() const
{
    QicsGridInfo &gi = gridInfo();
    QicsDataModel *dm = dataModel();
    if (!dm) return -1; // no data

    // we'll search from the end, to be as fast as possible
    for (int i = dm->lastRow(); i >= 0; --i) {
        int row = gi.modelRowIndex(i);
        if (!dm->isRowEmpty(row)) return i;
    }
    return -1;  // no data
}

int QicsTable::lastColumnWithData() const
{
    QicsGridInfo &gi = gridInfo();
    QicsDataModel *dm = dataModel();
    if (!dm) return -1; // no data

    // we'll search from the end, to be as fast as possible
    for (int i = dm->lastColumn(); i >= 0; --i) {
        int col = gi.modelColumnIndex(i);
        if (!dm->isColumnEmpty(col)) return i;
    }
    return -1;  // no data
}

int QicsTable::firstRowWithData() const
{
    QicsGridInfo &gi = gridInfo();
    QicsDataModel *dm = dataModel();
    if (!dm) return -1; // no data

    // we'll search from the start, to be as fast as possible
    for (int i = 0; i <= dm->lastRow(); ++i) {
        int row = gi.modelRowIndex(i);
        if (!dm->isRowEmpty(row)) return i;
    }
    return -1;  // no data
}

int QicsTable::firstColumnWithData() const
{
    QicsGridInfo &gi = gridInfo();
    QicsDataModel *dm = dataModel();
    if (!dm) return -1; // no data

    // we'll search from the start, to be as fast as possible
    for (int i = 0; i <= dm->lastColumn(); ++i) {
        int col = gi.modelColumnIndex(i);
        if (!dm->isColumnEmpty(col)) return i;
    }
    return -1;  // no data
}

QicsRegion QicsTable::dataRegion() const
{
    return QicsRegion(firstRowWithData(),
        firstColumnWithData(),
        lastRowWithData(),
        lastColumnWithData());
}

QicsRegion QicsTable::selectedRegion() const
{
    QicsSelectionList *sl = selectionList(true);
    if (!sl) return QicsRegion();
    return sl->region();
}

////////////////////////////////////////////////////////////////////
///////////         Property Methods                  //////////////
////////////////////////////////////////////////////////////////////

QicsCellRegion *QicsTable::cellRegion(const QicsRegion &region)
{
    if (myCellRegion) delete myCellRegion;
    myCellRegion = new QicsCellRegionTable(region, this, m_tableCommon->gridInfo().gridType(), styleManager(), dimensionManager());
    return myCellRegion;
}

const QicsSelection *QicsTable::currentSelection()
{
    return gridInfo().selectionManager()->currentSelection();
}

int QicsTable::margin() const
{
    return m_tableCommon->margin();
}

void QicsTable::setMargin(int margin)
{
    m_tableCommon->setMargin(margin);
    layout()->invalidate();
}

QColor QicsTable::foregroundColor() const
{
    return m_tableCommon->foregroundColor();
}

void QicsTable::setForegroundColor(const QColor &color)
{
    m_tableCommon->setForegroundColor(color);
}

QColor QicsTable::backgroundColor() const
{
    return m_tableCommon->backgroundColor();
}

void QicsTable::setBackgroundColor(const QColor &color)
{
    m_tableCommon->setBackgroundColor(color);
}

QColor QicsTable::selectedForegroundColor() const
{
    return m_tableCommon->selectedForegroundColor();
}

void QicsTable::setSelectedForegroundColor(const QColor &color)
{
    m_tableCommon->setSelectedForegroundColor(color);
}

QColor QicsTable::selectedBackgroundColor() const
{
    return m_tableCommon->selectedBackgroundColor();
}

void QicsTable::setSelectedBackgroundColor(const QColor &color)
{
    m_tableCommon->setSelectedBackgroundColor(color);
}

QColor QicsTable::editForegroundColor() const
{
    return m_tableCommon->editForegroundColor();
}

void QicsTable::setEditForegroundColor(const QColor &color)
{
    m_tableCommon->setEditForegroundColor(color);
}

QColor QicsTable::editBackgroundColor() const
{
    return m_tableCommon->editBackgroundColor();
}

void QicsTable::setEditBackgroundColor(const QColor &color)
{
    m_tableCommon->setEditBackgroundColor(color);
}

bool QicsTable::readOnly() const
{
    return m_tableCommon->readOnly();
}

void QicsTable::setReadOnly(bool b)
{
    m_tableCommon->setReadOnly(b);
}

QPixmap QicsTable::pixmap() const
{
    return m_tableCommon->pixmap();
}

void QicsTable::setPixmap(const QPixmap &p)
{
    m_tableCommon->setPixmap(p);
}

void QicsTable::setPixmap(const QString &file_name)
{
    setPixmap(QPixmap(file_name));
}

void QicsTable::setDefaultImagePath(const QString &path)
{
    gridInfo().setDefaultImagePath(path);
}

int QicsTable::pixmapSpacing() const
{
    return m_tableCommon->pixmapSpacing();
}

void QicsTable::setPixmapSpacing(int sp)
{
    m_tableCommon->setPixmapSpacing(sp);
}

QicsCellDisplay *QicsTable::displayer() const
{
    return m_tableCommon->displayer();
}

void QicsTable::setDisplayer(QicsCellDisplay *dsp)
{
    m_tableCommon->setDisplayer(dsp);
}

QicsDataItemFormatter *QicsTable::formatter() const
{
    return m_tableCommon->formatter();
}

void QicsTable::setFormatter(QicsDataItemFormatter *f)
{
    m_tableCommon->setFormatter(f);
}

int QicsTable::alignment() const
{
    return m_tableCommon->alignment();
}

void QicsTable::setAlignment(int flags)
{
    m_tableCommon->setAlignment(flags);
}

int QicsTable::textFlags() const
{
    return m_tableCommon->textFlags();
}

void QicsTable::setTextFlags(int flags)
{
    m_tableCommon->setTextFlags(flags);
}

QValidator *QicsTable::validator() const
{
    return m_tableCommon->validator();
}

void QicsTable::setValidator(QValidator *v)
{
    m_tableCommon->setValidator(v);
}

QicsPasteValidator *QicsTable::pasteValidator() const
{
    return m_tableCommon->pasteValidator();
}

void QicsTable::setPasteValidator(QicsPasteValidator *v)
{
    m_tableCommon->setPasteValidator(v);
}

QString QicsTable::label() const
{
    return m_tableCommon->label();
}

void QicsTable::setLabel(const QString &label)
{
    m_tableCommon->setLabel(label);
}

int QicsTable::maxLength() const
{
    return m_tableCommon->maxLength();
}

void QicsTable::setMaxLength(int len)
{
    m_tableCommon->setMaxLength(len);
}

QPen QicsTable::topBorderPen() const
{
    return m_tableCommon->topBorderPen();
}

QPen QicsTable::leftBorderPen() const
{
    return m_tableCommon->leftBorderPen();
}

QPen QicsTable::rightBorderPen() const
{
    return m_tableCommon->rightBorderPen();
}

QPen QicsTable::bottomBorderPen() const
{
    return m_tableCommon->bottomBorderPen();
}

void QicsTable::setTopBorderPen(const QPen &pen)
{
    m_tableCommon->setTopBorderPen(pen);
}

void QicsTable::setLeftBorderPen(const QPen &pen)
{
    m_tableCommon->setLeftBorderPen(pen);
}

void QicsTable::setRightBorderPen(const QPen &pen)
{
    m_tableCommon->setRightBorderPen(pen);
}

void QicsTable::setBottomBorderPen(const QPen &pen)
{
    m_tableCommon->setBottomBorderPen(pen);
}

void QicsTable::setBorderPen(const QPen &pen)
{
    m_tableCommon->setTopBorderPen(pen);
    m_tableCommon->setLeftBorderPen(pen);
    m_tableCommon->setRightBorderPen(pen);
    m_tableCommon->setBottomBorderPen(pen);
}

///////// Grid Attribute methods

QicsRegion QicsTable::viewport() const
{
    return myFullViewport;
}

void QicsTable::setViewport(const QicsRegion &vp)
{
    myFullViewport = vp;
    setMainGridViewport(vp);
}

QicsRegion QicsTable::mainGridViewport() const
{
    return m_tableCommon->viewport();
}

void QicsTable::setMainGridViewport(const QicsRegion &vp)
{
    QicsRegion old_region;
    QicsRegion new_region;

    // Set the row header viewport
    QicsHeaderGrid *hdr = LEFT_HEADER;
    if (hdr) {
        old_region = hdr->viewport();
        new_region = QicsRegion(vp.startRow(), old_region.startColumn(),
            vp.endRow(), old_region.endColumn());
        hdr->setViewport(new_region);
    }
    hdr = RIGHT_HEADER;
    if (hdr) {
        old_region = hdr->viewport();
        new_region = QicsRegion(vp.startRow(), old_region.startColumn(),
            vp.endRow(), old_region.endColumn());
        hdr->setViewport(new_region);
    }

    // Set the column header viewport
    hdr = TOP_HEADER;
    if (hdr) {
        old_region = hdr->viewport();
        new_region = QicsRegion(old_region.startRow(), vp.startColumn(),
            old_region.endRow(), vp.endColumn());
        hdr->setViewport(new_region);
    }
    hdr = BOTTOM_HEADER;
    if (hdr) {
        old_region = hdr->viewport();
        new_region = QicsRegion(old_region.startRow(), vp.startColumn(),
            old_region.endRow(), vp.endColumn());
        hdr->setViewport(new_region);
    }

    // Set the main viewport
    m_tableCommon->setViewport(vp);
    MAIN_GRID->setViewport(vp);
}

bool QicsTable::addCellSpan(QicsSpan span, bool visual)
{
    return m_tableCommon->addCellSpan(span, visual);
}

void QicsTable::removeCellSpan(int start_row, int start_col, bool visual)
{
    m_tableCommon->removeCellSpan(start_row, start_col, visual);
}

QicsSpanList *QicsTable::cellSpanList()
{
    return m_tableCommon->cellSpanList();
}

Qics::QicsGridCellClipping QicsTable::gridCellClipping() const
{
    return m_tableCommon->gridCellClipping();
}

void QicsTable::setGridCellClipping(Qics::QicsGridCellClipping c)
{
    m_tableCommon->setGridCellClipping(c);
}

bool QicsTable::drawPartialCells() const
{
    return m_tableCommon->drawPartialCells();
}

void QicsTable::setDrawPartialCells(bool b)
{
    m_tableCommon->setDrawPartialCells(b);
}

bool QicsTable::horizontalGridLinesVisible() const
{
    return m_tableCommon->horizontalGridLinesVisible();
}

void QicsTable::setHorizontalGridLinesVisible(bool b)
{
    m_tableCommon->setHorizontalGridLinesVisible(b);
    m_tableCommon->rowHeaderRef().setHorizontalGridLinesVisible(b);
    m_tableCommon->columnHeaderRef().setHorizontalGridLinesVisible(b);
}

bool QicsTable::verticalGridLinesVisible() const
{
    return m_tableCommon->verticalGridLinesVisible();
}

void QicsTable::setVerticalGridLinesVisible(bool b)
{
    m_tableCommon->setVerticalGridLinesVisible(b);
    m_tableCommon->rowHeaderRef().setVerticalGridLinesVisible(b);
    m_tableCommon->columnHeaderRef().setVerticalGridLinesVisible(b);
}

int QicsTable::horizontalGridLineWidth() const
{
    return m_tableCommon->horizontalGridLineWidth();
}

void QicsTable::setHorizontalGridLineWidth(int w)
{
    m_tableCommon->setHorizontalGridLineWidth(w);
    m_tableCommon->rowHeaderRef().setHorizontalGridLineWidth(w);
    m_tableCommon->columnHeaderRef().setHorizontalGridLineWidth(w);
}

int QicsTable::verticalGridLineWidth() const
{
    return m_tableCommon->verticalGridLineWidth();
}

void QicsTable::setVerticalGridLineWidth(int w)
{
    m_tableCommon->setVerticalGridLineWidth(w);
    m_tableCommon->rowHeaderRef().setVerticalGridLineWidth(w);
    m_tableCommon->columnHeaderRef().setVerticalGridLineWidth(w);
}

Qics::QicsLineStyle QicsTable::horizontalGridLineStyle() const
{
    return m_tableCommon->horizontalGridLineStyle();
}

void QicsTable::setHorizontalGridLineStyle(Qics::QicsLineStyle style)
{
    m_tableCommon->setHorizontalGridLineStyle(style);
    m_tableCommon->rowHeaderRef().setHorizontalGridLineStyle(style);
    m_tableCommon->columnHeaderRef().setHorizontalGridLineStyle(style);
}

Qics::QicsLineStyle QicsTable::verticalGridLineStyle() const
{
    return m_tableCommon->verticalGridLineStyle();
}

void QicsTable::setVerticalGridLineStyle(Qics::QicsLineStyle style)
{
    m_tableCommon->setVerticalGridLineStyle(style);
    m_tableCommon->rowHeaderRef().setVerticalGridLineStyle(style);
    m_tableCommon->columnHeaderRef().setVerticalGridLineStyle(style);
}

QPen QicsTable::horizontalGridLinePen() const
{
    return m_tableCommon->horizontalGridLinePen();
}

void QicsTable::setHorizontalGridLinePen(const QPen &pen)
{
    m_tableCommon->setHorizontalGridLinePen(pen);
    m_tableCommon->rowHeaderRef().setHorizontalGridLinePen(pen);
    m_tableCommon->columnHeaderRef().setHorizontalGridLinePen(pen);
}

QPen QicsTable::verticalGridLinePen() const
{
    return m_tableCommon->verticalGridLinePen();
}

void QicsTable::setVerticalGridLinePen(const QPen &pen)
{
    m_tableCommon->setVerticalGridLinePen(pen);
    m_tableCommon->rowHeaderRef().setVerticalGridLinePen(pen);
    m_tableCommon->columnHeaderRef().setVerticalGridLinePen(pen);
}

Qics::QicsCellOverflowBehavior QicsTable::cellOverflowBehavior() const
{
    return m_tableCommon->cellOverflowBehavior();
}

void QicsTable::setCellOverflowBehavior(Qics::QicsCellOverflowBehavior b)
{
    m_tableCommon->setCellOverflowBehavior(b);
}

QPoint QicsTable::cellToolTipOffset() const
{
    return m_tableCommon->cellToolTipOffset();
}

void QicsTable::setCellToolTipOffset(const QPoint &pos)
{
    m_tableCommon->setCellToolTipOffset(pos);
    m_tableCommon->rowHeaderRef().setCellToolTipOffset(pos);
    m_tableCommon->columnHeaderRef().setCellToolTipOffset(pos);
}

int QicsTable::maxOverflowCells() const
{
    return m_tableCommon->maxOverflowCells();
}

void QicsTable::setMaxOverflowCells(int num)
{
    m_tableCommon->setMaxOverflowCells(num);
}

int QicsTable::frameLineWidth() const
{
    return m_tableCommon->frameLineWidth();
}

void QicsTable::setFrameLineWidth(int lw)
{
    m_tableCommon->setFrameLineWidth(lw);
    //m_tableCommon->rowHeaderRef().setFrameLineWidth(lw);
    //m_tableCommon->columnHeaderRef().setFrameLineWidth(lw);
}

int QicsTable::frameStyle() const
{
    return m_tableCommon->frameStyle();
}

void QicsTable::setFrameStyle(int style)
{
    m_tableCommon->setFrameStyle(style);
    //m_tableCommon->rowHeaderRef().setFrameStyle(style);
    //m_tableCommon->columnHeaderRef().setFrameStyle(style);
}

Qics::QicsCurrentCellStyle QicsTable::currentCellStyle() const
{
    return m_tableCommon->currentCellStyle();
}

Qics::QicsSelectionStyle QicsTable::selectionStyle() const
{
    return m_tableCommon->selectionStyle();
}

void QicsTable::setSelectionStyle(QicsSelectionStyle s)
{
    m_tableCommon->setSelectionStyle(s);
}

void QicsTable::setCurrentCellStyle(QicsCurrentCellStyle s)
{
    m_tableCommon->setCurrentCellStyle(s);
}

int QicsTable::currentCellBorderWidth() const
{
    return m_tableCommon->currentCellBorderWidth();
}

void QicsTable::setCurrentCellBorderWidth(int w)
{
    m_tableCommon->setCurrentCellBorderWidth(w);
}

bool QicsTable::clickToEdit() const
{
    return m_tableCommon->clickToEdit();
}

void QicsTable::setClickToEdit(bool b)
{
    m_tableCommon->setClickToEdit(b);
}

bool QicsTable::autoSelectCellContents() const
{
    return m_tableCommon->autoSelectCellContents();
}

void QicsTable::setAutoSelectCellContents(bool b)
{
    m_tableCommon->setAutoSelectCellContents(b);
}

Qics::QicsTraversalKeyEditBehavior QicsTable::traversalKeyEditBehavior() const
{
    return m_tableCommon->traversalKeyEditBehavior();
}

void QicsTable::setTraversalKeyEditBehavior(QicsTraversalKeyEditBehavior beh)
{
    m_tableCommon->setTraversalKeyEditBehavior(beh);
}

Qt::Orientation QicsTable::enterTraversalDirection() const
{
    return m_tableCommon->enterTraversalDirection();
}

void QicsTable::setEnterTraversalDirection(Qt::Orientation dir)
{
    m_tableCommon->setEnterTraversalDirection(dir);

    if(dir == Qt::Vertical) {
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::NoModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseDownWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseUpWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::KeypadModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseDownWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::KeypadModifier | Qt::ShiftModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseUpWithJump);
        myKeyboardManager.setKeyAction(QicsKeyCombination(
            Qt::NoModifier, Qt::Key_Return), QicsKeyboardManager::TraverseDownWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Return), QicsKeyboardManager::TraverseUpWithJump);
    }
    else {
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::NoModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseRightWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseLeftWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::KeypadModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseRightWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::KeypadModifier | Qt::ShiftModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseLeftWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::NoModifier, Qt::Key_Return), QicsKeyboardManager::TraverseRightWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Return), QicsKeyboardManager::TraverseLeftWithJump);
    }
}

Qt::Orientation QicsTable::tabTraversalDirection() const
{
    return m_tableCommon->tabTraversalDirection();
}

void QicsTable::setTabTraversalDirection(Qt::Orientation dir)
{
    m_tableCommon->setTabTraversalDirection(dir);
    if(dir == Qt::Vertical) {
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::NoModifier, Qt::Key_Tab), QicsKeyboardManager::TraverseDownWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::NoModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseUpWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseUpWithJump);
    }
    else {
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::NoModifier, Qt::Key_Tab), QicsKeyboardManager::TraverseRightWithJump);
        myKeyboardManager.setKeyAction(QicsKeyCombination(
            Qt::NoModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseLeftWithJump);
        myKeyboardManager.setKeyAction(
            QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseLeftWithJump);
    }
}

QicsKeyboardManager::Action QicsTable::keyAction(const QicsKeyCombination& combo)
{
    return myKeyboardManager.keyAction(combo);
}

void QicsTable::setKeyAction(const QicsKeyCombination& combo,QicsKeyboardManager::Action action)
{
    myKeyboardManager.setKeyAction(combo,action);
}

QPixmap QicsTable::moreTextPixmap() const
{
    return m_tableCommon->moreTextPixmap();
}

bool QicsTable::tabTraverseToBegin() const
{
    return m_tableCommon->tabTraverseToBegin();
}

void QicsTable::setTabTraverseToBegin(bool traverse)
{
    if(traverse) {
        if(tabTraversalDirection() == Qt::Vertical) {
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Tab), QicsKeyboardManager::TraverseDownWithJump);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseUpWithJump);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseUpWithJump);
        }
        else {
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Tab), QicsKeyboardManager::TraverseRightWithJump);
            myKeyboardManager.setKeyAction(QicsKeyCombination(
                Qt::NoModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseLeftWithJump);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseLeftWithJump);
        }
    }
    else {
        if(tabTraversalDirection() == Qt::Vertical) {
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Tab), QicsKeyboardManager::TraverseDown);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseUp);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseUp);
        }
        else {
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Tab), QicsKeyboardManager::TraverseRight);
            myKeyboardManager.setKeyAction(QicsKeyCombination(
                Qt::NoModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseLeft);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Backtab), QicsKeyboardManager::TraverseLeft);
        }
    }
    m_tableCommon->setTabTraverseToBegin(traverse);
}

bool QicsTable::enterTraverseToBegin() const
{
    return m_tableCommon->enterTraverseToBegin();
}

void QicsTable::setEnterTraverseToBegin(bool traverse)
{
    m_tableCommon->setEnterTraverseToBegin(traverse);
    if(traverse) {
        if(enterTraversalDirection() == Qt::Vertical) {
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseDownWithJump);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseUpWithJump);
            myKeyboardManager.setKeyAction(QicsKeyCombination(
                Qt::NoModifier, Qt::Key_Return), QicsKeyboardManager::TraverseDownWithJump);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Return), QicsKeyboardManager::TraverseUpWithJump);
        }
        else {
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseRightWithJump);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseLeftWithJump);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Return), QicsKeyboardManager::TraverseRightWithJump);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Return), QicsKeyboardManager::TraverseLeftWithJump);
        }
    }
    else {
        if(enterTraversalDirection() == Qt::Vertical) {
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseDown);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseUp);
            myKeyboardManager.setKeyAction(QicsKeyCombination(
                Qt::NoModifier, Qt::Key_Return), QicsKeyboardManager::TraverseDown);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Return), QicsKeyboardManager::TraverseUp);
        }
        else {
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseRight);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Enter), QicsKeyboardManager::TraverseLeft);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::NoModifier, Qt::Key_Return), QicsKeyboardManager::TraverseRight);
            myKeyboardManager.setKeyAction(
                QicsKeyCombination(Qt::ShiftModifier, Qt::Key_Return), QicsKeyboardManager::TraverseLeft);
        }
    }
}

void QicsTable::setMoreTextPixmap(const QPixmap &pix)
{
    m_tableCommon->setMoreTextPixmap(pix);
}

bool QicsTable::dragEnabled() const
{
    return m_tableCommon->dragEnabled();
}

void QicsTable::setDragEnabled(bool b)
{
    m_tableCommon->setDragEnabled(b);
}

Qt::LayoutDirection QicsTable::layoutDirection() const
{
    return m_tableCommon->layoutDirection();
}

void QicsTable::setLayoutDirection(Qt::LayoutDirection layoutDirection)
{
    m_tableCommon->setLayoutDirection(layoutDirection);
    m_tableCommon->rowHeaderRef().setLayoutDirection(layoutDirection);
    m_tableCommon->columnHeaderRef().setLayoutDirection(layoutDirection);
    QFrame::setLayoutDirection(layoutDirection);
}

bool QicsTable::fillHandleVisible() const
{
    return m_tableCommon->fillHandleVisible();
}

void QicsTable::setFillHandleVisible(bool b)
{
    m_tableCommon->setFillHandleVisible(b);
}

bool QicsTable::ignoreModelSizeChanges() const
{
    return m_tableCommon->ignoreModelSizeChanges();
}

void QicsTable::setIgnoreModelSizeChanges(bool b)
{
    m_tableCommon->setIgnoreModelSizeChanges(b);
}

bool QicsTable::allowPropagateChanges() const
{
    return gridInfo().allowPropagateChanges();
}

void QicsTable::setAllowPropagateChanges(bool b)
{
    gridInfo().setAllowPropagateChanges(b);
}

Qics::QicsIndexType QicsTable::fakeCellsDrawingIndex() const
{
    return m_tableCommon->fakeCellsDrawingIndex();
}

void QicsTable::setFakeCellsDrawingIndex(Qics::QicsIndexType type)
{
    m_tableCommon->setFakeCellsDrawingIndex(type);
}

Qics::QicsHeaderDragStyle QicsTable::headerDragStyle() const
{
    return m_tableCommon->rhGridInfo().headerDragStyle();
}

void QicsTable::setHeaderDragStyle(const Qics::QicsHeaderDragStyle & style)
{
    m_tableCommon->rhGridInfo().setHeaderDragStyle(style);
    m_tableCommon->chGridInfo().setHeaderDragStyle(style);
}

int QicsTable::topRow() const
{
    return (MAIN_GRID->topRow());
}

int QicsTable::bottomRow() const
{
    return (MAIN_GRID->bottomRow());
}

int QicsTable::leftColumn() const
{
    return (MAIN_GRID->leftColumn());
}

int QicsTable::rightColumn() const
{
    return (MAIN_GRID->rightColumn());
}

void QicsTable::setCurrentCell(int row, int col)
{
    MAIN_GRID->traverseToCell(row, col);
}

void QicsTable::setCurrentCell(const QicsICell &cell)
{
    MAIN_GRID->traverseToCell(cell.row(), cell.column());
}

bool QicsTable::editCurrentCell()
{
    return (MAIN_GRID->editCurrentCell());
}

void QicsTable::uneditCurrentCell()
{
    MAIN_GRID->uneditCurrentCell();
}

bool QicsTable::editCell(int row, int col)
{
    return (MAIN_GRID->editCell(row, col));
}

Qics::QicsRepaintBehavior QicsTable::repaintBehavior() const
{
    return (gridInfo().gridRepaintBehavior());
}

void QicsTable::setRepaintBehavior(QicsRepaintBehavior r)
{
    gridInfo().setGridRepaintBehavior(r);
    rhGridInfo().setGridRepaintBehavior(r);
    chGridInfo().setGridRepaintBehavior(r);
}

Qics::QicsScrollBarMode QicsTable::hScrollBarMode() const
{
    return m_columnScroller->mode();
}

Qics::QicsScrollBarMode QicsTable::vScrollBarMode() const
{
    return m_rowScroller->mode();
}

void QicsTable::setHScrollBarMode(QicsScrollBarMode m)
{
    m_columnScroller->setMode(m);
}

void QicsTable::setVScrollBarMode(QicsScrollBarMode m)
{
    m_rowScroller->setMode(m);
}

void QicsTable::sortRows(int colnum, Qics::QicsSortOrder order,
                    int from, int to,
                    DataItemComparator func)
{
    QVector<int> colsnum;
    colsnum << colnum;
    m_tableCommon->sortRows(colsnum, order, from, to, func);
}

void QicsTable::sortColumns(int rownum, Qics::QicsSortOrder order,
                       int from, int to,
                       DataItemComparator func)
{
    QVector<int> rowsnum;
    rowsnum << rownum;
    m_tableCommon->sortColumns(rowsnum, order, from, to, func);
}

void QicsTable::sortRows(const QVector<int> &colsnum, Qics::QicsSortOrder order,
                    int from, int to,
                    DataItemComparator func)
{
    m_tableCommon->sortRows(colsnum, order, from, to, func);
}

void QicsTable::sortColumns(const QVector<int> &rowsnum, Qics::QicsSortOrder order,
                       int from, int to,
                       DataItemComparator func)
{
    m_tableCommon->sortColumns(rowsnum, order, from, to, func);
}

void QicsTable::moveRows(int target_row, const QVector<int> &rows)
{
    m_tableCommon->moveRows(target_row, rows);
}

void QicsTable::moveColumns(int target_col, const QVector<int> &cols)
{
    m_tableCommon->moveColumns(target_col, cols);
}

void QicsTable::setRowOrder(const QVector<int> &order)
{
    gridInfo().rowOrdering()->reorder(order);
    update();
}

void QicsTable::setColumnOrder(const QVector<int> &order)
{
    gridInfo().columnOrdering()->reorder(order);
    update();
}

bool QicsTable::rowHeaderUsesModel() const
{
    return m_tableCommon->rowHeaderUsesModel();
}

void QicsTable::setRowHeaderUsesModel(bool b)
{
    m_tableCommon->setRowHeaderUsesModel(b);
    configureFrozen();
}

bool QicsTable::columnHeaderUsesModel() const
{
    return m_tableCommon->columnHeaderUsesModel();
}

void QicsTable::setColumnHeaderUsesModel(bool b)
{
    m_tableCommon->setColumnHeaderUsesModel(b);
    configureFrozen();
}

void QicsTable::setSortingSensitivity(Qt::CaseSensitivity cs)
{
    m_tableCommon->setSortingSensitivity(cs);
}
////////////////////////////////////////////////////////////////////
///////////               Selection methods           //////////////
////////////////////////////////////////////////////////////////////

QicsSelectionList *QicsTable::selectionList(bool nocopy) const
{
    return m_selectionManager->selectionList(nocopy);
}

QicsSelectionList *QicsTable::selectionActionList() const
{
    return m_selectionManager->selectionActionList();
}

void QicsTable::setSelectionList(QicsSelectionList &sel_list)
{
    m_selectionManager->setSelectionList(sel_list);
}

void QicsTable::clearSelectionList()
{
    m_selectionManager->clearSelectionList();
}

void QicsTable::addSelection(QicsSelection &selection)
{
    m_selectionManager->addSelection(selection);
}

void QicsTable::selectAll()
{
    gridInfo().reportSelection(Qics::SelectionBegin, 0, 0, QicsLAST_ROW, QicsLAST_COLUMN);
    gridInfo().reportSelection(Qics::SelectionEnd, 0, 0, QicsLAST_ROW, QicsLAST_COLUMN);
}

void QicsTable::deleteSelected()
{
    m_tableCommon->deleteSelected();
}

Qics::QicsSelectionPolicy QicsTable::selectionPolicy() const
{
    return m_selectionManager->selectionPolicy();
}

void QicsTable::setSelectionPolicy(QicsSelectionPolicy policy)
{
    m_selectionManager->setSelectionPolicy(policy);
}

Qics::QicsSelectCurrentCellPolicy QicsTable::selectCurrentCellPolicy() const
{
    return m_selectionManager->selectCurrentCellPolicy();
}

void QicsTable::setSelectCurrentCellPolicy(QicsSelectCurrentCellPolicy policy)
{
    m_selectionManager->setSelectCurrentCellPolicy(policy);
}

void QicsTable::selectRow(int row, bool exclusive)
{
    QicsSelectionList sl(dataModel());
    if (!exclusive && m_selectionManager->selectionList(true))
        sl = *(m_selectionManager->selectionList(true));
    sl.append(QicsSelection(row, 0, row, QicsLAST_COLUMN));
    m_selectionManager->setSelectionList(sl);
}

void QicsTable::selectColumn(int col, bool exclusive)
{
    QicsSelectionList sl(dataModel());
    if (!exclusive && m_selectionManager->selectionList(true))
        sl = *(m_selectionManager->selectionList(true));
    sl.append(QicsSelection(0, col, QicsLAST_ROW, col));
    m_selectionManager->setSelectionList(sl);
}

int QicsTable::exclusiveSelectionBorderWidth() const
{
    return m_tableCommon->exclusiveSelectionBorderWidth();
}

void QicsTable::setExclusiveSelectionBorderWidth(int lw)
{
    m_tableCommon->setExclusiveSelectionBorderWidth(lw);
}

QColor QicsTable::exclusiveSelectionBorderColor() const
{
    return m_tableCommon->exclusiveSelectionBorderColor();
}

void QicsTable::setExclusiveSelectionBorderColor(const QColor &c)
{
    m_tableCommon->setExclusiveSelectionBorderColor(c);
}

QColor QicsTable::exclusiveSelectionHandleColor() const
{
    return m_tableCommon->exclusiveSelectionHandleColor();
}

void QicsTable::setExclusiveSelectionHandleColor(const QColor &c)
{
    m_tableCommon->setExclusiveSelectionHandleColor(c);
}

QColor QicsTable::exclusiveSelectionHandleBackColor() const
{
    return m_tableCommon->exclusiveSelectionHandleBackColor();
}

void QicsTable::setExclusiveSelectionHandleBackColor(const QColor &c)
{
    m_tableCommon->setExclusiveSelectionHandleBackColor(c);
}

QPen QicsTable::exclusiveSelectionDragPen() const
{
    return m_tableCommon->exclusiveSelectionDragPen();
}

void QicsTable::setExclusiveSelectionDragPen(const QPen &p)
{
    m_tableCommon->setExclusiveSelectionDragPen(p);
}

QColor QicsTable::exclusiveSelectionDragBackColor() const
{
    return m_tableCommon->exclusiveSelectionDragBackColor();
}

void QicsTable::setExclusiveSelectionDragBackColor(const QColor &c)
{
    m_tableCommon->setExclusiveSelectionDragBackColor(c);
}

QicsRubberBand::DrawingMode QicsTable::exclusiveSelectionDragMode() const
{
    return m_tableCommon->exclusiveSelectionDragMode();
}

void QicsTable::setExclusiveSelectionDragMode(QicsRubberBand::DrawingMode lw)
{
    m_tableCommon->setExclusiveSelectionDragMode(lw);
}

float QicsTable::exclusiveSelectionDragOpacity() const
{
    return m_tableCommon->exclusiveSelectionDragOpacity();
}

void QicsTable::setExclusiveSelectionDragOpacity(float lw)
{
    m_tableCommon->setExclusiveSelectionDragOpacity(lw);
}

#define DISPLAY_OPT(b) ((b) ? DisplayAlways : DisplayNever)

bool QicsTable::topHeaderVisible() const
{
    return (m_tableCommon->topHeaderVisible() != DisplayNever);
}

void QicsTable::setTopHeaderVisible(bool b)
{
    m_tableCommon->setTopHeaderVisible(DISPLAY_OPT(b));
    configureFrozen();
}

bool QicsTable::bottomHeaderVisible() const
{
    return (m_tableCommon->bottomHeaderVisible() != DisplayNever);
}

void QicsTable::setBottomHeaderVisible(bool b)
{
    m_tableCommon->setBottomHeaderVisible(DISPLAY_OPT(b));
    configureFrozen();
}

bool QicsTable::leftHeaderVisible() const
{
    return (m_tableCommon->leftHeaderVisible() != DisplayNever);
}

void QicsTable::setLeftHeaderVisible(bool b)
{
    m_tableCommon->setLeftHeaderVisible(DISPLAY_OPT(b));
    configureFrozen();
}

bool QicsTable::rightHeaderVisible() const
{
    return (m_tableCommon->rightHeaderVisible() != DisplayNever);
}

void QicsTable::setRightHeaderVisible(bool b)
{
    m_tableCommon->setRightHeaderVisible(DISPLAY_OPT(b));
    configureFrozen();
}

int QicsTable::columnHeaderWidth()
{
    if (leftHeaderVisible())
        return myGridGeometry.headerGrid(QicsGridGeometry::Left, 2)->width();
    if (rightHeaderVisible())
        return myGridGeometry.headerGrid(QicsGridGeometry::Right, 2)->width();
    return 0;
}

int QicsTable::rowHeaderHeight()
{
    if (topHeaderVisible())
        return myGridGeometry.headerGrid(QicsGridGeometry::Top, 2)->height();
    if (bottomHeaderVisible())
        return myGridGeometry.headerGrid(QicsGridGeometry::Bottom, 2)->height();
    return 0;
}

int QicsTable::tableMargin() const
{
    return (m_tableCommon->tableMargin());
}

int QicsTable::tableSpacing() const
{
    return (m_tableCommon->tableSpacing());
}

int QicsTable::gridSpacing() const
{
    return (m_tableCommon->gridSpacing());
}

void QicsTable::setTableMargin(int margin)
{
    m_tableCommon->setTableMargin(margin);
    m_gridLayout->setMargin(margin);
}

void QicsTable::setTableSpacing(int spacing)
{
    m_tableCommon->setTableSpacing(spacing);
    m_gridLayout->setSpacing(spacing);
}

void QicsTable::setGridSpacing(int spacing)
{
    m_tableCommon->setGridSpacing(spacing);
    m_gridLayout->setSpacing(spacing);
}


void QicsTable::scroll(Qics::QicsScrollDirection dir, int num)
{
    switch (dir)
    {
    case ScrollUp:
        m_scrollManager->setIndex(Qics::RowIndex, m_scrollManager->rowIndex() - num);
        break;
    case ScrollDown:
        m_scrollManager->setIndex(Qics::RowIndex, m_scrollManager->rowIndex() + num);
        break;
    case ScrollLeft:
        m_scrollManager->setIndex(Qics::ColumnIndex, m_scrollManager->columnIndex() - num);
        break;
    case ScrollRight:
        m_scrollManager->setIndex(Qics::ColumnIndex, m_scrollManager->columnIndex() + num);
        break;
    case ScrollToRow:
        if ( verticalScrollBar()->isVisible() )
            m_scrollManager->setIndex(RowIndex, num);
        break;
    case ScrollToColumn:
        if ( horizontalScrollBar()->isVisible() )
            m_scrollManager->setIndex(ColumnIndex, num);
        break;
    default:
        break;
    }
}

void QicsTable::freezeTopRows(int num_rows)
{
    if ((currentViewport().endRow() - m_bottomFrozenRows - num_rows) < 0) {
        // if num_rows are to big
        num_rows = currentViewport().endRow() - m_bottomFrozenRows;
    }   // num_rows = maximal of possible
    if (num_rows <= 0) {
        myUnfreezingFlag = true;
        num_rows = 0;
    }
    else
        myUnfreezingFlag = false;

    QicsRegion vp = MAIN_GRID->viewport();
    vp.setStartRow(vp.startRow() - m_topFrozenRows + num_rows);

    m_topFrozenRows = num_rows;

    setMainGridViewport(vp);
    configureFrozen();
}

void QicsTable::freezeBottomRows(int num_rows)
{
    if ((currentViewport().endRow() - m_topFrozenRows - num_rows) < 0) {
        // if num_rows are to big
        num_rows = currentViewport().endRow() - m_topFrozenRows;
    }   // num_rows = maximal of possible
    if (num_rows <= 0) {
        myUnfreezingFlag = true;
        num_rows = 0;
    }
    else
        myUnfreezingFlag = false;

    QicsRegion vp = MAIN_GRID->currentViewport/*viewport*/();
    if (num_rows)
        vp.setEndRow(vp.endRow() + m_bottomFrozenRows - num_rows);
    else
        vp.setEndRow(INT_MAX);

    m_bottomFrozenRows = num_rows;

    setMainGridViewport(vp);
    configureFrozen();
}

void QicsTable::freezeLeftColumns(int num_cols)
{
    if ((currentViewport().endColumn() - m_rightFrozenColumns - num_cols) < 0) {
        // if num_colss are to big
        num_cols = currentViewport().endColumn() - m_rightFrozenColumns;
    }   // num_cols = maximal of possible
    if (num_cols <= 0) {
        myUnfreezingFlag = true;
        num_cols = 0;
    }
    else
        myUnfreezingFlag = false;

    QicsRegion vp = MAIN_GRID->viewport();
    vp.setStartColumn(vp.startColumn() - m_leftFrozenColumns + num_cols);

    m_leftFrozenColumns = num_cols;

    setMainGridViewport(vp);
    configureFrozen();
}

void QicsTable::freezeRightColumns(int num_cols)
{
    if ((currentViewport().endColumn() - m_leftFrozenColumns - num_cols) < 0) {
        // if num_colss are to big
        num_cols = currentViewport().endColumn() - m_leftFrozenColumns;
    }   // num_cols = maximal of possible
    if (num_cols <= 0) {
        myUnfreezingFlag = true;
        num_cols = 0;
    }
    else
        myUnfreezingFlag = false;

    QicsRegion vp = MAIN_GRID->currentViewport/*viewport*/();
    if (num_cols)
        vp.setEndColumn(vp.endColumn() + m_rightFrozenColumns - num_cols);
    else
        vp.setEndColumn(INT_MAX);

    m_rightFrozenColumns = num_cols;

    setMainGridViewport(vp);
    configureFrozen();
}

void QicsTable::unfreezeTopRows()
{
    freezeTopRows(0);
}

void QicsTable::unfreezeBottomRows()
{
    freezeBottomRows(0);
}

void QicsTable::unfreezeLeftColumns()
{
    freezeLeftColumns(0);
}

void QicsTable::unfreezeRightColumns()
{
    freezeRightColumns(0);
}

void QicsTable::revertRepaintBehavior()
{
    gridInfo().revertGridRepaintBehavior();
    rhGridInfo().revertGridRepaintBehavior();
    chGridInfo().revertGridRepaintBehavior();
}

void QicsTable::handleHeaderDoubleClick(int idx, int button, Qics::QicsHeaderType type)
{
    Q_UNUSED(button);

    QicsHeader hdr(&(gridInfo()));
    QicsMouseMap * mouseMap = hdr.mouseButtonRef();
    if (mouseMap->currentLeftButton()) {
        QicsSelectionList *sl = selectionList(true);
        if (sl && sl->count()) {
            if ((type == RowHeader && sl->isRowSelected(idx)) || (type == ColumnHeader && sl->isColumnSelected(idx))) {

                setRepaintBehavior(RepaintOff);
                QApplication::setOverrideCursor(Qt::WaitCursor);//#90147

                for (int i = 0; i < sl->count(); ++i) {
                    const QicsSelection &sel = sl->at(i);
                    if (type == RowHeader) {
                        if (sel.isEntireRow()) {
                            // toggle all rows
                            int endRow = sel.bottomRow();
                            if (endRow == QicsLAST_ROW) endRow = dataModel()->numRows()-1;
                            for (int j = sel.topRow(); j <= endRow; ++j)
                                toggleRowHeightExpansion(j);
                        }
                    }
                    else {
                        if (sel.isEntireColumn()) {
                            // toggle all columns
                            int endCol = sel.rightColumn();
                            if (endCol == QicsLAST_COLUMN) endCol = dataModel()->numColumns()-1;
                            for (int j = sel.leftColumn(); j <= endCol; ++j)
                                toggleColumnWidthExpansion(j);
                        }
                    }
                }

                QApplication::restoreOverrideCursor();//#90147
                setRepaintBehavior(RepaintOn);

                return;
            }
        }

        if (type == RowHeader) {
            toggleRowHeightExpansion(idx, true);//#90147
        }
        else {
            toggleColumnWidthExpansion(idx, true);//#90147
        }
    }
}

void QicsTable::addRows(int how_many)
{
    int bottomFrozenRows = 0;

    if (m_bottomFrozenRows > 0) {
        bottomFrozenRows = m_bottomFrozenRows;
        unfreezeBottomRows();
    }

    m_tableCommon->addRows(how_many);

    if (bottomFrozenRows > 0)
        freezeBottomRows(bottomFrozenRows);
}

void QicsTable::insertRow(int row)
{
    int bottomFrozenRows = 0;

    if (m_bottomFrozenRows > 0) {
        bottomFrozenRows = m_bottomFrozenRows;
        unfreezeBottomRows();
    }

    m_tableCommon->insertRow(row);

    if (bottomFrozenRows > 0)
        freezeBottomRows(bottomFrozenRows);
}

void QicsTable::deleteRow(int row)
{
    deleteRows(1, row);
}

void QicsTable::deleteRows(int num, int start_position)
{
    int bottomFrozenRows = 0;

    if (m_bottomFrozenRows > 0) {
        bottomFrozenRows = m_bottomFrozenRows;
        unfreezeBottomRows();
    }

    m_tableCommon->deleteRows(num, start_position);

    if (bottomFrozenRows > 0)
        freezeBottomRows(bottomFrozenRows);
}

void QicsTable::addColumns(int how_many)
{
    int rightFrozenColumns = 0;
    if (m_rightFrozenColumns > 0) {
        rightFrozenColumns = m_rightFrozenColumns;
        unfreezeRightColumns();
    }

    m_tableCommon->addColumns(how_many);

    if (rightFrozenColumns > 0)
        freezeRightColumns(rightFrozenColumns);
}

void QicsTable::insertColumn(int col)
{
    int rightFrozenColumns = 0;

    if (m_rightFrozenColumns > 0) {
        rightFrozenColumns = m_rightFrozenColumns;
        unfreezeRightColumns();
    }

    m_tableCommon->insertColumn(col);

    if (rightFrozenColumns > 0)
        freezeRightColumns(rightFrozenColumns);
}

void QicsTable::deleteColumn(int col)
{
    deleteColumns(1, col);
}

void QicsTable::deleteColumns(int num, int start_position)
{
    int rightFrozenColumns = 0;

    if (m_rightFrozenColumns > 0) {
        rightFrozenColumns = m_rightFrozenColumns;
        unfreezeRightColumns();
    }

    m_tableCommon->deleteColumns(num, start_position);

    if (rightFrozenColumns > 0)
        freezeRightColumns(rightFrozenColumns);
}

#ifndef QICSTABLE_GPL
void
QicsTable::print(QPrinter *printer)
{
    print(printer, viewport());
}

void QicsTable::print(QPrinter *printer, const QicsRegion &region)
{
    QicsTablePrint* tp = new QicsTablePrint(m_tableCommon);
    tp->print(printer, region);
    delete tp;
}
#endif

void QicsTable::setDefaultRowHeightInPixels(int height)
{
    m_tableCommon->setDefaultRowHeightInPixels(height);
}

void QicsTable::setDefaultRowHeightInChars(int height)
{
    m_tableCommon->setDefaultRowHeightInChars(height);
}

void QicsTable::setDefaultColumnWidthInPixels(int width)
{
    m_tableCommon->setDefaultColumnWidthInPixels(width);
}

void QicsTable::setDefaultColumnWidthInChars(int width)
{
    m_tableCommon->setDefaultColumnWidthInChars(width);
}

void QicsTable::setCellWidthMode(Qics::QicsCellWidthMode mode)
{
    dimensionManager()->setCellWidthMode(mode);
}

void QicsTable::cut()
{
    if (myClipboardDelegate && myClipboardDelegate->cut(this, selectionList()))
        return;

    QApplication::clipboard()->setMimeData(m_tableCommon->cutCopyData());

    m_tableCommon->finishCut(true);
}

void QicsTable::copy()
{
    if (myClipboardDelegate && myClipboardDelegate->copy(this, selectionList()))
        return;

    QApplication::clipboard()->setMimeData(m_tableCommon->cutCopyData());

    m_tableCommon->finishCut(false);
}

void QicsTable::paste()
{
    QicsDataModel *dm = dataModel();
    if (!dm)
        return;

    if (myClipboardDelegate && myClipboardDelegate->paste(this, selectionList()))
        return;

    QicsICell cur_cell = gridInfo().currentCell();

    QicsSelectionList *list = selectionList(true);
    if (list && !list->isEmpty())
        cur_cell = QicsICell(list->at(0).topRow(), list->at(0).leftColumn());

    if (cur_cell.isValid()) {
        const QMimeData *data = QApplication::clipboard()->mimeData();

        QicsDataModelDefault tmp_dm;
        QicsPasteValidator *pValidator = cell(cur_cell.row(), cur_cell.column())->pasteValidator();

        if(!pValidator) {
            m_tableCommon->paste(data, cur_cell);
        }
        else if (QicsTableRegionDrag::decode(data, tmp_dm)) {
            if(pValidator->validate(cur_cell,0,tmp_dm,dm))
                m_tableCommon->paste(data, cur_cell);
        }
        else {
            tmp_dm.addRows(1);
            tmp_dm.addColumns(1);
            tmp_dm.setItem(0,0,QicsDataString(data->text()));
            if(pValidator->validate(cur_cell,0,tmp_dm,dm))
                m_tableCommon->paste(QApplication::clipboard()->mimeData(), cur_cell);
        }
    }
}

void QicsTable::handleGridPress(int row, int col, int button, const QPoint &pos)
{
    QicsScreenGrid *grid = 0;
    QObject *snd = sender();

    for (int i = 0; i <= GRID_BOTTOM_IDX; ++i) {
        for (int j = 0; j <= GRID_RIGHT_IDX; ++j) {
            if (myGrids[i][j] == snd)
            {
                grid = myGrids[i][j];
                break;
            }
        }
    }

    if (grid) {
        emit pressed(row, col, button, grid->mapToParent(pos));
    }
}

void QicsTable::handleGridClick(int row, int col, int button, const QPoint &pos)
{
    QicsScreenGrid *grid = 0;
    QObject *snd = sender();

    for (int i = 0; i <= GRID_BOTTOM_IDX; ++i) {
        for (int j = 0; j <= GRID_RIGHT_IDX; ++j) {
            if (myGrids[i][j] == snd) {
                grid = myGrids[i][j];
                break;
            }
        }
    }

    if (grid) {
        emit clicked(row, col, button, grid->mapToParent(pos));
    }
}

void QicsTable::handleGridDoubleClick(int row, int col, int button,
                                 const QPoint &pos)
{
    QicsScreenGrid *grid = 0;
    QObject *snd = sender();

    for (int i = 0; i <= GRID_BOTTOM_IDX; ++i) {
        for (int j = 0; j <= GRID_RIGHT_IDX; ++j) {
            if (myGrids[i][j] == snd) {
                grid = myGrids[i][j];
                break;
            }
        }
    }

    if (grid) {
        emit doubleClicked(row, col, button, grid->mapToParent(pos));
    }
}

void QicsTable::modelToVisualValueChanged(int row, int col)
{
    // check if cell is now edit
    const QicsCell* cur_cell = currentCell();

    if ( cur_cell->isValid() && cur_cell->displayer()->isEditing() &&
        cur_cell->columnIndex()==gridInfo().visualColumnIndex(col) &&
        cur_cell->rowIndex()==gridInfo().visualRowIndex(row) ) {
        cur_cell->displayer()->startEdit(MAIN_GRID,
            gridInfo().visualRowIndex(row), gridInfo().visualColumnIndex(col),
            cur_cell->dataValue());
    }

    emit valueChanged(gridInfo().visualRowIndex(row),
        gridInfo().visualColumnIndex(col));
}

void QicsTable::handleCurrentCellChange(int row, int col)
{
    myCurrentCell->setRowIndex(row);
    myCurrentCell->setColumnIndex(col);
    emit currentCellChanged(row, col);
}

void QicsTable::handleFilterChanged(int index, bool set)
{
    Q_UNUSED(index);
    Q_UNUSED(set);

    //store scroll indexes
    const int column_index = m_scrollManager->columnIndex();
    const int row_index = m_scrollManager->rowIndex();

    setRepaintBehavior(Qics::RepaintOff);

    configureFrozen();

    //restore scroll indexes
    m_scrollManager->setIndex(Qics::ColumnIndex, column_index);
    m_scrollManager->setIndex(Qics::RowIndex, row_index);

    setRepaintBehavior(Qics::RepaintOn);

    emit filterChanged(index, set);

    repaint();
}

void QicsTable::repaint()
{
    layout()->invalidate();
    gridInfo().redrawAllGrids(true);
    rhGridInfo().redrawAllGrids(true);
    chGridInfo().redrawAllGrids(true);
    QFrame::repaint();
}

////////////////////////////////////////////////////////////////////////
//////////////////     Traversal Methods     ///////////////////////////
////////////////////////////////////////////////////////////////////////

bool QicsTable::traverseToCell(int row, int col)
{
    return (MAIN_GRID->traverseToCell(row, col));
}

void QicsTable::traverseToBeginningOfTable()
{
    MAIN_GRID->traverseToBeginningOfTable();
}

void QicsTable::traverseToEndOfTable()
{
    MAIN_GRID->traverseToEndOfTable();
}

void QicsTable::traverseLeft()
{
    MAIN_GRID->traverseLeft();
}

void QicsTable::traverseRight()
{
    MAIN_GRID->traverseRight();
}

void QicsTable::traverseUp()
{
    MAIN_GRID->traverseUp();
}

void QicsTable::traverseDown()
{
    MAIN_GRID->traverseDown();
}

void QicsTable::traversePageUp()
{
    MAIN_GRID->traversePageUp();
}

void QicsTable::traversePageDown()
{
    MAIN_GRID->traversePageDown();
}

void QicsTable::traverseToBeginningOfRow()
{
    MAIN_GRID->traverseToBeginningOfRow();
}

void QicsTable::traverseToEndOfRow()
{
    MAIN_GRID->traverseToEndOfRow();
}

void QicsTable::traverseToBeginningOfColumn()
{
    MAIN_GRID->traverseToBeginningOfColumn();
}

void QicsTable::traverseToEndOfColumn()
{
    MAIN_GRID->traverseToEndOfColumn();
}

void QicsTable::resizeHeader(int val, int pos, Qics::QicsHeaderType type)
{
    if(type == Qics::RowHeader) {
        int old_width = rhDimensionManager()->columnWidth(pos, true, false, true);
        rhDimensionManager()->setColumnWidthInPixels(pos, false, (old_width + val < 10) ? 10 : old_width + val);

    }
    else if(type == Qics::ColumnHeader) {
        int old_height = chDimensionManager()->rowHeight(pos, true, false, true);
        chDimensionManager()->setRowHeightInPixels(pos, false, (old_height + val < 10) ? 10 : old_height + val);
    }
}

void QicsTable::setCursor(const QCursor& cursor)
{
    //setting cursor for grids
    if(LEFT_HEADER)
        rowHeaderRef().setCursor(cursor);

    if(TOP_HEADER)
        columnHeaderRef().setCursor(cursor);

    if(MAIN_GRID)
        mainGridRef().setCursor(cursor);

    //setting cursor for inner widgets
    if(myTopTitleWidget)
        myTopTitleWidget->setCursor(cursor);

    if(myBottomTitleWidget)
        myBottomTitleWidget->setCursor(cursor);

    if(myBottomLeftCornerWidget)
        myBottomLeftCornerWidget->setCursor(cursor);

    if(myBottomRightCornerWidget)
        myBottomRightCornerWidget->setCursor(cursor);

    if(myLeftTitleWidget)
        myLeftTitleWidget->setCursor(cursor);

    if(myRightTitleWidget)
        myRightTitleWidget->setCursor(cursor);

    if(myTopLeftCornerWidget)
        myTopLeftCornerWidget->setCursor(cursor);

    if(myTopRightCornerWidget)
        myTopRightCornerWidget->setCursor(cursor);

    QFrame::setCursor(cursor);

}

void QicsTable::setHorizontalScrollBar(QicsScroller* newScroller)
{
    if(!newScroller)
        return;

    horizontalScrollBar()->removeEventFilter(this);
    m_scrollManager->disconnectScroller(m_scrollManager->horizontalScroller());
    m_columnScroller = newScroller;
    m_scrollManager->connectScroller(m_columnScroller);
    m_gridLayout->addWidget(newScroller->widget(),
        GRID_LAYOUT_BOTTOM_SCROLLBAR_IDX,
        GRID_LAYOUT_MAIN_GRID_IDX);
    newScroller->installEventFilter(this);
}

void QicsTable::setVerticalScrollBar(QicsScroller* newScroller)
{
    if(!newScroller)
        return;

    verticalScrollBar()->removeEventFilter(this);
    m_scrollManager->disconnectScroller(m_scrollManager->verticalScroller());
    m_rowScroller = newScroller;
    m_scrollManager->connectScroller(m_rowScroller);
    m_gridLayout->addWidget(newScroller->widget(),
        GRID_LAYOUT_MAIN_GRID_IDX,
        GRID_LAYOUT_RIGHT_SCROLLBAR_IDX);
    newScroller->installEventFilter(this);
}

// Flag which block or unblock event recursion.
static bool eventInProgress = false;

bool QicsTable::event ( QEvent * e )
{
    switch (e->type())
    {
    case QEvent::PaletteChange: {
        QPalette pal = palette();
        // Setting palette for all child widgets
        if(myTopTitleWidget)
            myTopTitleWidget->setPalette(pal);
        if(myBottomTitleWidget)
            myBottomTitleWidget->setPalette(pal);
        if(myLeftTitleWidget)
            myLeftTitleWidget->setPalette(pal);
        if(myRightTitleWidget)
            myRightTitleWidget->setPalette(pal);
        if(myTopLeftCornerWidget)
            myTopLeftCornerWidget->setPalette(pal);
        if(myTopRightCornerWidget)
            myTopRightCornerWidget->setPalette(pal);
        if(myBottomLeftCornerWidget)
            myBottomLeftCornerWidget->setPalette(pal);
        if(myBottomRightCornerWidget)
            myBottomRightCornerWidget->setPalette(pal);

        // Let's color roles will work like in QTableWidget:
        //  QPalette::Window - changes background color of widget.
        //  QPalette::Base - changes background color of cells.
        //  QPalette::Text - changes foreground color of cells.
        //  QPalette::WindowText - changes foreground color of widget text.
        return true;
    }

    case QEvent::Wheel:
        if (QApplication::focusWidget() != MAIN_GRID)
            return false;

        if (!eventInProgress) {
            eventInProgress = true;
            QWheelEvent *we = static_cast<QWheelEvent*>(e);

            if(we->modifiers() & Qt::AltModifier && horizontalScrollBar())
                QApplication::sendEvent(horizontalScrollBar(),we);
            else if(verticalScrollBar())
                QApplication::sendEvent(verticalScrollBar(),we);
            return true;
        }
        else {
            eventInProgress = false;
            return true;
        }

    case QEvent::FontChange: {
        QFont f = font();

        //setting font for grids
        if(LEFT_HEADER)
            rowHeaderRef().setFont(f);
        if(TOP_HEADER)
            columnHeaderRef().setFont(f);
        //if(MAIN_GRID)
        //    mainGridRef().setFont(f);
        //setting font for inner widgets
        if(myTopTitleWidget)
            myTopTitleWidget->setFont(f);
        if(myBottomTitleWidget)
            myBottomTitleWidget->setFont(f);
        if(myBottomLeftCornerWidget)
            myBottomLeftCornerWidget->setFont(f);
        if(myBottomRightCornerWidget)
            myBottomRightCornerWidget->setFont(f);
        if(myLeftTitleWidget)
            myLeftTitleWidget->setFont(f);
        if(myRightTitleWidget)
            myRightTitleWidget->setFont(f);
        if(myTopLeftCornerWidget)
            myTopLeftCornerWidget->setFont(f);
        if(myTopRightCornerWidget)
            myTopRightCornerWidget->setFont(f);

        break;
    }

     //case QEvent::Paint:  // ### TODO:
     //    QTimer::singleShot(1, verticalScrollBar(), SLOT(update()));
     //    QTimer::singleShot(1, horizontalScrollBar(), SLOT(update()));
     //    break;

    default:
        break;
    }

    return QFrame::event(e);
}

bool QicsTable::eventFilter(QObject* sender,QEvent* e)
{
    Q_UNUSED(sender);
    Q_UNUSED(e);
    return false;
}

void QicsTable::focusNextPrevGrid(const QicsScreenGrid* current_grid, bool next)
{
    for(int i = 0 ;i< 3; ++i) {
        for(int j=0;j<3; ++j) {
            if(myGrids[i][j] == current_grid) {
                if(next) {
                    int m,n = i+1;
                    for(m= j;m<3; ++m) {
                        for(; n<3; ++n) {
                            if(myGrids[n][m] != 0) {
                                myGrids[n][m]->setFocus();
                                myGrids[n][m]->traverseToBeginningOfTable();
                                return;
                            }
                        }
                        n = 0;
                    }
                }
                else {
                    int m,n = i-1;
                    for(m = j;m>=0; --m) {
                        for(; n>=0; --n) {
                            if(myGrids[n][m] != 0) {
                                myGrids[n][m]->setFocus();
                                myGrids[n][m]->traverseToEndOfColumn();
                                myGrids[n][m]->traverseToEndOfRow();
                                return;
                            }
                        }
                        n = 2;
                    }
                }
            }
        }
    }
    if(!currentCell()->displayer()->editWhenCurrent())
        const_cast<QicsScreenGrid*>(current_grid)->uneditCurrentCell();
}

bool QicsTable::setMouseButton(Qt::MouseButton realButton,
                          Qt::Modifier mod,
                          Qt::MouseButton emulButton)
{
    bool retval = false;
    if (m_tableCommon->setMouseButton(realButton, mod, emulButton))
        retval = true;
    return retval;
}

void QicsTable::registerDisplayFactory(const QString& displayerName, QicsCellDisplayFactory* factory)
{
    displayFactories.insert(displayerName, factory);
}

QStringList QicsTable::registeredCellDisplays()
{
    if( displayFactories.count() < 1 )
        registerBuiltInCellDisplays();

    QStringList list;
    QString name;

    foreach(name, displayFactories.keys())
        list << name;

    return list;
}

QicsCellDisplay *QicsTable::createCellDisplay(const QString& displayerName)
{
    if( displayFactories.count() < 1 )
        registerBuiltInCellDisplays();

    QicsCellDisplayFactory* factory = displayFactories.value(displayerName);
    if( factory ) {
        QicsCellDisplay* disp = factory->createCellDisplay();
        createdDisplays.append(disp);
        return disp;
    }
    else
        return 0;
}

QicsCellDisplayConfigurationWidget *QicsTable::createCellDisplayConfigurationWidget(const QString& displayerName)
{
    if( displayFactories.count() < 1 )
        registerBuiltInCellDisplays();

    QicsCellDisplayFactory* factory = displayFactories.value(displayerName);
    if( factory )
        return factory->configurationWidget();
    else
        return 0;
}

void QicsTable::registerBuiltInCellDisplays()
{
    registerDisplayFactory(QicsTextCellDisplay::TextCellDisplayName,
        new QicsTextCellDisplayFactory);

    registerDisplayFactory(QicsCheckCellDisplay::CheckCellDisplayName,
        new QicsCheckCellDisplayFactory);

    registerDisplayFactory(QicsComboCellDisplay::ComboCellDisplayName,
        new QicsComboCellDisplayFactory);

    registerDisplayFactory(QicsDateTimeCellDisplay::DateTimeCellDisplayName,
        new QicsDateTimeCellDisplayFactory);

    registerDisplayFactory(QicsPushButtonCellDisplay::PushButtonCellDisplayName,
        new QicsPushButtonCellDisplayFactory);
}

/////////////////////////////////////////////////////////////////////////
// Filtering methods
/////////////////////////////////////////////////////////////////////////

void QicsTable::setRowFilter(int column, QicsAbstractFilterDelegate *filter, bool deleteOld)
{
    setRepaintBehavior(Qics::RepaintOff);

    gridInfo().rowFilter()->setFilter(column, filter, deleteOld);

    setRepaintBehavior(Qics::RepaintOn);
    repaint();
}

void QicsTable::removeRowFilter(int column, bool deleteOld)
{
    setRepaintBehavior(Qics::RepaintOff);

    gridInfo().rowFilter()->removeFilter(column, deleteOld);

    setRepaintBehavior(Qics::RepaintOn);
    repaint();
}

void QicsTable::removeAllRowFilters()
{
    setRepaintBehavior(Qics::RepaintOff);

    gridInfo().rowFilter()->removeAll();

    setRepaintBehavior(Qics::RepaintOn);
    repaint();
}

QStringList QicsTable::uniqueKeysForColumn(int mcol, bool noEmpty) const
{
    QicsDataModel *dm = dataModel();
    if (!dm) return QStringList();

    if (mcol < 0 || mcol >= dm->numColumns()) return QStringList();

    QSet<QString> set;

    const int numRows = dm->numRows();
    for (int i = 0; i < numRows; ++i) {
        QString s = dm->itemString(i, mcol);
        if (s.isEmpty() && noEmpty) continue;
        set.insert(s);
    }

    return set.toList();
}

bool QicsTable::hasHiddenRows() const
{
    return gridInfo().mappedDM()->hasHiddenRows();
}

bool QicsTable::hasHiddenColumns() const
{
    return gridInfo().mappedDM()->hasHiddenColumns();
}

void QicsTable::setAutoFitMode(Qics::QicsAutoFitMode mode)
{
    m_tableCommon->setAutoFitMode(mode);
}

Qics::QicsAutoFitMode QicsTable::autoFitMode() const
{
    return m_tableCommon->autoFitMode();
}

void QicsTable::aboutQicsTable()
{
    QString textAboutQicsTable = tr(
        "<h3>About QicsTable</h3>"
        "%1<p>QicsTable is a full-featured, high-performance, "
        "Qt-based table widget suited for use in industrial-strength applications. "
        "It incorporates a model-view-controller architecture to allow multiple views"
        " of the same data. The table also produces high-quality printed output. "
        "It provides unparalleled performance, salability, and flexibility.</p>"
        "<p>QicsTable is a product of "
        "<a href=\"http://www.ics.com/\">Integrated Computer Solutions</a></p>")
       .arg(QMessageBox::tr("<p>This program uses QicsTable version %1.</p>"
                            "<>Built on %2 at %3</p>"))
       .arg(QLatin1String(QICSTABLE_VERSION_STR))
       .arg(QLatin1String(__DATE__))
       .arg(QLatin1String(__TIME__));

    QMessageBox mb(QApplication::activeWindow());
    mb.setWindowTitle(tr("About QicsTable"));
    mb.setText(textAboutQicsTable);

    QImage logo(qicstable_logo_xpm);

    if (qGray(mb.palette().color(QPalette::Active, QPalette::Text).rgba()) >
        qGray(mb.palette().color(QPalette::Active, QPalette::Base).rgba())) {
        // light on dark, adjust some colors
        logo.setColor(0, 0xffffffff);
        logo.setColor(1, 0xff666666);
        logo.setColor(2, 0xffcccc66);
        logo.setColor(4, 0xffcccccc);
        logo.setColor(6, 0xffffff66);
        logo.setColor(7, 0xff999999);
        logo.setColor(8, 0xff3333ff);
        logo.setColor(9, 0xffffff33);
        logo.setColor(11, 0xffcccc99);
    }

    QPixmap pm = QPixmap::fromImage(logo);
    if (!pm.isNull())
        mb.setIconPixmap(pm);

    mb.addButton(QMessageBox::Ok);
    mb.exec();
}

/*void QicsTable::paintEvent(QPaintEvent *e)
{
        QFrame::paintEvent(e);

    if (frozenLineWidth() == 0)
        return;

    QPainter painter;
    painter.begin(this);
    painter.setPen(QPen(QBrush(frozenLineColor()), frozenLineWidth()));

    QList<QicsTableGrid *> tables;

    if (numFreezeTopRows()) {

        tables.clear();
        tables.append(TOP_LEFT_GRID);
        tables.append(TOP_MIDDLE_GRID);
        tables.append(TOP_RIGHT_GRID);

        foreach(QicsTableGrid *table, tables) {
            if (!table)
                continue;

            const QRect &geom = table->geometry();
            const QicsMappedDimensionManager *md = table->gridInfo().mappedDM();

            QPointF p1 = geom.bottomLeft() + QPointF(0, tableSpacing() / 2.0);
            QPointF p2 = p1 + QPoint(md->regionWidth(table->screenViewport()) - frozenLineWidth() + 2, 0);

            painter.save();
            painter.translate(frozenLineWidth() / 2, frozenLineWidth() / 2  + 2);
            painter.drawLine(p1, p2);
            painter.restore();
        }
    }

    if (numFreezeBottomRows()) {

        tables.clear();
        tables.append(BOTTOM_LEFT_GRID);
        tables.append(BOTTOM_MIDDLE_GRID);
        tables.append(BOTTOM_RIGHT_GRID);

        foreach(QicsTableGrid *table, tables) {
            if (!table)
                continue;

            const QRect &geom = table->geometry();
            const QicsMappedDimensionManager *md = table->gridInfo().mappedDM();

            QPointF p1 = geom.topLeft() - QPointF(0, frozenLineWidth() + tableSpacing() / 2.0 - 1);
            QPointF p2 = p1 + QPoint(md->regionWidth(table->screenViewport()) - frozenLineWidth() + 2, 0);

            painter.save();
            painter.translate(frozenLineWidth() / 2, frozenLineWidth() / 2 - 2);
            painter.drawLine(p1, p2);
            painter.restore();
        }
    }

    if (numFreezeLeftColumns()) {

        tables.clear();
        tables.append(TOP_LEFT_GRID);
        tables.append(MIDDLE_LEFT_GRID);
        tables.append(BOTTOM_LEFT_GRID);

        foreach(QicsTableGrid *table, tables) {
            if (!table)
                continue;

            const QRect &geom = table->geometry();
            const QicsMappedDimensionManager *md = table->gridInfo().mappedDM();

            QPointF p1 = geom.topRight() + QPointF(tableSpacing() / 2.0, 0);
            QPointF p2 = p1 + QPointF(0, md->regionHeight(table->screenViewport())- frozenLineWidth() + 2);

            painter.save();
            painter.translate(frozenLineWidth() / 2 + 2, frozenLineWidth() / 2);
            painter.drawLine(p1, p2);
            painter.restore();
        }
    }

    if (numFreezeRightColumns()) {

        tables.clear();
        tables.append(TOP_RIGHT_GRID);
        tables.append(MIDDLE_RIGHT_GRID);
        tables.append(BOTTOM_RIGHT_GRID);

        foreach(QicsTableGrid *table, tables) {
            if (!table)
                continue;

            const QRect &geom = table->geometry();
            const QicsMappedDimensionManager *md = table->gridInfo().mappedDM();

            QPointF p1 = geom.topLeft() - QPointF(frozenLineWidth() + tableSpacing() / 2.0 - 1, 0);
            QPointF p2 = p1 + QPointF(0, md->regionHeight(table->screenViewport()) - frozenLineWidth() + 2);

            painter.save();
            painter.translate(frozenLineWidth() / 2 - 2, frozenLineWidth() / 2);
            painter.drawLine(p1, p2);
            painter.restore();
        }
    }

    painter.end();
}*/


