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

#include "QicsDimensionManager.h"

#include "QicsUtil.h"
#include "QicsFilter.h"
#include "QicsStyleManager.h"
#include "QicsDataModel.h"
#include "QicsCell.h"


QicsDimensionManager::QicsCellSetting::QicsCellSetting()
    : row(-1), col(-1), font_height(0), font_width(0),
        cell_margin(0), border_width(0)
{
}

bool QicsDimensionManager::QicsCellSetting::isEmpty() const
{
    return ((font_height == 0) && (font_width == 0) &&
        (cell_margin == 0) && (border_width == 0));
}

QDomElement QicsDimensionManager::QicsCellSetting::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = doc->createElement(tag);
    QString attrName;
    QString attrValue;

    attrName = "role";
    attrValue = QString::number(role);
    e.setAttribute(attrName, attrValue);

    attrName = "row";
    attrValue = QString::number(row);
    e.setAttribute(attrName, attrValue);

    attrName = "col";
    attrValue = QString::number(col);
    e.setAttribute(attrName, attrValue);

    attrName = "font_height";
    attrValue = QString::number(font_height);
    e.setAttribute(attrName, attrValue);

    attrName = "font_width";
    attrValue = QString::number(font_width);
    e.setAttribute(attrName, attrValue);

    attrName = "cellMargin";
    attrValue = QString::number(cell_margin);
    e.setAttribute(attrName, attrValue);

    attrName = "borderWidth";
    attrValue = QString::number(border_width);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsCellSetting::configureFromDomXml(const QDomElement& e)
{
    role = (QicsGridRole) e.attribute("role").toInt();
    row = e.attribute("row").toInt();
    col = e.attribute("col").toInt();
    font_height = e.attribute("font_height").toInt();
    font_width = e.attribute("font_width").toInt();
    cell_margin = e.attribute("cell_margin").toInt();
    border_width = e.attribute("border_width").toInt();
}

//////////////////////////////////////////////////////////////////////////////

QicsDimensionManager::QicsRowSetting::QicsRowSetting()
    : row(-1), font_height(0), font_width(0),
        cell_margin(0), border_width(0), stretchable(false)
{
}

bool QicsDimensionManager::QicsRowSetting::isEmpty() const
{
    return ((font_height == 0) && (font_width == 0) &&
        (cell_margin == 0) && (border_width == 0));
}

QDomElement QicsDimensionManager::QicsRowSetting::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = doc->createElement(tag);
    QString attrName;
    QString attrValue;

    attrName = "role";
    attrValue = QString::number(role);
    e.setAttribute(attrName, attrValue);

    attrName = "row";
    attrValue = QString::number(row);
    e.setAttribute(attrName, attrValue);

    attrName = "font_height";
    attrValue = QString::number(font_height);
    e.setAttribute(attrName, attrValue);

    attrName = "font_width";
    attrValue = QString::number(font_width);
    e.setAttribute(attrName, attrValue);

    attrName = "cellMargin";
    attrValue = QString::number(cell_margin);
    e.setAttribute(attrName, attrValue);

    attrName = "borderWidth";
    attrValue = QString::number(border_width);
    e.setAttribute(attrName, attrValue);

    attrName = "stretchable";
    attrValue = QString::number(stretchable);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsRowSetting::configureFromDomXml(const QDomElement& e)
{
    role = (QicsGridRole) e.attribute("role").toInt();
    row = e.attribute("row").toInt();
    font_height = e.attribute("font_height").toInt();
    font_width = e.attribute("font_width").toInt();
    cell_margin = e.attribute("cell_margin").toInt();
    border_width = e.attribute("border_width").toInt();
    stretchable = e.attribute("stretchable").toInt();

}

//////////////////////////////////////////////////////////////////////////////

QicsDimensionManager::QicsColumnSetting::QicsColumnSetting()
    : col(-1), font_height(0), font_width(0),
        cell_margin(0), border_width(0), stretchable(false)
{
}

bool QicsDimensionManager::QicsColumnSetting::isEmpty() const
{
    return ((font_height == 0) && (font_width == 0) &&
        (cell_margin == 0) && (border_width == 0));
}

QDomElement QicsDimensionManager::QicsColumnSetting::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = doc->createElement(tag);
    QString attrName;
    QString attrValue;

    attrName = "role";
    attrValue = QString::number(role);
    e.setAttribute(attrName, attrValue);

    attrName = "col";
    attrValue = QString::number(col);
    e.setAttribute(attrName, attrValue);

    attrName = "font_height";
    attrValue = QString::number(font_height);
    e.setAttribute(attrName, attrValue);

    attrName = "font_width";
    attrValue = QString::number(font_width);
    e.setAttribute(attrName, attrValue);

    attrName = "cellMargin";
    attrValue = QString::number(cell_margin);
    e.setAttribute(attrName, attrValue);

    attrName = "borderWidth";
    attrValue = QString::number(border_width);
    e.setAttribute(attrName, attrValue);

    attrName = "stretchable";
    attrValue = QString::number(stretchable);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsColumnSetting::configureFromDomXml(const QDomElement& e)
{
    role = (QicsGridRole) e.attribute("role").toInt();
    col = e.attribute("col").toInt();
    font_height = e.attribute("font_height").toInt();
    font_width = e.attribute("font_width").toInt();
    cell_margin = e.attribute("cell_margin").toInt();
    border_width = e.attribute("border_width").toInt();
    stretchable = e.attribute("stretchable").toInt();
}

//////////////////////////////////////////////////////////////////////////////

QicsDimensionManager::QicsRepeatingRowSetting::QicsRepeatingRowSetting()
    : QicsRowSetting(), interval(1)
{
}

QDomElement QicsDimensionManager::QicsRepeatingRowSetting::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = QicsRowSetting::toDomXml(tag,doc);

    QString attrName;
    QString attrValue;

    attrName = "interval";
    attrValue = QString::number(interval);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsRepeatingRowSetting::configureFromDomXml(const QDomElement& e)
{
    QicsRowSetting::configureFromDomXml(e);
    interval = e.attribute("interval").toInt();
}

//////////////////////////////////////////////////////////////////////////////

QicsDimensionManager::QicsRepeatingColumnSetting::QicsRepeatingColumnSetting()
    : QicsColumnSetting(), interval(1)
{
}

QDomElement QicsDimensionManager::QicsRepeatingColumnSetting::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = QicsColumnSetting::toDomXml(tag,doc);
    QString attrName;
    QString attrValue;

    attrName = "interval";
    attrValue = QString::number(interval);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsRepeatingColumnSetting::configureFromDomXml(const QDomElement& e)
{
    QicsColumnSetting::configureFromDomXml(e);
    interval = e.attribute("interval").toInt();
}

//////////////////////////////////////////////////////////////////////////////

QicsDimensionManager::QicsRowHeight::QicsRowHeight()
    : height(0), chars(0), valid(false)

{
    mode = QicsDimensionUnset;
}

QicsDimensionManager::QicsRowHeight::QicsRowHeight(const QicsRowHeight &rh)
    : height(rh.height), chars(rh.chars), valid(rh.valid)
{
    mode = rh.mode;
}

QDomElement QicsDimensionManager::QicsRowHeight::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = doc->createElement(tag);
    QString attrName;
    QString attrValue;

    attrName = "mode";
    attrValue = QString::number(mode);
    e.setAttribute(attrName, attrValue);

    attrName = "height";
    attrValue = QString::number(height);
    e.setAttribute(attrName, attrValue);

    attrName = "chars";
    attrValue = QString::number(chars);
    e.setAttribute(attrName, attrValue);

    attrName = "valid";
    attrValue = QString::number(valid);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsRowHeight::configureFromDomXml(const QDomElement& e)
{
    mode = (QicsDimensionMode) e.attribute("mode").toInt();
    height = e.attribute("height").toInt();
    chars = e.attribute("chars").toInt();
    valid = e.attribute("valid").toInt();
}

//////////////////////////////////////////////////////////////////////////////

QicsDimensionManager::QicsRepeatingRowHeight::QicsRepeatingRowHeight()
    : QicsRowHeight(), row(-1), interval(0)
{
}

QicsDimensionManager::QicsRepeatingRowHeight::QicsRepeatingRowHeight(const QicsRepeatingRowHeight &rh)
    : QicsRowHeight(rh), row(rh.row), interval(rh.interval)
{
}

QDomElement QicsDimensionManager::QicsRepeatingRowHeight::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = QicsRowHeight::toDomXml(tag,doc);
    QString attrName;
    QString attrValue;

    attrName = "start";
    attrValue = QString::number(row);
    e.setAttribute(attrName, attrValue);

    attrName = "interval";
    attrValue = QString::number(interval);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsRepeatingRowHeight::configureFromDomXml(const QDomElement& e)
{
    QicsRowHeight::configureFromDomXml(e);

    row = e.attribute("start").toInt();
    interval = e.attribute("interval").toInt();
}

//////////////////////////////////////////////////////////////////////////////

QicsDimensionManager::QicsColumnWidth::QicsColumnWidth()
    : width(0), chars(0), valid(false)
{
    mode = QicsDimensionUnset;
}

QicsDimensionManager::QicsColumnWidth::QicsColumnWidth(const QicsColumnWidth &cw)
    : width(cw.width), chars(cw.chars), valid(cw.valid)
{
    mode = cw.mode;
}

QDomElement QicsDimensionManager::QicsColumnWidth::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = doc->createElement(tag);
    QString attrName;
    QString attrValue;

    attrName = "mode";
    attrValue = QString::number(mode);
    e.setAttribute(attrName, attrValue);

    attrName = "width";
    attrValue = QString::number(width);
    e.setAttribute(attrName, attrValue);

    attrName = "chars";
    attrValue = QString::number(chars);
    e.setAttribute(attrName, attrValue);

    attrName = "valid";
    attrValue = QString::number(valid);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsColumnWidth::configureFromDomXml(const QDomElement& e)
{
    mode = (QicsDimensionMode) e.attribute("mode").toInt();
    width = e.attribute("width").toInt();
    chars = e.attribute("chars").toInt();
    valid = e.attribute("valid").toInt();
}

//////////////////////////////////////////////////////////////////////////////

QicsDimensionManager::QicsRepeatingColumnWidth::QicsRepeatingColumnWidth()
    : QicsColumnWidth(), col(-1), interval(0)
{
}

QicsDimensionManager::QicsRepeatingColumnWidth::QicsRepeatingColumnWidth(const QicsRepeatingColumnWidth &rh)
    : QicsColumnWidth(rh), col(rh.col), interval(rh.interval)
{
}

QDomElement QicsDimensionManager::QicsRepeatingColumnWidth::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = QicsColumnWidth::toDomXml(tag,doc);
    QString attrName;
    QString attrValue;

    attrName = "start";
    attrValue = QString::number(col);
    e.setAttribute(attrName, attrValue);

    attrName = "interval";
    attrValue = QString::number(interval);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsRepeatingColumnWidth::configureFromDomXml(const QDomElement& e)
{
    QicsColumnWidth::configureFromDomXml(e);

    col = e.attribute("start").toInt();
    interval = e.attribute("interval").toInt();
}

//////////////////////////////////////////////////////////////////////////////

QicsDimensionManager::QicsDefaultDimensionSetting::QicsDefaultDimensionSetting()
    : height(0), width(90), height_chars(1), width_chars(4), cell_margin(0),
        border_width(0)
{
    mode = QicsDimensionPixel;
}

QicsDimensionManager::QicsDefaultDimensionSetting::QicsDefaultDimensionSetting(const QicsDefaultDimensionSetting &dds)
    : height(dds.height), width(dds.width), height_chars(dds.height_chars), width_chars(dds.width_chars),
        font_height(dds.font_height), font_width(dds.font_width), cell_margin(dds.cell_margin),
        border_width(dds.border_width)
{
    mode = dds.mode;
}

void QicsDimensionManager::QicsDefaultDimensionSetting::setHeightInPixels(int height_in)
{
    mode = QicsDimensionPixel;
    height = height_in;
}

void QicsDimensionManager::QicsDefaultDimensionSetting::setHeightInChars(int height_in)
{
    mode = QicsDimensionChar;
    height_chars = height_in;

    compute();
}

void QicsDimensionManager::QicsDefaultDimensionSetting::setWidthInPixels(int width_in)
{
    mode = QicsDimensionPixel;
    width = width_in;
}

void QicsDimensionManager::QicsDefaultDimensionSetting::setWidthInChars(int width_in)
{
    mode = QicsDimensionChar;
    width_chars = width_in;

    compute();
}

void QicsDimensionManager::QicsDefaultDimensionSetting::setFont(const QFont &fnt)
{
    font_height = qicsHeightOfFont(fnt);
    font_width = qicsWidthOfFont(fnt);

    compute();
}

void QicsDimensionManager::QicsDefaultDimensionSetting::compute()
{
    if (mode == QicsDimensionPixel) {
        height = font_height + 2*cell_margin + 2*border_width;
        return;
    }
    if (mode == QicsDimensionChar) {
        height = ((font_height * height_chars) + (2 * cell_margin) +
            (2 * border_width));
        width = ((font_width * width_chars) + (2 * cell_margin) +
            (2 * border_width));
        return;
    }
}

QDomElement QicsDimensionManager::QicsDefaultDimensionSetting::toDomXml(const QString &tag, QDomDocument* doc) const
{
    QDomElement e = doc->createElement(tag);
    QString attrName;
    QString attrValue;

    attrName = "mode";
    attrValue = QString::number(mode);
    e.setAttribute(attrName, attrValue);

    attrName = "height";
    attrValue = QString::number(height);
    e.setAttribute(attrName, attrValue);

    attrName = "width";
    attrValue = QString::number(width);
    e.setAttribute(attrName, attrValue);

    attrName = "height_chars";
    attrValue = QString::number(height_chars);
    e.setAttribute(attrName, attrValue);

    attrName = "width_chars";
    attrValue = QString::number(width_chars);
    e.setAttribute(attrName, attrValue);

    attrName = "font_height";
    attrValue = QString::number(font_height);
    e.setAttribute(attrName, attrValue);

    attrName = "font_width";
    attrValue = QString::number(font_width);
    e.setAttribute(attrName, attrValue);

    attrName = "cell_margin";
    attrValue = QString::number(cell_margin);
    e.setAttribute(attrName, attrValue);

    attrName = "border_width";
    attrValue = QString::number(border_width);
    e.setAttribute(attrName, attrValue);

    return e;
}

void QicsDimensionManager::QicsDefaultDimensionSetting::configureFromDomXml(const QDomElement& e)
{
    mode = (QicsDimensionMode) e.attribute("mode").toInt();

    height = e.attribute("height").toInt();
    width = e.attribute("width").toInt();
    height_chars = e.attribute("height_chars").toInt();
    width_chars = e.attribute("width_chars").toInt();
    font_height = e.attribute("font_height").toInt();
    font_width = e.attribute("font_width").toInt();
    cell_margin = e.attribute("cell_margin").toInt();
    border_width = e.attribute("border_width").toInt();

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define SPACE_BETWEEN_LINES 3

QicsDimensionManager::QicsDimensionManager(QicsGridInfo *grid_info, QObject *parent)
    : QObject(parent), myRowDM(0), myColumnDM(0), myGridInfo(grid_info),
        myLastStretchedRow(-1), myLastStretchedColumn(-1),
        myCellWidthMode(NoChange), myEmitSignalsFlag(true)
{
    myOrigDefaultDimensions.cell_margin =  * static_cast<int *>
        (styleManager()->getDefaultProperty(QicsCellStyle::CellMargin));

    myOrigDefaultDimensions.border_width =  * static_cast<int *>
        (styleManager()->getDefaultProperty(QicsCellStyle::BorderWidth));

    const QFont &fnt = * static_cast<QFont *>
        (styleManager()->getDefaultProperty(QicsCellStyle::Font));
    myOrigDefaultDimensions.setFont(fnt);

    myCurrentDefaultDimensions = myOrigDefaultDimensions;

    myOrigDefaultMinDimensions.setFont(fnt);
    myOrigDefaultMinDimensions.mode = QicsDimensionPixel;
    myOrigDefaultMinDimensions.height = 15;
    myOrigDefaultMinDimensions.width = 15;
    myCurrentDefaultMinDimensions = myOrigDefaultMinDimensions;
    myOrigDefaultMaxDimensions.setFont(fnt);
    myOrigDefaultMaxDimensions.mode = QicsDimensionPixel;
    myOrigDefaultMaxDimensions.height = INT_MAX-1;
    myOrigDefaultMaxDimensions.width = INT_MAX-1;
    myCurrentDefaultMaxDimensions = myOrigDefaultMaxDimensions;
    _allow_override_changes = true;
}

QicsDimensionManager::QicsDimensionManager(const QicsDimensionManager &dm, QicsGridInfo *grid_info, QObject *parent)
    : QObject(parent), myRowDM(0), myColumnDM(0), myGridInfo(grid_info),
        myLastStretchedRow(-1), myLastStretchedColumn(-1),
        myCellWidthMode(NoChange), myEmitSignalsFlag(true)
{
    myOrigDefaultDimensions = dm.myOrigDefaultDimensions;
    myCurrentDefaultDimensions = dm.myCurrentDefaultDimensions;

    myOrigDefaultMinDimensions = dm.myOrigDefaultMinDimensions;
    myCurrentDefaultMinDimensions = myOrigDefaultMinDimensions;

    myOrigDefaultMaxDimensions = dm.myOrigDefaultMaxDimensions;
    myCurrentDefaultMaxDimensions = myOrigDefaultMaxDimensions;

    dupSetCells(dm.mySetCells, mySetCells);
    dupSetCells(dm.mySetVisualCells, mySetVisualCells);

    dupSetRows(dm.mySetRows, mySetRows);
    dupSetRows(dm.mySetVisualRows, mySetVisualRows);

    dupSetColumns(dm.mySetColumns, mySetColumns);
    dupSetColumns(dm.mySetVisualColumns, mySetVisualColumns);

    dupRowHeights(dm.myRowHeights, myRowHeights);
    dupRowHeights(dm.myRowMinHeights, myRowMinHeights);
    dupRowHeights(dm.myVisualRowHeights, myVisualRowHeights);
    dupRowHeights(dm.myVisualRowMinHeights, myVisualRowMinHeights);

    dupColumnWidths(dm.myColumnWidths, myColumnWidths);
    dupColumnWidths(dm.myColumnMinWidths, myColumnMinWidths);
    dupColumnWidths(dm.myVisualColumnWidths, myVisualColumnWidths);
    dupColumnWidths(dm.myVisualColumnMinWidths, myVisualColumnMinWidths);

    dupRowHeights(dm.myRowMaxHeights, myRowMaxHeights);
    dupRowHeights(dm.myVisualRowMaxHeights, myVisualRowMaxHeights);

    dupColumnWidths(dm.myColumnMaxWidths, myColumnMaxWidths);
    dupColumnWidths(dm.myVisualColumnMaxWidths, myVisualColumnMaxWidths);

    myHiddenRows = dm.myHiddenRows;
    myHiddenColumns = dm.myHiddenColumns;

    myColumnOverrides = dm.myColumnOverrides;
    myRowOverrides = dm.myRowOverrides;
}

void QicsDimensionManager::dupSetCells(const QicsCellSettingV &from,
                                  QicsCellSettingV &to)
{
    to.resize(from.size());
    qCopy(from.begin(), from.end(), to.begin());
}

void QicsDimensionManager::dupSetRows(const QicsRowSettingV &from,
                                 QicsRowSettingV &to)
{
    to.resize(from.size());
    qCopy(from.begin(), from.end(), to.begin());
}

void QicsDimensionManager::dupSetColumns(const QicsColumnSettingV &from,
                                    QicsColumnSettingV &to)
{
    to.resize(from.size());
    qCopy(from.begin(), from.end(), to.begin());
}

void QicsDimensionManager::dupRowHeights(const QicsRowHeightPV &from,
                                    QicsRowHeightPV &to)
{
    QicsRowHeightPV::const_iterator iter, iter_end(from.constEnd());

    for (iter = from.constBegin(); iter != iter_end; ++iter) {
        QicsRowHeight *rh = 0;

        if (*iter)
            rh  = new QicsRowHeight(**iter);

        to.push_back(rh);
    }
}

void QicsDimensionManager::dupColumnWidths(const QicsColumnWidthPV &from,
                                      QicsColumnWidthPV &to)
{
    QicsColumnWidthPV::const_iterator iter, iter_end(from.constEnd());

    for (iter = from.constBegin(); iter != iter_end; ++iter) {
        QicsColumnWidth *cw = 0;

        if (*iter)
            cw  = new QicsColumnWidth(**iter);

        to.push_back(cw);
    }
}

QicsDimensionManager::~QicsDimensionManager()
{
    delRowHeights(myRowHeights);
    delRowHeights(myRowMinHeights);
    delRowHeights(myRowMaxHeights);
    delRowHeights(myVisualRowHeights);
    delRowHeights(myVisualRowMinHeights);
    delRowHeights(myVisualRowMaxHeights);

    delColumnWidths(myColumnWidths);
    delColumnWidths(myColumnMinWidths);
    delColumnWidths(myColumnMaxWidths);
    delColumnWidths(myVisualColumnWidths);
    delColumnWidths(myVisualColumnMinWidths);
    delColumnWidths(myVisualColumnMaxWidths);
}

void QicsDimensionManager::delRowHeights(QicsRowHeightPV &vals)
{
    qDeleteAll(vals.begin(), vals.end());
    vals.clear();
}

void QicsDimensionManager::delColumnWidths(QicsColumnWidthPV &vals)
{
    qDeleteAll(vals.begin(), vals.end());
    vals.clear();
}

void QicsDimensionManager::setControllingRowDimensionManager(QicsDimensionManager *dm)
{
    if (myRowDM)
        disconnect(myRowDM, 0, this, 0);

    myRowDM = dm;

    if (myRowDM) {
        connect(myRowDM, SIGNAL(dimensionChanged()),
            this, SIGNAL(dimensionChanged()));
        connect(myRowDM, SIGNAL(dimensionChanged(Qics::QicsIndexType, int, int)),
            this, SIGNAL(dimensionChanged(Qics::QicsIndexType, int, int)));
    }
}

void QicsDimensionManager::setControllingColumnDimensionManager(QicsDimensionManager *dm)
{
    if (myColumnDM)
        disconnect(myColumnDM, 0, this, 0);

    myColumnDM = dm;

    if (myColumnDM) {
        connect(myColumnDM, SIGNAL(dimensionChanged()),
            this, SIGNAL(dimensionChanged()));
        connect(myColumnDM, SIGNAL(dimensionChanged(Qics::QicsIndexType, int, int)),
            this, SIGNAL(dimensionChanged(Qics::QicsIndexType, int, int)));
    }
}

QicsDimensionManager::QicsGridRole QicsDimensionManager::gridRole(QicsGridType grid_type) const
{
    if (grid_type == QicsGridInfo::RowHeaderGrid)
        return QicsRowHeaderGridRole;

    if (grid_type == QicsGridInfo::ColumnHeaderGrid)
        return QicsColumnHeaderGridRole;

    return QicsTableGridRole;
}

//////////////////////////////////////////////////////////////////////
/////////////////////  Font set methods  /////////////////////////////
//////////////////////////////////////////////////////////////////////

void QicsDimensionManager::setDefaultFont(const QFont &fnt)
{
    myOrigDefaultDimensions.setFont(fnt);
    myCurrentDefaultDimensions.setFont(fnt);

    computeAllRowHeights();
    computeAllColumnWidths();
}

void QicsDimensionManager::setRowFont(QicsGridType grid_type,
                                 int row, bool visual_coords,
                                 const QFont &fnt)
{
    int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);
    int cur_height = rowHeight(mrow, !visual_coords, visual_coords);

    if (myRowDM)
        myRowDM->setRowFont(grid_type, row, visual_coords, fnt);
    else {
        setRowFont((visual_coords ? mySetVisualRows : mySetRows),
            grid_type, row, fnt);

        if (myCellWidthMode!=NoChange)
            setRowHeightFromContent(grid_type, row, visual_coords, qicsHeightOfFont(fnt));

        invalidateRowHeight(mrow);

        int new_height = rowHeight(mrow, !visual_coords, visual_coords, true);

        bool changed = (cur_height != new_height);

        changed |= computeDefaultColumnFontWidth();

        if (changed) {
            int mvrow = (!visual_coords ? gridInfo()->visualRowIndex(row) : row);
            emit dimensionChanged(Qics::RowIndex, mvrow, mvrow);
        }
    }
}

