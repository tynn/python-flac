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

#define __PyFLAC_encoder_MODULE__
#include "encoder.h"

#include "decoder.h"
#include "metadata.h"

#define _bool PyFLAC_bool_conv
#define _unsigned PyFLAC_unsigned_conv


PyFLAC_Enum(StreamEncoderState)
PyFLAC_Enum(StreamEncoderInitStatus)
PyFLAC_Enum(StreamEncoderReadStatus)
PyFLAC_Enum(StreamEncoderSeekStatus)
PyFLAC_Enum(StreamEncoderTellStatus)
PyFLAC_Enum(StreamEncoderWriteStatus)


static PyFLAC_Enum_Member_Def enum_member_StreamEncoderState[] = {
	PyFLAC_Enum_Member(
		"OK",
		STREAM_ENCODER_OK
	),
	PyFLAC_Enum_Member(
		"UNINITIALIZED",
		STREAM_ENCODER_UNINITIALIZED
	),
	PyFLAC_Enum_Member(
		"OGG_ERROR",
		STREAM_ENCODER_OGG_ERROR
	),
	PyFLAC_Enum_Member(
		"VERIFY_DECODER_ERROR",
		STREAM_ENCODER_VERIFY_DECODER_ERROR
	),
	PyFLAC_Enum_Member(
		"VERIFY_MISMATCH_IN_AUDIO_DATA",
		STREAM_ENCODER_VERIFY_MISMATCH_IN_AUDIO_DATA
	),
	PyFLAC_Enum_Member(
		"CLIENT_ERROR",
		STREAM_ENCODER_CLIENT_ERROR
	),
	PyFLAC_Enum_Member(
		"IO_ERROR",
		STREAM_ENCODER_IO_ERROR
	),
	PyFLAC_Enum_Member(
		"FRAMING_ERROR",
		STREAM_ENCODER_FRAMING_ERROR
	),
	PyFLAC_Enum_Member(
		"MEMORY_ALLOCATION_ERROR",
		STREAM_ENCODER_MEMORY_ALLOCATION_ERROR
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamEncoderInitStatus[] = {
	PyFLAC_Enum_Member(
		"OK",
		STREAM_ENCODER_INIT_STATUS_OK
	),
	PyFLAC_Enum_Member(
		"ENCODER_ERROR",
		STREAM_ENCODER_INIT_STATUS_ENCODER_ERROR
	),
	PyFLAC_Enum_Member(
		"UNSUPPORTED_CONTAINER",
		STREAM_ENCODER_INIT_STATUS_UNSUPPORTED_CONTAINER
	),
	PyFLAC_Enum_Member(
		"INVALID_CALLBACKS",
		STREAM_ENCODER_INIT_STATUS_INVALID_CALLBACKS
	),
	PyFLAC_Enum_Member(
		"INVALID_NUMBER_OF_CHANNELS",
		STREAM_ENCODER_INIT_STATUS_INVALID_NUMBER_OF_CHANNELS
	),
	PyFLAC_Enum_Member(
		"INVALID_BITS_PER_SAMPLE",
		STREAM_ENCODER_INIT_STATUS_INVALID_BITS_PER_SAMPLE
	),
	PyFLAC_Enum_Member(
		"INVALID_SAMPLE_RATE",
		STREAM_ENCODER_INIT_STATUS_INVALID_SAMPLE_RATE
	),
	PyFLAC_Enum_Member(
		"INVALID_BLOCK_SIZE",
		STREAM_ENCODER_INIT_STATUS_INVALID_BLOCK_SIZE
	),
	PyFLAC_Enum_Member(
		"INVALID_MAX_LPC_ORDER",
		STREAM_ENCODER_INIT_STATUS_INVALID_MAX_LPC_ORDER
	),
	PyFLAC_Enum_Member(
		"INVALID_QLP_COEFF_PRECISION",
		STREAM_ENCODER_INIT_STATUS_INVALID_QLP_COEFF_PRECISION
	),
	PyFLAC_Enum_Member(
		"BLOCK_SIZE_TOO_SMALL_FOR_LPC_ORDER",
		STREAM_ENCODER_INIT_STATUS_BLOCK_SIZE_TOO_SMALL_FOR_LPC_ORDER
	),
	PyFLAC_Enum_Member(
		"NOT_STREAMABLE",
		STREAM_ENCODER_INIT_STATUS_NOT_STREAMABLE
	),
	PyFLAC_Enum_Member(
		"INVALID_METADATA",
		STREAM_ENCODER_INIT_STATUS_INVALID_METADATA
	),
	PyFLAC_Enum_Member(
		"ALREADY_INITIALIZED",
		STREAM_ENCODER_INIT_STATUS_ALREADY_INITIALIZED
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamEncoderReadStatus[] = {
	PyFLAC_Enum_Member(
		"CONTINUE",
		STREAM_ENCODER_READ_STATUS_CONTINUE
	),
	PyFLAC_Enum_Member(
		"END_OF_STREAM",
		STREAM_ENCODER_READ_STATUS_END_OF_STREAM
	),
	PyFLAC_Enum_Member(
		"ABORT",
		STREAM_ENCODER_READ_STATUS_ABORT
	),
	PyFLAC_Enum_Member(
		"UNSUPPORTED",
		STREAM_ENCODER_READ_STATUS_UNSUPPORTED
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamEncoderSeekStatus[] = {
	PyFLAC_Enum_Member(
		"OK",
		STREAM_ENCODER_SEEK_STATUS_OK
	),
	PyFLAC_Enum_Member(
		"ERROR",
		STREAM_ENCODER_SEEK_STATUS_ERROR
	),
	PyFLAC_Enum_Member(
		"UNSUPPORTED",
		STREAM_ENCODER_SEEK_STATUS_UNSUPPORTED
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamEncoderTellStatus[] = {
	PyFLAC_Enum_Member(
		"OK",
		STREAM_ENCODER_TELL_STATUS_OK
	),
	PyFLAC_Enum_Member(
		"ERROR",
		STREAM_ENCODER_TELL_STATUS_ERROR
	),
	PyFLAC_Enum_Member(
		"UNSUPPORTED",
		STREAM_ENCODER_TELL_STATUS_UNSUPPORTED
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_StreamEncoderWriteStatus[] = {
	PyFLAC_Enum_Member(
		"OK",
		STREAM_ENCODER_WRITE_STATUS_OK
	),
	PyFLAC_Enum_Member(
		"FATAL_ERROR",
		STREAM_ENCODER_WRITE_STATUS_FATAL_ERROR
	),
	{ NULL }		/* Sentinel */
};


struct flac_VerifyDecoderErrorStatsObject {
	PyObject_HEAD
	FLAC__uint64 absolute_sample;
	unsigned frame_number, channel, sample;
	FLAC__int32 expected, got;
};


static PyMemberDef flac_VerifyDecoderErrorStats_members[] = {
	{
		"frame_number", T_UINT,
		offsetof(struct flac_VerifyDecoderErrorStatsObject, frame_number),
		READONLY, "VerifyDecoderErrorStats frame_number"
	}, {
		"channel", T_UINT,
		offsetof(struct flac_VerifyDecoderErrorStatsObject, channel),
		READONLY, "VerifyDecoderErrorStats channel"
	}, {
		"sample", T_UINT,
		offsetof(struct flac_VerifyDecoderErrorStatsObject, sample),
		READONLY, "VerifyDecoderErrorStats sample"
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_VerifyDecoderErrorStats_get (struct flac_VerifyDecoderErrorStatsObject *self, void *closure)
{
	if (closure == (void *) 2)
		return PyFLAC_uint64(self->absolute_sample);

	return PyFLAC_int32(closure ? self->expected : self->got);
}


static PyGetSetDef flac_VerifyDecoderErrorStats_getseters[] = {
	{
		"absolute_sample",
		(getter) flac_VerifyDecoderErrorStats_get, NULL,
		"VerifyDecoderErrorStats absolute_sample", (void *) 2
	}, {
		"expected",
		(getter) flac_VerifyDecoderErrorStats_get, NULL,
		"VerifyDecoderErrorStats expected", (void *) 1
	}, {
		"got",
		(getter) flac_VerifyDecoderErrorStats_get, NULL,
		"VerifyDecoderErrorStats got", (void *) 0
	}, { NULL }		/* Sentinel */
};


static PyTypeObject flac_VerifyDecoderErrorStatsType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(VerifyDecoderErrorStats),
	sizeof(struct flac_VerifyDecoderErrorStatsObject), 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	Py_TPFLAGS_DEFAULT, "FLAC VerifyDecoderErrorStats",
	0, 0, 0, 0, 0, 0, 0,
	flac_VerifyDecoderErrorStats_members, flac_VerifyDecoderErrorStats_getseters, 0, 0,
};


static void
flac_StreamEncoder_clear_metadata (flac_StreamEncoderObject *self)
{
	if (self->metadata_count)
		PyFLAC_free_list_of_type(self->metadata, self->metadata_count);
	else if (self->metadata)
		Py_XDECREF(*self->metadata);
	self->metadata = NULL;
	self->metadata_count = 0;
}


static void
flac_StreamEncoder_dealloc (flac_StreamEncoderObject *self)
{
	PyObject_GC_UnTrack(self);
	if (self->encoder)
		FLAC__stream_encoder_delete(self->encoder);
	Py_XDECREF(self->stream);
	flac_StreamEncoder_clear_metadata(self);
	Py_TYPE(self)->tp_free((PyObject *) self);
}


static int
flac_StreamEncoder_traverse (flac_StreamEncoderObject *self, visitproc visit, void *arg)
{
	Py_VISIT(self->stream);
	return 0;
}


static int
flac_StreamEncoder_clear (flac_StreamEncoderObject *self)
{
	Py_CLEAR(self->stream);
	return 0;
}


static PyObject *
flac_StreamEncoder_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	flac_StreamEncoderObject *self;

	(void)args, (void)kwds;

	self = (flac_StreamEncoderObject *) type->tp_alloc(type, 0);

	if (self)
	{
		self->stream = NULL;
		self->metadata = NULL;
		self->metadata_count = 0;

		self->encoder = FLAC__stream_encoder_new();

		if (!self->encoder)
		{
			Py_DECREF(self);
			return PyErr_NoMemory();
		}
	}

	return (PyObject *) self;
}


static FLAC__StreamEncoderReadStatus
flac_StreamEncoder_ReadCallback (const FLAC__StreamEncoder *encoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
{
	FLAC__StreamEncoderReadStatus status;
	PyObject *py_bytes;
	PyObject *callback_status;
	Py_ssize_t length;
	char *py_buffer;
	PyObject *self = (PyObject *) client_data;

	py_bytes = PyLong_FromSize_t(*bytes);
	if (!py_bytes)
		return FLAC__STREAM_ENCODER_READ_STATUS_ABORT;

	callback_status = PyObject_CallMethod(self, "__encoder_read__", "(O)", py_bytes);
	Py_DECREF(py_bytes);

	if (!callback_status)
		return FLAC__STREAM_ENCODER_READ_STATUS_ABORT;

	if (PyObject_Not(callback_status))
	{
		status = FLAC__STREAM_ENCODER_READ_STATUS_END_OF_STREAM;
	}
	else if (PyFLAC_type_Check(callback_status, StreamEncoderReadStatus))
	{
		status = PyFLAC_Enum_AsEnum(callback_status, StreamEncoderReadStatus);
		if (status == FLAC__STREAM_ENCODER_READ_STATUS_CONTINUE)
		{
			status = FLAC__STREAM_ENCODER_READ_STATUS_ABORT;
			PyErr_Format(PyExc_ValueError, "__encoder_read__ must not return %s.CONTINUE", Py_TYPE(callback_status)->tp_name);
		}
	}
	else if (PyBytes_AsStringAndSize(callback_status, &py_buffer, &length) < 0 || length > *bytes)
	{
		status = FLAC__STREAM_ENCODER_READ_STATUS_ABORT;
		PyErr_Format(PyExc_ValueError, "__encoder_read__ must return %s or %s, not %s", PyBytes_Type.tp_name, (PyFLAC_type(StreamEncoderReadStatus))->tp_name, Py_TYPE(callback_status)->tp_name);
	}
	else
	{
		*bytes = (size_t) length;
		Py_MEMCPY(buffer, py_buffer, *bytes);
		status = FLAC__STREAM_ENCODER_READ_STATUS_CONTINUE;
	}

	Py_DECREF(callback_status);
	return status;
}


static FLAC__StreamEncoderSeekStatus
flac_StreamEncoder_SeekCallback (const FLAC__StreamEncoder *encoder, FLAC__uint64 absolute_byte_offset, void *client_data)
{
	FLAC__StreamEncoderSeekStatus status;
	PyObject *py_offset;
	PyObject *callback_status;
	PyObject *self = (PyObject *) client_data;

	py_offset = PyFLAC_uint64(absolute_byte_offset);

	if (!py_offset)
		return FLAC__STREAM_ENCODER_SEEK_STATUS_ERROR;

	callback_status = PyObject_CallMethod(self, "__encoder_seek__", "(O)", py_offset);
	Py_DECREF(py_offset);

	if (!callback_status)
		return FLAC__STREAM_ENCODER_SEEK_STATUS_ERROR;

	if (PyFLAC_type_Check(callback_status, StreamEncoderSeekStatus))
	{
		status = PyFLAC_Enum_AsEnum(callback_status, StreamEncoderSeekStatus);
	}
	else if (callback_status == Py_None || callback_status == Py_False)
	{
		status = FLAC__STREAM_ENCODER_SEEK_STATUS_UNSUPPORTED;
	}
	else if (callback_status == Py_True)
	{
		status = FLAC__STREAM_ENCODER_SEEK_STATUS_OK;
	}
	else
	{
		status = FLAC__STREAM_ENCODER_SEEK_STATUS_ERROR;
		PyErr_Format(PyExc_TypeError, "__encoder_seek__ must return %s, bool or None, not %s", (PyFLAC_type(StreamEncoderSeekStatus))->tp_name, Py_TYPE(callback_status)->tp_name);
	}

	Py_DECREF(callback_status);
	return status;
}


static FLAC__StreamEncoderTellStatus
flac_StreamEncoder_TellCallback (const FLAC__StreamEncoder *encoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	FLAC__StreamEncoderTellStatus status;
	PyObject *callback_status;
	PyObject *self = (PyObject *) client_data;

	callback_status = PyObject_CallMethod(self, "__encoder_tell__", NULL);

	if (!callback_status)
		return FLAC__STREAM_ENCODER_TELL_STATUS_ERROR;

	if (PyFLAC_uint64_conv(callback_status, absolute_byte_offset) >= 0)
	{
		status = FLAC__STREAM_ENCODER_TELL_STATUS_OK;
	}
	else if (callback_status == Py_None || callback_status == Py_False)
	{
		status = FLAC__STREAM_ENCODER_TELL_STATUS_UNSUPPORTED;
	}
	else if (callback_status == Py_True)
	{
		status = FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
		PyErr_SetString(PyExc_ValueError, "__encoder_tell__ must not return True");
	}
	else if (!PyFLAC_type_Check(callback_status, StreamEncoderTellStatus))
	{
		status = FLAC__STREAM_ENCODER_TELL_STATUS_ERROR;
		PyErr_Format(PyExc_TypeError, "__encoder_tell__ must return %s, int, bool or None, not %s", (PyFLAC_type(StreamEncoderTellStatus))->tp_name, Py_TYPE(callback_status)->tp_name);
	}
	else if ((status = PyFLAC_Enum_AsEnum(callback_status, StreamEncoderTellStatus)) == FLAC__STREAM_ENCODER_TELL_STATUS_OK)
	{
		status = FLAC__STREAM_ENCODER_TELL_STATUS_ERROR;
		PyErr_Format(PyExc_ValueError, "__encoder_tell__ must not return %s.OK", Py_TYPE(callback_status)->tp_name);
	}

	Py_DECREF(callback_status);
	return status;
}


static void
flac_StreamEncoder_MetadataCallback (const FLAC__StreamEncoder *encoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	PyObject *py_metadata;
	PyObject *callback_status;
	PyObject *self = (PyObject *) client_data;

	if (PyFLAC_import_metadata() < 0)
		return;

	py_metadata = PyFLAC_StreamMetadata_FromClass(metadata);

	if (py_metadata)
	{
		callback_status = PyObject_CallMethod(self, "__encoder_metadata__", "(O)", py_metadata);
		Py_XDECREF(callback_status);
		Py_DECREF(py_metadata);
	}
}


static FLAC__StreamEncoderWriteStatus
flac_StreamEncoder_WriteCallback (const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame, void *client_data)
{
	FLAC__StreamEncoderWriteStatus status;
	PyObject *py_buffer;
	PyObject *py_samples;
	PyObject *py_current_frame;
	PyObject *callback_status;
	PyObject *self = (PyObject *) client_data;

	py_buffer = PyFLAC_data(buffer, bytes);
	py_samples = PyFLAC_unsigned(samples);

	if (samples)
		py_current_frame = PyFLAC_unsigned(current_frame);
	else
		py_current_frame = (Py_INCREF(Py_None), Py_None);

	status = FLAC__STREAM_ENCODER_WRITE_STATUS_FATAL_ERROR;
	if (py_buffer && py_samples && py_current_frame)
	{
		callback_status = PyObject_CallMethod(self, "__encoder_write__", "(OOO)", py_buffer, py_samples, py_current_frame);

		if (callback_status)
		{
			if (callback_status == Py_None || callback_status == Py_True)
				status = FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
			else if (PyFLAC_type_Check(callback_status, StreamEncoderWriteStatus))
				status = PyFLAC_Enum_AsEnum(callback_status, StreamEncoderWriteStatus);
			else if (PyObject_IsTrue(callback_status) > 0)
				status = FLAC__STREAM_ENCODER_WRITE_STATUS_OK;

			Py_DECREF(callback_status);
		}
	}

	Py_XDECREF(py_buffer);
	Py_XDECREF(py_samples);
	Py_XDECREF(py_current_frame);
	return status;
}


static void
flac_StreamEncoderProgressCallback (const FLAC__StreamEncoder *encoder, FLAC__uint64 bytes_written, FLAC__uint64 samples_written, unsigned frames_written, unsigned total_frames_estimate, void *client_data)
{
	PyObject *info[4];
	PyObject *callback_status;
	PyObject *self = (PyObject *) client_data;

	info[0] = PyFLAC_uint64(bytes_written);
	info[1] = PyFLAC_uint64(samples_written);
	info[2] = PyFLAC_unsigned(frames_written);
	info[3] = PyFLAC_unsigned(total_frames_estimate);

	if (info[0] && info[1] && info[2] && info[3])
	{
		callback_status = PyObject_CallMethod(self, "__encoder_progress__", "(OOOO)", info[0], info[1], info[2], info[3]);
		Py_XDECREF(callback_status);
	}

	Py_XDECREF(info[0]);
	Py_XDECREF(info[1]);
	Py_XDECREF(info[2]);
	Py_XDECREF(info[3]);
}


static PyObject *
flac_StreamEncoder_set_ogg_serial_number (flac_StreamEncoderObject *self, PyObject *args)
{
	long serial_number;

	if (!PyArg_ParseTuple(args, "l", &serial_number))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_ogg_serial_number(self->encoder, serial_number));
}


static PyObject *
flac_StreamEncoder_set_verify (flac_StreamEncoderObject *self, PyObject *args)
{
	FLAC__bool verify;

	if (!PyArg_ParseTuple(args, "O&", _bool, &verify))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_verify(self->encoder, verify));
}


static PyObject *
flac_StreamEncoder_set_streamable_subset (flac_StreamEncoderObject *self, PyObject *args)
{
	FLAC__bool streamable_subset;

	if (!PyArg_ParseTuple(args, "O&", _bool, &streamable_subset))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_streamable_subset(self->encoder, streamable_subset));
}


static PyObject *
flac_StreamEncoder_set_channels (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned channels;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &channels))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_channels(self->encoder, channels));
}


static PyObject *
flac_StreamEncoder_set_bits_per_sample (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned bits_per_sample;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &bits_per_sample))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_bits_per_sample(self->encoder, bits_per_sample));
}


static PyObject *
flac_StreamEncoder_set_sample_rate (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned sample_rate;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &sample_rate))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_sample_rate(self->encoder, sample_rate));
}


