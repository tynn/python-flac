/*
 *	This file is part of python-flac.
 *
 *	Copyright (c) 2015 Christian Schmitz <tynn.dev@gmail.com>
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
#include <structmember.h>

#define __PyFLAC_decoder_MODULE__
#include "decoder.h"

#include "metadata.h"


PyFLAC_Enum(StreamDecoderState)
PyFLAC_Enum(StreamDecoderInitStatus)
PyFLAC_Enum(StreamDecoderReadStatus)
PyFLAC_Enum(StreamDecoderSeekStatus)
PyFLAC_Enum(StreamDecoderTellStatus)
PyFLAC_Enum(StreamDecoderLengthStatus)
PyFLAC_Enum(StreamDecoderWriteStatus)
PyFLAC_Enum(StreamDecoderErrorStatus)


static PyFLAC_Enum_Member_Def enum_member_StreamDecoderState[] = {
	PyFLAC_Enum_Member(
		"SEARCH_FOR_METADATA",
		STREAM_DECODER_SEARCH_FOR_METADATA
	),
	PyFLAC_Enum_Member(
		"READ_METADATA",
		STREAM_DECODER_READ_METADATA
	),
	PyFLAC_Enum_Member(
		"SEARCH_FOR_FRAME_SYNC",
		STREAM_DECODER_SEARCH_FOR_FRAME_SYNC
	),
	PyFLAC_Enum_Member(
		"READ_FRAME",
		STREAM_DECODER_READ_FRAME
	),
	PyFLAC_Enum_Member(
		"END_OF_STREAM",
		STREAM_DECODER_END_OF_STREAM
	),
	PyFLAC_Enum_Member(
		"OGG_ERROR",
		STREAM_DECODER_OGG_ERROR
	),
	PyFLAC_Enum_Member(
		"SEEK_ERROR",
		STREAM_DECODER_SEEK_ERROR
	),
	PyFLAC_Enum_Member(
		"ABORTED",
		STREAM_DECODER_ABORTED
	),
	PyFLAC_Enum_Member(
		"MEMORY_ALLOCATION_ERROR",
		STREAM_DECODER_MEMORY_ALLOCATION_ERROR
	),
	PyFLAC_Enum_Member(
		"UNINITIALIZED",
		STREAM_DECODER_UNINITIALIZED
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamDecoderInitStatus[] = {
	PyFLAC_Enum_Member(
		"OK",
		STREAM_DECODER_INIT_STATUS_OK
	),
	PyFLAC_Enum_Member(
		"UNSUPPORTED_CONTAINER",
		STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER
	),
	PyFLAC_Enum_Member(
		"INVALID_CALLBACKS",
		STREAM_DECODER_INIT_STATUS_INVALID_CALLBACKS
	),
	PyFLAC_Enum_Member(
		"MEMORY_ALLOCATION_ERROR",
		STREAM_DECODER_INIT_STATUS_MEMORY_ALLOCATION_ERROR
	),
	PyFLAC_Enum_Member(
		"ERROR_OPENING_FILE",
		STREAM_DECODER_INIT_STATUS_ERROR_OPENING_FILE
	),
	PyFLAC_Enum_Member(
		"ALREADY_INITIALIZED",
		STREAM_DECODER_INIT_STATUS_ALREADY_INITIALIZED
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamDecoderReadStatus[] = {
	PyFLAC_Enum_Member(
		"CONTINUE",
		STREAM_DECODER_READ_STATUS_CONTINUE
	),
	PyFLAC_Enum_Member(
		"END_OF_STREAM",
		STREAM_DECODER_READ_STATUS_END_OF_STREAM
	),
	PyFLAC_Enum_Member(
		"ABORT",
		STREAM_DECODER_READ_STATUS_ABORT
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamDecoderSeekStatus[] = {
	PyFLAC_Enum_Member(
		"OK",
		STREAM_DECODER_SEEK_STATUS_OK
	),
	PyFLAC_Enum_Member(
		"ERROR",
		STREAM_DECODER_SEEK_STATUS_ERROR
	),
	PyFLAC_Enum_Member(
		"UNSUPPORTED",
		STREAM_DECODER_SEEK_STATUS_UNSUPPORTED
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamDecoderTellStatus[] = {
	PyFLAC_Enum_Member(
		"OK",
		STREAM_DECODER_TELL_STATUS_OK
	),
	PyFLAC_Enum_Member(
		"ERROR",
		STREAM_DECODER_TELL_STATUS_ERROR
	),
	PyFLAC_Enum_Member(
		"UNSUPPORTED",
		STREAM_DECODER_TELL_STATUS_UNSUPPORTED
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamDecoderLengthStatus[] = {
	PyFLAC_Enum_Member(
		"OK",
		STREAM_DECODER_LENGTH_STATUS_OK
	),
	PyFLAC_Enum_Member(
		"ERROR",
		STREAM_DECODER_LENGTH_STATUS_ERROR
	),
	PyFLAC_Enum_Member(
		"UNSUPPORTED",
		STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamDecoderWriteStatus[] = {
	PyFLAC_Enum_Member(
		"CONTINUE",
		STREAM_DECODER_WRITE_STATUS_CONTINUE
	),
	PyFLAC_Enum_Member(
		"ABORT",
		STREAM_DECODER_WRITE_STATUS_ABORT
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamDecoderErrorStatus[] = {
	PyFLAC_Enum_Member(
		"LOST_SYNC",
		STREAM_DECODER_ERROR_STATUS_LOST_SYNC
	),
	PyFLAC_Enum_Member(
		"BAD_HEADER",
		STREAM_DECODER_ERROR_STATUS_BAD_HEADER
	),
	PyFLAC_Enum_Member(
		"FRAME_CRC_MISMATCH",
		STREAM_DECODER_ERROR_STATUS_FRAME_CRC_MISMATCH
	),
	PyFLAC_Enum_Member(
		"UNPARSEABLE_STREAM",
		STREAM_DECODER_ERROR_STATUS_UNPARSEABLE_STREAM
	),
	{ NULL }		/* Sentinel */
};


#define flac_Int32Iter_New(data,data_size,data_host) \
	PyFLAC_Iter_New(data, data_size, flac_Int32Iter_next, "int32", (PyObject *) data_host)

static PyObject *
flac_Int32Iter_next (PyObject *iter, const void *data, Py_ssize_t index)
{
	return PyFLAC_int32(((FLAC__int32 *) data)[index]);
}


static PyObject *
flac_UnsignedIter_next (PyObject *iter, const void *data, Py_ssize_t index)
{
	return PyFLAC_unsigned(((unsigned *) data)[index]);
}


typedef struct {
	PyObject_HEAD
	PyObject *data_host;
	const FLAC__EntropyCodingMethod_PartitionedRiceContents *contents;
	unsigned capacity_by_order;
} flac_EntropyCodingMethodPartitionedRiceContentsObject;


static void
flac_EntropyCodingMethodPartitionedRiceContents_dealloc (flac_EntropyCodingMethodPartitionedRiceContentsObject *self)
{
	Py_XDECREF(self->data_host);
	PyObject_Del(self);
}


