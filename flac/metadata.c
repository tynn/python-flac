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

#define __PyFLAC_metadata_MODULE__
#include "metadata.h"

#include "_enum.h"

#include "format.h"

#include "_application_id.h"
#include "_bool.h"
#include "_byte.h"
#include "_uint32.h"
#include "_uint64.h"
#include "_unsigned.h"

#include "_list_of_type.h"


#define flac_Iter_New(type,data,data_size,data_host) \
	PyFLAC_Iter_New(data, data_size, flac_##type##_iternext, PyFLAC_name(type), (PyObject *) data_host)


#define PyFLAC_CHECK_metadata_type(metadata_type) \
	if (metadata_type < 0 || metadata_type >= FLAC__METADATA_TYPE_UNDEFINED) \
	{ \
		PyFLAC_RuntimeError("given " PyFLAC_name(MetadataType) " does not exist"); \
		return NULL; \
	}


#define flac_StreamMetadataData_DEF(type) \
static int flac_##type##_set (PyFLAC_StreamMetadataObject *, PyObject *, void *); \
static PyTypeObject PyFLAC_##type##Type = { \
	PyVarObject_HEAD_INIT(NULL,0) \
	PyFLAC_name(type), \
	sizeof(PyFLAC_StreamMetadataObject), \
};


PyFLAC_Enum(StreamMetadataPictureType)


static flac_Enum_Member enum_member_StreamMetadataPictureType[] = {
	PyFLAC_Enum_Member(
		"OTHER",
		STREAM_METADATA_PICTURE_TYPE_OTHER
	),
	PyFLAC_Enum_Member(
		"FILE_ICON_STANDARD",
		STREAM_METADATA_PICTURE_TYPE_FILE_ICON_STANDARD
	),
	PyFLAC_Enum_Member(
		"FILE_ICON",
		STREAM_METADATA_PICTURE_TYPE_FILE_ICON
	),
	PyFLAC_Enum_Member(
		"FRONT_COVER",
		STREAM_METADATA_PICTURE_TYPE_FRONT_COVER
	),
	PyFLAC_Enum_Member(
		"BACK_COVER",
		STREAM_METADATA_PICTURE_TYPE_BACK_COVER
	),
	PyFLAC_Enum_Member(
		"LEAFLET_PAGE",
		STREAM_METADATA_PICTURE_TYPE_LEAFLET_PAGE
	),
	PyFLAC_Enum_Member(
		"MEDIA",
		STREAM_METADATA_PICTURE_TYPE_MEDIA
	),
	PyFLAC_Enum_Member(
		"LEAD_ARTIST",
		STREAM_METADATA_PICTURE_TYPE_LEAD_ARTIST
	),
	PyFLAC_Enum_Member(
		"ARTIST",
		STREAM_METADATA_PICTURE_TYPE_ARTIST
	),
	PyFLAC_Enum_Member(
		"CONDUCTOR",
		STREAM_METADATA_PICTURE_TYPE_CONDUCTOR
	),
	PyFLAC_Enum_Member(
		"BAND",
		STREAM_METADATA_PICTURE_TYPE_BAND
	),
	PyFLAC_Enum_Member(
		"COMPOSER",
		STREAM_METADATA_PICTURE_TYPE_COMPOSER
	),
	PyFLAC_Enum_Member(
		"LYRICIST",
		STREAM_METADATA_PICTURE_TYPE_LYRICIST
	),
	PyFLAC_Enum_Member(
		"RECORDING_LOCATION",
		STREAM_METADATA_PICTURE_TYPE_RECORDING_LOCATION
	),
	PyFLAC_Enum_Member(
		"DURING_RECORDING",
		STREAM_METADATA_PICTURE_TYPE_DURING_RECORDING
	),
	PyFLAC_Enum_Member(
		"DURING_PERFORMANCE",
		STREAM_METADATA_PICTURE_TYPE_DURING_PERFORMANCE
	),
	PyFLAC_Enum_Member(
		"VIDEO_SCREEN_CAPTURE",
		STREAM_METADATA_PICTURE_TYPE_VIDEO_SCREEN_CAPTURE
	),
	PyFLAC_Enum_Member(
		"FISH",
		STREAM_METADATA_PICTURE_TYPE_FISH
	),
	PyFLAC_Enum_Member(
		"ILLUSTRATION",
		STREAM_METADATA_PICTURE_TYPE_ILLUSTRATION
	),
	PyFLAC_Enum_Member(
		"BAND_LOGOTYPE",
		STREAM_METADATA_PICTURE_TYPE_BAND_LOGOTYPE
	),
	PyFLAC_Enum_Member(
		"PUBLISHER_LOGOTYPE",
		STREAM_METADATA_PICTURE_TYPE_PUBLISHER_LOGOTYPE
	),
	PyFLAC_Enum_Member(
		"UNDEFINED",
		STREAM_METADATA_PICTURE_TYPE_UNDEFINED
	),
	{ NULL }		/* Sentinel */
};


static int
flac_list_of_bytes (PyObject *data, FLAC__byte **ret_bytes, Py_ssize_t *ret_length)
{
	Py_ssize_t length;
	char *_bytes;
	FLAC__byte *bytes;

	if (!data || data == Py_None)
	{
		*ret_bytes = NULL;
		*ret_length = 0;
		return 1;
	}

	if (!PyBytes_Check(data))
	{
		PyErr_Format(PyExc_TypeError, "must be %.50s, not %.50s", PyBytes_Type.tp_name, Py_TYPE(data)->tp_name);
		return 0;
	}

	if (PyBytes_AsStringAndSize(data, &_bytes, &length) < 0)
		return 0;

	bytes = (FLAC__byte *) malloc(length * sizeof(FLAC__byte));
	if (!bytes)
	{
		PyErr_NoMemory();
		return 0;
	}

	Py_MEMCPY(bytes, _bytes, length);

	*ret_bytes = bytes;
	*ret_length = length;
	return 1;
}


static PyTypeObject PyFLAC_StreamMetadataType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(StreamMetadata),
	sizeof(PyFLAC_StreamMetadataObject),
};


typedef enum {
	flac_StreamMetadataStreamInfo_min_blocksize,
	flac_StreamMetadataStreamInfo_max_blocksize,
	flac_StreamMetadataStreamInfo_min_framesize,
	flac_StreamMetadataStreamInfo_max_framesize,
	flac_StreamMetadataStreamInfo_sample_rate,
	flac_StreamMetadataStreamInfo_channels,
	flac_StreamMetadataStreamInfo_bits_per_sample,
	flac_StreamMetadataStreamInfo_total_samples,
	flac_StreamMetadataStreamInfo_md5sum,
	flac_StreamMetadataStreamInfo_member_count, /*9*/
} flac_StreamMetadataStreamInfo_member;
flac_StreamMetadataData_DEF(StreamMetadataStreamInfo)


typedef enum {
	flac_StreamMetadataPadding_length,
	flac_StreamMetadataPadding_member_count, /*1*/
} flac_StreamMetadataPadding_member;
flac_StreamMetadataData_DEF(StreamMetadataPadding)


typedef enum {
	flac_StreamMetadataApplication_id,
	flac_StreamMetadataApplication_data,
	flac_StreamMetadataApplication_member_count, /*2*/
} flac_StreamMetadataApplication_member;
flac_StreamMetadataData_DEF(StreamMetadataApplication)


typedef enum {
	flac_StreamMetadataSeekTable_points,
	flac_StreamMetadataSeekTable_member_count, /*1*/
} flac_StreamMetadataSeekTable_member;
flac_StreamMetadataData_DEF(StreamMetadataSeekTable)


typedef enum {
	flac_StreamMetadataVorbisComment_vendor_string,
	flac_StreamMetadataVorbisComment_comments,
	flac_StreamMetadataVorbisComment_member_count, /*2*/
} flac_StreamMetadataVorbisComment_member;
flac_StreamMetadataData_DEF(StreamMetadataVorbisComment)


typedef enum {
	flac_StreamMetadataCueSheet_media_catalog_number,
	flac_StreamMetadataCueSheet_lead_in,
	flac_StreamMetadataCueSheet_is_cd,
	flac_StreamMetadataCueSheet_tracks,
	flac_StreamMetadataCueSheet_member_count, /*4*/
} flac_StreamMetadataCueSheet_member;
flac_StreamMetadataData_DEF(StreamMetadataCueSheet)


typedef enum {
	flac_StreamMetadataPicture_type,
	flac_StreamMetadataPicture_mime_type,
	flac_StreamMetadataPicture_description,
	flac_StreamMetadataPicture_width,
	flac_StreamMetadataPicture_height,
	flac_StreamMetadataPicture_depth,
	flac_StreamMetadataPicture_colors,
	flac_StreamMetadataPicture_data,
	flac_StreamMetadataPicture_member_count, /*8*/
} flac_StreamMetadataPicture_member;
flac_StreamMetadataData_DEF(StreamMetadataPicture)


#define PyFLAC_Data_member_COUNT 9
#define PyFLAC_Data_member_O "OOOOOOOOO"
#define PyFLAC_Data_member_objects(objects,assign) \
	objects[0] assign, \
	objects[1] assign, \
	objects[2] assign, \
	objects[3] assign, \
	objects[4] assign, \
	objects[5] assign, \
	objects[6] assign, \
	objects[7] assign, \
	objects[8] assign


struct flac_StreamMetadataData_setter {
	int member_count;
	PyTypeObject *type;
	setter set_member;
	char *kwlist[PyFLAC_Data_member_COUNT + 1];
};


static struct flac_StreamMetadataData_setter flac_StreamMetadataData_setters[] = {
	{
		flac_StreamMetadataStreamInfo_member_count,
		PyFLAC_type(StreamMetadataStreamInfo),
		(setter) flac_StreamMetadataStreamInfo_set,
		{"min_blocksize", "max_blocksize", "min_framesize", "max_framesize", "sample_rate", "channels", "bits_per_sample", "total_samples", "md5sum", NULL}
	}, {
		flac_StreamMetadataPadding_member_count,
		PyFLAC_type(StreamMetadataPadding),
		(setter) flac_StreamMetadataPadding_set,
		{"length", NULL}
	}, {
		flac_StreamMetadataApplication_member_count,
		PyFLAC_type(StreamMetadataApplication),
		(setter) flac_StreamMetadataApplication_set,
		{"id", "data", NULL}
	}, {
		flac_StreamMetadataSeekTable_member_count,
		PyFLAC_type(StreamMetadataSeekTable),
		(setter) flac_StreamMetadataSeekTable_set,
		{"points", NULL}
	}, {
		flac_StreamMetadataVorbisComment_member_count,
		PyFLAC_type(StreamMetadataVorbisComment),
		(setter) flac_StreamMetadataVorbisComment_set,
		{"vendor_string", "comments", NULL}
	}, {
		flac_StreamMetadataCueSheet_member_count,
		PyFLAC_type(StreamMetadataCueSheet),
		(setter) flac_StreamMetadataCueSheet_set,
		{"media_catalog_number", "lead_in", "is_cd", "tracks", NULL}
	}, {
		flac_StreamMetadataPicture_member_count,
		PyFLAC_type(StreamMetadataPicture),
		(setter) flac_StreamMetadataPicture_set,
		{"picture_type", "mime_type", "description", "width", "height", "depth", "colors", "data", NULL}
	}
};


static void
flac_StreamMetadata_dealloc (PyFLAC_StreamMetadataObject *self)
{
	if (self->metadata)
		FLAC__metadata_object_delete(self->metadata);
	Py_XDECREF(self->metadata_type);
	PyObject_Del(self);
}


static PyObject *
flac_StreamMetadata_new_object (PyTypeObject *type, FLAC__MetadataType metadata_type, const FLAC__StreamMetadata *metadata)
{
	PyFLAC_StreamMetadataObject *self;

	if (metadata)
		metadata_type = metadata->type;

	PyFLAC_CHECK_metadata_type(metadata_type)

	self = PyObject_New(PyFLAC_StreamMetadataObject, flac_StreamMetadataData_setters[metadata_type].type);

	if (self)
	{
		self->metadata_type = PyFLAC_Enum_FromEnum(metadata_type, MetadataType);
		if (self->metadata_type)
		{
			Py_INCREF(self->metadata_type);

			if (metadata)
				self->metadata = FLAC__metadata_object_clone(metadata);
			else
				self->metadata = FLAC__metadata_object_new(metadata_type);

			if (!self->metadata)
			{
				Py_DECREF(self);
				return PyErr_NoMemory();
			}
		}
	}

	return (PyObject *) self;
}


static PyObject *
flac_StreamMetadata__new (PyTypeObject *type, PyObject *args, PyObject *kwds, const FLAC__MetadataType metadata_type)
{
	PyFLAC_StreamMetadataObject *self;
	PyObject *objects[PyFLAC_Data_member_COUNT];
	struct flac_StreamMetadataData_setter fs;
	int i;

	char format[] = "|" PyFLAC_Data_member_O;

	PyFLAC_CHECK_metadata_type(metadata_type)

	fs = flac_StreamMetadataData_setters[metadata_type];

	if (!fs.member_count)
		return flac_StreamMetadata_new_object(type, metadata_type, NULL);

	format[fs.member_count + 1] = '\0';
	PyFLAC_Data_member_objects(objects,=NULL);

	if (!PyArg_ParseTupleAndKeywords(args, kwds, format, fs.kwlist, PyFLAC_Data_member_objects(&objects,)))
		return NULL;

	self = (PyFLAC_StreamMetadataObject *) flac_StreamMetadata_new_object(type, metadata_type, NULL);
	if (self)
		for (i = 0; i < fs.member_count; i++)
			if (objects[i] && fs.set_member((PyObject *) self, objects[i], (void *) (long) i) < 0)
			{
				Py_DECREF(self);
				return NULL;
			}

	return (PyObject *) self;
}


static PyObject *
flac_StreamMetadata_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyObject *metadata;

	if (!PyArg_ParseTuple(args, "O!", PyFLAC_type(MetadataType), &metadata))
		return NULL;

	args = PyTuple_New(0);
	if (!args)
		return NULL;

	metadata = flac_StreamMetadata__new(type, args, kwds, PyFLAC_Enum_AsEnum(metadata, MetadataType));
	Py_DECREF(args);

	return metadata;
}


static PyObject *
flac_StreamMetadata_clone (PyFLAC_StreamMetadataObject *self)
{
	return flac_StreamMetadata_new_object(Py_TYPE(self), 0, self->metadata);
}