void QicsDimensionManager::setRowFont(QicsRowSettingV &settings,
                                 QicsGridType grid_type,
                                 int row, const QFont &fnt)
{
    QicsGridRole role = gridRole(grid_type);

    QicsRowSettingV::iterator iter, iter_end(settings.end());

    bool found = false;
    for (iter = settings.begin(); iter != iter_end; ++iter) {
        QicsRowSetting &rfs = *iter;

        if ((rfs.row == row) && (rfs.role == role)) {
            found = true;
            rfs.font_height = qicsHeightOfFont(fnt);
            rfs.font_width = qicsWidthOfFont(fnt);
            break;
        }
    }

    if (!found) {
        QicsRowSetting rfs;

        rfs.role = role;
        rfs.row = row;
        rfs.font_height = qicsHeightOfFont(fnt);
        rfs.font_width = qicsWidthOfFont(fnt);

        settings.push_back(rfs);
    }
}

void QicsDimensionManager::setRepeatingRowFont(QicsGridType grid_type,
                                          int row, unsigned int interval,
                                          const QFont &fnt)
{
    if (myRowDM)
        myRowDM->setRepeatingRowFont(grid_type, row, interval, fnt);
    else {
        QicsGridRole role = gridRole(grid_type);

        QicsRepeatingRowSettingV::iterator iter, iter_end(mySetRepeatingRows.end());

        bool found = false;
        for (iter = mySetRepeatingRows.begin(); iter != iter_end; ++iter) {
            QicsRepeatingRowSetting &rfs = *iter;

            if ((rfs.row == row) && (rfs.interval == interval) && (rfs.role == role)) {
                found = true;
                rfs.font_height = qicsHeightOfFont(fnt);
                rfs.font_width = qicsWidthOfFont(fnt);
                break;
            }
        }

        if (!found) {
            QicsRepeatingRowSetting rfs;

            rfs.role = role;
            rfs.row = row;
            rfs.interval = interval;
            rfs.font_height = qicsHeightOfFont(fnt);
            rfs.font_width = qicsWidthOfFont(fnt);

            mySetRepeatingRows.push_back(rfs);
        }

        emit dimensionChanged();
    }
}

void QicsDimensionManager::unsetRowFont(QicsGridType grid_type, int row,
                                   bool visual_coords)
{
    int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);
    int cur_height = rowHeight(mrow, !visual_coords, visual_coords);

    if (myRowDM)
        myRowDM->unsetRowFont(grid_type, row, visual_coords);
    else {
        unsetRowFont((visual_coords ? mySetVisualRows : mySetRows),
            grid_type, row);

        invalidateRowHeight(mrow);

        int new_height = rowHeight(mrow, !visual_coords, visual_coords, true);

        bool changed = (cur_height != new_height);

        changed |= computeDefaultColumnFontWidth();

        if (changed) {
            int mvrow = (!visual_coords ? gridInfo()->visualRowIndex(row) : row);
            emit dimensionChanged(Qics::RowIndex, mvrow, mvrow);
        }
    }
}

void QicsDimensionManager::unsetRowFont(QicsRowSettingV &settings,
                                   QicsGridType grid_type, int row)
{
    QicsGridRole role = gridRole(grid_type);

    QicsRowSettingV::iterator iter, iter_end(settings.end());

    for (iter = settings.begin(); iter != iter_end; ++iter) {
        QicsRowSetting &rfs = *iter;

        if ((rfs.row == row) && (rfs.role == role)) {
            rfs.font_height = 0;
            rfs.font_width = 0;

            if (rfs.isEmpty())
                settings.erase(iter);

            break;
        }
    }
}

void QicsDimensionManager::setColumnFont(QicsGridType grid_type,
                                    int col, bool visual_coords,
                                    const QFont &fnt)
{
    int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);
    int cur_width = columnWidth(mcol, !visual_coords, visual_coords);

    if (myColumnDM)
        myColumnDM->setColumnFont(grid_type, col, visual_coords, fnt);
    else {
        setColumnFont((visual_coords ? mySetVisualColumns : mySetColumns),
            grid_type, col, fnt);

        invalidateColumnWidth(mcol);

        int new_width = columnWidth(mcol, !visual_coords, visual_coords,
            true);
        bool changed = (cur_width != new_width);

        changed |= computeDefaultRowFontHeight();

        if (changed) {
            int mvcol = (!visual_coords ? gridInfo()->visualColumnIndex(col) : col);
            emit dimensionChanged(Qics::ColumnIndex, mvcol, mvcol);
        }
    }
}

void QicsDimensionManager::setColumnFont(QicsColumnSettingV &settings,
                                    QicsGridType grid_type,
                                    int col, const QFont &fnt)
{
    QicsGridRole role = gridRole(grid_type);

    QicsColumnSettingV::iterator iter, iter_end(settings.end());

    bool found = false;
    for (iter = settings.begin(); iter != iter_end; ++iter) {
        QicsColumnSetting &cfs = *iter;

        if ((cfs.col == col) && (cfs.role == role)) {
            found = true;
            cfs.font_width = qicsWidthOfFont(fnt);
            cfs.font_height = qicsHeightOfFont(fnt);
            break;
        }
    }

    if (!found) {
        QicsColumnSetting cfs;

        cfs.role = role;
        cfs.col = col;
        cfs.font_width = qicsWidthOfFont(fnt);
        cfs.font_height = qicsHeightOfFont(fnt);

        settings.push_back(cfs);
    }
}

void QicsDimensionManager::setRepeatingColumnFont(QicsGridType grid_type,
                                             int col, unsigned int interval,
                                             const QFont &fnt)
{
    if (myColumnDM)
        myColumnDM->setRepeatingColumnFont(grid_type, col, interval, fnt);
    else {
        QicsGridRole role = gridRole(grid_type);

        QicsRepeatingColumnSettingV::iterator iter, iter_end(mySetRepeatingColumns.end());

        bool found = false;
        for (iter = mySetRepeatingColumns.begin(); iter != iter_end; ++iter) {
            QicsRepeatingColumnSetting &rfs = *iter;

            if ((rfs.col == col) && (rfs.interval == interval) && (rfs.role == role)) {
                found = true;
                rfs.font_height = qicsHeightOfFont(fnt);
                rfs.font_width = qicsWidthOfFont(fnt);
                break;
            }
        }

        if (!found) {
            QicsRepeatingColumnSetting rfs;

            rfs.role = role;
            rfs.col = col;
            rfs.interval = interval;
            rfs.font_height = qicsHeightOfFont(fnt);
            rfs.font_width = qicsWidthOfFont(fnt);

            mySetRepeatingColumns.push_back(rfs);
        }

        emit dimensionChanged();
    }
}

void QicsDimensionManager::unsetColumnFont(QicsGridType grid_type, int col,
                                      bool visual_coords)
{
    int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);
    int cur_width = columnWidth(mcol, !visual_coords, visual_coords);

    if (myColumnDM)
        myColumnDM->unsetColumnFont(grid_type, col, visual_coords);
    else {
        unsetColumnFont((visual_coords ? mySetVisualColumns : mySetColumns),
            grid_type, col);

        invalidateColumnWidth(mcol);

        int new_width = columnWidth(mcol, !visual_coords, visual_coords,
            true);
        bool changed = (cur_width != new_width);

        changed |= computeDefaultRowFontHeight();

        if (changed) {
            int mvcol = (!visual_coords ? gridInfo()->visualColumnIndex(col) : col);
            emit dimensionChanged(Qics::ColumnIndex, mvcol, mvcol);
        }
    }
}

void QicsDimensionManager::unsetColumnFont(QicsColumnSettingV &settings,
                                      QicsGridType grid_type, int col)
{
    QicsGridRole role = gridRole(grid_type);

    QicsColumnSettingV::iterator iter, iter_end(settings.end());

    for (iter = settings.begin(); iter != iter_end; ++iter) {
        QicsColumnSetting &cfs = *iter;

        if ((cfs.col == col) && (cfs.role == role)) {
            cfs.font_height = 0;
            cfs.font_width = 0;

            if (cfs.isEmpty())
                settings.erase(iter);

            break;
        }
    }
}

void QicsDimensionManager::prepareRegionFont(QicsGridType grid_type,
                                        const QicsRegion &region,
                                        bool visual_coords, const QFont &fnt)
{
    Q_UNUSED(grid_type);
    Q_UNUSED(fnt);

    QicsGridInfo *info = gridInfo();
    QicsStyleManager *sm = info->styleManager();

    for (int i = region.top(); i <= region.bottom(); ++i) {
        int mr = visual_coords ? info->modelRowIndex(i) : i;

        if (myFontSizeVector.count() > mr) {
            for (int j = region.left(); j <= region.right(); ++j) {
                int mc = visual_coords ? info->modelColumnIndex(j) : j;

                QFont *font = (QFont*) sm->getCellProperty(mr, mc, QicsCellStyle::Font);
                if (!font)
                    continue;

                int size = qicsHeightOfFont(*font);
                QMap<int, int> &map = myFontSizeVector[mr];
                if (map.contains(size)) {
                    int v = map[size];
                    if (v) v--;
                    if (v)
                        map[size] = v;
                    else
                        map.remove(size);
                }
            }
        } else
            myFontSizeVector.resize(visual_coords ? info->modelRowIndex(region.bottom())+1 : region.bottom()+1);
    }
}

void QicsDimensionManager::setRegionFont(QicsGridType grid_type,
                                    const QicsRegion &region,
                                    bool visual_coords, const QFont &fnt)
{
    QicsGridRole role = gridRole(grid_type);
    QicsCellSettingV &settings = mySetCells;
    int font_height = qicsHeightOfFont(fnt);
    int font_width = qicsWidthOfFont(fnt);

    QicsCellSettingV::iterator iter, it_end(settings.end());
    QSet<QPair<int,int> > tempSet;
    for (iter = settings.begin(); iter < it_end; ++iter) {
        QicsCellSetting &setting = *iter;
        if (region.containsCell(setting.row, setting.col) && setting.role == role) {
            tempSet.insert(QPair<int,int>(setting.row, setting.col));
            setting.font_height = font_height;
            setting.font_width = font_width;
        }
    }

    for (int i = region.top(); i <= region.bottom(); ++i) {
        int mr = visual_coords ? gridInfo()->modelRowIndex(i) : i;

        QMap<int, int> &map = myFontSizeVector[mr];
        int v = map[font_height];
        v += region.width();
        map[font_height] = v;

        if (myCellWidthMode==NoChange)
            continue;

        int size = map.keys().last();
        setRowHeightFromContent(grid_type,mr,false,size);
    }
}

void QicsDimensionManager::setRowHeightFromContent(QicsGridType /*grid_type*/, int row, bool visual_coords, int content_height)
{
    int cell_margin = myOrigDefaultDimensions.cell_margin;
    int *cm = (int*)gridInfo()->styleManager()->getRowProperty(row, QicsCellStyle::CellMargin);
    if (cm) cell_margin = *cm;

    int border_width = myOrigDefaultDimensions.border_width;
    int *wb = (int*)gridInfo()->styleManager()->getRowProperty(row, QicsCellStyle::BorderWidth);
    if (wb) border_width = *wb;

    int height = computeRowHeight(1, content_height, cell_margin, border_width);
    int minrh = rowMinHeight(row);
    int maxrh = rowMaxHeight(row);
    height = qMin(height, maxrh);
    height = qMax(height, minrh);

    setRowHeightInPixels(row,visual_coords,height);
}

void QicsDimensionManager::setCellFont(QicsGridType grid_type,
                                  int row, int col, bool visual_coords,
                                  const QFont &fnt)
{
    setCellFont((visual_coords ? mySetVisualCells : mySetCells),
        grid_type, row, col, fnt);

    if (myCellWidthMode == NoChange)
        return;

    if (gridInfo()->cellValue(row,col)) {
        const int cols = gridInfo()->dataModel()->numColumns();
        const int rows = gridInfo()->dataModel()->numRows();
        int new_fontheight = qicsHeightOfFont(fnt);
        int new_font_width = columnWidth(col);

        bool set_font = true;
        for (int i=0;i<cols;++i) {
            if (i == col) continue;

            if (gridInfo()->cellValue(row,i)) {
                QicsCell myCell(row, i, gridInfo());
                int temp_fontheight=qicsHeightOfFont(myCell.font());

                if (temp_fontheight > new_fontheight) {
                    set_font = false;
                    break;
                }
            }
        }

        if (set_font)
            setRowHeightFromContent(grid_type,row,visual_coords,new_fontheight);

        if (myCellWidthMode==ChangeHeightWidth) {
            for (int i=0;i<rows;++i) {
                if (gridInfo()->cellValue(i,col)) {
                    QicsCell myCell(i,col, gridInfo());

                    QFontMetrics fm(myCell.font());
                    int temp_fontwidth = fm.width ( gridInfo()->cellValue(i,col)->string());

                    if (new_font_width<temp_fontwidth)
                        new_font_width=temp_fontwidth;
                }
            }
        }
        /* if (new_font_width != new_font_width)
          new_font_width = new_font_width + 10; */

        setColumnWidthInPixels(col,visual_coords,new_font_width);

    }
}

void QicsDimensionManager::setCellFont(QicsCellSettingV &settings,
                                  QicsGridType grid_type,
                                  int row, int col, const QFont &fnt)
{
    QicsGridRole role = gridRole(grid_type);

    QicsCellSettingV::iterator iter, it_end(settings.end());
    int font_height = qicsHeightOfFont(fnt);
    int font_width = qicsWidthOfFont(fnt);

    bool found = false;

    // Search for this cell in the list
    for (iter = settings.begin(); iter != it_end; ++iter) {
        QicsCellSetting &setting = *iter;

        if ((setting.row == row) && (setting.col == col) && (setting.role == role)) {
            setting.font_height = font_height;
            setting.font_width = font_width;

            found = true;
            break;
        }
    }

    if (!found) {
        // Didn't find it, must be a new cell

        QicsCellSetting setting;

        setting.role = role;
        setting.row = row;
        setting.col = col;
        setting.font_height = font_height;
        setting.font_width = font_width;

        settings.push_back(setting);
    }
}

void QicsDimensionManager::unsetCellFont(QicsGridType grid_type,
                                    int row, int col, bool visual_coords)
{
    int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);
    int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);
    int cur_height = rowHeight(mrow, !visual_coords, visual_coords);
    int cur_width = columnWidth(mcol, !visual_coords, visual_coords);

    bool changedRow = false;
    bool changedColumn = false;

    unsetCellFont((visual_coords ? mySetVisualCells : mySetCells),
        grid_type, row, col);

    if (myRowDM)
        myRowDM->unsetCellFont(grid_type, row, col, visual_coords);

    invalidateRowHeight(mrow);

    int new_height = rowHeight(mrow, !visual_coords, visual_coords, true);
    changedRow = (cur_height != new_height);

    if (myColumnDM)
        myColumnDM->unsetCellFont(grid_type, row, col, visual_coords);

    invalidateColumnWidth(mcol);

    int new_width = columnWidth(mcol, !visual_coords, visual_coords, true);
    changedColumn |= (cur_width != new_width);

    if (changedRow && changedColumn)
        emit dimensionChanged();
    else if (changedRow) {
        int mvrow = (!visual_coords ? gridInfo()->visualRowIndex(row) : row);
        emit dimensionChanged(Qics::RowIndex, mvrow, mvrow);
    }
    else if (changedColumn) {
        int mvcol = (!visual_coords ? gridInfo()->visualColumnIndex(col) : col);
        emit dimensionChanged(Qics::ColumnIndex, mvcol, mvcol);
    }
}

