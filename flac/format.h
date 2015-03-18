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

#ifndef __PyFLAC_format_h__
#define __PyFLAC_format_h__

#include <FLAC/format.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef PyObject * (* PyFLAC_Iter_Next) (PyObject *, const void *, Py_ssize_t);

typedef struct {
	PyObject_HEAD
	int e_value;
	char *e_name;
} PyFLAC_EnumObject;

typedef struct {
	char *e_name;
	int e_value;
	PyObject *e_object;
} PyFLAC_Enum_Member_Def;


#define PyFLAC_Enum_FromEnum_NAME(type) PyFLAC_##type##_FromEnum
#define PyFLAC_Enum_FromEnum(e_value,type) PyFLAC_Enum_FromEnum_NAME(type) (e_value)
#define PyFLAC_Enum_FromEnum_DEF(type) static PyObject * PyFLAC_Enum_FromEnum (int e_value,type)
#define PyFLAC_Enum_AsInt(object) (((PyFLAC_EnumObject *) object)->e_value)
#define PyFLAC_Enum_AsEnum(object,type) ((FLAC__##type) PyFLAC_Enum_AsInt(object))

#define PyFLAC_Enum(type) \
static PyTypeObject PyFLAC_##type##Type = { \
	PyVarObject_HEAD_INIT(NULL,0) \
	PyFLAC_name(type), \
	sizeof(PyFLAC_EnumObject), \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	Py_TPFLAGS_DEFAULT , "FLAC enum " #type, \
};

#define PyFLAC_Enum_Member(member_name,flac_enum) { member_name, FLAC__##flac_enum, NULL }