static PyObject *
flac_StreamEncoder_set_compression_level (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned compression_level;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &compression_level)) /* 0-8 */
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_compression_level(self->encoder, compression_level));
}


static PyObject *
flac_StreamEncoder_set_blocksize (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned blocksize;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &blocksize))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_blocksize(self->encoder, blocksize));
}


static PyObject *
flac_StreamEncoder_set_do_mid_side_stereo (flac_StreamEncoderObject *self, PyObject *args)
{
	FLAC__bool do_mid_side_stereo;

	if (!PyArg_ParseTuple(args, "O&", _bool, &do_mid_side_stereo))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_do_mid_side_stereo(self->encoder, do_mid_side_stereo));
}


static PyObject *
flac_StreamEncoder_set_loose_mid_side_stereo (flac_StreamEncoderObject *self, PyObject *args)
{
	FLAC__bool loose_mid_side_stereo;

	if (!PyArg_ParseTuple(args, "O&", _bool, &loose_mid_side_stereo))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_loose_mid_side_stereo(self->encoder, loose_mid_side_stereo));
}


static PyObject *
flac_StreamEncoder_set_apodization (flac_StreamEncoderObject *self, PyObject *args)
{
	const char *specification;

	if (!PyArg_ParseTuple(args, "s", &specification))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_apodization(self->encoder, specification));
}


