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
#include "format.h"

#include "_enum.h"


PyFLAC_Enum(EntropyCodingMethodType)
PyFLAC_Enum(SubframeType)
PyFLAC_Enum(ChannelAssignment)
PyFLAC_Enum(FrameNumberType)
PyFLAC_Enum(MetadataType)
PyFLAC_Enum(StreamMetadataPictureType)


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


static PyMethodDef flac_format_functions[] = {
	{ NULL }		/* Sentinel */
};


static int
flac_format_init (PyObject *module)
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

	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(MetadataType), enum_member_MetadataType))
	Py_INCREF(PyFLAC_type(MetadataType));
	PyFLAC_CHECK_status(PyModule_AddObject(module, "MetadataType", (PyObject *) PyFLAC_type(MetadataType)))

	PyFLAC_CHECK_status(PyFLAC_Enum_Ready(PyFLAC_type(StreamMetadataPictureType), enum_member_StreamMetadataPictureType))
	Py_INCREF(PyFLAC_type(StreamMetadataPictureType));
	PyFLAC_CHECK_status(PyModule_AddObject(module, "StreamMetadataPictureType", (PyObject *) PyFLAC_type(StreamMetadataPictureType)))

	return 0;
}


PyFLAC_MODINIT(
	format,
	flac_format_init,
	flac_format_functions,
	"libFLAC format"
)


PyFLAC_type_Check_function(EntropyCodingMethodType)
PyFLAC_type_Check_function(SubframeType)
PyFLAC_type_Check_function(ChannelAssignment)
PyFLAC_type_Check_function(FrameNumberType)
PyFLAC_type_Check_function(MetadataType)
PyFLAC_type_Check_function(StreamMetadataPictureType)


PyFLAC_Enum_FromEnum_function(EntropyCodingMethodType, enum_member_EntropyCodingMethodType)
PyFLAC_Enum_FromEnum_function(SubframeType, enum_member_SubframeType)
PyFLAC_Enum_FromEnum_function(ChannelAssignment, enum_member_ChannelAssignment)
PyFLAC_Enum_FromEnum_function(FrameNumberType, enum_member_FrameNumberType)
PyFLAC_Enum_FromEnum_function(MetadataType, enum_member_MetadataType)
PyFLAC_Enum_FromEnum_function(StreamMetadataPictureType, enum_member_StreamMetadataPictureType)