static PyMethodDef flac_StreamMetadata_methods[] = {
	{
		"__copy__",
		(PyCFunction) flac_StreamMetadata_clone,
		METH_NOARGS,
		"Clone"
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadata_get_metadata_type (PyFLAC_StreamMetadataObject *self, void *closure)
{
	Py_INCREF(self->metadata_type);
	return self->metadata_type;
}


static PyObject *
flac_StreamMetadata_get_length (PyFLAC_StreamMetadataObject *self, void *closure)
{
	return PyFLAC_unsigned(self->metadata->length);
}


static PyObject *
flac_StreamMetadata_get_is_last (PyFLAC_StreamMetadataObject *self, void *closure)
{
	return PyFLAC_bool(self->metadata->is_last);
}


static int
flac_StreamMetadata_set_is_last (PyFLAC_StreamMetadataObject *self, PyObject *value, void *closure)
{
	return _bool(value, &self->metadata->is_last) ? 0 : -1;
}


static PyGetSetDef flac_StreamMetadata_getseters[] = {
	{
		"type",
		(getter) flac_StreamMetadata_get_metadata_type, NULL,
		"StreamMetadata type", NULL
	}, {
		"is_last",
		(getter) flac_StreamMetadata_get_is_last,
		(setter) flac_StreamMetadata_set_is_last,
		"StreamMetadata is_last", NULL
	}, {
		"length",
		(getter) flac_StreamMetadata_get_length, NULL,
		"StreamMetadata length", NULL
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadata_compare (PyFLAC_StreamMetadataObject *self, PyFLAC_StreamMetadataObject *other, int op)
{
	if (op != Py_EQ && op != Py_NE)
	{
		Py_INCREF(Py_NotImplemented);
		return Py_NotImplemented;
	}

	if (!other)
		return PyFLAC_bool(op == Py_NE);

	return PyFLAC_bool((op == Py_NE) ^ FLAC__metadata_object_is_equal(self->metadata, other->metadata));
}


static PyObject *
flac_StreamMetadata_richcompare (PyFLAC_StreamMetadataObject *self, PyObject *other, int op)
{
	if (!PyFLAC_type_Check(other, StreamMetadata))
		other = NULL;

	return flac_StreamMetadata_compare(self, (PyFLAC_StreamMetadataObject *) other, op);
}


static int
flac_StreamMetadata_Ready ( void )
{
	PyTypeObject *type = PyFLAC_type(StreamMetadata);

	type->tp_dealloc = (destructor) flac_StreamMetadata_dealloc;
	type->tp_flags = Py_TPFLAGS_DEFAULT;
	type->tp_doc = "FLAC StreamMetadata";
	type->tp_richcompare = (richcmpfunc) flac_StreamMetadata_richcompare;
	type->tp_methods = flac_StreamMetadata_methods;
	type->tp_getset = flac_StreamMetadata_getseters;
	type->tp_dict = NULL;
	type->tp_new = flac_StreamMetadata_new;

	return PyType_Ready(type);
}


struct flac_StreamMetadataDataType {
	PyTypeObject *type;
	char *tp_doc;
	newfunc tp_new;
	PyMethodDef *tp_methods;
	PyGetSetDef tp_getset[];
};


static int
flac_StreamMetadataData_Ready (struct flac_StreamMetadataDataType *type)
{
	type->type->tp_dealloc = (destructor) flac_StreamMetadata_dealloc;
	type->type->tp_flags = Py_TPFLAGS_DEFAULT;
	type->type->tp_doc = type->tp_doc;
	type->type->tp_methods = type->tp_methods;
	type->type->tp_getset = type->tp_getset;
	type->type->tp_base = PyFLAC_type(StreamMetadata);
	type->type->tp_dict = NULL;
	type->type->tp_new = type->tp_new;

	return PyType_Ready(type->type);
}


static PyObject *
flac_StreamMetadataStreamInfo_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	return flac_StreamMetadata__new(type, args, kwds, FLAC__METADATA_TYPE_STREAMINFO);
}


static PyObject *
flac_StreamMetadataStreamInfo_get (PyFLAC_StreamMetadataObject *self, void *closure)
{
	char string[33], *hex;
	int i;

	switch ((flac_StreamMetadataStreamInfo_member) closure)
	{
		case flac_StreamMetadataStreamInfo_min_blocksize:
			return PyFLAC_unsigned(self->metadata->data.stream_info.min_blocksize);
		case flac_StreamMetadataStreamInfo_max_blocksize:
			return PyFLAC_unsigned(self->metadata->data.stream_info.max_blocksize);
		case flac_StreamMetadataStreamInfo_min_framesize:
			return PyFLAC_unsigned(self->metadata->data.stream_info.min_framesize);
		case flac_StreamMetadataStreamInfo_max_framesize:
			return PyFLAC_unsigned(self->metadata->data.stream_info.max_framesize);
		case flac_StreamMetadataStreamInfo_sample_rate:
			return PyFLAC_unsigned(self->metadata->data.stream_info.sample_rate);
		case flac_StreamMetadataStreamInfo_channels:
			return PyFLAC_unsigned(self->metadata->data.stream_info.channels);
		case flac_StreamMetadataStreamInfo_bits_per_sample:
			return PyFLAC_unsigned(self->metadata->data.stream_info.bits_per_sample);
		case flac_StreamMetadataStreamInfo_total_samples:
			return PyFLAC_uint64(self->metadata->data.stream_info.total_samples);
		case flac_StreamMetadataStreamInfo_md5sum:
			for(i = 0, hex = string; i < 16; i++, hex += 2)
				sprintf(hex, "%02x", self->metadata->data.stream_info.md5sum[i]);
			string[32] = '\0';
			return PyFLAC_string(string);
		default:
			return PyFLAC_getter_error(StreamMetadataStreamInfo);
	}
}


static int
flac_StreamMetadataStreamInfo_set_md5sum (PyFLAC_StreamMetadataObject *self, PyObject *value)
{
	FLAC__byte md5sum[16], b;
	char *md5sum_hex, c;
	size_t i;

	value = PyTuple_Pack(1, value);
	if (!value || !PyArg_ParseTuple(value, "es", "ascii", &md5sum_hex))
	{
		Py_XDECREF(value);
		return -1;
	}

	Py_DECREF(value);

	i = strlen(md5sum_hex);
	if (i == 32)
		for (i = 0; i < 32; i++)
		{
			c = md5sum_hex[i];

			if (c >= '0' && c <= '9')
				b = c - '0';
			else if (c >= 'a' && c <= 'f')
				b = c - 'a' + 10;
			else if (c >= 'A' && c <= 'F')
				b = c - 'A' + 10;
			else
				break;

			if (i & 1)
				md5sum[i / 2] |= b;
			else
				md5sum[i / 2] = b << 4;
		}

	PyMem_Free(md5sum_hex);

	if (i != 32)
	{
		PyErr_SetString(PyExc_ValueError, "md5sum must be a hex string of length 32");
		return -1;
	}

	Py_MEMCPY(self->metadata->data.stream_info.md5sum, md5sum, 16);
	return 0;
}


static int
flac_StreamMetadataStreamInfo_set (PyFLAC_StreamMetadataObject *self, PyObject *value, void *closure)
{
	flac_StreamMetadataStreamInfo_member member = (flac_StreamMetadataStreamInfo_member) closure;

	if (member < flac_StreamMetadataStreamInfo_member_count)
		PyFLAC_CHECK_delete(value, flac_StreamMetadataData_setters[FLAC__METADATA_TYPE_STREAMINFO].kwlist[member]);

	switch (member)
	{
		case flac_StreamMetadataStreamInfo_min_blocksize:
			return _unsigned(value, &self->metadata->data.stream_info.min_blocksize) ? 0 : -1;
		case flac_StreamMetadataStreamInfo_max_blocksize:
			return _unsigned(value, &self->metadata->data.stream_info.max_blocksize) ? 0 : -1;
		case flac_StreamMetadataStreamInfo_min_framesize:
			return _unsigned(value, &self->metadata->data.stream_info.min_framesize) ? 0 : -1;
		case flac_StreamMetadataStreamInfo_max_framesize:
			return _unsigned(value, &self->metadata->data.stream_info.max_framesize) ? 0 : -1;
		case flac_StreamMetadataStreamInfo_sample_rate:
			return _unsigned(value, &self->metadata->data.stream_info.sample_rate) ? 0 : -1;
		case flac_StreamMetadataStreamInfo_channels:
			return _unsigned(value, &self->metadata->data.stream_info.channels) ? 0 : -1;
		case flac_StreamMetadataStreamInfo_bits_per_sample:
			return _unsigned(value, &self->metadata->data.stream_info.bits_per_sample) ? 0 : -1;
		case flac_StreamMetadataStreamInfo_total_samples:
			return _uint64(value, &self->metadata->data.stream_info.total_samples) ? 0 : -1;
		case flac_StreamMetadataStreamInfo_md5sum:
			return flac_StreamMetadataStreamInfo_set_md5sum(self, value);
		default:
			return PyFLAC_setter_error(StreamMetadataStreamInfo);
	}
}


static struct flac_StreamMetadataDataType flac_StreamMetadataStreamInfoType = {
	PyFLAC_type(StreamMetadataStreamInfo),
	"FLAC StreamMetadata_StreamInfo",
	flac_StreamMetadataStreamInfo_new,
	NULL,
	{{
		"min_blocksize",
		(getter) flac_StreamMetadataStreamInfo_get,
		(setter) flac_StreamMetadataStreamInfo_set,
		"StreamMetadataStreamInfo min_blocksize",
		(void *) flac_StreamMetadataStreamInfo_min_blocksize
	}, {
		"max_blocksize",
		(getter) flac_StreamMetadataStreamInfo_get,
		(setter) flac_StreamMetadataStreamInfo_set,
		"StreamMetadataStreamInfo max_blocksize",
		(void *) flac_StreamMetadataStreamInfo_max_blocksize
	}, {
		"min_framesize",
		(getter) flac_StreamMetadataStreamInfo_get,
		(setter) flac_StreamMetadataStreamInfo_set,
		"StreamMetadataStreamInfo min_framesize",
		(void *) flac_StreamMetadataStreamInfo_min_framesize
	}, {
		"max_framesize",
		(getter) flac_StreamMetadataStreamInfo_get,
		(setter) flac_StreamMetadataStreamInfo_set,
		"StreamMetadataStreamInfo max_framesize",
		(void *) flac_StreamMetadataStreamInfo_max_framesize
	}, {
		"sample_rate",
		(getter) flac_StreamMetadataStreamInfo_get,
		(setter) flac_StreamMetadataStreamInfo_set,
		"StreamMetadataStreamInfo sample_rate",
		(void *) flac_StreamMetadataStreamInfo_sample_rate
	}, {
		"channels",
		(getter) flac_StreamMetadataStreamInfo_get,
		(setter) flac_StreamMetadataStreamInfo_set,
		"StreamMetadataStreamInfo channels",
		(void *) flac_StreamMetadataStreamInfo_channels
	}, {
		"bits_per_sample",
		(getter) flac_StreamMetadataStreamInfo_get,
		(setter) flac_StreamMetadataStreamInfo_set,
		"StreamMetadataStreamInfo bits_per_sample",
		(void *) flac_StreamMetadataStreamInfo_bits_per_sample
	}, {
		"total_samples",
		(getter) flac_StreamMetadataStreamInfo_get,
		(setter) flac_StreamMetadataStreamInfo_set,
		"StreamMetadataStreamInfo total_samples",
		(void *) flac_StreamMetadataStreamInfo_total_samples
	}, {
		"md5sum",
		(getter) flac_StreamMetadataStreamInfo_get,
		(setter) flac_StreamMetadataStreamInfo_set,
		"StreamMetadataStreamInfo md5sum",
		(void *) flac_StreamMetadataStreamInfo_md5sum
	}, { NULL }}		/* Sentinel */
};


static PyObject *
flac_StreamMetadataPadding_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	return flac_StreamMetadata__new(type, args, kwds, FLAC__METADATA_TYPE_PADDING);
}


static int
flac_StreamMetadataPadding_set (PyFLAC_StreamMetadataObject *self, PyObject *value, void *closure)
{
	if (closure != (void *) flac_StreamMetadataPadding_length)
		return PyFLAC_setter_error(StreamMetadataPadding);

	PyFLAC_CHECK_delete(value, flac_StreamMetadataData_setters[FLAC__METADATA_TYPE_PADDING].kwlist[0]);

	return _unsigned(value, &self->metadata->length) ? 0 : -1;
}


static struct flac_StreamMetadataDataType flac_StreamMetadataPaddingType = {
	PyFLAC_type(StreamMetadataPadding),
	"FLAC StreamMetadata_Padding",
	flac_StreamMetadataPadding_new,
	NULL,
	{{
		"length",
		(getter) flac_StreamMetadata_get_length,
		(setter) flac_StreamMetadataPadding_set,
		"StreamMetadataPadding length",
		(void *) flac_StreamMetadataPadding_length
	}, { NULL }}		/* Sentinel */
};


static PyObject *
flac_StreamMetadataApplication_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	return flac_StreamMetadata__new(type, args, kwds, FLAC__METADATA_TYPE_APPLICATION);
}


static PyObject *
flac_StreamMetadataApplication_get (PyFLAC_StreamMetadataObject *self, void *closure)
{
	switch ((flac_StreamMetadataApplication_member) closure)
	{
		case flac_StreamMetadataApplication_id:
			return PyFLAC_data(self->metadata->data.application.id, 4);
		case flac_StreamMetadataApplication_data:
			return PyFLAC_data(self->metadata->data.application.data, self->metadata->length - FLAC__STREAM_METADATA_APPLICATION_ID_LEN / 8);
		default:
			return PyFLAC_getter_error(StreamMetadataApplication);
	}
}


static int
flac_StreamMetadataApplication_set_data (PyFLAC_StreamMetadataObject *self, PyObject *data)
{
	FLAC__byte *bytes;
	Py_ssize_t length;

	Py_INCREF(data);
	if (PyUnicode_Check(data))
	{
		Py_DECREF(data);
		if (!(data = PyUnicode_AsUTF8String(data)))
			return -1;
	}

	if (!flac_list_of_bytes(data, &bytes, &length))
	{
		Py_DECREF(data);
		return -1;
	}

	Py_DECREF(data);

	FLAC__metadata_object_application_set_data(self->metadata, bytes, (unsigned) length, false);
	return 0;
}


static int
flac_StreamMetadataApplication_set (PyFLAC_StreamMetadataObject *self, PyObject *value, void *closure)
{
	flac_StreamMetadataApplication_member member = (flac_StreamMetadataApplication_member) closure;

	if (member < flac_StreamMetadataApplication_member_count)
		PyFLAC_CHECK_delete(value, flac_StreamMetadataData_setters[FLAC__METADATA_TYPE_APPLICATION].kwlist[member]);

	switch (member)
	{
		case flac_StreamMetadataApplication_id:
			return _application_id(value, &self->metadata->data.application.id) ? 0 : -1;
		case flac_StreamMetadataApplication_data:
			return flac_StreamMetadataApplication_set_data(self, value);
		default:
			return PyFLAC_setter_error(StreamMetadataApplication);
	}
}


static struct flac_StreamMetadataDataType flac_StreamMetadataApplicationType = {
	PyFLAC_type(StreamMetadataApplication),
	"FLAC StreamMetadata_Application",
	flac_StreamMetadataApplication_new,
	NULL,
	{{
		"id",
		(getter) flac_StreamMetadataApplication_get,
		(setter) flac_StreamMetadataApplication_set,
		"StreamMetadataApplication id",
		(void *) flac_StreamMetadataApplication_id
	}, {
		"data",
		(getter) flac_StreamMetadataApplication_get,
		(setter) flac_StreamMetadataApplication_set,
		"StreamMetadataApplication data",
		(void *) flac_StreamMetadataApplication_data
	}, { NULL }}		/* Sentinel */
};


typedef struct {
	PyObject_HEAD
	PyObject *sample_number, *stream_offset, *frame_samples;
	FLAC__StreamMetadata_SeekPoint data;
} flac_StreamMetadataSeekPointObject;


static PyTypeObject PyFLAC_StreamMetadataSeekPointType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(StreamMetadataSeekPoint),
	sizeof(flac_StreamMetadataSeekPointObject),
};


