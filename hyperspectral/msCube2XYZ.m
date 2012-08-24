function XYZ = msCube2XYZ(img, varargin)

wavelengths = 400:10:700;
XYZ = cube2XYZ(img, wavelengths, varargin{:});
