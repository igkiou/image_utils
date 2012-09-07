function [LUV c s h] = cube2Luv(img, wavelengths, varargin)

XYZ = cube2XYZ(img, wavelengths, varargin{:});
[LUV c s h] = XYZ2Luv(XYZ);
