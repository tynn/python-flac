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

#include "_bool.h"


#define PyFLAC_CHECK_initialized(self) PyFLAC_CHECK(!(self->init),(PyFLAC_RuntimeError("iterator not initialized"), NULL))


typedef struct {
	PyObject_HEAD
	FLAC__Metadata_SimpleIterator *iterator;
	FLAC__bool init, next;
} flac_MetadataSimpleIteratorObject;


static int
flac_MetadataSimpleIterator_PyErr_FromIteratorStatus (flac_MetadataSimpleIteratorObject *self)
{
	FLAC__bool init;

	init = self->init;
	self->init = false;

	switch (FLAC__metadata_simple_iterator_status(self->iterator))
	{
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_OK:
			self->init = init;
			return 1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_ILLEGAL_INPUT:
			self->init = init;
			return 0;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_NOT_A_FLAC_FILE:
			PyErr_SetString(PyFLAC_type(FlacFormatError), "not a FLAC file");
			return -1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_BAD_METADATA:
			PyErr_SetString(PyFLAC_type(FlacFormatError), "illegal metadata");
			return -1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_ERROR_OPENING_FILE:
			PyErr_SetString(PyExc_IOError, "Could not open the file");
			return -1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_NOT_WRITABLE:
			self->init = init;
			PyErr_SetString(PyExc_IOError, "File not open for writing");
			return -1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_READ_ERROR:
			PyErr_SetString(PyExc_IOError, "reading the FLAC file");
			return -1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_SEEK_ERROR:
			PyErr_SetString(PyExc_IOError, "seeking in the FLAC file");
			return -1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_WRITE_ERROR:
			PyErr_SetString(PyExc_IOError, "writing the FLAC file");
			return -1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_RENAME_ERROR:
			PyErr_SetString(PyExc_IOError, "renaming the FLAC file");
			return -1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_UNLINK_ERROR:
			PyErr_SetString(PyExc_IOError, "removing the temporary file");
			return -1;
		case FLAC__METADATA_SIMPLE_ITERATOR_STATUS_MEMORY_ALLOCATION_ERROR:
			self->init = init;
			PyErr_NoMemory();
			return -1;
		default:
			PyFLAC_RuntimeError("an unexpected error occurred");
			return -1;
	}
}


static PyTypeObject PyFLAC_MetadataSimpleIteratorType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(MetadataSimpleIterator),
	sizeof(flac_MetadataSimpleIteratorObject),
};


static PyObject *
flac_MetadataSimpleIterator_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	flac_MetadataSimpleIteratorObject *self;

	self = PyObject_New(flac_MetadataSimpleIteratorObject, type);

	if (self)
	{
		self->iterator = FLAC__metadata_simple_iterator_new();

		if (!self->iterator)
		{
			Py_DECREF(self);
			return PyErr_NoMemory();
		}

		self->init = false;
	}

	return (PyObject *) self;
}


static int
flac_MetadataSimpleIterator_init (flac_MetadataSimpleIteratorObject *self, PyObject *args, PyObject *kwds)
{
	FLAC__bool read_only, preserve_file_stats;
	const char *filename;

	read_only = preserve_file_stats = true;

	if (!PyArg_ParseTuple(args, "s|O&O&", &filename, _bool, &read_only, _bool, &preserve_file_stats))
		return -1;

	self->init = FLAC__metadata_simple_iterator_init(self->iterator, filename, read_only, preserve_file_stats);
	self->next = false;

	if (self->init)
		return 0;

	return flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self);
}


static void
flac_MetadataSimpleIterator_dealloc (flac_MetadataSimpleIteratorObject *self)
{
	if (self->iterator)
		FLAC__metadata_simple_iterator_delete(self->iterator);
	PyObject_Del(self);
}


static PyObject *
flac_MetadataSimpleIterator_is_writable (flac_MetadataSimpleIteratorObject *self)
{
	PyFLAC_CHECK_initialized(self);

	return PyFLAC_bool(FLAC__metadata_simple_iterator_is_writable(self->iterator));
}


static PyObject *
flac_MetadataSimpleIterator_next (flac_MetadataSimpleIteratorObject *self)
{
	PyFLAC_CHECK_initialized(self);

	self->next = true;

	if (FLAC__metadata_simple_iterator_next(self->iterator))
		Py_RETURN_TRUE;

	if (flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self) < 0)
		return NULL;

	Py_RETURN_FALSE;
}