static PyMemberDef flac_EntropyCodingMethodPartitionedRiceContents_members[] = {
	{
		"capacity_by_order", T_OBJECT_EX,
		offsetof(flac_EntropyCodingMethodPartitionedRiceContentsObject, capacity_by_order),
		READONLY, "EntropyCodingMethodPartitionedRiceContents capacity_by_order"
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_EntropyCodingMethodPartitionedRiceContents_get (flac_EntropyCodingMethodPartitionedRiceContentsObject *self, void *closure)
{
	const unsigned *data;
	Py_ssize_t size = 1;

	size <<= self->contents->capacity_by_order;
	data = closure ? self->contents->parameters : self->contents->raw_bits;

	return PyFLAC_Iter_New(data, size, flac_UnsignedIter_next, "unsigned", (PyObject *) self);
}


static PyGetSetDef flac_EntropyCodingMethodPartitionedRiceContents_getseters[] = {
	{
		"parameters",
		(getter) flac_EntropyCodingMethodPartitionedRiceContents_get, NULL,
		"EntropyCodingMethodPartitionedRiceContents parameters", (void *) 1
	}, {
		"raw_bits",
		(getter) flac_EntropyCodingMethodPartitionedRiceContents_get, NULL,
		"EntropyCodingMethodPartitionedRiceContents raw_bits", (void *) 0
	}, { NULL }		/* Sentinel */
};


static PyTypeObject flac_EntropyCodingMethodPartitionedRiceContentsType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(EntropyCodingMethodPartitionedRiceContents),
	sizeof(flac_EntropyCodingMethodPartitionedRiceContentsObject), 0,
	(destructor) flac_EntropyCodingMethodPartitionedRiceContents_dealloc,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	Py_TPFLAGS_DEFAULT, "FLAC EntropyCodingMethodPartitionedRiceContents",
	0, 0, 0, 0, 0, 0, 0,
	flac_EntropyCodingMethodPartitionedRiceContents_members,
	flac_EntropyCodingMethodPartitionedRiceContents_getseters,
	0, 0,
};


static PyObject *
flac_EntropyCodingMethodPartitionedRiceContents_FromClass (const FLAC__EntropyCodingMethod_PartitionedRiceContents *contents, PyObject *data_host)
{
	flac_EntropyCodingMethodPartitionedRiceContentsObject *self;

	self = PyObject_New(flac_EntropyCodingMethodPartitionedRiceContentsObject, &flac_EntropyCodingMethodPartitionedRiceContentsType);
	if (!self)
		return NULL;

	Py_INCREF(data_host);
	self->data_host = data_host;
	self->contents = contents;
	self->capacity_by_order = contents->capacity_by_order;

	return (PyObject *) self;
}


static PyObject *
flac_EntropyCodingMethodPartitionedRiceContents_iternext (PyObject *iter, const void *data, Py_ssize_t index)
{
	return flac_EntropyCodingMethodPartitionedRiceContents_FromClass(&((FLAC__EntropyCodingMethod_PartitionedRiceContents *) data)[index], iter);
}


typedef struct {
	PyObject_HEAD
	PyObject *data_host;
	const FLAC__EntropyCodingMethod *entropy_coding_method;
	PyObject *type;
} flac_EntropyCodingMethodObject;


static void
flac_EntropyCodingMethod_dealloc (flac_EntropyCodingMethodObject *self)
{
	Py_XDECREF(self->type);
	Py_XDECREF(self->data_host);
	PyObject_Del(self);
}


static PyMemberDef flac_EntropyCodingMethod_members[] = {
	{
		"type", T_OBJECT_EX,
		offsetof(flac_EntropyCodingMethodObject, type),
		READONLY, "EntropyCodingMethod type"
	}, { NULL }		/* Sentinel */
};


static PyTypeObject flac_EntropyCodingMethodType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(EntropyCodingMethod),
	sizeof(flac_EntropyCodingMethodObject), 0,
	(destructor) flac_EntropyCodingMethod_dealloc,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	Py_TPFLAGS_DEFAULT, "FLAC EntropyCodingMethod",
	0, 0, 0, 0, 0, 0, 0,
	flac_EntropyCodingMethod_members, 0, 0, 0,
};


static PyTypeObject flac_EntropyCodingMethodPartitionedRiceType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(EntropyCodingMethodPartitionedRice),
	sizeof(flac_EntropyCodingMethodObject)
};


static PyObject *
flac_EntropyCodingMethodPartitionedRice_get (flac_EntropyCodingMethodObject *self, void *closure)
{
	unsigned order = self->entropy_coding_method->data.partitioned_rice.order;

	if (closure)
		return PyFLAC_unsigned(order);

	return PyFLAC_Iter_New(self->entropy_coding_method->data.partitioned_rice.contents, order, flac_EntropyCodingMethodPartitionedRiceContents_iternext, PyFLAC_name(EntropyCodingMethodPartitionedRiceContents), (PyObject *) self);
}


static PyGetSetDef flac_EntropyCodingMethodPartitionedRice_getseters[] = {
	{
		"order",
		(getter) flac_EntropyCodingMethodPartitionedRice_get, NULL,
		"EntropyCodingMethodPartitionedRice order", (void *) 1
	}, {
		"contents",
		(getter) flac_EntropyCodingMethodPartitionedRice_get, NULL,
		"EntropyCodingMethodPartitionedRice contents", (void *) 0
	}, { NULL }		/* Sentinel */
};


static int
flac_EntropyCodingMethodPartitionedRice_Ready (void)
{
	PyTypeObject *type = &flac_EntropyCodingMethodPartitionedRiceType;

	if (type->tp_flags & Py_TPFLAGS_READY)
		return 0;

	type->tp_flags = Py_TPFLAGS_DEFAULT;
	type->tp_doc = "FLAC EntropyCodingMethodPartitionedRice";
	type->tp_getset = flac_EntropyCodingMethodPartitionedRice_getseters;
	type->tp_base = &flac_EntropyCodingMethodType;
	type->tp_dict = NULL;

	return PyType_Ready(type);
}


static PyObject *
flac_EntropyCodingMethod_FromClass (const FLAC__EntropyCodingMethod *entropy_coding_method, PyObject *data_host)
{
	flac_EntropyCodingMethodObject *self;
	PyObject *type;

	type = PyFLAC_Enum_FromEnum(entropy_coding_method->type, EntropyCodingMethodType);
	if (!type)
		return NULL;

	self = PyObject_New(flac_EntropyCodingMethodObject, &flac_EntropyCodingMethodPartitionedRiceType);
	if (!self)
		return NULL;

	Py_INCREF(data_host);
	self->data_host = data_host;
	self->entropy_coding_method = entropy_coding_method;
	Py_INCREF(type);
	self->type = type;

	return (PyObject *) self;
}


typedef struct {
	PyObject_HEAD
	PyObject *data_host;
	unsigned data_size;
	const FLAC__Subframe *subframe;
	PyObject *type;
	unsigned wasted_bits;
} flac_SubframeObject;


static void
flac_Subframe_dealloc (flac_SubframeObject *self)
{
	Py_XDECREF(self->type);
	Py_XDECREF(self->data_host);
	PyObject_Del(self);
}


static PyMemberDef flac_Subframe_members[] = {
	{
		"type", T_OBJECT_EX,
		offsetof(flac_SubframeObject, type),
		READONLY, "Subframe type"
	}, {
		"wasted_bits", T_UINT,
		offsetof(flac_SubframeObject, wasted_bits),
		READONLY, "Subframe wasted_bits"
	}, { NULL }		/* Sentinel */
};


static PyTypeObject flac_SubframeType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(Subframe),
	sizeof(flac_SubframeObject), 0,
	(destructor) flac_Subframe_dealloc,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	Py_TPFLAGS_DEFAULT, "FLAC Subframe",
	0, 0, 0, 0, 0, 0, 0,
	flac_Subframe_members, 0, 0, 0,
};


static PyTypeObject flac_SubframeSubtype[] = {
	{
		PyVarObject_HEAD_INIT(NULL,0)
		PyFLAC_name(SubframeConstant),
		sizeof(flac_SubframeObject)
	}, {
		PyVarObject_HEAD_INIT(NULL,0)
		PyFLAC_name(SubframeVerbatim),
		sizeof(flac_SubframeObject)
	}, {
		PyVarObject_HEAD_INIT(NULL,0)
		PyFLAC_name(SubframeFixed),
		sizeof(flac_SubframeObject)
	}, {
		PyVarObject_HEAD_INIT(NULL,0)
		PyFLAC_name(SubframeLPC),
		sizeof(flac_SubframeObject)
	}
};


static PyObject *
flac_SubframeConstant_value (flac_SubframeObject *self, void *closure)
{
	return PyFLAC_int32(self->subframe->data.constant.value);
}


static PyGetSetDef flac_SubframeConstant_getseters[] = {
	{
		"value",
		(getter) flac_SubframeConstant_value, NULL,
		"SubframeConstant value", NULL
	}, { NULL }		/* Sentinel */
};


enum flac_SubframeLPC_member {
	flac_SubframeLPC_entropy_coding_method,
	flac_SubframeLPC_order,
	flac_SubframeLPC_qlp_coeff_precision,
	flac_SubframeLPC_quantization_level,
	flac_SubframeLPC_qlp_coeff,
	flac_SubframeLPC_warmup,
	flac_SubframeLPC_residual,
};


static PyObject *
flac_SubframeFixed_get (flac_SubframeObject *self, void *closure)
{
	switch ((enum flac_SubframeLPC_member) closure)
	{
		case flac_SubframeLPC_entropy_coding_method:
			return flac_EntropyCodingMethod_FromClass(&self->subframe->data.fixed.entropy_coding_method, (PyObject *) self);
		case flac_SubframeLPC_order:
			return PyFLAC_unsigned(self->subframe->data.fixed.order);
		case flac_SubframeLPC_warmup:
			return flac_Int32Iter_New(self->subframe->data.fixed.warmup, self->subframe->data.fixed.order, self);
		case flac_SubframeLPC_residual:
			return flac_Int32Iter_New(self->subframe->data.fixed.residual, self->data_size, self);
		default:
			return PyFLAC_getter_error(SubframeFixed);
	}
}


