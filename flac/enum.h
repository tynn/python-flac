/*
 *	This file is part of python-flac.
 *
 *	Copyright (c) 2013 Christian Schmitz <tynn.dev@gmail.com>
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

#ifndef __enum_h__
#define __enum_h__


#define PyFLAC_ENUM_MEMBER(var,type,value) \
	var = PyType_GenericNew(&flac_##type##Type, NULL, NULL); \
	if (var) \
		PyObject_INIT_VAR(var, &flac_##type##Type, value);


#define PyFLAC_ENUM_TYPE(type) \
typedef struct flac_EnumObject flac_##type##Object; \
\
static PyTypeObject flac_##type##Type = { \
	PyObject_HEAD_INIT(NULL) \
	0, "flac." #type, \
	sizeof(flac_##type##Object), \
	0, (destructor) flac_Enum_dealloc, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	Py_TPFLAGS_DEFAULT , "FLAC enum " #type, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, \
	&PyInt_Type, \
};


#define PyFLAC_ADD_ENUM_MEMBER(type,member,enum_var) \
	PyFLAC_ENUM_MEMBER(enum_member,type,FLAC__##enum_var) \
	if (!enum_member) \
		return -1; \
	PyDict_SetItem(flac_##type##Type.tp_dict, PyString_FromString(member), enum_member);


#define PyFLAC_ENUM_LOCK(type) flac_##type##Type.tp_new = 0;

#define PyFLAC_E_VALUE(var) PyInt_AsLong(var)


struct flac_EnumObject { PyIntObject e_value; };

static void
flac_Enum_dealloc(PyObject *self)
{
    self->ob_type->tp_free((PyObject *)self);
}


#endif // __enum_h__