static PyObject *
flac_MetadataSimpleIterator_prev (flac_MetadataSimpleIteratorObject *self)
{
	PyFLAC_CHECK_initialized(self);

	if (FLAC__metadata_simple_iterator_prev(self->iterator))
		Py_RETURN_TRUE;

	if (flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self) < 0)
		return NULL;

	self->next = false;
	Py_RETURN_FALSE;
}


static PyObject *
flac_MetadataSimpleIterator_is_last (flac_MetadataSimpleIteratorObject *self)
{
	PyFLAC_CHECK_initialized(self);

	return PyFLAC_bool(FLAC__metadata_simple_iterator_is_last(self->iterator));
}


static PyObject *
flac_MetadataSimpleIterator_get_block_offset (flac_MetadataSimpleIteratorObject *self)
{
	PyFLAC_CHECK_initialized(self);

	return PyFLAC_off_t(FLAC__metadata_simple_iterator_get_block_offset(self->iterator));
}


static PyObject *
flac_MetadataSimpleIterator_get_block_type (flac_MetadataSimpleIteratorObject *self)
{
	PyObject *attr;

	PyFLAC_CHECK_initialized(self);

	attr = PyFLAC_Enum_FromEnum(FLAC__metadata_simple_iterator_get_block_type(self->iterator), MetadataType);
	Py_XINCREF(attr);

	return attr;
}


static PyObject *
flac_MetadataSimpleIterator_get_block_length (flac_MetadataSimpleIteratorObject *self)
{
	PyFLAC_CHECK_initialized(self);

	return PyFLAC_unsigned(FLAC__metadata_simple_iterator_get_block_length(self->iterator));
}


static PyObject *
flac_MetadataSimpleIterator_get_application_id (flac_MetadataSimpleIteratorObject *self)
{
	FLAC__byte id[4];

	PyFLAC_CHECK_initialized(self);

	if (FLAC__metadata_simple_iterator_get_application_id(self->iterator, id))
		return PyFLAC_data(id, 4);

	if (flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self) < 0)
		return NULL;

	Py_RETURN_NONE;
}



static PyObject *
flac_MetadataSimpleIterator__get_block (flac_MetadataSimpleIteratorObject *self)
{
	FLAC__StreamMetadata *block;

	if ((block = FLAC__metadata_simple_iterator_get_block(self->iterator)))
		return PyFLAC_StreamMetadata_FromClass(block);

	if (flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self) < 0)
		return NULL;

	return PyErr_NoMemory();
}


static PyObject *
flac_MetadataSimpleIterator_get_block (flac_MetadataSimpleIteratorObject *self)
{
	PyFLAC_CHECK_initialized(self);

	return flac_MetadataSimpleIterator__get_block(self);
}


static PyObject *
flac_MetadataSimpleIterator_set_block (flac_MetadataSimpleIteratorObject *self, PyObject *args)
{
	FLAC__bool use_padding;
	PyFLAC_StreamMetadataObject *metadata;

	PyFLAC_CHECK_initialized(self);

	use_padding = true;

	if (!PyArg_ParseTuple(args, "O!|O&", PyFLAC_type(StreamMetadata), &metadata, _bool, &use_padding))
		return NULL;

	if (FLAC__metadata_simple_iterator_set_block(self->iterator, metadata->metadata, use_padding))
		Py_RETURN_NONE;

	if (flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self) < 0)
		return NULL;

	if (FLAC__metadata_simple_iterator_get_block_type(self->iterator) == FLAC__METADATA_TYPE_STREAMINFO)
		PyErr_Format(PyExc_TypeError, "must be " PyFLAC_name(StreamMetadataStreamInfo) ", not %.50s", Py_TYPE(metadata)->tp_name);
	else
		PyErr_SetString(PyExc_TypeError, "cannot set a " PyFLAC_name(StreamMetadataStreamInfo));

	return NULL;
}


