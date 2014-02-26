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

#include <Python.h>

#include "flac.h"


#define MODULE_NAME "flac"
#define MODULE_DOC "libFLAC"


static PyMethodDef module_functions[] = {
	{ NULL }		/* Sentinel */
};


#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef moduledef = {
	PyModuleDef_HEAD_INIT,
	MODULE_NAME,			/* m_name */
	MODULE_DOC,				/* m_doc */
	-1,						/* m_size */
	module_functions,		/* m_methods */
	NULL,					/* m_reload */
	NULL,					/* m_traverse */
	NULL,					/* m_clear */
	NULL,					/* m_free */
};
#define PyFLAC_PyModule_Init PyInit_flac
#define PyFLAC_PyModule_Create(module) module = PyModule_Create(&moduledef);
#define PyFLAC_PyModule_RETURN(module) return module;
#else
#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
#define PyFLAC_PyModule_Init initflac
#define PyFLAC_PyModule_Create(module) module = Py_InitModule3(MODULE_NAME, module_functions, MODULE_DOC);
#define PyFLAC_PyModule_RETURN(module)
#endif


PyMODINIT_FUNC
PyFLAC_PyModule_Init ( void )
{
	PyObject *module;

	PyFLAC_PyModule_Create(module)
	PyFLAC_PyModule_AddObjects(module)
	PyFLAC_PyModule_RETURN(module)
}

