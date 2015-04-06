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

""" Unit tests for the flac.encoder module """

try : from . import _enum, _files
except : import _enum, _files
import itertools, unittest
from flac.stream_decoder import PcmDecoder, StreamDecoder, StreamDecoderState
from flac.stream_encoder import *


class FlacStreamEncoderTest (unittest.TestCase) :

	def setUp (self) :
		self.encoder = StreamEncoder()

	def tearDown (self) :
		del self.encoder

	def test_get_state (self) :
		self.assertIs(StreamEncoderState.UNINITIALIZED, self.encoder.get_state())

	def test_set_ogg_serial_number (self) :
		self.assertTrue(self.encoder.set_ogg_serial_number(7))

	def test_verify (self) :
		self.assertTrue(self.encoder.set_verify(True))
		self.assertTrue(self.encoder.get_verify())
		self.assertTrue(self.encoder.set_verify(False))
		self.assertFalse(self.encoder.get_verify())

	def test_streamable_subset (self) :
		self.assertTrue(self.encoder.set_streamable_subset(True))
		self.assertTrue(self.encoder.get_streamable_subset())
		self.assertTrue(self.encoder.set_streamable_subset(False))
		self.assertFalse(self.encoder.get_streamable_subset())

	def test_channels (self) :
		self.assertTrue(self.encoder.set_channels(1))
		self.assertEqual(self.encoder.get_channels(), 1)
		self.assertRaises(ValueError, self.encoder.set_channels, -1)

	def test_bits_per_sample (self) :
		self.assertTrue(self.encoder.set_bits_per_sample(32))
		self.assertEqual(self.encoder.get_bits_per_sample(), 32)
		self.assertRaises(ValueError, self.encoder.set_bits_per_sample, -1)

	def test_sample_rate (self) :
		self.assertTrue(self.encoder.set_sample_rate(22000))
		self.assertEqual(self.encoder.get_sample_rate(), 22000)
		self.assertRaises(ValueError, self.encoder.set_sample_rate, -1)

	def test_set_compression_level (self) :
		self.assertTrue(self.encoder.set_compression_level(6))
		self.assertRaises(ValueError, self.encoder.set_compression_level, -1)

	def test_blocksize (self) :
		self.assertTrue(self.encoder.set_blocksize(1024))
		self.assertEqual(self.encoder.get_blocksize(), 1024)
		self.assertRaises(ValueError, self.encoder.set_blocksize, -1)

	def test_do_mid_side_stereo (self) :
		self.assertTrue(self.encoder.set_do_mid_side_stereo(True))
		self.assertTrue(self.encoder.get_do_mid_side_stereo())
		self.assertTrue(self.encoder.set_do_mid_side_stereo(False))
		self.assertFalse(self.encoder.get_do_mid_side_stereo())

	def test_loose_mid_side_stereo (self) :
		self.assertTrue(self.encoder.set_loose_mid_side_stereo(True))
		self.assertTrue(self.encoder.get_loose_mid_side_stereo())
		self.assertTrue(self.encoder.set_loose_mid_side_stereo(False))
		self.assertFalse(self.encoder.get_loose_mid_side_stereo())

	def test_set_apodization (self) :
		self.assertTrue(self.encoder.set_apodization("hann;triangle;tukey(0.5)"))

	def test_max_lpc_order (self) :
		self.assertTrue(self.encoder.set_max_lpc_order(1))
		self.assertEqual(self.encoder.get_max_lpc_order(), 1)
		self.assertRaises(ValueError, self.encoder.set_max_lpc_order, -1)

	def test_qlp_coeff_precision (self) :
		self.assertTrue(self.encoder.set_qlp_coeff_precision(9))
		self.assertEqual(self.encoder.get_qlp_coeff_precision(), 9)
		self.assertRaises(ValueError, self.encoder.set_qlp_coeff_precision, -1)

	def test_do_qlp_coeff_prec_search (self) :
		self.assertTrue(self.encoder.set_do_qlp_coeff_prec_search(True))
		self.assertTrue(self.encoder.get_do_qlp_coeff_prec_search())
		self.assertTrue(self.encoder.set_do_qlp_coeff_prec_search(False))
		self.assertFalse(self.encoder.get_do_qlp_coeff_prec_search())

	def test_do_exhaustive_model_search (self) :
		self.assertTrue(self.encoder.set_do_exhaustive_model_search(True))
		self.assertTrue(self.encoder.get_do_exhaustive_model_search())
		self.assertTrue(self.encoder.set_do_exhaustive_model_search(False))
		self.assertFalse(self.encoder.get_do_exhaustive_model_search())

	def test_min_residual_partition_order (self) :
		self.assertTrue(self.encoder.set_min_residual_partition_order(11))
		self.assertEqual(self.encoder.get_min_residual_partition_order(), 11)
		self.assertRaises(ValueError, self.encoder.set_min_residual_partition_order, -1)

	def test_max_residual_partition_order (self) :
		self.assertTrue(self.encoder.set_max_residual_partition_order(3))
		self.assertEqual(self.encoder.get_max_residual_partition_order(), 3)
		self.assertRaises(ValueError, self.encoder.set_max_residual_partition_order, -1)

	def test_total_samples_estimate (self) :
		self.assertTrue(self.encoder.set_total_samples_estimate(9))
		self.assertEqual(self.encoder.get_total_samples_estimate(), 9)
		self.assertRaises(ValueError, self.encoder.set_total_samples_estimate, -1)

	def test_set_metadata (self) :
		self.assertTrue(self.encoder.set_metadata())
		self.assertTrue(self.encoder.set_metadata(_files.w10_1.metadata))

	def test_init_file (self) :
		with _files.mktmp() as file :
			self.assertIs(StreamEncoderInitStatus.OK, self.encoder.init_file(file.name))

	def test_init_ogg_file (self) :
		with _files.mktmp() as file :
			self.assertIs(StreamEncoderInitStatus.OK, self.encoder.init_ogg_file(file.name))

	def test_init_stream (self) :
		self.assertIs(StreamEncoderInitStatus.ENCODER_ERROR, self.encoder.init_stream(self))

	def test_init_ogg_stream (self) :
		self.assertIs(StreamEncoderInitStatus.ENCODER_ERROR, self.encoder.init_ogg_stream(self))