static PyObject *
flac_MetadataSimpleIterator_insert_block_after (flac_MetadataSimpleIteratorObject *self, PyObject *args)
{
	FLAC__bool use_padding;
	PyFLAC_StreamMetadataObject *metadata;

	PyFLAC_CHECK_initialized(self);

	use_padding = true;

	if (!PyArg_ParseTuple(args, "O!|O&", PyFLAC_type(StreamMetadata), &metadata, _bool, &use_padding))
		return NULL;

	if (FLAC__metadata_simple_iterator_insert_block_after(self->iterator, metadata->metadata, use_padding))
	{
		self->next = false;
		Py_RETURN_NONE;
	}

	switch (flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self))
	{
		case 0:
			PyErr_SetString(PyExc_TypeError, "cannot insert a " PyFLAC_name(StreamMetadataStreamInfo));
			break;
		case 1:
			if (FLAC__metadata_simple_iterator_is_writable(self->iterator))
				PyFLAC_RuntimeError("an unexpected error occurred");
			else
				PyErr_SetString(PyExc_IOError, "File not open for writing");
	}

	return NULL;
}


static PyObject *
flac_MetadataSimpleIterator_delete_block (flac_MetadataSimpleIteratorObject *self, PyObject *args)
{
	FLAC__bool use_padding;

	PyFLAC_CHECK_initialized(self);

	use_padding = true;

	if (!PyArg_ParseTuple(args, "|O&", _bool, &use_padding))
		return NULL;

	if (FLAC__metadata_simple_iterator_delete_block(self->iterator, use_padding))
		Py_RETURN_NONE;

	flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self);
	return NULL;
}


static PyObject *
flac_MetadataSimpleIterator_reset (flac_MetadataSimpleIteratorObject *self)
{
	PyFLAC_CHECK_initialized(self);

	while (FLAC__metadata_simple_iterator_prev(self->iterator));
	if (flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self) < 0)
		return NULL;

	self->next = false;

	Py_INCREF(self);
	return (PyObject *) self;
}


static PyMethodDef flac_MetadataSimpleIterator_methods[] = {
	{
		"is_writable",
		(PyCFunction) flac_MetadataSimpleIterator_is_writable,
		METH_NOARGS,
		"Is writable"
	}, {
		"next",
		(PyCFunction) flac_MetadataSimpleIterator_next,
		METH_NOARGS,
		"Next"
	}, {
		"prev",
		(PyCFunction) flac_MetadataSimpleIterator_prev,
		METH_NOARGS,
		"Prev"
	}, {
		"is_last",
		(PyCFunction) flac_MetadataSimpleIterator_is_last,
		METH_NOARGS,
		"Is last"
	}, {
		"get_block_offset",
		(PyCFunction) flac_MetadataSimpleIterator_get_block_offset,
		METH_NOARGS,
		"Get block offset"
	}, {
		"get_block_type",
		(PyCFunction) flac_MetadataSimpleIterator_get_block_type,
		METH_NOARGS,
		"Get block type"
	}, {
		"get_block_length",
		(PyCFunction) flac_MetadataSimpleIterator_get_block_length,
		METH_NOARGS,
		"Get block length"
	}, {
		"get_application_id",
		(PyCFunction) flac_MetadataSimpleIterator_get_application_id,
		METH_NOARGS,
		"Get application id"
	}, {
		"get_block",
		(PyCFunction) flac_MetadataSimpleIterator_get_block,
		METH_NOARGS,
		"Get block"
	}, {
		"set_block",
		(PyCFunction) flac_MetadataSimpleIterator_set_block,
		METH_VARARGS,
		"Set block"
	}, {
		"insert_block_after",
		(PyCFunction) flac_MetadataSimpleIterator_insert_block_after,
		METH_VARARGS,
		"Insert block after"
	}, {
		"delete_block",
		(PyCFunction) flac_MetadataSimpleIterator_delete_block,
		METH_VARARGS,
		"Delete block"
	}, {
		"reset",
		(PyCFunction) flac_MetadataSimpleIterator_reset,
		METH_NOARGS,
		"Reset"
	}, { NULL }		/* Sentinel */
};

#ifndef __PyFLAC3__

typedef struct {
	PyObject_HEAD
	flac_MetadataSimpleIteratorObject *parent;
} flac_MetadataSimpleIteratorIterObject;


