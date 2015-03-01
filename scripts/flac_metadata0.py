#!/usr/bin/env python
#	This file is part of python-flac.
#
#	Copyright (c) 2014 Christian Schmitz <tynn.dev@gmail.com>
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

import flac.metadata0 as flac
import mimetypes, os, signal, sys

def _printf (format, *args) :
	print (format.format(*args))

def _printt2 (tuple, fill = 1) :
	_printf('{1[0]:{0}}   {1[1]}', fill, tuple)

def _printt (*tuples) :
	if tuples :
		tuples = list(filter(bool, tuples))
		fill = len(max(tuples, key=lambda t : len(t[0]))[0])
		for tuple in tuples :
			_printt2(tuple, fill)

def get_streaminfo (filename) :
	metadata = flac.get_streaminfo(filename)
	if metadata :
		_printt(
			('min blocksize', metadata.min_blocksize),
			('max blocksize', metadata.max_blocksize),
			('min framesize', metadata.min_framesize),
			('max framesize', metadata.max_framesize),
			('sample rate', metadata.sample_rate),
			('channels', metadata.channels),
			('bits per sample', metadata.bits_per_sample),
			('total samples', metadata.total_samples),
			('md5sum', metadata.md5sum),
		)

def get_tags (filename) :
	metadata = flac.get_tags(filename)
	if metadata :
		_printt(*map(lambda c : c.to_name_value_pair(), metadata.comments))

def get_cuesheet (filename) :
	metadata = flac.get_cuesheet(filename)
	if metadata :
		_printf('FILE "{0:s}" WAVE', os.path.basename(filename))
		for track in metadata.tracks :
			if track.number < 100 :
				_printf('  TRACK {0:02d} AUDIO', track.number)
				for index in track.indices :
					# Assuming sample rate of 44.1kHz
					m = (track.offset + index.offset) // 588
					f, m = m % 75, m // 75
					s, m = m % 60, m // 60
					_printf('    INDEX {0:02d} {1:02d}:{2:02d}:{3:02d}', index.number, m, s, f)

def get_picture (filename, picture_type) :
	metadata = flac.get_picture(filename, picture_type)
	if metadata :
		filename = os.path.splitext(filename)[0] + '.' + str(metadata.type)
		mime_types = {'image/jpeg': '.jpg', 'image/png': '.png'}
		if metadata.mime_type in mime_types :
			filename += mime_types[metadata.mime_type]
		else :
			filename += mimetypes.guess_extension(metadata.mime_type)
		with open(filename, 'wb') as fp :
			fp.write(metadata.data)
		return (str(metadata.type).replace('_', ' '), os.path.abspath(filename))

def get_pictures (filename) :
	picture_types = [
		'OTHER', 'FILE_ICON_STANDARD', 'FILE_ICON',
		'FRONT_COVER', 'BACK_COVER', 'LEAFLET_PAGE',
		'MEDIA', 'LEAD_ARTIST', 'ARTIST', 'CONDUCTOR',
		'BAND', 'COMPOSER', 'LYRICIST', 'RECORDING_LOCATION',
		'DURING_RECORDING', 'DURING_PERFORMANCE',
		'VIDEO_SCREEN_CAPTURE', 'FISH', 'ILLUSTRATION',
		'BAND_LOGOTYPE', 'PUBLISHER_LOGOTYPE',
	]
	_printt(*filter(bool, map(lambda t : get_picture(filename, getattr(flac.StreamMetadataPictureType, t)), picture_types)))

def main () :
	signal.signal(signal.SIGINT, signal.SIG_DFL)
	cmd = {
		'streaminfo': get_streaminfo,
		'tags': get_tags,
		'cuesheet': get_cuesheet,
		'pictures': get_pictures,
	}
	def _raise () : raise
	def exit () : sys.exit(1)
	try :
		if '--debug' in sys.argv :
			exit = _raise
			sys.argv.remove('--debug')
		if sys.argv[1].lower() in cmd :
			if not os.path.isfile(sys.argv[2]) :
				raise Exception
			cmd[sys.argv[1].lower()](sys.argv[2])
		else :
			for file in sys.argv[1:] :
				if os.path.isfile(file) :
					print ('\n' + os.path.abspath(file))
					print ('\n# StreamInfo')
					get_streaminfo(file)
					print ('\n# Tags')
					get_tags(file)
					print ('\n# Cuesheet')
					get_cuesheet(file)
					print ('\n# Pictures')
					get_pictures(file)
	except :
		_printf('Usage: {0} [{1}] flacfile', sys.argv[0], '|'.join(cmd))
		exit()

if __name__ == '__main__' :
	main()

