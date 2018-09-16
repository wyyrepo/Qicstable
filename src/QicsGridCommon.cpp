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

#include "QicsGridCommon.h"

#include "QicsStyleManager.h"
#include "QicsSpanManager.h"
#include "QicsDimensionManager.h"
#include "QicsScreenGrid.h"
#include "QicsMouseMap.h"



QicsGridCommon::QicsGridCommon(QicsGridInfo *info, QObject *parent,
                               bool forward_signals)
    : QicsCellCommon(info, parent),
    myForwardSignals(forward_signals),
    m_autoFitMode(Qics::Native)
{
}

QicsGridCommon::QicsGridCommon(QObject *parent, bool forward_signals)
    : QicsCellCommon(parent),
    myForwardSignals(forward_signals),
    m_autoFitMode(Qics::Native)
{
}

QicsGridCommon::~QicsGridCommon()
{
    m_info = 0;
    myFollowModel = false;
}

void QicsGridCommon::setAttr(QicsCellStyle::QicsCellStyleProperty attr, const void *val)
{
    styleManager().setDefaultProperty(attr, val);
}

void *QicsGridCommon::getAttr(QicsCellStyle::QicsCellStyleProperty attr) const
{
    return (styleManager().getDefaultProperty(attr));
}

void QicsGridCommon::clearAttr(QicsCellStyle::QicsCellStyleProperty attr)
{
    styleManager().setDefaultProperty(attr, static_cast<void *> (0));
}

void QicsGridCommon::setGridAttr(QicsGridStyle::QicsGridStyleProperty attr, const void *val)
{
    styleManager().setGridProperty(attr, val);
}

void * QicsGridCommon::getGridAttr(QicsGridStyle::QicsGridStyleProperty attr) const
{
    return (styleManager().getGridProperty(attr));
}

void QicsGridCommon::clearGridAttr(QicsGridStyle::QicsGridStyleProperty attr)
{
    styleManager().setGridProperty(attr, static_cast<void *> (0));
}

void QicsGridCommon::initSignals()
{
    if (!myForwardSignals)
        return;

    if (m_info) {
        QicsGridInfo::QicsScreenGridPV grids = m_info->grids();
        QicsGridInfo::QicsScreenGridPV::const_iterator iter, iter_end(grids.constEnd());

        for (iter = grids.constBegin(); iter != iter_end; ++iter)
            connectGrid(*iter);

        connect(m_info, SIGNAL(gridAdded(QicsScreenGrid *)),
            this, SLOT(connectGrid(QicsScreenGrid *)));
        connect(m_info, SIGNAL(gridRemoved(QicsScreenGrid *)),
            this, SLOT(disconnectGrid(QicsScreenGrid *)));
        connect(m_info, SIGNAL(cellValueChanged(int, int)),
            this, SIGNAL(valueChanged(int, int)));
    }
}

void QicsGridCommon::setInfo(QicsGridInfo *info)
{
    if (!info || info == m_info)
        return;

    QicsCellCommon::setInfo(info);

    if (m_info)
        disconnect(m_info, 0, this, 0);

    if (m_info && myForwardSignals) {
        QicsGridInfo::QicsScreenGridPV grids = m_info->grids();
        QicsGridInfo::QicsScreenGridPV::const_iterator iter, iter_end(grids.constEnd());

        for (iter = grids.constBegin(); iter != iter_end; ++iter)
            disconnectGrid(*iter);
    }

    initSignals();
}

void QicsGridCommon::connectGrid(QicsScreenGrid *grid)
{
    connect(grid, SIGNAL(pressed(int, int, int, const QPoint &)),
        this, SLOT(handleGridPress(int, int, int, const QPoint &)));
    connect(grid, SIGNAL(clicked(int, int, int, const QPoint &)),
        this, SLOT(handleGridClick(int, int, int, const QPoint &)));
    connect(grid, SIGNAL(doubleClicked(int, int, int, const QPoint &)),
        this, SLOT(handleGridDoubleClick(int, int, int, const QPoint &)));
}

void QicsGridCommon::disconnectGrid(QicsScreenGrid *grid)
{
    disconnect(grid, 0, this, 0);
}

