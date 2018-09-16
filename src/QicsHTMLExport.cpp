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

#include "QicsHTMLExport.h"

#include <QDir>
#include "QicsTable.h"
#include "QicsSpanManager.h"
#include "QicsStyleManager.h"
#include "QicsWidgetCellDisplay.h"
#include "QicsRowHeader.h"
#include "QicsColumnHeader.h"
#include "QicsColumn.h"
#include "QicsRow.h"
#include "QicsCell.h"
#include "QicsNamespace.h"



QicsHTMLExportOptions::QicsHTMLExportOptions()
{
    pixmaps = true;
    widgets = true;
    coloring = true;
    selection = false;
    dimensions = true;
    headers = true;
    align = true;
    flags = true;
    font = true;
    fillEmpty = true;
    borders = true;
    tableBorders = true;

    pixFormat = "PNG";
    pixQuality = -1;

    pixNamingTemplate = "pix-%1-%2";
    widgetNamingTemplate = "wid-%1-%2";
}

////////////////////////////////////////////////////////////////////

QicsHTMLExport::QicsHTMLExport(QicsTable *table, const QicsHTMLExportOptions &options)
    : m_table(table), m_opts(options)
{
}

QicsHTMLExport::~QicsHTMLExport()
{
}

bool QicsHTMLExport::exportToFile(const QString& fileName)
{
    if (!m_table) return false;

    // define dimensions
    int startRow = 0;
    int startColumn = 0;
    int lastRow = m_table->currentViewport().numRows()-1;
    int lastColumn = m_table->currentViewport().numColumns()-1;
    if (m_opts.region.isValid()) {
        m_opts.region.normalize();
        startRow = qMax(0, m_opts.region.startRow());
        startColumn = qMax(0, m_opts.region.startColumn());
        lastRow = qMin(lastRow, m_opts.region.endRow());
        lastColumn = qMin(lastColumn, m_opts.region.endColumn());
    }

    // prepare file
    fi = QFileInfo(fileName);
    if (!fi.absoluteDir().exists()) return false;

    QString name = fi.fileName();
    if (name.isEmpty())
        name = fi.absolutePath() + "/table.html";
    else
        name = fi.absoluteFilePath();

    QFile f(name);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    fs.setDevice(&f);

    temp_html.clear();
    ts.setString(&temp_html);

    styleMap.clear();
    temp_style.clear();
    ss.setString(&temp_style);

    // begin export
    ts << "<table cellpadding=0 cellspacing=0" << endl;
    if (m_opts.tableBorders)
        ts << " border=1";
    ts << ">";

    QicsRowHeader *rh = m_table->rowHeader();
    int rs = 0;
    for (int i = 0; i < rh->numColumns(); ++i)
        if (!rh->columnRef(i).isHidden()) ++rs;

    QicsColumnHeader *ch = m_table->columnHeader();
    int cs = 0;
    for (int i = 0; i < ch->numRows(); ++i)
        if (!ch->rowRef(i).isHidden()) ++cs;

    // top header
    if (m_opts.headers && m_table->topHeaderVisible()) {
        ts << "<tr>" << endl;

        // dummy cell for left header if present
        if (m_table->leftHeaderVisible()) {
            if (cs && rs)
                ts << "<td rowspan=" << cs << " colspan=" << rs << ">" << "</td>" << endl;
        }

        // content
        int rnum = 0;
        for (int i = 0; i < ch->numRows(); ++i) {
            if (ch->rowRef(i).isHidden()) continue;
            if (rnum++) ts << "<tr>" << endl;

            for (int j = startColumn; j <= lastColumn; ++j) {
                if (m_table->columnRef(j).isHidden()) continue;

                doExportCell( ch->cellRef(i, j) );
            }
        }

        // dummy cell for right header if present
        if (m_table->rightHeaderVisible()) {
            if (cs && rs)
                ts << "<td rowspan=" << cs << " colspan=" << rs << ">" << "</td>" << endl;
        }
    }

    // content
    for (int i = startRow; i <= lastRow; ++i) {
        ts << "<tr>" << endl;

        // left header content
        if (m_opts.headers && m_table->leftHeaderVisible()) {
            for (int k = 0; k < rh->numColumns(); ++k) {
                if (rh->columnRef(k).isHidden()) continue;

                doExportCell( rh->cellRef(i, k) );
            }
        }

        // table content
        for (int j = startColumn; j <= lastColumn; ++j) {
            if (m_table->columnRef(j).isHidden()) continue;

            doExportCell( m_table->cellRef(i, j) );
        }

        // right header content
        if (m_opts.headers && m_table->rightHeaderVisible()) {
            for (int k = 0; k < rh->numColumns(); ++k) {
                if (rh->columnRef(k).isHidden()) continue;

                doExportCell( rh->cellRef(i, k) );
            }
        }
    }

    // bottom header
    if (m_opts.headers && m_table->bottomHeaderVisible()) {
        ts << "<tr>" << endl;

        // dummy cell for left header if present
        if (m_table->leftHeaderVisible()) {
            if (cs && rs)
                ts << "<td rowspan=" << cs << " colspan=" << rs << ">" << "</td>" << endl;
        }

        // content
        int rnum = 0;
        for (int i = 0; i < ch->numRows(); ++i) {
            if (ch->rowRef(i).isHidden()) continue;
            if (rnum++) ts << "<tr>" << endl;

            for (int j = startColumn; j <= lastColumn; ++j) {
                if (m_table->columnRef(j).isHidden()) continue;

                doExportCell( ch->cellRef(i, j) );
            }
        }

        // dummy cell for right header if present
        if (m_table->rightHeaderVisible()) {
            if (cs && rs)
                ts << "<td rowspan=" << cs << " colspan=" << rs << ">" << "</td>" << endl;
        }
    }

    ts << "</table>" << endl;

    // actual export
    fs << "<html><head>" << endl;
    // style table
    fs << "<style type=\"text/css\">" << endl;
    QMap<QString,QString>::const_iterator it, it_end(styleMap.constEnd());
    for (it = styleMap.constBegin(); it != it_end; ++it) {
        fs << "." << it.value() << endl << "{" << endl;
        fs << it.key();
        fs << endl << "}" << endl;
    }
    fs << "</style>" << endl;
    // html
    fs << "</head><body>" << endl;
    fs << temp_html;
    fs << "</body></html>" << endl;

    f.close();
    return true;
}

