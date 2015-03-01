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

#ifndef PyFLAC_uintX
#define PyFLAC_uintX(type,size) \
static int \
_##type (PyObject *obj, void *addr) \
{ \
	return _uintX(obj, addr, size); \
}


static int
_uintX (PyObject *obj, void *addr, size_t size)
{
	PY_LONG_LONG l;
	int overflow;

	const unsigned PY_LONG_LONG MAX = ((unsigned PY_LONG_LONG) -1) >> (sizeof(unsigned PY_LONG_LONG) - size) * 8;

	l = PyLong_AsLongLongAndOverflow(obj, &overflow);

	if (l >= 0 && ((unsigned PY_LONG_LONG) l) <= MAX)
	{
		Py_MEMCPY(addr, &l, size);
		return 1;
	}

	if (l == -1)
	{
		if (!overflow && PyErr_Occurred())
			return 0;

		if (overflow > 0 && size == sizeof(unsigned PY_LONG_LONG))
		{
			l = (PY_LONG_LONG) PyLong_AsUnsignedLongLong(obj);
			obj = (l == -1) ? PyErr_Occurred() : NULL;

			if (obj)
			{
				if (obj != PyExc_OverflowError)
					return 0;

				PyErr_Clear();
			}
			else
			{
				Py_MEMCPY(addr, &l, size);
				return 1;
			}
		}
	}

	PyErr_Format(PyExc_ValueError, "value must be >= 0 and <= %llu", MAX);
	return 0;
}

#endif // PyFLAC_uintX