void QicsDimensionManager::unsetCellFont(QicsCellSettingV &settings,
                                    QicsGridType grid_type,
                                    int row, int col)
{
    QicsGridRole role = gridRole(grid_type);

    QicsCellSettingV::iterator iter, it_end(settings.end());

    // Search for this cell in the list
    for (iter = settings.begin(); iter != it_end; ++iter) {
        QicsCellSetting &setting = *iter;

        if ((setting.row == row) && (setting.col == col) && (setting.role == role)) {
            setting.font_height = 0;
            setting.font_width = 0;

            if (setting.isEmpty()) {
                settings.erase(iter);
                it_end = settings.end();
            }

            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////
/////////////////// Row Height set methods ///////////////////////////
//////////////////////////////////////////////////////////////////////

void QicsDimensionManager::setRowHeightInPixels(int row, bool visual_coords,
                                           int height)
{
    bool isResizeHeader = false;
    if (row == -1) {
        isResizeHeader = true;
        row = 0;
    }
    else
        isResizeHeader = false;

    if (myRowDM)
        myRowDM->setRowHeightInPixels(row, visual_coords, height);
    else {
        QicsRowHeightPV &heights = visual_coords ? myVisualRowHeights : myRowHeights;

        if (_allow_override_changes)
            if (overriddenRowHeight(visual_coords ? myGridInfo->modelRowIndex(row) : row)>=0)
                resetRowHeight(visual_coords ? myGridInfo->modelRowIndex(row) : row);

        if (row >= heights.size())
            heights.resize(row + 20);

        if (!heights.at(row))
            heights[row] = new QicsRowHeight();

        heights.at(row)->mode = QicsDimensionPixel;

        if (!isResizeHeader)
            emit rowResized(row,(heights.at(row)->height)?heights.at(row)->height:myCurrentDefaultDimensions.height,height);
        else
            emit columnHeaderResized((heights.at(row)->height)?heights.at(row)->height:myCurrentDefaultDimensions.height,height);

        heights.at(row)->height = height;

        int mvrow = (!visual_coords ? gridInfo()->visualRowIndex(row) : row);
        emit dimensionChanged(Qics::RowIndex, mvrow, mvrow);
    }
}

void QicsDimensionManager::setRowHeightInChars(int row, bool visual_coords,
                                          int height)
{
    if (myRowDM)
        myRowDM->setRowHeightInChars(row, visual_coords, height);
    else {
        int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);
        int cur_height = rowHeight(mrow, !visual_coords, visual_coords);

        QicsRowHeightPV *pHeights;
        pHeights = (visual_coords ? &myVisualRowHeights : &myRowHeights);
        QicsRowHeightPV &heights = *pHeights;

        if (_allow_override_changes)
            if (overriddenRowHeight(visual_coords ? myGridInfo->modelRowIndex(row) : row)>=0)
                resetRowHeight(visual_coords ? myGridInfo->modelRowIndex(row) : row);

        if (row >= heights.size())
            heights.resize(row + 20);

        if (!heights.at(row))
            heights[row] = new QicsRowHeight();

        heights.at(row)->mode = QicsDimensionChar;
        heights.at(row)->chars = height;

        invalidateRowHeight(mrow);

        int new_height = rowHeight(mrow, !visual_coords, visual_coords, true);

        if (cur_height != new_height) {
            int mvrow = (!visual_coords ? gridInfo()->visualRowIndex(row) : row);
            emit dimensionChanged(Qics::RowIndex, mvrow, mvrow);
        }
    }
}

void QicsDimensionManager::setDefaultRowHeightInPixels(int height)
{
    if (myRowDM)
        myRowDM->setDefaultRowHeightInPixels(height);
    else {
        myOrigDefaultDimensions.setHeightInPixels(height);
        myCurrentDefaultDimensions.setHeightInPixels(height);

        emit dimensionChanged();
    }
}

void QicsDimensionManager::setDefaultRowHeightInChars(int height)
{
    if (myRowDM)
        myRowDM->setDefaultRowHeightInChars(height);
    else {
        myOrigDefaultDimensions.setHeightInChars(height);
        myCurrentDefaultDimensions.setHeightInChars(height);
        computeAllRowHeights();

        emit dimensionChanged();
    }
}

void QicsDimensionManager::setRowMinHeightInPixels(int row, bool visual_coords,
                                              int height)
{
    if (myRowDM)
        myRowDM->setRowMinHeightInPixels(row, visual_coords, height);
    else {
        QicsRowHeightPV *pHeights;
        pHeights = (visual_coords ? &myVisualRowMinHeights : &myRowMinHeights);
        QicsRowHeightPV &heights = *pHeights;

        if (row >= heights.size())
            heights.resize(row + 20);

        if (!heights.at(row))
            heights[row] = new QicsRowHeight();

        heights.at(row)->mode = QicsDimensionPixel;
        heights.at(row)->height = height;

        if (visual_coords) {
            if (height > rowHeight(row, false, true))
                setRowHeightInPixels(row, visual_coords, height);
        }
        else {
            if (height > rowHeight(row, true, false))
                setRowHeightInPixels(row, visual_coords, height);
        }
    }
}

void QicsDimensionManager::setRowMinHeightInChars(int row, bool visual_coords,
                                             int height)
{
    if (myRowDM)
        myRowDM->setRowMinHeightInChars(row, visual_coords, height);
    else {
        int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);

        int cur_height = (visual_coords ? rowMinHeight(mrow, false, true) :
            rowMinHeight(mrow, true, false));

        QicsRowHeightPV *pHeights;
        pHeights = (visual_coords ? &myVisualRowMinHeights : &myRowMinHeights);
        QicsRowHeightPV &heights = *pHeights;

        if (row >= heights.size())
            heights.resize(row + 20);

        if (!heights.at(row))
            heights[row] = new QicsRowHeight();

        heights.at(row)->mode = QicsDimensionChar;
        heights.at(row)->chars = height;

        if (visual_coords) {
            if (cur_height > rowMinHeight(row, false, true))
                setRowHeightInChars(row, visual_coords, height);
        }
        else {
            if (cur_height > rowHeight(row, true, false))
                setRowHeightInChars(row, visual_coords, height);
        }
    }
}

void QicsDimensionManager::setRowMaxHeightInPixels(int row, bool visual_coords,
                                              int height)
{
    if (myRowDM)
        myRowDM->setRowMaxHeightInPixels(row, visual_coords, height);
    else {
        QicsRowHeightPV *pHeights;
        pHeights = (visual_coords ? &myVisualRowMaxHeights : &myRowMaxHeights);
        QicsRowHeightPV &heights = *pHeights;

        if (row >= heights.size())
            heights.resize(row + 20);

        if (!heights.at(row))
            heights[row] = new QicsRowHeight();

        heights.at(row)->mode = QicsDimensionPixel;
        heights.at(row)->height = height;

        if (visual_coords) {
            if (height < rowHeight(row, false, true))
                setRowHeightInPixels(row, visual_coords, height);
        }
        else {
            if (height < rowHeight(row, true, false))
                setRowHeightInPixels(row, visual_coords, height);
        }
    }
}

void QicsDimensionManager::setRowMaxHeightInChars(int row, bool visual_coords,
                                             int height)
{
    if (myRowDM)
        myRowDM->setRowMaxHeightInChars(row, visual_coords, height);
    else {
        int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);

        int cur_height = (visual_coords ? rowMaxHeight(mrow, false, true) :
            rowMaxHeight(mrow, true, false));

        QicsRowHeightPV *pHeights;
        pHeights = (visual_coords ? &myVisualRowMaxHeights : &myRowMaxHeights);
        QicsRowHeightPV &heights = *pHeights;

        if (row >= heights.size())
            heights.resize(row + 20);

        if (!heights.at(row))
            heights[row] = new QicsRowHeight();

        heights.at(row)->mode = QicsDimensionChar;
        heights.at(row)->chars = height;

        if (visual_coords) {
            if (cur_height < rowMaxHeight(row, false, true))
                setRowHeightInChars(row, visual_coords, height);
        }
        else {
            if (cur_height < rowHeight(row, true, false))
                setRowHeightInChars(row, visual_coords, height);
        }
    }
}

void QicsDimensionManager::setRepeatingRowHeightInPixels(int row, unsigned int interval,
                                                    int height)
{
    if (myRowDM)
        myRowDM->setRowHeightInPixels(row, interval, height);
    else {
        QicsRepeatingRowHeightV::iterator iter, iter_end(myRepeatingRowHeights.end());

        bool found = false;
        for (iter = myRepeatingRowHeights.begin(); iter != iter_end; ++iter) {
            QicsRepeatingRowHeight &rrh = *iter;

            if ((rrh.row == row) && (rrh.interval == interval)) {
                rrh.mode = QicsDimensionPixel;
                rrh.height = height;

                found = true;
                break;
            }
        }

        if (!found) {
            QicsRepeatingRowHeight rrh;

            rrh.row = row;
            rrh.interval = interval;
            rrh.mode = QicsDimensionPixel;
            rrh.height = height;

            myRepeatingRowHeights.push_back(rrh);
        }

        emit dimensionChanged();
    }
}

void QicsDimensionManager::setRepeatingRowHeightInChars(int row, unsigned int interval,
                                                   int height)
{
    if (myRowDM)
        myRowDM->setRowHeightInChars(row, interval, height);
    else {
        QicsRepeatingRowHeightV::iterator iter, iter_end(myRepeatingRowHeights.end());

        bool found = false;
        for (iter = myRepeatingRowHeights.begin(); iter != iter_end; ++iter) {
            QicsRepeatingRowHeight &rrh = *iter;

            if ((rrh.row == row) && (rrh.interval == interval)) {
                rrh.mode = QicsDimensionChar;
                rrh.height = height;

                found = true;
                break;
            }
        }

        if (!found) {
            QicsRepeatingRowHeight rrh;

            rrh.row = row;
            rrh.interval = interval;
            rrh.mode = QicsDimensionChar;
            rrh.height = height;

            myRepeatingRowHeights.push_back(rrh);
        }

        emit dimensionChanged();
    }
}

//////////////////////////////////////////////////////////////////////
/////////////////// Column Width set methods  ////////////////////////
//////////////////////////////////////////////////////////////////////

void QicsDimensionManager::setColumnWidthInPixels(int col, bool visual_coords,
                                             int width)
{
    bool isResizeHeader = false;
    if (col == -1) {
        isResizeHeader = true;
        col = 0;
    }
    else
        isResizeHeader = false;

    if (myColumnDM)
        myColumnDM->setColumnWidthInPixels(col, visual_coords, width);
    else {
        QicsColumnWidthPV *pWidths;
        pWidths = (visual_coords ? &myVisualColumnWidths : &myColumnWidths);
        QicsColumnWidthPV &widths = *pWidths;

        if (_allow_override_changes)
            if (overriddenColumnWidth(visual_coords ? myGridInfo->modelColumnIndex(col) : col)>=0)
                resetColumnWidth(visual_coords ? myGridInfo->modelColumnIndex(col) : col);

        if (col >= widths.size())
            widths.resize(col + 20);

        if (!widths.at(col))
            widths[col] = new QicsColumnWidth();

        widths.at(col)->mode = QicsDimensionPixel;

        if (!isResizeHeader)
            emit columnResized(col,(widths.at(col)->width)?widths.at(col)->width:myCurrentDefaultDimensions.width,width);
        else
            emit rowHeaderResized((widths.at(col)->width)?widths.at(col)->width:myCurrentDefaultDimensions.width,width);

        (widths[col])->width = width;

        int mvcol = (!visual_coords ? gridInfo()->visualColumnIndex(col) : col);
        emit dimensionChanged(Qics::ColumnIndex, mvcol, mvcol);
    }
}

void QicsDimensionManager::setColumnWidthInChars(int col, bool visual_coords,
                                            int width)
{
    if (myColumnDM)
        myColumnDM->setColumnWidthInChars(col, visual_coords, width);
    else {
        int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);
        int cur_width = columnWidth(mcol, !visual_coords, visual_coords);

        QicsColumnWidthPV *pWidths;
        pWidths = (visual_coords ? &myVisualColumnWidths : &myColumnWidths);
        QicsColumnWidthPV &widths = *pWidths;

        if (_allow_override_changes)
            if (overriddenColumnWidth(visual_coords ? myGridInfo->modelColumnIndex(col) : col)>=0)
                resetColumnWidth(visual_coords ? myGridInfo->modelColumnIndex(col) : col);

        if (col >= widths.size())
            widths.resize(col + 20);

        if (!widths.at(col))
            widths[col] = new QicsColumnWidth();

        widths.at(col)->mode = QicsDimensionChar;
        widths.at(col)->chars = width;

        invalidateColumnWidth(mcol);

        int new_width = columnWidth(mcol, !visual_coords, visual_coords,
            true);

        if (cur_width != new_width) {
            int mvcol = (!visual_coords ? gridInfo()->visualColumnIndex(col) : col);
            emit dimensionChanged(Qics::ColumnIndex, mvcol, mvcol);
        }
    }
}

void QicsDimensionManager::setDefaultColumnWidthInPixels(int width)
{
    if (myColumnDM)
        myColumnDM->setDefaultColumnWidthInPixels(width);
    else {
        myOrigDefaultDimensions.setWidthInPixels(width);
        myCurrentDefaultDimensions.setWidthInPixels(width);

        emit dimensionChanged();
    }
}

void QicsDimensionManager::setDefaultColumnWidthInChars(int width)
{
    if (myColumnDM)
        myColumnDM->setDefaultColumnWidthInChars(width);
    else {
        myOrigDefaultDimensions.setWidthInChars(width);
        myCurrentDefaultDimensions.setWidthInChars(width);
        computeAllColumnWidths();

        emit dimensionChanged();
    }
}

void QicsDimensionManager::setColumnMinWidthInPixels(int col, bool visual_coords,
                                                int width)
{
    if (myColumnDM)
        myColumnDM->setColumnMinWidthInPixels(col, visual_coords, width);
    else {
        QicsColumnWidthPV *pWidths;
        pWidths = (visual_coords ? &myVisualColumnMinWidths : &myColumnMinWidths);
        QicsColumnWidthPV &widths = *pWidths;

        if (col >= widths.size())
            widths.resize(col + 20);

        if (!widths.at(col))
            widths[col] = new QicsColumnWidth();

        widths.at(col)->mode = QicsDimensionPixel;
        widths.at(col)->width = width;

        if (visual_coords) {
            if (width > columnWidth(col, false, true))
                setColumnWidthInPixels(col, visual_coords, width);
        }
        else {
            if (width > columnWidth(col, true, false))
                setColumnWidthInPixels(col, visual_coords, width);
        }
    }
}

void QicsDimensionManager::setColumnMinWidthInChars(int col, bool visual_coords,
                                               int width)
{
    if (myColumnDM)
        myColumnDM->setColumnMinWidthInChars(col, visual_coords, width);
    else {
        int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);

        int cur_width = (visual_coords ? columnMinWidth(mcol, false, true) :
            columnMinWidth(mcol, true, false));

        QicsColumnWidthPV *pWidths;
        pWidths = (visual_coords ? &myVisualColumnMinWidths :
            &myColumnMinWidths);
        QicsColumnWidthPV &widths = *pWidths;

        if (col >= widths.size())
            widths.resize(col + 20);

        if (!widths.at(col))
            widths[col] = new QicsColumnWidth();

        widths.at(col)->mode = QicsDimensionChar;
        widths.at(col)->chars = width;

        if (visual_coords) {
            if (cur_width > columnMinWidth(col, false, true))
                setColumnWidthInChars(col, visual_coords, width);
        }
        else {
            if (cur_width > columnWidth(col, true, false))
                setColumnWidthInChars(col, visual_coords, width);
        }
    }
}

void QicsDimensionManager::setColumnMaxWidthInPixels(int col, bool visual_coords,
                                                int width)
{
    if (myColumnDM)
        myColumnDM->setColumnMaxWidthInPixels(col, visual_coords, width);
    else {
        QicsColumnWidthPV *pWidths;
        pWidths = (visual_coords ? &myVisualColumnMaxWidths : &myColumnMaxWidths);
        QicsColumnWidthPV &widths = *pWidths;

        if (col >= widths.size())
            widths.resize(col + 20);

        if (!widths.at(col))
            widths[col] = new QicsColumnWidth();

        widths.at(col)->mode = QicsDimensionPixel;
        widths.at(col)->width = width;

        if (visual_coords) {
            if (width < columnWidth(col, false, true))
                setColumnWidthInPixels(col, visual_coords, width);
        }
        else {
            if (width < columnWidth(col, true, false))
                setColumnWidthInPixels(col, visual_coords, width);
        }
    }
}

void QicsDimensionManager::setColumnMaxWidthInChars(int col, bool visual_coords,
                                               int width)
{
    if (myColumnDM)
        myColumnDM->setColumnMaxWidthInChars(col, visual_coords, width);
    else {
        int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);

        int cur_width = (visual_coords ? columnMaxWidth(mcol, false, true) :
            columnMaxWidth(mcol, true, false));

        QicsColumnWidthPV *pWidths;
        pWidths = (visual_coords ? &myVisualColumnMaxWidths :
            &myColumnMaxWidths);
        QicsColumnWidthPV &widths = *pWidths;

        if (col >= widths.size())
            widths.resize(col + 20);

        if (!widths.at(col))
            widths[col] = new QicsColumnWidth();

        widths.at(col)->mode = QicsDimensionChar;
        widths.at(col)->chars = width;

        if (visual_coords) {
            if (cur_width < columnMinWidth(col, false, true))
                setColumnWidthInChars(col, visual_coords, width);
        }
        else {
            if (cur_width < columnMaxWidth(col, true, false))
                setColumnWidthInChars(col, visual_coords, width);
        }
    }
}

void QicsDimensionManager::setRepeatingColumnWidthInPixels(int col,
                                                      unsigned int interval,
                                                      int width)
{
    if (myColumnDM)
        myColumnDM->setColumnWidthInPixels(col, interval, width);
    else {
        QicsRepeatingColumnWidthV::iterator iter, iter_end(myRepeatingColumnWidths.end());

        bool found = false;
        for (iter = myRepeatingColumnWidths.begin(); iter != iter_end; ++iter) {
            QicsRepeatingColumnWidth &rrh = *iter;

            if ((rrh.col == col) && (rrh.interval == interval)) {
                rrh.mode = QicsDimensionPixel;
                rrh.width = width;

                found = true;
                break;
            }
        }

        if (!found) {
            QicsRepeatingColumnWidth rrh;

            rrh.col = col;
            rrh.interval = interval;
            rrh.mode = QicsDimensionPixel;
            rrh.width = width;

            myRepeatingColumnWidths.push_back(rrh);
        }

        emit dimensionChanged();
    }
}

