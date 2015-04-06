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

""" Streams for encoding """

from .encoder import *

def encoder_init (init_func, *init_args) :
	init_status = init_func(*init_args)
	if init_status is StreamEncoderInitStatus.OK :
		return True
	if init_status is StreamEncoderInitStatus.ALREADY_INITIALIZED :
		return False
	if init_status is StreamEncoderInitStatus.ENCODER_ERROR :
		raise FlacEncoderError(init_status)
	raise FlacFormatError(init_status)


class StreamEncoderSetup (StreamEncoder) :

	def __init__ (self, **kwargs) :
		self._encoder_setup(**kwargs)

	def _encoder_setup (self, **kwargs) :
		if 'verify' in kwargs :
			self.set_verify(bool(kwargs['verify']))
		if 'ogg_serial_number' in kwargs :
			self.set_ogg_serial_number(kwargs['ogg_serial_number'])
		if 'streamable_subset' in kwargs :
			self.set_streamable_subset(bool(kwargs['streamable_subset']))
		if 'do_mid_side_stereo' in kwargs :
			self.set_do_mid_side_stereo(bool(kwargs['do_mid_side_stereo']))
		if 'loose_mid_side_stereo' in kwargs :
			self.set_loose_mid_side_stereo(bool(kwargs['loose_mid_side_stereo']))
		if 'do_qlp_coeff_prec_search' in kwargs :
			self.set_do_qlp_coeff_prec_search(bool(kwargs['do_qlp_coeff_prec_search']))
		if 'do_exhaustive_model_search' in kwargs :
			self.set_do_exhaustive_model_search(bool(kwargs['do_exhaustive_model_search']))
		if 'channels' in kwargs :
			self.set_channels(kwargs['channels'])
		if 'bits_per_sample' in kwargs :
			self.set_bits_per_sample(kwargs['bits_per_sample'])
		if 'sample_rate' in kwargs :
			self.set_sample_rate(kwargs['sample_rate'])
		if 'compression_level' in kwargs :
			self.set_compression_level(kwargs['compression_level'])
		if 'blocksize' in kwargs :
			self.set_blocksize(kwargs['blocksize'])
		if 'apodization' in kwargs :
			self.set_apodization(kwargs['apodization'])
		if 'max_lpc_order' in kwargs :
			self.set_max_lpc_order(kwargs['max_lpc_order'])
		if 'qlp_coeff_precision' in kwargs :
			self.set_qlp_coeff_precision(kwargs['qlp_coeff_precision'])
		if 'min_residual_partition_order' in kwargs :
			self.set_min_residual_partition_order(kwargs['min_residual_partition_order'])
		if 'max_residual_partition_order' in kwargs :
			self.set_max_residual_partition_order(kwargs['max_residual_partition_order'])
		if 'total_samples_estimate' in kwargs :
			self.set_total_samples_estimate(kwargs['total_samples_estimate'])
		if 'metadata' in kwargs :
			self.set_metadata(kwargs['metadata'])


class EncoderFileStream (object) :

	def __init__ (self, file) :
		self._encoder_file = file

	def __encoder_read__ (self, bytes) :
		return self._encoder_file.read(bytes)

	def __encoder_write__ (self, buffer, samples, current_frame) :
		return self._encoder_file.write(buffer)

	def __encoder_seek__ (self, offset) :
		try : return bool(self._encoder_file.seek(offset)) or True
		except IOError : return False

	def __encoder_tell__ (self) :
		try : return self._encoder_file.tell()
		except IOError : return False


class FileEncoder (StreamEncoderSetup, EncoderFileStream) :

	def __init__ (self, file, ogg=False, **kwargs) :
		self.finish()
		StreamEncoderSetup.__init__(self, **kwargs)
		EncoderFileStream.__init__(self, file)
		encoder_init(self.init_ogg_stream if ogg else self.init_stream)

	def __del__ (self) :
		self.finish()

class StdoutEncoder (FileEncoder) :

	def __init__ (self, ogg=False, **kwargs) :
		from sys import stdout
		FileEncoder.__init__(self, stdout, ogg, **kwargs)