void QicsGridCommon::handleGridPress(int row, int col, int button, const QPoint &pos)
{
    if (!m_info)
        return;

    const QicsScreenGrid *grid = 0;
    const QObject *s = sender();

    QicsGridInfo::QicsScreenGridPV grids = m_info->grids();
    QicsGridInfo::QicsScreenGridPV::const_iterator iter, iter_end(grids.constEnd());

    for (iter = grids.constBegin(); iter != iter_end; ++iter)
        if (*iter == s) {
            grid = *iter;
            break;
        }

    if (grid)
        emit pressed(row, col, button, grid->mapToParent(pos));
}

void QicsGridCommon::handleGridClick(int row, int col, int button, const QPoint &pos)
{
    if (!m_info)
        return;

    QicsScreenGrid *grid = 0;
    const QObject *s = sender();

    QicsGridInfo::QicsScreenGridPV grids = m_info->grids();
    QicsGridInfo::QicsScreenGridPV::const_iterator iter, iter_end(grids.constEnd());

    for (iter = grids.constBegin(); iter != iter_end; ++iter)
        if (*iter == s) {
            grid = *iter;
            break;
        }

    if (grid)
        emit clicked(row, col, button, grid->mapToParent(pos));
}

void QicsGridCommon::handleGridDoubleClick(int row, int col, int button,
                                      const QPoint &pos)
{
    if (!m_info)
        return;

    QicsScreenGrid *grid = 0;
    const QObject *s = sender();

    QicsGridInfo::QicsScreenGridPV grids = m_info->grids();
    QicsGridInfo::QicsScreenGridPV::const_iterator iter, iter_end(grids.constEnd());

    for (iter = grids.constBegin(); iter != iter_end; ++iter)
        if (*iter == s) {
            grid = *iter;
            break;
        }

    if (grid)
        emit doubleClicked(row, col, button, grid->mapToParent(pos));
}

void QicsGridCommon::setDMMargin(int margin)
{
    dimensionManager().setDefaultMargin(margin);
}

void QicsGridCommon::setDMFont(const QFont &font)
{
    dimensionManager().setDefaultFont(font);
}

void QicsGridCommon::setDMBorderWidth(int bw)
{
    dimensionManager().setDefaultBorderWidth(bw);
}

QicsRegion QicsGridCommon::viewport() const
{
    return (* static_cast<QicsRegion *>
        (getGridAttr(QicsGridStyle::Viewport)));
}

void QicsGridCommon::setViewport(const QicsRegion &vp)
{
    setGridAttr(QicsGridStyle::Viewport, static_cast<const void *> (&vp));
}

bool QicsGridCommon::addCellSpan(QicsSpan span, bool visual)
{
    if (visual) {
        QicsSpan modelSpan(m_info->modelRowIndex(span.row()), m_info->modelColumnIndex(span.column()),
            span.height(), span.width());
        return styleManager().spanManager()->addCellSpan(modelSpan);
    }

    return styleManager().spanManager()->addCellSpan(span);
}

void QicsGridCommon::removeCellSpan(int start_row, int start_col, bool visual)
{
    styleManager().spanManager()->removeCellSpan(
        visual ? m_info->modelRowIndex(start_row) : start_row,
        visual ? m_info->modelColumnIndex(start_col) : start_col);
}

QicsSpanList *QicsGridCommon::cellSpanList()
{
    return styleManager().spanManager()->cellSpanList();
}

Qics::QicsGridCellClipping QicsGridCommon::gridCellClipping() const
{
    return (* static_cast<QicsGridCellClipping *>
        (getGridAttr(QicsGridStyle::GridCellClipping)));
}

void QicsGridCommon::setGridCellClipping(QicsGridCellClipping c)
{
    setGridAttr(QicsGridStyle::GridCellClipping, static_cast<const void *> (&c));
}

bool QicsGridCommon::drawPartialCells() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::DrawPartialCells)));
}

void QicsGridCommon::setDrawPartialCells(bool b)
{
    setGridAttr(QicsGridStyle::DrawPartialCells, static_cast<const void *> (&b));
}

bool QicsGridCommon::horizontalGridLinesVisible() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::HorizontalGridLinesVisible)));
}

void QicsGridCommon::setHorizontalGridLinesVisible(bool b)
{
    setGridAttr(QicsGridStyle::HorizontalGridLinesVisible,
        static_cast<const void *> (&b));
}

