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

""" Unit tests for the flac._export module """

import unittest
from flac._export import *

class FlacExportTest (unittest.TestCase) :

	def test_version_is_valid (self) :
		self.assertGreater(API_VERSION_CURRENT, 0)
		self.assertGreaterEqual(API_VERSION_REVISION, 0)
		self.assertGreaterEqual(API_VERSION_AGE, 0)

	def test_version_tuple_equals_single_parts (self) :
		self.assertTupleEqual(
			API_VERSION,
			(API_VERSION_CURRENT, API_VERSION_REVISION, API_VERSION_AGE)
		)

	def test_ogg_support_is_bool (self) :
		self.assertIsInstance(API_SUPPORTS_OGG_FLAC, bool)


if __name__ == "__main__" : unittest.main()

