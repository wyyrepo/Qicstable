// This is the SIP interface definition for all mapped types.
//
// Copyright (c) 2004 - 2014
// 	Integrated Computer Solutions, Inc. <info@ics.com>
// 
// This file is part of PyQicsTable.
// 
// This copy of PyQicsTable is licensed for use under the terms of the
// PyQicsTable Commercial License Agreement version ICS-2.6.  See the file
// LICENSE for more details.
// 
// PyQicsTable is supplied WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 
// QVector in Qt4, QValueVector in Qt3
%MappedType QVector<const QicsDataItem *>
{
%TypeHeaderCode
// Check for duplicate includes of QVector
#ifndef QVECTOR_H
#define QVECTOR_H

#include <QVector>

#endif
%End

%ConvertFromTypeCode
	// Handle no list.

	if (!sipCpp)
		return PyList_New(0);

	// Convert to a Python list of QicsDataItem instances.

	int i;
	PyObject *l;

	// Create the list.
	if ((l = PyList_New(sipCpp -> count())) == NULL)
		return NULL;

	// Get it.
	i = 0;
	QVector<const QicsDataItem *>::const_iterator it;

	for (it = sipCpp -> begin(); it != sipCpp -> end(); ++it)
	{
		PyObject *obj;

		if ((obj = sipBuildResult(NULL, "M", *it, sipClass_QicsDataItem)) == NULL || PyList_SetItem(l, i, obj) < 0)
		{
			Py_XDECREF(obj);
			Py_DECREF(l);

			return NULL;
		}

		++i;
	}

	return l;
%End

%ConvertToTypeCode
	// Convert a Python list of QicsDataItem instances to a
	// QVector<const QicsDataItem *> on the heap.
 
	if (sipIsErr == NULL)
		return PyList_Check(sipPy);

	if (sipPy == Py_None)
	{
		*sipCppPtr = NULL;
		return 0;
	}

	QVector<const QicsDataItem *> *qvv = new QVector<const QicsDataItem *>;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		int iserr = 0;

#if SIP_VERSION < 0x040800
		const QicsDataItem *itm = static_cast<QicsDataItem *>(sipForceConvertTo_QicsDataItem(PyList_GET_ITEM(sipPy, i), &iserr));
#else
		const QicsDataItem *itm = static_cast<QicsDataItem *>(sipForceConvertToInstance(PyList_GET_ITEM(sipPy, i), sipClass_QicsDataItem, NULL, SIP_NO_CONVERTORS, NULL, &iserr));
#endif

		if (!itm || iserr)
		{
			*sipIsErr = 1;
			delete qvv;

			return 0;
		}

		qvv -> append(itm);
	}
 
	*sipCppPtr = qvv;
 
	return 1;
%End
};


%MappedType QVector<QicsScreenGrid *>
{
%TypeHeaderCode
// Check for duplicate includes of QVector
#ifndef QVECTOR_H
#define QVECTOR_H

#include <QVector>

#endif
%End

%ConvertFromTypeCode
	// Handle no list.

	if (!sipCpp)
		return PyList_New(0);

	// Convert to a Python list of QicsScreenGrid instances.

	int i;
	PyObject *l;

	// Create the list.
	if ((l = PyList_New(sipCpp -> count())) == NULL)
		return NULL;

	// Get it.
	i = 0;
	QVector<QicsScreenGrid *>::const_iterator it;

	for (it = sipCpp -> begin(); it != sipCpp -> end(); ++it)
	{
		PyObject *obj;

		if ((obj = sipBuildResult(NULL, "M", *it, sipClass_QicsScreenGrid)) == NULL || PyList_SetItem(l, i, obj) < 0)
		{
			Py_XDECREF(obj);
			Py_DECREF(l);

			return NULL;
		}

		++i;
	}

	return l;
%End

%ConvertToTypeCode
	// Convert a Python list of QicsScreenGrid instances to a
	// QVector<QicsScreenGrid *> on the heap.
 
	if (sipIsErr == NULL)
		return PyList_Check(sipPy);

	if (sipPy == Py_None)
	{
		*sipCppPtr = NULL;
		return 0;
	}

	QVector<QicsScreenGrid *> *qvv = new QVector<QicsScreenGrid *>;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		int iserr = 0;
		
#if SIP_VERSION < 0x040800
		QicsScreenGrid *itm = static_cast<QicsScreenGrid *>(sipForceConvertTo_QicsScreenGrid(PyList_GET_ITEM(sipPy, i), &iserr));
#else
		QicsScreenGrid *itm = static_cast<QicsScreenGrid *>(sipForceConvertToInstance(PyList_GET_ITEM(sipPy, i), sipClass_QicsScreenGrid, NULL, SIP_NO_CONVERTORS, NULL, &iserr));
#endif

		if (!itm || iserr)
		{
			*sipIsErr = 1;
			delete qvv;

			return 0;
		}

		qvv -> append(itm);
	}
 
	*sipCppPtr = qvv;
 
	return 1;
