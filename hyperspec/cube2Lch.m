function [LCH s] = cube2Lch(cube, wavelengths, wtpoint, varargin)

if ((nargin < 3) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = cube2XYZ(cube, wavelengths, varargin{:});
[LCH s] = XYZ2Lch(XYZ, wtpoint);
