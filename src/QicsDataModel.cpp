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

#include "QicsDataModel.h"

#include <QString>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include "QicsDataItem.h"


QicsDataModel::QicsDataModel(int num_rows, int num_cols, QObject *parent)
    : QObject(parent), myNumRows(num_rows), myNumColumns(num_cols),
        m_emitSignals(true)
{
    if (myNumColumns < 0)
        myNumColumns = 0;

    if (myNumRows < 0)
        myNumRows = 0;
}

QicsDataModel::~QicsDataModel()
{
}

QString QicsDataModel::itemString(int row, int col) const
{
    const QicsDataItem *itm = item(row, col);
    return (itm ? itm->string() : QString());
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Adapted from code with the following copyright:

/* Copyright (C) 1999 Lucent Technologies */
/* Excerpted from 'The Practice of Programming' */
/* by Brian W. Kernighan and Rob Pike */

class QicsCSVReader
{
public:
    QicsCSVReader(QTextStream &stream, const char separator)
        : myStream(stream), mySeparator(separator)
    {
    }

    int getLine(QString &str);
    const QString & getField(int n) const;
    inline int getNumFields() const { return myNumFields; }

protected:
    QTextStream &myStream;
    QString myLine;
    QVector<QString> myFields;
    int myNumFields;
    char mySeparator;

    int split();
    int endOfLine(QChar c);
    int advPlain(const QString& line, QString& fld, int);
    int advQuoted(const QString& line, QString& fld, int);
};

// endofline: check for and consume \r, \n, \r\n, or EOF
int QicsCSVReader::endOfLine(QChar c)
{
    int eol;

    eol = (c=='\r' || c=='\n');
    if (c == '\r') {
        if (!myStream.atEnd()) {
            myStream >> c;
            if (c != '\n')
                myStream.device()->ungetChar(c.toLatin1());	// read too far
        }
    }

    return eol;
}

// getline: get one line, grow as needed
int QicsCSVReader::getLine(QString& str)
{
    QChar c;

    myLine = QString();

    while (!myStream.atEnd()) {
        myStream >> c;

        if (!endOfLine(c))
            myLine.append(c);
        else
            break;
    }

    split();
    str = myLine;

    return (!myStream.atEnd());
}

// split: split line into fields
int QicsCSVReader::split()
{
    QString fld;
    int i, j;

    myNumFields = 0;
    if (myLine.length() == 0)
        return 0;

    i = 0;
    do {
        if ((i < static_cast<int> (myLine.length())) && (myLine[i] == '"'))
            j = advQuoted(myLine, fld, ++i);	// skip quote
        else
            j = advPlain(myLine, fld, i);
        if (myNumFields >= static_cast<int> (myFields.size()))
            myFields.push_back(fld);
        else
            myFields[myNumFields] = fld;
        myNumFields++;
        i = j + 1;
    }
    while (j < static_cast<int> (myLine.length()));

    return myNumFields;
}

// advquoted: quoted field; return index of next separator
int QicsCSVReader::advQuoted(const QString& str, QString& fld, int i)
{
    int j;

    fld = "";

    for (j = i; j < str.length(); ++j) {
        if (str[j] == '"' && str[++j] != '"') {
            int k = str.indexOf(mySeparator, j);
            if (k == -1)	// no separator found
                k = str.length();
            for (k -= j; k-- > 0; )
                fld += str[j++];
            break;
        }
        fld += str[j];
    }

    return j;
}

// advplain: unquoted field; return index of next separator
int QicsCSVReader::advPlain(const QString& str, QString& fld, int i)
{
    int j;

    j = str.indexOf(mySeparator, i); // look for separator
    if (j == -1)               // none found
        j = str.length();
    fld = str.mid(i, (j - i));

    return j;
}

// getfield: return n-th field
const QString &QicsCSVReader::getField(int n) const
{
    static QString blank("");
    if (n < 0 || n >= myNumFields)
        return blank;
    else
        return myFields[n];
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void QicsDataModel::readASCII(QTextStream &stream, const char separator,
                         int start_row, int start_col, bool clear_model, bool add_columns)
{
    bool old_emit = m_emitSignals;
    m_emitSignals = false;

    if (clear_model)
        clearModel();

    QString line;
    int row = start_row;

    QicsCSVReader reader(stream, separator);

    while (reader.getLine(line) != 0 || (!line.isEmpty())) {
        // do we need to add a row?
        if (row > lastRow())
            addRows(row - lastRow());

        int nfields = reader.getNumFields();

        // do we need to add columns?
        if (add_columns && ((start_col + nfields - 1) > lastColumn()))
            addColumns(start_col + nfields - 1 - lastColumn());

        int col = start_col;
        for (int i = 0; i < nfields; ++i) {
            setItem(row, col, reader.getField(i));
            ++col;
        }

        ++row;
    }

    m_emitSignals = old_emit;

    if (m_emitSignals)
        emit modelSizeChanged(numRows(), numColumns());
}

void QicsDataModel::writeASCII(QTextStream &stream, const char separator,
                          int start_row, int start_col,
                          int nrows, int ncols)
{
    if (nrows == -1)
        nrows = lastRow();

    if (ncols == -1)
        ncols = lastColumn();

    for (int i = start_row; i <= (start_row + nrows); ++i) {
        if (i > nrows)
            break;

        for (int j = start_col; j <= (start_col + ncols); ++j) {
            if (j > ncols)
                break;

            if (j != start_col)
                stream << separator;

            const QicsDataItem *itm = item(i, j);
            if (itm)
                stream << itm->string();
        }

        stream << "\n";
    }
}

bool QicsDataModel::isRowEmpty(int row) const
{
    Q_UNUSED(row);
    return false;
}

bool QicsDataModel::isColumnEmpty(int col) const
{
    Q_UNUSED(col);
    return false;
}

bool QicsDataModel::isCellEmpty(int row, int col) const
{
    Q_UNUSED(row);
    Q_UNUSED(col);
    return false;
}


