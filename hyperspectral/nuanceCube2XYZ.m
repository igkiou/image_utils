function XYZ = nuanceCube2XYZ(img, varargin)

wavelengths = getNuanceWavelengths;
XYZ = cube2XYZ(img, wavelengths, varargin{:});
