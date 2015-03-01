/*
 *	This file is part of python-flac.
 *
 *	Copyright (c) 2015 Christian Schmitz <tynn.dev@gmail.com>
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

#ifndef __PyFLAC__list_of_type_h__
#define __PyFLAC__list_of_type_h__

static void
flac_free_list_of_type (PyObject **list, Py_ssize_t count)
{
	if (list)
	{
		while (count--)
			Py_DECREF(list[count]);
		PyMem_Del(list);
	}
}


static int
flac_list_of_type (PyObject *object, PyTypeObject *type, PyObject ***ret_list, Py_ssize_t *ret_count)
{
	PyObject *item, **list;
	Py_ssize_t count, i;

	if (PyObject_TypeCheck(object, type))
	{
		*ret_count = 1;
		*ret_list = NULL;
		return 1;
	}

	list = NULL;

	if (PyTuple_Check(object) || PyList_Check(object))
	{
		count = PySequence_Size(object);
		if (count == -1)
			return 0;

		if (!count)
		{
			*ret_count = 0;
			*ret_list = NULL;
			return 1;
		}

		list = PyMem_New(PyObject *, count);
		if (!list)
		{
			PyErr_NoMemory();
			return 0;
		}

		for (i = 0; i < count; i++)
		{
			item = PySequence_GetItem(object, i);
			if (!item)
			{
				flac_free_list_of_type(list, i);
				return 0;
			}

			if (!PyObject_TypeCheck(item, type))
				break;

			Py_INCREF(item);
			list[i] = item;
		}

		if (i == count)
		{
			*ret_count = count;
			*ret_list = list;
			return 1;
		}

		flac_free_list_of_type(list, i);
	}

	PyErr_Format(PyExc_TypeError, "must be a list of %.50s", type->tp_name);
	return 0;
}

#endif // __PyFLAC__list_of_type_h__