bool QicsGridCommon::verticalGridLinesVisible() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::VerticalGridLinesVisible)));
}

void QicsGridCommon::setVerticalGridLinesVisible(bool b)
{
    setGridAttr(QicsGridStyle::VerticalGridLinesVisible,
        static_cast<const void *> (&b));
}

bool QicsGridCommon::fillHandleVisible() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::FillHandleVisible)));
}

void QicsGridCommon::setFillHandleVisible(bool b)
{
    setGridAttr(QicsGridStyle::FillHandleVisible,
        static_cast<const void *> (&b));
}

int QicsGridCommon::horizontalGridLineWidth() const
{
    if (!horizontalGridLinesVisible()) return 0;
    return (* static_cast<int *>(getGridAttr(QicsGridStyle::HorizontalGridLineWidth)));
}

void QicsGridCommon::setHorizontalGridLineWidth(int w)
{
    setGridAttr(QicsGridStyle::HorizontalGridLineWidth,
        static_cast<const void *> (&w));
}

int QicsGridCommon::verticalGridLineWidth() const
{
    if (!verticalGridLinesVisible()) return 0;
    return  (* static_cast<int *>(getGridAttr(QicsGridStyle::VerticalGridLineWidth)));
}

void QicsGridCommon::setVerticalGridLineWidth(int w)
{
    setGridAttr(QicsGridStyle::VerticalGridLineWidth,
        static_cast<const void *> (&w));
}

Qics::QicsLineStyle QicsGridCommon::horizontalGridLineStyle() const
{
    return (* static_cast<QicsLineStyle *>
        (getGridAttr(QicsGridStyle::HorizontalGridLineStyle)));
}

void QicsGridCommon::setHorizontalGridLineStyle(QicsLineStyle style)
{
    setGridAttr(QicsGridStyle::HorizontalGridLineStyle,
        static_cast<const void *> (&style));
}

Qics::QicsLineStyle QicsGridCommon::verticalGridLineStyle() const
{
    return (* static_cast<QicsLineStyle *>
        (getGridAttr(QicsGridStyle::VerticalGridLineStyle)));
}

void QicsGridCommon::setVerticalGridLineStyle(QicsLineStyle style)
{
    setGridAttr(QicsGridStyle::VerticalGridLineStyle,
        static_cast<const void *> (&style));
}

int QicsGridCommon::horizontalShadeLineWidth() const
{
    const QicsLineStyle hls = horizontalGridLineStyle();
    if (hls == Qics::Raised || hls == Qics::Sunken)
        return 2 * horizontalGridLineWidth();

    return 0;
}

int QicsGridCommon::verticalShadeLineWidth() const
{
    const QicsLineStyle vls = verticalGridLineStyle();
    if (vls == Qics::Raised || vls == Qics::Sunken)
        return 2 * verticalGridLineWidth();

    return 0;
}

QPen QicsGridCommon::horizontalGridLinePen() const
{
    return (* static_cast<QPen *>
        (getGridAttr(QicsGridStyle::HorizontalGridLinePen)));
}

void QicsGridCommon::setHorizontalGridLinePen(const QPen &pen)
{
    setGridAttr(QicsGridStyle::HorizontalGridLinePen,
        static_cast<const void *> (&pen));
}

QPen QicsGridCommon::verticalGridLinePen() const
{
    return (* static_cast<QPen *>
        (getGridAttr(QicsGridStyle::VerticalGridLinePen)));
}

void QicsGridCommon::setVerticalGridLinePen(const QPen &pen)
{
    setGridAttr(QicsGridStyle::VerticalGridLinePen,
        static_cast<const void *> (&pen));
}

Qics::QicsCellOverflowBehavior QicsGridCommon::cellOverflowBehavior() const
{
    return (* static_cast<QicsCellOverflowBehavior *>
        (getGridAttr(QicsGridStyle::CellOverflowBehavior)));
}

void QicsGridCommon::setCellOverflowBehavior(QicsCellOverflowBehavior b)
{
    setGridAttr(QicsGridStyle::CellOverflowBehavior,
        static_cast<const void *> (&b));
}