static PyObject *
flac_StreamEncoder_set_max_lpc_order (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned max_lpc_order;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &max_lpc_order))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_max_lpc_order(self->encoder, max_lpc_order));
}


static PyObject *
flac_StreamEncoder_set_qlp_coeff_precision (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned qlp_coeff_precision;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &qlp_coeff_precision))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_qlp_coeff_precision(self->encoder, qlp_coeff_precision));
}


static PyObject *
flac_StreamEncoder_set_do_qlp_coeff_prec_search (flac_StreamEncoderObject *self, PyObject *args)
{
	FLAC__bool do_qlp_coeff_prec_search;

	if (!PyArg_ParseTuple(args, "O&", _bool, &do_qlp_coeff_prec_search))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_do_qlp_coeff_prec_search(self->encoder, do_qlp_coeff_prec_search));
}


static PyObject *
flac_StreamEncoder_set_do_exhaustive_model_search (flac_StreamEncoderObject *self, PyObject *args)
{
	FLAC__bool do_exhaustive_model_search;

	if (!PyArg_ParseTuple(args, "O&", _bool, &do_exhaustive_model_search))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_do_exhaustive_model_search(self->encoder, do_exhaustive_model_search));
}


static PyObject *
flac_StreamEncoder_set_min_residual_partition_order (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned min_residual_partition_order;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &min_residual_partition_order))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_min_residual_partition_order(self->encoder, min_residual_partition_order));
}


