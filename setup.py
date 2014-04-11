#	This file is part of python-flac.
#
#	Copyright (c) 2014 Christian Schmitz <tynn.dev@gmail.com>
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

from disttest import Extension, setup
import test.unit


def _sources (*files) :
	return ['flac/' + file for file in files]

headers = {
	"_export": [],
	"format": [
		'enum.h',
		'_enum.h',
		'format.h',
	],
}

def _headers () :
	yield 'PyFLAC.h'
	for module in headers :
		for header in headers[module] :
			yield header

def _ext_modules () :
	return [Extension(
			'flac/' + module,
			language = 'c',
			libraries = ['FLAC'],
			sources = _sources(module + '.c'),
			depends = _sources( 'PyFLAC.h', *headers[module] )
		) for module in headers]

setup(
	name = "python-flac",
	version = "0.0.1",
	author = 'Christian Schmitz',
	author_email = 'tynn.dev@gmail.com',
	license = 'LGPLv3+',
	description = 'libFLAC',
	long_description = '',
	url = 'https://github.com/tynn/python-flac',
	platforms = ['Linux'],
	headers = _sources(*set(_headers())),
	packages = ["flac"],
	ext_modules = _ext_modules(),
	test_suite = test.unit
)

