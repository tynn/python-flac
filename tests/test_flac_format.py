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

""" Unit tests for the flac.format module """

try : from . import _enum
except : import _enum
import unittest
from flac.format import *

class FormatErrorTest (unittest.TestCase) :

	def _raise_format_error (self) :
		raise FlacFormatError

	def test_ininstanceability (self) :
		self.assertRaises(FlacFormatError, self._raise_format_error)


def load_tests(loader, tests, pattern):
	enums = [
		ChannelAssignment,
		EntropyCodingMethodType,
		FrameNumberType,
		SubframeType,
		MetadataType,
	]
	return _enum._load_tests(loader, tests, *enums)


if __name__ == "__main__" : unittest.main()

