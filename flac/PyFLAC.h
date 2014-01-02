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

#ifndef __PyFLAC_h__
#define __PyFLAC_h__


#define PyFLAC_RETURN_bool(bool_value) if (bool_value) Py_RETURN_TRUE; else Py_RETURN_FALSE;

#define PyFLAC_RETURN_int(int_value) return Py_BuildValue("i", int_value);

#define PyFLAC_RETURN_string(string_value) return Py_BuildValue("s", string_value);

#define PyFLAC_RETURN_uint64(uint64_value) return Py_BuildValue("K", uint64_value);

#define PyFLAC_RETURN_unsigned(unsigned_value) return Py_BuildValue("I", unsigned_value);

#define PyFLAC_CHECK_status if (status < 0) return status;


#define PyFLAC_Type_Ready(type) \
	status = PyType_Ready(&flac_##type##Type); \
	PyFLAC_CHECK_status


#define PyFLAC_Add_Object(object) \
	Py_INCREF(&flac_##object##Type); \
	status = PyModule_AddObject(module, #object, (PyObject *) &flac_##object##Type); \
	PyFLAC_CHECK_status


int PyFLAC_Format_MetadataType_Check (PyObject *object);

PyObject * PyFLAC_Format_ChannelAssignment_New (FLAC__ChannelAssignment e_value);


#endif // __PyFLAC_h__

