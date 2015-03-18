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

#define __PyFLAC_format_MODULE__
#include "format.h"


static void
PyFLAC_free_list_of_type (PyObject **list, Py_ssize_t count)
{
	if (list)
	{
		while (count--)
			Py_DECREF(list[count]);
		PyMem_Del(list);
	}
}

static int
PyFLAC_list_of_type (PyObject *object, PyTypeObject *type, PyObject ***ret_list, Py_ssize_t *ret_count)
{
	PyObject *item, **list;
	Py_ssize_t count, i;

	if (PyObject_TypeCheck(object, type))
	{
		*ret_count = 1;
		*ret_list = NULL;
		return 1;
	}

	list = NULL;

	if (PyTuple_Check(object) || PyList_Check(object))
	{
		count = PySequence_Size(object);
		if (count == -1)
			return 0;

		if (!count)
		{
			*ret_count = 0;
			*ret_list = NULL;
			return 1;
		}

		list = PyMem_New(PyObject *, count);
		if (!list)
		{
			PyErr_NoMemory();
			return 0;
		}

		for (i = 0; i < count; i++)
		{
			item = PySequence_GetItem(object, i);
			if (!item)
			{
				PyFLAC_free_list_of_type(list, i);
				return 0;
			}

			if (!PyObject_TypeCheck(item, type))
				break;

			Py_INCREF(item);
			list[i] = item;
		}

		if (i == count)
		{
			*ret_count = count;
			*ret_list = list;
			return 1;
		}

		PyFLAC_free_list_of_type(list, i);
	}

	PyErr_Format(PyExc_TypeError, "must be a list of %.50s", type->tp_name);
	return 0;
}


static int
PyFLAC_application_id_conv (PyObject *obj, FLAC__byte (*id)[4])
{
	Py_ssize_t length;
	char *bytes;
	int state;

	Py_INCREF(obj);

	if (PyUnicode_Check(obj))
	{
		Py_DECREF(obj);
		if (!(obj = PyUnicode_AsASCIIString(obj)))
			return 0;
	}

	state = 0;

	if (!PyBytes_Check(obj))
		state = 1;
	else if (PyBytes_AsStringAndSize(obj, &bytes, &length) < 0)
		state = 2;
	else if (length != 4)
		state = 1;

	Py_DECREF(obj);

	switch (state)
	{
		case 1:
			PyErr_Format(PyExc_TypeError, "must be %.50s or unicode of length 4", PyBytes_Type.tp_name);
		case 2:
			return 0;
		default:
			(*id)[0] = bytes[0];
			(*id)[1] = bytes[1];
			(*id)[2] = bytes[2];
			(*id)[3] = bytes[3];
			return 1;
	}
}

static int
PyFLAC_bool_conv (PyObject *obj, FLAC__bool *b)
{
	int B;

	B = PyObject_IsTrue(obj);

	if (B < 0)
		return 0;

	*b = (FLAC__bool) B;
	return 1;
}

static int
flac_uintX (PyObject *obj, unsigned PY_LONG_LONG *value, Py_ssize_t size)
{
	PY_LONG_LONG l;
	int overflow;

	const unsigned PY_LONG_LONG MAX = ((unsigned PY_LONG_LONG) -1) >> (sizeof(unsigned PY_LONG_LONG) - size) * 8;

	l = PyLong_AsLongLongAndOverflow(obj, &overflow);

	if (l >= 0 && ((unsigned PY_LONG_LONG) l) <= MAX)
	{
		*value = (unsigned PY_LONG_LONG) l;
		return 1;
	}

	if (l == -1)
	{
		if (!overflow && PyErr_Occurred())
			return 0;

		if (overflow > 0 && size == sizeof(unsigned PY_LONG_LONG))
		{
			l = (PY_LONG_LONG) PyLong_AsUnsignedLongLong(obj);
			obj = (l == -1) ? PyErr_Occurred() : NULL;

			if (obj)
			{
				if (obj != PyExc_OverflowError)
					return 0;

				PyErr_Clear();
			}
			else
			{
				*value = (unsigned PY_LONG_LONG) l;
				return 1;
			}
		}
	}

	PyErr_Format(PyExc_ValueError, "value must be >= 0 and <= %llu", MAX);
	return 0;
}


static int
flac_Enum_add_member (PyTypeObject *type, PyFLAC_Enum_Member_Def *member)
{
	PyObject *name = PyString_FromString(member->e_name);
	if (!name)
		return -1;

	member->e_object = PyObject_New(PyObject, type);
	if (!member->e_object)
		return -1;

	((PyFLAC_EnumObject *) member->e_object)->e_value = member->e_value;
	((PyFLAC_EnumObject *) member->e_object)->e_name = member->e_name;

	PyFLAC_CHECK_status(PyObject_GenericSetAttr((PyObject *) type, name, member->e_object));
	Py_DECREF(name);

	return 0;
}