static void
flac_StreamMetadataSeekPoint_dealloc (flac_StreamMetadataSeekPointObject *self)
{
	Py_DECREF(self->sample_number);
	Py_DECREF(self->stream_offset);
	Py_DECREF(self->frame_samples);
	PyObject_Del(self);
}


static PyObject *
flac_StreamMetadataSeekPoint_new_object (PyTypeObject *type, FLAC__StreamMetadata_SeekPoint data)
{
	flac_StreamMetadataSeekPointObject *self;

	self = PyObject_New(flac_StreamMetadataSeekPointObject, type);

	if (self) {
		self->data = data;

		self->sample_number = PyFLAC_uint64(data.sample_number);

		if (self->sample_number)
		{
			self->stream_offset = PyFLAC_uint64(data.stream_offset);

			if (self->stream_offset)
				self->frame_samples = PyFLAC_unsigned(data.frame_samples);
		}

		if (!self->sample_number || !self->stream_offset || !self->frame_samples)
		{
			Py_XDECREF(self->sample_number);
			Py_XDECREF(self->stream_offset);
			Py_DECREF(self);
			return NULL;
		}
	}

	return (PyObject *) self;
}


static PyObject *
flac_StreamMetadataSeekPoint_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	FLAC__StreamMetadata_SeekPoint data = { 0, 0, 0 };

	static char *kwlist[] = {"sample_number", "stream_offset", "frame_samples", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O&O&O&", kwlist, _uint64, &data.sample_number, _uint64, &data.stream_offset, _unsigned, &data.frame_samples))
		return NULL;

	return flac_StreamMetadataSeekPoint_new_object(type, data);
}


static PyObject *
flac_StreamMetadataSeekPoint_get (flac_StreamMetadataSeekPointObject *self, void *closure)
{
	PyObject *get;

	if (closure == (void *) 2)
		get = self->sample_number;
	else if (closure)
		get = self->stream_offset;
	else
		get = self->frame_samples;

	Py_INCREF(get);
	return get;
}


static PyGetSetDef flac_StreamMetadataSeekPoint_getseters[] = {
	{
		"sample_number",
		(getter) flac_StreamMetadataSeekPoint_get, NULL,
		"StreamMetadataSeekPoint sample number", (void *) 2
	}, {
		"stream_offset",
		(getter) flac_StreamMetadataSeekPoint_get, NULL,
		"StreamMetadataSeekPoint stream offset", (void *) 1
	}, {
		"frame_samples",
		(getter) flac_StreamMetadataSeekPoint_get, NULL,
		"StreamMetadataSeekPoint frame samples", (void *) 0
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadataSeekPoint_richcompare (flac_StreamMetadataSeekPointObject *self, PyObject *other, int op)
{
	FLAC__uint64 off1, off2, num1, num2;
	unsigned sam1, sam2;
	int comp;

	if (!PyFLAC_type_Check(other, StreamMetadataSeekPoint))
	{
		if (op != Py_EQ && op != Py_NE)
		{
			Py_INCREF(Py_NotImplemented);
			return Py_NotImplemented;
		}

		return PyFLAC_bool(op == Py_NE);
	}

	off1 = self->data.stream_offset;
	num1 = self->data.sample_number;
	sam1 = self->data.frame_samples;

	self = (flac_StreamMetadataSeekPointObject *) other;
	off2 = self->data.stream_offset;
	num2 = self->data.sample_number;
	sam2 = self->data.frame_samples;

	if (num1 < num2)
		comp = -1;
	else if (num1 > num2)
		comp = 1;
	else if (off1 < off2)
		comp = -1;
	else if (off1 > off2)
		comp = 1;
	else if (sam1 < sam2)
		comp = -1;
	else if (sam1 > sam2)
		comp = 1;
	else
		comp = 0;

	switch (op)
	{
		case Py_LT:
			comp = comp < 0;
			break;
		case Py_LE:
			comp = comp <= 0;
			break;
		case Py_EQ:
			comp = comp == 0;
			break;
		case Py_NE:
			comp = comp != 0;
			break;
		case Py_GT:
			comp = comp > 0;
			break;
		case Py_GE:
			comp = comp >= 0;
			break;
		default:
			Py_INCREF(Py_NotImplemented);
			return Py_NotImplemented;
	}

	return PyFLAC_bool(comp);
}


static int
flac_StreamMetadataSeekPoint_Ready ( void )
{
	PyTypeObject *type = PyFLAC_type(StreamMetadataSeekPoint);

	type->tp_dealloc = (destructor) flac_StreamMetadataSeekPoint_dealloc;
	type->tp_flags = Py_TPFLAGS_DEFAULT;
	type->tp_doc = "FLAC StreamMetadataSeekPoint";
	type->tp_richcompare = (richcmpfunc) flac_StreamMetadataSeekPoint_richcompare;
	type->tp_getset = flac_StreamMetadataSeekPoint_getseters;
	type->tp_dict = NULL;
	type->tp_new = flac_StreamMetadataSeekPoint_new;

	return PyType_Ready(type);
}


static PyObject *
flac_StreamMetadataSeekPoint_iternext (PyObject *iter, const void *data, Py_ssize_t index)
{
	return flac_StreamMetadataSeekPoint_new_object(PyFLAC_type(StreamMetadataSeekPoint), ((FLAC__StreamMetadata_SeekPoint *) data)[index]);
}


static PyObject *
flac_StreamMetadataSeekTable_resize_points (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned new_num_points;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &new_num_points))
		return NULL;

	if (!FLAC__metadata_object_seektable_resize_points(self->metadata, new_num_points))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataSeekTable_set_point (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	PyObject *point;
	unsigned point_num;

	if (!PyArg_ParseTuple(args, "O&O!", _unsigned, &point_num, PyFLAC_type(StreamMetadataSeekPoint), &point))
		return NULL;

	if (self->metadata->data.seek_table.num_points <= point_num)
	{
		PyErr_SetString(PyExc_IndexError, "point number out of range");
		return NULL;
	}

	FLAC__metadata_object_seektable_set_point(self->metadata, point_num, ((flac_StreamMetadataSeekPointObject *) point)->data);
	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataSeekTable_get_point (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned point_num;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &point_num))
		return NULL;

	if (self->metadata->data.seek_table.num_points <= point_num)
	{
		PyErr_SetString(PyExc_IndexError, "point number out of range");
		return NULL;
	}

	return flac_StreamMetadataSeekPoint_new_object(PyFLAC_type(StreamMetadataSeekPoint), self->metadata->data.seek_table.points[point_num]);
}


static PyObject *
flac_StreamMetadataSeekTable_insert_point (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	PyObject *point;
	unsigned point_num;

	if (!PyArg_ParseTuple(args, "O&O!", _unsigned, &point_num, PyFLAC_type(StreamMetadataSeekPoint), &point))
		return NULL;

	if (self->metadata->data.seek_table.num_points < point_num)
	{
		PyErr_SetString(PyExc_IndexError, "point number out of range");
		return NULL;
	}

	if (!FLAC__metadata_object_seektable_insert_point(self->metadata, point_num, ((flac_StreamMetadataSeekPointObject *) point)->data))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataSeekTable_delete_point (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned point_num;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &point_num))
		return NULL;

	if (self->metadata->data.seek_table.num_points <= point_num)
	{
		PyErr_SetString(PyExc_IndexError, "point number out of range");
		return NULL;
	}

	if (!FLAC__metadata_object_seektable_delete_point(self->metadata, point_num))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataSeekTable_is_legal (PyFLAC_StreamMetadataObject *self)
{
	return PyFLAC_bool(FLAC__metadata_object_seektable_is_legal(self->metadata));
}


