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

class FlacExportTest (unittest.TestCase) :

	def _test_attrs (self, module) :
		attrs = filter(lambda name : not name.startswith('_'), dir(module))
		for attr in attrs :
			self.assertIs(getattr(flac.flac, attr), getattr(module, attr))

	def test__export (self) :
		import flac._export
		self._test_attrs(flac._export)

	def test_format (self) :
		import flac.format
		self._test_attrs(flac.format)

if __name__ == "__main__" : unittest.main()

