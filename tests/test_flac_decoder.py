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

""" Unit tests for the flac.decoder module """

try : from . import _enum, _files
except : import _enum, _files
import unittest
from flac.stream_decoder import *

class FlacStreamDecoderTest (unittest.TestCase) :

	def __decoder_read__ (self, bytes) :
		pass

	def __decoder_eof__ (self) :
		return True

	def setUp (self) :
		self.decoder = StreamDecoder()

	def tearDown (self) :
		del self.decoder

	def test_get_state (self) :
		self.assertIs(StreamDecoderState.UNINITIALIZED, self.decoder.get_state())

	def test_md5_checking (self) :
		self.assertTrue(self.decoder.set_md5_checking(True))
		self.assertTrue(self.decoder.get_md5_checking())
		self.assertTrue(self.decoder.set_md5_checking(False))
		self.assertFalse(self.decoder.get_md5_checking())

	def test_get_decode_position (self) :
		self.assertIsNone(self.decoder.get_decode_position())

	def test_metadata_respond (self) :
		self.assertTrue(self.decoder.set_metadata_respond_all())
		self.assertTrue(self.decoder.set_metadata_respond(MetadataType.PADDING))
		self.assertTrue(self.decoder.set_metadata_respond_application("FLAc"))
		with self.assertRaises(TypeError):
			self.decoder.set_metadata_respond_application("FLA")
		with self.assertRaises(TypeError):
			self.decoder.set_metadata_respond_application("FLAcC")

	def test_metadata_ignore (self) :
		self.assertTrue(self.decoder.set_metadata_ignore_all())
		self.assertTrue(self.decoder.set_metadata_ignore(MetadataType.PADDING))
		self.assertTrue(self.decoder.set_metadata_ignore_application("FLAc"))
		with self.assertRaises(TypeError):
			self.decoder.set_metadata_ignore_application("FLA")
		with self.assertRaises(TypeError):
			self.decoder.set_metadata_ignore_application("FLAcC")

	def test_init_file (self) :
		self.assertIs(StreamDecoderInitStatus.OK, self.decoder.init_file(__file__))
		self.assertRaises(FlacFormatError, self.decoder.process_single)

	def test_init_ogg_file (self) :
		self.assertIs(StreamDecoderInitStatus.OK, self.decoder.init_ogg_file(__file__))
		self.assertFalse(self.decoder.process_single())

	def test_init_stream (self) :
		self.assertIs(StreamDecoderInitStatus.OK, self.decoder.init_stream(self))
		self.assertFalse(self.decoder.process_single())

	def test_init_ogg_file (self) :
		self.assertIs(StreamDecoderInitStatus.OK, self.decoder.init_ogg_stream(self))
		self.assertFalse(self.decoder.process_single())

	def test_set_ogg_serial_number (self) :
		self.assertTrue(self.decoder.set_ogg_serial_number(11))


class FlacStreamDecoderInitTest (unittest.TestCase) :

	def setUp (self) :
		self.decoder = StreamDecoder()
		self.decoder.set_md5_checking(False)
		self.decoder.init_file(_files.w10_1)

	def tearDown (self) :
		del self.decoder

	def test_non_init (self) :
		self.assertFalse(self.decoder.set_md5_checking(True))
		self.assertFalse(self.decoder.set_metadata_respond_all())
		self.assertFalse(self.decoder.set_metadata_respond(MetadataType.PADDING))
		self.assertFalse(self.decoder.set_metadata_respond_application("FLAc"))
		self.assertFalse(self.decoder.set_metadata_ignore_all())
		self.assertFalse(self.decoder.set_metadata_ignore(MetadataType.PADDING))
		self.assertFalse(self.decoder.set_metadata_ignore_application("FLAc"))

	def test_finish (self) :
		self.assertTrue(self.decoder.finish())

	def test_flush (self) :
		self.assertTrue(self.decoder.flush())

	def test_set_ogg_serial_number (self) :
		self.assertFalse(self.decoder.set_ogg_serial_number(11))


