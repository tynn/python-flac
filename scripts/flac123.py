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
import ao, flac.stream_decoder, os.path, sys

def excepthook (type, value, traceback) :
	import sys
	if type is KeyboardInterrupt :
		print(end='\r')
		sys.exit()
	print(value, file=sys.stderr)
	print("Usage:", sys.argv[0], "[device] flacfile...", file=sys.stderr)
sys.excepthook = excepthook

if os.path.isfile(sys.argv[1]) :
	sys.argv.insert(1, None)

decoder = flac.stream_decoder.PcmDecoder()
decoder.__decoder_write_pcm__ = ao.AudioDevice(sys.argv[1]).play
for filename in sys.argv[2:] :
	flac.stream_decoder.decoder_init(decoder.init_file, filename)
	decoder.process_until_end_of_stream()
	decoder.finish()

