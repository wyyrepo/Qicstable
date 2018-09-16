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

#ifndef QICSGROUPINFO_H
#define QICSGROUPINFO_H

#include <QList>
#include <QColor>
#include <QFont>

#include <QicsNamespace.h>

class QicsSpecialRowData;
class QicsExpandableStaticRowData;
class QicsTreeTable;
class QicsRow;
class QicsCell;
class QicsDataItem;


/*!
* \class QicsGroupInfo QicsGroupInfo.h
* \brief Helper class which is used to obtain information about groups and
*  to customize their look.
*
*  Stores information for a group and provides pointers to
*  a QicsSpecialRowData objects for header and summary rows.
*  Each %QicsGroupInfo object also contains list of subgroups.
*
*  A lot of group's properties can be read with %QicsGroupInfo API, as well as
*  changed header and summary visual properties such as label, font, and color.
*  This is useful when implementing custom QicsSummarizer to handle over
*  visual parameters of the groups.
*/

////////////////////////////////////////////////////
/*! \file */
////////////////////////////////////////////////////


class QICS_EXPORT QicsGroupInfo
{
    friend class QicsTreeTable;
public:
    /*!
    *  Constructor, creates QicsGroupInfo object for the QicsTreeTable \a table.
    * \a parent is the parent group, or 0 if it is top level group.
    */
    QicsGroupInfo(QicsTreeTable *table, QicsGroupInfo *parent = 0);

    /*!
    *  Destructor for the class.
    */
    ~QicsGroupInfo();

    /*!
    *  Returns pointer to a QicsGroupInfo object of child group at \a index.
    *  Returns 0 if group has no children or index is wrong.
    *  \sa groups(), count(), parentGroup(), level().
    */
    inline QicsGroupInfo* childAt(int index) const;

    /*!
    *  Returns number of subgroups. The same as size().
    *  \sa size().
    */
    inline int count() const { return m_groups.count(); }

    /*!
    *  Returns number of subgroups. The same as count().
    *  \sa count().
    */
    inline int size() const { return m_groups.count(); }

    /*!
    *  Returns list of subgroups.
    *  \sa childAt().
    */
    inline const QList<QicsGroupInfo*> &groups() const { return m_groups; }

    /*!
    *  Returns a list of indexes of table rows in the group.
    *  Indexes are model-based. The order of the rows in the list is
    *  undefined.
    */
    inline const QList<int>& rows() const { return m_rows; }

    /*!
    *  Returns QicsTreeTable object.
    */
    inline QicsTreeTable* table() { return m_table; }

    /*!
    *  Returns parent group or 0 if this group is top-level.
    *  \sa childAt(), level()
    */
    inline QicsGroupInfo* parentGroup() { return m_parent; }

    /*!
    *  Returns index of the column of this group.
    */
    inline int columnIndex() const { return m_col; }

    /*!
    *  Returns 0-based level of this group.
    *  \sa isFirst(), isLast().
    */
    inline int level() const { return m_lev; }

    /*!
    *  Returns content of this group.
    *  \since 2.4
    */
    inline const QString& content() const { return m_content; }

    /*!
    *  Returns data of this group.
    *  \since 2.4
    */
    inline const QString& data() const { return m_data; }

    /*!
    *  Returns true if this group is top-level (level is 0).
    *  Equal to !level().
    *  \sa isLast(), level().
    */

    inline bool isFirst() const { return !m_lev; }

    /*!
    *  Returns true if this group is last-level (i.e. contains only rows with data).
    *  \sa isFirst(), level().
    */
    bool isLast() const;

    /*!
    *  Returns visual index of the first data row of the group.
    *  \sa lastDataRowVisualIndex()
    *	\since 2.4
    */
    int firstDataRowVisualIndex() const;

    /*!
    *  Returns visual index of the last data row of the group.
    *  \sa firstDataRowVisualIndex()
    *	\since 2.4
    */
    int lastDataRowVisualIndex() const;

