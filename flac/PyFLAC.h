/*
 *	This file is part of python-flac.
 *
 *	Copyright (c) 2014 Christian Schmitz <tynn.dev@gmail.com>
 *
 *	python-flac is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	python-flac is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public License
 *	along with python-flac.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __PyFLAC_h__
#define __PyFLAC_h__

#include <Python.h>


#if PY_MAJOR_VERSION >= 3
	#define __PyFLAC3__

	#define Py_TPFLAGS_HAVE_ITER 0

	#if PY_MAJOR_VERSION >= 4

		#define PyString_GET_SIZE PyUnicode_GET_LENGTH

	#else // PY_MAJOR_VERSION >= 4

		#define PyString_GET_SIZE PyUnicode_GET_SIZE

	#endif // PY_MAJOR_VERSION >= 4

	#define PyInt_FromLong PyLong_FromLong

	#define PyStringObject PyUnicodeObject
	#define PyString_Type PyUnicode_Type
	#define PyString_FromFormat PyUnicode_FromFormat
	#define PyString_FromString PyUnicode_FromString
	#define PyString_FromStringAndSize PyUnicode_FromStringAndSize

#else // PY_MAJOR_VERSION >= 3

	#define PyLong_FromUnsignedLong(unsigned_value) \
		((long) unsigned_value < 0 ? PyLong_FromUnsignedLong((unsigned long) unsigned_value) : PyFLAC_int(unsigned_value))

	#define PyLong_FromUnsignedLongLong(unsigned_value) \
		((unsigned_value > (unsigned PY_LONG_LONG) (unsigned long) -1) ? PyLong_FromUnsignedLongLong(unsigned_value) : PyLong_FromUnsignedLong((unsigned long) unsigned_value))

#endif // PY_MAJOR_VERSION >= 3


#define PyFLAC_name(type) "flac." #type


#define PyFLAC_RuntimeError(msg) PyErr_SetString(PyExc_RuntimeError, msg)

#define PyFLAC_getter_error(type) (PyFLAC_RuntimeError("wrong use of getter of " PyFLAC_name(type)), NULL)

#define PyFLAC_setter_error(type) (PyFLAC_RuntimeError("wrong use of setter of " PyFLAC_name(type)), -1)


#define PyFLAC_bool(bool_value) PyBool_FromLong((long) bool_value)

#define PyFLAC_int(int_value) PyInt_FromLong((long) int_value)

#define PyFLAC_string(string_value) PyString_FromString((char *) string_value)

#define PyFLAC_string2(string_value,string_length) PyString_FromStringAndSize((char *) string_value, (Py_ssize_t) string_length)

#define PyFLAC_data(data,length) PyBytes_FromStringAndSize((char *) data, (Py_ssize_t) length)

#define PyFLAC_off_t(off_value) PyLong_FromLongLong((PY_LONG_LONG) off_value)

#define PyFLAC_uint64(uint64_value) PyLong_FromUnsignedLongLong((unsigned PY_LONG_LONG) uint64_value)

#define PyFLAC_unsigned(unsigned_value) PyLong_FromUnsignedLong((unsigned long) unsigned_value)

#define PyFLAC_uint32(uint32_value) PyFLAC_unsigned(uint32_value)

#define PyFLAC_uint8(uint8_value) PyFLAC_unsigned(uint8_value)

#define PyFLAC_byte(byte_value) PyFLAC_uint8(byte_value)


#define PyFLAC_CHECK(test,error) do { if (test) return error; } while (0)

#define PyFLAC_CHECK_status(status) PyFLAC_CHECK(status < 0,-1)

#define PyFLAC_CHECK_delete(value,member) PyFLAC_CHECK(!(value),(PyErr_Format(PyExc_TypeError, "cannot delete the member %s", member), -1))


#define PyFLAC_type(type) &PyFLAC_##type##Type

#define PyFLAC_type_Check(object,type) PyObject_TypeCheck(object, PyFLAC_type(type))


#ifdef __PyFLAC3__

	#define PyFLAC_PyModule_Prepare(name,functions,doc) \
	static struct PyModuleDef moduledef = { \
		PyModuleDef_HEAD_INIT, \
		PyFLAC_name(name),		/* m_name */ \
		doc,					/* m_doc */ \
		-1,						/* m_size */ \
		functions,				/* m_methods */ \
		NULL,					/* m_reload */ \
		NULL,					/* m_traverse */ \
		NULL,					/* m_clear */ \
		NULL,					/* m_free */ \
	};
	#define PyFLAC_PyModule_Init(name) PyInit_##name
	#define PyFLAC_PyModule_Create(name,functions,doc) PyModule_Create(&moduledef)
	#define PyFLAC_PyModule_RETURN return

#else // __PyFLAC3__

	#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
		#define PyMODINIT_FUNC void
	#endif

	#define PyFLAC_PyModule_Prepare(name,functions,doc)
	#define PyFLAC_PyModule_Init(name) init##name
	#define PyFLAC_PyModule_Create(name,functions,doc) Py_InitModule3(PyFLAC_name(name), functions, doc)
	#define PyFLAC_PyModule_RETURN (void)

#endif // __PyFLAC3__


#define PyFLAC_MODINIT(name,initfunction,buildfunction,functions,doc) \
PyFLAC_PyModule_Prepare(name,functions,doc) \
PyMODINIT_FUNC \
PyFLAC_PyModule_Init(name) ( void ) \
{ \
	PyObject *module; \
	if (initfunction() < 0) { \
		if (!PyErr_Occurred()) \
			PyErr_SetString(PyExc_SystemError, "error return without exception set"); \
		PyFLAC_PyModule_RETURN NULL; \
	} \
	else \
	{ \
		module = PyFLAC_PyModule_Create(name,functions,doc); \
		if (module != NULL) \
			buildfunction(module); \
		PyFLAC_PyModule_RETURN module; \
	} \
}


#define PyFLAC_PyModule_AddType(module,type) \
	PyModule_AddObject(module, #type, (PyObject *) PyFLAC_type(type))


#endif // __PyFLAC_h__