static int
flac_Enum_sort_member (PyFLAC_Enum_Member_Def *member, int member_count)
{
	int i, j;
	PyFLAC_Enum_Member_Def tmp;

	for (i = 0; i < member_count; i++)
		while (member[i].e_value != i)
		{
			j = member[i].e_value;
			if (member[j].e_value == j)
			{
				PyFLAC_RuntimeError("repeating enum value");
				return -1;
			}

			tmp = member[j];
			member[j] = member[i];
			member[i] = tmp;
		}

	return 0;
}


static PyObject *
flac_Enum_repr (PyObject *self)
{
	return PyString_FromFormat("<enum %s of type %s>", ((PyFLAC_EnumObject *) self)->e_name, Py_TYPE(self)->tp_name);
}

static PyObject *
flac_Enum_str (PyObject *self)
{
	return PyString_FromString(((PyFLAC_EnumObject *) self)->e_name);
}


static int
PyFLAC_Enum_Ready (PyTypeObject *type, PyFLAC_Enum_Member_Def *member)
{
	int i = 0;

	if (type->tp_flags & Py_TPFLAGS_READY)
		return 0;

	type->tp_dealloc = (destructor) PyObject_Del;
	type->tp_repr = (reprfunc) flac_Enum_repr;
	type->tp_str = (reprfunc) flac_Enum_str;
	type->tp_dict = NULL;

	while (member[i].e_name)
		i++;
	PyFLAC_CHECK_status(flac_Enum_sort_member(member, i));

	PyFLAC_CHECK_status(PyType_Ready(type));

	while (i--)
		PyFLAC_CHECK_status(flac_Enum_add_member(type, &member[i]));

	return 0;
}


static PyObject *
PyFLAC_Enum_FromInt (int e_value, PyFLAC_Enum_Member_Def *data, const char *err_msg)
{
	int i;

	for (i = 0; i < e_value; i++)
		if(!data[i].e_name)
		{
			if(!err_msg)
				err_msg = "invalid enum value";

			PyErr_SetString(PyExc_ValueError, err_msg);
			return NULL;
		}

	return data[e_value].e_object;
}


PyFLAC_Enum(EntropyCodingMethodType)
PyFLAC_Enum(SubframeType)
PyFLAC_Enum(ChannelAssignment)
PyFLAC_Enum(FrameNumberType)
PyFLAC_Enum(MetadataType)


