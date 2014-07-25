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

typedef struct {
	PyObject_HEAD
	FLAC__StreamMetadata *metadata;
	PyObject *data;
} PyFLAC_StreamMetadataObject;

typedef struct {
	PyObject_HEAD
	PyFLAC_StreamMetadataObject *parent;
} PyFLAC_StreamMetadataDataObject;


#include "_C_API.h"

#define			_StreamMetadata_type_								 0
#define			_StreamMetadata_FromClass_							 1


#ifdef __PyFLAC_metadata_MODULE__

PyFLAC__C_API_DEF(2)
#define _c_api_init { \
	PyFLAC_type_PUT(StreamMetadata,_StreamMetadata_type_) \
	PyFLAC__C_API_PUT(_StreamMetadata_FromClass_,PyFLAC_StreamMetadata_FromClass) \
	PyFLAC__C_API_INIT(metadata) \
}

static PyObject * PyFLAC_StreamMetadata_FromClass (const FLAC__StreamMetadata *metadata);

#else // __PyFLAC_metadata_MODULE__

PyFLAC__C_API(metadata)

#define PyFLAC_StreamMetadataType \
	PyFLAC_type_API(metadata,_StreamMetadata_type_)

#define PyFLAC_StreamMetadata_FromClass \
	(*(PyObject * (*)(const FLAC__StreamMetadata *)) PyFLAC_API(metadata)[_StreamMetadata_FromClass_])

#endif // __PyFLAC_metadata_MODULE__


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PyFLAC_metadata_h__

