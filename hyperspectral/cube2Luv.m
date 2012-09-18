function LUV = cube2Luv(img, wavelengths, wtpoint, varargin)

if ((nargin < 3) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = cube2XYZ(img, wavelengths, varargin{:});
LUV = XYZ2Luv(XYZ, wtpoint);
