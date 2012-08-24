function writeNuanceCube(cube, name, varargin)

EXT = 'tif';
wavelengths = 420:10:720;
writeCube(cube, name, EXT, wavelengths, varargin{:});
