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

#include "QicsCellRegion.h"

#include "QicsHeader.h"
#include "QicsTable.h"
#include "QicsStyleManager.h"
#include "QicsAbstractAttributeController.h"
#include "QicsCell.h"


QicsCellRegion::QicsCellRegion(int begin_row, int begin_col, int end_row, int end_col)
    : myInitCoords(true)
{
    myRegion = QicsRegion(begin_row, begin_col, end_row, end_col);
}

QicsCellRegion::QicsCellRegion(const QicsRegion &region)
    : myRegion(region), myInitCoords(true)
{
}

void QicsCellRegion::setColors(const QColor &fc, const QColor &bc, const QColor &sfc, const QColor &sbc)
{
    if (!myRegion.isValid()) return;

    myInitCoords = true;

    if (fc.isValid()) {
        setRegionAttr(myRegion, QicsCellStyle::ForeColor, static_cast<const void *> (&fc));
        myInitCoords = false;
    }

    if (bc.isValid()) {
        setRegionAttr(myRegion, QicsCellStyle::BackColor, static_cast<const void *> (&bc));
        myInitCoords = false;
    }

    if (sfc.isValid()) {
        setRegionAttr(myRegion, QicsCellStyle::SelForeColor, static_cast<const void *> (&sfc));
        myInitCoords = false;
    }

    if (sbc.isValid()) {
        setRegionAttr(myRegion, QicsCellStyle::SelBackColor, static_cast<const void *> (&sbc));
    }
}

void QicsCellRegion::setForegroundColor(const QColor &fc)
{
    if (!(fc.isValid())) return;

    setRegionAttr(myRegion, QicsCellStyle::ForeColor, static_cast<const void *> (&fc));
}

void QicsCellRegion::setBackgroundColor(const QColor &bc)
{
    if (!(bc.isValid())) return;

    setRegionAttr(myRegion, QicsCellStyle::BackColor, static_cast<const void *> (&bc));
}

void QicsCellRegion::setSelectedForegroundColor(const QColor &fc)
{
    if (!(fc.isValid())) return;

    setRegionAttr(myRegion, QicsCellStyle::SelForeColor, static_cast<const void *> (&fc));
}

void QicsCellRegion::setSelectedBackgroundColor(const QColor &bc)
{
    if (!(bc.isValid())) return;

    setRegionAttr(myRegion, QicsCellStyle::SelBackColor, static_cast<const void *> (&bc));
}

void QicsCellRegion::setEditForegroundColor(const QColor &fc)
{
    if (!(fc.isValid())) return;

    setRegionAttr(myRegion, QicsCellStyle::EditForegroundColor, static_cast<const void *> (&fc));
}

void QicsCellRegion::setEditBackgroundColor(const QColor &bc)
{
    if (!(bc.isValid())) return;

    setRegionAttr(myRegion, QicsCellStyle::EditBackgroundColor, static_cast<const void *> (&bc));
}

void QicsCellRegion::setFontAttribute(QicsCellStyle::QicsCellStyleProperty prop,
                                      bool value, bool updateCellSizes)
{
    if (!myRegion.isValid()) return;

    prepareFontInRegion(myRegion);

    setRegionAttr(myRegion, prop, static_cast<const void *> (&value));

    if (updateCellSizes)
        updateFontInRegion(myRegion);
}

void QicsCellRegion::setFontSize(int size, bool updateCellSizes)
{
    if (!myRegion.isValid()) return;

    prepareFontInRegion(myRegion);

    setRegionAttr(myRegion, QicsCellStyle::FontSize, static_cast<const void *> (&size));

    if (updateCellSizes)
        updateFontInRegion(myRegion);
}

void QicsCellRegion::setFontWeight(int weight, bool updateCellSizes)
{
    if (!myRegion.isValid()) return;

    prepareFontInRegion(myRegion);

    setRegionAttr(myRegion, QicsCellStyle::FontWeight, static_cast<const void *> (&weight));

    if (updateCellSizes)
        updateFontInRegion(myRegion);
}

void QicsCellRegion::setFontStretch(int factor, bool updateCellSizes)
{
    if (!myRegion.isValid()) return;

    prepareFontInRegion(myRegion);

    setRegionAttr(myRegion, QicsCellStyle::FontStretch, static_cast<const void *> (&factor));

    if (updateCellSizes)
        updateFontInRegion(myRegion);
}

