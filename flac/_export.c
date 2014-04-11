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

#include "PyFLAC.h"
#include <FLAC/export.h>


static PyMethodDef flac__export_functions[] = {
	{ NULL }		/* Sentinel */
};


static int
flac__export_init (PyObject *module)
{
	PyObject *object;

	object = PyInt_FromLong(FLAC_API_VERSION_CURRENT);
	PyFLAC_CHECK_status(PyModule_AddObject(module, "API_VERSION_CURRENT", object))

	object = PyInt_FromLong(FLAC_API_VERSION_REVISION);
	PyFLAC_CHECK_status(PyModule_AddObject(module, "API_VERSION_REVISION", object))

	object = PyInt_FromLong(FLAC_API_VERSION_AGE);
	PyFLAC_CHECK_status(PyModule_AddObject(module, "API_VERSION_AGE", object))

	object = Py_BuildValue("(iii)", FLAC_API_VERSION_CURRENT, FLAC_API_VERSION_REVISION, FLAC_API_VERSION_AGE);
	PyFLAC_CHECK_status(PyModule_AddObject(module, "API_VERSION", object))

	object = PyBool_FromLong(FLAC_API_SUPPORTS_OGG_FLAC);
	PyFLAC_CHECK_status(PyModule_AddObject(module, "API_SUPPORTS_OGG_FLAC", object))

	return 0;
}


PyFLAC_MODINIT(
	_export,
	flac__export_init,
	flac__export_functions,
	"libFLAC"
)

