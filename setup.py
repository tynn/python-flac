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

from disttest import Extension, setup
import tests


def _sources (*files) : return ['flac/' + file for file in files]

headers = {
	"_export": [],
	"format": [
			'enum.h',
			'format.h',
			'_enum.h',
		],
	"metadata": [
			'enum.h',
			'format.h',
			'metadata.h',
			'_application_id.h',
			'_bool.h',
			'_enum.h',
			'_list_of_type.h',
			'_uintX.h',
			'_uint8.h',
			'_uint32.h',
			'_uint64.h',
			'_unsigned.h',
		],
	"metadata0": [
			'enum.h',
			'format.h',
			'metadata.h',
			'_uintX.h',
			'_unsigned.h',
		],
	"metadata1": [
			'enum.h',
			'format.h',
			'metadata.h',
			'_bool.h',
		],
	"metadata2": [
			'enum.h',
			'metadata.h',
		],
}

def _headers () :
	yield 'PyFLAC.h'
	yield '_C_API.h'
	for module in headers :
		for header in headers[module] :
			yield header

setup(
	name = "python-flac",
	version = "0.0a",
	author = 'Christian Schmitz',
	author_email = 'tynn.dev@gmail.com',
	license = 'LGPLv3+',
	description = 'libFLAC',
	long_description = '',
	url = 'https://github.com/tynn/python-flac',
	platforms = ['Linux'],
	headers = _sources(*set(_headers())),
	packages = ["flac"],
	ext_modules = [Extension(
			'flac/' + module,
			language = 'c',
			libraries = ['FLAC'],
			sources = _sources(module + '.c'),
			depends = _sources('PyFLAC.h', '_C_API.h', *headers[module])
		) for module in headers],
	test_suite = tests
)