void QicsDimensionManager::setRepeatingColumnWidthInChars(int col,
                                                     unsigned int interval,
                                                     int width)
{
    if (myColumnDM)
        myColumnDM->setColumnWidthInChars(col, interval, width);
    else {
        QicsRepeatingColumnWidthV::iterator iter, iter_end(myRepeatingColumnWidths.end());

        bool found = false;
        for (iter = myRepeatingColumnWidths.begin(); iter != iter_end; ++iter) {
            QicsRepeatingColumnWidth &rrh = *iter;

            if ((rrh.col == col) && (rrh.interval == interval)) {
                rrh.mode = QicsDimensionChar;
                rrh.width = width;

                found = true;
                break;
            }
        }

        if (!found) {
            QicsRepeatingColumnWidth rrh;

            rrh.col = col;
            rrh.interval = interval;
            rrh.mode = QicsDimensionChar;
            rrh.width = width;

            myRepeatingColumnWidths.push_back(rrh);
        }

        emit dimensionChanged();
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////// Margin set methods //////////////////////////////
//////////////////////////////////////////////////////////////////////

void QicsDimensionManager::setDefaultMargin(int margin)
{
    myOrigDefaultDimensions.cell_margin = margin;
    myCurrentDefaultDimensions.cell_margin = margin;

    computeAllRowHeights();
    computeAllColumnWidths();

    const int rows = gridInfo()->dataModel()->numRows();

    for (int i=0;i<rows;++i) {
        int cur_height = rowHeight(i, true,true);

        invalidateRowHeight(i);

        int new_height = rowHeight(i,true,true, true);

        bool changed = (cur_height != new_height);

        changed |= computeDefaultColumnFontWidth();

        if (changed)
            emit dimensionChanged(Qics::RowIndex, i, i);
    }

    const int cols = gridInfo()->dataModel()->numColumns();

    for (int i=0;i<cols;++i) {
        int cur_width = columnWidth(i,true,true);

        invalidateColumnWidth(i);

        int new_width = columnWidth(i,true,true, true);
        bool changed = (cur_width != new_width);

        changed |= computeDefaultRowFontHeight();

        if (changed)
            emit dimensionChanged(Qics::ColumnIndex,i, i);
    }
}

void QicsDimensionManager::setRowMargin(QicsGridType grid_type,
                                   int row, bool visual_coords,
                                   int margin)
{
    if (myRowDM)
        myRowDM->setRowMargin(grid_type, row, visual_coords, margin);
    else {
        int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);
        int cur_height = rowHeight(mrow, !visual_coords, visual_coords);

        QicsGridRole role = gridRole(grid_type);

        QicsRowSettingV *pSettings;
        pSettings = (visual_coords ? &mySetVisualRows : &mySetRows);
        QicsRowSettingV &settings = *pSettings;

        QicsRowSettingV::iterator iter, iter_end(settings.end());

        bool found = false;
        for (iter = settings.begin(); iter != iter_end; ++iter) {
            QicsRowSetting &rfs = *iter;

            if ((rfs.row == row) && (rfs.role == role)) {
                found = true;
                rfs.cell_margin = margin;
                break;
            }
        }

        if (!found) {
            QicsRowSetting rfs;

            rfs.role = role;
            rfs.row = row;
            rfs.cell_margin = margin;

            settings.push_back(rfs);
        }

        invalidateRowHeight(mrow);

        int new_height = rowHeight(mrow, !visual_coords, visual_coords, true);

        bool changed = (cur_height != new_height);

        changed |= computeDefaultColumnFontWidth();

        if (changed) {
            int mvrow = (!visual_coords ? gridInfo()->visualRowIndex(row) : row);
            emit dimensionChanged(Qics::RowIndex, mvrow, mvrow);
        }
    }
}

void QicsDimensionManager::setColumnMargin(QicsGridType grid_type,
                                      int col, bool visual_coords,
                                      int margin)
{
    if (myColumnDM)
        myColumnDM->setColumnMargin(grid_type, col, visual_coords, margin);
    else {
        int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);
        int cur_width = columnWidth(mcol, !visual_coords, visual_coords);

        QicsGridRole role = gridRole(grid_type);

        QicsColumnSettingV *pSettings;
        pSettings = (visual_coords ? &mySetVisualColumns : &mySetColumns);
        QicsColumnSettingV &settings = *pSettings;

        QicsColumnSettingV::iterator iter, iter_end(settings.end());

        bool found = false;
        for (iter = settings.begin(); iter != iter_end; ++iter) {
            QicsColumnSetting &rfs = *iter;

            if ((rfs.col == col) && (rfs.role == role)) {
                found = true;
                rfs.cell_margin = margin;
                break;
            }
        }

        if (!found) {
            QicsColumnSetting rfs;

            rfs.role = role;
            rfs.col = col;
            rfs.cell_margin = margin;

            settings.push_back(rfs);
        }

        invalidateColumnWidth(mcol);

        int new_width = columnWidth(mcol, !visual_coords, visual_coords,
            true);
        bool changed = (cur_width != new_width);

        changed |= computeDefaultRowFontHeight();

        if (changed) {
            int mvcol = (!visual_coords ? gridInfo()->visualColumnIndex(col) : col);
            emit dimensionChanged(Qics::ColumnIndex, mvcol, mvcol);
        }
    }
}

void QicsDimensionManager::setCellMargin(QicsGridType grid_type,
                                    int row, int col, bool visual_coords,
                                    int margin)
{
    int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);
    int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);
    int cur_height = rowHeight(mrow, !visual_coords, visual_coords);
    int cur_width = columnWidth(mcol, !visual_coords, visual_coords);

    QicsGridRole role = gridRole(grid_type);

    QicsCellSettingV *pSettings;
    pSettings = (visual_coords ? &mySetVisualCells : &mySetCells);
    QicsCellSettingV &settings = *pSettings;

    QicsCellSettingV::iterator iter, iter_end(settings.end());

    bool found = false;

    // Search for this cell in the list
    for (iter = settings.begin(); iter != iter_end; ++iter) {
        QicsCellSetting setting = *iter;

        if ((setting.row == row) && (setting.col == col) && (setting.role == role)) {
            setting.cell_margin = margin;

            found = true;
            break;
        }
    }

    if (!found) {
        // Didn't find it, must be a new cell

        QicsCellSetting setting;

        setting.role = role;
        setting.row = row;
        setting.col = col;
        setting.cell_margin = margin;

        settings.push_back(setting);
    }

    bool changedRow = false;
    bool changedColumn = false;

    if (myRowDM)
        myRowDM->setCellMargin(grid_type, row, col, visual_coords, margin);
    else {
        invalidateRowHeight(mrow);

        int new_height = rowHeight(mrow, !visual_coords, visual_coords, true);
        changedRow |= (cur_height != new_height);
    }

    if (myColumnDM)
        myColumnDM->setCellMargin(grid_type, row, col, visual_coords, margin);
    else {
        invalidateColumnWidth(mcol);

        int new_width = columnWidth(mcol, !visual_coords, visual_coords,
            true);
        changedColumn |= (cur_width != new_width);
    }

    if (changedRow && changedColumn)
        emit dimensionChanged();
    else if (changedRow) {
        int mvrow = (!visual_coords ? gridInfo()->visualRowIndex(row) : row);
        emit dimensionChanged(Qics::RowIndex, mvrow, mvrow);
    }
    else if (changedColumn) {
        int mvcol = (!visual_coords ? gridInfo()->visualColumnIndex(col) : col);
        emit dimensionChanged(Qics::ColumnIndex, mvcol, mvcol);
    }
}

void QicsDimensionManager::setRepeatingRowMargin(QicsGridType grid_type,
                                            int row, unsigned int interval,
                                            int margin)
{
    if (myRowDM)
        myRowDM->setRepeatingRowMargin(grid_type, row, interval, margin);
    else {
        QicsGridRole role = gridRole(grid_type);

        QicsRepeatingRowSettingV::iterator iter, iter_end(mySetRepeatingRows.end());

        bool found = false;
        for (iter = mySetRepeatingRows.begin(); iter != iter_end; ++iter) {
            QicsRepeatingRowSetting &rfs = *iter;

            if ((rfs.row == row) && (rfs.interval == interval) && (rfs.role == role)) {
                found = true;
                rfs.cell_margin = margin;
                break;
            }
        }

        if (!found) {
            QicsRepeatingRowSetting rfs;

            rfs.role = role;
            rfs.row = row;
            rfs.interval = interval;
            rfs.cell_margin = margin;

            mySetRepeatingRows.push_back(rfs);
        }

        emit dimensionChanged();
    }
}

void QicsDimensionManager::setRepeatingColumnMargin(QicsGridType grid_type,
                                               int col, unsigned int interval,
                                               int margin)
{
    if (myColumnDM)
        myColumnDM->setRepeatingColumnMargin(grid_type, col, interval, margin);
    else {
        QicsGridRole role = gridRole(grid_type);

        QicsRepeatingColumnSettingV::iterator iter, iter_end(mySetRepeatingColumns.end());

        bool found = false;
        for (iter = mySetRepeatingColumns.begin(); iter != iter_end; ++iter) {
            QicsRepeatingColumnSetting &rcs = *iter;

            if ((rcs.col == col) && (rcs.interval == interval) && (rcs.role == role)) {
                found = true;
                rcs.cell_margin = margin;
                break;
            }
        }

        if (!found) {
            QicsRepeatingColumnSetting rcs;

            rcs.role = role;
            rcs.col = col;
            rcs.interval = interval;
            rcs.cell_margin = margin;

            mySetRepeatingColumns.push_back(rcs);
        }

        emit dimensionChanged();
    }
}

/////////////////////////////////////////////////////////////////////////
//////////////////// Border width  methods //////////////////////////////
/////////////////////////////////////////////////////////////////////////

void QicsDimensionManager::setDefaultBorderWidth(int bw)
{
    myCurrentDefaultDimensions.border_width = bw;

    computeAllRowHeights();
    computeAllColumnWidths();
}

void QicsDimensionManager::setRowBorderWidth(QicsGridType grid_type,
                                        int row, bool visual_coords,
                                        int bw)
{
    if (myRowDM)
        myRowDM->setRowBorderWidth(grid_type, row, visual_coords, bw);
    else {
        int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);
        int cur_height = rowHeight(mrow, !visual_coords, visual_coords);

        QicsGridRole role = gridRole(grid_type);

        QicsRowSettingV *pSettings;
        pSettings = (visual_coords ? &mySetVisualRows : &mySetRows);
        QicsRowSettingV &settings = *pSettings;

        QicsRowSettingV::iterator iter, iter_end(settings.end());

        bool found = false;
        for (iter = settings.begin(); iter != iter_end; ++iter) {
            QicsRowSetting &rfs = *iter;

            if ((rfs.row == row) && (rfs.role == role)) {
                found = true;
                rfs.border_width = bw;
                break;
            }
        }

        if (!found) {
            QicsRowSetting rfs;

            rfs.role = role;
            rfs.row = row;
            rfs.border_width = bw;

            settings.push_back(rfs);
        }

        invalidateRowHeight(mrow);

        int new_height = rowHeight(mrow, !visual_coords, visual_coords, true);

        bool changed = (cur_height != new_height);

        changed |= computeDefaultColumnFontWidth();

        if (changed) {
            int mvrow = (!visual_coords ? gridInfo()->visualRowIndex(row) : row);
            emit dimensionChanged(Qics::RowIndex, mvrow, mvrow);
        }
    }
}

void QicsDimensionManager::setColumnBorderWidth(QicsGridType grid_type,
                                           int col, bool visual_coords,
                                           int bw)
{
    if (myColumnDM)
        myColumnDM->setColumnBorderWidth(grid_type, col, visual_coords, bw);
    else {
        int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);
        int cur_width = columnWidth(mcol, !visual_coords, visual_coords);

        QicsGridRole role = gridRole(grid_type);

        QicsColumnSettingV *pSettings;
        pSettings = (visual_coords ? &mySetVisualColumns : &mySetColumns);
        QicsColumnSettingV &settings = *pSettings;

        QicsColumnSettingV::iterator iter, iter_end(settings.end());

        bool found = false;
        for (iter = settings.begin(); iter != iter_end; ++iter) {
            QicsColumnSetting &rfs = *iter;

            if ((rfs.col == col) && (rfs.role == role)) {
                found = true;
                rfs.border_width = bw;
                break;
            }
        }

        if (!found) {
            QicsColumnSetting rfs;

            rfs.role = role;
            rfs.col = col;
            rfs.border_width = bw;

            settings.push_back(rfs);
        }

        invalidateColumnWidth(mcol);

        int new_width = columnWidth(mcol, !visual_coords, visual_coords,true);
        bool changed = (cur_width != new_width);

        changed |= computeDefaultRowFontHeight();

        if (changed) {
            int mvcol = (!visual_coords ? gridInfo()->visualColumnIndex(col) : col);
            emit dimensionChanged(Qics::ColumnIndex, mvcol, mvcol);
        }
    }
}

void QicsDimensionManager::setCellBorderWidth(QicsGridType grid_type,
                                         int row, int col, bool visual_coords,
                                         int bw)
{
    int mrow = (visual_coords ? gridInfo()->modelRowIndex(row) : row);
    int mcol = (visual_coords ? gridInfo()->modelColumnIndex(col) : col);
    int cur_height = rowHeight(mrow, !visual_coords, visual_coords);
    int cur_width = columnWidth(mcol, !visual_coords, visual_coords);

    QicsGridRole role = gridRole(grid_type);

    QicsCellSettingV *pSettings;
    pSettings = (visual_coords ? &mySetVisualCells : &mySetCells);
    QicsCellSettingV &settings = *pSettings;

    QicsCellSettingV::iterator iter, iter_end(settings.end());

    bool found = false;

    // Search for this cell in the list
    for (iter = settings.begin(); iter != iter_end; ++iter) {
        QicsCellSetting setting = *iter;

        if ((setting.row == row) && (setting.col == col) && (setting.role == role)) {
            setting.border_width = bw;

            found = true;
            break;
        }
    }

    if (!found) {
        // Didn't find it, must be a new cell

        QicsCellSetting setting;

        setting.role = role;
        setting.row = row;
        setting.col = col;
        setting.border_width = bw;

        settings.push_back(setting);
    }

    bool changedRow = false;
    bool changedColumn = false;

    if (myRowDM)
        myRowDM->setCellBorderWidth(grid_type, row, col, visual_coords, bw);
    else {
        invalidateRowHeight(mrow);

        int new_height = rowHeight(mrow, !visual_coords, visual_coords, true);
        changedRow |= (cur_height != new_height);
    }

    if (myColumnDM)
        myColumnDM->setCellBorderWidth(grid_type, row, col, visual_coords, bw);
    else {
        invalidateColumnWidth(mcol);

        int new_width = columnWidth(mcol, !visual_coords, visual_coords,
            true);
        changedColumn |= (cur_width != new_width);
    }

    if (changedRow && changedColumn)
        emit dimensionChanged();
    else if (changedRow) {
        int mvrow = (!visual_coords ? gridInfo()->visualRowIndex(row) : row);
        emit dimensionChanged(Qics::RowIndex, mvrow, mvrow);
    }
    else if (changedColumn) {
        int mvcol = (!visual_coords ? gridInfo()->visualColumnIndex(col) : col);
        emit dimensionChanged(Qics::ColumnIndex, mvcol, mvcol);
    }
}

void QicsDimensionManager::setRepeatingRowBorderWidth(QicsGridType grid_type,
                                                 int row, unsigned int interval,
                                                 int bw)
{
    if (myRowDM)
        myRowDM->setRepeatingRowBorderWidth(grid_type, row, interval, bw);
    else {
        QicsGridRole role = gridRole(grid_type);

        QicsRepeatingRowSettingV::iterator iter, iter_end(mySetRepeatingRows.end());

        bool found = false;
        for (iter = mySetRepeatingRows.begin(); iter != iter_end; ++iter) {
            QicsRepeatingRowSetting &rfs = *iter;

            if ((rfs.row == row) && (rfs.interval == interval) && (rfs.role == role)) {
                found = true;
                rfs.border_width = bw;
                break;
            }
        }

        if (!found) {
            QicsRepeatingRowSetting rfs;

            rfs.role = role;
            rfs.row = row;
            rfs.interval = interval;
            rfs.border_width = bw;

            mySetRepeatingRows.push_back(rfs);
        }

        emit dimensionChanged();
    }
}

void QicsDimensionManager::setRepeatingColumnBorderWidth(QicsGridType grid_type,
                                                    int col, unsigned int interval,
                                                    int bw)
{
    if (myColumnDM)
        myColumnDM->setRepeatingColumnBorderWidth(grid_type, col, interval, bw);
    else {
        QicsGridRole role = gridRole(grid_type);

        QicsRepeatingColumnSettingV::iterator iter, iter_end(mySetRepeatingColumns.end());

        bool found = false;
        for (iter = mySetRepeatingColumns.begin(); iter != iter_end; ++iter) {
            QicsRepeatingColumnSetting &rcs = *iter;

            if ((rcs.col == col) && (rcs.interval == interval) && (rcs.role == role)) {
                found = true;
                rcs.border_width = bw;
                break;
            }
        }

        if (!found) {
            QicsRepeatingColumnSetting rcs;

            rcs.role = role;
            rcs.col = col;
            rcs.interval = interval;
            rcs.border_width = bw;

            mySetRepeatingColumns.push_back(rcs);
        }

        emit dimensionChanged();
    }
}

////////////////////////////////////////////////////////////////////
/////////////////  Height/Width retrieval methods  /////////////////
////////////////////////////////////////////////////////////////////

int QicsDimensionManager::rowHeight(int row,
                                bool use_model_settings,
                                bool use_visual_settings,
                                bool force_calc) const
{
    if (myRowDM)
        return (myRowDM->rowHeight(row, use_model_settings,use_visual_settings));

    int mrow = (use_model_settings ? row : -1);

    if (mrow >= 0) {
        // Check for overridden row height
        int orh = overriddenRowHeight(mrow);
        if (orh >= 0)
            return orh;
    }

    QicsDimensionManager *self = const_cast<QicsDimensionManager *> (this);

    bool have_visual_settings = ((myVisualRowHeights.size() > 0) ||
        (mySetVisualRows.size() > 0) ||
        (mySetVisualCells.size() > 0) ||
        (mySetRepeatingRows.size() > 0) ||
        (myRepeatingRowHeights.size() > 0));


    int height = myCurrentDefaultDimensions.height;

    if (have_visual_settings && use_visual_settings) {
        // Need to do this computation every time...
        int vrow = gridInfo()->visualRowIndex(row);

        height = self->computeRowHeight(mrow, self->myRowHeights,
            vrow, self->myVisualRowHeights,
            self->myCurrentDefaultDimensions);
    }
    else {
        QicsRowHeight *rh = 0;

        if (use_model_settings && (row >= 0) && (row < myRowHeights.size()))
            rh = myRowHeights.at(row);

        if (force_calc || (rh && !rh->valid)) {
            height = self->computeRowHeight(mrow, self->myRowHeights,
                -1, self->myVisualRowHeights,
                self->myCurrentDefaultDimensions);
        }
        else if (rh)
            height = rh->height;
    }

    if (myRowDM) {
        int h = myRowDM->rowHeight(row, use_model_settings, use_visual_settings);
        if (h > height || h == height) return h;
        myRowDM->setRowHeightInPixels(row, false, height);
    }

    return height;
}

int QicsDimensionManager::columnWidth(int col,
                                  bool use_model_settings,
                                  bool use_visual_settings,
                                  bool force_calc) const
{
    if (myColumnDM)
        return (myColumnDM->columnWidth(col, use_model_settings, use_visual_settings));

    int mcol = (use_model_settings ? col : -1);

    if (mcol >= 0) {
        // Check for overridden column width
        int ocw = overriddenColumnWidth(mcol);
        if (ocw >= 0)
            return ocw;
    }

    QicsDimensionManager *self = const_cast<QicsDimensionManager *> (this);

    bool have_visual_settings = ((myVisualColumnWidths.size() > 0) ||
        (mySetVisualColumns.size() > 0) ||
        (mySetVisualCells.size() > 0) ||
        (mySetRepeatingColumns.size() > 0) ||
        (myRepeatingColumnWidths.size()));

    int width = myCurrentDefaultDimensions.width;

    if (have_visual_settings && use_visual_settings) {
        // Need to do this computation every time...
        int vcol = gridInfo()->visualColumnIndex(col);

        width = self->computeColumnWidth(mcol, self->myColumnWidths,
            vcol, self->myVisualColumnWidths,
            self->myCurrentDefaultDimensions);
    }
    else {
        QicsColumnWidth *cw = 0;

        if (use_model_settings && (col >= 0) && (col < myColumnWidths.size()))
            cw = myColumnWidths.at(col);

        if (force_calc || (cw && !cw->valid)) {
            width = self->computeColumnWidth(mcol, self->myColumnWidths,
                -1, self->myVisualColumnWidths,
                self->myCurrentDefaultDimensions);
        }
        else if (cw)
            width = cw->width;
    }

    if (myColumnDM) {
        int w = myColumnDM->columnWidth(col, use_model_settings, use_visual_settings);
        if (w > width || w == width) return w;
        myColumnDM->setColumnWidthInPixels(col, false, width);
    }

    return width;
}

