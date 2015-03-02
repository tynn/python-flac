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

#ifndef ___application_id_h__
#define ___application_id_h__

static int
_application_id (PyObject *obj, FLAC__byte (*id)[4])
{
	Py_ssize_t length;
	char *bytes;
	int state;

	Py_INCREF(obj);

	if (PyUnicode_Check(obj))
	{
		Py_DECREF(obj);
		if (!(obj = PyUnicode_AsASCIIString(obj)))
			return 0;
	}

	state = 0;

	if (!PyBytes_Check(obj))
		state = 1;
	else if (PyBytes_AsStringAndSize(obj, &bytes, &length) < 0)
		state = 2;
	else if (length != 4)
		state = 1;

	Py_DECREF(obj);

	switch (state)
	{
		case 1:
			PyErr_Format(PyExc_TypeError, "must be %.50s or unicode of length 4", PyBytes_Type.tp_name);
		case 2:
			return 0;
		default:
			(*id)[0] = bytes[0];
			(*id)[1] = bytes[1];
			(*id)[2] = bytes[2];
			(*id)[3] = bytes[3];
			return 1;
	}
}

#endif // ___application_id_h__

