function XYZ = nuanceCube2XYZ(cube, varargin)

wavelengths = getNuanceWavelengths;
XYZ = cube2XYZ(cube, wavelengths, varargin{:});