static PyGetSetDef flac_SubframeFixed_getseters[] = {
	{
		"entropy_coding_method",
		(getter) flac_SubframeFixed_get, NULL,
		"SubframeFixed entropy_coding_method", (void *) flac_SubframeLPC_entropy_coding_method
	}, {
		"order",
		(getter) flac_SubframeFixed_get, NULL,
		"SubframeFixed order", (void *) flac_SubframeLPC_order
	}, {
		"warmup",
		(getter) flac_SubframeFixed_get, NULL,
		"SubframeFixed warmup", (void *) flac_SubframeLPC_warmup
	}, {
		"residual",
		(getter) flac_SubframeFixed_get, NULL,
		"SubframeFixed residual", (void *) flac_SubframeLPC_residual
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_SubframeLPC_get (flac_SubframeObject *self, void *closure)
{
	switch ((enum flac_SubframeLPC_member) closure)
	{
		case flac_SubframeLPC_entropy_coding_method:
			return flac_EntropyCodingMethod_FromClass(&self->subframe->data.lpc.entropy_coding_method, (PyObject *) self);
		case flac_SubframeLPC_order:
			return PyFLAC_unsigned(self->subframe->data.lpc.order);
		case flac_SubframeLPC_qlp_coeff_precision:
			return PyFLAC_unsigned(self->subframe->data.lpc.qlp_coeff_precision);
		case flac_SubframeLPC_quantization_level:
			return PyFLAC_int(self->subframe->data.lpc.quantization_level);
		case flac_SubframeLPC_qlp_coeff:
			return flac_Int32Iter_New(self->subframe->data.lpc.qlp_coeff, self->subframe->data.lpc.order, self);
		case flac_SubframeLPC_warmup:
			return flac_Int32Iter_New(self->subframe->data.lpc.warmup, self->subframe->data.lpc.order, self);
		case flac_SubframeLPC_residual:
			return flac_Int32Iter_New(self->subframe->data.lpc.residual, self->data_size, self);
		default:
			return PyFLAC_getter_error(SubframeLPC);
	}
}


static PyGetSetDef flac_SubframeLPC_getseters[] = {
	{
		"entropy_coding_method",
		(getter) flac_SubframeLPC_get, NULL,
		"SubframeLPC entropy_coding_method", (void *) flac_SubframeLPC_entropy_coding_method
	}, {
		"order",
		(getter) flac_SubframeLPC_get, NULL,
		"SubframeLPC order", (void *) flac_SubframeLPC_order
	}, {
		"qlp_coeff_precision",
		(getter) flac_SubframeLPC_get, NULL,
		"SubframeLPC qlp_coeff_precision", (void *) flac_SubframeLPC_qlp_coeff_precision
	}, {
		"quantization_level",
		(getter) flac_SubframeLPC_get, NULL,
		"SubframeLPC quantization_level", (void *) flac_SubframeLPC_quantization_level
	}, {
		"qlp_coeff",
		(getter) flac_SubframeLPC_get, NULL,
		"SubframeLPC qlp_coeff", (void *) flac_SubframeLPC_qlp_coeff
	}, {
		"warmup",
		(getter) flac_SubframeLPC_get, NULL,
		"SubframeLPC warmup", (void *) flac_SubframeLPC_warmup
	}, {
		"residual",
		(getter) flac_SubframeLPC_get, NULL,
		"SubframeLPC residual", (void *) flac_SubframeLPC_residual
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_SubframeVerbatim_data (flac_SubframeObject *self, void *closure)
{
	return flac_Int32Iter_New(self->subframe->data.verbatim.data, self->data_size, self);
}


static PyGetSetDef flac_SubframeVerbatim_getseters[] = {
	{
		"data",
		(getter) flac_SubframeVerbatim_data, NULL,
		"SubframeVerbatim data", NULL
	}, { NULL }		/* Sentinel */
};


static PyGetSetDef *flac_SubframeSubtype_getseters[] = {
	flac_SubframeConstant_getseters,
	flac_SubframeVerbatim_getseters,
	flac_SubframeFixed_getseters,
	flac_SubframeLPC_getseters,
};


static int
flac_SubframeSubtype_Ready (FLAC__SubframeType subframe_type, const char *doc)
{
	PyTypeObject *type = &flac_SubframeSubtype[subframe_type];

	if (type->tp_flags & Py_TPFLAGS_READY)
		return 0;

	type->tp_flags = Py_TPFLAGS_DEFAULT;
	type->tp_doc = doc;
	type->tp_getset = flac_SubframeSubtype_getseters[subframe_type];
	type->tp_base = &flac_SubframeType;
	type->tp_dict = NULL;

	return PyType_Ready(type);
}


static PyObject *
flac_Subframe_FromClass (const FLAC__Subframe *subframe, unsigned data_size, PyObject *data_host)
{
	flac_SubframeObject *self;
	PyObject *subframe_type;
	PyTypeObject *type = &flac_SubframeSubtype[subframe->type];

	subframe_type = PyFLAC_Enum_FromEnum(subframe->type, SubframeType);
	if (!subframe_type)
		return NULL;

	self = PyObject_New(flac_SubframeObject, type);
	if (!self)
		return NULL;

	Py_INCREF(data_host);
	self->data_host = data_host;
	self->data_size = data_size;
	self->subframe = subframe;
	Py_INCREF(subframe_type);
	self->type = subframe_type;
	self->wasted_bits = subframe->wasted_bits;

	return (PyObject *) self;
}


static int
flac__subframe_init (void)
{
	PyFLAC_CHECK_status(PyType_Ready(&flac_SubframeType));

	PyFLAC_CHECK_status(flac_SubframeSubtype_Ready(FLAC__SUBFRAME_TYPE_CONSTANT, "FLAC SubframeConstant"));
	PyFLAC_CHECK_status(flac_SubframeSubtype_Ready(FLAC__SUBFRAME_TYPE_VERBATIM, "FLAC SubframeVerbatim"));
	PyFLAC_CHECK_status(flac_SubframeSubtype_Ready(FLAC__SUBFRAME_TYPE_FIXED, "FLAC SubframeFixed"));
	PyFLAC_CHECK_status(flac_SubframeSubtype_Ready(FLAC__SUBFRAME_TYPE_LPC, "FLAC SubframeLPC"));

	PyFLAC_CHECK_status(PyType_Ready(&flac_EntropyCodingMethodType));
	PyFLAC_CHECK_status(flac_EntropyCodingMethodPartitionedRice_Ready());

	return 0;
}


struct flac_SubframesObject {
	PyObject_VAR_HEAD
	struct flac_Subframe_obj {
		FLAC__Subframe obj;
		void *data;
		unsigned data_size;
	} data[1];
};


static void
flac_Subframes_dealloc (struct flac_SubframesObject *self)
{
	Py_ssize_t i;

	for (i = 0; i < ((PyVarObject *) self)->ob_size; i++)
		if (self->data[i].data)
			PyMem_Del(self->data[i].data);

	PyObject_Del(self);
}


static PyTypeObject flac_SubframesType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(Subframes),
	sizeof(struct flac_SubframesObject) - sizeof(struct flac_Subframe_obj),
	sizeof(struct flac_Subframe_obj),
	(destructor) flac_Subframes_dealloc,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	Py_TPFLAGS_DEFAULT, "FLAC Subframes",
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};


typedef struct {
	PyObject_HEAD
	unsigned blocksize;
	unsigned sample_rate;
	unsigned channels;
	PyObject *channel_assignment;
	unsigned bits_per_sample;
	PyObject *number_type;
	PyObject *number;
	FLAC__uint8 header_crc;
	FLAC__uint16 footer_crc;
	struct flac_SubframesObject *subframes;
} flac_FrameObject;


static void
flac_Frame_dealloc (flac_FrameObject *self)
{
	Py_XDECREF(self->channel_assignment);
	Py_XDECREF(self->number_type);
	Py_XDECREF(self->number);
	Py_XDECREF(self->subframes);
	PyObject_Del(self);
}


static PyMemberDef flac_Frame_members[] = {
	{
		"blocksize", T_UINT,
		offsetof(flac_FrameObject, blocksize),
		READONLY, "Frame blocksize"
	}, {
		"sample_rate", T_UINT,
		offsetof(flac_FrameObject, sample_rate),
		READONLY, "Frame sample_rate"
	}, {
		"channels", T_UINT,
		offsetof(flac_FrameObject, channels),
		READONLY, "Frame channels"
	}, {
		"channel_assignment", T_OBJECT_EX,
		offsetof(flac_FrameObject, channel_assignment),
		READONLY, "Frame channel_assignment"
	}, {
		"bits_per_sample", T_UINT,
		offsetof(flac_FrameObject, bits_per_sample),
		READONLY, "Frame bits_per_sample"
	}, {
		"number_type", T_OBJECT_EX,
		offsetof(flac_FrameObject, number_type),
		READONLY, "Frame number_type"
	}, {
		"number", T_OBJECT_EX,
		offsetof(flac_FrameObject, number),
		READONLY, "Frame number"
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_Frame_subframes_iternext (PyObject *iter, const void *data, Py_ssize_t index)
{
	struct flac_Subframe_obj *data_obj;

	if (flac__subframe_init() < 0)
		return NULL;

	data_obj = &((struct flac_Subframe_obj *) data)[index];
	return flac_Subframe_FromClass(&data_obj->obj, data_obj->data_size, iter);
}


static PyObject *
flac_Frame_subframes_get (flac_FrameObject *self, void *closure)
{
	return PyFLAC_Iter_New(self->subframes->data, ((PyVarObject *) self->subframes)->ob_size, flac_Frame_subframes_iternext, PyFLAC_name(Subframe), (PyObject *) self->subframes);
}


static int
flac_Frame_subframes_set (flac_FrameObject *self, PyObject *value, void *closure)
{
	if (!value || value == Py_None)
	{
		Py_CLEAR(self->subframes);
		return 0;
	}

	PyErr_SetString(PyExc_ValueError, "subframes must be None");
	return -1;
}


static PyObject *
flac_Frame_crc (flac_FrameObject *self, void *closure)
{
	if (closure)
		return PyFLAC_uint8(self->header_crc);

	return PyFLAC_uint16(self->footer_crc);
}


static PyGetSetDef flac_Frame_getseters[] = {
	{
		"subframes",
		(getter) flac_Frame_subframes_get,
		(setter) flac_Frame_subframes_set,
		"Frame subframes", NULL
	}, {
		"header_crc",
		(getter) flac_Frame_crc, NULL,
		"Frame header_crc", (void *) 1
	}, {
		"footer_crc",
		(getter) flac_Frame_crc, NULL,
		"Frame footer_crc", (void *) 0
	}, { NULL }		/* Sentinel */
};


static PyTypeObject flac_FrameType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(Frame),
	sizeof(flac_FrameObject), 0,
	(destructor) flac_Frame_dealloc,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	Py_TPFLAGS_DEFAULT, "FLAC Frame",
	0, 0, 0, 0, 0, 0, 0,
	flac_Frame_members, flac_Frame_getseters, 0, 0,
};


static PyObject *
flac_Frame_FromFrame (const FLAC__Frame *frame)
{
	flac_FrameObject *self;
	struct flac_SubframesObject *subframes;
	unsigned blocksize, i;
	void **data;

	subframes = PyObject_NewVar(struct flac_SubframesObject, &flac_SubframesType, frame->header.channels);

	if (!subframes)
		return NULL;

	for (i = 0; i < frame->header.channels; i++)
	{
		subframes->data[i].obj = frame->subframes[i];
		subframes->data[i].data = NULL;
	}

	blocksize = frame->header.blocksize;

	for (i = 0; i < frame->header.channels; i++)
	{
		switch (frame->subframes[i].type)
		{
			case FLAC__SUBFRAME_TYPE_FIXED:
				data = (void **) &frame->subframes[i].data.fixed.residual;
				subframes->data[i].data_size = blocksize - frame->subframes[i].data.fixed.order;
				break;
			case FLAC__SUBFRAME_TYPE_LPC:
				data = (void **) &frame->subframes[i].data.lpc.residual;
				subframes->data[i].data_size = blocksize - frame->subframes[i].data.lpc.order;
				break;
			case FLAC__SUBFRAME_TYPE_VERBATIM:
				data = (void **) &frame->subframes[i].data.verbatim.data;
				subframes->data[i].data_size = blocksize;
				break;
			default:
				data = NULL;
				subframes->data[i].data_size = 0;
		}

		if (data)
		{
			subframes->data[i].data = PyMem_New(FLAC__int32, subframes->data[i].data_size);
			if (!subframes->data[i].data)
			{
				Py_DECREF(subframes);
				return PyErr_NoMemory();
			}

			Py_MEMCPY(subframes->data[i].data, *data, sizeof(FLAC__int32) * subframes->data[i].data_size);
			*data = subframes->data[i].data;
		}
	}

	self = PyObject_New(flac_FrameObject, &flac_FrameType);

	if (!self)
	{
		Py_DECREF(subframes);
		return NULL;
	}

	self->subframes = subframes;

	self->blocksize = blocksize;
	self->sample_rate = frame->header.sample_rate;
	self->channels = frame->header.channels;
	self->bits_per_sample = frame->header.bits_per_sample;

	self->header_crc = frame->header.crc;
	self->footer_crc = frame->footer.crc;

	if (frame->header.number_type == FLAC__FRAME_NUMBER_TYPE_SAMPLE_NUMBER)
		self->number = PyFLAC_uint64(frame->header.number.sample_number);
	else
		self->number = PyFLAC_uint32(frame->header.number.frame_number);

	self->channel_assignment = PyFLAC_Enum_FromEnum(frame->header.channel_assignment, ChannelAssignment);
	Py_XINCREF(self->channel_assignment);
	self->number_type = PyFLAC_Enum_FromEnum(frame->header.number_type, FrameNumberType);
	Py_XINCREF(self->number_type);

	if (!self->number || !self->number_type || !self->channel_assignment)
		Py_CLEAR(self);

	return (PyObject *) self;
}


static PyObject *
flac_Buffer_data_iternext (PyObject *iter, const void *data, Py_ssize_t index)
{
	return PyFLAC_int32(((FLAC__int32 *) data)[index]);
}


static PyObject *
flac_Buffer_data (PyBytesObject *self, void *closure)
{
	return PyFLAC_Iter_New(self->ob_sval, ((PyVarObject *) self)->ob_size / 4, flac_Buffer_data_iternext, "int32", (PyObject *) self);
}


static PyGetSetDef flac_Buffer_getseters[] = {
	{
		"data",
		(getter) flac_Buffer_data, NULL,
		"Buffer data", NULL
	}, { NULL }		/* Sentinel */
};


static PyTypeObject flac_BufferType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(Buffer),
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	Py_TPFLAGS_DEFAULT, "FLAC Buffer",
	0, 0, 0, 0, 0, 0, 0, 0,
	flac_Buffer_getseters, 0, 0,
};


static void
flac_StreamDecoder_dealloc (flac_StreamDecoderObject *self)
{
	PyObject_GC_UnTrack(self);
	if (self->decoder)
	{
		FLAC__stream_decoder_finish(self->decoder);
		FLAC__stream_decoder_delete(self->decoder);
	}
	Py_XDECREF(self->stream);
	Py_TYPE(self)->tp_free((PyObject *) self);
}


static int
flac_StreamDecoder_traverse (flac_StreamDecoderObject *self, visitproc visit, void *arg)
{
	Py_VISIT(self->stream);
	return 0;
}


static int
flac_StreamDecoder_clear (flac_StreamDecoderObject *self)
{
	Py_CLEAR(self->stream);
	return 0;
}


static PyObject *
flac_StreamDecoder_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	flac_StreamDecoderObject *self;

	(void)args, (void)kwds;

	self = (flac_StreamDecoderObject *) type->tp_alloc(type, 0);

	if (self)
	{
		self->stream = NULL;
		self->decoder = FLAC__stream_decoder_new();

		if (!self->decoder)
		{
			flac_StreamDecoder_dealloc(self);
			return PyErr_NoMemory();
		}
	}

	return (PyObject *) self;
}


static PyObject *
flac_stream_from_client_data (void *client_data)
{
	flac_StreamDecoderObject *self = (flac_StreamDecoderObject *) client_data;
	if (self->stream)
		return self->stream;
	return (PyObject *) self;
}


static FLAC__StreamDecoderReadStatus
flac_StreamDecoder_ReadCallback (const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
{
	FLAC__StreamDecoderReadStatus status;
	PyObject *py_bytes;
	PyObject *callback_status;
	Py_ssize_t length;
	char *py_buffer;

	py_bytes = PyLong_FromSize_t(*bytes);
	if (!py_bytes)
		return FLAC__STREAM_DECODER_READ_STATUS_ABORT;

	callback_status = PyObject_CallMethod(flac_stream_from_client_data(client_data), "__decoder_read__", "(O)", py_bytes);
	Py_DECREF(py_bytes);

	if (!callback_status)
		return FLAC__STREAM_DECODER_READ_STATUS_ABORT;

	if (PyObject_Not(callback_status))
	{
		status = FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
	}
	else if (PyFLAC_type_Check(callback_status, StreamDecoderReadStatus))
	{
		status = PyFLAC_Enum_AsEnum(callback_status, StreamDecoderReadStatus);
		if (status == FLAC__STREAM_DECODER_READ_STATUS_CONTINUE)
		{
			status = FLAC__STREAM_DECODER_READ_STATUS_ABORT;
			PyErr_Format(PyExc_ValueError, "__decoder_read__ must not return %s.CONTINUE", Py_TYPE(callback_status)->tp_name);
		}
	}
	else if (!PyBytes_Check(callback_status))
	{
		status = FLAC__STREAM_DECODER_READ_STATUS_ABORT;
		PyErr_Format(PyExc_ValueError, "__decoder_read__ must return %s or %s, not %s", PyBytes_Type.tp_name, (PyFLAC_type(StreamDecoderReadStatus))->tp_name, Py_TYPE(callback_status)->tp_name);
	}
	else if (PyBytes_AsStringAndSize(callback_status, &py_buffer, &length) < 0)
	{
		status = FLAC__STREAM_DECODER_READ_STATUS_ABORT;
	}
	else if (length > *bytes)
	{
		status = FLAC__STREAM_DECODER_READ_STATUS_ABORT;
		PyErr_Format(PyExc_ValueError, "__decoder_read__ must return %s of length <= %zu", PyBytes_Type.tp_name, *bytes);
	}
	else
	{
		*bytes = (size_t) length;
		Py_MEMCPY(buffer, py_buffer, *bytes);
		status = FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
	}

	Py_DECREF(callback_status);
	return status;
}


static FLAC__StreamDecoderSeekStatus
flac_StreamDecoder_SeekCallback (const FLAC__StreamDecoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data)
{
	FLAC__StreamDecoderSeekStatus status;
	PyObject *py_offset;
	PyObject *callback_status;

	py_offset = PyFLAC_uint64(absolute_byte_offset);

	if (!py_offset)
		return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;

	callback_status = PyObject_CallMethod(flac_stream_from_client_data(client_data), "__decoder_seek__", "(O)", py_offset);
	Py_DECREF(py_offset);

	if (!callback_status)
		return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;

	if (PyFLAC_type_Check(callback_status, StreamDecoderSeekStatus))
	{
		status = PyFLAC_Enum_AsEnum(callback_status, StreamDecoderSeekStatus);
	}
	else if (callback_status == Py_None || callback_status == Py_False)
	{
		status = FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED;
	}
	else if (callback_status == Py_True)
	{
		status = FLAC__STREAM_DECODER_SEEK_STATUS_OK;
	}
	else
	{
		status = FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
		PyErr_Format(PyExc_TypeError, "__decoder_seek__ must return %s, bool or None, not %s", (PyFLAC_type(StreamDecoderSeekStatus))->tp_name, Py_TYPE(callback_status)->tp_name);
	}

	Py_DECREF(callback_status);
	return status;
}


static FLAC__StreamDecoderTellStatus
flac_StreamDecoder_TellCallback (const FLAC__StreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	FLAC__StreamDecoderTellStatus status;
	PyObject *callback_status;

	callback_status = PyObject_CallMethod(flac_stream_from_client_data(client_data), "__decoder_tell__", NULL);

	if (!callback_status)
		return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;

	if (PyFLAC_uint64_conv(callback_status, absolute_byte_offset) >= 0)
	{
		status = FLAC__STREAM_DECODER_TELL_STATUS_OK;
	}
	else if (callback_status == Py_None || callback_status == Py_False)
	{
		status = FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED;
	}
	else if (callback_status == Py_True)
	{
		status = FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
		PyErr_SetString(PyExc_ValueError, "__decoder_tell__ must not return True");
	}
	else if (!PyFLAC_type_Check(callback_status, StreamDecoderTellStatus))
	{
		status = FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
		PyErr_Format(PyExc_TypeError, "__decoder_tell__ must return %s, int, bool or None, not %s", (PyFLAC_type(StreamDecoderTellStatus))->tp_name, Py_TYPE(callback_status)->tp_name);
	}
	else if ((status = PyFLAC_Enum_AsEnum(callback_status, StreamDecoderTellStatus)) == FLAC__STREAM_DECODER_TELL_STATUS_OK)
	{
		status = FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
		PyErr_Format(PyExc_ValueError, "__decoder_tell__ must not return %s.OK", Py_TYPE(callback_status)->tp_name);
	}

	Py_DECREF(callback_status);
	return status;
}


static FLAC__StreamDecoderLengthStatus
flac_StreamDecoder_LengthCallback (const FLAC__StreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data)
{
	FLAC__StreamDecoderLengthStatus status;
	PyObject *callback_status;

	callback_status = PyObject_CallMethod(flac_stream_from_client_data(client_data), "__decoder_length__", NULL);

	if (!callback_status)
		return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;

	if (callback_status == Py_None || callback_status == Py_False)
	{
		status = FLAC__STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED;
	}
	else if (PyFLAC_type_Check(callback_status, StreamDecoderLengthStatus))
	{
		status = PyFLAC_Enum_AsEnum(callback_status, StreamDecoderLengthStatus);
		if (status == FLAC__STREAM_DECODER_LENGTH_STATUS_OK)
		{
			status = FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
			PyErr_Format(PyExc_ValueError, "__decoder_length__ must not return %s.OK", Py_TYPE(callback_status)->tp_name);
		}
	}
	else if (PyFLAC_uint64_conv(callback_status, stream_length) < 0)
	{
		status = FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
	}
	else
	{
		status = FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
	}

	Py_DECREF(callback_status);
	return status;
}


static FLAC__bool
flac_StreamDecoder_EofCallback (const FLAC__StreamDecoder *decoder, void *client_data)
{
	PyObject *callback_status;
	int B;

	callback_status = PyObject_CallMethod(flac_stream_from_client_data(client_data), "__decoder_eof__", NULL);

	if (!callback_status)
		return true;

	B = PyObject_Not(callback_status);
	Py_DECREF(callback_status);

	return B == 0;
}


static FLAC__StreamDecoderWriteStatus
flac_StreamDecoder_WriteCallback (const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 *const buffer[], void *client_data)
{
	FLAC__StreamDecoderWriteStatus status;
	PyObject *py_frame;
	PyObject *py_buffer;
	PyObject *callback_status;
	PyBytesObject *py_data;
	unsigned blocksize, i;
	PyObject *self = (PyObject *) client_data;

	if (PyErr_Occurred())
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;

	py_frame = flac_Frame_FromFrame(frame);
	if (!py_frame)
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;

	py_buffer = PyTuple_New(frame->header.channels);

	if (!py_buffer)
	{
		Py_DECREF(py_frame);
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}

	blocksize = frame->header.blocksize * 4;

	for (i = 0; i < frame->header.channels; i++)
	{
		py_data = PyObject_NewVar(PyBytesObject, &flac_BufferType, blocksize);
		py_data->ob_shash = -1;
#ifndef __PyFLAC3__
		py_data->ob_sstate = SSTATE_NOT_INTERNED;
#endif // __PyFLAC3__

		if (!py_data || PyTuple_SetItem(py_buffer, i, (PyObject *) py_data) < 0)
		{
			Py_DECREF(py_buffer);
			Py_DECREF(py_frame);
			return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
		}

		Py_MEMCPY(py_data->ob_sval, buffer[i], blocksize);
	}

	callback_status = PyObject_CallMethod(self, "__decoder_write__", "(OO)", py_frame, py_buffer);
	Py_DECREF(py_frame);
	Py_DECREF(py_buffer);

	if (!callback_status)
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;

	if (PyFLAC_type_Check(callback_status, StreamDecoderWriteStatus))
		status = PyFLAC_Enum_AsEnum(callback_status, StreamDecoderWriteStatus);
	else if (callback_status == Py_False)
		status = FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	else
		status = FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;

	Py_DECREF(callback_status);
	return status;
}


static void
flac_StreamDecoder_MetadataCallback (const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	PyObject *py_metadata;
	PyObject *callback_status;
	PyObject *self = (PyObject *) client_data;

	if (PyFLAC_import_metadata() < 0)
		return;

	py_metadata = PyFLAC_StreamMetadata_FromClass(metadata);

	if (py_metadata)
	{
		callback_status = PyObject_CallMethod(self, "__decoder_metadata__", "(O)", py_metadata);
		Py_XDECREF(callback_status);
		Py_DECREF(py_metadata);
	}
}


static void
flac_StreamDecoder_ErrorCallback (const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
	PyObject *py_status;
	PyObject *callback_status = NULL;
	PyObject *self = (PyObject *) client_data;

	py_status = PyFLAC_Enum_FromEnum(status, StreamDecoderErrorStatus);

	if (py_status)
	{
		Py_INCREF(py_status);

		if (PyObject_HasAttrString(self, "__decoder_error__"))
			callback_status = PyObject_CallMethod(self, "__decoder_error__", "(O)", py_status);

		if (callback_status != Py_True && !PyErr_Occurred())
			PyErr_SetObject(PyFLAC_type(FlacFormatError), py_status);

		Py_XDECREF(callback_status);
		Py_DECREF(py_status);
	}
}


static PyObject *
flac_StreamDecoder_set_ogg_serial_number (flac_StreamDecoderObject *self, PyObject *args)
{
	long serial_number;

	if (!PyArg_ParseTuple(args, "l", &serial_number))
		return NULL;

	return PyFLAC_bool(FLAC__stream_decoder_set_ogg_serial_number(self->decoder, serial_number));
}


static PyObject *
flac_StreamDecoder_set_md5_checking (flac_StreamDecoderObject *self, PyObject *arg)
{
	int value;

	value = PyObject_Not(arg);
	if (value < 0)
		return NULL;

	return PyFLAC_bool(FLAC__stream_decoder_set_md5_checking(self->decoder, (FLAC__bool) !value));
}


static PyObject *
flac_StreamDecoder_set_metadata_respond (flac_StreamDecoderObject *self, PyObject *args)
{
	PyObject *type;

	if (!PyArg_ParseTuple(args, "O!", PyFLAC_type(MetadataType), &type))
		return NULL;

	return PyFLAC_bool(FLAC__stream_decoder_set_metadata_respond(self->decoder, PyFLAC_Enum_AsEnum(type, MetadataType)));
}


static PyObject *
flac_StreamDecoder_set_metadata_respond_application (flac_StreamDecoderObject *self, PyObject *args)
{
	FLAC__byte id[4];

	if (!PyArg_ParseTuple(args, "O&", PyFLAC_application_id_conv, &id))
		return NULL;

	return PyFLAC_bool(FLAC__stream_decoder_set_metadata_respond_application(self->decoder, id));
}


static PyObject *
flac_StreamDecoder_set_metadata_respond_all (flac_StreamDecoderObject *self)
{
	return PyFLAC_bool(FLAC__stream_decoder_set_metadata_respond_all(self->decoder));
}


static PyObject *
flac_StreamDecoder_set_metadata_ignore (flac_StreamDecoderObject *self, PyObject *args)
{
	PyObject *type;

	if (!PyArg_ParseTuple(args, "O!", PyFLAC_type(MetadataType), &type))
		return NULL;

	return PyFLAC_bool(FLAC__stream_decoder_set_metadata_ignore(self->decoder, PyFLAC_Enum_AsEnum(type, MetadataType)));
}


static PyObject *
flac_StreamDecoder_set_metadata_ignore_application (flac_StreamDecoderObject *self, PyObject *args)
{
	FLAC__byte id[4];

	if (!PyArg_ParseTuple(args, "O&", PyFLAC_application_id_conv, &id))
		return NULL;

	return PyFLAC_bool(FLAC__stream_decoder_set_metadata_ignore_application(self->decoder, id));
}


static PyObject *
flac_StreamDecoder_set_metadata_ignore_all (flac_StreamDecoderObject *self)
{
	return PyFLAC_bool(FLAC__stream_decoder_set_metadata_ignore_all(self->decoder));
}


static PyObject *
flac_StreamDecoder_get_state (flac_StreamDecoderObject *self)
{
	PyObject *state;

	state = PyFLAC_Enum_FromEnum(FLAC__stream_decoder_get_state(self->decoder), StreamDecoderState);
	Py_XINCREF(state);

	return state;
}


static PyObject *
flac_StreamDecoder_get_resolved_state_string (flac_StreamDecoderObject *self)
{
	return PyFLAC_string(FLAC__stream_decoder_get_resolved_state_string(self->decoder));
}


static PyObject *
flac_StreamDecoder_get_md5_checking (flac_StreamDecoderObject *self)
{
	return PyFLAC_bool(FLAC__stream_decoder_get_md5_checking(self->decoder));
}


static PyObject *
flac_StreamDecoder_get_total_samples (flac_StreamDecoderObject *self)
{
	return PyFLAC_uint64(FLAC__stream_decoder_get_total_samples(self->decoder));
}


static PyObject *
flac_StreamDecoder_get_channels (flac_StreamDecoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_decoder_get_channels(self->decoder));
}


static PyObject *
flac_StreamDecoder_get_channel_assignment (flac_StreamDecoderObject *self)
{
	PyObject *channel_assignment;

	channel_assignment = PyFLAC_Enum_FromEnum(FLAC__stream_decoder_get_channel_assignment(self->decoder), ChannelAssignment);
	Py_XINCREF(channel_assignment);

	return channel_assignment;
}


static PyObject *
flac_StreamDecoder_get_bits_per_sample (flac_StreamDecoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_decoder_get_bits_per_sample(self->decoder));
}


static PyObject *
flac_StreamDecoder_get_sample_rate (flac_StreamDecoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_decoder_get_sample_rate(self->decoder));
}