class FlacStreamEncoderInitTest (unittest.TestCase) :

	def setUp (self) :
		self.file = _files.mktmp(mode='r')
		self.encoder = StreamEncoder()
		self.encoder.init_file(self.file.name)

	def tearDown (self) :
		self.file.close()
		del self.file, self.encoder

	def test_set_ogg_serial_number (self) :
		self.assertFalse(self.encoder.set_ogg_serial_number(7))

	def test_set_verify (self) :
		self.assertFalse(self.encoder.set_verify(True))

	def test_set_streamable_subset (self) :
		self.assertFalse(self.encoder.set_streamable_subset(True))

	def test_set_channels (self) :
		self.assertFalse(self.encoder.set_channels(1))

	def test_set_bits_per_sample (self) :
		self.assertFalse(self.encoder.set_bits_per_sample(32))

	def test_set_sample_rate (self) :
		self.assertFalse(self.encoder.set_sample_rate(22000))

	def test_set_compression_level (self) :
		self.assertFalse(self.encoder.set_compression_level(6))

	def test_set_blocksize (self) :
		self.assertFalse(self.encoder.set_blocksize(1024))

	def test_set_do_mid_side_stereo (self) :
		self.assertFalse(self.encoder.set_do_mid_side_stereo(False))

	def test_set_loose_mid_side_stereo (self) :
		self.assertFalse(self.encoder.set_loose_mid_side_stereo(True))

	def test_set_apodization (self) :
		self.assertFalse(self.encoder.set_apodization("hann;triangle;tukey(0.5)"))

	def test_set_max_lpc_order (self) :
		self.assertFalse(self.encoder.set_max_lpc_order(1))

	def test_set_qlp_coeff_precision (self) :
		self.assertFalse(self.encoder.set_qlp_coeff_precision(9))

	def test_set_do_qlp_coeff_prec_search (self) :
		self.assertFalse(self.encoder.set_do_qlp_coeff_prec_search(True))

	def test_set_do_exhaustive_model_search (self) :
		self.assertFalse(self.encoder.set_do_exhaustive_model_search(True))

	def test_set_min_residual_partition_order (self) :
		self.assertFalse(self.encoder.set_min_residual_partition_order(11))

	def test_set_max_residual_partition_order (self) :
		self.assertFalse(self.encoder.set_max_residual_partition_order(3))

	def test_set_total_samples_estimate (self) :
		self.assertFalse(self.encoder.set_total_samples_estimate(9))

	def test_set_metadata (self) :
		self.assertFalse(self.encoder.set_metadata())

	def test_finish (self) :
		self.assertTrue(self.encoder.finish())

	def test_get_resolved_state_string (self) :
		string = self.encoder.get_resolved_state_string()
		self.assertEqual(string, "FLAC__STREAM_ENCODER_" + str(self.encoder.get_state()))

	def test_get_verify_decoder_state (self) :
		self.assertIsInstance(self.encoder.get_verify_decoder_state(), StreamDecoderState)

	def test_get_verify_decoder_error_stats (self) :
		stats = self.encoder.get_verify_decoder_error_stats()
		self.assertTrue(hasattr(stats, 'absolute_sample'))
		self.assertTrue(hasattr(stats, 'channel'))
		self.assertTrue(hasattr(stats, 'expected'))
		self.assertTrue(hasattr(stats, 'frame_number'))
		self.assertTrue(hasattr(stats, 'got'))
		self.assertTrue(hasattr(stats, 'sample'))


