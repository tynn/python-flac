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

""" Unit tests for the flac.metadata module """

try : from . import _enum
except : import _enum
import unittest
from flac.metadata import *

class _not_a_value (object) :
	def __bool__ (self) : raise TypeError("not a value")
	__nonzero__ = __bool__
_not_a_value = _not_a_value()


class AttrsTest (object) :
	def setUp (self) :
		self.obj = self._class_()

	def test_attrs (self) :
		for attr, value in self._attrs_.items() :
			setattr(self.obj, attr, value)
			self.assertRaises(TypeError, setattr, self.obj, attr, _not_a_value)
			if type(value) == list :
				self.assertListEqual(list(getattr(self.obj, attr)), value)
			else :
				self.assertEqual(getattr(self.obj, attr), value)
		obj = self._class_(**self._attrs_)
		self.assertEqual(self.obj, obj)


class MetadataTest (AttrsTest) :
	def test_instance (self) :
		self.assertIsInstance(self.obj, StreamMetadata)

	def test_type (self) :
		self.assertIs(self.obj.type, self._type_)

	def test_base_new (self) :
		obj = StreamMetadata(self._type_)
		self.assertEqual(self.obj, obj)


class FlacStreamMetadataStreamInfoTest (MetadataTest, unittest.TestCase) :
	_type_ = MetadataType.STREAMINFO
	_class_ = StreamMetadataStreamInfo
	_attrs_ = {
		'min_blocksize': 7,
		'max_blocksize': 99,
		'min_framesize': 23,
		'max_framesize': 42,
		'sample_rate': 101,
		'channels': 2,
		'bits_per_sample': 16,
		'total_samples': 43,
		'md5sum': "0123456789abcdef0123456789abcdef"
	}


class FlacStreamMetadataPaddingTest (MetadataTest, unittest.TestCase) :
	_type_ = MetadataType.PADDING
	_class_ = StreamMetadataPadding
	_attrs_ = { 'length': 23 }


class FlacStreamMetadataApplicationTest (MetadataTest, unittest.TestCase) :
	_type_ = MetadataType.APPLICATION
	_class_ = StreamMetadataApplication
	_attrs_ = { 'id': b"tEst", 'data': b"FooBar" }


class FlacStreamMetadataSeekTableTest (MetadataTest, unittest.TestCase) :
	_type_ = MetadataType.SEEKTABLE
	_class_ = StreamMetadataSeekTable
	_attrs_ = { 'points': [
					StreamMetadataSeekPoint(123, 4),
					StreamMetadataSeekPoint(12, 34),
				] }

	point = StreamMetadataSeekPoint(1)

	def test_resize_points (self) :
		self.assertTrue(self.obj.resize_points(11))
		self.assertEqual(len(list(self.obj.points)), 11)

	def test_set_point (self) :
		points = self._attrs_['points'][:]
		self.obj.points = points
		self.assertRaises(IndexError, self.obj.set_point,
							len(list(self.obj.points)), self.point)
		self.assertTrue(self.obj.set_point(1, self.point))
		points[1] = self.point
		self.assertListEqual(list(self.obj.points), points)

	def test_get_point (self) :
		self.obj.points = self._attrs_['points']
		self.assertRaises(IndexError, self.obj.get_point, len(list(self.obj.points)))
		self.assertEqual(self.obj.get_point(1), self._attrs_['points'][1])

	def test_insert_point (self) :
		self.obj.points = self._attrs_['points']
		self.assertRaises(IndexError, self.obj.insert_point,
							len(list(self.obj.points)) + 3, self.point)
		self.assertTrue(self.obj.insert_point(2, self.point))
		self.assertListEqual(list(self.obj.points), self._attrs_['points'] + [self.point])

	def test_delete_point (self) :
		self.obj.points = self._attrs_['points']
		self.assertRaises(IndexError, self.obj.delete_point, len(list(self.obj.points)))
		self.assertTrue(self.obj.delete_point(1))
		self.assertListEqual(list(self.obj.points), self._attrs_['points'][:1])

	def test_is_legal (self) :
		self.assertTrue(self.obj.is_legal())
		self.obj.points = self._attrs_['points']
		self.assertFalse(self.obj.is_legal())
		self.obj.template_sort()
		self.assertTrue(self.obj.is_legal())

	def test_template_append_placeholders (self) :
		self.assertTrue(self.obj.template_append_placeholders(5))
		self.assertEqual(len(list(self.obj.points)), 5)

	def test_template_append_point (self) :
		self.assertTrue(self.obj.template_append_point(1))
		self.assertListEqual(list(self.obj.points), [self.point])

	def test_template_append_points (self) :
		points = [StreamMetadataSeekPoint(i) for i in range(1, 5)]
		self.assertTrue(self.obj.template_append_points(*range(1, 5)))
		self.assertListEqual(list(self.obj.points), points)
		self.assertTrue(self.obj.template_append_points(list(range(1, 5))))
		self.assertListEqual(list(self.obj.points), 2 * points)

	def test_template_append_spaced_points (self) :
		self.assertTrue(self.obj.template_append_spaced_points(3, 12))
		self.assertListEqual(list(self.obj.points),
					[StreamMetadataSeekPoint(i) for i in range(0, 12, 4)])

	def test_template_append_spaced_points_by_samples (self) :
		self.assertTrue(self.obj.template_append_spaced_points_by_samples(4, 12))
		self.assertListEqual(list(self.obj.points),
					[StreamMetadataSeekPoint(i) for i in range(0, 12, 4)])

	def test_template_sort (self) :
		self.obj.points = self._attrs_['points']
		self.obj.template_sort()
		self.assertListEqual(list(self.obj.points), list(reversed(self._attrs_['points'])))