int QicsDimensionManager::rowMinHeight(int row, bool use_model_settings,
                                   bool use_visual_settings) const
{
    if (myRowDM)
        return (myRowDM->rowMinHeight(row, use_model_settings, use_visual_settings));

    int vrow = gridInfo()->visualRowIndex(row);

    QicsRowHeight *rh = 0;
    int height;

    if (use_visual_settings && (vrow >= 0) && (vrow < myVisualRowMinHeights.size()))
        rh = myVisualRowMinHeights.at(vrow);

    if (!rh && use_model_settings && (row >= 0) && (row < myRowMinHeights.size()))
        rh = myRowMinHeights.at(row);

    if (rh)
        height = rh->height;
    else
        height = myCurrentDefaultMinDimensions.height;

    if (myRowDM) {
        int h = myRowDM->rowMinHeight(row, use_model_settings, use_visual_settings);
        if (h < height || h == height) return h;
        myRowDM->setRowMinHeightInPixels(row, false, height);
    }

    return height;
}

int QicsDimensionManager::rowMaxHeight(int row, bool use_model_settings,
                                   bool use_visual_settings) const
{
    if (myRowDM)
        return (myRowDM->rowMaxHeight(row, use_model_settings, use_visual_settings));

    int vrow = gridInfo()->visualRowIndex(row);

    QicsRowHeight *rh = 0;
    int height;

    if (use_visual_settings && (vrow >= 0) && (vrow < myVisualRowMaxHeights.size()))
        rh = myVisualRowMaxHeights.at(vrow);

    if (!rh && use_model_settings && (row >= 0) && (row < myRowMaxHeights.size()))
        rh = myRowMaxHeights.at(row);

    if (rh)
        height = rh->height;
    else
        height = myCurrentDefaultMaxDimensions.height;

    if (myRowDM) {
        int h = myRowDM->rowMaxHeight(row, use_model_settings, use_visual_settings);
        if (h > height || h == height) return h;
        myRowDM->setRowMaxHeightInPixels(row, false, height);
    }

    return height;
}

int QicsDimensionManager::columnMinWidth(int col, bool use_model_settings,
                                     bool use_visual_settings) const
{
    if (myColumnDM)
        return (myColumnDM->columnMinWidth(col, use_model_settings, use_visual_settings));

    int vcol = gridInfo()->visualColumnIndex(col);

    QicsColumnWidth *cw = 0;
    int width;

    if (use_visual_settings && (vcol >= 0) && (vcol < myVisualColumnMinWidths.size()))
        cw = myVisualColumnMinWidths.at(vcol);

    if (!cw && use_model_settings && (col >= 0) && col < myColumnMinWidths.size())
        cw = myColumnMinWidths.at(col);

    if (cw)
        width = cw->width;
    else
        width = myCurrentDefaultMinDimensions.width;

    if (myColumnDM) {
        int w = myColumnDM->columnMinWidth(col, use_model_settings, use_visual_settings);
        if (w < width || w == width) return w;
        myColumnDM->setColumnMinWidthInPixels(col, false, width);
    }

    return width;
}

int QicsDimensionManager::columnMaxWidth(int col, bool use_model_settings,
                                     bool use_visual_settings) const
{
    if (myColumnDM)
        return (myColumnDM->columnMaxWidth(col, use_model_settings, use_visual_settings));

    int vcol = gridInfo()->visualColumnIndex(col);

    QicsColumnWidth *cw = 0;
    int width;

    if (use_visual_settings && (vcol >= 0) && (vcol < myVisualColumnMaxWidths.size()))
        cw = myVisualColumnMaxWidths.at(vcol);

    if (!cw && use_model_settings && (col >= 0) && col < myColumnMaxWidths.size())
        cw = myColumnMaxWidths.at(col);

    if (cw)
        width = cw->width;
    else
        width = myCurrentDefaultMaxDimensions.width;

    if (myColumnDM) {
        int w = myColumnDM->columnMaxWidth(col, use_model_settings, use_visual_settings);
        if (w > width || w == width) return w;
        myColumnDM->setColumnMaxWidthInPixels(col, false, width);
    }

    return width;
}

int QicsDimensionManager::regionHeight(const QicsRegion &region) const
{
    int height = 0;
    int num_hidden_rows = 0;
    int i;

    for (i = region.startRow(); i <= region.endRow(); ++i) {
        if (isRowHidden(i))
            ++num_hidden_rows;
        else
            height += rowHeight(i);
    }

    int hlw = * static_cast<int *>
        (styleManager()->getGridProperty(QicsGridStyle::HorizontalGridLineWidth));

    height += (hlw * (region.endRow() - region.startRow() - num_hidden_rows));

    return height;
}

int QicsDimensionManager::regionWidth(const QicsRegion &region) const
{
    int width = 0;
    int num_hidden_cols = 0;
    int j;

    for (j = region.startColumn(); j <= region.endColumn(); ++j) {
        if (isColumnHidden(j))
            ++num_hidden_cols;
        else
            width += columnWidth(j);
    }

    int vlw = * static_cast<int *>
        (styleManager()->getGridProperty(QicsGridStyle::VerticalGridLineWidth));

    width += (vlw * (region.endColumn() - region.startColumn() - num_hidden_cols));

    return width;
}

void QicsDimensionManager::overrideRowHeight(int row, int height)
{
    // Search for existing override
    if (myRowDM) {
        myRowDM->overrideRowHeight(row, height);
        return;
    }

    emit rowResized(row,rowHeight(row),height);

    QicsOverrideSettingL::iterator iter, iter_end(myRowOverrides.end());

    for (iter = myRowOverrides.begin(); iter != iter_end; ++iter) {
        QicsOverrideSetting &os = *iter;

        if (os.index() == row) {
            os.setValue(height);
            emit dimensionChanged(Qics::RowIndex, row, row);
            return;
        }
    }

    // Add the new override
    myRowOverrides.push_front(QicsOverrideSetting(row, height));
    emit dimensionChanged(Qics::RowIndex, row, row);
}

void QicsDimensionManager::overrideColumnWidth(int col, int width)
{
    // Search for existing override
    if (myColumnDM) {
        myColumnDM->overrideColumnWidth(col, width);
        return;
    }

    QicsOverrideSettingL::iterator iter, iter_end(myColumnOverrides.end());
    emit columnResized(col,columnWidth(col),width);

    for (iter = myColumnOverrides.begin(); iter != iter_end; ++iter) {
        QicsOverrideSetting &os = *iter;

        if (os.index() == col) {
            os.setValue(width);
            emit dimensionChanged(Qics::ColumnIndex, col, col);
            return;
        }
    }

    // Add the new override
    myColumnOverrides.push_front(QicsOverrideSetting(col, width));
    emit dimensionChanged(Qics::ColumnIndex, col, col);
}

void QicsDimensionManager::resetRowHeight(int row)
{
    // Search for existing override
    if (myRowDM) {
        myRowDM->resetRowHeight(row);
        return;
    }

    QicsOverrideSettingL::iterator iter, iter_end(myRowOverrides.end());

    for (iter = myRowOverrides.begin(); iter != iter_end; ++iter) {
        QicsOverrideSetting &os = *iter;

        if (os.index() == row) {
            int prev_height = rowHeight(row);
            myRowOverrides.erase(iter);
            emit rowResized(row,prev_height,rowHeight(row));
            emit dimensionChanged(Qics::RowIndex, row, row);
            return;
        }
    }
}

void QicsDimensionManager::resetColumnWidth(int col)
{
    // Search for existing override
    if (myColumnDM) {
        myColumnDM->resetColumnWidth(col);
        return;
    }

    QicsOverrideSettingL::iterator iter, iter_end(myColumnOverrides.end());

    for (iter = myColumnOverrides.begin(); iter != iter_end; ++iter) {
        QicsOverrideSetting &os = *iter;

        if (os.index() == col) {
            int prev_width = columnWidth(col);
            myColumnOverrides.erase(iter);
            emit columnResized(col,prev_width,columnWidth(col));
            emit dimensionChanged(Qics::ColumnIndex, col, col);
            return;
        }
    }
}

int QicsDimensionManager::overriddenRowHeight(int row) const
{
    // Search for existing override
    if (myRowDM) {
        return myRowDM->overriddenRowHeight(row);
    }

    QicsOverrideSettingL::const_iterator iter, iter_end(myRowOverrides.constEnd());

    for (iter = myRowOverrides.constBegin(); iter != iter_end; ++iter) {
        const QicsOverrideSetting &os = *iter;

        if (os.index() == row)
            return os.value();
    }

    return -1;
}

int QicsDimensionManager::overriddenColumnWidth(int col) const
{
    // Search for existing override
    if (myColumnDM) {
        return myColumnDM->overriddenColumnWidth(col);
    }

    QicsOverrideSettingL::const_iterator iter, iter_end(myColumnOverrides.constEnd());

    for (iter = myColumnOverrides.constBegin(); iter != iter_end; ++iter) {
        const QicsOverrideSetting &os = *iter;

        if (os.index() == col)
            return os.value();
    }

    return -1;
}

////////////////////////////////////////////////////////////////////
///////////////   Protected computation methods   //////////////////
////////////////////////////////////////////////////////////////////

int QicsDimensionManager::computeRowHeight(int nlines, int font_height,
                                       int cell_margin, int border_width)
{
    return ((nlines * font_height) + ((nlines - 1) * SPACE_BETWEEN_LINES) +
        (2 * cell_margin) + (2 * border_width));
}

int QicsDimensionManager::computeRowHeight(int row,
                                       QicsRowHeightPV &row_heights,
                                       int visual_row,
                                       QicsRowHeightPV &vrow_heights,
                                       QicsDefaultDimensionSetting &default_dims)
{
    int row_height;
    bool using_vis = false;
    const QicsRowHeight *rh = 0;
    const QicsRowHeight *mrh = 0;

    if ((visual_row >= 0) && (visual_row < vrow_heights.size())) {
        using_vis = true;
        rh = vrow_heights.at(visual_row);
    }

    if (!rh && (visual_row >= 0)) {
        QicsRepeatingRowHeightV::const_iterator iter_rrh, iter_rrh_end(myRepeatingRowHeights.constEnd());

        for (iter_rrh = myRepeatingRowHeights.constBegin(); iter_rrh != iter_rrh_end; ++iter_rrh) {
            const QicsRepeatingRowHeight &rrh = *iter_rrh;

            if (visual_row < rrh.row)
                continue;

            if (((visual_row - rrh.row) % rrh.interval) == 0) {
                rh = &rrh;
                break;
            }
        }
    }

    if ((row >= 0) && (row < row_heights.size())) {
        mrh = row_heights.at(row);

        if (!rh)
            rh = row_heights.at(row);
    }

    QicsDimensionMode mode;
    int chars;

    if (rh && (rh->mode != QicsDimensionUnset)) {
        mode = rh->mode;
        row_height = rh->height;
    }
    else {
        mode = default_dims.mode;
        row_height = default_dims.height;
    }

    // We don't need to do anything else if we are currently set by pixel
    if (mode == QicsDimensionPixel)
        return row_height;

    if (rh && (rh->chars > 0))
        chars = rh->chars;
    else
        chars = default_dims.height_chars;

    if (!using_vis) {
        using_vis = ((mySetVisualRows.size() > 0) &&
            (mySetVisualCells.size() > 0));
    }

    // This is the "default" row height, based on default settings
    // (plus any column settings that affect all rows.  Our row
    // height cannot be less than this.

    row_height = computeRowHeight(chars, default_dims.font_height,
        default_dims.cell_margin,
        default_dims.border_width);

    int font_height = 0;
    int cell_margin = 0;
    int border_width = 0;

    // Search for an explicit setting for this model row
    QicsRowSettingV::const_iterator iter_rs, iter_rs_end(mySetRows.constEnd());
    for (iter_rs = mySetRows.constBegin(); iter_rs != iter_rs_end; ++iter_rs) {
        const QicsRowSetting &setting = *iter_rs;

        if (setting.row == row) {
            if (setting.font_height >= 0)
                font_height = setting.font_height;

            if (setting.cell_margin >= 0)
                cell_margin = setting.cell_margin;

            if (setting.border_width >= 0)
                border_width = setting.border_width;

            break;
        }
    }

    if (visual_row >= 0) {
        // First, look for a repeating row setting that hits this
        // visual row.  This setting will override any model row
        // setting, so we just assign "over-the-top" of any settings
        // that were found previously.

        QicsRepeatingRowSettingV::const_iterator iter_rrs, iter_rrs_end(mySetRepeatingRows.constEnd());

        for (iter_rrs = mySetRepeatingRows.constBegin(); iter_rrs != iter_rrs_end; ++iter_rrs) {
            const QicsRepeatingRowSetting &setting = *iter_rrs;

            if (visual_row < setting.row)
                continue;

            if (((visual_row - setting.row) % setting.interval) == 0) {
                if (setting.font_height >= 0)
                    font_height = setting.font_height;

                if (setting.cell_margin >= 0)
                    cell_margin = setting.cell_margin;

                if (setting.border_width >= 0)
                    border_width = setting.border_width;

                break;
            }
        }

        // Now, search for an explicit setting for this visual row.
        // Visual rows override repeating rows and model rows,
        // so we just assign "over-the-top" of any settings that were
        // found previously.
        QicsRowSettingV::const_iterator iter_rs, iter_rs_end(mySetVisualRows.constEnd());

        for (iter_rs = mySetVisualRows.constBegin(); iter_rs != iter_rs_end; ++iter_rs) {
            const QicsRowSetting &setting = *iter_rs;

            if (setting.row == visual_row) {
                if (setting.font_height >= 0)
                    font_height = setting.font_height;

                if (setting.cell_margin >= 0)
                    cell_margin = setting.cell_margin;

                if (setting.border_width >= 0)
                    border_width = setting.border_width;

                break;
            }
        }
    }

    if (font_height == 0)
        font_height = default_dims.font_height;
    if (cell_margin == 0)
        cell_margin = default_dims.cell_margin;
    if (border_width == 0)
        border_width = default_dims.border_width;

    // Ok, now we compute the row height.  If it's bigger than the default,
    // then make this our new height.

    row_height = qMax(row_height, computeRowHeight(chars, font_height,
        cell_margin, border_width));

    // Now, we need to see if any individual cell settings might make this
    // row even bigger.

    // Search for an explicit setting for any model cells in this row
    // Cell settings can only cause an increase in the row height.

    int c_font_height = font_height;
    int c_cell_margin = cell_margin;
    int c_border_width = border_width;

    QicsCellSettingV::const_iterator iter_cell, iter_cell_end(mySetCells.constEnd());

    for (iter_cell = mySetCells.constBegin(); iter_cell != iter_cell_end; ++iter_cell) {
        const QicsCellSetting &setting = *iter_cell;

        if (setting.row == row) {
            c_font_height = font_height;
            c_cell_margin = cell_margin;
            c_border_width = border_width;

            if (setting.font_height >= 0)
                c_font_height = setting.font_height;

            if (setting.cell_margin >= 0)
                c_cell_margin = setting.cell_margin;

            if (setting.border_width >= 0)
                c_border_width = setting.border_width;

            // Ok, we've got the settings for this model row.  Before we
            // compute, we need to check for settings for this visual row.

            if (visual_row >= 0) {
                int cell_vcol = gridInfo()->visualColumnIndex(setting.col);

                QicsCellSettingV::const_iterator iter_vis_cell, iter_vis_cell_end(mySetVisualCells.constEnd());

                for (iter_vis_cell = mySetVisualCells.constBegin(); iter_vis_cell != iter_vis_cell_end;++iter_vis_cell) {
                    const QicsCellSetting &vsetting = *iter_vis_cell;

                    if ((vsetting.row == visual_row) && (vsetting.col == cell_vcol)) {
                        if (vsetting.font_height >= 0)
                            c_font_height = setting.font_height;

                        if (vsetting.cell_margin >= 0)
                            c_cell_margin = setting.cell_margin;

                        if (vsetting.border_width >= 0)
                            c_border_width = setting.border_width;

                        break;
                    }
                }
            }

            break;
        }
    }

    // Finally, we can compute the row height based on any cell settings.
    // If it's bigger than the current row height, then make this our new height.

    row_height = qMax(row_height, computeRowHeight(chars, c_font_height, c_cell_margin, c_border_width));
    int max_height = rowMaxHeight(row);

    if (row_height > max_height)
        row_height = max_height;

    // If there was no explicit setting for this row, and the row
    // height is the same as the default row height, we don't need
    // to do anything

    if ((row >= 0) && (mrh || (row_height != default_dims.height))) {
        // Otherwise, we store the new row height, but it's only valid
        // in the future if we haven't had to change anything because
        // of a visual row.

        if (row >= row_heights.size())
            row_heights.resize(row + 20);

        if (!row_heights.at(row))
            row_heights[row] = new QicsRowHeight();

        row_heights.at(row)->valid = !using_vis;
        row_heights.at(row)->height = row_height;
    }

    return row_height;
}

int QicsDimensionManager::computeColumnWidth(int nchars, int font_width,
                                         int cell_margin, int border_width)
{
    return ((nchars * font_width) + (2 * cell_margin) + (2 * border_width));
}

