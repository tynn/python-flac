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

#ifndef __enum_h__
#define __enum_h__

#define PyFLAC_Enum_FromEnum(e_value,type) PyFLAC_##type##_FromEnum (e_value)

#define PyFLAC_Enum_FromEnum_use(type) PyObject * PyFLAC_Enum_FromEnum(int e_value,type)

#define PyFLAC_Enum_AsInt(object) (((struct flac_EnumObject *) object)->e_value)

struct flac_EnumObject {
	PyObject_HEAD
	int e_value;
	char *e_name;
};

#endif // __enum_h__