static PyObject *
flac_StreamDecoder_get_blocksize (flac_StreamDecoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_decoder_get_blocksize(self->decoder));
}


static PyObject *
flac_StreamDecoder_get_decode_position (flac_StreamDecoderObject *self)
{
	FLAC__uint64 position;

	if (FLAC__stream_decoder_get_decode_position (self->decoder, &position))
		return PyFLAC_uint64(position);

	Py_RETURN_NONE;
}


static PyObject *
_flac_StreamDecoder_return_status (FLAC__StreamDecoderInitStatus status)
{
	PyObject *py_status;

	switch (status)
	{
		case FLAC__STREAM_DECODER_INIT_STATUS_MEMORY_ALLOCATION_ERROR:
			return PyErr_NoMemory();
		case FLAC__STREAM_DECODER_INIT_STATUS_ERROR_OPENING_FILE:
			return PyErr_SetFromErrno(PyExc_IOError);
		default:
			py_status = PyFLAC_Enum_FromEnum(status, StreamDecoderInitStatus);
			Py_XINCREF(py_status);
			return py_status;
	}
}


static PyObject *
_flac_StreamDecoder_init_stream (flac_StreamDecoderObject *self, PyObject *args, PyObject *kwds, FLAC__bool is_ogg)
{
	FLAC__StreamDecoderInitStatus status;
	PyObject *stream = NULL;
	FLAC__StreamDecoderSeekCallback seek_callback = NULL;
	FLAC__StreamDecoderTellCallback tell_callback = NULL;
	FLAC__StreamDecoderLengthCallback length_callback = NULL;
	FLAC__StreamDecoderEofCallback eof_callback = NULL;
	FLAC__StreamDecoderMetadataCallback metadata_callback = NULL;

	static char *kwlist[] = {"stream",  NULL};

	stream = (PyObject *) self;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &stream))
		return NULL;

	if (PyObject_HasAttrString((PyObject *) stream, "__decoder_seek__"))
	{
		seek_callback = flac_StreamDecoder_SeekCallback;
		tell_callback = flac_StreamDecoder_TellCallback;
		length_callback = flac_StreamDecoder_LengthCallback;
		eof_callback = flac_StreamDecoder_EofCallback;
	}
	else
	{
		if (PyObject_HasAttrString((PyObject *) stream, "__decoder_tell__"))
			tell_callback = flac_StreamDecoder_TellCallback;
		if (PyObject_HasAttrString((PyObject *) stream, "__decoder_length__"))
			length_callback = flac_StreamDecoder_LengthCallback;
		if (PyObject_HasAttrString((PyObject *) stream, "__decoder_eof__"))
			eof_callback = flac_StreamDecoder_EofCallback;
	}

	if (PyObject_HasAttrString((PyObject *) self, "__decoder_metadata__"))
		metadata_callback = flac_StreamDecoder_MetadataCallback;

	if (is_ogg)
		status = FLAC__stream_decoder_init_ogg_stream(self->decoder, flac_StreamDecoder_ReadCallback, seek_callback, tell_callback, length_callback, eof_callback, flac_StreamDecoder_WriteCallback, metadata_callback, flac_StreamDecoder_ErrorCallback, self);
	else
		status = FLAC__stream_decoder_init_stream(self->decoder, flac_StreamDecoder_ReadCallback, seek_callback, tell_callback, length_callback, eof_callback, flac_StreamDecoder_WriteCallback, metadata_callback, flac_StreamDecoder_ErrorCallback, self);

	if (status == FLAC__STREAM_DECODER_INIT_STATUS_OK && stream != (PyObject *) self)
		self->stream = (Py_INCREF(stream), stream);

	return _flac_StreamDecoder_return_status(status);
}