QPoint QicsGridCommon::cellToolTipOffset() const
{
    return (* static_cast<QPoint *>
        (getGridAttr(QicsGridStyle::CellToolTipOffset)));
}

void QicsGridCommon::setCellToolTipOffset(const QPoint &pos)
{
    setGridAttr(QicsGridStyle::CellToolTipOffset,
        static_cast<const void *> (&pos));
}

int QicsGridCommon::maxOverflowCells() const
{
    return (* static_cast<int *>
        (getGridAttr(QicsGridStyle::MaxOverflowCells)));
}

void QicsGridCommon::setMaxOverflowCells(int num)
{
    setGridAttr(QicsGridStyle::MaxOverflowCells, static_cast<const void *> (&num));
}

int QicsGridCommon::frameLineWidth() const
{
    return (* static_cast<int *>
        (getGridAttr(QicsGridStyle::FrameLineWidth)));
}

void QicsGridCommon::setFrameLineWidth(int lw)
{
    setGridAttr(QicsGridStyle::FrameLineWidth, static_cast<const void *> (&lw));
}

int QicsGridCommon::frameStyle() const
{
    return (* static_cast<int *>
        (getGridAttr(QicsGridStyle::FrameStyle)));
}

void QicsGridCommon::setFrameStyle(int style)
{
    setGridAttr(QicsGridStyle::FrameStyle, static_cast<const void *> (&style));
}

Qics::QicsCurrentCellStyle QicsGridCommon::currentCellStyle() const
{
    return (* static_cast<QicsCurrentCellStyle *>
        (getGridAttr(QicsGridStyle::CurrentCellStyle)));
}

void QicsGridCommon::setCurrentCellStyle(QicsCurrentCellStyle s)
{
    setGridAttr(QicsGridStyle::CurrentCellStyle,
        static_cast<const void *> (&s));
}

Qics::QicsSelectionStyle QicsGridCommon::selectionStyle() const
{
    return (* static_cast<QicsSelectionStyle *>
        (getGridAttr(QicsGridStyle::SelectionStyle)));
}

void QicsGridCommon::setSelectionStyle(QicsSelectionStyle s)
{
    setGridAttr(QicsGridStyle::SelectionStyle,
        static_cast<const void *> (&s));
}

bool QicsGridCommon::highlightSelection() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::HighlightSelection)));
}

void QicsGridCommon::setHighlightSelection(bool s)
{
    setGridAttr(QicsGridStyle::HighlightSelection,
        static_cast<const void *> (&s));
}

int QicsGridCommon::currentCellBorderWidth() const
{
    return (* static_cast<int *>
        (getGridAttr(QicsGridStyle::CurrentCellBorderWidth)));
}

void QicsGridCommon::setCurrentCellBorderWidth(int w)
{
    setGridAttr(QicsGridStyle::CurrentCellBorderWidth,
        static_cast<const void *> (&w));
}

bool QicsGridCommon::clickToEdit() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::ClickToEdit)));
}

void QicsGridCommon::setClickToEdit(bool b)
{
    setGridAttr(QicsGridStyle::ClickToEdit, static_cast<const void *> (&b));
}

bool QicsGridCommon::autoSelectCellContents() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::AutoSelectCellContents)));
}

void QicsGridCommon::setAutoSelectCellContents(bool b)
{
    setGridAttr(QicsGridStyle::AutoSelectCellContents,
        static_cast<const void *> (&b));
}

Qics::QicsTraversalKeyEditBehavior QicsGridCommon::traversalKeyEditBehavior() const
{
    return (* static_cast<QicsTraversalKeyEditBehavior *>
        (getGridAttr(QicsGridStyle::TraversalKeyEditBehavior)));
}

void QicsGridCommon::setTraversalKeyEditBehavior(QicsTraversalKeyEditBehavior beh)
{
    setGridAttr(QicsGridStyle::TraversalKeyEditBehavior,
        static_cast<const void *> (&beh));
}

Qt::Orientation QicsGridCommon::enterTraversalDirection() const
{
    return (* static_cast<Qt::Orientation *>
        (getGridAttr(QicsGridStyle::EnterTraversalDirection)));
}

void QicsGridCommon::setEnterTraversalDirection(Qt::Orientation dir)
{
    setGridAttr(QicsGridStyle::EnterTraversalDirection,
        static_cast<const void *> (&dir));
}