#define PyFLAC_Enum_FromEnum_function(type,data) \
PyFLAC_Enum_FromEnum_DEF(type) \
{ return PyFLAC_Enum_FromInt(e_value, data, "invalid value for enum " #type); }


#include "_C_API.h"

#define			_EntropyCodingMethodType_type_						 0
#define			_SubframeType_type_									 1
#define			_ChannelAssignment_type_							 2
#define			_FrameNumberType_type_								 3
#define			_MetadataType_type_									 4

#define			_FlacFormatError_type_								 5

#define			_EntropyCodingMethodType_FromEnum_					 6
#define			_SubframeType_FromEnum_								 7
#define			_ChannelAssignment_FromEnum_						 8
#define			_FrameNumberType_FromEnum_							 9
#define			_MetadataType_FromEnum_								10

#define			_Enum_Ready_										11
#define			_Enum_FromInt_										12

#define			_Iter_New_											13

#define			_application_id_conv_								14
#define			_bool_conv_											15
#define			_uint8_conv_										16
#define			_uint32_conv_										17
#define			_uint64_conv_										18
#define			_unsigned_conv_										19

#define			_list_of_type_										20
#define			_free_list_of_type_									21


#ifdef __PyFLAC_format_MODULE__

static int flac_uintX (PyObject *, unsigned PY_LONG_LONG *, Py_ssize_t);
#define PyFLAC_uintX(type,TYPE) \
static int \
PyFLAC_##type##_conv (PyObject *obj, TYPE *v) \
{ \
	unsigned PY_LONG_LONG l; \
	if (!flac_uintX(obj, &l, sizeof(TYPE))) return 0; \
	*v = (TYPE) l; \
	return 1; \
}

#define PyFLAC_conv_PUT(type,i) PyFLAC__C_API_PUT(i,PyFLAC_##type##_conv)

PyFLAC__C_API_DEF(22)
#define _c_api_init { \
	PyFLAC_type_PUT(EntropyCodingMethodType,_EntropyCodingMethodType_type_) \
	PyFLAC_type_PUT(SubframeType,_SubframeType_type_) \
	PyFLAC_type_PUT(ChannelAssignment,_ChannelAssignment_type_) \
	PyFLAC_type_PUT(FrameNumberType,_FrameNumberType_type_) \
	PyFLAC_type_PUT(MetadataType,_MetadataType_type_) \
	PyFLAC_type_PUT(FlacFormatError,_FlacFormatError_type_) \
	PyFLAC_Enum_FromEnum_PUT(EntropyCodingMethodType,_EntropyCodingMethodType_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(SubframeType,_SubframeType_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(ChannelAssignment,_ChannelAssignment_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(FrameNumberType,_FrameNumberType_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(MetadataType,_MetadataType_FromEnum_) \
	PyFLAC__C_API_PUT(_Enum_Ready_,PyFLAC_Enum_Ready) \
	PyFLAC__C_API_PUT(_Enum_FromInt_,PyFLAC_Enum_FromInt) \
	PyFLAC__C_API_PUT(_Iter_New_,PyFLAC_Iter_New) \
	PyFLAC_conv_PUT(application_id,_application_id_conv_)\
	PyFLAC_conv_PUT(bool,_bool_conv_)\
	PyFLAC_conv_PUT(uint8,_uint8_conv_)\
	PyFLAC_conv_PUT(uint32,_uint32_conv_)\
	PyFLAC_conv_PUT(uint64,_uint64_conv_)\
	PyFLAC_conv_PUT(unsigned,_unsigned_conv_)\
	PyFLAC__C_API_PUT(_list_of_type_,PyFLAC_list_of_type) \
	PyFLAC__C_API_PUT(_free_list_of_type_,PyFLAC_free_list_of_type) \
	PyFLAC__C_API_INIT(format) \
	PyFLAC__C_API_CHECK \
}

PyFLAC_Enum_FromEnum_DEF(EntropyCodingMethodType);
PyFLAC_Enum_FromEnum_DEF(SubframeType);
PyFLAC_Enum_FromEnum_DEF(ChannelAssignment);
PyFLAC_Enum_FromEnum_DEF(FrameNumberType);
PyFLAC_Enum_FromEnum_DEF(MetadataType);

static int PyFLAC_Enum_Ready (PyTypeObject *, PyFLAC_Enum_Member_Def *);
static PyObject * PyFLAC_Enum_FromInt (int, PyFLAC_Enum_Member_Def *, const char *);

static PyObject * PyFLAC_Iter_New (const void *, Py_ssize_t, PyFLAC_Iter_Next, const char *, PyObject *);

static PyObject *flac_FlacFormatErrorType;
#define PyFLAC_FlacFormatErrorType (*flac_FlacFormatErrorType)

static int PyFLAC_application_id_conv (PyObject *, FLAC__byte (*)[4]);
static int PyFLAC_bool_conv (PyObject *, FLAC__bool *);
PyFLAC_uintX(uint8,FLAC__uint8)
PyFLAC_uintX(uint32,FLAC__uint32)
PyFLAC_uintX(uint64,FLAC__uint64)
PyFLAC_uintX(unsigned,unsigned)

static int PyFLAC_list_of_type (PyObject *, PyTypeObject *, PyObject ***, Py_ssize_t *);
static void PyFLAC_free_list_of_type (PyObject **, Py_ssize_t);


#else // __PyFLAC_format_MODULE__

PyFLAC__C_API(format)

#define PyFLAC_EntropyCodingMethodTypeType \
	PyFLAC_type_API(format,_EntropyCodingMethodType_type_)
#define PyFLAC_SubframeTypeType \
	PyFLAC_type_API(format,_SubframeType_type_)
#define PyFLAC_ChannelAssignmentType \
	PyFLAC_type_API(format,_ChannelAssignment_type_)
#define PyFLAC_FrameNumberTypeType \
	PyFLAC_type_API(format,_FrameNumberType_type_)
#define PyFLAC_MetadataTypeType \
	PyFLAC_type_API(format,_MetadataType_type_)

#define PyFLAC_FlacFormatErrorType \
	(*(PyObject *) PyFLAC_API(format)[_FlacFormatError_type_])

#define PyFLAC_EntropyCodingMethodType_FromEnum \
	PyFLAC_Enum_FromEnum_API(format,_EntropyCodingMethodType_FromEnum_)
#define PyFLAC_SubframeType_FromEnum \
	PyFLAC_Enum_FromEnum_API(format,_SubframeType_FromEnum_)
#define PyFLAC_ChannelAssignment_FromEnum \
	PyFLAC_Enum_FromEnum_API(format,_ChannelAssignment_FromEnum_)
#define PyFLAC_FrameNumberType_FromEnum \
	PyFLAC_Enum_FromEnum_API(format,_FrameNumberType_FromEnum_)
#define PyFLAC_MetadataType_FromEnum \
	PyFLAC_Enum_FromEnum_API(format,_MetadataType_FromEnum_)

#define PyFLAC_Enum_Ready \
	(*(int (*)(PyTypeObject *, PyFLAC_Enum_Member_Def *)) PyFLAC_API(format)[_Enum_Ready_])
#define PyFLAC_Enum_FromInt \
	(*(PyObject * (*)(int, PyFLAC_Enum_Member_Def *, const char *)) PyFLAC_API(format)[_Enum_FromInt_])

#define PyFLAC_Iter_New \
	(*(PyObject * (*)(const void *, Py_ssize_t, PyFLAC_Iter_Next, const char *, PyObject *)) PyFLAC_API(format)[_Iter_New_])

#define PyFLAC_application_id_conv \
	(*(int (*)(PyObject *, FLAC__byte (*)[4])) PyFLAC_API(format)[_application_id_conv_])
#define PyFLAC_conv_DEF(TYPE,i) \
	(*(int (*)(PyObject *, TYPE *)) PyFLAC_API(format)[i])
#define PyFLAC_bool_conv PyFLAC_conv_DEF(FLAC__bool,_bool_conv_)
#define PyFLAC_uint8_conv PyFLAC_conv_DEF(FLAC__uint8,_uint8_conv_)
#define PyFLAC_uint32_conv PyFLAC_conv_DEF(FLAC__uint32,_uint32_conv_)
#define PyFLAC_uint64_conv PyFLAC_conv_DEF(FLAC__uint64,_uint64_conv_)
#define PyFLAC_unsigned_conv  PyFLAC_conv_DEF(unsigned,_unsigned_conv_)

#define PyFLAC_list_of_type \
	(*(int (*)(PyObject *, PyTypeObject *, PyObject ***, Py_ssize_t *)) PyFLAC_API(format)[_list_of_type_])
#define PyFLAC_free_list_of_type \
	(*(void (*)(PyObject **, Py_ssize_t)) PyFLAC_API(format)[_free_list_of_type_])


#endif // __PyFLAC_format_MODULE__


#define PyFLAC_byte_conv PyFLAC_uint8_conv

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PyFLAC_format_h__

