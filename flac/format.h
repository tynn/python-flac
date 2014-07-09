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

#define			_EntropyCodingMethodType_Check_						 0
#define			_SubframeType_Check_								 1
#define			_ChannelAssignment_Check_							 2
#define			_FrameNumberType_Check_								 3
#define			_MetadataType_Check_								 4
#define			_StreamMetadataPictureType_Check_					 5

#define			_EntropyCodingMethodType_FromEnum_					 6
#define			_SubframeType_FromEnum_								 7
#define			_ChannelAssignment_FromEnum_						 8
#define			_FrameNumberType_FromEnum_							 9
#define			_MetadataType_FromEnum_								10
#define			_StreamMetadataPictureType_FromEnum_				11


#ifdef __PyFLAC_format_MODULE__

PyFLAC_type_Check_DEF(EntropyCodingMethodType);
PyFLAC_type_Check_DEF(SubframeType);
PyFLAC_type_Check_DEF(ChannelAssignment);
PyFLAC_type_Check_DEF(FrameNumberType);
PyFLAC_type_Check_DEF(MetadataType);
PyFLAC_type_Check_DEF(StreamMetadataPictureType);

PyFLAC_Enum_FromEnum_DEF(EntropyCodingMethodType);
PyFLAC_Enum_FromEnum_DEF(SubframeType);
PyFLAC_Enum_FromEnum_DEF(ChannelAssignment);
PyFLAC_Enum_FromEnum_DEF(FrameNumberType);
PyFLAC_Enum_FromEnum_DEF(MetadataType);
PyFLAC_Enum_FromEnum_DEF(StreamMetadataPictureType);

PyFLAC__C_API_INIT_begin(12)
	PyFLAC_type_Check_PUT(EntropyCodingMethodType,_EntropyCodingMethodType_Check_);
	PyFLAC_type_Check_PUT(SubframeType,_SubframeType_Check_);
	PyFLAC_type_Check_PUT(ChannelAssignment,_ChannelAssignment_Check_);
	PyFLAC_type_Check_PUT(FrameNumberType,_FrameNumberType_Check_);
	PyFLAC_type_Check_PUT(MetadataType,_MetadataType_Check_);
	PyFLAC_type_Check_PUT(StreamMetadataPictureType,_StreamMetadataPictureType_Check_);

	PyFLAC_Enum_FromEnum_PUT(EntropyCodingMethodType,_EntropyCodingMethodType_FromEnum_);
	PyFLAC_Enum_FromEnum_PUT(SubframeType,_SubframeType_FromEnum_);
	PyFLAC_Enum_FromEnum_PUT(ChannelAssignment,_ChannelAssignment_FromEnum_);
	PyFLAC_Enum_FromEnum_PUT(FrameNumberType,_FrameNumberType_FromEnum_);
	PyFLAC_Enum_FromEnum_PUT(MetadataType,_MetadataType_FromEnum_);
	PyFLAC_Enum_FromEnum_PUT(StreamMetadataPictureType,_StreamMetadataPictureType_FromEnum_);
PyFLAC__C_API_INIT_end(format)

#else

PyFLAC__C_API(format)

#define PyFLAC_EntropyCodingMethodType_Check \
	PyFLAC_type_Check_API(format,_EntropyCodingMethodType_Check_)
#define PyFLAC_SubframeType_Check \
	PyFLAC_type_Check_API(format,_SubframeType_Check_)
#define PyFLAC_ChannelAssignment_Check \
	PyFLAC_type_Check_API(format,_ChannelAssignment_Check_)
#define PyFLAC_FramtypeberType_Check \
	PyFLAC_type_Check_API(format,_FrameNumberType_Check_)
#define PyFLAC_MetadataType_Check \
	PyFLAC_type_Check_API(format,_MetadataType_Check_)
#define PyFLAC_StreamMetadataPictureType_Check \
	PyFLAC_type_Check_API(format,_StreamMetadataPictureType_Check_)

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