Qt::Orientation QicsGridCommon::tabTraversalDirection() const
{
    return (* static_cast<Qt::Orientation *>
        (getGridAttr(QicsGridStyle::TabTraversalDirection)));
}

void QicsGridCommon::setTabTraversalDirection(Qt::Orientation dir)
{
    setGridAttr(QicsGridStyle::TabTraversalDirection,
        static_cast<const void *> (&dir));
}

void QicsGridCommon::setTabTraverseToBegin(bool traverse)
{
    setGridAttr(QicsGridStyle::TabTraverseToBegin,
        static_cast<const void *> (&traverse));
}

bool QicsGridCommon::tabTraverseToBegin() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::TabTraverseToBegin)));
}

void QicsGridCommon::setEnterTraverseToBegin(bool traverse)
{
    setGridAttr(QicsGridStyle::EnterTraverseToBegin,
        static_cast<const void *> (&traverse));
}

bool QicsGridCommon::enterTraverseToBegin() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::EnterTraverseToBegin)));
}

QPixmap QicsGridCommon::moreTextPixmap() const
{
    QPixmap *pix = static_cast<QPixmap *>
        (getGridAttr(QicsGridStyle::MoreTextPixmap));

    if (pix)
        return (*pix);

    return QPixmap();
}

void QicsGridCommon::setMoreTextPixmap(const QPixmap &pix)
{
    const QPixmap *val;

    if (pix.isNull())
        val = 0;
    else
        val = &pix;

    setGridAttr(QicsGridStyle::MoreTextPixmap, static_cast<const void *> (val));
}

QPalette QicsGridCommon::gridPalette() const
{
    return (* static_cast<QPalette *>
        (getGridAttr(QicsGridStyle::GridPalette)));
}

void QicsGridCommon::setGridPalette(const QPalette &pal)
{
    setGridAttr(QicsGridStyle::GridPalette, static_cast<const void *> (&pal));
}

bool QicsGridCommon::dragEnabled() const
{
    return (* static_cast<bool *>
        (getGridAttr(QicsGridStyle::DragEnabled)));
}

void QicsGridCommon::setDragEnabled(bool b)
{
    setGridAttr(QicsGridStyle::DragEnabled, static_cast<const void *> (&b));
}

Qt::LayoutDirection QicsGridCommon::layoutDirection() const
{
    return (* static_cast<Qt::LayoutDirection *>
        (getGridAttr(QicsGridStyle::LayoutDirection)));
}

void QicsGridCommon::setFakeCellsDrawingIndex(Qics::QicsIndexType type)
{
    setGridAttr(QicsGridStyle::FakeCellsDrawingIndex, static_cast<const void *> (&type));
}

Qics::QicsIndexType QicsGridCommon::fakeCellsDrawingIndex() const
{
    return (* static_cast<Qics::QicsIndexType *>(getGridAttr(QicsGridStyle::FakeCellsDrawingIndex)));
}

void QicsGridCommon::setLayoutDirection(Qt::LayoutDirection layoutDirection)
{
    setGridAttr(QicsGridStyle::LayoutDirection, static_cast <const void *>(&layoutDirection));
}

Qics::QicsCurrentCellDraggingPolicy QicsGridCommon::currentCellDraggingPolicy()const
{
    return (* static_cast<Qics::QicsCurrentCellDraggingPolicy *>
        (getGridAttr(QicsGridStyle::CurrentCellDraggingPolicy)));
}

void QicsGridCommon::setCurrentCellDraggingPolicy(Qics::QicsCurrentCellDraggingPolicy policy)
{
    setGridAttr(QicsGridStyle::CurrentCellDraggingPolicy, static_cast<const void *> (&policy));
}

Qics::QicsCellDecorationStyle QicsGridCommon::cellDecorationStyle() const
{
    return (* static_cast<Qics::QicsCellDecorationStyle *>
        (getGridAttr(QicsGridStyle::CellDecorationStyle)));
}

void QicsGridCommon::setCellDecorationStyle(Qics::QicsCellDecorationStyle s)
{
    setGridAttr(QicsGridStyle::CellDecorationStyle, static_cast<const void *> (&s));
}

