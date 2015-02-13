python-flac
===========

This is a work in progress for a pure Python C wrapper for [libFLAC](https://xiph.org/flac/).


Native flac modules
-------------------

`flac._export` contains some compiletime information about the used FLAC Library.  
`flac.format` contains some common enums.  
`flac.metadata` contains the definitions of the metadata objects.  
`flac.metadata0` contains the level 0 metadata API.  
`flac.metadata1` contains the level 1 metadata API.  
`flac.metadata2` will contain the level 2 metadata API.

Python flac modules
-------------------

`flac` only provides all information found in `flac._export`.  
`flac.flac` provides every member of any other native module.

