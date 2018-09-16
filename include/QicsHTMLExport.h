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

#ifndef QICSHTMLEXPORT_H
#define QICSHTMLEXPORT_H

#include <QString>
#include <QMap>
#include <QPen>
#include <QFileInfo>
#include <QTextStream>
#include "QicsNamespace.h"
#include "QicsRegion.h"

class QicsTable;
class QicsCell;

/*! \struct QicsHTMLExportOptions QicsHTMLExport.h
 * \nosubgrouping
 * \brief Struct used to specify HTML export options for QicsHTMLExport class.
 * QicsHTMLExportOptions struct is used to specify HTML export options for QicsHTMLExport class.

 All the options are set to \b true by default.
*/

class QICS_EXPORT QicsHTMLExportOptions
{
public:
    QicsHTMLExportOptions();

    bool pixmaps;                   //!< Allows export pixmaps as external files (into the same folder as html file).
    bool widgets;                   //!< Allows export widgets (as images).
    bool coloring;                  //!< Allows export background/text color of a cell.
    bool selection;                 //!< If coloring is on, additionally highlights current selection .
    bool dimensions;                //!< Allows export cell dimensions (width and height).
    bool headers;                   //!< Allows export table headers.
    bool align;                     //!< Allows export cell alignment.
    bool flags;                     //!< Allows export cell flags.
    bool font;                      //!< Allows export font data (size, family, weight etc.).
    bool borders;                   //!< Allows export cell borders.
    bool tableBorders;              //!< Allows export table (global) borders.
    bool fillEmpty;                 //!< Fills empty cells with &nbsp tag.

    QicsRegion  region;             //!< Defines region of cells to export. Invalid (default) means whole table.

    QString	pixFormat;              //!< Images format (i.e. "PNG", "JPG", etc, see QPixmap::save() documentation). \n \b PNG by default.
    QString	pixNamingTemplate;      //!< Template for naming exported images. \n \b "pix-%1-%2" by default, where %%1 - cell index, %%2 - row index.
    QString widgetNamingTemplate;   //!< Template for naming exported widgets. \n \b "wid-%1-%2" by default, where %%1 - cell index, %%2 - row index.

    int pixQuality;                 //!< Image quality: 0 - best compression, 100 - best quality, -1 - default value. \n \b -1 by default.
};

////////////////////////////////////////////////////////////////////

/*! \class QicsHTMLExport QicsHTMLExport.h
 * \nosubgrouping
 * \brief QicsHTMLExport is a helper class that allows saving table data to HTML.

    To specify which attributes of the table to export, use QicsHTMLExportOptions
    structure passing it to QicsHTMLExport constructor.

    Example of usage:

    \code
    void exportTable(QicsTable *table)
    {
        QicsHTMLExportOptions opts;				// create export options structure
        QString fileName("table.html");			// name of the HTML file

        opts.pixFormat = "JPG";					// export format is JPEG
        opts.pixQuality = 80;					// quality of the images is 80%
        opts.pixNamingTemplate = "image%1-%2";	// all the exported images will get names like "image5-3.jpg"
                                                // (where %1 is cell index, %2 is row index)
        opts.selection = false;					// do not mark currently selected region in HTML

        QicsHTMLExport exporter(table, opts);	// create exporter object
        exporter.exportToFile(fileName);		// export the table
    }
    \endcode

 */

////////////////////////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////////////////////////

class QICS_EXPORT QicsHTMLExport
{
public:
    /*! Constructor.
        \sa QicsHTMLExportOptions
    */
    QicsHTMLExport(QicsTable *table, const QicsHTMLExportOptions &options = QicsHTMLExportOptions());

    ~QicsHTMLExport();

    /*! Exports table's contents to HTML file.
    */
    bool exportToFile(const QString& fileName);

private:
    /*! \internal Performs export of \a cell cell in HTML.
    */
    void doExportCell(const QicsCell &cell);

    /*! \internal Performs export of \a pen attributes at \a border to CSS style.
    */
    void doExportPen(const QPen &pen, Qics::QicsBoxBorders border);

    QicsTable *m_table;
    QFileInfo fi;
    QTextStream	ts;
    QTextStream ss;
    QTextStream fs;
    QString temp_style;
    QString temp_html;
    QMap<QString,QString> styleMap;
    QicsHTMLExportOptions m_opts;
};

#endif //QICSHTMLEXPORT_H