void QicsHTMLExport::doExportCell(const QicsCell &cell)
{
    int row = cell.rowIndex(), col = cell.columnIndex();
    QicsGridInfo &gi = cell.gridInfo();
    QicsSpanManager *spm = cell.styleManager().spanManager();
    QicsRegion r;

    if (spm->insideSpan(gi, row, col, r))
        return;

    bool spanner = spm->isSpanner(gi, row, col, r);
    if (spanner)
        ts << "<td rowspan=" << r.numRows() << " colspan=" << r.numColumns();
    else
        ts << "<td";

    // dimensions
    if (m_opts.dimensions) {
        if (spanner) {
            QicsMappedDimensionManager *mdm = gi.mappedDM();
            ts << " width=" << mdm->regionWidth(r) << " height=" << mdm->regionHeight(r);
        } else {
            if (cell.widthInPixels() > 0)
                ts << " width=" << cell.widthInPixels();
            if (cell.heightInPixels() > 0)
                ts << " height=" << cell.heightInPixels();
        }
    }

    // text flags
    if (m_opts.flags) {
        int tf = cell.textFlags();
        if (!(tf & Qt::TextWordWrap || tf & Qt::TextWrapAnywhere)) ts << " nowrap";
        // nowrap does not work for long strings - consider making an option to force replacing spaces with &nbsp there
    }

    ts << " class=\"";
    temp_style.clear();

    // alignment
    if (m_opts.align) {
        int al = cell.alignment();
        if (al & Qt::AlignLeft) ss << " text-align: left;";
        if (al & Qt::AlignRight) ss << " text-align: right;";
        if (al & Qt::AlignHCenter) ss << " text-align: center;";
        if (al & Qt::AlignJustify) ss << " text-align: justify;";
        if (al & Qt::AlignTop) ss << " vertical-align: top;";
        if (al & Qt::AlignBottom) ss << " vertical-align: bottom;";
        if (al & Qt::AlignVCenter) ss << " vertical-align: center;";
    }

    // bg color
    if (m_opts.coloring) {
        QColor bgc = cell.backgroundColor();
        if (cell.selected() && m_opts.selection)
            bgc = cell.selectedBackgroundColor();
        if (bgc.isValid() && (gi.gridType() != Qics::TableGrid || (gi.gridType() == Qics::TableGrid && bgc != m_table->backgroundColor())))
            ss << " background-color: " << bgc.name() << ";";
    }

    // pen borders
    if (m_opts.borders) {
        doExportPen(cell.topBorderPen(), Qics::TopBorder);
        doExportPen(cell.bottomBorderPen(), Qics::BottomBorder);
        doExportPen(cell.leftBorderPen(), Qics::LeftBorder);
        doExportPen(cell.rightBorderPen(), Qics::RightBorder);
    }


    QString img;

    // picture content
    const QPixmap &pm = cell.pixmap();
    if (!pm.isNull() && m_opts.pixmaps) {
        QString name = m_opts.pixNamingTemplate.arg(row).arg(col) + "." + m_opts.pixFormat.toLower();
        pm.save(fi.absolutePath() + "/" + name, m_opts.pixFormat.toLocal8Bit().data(), m_opts.pixQuality);
        img = "<img src='" + name + "'>";
    }

    // widget displayer
    QicsWidgetCellDisplay *cw = dynamic_cast<QicsWidgetCellDisplay*>(cell.displayer());
    if (cw && cw->widget() && m_opts.widgets) {
#if QT_VERSION < 0x050000
        const QPixmap &pm = QPixmap::grabWidget(cw->widget());
#else
        const QPixmap &pm = cw->widget()->grab();
#endif
        if (!pm.isNull()) {
            QString name = m_opts.widgetNamingTemplate.arg(row).arg(col) + "." + m_opts.pixFormat.toLower();;
            pm.save(fi.absolutePath() + "/" + name, m_opts.pixFormat.toLocal8Bit().data(), m_opts.pixQuality);
            img += "<img src='" + name + "'>";
        }
    }

    // text content
    QString text;
    if (cell.dataValue())
        text = cell.dataValue()->string();
    else
        text = cell.label();

    if (text.isEmpty()) {

        switch (gi.gridType()) {
            case Qics::ColumnHeaderGrid:
                text = QString::number(gi.modelColumnIndex(col));
                break;
            case Qics::RowHeaderGrid:
                text = QString::number(gi.modelRowIndex(row));
                break;
            default:
                if (m_opts.fillEmpty)
                    text = "&nbsp;";
                break;
        }

    }

    if (!text.isEmpty()) {
        // fg color
        if (m_opts.coloring) {
            QColor fgc = cell.foregroundColor();
            if (cell.selected() && m_opts.selection)
                fgc = cell.selectedForegroundColor();
            if (fgc.isValid() && (gi.gridType() != Qics::TableGrid || (gi.gridType() == Qics::TableGrid && fgc != m_table->foregroundColor())))
                ss << " color: " << fgc.name() << ";";
        }

        // font
        const QFont &fnt = cell.font();

        if (m_opts.font) {
            // size
            ss << " font-size: ";
            if (fnt.pixelSize() > 0)
                ss << fnt.pixelSize() << "px;";
            else
                ss << fnt.pointSize() << "pt;";

            // family
            ss << " font-family: " << fnt.family() << ";";

            // attrs
            if (fnt.bold()) ss << " font-weight: bold;";
            if (fnt.italic()) ss << " font-style: italic;";

            QString decor;
            if (fnt.underline()) decor += " underline";
            if (fnt.strikeOut()) decor += " line-through";
            if (!decor.isEmpty())
                ss << " text-decoration: " << decor << ";";
        }

        // map ss to style map
        if (styleMap.contains(temp_style)) {
            ts << styleMap[temp_style];
        }
        else {
            QString style = QString("style%1").arg(styleMap.size());
            styleMap[temp_style] = style;
            ts << style;
        }

        ts << "\"";
        ts << ">" << endl;

        if (!img.isEmpty()) ts << img << endl;

        // export text
        text = text.replace(' ', "&nbsp;");
        ts << text << endl;
    }
    else {
        ts << "\">";
        if (!img.isEmpty()) ts << img << endl;
    }

    ts << "</td>" << endl;
}

void QicsHTMLExport::doExportPen(const QPen &pen, Qics::QicsBoxBorders border)
{
    if (pen.style() == Qt::NoPen)
        return;

    QString prefix;
    switch (border)
    {
    case Qics::TopBorder:
        prefix = "border-top-";
        break;
    case Qics::BottomBorder:
        prefix = "border-bottom-";
        break;
    case Qics::LeftBorder:
        prefix = "border-left-";
        break;
    case Qics::RightBorder:
        prefix = "border-right-";
        break;
    default:
        return;
    }

    // width
    if (pen.width())
        ss << prefix << "width: " << pen.width() << "px; ";
    else
        ss << prefix << "width: 1px; ";
    // color
    ss << prefix << "color: " << pen.color().name() << "; ";
    // style
    ss << prefix << "style: ";

    switch (pen.style())
    {
    case Qt::SolidLine:
        ss << "solid; "; break;
    case Qt::DashDotLine:
    case Qt::CustomDashLine:
    case Qt::DashLine:
        ss << "dashed; "; break;
    case Qt::DashDotDotLine:
    case Qt::DotLine:
        ss << "dotted; "; break;
    default:
        break;
    }
}


