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

#ifndef __PyFLAC_metadata_h__
#define __PyFLAC_metadata_h__

#include <FLAC/metadata.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "_C_API.h"

#define			_StreamMetadata_type_								 0
#define			_StreamMetadataStreamInfoData_type_					 1
#define			_StreamMetadataApplicationData_type_				 2
#define			_StreamMetadataSeekPoint_type_						 3
#define			_StreamMetadataSeekTableData_type_					 4
#define			_StreamMetadataVorbisCommentEntry_type_				 5
#define			_StreamMetadataVorbisCommentData_type_				 6
#define			_StreamMetadataCueSheetIndex_type_					 7
#define			_StreamMetadataCueSheetTrack_type_					 8
#define			_StreamMetadataCueSheetData_type_					 9
#define			_StreamMetadataPictureData_type_					10

#define			_StreamMetadata_FromClass_							11


#ifdef __PyFLAC_metadata_MODULE__

PyFLAC__C_API_DEF(12)
#define _c_api_init { \
	PyFLAC_type_PUT(StreamMetadata,_StreamMetadata_type_) \
	PyFLAC_type_PUT(StreamMetadataStreamInfoData,_StreamMetadataStreamInfoData_type_) \
	PyFLAC_type_PUT(StreamMetadataApplicationData,_StreamMetadataApplicationData_type_) \
	PyFLAC_type_PUT(StreamMetadataSeekPoint,_StreamMetadataSeekPoint_type_) \
	PyFLAC_type_PUT(StreamMetadataSeekTableData,_StreamMetadataSeekTableData_type_) \
	PyFLAC_type_PUT(StreamMetadataVorbisCommentEntry,_StreamMetadataVorbisCommentEntry_type_) \
	PyFLAC_type_PUT(StreamMetadataVorbisCommentData,_StreamMetadataVorbisCommentData_type_) \
	PyFLAC_type_PUT(StreamMetadataCueSheetIndex,_StreamMetadataCueSheetIndex_type_) \
	PyFLAC_type_PUT(StreamMetadataCueSheetTrack,_StreamMetadataCueSheetTrack_type_) \
	PyFLAC_type_PUT(StreamMetadataCueSheetData,_StreamMetadataCueSheetData_type_) \
	PyFLAC_type_PUT(StreamMetadataPictureData,_StreamMetadataPictureData_type_) \
	PyFLAC__C_API_PUT(_StreamMetadata_FromClass_,PyFLAC_StreamMetadata_FromClass) \
	PyFLAC__C_API_INIT(metadata) \
}

static PyObject * PyFLAC_StreamMetadata_FromClass (const FLAC__StreamMetadata *metadata);

#else // __PyFLAC_metadata_MODULE__

PyFLAC__C_API(metadata)

#define PyFLAC_StreamMetadataType \
	PyFLAC_type_API(metadata,_StreamMetadata_type_)
#define PyFLAC_StreamMetadataStreamInfoDataType \
	PyFLAC_type_API(metadata,_StreamMetadataStreamInfoData_type_)
#define PyFLAC_StreamMetadataApplicationDataType \
	PyFLAC_type_API(metadata,_StreamMetadataApplicationData_type_)
#define PyFLAC_StreamMetadataSeekPointType \
	PyFLAC_type_API(metadata,_StreamMetadataSeekPoint_type_)
#define PyFLAC_StreamMetadataSeekTableDataType \
	PyFLAC_type_API(metadata,_StreamMetadataSeekTableData_type_)
#define PyFLAC_StreamMetadataVorbisCommentEntryType \
	PyFLAC_type_API(metadata,_StreamMetadataVorbisCommentEntry_type_)
#define PyFLAC_StreamMetadataVorbisCommentDataType \
	PyFLAC_type_API(metadata,_StreamMetadataVorbisCommentData_type_)
#define PyFLAC_StreamMetadataCueSheetIndexType \
	PyFLAC_type_API(metadata,_StreamMetadataCueSheetIndex_type_)
#define PyFLAC_StreamMetadataCueSheetTrackType \
	PyFLAC_type_API(metadata,_StreamMetadataCueSheetTrack_type_)
#define PyFLAC_StreamMetadataCueSheetDataType \
	PyFLAC_type_API(metadata,_StreamMetadataCueSheetData_type_)
#define PyFLAC_StreamMetadataPictureDataType \
	PyFLAC_type_API(metadata,_StreamMetadataPictureData_type_)

#define PyFLAC_StreamMetadata_FromClass \
	(*(PyObject * (*)(const FLAC__StreamMetadata *)) PyFLAC_API(metadata)[_StreamMetadata_FromClass_])

#endif // __PyFLAC_metadata_MODULE__


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PyFLAC_metadata_h__