class FlacStreamDecoderDecodeTest (StreamDecoder, unittest.TestCase) :

	def __decoder_metadata__ (self, metadata) :
		self.metadata.append(metadata)

	def __decoder_write__ (self, frame, buffer) :
		pass

	def setUp (self) :
		self.metadata = []

	def tearDown (self) :
		self.finish()
		del self.metadata

	def _process_metadata (self, file) :
		self.init_file(file)
		return self.process_until_end_of_metadata()

	def test_stream_info (self) :
		self._process_metadata(_files.w10_1)
		self.process_single()
		stream_info = self.metadata[0]
		self.assertEqual(self.get_bits_per_sample(), stream_info.bits_per_sample)
		self.assertEqual(self.get_channels(), stream_info.channels)
		self.assertEqual(self.get_sample_rate(), stream_info.sample_rate)
		self.assertEqual(self.get_total_samples(), stream_info.total_samples)
		self.assertTrue(stream_info.min_blocksize <= self.get_blocksize() <= stream_info.max_blocksize)

	def test_get_resolved_state_string (self) :
		string = self.get_resolved_state_string()
		self.assertIsInstance(string, str)
		self.assertEqual(string, "FLAC__STREAM_DECODER_" + str(self.get_state()))

	def test_get_state (self) :
		self.assertIsInstance(self.get_state(), StreamDecoderState)

	def test_get_channel_assignment (self) :
		self._process_metadata(_files.w10_1)
		self.process_single()
		self.assertIsInstance(self.get_channel_assignment(), ChannelAssignment)

	def test_set_metadata_respond_all (self) :
		self.set_metadata_respond_all()
		self._process_metadata(_files.w10_1)
		self.assertListEqual(self.metadata, _files.w10_1.metadata)

	def test_set_metadata_respond_all (self) :
		self.set_metadata_respond(MetadataType.PADDING)
		self._process_metadata(_files.w10_1)
		self.assertListEqual(self.metadata, _files.w10_1.metadata[::2])

	def test_set_metadata_respond_application (self) :
#		self.assertFalse(self.decoder.set_metadata_respond_application("FLAc"))
		pass

	def test_set_metadata_ignore_all (self) :
		self.set_metadata_ignore_all()
		self._process_metadata(_files.w10_1)
		self.assertListEqual(self.metadata, [])

	def test_set_metadata_ignore (self) :
		self.set_metadata_ignore(MetadataType.STREAMINFO)
		self._process_metadata(_files.w10_1)
		self.assertListEqual(self.metadata, [])

	def test_set_metadata_ignore_application (self) :
#		self.assertFalse(self.decoder.set_metadata_ignore_application("FLAc"))
		pass

	def test_process_single (self) :
		self.set_metadata_respond_all()
		self.init_file(_files.w10_1)
		self.assertTrue(self.process_single())
		self.assertEqual(self.metadata, _files.w10_1.metadata[:1])

	def test_process_until_end_of_metadata (self) :
		self.assertTrue(self._process_metadata(_files.w10_1))
		self.assertEqual(self.get_state(), StreamDecoderState.SEARCH_FOR_FRAME_SYNC)

	def test_process_until_end_of_stream (self) :
		self.init_file(_files.w10_1)
		self.assertTrue(self.process_until_end_of_stream())
		self.assertEqual(self.get_state(), StreamDecoderState.END_OF_STREAM)

	def test_skip_single_frame (self) :
		self.init_file(_files.w10_1)
		self.assertFalse(self.skip_single_frame())

	def test_reset (self) :
		self.set_metadata_respond_all()
		self._process_metadata(_files.w10_1)
		self.assertTrue(self.reset())
		self.process_until_end_of_stream()
		self.assertEqual(self.get_state(), StreamDecoderState.END_OF_STREAM)
		self.assertListEqual(self.metadata, _files.w10_1.metadata * 2)


class FlacFileDecoderTest (unittest.TestCase) :

	def __decoder_write__ (self, frame, buffer) :
		self.frame = frame

	def setUp (self) :
		self.frame = None
		self.file = open(_files.w10_1, 'rb')
		self.decoder = FileDecoder(self.file)
		self.decoder.__decoder_write__ = self.__decoder_write__

	def tearDown (self) :
		self.file.close()
		del self.frame, self.file, self.decoder

	def test_get_decode_position (self) :
		self.decoder.process_until_end_of_metadata()
		tell = self.file.tell()
		self.decoder.process_single()
		self.assertGreaterEqual(self.decoder.get_decode_position(), tell)
		self.assertLessEqual(self.decoder.get_decode_position(), self.file.tell())

	def test_seek_absolute (self) :
		self.assertTrue(self.decoder.seek_absolute(11))
		self.assertEqual(self.frame.number, 11)


class FlacStdinDecoderTest (unittest.TestCase) :

	def setUp (self) :
		self.decoder = StdinDecoder()
		self.decoder.__decoder_write__ = None

	def tearDown (self) :
		del self.decoder

	def test_seek_absolute (self) :
		self.assertFalse(self.decoder.seek_absolute(0))


class FlacPcmDecoderTest (unittest.TestCase) :

	def test_data (self) :
		data = []
		def write_pcm (pcm) :
			data.append(pcm)
		decoder = PcmDecoder()
		decoder.__decoder_write_pcm__ = write_pcm
		decoder.init_file(_files.w10_1)
		decoder.process_until_end_of_stream()
		self.assertEqual(b''.join(data), _files.w10_1.data)


def load_tests(loader, tests, pattern):
	enums = [
		StreamDecoderState,
		StreamDecoderInitStatus,
		StreamDecoderReadStatus,
		StreamDecoderSeekStatus,
		StreamDecoderTellStatus,
		StreamDecoderLengthStatus,
		StreamDecoderWriteStatus,
		StreamDecoderErrorStatus,
	]
	return _enum._load_tests(loader, tests, *enums)


if __name__ == "__main__" : unittest.main()

