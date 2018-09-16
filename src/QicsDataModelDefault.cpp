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

#include "QicsDataModelDefault.h"



QicsDataModelDefault::QicsDataModelDefault(int num_rows, int num_cols, QObject *parent)
    : QicsDataModel(num_rows, num_cols, parent)
{
    // we essentially leave the model empty at this point
    // only expand what we really need, it makes insertion
    // slightly longer, but for large sparse models it saves
    // alot of space.
}

QicsDataModelDefault::~QicsDataModelDefault()
{
    // NOTE: i was tempted to re-use code and call the deleteRow
    // member for every row, but then i realized that they do different
    // things and that deleteRow is very slow for the full model, as you
    // shift everything above it up.

    m_emitSignals = false;
    clearModel();
    // no need to reset the flag as the model is dead.
}

QicsDataModel *QicsDataModelDefault::create(int num_rows , int num_cols, QObject *parent)
{
    return (new QicsDataModelDefault(num_rows, num_cols, parent));
}

void QicsDataModelDefault::clearModel()
{
    // Go through each Row and delete all items,
    // then delete the Rows themselves.

    // only send the signal once
    bool old_emit = m_emitSignals;
    m_emitSignals = false;

    int num_rows = numRows();
    int num_cols = numColumns();

    const int row_size = myVectorOfRowPointers.size();
    for (int r = 0; r < row_size; ++r) {
        QicsDataItemPV *the_row_vec = myVectorOfRowPointers.at(r);

        if (the_row_vec) {
            clearRow(r);
            delete the_row_vec;
            the_row_vec = 0;
        }
    }

    myVectorOfRowPointers.clear();

    setNumRows(0);
    setNumColumns(0);

    // reset.
    m_emitSignals = old_emit;

    if (m_emitSignals) {
        emit modelSizeChanged(numRows(), numColumns());
        emit rowsDeleted(num_rows, 0);
        emit columnsDeleted(num_cols, 0);
    }
}

void QicsDataModelDefault::insertColumns(int number_of_cols, int starting_position)
{
    // NOTE:
    // Ok, here is some pain, we need to go to every row, verify
    // that we have a row, then see if the row has expanded out to the column,
    // if it has, then we need to expand the vector, if not its a noop since
    // nothing needs to shift.
    if ((number_of_cols <= 0) || (starting_position <0 )) return;

    emit prepareForColumnChanges(number_of_cols, starting_position);

    if (starting_position > lastColumn()) {
        // in this case no columns will be past where we want to insert,
        // so just give the model permission to expand this far.
        // We assume that we are just going to add number_of_cols to the
        // end of the model.

        starting_position = lastColumn() + 1;
        setNumColumns(numColumns() + number_of_cols);
    }
    else {
        // we are in the model, see if there are columns to shift.
        // go through each row, if there is a row there, and its big enough
        // to have a column which is past the starting_position, then we
        // need to insert. (this will be painfull).
        const int row_size = myVectorOfRowPointers.size();

        for (int r = 0; r < row_size; ++r) {
            if (myVectorOfRowPointers.at(r)) {
                QicsDataItemPV &the_row_vec = *(myVectorOfRowPointers.at(r));
                if (the_row_vec.size()>= starting_position) {
                    // we need to insert here then...
                    QicsDataItemPV::iterator pos = the_row_vec.begin() + starting_position;
                    // shift everything in this vector.
                    the_row_vec.insert(pos, number_of_cols, 0);
                }
            }
        }
        // increase the alowed model size.
        setNumColumns(numColumns() + number_of_cols);
    }

    emit columnsInserted(number_of_cols, starting_position);

    if (m_emitSignals)
        emit modelSizeChanged(starting_position, number_of_cols, Qics::ColumnIndex);
}