class FlacStreamMetadataVorbisCommentTest (MetadataTest, unittest.TestCase) :
	_type_ = MetadataType.VORBIS_COMMENT
	_class_ = StreamMetadataVorbisComment
	_attrs_ = {
			'vendor_string': "FooBar",
			'comments': [
					StreamMetadataVorbisCommentEntry("ARTIST", "FooBar"),
					StreamMetadataVorbisCommentEntry("ALBUM", "FooBar"),
				]
			}

	comment = StreamMetadataVorbisCommentEntry("TRACK", "02")

	def test_resize_comments (self) :
		self.obj.comments = self._attrs_['comments']
		self.assertTrue(self.obj.resize_comments(13))
		self.assertEqual(len(list(self.obj.comments)), 13)

	def test_set_comment (self) :
		self.assertRaises(IndexError, self.obj.set_comment,
							len(list(self.obj.comments)), self.comment)
		self.obj.comments = self._attrs_['comments']
		self.assertTrue(self.obj.set_comment(1, self.comment))
		self.assertListEqual(list(self.obj.comments), [
				self._attrs_['comments'][0],
				self.comment
			])

	def test_get_comment (self) :
		self.obj.comments = self._attrs_['comments']
		self.assertRaises(IndexError, self.obj.get_comment, len(list(self.obj.comments)))
		self.assertEqual(self.obj.get_comment(1), self._attrs_['comments'][1])

	def test_insert_comment (self) :
		self.assertRaises(IndexError, self.obj.insert_comment,
							len(list(self.obj.comments)) + 3, self.comment)
		self.obj.comments = self._attrs_['comments']
		self.assertTrue(self.obj.insert_comment(1, self.comment))
		self.assertListEqual(list(self.obj.comments), [
				self._attrs_['comments'][0],
				self.comment,
				self._attrs_['comments'][1]
			])

	def test_append_comment (self) :
		self.obj.comments = self._attrs_['comments']
		self.assertTrue(self.obj.append_comment(self.comment))
		self.assertListEqual(list(self.obj.comments), self._attrs_['comments'] + [self.comment])

	def test_replace_comment (self) :
		comments = self._attrs_['comments'] + self._attrs_['comments']
		self.obj.comments = comments
		comment = StreamMetadataVorbisCommentEntry("ALBUM", "Something")
		self.assertTrue(self.obj.replace_comment(comment, False))
		comments[1] = comment
		self.assertListEqual(list(self.obj.comments), comments)
		self.assertTrue(self.obj.replace_comment(comments[3], True))
		comments[1] = comments[3]
		del comments[3]
		self.assertListEqual(list(self.obj.comments), comments)

	def test_delete_comment (self) :
		self.assertRaises(IndexError, self.obj.delete_comment, len(list(self.obj.comments)))
		self.obj.comments = self._attrs_['comments']
		self.assertTrue(self.obj.delete_comment(1))
		self.assertListEqual(list(self.obj.comments), self._attrs_['comments'][:1])

	def test_find_entry_from (self) :
		self.obj.comments = self._attrs_['comments'] + self._attrs_['comments']
		field_name = self._attrs_['comments'][1].to_name_value_pair()[0]
		index = self.obj.find_entry_from(field_name)
		self.assertEqual(index, 1)
		self.assertEqual(self.obj.find_entry_from(field_name, index + 1), 3)
		self.assertEqual(self.obj.find_entry_from("FOOBAR"), -1)

	def test_remove_entry_matching (self) :
		self.obj.comments = self._attrs_['comments']
		self.assertFalse(self.obj.remove_entry_matching("FOOBAR"))
		field_name = self._attrs_['comments'][1].to_name_value_pair()[0]
		self.assertTrue(self.obj.remove_entry_matching(field_name))
		self.assertListEqual(list(self.obj.comments), [self._attrs_['comments'][0]])

	def test_remove_entries_matching (self) :
		self.obj.comments = self._attrs_['comments'] + self._attrs_['comments']
		self.assertEqual(self.obj.remove_entries_matching("FOOBAR"), 0)
		field_name = self._attrs_['comments'][1].to_name_value_pair()[0]
		self.assertEqual(self.obj.remove_entries_matching(field_name), 2)
		self.assertListEqual(list(self.obj.comments), [self._attrs_['comments'][0]] * 2)