void QicsCellRegion::setFontFamily(const QString &family, bool updateCellSizes)
{
    if (!myRegion.isValid()) return;

    prepareFontInRegion(myRegion);

    setRegionAttr(myRegion, QicsCellStyle::FontFamily, static_cast<const void *> (&family));

    if (updateCellSizes)
        updateFontInRegion(myRegion);
}

void QicsCellRegion::setBoxPen(const QPen &pen, QicsBoxBorders borders)
{
    if (borders & Qics::TopBorder)
        setRegionAttr(
        QicsRegion(myRegion.top(), myRegion.left(), myRegion.top(), myRegion.right()),
        QicsCellStyle::TopBorderPen, static_cast<const void *> (&pen)	);

    if (borders & Qics::BottomBorder)
        setRegionAttr(
        QicsRegion(myRegion.bottom(), myRegion.left(), myRegion.bottom(), myRegion.right()),
        QicsCellStyle::BottomBorderPen, static_cast<const void *> (&pen)	);

    if (borders & Qics::LeftBorder)
        setRegionAttr(
        QicsRegion(myRegion.top(), myRegion.left(), myRegion.bottom(), myRegion.left()),
        QicsCellStyle::LeftBorderPen, static_cast<const void *> (&pen)	);

    if (borders & Qics::RightBorder)
        setRegionAttr(
        QicsRegion(myRegion.top(), myRegion.right(), myRegion.bottom(), myRegion.right()),
        QicsCellStyle::RightBorderPen, static_cast<const void *> (&pen)	);
}


void QicsCellRegion::setRegionAttr(const QicsRegion &region, QicsCellStyle::QicsCellStyleProperty prop, const void *val)
{
    bool rc = mySM->isReportingChanges();
    mySM->setReportChanges(false);

    // check if the property is virtual
    bool isVirtual = false;
    if (prop > QicsCellStyle::LastProperty) isVirtual = true;

    // check if property supported
    QicsAbstractAttributeController *mac = mySM->modelAttributeController();
    bool use_mac = (mac && mac->isPropertySupported(prop));

    if (use_mac || !isVirtual) {
        // check if we have to set table attributes
        if (region.isEntireRow() && region.isEntireColumn()) {
            mySM->setDefaultProperty(prop, val);
        }
        else
            // check if we have to set row attributes
            if (region.isEntireRow()) {
                for (int j = region.top(); j <= region.bottom(); ++j) {
                    int mr = cellRef(j,0).modelRowIndex();
                    if (use_mac)
                        if (mac->setRowProperty(mr, prop, val))
                            continue;

                    mySM->setRowProperty(mr, false, prop, val);
                }
            }
            else
                // check if we have to set column attributes
                if (region.isEntireColumn()) {
                    for (int i = region.left(); i <= region.right(); ++i) {
                        int mc = cellRef(0,i).modelColumnIndex();
                        if (use_mac)
                            if (mac->setColumnProperty(mc, prop, val))
                                continue;

                        mySM->setColumnProperty(mc, false, prop, val);
                    }
                }
                else {
                    // set cell attributes
                    int r = 0, c = 0;

                    if (myInitCoords) {
                        modelRows.resize(region.height());
                        modelCols.resize(region.width());

                        for (int i = region.left(); i <= region.right(); ++i)
                            modelCols[c++] = cellRef(0,i).modelColumnIndex();

                        for (int j = region.top(); j <= region.bottom(); ++j)
                            modelRows[r++] = cellRef(j,0).modelRowIndex();
                    }
                    else {
                        r = modelRows.size();
                        c = modelCols.size();
                    }

                    myInitCoords = true;

                    for (int i = 0; i < r; ++i) {
                        int mr = modelRows.at(i);

                        for (int j = 0; j < c; ++j) {
                            int mc = modelCols.at(j);

                            if (use_mac)
                                if (mac->setCellProperty(mr, mc, prop, val))
                                    continue;

                            mySM->setCellProperty(mr, mc, false, prop, val);
                        }
                    }
                }
    }
    else {     // if no contoller is set and prop is virtual, then devirtualize it

        // check if we have to set table attributes
        if (region.isEntireRow() && region.isEntireColumn()) {
            mySM->setDefaultProperty(prop, val);
        }
        else
            // check if we have to set row attributes
            if (region.isEntireRow()) {
                for (int j = region.top(); j <= region.bottom(); ++j)
                    setVirtualCellAttr(j, 0, prop, val);
            }
            else
                // check if we have to set column attributes
                if (region.isEntireColumn()) {
                    for (int i = region.left(); i <= region.right(); ++i)
                        setVirtualCellAttr(0, i, prop, val);
                }
                else
                    // set cell attributes
                    for (int i = region.left(); i <= region.right(); ++i)
                        for (int j = region.top(); j <= region.bottom(); ++j)
                            setVirtualCellAttr(j, i, prop, val);
    }

    mySM->setReportChanges(rc);
}

