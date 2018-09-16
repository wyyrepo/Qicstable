// This is the SIP file containing all utility functions that can be used by
// handwritten code.
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


%ModuleHeaderCode
#include <QicsDataItem.h>


DataItemComparator PyQicsTable_getComparator(PyObject *);
void PyQicsTable_releaseComparator(PyObject *);
PyObject *PyQicsTable_wrapComparator(DataItemComparator);
%End


%ModuleCode

#include <QStack>


// The stack of Python callables that implement data item comparators.  It is
// unlikely that the stack would ever be more than one deep (it would imply a
// nested sort), but you never know.
static QStack<PyObject> pyCompStack;


// Call a Python callable to compare two data items.
static int cppComparator(const QicsDataItem *itm1, const QicsDataItem *itm2)
{
	int res, iserr = 1;
	PyObject *args;
	PyObject pyfunc;

	pyfunc = pyCompStack.top();

	SIP_BLOCK_THREADS

	// Call the Python comparator.
	args = sipBuildResult(NULL, "(MM)", itm1, sipClass_QicsDataItem, itm2, sipClass_QicsDataItem);

	if (args != NULL)
	{
		PyObject *robj = PyObject_CallObject(&pyfunc, args);

		Py_DECREF(args);

		// Handle the result.
		if (robj != NULL)
		{
			res = PyInt_AsLong(robj);

			if (PyErr_Occurred())
				PyErr_SetString(PyExc_TypeError, "non-integer value returned from QicsDataItem comparator");
			else
				iserr = 0;

			Py_DECREF(robj);
		}
	}

	// Handle any errors.
	if (iserr)
	{
		PyErr_Print();
		res = 0;
	}

	SIP_UNBLOCK_THREADS

	return res;
}


// Return a C++ comparator function that is a wrapper around the given optional
// Python callable.
DataItemComparator PyQicsTable_getComparator(PyObject *comp)
{
	if (comp && (comp != Py_None))
	{
		pyCompStack.push(*comp);
		return cppComparator;
	}

	return 0;
}


// Release a C++ comparator function that was a wrapper around the given
// optional Python callable.
void PyQicsTable_releaseComparator(PyObject *comp)
{
	if (comp && comp != Py_None)
		pyCompStack.pop();
}


// This function invokes a C++ comparator.  We abuse the self argument and use
// it to pass the address of the comparator.
static PyObject *invokeComparator(PyObject *self, PyObject *args)
{
	// Get the address of the C++ comparator.
	DataItemComparator comp = (DataItemComparator)PyCObject_AsVoidPtr(self);

	if (!comp)
		return NULL;

	// Get the two arguments.
	PyObject *itm1obj, *itm2obj;

	if (!PyArg_ParseTuple(args, "OO", &itm1obj, &itm2obj))
		return NULL;

	int iserr = 0;

#if SIP_VERSION < 0x040800
	const QicsDataItem *itm1 = static_cast<const QicsDataItem *>(sipConvertToCpp(itm1obj, sipClass_QicsDataItem, &iserr));
	const QicsDataItem *itm2 = static_cast<const QicsDataItem *>(sipConvertToCpp(itm2obj, sipClass_QicsDataItem, &iserr));
#else
	const QicsDataItem *itm1 = static_cast<const QicsDataItem *>(sipConvertToInstance(itm1obj, sipClass_QicsDataItem, NULL, SIP_NO_CONVERTORS, NULL, &iserr));
	const QicsDataItem *itm2 = static_cast<const QicsDataItem *>(sipConvertToInstance(itm2obj, sipClass_QicsDataItem, NULL, SIP_NO_CONVERTORS, NULL, &iserr));
#endif

	if (iserr)
		return NULL;

	// Call the C++ comparator.
	int res;

	Py_BEGIN_ALLOW_THREADS
	res = comp(itm1, itm2);
	Py_END_ALLOW_THREADS

	// Return the result as a Python object.
	return PyInt_FromLong(res);
}


// Wrap a C++ comparator function on the fly and return the corresponding
// Python callable.
PyObject *PyQicsTable_wrapComparator(DataItemComparator comp)
{
	static PyMethodDef md = {
		"invokeComparator", invokeComparator, METH_VARARGS, NULL
	};

	// Handle the trivial case.
	if (!comp)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

	// Abuse self to pass the address of the C++ comparator.  Note that
	// this may not be portable according to ISO.  If it's a problem then
	// create a copy of the address on the heap and pass a pointer to that
	// instead.
	PyObject *cppcomp = PyCObject_FromVoidPtr((void *)comp, NULL);

	if (cppcomp == NULL)
		return NULL;

	// Create the Python callable.
	PyObject *pycomp = PyCFunction_New(&md, cppcomp);

	Py_DECREF(cppcomp);

	return pycomp;
}

%End

