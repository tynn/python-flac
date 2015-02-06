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

#include "metadata.h"


static PyMethodDef flac_metadata2_functions[] = {
	{ NULL }		/* Sentinel */
};


static int
flac_metadata2_init ( void )
{
	PyFLAC_CHECK_status(PyFLAC_import_metadata());

	return 0;
}


static void
flac_metadata2_build (PyObject *module)
{}


PyFLAC_MODINIT(
	metadata2,
	flac_metadata2_init,
	flac_metadata2_build,
	flac_metadata2_functions,
	"libFLAC metadata level 2 interface"
)

