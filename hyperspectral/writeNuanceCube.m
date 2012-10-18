function writeNuanceCube(cube, name, varargin)

EXT = 'tif';
wavelengths = getNuanceWavelengths;
writeCube(cube, name, EXT, wavelengths, varargin{:});
