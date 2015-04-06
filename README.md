python-flac
===========

This is a work in progress for a pure Python C wrapper for [libFLAC](https://xiph.org/flac/).


Native flac modules
-------------------

`flac._export` contains some compiletime information about the used FLAC Library.  
`flac.format` contains some common enums.  
`flac.decoder` contains the definitions of the stream decoder.  
`flac.encoder` contains the definitions of the stream encoder.  
`flac.metadata` contains the definitions of the metadata objects.  
`flac.metadata0` contains the level 0 metadata API.  
`flac.metadata1` contains the level 1 metadata API.  
`flac.metadata2` will contain the level 2 metadata API.

Python flac modules
-------------------

`flac` only provides all information found in `flac._export`.  
`flac.flac` provides every member of any other native and Python module.  
`flac.stream_decoder` provides every member of `flac.decoder` and some basic
	decoder implementations and utilities.  
`flac.stream_encoder` provides every member of `flac.encoder` and some basic
	encoder implementations and utilities.  
`flac.stream_metadata` provides every member of the metadata modules.  