int QicsDimensionManager::computeColumnWidth(int col,
                                         QicsColumnWidthPV &col_widths,
                                         int visual_col,
                                         QicsColumnWidthPV &vcol_widths,
                                         QicsDefaultDimensionSetting &default_dims)
{
    int col_width;
    bool using_vis = false;
    const QicsColumnWidth *cw = 0;
    const QicsColumnWidth *mcw = 0;

    if ((visual_col >= 0) && (visual_col < vcol_widths.size())) {
        using_vis = true;
        cw = vcol_widths.at(visual_col);
    }

    if (!cw && (visual_col >= 0)) {
        QicsRepeatingColumnWidthV::const_iterator iter_rcw, iter_rcw_end(myRepeatingColumnWidths.constEnd());

        for (iter_rcw = myRepeatingColumnWidths.constBegin(); iter_rcw != iter_rcw_end; ++iter_rcw) {
            const QicsRepeatingColumnWidth &rcw = *iter_rcw;

            if (visual_col < rcw.col)
                continue;

            if (((visual_col - rcw.col) % rcw.interval) == 0) {
                cw = &rcw;
                break;
            }
        }
    }

    if ((col < col_widths.size()) && !col_widths.isEmpty()) {
        mcw = col_widths.at(col);

        if (!cw)
            cw = col_widths.at(col);
    }

    QicsDimensionMode mode;
    int chars;

    if (cw && (cw->mode != QicsDimensionUnset)) {
        mode = cw->mode;
        col_width = cw->width;
    }
    else {
        mode = default_dims.mode;
        col_width = default_dims.width;
    }

    // We don't need to do anything else if we are currently set by pixel
    if (mode == QicsDimensionPixel)
        return col_width;

    if (cw && (cw->chars > 0))
        chars = cw->chars;
    else
        chars = default_dims.width_chars;

    if (!using_vis) {
        using_vis = ((mySetVisualColumns.size() > 0) &&
            (mySetVisualCells.size() > 0));
    }

    // This is the "default" column width, based on default settings
    // (plus any column settings that affect all columns.  Our column
    // width cannot be less than this.

    col_width = computeColumnWidth(chars, default_dims.font_width,
        default_dims.cell_margin,
        default_dims.border_width);

    int font_width = 0;
    int cell_margin = 0;
    int border_width = 0;

    // Search for an explicit setting for this model column
    QicsColumnSettingV::const_iterator iter_rs, iter_rs_end(mySetColumns.constEnd());

    for (iter_rs = mySetColumns.constBegin(); iter_rs != iter_rs_end; ++iter_rs) {
        const QicsColumnSetting &setting = *iter_rs;

        if (setting.col == col) {
            if (setting.font_width >= 0)
                font_width = setting.font_width;

            if (setting.cell_margin >= 0)
                cell_margin = setting.cell_margin;

            if (setting.border_width >= 0)
                border_width = setting.border_width;

            break;
        }
    }

    if (visual_col >= 0) {
        // First, look for a repeating column setting that hits this
        // visual column.  This setting will override any model column
        // setting, so we just assign "over-the-top" of any settings
        // that were found previously.

        QicsRepeatingColumnSettingV::const_iterator iter_rrs, iter_rrs_end(mySetRepeatingColumns.constEnd());

        for (iter_rrs = mySetRepeatingColumns.constBegin(); iter_rrs != iter_rrs_end; ++iter_rrs) {
            const QicsRepeatingColumnSetting &setting = *iter_rrs;

            if (visual_col < setting.col)
                continue;

            if (((visual_col - setting.col) % setting.interval) == 0) {
                if (setting.font_width >= 0)
                    font_width = setting.font_width;

                if (setting.cell_margin >= 0)
                    cell_margin = setting.cell_margin;

                if (setting.border_width >= 0)
                    border_width = setting.border_width;

                break;
            }
        }

        // Now, search for an explicit setting for this visual column.
        // Visual columns override repeating columns and model columns,
        // so we just assign "over-the-top" of any settings that were
        // found previously.
        QicsColumnSettingV::const_iterator iter_rs, iter_rs_end(mySetVisualColumns.constEnd());
        for (iter_rs = mySetVisualColumns.constBegin(); iter_rs != iter_rs_end; ++iter_rs) {
            const QicsColumnSetting &setting = *iter_rs;

            if (setting.col == visual_col) {
                if (setting.font_width > 0)
                    font_width = setting.font_width;

                if (setting.cell_margin >= 0)
                    cell_margin = setting.cell_margin;

                if (setting.border_width >= 0)
                    border_width = setting.border_width;

                break;
            }
        }
    }

    if (font_width == 0)
        font_width = default_dims.font_width;
    if (cell_margin == 0)
        cell_margin = default_dims.cell_margin;
    if (border_width == 0)
        border_width = default_dims.border_width;

    // Ok, now we compute the column width.  If it's bigger than the default,
    // then make this our new width.

    col_width = qMax(col_width, computeColumnWidth(chars, font_width,cell_margin, border_width));

    // Now, we need to see if any individual cell settings might make this
    // column even bigger.

    // Search for an explicit setting for any model cells in this column
    // Cell settings can only cause an increase in the column width.

    int c_font_width = font_width;
    int c_cell_margin = cell_margin;
    int c_border_width = border_width;
    QicsCellSettingV::const_iterator iter_cell, iter_cell_end(mySetCells.constEnd());

    for (iter_cell = mySetCells.constBegin(); iter_cell != iter_cell_end; ++iter_cell) {
        const QicsCellSetting &setting = *iter_cell;

        if (setting.col == col) {
            c_font_width = font_width;
            c_cell_margin = cell_margin;
            c_border_width = border_width;

            if (setting.font_width >= 0)
                c_font_width = setting.font_width;

            if (setting.cell_margin >= 0)
                c_cell_margin = setting.cell_margin;

            if (setting.border_width >= 0)
                c_border_width = setting.border_width;

            // Ok, we've got the settings for this model column.  Before we
            // compute, we need to check for settings for this visual column.

            if (visual_col >= 0) {
                int cell_vrow = gridInfo()->visualRowIndex(setting.row);

                QicsCellSettingV::const_iterator iter_vis_cell, iter_vis_cell_end(mySetVisualCells.constEnd());

                for (iter_vis_cell = mySetVisualCells.constBegin();iter_vis_cell != iter_vis_cell_end;++iter_vis_cell) {
                    const QicsCellSetting &vsetting = *iter_vis_cell;

                    if ((vsetting.col == visual_col) && (vsetting.row == cell_vrow)) {
                        if (vsetting.font_width > 0)
                            c_font_width = setting.font_width;

                        if (vsetting.cell_margin >= 0)
                            c_cell_margin = setting.cell_margin;

                        if (vsetting.border_width >= 0)
                            c_border_width = setting.border_width;

                        break;
                    }
                }
            }
            break;
        }
    }

    // Finally, we can compute the column width based on any cell settings.
    // If it's bigger than the current column width, then make this our new width.

    col_width = qMax(col_width, computeColumnWidth(chars, c_font_width, c_cell_margin, c_border_width));
    int max_width = columnMaxWidth(col);

    if (col_width > max_width)
        col_width = max_width;

    // If there was no explicit setting for this column, and the column
    // width is the same as the default column width, we don't need
    // to do anything

    if ((col >= 0) && (mcw || (col_width != default_dims.width))) {
        // Otherwise, we store the new column width, but it's only valid
        // in the future if we haven't had to change anything because
        // of a visual column.

        if (col >= col_widths.size())
            col_widths.resize(col + 20);

        if (!col_widths.at(col))
            col_widths[col] = new QicsColumnWidth();

        col_widths.at(col)->valid = !using_vis;
        col_widths.at(col)->width = col_width;
    }

    return col_width;
}

bool QicsDimensionManager::computeDefaultRowFontHeight()
{
    int font_height = 0;

    QicsColumnSettingV::const_iterator iter1, iter1_end(mySetColumns.constEnd());
    for (iter1 = mySetColumns.constBegin(); iter1 != iter1_end; ++iter1) {
        const QicsColumnSetting &setting = *iter1;
        font_height = qMax(font_height, setting.font_height);
    }

    QicsColumnSettingV::const_iterator iter2, iter2_end(mySetVisualColumns.constEnd());
    for (iter2 = mySetVisualColumns.constBegin(); iter2 != iter2_end; ++iter2) {
        const QicsColumnSetting &setting = *iter2;
        font_height = qMax(font_height, setting.font_height);
    }

    if (font_height <= 0)
        font_height = myOrigDefaultDimensions.font_height;

    int old_height = myCurrentDefaultDimensions.height;

    myCurrentDefaultDimensions.font_height = font_height;
    myCurrentDefaultDimensions.compute();

    myCurrentDefaultMinDimensions.font_height = font_height;
    myCurrentDefaultMinDimensions.compute();
    myCurrentDefaultMaxDimensions.font_height = font_height;
    myCurrentDefaultMaxDimensions.compute();

    return (old_height != myCurrentDefaultDimensions.height);
}

bool QicsDimensionManager::computeDefaultColumnFontWidth()
{
    int font_width = 0;

    QicsRowSettingV::const_iterator iter1, iter1_end(mySetRows.constEnd());
    for (iter1 = mySetRows.constBegin(); iter1 != iter1_end; ++iter1) {
        const QicsRowSetting &setting = *iter1;
        font_width = qMax(font_width, setting.font_width);
    }

    QicsRowSettingV::const_iterator iter2, iter2_end(mySetVisualRows.constEnd());
    for (iter2 = mySetVisualRows.constBegin(); iter2 != iter2_end; ++iter2) {
        const QicsRowSetting &setting = *iter2;
        font_width = qMax(font_width, setting.font_width);
    }

    if (font_width <= 0)
        font_width = myOrigDefaultDimensions.font_width;

    int old_width = myCurrentDefaultDimensions.width;

    myCurrentDefaultDimensions.font_width = font_width;
    myCurrentDefaultDimensions.compute();

    myCurrentDefaultMinDimensions.font_width = font_width;
    myCurrentDefaultMinDimensions.compute();

    myCurrentDefaultMaxDimensions.font_width = font_width;
    myCurrentDefaultMaxDimensions.compute();

    return (old_width != myCurrentDefaultDimensions.width);
}

bool QicsDimensionManager::computeAllRowHeights()
{
    bool changed = false;
    int i;

    for (i = 0; i < myRowHeights.size(); ++i) {
        const QicsRowHeight *rh = myRowHeights.at(i);
        if (rh) {
            int height = rh->height;

            computeRowHeight(i, myRowHeights,
                -1, myVisualRowHeights,
                myCurrentDefaultDimensions);

            changed |= (rh->height != height);
        }
    }

    for (i = 0; i < myRowMinHeights.size(); ++i) {
        computeRowHeight(i, myRowMinHeights,
            -1, myVisualRowMinHeights,
            myCurrentDefaultMinDimensions);
    }

    for (i = 0; i < myRowMaxHeights.size(); ++i) {
        computeRowHeight(i, myRowMaxHeights,
            -1, myVisualRowMaxHeights,
            myCurrentDefaultMaxDimensions);
    }

    return changed;
}

bool
QicsDimensionManager::computeAllColumnWidths()
{
    bool changed = false;
    int i;

    for (i = 0; i < myColumnWidths.size(); ++i) {
        const QicsColumnWidth *cw = myColumnWidths.at(i);
        if (cw) {
            int width = cw->width;

            computeColumnWidth(i, myColumnWidths,
                -1, myVisualColumnWidths,
                myCurrentDefaultDimensions);

            changed |= (cw->width != width);
        }
    }

    for (i = 0; i < myColumnMinWidths.size(); ++i) {
        computeColumnWidth(i, myColumnMinWidths,
            -1, myVisualColumnMinWidths,
            myCurrentDefaultMinDimensions);
    }

    for (i = 0; i < myColumnMaxWidths.size(); ++i) {
        computeColumnWidth(i, myColumnMaxWidths,
            -1, myVisualColumnMaxWidths,
            myCurrentDefaultMaxDimensions);
    }

    return changed;
}

void QicsDimensionManager::invalidateRowHeight(int row)
{
    if ((row >= 0) && (row < myRowHeights.size())) {
        QicsRowHeight *rh = myRowHeights.at(row);

        if (rh)
            rh->valid = false;
    }
}

void QicsDimensionManager::invalidateColumnWidth(int col)
{
    if ((col >= 0) && (col < myColumnWidths.size())) {
        QicsColumnWidth *cw = myColumnWidths.at(col);

        if (cw)
            cw->valid = false;
    }
}

////////////////////////////////////////////////////////////////////////
///////////////////   Debugging methods   //////////////////////////////
////////////////////////////////////////////////////////////////////////
#ifdef notdef

void
QicsDimensionManager::dumpWidths() const
{
    int i;

    qDebug("Default Column Width:  %d", myCurrentDefaultDimensions.width);

    qDebug("Model Column Widths");
    for (i = 0; i < myColumnWidths.size(); ++i)
    {
        if (myColumnWidths.at(i))
        {
            qDebug("\tcol %d:  %d", i, myColumnWidths.at(i)->width);
        }
    }

    qDebug("Visual Column Widths");
    for (i = 0; i < myVisualColumnWidths.size(); ++i)
    {
        if (myVisualColumnWidths.at(i))
        {
            qDebug("\tcol %d:  %d", i, myVisualColumnWidths.at(i)->width);
        }
    }
}

void
QicsDimensionManager::dumpHeights() const
{
    int i;

    qDebug("Default Row Height:  %d", myCurrentDefaultDimensions.height);

    qDebug("Model Row Heights");
    for (i = 0; i < myRowHeights.size(); ++i)
    {
        if (myRowHeights.at(i))
        {
            qDebug("\trow %d:  %d", i, myRowHeights.at(i)->height);
        }
    }

    qDebug("Visual Row Heights");
    for (i = 0; i < myVisualRowHeights.size(); ++i)
    {
        if (myVisualRowHeights.at(i))
        {
            qDebug("\trow %d:  %d", i, myVisualRowHeights.at(i)->height);
        }
    }
}

#endif

////////////////////////////////////////////////////////////////////////
/////////////////      Hidden Rows and Columns      ////////////////////
////////////////////////////////////////////////////////////////////////

void QicsDimensionManager::hideRow(int row)
{
    if (myRowDM) {
        myRowDM->hideRow(row);
        return;
    }

    // first make sure it's not already there
    if (myHiddenRows.contains(row))
        return;

    myHiddenRows.insert(row);

    if (myEmitSignalsFlag) {
        emit rowVisibilityChanged(row, false);
        emit dimensionChanged(Qics::RowIndex, row, row);
    }
}

bool QicsDimensionManager::hasHiddenRows() const
{
    return !myHiddenRows.isEmpty();
}

void QicsDimensionManager::hideColumn(int col)
{
    if (myColumnDM) {
        myColumnDM->hideColumn(col);
        return;
    }

    // first make sure it's not already there
    if (myHiddenColumns.contains(col))
        return;

    myHiddenColumns.insert(col);

    if (myEmitSignalsFlag) {
        emit columnVisibilityChanged(col, false);
        emit dimensionChanged(Qics::ColumnIndex, col, col);
    }
}

bool QicsDimensionManager::hasHiddenColumns() const
{
    return !myHiddenColumns.isEmpty();
}

void QicsDimensionManager::showRow(int row)
{
    if (myRowDM) {
        myRowDM->showRow(row);
        return;
    }

    if (myHiddenRows.remove(row) && myEmitSignalsFlag) {
        emit rowVisibilityChanged(row, true);
        emit dimensionChanged(Qics::RowIndex, row, row);
    }
}

void QicsDimensionManager::showColumn(int col)
{
    if (myColumnDM) {
        myColumnDM->showColumn(col);
        return;
    }

    if (myHiddenColumns.remove(col) && myEmitSignalsFlag) {
        emit columnVisibilityChanged(col, true);
        emit dimensionChanged(Qics::ColumnIndex, col, col);
    }
}

bool QicsDimensionManager::isRowHidden(int row) const
{
    if (row < 0)
        return true;

    if (myRowDM)
        return (myRowDM->isRowHidden(row));

    return myHiddenRows.contains(row);
}

bool QicsDimensionManager::isRowFiltered(int row) const
{
    if (myRowDM)
        return (myRowDM->isRowFiltered(row));

    return myGridInfo->rowFilter()->isFiltered(row);
}

bool QicsDimensionManager::isColumnHidden(int col) const
{
    if (col < 0)
        return true;

    if (myColumnDM)
        return (myColumnDM->isColumnHidden(col));

    return myHiddenColumns.contains(col);
}

//////////////////////////////////////////////////////////////////////
/////////////////////  Resizing Slots  ///////////////////////////////
//////////////////////////////////////////////////////////////////////

void QicsDimensionManager::insertRows(int num, int start_position)
{
    if (myGridInfo->ignoreModelSizeChanges())
        return;

    // If another dimension manager is handling rows,
    // we don't need to do anything.
    if (myRowDM)
        return;

    // make sure we are in bounds..
    // and no one is trying to clobber us with a bad number of rows.
    if ((start_position < 0) || (num <= 0))
        return;

    // First, we do the row heights

    if (start_position < myRowHeights.size()) {
        QicsRowHeightPV::iterator pos = myRowHeights.begin() + start_position;
        myRowHeights.insert(pos, num, 0);
    }

    // Second, we do the row min heights

    if (start_position < myRowMinHeights.size()) {
        QicsRowHeightPV::iterator pos = myRowMinHeights.begin() + start_position;
        myRowMinHeights.insert(pos, num, 0);
    }

    // And we do the row max heights
    if (start_position < myRowMaxHeights.size()) {
        QicsRowHeightPV::iterator pos = myRowMaxHeights.begin() + start_position;
        myRowMaxHeights.insert(pos, num, 0);
    }

    // Then Hide setings
    QList<int> list = myHiddenRows.toList();
    qSort(list.begin(), list.end());    // we MUST sort the list - as QSet is unsorted internally
    QList<int>::iterator iter, iter_end(list.end());

    for (iter = list.begin(); iter != iter_end; ++iter) {
        if ( *iter >= start_position )
            *iter += num;
    }
    myHiddenRows = list.toSet();

    // Next, any row settings
    QicsRowSettingV::iterator iter_rs, iter_rs_end(mySetRows.end());

    for (iter_rs = mySetRows.begin(); iter_rs != iter_rs_end; ++iter_rs) {
        QicsRowSetting &setting = *iter_rs;

        if (setting.row >= start_position)
            setting.row += num;
    }

    // Finally, any cell settings
    QicsCellSettingV::iterator iter_cell, iter_cell_end(mySetCells.end());

    for (iter_cell = mySetCells.begin(); iter_cell != iter_cell_end; ++iter_cell) {
        QicsCellSetting &setting = *iter_cell;

        if (setting.row >= start_position)
            setting.row += num;
    }

    // fix font vector
    if (myFontSizeVector.size() > start_position)
        myFontSizeVector.insert(start_position, num, QMap<int,int>());

    // And set Current cell to correct
/*    int col = myGridInfo->currentCell().column();
    int row = myGridInfo->currentCell().row();
    QicsICell cell(row, col);

    if (start_position <= myGridInfo->modelRowIndex(row) && row != -1 && col !=-1) {
        cell.setRow(row+num);
        cell.setColumn(col);
        if (cell.isValid())
            myGridInfo->setCurrentCell(cell);
    }*/

    // update row filtering
    myGridInfo->rowFilter()->handleRowsAdded(num, start_position);
}

void QicsDimensionManager::insertColumns(int num, int start_position)
{
    if (myGridInfo->ignoreModelSizeChanges())
        return;

    // If another dimension manager is handling columns,
    // we don't need to do anything.
    if (myColumnDM)
        return;

    // make sure we are in bounds..
    // and no one is trying to clobber us with a bad number of columns.
    if ((start_position < 0) || (num <= 0))
        return;

    // First, we do the column widths

    if (start_position < myColumnWidths.size()) {
        QicsColumnWidthPV::iterator pos = myColumnWidths.begin() + start_position;
        myColumnWidths.insert(pos, num, 0);
    }

    // Second, we do the column min widths
    if (start_position < myColumnMinWidths.size()) {
        QicsColumnWidthPV::iterator pos = myColumnMinWidths.begin() + start_position;
        myColumnMinWidths.insert(pos, num, 0);
    }

    // We do the column max widths
    if (start_position < myColumnMaxWidths.size()) {
        QicsColumnWidthPV::iterator pos = myColumnMaxWidths.begin() + start_position;
        myColumnMaxWidths.insert(pos, num, 0);
    }

    // Next, any column settings
    QicsColumnSettingV::iterator iter_cs, iter_cs_end(mySetColumns.end());
    for (iter_cs = mySetColumns.begin(); iter_cs != iter_cs_end; ++iter_cs) {
        QicsColumnSetting &setting = *iter_cs;
        if (setting.col >= start_position)
            setting.col += num;
    }

    // Then Hide setings
    QList<int> list = myHiddenColumns.toList();
    qSort(list.begin(), list.end());    // we MUST sort the list - as QSet is unsorted internally
    QList<int>::iterator iter, iter_end(list.end());

    for (iter = list.begin(); iter != iter_end; ++iter) {
        if ( *iter >= start_position )
            *iter += num;
    }

    myHiddenColumns = list.toSet();

    // Finally, any cell settings
    QicsCellSettingV::iterator iter_cell, iter_cell_end(mySetCells.end());

    for (iter_cell = mySetCells.begin(); iter_cell != iter_cell_end; ++iter_cell) {
        QicsCellSetting &setting = *iter_cell;

        if (setting.col >= start_position)
            setting.col += num;
    }

    // And set Current cell to correct
    /*int col = myGridInfo->currentCell().column();
    int row = myGridInfo->currentCell().row();
    QicsICell cell(row, col);

    if (start_position <= myGridInfo->modelColumnIndex(col) && row != -1 && col !=-1) {
        cell.setRow(row);
        cell.setColumn(col+num);
        if (cell.isValid())
            myGridInfo->setCurrentCell(cell);
    }*/

    // update row filtering
    myGridInfo->rowFilter()->handleColumnsAdded(num, start_position);
}

