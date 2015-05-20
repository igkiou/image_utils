function LUV = cube2Luv(cube, wavelengths, wtpoint, varargin)

if ((nargin < 3) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = cube2XYZ(cube, wavelengths, varargin{:});
LUV = XYZ2Luv(XYZ, wtpoint);
