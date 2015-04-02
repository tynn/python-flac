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

#ifndef __PyFLAC_decoder_h__
#define __PyFLAC_decoder_h__

#include <FLAC/stream_decoder.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
	PyObject_HEAD
	FLAC__StreamDecoder *decoder;
	PyObject *stream;
} flac_StreamDecoderObject;


#include "_C_API.h"

#define			_StreamDecoder_type_								 0
#define			_StreamDecoderState_type_							 1
#define			_StreamDecoderInitStatus_type_						 2
#define			_StreamDecoderReadStatus_type_						 3
#define			_StreamDecoderSeekStatus_type_						 4
#define			_StreamDecoderTellStatus_type_						 5
#define			_StreamDecoderLengthStatus_type_					 6
#define			_StreamDecoderWriteStatus_type_						 7
#define			_StreamDecoderErrorStatus_type_						 8

#define			_StreamDecoderState_FromEnum_						 9
#define			_StreamDecoderInitStatus_FromEnum_					10
#define			_StreamDecoderReadStatus_FromEnum_					11
#define			_StreamDecoderSeekStatus_FromEnum_					12
#define			_StreamDecoderTellStatus_FromEnum_					13
#define			_StreamDecoderLengthStatus_FromEnum_				14
#define			_StreamDecoderWriteStatus_FromEnum_					15
#define			_StreamDecoderErrorStatus_FromEnum_					16


#ifdef __PyFLAC_decoder_MODULE__

#include "format.h"

PyFLAC__C_API_DEF(17)
#define _c_api_init { \
	PyFLAC_type_PUT(StreamDecoder,_StreamDecoder_type_) \
	PyFLAC_type_PUT(StreamDecoderState,_StreamDecoderState_type_) \
	PyFLAC_type_PUT(StreamDecoderInitStatus,_StreamDecoderInitStatus_type_) \
	PyFLAC_type_PUT(StreamDecoderReadStatus,_StreamDecoderReadStatus_type_) \
	PyFLAC_type_PUT(StreamDecoderSeekStatus,_StreamDecoderSeekStatus_type_) \
	PyFLAC_type_PUT(StreamDecoderTellStatus,_StreamDecoderTellStatus_type_) \
	PyFLAC_type_PUT(StreamDecoderLengthStatus,_StreamDecoderLengthStatus_type_) \
	PyFLAC_type_PUT(StreamDecoderWriteStatus,_StreamDecoderWriteStatus_type_) \
	PyFLAC_type_PUT(StreamDecoderErrorStatus,_StreamDecoderErrorStatus_type_) \
	PyFLAC_Enum_FromEnum_PUT(StreamDecoderState,_StreamDecoderState_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamDecoderInitStatus,_StreamDecoderInitStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamDecoderReadStatus,_StreamDecoderReadStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamDecoderSeekStatus,_StreamDecoderSeekStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamDecoderTellStatus,_StreamDecoderTellStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamDecoderLengthStatus,_StreamDecoderLengthStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamDecoderWriteStatus,_StreamDecoderWriteStatus_FromEnum_) \
	PyFLAC_Enum_FromEnum_PUT(StreamDecoderErrorStatus,_StreamDecoderErrorStatus_FromEnum_) \
	PyFLAC__C_API_INIT(decoder) \
	PyFLAC__C_API_CHECK \
}

PyFLAC_Enum_FromEnum_DEF(StreamDecoderState);
PyFLAC_Enum_FromEnum_DEF(StreamDecoderInitStatus);
PyFLAC_Enum_FromEnum_DEF(StreamDecoderReadStatus);
PyFLAC_Enum_FromEnum_DEF(StreamDecoderSeekStatus);
PyFLAC_Enum_FromEnum_DEF(StreamDecoderTellStatus);
PyFLAC_Enum_FromEnum_DEF(StreamDecoderLengthStatus);
PyFLAC_Enum_FromEnum_DEF(StreamDecoderWriteStatus);
PyFLAC_Enum_FromEnum_DEF(StreamDecoderErrorStatus);

#else // __PyFLAC_decoder_h__

PyFLAC__C_API(decoder)

#define PyFLAC_StreamDecoderType \
	PyFLAC_type_API(decoder,_StreamDecoder_type_)
#define PyFLAC_StreamDecoderStateType \
	PyFLAC_type_API(decoder,_StreamDecoderState_type_)
#define PyFLAC_StreamDecoderInitStatusType \
	PyFLAC_type_API(decoder,_StreamDecoderInitStatus_type_)
#define PyFLAC_StreamDecoderReadStatusType \
	PyFLAC_type_API(decoder,_StreamDecoderReadStatus_type_)
#define PyFLAC_StreamDecoderSeekStatusType \
	PyFLAC_type_API(decoder,_StreamDecoderSeekStatus_type_)
#define PyFLAC_StreamDecoderTellStatusType \
	PyFLAC_type_API(decoder,_StreamDecoderTellStatus_type_)
#define PyFLAC_StreamDecoderLengthStatusType \
	PyFLAC_type_API(decoder,_StreamDecoderLengthStatus_type_)
#define PyFLAC_StreamDecoderWriteStatusType \
	PyFLAC_type_API(decoder,_StreamDecoderWriteStatus_type_)
#define PyFLAC_StreamDecoderErrorStatusType \
	PyFLAC_type_API(decoder,_StreamDecoderErrorStatus_type_)

#define PyFLAC_StreamDecoderState_FromEnum \
	PyFLAC_Enum_FromEnum_API(decoder,_StreamDecoderState_FromEnum_)
#define PyFLAC_StreamDecoderInitStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(decoder,_StreamDecoderInitStatus_FromEnum_)
#define PyFLAC_StreamDecoderReadStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(decoder,_StreamDecoderReadStatus_FromEnum_)
#define PyFLAC_StreamDecoderSeekStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(decoder,_StreamDecoderSeekStatus_FromEnum_)
#define PyFLAC_StreamDecoderTellStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(decoder,_StreamDecoderTellStatus_FromEnum_)
#define PyFLAC_StreamDecoderLengthStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(decoder,_StreamDecoderLengthStatus_FromEnum_)
#define PyFLAC_StreamDecoderWriteStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(decoder,_StreamDecoderWriteStatus_FromEnum_)
#define PyFLAC_StreamDecoderErrorStatus_FromEnum \
	PyFLAC_Enum_FromEnum_API(decoder,_StreamDecoderErrorStatus_FromEnum_)

#endif // __PyFLAC_decoder_MODULE__


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PyFLAC_decoder_h__