%End
};


%MappedType QVector<QicsSpan>
{
%TypeHeaderCode
// Check for duplicate includes of QVector
#ifndef QVECTOR_H
#define QVECTOR_H

#include <QVector>

#endif
%End

%ConvertFromTypeCode
	// Handle no list.

	if (!sipCpp)
		return PyList_New(0);

	// Convert to a Python list of QicsScreenGrid instances.

	int i;
	PyObject *l;

	// Create the list.
	if ((l = PyList_New(sipCpp -> count())) == NULL)
		return NULL;

	// Get it.
	i = 0;
	QVector<QicsSpan>::const_iterator it;

	for (it = sipCpp -> begin(); it != sipCpp -> end(); ++it)
	{
		PyObject *obj;

		if ((obj = sipBuildResult(NULL, "P", new QicsSpan(*it), sipClass_QicsSpan)) == NULL || PyList_SetItem(l, i, obj) < 0)
		{
			Py_XDECREF(obj);
			Py_DECREF(l);

			return NULL;
		}

		++i;
	}

	return l;
%End

%ConvertToTypeCode
	// Convert a Python list of QicsSpan instances to a
	// QVector<QicsSpan> on the heap.
 
	if (sipIsErr == NULL)
		return PyList_Check(sipPy);

	if (sipPy == Py_None)
	{
		*sipCppPtr = NULL;
		return 0;
	}

	QVector<QicsSpan> *qvv = new QVector<QicsSpan>;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		int iserr = 0;

#if SIP_VERSION < 0x040800
		QicsSpan *itm = static_cast<QicsSpan *>(sipForceConvertTo_QicsSpan(PyList_GET_ITEM(sipPy, i), &iserr));
#else
		QicsSpan *itm = static_cast<QicsSpan *>(sipForceConvertToInstance(PyList_GET_ITEM(sipPy, i), sipClass_QicsSpan, NULL, SIP_NO_CONVERTORS, NULL, &iserr));
#endif

		if (!itm || iserr)
		{
			*sipIsErr = 1;
			delete qvv;

			return 0;
		}

		qvv -> append(*itm);
	}
 
	*sipCppPtr = qvv;
 
	return 1;
%End
};
// QVector<int> is implemented as a Python list of integers.
%MappedType QVector<int>
{
%TypeHeaderCode
#ifndef Q_VECTOR
#define Q_VECTOR
#include <QVector>
#endif
%End

%ConvertFromTypeCode
    // Create the list.
    PyObject *l;

    if ((l = PyList_New(sipCpp->size())) == NULL)
        return NULL;

    // Set the list elements.
    for (int i = 0; i < sipCpp->size(); ++i)
    {
        PyObject *pobj;

        if ((pobj = PyInt_FromLong(sipCpp->value(i))) == NULL)
        {
            Py_DECREF(l);

            return NULL;
        }

        PyList_SET_ITEM(l, i, pobj);
    }

    return l;
%End

%ConvertToTypeCode
    // Check the type if that is all that is required.
    if (sipIsErr == NULL)
        return PyList_Check(sipPy);

    QVector<int> *qv = new QVector<int>;
 
    for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
        qv->append(PyInt_AsLong(PyList_GET_ITEM(sipPy, i)));
 
    *sipCppPtr = qv;
 
    return sipGetState(sipTransferObj);
%End
};