class FlacStreamMetadataCueSheetTest (MetadataTest, unittest.TestCase) :
	_type_ = MetadataType.CUESHEET
	_class_ = StreamMetadataCueSheet
	_attrs_ = {
			'media_catalog_number': "FooBar",
			'lead_in': 2000,
			'is_cd': True,
			'tracks': [
					StreamMetadataCueSheetTrack(123, 21, isrc = "0123456789ab"),
					StreamMetadataCueSheetTrack()
				]
		}

	track = StreamMetadataCueSheetTrack(1, 2)
	index = StreamMetadataCueSheetIndex(1, 2)

	def test_track_resize_indices (self) :
		self.obj.tracks = self.track
		self.assertTrue(self.obj.track_resize_indices(0, 3))
		self.assertEqual(len(list(list(self.obj.tracks)[0].indices)), 3)

	def test_track_set_index (self) :
		self.track.indices = self.index
		self.obj.tracks = self.track
		self.assertRaises(IndexError, self.obj.track_set_index,
							0, len(list(list(self.obj.tracks)[0].indices)), self.index)
		index = StreamMetadataCueSheetIndex(1, 3)
		self.assertTrue(self.obj.track_set_index(0, 0, index))
		self.assertListEqual(list(list(self.obj.tracks)[0].indices), [index])

	def test_track_get_index (self) :
		self.track.indices = self.index, self.index
		self.obj.tracks = self.track
		self.assertRaises(IndexError, self.obj.track_get_index,
							0, len(list(list(self.obj.tracks)[0].indices)))
		self.assertEqual(self.obj.track_get_index(0, 1), self.index)

	def test_track_insert_index (self) :
		self.obj.tracks = self.track
		self.assertRaises(IndexError, self.obj.track_insert_index,
							0, len(list(list(self.obj.tracks)[0].indices)) + 3, self.index)
		self.assertTrue(self.obj.track_insert_index(0, 0, self.index))
		self.assertListEqual(list(list(self.obj.tracks)[0].indices),
				[self.index] + list(self.track.indices))

	def test_track_insert_blank_index (self) :
		self.obj.tracks = self.track
		self.assertRaises(IndexError, self.obj.track_insert_blank_index,
							0, len(list(list(self.obj.tracks)[0].indices)) + 3)
		self.assertTrue(self.obj.track_insert_blank_index(0, 0))
		self.assertListEqual(list(list(self.obj.tracks)[0].indices),
				[StreamMetadataCueSheetIndex()] + list(self.track.indices))

	def test_track_delete_index (self) :
		self.track.indices = [self.index, self.index]
		self.obj.tracks = self.track
		self.assertRaises(IndexError, self.obj.track_delete_index,
							0, len(list(list(self.obj.tracks)[0].indices)))
		self.assertTrue(self.obj.track_delete_index(0, 1))
		self.assertListEqual(list(list(self.obj.tracks)[0].indices), [self.index])

	def test_resize_tracks (self) :
		self.assertTrue(self.obj.resize_tracks(7))
		self.assertEqual(len(list(self.obj.tracks)), 7)

	def test_set_track (self) :
		self.assertRaises(IndexError, self.obj.set_track,
							len(list(self.obj.tracks)), self.track)
		track = StreamMetadataCueSheetTrack(1)
		self.obj.tracks = track, StreamMetadataCueSheetTrack(2)
		self.assertTrue(self.obj.set_track(1, self.track))
		self.assertListEqual(list(self.obj.tracks), [track, self.track])

	def test_get_track (self) :
		self.obj.tracks = self._attrs_['tracks']
		self.assertRaises(IndexError, self.obj.get_track, len(list(self.obj.tracks)))
		self.assertEqual(self.obj.get_track(1), self._attrs_['tracks'][1])

	def test_insert_track (self) :
		self.assertRaises(IndexError, self.obj.insert_track,
							len(list(self.obj.tracks)) + 3, self.track)
		self.assertTrue(self.obj.insert_track(0, self.track))
		self.assertListEqual(list(self.obj.tracks), [self.track])

	def test_insert_blank_track (self) :
		self.assertRaises(IndexError, self.obj.insert_blank_track,
							len(list(self.obj.tracks)) + 3)
		self.assertTrue(self.obj.insert_blank_track(0))
		self.assertListEqual(list(self.obj.tracks), [StreamMetadataCueSheetTrack()])

	def test_delete_track (self) :
		self.assertRaises(IndexError, self.obj.delete_track,
							len(list(self.obj.tracks)) + 3)
		track = StreamMetadataCueSheetTrack(1)
		self.obj.tracks = track, self.track
		self.assertTrue(self.obj.delete_track(1))
		self.assertListEqual(list(self.obj.tracks), [track])

	def test_is_legal (self) :
		self.assertFalse(self.obj.is_legal(False))
		self.assertFalse(self.obj.is_legal(True))
		self.assertRaises(FlacFormatError, self.obj.is_legal, False, True)
		self.assertRaises(FlacFormatError, self.obj.is_legal, True, True)

	def test_calculate_cddb_id (self) :
		tracks = []
		attrs = { 'offset': 0, 'number': 1, 'type': 1, 'pre_emphasis': False }
		attrs['indices'] = [StreamMetadataCueSheetIndex(number = 1)]
		tracks.append(StreamMetadataCueSheetTrack(**attrs))
		attrs['offset'] = 10534020
		attrs['number'] += 1
		tracks.append(StreamMetadataCueSheetTrack(**attrs))
		attrs['offset'] = 20010816
		attrs['number'] += 1
		tracks.append(StreamMetadataCueSheetTrack(**attrs))
		tracks.append(StreamMetadataCueSheetTrack(offset = 30491916, number = 255))
		self.obj.lead_in = 88200
		self.obj.is_cd = True
		self.obj.tracks = tracks
		self.assertEqual(self.obj.calculate_cddb_id(), 0x1602b303)


