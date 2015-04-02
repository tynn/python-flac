#	This file is part of python-flac.
#
#	Copyright (c) 2015 Christian Schmitz <tynn.dev@gmail.com>
#
#	python-flac is free software: you can redistribute it and/or modify
#	it under the terms of the GNU Lesser General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	python-flac is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU Lesser General Public License for more details.
#
#	You should have received a copy of the GNU Lesser General Public License
#	along with python-flac.  If not, see <http://www.gnu.org/licenses/>.

""" Streams for decoding """

from .decoder import *

def decoder_init (init_func, *init_args) :
	init_status = init_func(*init_args)
	if init_status is StreamDecoderInitStatus.OK :
		return True
	if init_status is StreamDecoderInitStatus.ALREADY_INITIALIZED :
		return False
	raise FlacFormatError(init_status)


class StreamDecoderSetup (StreamDecoder) :

	def __init__ (self, **kwargs) :
		self._decoder_setup(**kwargs)

	def _decoder_setup (self, **kwargs) :
		if 'metadata_ignore_all' in kwargs and kwargs['metadata_ignore_all'] :
			self.set_metadata_ignore_all()
		elif 'metadata_respond_all' in kwargs and kwargs['metadata_respond_all'] :
			self.set_metadata_respond_all()
		else :
			if 'metadata_ignore' in kwargs :
				ignore = set(kwargs['metadata_ignore'])
				for metadata in ignore :
					self.set_metadata_ignore(metadata)
			else :
				ignore = set()
			if 'metadata_respond' in kwargs :
				for metadata in set(kwargs['metadata_respond']) - ignore :
					self.set_metadata_respond(metadata)
			if 'metadata_ignore_application' in kwargs :
				ignore = set(kwargs['metadata_ignore_application'])
				for app in ignore :
					self.set_metadata_ignore_application(app)
			else :
				ignore = set()
			if 'metadata_respond_application' in kwargs :
				for app in set(kwargs['metadata_respond_application']) - ignore :
					self.set_metadata_respond_application(app)
		if 'ogg_serial_number' in kwargs :
			self.set_ogg_serial_number(kwargs['ogg_serial_number'])
		if 'md5_checking' in kwargs :
			self.set_md5_checking(kwargs['md5_checking'])


class DecoderFileStream (object) :

	def __init__ (self, file) :
		try :
			file.seek(0, 2)
			self._decoder_length = file.tell()
			file.seek(0)
		except IOError :
			self._decoder_length = None
		self._decoder_eof = False
		self._decoder_file = file

	def __decoder_read__ (self, bytes) :
		data = self._decoder_file.read(bytes)
		if not data :
			self._decoder_eof = True
		return data

	def __decoder_seek__ (self, offset) :
		try : return bool(self._decoder_file.seek(offset)) or True
		except IOError : return False

	def __decoder_tell__ (self) :
		try : return self._decoder_file.tell()
		except IOError : return False

	def __decoder_length__ (self) :
		return self._decoder_length

	def __decoder_eof__ (self) :
		return self._decoder_eof


class FileDecoder (StreamDecoderSetup, DecoderFileStream) :

	def __init__ (self, file, ogg=False, **kwargs) :
		self.finish()
		StreamDecoderSetup.__init__(self, **kwargs)
		DecoderFileStream.__init__(self, file)
		decoder_init(self.init_ogg_stream if ogg else self.init_stream)

class StdinDecoder (FileDecoder) :

	def __init__ (self, ogg=False, **kwargs) :
		from sys import stdin
		FileDecoder.__init__(self, stdin, ogg, **kwargs)


class PcmDecoder (StreamDecoder) :
	from itertools import islice as _slice

	def __init__ (self, big_endian=None) :
		from sys import byteorder
		self.I = (3, 2, 1, 0) if byteorder == 'big' else (0, 1, 0, 1)

	def __decoder_write__ (self, frame, buffer) :
		if frame.channels > 2 :
			raise NotImplementedError("only one or two channels are supported")
		l = frame.channels * 2
		pcm = bytearray(l * frame.blocksize)
		for n, data in enumerate(buffer) :
			pcm[2*n+self.I[2]::l] = self._slice(data, self.I[0], None, 4)
			pcm[2*n+self.I[3]::l] = self._slice(data, self.I[1], None, 4)
		pcm = bytes(pcm)
		return self.__decoder_write_pcm__(pcm)

