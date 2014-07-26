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

""" Abstract unit test case for the flac enums """

import unittest

class EnumTest (unittest.TestCase) :

	def __init__ (self, *args, **kwargs) :
		def _enum () :
			for key in dir(self._type) :
				value = getattr(self._type, key)
				if isinstance (value, self._type) :
					yield value
		self._enum = list(_enum())
		unittest.TestCase.__init__(self, *args, **kwargs)

	def _pairs (self) :
		for e1 in self._enum :
			for e2 in self._enum :
				if e1 != e2 :
					yield e1, e2

	def test_ininstanceability (self) :
		self.assertRaises(TypeError, self._type)

	def test_equality (self) :
		for e1 in self._enum :
			self.assertEqual(e1, e1)

	def test_inequality (self) :
		for e1, e2 in self._pairs() :
			self.assertNotEqual(e1, e2)

	def test_boolvalue (self) :
		for e1 in self._enum :
			self.assertTrue(e1)
			self.assertFalse(not e1)


def _load_tests (loader, tests, *types) :
	for type in types :
		class DefaultEnumTest (EnumTest) : _type = type
		tests.addTest(loader.loadTestsFromTestCase(DefaultEnumTest))
	return tests

