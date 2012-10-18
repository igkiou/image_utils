function cube = getNuanceCube(name, varargin)

EXT = 'tif';
wavelengths = getNuanceWavelengths;
cube = getCube(name, EXT, wavelengths, varargin{:});
