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

#include "_enum.h"


PyFLAC_Enum(EntropyCodingMethodType)
PyFLAC_Enum(SubframeType)
PyFLAC_Enum(ChannelAssignment)
PyFLAC_Enum(FrameNumberType)
PyFLAC_Enum(MetadataType)


static flac_Enum_Member enum_member_EntropyCodingMethodType[] = {
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


static flac_Enum_Member enum_member_SubframeType[] = {
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


static flac_Enum_Member enum_member_ChannelAssignment[] = {
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


static flac_Enum_Member enum_member_FrameNumberType[] = {
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


static flac_Enum_Member enum_member_MetadataType[] = {
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