void QicsDimensionManager::deleteRows(int num, int start_position)
{
    if (myGridInfo->ignoreModelSizeChanges())
        return;

    // If another dimension manager is handling rows,
    // we don't need to do anything.
    if (myRowDM)
        return;

    // make sure we are in bounds..
    // and no one is trying to clobber us with a bad number of rows.
    if ((start_position < 0) || (num <= 0))
        return;

    // First, we do the row heights

    if (start_position < myRowHeights.size()) {
        QicsRowHeightPV::iterator start_pos = myRowHeights.begin() + start_position;

        QicsRowHeightPV::iterator end_pos;
        if ((start_position + num) >= myRowHeights.size())
            end_pos = myRowHeights.end();
        else
            end_pos= myRowHeights.begin() + (start_position + num);

        QicsRowHeightPV::iterator iter;
        for (iter = start_pos; iter < end_pos; ++iter)
            delete (*iter);

        myRowHeights.erase(start_pos, end_pos);
    }

    // Second, we do the row min heights
    if (start_position < myRowMinHeights.size()) {
        QicsRowHeightPV::iterator start_pos = myRowMinHeights.begin() + start_position;

        QicsRowHeightPV::iterator end_pos;
        if ((start_position + num) >= myRowHeights.size())
            end_pos = myRowMinHeights.end();
        else
            end_pos= myRowMinHeights.begin() + (start_position + num);

        QicsRowHeightPV::iterator iter;
        for (iter = start_pos; iter < end_pos; ++iter)
            delete (*iter);

        myRowMinHeights.erase(start_pos, end_pos);
    }

    // And, we do the row max heights
    if (start_position < myRowMaxHeights.size()) {
        QicsRowHeightPV::iterator start_pos = myRowMaxHeights.begin() + start_position;

        QicsRowHeightPV::iterator end_pos;
        if ((start_position + num) >= myRowHeights.size())
            end_pos = myRowMaxHeights.end();
        else
            end_pos= myRowMaxHeights.begin() + (start_position + num);

        QicsRowHeightPV::iterator iter;
        for (iter = start_pos; iter < end_pos; ++iter)
            delete (*iter);

        myRowMaxHeights.erase(start_pos, end_pos);
    }

    // Then Hide setings
    QSet<int> tempSet(myHiddenRows);
    myHiddenRows.clear();

    for (QSet<int>::const_iterator iter = tempSet.constBegin(); iter != tempSet.constEnd(); ++iter) {
        if (*iter < start_position) {
            myHiddenRows.insert(*iter);
            continue;
        }

        if (*iter >= start_position+num) {
            myHiddenRows.insert(*iter - num);
            continue;
        }
    }

    // Next, any row settings
    {
        int last = start_position + num;
        QicsRowSettingV::iterator iter = mySetRows.begin();
        QicsRowSettingV::iterator iter_end(mySetRows.end());

        while (iter != iter_end) {
            QicsRowSetting &setting = *iter;

            if (setting.row >= start_position) {
                if (setting.row < last) {
                    mySetRows.erase(iter);
                    iter_end = mySetRows.end();
                }
                else {
                    setting.row -= num;
                    ++iter;
                }
            }
            else
                ++iter;
        }
    }

    // Finally, any cell settings
    {
        int last = start_position + num;
        QicsCellSettingV::iterator iter = mySetCells.begin();
        QicsCellSettingV::iterator iter_end(mySetCells.end());

        while (iter != iter_end) {
            QicsCellSetting &setting = *iter;

            if (setting.row >= start_position) {
                if (setting.row < last) {
                    mySetCells.erase(iter);
                    iter_end = mySetCells.end();
                }
                else {
                    setting.row -= num;
                    ++iter;
                }
            }
            else
                ++iter;
        }
    }

    // fix font vector
    if (myFontSizeVector.size() > start_position)
        myFontSizeVector.remove(start_position, num);

    // And set Current cell to correct
    /*int col = myGridInfo->currentCell().column();
    int row = myGridInfo->currentCell().row();
    QicsICell cell(row, col);

    if (start_position <= myGridInfo->modelRowIndex(row) && row != -1 && col !=-1) {
        if (myGridInfo->modelRowIndex(row) >= start_position+num) {
            cell.setRow(row-num);
            cell.setColumn(col);
            if (cell.isValid())
                myGridInfo->setCurrentCell(cell);
        }
    }*/

    // update row filtering
    myGridInfo->rowFilter()->handleRowsRemoved(num, start_position);
}

void QicsDimensionManager::deleteColumns(int num, int start_position)
{
    if (myGridInfo->ignoreModelSizeChanges())
        return;

    // If another dimension manager is handling columns,
    // we don't need to do anything.
    if (myColumnDM)
        return;

    // make sure we are in bounds..
    // and no one is trying to clobber us with a bad number of columns.
    if ((start_position < 0) || (num <= 0))
        return;

    // First, we do the column widths

    if (start_position < myColumnWidths.size()) {
        QicsColumnWidthPV::iterator start_pos = myColumnWidths.begin() + start_position;

        QicsColumnWidthPV::iterator end_pos;
        if ((start_position + num) >= myColumnWidths.size())
            end_pos = myColumnWidths.end();
        else
            end_pos= myColumnWidths.begin() + (start_position + num);

        myColumnWidths.erase(start_pos, end_pos);
    }

    // Second, we do the column min widths
    if (start_position < myColumnMinWidths.size()) {
        QicsColumnWidthPV::iterator start_pos = myColumnMinWidths.begin() + start_position;

        QicsColumnWidthPV::iterator end_pos;
        if ((start_position + num) >= myColumnMinWidths.size())
            end_pos = myColumnMinWidths.end();
        else
            end_pos= myColumnMinWidths.begin() + (start_position + num);

        myColumnMinWidths.erase(start_pos, end_pos);
    }

    // We do the column max widths
    if (start_position < myColumnMaxWidths.size()) {
        QicsColumnWidthPV::iterator start_pos = myColumnMaxWidths.begin() + start_position;

        QicsColumnWidthPV::iterator end_pos;
        if ((start_position + num) >= myColumnMaxWidths.size())
            end_pos = myColumnMaxWidths.end();
        else
            end_pos= myColumnMaxWidths.begin() + (start_position + num);

        myColumnMaxWidths.erase(start_pos, end_pos);
    }

    // Next, any column settings
    {
        int last = start_position + num;
        QicsColumnSettingV::iterator iter = mySetColumns.begin();
        QicsColumnSettingV::iterator iter_end(mySetColumns.end());

        while (iter != iter_end) {
            QicsColumnSetting &setting = *iter;

            if (setting.col >= start_position) {
                if (setting.col < last) {
                    mySetColumns.erase(iter);
                    iter_end = mySetColumns.end();
                }
                else {
                    setting.col -= num;
                    ++iter;
                }
            }
            else
                ++iter;
        }
    }

    // Then Hide setings
    QSet<int> tempSet(myHiddenColumns);
    myHiddenColumns.clear();
    QSet<int>::const_iterator iter, iter_end(tempSet.constEnd());

    for (iter = tempSet.constBegin(); iter != iter_end; ++iter) {
        if (*iter < start_position) {
            myHiddenColumns.insert(*iter);
            continue;
        }

        if (*iter >= start_position+num) {
            myHiddenColumns.insert(*iter - num);
            continue;
        }
    }

    // Finally, any cell settings
    {
        int last = start_position + num;
        QicsCellSettingV::iterator iter = mySetCells.begin();
        QicsCellSettingV::iterator iter_end(mySetCells.end());

        while (iter != iter_end) {
            QicsCellSetting &setting = *iter;

            if (setting.col >= start_position) {
                if (setting.col < last) {
                    mySetCells.erase(iter);
                    iter_end = mySetCells.end();
                }
                else {
                    setting.col -= num;
                    ++iter;
                }
            }
            else
                ++iter;
        }
    }

    // And set Current cell to correct
    /*int col = myGridInfo->currentCell().column();
    int row = myGridInfo->currentCell().row();
    QicsICell cell(row, col);
    if (start_position <= myGridInfo->modelColumnIndex(col) && row != -1 && col !=-1) {
        if (myGridInfo->modelColumnIndex(col) >= start_position+num) {
            cell.setRow(row);
            cell.setColumn(col-num);
            if (cell.isValid())
                myGridInfo->setCurrentCell(cell);
        }
    }*/

    // update row filtering
    myGridInfo->rowFilter()->handleColumnsRemoved(num, start_position);
}

////////////////////////////////////////////////////////////////////
/////////////////         Configure to Xml         /////////////////
////////////////////////////////////////////////////////////////////

QDomElement QicsDimensionManager::toDomXml(QDomDocument* doc)
{
    QDomElement e = doc->createElement("DimensionManager");
    QString attrName;
    QString attrValue;

    attrName = "hiddenRows";
    QStringList hiddenRowList;

    int row;
    foreach(row, myHiddenRows)
        hiddenRowList << QString::number(row);

    attrValue = hiddenRowList.join(",");
    e.setAttribute(attrName, attrValue);

    attrName = "hiddenColumns";
    QStringList hiddenColumnList;
    int col;

    foreach(col, myHiddenColumns)
        hiddenColumnList << QString::number(col);

    attrValue = hiddenColumnList.join(",");
    e.setAttribute(attrName, attrValue);

    attrName = "lastStretchedRow";
    attrValue = QString::number(myLastStretchedRow);
    e.setAttribute(attrName, attrValue);

    attrName = "lastStretchedColumn";
    attrValue = QString::number(myLastStretchedColumn);
    e.setAttribute(attrName, attrValue);

    attrName = "allowOverrides";
    attrValue = QString::number((int)_allow_override_changes);
    e.setAttribute(attrName, attrValue);

    QDomElement ce = myOrigDefaultDimensions.toDomXml("OrigDefaultDimensions", doc);
    e.appendChild(ce);

    ce =  myCurrentDefaultDimensions.toDomXml("CurrentDefaultDimensions", doc);
    e.appendChild(ce);

    ce = myOrigDefaultMinDimensions.toDomXml("OrigDefaultMinDimensions", doc);
    e.appendChild(ce);

    ce = myOrigDefaultMaxDimensions.toDomXml("OrigDefaultMaxDimensions", doc);
    e.appendChild(ce);

    ce = myCurrentDefaultMinDimensions.toDomXml("CurrentDefaultMinDimensions", doc);;
    e.appendChild(ce);

    ce = myCurrentDefaultMaxDimensions.toDomXml("CurrentDefaultMaxDimensions", doc);;
    e.appendChild(ce);

    ce = saveMySetCellsToDomXml("SetCells", doc);
    e.appendChild(ce);

    ce = saveMySetRowsToDomXml("SetRows", doc);
    e.appendChild(ce);

    ce = saveMySetColumnsToDomXml("SetColumns", doc);
    e.appendChild(ce);

    ce = saveMyRowHeightsToDomXml("RowHeights", doc);
    e.appendChild(ce);

    ce = saveMyColumnWidthsToDomXml("ColumnWidths", doc);
    e.appendChild(ce);

    ce = saveMyMinRowHeightsToDomXml("RowHeights", doc);
    e.appendChild(ce);

    ce = saveMyMinColumnWidthsToDomXml("ColumnWidths", doc);
    e.appendChild(ce);

    ce = saveMyMaxRowHeightsToDomXml("RowHeights", doc);
    e.appendChild(ce);

    ce = saveMyMaxColumnWidthsToDomXml("ColumnWidths", doc);
    e.appendChild(ce);

    ce = saveMySetVisualCellsToDomXml("SetVisualCells", doc);
    e.appendChild(ce);

    ce = saveMySetVisualRowsToDomXml("SetVisualRows", doc);
    e.appendChild(ce);

    ce = saveMySetVisualColumnsToDomXml("SetVisualColumns", doc);
    e.appendChild(ce);

    ce = saveMySetRepeatingRowsToDomXml("SetRepeatingRows", doc);
    e.appendChild(ce);

    ce = saveMySetRepeatingColumnsToDomXml("SetRepeatingColumns", doc);
    e.appendChild(ce);

    ce = saveMyVisualRowHeightsToDomXml("VisualRowHeights", doc);
    e.appendChild(ce);

    ce = saveMyVisualColumnWidthsToDomXml("VisualColumnWidths", doc);
    e.appendChild(ce);

    ce = saveMyRepeatingRowHeightsToDomXml("RepeatingHeights", doc);
    e.appendChild(ce);

    ce = saveMyRepeatingColumnWidthsToDomXml("RepeatingWidths", doc);
    e.appendChild(ce);

    return e;
}

QDomElement QicsDimensionManager::saveMySetCellsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<mySetCells.size(); ++i) {
        QDomElement ce = mySetCells.at(i).toDomXml("CellSetting",doc);
        ce.setAttribute("pos", QString::number(i));
        e.appendChild(ce);
    }

    return e;
}

QDomElement QicsDimensionManager::saveMySetRowsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<mySetRows.size(); ++i) {
        QDomElement ce = mySetRows.at(i).toDomXml("RowSetting",doc);
        ce.setAttribute("pos", QString::number(i));
        e.appendChild(ce);
    }

    return e;
}

