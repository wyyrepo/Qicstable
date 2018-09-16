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

#ifndef QICSTABLEPRINT_H
#define QICSTABLEPRINT_H

#include <QImage>
#include "QicsTableCommon.h"

class QPrinter;
class QicsPrintPreviewPage;
class QicsPrintPreviewWidget;
class QicsTable;

/////////////////////////////////////////////////////////////////////////////////

/*!
* \class QicsTablePrint QicsTablePrint.h
* \nosubgrouping
*
* QicsTablePrint is used for printing a table.  An instance of the
* QicsTablePrint class is created from an existing QicsTable object.
* The table print object contains all of the same settings as the original QicsTable. After the
* table print object is created, any changes changes to cell, grid, or table
* attributes only modify the print object, not the orignal table object.
* This allows the programmer to set different attribute values for printing.
* \b NOTE:  Changes to the data model \b will be reflected in the original
* table object as well as the print object.
*
* To easily perform WYSIWYG printing, \a addons folder contains helper class
* QicsPrintPreviewDialog. It provides static method previewAndPrint(QicsTable *table)
* which invokes preview of the \a table.
* \sa \link print_preview Print Preview Framework \link
*/
////////////////////////////////////////////////////

/*! \file */


struct QICS_EXPORT PagePreviewParams
{
    QicsICell start_cell;
    bool first_row;
    bool first_col;
    QPixmap buf;
    QPixmap buf_tn;
};

////////////////////////////////////////////////////

class QICS_EXPORT QicsTablePrint:  public QicsTableCommon
{
    Q_OBJECT
public:

    Q_ENUMS( QicsTableDisplayOption )

    /** @name Table Printing Properties
    */

    //@{
    /*!
    * Specifies the page margin of the table.  The page margin is the
    * space between the edge of the printable area of the page and
    * the edges of the table.
    *
    * The default value of this property is \b 10.
    */
    Q_PROPERTY( int pageMargin READ pageMargin WRITE setPageMargin )
    //@}

public:
    /*!
    * Constructor for the class.
    * \param table the table object that is used to set initial attribute
    *              values in the print object.
    */
    QicsTablePrint(QicsTable *table);

    /*!
    * \internal
    * Constructor for the class.
    * \param tc the table object that is used to set initial attribute
    *           values in the print object.
    */
    QicsTablePrint(QicsTableCommon *tc);
    /*!
    * Destructor for the class.
    */
    virtual ~QicsTablePrint();

    /*
    * Returns the page margin.
    * See the \link #pageMargin "pageMargin" \endlink
    * property for details.
    * \sa setPageMargin()
    */
    inline int pageMargin() const { return myPageMargin; }

public slots:
    /*!
    * Prints the entire table.
    * \param printer the device to print to.
    * <h5>Example</h5> <pre><code>
    QicsDataModel *dm = new QicsDataModelDefault(numRows, numCols);
    QicsTable *table = new QicsTable(dm, 0);
    QicsTablePrint *printTable = new QicsTablePrint(table);
    QPrinter printer;
    printer.setOutputFileName("printed.ps");
    printTable->print(&printer);
    delete printTable;
    </code>
    </pre>
    */
    void print(QPrinter *printer);
    /*!
    * Prints a region of the table.
    * \param printer the device to print to.
    * \param region the row/column region to be printed.
    *
    */
    void print(QPrinter *printer, const QicsRegion &region);

    /*!
    * Sets the page margin of the table.
    * See the \link #pageMargin "pageMargin" \endlink
    * property for details.
    * \sa pageMargin()
    */
    inline void setPageMargin(int margin) { myPageMargin = margin; }

    /*!
    Sets print preview widget to \a pw. This widget will be used
    as a WYSIWYG printing base.
    See \link print_preview Print Preview \endlink and \link stocks2_demo Stocks2 \endlink demo.
    \sa QicsPrintPreviewWidget
    */
    void setPreviewWidget(QicsPrintPreviewWidget *pw);

    /*!
    Sets print preview region to \a region. By default, the whole table
    will be used.
    \sa setPreviewWidget(), QicsPrintPreviewWidget
    */
    void setPreviewRegion(const QicsRegion &region = QicsRegion());

protected slots:
    void countPages(int *pages, QPrinter *printer);
    void drawPage(QicsPrintPreviewPage *page);
    void printPage(QicsPrintPreviewPage *page, QPainter *painter);
    void terminateCount();

protected:
    /*!
    * \internal
    * Copies values from \a tc to the print object.
    */
    virtual void initObjects(QicsTableCommon *tc);

    /*!
    * \internal
    * Sets custom values that are different when printing
    */
    virtual void customize();

    /*!
    * \internal
    * Prints a single page of the table.
    * \param start_cell the first cell to be printed
    * \param painter painter to use when printing
    * \param first_row \b true if start_cell is at the top of the region
    *                  to be printed
    * \param first_col \b true if start_cell is at the left of the region
    *                  to be printed
    * \return the last cell that was printed
    */
    QicsICell printPage(const QicsICell &start_cell, QPainter *painter,
        bool first_row, bool first_col);

    /*!
    * \internal
    * the main style manager
    */
    QicsStyleManager *myStyleManager;
    /*!
    * \internal
    * the row header style manager
    */
    QicsStyleManager *myRowHeaderStyleManager;
    /*!
    * \internal
    * the column header style manager
    */
    QicsStyleManager *myColumnHeaderStyleManager;

    /*!
    * \internal
    * the main dimension manager
    */
    QicsDimensionManager *myDimensionManager;
    /*!
    * the dimension manager used by row headers
    */
    QicsDimensionManager *myRowHeaderDM;
    /*!
    * the dimension manager used by column headers
    */
    QicsDimensionManager *myColumnHeaderDM;

    int myPageMargin;

    QicsRegion myRegion;
    QicsPrintPreviewWidget *myPreview;
    QList<PagePreviewParams> myPageParams;
    bool m_progress;
};

#endif //QICSTABLEPRINT_H


