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

#define PyFLAC_Enum(type) \
typedef struct flac_EnumObject flac_##type##Object; \
\
static PyTypeObject flac_##type##Type = { \
	PyObject_HEAD_INIT(NULL) \
	0, "flac." #type, \
	sizeof(flac_##type##Object), \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	Py_TPFLAGS_DEFAULT , "FLAC enum " #type, \
};

#define PyFLAC_Enum_Member(member_name,flac_enum) { member_name, FLAC__##flac_enum, NULL }

#define PyFLAC_Enum_FromEnum(e_value,type) PyFLAC_##type##_FromEnum (e_value)

#define PyFLAC_Enum_FromEnum_use(type) PyObject * PyFLAC_Enum_FromEnum(int e_value,type)

#define PyFLAC_Enum_FromEnum_function(type,data) \
PyFLAC_Enum_FromEnum_use(type) \
{ \
	return PyFLAC_Enum_FromInt(e_value, data, "invalid value for enum " #type); \
}

struct flac_EnumObject {
	PyObject_HEAD
	int e_value;
	char *e_name;
};

typedef struct {
	char *e_name;
	int e_value;
	PyObject *e_object;
} flac_Enum_Member;

int PyFLAC_Enum_Ready (PyTypeObject *type, flac_Enum_Member *data);

PyObject * PyFLAC_Enum_FromInt (int e_value, flac_Enum_Member *data, const char *err_msg);

int PyFLAC_Enum_AsInt (PyObject *object);

#endif // __enum_h__