static PyObject *
flac_StreamEncoder_set_max_residual_partition_order (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned max_residual_partition_order;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &max_residual_partition_order))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_max_residual_partition_order(self->encoder, max_residual_partition_order));
}


static PyObject *
flac_StreamEncoder_set_total_samples_estimate (flac_StreamEncoderObject *self, PyObject *args)
{
	FLAC__uint64 total_samples_estimate;

	if (!PyArg_ParseTuple(args, "O&", PyFLAC_uint64_conv, &total_samples_estimate))
		return NULL;

	return PyFLAC_bool(FLAC__stream_encoder_set_total_samples_estimate(self->encoder, total_samples_estimate));
}


static PyObject *
flac_StreamEncoder_set_metadata (flac_StreamEncoderObject *self, PyObject *args)
{
	FLAC__bool result;
	PyObject *py_metadata;
	Py_ssize_t metadata_count, i;
	FLAC__StreamMetadata **metadata = NULL;
	PyObject **metadata_list = NULL;

	if (PyFLAC_import_metadata() < 0)
		return NULL;

	if (!PyArg_ParseTuple(args, "O", &py_metadata))
	{
		PyErr_Clear();
		py_metadata = args;
	}

	if (py_metadata == Py_None)
	{
		if (!FLAC__stream_encoder_set_metadata(self->encoder, NULL, 0));
			Py_RETURN_FALSE;

		flac_StreamEncoder_clear_metadata(self);
		Py_RETURN_TRUE;
	}

	if (!PyFLAC_list_of_type(py_metadata, PyFLAC_type(StreamMetadata), &metadata_list, &metadata_count))
		return NULL;

	if (metadata_count > 65535)
	{
		PyErr_SetString(PyExc_ValueError, "number of metadata blocks > 65535");
	}
	else
	{
		metadata = PyMem_New(FLAC__StreamMetadata *, metadata_count);

		if (!metadata)
			PyErr_NoMemory();
		else if (metadata_list == NULL && metadata_count == 1)
			metadata[0] = ((PyFLAC_StreamMetadataObject *) py_metadata)->metadata;
		else for (i = 0; i < metadata_count; i++)
			metadata[i] = ((PyFLAC_StreamMetadataObject *) metadata_list[i])->metadata;
	}

	if (!metadata)
	{
		PyFLAC_free_list_of_type(metadata_list, metadata_count);
		return NULL;
	}

	result = FLAC__stream_encoder_set_metadata(self->encoder, metadata, (unsigned) metadata_count);
	PyMem_Del(metadata);

	if (!result)
	{
		PyFLAC_free_list_of_type(metadata_list, metadata_count);
		Py_RETURN_FALSE;
	}

	if (metadata_list == NULL && metadata_count == 1)
	{
		Py_INCREF(py_metadata);
		metadata_list = &py_metadata;
		metadata_count = 0;
	}

	flac_StreamEncoder_clear_metadata(self);
	self->metadata = metadata_list;
	self->metadata_count = metadata_count;
	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamEncoder_get_state (flac_StreamEncoderObject *self)
{
	PyObject *state;

	state = PyFLAC_Enum_FromEnum(FLAC__stream_encoder_get_state(self->encoder), StreamEncoderState);
	Py_XINCREF(state);

	return state;
}


static PyObject *
flac_StreamEncoder_get_verify_decoder_state (flac_StreamEncoderObject *self)
{
	PyObject *state;

	if (PyFLAC_import_decoder() < 0)
		return NULL;

	state = PyFLAC_Enum_FromEnum(FLAC__stream_encoder_get_verify_decoder_state(self->encoder), StreamDecoderState);
	Py_XINCREF(state);

	return state;
}


static PyObject *
flac_StreamEncoder_get_resolved_state_string (flac_StreamEncoderObject *self)
{
	return PyFLAC_string(FLAC__stream_encoder_get_resolved_state_string(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_verify_decoder_error_stats (flac_StreamEncoderObject *self)
{
	struct flac_VerifyDecoderErrorStatsObject *stats;

	stats = PyObject_New(struct flac_VerifyDecoderErrorStatsObject, &flac_VerifyDecoderErrorStatsType);
	if (!stats)
		return NULL;

	FLAC__stream_encoder_get_verify_decoder_error_stats(self->encoder, &stats->absolute_sample, &stats->frame_number, &stats->channel, &stats->sample, &stats->expected, &stats->got);
	return (PyObject *) stats;
}


static PyObject *
flac_StreamEncoder_get_verify (flac_StreamEncoderObject *self)
{
	return PyFLAC_bool(FLAC__stream_encoder_get_verify(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_streamable_subset (flac_StreamEncoderObject *self)
{
	return PyFLAC_bool(FLAC__stream_encoder_get_streamable_subset(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_channels (flac_StreamEncoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_encoder_get_channels(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_bits_per_sample (flac_StreamEncoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_encoder_get_bits_per_sample(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_sample_rate (flac_StreamEncoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_encoder_get_sample_rate(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_blocksize (flac_StreamEncoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_encoder_get_blocksize(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_do_mid_side_stereo (flac_StreamEncoderObject *self)
{
	return PyFLAC_bool(FLAC__stream_encoder_get_do_mid_side_stereo(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_loose_mid_side_stereo (flac_StreamEncoderObject *self)
{
	return PyFLAC_bool(FLAC__stream_encoder_get_loose_mid_side_stereo(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_max_lpc_order (flac_StreamEncoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_encoder_get_max_lpc_order(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_qlp_coeff_precision (flac_StreamEncoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_encoder_get_qlp_coeff_precision(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_do_qlp_coeff_prec_search (flac_StreamEncoderObject *self)
{
	return PyFLAC_bool(FLAC__stream_encoder_get_do_qlp_coeff_prec_search(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_do_exhaustive_model_search (flac_StreamEncoderObject *self)
{
	return PyFLAC_bool(FLAC__stream_encoder_get_do_exhaustive_model_search(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_min_residual_partition_order (flac_StreamEncoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_encoder_get_min_residual_partition_order(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_max_residual_partition_order (flac_StreamEncoderObject *self)
{
	return PyFLAC_unsigned(FLAC__stream_encoder_get_max_residual_partition_order(self->encoder));
}


static PyObject *
flac_StreamEncoder_get_total_samples_estimate (flac_StreamEncoderObject *self)
{
	return PyFLAC_uint64(FLAC__stream_encoder_get_total_samples_estimate(self->encoder));
}


static PyObject *
_flac_StreamEncoder_return_status (FLAC__StreamEncoderInitStatus status, const FLAC__StreamEncoder *encoder)
{
	PyObject *py_status;

	if (status == FLAC__STREAM_ENCODER_INIT_STATUS_ENCODER_ERROR)
		switch (FLAC__stream_encoder_get_state(encoder))
		{
			case FLAC__STREAM_ENCODER_MEMORY_ALLOCATION_ERROR:
				return PyErr_NoMemory();
			case FLAC__STREAM_ENCODER_IO_ERROR:
				return PyErr_SetFromErrno(PyExc_IOError);
			default:
				break;
		}

	py_status = PyFLAC_Enum_FromEnum(status, StreamEncoderInitStatus);
	Py_XINCREF(py_status);
	return py_status;
}


static PyObject *
_flac_StreamEncoder_init_stream (flac_StreamEncoderObject *self, PyObject *args, PyObject *kwds, FLAC__bool is_ogg)
{
	FLAC__StreamEncoderInitStatus status;
	PyObject *stream = NULL;
	FLAC__StreamEncoderReadCallback read_callback = NULL;
	FLAC__StreamEncoderSeekCallback seek_callback = NULL;
	FLAC__StreamEncoderTellCallback tell_callback = NULL;
	FLAC__StreamEncoderMetadataCallback metadata_callback = NULL;

	static char *kwlist[] = {"stream",  NULL};

	stream = (PyObject *) self;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &stream))
		return NULL;

	if (PyObject_HasAttrString((PyObject *) stream, "__encoder_seek__"))
		seek_callback = flac_StreamEncoder_SeekCallback;

	if (seek_callback || PyObject_HasAttrString((PyObject *) stream, "__encoder_tell__"))
		tell_callback = flac_StreamEncoder_TellCallback;

	if (PyObject_HasAttrString((PyObject *) self, "__encoder_metadata__"))
		metadata_callback = flac_StreamEncoder_MetadataCallback;

	if (is_ogg)
	{
		if (seek_callback)
			read_callback = flac_StreamEncoder_ReadCallback;

		status = FLAC__stream_encoder_init_ogg_stream(self->encoder, read_callback, flac_StreamEncoder_WriteCallback, seek_callback, tell_callback, metadata_callback, self);
	}
	else
	{
		status = FLAC__stream_encoder_init_stream(self->encoder, flac_StreamEncoder_WriteCallback, seek_callback, tell_callback, metadata_callback, self);
	}

	if (status == FLAC__STREAM_ENCODER_INIT_STATUS_OK && stream != (PyObject *) self)
		self->stream = (Py_INCREF(stream), stream);

	return _flac_StreamEncoder_return_status(status, self->encoder);
}

static PyObject *
flac_StreamEncoder_init_stream (flac_StreamEncoderObject *self, PyObject *args, PyObject *kwds)
{
	return _flac_StreamEncoder_init_stream(self, args, kwds, false);
}

static PyObject *
flac_StreamEncoder_init_ogg_stream (flac_StreamEncoderObject *self, PyObject *args, PyObject *kwds)
{
	return _flac_StreamEncoder_init_stream(self, args, kwds, true);
}


typedef FLAC__StreamEncoderInitStatus(*flac_init_file)(FLAC__StreamEncoder *, const char *, FLAC__StreamEncoderProgressCallback, void *);

static PyObject *
_flac_StreamEncoder_init_file (flac_StreamEncoderObject *self, PyObject *args, flac_init_file flac_init_file_function)
{
	const char *file_name;
	FLAC__StreamEncoderProgressCallback progress_callback = NULL;

	if (!PyArg_ParseTuple(args, "s", &file_name))
		return NULL;

	if (PyObject_HasAttrString((PyObject *) self, "__encoder_progress__"))
		progress_callback = flac_StreamEncoderProgressCallback;

	return _flac_StreamEncoder_return_status(flac_init_file_function(self->encoder, file_name, progress_callback, self), self->encoder);
}

static PyObject *
flac_StreamEncoder_init_file (flac_StreamEncoderObject *self, PyObject *args, PyObject *kwds)
{
	return _flac_StreamEncoder_init_file(self, args, FLAC__stream_encoder_init_file);
}

static PyObject *
flac_StreamEncoder_init_ogg_file (flac_StreamEncoderObject *self, PyObject *args, PyObject *kwds)
{
	return _flac_StreamEncoder_init_file(self, args, FLAC__stream_encoder_init_ogg_file);
}


static PyObject *
flac_StreamEncoder_finish (flac_StreamEncoderObject *self)
{
	FLAC__bool result;

	Py_CLEAR(self->stream);
	result = FLAC__stream_encoder_finish(self->encoder);
	flac_StreamEncoder_clear_metadata(self);
	return PyFLAC_bool(result);
}

static int
_int32_array (PyObject *obj, FLAC__int32 **addr)
{
	FLAC__int32 *buffer, *tmp;
	PyObject *item;
	Py_ssize_t count, i;
	long val;

	if (PySequence_Check(obj))
	{
		count = PySequence_Size(obj);
		if (count == -1)
			return 0;

		buffer = PyMem_New(FLAC__int32, count + 1);
		if (!buffer)
		{
			PyErr_NoMemory();
			return 0;
		}

		buffer[0] = count;

		for (i = 0; i < count; i++)
		{
			item = PySequence_GetItem(obj, i);
			if (!item)
				break;

			val = PyInt_AsLong(item);
			if ((val == -1 && PyErr_Occurred()) || val != (FLAC__int32) val)
				break;

			buffer[i+1] = val;
			Py_DECREF(item);
		}

		if (count != i)
		{
			PyMem_Del(buffer);
			if (!PyErr_Occurred())
				PyErr_SetString(PyExc_OverflowError, "Python int too large to convert to int32");
			return 0;
		}

		*addr = buffer;
		return 1;
	}
	else if (PyIter_Check(obj) || (obj = PyObject_GetIter(obj)))
	{
		buffer = PyMem_New(FLAC__int32, count = 16);

		i = 1;
		while ((item = PyIter_Next(obj)))
		{
			val = PyInt_AsLong(item);
			Py_DECREF(item);
			if (val == -1 && PyErr_Occurred())
				break;

			if (val != (FLAC__int32) val)
			{
				PyErr_SetString(PyExc_OverflowError, "Python int too large to convert to int32");
				break;
			}

			if (count <= i)
			{
				tmp = buffer;
				if (!PyMem_Resize(buffer, FLAC__int32, count = 2 * count))
				{
					PyErr_NoMemory();
					buffer = tmp;
					break;
				}
			}

			buffer[i++] = val;
		}

		if (PyErr_Occurred())
		{
			PyMem_Del(buffer);
			return 0;
		}

		buffer[0] = i - 1;
		*addr = buffer;
		return 1;
	}

	PyErr_SetString(PyExc_TypeError, "must be a sequence or an iterator of int32");
	return 0;
}


#ifdef __PyFLAC3__
#define flac_bytes "y#"
#else // __PyFLAC3__
#define flac_bytes "s#"
#endif // __PyFLAC3__

static PyObject *
flac_StreamEncoder_process (flac_StreamEncoderObject *self, PyObject *args)
{
	PyObject *list;
	Py_ssize_t buffer_count, i, len;
	char *strbuf;
	FLAC__int32 **buffer = NULL;
	PyObject *result = NULL;
	PyObject **buffer_list = NULL;
	unsigned channels, samples = -1;

	if (FLAC__stream_encoder_get_state(self->encoder) != FLAC__STREAM_ENCODER_OK)
		return PyErr_Format(flac_FlacEncoderErrorType, "encoder state must be %s.OK", (PyFLAC_type(StreamEncoderState))->tp_name);

	channels = FLAC__stream_encoder_get_channels(self->encoder);

	if (!PyArg_ParseTuple(args, flac_bytes, &strbuf, &samples))
		PyErr_Clear();
	else if (channels == 1)
		return PyFLAC_bool(FLAC__stream_encoder_process(self->encoder, (const FLAC__int32 * const*) &strbuf, samples / 4));
	else
		return PyErr_Format(PyExc_ValueError, "data for %u channels required", channels);

	buffer = PyMem_New(FLAC__int32 *, channels);
	if (!buffer)
		return PyErr_NoMemory();

	if (!PyArg_ParseTuple(args, "O", &list))
	{
		PyErr_Clear();
		list = args;
	}

	if (!PyFLAC_list_of_type(list, NULL, &buffer_list, &buffer_count))
	{
		PyMem_Del(buffer);
		return NULL;
	}

	list = PyList_New(buffer_count);
	for (i = 0; i < buffer_count; i++)
		PyList_SET_ITEM(list, i, buffer_list[i]);
	PyMem_Del(buffer_list);
	buffer_list = NULL;

	if (channels <= buffer_count && PyFLAC_list_of_type(list, &PyBytes_Type, &buffer_list, &buffer_count))
	{
		for (i = 0; i < channels; i++)
		{
			if (PyBytes_AsStringAndSize(buffer_list[i], &strbuf, &len) < 0)
				break;

			if (len % 4)
			{
				PyErr_SetString(PyExc_TypeError, "length of data must be a multiple of 4");
				break;
			}

			if (len < samples)
				samples = len;

			buffer[i] = (FLAC__int32 *) strbuf;
		}

		if (channels == i)
			result = PyFLAC_bool(FLAC__stream_encoder_process(self->encoder, (const FLAC__int32 * const*) buffer, samples / 4));

		PyFLAC_free_list_of_type(buffer_list, buffer_count);
	}
	else
	{
		PyErr_Clear();
		i = channels + 1;

		if (channels == 1 && _int32_array(list, &buffer[0]))
		{
			if (buffer[0][0] < samples)
				samples = buffer[0][0];

			buffer[0]++;
			i = 1;
		}
		else if (channels > buffer_count)
		{
			i = channels + 1;
			PyErr_Format(PyExc_ValueError, "data for %u channels required", channels);
		}
		else
		{
			PyErr_Clear();
			for (i = 0; i < channels; i++)
			{
				if (!_int32_array(PyList_GET_ITEM(list, i), &buffer[i]))
					break;

				if (buffer[i][0] < samples)
					samples = buffer[i][0];

				buffer[i]++;
			}
		}

		if (channels == i)
			result = PyFLAC_bool(FLAC__stream_encoder_process(self->encoder, (const FLAC__int32 * const*) buffer, samples));
		else if (channels < i)
			i = 0;

		while (i--)
			PyMem_Del(--buffer[i]);
	}

	PyMem_Del(buffer);
	return result;
}

static PyObject *
flac_StreamEncoder_process_interleaved (flac_StreamEncoderObject *self, PyObject *args)
{
	unsigned channels, samples;
	char *strbuf;
	FLAC__int32 *buffer = NULL;
	PyObject *result = NULL;

	if (FLAC__stream_encoder_get_state(self->encoder) != FLAC__STREAM_ENCODER_OK)
		return PyErr_Format(flac_FlacEncoderErrorType, "encoder state must be %s.OK", (PyFLAC_type(StreamEncoderState))->tp_name);

	channels = FLAC__stream_encoder_get_channels(self->encoder);

	if (PyArg_ParseTuple(args, flac_bytes, &strbuf, &samples))
	{
		if (samples % 4)
		{
			PyErr_SetString(PyExc_TypeError, "length of data must be a multiple of 4");
			return NULL;
		}

		samples /= 4;
		buffer = (FLAC__int32 *) strbuf;
	}
	else
	{
		_int32_array(args, &buffer);
		PyErr_Clear();

		if (!buffer && !PyArg_ParseTuple(args, "O&", _int32_array, &buffer))
			return NULL;

		samples = buffer[0];
		buffer++;
		strbuf = NULL;
	}

	if (samples % channels)
		PyErr_SetString(PyExc_TypeError, "length of data must be a multiple of channels");
	else
		result = PyFLAC_bool(FLAC__stream_encoder_process_interleaved(self->encoder, buffer, samples/channels));

	if (!strbuf)
		PyMem_Del(--buffer);

	return result;
}


static PyMethodDef flac_StreamEncoder_methods[] = {
	{
		"set_ogg_serial_number",
		(PyCFunction) flac_StreamEncoder_set_ogg_serial_number,
		METH_VARARGS,
		"Set ogg serial number"
	}, {
		"set_verify",
		(PyCFunction) flac_StreamEncoder_set_verify,
		METH_VARARGS,
		"Set verify"
	}, {
		"set_streamable_subset",
		(PyCFunction) flac_StreamEncoder_set_streamable_subset,
		METH_VARARGS,
		"Set streamable subset"
	}, {
		"set_channels",
		(PyCFunction) flac_StreamEncoder_set_channels,
		METH_VARARGS,
		"Set channels"
	}, {
		"set_bits_per_sample",
		(PyCFunction) flac_StreamEncoder_set_bits_per_sample,
		METH_VARARGS,
		"Set bits per sample"
	}, {
		"set_sample_rate",
		(PyCFunction) flac_StreamEncoder_set_sample_rate,
		METH_VARARGS,
		"Set sample rate"
	}, {
		"set_compression_level",
		(PyCFunction) flac_StreamEncoder_set_compression_level,
		METH_VARARGS,
		"Set compression level"
	}, {
		"set_blocksize",
		(PyCFunction) flac_StreamEncoder_set_blocksize,
		METH_VARARGS,
		"Set blocksize"
	}, {
		"set_do_mid_side_stereo",
		(PyCFunction) flac_StreamEncoder_set_do_mid_side_stereo,
		METH_VARARGS,
		"Set do mid side stereo"
	}, {
		"set_loose_mid_side_stereo",
		(PyCFunction) flac_StreamEncoder_set_loose_mid_side_stereo,
		METH_VARARGS,
		"Set loose mid side stereo"
	}, {
		"set_apodization",
		(PyCFunction) flac_StreamEncoder_set_apodization,
		METH_VARARGS,
		"Set apodization"
	}, {
		"set_max_lpc_order",
		(PyCFunction) flac_StreamEncoder_set_max_lpc_order,
		METH_VARARGS,
		"Set max lpc order"
	}, {
		"set_qlp_coeff_precision",
		(PyCFunction) flac_StreamEncoder_set_qlp_coeff_precision,
		METH_VARARGS,
		"Set qlp coeff precision"
	}, {
		"set_do_qlp_coeff_prec_search",
		(PyCFunction) flac_StreamEncoder_set_do_qlp_coeff_prec_search,
		METH_VARARGS,
		"Set do qlp coeff prec search"
	}, {
		"set_do_exhaustive_model_search",
		(PyCFunction) flac_StreamEncoder_set_do_exhaustive_model_search,
		METH_VARARGS,
		"Set do exhaustive model search"
	}, {
		"set_min_residual_partition_order",
		(PyCFunction) flac_StreamEncoder_set_min_residual_partition_order,
		METH_VARARGS,
		"Set min residual partition order"
	}, {
		"set_max_residual_partition_order",
		(PyCFunction) flac_StreamEncoder_set_max_residual_partition_order,
		METH_VARARGS,
		"set max residual partition order"
	}, {
		"set_total_samples_estimate",
		(PyCFunction) flac_StreamEncoder_set_total_samples_estimate,
		METH_VARARGS,
		"Set total samples estimate"
	}, {
		"set_metadata",
		(PyCFunction) flac_StreamEncoder_set_metadata,
		METH_VARARGS,
		"Set metadata"
	}, {
		"get_state",
		(PyCFunction) flac_StreamEncoder_get_state,
		METH_NOARGS,
		"Get state"
	}, {
		"get_verify_decoder_state",
		(PyCFunction) flac_StreamEncoder_get_verify_decoder_state,
		METH_NOARGS,
		"Get verify decoder state"
	}, {
		"get_resolved_state_string",
		(PyCFunction) flac_StreamEncoder_get_resolved_state_string,
		METH_NOARGS,
		"Get resolved state string"
	}, {
		"get_verify_decoder_error_stats",
		(PyCFunction) flac_StreamEncoder_get_verify_decoder_error_stats,
		METH_NOARGS,
		"Get verify decoder error stats"
	}, {
		"get_verify",
		(PyCFunction) flac_StreamEncoder_get_verify,
		METH_NOARGS,
		"Get verify"
	}, {
		"get_streamable_subset",
		(PyCFunction) flac_StreamEncoder_get_streamable_subset,
		METH_NOARGS,
		"Get streamable subset"
	}, {
		"get_channels",
		(PyCFunction) flac_StreamEncoder_get_channels,
		METH_NOARGS,
		"Get channels"
	}, {
		"get_bits_per_sample",
		(PyCFunction) flac_StreamEncoder_get_bits_per_sample,
		METH_NOARGS,
		"Get bits per sample"
	}, {
		"get_sample_rate",
		(PyCFunction) flac_StreamEncoder_get_sample_rate,
		METH_NOARGS,
		"Get sample rate"
	}, {
		"get_blocksize",
		(PyCFunction) flac_StreamEncoder_get_blocksize,
		METH_NOARGS,
		"Get blocksize"
	}, {
		"get_do_mid_side_stereo",
		(PyCFunction) flac_StreamEncoder_get_do_mid_side_stereo,
		METH_NOARGS,
		"Get do mid side stereo"
	}, {
		"get_loose_mid_side_stereo",
		(PyCFunction) flac_StreamEncoder_get_loose_mid_side_stereo,
		METH_NOARGS,
		"Get loose mid side stereo"
	}, {
		"get_max_lpc_order",
		(PyCFunction) flac_StreamEncoder_get_max_lpc_order,
		METH_NOARGS,
		"Get max lpc order"
	}, {
		"get_qlp_coeff_precision",
		(PyCFunction) flac_StreamEncoder_get_qlp_coeff_precision,
		METH_NOARGS,
		"Get qlp coeff precision"
	}, {
		"get_do_qlp_coeff_prec_search",
		(PyCFunction) flac_StreamEncoder_get_do_qlp_coeff_prec_search,
		METH_NOARGS,
		"Get do qlp coeff prec search"
	}, {
		"get_do_exhaustive_model_search",
		(PyCFunction) flac_StreamEncoder_get_do_exhaustive_model_search,
		METH_NOARGS,
		"Get do exhaustive model search"
	}, {
		"get_min_residual_partition_order",
		(PyCFunction) flac_StreamEncoder_get_min_residual_partition_order,
		METH_NOARGS,
		"Get min residual partition order"
	}, {
		"get_max_residual_partition_order",
		(PyCFunction) flac_StreamEncoder_get_max_residual_partition_order,
		METH_NOARGS,
		"Get max residual partition order"
	}, {
		"get_total_samples_estimate",
		(PyCFunction) flac_StreamEncoder_get_total_samples_estimate,
		METH_NOARGS,
		"Get total samples estimate"
	}, {
		"init_stream",
		(PyCFunction) flac_StreamEncoder_init_stream,
		METH_VARARGS | METH_KEYWORDS,
		"Init stream"
	}, {
		"init_ogg_stream",
		(PyCFunction) flac_StreamEncoder_init_ogg_stream,
		METH_VARARGS | METH_KEYWORDS,
		"Init ogg stream"
	}, {
		"init_file",
		(PyCFunction) flac_StreamEncoder_init_file,
		METH_VARARGS,
		"Init file"
	}, {
		"init_ogg_file",
		(PyCFunction) flac_StreamEncoder_init_ogg_file,
		METH_VARARGS,
		"Init ogg file"
	}, {
		"finish",
		(PyCFunction) flac_StreamEncoder_finish,
		METH_NOARGS,
		"Finish"
	}, {
		"process",
		(PyCFunction) flac_StreamEncoder_process,
		METH_VARARGS,
		"Process"
	}, {
		"process_interleaved",
		(PyCFunction) flac_StreamEncoder_process_interleaved,
		METH_VARARGS,
		"Process interleaved"
	}, { NULL }		/* Sentinel */
};


static PyTypeObject PyFLAC_StreamEncoderType = {
	PyVarObject_HEAD_INIT(NULL,0)
	"flac.StreamEncoder",							/* tp_name */
	sizeof(flac_StreamEncoderObject),				/* tp_basicsize */
	0,												/* tp_itemsize */
	(destructor) flac_StreamEncoder_dealloc,		/* tp_dealloc */
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
	"FLAC StreamEncoder",							/* tp_doc */
	(traverseproc) flac_StreamEncoder_traverse,		/* tp_traverse */
	(inquiry)  flac_StreamEncoder_clear,			/* tp_clear */
	0,												/* tp_richcompare */
	0,												/* tp_weaklistoffset */
	0,												/* tp_iter */
	0,												/* tp_iternext */
	flac_StreamEncoder_methods,						/* tp_methods */
	0,												/* tp_members */
	0,												/* tp_getset */
	0,												/* tp_base */
	0,												/* tp_dict */
	0,												/* tp_descr_get */
	0,												/* tp_descr_set */
	0,												/* tp_dictoffset */
	0,												/* tp_init */
	0,												/* tp_alloc */
	flac_StreamEncoder_new,							/* tp_new */
};


static PyMethodDef flac_encoder_functions[] = {
	{ NULL }		/* Sentinel */
};


PyFLAC_Enum_FromEnum_function(StreamEncoderState, enum_member_StreamEncoderState)
PyFLAC_Enum_FromEnum_function(StreamEncoderInitStatus, enum_member_StreamEncoderInitStatus)
PyFLAC_Enum_FromEnum_function(StreamEncoderReadStatus, enum_member_StreamEncoderReadStatus)
PyFLAC_Enum_FromEnum_function(StreamEncoderSeekStatus, enum_member_StreamEncoderSeekStatus)
PyFLAC_Enum_FromEnum_function(StreamEncoderTellStatus, enum_member_StreamEncoderTellStatus)
PyFLAC_Enum_FromEnum_function(StreamEncoderWriteStatus, enum_member_StreamEncoderWriteStatus)


static int
flac_encoder_init (void)
{
	PyFLAC_CHECK_status(PyFLAC_import_format());
	flac_FlacEncoderErrorType = PyErr_NewExceptionWithDoc("flac.FlacEncoderError", "FLAC stream encoder error", PyFLAC_type(FlacFormatError), NULL);
	if (!flac_FlacEncoderErrorType)
		return -1;

	PyFLAC_CHECK_status(PyType_Ready(PyFLAC_type(StreamEncoder)));

	flac_VerifyDecoderErrorStatsType.tp_dealloc = (destructor) PyObject_Del;
	PyFLAC_CHECK_status(PyType_Ready(&flac_VerifyDecoderErrorStatsType));

	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamEncoderState), enum_member_StreamEncoderState));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamEncoderInitStatus), enum_member_StreamEncoderInitStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamEncoderReadStatus), enum_member_StreamEncoderReadStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamEncoderSeekStatus), enum_member_StreamEncoderSeekStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamEncoderTellStatus), enum_member_StreamEncoderTellStatus));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamEncoderWriteStatus), enum_member_StreamEncoderWriteStatus));

	_c_api_init

	return 0;
}


static void
flac_encoder_build (PyObject *module)
{
	Py_INCREF(PyFLAC_type(StreamEncoder));
	PyFLAC_PyModule_AddType(module, StreamEncoder);

	Py_INCREF(PyFLAC_type(StreamEncoderState));
	PyFLAC_PyModule_AddType(module, StreamEncoderState);

	Py_INCREF(PyFLAC_type(StreamEncoderInitStatus));
	PyFLAC_PyModule_AddType(module, StreamEncoderInitStatus);

	Py_INCREF(PyFLAC_type(StreamEncoderReadStatus));
	PyFLAC_PyModule_AddType(module, StreamEncoderReadStatus);

	Py_INCREF(PyFLAC_type(StreamEncoderSeekStatus));
	PyFLAC_PyModule_AddType(module, StreamEncoderSeekStatus);

	Py_INCREF(PyFLAC_type(StreamEncoderTellStatus));
	PyFLAC_PyModule_AddType(module, StreamEncoderTellStatus);

	Py_INCREF(PyFLAC_type(StreamEncoderWriteStatus));
	PyFLAC_PyModule_AddType(module, StreamEncoderWriteStatus);

	Py_INCREF(PyFLAC_type(FlacFormatError));
	PyFLAC_PyModule_AddType(module, FlacFormatError);

	Py_INCREF(PyFLAC_type(FlacEncoderError));
	PyFLAC_PyModule_AddType(module, FlacEncoderError);

	_c_api_build(module);
}


PyFLAC_MODINIT(
	encoder,
	flac_encoder_init,
	flac_encoder_build,
	flac_encoder_functions,
	"libFLAC stream_encoder"
)