static PyObject *
flac_StreamMetadataSeekTable_template_append_placeholders (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned num;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &num))
		return NULL;

	if (!FLAC__metadata_object_seektable_template_append_placeholders(self->metadata, num))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataSeekTable_template_append_point (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	FLAC__uint64 sample_number;

	if (!PyArg_ParseTuple(args, "O&", _uint64, &sample_number))
		return NULL;

	if (!FLAC__metadata_object_seektable_template_append_point(self->metadata, sample_number))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataSeekTable_template_append_points (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	FLAC__uint64 *sample_numbers;
	PyObject *sample_numbers_list, *item;
	Py_ssize_t num, i;

	if (!PyArg_ParseTuple(args, "O", &sample_numbers_list))
	{
		PyErr_Clear();
		sample_numbers_list = args;
	}
	else if (!PyTuple_Check(sample_numbers_list) && !PyList_Check(sample_numbers_list))
	{
		sample_numbers_list = args;
	}

	num = PySequence_Size(sample_numbers_list);
	if (num == -1)
		return NULL;

	sample_numbers = PyMem_New(FLAC__uint64, num);
	if (!sample_numbers)
		return PyErr_NoMemory();

	for (i = 0; i < num; i++)
	{
		item = PySequence_GetItem(sample_numbers_list, i);
		if (!item || !_uint64(item, &sample_numbers[i]))
			break;
	}

	if (i != num)
	{
		PyMem_Del(sample_numbers);
		return NULL;
	}

	if (!FLAC__metadata_object_seektable_template_append_points(self->metadata, sample_numbers, (unsigned) num))
	{
		PyMem_Del(sample_numbers);
		return PyErr_NoMemory();
	}

	PyMem_Del(sample_numbers);
	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataSeekTable_template_append_spaced_points (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	FLAC__uint64 total_samples;
	unsigned num;

	if (!PyArg_ParseTuple(args, "O&O&", _unsigned, &num, _uint64, &total_samples))
		return NULL;

	if (!total_samples)
	{
		PyErr_SetString(PyExc_ValueError, "the total number of samples must be positive");
		return NULL;
	}

	if (!FLAC__metadata_object_seektable_template_append_spaced_points(self->metadata, num, total_samples))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataSeekTable_template_append_spaced_points_by_samples (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	FLAC__uint64 total_samples;
	unsigned samples;

	if (!PyArg_ParseTuple(args, "O&O&", _unsigned, &samples, _uint64, &total_samples))
		return NULL;

	if (!samples)
	{
		PyErr_SetString(PyExc_ValueError, "the number of samples must be positive");
		return NULL;
	}

	if (!total_samples)
	{
		PyErr_SetString(PyExc_ValueError, "the total number of samples must be positive");
		return NULL;
	}

	if (!FLAC__metadata_object_seektable_template_append_spaced_points_by_samples(self->metadata, samples, total_samples))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataSeekTable_template_sort (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	FLAC__bool compact;

	compact = false;

	if (!PyArg_ParseTuple(args, "|O&", _bool, &compact))
		return NULL;

	if (!FLAC__metadata_object_seektable_template_sort(self->metadata, compact))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyMethodDef flac_StreamMetadataSeekTable_methods[] = {
	{
		"resize_points",
		(PyCFunction) flac_StreamMetadataSeekTable_resize_points,
		METH_VARARGS,
		"Resize points"
	}, {
		"set_point",
		(PyCFunction) flac_StreamMetadataSeekTable_set_point,
		METH_VARARGS,
		"Set point"
	}, {
		"get_point",
		(PyCFunction) flac_StreamMetadataSeekTable_get_point,
		METH_VARARGS,
		"Get point"
	}, {
		"insert_point",
		(PyCFunction) flac_StreamMetadataSeekTable_insert_point,
		METH_VARARGS,
		"Insert point"
	}, {
		"delete_point",
		(PyCFunction) flac_StreamMetadataSeekTable_delete_point,
		METH_VARARGS,
		"Delete point"
	}, {
		"is_legal",
		(PyCFunction) flac_StreamMetadataSeekTable_is_legal,
		METH_NOARGS,
		"Is legal"
	}, {
		"template_append_placeholders",
		(PyCFunction) flac_StreamMetadataSeekTable_template_append_placeholders,
		METH_VARARGS,
		"Template append placeholders"
	}, {
		"template_append_point",
		(PyCFunction) flac_StreamMetadataSeekTable_template_append_point,
		METH_VARARGS,
		"Template append point"
	}, {
		"template_append_points",
		(PyCFunction) flac_StreamMetadataSeekTable_template_append_points,
		METH_VARARGS,
		"Template append points"
	}, {
		"template_append_spaced_points",
		(PyCFunction) flac_StreamMetadataSeekTable_template_append_spaced_points,
		METH_VARARGS,
		"Template append spaced points"
	}, {
		"template_append_spaced_points_by_samples",
		(PyCFunction) flac_StreamMetadataSeekTable_template_append_spaced_points_by_samples,
		METH_VARARGS,
		"Template append spaced points by samples"
	}, {
		"template_sort",
		(PyCFunction) flac_StreamMetadataSeekTable_template_sort,
		METH_VARARGS,
		"Template sort"
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadataSeekTable_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	return flac_StreamMetadata__new(type, args, kwds, FLAC__METADATA_TYPE_SEEKTABLE);
}


static PyObject *
flac_StreamMetadataSeekTable_get_points (PyFLAC_StreamMetadataObject *self, void *closure)
{
	return flac_Iter_New(StreamMetadataSeekPoint, self->metadata->data.seek_table.points, self->metadata->data.seek_table.num_points, self);
}


static int
flac_StreamMetadataSeekTable_set (PyFLAC_StreamMetadataObject *self, PyObject *points, void *closure)
{
	PyObject **point_list;
	Py_ssize_t point_count, i;

	if (closure != (void *) flac_StreamMetadataSeekTable_points)
		return PyFLAC_setter_error(StreamMetadataSeekTable);

	PyFLAC_CHECK_delete(points, flac_StreamMetadataData_setters[FLAC__METADATA_TYPE_SEEKTABLE].kwlist[0]);

	point_list = NULL;

	if (!flac_list_of_type(points, PyFLAC_type(StreamMetadataSeekPoint), &point_list, &point_count))
		return -1;

	if (!FLAC__metadata_object_seektable_resize_points(self->metadata, (unsigned) point_count))
	{
		flac_free_list_of_type(point_list, point_count);
		PyErr_NoMemory();
		return -1;
	}

	if (point_list != NULL)
	{
		for (i = 0; i < point_count; i++)
			FLAC__metadata_object_seektable_set_point(self->metadata, i, ((flac_StreamMetadataSeekPointObject *) point_list[i])->data);
		flac_free_list_of_type(point_list, point_count);
	}
	else if (point_count)
	{
		FLAC__metadata_object_seektable_set_point(self->metadata, 0, ((flac_StreamMetadataSeekPointObject *) points)->data);
	}

	return 0;
}


static struct flac_StreamMetadataDataType flac_StreamMetadataSeekTableType = {
	PyFLAC_type(StreamMetadataSeekTable),
	"FLAC StreamMetadata_SeekTable",
	flac_StreamMetadataSeekTable_new,
	flac_StreamMetadataSeekTable_methods,
	{{
		"points",
		(getter) flac_StreamMetadataSeekTable_get_points,
		(setter) flac_StreamMetadataSeekTable_set,
		"StreamMetadataSeekTable id",
		(void *) flac_StreamMetadataSeekTable_points
	}, { NULL }}		/* Sentinel */
};


/* TODO - Create real string subtype */
static PyTypeObject PyFLAC_StreamMetadataVorbisCommentEntryType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(StreamMetadataVorbisCommentEntry),
	sizeof(PyStringObject) + sizeof(FLAC__StreamMetadata_VorbisComment_Entry),
};


static FLAC__StreamMetadata_VorbisComment_Entry *
flac_StreamMetadataVorbisCommentEntry_entry (PyObject *self)
{
	void *ptr;

	ptr = self;
	ptr += sizeof(PyStringObject);
	ptr += PyString_GET_SIZE(self) * PyString_Type.tp_itemsize;

	return (FLAC__StreamMetadata_VorbisComment_Entry *) ptr;
}


static void
flac_StreamMetadataVorbisCommentEntry_dealloc (PyObject *self)
{
	FLAC__StreamMetadata_VorbisComment_Entry *entry;

	entry = flac_StreamMetadataVorbisCommentEntry_entry(self);
	if (entry->entry)
		PyMem_Del(entry->entry);

	PyString_Type.tp_dealloc(self);
}


static PyObject *
flac_StreamMetadataVorbisCommentEntry_new_object (PyTypeObject *type, FLAC__StreamMetadata_VorbisComment_Entry *entry)
{
	FLAC__StreamMetadata_VorbisComment_Entry *self_entry;
	PyObject *self;

	self = PyString_Type.tp_new(type, Py_BuildValue("(s)", entry->entry), NULL);

	if (self)
	{
		self_entry = flac_StreamMetadataVorbisCommentEntry_entry(self);
		self_entry->length = entry->length;

		if (entry->entry)
		{
			self_entry->entry = PyMem_New(FLAC__byte, entry->length + 1);

			if (!self_entry->entry)
			{
				Py_DECREF(self);
				return PyErr_NoMemory();
			}

			Py_MEMCPY(self_entry->entry, entry->entry, entry->length);
			self_entry->entry[entry->length] = '\0';
		}
		else
		{
			self_entry->entry = NULL;
		}
	}

	return self;
}


static PyObject *
flac_StreamMetadataVorbisCommentEntry_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	FLAC__StreamMetadata_VorbisComment_Entry entry;
	PyObject *self;
	const char *field_name, *field_value;

	(void)kwds;

	field_value = NULL;
	if (!PyArg_ParseTuple(args, "s|s", &field_name, &field_value))
		return NULL;

	if (field_value)
	{
		if (!FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, field_name, field_value))
		{
			if (!FLAC__format_vorbiscomment_entry_name_is_legal(field_name))
			{
				PyErr_SetString(PyExc_ValueError, "field_name does not comply with the Vorbis comment specification");
				return NULL;
			}
			else if (!FLAC__format_vorbiscomment_entry_value_is_legal((FLAC__byte *) field_value, strlen(field_value)))
			{
				PyErr_SetString(PyExc_ValueError, "field_value does not comply with the Vorbis comment specification");
				return NULL;
			}
			else
			{
				return PyErr_NoMemory();
			}
		}
	}
	else
	{
		entry.length = strlen(field_name);
		entry.entry = PyMem_New(FLAC__byte, entry.length + 1);

		if (!entry.entry)
			return PyErr_NoMemory();

		Py_MEMCPY(entry.entry, field_name, entry.length);
		entry.entry[entry.length] = '\0';
	}

	self = flac_StreamMetadataVorbisCommentEntry_new_object(type, &entry);

	PyMem_Del(entry.entry);

	return self;
}


static PyObject *
flac_StreamMetadataVorbisCommentEntry_is_legal (PyObject *self)
{
	FLAC__StreamMetadata_VorbisComment_Entry *entry;

	entry = flac_StreamMetadataVorbisCommentEntry_entry(self);

	return PyFLAC_bool(FLAC__format_vorbiscomment_entry_is_legal(entry->entry, entry->length));
}


static PyObject *
flac_StreamMetadataVorbisCommentEntry_to_name_value_pair (PyObject *self)
{
	FLAC__StreamMetadata_VorbisComment_Entry *entry;
	PyObject *pair;
	char *field_name, *field_value;

	entry = flac_StreamMetadataVorbisCommentEntry_entry(self);

	if (!FLAC__metadata_object_vorbiscomment_entry_to_name_value_pair(*entry, &field_name, &field_value))
	{
		if (FLAC__format_vorbiscomment_entry_is_legal(entry->entry, entry->length))
			return PyErr_NoMemory();
		else
			Py_RETURN_NONE;
	}

	pair = Py_BuildValue("(ss)", field_name, field_value);

	free(field_name);
	free(field_value);

	return pair;
}


static PyObject *
flac_StreamMetadataVorbisCommentEntry_matches (PyObject *self, PyObject *args)
{
	FLAC__StreamMetadata_VorbisComment_Entry *entry;
	const char *field_name;

	if (!PyArg_ParseTuple(args, "s", &field_name))
		return NULL;

	entry = flac_StreamMetadataVorbisCommentEntry_entry(self);

	return PyFLAC_bool(FLAC__metadata_object_vorbiscomment_entry_matches(*entry, field_name, strlen(field_name)));
}


static PyMethodDef flac_StreamMetadataVorbisCommentEntry_methods[] = {
	{
		"is_legal",
		(PyCFunction) flac_StreamMetadataVorbisCommentEntry_is_legal,
		METH_NOARGS,
		"Is legal"
	}, {
		"to_name_value_pair",
		(PyCFunction) flac_StreamMetadataVorbisCommentEntry_to_name_value_pair,
		METH_NOARGS,
		"To name value pair"
	}, {
		"matches",
		(PyCFunction) flac_StreamMetadataVorbisCommentEntry_matches,
		METH_VARARGS,
		"Matches"
	}, { NULL }		/* Sentinel */
};


static int
flac_StreamMetadataVorbisCommentEntry_Ready ( void )
{
	PyTypeObject *type = PyFLAC_type(StreamMetadataVorbisCommentEntry);

	type->tp_dealloc = (destructor) flac_StreamMetadataVorbisCommentEntry_dealloc;
	type->tp_flags = Py_TPFLAGS_DEFAULT;
	type->tp_doc = "FLAC StreamMetadataVorbisCommentEntry";
	type->tp_methods = flac_StreamMetadataVorbisCommentEntry_methods;
	type->tp_base = &PyString_Type;
	type->tp_dict = NULL;
	type->tp_new = flac_StreamMetadataVorbisCommentEntry_new;

	return PyType_Ready(type);
}


static PyObject *
flac_StreamMetadataVorbisCommentEntry_iternext (PyObject *iter, const void *data, Py_ssize_t index)
{
	return flac_StreamMetadataVorbisCommentEntry_new_object(PyFLAC_type(StreamMetadataVorbisCommentEntry), &((FLAC__StreamMetadata_VorbisComment_Entry *) data)[index]);
}


static PyObject *
flac_StreamMetadataVorbisComment_resize_comments (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned new_num_comments;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &new_num_comments))
		return NULL;

	if (!FLAC__metadata_object_vorbiscomment_resize_comments(self->metadata, new_num_comments))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataVorbisComment_set_comment (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	FLAC__StreamMetadata_VorbisComment_Entry *entry;
	PyObject *py_entry;
	unsigned comment_num;

	if (!PyArg_ParseTuple(args, "O&O!", _unsigned, &comment_num, PyFLAC_type(StreamMetadataVorbisCommentEntry), &py_entry))
		return NULL;

	if (self->metadata->data.vorbis_comment.num_comments <= comment_num)
	{
		PyErr_SetString(PyExc_IndexError, "comment number out of range");
		return NULL;
	}

	entry = flac_StreamMetadataVorbisCommentEntry_entry(py_entry);

	if (FLAC__metadata_object_vorbiscomment_set_comment(self->metadata, comment_num, *entry, true))
		Py_RETURN_TRUE;

	if (FLAC__format_vorbiscomment_entry_is_legal(entry->entry, entry->length))
		return PyErr_NoMemory();

	Py_RETURN_FALSE;
}


static PyObject *
flac_StreamMetadataVorbisComment_get_comment (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned comment_num;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &comment_num))
		return NULL;

	if (self->metadata->data.vorbis_comment.num_comments <= comment_num)
	{
		PyErr_SetString(PyExc_IndexError, "comment number out of range");
		return NULL;
	}

	return flac_StreamMetadataVorbisCommentEntry_new_object(PyFLAC_type(StreamMetadataVorbisCommentEntry), &self->metadata->data.vorbis_comment.comments[comment_num]);
}


static PyObject *
flac_StreamMetadataVorbisComment_insert_comment (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	FLAC__StreamMetadata_VorbisComment_Entry *entry;
	PyObject *py_entry;
	unsigned comment_num;

	if (!PyArg_ParseTuple(args, "O&O!", _unsigned, &comment_num, PyFLAC_type(StreamMetadataVorbisCommentEntry), &py_entry))
		return NULL;

	if (self->metadata->data.vorbis_comment.num_comments < comment_num)
	{
		PyErr_SetString(PyExc_IndexError, "comment number out of range");
		return NULL;
	}

	entry = flac_StreamMetadataVorbisCommentEntry_entry(py_entry);

	if (FLAC__metadata_object_vorbiscomment_insert_comment(self->metadata, comment_num, *entry, true))
		Py_RETURN_TRUE;

	if (FLAC__format_vorbiscomment_entry_is_legal(entry->entry, entry->length))
		return PyErr_NoMemory();

	Py_RETURN_FALSE;
}


static PyObject *
flac_StreamMetadataVorbisComment_append_comment (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	FLAC__StreamMetadata_VorbisComment_Entry *entry;
	PyObject *py_entry;

	if (!PyArg_ParseTuple(args, "O!", PyFLAC_type(StreamMetadataVorbisCommentEntry), &py_entry))
		return NULL;

	entry = flac_StreamMetadataVorbisCommentEntry_entry(py_entry);

	if (FLAC__metadata_object_vorbiscomment_append_comment(self->metadata, *entry, true))
		Py_RETURN_TRUE;

	if (FLAC__format_vorbiscomment_entry_is_legal(entry->entry, entry->length))
		return PyErr_NoMemory();

	Py_RETURN_FALSE;
}


static PyObject *
flac_StreamMetadataVorbisComment_replace_comment (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	FLAC__bool all;
	FLAC__StreamMetadata_VorbisComment_Entry *entry;
	PyObject *py_entry;

	all = true;

	if (!PyArg_ParseTuple(args, "O!|O&", PyFLAC_type(StreamMetadataVorbisCommentEntry), &py_entry, _bool, &all))
		return NULL;

	entry = flac_StreamMetadataVorbisCommentEntry_entry(py_entry);

	if (FLAC__metadata_object_vorbiscomment_replace_comment(self->metadata, *entry, all, true))
		Py_RETURN_TRUE;

	if (FLAC__format_vorbiscomment_entry_is_legal(entry->entry, entry->length))
		return PyErr_NoMemory();

	Py_RETURN_FALSE;
}


static PyObject *
flac_StreamMetadataVorbisComment_delete_comment (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned comment_num;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &comment_num))
		return NULL;

	if (self->metadata->data.vorbis_comment.num_comments <= comment_num)
	{
		PyErr_SetString(PyExc_IndexError, "comment number out of range");
		return NULL;
	}

	if (!FLAC__metadata_object_vorbiscomment_delete_comment(self->metadata, comment_num))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataVorbisComment_find_entry_from (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	const char *field_name;
	unsigned offset;

	offset = 0;

	if (!PyArg_ParseTuple(args, "s|O&", &field_name, _unsigned, &offset))
		return NULL;

	return PyFLAC_int(FLAC__metadata_object_vorbiscomment_find_entry_from(self->metadata, offset, field_name));
}


static PyObject *
flac_StreamMetadataVorbisComment_remove_entry_matching (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	const char *field_name;
	int result;

	if (!PyArg_ParseTuple(args, "s", &field_name))
		return NULL;

	result = FLAC__metadata_object_vorbiscomment_remove_entry_matching(self->metadata, field_name);

	if (result < 0)
		return PyErr_NoMemory();

	return PyFLAC_bool(result);
}