class FlacStreamMetadataPictureTest (MetadataTest, unittest.TestCase) :
	_type_ = MetadataType.PICTURE
	_class_ = StreamMetadataPicture
	_attrs_ = {
			'picture_type': StreamMetadataPictureType.FRONT_COVER,
			'mime_type': "image/png",
			'description': "Frontcover",
			'width': 200,
			'height': 200,
			'depth': 1,
			'colors': 2,
			'data': b"FooBar"
		}

	def test_is_legal (self) :
		self.assertFalse(self.obj.is_legal())
		self.assertRaises(FlacFormatError, self.obj.is_legal, True)


class RichcompareTest (object) :
	def _test_richcompare (self, o1, o2, results) :
		self.assertEqual(o1 < o2, results[0])
		self.assertEqual(o1 > o2, results[1])
		self.assertEqual(o1 <= o2, results[2])
		self.assertEqual(o1 >= o2, results[3])
		self.assertEqual(o1 == o2, results[4])
		self.assertEqual(o1 != o2, results[5])

	def _test_le_ge_eq (self, o1, o2) :
		self._test_richcompare(o1, o2, [0, 0, 1, 1, 1, 0])

	def _test_lt_gt_ne (self, o1, o2) :
		self._test_richcompare(o1, o2, [1, 0, 1, 0, 0, 1])
		self._test_richcompare(o2, o1, [0, 1, 0, 1, 0, 1])


class FlacStreamMetadataSeekPointTest (RichcompareTest, unittest.TestCase) :
	def test_le_ge_eq (self) :
		self._test_le_ge_eq(StreamMetadataSeekPoint(1, 2, 3),
							StreamMetadataSeekPoint(1, 2, 3))

	def test_lt_gt_ne (self) :
		o1 = StreamMetadataSeekPoint(1, 2, 3)
		o2 = StreamMetadataSeekPoint(1, 2, 4)
		self._test_lt_gt_ne(o1, o2)
		o2 = StreamMetadataSeekPoint(1, 3, 3)
		self._test_lt_gt_ne(o1, o2)
		o2 = StreamMetadataSeekPoint(2, 2, 3)
		self._test_lt_gt_ne(o1, o2)