static PyObject *
flac_StreamDecoder_init_stream (flac_StreamDecoderObject *self, PyObject *args, PyObject *kwds)
{
	return _flac_StreamDecoder_init_stream(self, args, kwds, false);
}

static PyObject *
flac_StreamDecoder_init_ogg_stream (flac_StreamDecoderObject *self, PyObject *args, PyObject *kwds)
{
	return _flac_StreamDecoder_init_stream(self, args, kwds, true);
}


typedef FLAC__StreamDecoderInitStatus(*flac_init_file)(FLAC__StreamDecoder *, const char *, FLAC__StreamDecoderWriteCallback, FLAC__StreamDecoderMetadataCallback, FLAC__StreamDecoderErrorCallback, void *);

static PyObject *
_flac_StreamDecoder_init_file (flac_StreamDecoderObject *self, PyObject *args, flac_init_file flac_init_file_function)
{
	const char *file_name;
	FLAC__StreamDecoderMetadataCallback metadata_callback = NULL;

	if (!PyArg_ParseTuple(args, "s", &file_name))
		return NULL;

	if (PyObject_HasAttrString((PyObject *) self, "__decoder_metadata__"))
		metadata_callback = flac_StreamDecoder_MetadataCallback;

	return _flac_StreamDecoder_return_status(flac_init_file_function(self->decoder, file_name, flac_StreamDecoder_WriteCallback, metadata_callback, flac_StreamDecoder_ErrorCallback, self));
}

