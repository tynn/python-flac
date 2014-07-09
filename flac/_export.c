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


static PyObject *objects[5];


static int
flac__export_init (void)
{
	PyObject *object;

	object = PyInt_FromLong(FLAC_API_VERSION_CURRENT);
	if (object == NULL) return -1;
	objects[0] = object;

	object = PyInt_FromLong(FLAC_API_VERSION_REVISION);
	if (object == NULL) return -1;
	objects[1] = object;

	object = PyInt_FromLong(FLAC_API_VERSION_AGE);
	if (object == NULL) return -1;
	objects[2] = object;

	object = Py_BuildValue("(iii)", FLAC_API_VERSION_CURRENT, FLAC_API_VERSION_REVISION, FLAC_API_VERSION_AGE);
	if (object == NULL) return -1;
	objects[3] = object;

	object = PyBool_FromLong(FLAC_API_SUPPORTS_OGG_FLAC);
	if (object == NULL) return -1;
	objects[4] = object;

	return 0;
}


static void
flac__export_build (PyObject *module)
{
	PyModule_AddObject(module, "API_VERSION_CURRENT", objects[0]);
	PyModule_AddObject(module, "API_VERSION_REVISION", objects[1]);
	PyModule_AddObject(module, "API_VERSION_AGE", objects[2]);
	PyModule_AddObject(module, "API_VERSION", objects[3]);
	PyModule_AddObject(module, "API_SUPPORTS_OGG_FLAC", objects[4]);
}


PyFLAC_MODINIT(
	_export,
	flac__export_init,
	flac__export_build,
	flac__export_functions,
	"libFLAC"
)

