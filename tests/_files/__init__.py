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

""" Files for unit testing """

import os.path, shutil, tempfile
from . import w10_1

def mktmp (**kwargs) :
	return tempfile.NamedTemporaryFile(**kwargs)


class File (str) :

	dir = os.path.dirname(__file__)
	dir = os.path.abspath(dir) + '/'

	def __new__ (cls, mod) :
		name = mod.__name__.split('.')[-1]
		self = str.__new__(cls, File.dir + name + '.flac')
		self.basename = name
		return self

	def __init__ (self, mod) :
		self.metadata = mod.metadata
		self.metadata[-1].is_last = True

	def __getattr__ (self, name) :
		if name != 'data' :
			raise AttributeError("'File' object has no attribute " + repr(name))
		with open(File.dir + self.basename + '.raw', 'rb') as f :
			return f.read()

	def tmp (self, **kwargs) :
		tmp = mktmp(**kwargs)
		shutil.copyfile(self, tmp.name)
		return tmp

w10_1 = File(w10_1)