static PyObject *
flac_StreamDecoder_init_file (flac_StreamDecoderObject *self, PyObject *args)
{
	return _flac_StreamDecoder_init_file(self, args, FLAC__stream_decoder_init_file);
}

static PyObject *
flac_StreamDecoder_init_ogg_file (flac_StreamDecoderObject *self, PyObject *args)
{
	return _flac_StreamDecoder_init_file(self, args, FLAC__stream_decoder_init_ogg_file);
}


static PyObject *
flac_StreamDecoder_finish (flac_StreamDecoderObject *self)
{
	Py_CLEAR(self->stream);
	return PyFLAC_bool(FLAC__stream_decoder_finish(self->decoder));
}


static PyObject *
flac_StreamDecoder_flush (flac_StreamDecoderObject *self)
{
	if (!FLAC__stream_decoder_flush(self->decoder))
		return PyErr_NoMemory();
	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamDecoder_reset (flac_StreamDecoderObject *self)
{
	if (FLAC__stream_decoder_reset(self->decoder))
		Py_RETURN_TRUE;
	else if (PyErr_Occurred())
		return NULL;
	else if (FLAC__stream_decoder_get_state(self->decoder) == FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR)
		return PyErr_NoMemory();
	else
		Py_RETURN_FALSE;
}


static PyObject *
flac_StreamDecoder_process_single (flac_StreamDecoderObject *self)
{
	FLAC__bool result;

	result = FLAC__stream_decoder_process_single(self->decoder);
	if (PyErr_Occurred())
		return NULL;

	return PyFLAC_bool(result);
}


static PyObject *
flac_StreamDecoder_process_until_end_of_metadata (flac_StreamDecoderObject *self)
{
	FLAC__StreamDecoder *decoder;
	FLAC__bool result = true;

	decoder = self->decoder;

	while (result)
	{
		switch (FLAC__stream_decoder_get_state(decoder))
		{
			case FLAC__STREAM_DECODER_SEARCH_FOR_METADATA:
			case FLAC__STREAM_DECODER_READ_METADATA:
				result = FLAC__stream_decoder_process_single(decoder);
				if (PyErr_Occurred())
					return NULL;
				break;
			case FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC:
			case FLAC__STREAM_DECODER_READ_FRAME:
			case FLAC__STREAM_DECODER_END_OF_STREAM:
			case FLAC__STREAM_DECODER_ABORTED:
				Py_RETURN_TRUE;
			default:
				Py_RETURN_FALSE;
		}
	}

	Py_RETURN_FALSE;
}


static PyObject *
flac_StreamDecoder_process_until_end_of_stream (flac_StreamDecoderObject *self)
{
	FLAC__StreamDecoder *decoder;
	FLAC__bool result = true;

	decoder = self->decoder;

	while (result)
	{
		switch (FLAC__stream_decoder_get_state(decoder))
		{
			case FLAC__STREAM_DECODER_SEARCH_FOR_METADATA:
			case FLAC__STREAM_DECODER_READ_METADATA:
			case FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC:
			case FLAC__STREAM_DECODER_READ_FRAME:
				result = FLAC__stream_decoder_process_single(decoder);
				if (PyErr_Occurred())
					return NULL;
				break;
			case FLAC__STREAM_DECODER_END_OF_STREAM:
			case FLAC__STREAM_DECODER_ABORTED:
				Py_RETURN_TRUE;
			default:
				Py_RETURN_FALSE;
		}
	}

	Py_RETURN_FALSE;
}


static PyObject *
flac_StreamDecoder_skip_single_frame (flac_StreamDecoderObject *self)
{
	if (FLAC__stream_decoder_skip_single_frame(self->decoder))
		Py_RETURN_TRUE;
	else if (PyErr_Occurred())
		return NULL;
	else
		Py_RETURN_FALSE;
}


static PyObject *
flac_StreamDecoder_seek_absolute (flac_StreamDecoderObject *self, PyObject *args)
{
	FLAC__uint64 sample;

	if (!PyArg_ParseTuple(args, "O&", PyFLAC_uint64_conv, &sample))
		return NULL;

	if (FLAC__stream_decoder_seek_absolute(self->decoder, sample))
		Py_RETURN_TRUE;
	else if (PyErr_Occurred())
		return NULL;
	else
		Py_RETURN_FALSE;
}


static PyMethodDef flac_StreamDecoder_methods[] = {
	{
		"set_ogg_serial_number",
		(PyCFunction) flac_StreamDecoder_set_ogg_serial_number,
		METH_VARARGS,
		"Set ogg serial number"
	}, {
		"set_md5_checking",
		(PyCFunction) flac_StreamDecoder_set_md5_checking,
		METH_O,
		"Set md5 checking"
	}, {
		"set_metadata_respond",
		(PyCFunction) flac_StreamDecoder_set_metadata_respond,
		METH_VARARGS,
		"Set metadata respond"
	}, {
		"set_metadata_respond_application",
		(PyCFunction) flac_StreamDecoder_set_metadata_respond_application,
		METH_VARARGS,
		"Set metadata respond application"
	}, {
		"set_metadata_respond_all",
		(PyCFunction) flac_StreamDecoder_set_metadata_respond_all,
		METH_NOARGS,
		"Set metadata respond all"
	}, {
		"set_metadata_ignore",
		(PyCFunction) flac_StreamDecoder_set_metadata_ignore,
		METH_VARARGS,
		"Set metadata ignore"
	}, {
		"set_metadata_ignore_application",
		(PyCFunction) flac_StreamDecoder_set_metadata_ignore_application,
		METH_VARARGS,
		"Set metadata ignore application"
	}, {
		"set_metadata_ignore_all",
		(PyCFunction) flac_StreamDecoder_set_metadata_ignore_all,
		METH_NOARGS,
		"Set metadata ignore all"
	}, {
		"get_state",
		(PyCFunction) flac_StreamDecoder_get_state,
		METH_NOARGS,
		"Get state"
	}, {
		"get_resolved_state_string",
		(PyCFunction) flac_StreamDecoder_get_resolved_state_string,
		METH_NOARGS,
		"Get resolved state string"
	}, {
		"get_md5_checking",
		(PyCFunction) flac_StreamDecoder_get_md5_checking,
		METH_NOARGS,
		"Get md5 checking"
	}, {
		"get_total_samples",
		(PyCFunction) flac_StreamDecoder_get_total_samples,
		METH_NOARGS,
		"Get total samples"
	}, {
		"get_channels",
		(PyCFunction) flac_StreamDecoder_get_channels,
		METH_NOARGS,
		"Get channels"
	}, {
		"get_channel_assignment",
		(PyCFunction) flac_StreamDecoder_get_channel_assignment,
		METH_NOARGS,
		"Get channel assignment"
	}, {
		"get_bits_per_sample",
		(PyCFunction) flac_StreamDecoder_get_bits_per_sample,
		METH_NOARGS,
		"Get bits per sample"
	}, {
		"get_sample_rate",
		(PyCFunction) flac_StreamDecoder_get_sample_rate,
		METH_NOARGS,
		"Get sample rate"
	}, {
		"get_blocksize",
		(PyCFunction) flac_StreamDecoder_get_blocksize,
		METH_NOARGS,
		"Get blocksize"
	}, {
		"get_decode_position",
		(PyCFunction) flac_StreamDecoder_get_decode_position,
		METH_NOARGS,
		"Get decode position"
	}, {
		"init_stream",
		(PyCFunction) flac_StreamDecoder_init_stream,
		METH_VARARGS | METH_KEYWORDS,
		"Init stream"
	}, {
		"init_ogg_stream",
		(PyCFunction) flac_StreamDecoder_init_ogg_stream,
		METH_VARARGS | METH_KEYWORDS,
		"Init ogg stream"
	}, {
		"init_file",
		(PyCFunction) flac_StreamDecoder_init_file,
		METH_VARARGS,
		"Init file"
	}, {
		"init_ogg_file",
		(PyCFunction) flac_StreamDecoder_init_ogg_file,
		METH_VARARGS,
		"Init ogg file"
	}, {
		"finish",
		(PyCFunction) flac_StreamDecoder_finish,
		METH_NOARGS,
		"Finish"
	}, {
		"flush",
		(PyCFunction) flac_StreamDecoder_flush,
		METH_NOARGS,
		"Flush"
	}, {
		"reset",
		(PyCFunction) flac_StreamDecoder_reset,
		METH_NOARGS,
		"Reset"
	}, {
		"process_single",
		(PyCFunction) flac_StreamDecoder_process_single,
		METH_NOARGS,
		"Process single"
	}, {
		"process_until_end_of_metadata",
		(PyCFunction) flac_StreamDecoder_process_until_end_of_metadata,
		METH_NOARGS,
		"Process until end of metadata"
	}, {
		"process_until_end_of_stream",
		(PyCFunction) flac_StreamDecoder_process_until_end_of_stream,
		METH_NOARGS,
		"Process until end of stream"
	}, {
		"skip_single_frame",
		(PyCFunction) flac_StreamDecoder_skip_single_frame,
		METH_NOARGS,
		"Skip single frame"
	}, {
		"seek_absolute",
		(PyCFunction) flac_StreamDecoder_seek_absolute,
		METH_VARARGS,
		"Seek absolute"
	}, { NULL }		/* Sentinel */
};


static PyTypeObject PyFLAC_StreamDecoderType = {
	PyVarObject_HEAD_INIT(NULL,0)
	"flac.StreamDecoder",							/* tp_name */
	sizeof(flac_StreamDecoderObject),				/* tp_basicsize */
	0,												/* tp_itemsize */
	(destructor) flac_StreamDecoder_dealloc,		/* tp_dealloc */
	0,												/* tp_print */
	0,												/* tp_getattr */
	0,												/* tp_setattr */
	0,												/* tp_compare */
	0,												/* tp_repr */
	0,												/* tp_as_number */
	0,												/* tp_as_sequence */
	0,												/* tp_as_mapping */
	0,												/* tp_hash */
	0,												/* tp_call */
	0,												/* tp_str */
	0,												/* tp_getattro */
	0,												/* tp_setattro */
	0,												/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE, /* tp_flags */
	"FLAC StreamDecoder",							/* tp_doc */
	(traverseproc) flac_StreamDecoder_traverse,		/* tp_traverse */
	(inquiry)  flac_StreamDecoder_clear,			/* tp_clear */
	0,												/* tp_richcompare */
	0,												/* tp_weaklistoffset */
	0,												/* tp_iter */
	0,												/* tp_iternext */
	flac_StreamDecoder_methods,						/* tp_methods */
	0,												/* tp_members */
	0,												/* tp_getset */
	0,												/* tp_base */
	0,												/* tp_dict */
	0,												/* tp_descr_get */
	0,												/* tp_descr_set */
	0,												/* tp_dictoffset */
	0,												/* tp_init */
	0,												/* tp_alloc */
	flac_StreamDecoder_new,							/* tp_new */
};


static PyMethodDef flac_decoder_functions[] = {
	{ NULL }		/* Sentinel */
};


PyFLAC_Enum_FromEnum_function(StreamDecoderState, enum_member_StreamDecoderState)
PyFLAC_Enum_FromEnum_function(StreamDecoderInitStatus, enum_member_StreamDecoderInitStatus)
PyFLAC_Enum_FromEnum_function(StreamDecoderReadStatus, enum_member_StreamDecoderReadStatus)
PyFLAC_Enum_FromEnum_function(StreamDecoderSeekStatus, enum_member_StreamDecoderSeekStatus)
PyFLAC_Enum_FromEnum_function(StreamDecoderTellStatus, enum_member_StreamDecoderTellStatus)
PyFLAC_Enum_FromEnum_function(StreamDecoderLengthStatus, enum_member_StreamDecoderLengthStatus)
PyFLAC_Enum_FromEnum_function(StreamDecoderWriteStatus, enum_member_StreamDecoderWriteStatus)
PyFLAC_Enum_FromEnum_function(StreamDecoderErrorStatus, enum_member_StreamDecoderErrorStatus)


static int
flac_decoder_init (void)
{
	PyFLAC_CHECK_status(PyFLAC_import_format());

	PyFLAC_CHECK_status(PyType_Ready(PyFLAC_type(StreamDecoder)));

	PyFLAC_CHECK_status(PyType_Ready(&flac_FrameType));
	PyFLAC_CHECK_status(PyType_Ready(&flac_SubframesType));

	flac_BufferType.tp_base = &PyBytes_Type;
	PyFLAC_CHECK_status(PyType_Ready(&flac_BufferType));

	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamDecoderState), enum_member_StreamDecoderState));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamDecoderInitStatus), enum_member_StreamDecoderInitStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamDecoderReadStatus), enum_member_StreamDecoderReadStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamDecoderSeekStatus), enum_member_StreamDecoderSeekStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamDecoderTellStatus), enum_member_StreamDecoderTellStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamDecoderLengthStatus), enum_member_StreamDecoderLengthStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamDecoderWriteStatus), enum_member_StreamDecoderWriteStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamDecoderErrorStatus), enum_member_StreamDecoderErrorStatus));

	_c_api_init

	return 0;
}