static PyObject *
flac_StreamMetadataVorbisComment_remove_entries_matching (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	const char *field_name;
	int result;

	if (!PyArg_ParseTuple(args, "s", &field_name))
		return NULL;

	result = FLAC__metadata_object_vorbiscomment_remove_entries_matching(self->metadata, field_name);

	if (result < 0)
		return PyErr_NoMemory();

	return PyFLAC_int(result);
}


static PyMethodDef flac_StreamMetadataVorbisComment_methods[] = {
	{
		"resize_comments",
		(PyCFunction) flac_StreamMetadataVorbisComment_resize_comments,
		METH_VARARGS,
		"Resize comments"
	}, {
		"set_comment",
		(PyCFunction) flac_StreamMetadataVorbisComment_set_comment,
		METH_VARARGS,
		"Set comment"
	}, {
		"get_comment",
		(PyCFunction) flac_StreamMetadataVorbisComment_get_comment,
		METH_VARARGS,
		"Get comment"
	}, {
		"insert_comment",
		(PyCFunction) flac_StreamMetadataVorbisComment_insert_comment,
		METH_VARARGS,
		"Insert comment"
	}, {
		"append_comment",
		(PyCFunction) flac_StreamMetadataVorbisComment_append_comment,
		METH_VARARGS,
		"Append comment"
	}, {
		"replace_comment",
		(PyCFunction) flac_StreamMetadataVorbisComment_replace_comment,
		METH_VARARGS,
		"Replace comment"
	}, {
		"delete_comment",
		(PyCFunction) flac_StreamMetadataVorbisComment_delete_comment,
		METH_VARARGS,
		"Delete comment"
	}, {
		"find_entry_from",
		(PyCFunction) flac_StreamMetadataVorbisComment_find_entry_from,
		METH_VARARGS,
		"Find entry from"
	}, {
		"remove_entry_matching",
		(PyCFunction) flac_StreamMetadataVorbisComment_remove_entry_matching,
		METH_VARARGS,
		"Remove entry matching"
	}, {
		"remove_entries_matching",
		(PyCFunction) flac_StreamMetadataVorbisComment_remove_entries_matching,
		METH_VARARGS,
		"Remove entries matching"
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadataVorbisComment_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	return flac_StreamMetadata__new(type, args, kwds, FLAC__METADATA_TYPE_VORBIS_COMMENT);
}


static int
flac_StreamMetadataVorbisComment_set_vendor_string (PyFLAC_StreamMetadataObject *self, PyObject *py_entry)
{
	FLAC__StreamMetadata_VorbisComment_Entry *entry;
	PyObject *args;
	const char *tp_name;

	if (!py_entry)
	{
		PyErr_SetString(PyExc_TypeError, "cannot delete the vendor string");
		return -1;
	}

	Py_INCREF(py_entry);
	if (!PyFLAC_type_Check(py_entry, StreamMetadataVorbisCommentEntry))
	{
		args = PyTuple_Pack(1, py_entry);
		Py_DECREF(py_entry);
		if (!args)
			return -1;

		tp_name = Py_TYPE(py_entry)->tp_name;

		py_entry = flac_StreamMetadataVorbisCommentEntry_new(PyFLAC_type(StreamMetadataVorbisCommentEntry), args, NULL);
		Py_DECREF(args);
		if (!py_entry)
			PyErr_Format(PyExc_TypeError, "must be " PyFLAC_name(StreamMetadataVorbisCommentEntry) ", not %.50s", tp_name);
	}

	if (py_entry)
	{
		entry = flac_StreamMetadataVorbisCommentEntry_entry(py_entry);

		if (FLAC__metadata_object_vorbiscomment_set_vendor_string(self->metadata, *entry, true))
		{
			Py_DECREF(py_entry);
			return 0;
		}

		if (FLAC__format_vorbiscomment_entry_is_legal(entry->entry, entry->length))
			PyErr_NoMemory();
		else
			PyErr_SetString(PyExc_ValueError, "the entry is not legal");

		Py_DECREF(py_entry);
	}

	return -1;
}


static int
flac_StreamMetadataVorbisComment_set_comments (PyFLAC_StreamMetadataObject *self, PyObject *comments)
{
	FLAC__StreamMetadata_VorbisComment_Entry *entry;
	PyObject **comment_list;
	Py_ssize_t comment_count, i, invalid;
	int result;

	comment_list = NULL;
	result = 0;

	if (!comments)
	{
		PyErr_SetString(PyExc_TypeError, "cannot delete the comments");
		result = -1;
	}
	else if (!flac_list_of_type(comments, PyFLAC_type(StreamMetadataVorbisCommentEntry), &comment_list, &comment_count))
	{
		result = -1;
	}
	else if (!FLAC__metadata_object_vorbiscomment_resize_comments(self->metadata, (unsigned) comment_count))
	{
		PyErr_NoMemory();
		result = -1;
	}
	else if (comment_list != NULL)
	{
		invalid = -1;

		for (i = 0; i < comment_count; i++)
		{
			entry = flac_StreamMetadataVorbisCommentEntry_entry(comment_list[i]);

			if (!FLAC__metadata_object_vorbiscomment_set_comment(self->metadata, (unsigned) i, *entry, true))
			{
				if (FLAC__format_vorbiscomment_entry_is_legal(entry->entry, entry->length))
				{
					PyErr_NoMemory();
					result = -1;
					break;
				}

				if (invalid == -1)
					invalid = i;
				else if (invalid != comment_count)
					invalid = comment_count;
			}
		}

		if (invalid != -1 && !result)
		{
			if (invalid == comment_count)
				PyErr_SetString(PyExc_ValueError, "some entries do not comply with the Vorbis comment specification");
			else
				PyErr_Format(PyExc_ValueError, "entry at %zu does not comply with the Vorbis comment specification", invalid);
			result = -1;
		}
	}
	else if (comment_count)
	{
		entry = flac_StreamMetadataVorbisCommentEntry_entry(comments);

		if (!FLAC__metadata_object_vorbiscomment_set_comment(self->metadata, 0, *entry, true))
		{
			if (FLAC__format_vorbiscomment_entry_is_legal(entry->entry, entry->length))
				PyErr_NoMemory();
			else
				PyErr_SetString(PyExc_ValueError, "entry does not comply with the Vorbis comment specification");

			result = -1;
		}
	}

	flac_free_list_of_type(comment_list, comment_count);
	return result;
}


static PyObject *
flac_StreamMetadataVorbisComment_get (PyFLAC_StreamMetadataObject *self, void *closure)
{
	if (closure == (void *) flac_StreamMetadataVorbisComment_vendor_string)
		return flac_StreamMetadataVorbisCommentEntry_new_object(PyFLAC_type(StreamMetadataVorbisCommentEntry), &self->metadata->data.vorbis_comment.vendor_string);

	if (closure != (void *) flac_StreamMetadataVorbisComment_comments)
		return PyFLAC_getter_error(StreamMetadataVorbisComment);

	return flac_Iter_New(StreamMetadataVorbisCommentEntry, self->metadata->data.vorbis_comment.comments, self->metadata->data.vorbis_comment.num_comments, self);
}


static int
flac_StreamMetadataVorbisComment_set (PyFLAC_StreamMetadataObject *self, PyObject *value, void *closure)
{
	flac_StreamMetadataVorbisComment_member member = (flac_StreamMetadataVorbisComment_member) closure;

	if (member < flac_StreamMetadataVorbisComment_member_count)
		PyFLAC_CHECK_delete(value, flac_StreamMetadataData_setters[FLAC__METADATA_TYPE_VORBIS_COMMENT].kwlist[member]);

	switch (member)
	{
		case flac_StreamMetadataVorbisComment_vendor_string:
			return flac_StreamMetadataVorbisComment_set_vendor_string(self, value);
		case flac_StreamMetadataVorbisComment_comments:
			return flac_StreamMetadataVorbisComment_set_comments(self, value);
		default:
			return PyFLAC_setter_error(StreamMetadataVorbisComment);
	}
}


static struct flac_StreamMetadataDataType flac_StreamMetadataVorbisCommentType = {
	PyFLAC_type(StreamMetadataVorbisComment),
	"FLAC StreamMetadata_VorbisComment",
	flac_StreamMetadataVorbisComment_new,
	flac_StreamMetadataVorbisComment_methods,
	{{
		"vendor_string",
		(getter) flac_StreamMetadataVorbisComment_get,
		(setter) flac_StreamMetadataVorbisComment_set,
		"StreamMetadataVorbisComment vendor_string",
		(void *) flac_StreamMetadataVorbisComment_vendor_string
	}, {
		"comments",
		(getter) flac_StreamMetadataVorbisComment_get,
		(setter) flac_StreamMetadataVorbisComment_set,
		"StreamMetadataVorbisComment comments",
		(void *) flac_StreamMetadataVorbisComment_comments
	}, { NULL }}		/* Sentinel */
};


typedef struct {
	PyObject_HEAD
	PyObject *offset, *number;
	FLAC__StreamMetadata_CueSheet_Index data;
} flac_StreamMetadataCueSheetIndexObject;


/* TODO - Use real tuple subtype */
static PyTypeObject PyFLAC_StreamMetadataCueSheetIndexType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(StreamMetadataCueSheetIndex),
	sizeof(flac_StreamMetadataCueSheetIndexObject),
};


static void
flac_StreamMetadataCueSheetIndex_dealloc (flac_StreamMetadataCueSheetIndexObject *self)
{
	Py_DECREF(self->offset);
	Py_DECREF(self->number);
	PyObject_Del(self);
}


static PyObject *
flac_StreamMetadataCueSheetIndex_new_object (PyTypeObject *type, FLAC__StreamMetadata_CueSheet_Index data)
{
	flac_StreamMetadataCueSheetIndexObject *self;

	self = PyObject_New(flac_StreamMetadataCueSheetIndexObject, type);

	if (self) {
		self->data = data;

		self->offset = PyFLAC_uint64(data.offset);
		if (self->offset)
			self->number = PyFLAC_byte(data.number);

		if (!self->offset || !self->number)
		{
			Py_XDECREF(self->offset);
			Py_DECREF(self);
			return NULL;
		}
	}

	return (PyObject *) self;
}


static PyObject *
flac_StreamMetadataCueSheetIndex_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	FLAC__StreamMetadata_CueSheet_Index data = { 0, 0 };

	static char *kwlist[] = {"offset", "number", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O&O&", kwlist, _uint64, &data.offset, _byte, &data.number))
		return NULL;

	return flac_StreamMetadataCueSheetIndex_new_object(type, data);
}


static PyObject *
flac_StreamMetadataCueSheetIndex_get (flac_StreamMetadataCueSheetIndexObject *self, void *closure)
{
	PyObject *get;

	if (closure)
		get = self->offset;
	else
		get = self->number;

	Py_INCREF(get);
	return get;
}


static PyGetSetDef flac_StreamMetadataCueSheetIndex_getseters[] = {
	{
		"offset",
		(getter) flac_StreamMetadataCueSheetIndex_get, NULL,
		"StreamMetadataCueSheetIndex offset", (void *) 1
	}, {
		"number",
		(getter) flac_StreamMetadataCueSheetIndex_get, NULL,
		"StreamMetadataCueSheetIndex number", (void *) 0
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadataCueSheetIndex_richcompare (flac_StreamMetadataCueSheetIndexObject *self, PyObject *other, int op)
{
	FLAC__uint64 off1, off2;
	FLAC__byte num1, num2;
	int comp;

	if (!PyFLAC_type_Check(other, StreamMetadataCueSheetIndex))
	{
		if (op != Py_EQ && op != Py_NE)
		{
			Py_INCREF(Py_NotImplemented);
			return Py_NotImplemented;
		}

		return PyFLAC_bool(op == Py_NE);
	}

	off1 = self->data.offset;
	num1 = self->data.number;

	self = (flac_StreamMetadataCueSheetIndexObject *) other;
	off2 = self->data.offset;
	num2 = self->data.number;

	if (off1 < off2)
		comp = -1;
	else if (off1 > off2)
		comp = 1;
	else if (num1 < num2)
		comp = -1;
	else if (num1 > num2)
		comp = 1;
	else
		comp = 0;

	switch (op)
	{
		case Py_LT:
			comp = comp < 0;
			break;
		case Py_LE:
			comp = comp <= 0;
			break;
		case Py_EQ:
			comp = comp == 0;
			break;
		case Py_NE:
			comp = comp != 0;
			break;
		case Py_GT:
			comp = comp > 0;
			break;
		case Py_GE:
			comp = comp >= 0;
			break;
		default:
			Py_INCREF(Py_NotImplemented);
			return Py_NotImplemented;
	}

	return PyFLAC_bool(comp);
}


static int
flac_StreamMetadataCueSheetIndex_Ready ( void )
{
	PyTypeObject *type = PyFLAC_type(StreamMetadataCueSheetIndex);

	type->tp_dealloc = (destructor) flac_StreamMetadataCueSheetIndex_dealloc;
	type->tp_flags = Py_TPFLAGS_DEFAULT;
	type->tp_doc = "FLAC StreamMetadataCueSheetIndex";
	type->tp_richcompare = (richcmpfunc) flac_StreamMetadataCueSheetIndex_richcompare;
	type->tp_getset = flac_StreamMetadataCueSheetIndex_getseters;
	type->tp_dict = NULL;
	type->tp_new = flac_StreamMetadataCueSheetIndex_new;

	return PyType_Ready(type);
}


static PyObject *
flac_StreamMetadataCueSheetIndex_iternext (PyObject *iter, const void *data, Py_ssize_t index)
{
	return flac_StreamMetadataCueSheetIndex_new_object(PyFLAC_type(StreamMetadataCueSheetIndex), ((FLAC__StreamMetadata_CueSheet_Index *) data)[index]);
}


typedef struct {
	PyFLAC_StreamMetadataObject cuesheet;
	FLAC__StreamMetadata_CueSheet_Track *data;
} flac_StreamMetadataCueSheetTrackObject;


static PyTypeObject PyFLAC_StreamMetadataCueSheetTrackType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(StreamMetadataCueSheetTrack),
	sizeof(flac_StreamMetadataCueSheetTrackObject),
};


static PyObject *
flac_StreamMetadataCueSheetTrack_new_object (PyTypeObject *type, FLAC__StreamMetadata_CueSheet_Track *track)
{
	FLAC__bool use_new;
	flac_StreamMetadataCueSheetTrackObject *self;

	self = PyObject_New(flac_StreamMetadataCueSheetTrackObject, type);

	if (!self)
		return NULL;

	self->cuesheet.metadata = FLAC__metadata_object_new(FLAC__METADATA_TYPE_CUESHEET);

	if (self->cuesheet.metadata)
	{
		self->cuesheet.metadata_type = NULL;

		if ((use_new = !track))
			track = FLAC__metadata_object_cuesheet_track_new();

		if (track)
		{
			if (FLAC__metadata_object_cuesheet_insert_track(self->cuesheet.metadata, 0, track, !use_new))
			{
				self->data = &self->cuesheet.metadata->data.cue_sheet.tracks[0];
				return (PyObject *) self;
			}

			if (use_new)
				FLAC__metadata_object_cuesheet_track_delete(track);
		}
	}

	Py_DECREF(self);
	return PyErr_NoMemory();
}


static PyObject *
flac_StreamMetadataCueSheetTrack_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	(void)args, (void)kwds;

	return flac_StreamMetadataCueSheetTrack_new_object(type, NULL);
}


