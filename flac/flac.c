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


#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif


static PyMethodDef flac_methods[] = {
	{ NULL }		/* Sentinel */
};


PyMODINIT_FUNC
initflac ( void )
{
	PyObject *module;

	module = Py_InitModule3("flac", flac_methods, "libFLAC");

	PyFLAC_PyModule_AddFormatObjects(module);
	PyFLAC_PyModule_AddMetadataObjects(module);
}