class FlacStreamMetadataVorbisCommentEntryTest (unittest.TestCase) :
	def test_is_legal (self) :
		self.assertFalse(StreamMetadataVorbisCommentEntry("FooBar").is_legal())
		self.assertTrue(StreamMetadataVorbisCommentEntry("ALBUM", "FooBar").is_legal())

	def test_to_name_value_pair (self) :
		name_value_pair = "ALBUM", "FooBar"
		entry = StreamMetadataVorbisCommentEntry(*name_value_pair)
		self.assertEqual(entry.to_name_value_pair(), name_value_pair)

	def test_matches (self) :
		field_name = "ALBUM"
		entry = StreamMetadataVorbisCommentEntry(field_name, "FooBar")
		self.assertTrue(entry.matches(field_name))
		self.assertFalse(entry.matches("FooBar"))


class FlacStreamMetadataCueSheetIndexTest (RichcompareTest, unittest.TestCase) :
	def test_le_ge_eq (self) :
		self._test_le_ge_eq(StreamMetadataCueSheetIndex(1, 2),
							StreamMetadataCueSheetIndex(1, 2))

	def test_lt_gt_ne (self) :
		o1 = StreamMetadataCueSheetIndex(1, 2)
		o2 = StreamMetadataCueSheetIndex(1, 3)
		self._test_lt_gt_ne(o1, o2)
		o2 = StreamMetadataCueSheetIndex(2, 2)
		self._test_lt_gt_ne(o1, o2)


class FlacStreamMetadataCueSheetTrackTest (AttrsTest, unittest.TestCase) :
	_class_ = StreamMetadataCueSheetTrack
	_attrs_ = {
			'offset': 6,
			'number': 4,
			'isrc': '1234567890ab',
			'type': 1,
			'pre_emphasis': True,
			'indices': [
					StreamMetadataCueSheetIndex(1, 2),
					StreamMetadataCueSheetIndex(1, 3)
				]
		}

	def setUp (self) :
		AttrsTest.setUp(self)
		self.obj.indices = self._attrs_['indices']

	def test_irsc (self) :
		self.obj.isrc = self._attrs_['isrc']
		self.assertEqual(self.obj.isrc, self._attrs_['isrc'])
		self.obj.isrc = ""
		self.assertEqual(self.obj.isrc, "")
		self.assertRaises(ValueError, setattr, self.obj, 'isrc', 'a')

	def test_resize_indices (self) :
		self.assertTrue(self.obj.resize_indices(9))
		self.assertEqual(len(list(self.obj.indices)), 9)

	def test_set_index (self) :
		index = StreamMetadataCueSheetIndex(2, 2)
		self.assertRaises(IndexError, self.obj.set_index,
							len(list(self.obj.indices)), index)
		self.assertTrue(self.obj.set_index(1, index))
		self.assertListEqual(list(self.obj.indices), [self._attrs_['indices'][0], index])

	def test_get_index (self) :
		self.obj.indices = self._attrs_['indices']
		self.assertRaises(IndexError, self.obj.get_index, len(list(self.obj.indices)))
		self.assertEqual(self.obj.get_index(1), self._attrs_['indices'][1])

	def test_insert_index (self) :
		index = StreamMetadataCueSheetIndex(2, 2)
		self.assertRaises(IndexError, self.obj.insert_index,
							len(list(self.obj.indices)) + 3, index)
		self.assertTrue(self.obj.insert_index(1, index))
		indices = self._attrs_['indices'][:]
		indices.insert(1, index)
		self.assertListEqual(list(self.obj.indices), indices)

	def test_insert_blank_index (self) :
		self.assertRaises(IndexError, self.obj.insert_blank_index,
							len(list(self.obj.indices)) + 3)
		self.assertTrue(self.obj.insert_blank_index(1))
		indices = self._attrs_['indices'][:]
		indices.insert(1, StreamMetadataCueSheetIndex())
		self.assertListEqual(list(self.obj.indices), indices)

	def test_delete_index (self) :
		self.assertRaises(IndexError, self.obj.delete_index,
							len(list(self.obj.indices)))
		self.assertTrue(self.obj.delete_index(1))
		self.assertListEqual(list(self.obj.indices), [self._attrs_['indices'][0]])


def load_tests(loader, tests, pattern):
	return _enum._load_tests(loader, tests, StreamMetadataPictureType)


if __name__ == "__main__" : unittest.main()