int QicsGridCommon::exclusiveSelectionBorderWidth() const
{
    return (* static_cast<int *>
        (getGridAttr(QicsGridStyle::ExclusiveSelectionBorderWidth)));
}

void QicsGridCommon::setExclusiveSelectionBorderWidth(int lw)
{
    setGridAttr(QicsGridStyle::ExclusiveSelectionBorderWidth, static_cast<const void *> (&lw));
}

QColor QicsGridCommon::exclusiveSelectionBorderColor() const
{
    return (* static_cast<QColor *>
        (getGridAttr(QicsGridStyle::ExclusiveSelectionBorderColor)));
}

void QicsGridCommon::setExclusiveSelectionBorderColor(const QColor &c)
{
    setGridAttr(QicsGridStyle::ExclusiveSelectionBorderColor, static_cast<const void *> (&c));
}

QColor QicsGridCommon::exclusiveSelectionHandleColor() const
{
    return (* static_cast<QColor *>
        (getGridAttr(QicsGridStyle::ExclusiveSelectionHandleColor)));
}

void QicsGridCommon::setExclusiveSelectionHandleColor(const QColor &c)
{
    setGridAttr(QicsGridStyle::ExclusiveSelectionHandleColor, static_cast<const void *> (&c));
}

QColor QicsGridCommon::exclusiveSelectionHandleBackColor() const
{
    return (* static_cast<QColor *>
        (getGridAttr(QicsGridStyle::ExclusiveSelectionHandleBackColor)));
}

void QicsGridCommon::setExclusiveSelectionHandleBackColor(const QColor &c)
{
    setGridAttr(QicsGridStyle::ExclusiveSelectionHandleBackColor, static_cast<const void *> (&c));
}

QPen QicsGridCommon::exclusiveSelectionDragPen() const
{
    return (* static_cast<QPen *>
        (getGridAttr(QicsGridStyle::ExclusiveSelectionDragPen)));
}

void QicsGridCommon::setExclusiveSelectionDragPen(const QPen &p)
{
    setGridAttr(QicsGridStyle::ExclusiveSelectionDragPen, static_cast<const void *> (&p));
}

QColor QicsGridCommon::exclusiveSelectionDragBackColor() const
{
    return (* static_cast<QColor *>
        (getGridAttr(QicsGridStyle::ExclusiveSelectionDragBackColor)));
}

void QicsGridCommon::setExclusiveSelectionDragBackColor(const QColor &c)
{
    setGridAttr(QicsGridStyle::ExclusiveSelectionDragBackColor, static_cast<const void *> (&c));
}

QicsRubberBand::DrawingMode QicsGridCommon::exclusiveSelectionDragMode() const
{
    return (* static_cast<QicsRubberBand::DrawingMode *>
        (getGridAttr(QicsGridStyle::ExclusiveSelectionDragMode)));
}

void QicsGridCommon::setExclusiveSelectionDragMode(QicsRubberBand::DrawingMode lw)
{
    setGridAttr(QicsGridStyle::ExclusiveSelectionDragMode, static_cast<const void *> (&lw));
}

float QicsGridCommon::exclusiveSelectionDragOpacity() const
{
    return (* static_cast<float *>
        (getGridAttr(QicsGridStyle::ExclusiveSelectionDragOpacity)));
}

void QicsGridCommon::setExclusiveSelectionDragOpacity(float lw)
{
    setGridAttr(QicsGridStyle::ExclusiveSelectionDragOpacity, static_cast<const void *> (&lw));
}

/*void QicsGridCommon::gridDeleted()
{
    if (myInfo)
        disconnect(myInfo, 0, this, 0);
    myInfo = 0;
}*/

bool QicsGridCommon::setMouseButton( Qt::MouseButton realButton,
                                    Qt::Modifier buttonModifier, Qt::MouseButton emulButton)
{
    QicsMouseMap * myMMap = ( static_cast<QicsMouseMap *>
        (getGridAttr(QicsGridStyle::MapMouseButton)));

    if (myMMap) {
        myMMap->setMapMButton( realButton, buttonModifier, emulButton );
        return true;
    }

    return false;
}

QicsMouseMap *QicsGridCommon::mouseButtonRef() const
{
    return (static_cast<QicsMouseMap *>
        (getGridAttr(QicsGridStyle::MapMouseButton)));
}


