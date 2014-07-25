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

#include "enum.h"

#include "_C_API.h"

#define			_EntropyCodingMethodType_type_						 0
#define			_SubframeType_type_									 1
#define			_ChannelAssignment_type_							 2
#define			_FrameNumberType_type_								 3
#define			_MetadataType_type_									 4
#define			_StreamMetadataPictureType_type_					 5

#define			_EntropyCodingMethodType_FromEnum_					 6
#define			_SubframeType_FromEnum_								 7
#define			_ChannelAssignment_FromEnum_						 8
#define			_FrameNumberType_FromEnum_							 9
#define			_MetadataType_FromEnum_								10
#define			_StreamMetadataPictureType_FromEnum_				11


#ifdef __PyFLAC_format_MODULE__

PyFLAC__C_API_DEF(12)
#define _c_api_init { \
	PyFLAC_type_PUT(EntropyCodingMethodType,_EntropyCodingMethodType_type_) \
	PyFLAC_type_PUT(SubframeType,_SubframeType_type_) \
	PyFLAC_type_PUT(ChannelAssignment,_ChannelAssignment_type_) \
	PyFLAC_type_PUT(FrameNumberType,_FrameNumberType_type_) \
	PyFLAC_type_PUT(MetadataType,_MetadataType_type_) \
	PyFLAC_type_PUT(StreamMetadataPictureType,_StreamMetadataPictureType_type_) \
	PyFLAC_Enum_FromEnum_PUT(EntropyCodingMethodType,_EntropyCodingMethodType_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(SubframeType,_SubframeType_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(ChannelAssignment,_ChannelAssignment_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(FrameNumberType,_FrameNumberType_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(MetadataType,_MetadataType_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamMetadataPictureType,_StreamMetadataPictureType_FromEnum_) \
	PyFLAC__C_API_INIT(format) \
	PyFLAC__C_API_CHECK \
}

PyFLAC_Enum_FromEnum_DEF(EntropyCodingMethodType);
PyFLAC_Enum_FromEnum_DEF(SubframeType);
PyFLAC_Enum_FromEnum_DEF(ChannelAssignment);
PyFLAC_Enum_FromEnum_DEF(FrameNumberType);
PyFLAC_Enum_FromEnum_DEF(MetadataType);
PyFLAC_Enum_FromEnum_DEF(StreamMetadataPictureType);

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
#define PyFLAC_StreamMetadataPictureTypeType \
	PyFLAC_type_API(format,_StreamMetadataPictureType_type_)

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
#define PyFLAC_StreamMetadataPictureType_FromEnum \
	PyFLAC_Enum_FromEnum_API(format,_StreamMetadataPictureType_FromEnum_)

#endif // __PyFLAC_format_MODULE__


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PyFLAC_format_h__