static int
flac_StreamMetadataCueSheetTrack_copy_isrc (FLAC__StreamMetadata_CueSheet_Track *data, char *isrc)
{
	switch (strlen(isrc))
	{
		case 0:
			isrc = "\0\0\0\0\0\0\0\0\0\0\0\0";
		case 12:
			Py_MEMCPY(&data->isrc, isrc, 12);
			return 0;
		default:
			PyErr_SetString(PyExc_ValueError, "isrc must be of length 0 or 12");
			return -1;
	}
}


static int flac_StreamMetadataCueSheetTrack_set_indices (flac_StreamMetadataCueSheetTrackObject *, PyObject *, void *);

static int
flac_StreamMetadataCueSheetTrack_init (flac_StreamMetadataCueSheetTrackObject *self, PyObject *args, PyObject *kwds)
{
	FLAC__bool type, pre_emphasis;
	FLAC__StreamMetadata_CueSheet_Track *data;
	PyObject *indices;
	char *isrc;
	int b;

	static char *kwlist[] = {"offset", "number", "isrc", "type", "pre_emphasis", "indices", NULL};

	data = self->data;
	type = pre_emphasis = false;
	indices = NULL;
	isrc = NULL;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O&O&esO&O&O", kwlist, _uint64, &data->offset, _byte, &data->number, "ascii", &isrc, _bool, &type, _bool, &pre_emphasis, &indices))
		return -1;

	if (isrc)
	{
		b = flac_StreamMetadataCueSheetTrack_copy_isrc(data, isrc);
		PyMem_Free(isrc);
		PyFLAC_CHECK_status(b);
	}

	if (indices)
		PyFLAC_CHECK_status(flac_StreamMetadataCueSheetTrack_set_indices(self, indices, NULL));

	data->type = type ? 1 : 0;
	data->pre_emphasis = pre_emphasis ? 1 : 0;

	return 0;
}


static PyObject *
flac_StreamMetadataCueSheetTrack_clone (flac_StreamMetadataCueSheetTrackObject *self)
{
	return flac_StreamMetadataCueSheetTrack_new_object(Py_TYPE(self), self->data);
}


