/*
 *	This file is part of python-flac.
 *
 *	Copyright (c) 2013 Christian Schmitz <tynn.dev@gmail.com>
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
#include "format.h"

#include "enum.h"


PyFLAC_Enum(EntropyCodingMethodType)
PyFLAC_Enum(SubframeType)
PyFLAC_Enum(ChannelAssignment)
PyFLAC_Enum(FrameNumberType)


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


int
PyFLAC_PyModule_AddFormatObjects (PyObject *module)
{
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(EntropyCodingMethodType), enum_member_EntropyCodingMethodType))
	Py_INCREF(PyFLAC_type(EntropyCodingMethodType));
	PyFLAC_CHECK_status(PyModule_AddObject(module, "EntropyCodingMethodType", (PyObject *) PyFLAC_type(EntropyCodingMethodType)))

	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(SubframeType), enum_member_SubframeType))
	Py_INCREF(PyFLAC_type(SubframeType));
	PyFLAC_CHECK_status(PyModule_AddObject(module, "SubframeType", (PyObject *) PyFLAC_type(SubframeType)))

	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(ChannelAssignment), enum_member_ChannelAssignment))
	Py_INCREF(PyFLAC_type(ChannelAssignment));
	PyFLAC_CHECK_status(PyModule_AddObject(module, "ChannelAssignment", (PyObject *) PyFLAC_type(ChannelAssignment)))

	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(FrameNumberType), enum_member_FrameNumberType))
	Py_INCREF(PyFLAC_type(FrameNumberType));
	PyFLAC_CHECK_status(PyModule_AddObject(module, "FrameNumberType", (PyObject *) PyFLAC_type(FrameNumberType)))

	return 0;
}


PyFLAC_type_Check_function(EntropyCodingMethodType)
PyFLAC_type_Check_function(SubframeType)
PyFLAC_type_Check_function(ChannelAssignment)
PyFLAC_type_Check_function(FrameNumberType)


PyFLAC_Enum_FromEnum_function(EntropyCodingMethodType, enum_member_EntropyCodingMethodType)
PyFLAC_Enum_FromEnum_function(SubframeType, enum_member_SubframeType)
PyFLAC_Enum_FromEnum_function(ChannelAssignment, enum_member_ChannelAssignment)
PyFLAC_Enum_FromEnum_function(FrameNumberType, enum_member_FrameNumberType)