void QicsCellRegion::setVirtualCellAttr(int row, int col, QicsCellStyle::QicsCellStyleProperty prop, const void *val)
{
    if (prop <= QicsCellStyle::FontStretch) {
        QicsCell &cl = cellRef(row, col);
        QFont f = cl.font();

        switch (prop) {
            case QicsCellStyle::FontFamily:
                f.setFamily(*(QString*)val);
                break;
            case QicsCellStyle::FontSize:
                f.setPointSize(*(int*)val);
                break;
            case QicsCellStyle::FontWeight:
                f.setWeight(*(int*)val);
                break;
            case QicsCellStyle::FontBold:
                f.setBold(*(bool*)val);
                break;
            case QicsCellStyle::FontItalic:
                f.setItalic(*(bool*)val);
                break;
            case QicsCellStyle::FontUnderline:
                f.setUnderline(*(bool*)val);
                break;
            case QicsCellStyle::FontOverline:
                f.setOverline(*(bool*)val);
                break;
            case QicsCellStyle::FontStrikeOut:
                f.setStrikeOut(*(bool*)val);
                break;
            case QicsCellStyle::FontFixed:
                f.setFixedPitch(*(bool*)val);
                break;
            case QicsCellStyle::FontKerning:
                f.setKerning(*(bool*)val);
                break;
            case QicsCellStyle::FontStretch:
                f.setStretch(*(int*)val);
                break;
            default: break;
        }
        cl.setFont(f);
    }
}

////////////////////////////////////////////////////////////////////////////
// QicsCellRegionHeader
////////////////////////////////////////////////////////////////////////////

QicsCellRegionHeader::QicsCellRegionHeader(const QicsRegion &region, QicsHeader *header, QicsGridType gt,
                                           QicsStyleManager *sm, QicsDimensionManager *dm)
    : QicsCellRegion(region), myHeader(header)
{
    myGridType = gt;
    mySM = sm;
    myDM = dm;
}

void QicsCellRegionHeader::updateFontInRegion(const QicsRegion &region)
{
    if (!myDM) return;

    // check if we have to set row attributes
    if (region.isEntireRow()) {
        for (int j = region.top(); j <= region.bottom(); ++j) {
            const QFont &f = myHeader->rowRef(j).font();
            myDM->setRowFont(myGridType, myHeader->gridInfo().modelRowIndex(j), false, f);
        }

        return;
    }

    // check if we have to set column attributes
    if (region.isEntireColumn()) {
        for (int i = region.left(); i <= region.right(); ++i) {
            const QFont &f = myHeader->columnRef(i).font();
            myDM->setColumnFont(myGridType, myHeader->gridInfo().modelColumnIndex(i), false, f);
        }

        return;
    }

    for (int i = region.left(); i <= region.right(); ++i)
        for (int j = region.top(); j <= region.bottom(); ++j) {
            const QFont &f = myHeader->cellRef(j, i).font();
            myDM->setCellFont(myGridType, myHeader->gridInfo().modelRowIndex(j), myHeader->gridInfo().modelColumnIndex(i), false, f);
        }
}

void QicsCellRegionHeader::prepareFontInRegion(const QicsRegion &region)
{
    if (!myDM) return;
    if (region.isEntireRow() || region.isEntireColumn()) return;

    const QFont &f = myHeader->cellRef(region.top(), region.left()).font();
    myDM->prepareRegionFont(myGridType, region, true, f);
}

void QicsCellRegionHeader::setFont(const QFont &font, bool updateCellSizes)
{
    if (!myRegion.isValid()) return;

    setRegionAttr(myRegion, QicsCellStyle::Font, static_cast<const void *> (&font));

    if (myRegion.isEntireRow() || myRegion.isEntireColumn()) return;

    if (updateCellSizes)
        myDM->setRegionFont(myGridType, myRegion, true, font);
}

void QicsCellRegionHeader::setSelectedFont(const QFont &font)
{
    if (!myRegion.isValid()) return;

    setRegionAttr(myRegion, QicsCellStyle::SelectedFont, static_cast<const void *> (&font));
}

QPalette QicsCellRegionHeader::palette()
{
    return myHeader->palette();
}

