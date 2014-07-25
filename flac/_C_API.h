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

#ifndef __PyFLAC__C_API_h__
#define __PyFLAC__C_API_h__


#ifndef PyFLAC_name
#define PyFLAC_name(type) "flac." #type
#endif // PyFLAC_name


#define PyFLAC__C_API_DEF(count) \
static PyObject *_c_api; \
static void *_C_API[count];


#define PyFLAC__C_API_PUT(i,name) _C_API[i] = ((void *) name);


#define PyFLAC__C_API_INIT(module) _c_api = PyCapsule_New((void *) _C_API, PyFLAC_name(module) "._C_API", NULL);
#define PyFLAC__C_API_CHECK if (_c_api == NULL) return -1;


#define _c_api_build(module) PyModule_AddObject(module, "_C_API", _c_api)


#define PyFLAC_API(module) PyFLAC_##module##_API

#define PyFLAC__C_API(module) \
static void **PyFLAC_API(module); \
static int \
PyFLAC_import_##module (void) \
{ \
	if (PyFLAC_API(module) != NULL) return 1; \
	if (PyImport_ImportModule(PyFLAC_name(module)) == NULL) return -1; \
	PyFLAC_API(module) = (void **) PyCapsule_Import(PyFLAC_name(module) "._C_API", 0); \
	return PyFLAC_API(module) == NULL ? -1 : 0; \
}


#define PyFLAC_type_PUT(type,i) \
	PyFLAC__C_API_PUT(i,PyFLAC_type(type))

#define PyFLAC_type_API(module,i) \
	(*(PyTypeObject *) PyFLAC_API(module)[i])


#define PyFLAC_Enum_FromEnum_PUT(type,i) \
	PyFLAC__C_API_PUT(i,PyFLAC_Enum_FromEnum_NAME(type))

#define PyFLAC_Enum_FromEnum_API(module,i) \
	(*(PyObject * (*)(int)) PyFLAC_API(module)[i])


#endif // __PyFLAC__C_API_h__

