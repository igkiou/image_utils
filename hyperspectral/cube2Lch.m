function [LCH s] = cube2Lch(img, wavelengths, wtpoint, varargin)

if ((nargin < 3) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = cube2XYZ(img, wavelengths, varargin{:});
[LCH s] = XYZ2Lch(XYZ, wtpoint);
