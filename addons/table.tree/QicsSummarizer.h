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

#ifndef QICSSUMMARIZER_H
#define QICSSUMMARIZER_H

/*!
*  \class QicsSummarizer QicsTreeTable.h
*  \brief Used for handling summary and header rows.
*
*  QicsSummarizer should be subclassed by user and then added to QicsTreeTable
*  using setSummarizer() function. Every time when the table regroup occurs,
*  QicsTreeTable calls QicsSummarizer::onGrouping(QicsGroupInfo *gi). The table iterates through
*  all QicsGroupInfo objects and calls this function for each of them. The subclassed
*  implementation can make appropriate actions for header and summary rows.
*
*  An example how the summarizer could be subclassed.

*  \code
class MySummarizer : public QicsSummarizer
{
public:
virtual void onGrouping(QicsGroupInfo *gi)
{
// show summaty rows only for 1st two groups
if (gi->level() > 1)
gi->hideSummary();

// set attributes of the summary label
QString s = QString("Grouped by: %1  Items count: %2  Level: %3")
.arg(gi->columnIndex()).arg(gi->rows().count()).arg(gi->level());
// make an indent according to the level of the group
if (gi->level()) {
QString lvl; lvl.fill(' ', gi->level());
s = lvl + s;
}
gi->summaryRef().setLabel(s);
gi->summaryRef().setBackgroundColor(gi->isLast() ? Qt::blue : gi->isFirst() ? Qt::yellow : Qt::gray);
gi->summaryRef().setFont(QFont("Courier", 10-gi->level(), QFont::Bold, true));

if (gi->isLast())
gi->summaryRef().setForegroundColor(Qt::yellow);

// set attributes of the header label
gi->headerRef().setBackgroundColor(gi->level() ? Qt::cyan : Qt::green);
gi->headerRef().setFont(QFont("Arial", 12-gi->level(), QFont::Bold));
}
};
*  \endcode
*/

class QICS_EXPORT QicsSummarizer
{
public:
    /*!
    * Constructor, does nothing. Used for subclassing.
    */
    QicsSummarizer() {};
    virtual ~QicsSummarizer() {};

    /*!
    * Called each time when regroup by column occurs,
    * passing QicsGroupInfo object \a gi as parameter.
    * Used for subclassing.
    */
    virtual void onGrouping(QicsGroupInfo *gi) = 0;

    /*!
    * Called each time when value of the cell at \a row, \a col
    * is changed, passing QicsGroupInfo object \a gi as parameter
    * (it will be 0 however if there are active groups).
    * Used for subclassing.
    * \since 2.4
    */
    virtual void onCellValueChanged(int row, int col, QicsGroupInfo *gi) = 0;
};

#endif //QICSSUMMARIZER_H


