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

#ifndef QICSSORTER_H
#define QICSSORTER_H

#include <QObject>
#include <QVector>
#include <QDomElement>
#include "QicsNamespace.h"
#include "QicsDataItem.h"

class QicsDataModel;
class QicsAbstractSorterDelegate;

///////////////////////////////////////////////////////////////////////////


/*! \internal
* \class QicsSorter QicsSorter.h
* \brief manager for row and column ordering
*
* The sorter is responsible for mapping from visible display indices
* to physical model indices.   A single sorter should be shared
* between all the data models that must sort or scroll together in a ganged
* cluster.
*
* The Sorter itself does relatively little.  It just provides the
* ordering vector externally to the data model so that several
* tables can share the same one.
*/
////////////////////////////////////////////////////

/*! \file */

////////////////////////////////////////////////////

//
class QICS_EXPORT QicsSorter: public QObject, public Qics
{
    Q_OBJECT
public:

    /*!
    * Constructor for the class.
    */
    QicsSorter(Qics::QicsIndexType _type, QicsDataModel *dm, QObject *parent = 0);
    ~QicsSorter();

    inline QicsDataModel *dataModel() const { return myDataModel; }
    inline Qics::QicsIndexType type() const { return myType; }

    /*!
    * Sets case sensitivity while sorting to \a cs.
    * \since 2.3
    */
    void setSortingSensitivity(Qt::CaseSensitivity cs);

    /*!
    * Gets case sensitivity while sorting.
    * \since 2.4
    */
    Qt::CaseSensitivity sortingSensitivity() const;

    void setDefaultSorterDelegate(QicsAbstractSorterDelegate *sorter);
    inline QicsAbstractSorterDelegate *defaultSorterDelegate() const {return m_defaultSorterDelegate;}

    /*! Returns sorter delegate installed for \a index, or 0 if none.
    */
    inline QicsAbstractSorterDelegate* sorterDelegate(int index) {return m_sorterDelegates.value(index);}

    /*! Install sorter delegate \a index.
    *  If \a sorter = 0, it is equal to call removeSorterDelegate(index).
    */
    void setSorterDelegate(int index, QicsAbstractSorterDelegate *sorter);

    /*! Remove sorter delegate installed for \a index.
    */
    void removeSorterDelegate(int index);

    /*!
    * Returns true if \a index has any sorter delegate installed.
    */
    inline bool hasSorterDelegate(int index) const {return m_sorterDelegates.contains(index);}

    /*!
    * Reorder the indices based on the data in the model
    * \param otherAxisesIndex The data model index of the column or
    * row to sort on.   The interpretation is picked appropriately
    * based on the type of this instance.
    * \param order Ascending or Descending
    * \param from start sort range at this item
    * \param to end sort range at this item.  Default (-1) indicates
    * the end of the vector.
    * \param func an optional comparator function for data with
    * special requirements.
    */
    void sort(const QVector<int> &otherAxisesIndex,
        QicsSortOrder order = Qics::Ascending,
        int from = 0, int to = -1,
        DataItemComparator func = 0);

    /*!
    * map a visual coordinate to a model one
    */
    int visualToModel(int x);

    /*!
    * returns a vector of coords mapped from visual to model
    */
    inline QVector<int>& visualToModelVector() { return m_order; }

    /*!
    * map a model coordinate back to visual one
    */
    int modelToVisual(int x);

    /*!
    * change the ordering vector so that a set of items (possibly
    * discontiguous), is moved to a new location before \a target.
    * \param target move the items before this one (visual coordinates)
    * \param itms item indices to move (visual coordinates)
    */
    void moveItems(int target, const QVector<int> &itms);

    /**
    * Sets the sorting algorithm wich will be used.
    * @param mode  sort mode sets to.
    */
    inline void setSortMode(Qics::QicsSortMode mode) { mySortMode = mode; }

    /**
    * Gets current sorting mode.
    * @return current sorting mode.
    */
    inline Qics::QicsSortMode sortMode() const { return mySortMode; }

    /**
    * Explicitly reorders items as it is specified by \a newOrder as vector of indexes.
    * \a from is starting index, \a to is finishing.
    */
    void reorder(const QVector<int> &newOrder, int from = 0, int to = -1);

    /**
    * Returns current ordering.
    */
    const QVector<int>& currentOrder()
    {
        if (m_order.isEmpty()) fillVisualToModelMap();
        return m_order;
    }

    void deleteVisualElements(int how_many, int start);

protected slots:
    /*! \internal
    * Handles insertion of \a num items.   This slot is from the
    * model up to the view, not the other way around
    */
    void insertElements(int num, int start_position);

    /*! \internal
    * Handles insertion of \a num items at the end.  This slot is
    * from the model to the view, not the other way around.
    */
    void appendElements(int num);

    /*! \internal
    * Handles element visibility changes
    */
    void elementVisibilityChanged(int index, bool visible);

    /*! \internal
    * Handles deletion of \a num items.  This slot is
    * from the model to the view, not the other way around.
    */
    void deleteElements(int num, int start_position);

    QDomElement toDomXml(const QString &tag, QDomDocument* doc) const;

    void configureFromDomXml(const QDomElement&);

protected:
    /*! \internal method to check an instance for sanity.  Makes
    * a lot of assertions.
    */
    void integrityCheck();

    // Property that hold sorting customizations.
    Qics::QicsSortMode mySortMode;

private:
    /*! \internal expand order vector to hold at least \a how_many elements
    */
    void expandTo(int how_many);

    /*! \internal compute the visualToModel map (aka "order") */
    void fillVisualToModelMap();

    /*! \internal compute the modelToVisual map */
    void fillModelToVisualMap();

    /*! \internal toss the modelToVisual map */
    void flushModelToVisualMap();

    /*! \internal do all the things needed when changing the order vector
    */
    void installNewOrder(int *neworder, int newlen);

signals:
    /*! let people know the order has changed
    * \param type the style of axis which changed
    * \param map a list  which would map old visual coordinates to new.
    * old visual coords which vanished will go away.
    */
    void orderChanged(Qics::QicsIndexType type, int *map, int size);

private:
    /*! row or column indicator */
    Qics::QicsIndexType	myType;

    /*! the data model which I belong to */
    QicsDataModel *myDataModel;

    /*! mapping from visual order to physical order */
    QVector<int> m_order;

    QSet<int> m_hidden;

    /*! reverse mapping from physical order to visual order */
    QMap<int, int> m_modelToVisualMap;

    QMap<int, QicsAbstractSorterDelegate *> m_sorterDelegates;
    QicsAbstractSorterDelegate *m_defaultSorterDelegate;
};

#endif //QICSSORTER_H