static PyFLAC_Enum_Member_Def enum_member_EntropyCodingMethodType[] = {
	PyFLAC_Enum_Member(
		"PARTITIONED_RICE",
		ENTROPY_CODING_METHOD_PARTITIONED_RICE
	),
	PyFLAC_Enum_Member(
		"PARTITIONED_RICE2",
		ENTROPY_CODING_METHOD_PARTITIONED_RICE2
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_SubframeType[] = {
	PyFLAC_Enum_Member(
		"CONSTANT",
		SUBFRAME_TYPE_CONSTANT
	),
	PyFLAC_Enum_Member(
		"VERBATIM",
		SUBFRAME_TYPE_VERBATIM
	),
	PyFLAC_Enum_Member(
		"FIXED",
		SUBFRAME_TYPE_FIXED
	),
	PyFLAC_Enum_Member(
		"LPC",
		SUBFRAME_TYPE_LPC
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_ChannelAssignment[] = {
	PyFLAC_Enum_Member(
		"INDEPENDENT",
		CHANNEL_ASSIGNMENT_INDEPENDENT
	),
	PyFLAC_Enum_Member(
		"LEFT_SIDE",
		CHANNEL_ASSIGNMENT_LEFT_SIDE
	),
	PyFLAC_Enum_Member(
		"RIGHT_SIDE",
		CHANNEL_ASSIGNMENT_RIGHT_SIDE
	),
	PyFLAC_Enum_Member(
		"MID_SIDE",
		CHANNEL_ASSIGNMENT_MID_SIDE
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_FrameNumberType[] = {
	PyFLAC_Enum_Member(
		"FRAME_NUMBER",
		FRAME_NUMBER_TYPE_FRAME_NUMBER
	),
	PyFLAC_Enum_Member(
		"SAMPLE_NUMBER",
		FRAME_NUMBER_TYPE_SAMPLE_NUMBER
	),
	{ NULL }		/* Sentinel */
};


static PyFLAC_Enum_Member_Def enum_member_MetadataType[] = {
	PyFLAC_Enum_Member(
		"STREAMINFO",
		METADATA_TYPE_STREAMINFO
	),
	PyFLAC_Enum_Member(
		"PADDING",
		METADATA_TYPE_PADDING
	),
	PyFLAC_Enum_Member(
		"APPLICATION",
		METADATA_TYPE_APPLICATION
	),
	PyFLAC_Enum_Member(
		"SEEKTABLE",
		METADATA_TYPE_SEEKTABLE
	),
	PyFLAC_Enum_Member(
		"VORBIS_COMMENT",
		METADATA_TYPE_VORBIS_COMMENT
	),
	PyFLAC_Enum_Member(
		"CUESHEET",
		METADATA_TYPE_CUESHEET
	),
	PyFLAC_Enum_Member(
		"PICTURE",
		METADATA_TYPE_PICTURE
	),
	PyFLAC_Enum_Member(
		"UNDEFINED",
		METADATA_TYPE_UNDEFINED
	),
	{ NULL }		/* Sentinel */
};


static PyMethodDef flac_format_functions[] = {
	{ NULL }		/* Sentinel */
};


PyFLAC_Enum_FromEnum_function(EntropyCodingMethodType, enum_member_EntropyCodingMethodType)
PyFLAC_Enum_FromEnum_function(SubframeType, enum_member_SubframeType)
PyFLAC_Enum_FromEnum_function(ChannelAssignment, enum_member_ChannelAssignment)
PyFLAC_Enum_FromEnum_function(FrameNumberType, enum_member_FrameNumberType)
PyFLAC_Enum_FromEnum_function(MetadataType, enum_member_MetadataType)


struct flac_IterObject {
	PyObject_HEAD
	PyObject *data_host;
	Py_ssize_t data_size;
	Py_ssize_t index;
	const void *data;
	PyFLAC_Iter_Next iternext;
	const char* type;
};


static void
flac_Iter_dealloc (struct flac_IterObject *self)
{
	Py_XDECREF(self->data_host);
	PyObject_Del(self);
}


static PyObject *
flac_Iter_repr (PyObject *self)
{
	return PyString_FromFormat("<%s of %s at %p>", Py_TYPE(self)->tp_name, ((struct flac_IterObject *) self)->type, self);
}


static PyObject *
flac_Iter_next (struct flac_IterObject *self)
{
	if (self->index >= self->data_size)
		return NULL;

	return self->iternext((PyObject *) self, self->data, self->index++);
}


static PyObject *
flac_Iter_len (struct flac_IterObject *self)
{
	return PyInt_FromSsize_t(self->data_size);
}


static PyMethodDef flac_Iter_methods[] = {
	{
		"__length_hint__",
		(PyCFunction) flac_Iter_len,
		METH_NOARGS,
		"Private method returning (an estimate of) len(list(it))."
	}, { NULL }		/* Sentinel */
};


static PyTypeObject flac_IterType = {
	PyVarObject_HEAD_INIT(NULL,0)
	PyFLAC_name(Iter),
	sizeof(struct flac_IterObject), 0,
	(destructor) flac_Iter_dealloc,
	0, 0, 0, 0, (reprfunc) flac_Iter_repr,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	"Generic iterator", 0, 0, 0, 0, 0,
	(iternextfunc) flac_Iter_next,
	flac_Iter_methods, 0, 0, 0, 0, 0, 0
};


static PyObject *
PyFLAC_Iter_New (const void *data, Py_ssize_t data_size, PyFLAC_Iter_Next iternext, const char *type, PyObject *data_host)
{
	struct flac_IterObject *self;

	self = PyObject_New(struct flac_IterObject, &flac_IterType);
	if (!self)
		return NULL;

	Py_INCREF(data_host);
	self->data_host = data_host;
	self->iternext = iternext;
	self->data_size = data_size;
	self->data = data;
	self->index = 0;
	self->type = type;

	return (PyObject *) self;
}


static int
flac_format_init (void)
{
	flac_FlacFormatErrorType = PyErr_NewExceptionWithDoc("flac.FlacFormatError", "FLAC format error", NULL, NULL);
	if (!flac_FlacFormatErrorType)
		return -1;

	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(EntropyCodingMethodType), enum_member_EntropyCodingMethodType));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(SubframeType), enum_member_SubframeType));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(ChannelAssignment), enum_member_ChannelAssignment));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(FrameNumberType), enum_member_FrameNumberType));
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(MetadataType), enum_member_MetadataType));

	flac_IterType.tp_iter = PyObject_SelfIter;
	PyFLAC_CHECK_status(PyType_Ready(&flac_IterType));

	_c_api_init

	return 0;
}


static void
flac_format_build (PyObject *module)
{
	Py_INCREF(PyFLAC_type(FlacFormatError));
	PyFLAC_PyModule_AddType(module, FlacFormatError);

	Py_INCREF(PyFLAC_type(EntropyCodingMethodType));
	PyFLAC_PyModule_AddType(module, EntropyCodingMethodType);

	Py_INCREF(PyFLAC_type(SubframeType));
	PyFLAC_PyModule_AddType(module, SubframeType);

	Py_INCREF(PyFLAC_type(ChannelAssignment));
	PyFLAC_PyModule_AddType(module, ChannelAssignment);

	Py_INCREF(PyFLAC_type(FrameNumberType));
	PyFLAC_PyModule_AddType(module, FrameNumberType);

	Py_INCREF(PyFLAC_type(MetadataType));
	PyFLAC_PyModule_AddType(module, MetadataType);

	_c_api_build(module);
}


PyFLAC_MODINIT(
	format,
	flac_format_init,
	flac_format_build,
	flac_format_functions,
	"libFLAC format"
)

