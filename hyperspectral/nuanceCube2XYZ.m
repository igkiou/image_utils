function XYZ = nuanceCube2XYZ(img, varargin)

wavelengths = 420:10:720;
XYZ = cube2XYZ(img, wavelengths, varargin{:});