QDomElement QicsDimensionManager::saveMySetColumnsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<mySetColumns.size(); ++i) {
        QDomElement ce = mySetColumns.at(i).toDomXml("ColumnSetting",doc);
        ce.setAttribute("pos", QString::number(i));
        e.appendChild(ce);
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyRowHeightsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myRowHeights.size(); ++i) {
        if(myRowHeights.at(i)) {
            QDomElement ce = myRowHeights.at(i)->toDomXml("RowHeight",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyColumnWidthsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myColumnWidths.size(); ++i) {
        if(myColumnWidths.at(i)) {
            QDomElement ce = myColumnWidths.at(i)->toDomXml("ColumnWidth",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyMinRowHeightsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myRowMinHeights.size(); ++i) {
        if(myRowMinHeights.at(i)) {
            QDomElement ce = myRowMinHeights.at(i)->toDomXml("RowMinHeight",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyMaxRowHeightsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myRowMaxHeights.size(); ++i) {
        if(myRowMaxHeights.at(i)) {
            QDomElement ce = myRowMaxHeights.at(i)->toDomXml("RowMinHeight",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyMinColumnWidthsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myColumnMinWidths.size(); ++i) {
        if(myColumnMinWidths.at(i)) {
            QDomElement ce = myColumnMinWidths.at(i)->toDomXml("ColumnMinWidth",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyMaxColumnWidthsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myColumnMaxWidths.size(); ++i) {
        if(myColumnMaxWidths.at(i)) {
            QDomElement ce = myColumnMaxWidths.at(i)->toDomXml("ColumnMinWidth",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyHiddenRowsToDomXml(const QString&, QDomDocument* )
{
    return QDomElement();
}

QDomElement QicsDimensionManager::saveMyHiddenColumnsToDomXml(const QString& , QDomDocument*)
{
    return QDomElement();
}

QDomElement QicsDimensionManager::saveMySetVisualCellsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<mySetVisualCells.size(); ++i) {
        QDomElement ce = mySetVisualCells.at(i).toDomXml("VisualCellSetting",doc);
        ce.setAttribute("pos", QString::number(i));
        e.appendChild(ce);
    }

    return e;
}

QDomElement QicsDimensionManager::saveMySetVisualRowsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<mySetVisualRows.size(); ++i) {
        QDomElement ce = mySetVisualRows.at(i).toDomXml("VisualRowSetting",doc);
        ce.setAttribute("pos", QString::number(i));
        e.appendChild(ce);
    }

    return e;
}

QDomElement QicsDimensionManager::saveMySetVisualColumnsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);
    QicsColumnSetting s;
    foreach(s, mySetVisualColumns) {
        QDomElement ce = s.toDomXml("VisualColumnSetting",doc);
        e.appendChild(ce);
    }

    return e;
}

QDomElement QicsDimensionManager::saveMySetRepeatingRowsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<mySetRepeatingRows.size(); ++i) {
        QDomElement ce = mySetRepeatingRows.at(i).toDomXml("RepeatingRowSetting",doc);
        ce.setAttribute("pos", QString::number(i));
        e.appendChild(ce);
    }

    return e;
}

QDomElement QicsDimensionManager::saveMySetRepeatingColumnsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<mySetRepeatingColumns.size(); ++i) {
        QDomElement ce = mySetRepeatingRows.at(i).toDomXml("RepeatingColumnSetting",doc);
        ce.setAttribute("pos", QString::number(i));
        e.appendChild(ce);
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyVisualRowHeightsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myVisualRowHeights.size(); ++i) {
        if(myVisualRowHeights.at(i)) {
            QDomElement ce = myVisualRowHeights.at(i)->toDomXml("VisualRowHeight",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyVisualColumnWidthsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myVisualColumnWidths.size(); ++i) {
        if(myVisualColumnWidths.at(i)) {
            QDomElement ce = myVisualColumnWidths.at(i)->toDomXml("VisualColumnWidth",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyVisualRowMinHeightsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myVisualRowMinHeights.size(); ++i) {
        if(myVisualRowMinHeights.at(i)) {
            QDomElement ce = myVisualRowMinHeights.at(i)->toDomXml("VisualRowMinHeight",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyVisualRowMaxHeightsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myVisualRowMaxHeights.size(); ++i) {
        if(myVisualRowMaxHeights.at(i)) {
            QDomElement ce = myVisualRowMaxHeights.at(i)->toDomXml("VisualRowMaxHeight",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyVisualColumnMinWidthsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myVisualColumnMinWidths.size(); ++i) {
        if(myVisualColumnMinWidths.at(i)) {
            QDomElement ce = myVisualColumnMinWidths.at(i)->toDomXml("VisualColumnMinWidth",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyVisualColumnMaxWidthsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myVisualColumnMaxWidths.size(); ++i) {
        if(myVisualColumnMaxWidths.at(i)) {
            QDomElement ce = myVisualColumnMaxWidths.at(i)->toDomXml("VisualColumnMaxWidth",doc);
            ce.setAttribute("pos", QString::number(i));
            e.appendChild(ce);
        }
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyRepeatingRowHeightsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myRepeatingRowHeights.size(); ++i) {
        QDomElement ce = myRepeatingRowHeights.at(i).toDomXml("RepeatingRowHeight",doc);
        ce.setAttribute("pos", QString::number(i));
        e.appendChild(ce);
    }

    return e;
}

QDomElement QicsDimensionManager::saveMyRepeatingColumnWidthsToDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement e = doc->createElement(tag);

    for(int i=0; i<myRepeatingColumnWidths.size(); ++i) {
        QDomElement ce = myRepeatingColumnWidths.at(i).toDomXml("RepeatingColumnWidth",doc);
        ce.setAttribute("pos", QString::number(i));
        e.appendChild(ce);
    }

    return e;
}

void QicsDimensionManager::configureFromDomXml(const QDomElement& e)
{

    loadMyHiddenRowsFromDomXml(e);
    loadMyHiddenColumnsFromDomXml(e);

    myLastStretchedRow = e.attribute("lastStretchedRow").toInt();
    myLastStretchedColumn = e.attribute("lastStretchedColumn").toInt();
    _allow_override_changes = (bool)e.attribute("lastStretchedRow").toInt();

    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        QString tag = child.tagName();
        if( tag == "OrigDefaultDimensions" )
            myOrigDefaultDimensions.configureFromDomXml(child);
        else if( tag == "CurrentDefaultDimensions" )
            myCurrentDefaultDimensions.configureFromDomXml(child);
        if( tag == "OrigDefaultMinDimensions" )
            myOrigDefaultMinDimensions.configureFromDomXml(child);
        if( tag == "OrigDefaultMaxDimensions" )
            myOrigDefaultMaxDimensions.configureFromDomXml(child);
        else if( tag == "CurrentDefaultMinDimensions" )
            myCurrentDefaultMinDimensions.configureFromDomXml(child);
        else if( tag == "CurrentDefaultMaxDimensions" )
            myCurrentDefaultMaxDimensions.configureFromDomXml(child);
        else if( tag == "SetCells" )
            loadMySetCellsFromDomXml(child);
        else if( tag == "SetRows" )
            loadMySetRowsFromDomXml(child);
        else if( tag == "SetColumns" )
            loadMySetColumnsFromDomXml(child);
        else if( tag == "RowHeights" )
            loadMyRowHeightsFromDomXml(child);
        else if( tag == "ColumnWidths" )
            loadMyColumnWidthsFromDomXml(child);
        else if( tag == "MinRowHeights" )
            loadMyMinRowHeightsFromDomXml(child);
        else if( tag == "MinColumnWidths" )
            loadMyMinColumnWidthsFromDomXml(child);
        else if( tag == "MaxRowHeights" )
            loadMyMaxRowHeightsFromDomXml(child);
        else if (tag == "MaxColumnWidth")
            loadMyMaxColumnWidthsFromDomXml(child);
        else if( tag == "SetVisualCells" )
            loadMySetVisualCellsFromDomXml(child);
        else if( tag == "SetVisualRows" )
            loadMySetVisualRowsFromDomXml(child);
        else if( tag == "SetVisualColumns" )
            loadMySetVisualColumnsFromDomXml(child);
        else if( tag == "SetRepeatingRows" )
            loadMySetRepeatingRowsFromDomXml(child);
        else if( tag == "SetRepeatingColumns" )
            loadMySetRepeatingColumnsFromDomXml(child);
        else if( tag == "VisualRowHeights" )
            loadMyVisualRowHeightsFromDomXml(child);
        else if( tag == "VisualColumnWidths" )
            loadMyVisualColumnWidthsFromDomXml(child);
        else if( tag == "VisualRowMinHeights" )
            loadMyVisualRowMinHeightsFromDomXml(child);
        else if( tag == "VisualColumnMinWidths" )
            loadMyVisualColumnMinWidthsFromDomXml(child);
        else if( tag == "VisualRowMaxHeights" )
            loadMyVisualRowMaxHeightsFromDomXml(child);
        else if( tag == "VisualColumnMaxWidths" )
            loadMyVisualColumnMaxWidthsFromDomXml(child);
        else if( tag == "RepeatingRowHeights" )
            loadMyRepeatingRowHeightsFromDomXml(child);
        else if( tag == "RepeatingColumnWidths" )
            loadMyRepeatingColumnWidthsFromDomXml(child);

        child = child.nextSiblingElement();
    }

    //FORCE RECALC ALL
    emit dimensionChanged();
}

void QicsDimensionManager::loadMySetCellsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    mySetCells.clear();
    while (!child.isNull()) {
        QicsCellSetting s;
        s.configureFromDomXml(child);
        mySetCells << s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMySetRowsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    mySetRows.clear();
    while (!child.isNull()) {
        QicsRowSetting s;
        s.configureFromDomXml(child);
        mySetRows << s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMySetColumnsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    mySetColumns.clear();
    while (!child.isNull()) {
        QicsColumnSetting s;
        s.configureFromDomXml(child);
        mySetColumns << s;

        child = child.nextSiblingElement();
    }
}

void
QicsDimensionManager::loadMyRowHeightsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int row = child.attribute("pos").toInt();

        if (row >= myRowHeights.size())
            myRowHeights.resize(row + 20);

        QicsRowHeight* s = myRowHeights.at(row);
        if(!s)
            s = new QicsRowHeight();

        s->configureFromDomXml(child);
        myRowHeights[row] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyColumnWidthsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int col = child.attribute("pos").toInt();

        if (col >= myColumnWidths.size())
            myColumnWidths.resize(col + 20);

        QicsColumnWidth* s = myColumnWidths.at(col);
        if(!s)
            s = new QicsColumnWidth();

        s->configureFromDomXml(child);
        myColumnWidths[col] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyMinRowHeightsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int row = child.attribute("pos").toInt();

        if (row >= myRowMinHeights.size())
            myRowHeights.resize(row + 20);

        QicsRowHeight* s = myRowMinHeights.at(row);
        if(!s)
            s = new QicsRowHeight();

        s->configureFromDomXml(child);
        myRowMinHeights[row] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyMaxRowHeightsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int row = child.attribute("pos").toInt();

        if (row >= myRowMaxHeights.size())
            myRowHeights.resize(row + 20);

        QicsRowHeight* s = myRowMaxHeights.at(row);
        if(!s)
            s = new QicsRowHeight();

        s->configureFromDomXml(child);
        myRowMaxHeights[row] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyMinColumnWidthsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int col = child.attribute("pos").toInt();

        if (col >= myColumnMinWidths.size())
            myColumnMinWidths.resize(col + 20);

        QicsColumnWidth* s = myColumnMinWidths.at(col);
        if(!s)
            s = new QicsColumnWidth();

        s->configureFromDomXml(child);
        myColumnMinWidths[col] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyMaxColumnWidthsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int col = child.attribute("pos").toInt();

        if (col >= myColumnMaxWidths.size())
            myColumnMaxWidths.resize(col + 20);

        QicsColumnWidth* s = myColumnMaxWidths.at(col);
        if(!s)
            s = new QicsColumnWidth();

        s->configureFromDomXml(child);
        myColumnMaxWidths[col] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyHiddenRowsFromDomXml(const QDomElement& e)
{
    QStringList hrList = e.attribute("hiddenRows").split(",");;
    myHiddenRows.clear();
    QString row;

    if(row.isEmpty())
        return;

    foreach(row, hrList)
        myHiddenRows << row.toInt();
}

void QicsDimensionManager::loadMyHiddenColumnsFromDomXml(const QDomElement& e)
{
    QStringList hcList = e.attribute("hiddenColumns").split(",");;
    myHiddenColumns.clear();
    QString col;

    if(col.isEmpty())
        return;

    foreach(col, hcList)
        myHiddenColumns << col.toInt();
}

void QicsDimensionManager::loadMySetVisualCellsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    mySetVisualCells.clear();

    while (!child.isNull()) {
        QicsCellSetting s;
        s.configureFromDomXml(child);
        mySetVisualCells << s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMySetVisualRowsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    mySetVisualRows.clear();

    while (!child.isNull()) {
        QicsRowSetting s;
        s.configureFromDomXml(child);
        mySetVisualRows << s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMySetVisualColumnsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    mySetVisualColumns.clear();

    while (!child.isNull()) {
        QicsColumnSetting s;
        s.configureFromDomXml(child);
        mySetVisualColumns << s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMySetRepeatingRowsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    mySetRepeatingRows.clear();

    while (!child.isNull()) {
        QicsRepeatingRowSetting s;
        s.configureFromDomXml(child);
        mySetRepeatingRows << s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMySetRepeatingColumnsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    mySetRepeatingColumns.clear();

    while (!child.isNull()) {
        QicsRepeatingColumnSetting s;
        s.configureFromDomXml(child);
        mySetRepeatingColumns << s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyVisualRowHeightsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int row = child.attribute("pos").toInt();

        if (row >= myVisualRowHeights.size())
            myVisualRowHeights.resize(row + 20);

        QicsRowHeight* s = myVisualRowHeights.at(row);
        if(!s)
            s = new QicsRowHeight();

        s->configureFromDomXml(child);
        myVisualRowHeights[row] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyVisualColumnWidthsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int col = child.attribute("pos").toInt();

        if (col >= myVisualColumnWidths.size())
            myVisualColumnWidths.resize(col + 20);

        QicsColumnWidth* s = myVisualColumnWidths.at(col);
        if(!s)
            s = new QicsColumnWidth();

        s->configureFromDomXml(child);
        myVisualColumnWidths[col] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyVisualRowMinHeightsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int row = child.attribute("pos").toInt();

        if (row >= myVisualRowMinHeights.size())
            myVisualRowMinHeights.resize(row + 20);

        QicsRowHeight* s = myVisualRowMinHeights.at(row);
        if(!s)
            s = new QicsRowHeight();

        s->configureFromDomXml(child);
        myVisualRowHeights[row] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyVisualRowMaxHeightsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int row = child.attribute("pos").toInt();

        if (row >= myVisualRowMaxHeights.size())
            myVisualRowMaxHeights.resize(row + 20);

        QicsRowHeight* s = myVisualRowMaxHeights.at(row);
        if(!s)
            s = new QicsRowHeight();

        s->configureFromDomXml(child);
        myVisualRowHeights[row] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyVisualColumnMinWidthsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int col = child.attribute("pos").toInt();

        if (col >= myVisualColumnMinWidths.size())
            myVisualColumnMinWidths.resize(col + 20);

        QicsColumnWidth* s = myVisualColumnWidths.at(col);
        if(!s)
            s = new QicsColumnWidth();

        s->configureFromDomXml(child);
        myVisualColumnWidths[col] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyVisualColumnMaxWidthsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        int col = child.attribute("pos").toInt();

        if (col >= myVisualColumnMaxWidths.size())
            myVisualColumnMaxWidths.resize(col + 20);

        QicsColumnWidth* s = myVisualColumnWidths.at(col);
        if(!s)
            s = new QicsColumnWidth();

        s->configureFromDomXml(child);
        myVisualColumnWidths[col] = s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyRepeatingRowHeightsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    myRepeatingRowHeights.clear();

    while (!child.isNull()) {
        QicsRepeatingRowHeight s;
        s.configureFromDomXml(child);
        myRepeatingRowHeights << s;

        child = child.nextSiblingElement();
    }
}

void QicsDimensionManager::loadMyRepeatingColumnWidthsFromDomXml(const QDomElement& e)
{
    QDomElement child = e.firstChildElement();

    myRepeatingColumnWidths.clear();

    while (!child.isNull()) {
        QicsRepeatingColumnWidth s;
        s.configureFromDomXml(child);
        myRepeatingColumnWidths << s;

        child = child.nextSiblingElement();
    }
}

////////////////////////////////////////////////////////////////////
/////////////////         Stretch methods          /////////////////
////////////////////////////////////////////////////////////////////

bool QicsDimensionManager::isRowStretchable(int row, bool visual_coords) const
{
    if (myRowDM)
        return myRowDM->isRowStretchable(row, visual_coords);
    else {
        const QicsRowSettingV *pSettings;
        pSettings = (visual_coords ? &mySetVisualRows : &mySetRows);
        const QicsRowSettingV &settings = *pSettings;

        QicsRowSettingV::const_iterator iter, iter_end(settings.constEnd());
        for (iter = settings.constBegin(); iter != iter_end; ++iter) {
            const QicsRowSetting &rfs = *iter;

            if ((rfs.row == row) && (rfs.role == QicsTableGridRole))
                return rfs.stretchable;
        }
    }

    return false;
}

void QicsDimensionManager::setRowStretchable(int row, bool visual_coords, bool set)
{
    if (myRowDM)
        myRowDM->setRowStretchable(row, visual_coords, set);
    else {
        QicsRowSettingV *pSettings;
        pSettings = (visual_coords ? &mySetVisualRows : &mySetRows);
        QicsRowSettingV &settings = *pSettings;

        QicsRowSettingV::iterator iter, it_end = settings.end();

        bool found = false;
        for (iter = settings.begin(); iter != it_end; ++iter) {
            QicsRowSetting &rfs = *iter;

            if ((rfs.row == row) && (rfs.role == QicsTableGridRole)) {
                found = true;
                rfs.stretchable = set;
                break;
            }
        }

        if (!found) {
            QicsRowSetting rfs;

            rfs.role = QicsTableGridRole;
            rfs.row = row;
            rfs.stretchable = set;

            settings.push_back(rfs);
        }
    }
}

bool QicsDimensionManager::isColumnStretchable(int col, bool visual_coords) const
{
    if (myColumnDM)
        return myColumnDM->isColumnStretchable(col, visual_coords);
    else {
        const QicsColumnSettingV *pSettings;
        pSettings = (visual_coords ? &mySetVisualColumns : &mySetColumns);
        const QicsColumnSettingV &settings = *pSettings;

        QicsColumnSettingV::const_iterator iter, it_end = settings.end();
        for (iter = settings.begin(); iter != it_end; ++iter) {
            const QicsColumnSetting &cfs = *iter;

            if ((cfs.col == col) && (cfs.role == QicsTableGridRole))
                return cfs.stretchable;
        }
    }

    return false;
}

void QicsDimensionManager::setColumnStretchable(int col, bool visual_coords, bool set)
{
    if (myColumnDM)
        myColumnDM->setColumnStretchable(col, visual_coords, set);
    else {
        QicsColumnSettingV *pSettings;
        pSettings = (visual_coords ? &mySetVisualColumns : &mySetColumns);
        QicsColumnSettingV &settings = *pSettings;

        QicsColumnSettingV::iterator iter, it_end = settings.end();

        bool found = false;
        for (iter = settings.begin(); iter != it_end; ++iter) {
            QicsColumnSetting &cfs = *iter;

            if ((cfs.col == col) && (cfs.role == QicsTableGridRole)) {
                found = true;
                cfs.stretchable = set;
                break;
            }
        }

        if (!found) {
            QicsColumnSetting cfs;

            cfs.role = QicsTableGridRole;
            cfs.col = col;
            cfs.stretchable = set;

            settings.push_back(cfs);
        }
    }
}

unsigned int QicsDimensionManager::stretchRows(int start_row, int end_row, int space, bool visual_coords)
{
    if (myRowDM)
        return myRowDM->stretchColumns( start_row, end_row, space, visual_coords);
    else {
        QList<int> stretchable_rows;

        QicsRowSettingV::iterator iter, it_end = mySetRows.end();
        for (iter = mySetRows.begin(); iter != it_end; ++iter) {
            QicsRowSetting &rfs = *iter;

            int row = (visual_coords ? gridInfo()->visualRowIndex(rfs.row) : rfs.row);

            if ((rfs.role == QicsTableGridRole) && ((row >= start_row) && (row <= end_row)) &&
                rfs.stretchable && !isRowHidden(rfs.row)) {
                if ( (rowHeight(rfs.row) != rowMinHeight(rfs.row)) || space>0 )
                    stretchable_rows.push_back(rfs.row);
            }
        }

        if (visual_coords) {
            it_end = mySetVisualRows.end();
            for (iter = mySetVisualRows.begin(); iter != it_end; ++iter) {
                QicsRowSetting &rfs = *iter;
                int row = gridInfo()->modelRowIndex(rfs.row);

                if ((rfs.role == QicsTableGridRole) &&
                    ((rfs.row >= start_row) && (rfs.row <= end_row)) &&
                    rfs.stretchable && !isRowHidden(row)) {
                    if (stretchable_rows.indexOf(row) == -1 &&
                        ((rowHeight(rfs.row) != rowMinHeight(rfs.row)) || space>0) ) {
                        stretchable_rows.push_back(row);
                    }
                }
            }
        }

        int nrows = stretchable_rows.size();

        if (nrows > 0) {
            int toSize = space;
            space = toSize - regionHeight(QicsRegion(start_row, 0, end_row, 0))-6;

            int each = space / nrows;
            int remainder = space % nrows;
            int remainder_part = (space < 0 ? -1 : 1);

            int next_row = myLastStretchedRow;
            int i, set_to;

            if (remainder < 0)
                remainder = remainder * -1;

            // Find the next row to add partials to
            qSort(stretchable_rows);

            for (i = 0; i < nrows; ++i)
                if (stretchable_rows.at(i) > myLastStretchedRow) {
                    next_row = i;
                    break;
                }

            for (i = next_row; i < nrows; ++i) {
                int row = stretchable_rows.at(i);
                int delta = each;

                if (remainder-- > 0) {
                    delta += remainder_part;

                    if (remainder == 0)
                        myLastStretchedRow = row;
                }

                if (delta != 0) {
                    set_to = rowHeight(row) + delta ;
                    if ( set_to < rowMinHeight(row) ) {
                        remainder_part -= set_to ;
                        set_to = rowMinHeight(row);
                    }
                    setRowHeightInPixels(row, false, set_to);
                }
            }

            if ( next_row > nrows )
                next_row = nrows;
            for (i = 0; i < next_row; ++i) {
                int row = stretchable_rows.at(i);
                int delta = each;

                if (remainder-- > 0) {
                    delta += remainder_part;

                    if (remainder == 0)
                        myLastStretchedRow = row;
                }

                if (delta != 0) {
                    set_to = rowHeight(row) + delta;
                    if ( set_to < rowMinHeight(row) ) {
                        remainder_part -= set_to ;
                        set_to = rowMinHeight(row);
                    }
                    setRowHeightInPixels(row, false, set_to);
                }
            }
        }
        return nrows;
    } // DM
}

unsigned int QicsDimensionManager::stretchColumns(int start_col, int end_col, int space,
                                     bool visual_coords)
{
    if (myColumnDM)
        return myColumnDM->stretchColumns( start_col, end_col, space, visual_coords);
    else
    {
        QList<int> stretchable_cols;

        QicsColumnSettingV::iterator iter, it_end = mySetColumns.end();
        for (iter = mySetColumns.begin(); iter != it_end; ++iter) {
            QicsColumnSetting &rfs = *iter;

            int col = (visual_coords ? gridInfo()->visualColumnIndex(rfs.col) : rfs.col);

            if ((rfs.role == QicsTableGridRole) &&
                ((col >= start_col) && (col <= end_col)) &&
                rfs.stretchable && !isColumnHidden(rfs.col)) {
                if ( (columnWidth(rfs.col) != columnMinWidth(rfs.col)) || space>0)
                    stretchable_cols.push_back(rfs.col);
            }
        }

        if (visual_coords) {
            it_end = mySetVisualColumns.end();
            for (iter = mySetVisualColumns.begin(); iter != it_end; ++iter) {
                QicsColumnSetting &rfs = *iter;
                int col = gridInfo()->modelColumnIndex(rfs.col);

                if ((rfs.role == QicsTableGridRole) &&
                    ((rfs.col >= start_col) && (rfs.col <= end_col)) &&
                    rfs.stretchable && !isColumnHidden(col)) {

                    if ((stretchable_cols.indexOf(col) == -1) &&
                        (columnWidth(rfs.col) != columnMinWidth(rfs.col) || space>0) ) {
                        stretchable_cols.push_back(col);
                    }
                }
            }
        }

        const int ncols = stretchable_cols.size();

        if (ncols > 0) {
            int toSize = space;
            space = toSize - regionWidth(QicsRegion(0, start_col, 0, end_col))-6;

            int each = space / ncols;
            int remainder = space % ncols;
            int remainder_part = (space < 0 ? -1 : 1);

            int next_col = myLastStretchedColumn;
            int i, set_to;

            if (remainder < 0)
                remainder = remainder * -1;

            // Find the next column to add partials to
            qSort(stretchable_cols);

            for (i = 0; i < ncols; ++i)
                if (stretchable_cols.at(i) > myLastStretchedColumn) {
                    next_col = i;
                    break;
                }

            for (i = next_col; i < ncols; ++i) {
                int col = stretchable_cols.at(i);
                int delta = each;

                if (remainder-- > 0) {
                    delta += remainder_part;

                    if (remainder == 0)
                        myLastStretchedColumn = col;
                }

                if (delta != 0) {
                    set_to = columnWidth(col) + delta;
                    if ( set_to < columnMinWidth(col) ) {
                        remainder_part -= set_to ;
                        set_to = columnMinWidth(col);
                    }
                    setColumnWidthInPixels(col, false, set_to);
                }
            }

            if ( next_col > ncols )
                next_col = ncols;

            for (i = 0; i < next_col ; ++i) {
                int col = stretchable_cols.at(i);
                int delta = each;

                if (remainder-- > 0) {
                    delta += remainder_part;

                    if (remainder == 0)
                        myLastStretchedColumn = col;
                }

                if (delta != 0) {
                    set_to = columnWidth(col) + delta;
                    if ( set_to < columnMinWidth(col) ) {
                        remainder_part -= set_to ;
                        set_to = columnMinWidth(col);
                    }
                    setColumnWidthInPixels(col, false, set_to);
                }
            }
        }
        return ncols;
    } // DM
}


