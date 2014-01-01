#	This file is part of python-flac.
#
#	Copyright (c) 2013 Christian Schmitz <tynn.dev@gmail.com>
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

from distutils.core import Extension, setup


setup(
	name = "python-flac",
	version = "0.0.1",
	author = 'Christian Schmitz',
	author_email = 'tynn.dev@gmail.com',
	license = 'LGPLv3+',
	description = '',
	long_description = '',
	url = 'https://github.com/tynn/python-flac',
	platforms = ['any'],
	ext_modules = [Extension(
		'flac',
		sources = [
			'flac/flac.c',
		],
		libraries = ['FLAC']
	)]
)