    /*!
    *  Returns index of the group header row in the data model.
    *  \sa headerVisualIndex()
    */
    inline int headerModelIndex() const { return m_hmi; }

    /*!
    *  Returns index of the group header row in the view.
    *  \sa headerModelIndex()
    */
    int headerVisualIndex() const;

    /*!
    *  Returns reference to header row.
    *  \sa headerCellRef(int index)
    */
    QicsRow& headerRef();

    /*!
    *  Returns reference to header row in the row header.
    */
    QicsRow& headerRowRef();

    /*!
    *  Returns reference to cell at column index \a col in the header row.
    *  Returns empty cell if index is not valid.
    *  \sa headerRef()
    */
    QicsCell& headerCellRef(int index);

    /*!
    *  Returns pointer to a QicsExpandableStaticRowData object for
    *  the group header row.
    */
    QicsExpandableStaticRowData* headerRowData() const;

    /*!
    *  Returns index of the group summary row in the data model.
    *  \sa summaryVisualIndex()
    */
    inline int summaryModelIndex() const { return m_smi; }

    /*!
    *  Returns index of the group summary row in the view.
    *  \sa summaryModelIndex()
    */
    int summaryVisualIndex() const;

    /*!
    *  Returns reference to summary row.
    *  \sa summaryCellRef(int index);
    */
    QicsRow& summaryRef();

    /*!
    *  Returns reference to summary row in the row header.
    */
    QicsRow& summaryRowRef();

    /*!
    *  Returns reference to cell at column index \a col in the summary row.
    *  Returns empty cell if index is not valid.
    *  \sa summaryRef()
    */
    QicsCell& summaryCellRef(int index);

    /*!
    *  Returns pointer to a QicsSpecialRowData object for
    *  the group summary row.
    */
    QicsSpecialRowData* summaryRowData() const;

    /*!
    *  Shows summary row for this group according to \a show.
    *  Summaty row will be shown only if table accepts summary rows.
    *  \sa QicsTreeTable::setSummaryRowsVisible()
    */
    void showSummary(bool show=true);

    /*!
    *  Hides summary row. Equal to showSummary(false);
    *  \sa showSummary()
    */
    inline void hideSummary() { showSummary(false); }

    /*!
    *  Spans/unspans summary row depending of \a span.
    *  (spans if span is true, unspans if false)
    */
    void spanSummary(bool span);

    /*!
    *  Spans/unspans header row depending of \a span.
    *  (spans if span is true, unspans if false)
    */
    void spanHeader(bool span);

    /*!
    *  Returns reference to cell in the group at row \a row and column \a col (model coordinates).
    *  Rows are relative-based beginning since 0.
    */
    QicsCell& cellRef(int row, int col);

    /*!
    *  Sets data \a item of summary row for column \a col (model coordinates).
    *  Since 2.4.
    */
    void setSummaryData(int col, const QicsDataItem &item);

    /*!
    *  Sets data \a item of header row for column \a col (model coordinates).
    *  Since 2.4.
    */
    void setHeaderData(int col, const QicsDataItem &item);

    /*!
    *  Performs recalculation of summary row.
    *  Since 2.4.
    */
    void recalculate();

private:
    void clear();
    QicsGroupInfo* addGroup();
    void addRow(int index);

    inline void setHeaderIndex(int i) { m_hmi = i; }
    inline void setSummaryIndex(int i) { m_smi = i; }
    inline void setColumnIndex(int i) { m_col = i; }
    inline void setLevel(int i) { m_lev = i; }
    inline void setContent(const QString &s) { m_content = s; }
    inline void setData(const QString &s) { m_data = s; }

    QList<QicsGroupInfo*> m_groups;
    QList<int> m_rows;
    int m_smi, m_hmi, m_col, m_lev;
    QString m_content, m_data;
    QicsTreeTable *m_table;
    QicsGroupInfo *m_parent;
};

inline QicsGroupInfo* QicsGroupInfo::childAt(int index) const
{
    return index < 0 || index >= count() ? 0 : m_groups.at(index);
}

#endif //QICSGROUPINFO_H


