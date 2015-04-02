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

""" Unit tests for the flac.flac module """

import unittest
import flac.flac

class FlacFlacTest (unittest.TestCase) :

	def _test_attrs (self, module) :
		attrs = filter(lambda name : not name.startswith('_'), dir(module))
		for attr in attrs :
			self.assertIs(getattr(flac.flac, attr), getattr(module, attr))

	def test_decoder (self) :
		import flac.decoder
		self._test_attrs(flac.decoder)

	def test_format (self) :
		import flac.format
		self._test_attrs(flac.format)

	def test_metadata (self) :
		import flac.metadata
		self._test_attrs(flac.metadata)

	def test_metadata0 (self) :
		import flac.metadata0
		self._test_attrs(flac.metadata0)

	def test_metadata1 (self) :
		import flac.metadata1
		self._test_attrs(flac.metadata1)

	def test_metadata2 (self) :
		import flac.metadata2
		self._test_attrs(flac.metadata2)

	def test_stream_decoder (self) :
		import flac.stream_decoder
		self._test_attrs(flac.stream_decoder)


if __name__ == "__main__" : unittest.main()