static PyTypeObject PyFLAC_MetadataSimpleIteratorIterType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(MetadataSimpleIteratorIter),
	sizeof(flac_MetadataSimpleIteratorIterObject),
};


static void
flac_MetadataSimpleIteratorIter_dealloc (flac_MetadataSimpleIteratorIterObject *self)
{
	Py_XDECREF(self->parent);
	PyObject_Del(self);
}


static PyObject *
flac_MetadataSimpleIterator_iter (flac_MetadataSimpleIteratorObject *self)
{
	flac_MetadataSimpleIteratorIterObject *iter;

	iter = PyObject_New(flac_MetadataSimpleIteratorIterObject, PyFLAC_type(MetadataSimpleIteratorIter));
	if (iter)
	{
		Py_INCREF(self);
		iter->parent = self;
	}

	return (PyObject *) iter;
}


static PyObject * flac_MetadataSimpleIterator_get_next_block (flac_MetadataSimpleIteratorObject *);

static int
flac_MetadataSimpleIteratorIter_Ready ( void )
{
	PyTypeObject *type = PyFLAC_type(MetadataSimpleIteratorIter);

	type->tp_dealloc = (destructor) flac_MetadataSimpleIteratorIter_dealloc;
	type->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER;
	type->tp_doc = "FLAC MetadataSimpleIterator iterable";
	type->tp_iter = PyObject_SelfIter;
	type->tp_iternext = (iternextfunc) flac_MetadataSimpleIterator_get_next_block;
	type->tp_dict = NULL;

	return PyType_Ready(type);
}

#endif // __PyFLAC3__


static PyObject *
flac_MetadataSimpleIterator_get_next_block (flac_MetadataSimpleIteratorObject *self)
{
#ifndef __PyFLAC3__
	self = ((flac_MetadataSimpleIteratorIterObject *) self)->parent;
#endif // __PyFLAC3__

	PyFLAC_CHECK_initialized(self);

	if (self->next && !FLAC__metadata_simple_iterator_next(self->iterator))
	{
		flac_MetadataSimpleIterator_PyErr_FromIteratorStatus(self);
		return NULL;
	}

	self->next = true;
	return flac_MetadataSimpleIterator__get_block(self);
}


static int
flac_MetadataSimpleIterator_Ready ( void )
{
	PyTypeObject *type = PyFLAC_type(MetadataSimpleIterator);

	type->tp_dealloc = (destructor) flac_MetadataSimpleIterator_dealloc;
	type->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER;
	type->tp_doc = "FLAC MetadataSimpleIterator";
#ifdef __PyFLAC3__
	type->tp_iter = PyObject_SelfIter;
	type->tp_iternext = (iternextfunc) flac_MetadataSimpleIterator_get_next_block;
#else // __PyFLAC3__
	type->tp_iter = (getiterfunc) flac_MetadataSimpleIterator_iter;
#endif // __PyFLAC3__
	type->tp_methods = flac_MetadataSimpleIterator_methods;
	type->tp_dict = NULL;
	type->tp_init = (initproc) flac_MetadataSimpleIterator_init;
	type->tp_new = flac_MetadataSimpleIterator_new;

	return PyType_Ready(type);
}


static PyMethodDef flac_metadata1_functions[] = {
	{ NULL }		/* Sentinel */
};


static int
flac_metadata1_init ( void )
{
	PyFLAC_CHECK_status(PyFLAC_import_format());
	PyFLAC_CHECK_status(PyFLAC_import_metadata());

	PyFLAC_CHECK_status(flac_MetadataSimpleIterator_Ready());
#ifndef __PyFLAC3__
	PyFLAC_CHECK_status(flac_MetadataSimpleIteratorIter_Ready());
#endif // __PyFLAC3__

	return 0;
}


static void
flac_metadata1_build (PyObject *module)
{
	Py_INCREF(PyFLAC_type(MetadataSimpleIterator));
	PyFLAC_PyModule_AddType(module, MetadataSimpleIterator);

	Py_INCREF(PyFLAC_type(FlacFormatError));
	PyFLAC_PyModule_AddType(module, FlacFormatError);
}


PyFLAC_MODINIT(
	metadata1,
	flac_metadata1_init,
	flac_metadata1_build,
	flac_metadata1_functions,
	"libFLAC metadata level 1 interface"
)

