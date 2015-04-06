#!/usr/bin/env python
#	This file is part of python-flac.
#
#	Copyright (c) 2015 Christian Schmitz <tynn.dev@gmail.com>
#
#	python-flac is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	python-flac is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with python-flac.  If not, see <http://www.gnu.org/licenses/>.

from __future__ import print_function
import os.path, sys, flac23
from flac.decoder import StreamDecoder, MetadataType
from flac.encoder import StreamEncoder

class Flac8 (StreamDecoder) :

	def __init__ (self, filename) :
		self._streaminfo = None
		self._metadata = []
		StreamDecoder.__init__(self)
		self.set_metadata_respond_all()
		self.init_file(filename)

	def __decoder_metadata__ (self, metadata) :
		if metadata.type is MetadataType.STREAMINFO :
			self._streaminfo = metadata
		else :
			self._metadata.append(metadata)

	def __decoder_write__ (self, frame, buffer) :
		self._encoder.process(buffer)

	def transcode (self, filename) :
		self.process_until_end_of_metadata()
		self.reset()
		self._encoder = StreamEncoder()
		self._encoder.set_sample_rate(self._streaminfo.sample_rate)
		self._encoder.set_channels(self._streaminfo.channels)
		self._encoder.set_bits_per_sample(self._streaminfo.bits_per_sample)
		self._encoder.set_compression_level(101)
		if self._metadata :
			self._encoder.set_metadata(self._metadata)
		self._encoder.init_file(filename)
		self.process_until_end_of_stream()
		self._encoder.finish()
		del self._encoder


if '-f' in sys.argv :
	sys.argv.remove('-f')
elif os.path.exists(sys.argv[2]) :
	print("Usage:", sys.argv[0], "[-f] input output", file=sys.stderr)
	sys.exit(1)
Flac8(sys.argv[1]).transcode(sys.argv[2])

