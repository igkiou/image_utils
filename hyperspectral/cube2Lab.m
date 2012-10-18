function LAB = cube2Lab(cube, wavelengths, wtpoint, varargin)

if ((nargin < 3) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = cube2XYZ(cube, wavelengths, varargin{:});
LAB = XYZ2Lab(XYZ, wtpoint);