void QicsDataModelDefault::insertRows(int number_of_rows, int starting_position)
{
    // NOTE:
    // Inserting rows is easy (compared to columns). basically we
    // just need to expand MyVectorOfRowPointers.. everything will
    // just shift correctly. One Operation on a potentially big vector
    // this is painful in column.

    // make sure we are in bounds..
    // and no one is trying to clobber us with a bad number of rows.
    if (starting_position < 0 || number_of_rows <= 0 ) return;

    emit prepareForRowChanges(number_of_rows, starting_position);

    // for insertions out of model, just expand the model.
    if (starting_position > lastRow()) {
        // We assume that we are just going to add number_of_rows to the
        // end of the model.
        starting_position = numRows();
        setNumRows(numRows() + number_of_rows);
    }
    else {
        // We are in the model, we will need to shift.
        // if we are too small then we are done too..
        if(myVectorOfRowPointers.size() > starting_position) {
            QicsDataItemPVPV::iterator pos = myVectorOfRowPointers.begin() + starting_position;
            // nope, we got here and we have things beyond this point, we will
            // need to do the expensive shift.. thank god this isn't a column,
            // then it would be really expensive.
            myVectorOfRowPointers.insert(pos, number_of_rows, 0);
        }
        setNumRows(numRows() + number_of_rows);
    }

    emit rowsInserted(number_of_rows, starting_position);

    if (m_emitSignals)
        emit modelSizeChanged(starting_position, number_of_rows, Qics::RowIndex);
}

void QicsDataModelDefault::addColumns(int cols)
{
    setNumColumns(numColumns() + cols);

    emit columnsAdded(cols);

    if (m_emitSignals)
        emit modelSizeChanged(numRows(), numColumns());
}

void QicsDataModelDefault::addRows(int rows)
{
    setNumRows(numRows() + rows);

    emit rowsAdded(rows);

    if (m_emitSignals)
        emit modelSizeChanged(numRows(), numColumns());
}

const QicsDataItem *QicsDataModelDefault::item(int row, int col) const
{
    // get what's in the row position..
    QicsDataItemPV *the_row_vec = myVectorOfRowPointers.value(row, 0);

    // verify that the row isn't empty
    if (!the_row_vec)
        return 0;

    // we are all set. get it.
    return the_row_vec->value(col, 0);
}

QicsDataModelRow QicsDataModelDefault::rowItems(int row, int first_col, int last_col) const
{
    QicsDataModelRow qt;

    if ((last_col == -1) || (last_col > lastColumn()))
        last_col = lastColumn();

    // create the vector, even if we don't have it, we will return a full
    // vector containing NULLS.
    qt.resize(last_col - first_col + 1);

    // Now let's see what we can put in here.. if we don't have the required row
    // then return
    if((myVectorOfRowPointers.size() <= row) ||	(!myVectorOfRowPointers.at(row)))
        return qt;

    QicsDataItemPV &the_row_vec = *(myVectorOfRowPointers.at(row));
    for (int col = first_col; col <= last_col; ++col)
        qt[col-first_col] = the_row_vec.at(col);

    return qt;
}

QicsDataModelColumn QicsDataModelDefault::columnItems(int col, int first_row, int last_row) const
{
    QicsDataModelColumn qc;

    if ((last_row == -1) || (last_row > lastRow()))
        last_row = lastRow();

    // set it up and make them all null
    qc.resize(last_row - first_row + 1);

    const int row_size = myVectorOfRowPointers.size();
    for (int r = first_row; ((r <= last_row) && (r < row_size));  ++r) {
        if(myVectorOfRowPointers.at(r)) {
            QicsDataItemPV &the_row_vec = *(myVectorOfRowPointers.at(r));
            if (col < the_row_vec.size())
                qc[r-first_row] = the_row_vec.at(col);
        }
    }

    return qc;
}