static void
flac_decoder_build (PyObject *module)
{
	Py_INCREF(PyFLAC_type(StreamDecoder));
	PyFLAC_PyModule_AddType(module, StreamDecoder);

	Py_INCREF(PyFLAC_type(StreamDecoderState));
	PyFLAC_PyModule_AddType(module, StreamDecoderState);

	Py_INCREF(PyFLAC_type(StreamDecoderInitStatus));
	PyFLAC_PyModule_AddType(module, StreamDecoderInitStatus);

	Py_INCREF(PyFLAC_type(StreamDecoderReadStatus));
	PyFLAC_PyModule_AddType(module, StreamDecoderReadStatus);

	Py_INCREF(PyFLAC_type(StreamDecoderSeekStatus));
	PyFLAC_PyModule_AddType(module, StreamDecoderSeekStatus);

	Py_INCREF(PyFLAC_type(StreamDecoderTellStatus));
	PyFLAC_PyModule_AddType(module, StreamDecoderTellStatus);

	Py_INCREF(PyFLAC_type(StreamDecoderLengthStatus));
	PyFLAC_PyModule_AddType(module, StreamDecoderLengthStatus);

	Py_INCREF(PyFLAC_type(StreamDecoderWriteStatus));
	PyFLAC_PyModule_AddType(module, StreamDecoderWriteStatus);

	Py_INCREF(PyFLAC_type(StreamDecoderErrorStatus));
	PyFLAC_PyModule_AddType(module, StreamDecoderErrorStatus);

	Py_INCREF(PyFLAC_type(MetadataType));
	PyFLAC_PyModule_AddType(module, MetadataType);

	Py_INCREF(PyFLAC_type(ChannelAssignment));
	PyFLAC_PyModule_AddType(module, ChannelAssignment);

	Py_INCREF(PyFLAC_type(FlacFormatError));
	PyFLAC_PyModule_AddType(module, FlacFormatError);

	_c_api_build(module);
}


PyFLAC_MODINIT(
	decoder,
	flac_decoder_init,
	flac_decoder_build,
	flac_decoder_functions,
	"libFLAC stream_decoder"
)