static PyObject *
flac_StreamMetadataCueSheetTrack_resize_indices (PyObject *self, PyObject *args)
{
	FLAC__StreamMetadata *metadata;
	unsigned track_num, new_num_indices;

	if (PyFLAC_type_Check(self, StreamMetadataCueSheet))
	{
		if (!PyArg_ParseTuple(args, "O&O&", _unsigned, &track_num, _unsigned, &new_num_indices))
			return NULL;

		metadata = ((PyFLAC_StreamMetadataObject *) self)->metadata;

		if (metadata->data.cue_sheet.num_tracks <= track_num)
		{
			PyErr_SetString(PyExc_IndexError, "track number out of range");
			return NULL;
		}
	}
	else
	{
		if (!PyArg_ParseTuple(args, "O&", _unsigned, &new_num_indices))
			return NULL;

		metadata = ((flac_StreamMetadataCueSheetTrackObject *) self)->cuesheet.metadata;
		track_num = 0;
	}

	if (!FLAC__metadata_object_cuesheet_track_resize_indices(metadata, track_num, new_num_indices))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataCueSheetTrack_set_index (PyObject *self, PyObject *args)
{
	FLAC__StreamMetadata *metadata;
	PyObject *index;
	unsigned track_num, index_num;

	if (PyFLAC_type_Check(self, StreamMetadataCueSheet))
	{
		if (!PyArg_ParseTuple(args, "O&O&O!", _unsigned, &track_num, _unsigned, &index_num, PyFLAC_type(StreamMetadataCueSheetIndex), &index))
			return NULL;

		metadata = ((PyFLAC_StreamMetadataObject *) self)->metadata;

		if (metadata->data.cue_sheet.num_tracks <= track_num)
		{
			PyErr_SetString(PyExc_IndexError, "track number out of range");
			return NULL;
		}
	}
	else
	{
		if (!PyArg_ParseTuple(args, "O&O!", _unsigned, &index_num, PyFLAC_type(StreamMetadataCueSheetIndex), &index))
			return NULL;

		metadata = ((flac_StreamMetadataCueSheetTrackObject *) self)->cuesheet.metadata;
		track_num = 0;
	}

	if (metadata->data.cue_sheet.tracks[track_num].num_indices <= index_num)
	{
		PyErr_SetString(PyExc_IndexError, "index number out of range");
		return NULL;
	}

	metadata->data.cue_sheet.tracks[track_num].indices[index_num] = ((flac_StreamMetadataCueSheetIndexObject *) index)->data;
	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataCueSheetTrack_get_index (PyObject *self, PyObject *args)
{
	FLAC__StreamMetadata *metadata;
	unsigned track_num, index_num;

	if (PyFLAC_type_Check(self, StreamMetadataCueSheet))
	{
		if (!PyArg_ParseTuple(args, "O&O&", _unsigned, &track_num, _unsigned, &index_num))
			return NULL;

		metadata = ((PyFLAC_StreamMetadataObject *) self)->metadata;

		if (metadata->data.cue_sheet.num_tracks <= track_num)
		{
			PyErr_SetString(PyExc_IndexError, "track number out of range");
			return NULL;
		}
	}
	else
	{
		if (!PyArg_ParseTuple(args, "O&", _unsigned, &index_num))
			return NULL;

		metadata = ((flac_StreamMetadataCueSheetTrackObject *) self)->cuesheet.metadata;
		track_num = 0;
	}

	if (metadata->data.cue_sheet.tracks[track_num].num_indices <= index_num)
	{
		PyErr_SetString(PyExc_IndexError, "index number out of range");
		return NULL;
	}

	return flac_StreamMetadataCueSheetIndex_new_object(PyFLAC_type(StreamMetadataCueSheetIndex), metadata->data.cue_sheet.tracks[track_num].indices[index_num]);
}


static PyObject *
flac_StreamMetadataCueSheetTrack_insert_index (PyObject *self, PyObject *args)
{
	FLAC__StreamMetadata *metadata;
	PyObject *index;
	unsigned track_num, index_num;

	if (PyFLAC_type_Check(self, StreamMetadataCueSheet))
	{
		if (!PyArg_ParseTuple(args, "O&O&O!", _unsigned, &track_num, _unsigned, &index_num, PyFLAC_type(StreamMetadataCueSheetIndex), &index))
			return NULL;

		metadata = ((PyFLAC_StreamMetadataObject *) self)->metadata;

		if (metadata->data.cue_sheet.num_tracks <= track_num)
		{
			PyErr_SetString(PyExc_IndexError, "track number out of range");
			return NULL;
		}
	}
	else
	{
		if (!PyArg_ParseTuple(args, "O&O!", _unsigned, &index_num, PyFLAC_type(StreamMetadataCueSheetIndex), &index))
			return NULL;

		metadata = ((flac_StreamMetadataCueSheetTrackObject *) self)->cuesheet.metadata;
		track_num = 0;
	}

	if (metadata->data.cue_sheet.tracks[track_num].num_indices < index_num)
		PyErr_SetString(PyExc_IndexError, "index number out of range");
	else if (!FLAC__metadata_object_cuesheet_track_insert_index(metadata, track_num, index_num, ((flac_StreamMetadataCueSheetIndexObject *) index)->data))
		PyErr_NoMemory();
	else
		Py_RETURN_TRUE;

	return NULL;
}


static PyObject *
flac_StreamMetadataCueSheetTrack_insert_blank_index (PyObject *self, PyObject *args)
{
	FLAC__StreamMetadata *metadata;
	unsigned track_num, index_num;

	if (PyFLAC_type_Check(self, StreamMetadataCueSheet))
	{
		if (!PyArg_ParseTuple(args, "O&O&", _unsigned, &track_num, _unsigned, &index_num))
			return NULL;

		metadata = ((PyFLAC_StreamMetadataObject *) self)->metadata;

		if (metadata->data.cue_sheet.num_tracks <= track_num)
		{
			PyErr_SetString(PyExc_IndexError, "track number out of range");
			return NULL;
		}
	}
	else
	{
		if (!PyArg_ParseTuple(args, "O&", _unsigned, &index_num))
			return NULL;

		metadata = ((flac_StreamMetadataCueSheetTrackObject *) self)->cuesheet.metadata;
		track_num = 0;
	}

	if (metadata->data.cue_sheet.tracks[track_num].num_indices < index_num)
		PyErr_SetString(PyExc_IndexError, "index number out of range");
	else if (!FLAC__metadata_object_cuesheet_track_insert_blank_index(metadata, track_num, index_num))
		PyErr_NoMemory();
	else
		Py_RETURN_TRUE;

	return NULL;
}


static PyObject *
flac_StreamMetadataCueSheetTrack_delete_index (PyObject *self, PyObject *args)
{
	FLAC__StreamMetadata *metadata;
	unsigned track_num, index_num;

	if (PyFLAC_type_Check(self, StreamMetadataCueSheet))
	{
		if (!PyArg_ParseTuple(args, "O&O&", _unsigned, &track_num, _unsigned, &index_num))
			return NULL;

		metadata = ((PyFLAC_StreamMetadataObject *) self)->metadata;

		if (metadata->data.cue_sheet.num_tracks <= track_num)
		{
			PyErr_SetString(PyExc_IndexError, "track number out of range");
			return NULL;
		}
	}
	else
	{
		if (!PyArg_ParseTuple(args, "O&", _unsigned, &index_num))
			return NULL;

		metadata = ((flac_StreamMetadataCueSheetTrackObject *) self)->cuesheet.metadata;
		track_num = 0;
	}

	if (metadata->data.cue_sheet.tracks[track_num].num_indices <= index_num)
		PyErr_SetString(PyExc_IndexError, "index number out of range");
	else if (!FLAC__metadata_object_cuesheet_track_delete_index(metadata, track_num, index_num))
		PyErr_NoMemory();
	else
		Py_RETURN_TRUE;

	return NULL;
}


static PyMethodDef flac_StreamMetadataCueSheetTrack_methods[] = {
	{
		"__copy__",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_clone,
		METH_NOARGS,
		"Clone"
	}, {
		"resize_indices",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_resize_indices,
		METH_VARARGS,
		"Resize indices"
	}, {
		"set_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_set_index,
		METH_VARARGS,
		"Set index"
	}, {
		"get_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_get_index,
		METH_VARARGS,
		"Get index"
	}, {
		"insert_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_insert_index,
		METH_VARARGS,
		"Insert index"
	}, {
		"insert_blank_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_insert_blank_index,
		METH_VARARGS,
		"Insert blank index"
	}, {
		"delete_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_delete_index,
		METH_VARARGS,
		"Delete index"
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadataCueSheetTrack_get (flac_StreamMetadataCueSheetTrackObject *self, void *closure)
{
	if (closure == (void *) 4)
		return PyFLAC_uint64(self->data->offset);

	if (closure == (void *) 3)
		return PyFLAC_byte(self->data->number);

	if (closure == (void *) 2)
		return PyFLAC_string(self->data->isrc);

	if (closure)
		return PyFLAC_bool(self->data->type);

	return PyFLAC_bool(self->data->pre_emphasis);
}


static PyObject *
flac_StreamMetadataCueSheetTrack_get_indices (flac_StreamMetadataCueSheetTrackObject *self, void *closure)
{
	return flac_Iter_New(StreamMetadataCueSheetIndex, self->data->indices, self->data->num_indices, self);
}


static int
flac_StreamMetadataCueSheetTrack_set (flac_StreamMetadataCueSheetTrackObject *self, PyObject *value, void *closure)
{
	FLAC__bool b;
	char *isrc;

	if (closure == (void *) 4)
	{
		PyFLAC_CHECK_delete(value, "offset");
		return _uint64(value, &self->data->offset) ? 0 : -1;
	}

	if (closure == (void *) 3)
	{
		PyFLAC_CHECK_delete(value, "number");
		return _byte(value, &self->data->number) ? 0 : -1;
	}

	if (closure == (void *) 2)
	{
		PyFLAC_CHECK_delete(value, "isrc");

		value = PyTuple_Pack(1, value);
		if (!value || !PyArg_ParseTuple(value, "es", "ascii", &isrc))
		{
			Py_XDECREF(value);
			return -1;
		}

		Py_DECREF(value);

		b = flac_StreamMetadataCueSheetTrack_copy_isrc(self->data, isrc) < 0;
		PyMem_Free(isrc);
		if (b)
			return -1;
	}
	else
	{
		PyFLAC_CHECK_delete(value, closure ? "type" : "pre_emphasis");

		if (!_bool(value, &b))
			return -1;

		if (closure)
			self->data->type = b ? 1 : 0;
		else
			self->data->pre_emphasis = b ? 1 : 0;
	}

	return 0;
}


static int
flac_StreamMetadataCueSheetTrack_set_indices (flac_StreamMetadataCueSheetTrackObject *self, PyObject *indices, void *closure)
{
	PyObject **index_list;
	Py_ssize_t index_count, i;

	PyFLAC_CHECK_delete(indices, "indices");

	index_list = NULL;

	if (!flac_list_of_type(indices, PyFLAC_type(StreamMetadataCueSheetIndex), &index_list, &index_count))
		return -1;

	if (!FLAC__metadata_object_cuesheet_track_resize_indices(self->cuesheet.metadata, 0, (unsigned) index_count))
	{
		flac_free_list_of_type(index_list, index_count);
		PyErr_NoMemory();
		return -1;
	}

	if (index_list != NULL)
	{
		for (i = 0; i < index_count; i++)
			self->cuesheet.metadata->data.cue_sheet.tracks[0].indices[i] = ((flac_StreamMetadataCueSheetIndexObject *) index_list[i])->data;
		flac_free_list_of_type(index_list, index_count);
	}
	else if (index_count)
	{
		self->cuesheet.metadata->data.cue_sheet.tracks[0].indices[0] = ((flac_StreamMetadataCueSheetIndexObject *) indices)->data;
	}

	return 0;
}


static PyGetSetDef flac_StreamMetadataCueSheetTrack_getseters[] = {
	{
		"offset",
		(getter) flac_StreamMetadataCueSheetTrack_get,
		(setter) flac_StreamMetadataCueSheetTrack_set,
		"StreamMetadataCueSheetTrack offset", (void *) 4
	}, {
		"number",
		(getter) flac_StreamMetadataCueSheetTrack_get,
		(setter) flac_StreamMetadataCueSheetTrack_set,
		"StreamMetadataCueSheetTrack number", (void *) 3
	}, {
		"isrc",
		(getter) flac_StreamMetadataCueSheetTrack_get,
		(setter) flac_StreamMetadataCueSheetTrack_set,
		"StreamMetadataCueSheetTrack isrc", (void *) 2
	}, {
		"type",
		(getter) flac_StreamMetadataCueSheetTrack_get,
		(setter) flac_StreamMetadataCueSheetTrack_set,
		"StreamMetadataCueSheetTrack type", (void *) 1
	}, {
		"pre_emphasis",
		(getter) flac_StreamMetadataCueSheetTrack_get,
		(setter) flac_StreamMetadataCueSheetTrack_set,
		"StreamMetadataCueSheetTrack pre_emphasis", (void *) 0
	}, {
		"indices",
		(getter) flac_StreamMetadataCueSheetTrack_get_indices,
		(setter) flac_StreamMetadataCueSheetTrack_set_indices,
		"StreamMetadataCueSheetTrack indices", NULL
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadataCueSheetTrack_richcompare (flac_StreamMetadataCueSheetTrackObject *self, PyObject *other, int op)
{
	if (!PyFLAC_type_Check(other, StreamMetadataCueSheetTrack))
		other = NULL;

	return flac_StreamMetadata_compare((PyFLAC_StreamMetadataObject *) self, (PyFLAC_StreamMetadataObject *) other, op);
}


static int
flac_StreamMetadataCueSheetTrack_Ready ( void )
{
	PyTypeObject *type = PyFLAC_type(StreamMetadataCueSheetTrack);

	type->tp_dealloc = (destructor) flac_StreamMetadata_dealloc;
	type->tp_flags = Py_TPFLAGS_DEFAULT;
	type->tp_doc = "FLAC StreamMetadataCueSheetTrack";
	type->tp_richcompare = (richcmpfunc) flac_StreamMetadataCueSheetTrack_richcompare;
	type->tp_methods = flac_StreamMetadataCueSheetTrack_methods;
	type->tp_getset = flac_StreamMetadataCueSheetTrack_getseters;
	type->tp_dict = NULL;
	type->tp_init = (initproc) flac_StreamMetadataCueSheetTrack_init;
	type->tp_new = flac_StreamMetadataCueSheetTrack_new;

	return PyType_Ready(type);
}


static PyObject *
flac_StreamMetadataCueSheetTrack_iternext (PyObject *iter, const void *data, Py_ssize_t index)
{
	return flac_StreamMetadataCueSheetTrack_new_object(PyFLAC_type(StreamMetadataCueSheetTrack), &((FLAC__StreamMetadata_CueSheet_Track *) data)[index]);
}


static PyObject *
flac_StreamMetadataCueSheet_resize_tracks (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned new_num_tracks;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &new_num_tracks))
		return NULL;

	if (!FLAC__metadata_object_cuesheet_resize_tracks(self->metadata, new_num_tracks))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataCueSheet_set_track (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	PyObject *track;
	unsigned track_num;

	if (!PyArg_ParseTuple(args, "O&O!", _unsigned, &track_num, PyFLAC_type(StreamMetadataCueSheetTrack), &track))
		return NULL;

	if (self->metadata->data.cue_sheet.num_tracks <= track_num)
	{
		PyErr_SetString(PyExc_IndexError, "track number out of range");
		return NULL;
	}

	if (!FLAC__metadata_object_cuesheet_set_track(self->metadata, track_num, ((flac_StreamMetadataCueSheetTrackObject *) track)->data, true))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataCueSheet_get_track (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned track_num;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &track_num))
		return NULL;

	if (self->metadata->data.cue_sheet.num_tracks <= track_num)
	{
		PyErr_SetString(PyExc_IndexError, "track number out of range");
		return NULL;
	}

	return flac_StreamMetadataCueSheetTrack_new_object(PyFLAC_type(StreamMetadataCueSheetTrack), &self->metadata->data.cue_sheet.tracks[track_num]);
}


static PyObject *
flac_StreamMetadataCueSheet_insert_track (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	PyObject *track;
	unsigned track_num;

	if (!PyArg_ParseTuple(args, "O&O!", _unsigned, &track_num, PyFLAC_type(StreamMetadataCueSheetTrack), &track))
		return NULL;

	if (self->metadata->data.cue_sheet.num_tracks < track_num)
	{
		PyErr_SetString(PyExc_IndexError, "track number out of range");
		return NULL;
	}

	if (!FLAC__metadata_object_cuesheet_insert_track(self->metadata, track_num, ((flac_StreamMetadataCueSheetTrackObject *) track)->data, true))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataCueSheet_insert_blank_track (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned track_num;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &track_num))
		return NULL;

	if (self->metadata->data.cue_sheet.num_tracks < track_num)
	{
		PyErr_SetString(PyExc_IndexError, "track number out of range");
		return NULL;
	}

	if (!FLAC__metadata_object_cuesheet_insert_blank_track(self->metadata, track_num))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataCueSheet_delete_track (PyFLAC_StreamMetadataObject *self, PyObject *args)
{
	unsigned track_num;

	if (!PyArg_ParseTuple(args, "O&", _unsigned, &track_num))
		return NULL;

	if (self->metadata->data.cue_sheet.num_tracks <= track_num)
	{
		PyErr_SetString(PyExc_IndexError, "track number out of range");
		return NULL;
	}

	if (!FLAC__metadata_object_cuesheet_delete_track(self->metadata, track_num))
		return PyErr_NoMemory();

	Py_RETURN_TRUE;
}


static PyObject *
flac_StreamMetadataCueSheet_is_legal (PyFLAC_StreamMetadataObject *self, PyObject *args, PyObject *kwds)
{
	FLAC__bool check_cd_da_subset, raise_violation, is_legal;
	const char * violation;

	static char *kwlist[] = {"check_cd_da_subset", "raise_violation", NULL};

	check_cd_da_subset = raise_violation = false;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O&O&", kwlist, _bool, &check_cd_da_subset, _bool, &raise_violation))
		return NULL;

	is_legal = FLAC__metadata_object_cuesheet_is_legal(self->metadata, check_cd_da_subset, raise_violation ? &violation : NULL);

	if (raise_violation && !is_legal)
	{
		PyErr_Format(PyFLAC_type(FlacFormatError), "%s", violation);
		return NULL;
	}

	return PyFLAC_bool(is_legal);
}


static PyObject *
flac_StreamMetadataCueSheet_calculate_cddb_id (PyFLAC_StreamMetadataObject *self)
{
	return PyFLAC_uint32(FLAC__metadata_object_cuesheet_calculate_cddb_id(self->metadata));
}


static PyMethodDef flac_StreamMetadataCueSheet_methods[] = {
	{
		"track_resize_indices",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_resize_indices,
		METH_VARARGS,
		"Track resize indices"
	}, {
		"track_set_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_set_index,
		METH_VARARGS,
		"Track set index"
	}, {
		"track_get_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_get_index,
		METH_VARARGS,
		"Track get index"
	}, {
		"track_insert_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_insert_index,
		METH_VARARGS,
		"Track insert index"
	}, {
		"track_insert_blank_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_insert_blank_index,
		METH_VARARGS,
		"Track insert blank index"
	}, {
		"track_delete_index",
		(PyCFunction) flac_StreamMetadataCueSheetTrack_delete_index,
		METH_VARARGS,
		"Track delete index"
	}, {
		"resize_tracks",
		(PyCFunction) flac_StreamMetadataCueSheet_resize_tracks,
		METH_VARARGS,
		"Resize tracks"
	}, {
		"set_track",
		(PyCFunction) flac_StreamMetadataCueSheet_set_track,
		METH_VARARGS,
		"Set track"
	}, {
		"get_track",
		(PyCFunction) flac_StreamMetadataCueSheet_get_track,
		METH_VARARGS,
		"Get track"
	}, {
		"insert_track",
		(PyCFunction) flac_StreamMetadataCueSheet_insert_track,
		METH_VARARGS,
		"Insert track"
	}, {
		"insert_blank_track",
		(PyCFunction) flac_StreamMetadataCueSheet_insert_blank_track,
		METH_VARARGS,
		"Insert blank track"
	}, {
		"delete_track",
		(PyCFunction) flac_StreamMetadataCueSheet_delete_track,
		METH_VARARGS,
		"Delete track"
	}, {
		"is_legal",
		(PyCFunction) flac_StreamMetadataCueSheet_is_legal,
		METH_VARARGS | METH_KEYWORDS,
		"Is legal"
	}, {
		"calculate_cddb_id",
		(PyCFunction) flac_StreamMetadataCueSheet_calculate_cddb_id,
		METH_NOARGS,
		"Calculate cddb id"
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadataCueSheet_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	return flac_StreamMetadata__new(type, args, kwds, FLAC__METADATA_TYPE_CUESHEET);
}


static PyObject *
flac_StreamMetadataCueSheet_get_tracks (PyFLAC_StreamMetadataObject *self)
{
	return flac_Iter_New(StreamMetadataCueSheetTrack, self->metadata->data.cue_sheet.tracks, self->metadata->data.cue_sheet.num_tracks, self);
}


static PyObject *
flac_StreamMetadataCueSheet_get (PyFLAC_StreamMetadataObject *self, void *closure)
{
	switch ((flac_StreamMetadataCueSheet_member) closure)
	{
		case flac_StreamMetadataCueSheet_media_catalog_number:
			return PyFLAC_string(self->metadata->data.cue_sheet.media_catalog_number);
		case flac_StreamMetadataCueSheet_lead_in:
			return PyFLAC_uint64(self->metadata->data.cue_sheet.lead_in);
		case flac_StreamMetadataCueSheet_is_cd:
			return PyFLAC_bool(self->metadata->data.cue_sheet.is_cd);
		case flac_StreamMetadataCueSheet_tracks:
			return flac_StreamMetadataCueSheet_get_tracks(self);
		default:
			return PyFLAC_getter_error(StreamMetadataCueSheet);
	}
}


static int
flac_StreamMetadataCueSheet_set_media_catalog_number (PyFLAC_StreamMetadataObject *self, PyObject *value)
{
	char *media_catalog_number, media_catalog_number_cpy[129];
	size_t len, i;
	int value_error;

	value = PyTuple_Pack(1, value);
	if (!value || !PyArg_ParseTuple(value, "es", "ascii", &media_catalog_number))
	{
		Py_XDECREF(value);
		return -1;
	}

	Py_DECREF(value);

	len = strlen(media_catalog_number);
	value_error = 1;

	if (len <= 128)
	{
		value_error = 0;

		for (i = 0; i < len; i++)
		{
			if (media_catalog_number[i] < 0x20 || media_catalog_number[i] > 0x7e)
			{
				value_error = 1;
				i = 129;
				break;
			}

			media_catalog_number_cpy[i] = media_catalog_number[i];
		}

		for (; i < 129; i++)
			media_catalog_number_cpy[i] = '\0';
	}

	PyMem_Free(media_catalog_number);

	if (value_error)
	{
		PyErr_SetString(PyExc_ValueError, "media catalog number must be of ascii printable characters 0x20-0x7e and 0 to 128 bytes long");
		return -1;
	}

	Py_MEMCPY(self->metadata->data.cue_sheet.media_catalog_number, media_catalog_number_cpy, 129);
	return 0;
}


static int
flac_StreamMetadataCueSheet_set_tracks (PyFLAC_StreamMetadataObject *self, PyObject *tracks)
{
	PyObject **track_list;
	Py_ssize_t track_count, i;

	track_list = NULL;

	if (!tracks)
	{
		PyErr_SetString(PyExc_TypeError, "cannot delete the tracks");
		return -1;
	}

	if (!flac_list_of_type(tracks, PyFLAC_type(StreamMetadataCueSheetTrack), &track_list, &track_count))
		return -1;

	if (!FLAC__metadata_object_cuesheet_resize_tracks(self->metadata, (unsigned) track_count))
	{
		flac_free_list_of_type(track_list, track_count);
		PyErr_NoMemory();
		return -1;
	}

	if (track_list != NULL)
	{
		for (i = 0; i < track_count; i++)
			if (!FLAC__metadata_object_cuesheet_set_track(self->metadata, i, ((flac_StreamMetadataCueSheetTrackObject *) track_list[i])->data, true))
				break;

		flac_free_list_of_type(track_list, track_count);

		if (i == track_count)
			return 0;
	}
	else if (!track_count || FLAC__metadata_object_cuesheet_set_track(self->metadata, 0, ((flac_StreamMetadataCueSheetTrackObject *) tracks)->data, true))
	{
		return 0;
	}

	PyErr_NoMemory();
	return -1;
}


static int
flac_StreamMetadataCueSheet_set (PyFLAC_StreamMetadataObject *self, PyObject *value, void *closure)
{
	flac_StreamMetadataCueSheet_member member = (flac_StreamMetadataCueSheet_member) closure;

	if (member < flac_StreamMetadataCueSheet_member_count)
		PyFLAC_CHECK_delete(value, flac_StreamMetadataData_setters[FLAC__METADATA_TYPE_CUESHEET].kwlist[member]);

	switch (member)
	{
		case flac_StreamMetadataCueSheet_media_catalog_number:
			return flac_StreamMetadataCueSheet_set_media_catalog_number(self, value);
		case flac_StreamMetadataCueSheet_lead_in:
			return _uint64(value, &self->metadata->data.cue_sheet.lead_in) ? 0 : -1;
		case flac_StreamMetadataCueSheet_is_cd:
			return _bool(value, &self->metadata->data.cue_sheet.is_cd) ? 0 : -1;
		case flac_StreamMetadataCueSheet_tracks:
			return flac_StreamMetadataCueSheet_set_tracks(self, value);
		default:
			return PyFLAC_setter_error(StreamMetadataCueSheet);
	}
}


static struct flac_StreamMetadataDataType flac_StreamMetadataCueSheetType = {
	PyFLAC_type(StreamMetadataCueSheet),
	"FLAC StreamMetadata_CueSheet",
	flac_StreamMetadataCueSheet_new,
	flac_StreamMetadataCueSheet_methods,
	{{
		"media_catalog_number",
		(getter) flac_StreamMetadataCueSheet_get,
		(setter) flac_StreamMetadataCueSheet_set,
		"StreamMetadataCueSheet media_catalog_number",
		(void *) flac_StreamMetadataCueSheet_media_catalog_number
	}, {
		"lead_in",
		(getter) flac_StreamMetadataCueSheet_get,
		(setter) flac_StreamMetadataCueSheet_set,
		"StreamMetadataCueSheet lead_in",
		(void *) flac_StreamMetadataCueSheet_lead_in
	}, {
		"is_cd",
		(getter) flac_StreamMetadataCueSheet_get,
		(setter) flac_StreamMetadataCueSheet_set,
		"StreamMetadataCueSheet is_cd",
		(void *) flac_StreamMetadataCueSheet_is_cd
	}, {
		"tracks",
		(getter) flac_StreamMetadataCueSheet_get,
		(setter) flac_StreamMetadataCueSheet_set,
		"StreamMetadataCueSheet tracks",
		(void *) flac_StreamMetadataCueSheet_tracks
	}, { NULL }}		/* Sentinel */
};


static PyObject *
flac_StreamMetadataPicture_is_legal (PyFLAC_StreamMetadataObject *self, PyObject *args, PyObject *kwds)
{
	FLAC__bool raise_violation, is_legal;
	const char * violation;

	static char *kwlist[] = {"raise_violation", NULL};

	raise_violation = false;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O&", kwlist, _bool, &raise_violation))
		return NULL;

	is_legal = !FLAC__metadata_object_picture_is_legal(self->metadata, raise_violation ? &violation : NULL);

	if (raise_violation && !is_legal)
	{
		PyErr_Format(PyFLAC_type(FlacFormatError), "%s", violation);
		return NULL;
	}

	return PyFLAC_bool(is_legal);
}


static PyMethodDef flac_StreamMetadataPicture_methods[] = {
	{
		"is_legal",
		(PyCFunction) flac_StreamMetadataPicture_is_legal,
		METH_VARARGS | METH_KEYWORDS,
		"Is legal"
	}, { NULL }		/* Sentinel */
};


static PyObject *
flac_StreamMetadataPicture_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	return flac_StreamMetadata__new(type, args, kwds, FLAC__METADATA_TYPE_PICTURE);
}


static PyObject *
flac_StreamMetadataPicture_get (PyFLAC_StreamMetadataObject *self, void *closure)
{
	PyObject *attr;

	switch ((flac_StreamMetadataPicture_member) closure)
	{
		case flac_StreamMetadataPicture_type:
			attr = PyFLAC_Enum_FromEnum(self->metadata->data.picture.type, StreamMetadataPictureType);
			Py_XINCREF(attr);
			return attr;
		case flac_StreamMetadataPicture_mime_type:
			return PyFLAC_string(self->metadata->data.picture.mime_type);
		case flac_StreamMetadataPicture_description:
			return PyFLAC_string(self->metadata->data.picture.description);
		case flac_StreamMetadataPicture_width:
			return PyFLAC_unsigned(self->metadata->data.picture.width);
		case flac_StreamMetadataPicture_height:
			return PyFLAC_unsigned(self->metadata->data.picture.height);
		case flac_StreamMetadataPicture_depth:
			return PyFLAC_unsigned(self->metadata->data.picture.depth);
		case flac_StreamMetadataPicture_colors:
			return PyFLAC_unsigned(self->metadata->data.picture.colors);
		case flac_StreamMetadataPicture_data:
			return PyFLAC_data(self->metadata->data.picture.data, self->metadata->data.picture.data_length);
		default:
			return PyFLAC_getter_error(StreamMetadataPicture);
	}
}


static int
flac_StreamMetadataPicture_set_type (PyFLAC_StreamMetadataObject *self, PyObject *value)
{
	PyObject *type;

	value = PyTuple_Pack(1, value);
	if (!value || !PyArg_ParseTuple(value, "O!", PyFLAC_type(StreamMetadataPictureType), &type))
	{
		Py_XDECREF(value);
		return -1;
	}

	self->metadata->data.picture.type = PyFLAC_Enum_AsEnum(type, StreamMetadata_Picture_Type);

	Py_DECREF(value);
	return 0;
}


static int
flac_StreamMetadataPicture_set_mime_type (PyFLAC_StreamMetadataObject *self, PyObject *value)
{
	char *mime_type;
	size_t len, i;
	int result;

	result = -1;

	value = PyTuple_Pack(1, value);
	if (!value || !PyArg_ParseTuple(value, "es", "ascii", &mime_type))
	{
		Py_XDECREF(value);
		return -1;
	}

	Py_DECREF(value);

	len = strlen(mime_type);

	for (i = 0; i < len; i++)
		if (mime_type[i] < 0x20 || mime_type[i] > 0x7e)
			break;

	if (i != len)
		PyErr_SetString(PyExc_ValueError, "mime type must be of ascii printable characters 0x20-0x7e");
	else if (FLAC__metadata_object_picture_set_mime_type(self->metadata, mime_type, true))
		result = 0;
	else
		PyErr_NoMemory();

	PyMem_Free(mime_type);

	return result;
}


static int
flac_StreamMetadataPicture_set_description (PyFLAC_StreamMetadataObject *self, PyObject *value)
{
	char *description;
	int result;

	result = -1;

	value = PyTuple_Pack(1, value);
	if (!value || !PyArg_ParseTuple(value, "es", "utf-8", &description))
	{
		Py_XDECREF(value);
		return -1;
	}

	Py_DECREF(value);

	if (FLAC__metadata_object_picture_set_description(self->metadata, (FLAC__byte *) description, true))
		result = 0;
	else
		PyErr_NoMemory();

	PyMem_Free(description);

	return result;
}


static int
flac_StreamMetadataPicture_set_data (PyFLAC_StreamMetadataObject *self, PyObject *data)
{
	FLAC__byte *bytes;
	Py_ssize_t length;

	if (!flac_list_of_bytes(data, &bytes, &length))
		return -1;

	FLAC__metadata_object_picture_set_data(self->metadata, bytes, (unsigned) length, false);
	return 0;
}


static int
flac_StreamMetadataPicture_set (PyFLAC_StreamMetadataObject *self, PyObject *value, void *closure)
{
	flac_StreamMetadataPicture_member member = (flac_StreamMetadataPicture_member) closure;

	if (member < flac_StreamMetadataPicture_member_count)
		PyFLAC_CHECK_delete(value, flac_StreamMetadataData_setters[FLAC__METADATA_TYPE_PICTURE].kwlist[member]);

	switch (member)
	{
		case flac_StreamMetadataPicture_type:
			return flac_StreamMetadataPicture_set_type(self, value);
		case flac_StreamMetadataPicture_mime_type:
			return flac_StreamMetadataPicture_set_mime_type(self, value);
		case flac_StreamMetadataPicture_description:
			return flac_StreamMetadataPicture_set_description(self, value);
		case flac_StreamMetadataPicture_width:
			return _uint32(value, &self->metadata->data.picture.width) ? 0 : -1;
		case flac_StreamMetadataPicture_height:
			return _uint32(value, &self->metadata->data.picture.height) ? 0 : -1;
		case flac_StreamMetadataPicture_depth:
			return _uint32(value, &self->metadata->data.picture.depth) ? 0 : -1;
		case flac_StreamMetadataPicture_colors:
			return _uint32(value, &self->metadata->data.picture.colors) ? 0 : -1;
		case flac_StreamMetadataPicture_data:
			return flac_StreamMetadataPicture_set_data(self, value);
		default:
			return PyFLAC_setter_error(StreamMetadataPicture);
	}
}


static struct flac_StreamMetadataDataType flac_StreamMetadataPictureType = {
	PyFLAC_type(StreamMetadataPicture),
	"FLAC StreamMetadata_Picture",
	flac_StreamMetadataPicture_new,
	flac_StreamMetadataPicture_methods,
	{{
		"picture_type",
		(getter) flac_StreamMetadataPicture_get,
		(setter) flac_StreamMetadataPicture_set,
		"StreamMetadataPicture type",
		(void *) flac_StreamMetadataPicture_type,
	}, {
		"mime_type",
		(getter) flac_StreamMetadataPicture_get,
		(setter) flac_StreamMetadataPicture_set,
		"StreamMetadataPicture mime_type",
		(void *) flac_StreamMetadataPicture_mime_type,
	}, {
		"description",
		(getter) flac_StreamMetadataPicture_get,
		(setter) flac_StreamMetadataPicture_set,
		"StreamMetadataPicture description",
		(void *) flac_StreamMetadataPicture_description,
	}, {
		"width",
		(getter) flac_StreamMetadataPicture_get,
		(setter) flac_StreamMetadataPicture_set,
		"StreamMetadataPicture width",
		(void *) flac_StreamMetadataPicture_width,
	}, {
		"height",
		(getter) flac_StreamMetadataPicture_get,
		(setter) flac_StreamMetadataPicture_set,
		"StreamMetadataPicture height",
		(void *) flac_StreamMetadataPicture_height,
	}, {
		"depth",
		(getter) flac_StreamMetadataPicture_get,
		(setter) flac_StreamMetadataPicture_set,
		"StreamMetadataPicture depth",
		(void *) flac_StreamMetadataPicture_depth,
	}, {
		"colors",
		(getter) flac_StreamMetadataPicture_get,
		(setter) flac_StreamMetadataPicture_set,
		"StreamMetadataPicture colors",
		(void *) flac_StreamMetadataPicture_colors,
	}, {
		"data",
		(getter) flac_StreamMetadataPicture_get,
		(setter) flac_StreamMetadataPicture_set,
		"StreamMetadataPicture data",
		(void *) flac_StreamMetadataPicture_data,
	}, { NULL }}		/* Sentinel */
};


static PyMethodDef flac_metadata_functions[] = {
	{ NULL }		/* Sentinel */
};


static PyObject *
PyFLAC_StreamMetadata_FromClass (const FLAC__StreamMetadata *metadata)
{
	return flac_StreamMetadata_new_object(PyFLAC_type(StreamMetadata), 0, metadata);
}


PyFLAC_Enum_FromEnum_function(StreamMetadataPictureType, enum_member_StreamMetadataPictureType)


static int
flac_metadata_init ( void )
{
	PyFLAC_CHECK_status(PyFLAC_import_format());

	PyFLAC_CHECK_status(flac_StreamMetadata_Ready());

	PyFLAC_CHECK_status(flac_StreamMetadataSeekPoint_Ready());
	PyFLAC_CHECK_status(flac_StreamMetadataVorbisCommentEntry_Ready());
	PyFLAC_CHECK_status(flac_StreamMetadataCueSheetIndex_Ready());
	PyFLAC_CHECK_status(flac_StreamMetadataCueSheetTrack_Ready());

	PyFLAC_CHECK_status(flac_StreamMetadataData_Ready(&flac_StreamMetadataStreamInfoType));
	PyFLAC_CHECK_status(flac_StreamMetadataData_Ready(&flac_StreamMetadataPaddingType));
	PyFLAC_CHECK_status(flac_StreamMetadataData_Ready(&flac_StreamMetadataApplicationType));
	PyFLAC_CHECK_status(flac_StreamMetadataData_Ready(&flac_StreamMetadataSeekTableType));
	PyFLAC_CHECK_status(flac_StreamMetadataData_Ready(&flac_StreamMetadataVorbisCommentType));
	PyFLAC_CHECK_status(flac_StreamMetadataData_Ready(&flac_StreamMetadataCueSheetType));
	PyFLAC_CHECK_status(flac_StreamMetadataData_Ready(&flac_StreamMetadataPictureType));

	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamMetadataPictureType), enum_member_StreamMetadataPictureType));

	_c_api_init

	return 0;
}


static void
flac_metadata_build (PyObject *module)
{
	Py_INCREF(PyFLAC_type(StreamMetadata));
	PyFLAC_PyModule_AddType(module, StreamMetadata);

	Py_INCREF(PyFLAC_type(StreamMetadataStreamInfo));
	PyFLAC_PyModule_AddType(module, StreamMetadataStreamInfo);

	Py_INCREF(PyFLAC_type(StreamMetadataPadding));
	PyFLAC_PyModule_AddType(module, StreamMetadataPadding);

	Py_INCREF(PyFLAC_type(StreamMetadataApplication));
	PyFLAC_PyModule_AddType(module, StreamMetadataApplication);

	Py_INCREF(PyFLAC_type(StreamMetadataSeekPoint));
	PyFLAC_PyModule_AddType(module, StreamMetadataSeekPoint);

	Py_INCREF(PyFLAC_type(StreamMetadataSeekTable));
	PyFLAC_PyModule_AddType(module, StreamMetadataSeekTable);

	Py_INCREF(PyFLAC_type(StreamMetadataVorbisCommentEntry));
	PyFLAC_PyModule_AddType(module, StreamMetadataVorbisCommentEntry);

	Py_INCREF(PyFLAC_type(StreamMetadataVorbisComment));
	PyFLAC_PyModule_AddType(module, StreamMetadataVorbisComment);

	Py_INCREF(PyFLAC_type(StreamMetadataCueSheetIndex));
	PyFLAC_PyModule_AddType(module, StreamMetadataCueSheetIndex);

	Py_INCREF(PyFLAC_type(StreamMetadataCueSheetTrack));
	PyFLAC_PyModule_AddType(module, StreamMetadataCueSheetTrack);

	Py_INCREF(PyFLAC_type(StreamMetadataCueSheet));
	PyFLAC_PyModule_AddType(module, StreamMetadataCueSheet);

	Py_INCREF(PyFLAC_type(StreamMetadataPicture));
	PyFLAC_PyModule_AddType(module, StreamMetadataPicture);

	Py_INCREF(PyFLAC_type(StreamMetadataPictureType));
	PyFLAC_PyModule_AddType(module, StreamMetadataPictureType);

	Py_INCREF(PyFLAC_type(MetadataType));
	PyFLAC_PyModule_AddType(module, MetadataType);

	Py_INCREF(PyFLAC_type(FlacFormatError));
	PyFLAC_PyModule_AddType(module, FlacFormatError);

	_c_api_build(module);
}


PyFLAC_MODINIT(
	metadata,
	flac_metadata_init,
	flac_metadata_build,
	flac_metadata_functions,
	"libFLAC metadata"
)

