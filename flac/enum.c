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

#include <Python.h>

#include "PyFLAC.h"
#include "enum.h"


static void
flac_Enum_dealloc (PyObject *self)
{
    self->ob_type->tp_free((PyObject *)self);
}


static PyObject *
flac_Enum_repr (PyObject *self)
{
	if (!self)
		return NULL;

	return PyString_FromFormat("<enum %s of type %s>", ((struct flac_EnumObject *) self)->e_name, self->ob_type->tp_name);
}


static int
flac_Enum_add_member (PyTypeObject *type, flac_Enum_Member *member)
{
	member->e_object = PyType_GenericNew(type, NULL, NULL);
	if (!member->e_object)
		return -1;

	((struct flac_EnumObject *) member->e_object)->e_value = member->e_value;
	((struct flac_EnumObject *) member->e_object)->e_name = member->e_name;

	PyFLAC_CHECK_status(PyDict_SetItem(type->tp_dict, PyString_FromString(member->e_name), member->e_object))

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


int
PyFLAC_Enum_Ready (PyTypeObject *type, flac_Enum_Member *member)
{
	int i;

	type->tp_dealloc = (destructor) flac_Enum_dealloc;
	type->tp_repr = (reprfunc) flac_Enum_repr;

	PyFLAC_CHECK_status(PyType_Ready(type))

	for (i = 0; member[i].e_name; i++)
		PyFLAC_CHECK_status(flac_Enum_add_member(type, &member[i]))

	PyFLAC_CHECK_status(flac_Enum_sort_member(member, i))

	return 0;
}