void QicsDataModelDefault::setItem(int row, int col, const QicsDataItem &it)
{
    // sanity check...
    if (!contains(row, col) ) return;

    // first, let's expand the model to that row if we need to.
    if (myVectorOfRowPointers.size() <= row) {
        // Ok, there aren't even the required number of columns
        // in this vector. let's expand the number of columns.
        // This doesnt give us rows, just places (columns) to
        // put them
        //SLOW ME DOWN BABY, lets speed this up.
        //myVectorOfRowPointers.resize(row+1);
        myVectorOfRowPointers.resize(myNumRows);
    }

    // see if we have a row there (this row might not be used yet)
    QicsDataItemPV *the_row_vec = myVectorOfRowPointers.at(row);
    if (!the_row_vec) {
        //Ok this just gives us a row vector, make it at leaste
        //as big as the column we need to set. add one for good luck.
        //myVectorOfRowPointers[row] = the_row_vec = new QicsDataItemPV(col+1);
        myVectorOfRowPointers[row] = the_row_vec = new QicsDataItemPV(myNumColumns);
    }

    // now everyone is happy, lets make a deep copy of this.
    // first we have to do this weird reference cast to make the world wonderful

    // now lets see if we have enough columns
    if (the_row_vec->size() <= col)
        the_row_vec->resize(myNumColumns);

    // see if there is already something at this location:
    QicsDataItem *item = the_row_vec->at(col);
    delete item;

    the_row_vec->replace(col, it.clone());

    if (m_emitSignals) {
        emit modelChanged(QicsRegion(row,col,row,col));
        emit cellValueChanged( row, col );
    }
}

void QicsDataModelDefault::clearItem (int row, int col)
{
    QicsDataItemPV *the_row_vec = myVectorOfRowPointers.value(row, 0);
    if (!the_row_vec)
        return;

    QicsDataItem *item = the_row_vec->value(col, 0);
    if (!item)
        return;

    delete item;
    the_row_vec->replace(col, 0);

    if (m_emitSignals)
        emit modelChanged(QicsRegion(row,col));
}

void QicsDataModelDefault::deleteRows(int num_rows, int start_row)
{
    if ((start_row < 0) || (num_rows <= 0))
        return;

    emit prepareForRowChanges(0-num_rows, start_row);

    bool oldSignalFlag = m_emitSignals;
    // Only one signal please
    m_emitSignals = false;

    int rows_deleted = 0;

    while ((num_rows > rows_deleted) && (start_row <= lastRow())) {
        if (myVectorOfRowPointers.size() > start_row) {
            // we have allocated space for the row
            // make sure the row itself was allocated.
            //if (myVectorOfRowPointers[start_row]) {
            clearRow(start_row);
            QicsDataItemPVPV::iterator pos = myVectorOfRowPointers.begin() + start_row;
            // delete this vector;
            delete (*pos);
            myVectorOfRowPointers.erase(pos);
            //}
        }

        setNumRows(numRows() - 1);
        ++rows_deleted;
    }

    // reset the emit flag
    m_emitSignals = oldSignalFlag;

    if (rows_deleted > 0) {
        emit rowsDeleted(rows_deleted, start_row);

        if (m_emitSignals)
            emit modelSizeChanged(start_row, 0 - num_rows, Qics::RowIndex);
    }
}

void QicsDataModelDefault::deleteColumns(int num_cols, int start_col)
{
    if ((start_col < 0) || (num_cols <= 0))
        return;

    emit prepareForColumnChanges(0-num_cols, start_col);

    int cols_deleted = 0;

    while ((num_cols > cols_deleted) && (start_col <= lastColumn())) {
        // go through each row, if there is a row there, and its bigenough
        // to have a column which is past the starting_position, then we
        // need to erase. (this will be painfull).
        const int num_rows = myVectorOfRowPointers.size();
        for (int r = 0; r < num_rows; ++r) {
            if (myVectorOfRowPointers.at(r)) {
                QicsDataItemPV &the_row_vec = *(myVectorOfRowPointers.at(r));

                if (the_row_vec.size()> start_col) {
                    // we need to delete here then...
                    QicsDataItemPV::iterator pos = the_row_vec.begin() + start_col;
                    delete (*pos);
                    // shift everything in this vector.
                    the_row_vec.erase(pos);
                }
            }
        }
        setNumColumns(numColumns() - 1);
        ++cols_deleted;
    }

    if (cols_deleted > 0) {
        emit columnsDeleted(cols_deleted, start_col);

        if (m_emitSignals)
            emit modelSizeChanged(start_col, 0-num_cols, Qics::ColumnIndex);
    }
}

void QicsDataModelDefault::deleteRow (int row)
{
    deleteRows(1, row);
}

void QicsDataModelDefault::deleteColumn (int col)
{
    deleteColumns(1, col);
}