QicsCell& QicsCellRegionHeader::cellRef(int r, int c)
{
    return myHeader->cellRef(r, c);
}


////////////////////////////////////////////////////////////////////////////
// QicsCellRegionTable
////////////////////////////////////////////////////////////////////////////

QicsCellRegionTable::QicsCellRegionTable(const QicsRegion &region, QicsTable *table, QicsGridType gt,
                                         QicsStyleManager *sm, QicsDimensionManager *dm)
    : QicsCellRegion(region), myTable(table)
{
    myGridType = gt;
    mySM = sm;
    myDM = dm;
}

void QicsCellRegionTable::setRegionAttr(const QicsRegion &region, QicsCellStyle::QicsCellStyleProperty prop, const void *val)
{
    if (!region.isValid() || !mySM) return;

    QicsCellRegion::setRegionAttr(region, prop, val);

    if (myTable->repaintBehavior() == Qics::RepaintOn)
        myTable->repaint();
}

void QicsCellRegionTable::updateFontInRegion(const QicsRegion &region)
{
    if (!myDM) return;

    QicsRepaintBehavior oldRB = myTable->repaintBehavior();
    myTable->setRepaintBehavior(Qics::RepaintOff);

    if (region.isEntireRow() && region.isEntireColumn()) {
        const QFont &f = myTable->rowRef(0).font();
        myDM->setDefaultFont(f);
        myTable->setRepaintBehavior(oldRB);
        myTable->repaint();
        return;
    }

    // check if we have to set row attributes
    if (region.isEntireRow()) {
        for (int j = region.top(); j <= region.bottom(); ++j) {
            const QFont &f = myTable->rowRef(j).font();
            QicsCell cell(j,0,myTable);
            myDM->setRowFont(myGridType, cell.modelRowIndex(), false, f);
        }

        myTable->setRepaintBehavior(oldRB);
        myTable->repaint();
        return;
    }

    // check if we have to set column attributes
    if (region.isEntireColumn()) {
        for (int i = region.left(); i <= region.right(); ++i) {
            const QFont &f = myTable->columnRef(i).font();
            QicsCell cell(0,i,myTable);
            myDM->setColumnFont(myGridType, cell.modelColumnIndex(), false, f);
        }

        myTable->setRepaintBehavior(oldRB);
        myTable->repaint();
        return;
    }

    const QFont &f = myTable->cellRef(region.top(), region.left()).font();
    myDM->setRegionFont(myGridType, region, true, f);

    myTable->setRepaintBehavior(oldRB);
    myTable->repaint();
}

void QicsCellRegionTable::prepareFontInRegion(const QicsRegion &region)
{
    if (!myDM) return;
    if (region.isEntireRow() || region.isEntireColumn()) return;

    const QFont &f = myTable->cellRef(region.top(), region.left()).font();
    myDM->prepareRegionFont(myGridType, region, true, f);
}

void QicsCellRegionTable::setFont(const QFont &font, bool updateCellSizes)
{
    if (!myDM) return;
    if (!myRegion.isValid()) return;

    setRegionAttr(myRegion, QicsCellStyle::Font, static_cast<const void *> (&font));

    if (updateCellSizes) {
        QicsRepaintBehavior oldRB = myTable->repaintBehavior();
        myTable->setRepaintBehavior(Qics::RepaintOff);

        // check if we have to set row attributes
        if (myRegion.isEntireRow()) {

            for (int j = myRegion.top(); j <= myRegion.bottom(); ++j)
                myDM->setRowFont(myGridType, j, true, font);

            myTable->setRepaintBehavior(oldRB);
            return;
        }

        // check if we have to set column attributes
        if (myRegion.isEntireColumn()) {
            for (int i = myRegion.left(); i <= myRegion.right(); ++i)
                myDM->setColumnFont(myGridType, i, true, font);

            myTable->setRepaintBehavior(oldRB);
            return;
        }

        myDM->setRegionFont(myGridType, myRegion, true, font);
        myTable->setRepaintBehavior(oldRB);
    }
}

void QicsCellRegionTable::setSelectedFont(const QFont &font)
{
    if (!myRegion.isValid()) return;

    setRegionAttr(myRegion, QicsCellStyle::SelectedFont, static_cast<const void *> (&font));
}

QPalette QicsCellRegionTable::palette()
{
    return myTable->palette();
}

QicsCell& QicsCellRegionTable::cellRef(int r, int c)
{
    return myTable->cellRef(r, c);
}


