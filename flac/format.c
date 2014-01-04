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
#include <FLAC/format.h>

#include "PyFLAC.h"
#include "format.h"

#include "enum.h"


PyFLAC_ENUM_TYPE(ChannelAssignment)
PyFLAC_ENUM_TYPE(MetadataType)


int
PyFLAC_Format_MetadataType_Check (PyObject *object)
{
	return PyObject_TypeCheck(object, &flac_MetadataTypeType);
}


PyObject *
PyFLAC_Format_ChannelAssignment_FromEnum (FLAC__ChannelAssignment e_value)
{
	PyObject *py_value;

	PyFLAC_ENUM_MEMBER(
		py_value,
		ChannelAssignment,
		e_value
	)

	return py_value;
}


static int
enum_ChannelAssignment ( void )
{
	PyObject *enum_member;

	PyFLAC_ADD_ENUM_MEMBER(
		ChannelAssignment,
		"INDEPENDENT",
		CHANNEL_ASSIGNMENT_INDEPENDENT
	)
	PyFLAC_ADD_ENUM_MEMBER(
		ChannelAssignment,
		"LEFT_SIDE",
		CHANNEL_ASSIGNMENT_LEFT_SIDE
	)
	PyFLAC_ADD_ENUM_MEMBER(
		ChannelAssignment,
		"RIGHT_SIDE",
		CHANNEL_ASSIGNMENT_RIGHT_SIDE
	)
	PyFLAC_ADD_ENUM_MEMBER(
		ChannelAssignment,
		"MID_SIDE",
		CHANNEL_ASSIGNMENT_MID_SIDE
	)

	PyFLAC_ENUM_LOCK(ChannelAssignment)
	return 0;
}


static int
enum_MetadataType ( void )
{
	PyObject *enum_member;

	PyFLAC_ADD_ENUM_MEMBER(
		MetadataType,
		"STREAMINFO",
		METADATA_TYPE_STREAMINFO
	)
	PyFLAC_ADD_ENUM_MEMBER(
		MetadataType,
		"PADDING",
		METADATA_TYPE_PADDING
	)
	PyFLAC_ADD_ENUM_MEMBER(
		MetadataType,
		"APPLICATION",
		METADATA_TYPE_APPLICATION
	)
	PyFLAC_ADD_ENUM_MEMBER(
		MetadataType,
		"SEEKTABLE",
		METADATA_TYPE_SEEKTABLE
	)
	PyFLAC_ADD_ENUM_MEMBER(
		MetadataType,
		"VORBIS_COMMENT",
		METADATA_TYPE_VORBIS_COMMENT
	)
	PyFLAC_ADD_ENUM_MEMBER(
		MetadataType,
		"CUESHEET",
		METADATA_TYPE_CUESHEET
	)
	PyFLAC_ADD_ENUM_MEMBER(
		MetadataType,
		"PICTURE",
		METADATA_TYPE_PICTURE
	)
	PyFLAC_ADD_ENUM_MEMBER(
		MetadataType,
		"UNDEFINED",
		METADATA_TYPE_UNDEFINED
	)

	PyFLAC_ENUM_LOCK(MetadataType)
	return 0;
}


int
PyFLAC_FormatTypes_Ready ( void )
{
	int status;

	PyFLAC_Enum_Ready(ChannelAssignment)
	PyFLAC_Enum_Ready(MetadataType)

	enum_ChannelAssignment();
	enum_MetadataType();

	return status;
}


int
PyFLAC_PyModule_AddFormatObjects (PyObject *module)
{
	int status;

	PyFLAC_Add_Object(ChannelAssignment)
	PyFLAC_Add_Object(MetadataType)

	return status;
}

