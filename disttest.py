#	Copyright (c) 2014 Christian Schmitz <tynn.dev@gmail.com>
#
#	Permission is hereby granted, free of charge, to any person obtaining a copy
#	of this software and associated documentation files (the "Software"), to deal
#	in the Software without restriction, including without limitation the rights
#	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#	copies of the Software, and to permit persons to whom the Software is
#	furnished to do so, subject to the following conditions:
#
#	The above copyright notice and this permission notice shall be included in all
#	copies or substantial portions of the Software.
#
#	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#	SOFTWARE.

"""
	This is the disttest python module. It is a fully functional replacement for
	the distutils.core module. Only the setup function is extended to support a
	test_suite keyword argument and to expose a test command.

	Just replace all imports from distutils.core with the respective imports
	from disttest.

	Pull the gist to include the module:
		git remote add disttest https://gist.github.com/9779576.git
		git pull disttest master
"""

import imp, importlib, inspect, os, sys, unittest
from distutils.core import *


class test (Command) :

	description = "perform unit tests on extensions"
	user_options = [
		('test-suite=', 't', "test suite module to run"),
		('check', 'c', "run check command"),
		('skip-build', None, "skip the build steps"),
		('build-dir=', None, "build directory (implies --skip-build)"),
	]
	boolean_options = ['check', 'skip-build']

	def initialize_options (self) :
		self.build_dir = None
		self.check = None
		self.skip_build = None
		self.test_suite = None

	def finalize_options (self) :
		self.set_unittest_options()
		if not self.test_suite and hasattr(self.distribution, 'test_suite') :
			self.test_suite = getattr(self.distribution, 'test_suite')

		self.set_undefined_options('build', ('build_lib', 'build_dir'))
		self.set_undefined_options('install', ('skip_build', 'skip_build'))

	def set_unittest_options (self) :
		self.loader = unittest.defaultTestLoader
		self.suiteType = unittest.TestSuite

	def run (self) :
		if self.check :
			self.run_command('check')

		if not self.skip_build :
			self.run_command('build')

		if self.test_suite :
			self.run_tests()

	def run_tests (self) :
		sys.path.insert(0, self.build_dir)

		try : tests = self.suiteType(self.load_tests(self.test_suite))
		except ValueError :
			tests = self.suiteType()
			for _tests in self.test_suite :
				tests.addTest(self.load_tests(_tests))

		if not self.dry_run :
			unittest.TextTestRunner().run(tests)

	def load_tests (self, tests) :
		if isinstance(tests, (unittest.TestCase, unittest.TestSuite)) :
			return tests

		if isinstance(tests, str) :
			tests = self._load_module(tests)

		if inspect.ismodule(tests) :
			return self.loader.loadTestsFromModule(tests)

		raise ValueError('Not a test suite: {}'.format(tests))

	def _load_module (self, test) :
		try : return importlib.import_module(test)
		except :
			try : return importlib.import_module(test.replace(os.path.sep, '.'))
			except : pass

		test = os.path.splitext(test)
		try : return importlib.import_module(test[0])
		except :
			try : return importlib.import_module(test[0].replace(os.path.sep, '.'))
			except : pass

		if test[0].endswith(os.path.sep) :
			try : return importlib.import_module(test[0][:-1].replace(os.path.sep, '.'))
			except : pass

		test = os.path.abspath(os.path.dirname(test[0])), os.path.sep, os.path.basename(test[0]), test[1]
		if not test[2] :
			raise ImportError("Empty module name")

		if test[0] not in sys.path :
			sys.path.append(test[0])

		imp.acquire_lock()
		try : return imp.load_compiled('disttest._' + test[2], ''.join(test))
		except :
			try : return imp.load_source('disttest._' + test[2], ''.join(test))
			except :
				info = imp.find_module(test[2])
				try : imp.load_module('disttest._' + test[2], *info)
				finally : info[0].close()
		finally : imp.release_lock()

		raise ImportError("No module named {}".format(test[2]))


def extend_setup (_setup = setup, _Distribution = Distribution) :
	"""
		Extend the default or any other compatible distutils setup function.
		The Distribution class used with setup must be supplied, if setup
		does not distutils.dist.Distribution as default.
	"""

	def setup (**attrs) :
		"""Extended setup function"""

		if 'cmdclass' not in attrs :
			attrs['cmdclass'] = {'test': test}
		elif 'test' not in attrs['cmdclass'] :
			attrs['cmdclass']['test'] = test

		if 'distclass' not in attrs :
			attrs['distclass'] = type('Distribution', (_Distribution, object), dict(test_suite = None))
		elif not hasattr(attrs['distclass'], 'test_suite') :
			attrs['distclass'] = type('Distribution', (attrs['distclass'], object), dict(test_suite = None))

		_setup(**attrs)

	return setup


setup = extend_setup()
setup_keywords = setup_keywords + ('test_suite',)

