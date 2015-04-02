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

import flac.metadata as flac

metadata = [
	flac.StreamMetadataStreamInfo(
		min_blocksize=4608,
		max_blocksize=4608,
		min_framesize=8284,
		max_framesize=14537,
		sample_rate=44100,
		channels=2,
		bits_per_sample=16,
		total_samples=44100,
		md5sum="bc20ef2282da48cc5c76463334e8279a"
	),
	flac.StreamMetadataVorbisComment(
		vendor_string="Lavf55.19.104",
		comments=[
			flac.StreamMetadataVorbisCommentEntry("DESCRIPTION", "audiotest wave"),
			flac.StreamMetadataVorbisCommentEntry("encoder", "Lavf55.19.104"),
		]
	),
	flac.StreamMetadataPadding(length=8192),
]

