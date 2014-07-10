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

#ifndef __PyFLAC__enum_h__
#define __PyFLAC__enum_h__

#include "enum.h"

#define PyFLAC_Enum(type) \
typedef struct flac_EnumObject flac_##type##Object; \
\
static PyTypeObject PyFLAC_##type##Type = { \
	PyVarObject_HEAD_INIT(NULL,0) \
	"flac." #type, \
	sizeof(flac_##type##Object), \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	Py_TPFLAGS_DEFAULT , "FLAC enum " #type, \
};

#define PyFLAC_Enum_Member(member_name,flac_enum) { member_name, FLAC__##flac_enum, NULL }

#define PyFLAC_Enum_FromEnum_function(type,data) \
PyFLAC_Enum_FromEnum_DEF(type) \
{ \
	return PyFLAC_Enum_FromInt(e_value, data, "invalid value for enum " #type); \
}


typedef struct {
	char *e_name;
	int e_value;
	PyObject *e_object;
} flac_Enum_Member;


static void
flac_Enum_dealloc (PyObject *self)
{
	Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject *
flac_Enum_repr (PyObject *self)
{
	return PyString_FromFormat("<enum %s of type %s>", ((struct flac_EnumObject *) self)->e_name, Py_TYPE(self)->tp_name);
}

static PyObject *
flac_Enum_str (PyObject *self)
{
	return PyString_FromString(((struct flac_EnumObject *) self)->e_name);
}

static int
flac_Enum_add_member (PyTypeObject *type, flac_Enum_Member *member)
{
	member->e_object = PyType_GenericNew(type, NULL, NULL);
	if (!member->e_object)
		return -1;

	((struct flac_EnumObject *) member->e_object)->e_value = member->e_value;
	((struct flac_EnumObject *) member->e_object)->e_name = member->e_name;

	PyFLAC_CHECK_status(PyObject_GenericSetAttr((PyObject *) type, PyString_FromString(member->e_name), member->e_object))

	return 0;
}

static int
flac_Enum_sort_member (flac_Enum_Member *member, int member_count)
{
	int i, j;
	flac_Enum_Member tmp;

	for (i = 0; i < member_count; i++)
		while (member[i].e_value != i)
		{
			j = member[i].e_value;
			if (member[j].e_value == j)
				return -1;

			tmp = member[j];
			member[j] = member[i];
			member[i] = tmp;
		}

	return 0;
}

static int
PyFLAC_Enum_Ready (PyTypeObject *type, flac_Enum_Member *member)
{
	int i;

	type->tp_dealloc = (destructor) flac_Enum_dealloc;
	type->tp_repr = (reprfunc) flac_Enum_repr;
	type->tp_str = (reprfunc) flac_Enum_str;
	type->tp_dict = NULL;

	PyFLAC_CHECK_status(PyType_Ready(type))

	for (i = 0; member[i].e_name; i++)
		PyFLAC_CHECK_status(flac_Enum_add_member(type, &member[i]))

	PyFLAC_CHECK_status(flac_Enum_sort_member(member, i))

	return 0;
}

static PyObject *
PyFLAC_Enum_FromInt (int e_value, flac_Enum_Member *data, const char *err_msg)
{
	int i;

	for (i = 0; i < e_value; i++)
		if(!data[i].e_name)
		{
			if(!err_msg)
				err_msg = "invalid enum value";

			PyErr_SetString(PyExc_ValueError, err_msg);
			return NULL;
		}

	return data[e_value].e_object;
}

#endif // __PyFLAC__enum_h__

