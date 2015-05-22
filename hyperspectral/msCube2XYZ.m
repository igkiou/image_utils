function XYZ = msCube2XYZ(cube, varargin)

wavelengths = getMsWavelengths;
XYZ = cube2XYZ(cube, wavelengths, varargin{:});
