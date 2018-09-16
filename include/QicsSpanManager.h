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

#ifndef QICSSPANMANAGER_H
#define QICSSPANMANAGER_H

#include <QObject>
#include <QDomElement>
#include "QicsNamespace.h"
#include "QicsSpan.h"
#include "QicsRegion.h"


class QicsGridInfo;

/*! \internal
* \class QicsSpanManager QicsSpanManager.h
* \brief Manager for span regions in the table
*
* The span manager handles all operations relating to span regions in the QicsTable.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

class QICS_EXPORT QicsSpanManager: public QObject, public Qics
{
    Q_OBJECT
public:
    /*!
    * The constructor
    */
    QicsSpanManager(QicsGridInfo *grid_info, QObject *parent = 0);

    /*!
    * Copy constructor
    */
    QicsSpanManager(const QicsSpanManager &sm);

    ~QicsSpanManager();

    /*!
    * Adds a cell spanning region to the table.  The contents of the cell
    * at the top, left of \a span will be drawn across all cells in \a span.
    * Two cell spanning regions cannot overlap.  If this method is called with
    * a spanning region that overlaps an existing spanning region, the method
    * will return \b false.  If the new spanning region is accepted, the method
    * will return \b true.
    *
    * The coordinates must be in model coordinate, rather than visual.
    */
    bool addCellSpan(const QicsSpan &span);
    /*!
    * Removes a cell spanning region from the table.  Because cell spanning regions
    * cannot overlap, it is sufficient to specify the existing region by giving
    * its starting cell.
    */
    void removeCellSpan(int start_row, int start_col);
    /*!
    * Removes all the spans.
    * \since 2.2
    */
    void removeAllSpans();

    /*!
    * Returns the table's cell spanning list in model coordinates.  The returned value
    * is a \b copy of the table's internal list, and it is the responsibility
    * of the caller to delete it when it is no longer used.  Modifications
    * to this list will not affect the table -- use QicsSpanManager::addCellSpan
    * and QicsSpanManager::removeCellSpan instead.
    * \sa visualCellSpanList()
    */
    QicsSpanList *cellSpanList() const;

    /*!
    * Returns \b true if cell (\a row, \a col ) is the beginning cell in a
    * cell span region, \b false otherwise.  When this method returns \a true,
    * \a reg_return is set to the applicable cell span region.
    * Note: row and column are in \a visual coordinates
    */
    bool isSpanner(const QicsGridInfo &, int row, int col, QicsRegion &reg_return) const;

    /*!
    * Returns \b true if cell (\a row, \a col ) is in a cell span region,
    * \b false otherwise. It is faster then calling isSpanner().
    * When this method returns \a true, \a spanner is set to true if the cell is spanner,
    * and \a reg_return is set to the applicable cell span region.
    * Note: row and column are in \a visual coordinates
    * \since 2.2
    */
    bool insideSpan(const QicsGridInfo &, int row, int col, QicsRegion &reg_return, bool *spanner = 0) const;

    /*!
    * Iterates through all cell span regions, looking for regions
    * that begin in cells in row \a row.  Returns a span whose
    * row and height are the maximum row span.
    * \param gi the GridInfo to use for mapping
    * \param row the row to check in \a visual coordinates
    * \return the maximal region in \a visual coordinates
    */
    QicsRegion maxSpanForRow(QicsGridInfo &gi, int row) const;
    /*!
    * Iterates through all cell span regions, looking for regions
    * that begin in cells in column \a col.  Returns a region whose
    * col and width are the maximum column span.
    * \param gi the GridInfo to use for mapping
    * \param col the column to check in \a visual coordinates
    * \return the maximal region in \a visual coordinates
    */
    QicsRegion maxSpanForColumn(QicsGridInfo &gi, int col) const;

    /*! \internal
    * Sets the flag which specifies if the style manager should emit
    * a signal whenever a property is changed.
    * \since 2.2
    */
    inline void setReportChanges(bool report) { myReportChanges = report; }

    /*! \internal
    * Returns the flag which specifies if the style manager should emit
    * a signal whenever a property is changed.
    * \since 2.2
    */
    inline bool isReportingChanges() const { return myReportChanges; }

public slots:

    void insertRows(int num, int start_position);

    void insertColumns(int num, int start_position);

    void deleteRows(int num, int start_position);

    void deleteColumns(int num, int start_position);
    /*!
    *  Store configuration to \a doc.
    */
    QDomElement toDomXml(QDomDocument* doc);
    /*!
    *  Reconfigure from QDomElement.
    */
    void configureFromDomXml(const QDomElement&);

signals:
    /*!
    * Signaled when a span is added or removed.
    */
    void spanChanged(const QicsSpan &span);

private slots:
    void columnVisibilityChanged(int index, bool visible);
    void rowVisibilityChanged(int index, bool visible);

protected:
    // \internal the cell span list.   Cells on the list
    // are in model coordinates, so we do not have to remap
    // them when sorting
    QicsSpanList myCellSpanList;

    bool myReportChanges;

    QicsGridInfo *myInfo;
};

#endif //QICSSPANMANAGER_H


