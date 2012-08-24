function cube = getNuanceCube(name, varargin)

EXT = 'tif';
wavelengths = 420:10:720;
cube = getCube(name, EXT, wavelengths, varargin{:});