class FlacFileEncoderTest (StreamDecoder, unittest.TestCase) :
	try : _zip = itertools.izip
	except : _zip = zip

	def __decoder_write__ (self, frame, buffer) :
		self.assertTrue(self.process_buffer.process(buffer))
		self.assertTrue(self.process_int.process(buf.data for buf in buffer))
		data = self._zip(*(buf.data for buf in buffer))
		data = itertools.chain.from_iterable(data)
		self.assertTrue(self.process_interleaved.process_interleaved(data))

	def assertDataEqual (self, filename, data, msg=None) :
		data = []
		def write_pcm (pcm) :
			data.append(pcm)
		decoder = PcmDecoder(big_endian=True)
		decoder.__decoder_write_pcm__ = write_pcm
		decoder.init_file(filename)
		decoder.process_until_end_of_stream()
		self.assertEqual(b''.join(data), _files.w10_1.data, msg)

	def setUp (self) :
		self.init_file(_files.w10_1)

	def tearDown (self) :
		self.finish()

	def test_encoding (self) :
		with _files.mktmp(mode='w+b') as e1, \
				_files.mktmp(mode='w+b') as e2, \
					_files.mktmp(mode='w+b') as e3 :
			self.process_buffer = FileEncoder(e1, verify=True)
			self.process_int = FileEncoder(e2, verify=True)
			self.process_interleaved = FileEncoder(e3, verify=True)
			self.process_until_end_of_stream()
			self.assertTrue(self.process_buffer.finish())
			del self.process_buffer
			data = _files.w10_1.data
			self.assertDataEqual(e1.name, data, "process with buffer data failed")
			self.assertTrue(self.process_int.finish())
			del self.process_int
			self.assertDataEqual(e2.name, data, "process with int data failed")
			self.assertTrue(self.process_interleaved.finish())
			del self.process_interleaved
			self.assertDataEqual(e3.name, data, "process_interleaved with int data failed")


class EncoderErrorTest (unittest.TestCase) :

	def _raise_encoder_error (self) :
		raise FlacEncoderError

	def test_ininstanceability (self) :
		self.assertRaises(FlacEncoderError, self._raise_encoder_error)

def load_tests(loader, tests, pattern):
	enums = [
		StreamEncoderState,
		StreamEncoderInitStatus,
		StreamEncoderReadStatus,
		StreamEncoderSeekStatus,
		StreamEncoderTellStatus,
		StreamEncoderWriteStatus,
	]
	return _enum._load_tests(loader, tests, *enums)


if __name__ == "__main__" : unittest.main()