// This model is designed to add rows fast, its relitivly slow for
// adding columns. basicly, its as slow as just doing a setItem for
// each item. there are alot of extra checks and a few could be reduced
// in this function, but not many.
void QicsDataModelDefault::setColumnItems(int col, const QicsDataModelColumn &in_vector)
{
    if (!contains(0,col)) return; // No-op for col out of model.

    // turn off signal emition while we do many setItems.
    bool old_emit = m_emitSignals;
    m_emitSignals = false;

    for (int r=0; r < in_vector.size(); ++r) {
        // if this row is out, then they all are..
        if (!contains(r,col)) break;

        if (in_vector.at(r))
            setItem(r, col, *(in_vector.at(r)));
        else
            clearItem(r,col);
    }

    m_emitSignals = old_emit;

    if (m_emitSignals)
        emit modelChanged(QicsRegion(0, col, lastRow(), col));
}

// this function has
// slight preformance improvments over calling setItem for each item
// the reasion is we only check for the row once, then we stick
// the items in quickly.
void QicsDataModelDefault::setRowItems(int row, const QicsDataModelRow &in_vector)
{
    if (!contains(row, 0)) return; // No opp if we arnt in the model.

    // first, lets expand the model to that row if we need to.
    if(myVectorOfRowPointers.size() <= row) {
        // Ok, there arnt even the required number of columns
        // in this vector. lets expand the number of columns.
        // This Doesnt give us rows, just places (columns) to
        // put them
        //SLOW ME DOWN BABY, lets speed this up.
        myVectorOfRowPointers.resize(row+1);
    }

    // see if we have a Row There (this row might not be used yet)
    if (!myVectorOfRowPointers.at(row)) {
        //Ok this just gives us a row vector, make it at leaste
        //as big as the column we need to set. add one for good luck.
        myVectorOfRowPointers[row] = new QicsDataItemPV;
    }

    // now everyone is happy, lets make a deep copy of this.
    // first we have to do this wird reference cast to make the world
    //wonderful
    QicsDataItemPV &the_row_vec = *(myVectorOfRowPointers.at(row));

    for (int c=0; c < in_vector.size(); ++c) {
        // verify that this part of the row in is in the model.
        if (!contains(row, c)) break;

        if (in_vector.at(c))
            the_row_vec.push_back(in_vector.at(c)->clone());
        else {
            // they passed us a null.
            the_row_vec.push_back(0);
        }

    }

    if (m_emitSignals)
        emit modelChanged(QicsRegion(row,0,row,lastColumn()));
}

void QicsDataModelDefault::clearRow(int row)
{
   QicsDataItemPV *the_row_vec = myVectorOfRowPointers.value(row, 0);
   if (!the_row_vec)
       return;

    qDeleteAll(*the_row_vec);
    the_row_vec->clear();

    if (m_emitSignals)
        emit modelChanged(QicsRegion(row,0,row,lastColumn()));
}

bool QicsDataModelDefault::isCellEmpty(int row, int col) const
{
    QString text = itemString(row, col);
    return text.isEmpty();
}

bool QicsDataModelDefault::isRowEmpty(int row) const
{
    // iterate on row and check data presense
    QicsDataItemPV *the_row_vec = myVectorOfRowPointers.value(row, 0);
    if (!the_row_vec)
        return true;

    QicsDataItemPV::const_iterator iter, it_e = the_row_vec->constEnd();

    for (iter = the_row_vec->constBegin(); iter != it_e; ++iter) {
        if (*iter && !(*iter)->string().isEmpty())
            return false;
    }

    return true;
}

bool QicsDataModelDefault::isColumnEmpty(int column) const
{
    const int num_rows = myVectorOfRowPointers.size();

    for (int r = 0; r < num_rows; ++r) {
        QicsDataItemPV *the_row_vec = myVectorOfRowPointers.at(r);
        if (the_row_vec) {
            if (the_row_vec->size() > column) {
                QicsDataItemPV::const_iterator pos = the_row_vec->constBegin() + column;
                if (*pos && !(*pos)->string().isEmpty())
                    return false;
            }
        }
    }

    return true;
}


