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

#define			_StreamMetadata_Check_								 1
#define			_StreamMetadataStreamInfoData_Check_				 2
#define			_StreamMetadataApplicationData_Check_				 3
#define			_StreamMetadataSeekPoint_Check_						 4
#define			_StreamMetadataSeekTableData_Check_					 5
#define			_StreamMetadataVorbisCommentEntry_Check_			 6
#define			_StreamMetadataVorbisCommentData_Check_				 7
#define			_StreamMetadataCueSheetIndex_Check_					 8
#define			_StreamMetadataCueSheetTrack_Check_					 9
#define			_StreamMetadataCueSheetData_Check_					10
#define			_StreamMetadataPictureData_Check_					11

#define			_StreamMetadata_FromClass_							12


#ifdef __PyFLAC_metadata_MODULE__

PyFLAC__C_API_DEF(13)
#define _c_api_init { \
	PyFLAC_type_PUT(StreamMetadata,_StreamMetadata_type_) \
	PyFLAC_type_Check_PUT(StreamMetadata,_StreamMetadata_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataStreamInfoData,_StreamMetadataStreamInfoData_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataApplicationData,_StreamMetadataApplicationData_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataSeekPoint,_StreamMetadataSeekPoint_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataSeekTableData,_StreamMetadataSeekTableData_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataVorbisCommentEntry,_StreamMetadataVorbisCommentEntry_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataVorbisCommentData,_StreamMetadataVorbisCommentData_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataCueSheetIndex,_StreamMetadataCueSheetIndex_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataCueSheetTrack,_StreamMetadataCueSheetTrack_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataCueSheetData,_StreamMetadataCueSheetData_Check_) \
	PyFLAC_type_Check_PUT(StreamMetadataPictureData,_StreamMetadataPictureData_Check_) \
	PyFLAC__C_API_PUT(_StreamMetadata_FromClass_,PyFLAC_StreamMetadata_FromClass) \
	PyFLAC__C_API_INIT(metadata) \
}

PyFLAC_type_Check_DEF(StreamMetadata);
PyFLAC_type_Check_DEF(StreamMetadataStreamInfoData);
PyFLAC_type_Check_DEF(StreamMetadataApplicationData);
PyFLAC_type_Check_DEF(StreamMetadataSeekPoint);
PyFLAC_type_Check_DEF(StreamMetadataSeekTableData);
PyFLAC_type_Check_DEF(StreamMetadataVorbisCommentEntry);
PyFLAC_type_Check_DEF(StreamMetadataVorbisCommentData);
PyFLAC_type_Check_DEF(StreamMetadataCueSheetIndex);
PyFLAC_type_Check_DEF(StreamMetadataCueSheetTrack);
PyFLAC_type_Check_DEF(StreamMetadataCueSheetData);
PyFLAC_type_Check_DEF(StreamMetadataPictureData);

static PyObject * PyFLAC_StreamMetadata_FromClass (const FLAC__StreamMetadata *metadata);

#else

PyFLAC__C_API(metadata)

#define PyFLAC_StreamMetadataType \
	PyFLAC_type_API(metadata,_StreamMetadata_type_)

#define PyFLAC_StreamMetadata_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadata_Check_)
#define PyFLAC_StreamMetadataStreamInfoData_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataStreamInfoData_Check_)
#define PyFLAC_StreamMetadataApplicationData_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataApplicationData_Check_)
#define PyFLAC_StreamMetadataSeekPoint_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataSeekPoint_Check_)
#define PyFLAC_StreamMetadataSeekTableData_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataSeekTableData_Check_)
#define PyFLAC_StreamMetadataVorbisCommentEntry_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataVorbisCommentEntry_Check_)
#define PyFLAC_StreamMetadataVorbisCommentData_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataVorbisCommentData_Check_)
#define PyFLAC_StreamMetadataCueSheetIndex_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataCueSheetIndex_Check_)
#define PyFLAC_StreamMetadataCueSheetTrack_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataCueSheetTrack_Check_)
#define PyFLAC_StreamMetadataCueSheetData_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataCueSheetData_Check_)
#define PyFLAC_StreamMetadataPictureData_Check \
	PyFLAC_type_Check_API(metadata,_StreamMetadataPictureData_Check_)

#define PyFLAC_StreamMetadata_FromClass \
	(*(PyObject * (*)(const FLAC__StreamMetadata *metadata)) PyFLAC_API(metadata)[_StreamMetadata_FromClass_])

#endif // __PyFLAC_metadata_MODULE__


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PyFLAC_metadata_h__

