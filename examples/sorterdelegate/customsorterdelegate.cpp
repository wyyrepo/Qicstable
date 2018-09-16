/****************************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of an example program for QicsTable. This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "customsorterdelegate.h"

#include <QicsDataModelQtModelAdapter.h>


CustomSorterDelegate::CustomSorterDelegate()
    : QicsAbstractSorterDelegate()
{
}

CustomSorterDelegate::~CustomSorterDelegate()
{
}

int CustomSorterDelegate::compare(const int &index1, const int &index2, const int &sortIndex)
{
    QicsDataModel *dataModel = model();

    if (!dataModel) return 0;

    QicsDataModelQtModelAdapter *model_adapter = qobject_cast<QicsDataModelQtModelAdapter *>(dataModel);
    if (!model_adapter) return 0;

    QAbstractItemModel* qt_model = model_adapter->model();
    if (!qt_model) return 0;

    QModelIndex itemIndex1, itemIndex2;

    if (indexType() == Qics::RowIndex) {
        itemIndex1 = qt_model->index(index1, sortIndex);
        itemIndex2 = qt_model->index(index2, sortIndex);
    }
    else {
        itemIndex1 = qt_model->index(sortIndex, index1);
        itemIndex2 = qt_model->index(sortIndex, index2);
    }

    if (!itemIndex1.isValid() || !itemIndex2.isValid()) return 0;

    QString itemType1 = qt_model->data(itemIndex1, Qt::UserRole).toString();
    QString itemType2 = qt_model->data(itemIndex2, Qt::UserRole).toString();

    QString itemValue1 = qt_model->data(itemIndex1, Qt::DisplayRole).toString();
    QString itemValue2 = qt_model->data(itemIndex2, Qt::DisplayRole).toString();

    bool item1IsEmpty = itemValue1.isEmpty();
    bool item2IsEmpty = itemValue2.isEmpty();

    if (item1IsEmpty && item2IsEmpty) return 0;
    if (item1IsEmpty) return 1;
    if (item2IsEmpty) return -1;

    int ordFlip = (sortOrder() == Qics::Ascending) ? 1 : -1;

    if (itemType1 == itemType2) {
        // numbers compare
        if (isNumberType(itemType1)) {
            bool ok1, ok2;
            double val1 = itemValue1.toDouble(&ok1);
            double val2 = itemValue2.toDouble(&ok2);
            if (ok1 && ok2)
                return (val1 == val2 ? 0 : ordFlip * (val1 < val2 ? -1 : 1));

            if (ok1)
                return -1;
            if (ok2)
                return 1;
        }

        if (itemType1.toUpper() == "HEX") {
            // Special handling for Hexadezimal values
            bool ok1, ok2;
            int val1 = itemValue1.toInt(&ok1, 16);
            int val2 = itemValue2.toInt(&ok2, 16);
            if (ok1 && ok2)
                return (val1 == val2 ? 0 : ordFlip * (val1 < val2 ? -1 : 1));

            if (ok1)
                return -1;
            if (ok2)
                return 1;

        }
        // You can add check for another types
        // f.e. date, time, etc.
    }
    // string compare
    return ordFlip * stringCompare(itemValue1, itemValue2);
}

bool CustomSorterDelegate::isNumberType(const QString &typeName) const
{
    QString type_name = typeName.toUpper();

    if (type_name != "STRING" && type_name != "HEX")
        return true;

    return false;
}


