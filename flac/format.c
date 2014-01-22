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

#include <Python.h>

#include "PyFLAC.h"
#include "format.h"

#include "enum.h"


PyFLAC_Enum(ChannelAssignment)
PyFLAC_Enum(MetadataType)


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


int
PyFLAC_PyModule_AddFormatObjects (PyObject *module)
{
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(ChannelAssignment), enum_member_ChannelAssignment))
	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(MetadataType), enum_member_MetadataType))

	Py_INCREF(PyFLAC_type(ChannelAssignment));
	PyFLAC_CHECK_status(PyModule_AddObject(module, "ChannelAssignment", (PyObject *) PyFLAC_type(ChannelAssignment)))
	
	Py_INCREF(PyFLAC_type(MetadataType));
	PyFLAC_CHECK_status(PyModule_AddObject(module, "MetadataType", (PyObject *) PyFLAC_type(MetadataType)))

	return 0;
}


int
PyFLAC_Format_MetadataType_Check (PyObject *object)
{
	return PyObject_TypeCheck(object, PyFLAC_type(MetadataType));
}


PyObject *
PyFLAC_Format_ChannelAssignment_FromEnum (FLAC__ChannelAssignment e_value)
{
	return enum_member_MetadataType[e_value].e_object;
}

