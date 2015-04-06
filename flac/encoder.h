/*
 *	This file is part of python-flac.
 *
 *	Copyright (c) 2015 Christian Schmitz <tynn.dev@gmail.com>
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

#ifndef __PyFLAC_encoder_h__
#define __PyFLAC_encoder_h__

#include <FLAC/stream_encoder.h>
#include <FLAC/metadata.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
	PyObject_HEAD
	FLAC__StreamEncoder *encoder;
	PyObject *stream;
	PyObject **metadata;
	Py_ssize_t metadata_count;
} flac_StreamEncoderObject;


#include "_C_API.h"

#define			_StreamEncoder_type_								 0
#define			_StreamEncoderState_type_							 1
#define			_StreamEncoderInitStatus_type_						 2
#define			_StreamEncoderReadStatus_type_						 3
#define			_StreamEncoderSeekStatus_type_						 4
#define			_StreamEncoderTellStatus_type_						 5
#define			_StreamEncoderWriteStatus_type_						 6

#define			_FlacEncoderError_type_							 7

#define			_StreamEncoderState_FromEnum_						 8
#define			_StreamEncoderInitStatus_FromEnum_					 9
#define			_StreamEncoderReadStatus_FromEnum_					10
#define			_StreamEncoderSeekStatus_FromEnum_					11
#define			_StreamEncoderTellStatus_FromEnum_					12
#define			_StreamEncoderWriteStatus_FromEnum_					13


#ifdef __PyFLAC_encoder_MODULE__

#include "format.h"

PyFLAC__C_API_DEF(14)
#define _c_api_init { \
	PyFLAC_type_PUT(StreamEncoder,_StreamEncoder_type_) \
	PyFLAC_type_PUT(StreamEncoderState,_StreamEncoderState_type_) \
	PyFLAC_type_PUT(StreamEncoderInitStatus,_StreamEncoderInitStatus_type_) \
	PyFLAC_type_PUT(StreamEncoderReadStatus,_StreamEncoderReadStatus_type_) \
	PyFLAC_type_PUT(StreamEncoderSeekStatus,_StreamEncoderSeekStatus_type_) \
	PyFLAC_type_PUT(StreamEncoderTellStatus,_StreamEncoderTellStatus_type_) \
	PyFLAC_type_PUT(StreamEncoderWriteStatus,_StreamEncoderWriteStatus_type_) \
	PyFLAC_Enum_FromEnum_PUT(StreamEncoderState,_StreamEncoderState_FromEnum_) \
	PyFLAC_type_PUT(FlacEncoderError,_FlacEncoderError_type_) \
	PyFLAC_Enum_FromEnum_PUT(StreamEncoderInitStatus,_StreamEncoderInitStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamEncoderReadStatus,_StreamEncoderReadStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamEncoderSeekStatus,_StreamEncoderSeekStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamEncoderTellStatus,_StreamEncoderTellStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamEncoderWriteStatus,_StreamEncoderWriteStatus_FromEnum_) \
	PyFLAC__C_API_INIT(encoder) \
	PyFLAC__C_API_CHECK \
}

static PyObject *flac_FlacEncoderErrorType;
#define PyFLAC_FlacEncoderErrorType (*flac_FlacEncoderErrorType)

PyFLAC_Enum_FromEnum_DEF(StreamEncoderState);
PyFLAC_Enum_FromEnum_DEF(StreamEncoderInitStatus);
PyFLAC_Enum_FromEnum_DEF(StreamEncoderReadStatus);
PyFLAC_Enum_FromEnum_DEF(StreamEncoderSeekStatus);
PyFLAC_Enum_FromEnum_DEF(StreamEncoderTellStatus);
PyFLAC_Enum_FromEnum_DEF(StreamEncoderWriteStatus);

#else // __PyFLAC_encoder_h__

PyFLAC__C_API(encoder)

#define PyFLAC_StreamEncoderType \
	PyFLAC_type_API(encoder,_StreamEncoder_type_)
#define PyFLAC_StreamEncoderStateType \
	PyFLAC_type_API(encoder,_StreamEncoderState_type_)
#define PyFLAC_StreamEncoderInitStatusType \
	PyFLAC_type_API(encoder,_StreamEncoderInitStatus_type_)
#define PyFLAC_StreamEncoderReadStatusType \
	PyFLAC_type_API(encoder,_StreamEncoderReadStatus_type_)
#define PyFLAC_StreamEncoderSeekStatusType \
	PyFLAC_type_API(encoder,_StreamEncoderSeekStatus_type_)
#define PyFLAC_StreamEncoderTellStatusType \
	PyFLAC_type_API(encoder,_StreamEncoderTellStatus_type_)
#define PyFLAC_StreamEncoderWriteStatusType \
	PyFLAC_type_API(encoder,_StreamEncoderWriteStatus_type_)

#define PyFLAC_FlacEncoderErrorType \
	(*(PyObject *) PyFLAC_API(format)[_FlacEncoderError_type_])

#define PyFLAC_StreamEncoderState_FromEnum \
	PyFLAC_Enum_FromEnum_API(encoder,_StreamEncoderState_FromEnum_)
#define PyFLAC_StreamEncoderInitStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(encoder,_StreamEncoderInitStatus_FromEnum_)
#define PyFLAC_StreamEncoderReadStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(encoder,_StreamEncoderReadStatus_FromEnum_)
#define PyFLAC_StreamEncoderSeekStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(encoder,_StreamEncoderSeekStatus_FromEnum_)
#define PyFLAC_StreamEncoderTellStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(encoder,_StreamEncoderTellStatus_FromEnum_)
#define PyFLAC_StreamEncoderWriteStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(encoder,_StreamEncoderWriteStatus_FromEnum_)

#endif // __PyFLAC_encoder_MODULE__


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PyFLAC_encoder_h__

