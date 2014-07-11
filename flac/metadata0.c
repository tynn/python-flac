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
#include "enum.h"

#include "format.h"
#include "metadata.h"


static PyObject *
flac_metadata0_get_streaminfo (PyObject *null, PyObject *args)
{
	FLAC__StreamMetadata *metadata;
	PyObject *py_metadata;
	char *filename;

	(void)null;

	if (!PyArg_ParseTuple(args, "s", &filename))
		return NULL;

	metadata = FLAC__metadata_object_new(FLAC__METADATA_TYPE_STREAMINFO);
	if (!metadata)
		return PyErr_NoMemory();

	if (FLAC__metadata_get_streaminfo(filename, metadata))
		py_metadata = PyFLAC_StreamMetadata_FromClass(metadata);
	else
		Py_INCREF(py_metadata = Py_None);

	FLAC__metadata_object_delete(metadata);
	return py_metadata;
}


static PyObject *
flac_metadata0_get_tags (PyObject *null, PyObject *args)
{
	FLAC__StreamMetadata *metadata;
	PyObject *py_metadata;
	char *filename;

	(void)null;

	if (!PyArg_ParseTuple(args, "s", &filename))
		return NULL;

	if (FLAC__metadata_get_tags(filename, &metadata))
	{
		py_metadata = PyFLAC_StreamMetadata_FromClass(metadata);
		FLAC__metadata_object_delete(metadata);
		return py_metadata;
	}

	Py_RETURN_NONE;
}


static PyObject *
flac_metadata0_get_cuesheet (PyObject *null, PyObject *args)
{
	FLAC__StreamMetadata *metadata;
	PyObject *py_metadata;
	char *filename;

	(void)null;

	if (!PyArg_ParseTuple(args, "s", &filename))
		return NULL;

	if (FLAC__metadata_get_cuesheet(filename, &metadata))
	{
		py_metadata = PyFLAC_StreamMetadata_FromClass(metadata);
		FLAC__metadata_object_delete(metadata);
		return py_metadata;
	}

	Py_RETURN_NONE;
}


static PyObject *
flac_metadata0_get_picture (PyObject *null, PyObject *args)
{
	FLAC__StreamMetadata_Picture_Type picture_type;
	FLAC__StreamMetadata *metadata;
	PyObject *py_picture_type;
	PyObject *py_metadata;
	char *filename;

	(void)null;

	if (!PyArg_ParseTuple(args, "s|O!ssIIII", &filename, PyFLAC_type(StreamMetadataPictureType), &py_picture_type))
		return NULL;

	picture_type = py_picture_type ? PyFLAC_Enum_AsInt(py_picture_type) : -1;

	if (FLAC__metadata_get_picture(filename, &metadata, picture_type, NULL, NULL, -1, -1, -1, -1))
	{
		py_metadata = PyFLAC_StreamMetadata_FromClass(metadata);
		FLAC__metadata_object_delete(metadata);
		return py_metadata;
	}

	Py_RETURN_NONE;
}


static PyMethodDef flac_metadata0_functions[] = {
	{
		"get_streaminfo",
		(PyCFunction) flac_metadata0_get_streaminfo,
		METH_VARARGS,
		"Metadata level 0 interface: read streaminfo of FLAC file"
	}, {
		"get_tags",
		(PyCFunction) flac_metadata0_get_tags,
		METH_VARARGS,
		"Metadata level 0 interface: read tags of FLAC file"
	}, {
		"get_cuesheet",
		(PyCFunction) flac_metadata0_get_cuesheet,
		METH_VARARGS,
		"Metadata level 0 interface: read cuesheet of FLAC file"
	}, {
		"get_picture",
		(PyCFunction) flac_metadata0_get_picture,
		METH_VARARGS,
		"Metadata level 0 interface: read picture of FLAC file"
	}, { NULL }		/* Sentinel */
};


static int
flac_metadata0_init (void)
{
	PyFLAC_CHECK_status(PyFLAC_import_format())
	PyFLAC_CHECK_status(PyFLAC_import_metadata())
	return 0;
}


static void
flac_metadata0_build (PyObject *module)
{}


PyFLAC_MODINIT(
	metadata0,
	flac_metadata0_init,
	flac_metadata0_build,
	flac_metadata0_functions,
	"libFLAC metadata level 0 interface"
)

